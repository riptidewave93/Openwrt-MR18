# Openwrt-MR18

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=c2a0c9d4a685d7b498e59a11d06a01fe25835351

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
  * Fix Sysupgrade bug with config restore
  * Fix MAC on WLAN2

Working
-----
##### MR18
  * All LED's and Reset Button
  * All WiFi radios
  * NAND Read/Write
  * Basic Sysfs Integration
  * Nandloader bootable images
  * Ethernet
  * Sysconfig w/ kernel_backup support
  * Automatic Wifi Caldata partition selection (Let's prevent ecc errors if possible!)

Notice
------
This is CLOSE to being ready for an actual unofficial "release" so keep kicking butt! :)
