#!/bin/env bash
rm -f Atlas.iso
dd if=/dev/zero of=Atlas.iso count=70000
mkfs.vfat -F 32 Atlas.iso
dd if=boot of=Atlas.iso conv=notrunc
dd if=bpb.bin of=Atlas.iso conv=notrunc bs=1 count=90
sudo mount Atlas.iso COMMON
sudo cp boot2 COMMON/
sudo mkdir COMMON/boot
echo Dark Moon v0.0.2 > test.txt
sudo cp test.txt COMMON/boot/
sudo umount COMMON
