#!/bin/sh
#
# Copyright (C) 2015 Chris Blake <chrisrblake93@gmail.com>
#
# Custom upgrade script for Meraki NAND devices (MR18) to support
# caldata & kernel_backup. Based on dir825.sh and stock nand functions
#

. /lib/ar71xx.sh
. /lib/functions.sh
. /lib/upgrade/nand.sh

get_magic_at() {
	local mtddev=$1
	local pos=$2
	dd bs=1 count=2 skip=$pos if=$mtddev 2>/dev/null | hexdump -v -n 4 -e '1/1 "%02x"'
}

merakinand_is_caldata_valid() {
	local mtddev=$1
	local magic

	magic=$(get_magic_at $mtddev 4096)
	[ "$magic" != "a55a" ] && return 0

	magic=$(get_magic_at $mtddev 20480)
	[ "$magic" != "a55a" ] && return 0

	return 1
}

merakinand_copy_caldata() {
	local cal_src=$1
	local cal_dst=$2
	local mtd_src
	local mtd_dst
	local md5_src
	local md5_dst

	mtd_src=$(find_mtd_part $cal_src)
	[ -z "$mtd_src" ] && {
		echo "no $cal_src partition found"
		return 1
	}

	mtd_dst=$(find_mtd_part $cal_dst)
	[ -z "$mtd_dst" ] && {
		echo "no $cal_dst partition found"
		return 1
	}

	merakinand_is_caldata_valid "$mtd_src" && {
		echo "no valid calibration data found in $cal_src"
		return 1
	}

	merakinand_is_caldata_valid "$mtd_dst" && {
		echo "Copying calibration data from $cal_src to $cal_dst..."
		dd if="$mtd_src" 2>/dev/null | mtd -q -q write - "$cal_dst"
	}

        md5_src=$(md5sum "$mtd_src") && md5_src="${md5_src%% *}"
        md5_dst=$(md5sum "$mtd_dst") && md5_dst="${md5_dst%% *}"

	[ "$md5_src" != "$md5_dst" ] && {
		echo "calibration data mismatch $cal_src:$md5_src $cal_dst:$md5_dst"
		return 1
	}

	return 0
}

merakinand_do_platform_check() {
	local board_name="$1"
	local tar_file="$2"
	local control_length=`(tar xf $tar_file sysupgrade-$board_name/CONTROL -O | wc -c) 2> /dev/null`
	local file_type="$(identify $2)"

	# Got to start somewhere...
	echo "DEBUG: board_name = $board_name";
	echo "DEBUG: tar_file = $tar_file";
	echo "DEBUG: control_length = $control_length";
	echo "DEBUG: file_type = $file_type";

	# What device are we?
	case "$board_name" in
	"mr18")
		# We need a TAR that is using a SquashFS root
		[ "$control_length" = 0 -a "$file_type" != "squashfs" ] && {
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
	echo -n $1 > /tmp/sysupgrade-nand-path
	cp /sbin/upgraded /tmp/
	exit 1
	#merakinand_upgrade_stage1
}
