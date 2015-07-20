/*
 *  Cisco Meraki MR18 board support
 *
 *  Copyright (C) 2015 Chris Blake <chrisrblake93@gmail.com>
 *  Copyright (C) 2015 Christian Lamparter <chunkeey@googlemail.com>
 *  Copyright (C) 2015 Thomas Hebb <tommyhebb@gmail.com>
 *
 *  Based on Cisco Meraki GPL Release r23-20150601 MR18 Device Config
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/platform/ar934x_nfc.h>
#include <linux/platform_data/phy-at803x.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include <linux/leds-nu801.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-nfc.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define MR18_GPIO_LED_POWER_WHITE    18
#define MR18_GPIO_LED_POWER_ORANGE    21

#define MR18_GPIO_BTN_RESET    17
#define MR18_KEYS_POLL_INTERVAL    20  /* msecs */
#define MR18_KEYS_DEBOUNCE_INTERVAL  (3 * MR18_KEYS_POLL_INTERVAL)

#define MR18_WAN_PHYADDR    3

static struct gpio_led MR18_leds_gpio[] __initdata = {
  {
    .name    = "mr18:white:power",
    .gpio    = MR18_GPIO_LED_POWER_WHITE,
    .active_low  = 1,
  }, {
    .name    = "mr18:orange:power",
    .gpio    = MR18_GPIO_LED_POWER_ORANGE,
    .active_low  = 0,
  },
};

static struct gpio_keys_button MR18_gpio_keys[] __initdata = {
  {
    .desc    = "reset",
    .type    = EV_KEY,
    .code    = KEY_RESTART,
    .debounce_interval = MR18_KEYS_DEBOUNCE_INTERVAL,
    .gpio    = MR18_GPIO_BTN_RESET,
    .active_low  = 1,
  }
};

static struct led_nu801_template tricolor_led_template = {
    .device_name = "mr18",
    .name = "tricolor",
    .num_leds = 1,
    .cki = 11,
    .sdi = 12,
    .lei = -1,
    .ndelay = 500,
    .init_brightness = {
        LED_OFF,
        LED_OFF,
        LED_OFF,
    },
    .default_trigger = "none",
    .led_colors = { "red", "green", "blue" },
};

static struct led_nu801_platform_data tricolor_led_data = {
    .num_controllers = 1,
    .template = &tricolor_led_template,
};

static struct platform_device tricolor_leds = {
    .name = "leds-nu801",
    .id = -1,
    .dev.platform_data = &tricolor_led_data,
};

/*
 * Copied from U-Boot Atheros LSDK-9.5.5.36
 */

#define OTP_INTF2_ADDRESS                                            0x18131008
#define OTP_LDO_CONTROL_ADDRESS                                      0x18131024
#define OTP_LDO_STATUS_ADDRESS                                       0x1813102c

#define OTP_LDO_STATUS_POWER_ON_MSB                                  0
#define OTP_LDO_STATUS_POWER_ON_LSB                                  0
#define OTP_LDO_STATUS_POWER_ON_MASK                                 0x00000001
#define OTP_LDO_STATUS_POWER_ON_RESET                                0x0 // 0

#define OTP_MEM_0_ADDRESS                                            0x18130000
#define OTP_STATUS0_EFUSE_READ_DATA_VALID_MASK                       0x00000004

#define OTP_STATUS0_ADDRESS                                          0x18131018
#define OTP_STATUS1_ADDRESS                                          0x1813101c

#define ath_reg_rd(_phys)       (*(volatile uint32_t *)KSEG1ADDR(_phys))

#define ath_reg_wr_nf(_phys, _val) \
        ((*(volatile uint32_t *)KSEG1ADDR(_phys)) = (_val))

#define ath_reg_wr(_phys, _val) do {    \
        ath_reg_wr_nf(_phys, _val);     \
        ath_reg_rd(_phys);              \
} while(0)

#define ath_reg_rmw_set(_reg, _mask)    do {                    \
        ath_reg_wr((_reg), (ath_reg_rd((_reg)) | (_mask)));     \
        ath_reg_rd((_reg));                                     \
} while(0)

#define ath_reg_rmw_clear(_reg, _mask)    do {                    \
        ath_reg_wr((_reg), (ath_reg_rd((_reg)) & ~(_mask)));     \
        ath_reg_rd((_reg));                                     \
} while(0)

