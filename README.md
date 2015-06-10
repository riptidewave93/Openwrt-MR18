# Openwrt-MR18
Bringup repo for the Cisco Meraki MR18 on OpenWRT!

Code is based on revision http://git.openwrt.org/?p=openwrt.git;a=commit;h=b14a040e0d99b81b75820e90aff0f943d9bde167

Building
-----
#### Build Only
`./build.sh`

#### Modify Configs and Build
`./build.sh modify`

Images by default will be in ./openwrt/bin/ar71xx/

To Do
-----
##### MR18
  * Fix NAND ECC errors on non caldata partitions! (Can we base ECC on name?)
  * FIX Gigibit Ethernet NIC (Up but no data, PLL/tuneables wrong/missing)
  * Fix Wi-Fi Cal offset address's (This may be a bitch with so many interfaces)
  * Enable SysFS HW Detection (ATM mach is hardcoded into kernel cmdline)
  * Enable Base System configuration (LAN/LED System Configs)
  * Enable Sysupgrade
  * Fix bootloader issue (U-Boot? Hard coded images? new/port driver to parse Cisco crap?)

Working
-----
##### MR18
  * All LED's and Reset Button!
  * NIC Shows up and detects cable


Notice
------
This is AGES from being ready for an actual "release" so lets get to work! :)
