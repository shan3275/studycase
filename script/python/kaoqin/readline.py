# -*- coding: UTF-8 -*
#!/usr/bin/python
#Filename:readline.py
import time
import os
import sys

##将原始的数据文件进行第一次分割，分割后的文件以mac地址命名
##每个文件存放该mac地址相关的信息
##@mac_file :原始数据文件名
def select_user(day):
    mac_file = "log/" + day + "Mac_list.txt"
    temp_dir = "temp/" + day + "/"
    os.mkdir(temp_dir)
    for line in open(mac_file):
        str = line.split()
        f = file(temp_dir+str[2]+".log", 'aw')
        f.write(line)
        f.close()


##对单个mac文件进行处理，取第一行和最后一行，作为上线和下线的标志
##@mac_file: 单个mac文件
##return: 返回新的字符串，字符串格式如下：
##   date        mac            work_on_time work_off_time
## 2016-08-27 80:e6:50:00:ec:26 09:10:35     19:16:34
def user_entry(mac_file):
    fp = open(mac_file, 'r')
    fp_line = []
    for line in fp:
        fp_line.append(line)
    fp.close()
    first_str  = fp_line[0].split()
    last_str   = fp_line[-1].split()
    return first_str[0]+" "+first_str[2]+" "+first_str[1]+" "+last_str[1]

##读取临时目录下面的文件，每个文件产生一行信息，组成一个新的文件。
##@dir_in :输入的目录名
##@file_out: 输出的文件名
def user_result(day):
    file_out = "result/" + day + "result.log"
    dir_in = "temp/" + day
    f = file(file_out, 'w')
    for filename in os.listdir(dir_in):
        single_file = dir_in + "/" + filename
	print single_file
        f.write(user_entry(single_file) + '\n')
    f.close()

if __name__ == '__main__':
    ##手动输入参数，格式：2016-08-30
    if sys.argv[1]:
        day = sys.argv[1]
    else:
        day = datetime.date.today() - datetime.timedelta(days=1)
    print day
    select_user(day)
    user_result(day)
