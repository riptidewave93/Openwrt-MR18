/*
 *  Cisco Meraki MR18 board support
 *
 *  Copyright (C) 2015 Chris Blake <chrisrblake93@gmail.com>
 *
 *  Based on Cisco Meraki GPL Release r23-20150601
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */
 #include <linux/platform_device.h>
 #include <linux/gpio.h>
 #include <linux/mtd/mtd.h>
 #include <linux/mtd/partitions.h>
 #include <linux/spi/flash.h>
 #include <linux/pci.h>
 #include <asm/mips_machine.h>
 #include <linux/ath9k_platform.h>
 #include <asm/mach-ath79/ath79_nand_regs.h>

 #include <linux/leds.h>
 #include <linux/leds-nu801.h>

 #include "common.h"
 #include "pci.h"
 #include "dev-ap9x-pci.h"
 #include "dev-gpio-buttons.h"
 #include "dev-eth.h"
 #include "dev-leds-gpio.h"
 #include "dev-m25p80.h"
 #include "dev-usb.h"
 #include "dev-wmac.h"
 #include "machtypes.h"

#define MR18_GPIO_LED_POWER_WHITE    18
#define MR18_GPIO_LED_POWER_ORANGE    21

#define MR18_GPIO_BTN_RESET    17
#define MR18_KEYS_POLL_INTERVAL    20  /* msecs */
#define MR18_KEYS_DEBOUNCE_INTERVAL  (3 * MR18_KEYS_POLL_INTERVAL)

#define MR18_WAN_PHYMASK    BIT(0)

#define MR18_MAC0_OFFSET    0
#define MR18_WMAC0_MAC_OFFSET    0x120c
#define MR18_WMAC1_MAC_OFFSET    0x520c
#define MR18_WMAC2_MAC_OFFSET    0x920c

#define MR18_CALDATA0_OFFSET    0x1000
#define MR18_CALDATA1_OFFSET    0x5000
#define MR18_CALDATA2_OFFSET    0x9000

static struct gpio_led MR18_leds_gpio[] __initdata = {
  {
    .name    = "mr18:power:white",
    .gpio    = MR18_GPIO_LED_POWER_WHITE,
    .active_low  = 1,
  }, {
    .name    = "mr18:power:orange",
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

static struct mtd_partition mr18_nand_flash_parts[] = {
  {
    .name  = "nandloader",
    .offset = 0x00000000,
    .size  = 0x00080000,   /* 512 KiB  */
  },
  {
    .name  = "part1",
    .offset  = 0x00080000,
    .size  = 0x00800000,   /* 8192 KiB */
  },
  {
    .name  = "part2",
    .offset  = 0x00880000,
    .size  = 0x00800000,   /* 8192 KiB */
  },
  {
    .name  = "ubi",
    .offset  = 0x01080000,
    .size  = 0x6F00000,  /* 113664 KiB */
  },
  {
    .name  = "odm-caldata",
    .offset  = 0x7FE0000,
    .size  = 0x20000,  /* 128 KiB */
  },
};

static struct flash_platform_data mr18_nand_flash_data = {
  .parts    = mr18_nand_flash_parts,
  .nr_parts  = ARRAY_SIZE(mr18_nand_flash_parts),
};

static struct platform_device mr18_nand = {
    .name = "ath79-nand",
    .id = -1,
    .dev.platform_data = &mr18_nand_flash_data,
};

static void __init MR18_setup(void)
{
  u8 *mac = (u8 *) KSEG1ADDR(0x1fff0000);

  ath79_register_m25p80(NULL);

  /* Debug the things */
  printk("Detected Meraki MR18\n");

  /* MDIO */
  ath79_register_mdio(0, 0x0);


  /* Std NIC? */
  ath79_init_mac(ath79_eth0_data.mac_addr, mac + MR18_MAC0_OFFSET, 0);
  ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RGMII;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_register_eth(0);

  /* NAND - NFC OpenWRT Driver
  ath79_nfc_set_ecc_mode(AR934X_NFC_ECC_SOFT);
  ath79_nfc_set_parts(mr18_nand_flash_parts,
                      ARRAY_SIZE(mr18_nand_flash_parts));
  ath79_register_nfc();
  */

  /* NAND - Meraki ath79 Driver */
  platform_device_register(&mr18_nand);

  /* Register the LED's and Buttons */
  platform_device_register(&tricolor_leds);
  ath79_register_leds_gpio(-1, ARRAY_SIZE(MR18_leds_gpio),
           MR18_leds_gpio);
  ath79_register_gpio_keys_polled(-1, MR18_KEYS_POLL_INTERVAL,
          ARRAY_SIZE(MR18_gpio_keys),
          MR18_gpio_keys);

  /* tricolor things
  if (tricolor_led_template.lei < 0) {
    if( gpio_request(4, "tricolor") == 0) {
      if( gpio_direction_output(4, 0) < 0) {
        	pr_err("%s: Unable to pull GPIO4 low\n", __func__);
        }
    }
  }
  */

  /* Load up PCI stuff
  ath79_register_pci();
  ath79_register_wmac(mac + MR18_WMAC0_MAC_OFFSET, NULL);
  */

}

MIPS_MACHINE(ATH79_MACH_MR18, "MR18", "Meraki MR18", MR18_setup);
