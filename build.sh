#!/bin/bash

firstbuild=0
clonedir=./openwrt
partbuilderdir=./meraki-partbuilder
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
  git reset --hard b14a040e0d99b81b75820e90aff0f943d9bde167
  cd - > /dev/null
fi

if [ ! -d $partbuilderdir ]; then
  Msg "Cloning Nandloader Image Builder..."
  git clone https://github.com/riptidewave93/meraki-partbuilder.git $partbuilderdir
  chmod +x $partbuilderdir/partbuilder.sh
fi

if [ $firstbuild = "0" ]; then
  Msg "Cleaning Builddir..."
  cd $clonedir
  rm -rf ./bin
  # make target/linux/clean
  rm -rf ./build_dir/target-mips_34kc_uClibc-0.9.33.2/linux-ar71xx*
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

Msg "Applying configurations..."
cp ./configs/openwrt-config $clonedir/.config
cp ./configs/kernel-config-3.18 $clonedir/target/linux/ar71xx/config-3.18
cp ./configs/kernel-config-3.18 $clonedir/target/linux/ar71xx/nand/config-default

if [ $modify -eq 1 ]; then
  cd $clonedir
  Msg "Loading OpenWRT Menuconfig"
  make menuconfig
  Msg "Loading Kernel Menuconfig"
  make kernel_menuconfig
  cd - > /dev/null
  Msg "Copy kernel config to nand-default? (You normally want this....)"
  Msg "y/N?"
  read input
  if [[ $input ==  *"y"* ]]; then
    Msg "Copying Config..."
    cp $clonedir/build_dir/target-mips_34kc_uClibc-0.9.33.2/linux-ar71xx_nand/linux-3.18.14/.config $clonedir/target/linux/ar71xx/config-3.18
    cp $clonedir/target/linux/ar71xx/config-3.18 $clonedir/target/linux/ar71xx/nand/config-default
  fi
fi

Msg "Building Time!!!"
cd $clonedir
make -j$cpu_num V=s
cd - > /dev/null
Msg "Compile Complete!"

if [ $modify -eq 1 ]; then
  Msg "Would you like to save your new configurations to the configs folder?"
  Msg "y/N?"
  read input
  if [[ $input ==  *"y"* ]]; then
    Msg "Saving configs..."
    cp $clonedir/.config ./configs/openwrt-config
    cp $clonedir/target/linux/ar71xx/config-3.18 ./configs/kernel-config-3.18
  fi
fi

if [ -d $clonedir/bin/ar71xx/ ]; then
  Msg "Generating Nandloader Image"
  # Change to device image once we get the platform in makefile setup
  $partbuilderdir/partbuilder.sh $clonedir/bin/ar71xx/openwrt-ar71xx-nand-vmlinux-initramfs.bin ./openwrt-ar71xx-nand-vmlinux-initramfs-nandloader-part1.bin

  Msg "Moving Images to Local Dir..."
  cp $clonedir/bin/ar71xx/openwrt-ar71xx-nand-vmlinux-initramfs.elf ./
fi

Msg "Build.sh Finished!"
