# Openwrt-MR18

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=28d26243956a0885fd206c85cb2ca194c1de7352

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
  * Create Image Build Scripts (So we can stop using the default elf binaries)
  * Enable/Setup Sysupgrade
  * Move LED Driver to device driver instead of kernel patch

Working
-----
##### MR18
  * All LED's and Reset Button
  * All WiFi radios
  * NAND Read/Write
  * Basic Sysfs Integration
  * Nandloader bootable images
  * Ethernet (Very dirty patch atm)
  * Caldata Generation using nanddump (needed for sysupgrade)

Notice
------
This is AGES from being ready for an actual "release" so lets get to work! :)
