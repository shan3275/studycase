#!/bin/sh
# Program:
#       This program is used to set environment variable.
# History:
# 2011/08/29         shan      First release
m="abc"
echo $m # 输出为abc
declare -u m
echo $m # 输出为abc,
m="cde"
echo $m # 输出为CDE
declare -l m="HELL"
echo $m # 输出为hell


