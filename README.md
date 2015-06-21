# Openwrt-MR18
Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=b14a040e0d99b81b75820e90aff0f943d9bde167

Building
-----
#### Build Only
`./build.sh`

#### Modify Configs and Build
`./build.sh modify`

Images by default will be moved to the root dir of the repo.

To Do
-----
##### MR18
  * Fixup WiFi caldata to work on NAND (Research how other OpenWRT devices do it)
  * Enable/Setup Sysupgrade
  * Fixup Ethernet (Seems our PHY chip has its own mido requirements, needs more research)
  * Create Image Build Scripts (So we can stop using the default elf binaries)
  * Determine/Fixup default LED settings
  * Move LED Driver to device driver instead of kernel patch
  * Fix/plan bootloader approach (U-Boot? Hard coded images? new/port driver to parse Cisco crap?)

Working
-----
##### MR18
  * All LED's and Reset Button
  * SPI Flash Storage
  * WiFi (When caldata is on SPI)
  * NAND
  * Sysfs Integration
  * Nandloader bootable images

Notice
------
This is AGES from being ready for an actual "release" so lets get to work! :)
