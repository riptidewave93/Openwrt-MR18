#!/bin/bash

firstbuild=0
clonedir=./openwrt
cpu_num=$(grep -c processor /proc/cpuinfo)

# Print messages in cyan blue
Msg() {
  echo -e "\e[96m$1\e[39m"
}

# Do we want menuconfig's and an option to save configs?
if [ "$1" = "modify" ]; then
  modify=1
else
  modify=0
fi

Msg "Starting Build Process!"

if [ ! -d $clonedir ]; then
  firstbuild=1
  Msg "Cloning Repo..."
  git clone git://git.openwrt.org/openwrt.git $clonedir
  cd $clonedir
  git reset --hard 19cc7583f050ea411e30b0bf203a8801e5ee45ae
  cd - > /dev/null
fi


if [ $firstbuild = "0" ]; then
  Msg "Cleaning Builddir..."
  cd $clonedir
  rm -rf ./bin
  cd - > /dev/null
fi

Msg "Applying overlay..."
cp -R ./overlay/* $clonedir/

if [ $firstbuild = "1" ]; then
  Msg "Running first build configurations..."
  cd $clonedir
  ./scripts/feeds update -a
  ./scripts/feeds install -a
  make defconfig
  make prereq
  cd - > /dev/null
fi

if [ $modify -eq 1 ]; then
  cd $clonedir
  Msg "Loading OpenWRT Menuconfig"
  make menuconfig -j$cpu_num V=s
  Msg "Loading Kernel Menuconfig"
  make kernel_menuconfig -j$cpu_num V=s
  cd - > /dev/null
fi

Msg "Building Time!!!"
cd $clonedir
make -j$cpu_num V=s

if [ $? -ne 0 ]; then
  cd - > /dev/null
  Msg "Build Failed! Exiting..."
  exit 1
else
  cd - > /dev/null
  Msg "Compile Complete!"
fi

Msg "Build.sh Finished!"
