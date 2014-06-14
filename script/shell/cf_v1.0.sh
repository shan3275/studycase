#!/bin/sh
sfdisk --force /dev/sdb < /home/shan/cf.partion
sfdisk -d /dev/sdb
mkfs.vfat -F 16 /dev/sdb1
mkfs.ext2 /dev/sdb2
mkfs.ext2 /dev/sdb3
mkfs.ext2 /dev/sdb5
mkfs.ext2 /dev/sdb6
mkfs.ext2 /dev/sdb7
mkfs.ext2 /dev/sdb8
mkfs.ext2 /dev/sdb9
mkfs.ext2 /dev/sdb10
fsck /dev/sdb1
fsck /dev/sdb2
fsck /dev/sdb3
fsck /dev/sdb5
fsck /dev/sdb6
fsck /dev/sdb7
fsck /dev/sdb8
fsck /dev/sdb9
fsck /dev/sdb10
mount /dev/sdb1 /mnt
cp /home/shan/FlowOS_V1.2.2662M.elf /mnt/FlowOS.elf
cp /home/shan/SempIPE_40P.4_HW_TEST_003.spkg /mnt/SempIPE.spkg
ls -lh /mnt
umount /mnt
exit 0
