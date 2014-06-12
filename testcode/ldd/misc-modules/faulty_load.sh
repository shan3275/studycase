#!/bin/sh
module=complete
device=complete
mode="666"


# invoke insmod with all arguments we got
# and use a pathname, as insmod doesn't look in . by default
/sbin/insmod ./$module.ko $* || exit 1

# retrieve major number
major=$(awk "\$2==\"$device\" {print \$1}" /proc/devices)

# Remove stale nodes and replace them, then give gid and perms
# Usually the script is shorter, it's c531drv that has several devices in it.

rm -f /dev/${module}
mknod /dev/${module} c $major 0

