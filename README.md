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
  1. Cleanup NIC Init (Make a proper patch)
  * Port kernel builder to C from bash
  * Fixup sysupgrade/initramfs device file generation
  * Enable/Setup Sysupgrade w/ custom upgrade script
  * Fix MAC on WLAN2
  * Possibly cleanup Caldata generation code

Working
-----
##### MR18
  * All LED's and Reset Button
  * All WiFi radios
  * NAND Read/Write
  * Basic Sysfs Integration
  * Nandloader bootable images
  * Ethernet (dirty patch atm)
  * Caldata Generation using nanddump (needed for sysupgrade)

Notice
------
This is CLOSE to being ready for an actual unofficial "release" so keep kicking butt! :)
