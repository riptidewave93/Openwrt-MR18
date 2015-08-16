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
  * Port kernel builder to C from bash
  * Fixup sysupgrade/initramfs device file generation
  * Enable/Setup Sysupgrade w/ custom upgrade script
  * Fix MAC on WLAN2
  * Possibly cleanup Caldata generation code (can be done within sysupgrade code, like other boards)

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