static void qca955x_gmac_sgmii_res_cal(void)
{
   unsigned int read_data;
   unsigned int reversed_sgmii_value;

#ifdef CALC_SGMII_CAL_VALUE
   unsigned int read_data_otp, otp_value, otp_per_val, rbias_per;
   unsigned int read_data_spi;
   unsigned int rbias_pos_or_neg, res_cal_val;
   unsigned int sgmii_pos, sgmii_res_cal_value;
   unsigned int use_value;
   unsigned int *address_spi = (unsigned int *)0xbffffffc;
   ath_reg_wr(OTP_INTF2_ADDRESS, 0x7d);
   ath_reg_wr(OTP_LDO_CONTROL_ADDRESS, 0x0);

   while (ath_reg_rd(OTP_LDO_STATUS_ADDRESS) & OTP_LDO_STATUS_POWER_ON_MASK);

   read_data = ath_reg_rd(OTP_MEM_0_ADDRESS  4);

   while (!(ath_reg_rd(OTP_STATUS0_ADDRESS) & OTP_STATUS0_EFUSE_READ_DATA_VALID_MASK));

   read_data_otp = ath_reg_rd(OTP_STATUS1_ADDRESS);

   if (read_data_otp & 0x1fff) {
      read_data = read_data_otp;
   } else {
      read_data_spi = *(address_spi);
      if ((read_data_spi & 0xffff0000) == 0x5ca10000) {
         read_data = read_data_spi;
      } else {
         read_data = read_data_otp;
      }
   }

   if (read_data & 0x00001000) {
      otp_value = (read_data & 0xfc0) >> 6;
   } else {
      otp_value = read_data & 0x3f;
   }

   if (otp_value > 31) {
      otp_per_val = 63 - otp_value;
      rbias_pos_or_neg = 1;
   } else {
      otp_per_val = otp_value;
      rbias_pos_or_neg = 0;
   }

   rbias_per = otp_per_val * 15;

   if (rbias_pos_or_neg == 1) {
      res_cal_val = (rbias_per  34) / 21;
      sgmii_pos = 1;
   } else {
      if (rbias_per > 34) {
         res_cal_val = (rbias_per - 34) / 21;
         sgmii_pos = 0;
      } else {
         res_cal_val = (34 - rbias_per) / 21;
         sgmii_pos = 1;
      }
   }

  if (sgmii_pos == 1) {
      sgmii_res_cal_value = 8  res_cal_val;
   } else {
      sgmii_res_cal_value = 8 - res_cal_val;
   }

   reversed_sgmii_value = 0;
   use_value = 0x8;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) >> 3);
   use_value = 0x4;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) >> 1);
   use_value = 0x2;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) << 1);
   use_value = 0x1;
   reversed_sgmii_value = reversed_sgmii_value | ((sgmii_res_cal_value & use_value) << 3);

   reversed_sgmii_value &= 0xf;
#else
   reversed_sgmii_value = 0xe;
#endif
   printk(KERN_INFO "SGMII cal value = 0x%x\n", reversed_sgmii_value);

   // To Check the locking of the SGMII PLL

#define SGMII_SERDES_ADDRESS                                         0x18070018
#define SGMII_SERDES_RES_CALIBRATION_LSB                             23
#define SGMII_SERDES_RES_CALIBRATION_MASK                            0x07800000
#define SGMII_SERDES_RES_CALIBRATION_GET(x)                          (((x) & SGMII_SERDES_RES_CALIBRATION_MASK) >> SGMII_SERDES_RES_CALIBRATION_LSB)
#define SGMII_SERDES_RES_CALIBRATION_SET(x)                          (((x) << SGMII_SERDES_RES_CALIBRATION_LSB) & SGMII_SERDES_RES_CALIBRATION_MASK)

   read_data = (ath_reg_rd(SGMII_SERDES_ADDRESS) &
            ~SGMII_SERDES_RES_CALIBRATION_MASK) |
         SGMII_SERDES_RES_CALIBRATION_SET(reversed_sgmii_value);

   ath_reg_wr(SGMII_SERDES_ADDRESS, read_data);

#define ETH_SGMII_SERDES_ADDRESS                                     0x1805004c
#define ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK                         0x00000004
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK                         0x00000002
#define ETH_SGMII_SERDES_EN_PLL_MASK                                 0x00000001

   ath_reg_wr(ETH_SGMII_SERDES_ADDRESS,
         ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK |
         ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK |
         ETH_SGMII_SERDES_EN_PLL_MASK);

#define SGMII_SERDES_ADDRESS                                         0x18070018
#define SGMII_SERDES_CDR_BW_LSB                                      1
#define SGMII_SERDES_CDR_BW_MASK                                     0x00000006
#define SGMII_SERDES_CDR_BW_SET(x)                                   (((x) << SGMII_SERDES_CDR_BW_LSB) & SGMII_SERDES_CDR_BW_MASK)
#define SGMII_SERDES_TX_DR_CTRL_LSB                                  4
#define SGMII_SERDES_TX_DR_CTRL_MASK                                 0x00000070
#define SGMII_SERDES_TX_DR_CTRL_SET(x)                               (((x) << SGMII_SERDES_TX_DR_CTRL_LSB) & SGMII_SERDES_TX_DR_CTRL_MASK)
#define SGMII_SERDES_PLL_BW_MASK                                     0x00000100
#define SGMII_SERDES_EN_SIGNAL_DETECT_MASK                           0x00010000
#define SGMII_SERDES_FIBER_SDO_MASK                                  0x00020000
#define SGMII_SERDES_VCO_REG_LSB                                     27
#define SGMII_SERDES_VCO_REG_MASK                                    0x78000000
#define SGMII_SERDES_VCO_REG_SET(x)                                  (((x) << SGMII_SERDES_VCO_REG_LSB) & SGMII_SERDES_VCO_REG_MASK)

   ath_reg_rmw_set(SGMII_SERDES_ADDRESS,
         SGMII_SERDES_CDR_BW_SET(3) |
         SGMII_SERDES_TX_DR_CTRL_SET(1) |
         SGMII_SERDES_PLL_BW_MASK |
         SGMII_SERDES_EN_SIGNAL_DETECT_MASK |
         SGMII_SERDES_FIBER_SDO_MASK |
         SGMII_SERDES_VCO_REG_SET(3));

  ath79_device_reset_clear(QCA953X_RESET_ETH_SWITCH_ANALOG);
  ath79_device_reset_clear(QCA953X_RESET_ETH_SWITCH);

