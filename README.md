# Openwrt-MR18
Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=b14a040e0d99b81b75820e90aff0f943d9bde167

Building
-----
#### Build Only
`./build.sh`

#### Modify Configs and Build
`./build.sh modify`

Images by default will be in ./openwrt/bin/ar71xx/

Priority List
-----
  1. Get NAND Working (all partitions)
  2. Get JTAG on NAND working (To test U-Boot & flashing)
  3. Bringup device ethernet Interface
  4. Bringup W-Fi interfaces
  5. Figure out plan for the bootloader (U-Boot?)
  6. Work on system integration (sysupgrade, autodetect, etc.)
  7. Go from there....

To Do
-----
##### MR18
  * Fix NAND ECC errors (try using/fixing OpenWRT Driver if possible)
  * Bringup Ethernet Interface
  * Fix Wi-Fi Cal offset address
  * Enable SysFS HW Detection (ATM mach is hardcoded into kernel cmdline)
  * Fix bootloader problem (U-Boot? Hard coded images?)

Working
-----
##### MR18
  * All LED's and Reset Button!

Notice
------
This is AGES from being ready for an actual "release" so lets get to work! :)
