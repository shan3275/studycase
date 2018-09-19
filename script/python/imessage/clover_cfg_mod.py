#!/usr/bin/python
# coding:utf-8
#Filename:clover_cfg_mod.py
# 用途：修改clover配置文件，更换mac四码
#
import time,sys,os,re
import logging
from logging import handlers
import yaml
import subprocess
import commands   ##执行系统命令库
import plistlib   ##解析plist文件库
import getopt
import binascii

global logger
global log_file
global log_app

def load_yaml(yaml_file) :
    #print(u'load: ' + yaml_file)
    with open(yaml_file, u'r') as f :
        return yaml.load(f.read())

def log_init(log_app_name, file_name):
    Logger = logging.getLogger(log_app_name)
    Logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    ch = logging.handlers.TimedRotatingFileHandler(
                    filename=file_name,
                    when='midnight',
                    backupCount=3
                    )
    ch.setLevel(logging.DEBUG)
    ch.setFormatter(formatter)
    # add ch to logger
    Logger.addHandler(ch)

    #控制台输出
    sh = logging.StreamHandler()
    sh.setFormatter(formatter)
    #Logger.addHandler(sh)

    #print "日志初始化完成！"
    Logger.info("日志初始化完成！")

    return Logger

def mount_efi():
    global logger
    output = commands.getoutput("df -lh|grep efi")
    if output == '':
        logger.info("efi目录未加载，加载：")
        os.system("mkdir -p /Volumes/efi")
        os.system("mount -t msdos /dev/disk0s1 /Volumes/efi")
    else:
        logger.info("efi磁盘已挂载。")
        return True
    #再次检查是否挂载成功。
    output = commands.getoutput("df -lh|grep efi")
    if output == '':
        logger.info("efi磁盘挂载失败！！！")
        return False
    else:
        logger.info("efi磁盘挂载成功。")
        return True 

def unmount_efi():
    global logger
    output = commands.getoutput("df -lh|grep efi")
    if output != '':
        logger.info("efi目录已加载，准备卸载：")
        os.system("umount /dev/disk0s1")
    else:
        logger.info("efi磁盘未加载，异常。")
        return False
    #再次检查是否挂载成功。
    output = commands.getoutput("df -lh|grep efi")
    if output == '':
        logger.info("efi磁盘卸载成功")
        return True
    else:
        logger.info("efi磁盘卸载失败！！！")
        return False



def ed_codestr_parse(str):
    if str == None:
        print("输入参数错误，格式要求：<ROM>:<MLB>:<SN>:<BID>:<MODEL>")
        print("python clover_cfg_mod.py 0cbc9f34d004:C0250530060G36DAT:C02P68U3FY0T:Mac-81E3E92DD6088272:iMac14,4  conf.yaml")
        return None
        
    row = str.split(':')

    #print "row(%d): %s" %(len(row), row)
    if len(row) != 5:
        print("输入参数错误，格式要求：<ROM>:<MLB>:<SN>:<BID>:<MODEL>")
        print("python clover_cfg_mod.py 0cbc9f34d004:C0250530060G36DAT:C02P68U3FY0T:Mac-81E3E92DD6088272:iMac14,4  conf.yaml")
        print("python clover_cfg_mod.py C82A14EB5B2E:C0712840MEVDKP2A3:DCYG2M6JDJD0:Mac-937CB26E2E02BB01:MacBookAir7,2  conf.yaml")
        return None

    #ROM,MLB,SN,Board-ID,model
    entry  = {}
    entry['ROM']    = row[0]
    entry['MLB']    = row[1]
    entry['SN']     = row[2]
    entry['BID']    = row[3]
    entry['MODEL']  = row[4]

    return entry

