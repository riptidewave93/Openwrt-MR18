#!/bin/sh
#
# Copyright (C) 2015 Chris Blake <chrisrblake93@gmail.com>
#
# Custom upgrade script for Meraki NAND devices (ex. MR18)
# Based on dir825.sh and stock nand functions
#
. /lib/ar71xx.sh
. /lib/functions.sh
. /lib/upgrade/nand.sh

get_magic_at() {
	local mtddev=$1
	local pos=$2
	dd bs=1 count=2 skip=$pos if=$mtddev 2>/dev/null | hexdump -v -n 4 -e '1/1 "%02x"'
}

merakinand_do_kernel_check() {
	local board_name="$1"
	local tar_file="$2"
	local image_magic_word=`(tar xf $tar_file sysupgrade-$board_name/kernel -O | dd bs=1 count=4 skip=0 >/dev/null | hexdump -v -n 4 -e '1/1 "%02x"')`

	# What is our magic string we need to look for?
	case "$board_name" in
	"mr18")
		[ "$image_magic_word" != "8e73ed8a" ] && {
			return 1
		}
		;;
	*)
		return 1
		;;
	esac

	return 0
}

merakinand_do_platform_check() {
	local board_name="$1"
	local tar_file="$2"
	local control_length=`(tar xf $tar_file sysupgrade-$board_name/CONTROL -O | wc -c) 2> /dev/null`
	local file_type="$(identify $2)"
	local kernel_magic="$(merakinand_do_kernel_check $1 $2)"

	# Got to start somewhere...
	echo "DEBUG: board_name = $board_name";
	echo "DEBUG: tar_file = $tar_file";
	echo "DEBUG: control_length = $control_length";
	echo "DEBUG: file_type = $file_type";
	echo "DEBUG: kernel_magic = $kernel_magic"

	# What device are we?
	case "$board_name" in
	"mr18")
		# We need a TAR that is using a SquashFS root
		[ "$control_length" = 0 -a "$file_type" != "squashfs" -a "$kernel_magic" != 0 ] && {
			echo "Invalid sysupgrade file for $board_name"
			return 1
		}
		;;
	*)
		echo "Unsupported device $board_name";
		return 1
		;;
	esac

	echo -n $2 > /tmp/sysupgrade-nand-path
	cp /sbin/upgraded /tmp/

	return 0
}

merakinand_do_upgrade() {
	local tar_file="$1"
	local board_name="$(cat /tmp/sysinfo/board_name)"
	local kernel_mtd="$(find_mtd_index $CI_KERNPART)"
	local kernel_backup_mtd="$(find_mtd_index kernel_backup)"
	local kernel_length=`(tar xf $tar_file sysupgrade-$board_name/kernel -O | wc -c) 2> /dev/null`
	local rootfs_length=`(tar xf $tar_file sysupgrade-$board_name/root -O | wc -c) 2> /dev/null`
	local root_ubivol="$( nand_find_volume $ubidev rootfs )"
	local data_ubivol="$( nand_find_volume $ubidev rootfs_data )"

	local ubidev="$( nand_find_ubi "$CI_UBIPART" )"
	if [ ! "$ubidev" ]; then
		ubiattach -m "$mtdnum"
		sync
		ubidev="$( nand_find_ubi "$CI_UBIPART" )"
	fi

	# flash kernels
	if [ "$kernel_length" != 0 -a -n "$kernel_mtd" -a -n "$kernel_backup_mtd" ]; then
		echo "DEBUG: Flashing kernel from $tar_file to MTD $CI_KERNPART and kernel_backup"
		#tar xf $tar_file sysupgrade-$board_name/kernel -O | mtd write - $CI_KERNPART
		#tar xf $tar_file sysupgrade-$board_name/kernel -O | mtd write - kernel_backup
	else
		echo "DEBUG: Check Failed! kernel_length = $kernel_length, kernel_mtd = $kernel_mtd, kernel_backup_mtd = $kernel_backup_mtd"
		return 1
	fi

	# kill ubi partitions
	echo "DEBUG: Killing rootfs and rootfs_data on /dev/$ubidev"
	#[ "$root_ubivol" ] && ubirmvol /dev/$ubidev -N rootfs || true
	#[ "$data_ubivol" ] && ubirmvol /dev/$ubidev -N rootfs_data || true

	# create rootfs
	echo "DEBUG: Creating rootfs on /dev/$ubidev with a size of $rootfs_length"
	#if ! ubimkvol /dev/$ubidev -N rootfs -s $rootfs_length; then
	#	echo "cannot create rootfs volume"
	#	return 1;
	#fi

	# create rootfs_data
	echo "DEBUG: Creating rootfs_data on /dev/$ubidev with max size"
	#if ! ubimkvol /dev/$ubidev -N rootfs_data -m; then
	#	echo "cannot initialize rootfs_data volume"
	#	return 1
	#fi
	sync

	# We just rebuilt these, lets find them again!
	root_ubivol="$( nand_find_volume $ubidev rootfs )"
	data_ubivol="$( nand_find_volume $ubidev rootfs_data )"

	# flash rootfs
	echo "DEBUG: Flashing rootfs to /dev/$root_ubivol"
	#tar xf $tar_file sysupgrade-$board_name/root -O | \
	#	ubiupdatevol /dev/$root_ubivol -s $rootfs_length -

	echo "DEBUG: Run nand_do_upgrade_success goes here!"
	#nand_do_upgrade_success
}
