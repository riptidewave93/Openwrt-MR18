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
  * Create Device Images
    1. Port kernel builder to C from bash
    * Figure out NAND layout for release
      * Figure out how to handle MAC information
    * Create image generation code for ./target/linux/ar71xx/image/Makefile
    * Enable/Setup Sysupgrade
  * Move LED Driver to device driver instead of kernel patch
  * Possibly cleanup Caldata generation code

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
This is CLOSE to being ready for an actual unofficial "release" so keep kicking butt! :)