#define SGMII_SERDES_LOCK_DETECT_STATUS_MASK                         0x00008000
   while (!(ath_reg_rd(SGMII_SERDES_ADDRESS) & SGMII_SERDES_LOCK_DETECT_STATUS_MASK));
}

static void mdio_init(void)
{
       uint32_t rddata;

#define GPIO_IN_ENABLE3_ADDRESS                                      0x18040050

#define GPIO_IN_ENABLE3_MII_GE1_MDI_MASK                             0x00ff0000
#define GPIO_IN_ENABLE3_MII_GE1_MDI_LSB                              16
#define GPIO_IN_ENABLE3_MII_GE1_MDI_SET(x)                           (((x) << GPIO_IN_ENABLE3_MII_GE1_MDI_LSB) & GPIO_IN_ENABLE3_MII_GE1_MDI_MASK)

        rddata = ath_reg_rd(GPIO_IN_ENABLE3_ADDRESS)&
                ~GPIO_IN_ENABLE3_MII_GE1_MDI_MASK;
        rddata |= GPIO_IN_ENABLE3_MII_GE1_MDI_SET(19);
        ath_reg_wr(GPIO_IN_ENABLE3_ADDRESS, rddata);

#define GPIO_OE_ADDRESS                                              0x18040000
        ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << 19)); // GPIO 19 output
        ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << 17)); // GPIO 17 output

#define GPIO_OUT_FUNCTION4_ADDRESS                                   0x1804003c
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK                       0xff000000
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK                       0x0000ff00

        rddata = ath_reg_rd(GPIO_OUT_FUNCTION4_ADDRESS) &
                ~ (GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK |
                GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK);

#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_LSB                        24
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_SET(x)                     (((x) << GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_LSB) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_LSB                        8
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_SET(x)                     (((x) << GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_LSB) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK)

        rddata |= GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_SET(0x20) |
                  GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_SET(0x21);

        ath_reg_wr(GPIO_OUT_FUNCTION4_ADDRESS, rddata);

}

void mr18_special(void)
{
	#define SWITCH_CLOCK_SPARE_ADDRESS                                   0x18050020
	ath_reg_wr(SWITCH_CLOCK_SPARE_ADDRESS, 0x520);	// USB and I2C?

	mdio_init();
	qca955x_gmac_sgmii_res_cal();
}

static void __init mr18_setup(void)
{
  /* SPI - slot not populated, but functional when added! */
  ath79_register_m25p80(NULL);

  /* NAND */
  ath79_nfc_set_ecc_mode(AR934X_NFC_ECC_SOFT_BCH);
  ath79_register_nfc();

  /* Setup SoC Eth Config */
  ath79_setup_qca955x_eth_cfg(QCA955X_ETH_CFG_RGMII_EN);
  ath79_setup_qca955x_eth_rx_delay(3,3);

  /* MDIO Interface */
  ath79_register_mdio(0, 0x0);

  /* GMAC0 is connected to an Atheros AR8035-A */
  ath79_init_mac(ath79_eth0_data.mac_addr, NULL, 0);
  ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
  ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RGMII;
  ath79_eth0_data.phy_mask = 0x1f;
  ath79_eth0_pll_data.pll_1000 = 0xa6000000;
  ath79_eth0_pll_data.pll_100 = 0xa0000101;
  ath79_eth0_pll_data.pll_10 = 0x80001313;
  ath79_register_eth(0);

  /* LEDs and Buttons */
  platform_device_register(&tricolor_leds);
  ath79_register_leds_gpio(-1, ARRAY_SIZE(MR18_leds_gpio),
                          MR18_leds_gpio);
  ath79_register_gpio_keys_polled(-1, MR18_KEYS_POLL_INTERVAL,
                                  ARRAY_SIZE(MR18_gpio_keys),
                                  MR18_gpio_keys);

  /* Clear RTC reset (Needed by SoC WiFi) */
  ath79_device_reset_clear(QCA955X_RESET_RTC);

  /* WiFi */
  ath79_register_wmac_simple();
  ap91_pci_init_simple();
}
MIPS_MACHINE(ATH79_MACH_MR18, "MR18", "Meraki MR18", mr18_setup);
