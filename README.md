# Openwrt-MR18
[![Build Status](http://198.199.68.234/api/badge/github.com/riptidewave93/Openwrt-MR18/status.svg?branch=master)](http://198.199.68.234/github.com/riptidewave93/Openwrt-MR18)

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=b8455a82b58c8ed16b0fc6742b00de6fc575f0e6

Building
-----
#### Build Only
`./build.sh`

#### Modify Configs and Build
`./build.sh modify`

Images by default will be moved to the root dir of the repo. Plan to build nandloader images for public drop.

To Do
-----
##### MR18
  * Enable/Setup Sysupgrade
  * Fixup Ethernet (Seems our PHY chip has its own mdio requirements, needs more research)
  * Create Image Build Scripts (So we can stop using the default elf binaries)
  * Determine/Fixup default LED settings
  * Move LED Driver to device driver instead of kernel patch

Working
-----
##### MR18
  * All LED's and Reset Button
  * WiFi
  * NAND
  * Sysfs Integration
  * Nandloader bootable images

Notice
------
This is AGES from being ready for an actual "release" so lets get to work! :)
