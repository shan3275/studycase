#!/bin/sh
sfdisk --force /dev/sdb < /home/shan/cf.partion
sfdisk -d /dev/sdb
ehco -e "格式化CF卡"
echo -e "格式化 /dev/sdb1"
mkfs.vfat -F 16 /dev/sdb1
for i in 2 3 4 5 6 7 8 9 10
do
	echo -e "格式化 /dev/sdb$i"
	mkfs.ext2 /dev/sdb$i
done
## check /dev/sdb
echo -e "检查CF卡分区"
for i in 1 2 3 4 5 6 7 8 9 10
do
	echo -e "检查 /dev/sdb$i"
	fsck /dev/sdb$i
done
echo -e "拷贝系统文件"
mount /dev/sdb1 /mnt
cp /home/shan/FlowOS_V1.2.2662M.elf /mnt/FlowOS.elf
cp /home/shan/SempIPE_40P.4_HW_TEST_003.spkg /mnt/SempIPE.spkg
ls -lh /mnt
umount /mnt
exit 0
