# Openwrt-MR18

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

**NOTE: This repo is NO LONGER MAINTAINED as these changes were applied upstream. Refer to http://git.openwrt.org/?p=openwrt.git;a=commit;h=08649070b3de6da8913a5e93d853834ad7293934 and enjoy the official nightles!**

Building
-----
#### Build Only
`./build.sh`

#### Modify Configs and Build
`./build.sh modify`

Note that you will need to run a modify on the first compile to select the NAND image in the OpenWRT menuconfig.

To Do
-----
##### MR18
  * You tell me!

Working
-----
##### MR18
  * All LED's and Reset Button
  * All WiFi radios
  * NAND Read/Write
  * Basic Sysfs Integration
  * Nandloader bootable images
  * Ethernet
  * Sysconfig/Sysupgrade
  * Automatic Wifi Caldata partition selection (Let's prevent ecc errors if possible!)
  * Merged upstream!

Notice
------
No promises this won't brick your AP, and no promises that this will even work!
