# Openwrt-MR18

Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=dfa904f762846dc0e9d46119490d2466c4b95b60

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
