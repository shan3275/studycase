# -*- coding: UTF-8 -*
#!/usr/bin/python
#Filename:readline.py
import time
import os
import MySQLdb

##查询数据库
##@key : 关键字
##@db_table :  database table
##return : 返回查询结果，单条语句
def query_db(key, value, db_table):
    conn=MySQLdb.connect(host='localhost',user='root',passwd='haiyao',db='kq',port=3306, charset='utf8')
    cur=conn.cursor()
    sql = "select * from %s where %s='%s'" %(db_table, key, value)
    cur.execute(sql)
    info = cur.fetchone()
    cur.close()
    conn.close()
    return info

##插入数据库
##@entry : 表项
##@db_table :  database table
def insert_db(entry, db_table):
    conn=MySQLdb.connect(host='localhost',user='root',passwd='haiyao',db='kq',port=3306,charset='utf8')
    cur=conn.cursor()
    sql = "INSERT INTO `%s` VALUES %s" %(db_table, entry)
    print sql
    cur.execute(sql)
    conn.commit()
    cur.close()
    conn.close()

##更新考勤数据到数据库中
##@file : 输入文件名
def update_kq(file):
    for line in open(file):
        str = line.split() ##str: data mac time time
        person_info = query_db('mac', str[1], 'kq_person_info')
        print person_info #person_info: id name mac
        if person_info:
            entry = "(null, '%s', '%s', '%s', '%s', '%s')" %(str[0], person_info[1], str[1], str[2], str[3])
            print entry
            insert_db(entry, 'kq_day')



if __name__ == "__main__":
    update_kq('result.log')

