# Openwrt-MR18

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=21067c3680641ba52f00d11206f1eff7157ccd53

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
  1. Create Device Images
    1. Port kernel builder to C from bash
    * Create image generation code for ./target/linux/ar71xx/image/Makefile
    * Enable/Setup Sysupgrade
  * Fixup WLAN2 MAC
  * Move LED Driver to device driver instead of kernel patch
  * Possibly cleanup caldata generation code

Working
-----
##### MR18
  * All LED's and Reset Button
  * All WiFi radios
  * NAND Read/Write
  * Basic Sysfs Integration
  * Nandloader bootable images
  * Ethernet
  * Caldata Generation using nanddump

Notice
------
This is CLOSE to being ready for an actual unofficial "release" so keep kicking butt! :)