if __name__ == '__main__':
    mac_product_dict = {
    'MacBookAir7,2': 
      {
        'BiosVersion': 'MBA71.88Z.0177.B00.1804091616', 
        'BoardType': 10, 
        'SerialNumber': 'C02TKMVMG940', 
        'Family': 'MacBook Air', 
        'BoardSerialNumber': 'C02714401QXFF4NJA', 
        'BoardVersion': '1.0', 
        'ChassisAssetTag': 'Air-Enclosure', 
        'ChassisManufacturer': 'Apple Inc.', 
        'BiosVendor': 'Apple Inc.', 
        'BiosReleaseDate': '04/09/2018', 
        'FirmwareFeatures': '0xE00FE137', 
        'LocationInChassis': 'Part Component', 
        'BoardManufacturer': 'Apple Inc.', 
        'Version': '1.0', 
        'Board-ID': 'Mac-937CB26E2E02BB01', 
        'ChassisType': '0x09', 
        'Mobile': True, 
        'ProductName': 'MacBookAir7,2', 
        'PlatformFeature': '0xFFFF', 
        'FirmwareFeaturesMask': '0xFF1FFF3F', 
        'Manufacturer': 'Apple Inc.'
      },
      'iMac14,4': 
      {
        'BiosVersion': 'IM144.88Z.0189.B00.1804111137', 
        'BoardType': 10, 
        'SerialNumber': 'C02P68U3FY0T', 
        'Family': 'iMac', 
        'BoardSerialNumber': 'C0250530060G36DAT', 
        'BoardVersion': '1.0', 
        'ChassisAssetTag': 'iMac-Aluminum', 
        'ChassisManufacturer': 'Apple Inc.', 
        'BiosVendor': 'Apple Inc.', 
        'BiosReleaseDate': '04/11/2018', 
        'FirmwareFeatures': '0xF00FE137',
        'LocationInChassis': 'Part Component',
        'BoardManufacturer': 'Apple Inc.', 
        'Version': '1.0', 
        'Board-ID': 'Mac-81E3E92DD6088272', 
        'ChassisType': '0x0D', 
        'Mobile': False,
        'ProductName': 'iMac14,4',
        'PlatformFeature': '0x01', 
        'FirmwareFeaturesMask': '0xFF1FFF3F', 
        'Manufacturer': 'Apple Inc.', 
      }
    }
    ##配置文件路径
    #config_plist    = "./config.plist" 
    #config_plist_bc = "./config.plist-back" 
    config_plist    = "/Volumes/efi/EFI/CLOVER/config.plist" 
    config_plist_bc = "/Volumes/efi/EFI/CLOVER/config.plist-back" 

    conf = load_yaml(sys.argv[2])
    log_file      = conf['log']['log_file']
    log_app       = conf['log']['log_app']
    if log_file == None or log_app == None:
        print 'configuration file error, log_file or log_app is None!!'
        print 'Fail'
        os._exit()

    logger = log_init(log_app, log_file)
    if mount_efi() == False:
        print 'Fail'
        os._exit()

    entry={}
    #MLB,ROM,SerialNumber,CustomUUID = argv_probe(sys.argv)
    entry = ed_codestr_parse(sys.argv[1])
    if entry == None:
        print 'Fail'
        os._exit()

    CustomUUID = commands.getoutput("uuidgen")
    logger.info("MLB:          %s", entry['MLB'])
    logger.info("ROM:          %s", entry['ROM'])
    logger.info("SerialNumber: %s", entry['SN'])
    logger.info("BID:          %s", entry['BID'])
    logger.info("MODEL:        %s", entry['MODEL'])
    logger.info("CustomUUID:   %s", CustomUUID)

    pl = plistlib.readPlist(config_plist)
    ###如果找不到对应型号的配置文件，卸载磁盘退出
    if mac_product_dict.has_key(entry['MODEL']) == False:
        logger.info('找不到型号：%s', entry['MODEL'])
        print 'Fail'
        if unmount_efi() == False:
            print 'Fail'
            os._exit()
        os._exit()
    ####找到对应型号的配置文件
    pl['RtVariables']['MLB'] = entry['MLB']
    pl['RtVariables']['ROM'] = plistlib.Data(binascii.a2b_hex(entry['ROM']))
    pl['SystemParameters']['CustomUUID'] = CustomUUID 
    pl['SMBIOS']['SerialNumber'] = entry['SN']
    pl['SMBIOS']['BoardSerialNumber'] = entry['MLB']
    ###根据配置文件修改参数
    product_dict = mac_product_dict[entry['MODEL']]
    for key in product_dict:
         logger.info('%s:%s', key, product_dict[key])
         pl['SMBIOS'][key] = product_dict[key]
    os.system('mv '+config_plist+' '+config_plist_bc)
    plistlib.writePlist(pl, config_plist)

    if unmount_efi() == False:
        print 'Fail'
        os._exit()
    print 'Success'