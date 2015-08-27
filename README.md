# Openwrt-MR18

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=b2449f768a1203da89c9729b14e8e9f9c4f46a99

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
  * Code Cleanup?
  * Start merging things into official nightlies!

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
No promises this won't brick your AP, and no promises that this will even work!
