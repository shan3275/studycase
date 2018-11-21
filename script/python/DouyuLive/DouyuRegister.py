#!/usr/bin/python
#-*- coding: UTF-8 -*-
# FileName : DouyuRegister.py
# Author   : Shan
# DateTime : 2018/11/14
# SoftWare : PyCharm

import requests,json
import time,os
import logging
from logging import handlers
from collections import OrderedDict
import re
import random,string
import sys
import urllib
from urllib import urlencode
from urllib import unquote
from cmd import Cmd
import yaml

global logger
def log_init(log_app_name, file_name):
    logger = logging.getLogger(log_app_name)
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    ch = logging.handlers.TimedRotatingFileHandler(
                    filename=file_name,
                    when='midnight',
                    backupCount=3
                    )
    ch.setLevel(logging.DEBUG)
    ch.setFormatter(formatter)
    # add ch to logger
    logger.addHandler(ch)

    #控制台输出
    sh = logging.StreamHandler()
    sh.setFormatter(formatter)
    #logger.addHandler(sh)

    return logger

global CONF
def read_yaml():
    # 你的yaml格式文件路径
    path = 'yaml.conf'
    with open(path,'r') as file:
        # 将yaml格式内容转换成 dict类型
        load_data = yaml.load(file.read())
        logger.debug(load_data)
        return load_data

"""
功能：     注册时从斗鱼官方服务器获取极验需要的参数
输入参数：  无
输出参数：  False : 失败，返回False，并有日志记录
           ou    : 成功，返回数据字典，字典内容如下
                ou.gt ：斗鱼官方的GeeTest ID，不变
                ou.challenge：斗鱼分配给客户端的ID,每次都会变化
"""
def RegisterGetChallenge():
    result = {}
    url = "https://passport.douyu.com/iframe/checkGeeTest"
    headers = {'Content-Type'   : 'application/x-www-form-urlencoded; charset=UTF-8',
               'User-Agent'     : 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Mobile Safari/537.36',
               'accept'         : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
               'accept-encoding': 'gzip, deflate, br',
               'accept-language': 'zh-CN,zh;q=0.9,en;q=0.8,zh-TW;q=0.7',
               'referer'        : 'https://passport.douyu.com/member/regNew?client_id=1&lang=cn'}
    data = ""

    r = requests.post(url, data=data,headers=headers)
    logger.info(r.url)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.info('post 成功, r.status_code=%d', r.status_code)

    responsed = r.json()
    ##判断返回数据标志位
    if responsed['error'] != 0:
        logger.error('获取数据失败 error=%d', responsed['error'])
        return False
    logger.error('获取数据成功 error=%d', responsed['error'])
    logger.info(responsed)

    ##判断返回数据中是否有gt和challenge
    rsdt = responsed['data']
    if rsdt.has_key('challenge') != True or rsdt.has_key('gt') != True :
        logger.error('challenge or gt 不存在')
        return False

    result['gt'] = rsdt['gt']
    result['challenge'] = rsdt['challenge']
    logger.info(result)
    return result

"""
功能：     登陆时从斗鱼官方服务器获取极验需要的参数
输入参数：  无
输出参数：  False : 失败，返回False，并有日志记录
           ou    : 成功，返回数据字典，字典内容如下
                ou.gt ：斗鱼官方的GeeTest ID，不变
                ou.challenge：斗鱼分配给客户端的ID,每次都会变化
"""
def LoginGetChallenge():
    result = {}
    url = "https://www.douyu.com/member/login/check_geetest_status"
    headers = {'Content-Type'   : 'application/x-www-form-urlencoded',
               'User-Agent'     : 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Mobile Safari/537.36',
               'accept'         : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
               'accept-encoding': 'gzip, deflate, br',
               'accept-language': 'zh-CN,zh;q=0.9,en;q=0.8,zh-TW;q=0.7'}
    data = "op_type=3722"

    r = requests.post(url, data=data,headers=headers)
    logger.info(r.url)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.info('post 成功, r.status_code=%d', r.status_code)

    responsed = r.json()
    ##判断返回数据标志位
    if responsed['error'] != 0:
        logger.error('获取数据失败 error=%d', responsed['error'])
        return False
    logger.error('获取数据成功 error=%d', responsed['error'])
    logger.info(responsed)

    ##判断返回数据中是否有gt和challenge
    rsdt = responsed['data']
    if rsdt.has_key('challenge') != True or rsdt.has_key('gt') != True :
        logger.error('challenge or gt 不存在')
        return False

    result['gt'] = rsdt['gt']
    result['challenge'] = rsdt['challenge']
    logger.info(result)
    return result

"""
功能：    查询点数接口
输入参数： para : dict{}字典，内容如下：
            para.user:  第三方极验平台用户名
            para.pwd:   第三方极验平台密码
输出参数： False : 失败，返回False，并有日志记录
          dianshu:  返回用户所剩点数
"""
def InquiryGeetestPoints(para):
    if para.has_key('user') != True or para.has_key('pwd') != True :
        logger.error('参数错误')
        return False

    payload = {'user': para['user'], 'pass': para['pwd'], 'format':'utf8'}
    r = requests.get("http://jiyanapi.c2567.com/chaxundianshu", params=payload)
    logger.debug(r.url)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('get 失败,r.status_code=%d', r.status_code)
        return False
    logger.info('get 成功, r.status_code=%d', r.status_code)
    logger.info(r.text)

    data = r.json()
    if data['status'] != 'ok':
        logger.error('返回数据有问题')
        return False
    logger.info('查询点数为：%d', data['dianshu'])
    return data['dianshu']

"""
功能：     通过第三方平台接口进行极验
输入参数：  para=dict() 单个字典参数，字典中包含如下元素
               para.user: 第三方极验平台用户名
               para.pwd： 第三方极验平台密码
               para.gt    斗鱼官方的GeeTest ID，不变
               para.challenge  斗鱼分配给客户端的ID,每次都会变化
输出参数：  False : 失败，返回False，并有日志记录
           out   : 成功，返回数据字典，字典内容如下
                out.validate ：第三方极验平台返回的识别号
                out.challenge：斗鱼分配给客户端的ID,每次都会变化
"""
def JiYanGeetest(para):
    if para == False:
        logger.error('输入参数为空')
        return False

    ##判断点数是否足够
    GeetestInfo = dict(user=para['user'], pwd=para['pwd'])
    logger.info(GeetestInfo)
    points = InquiryGeetestPoints(GeetestInfo)
    if points == False:
        logger.info('获取点数失败')
        return False
    if points < 50:
        logger.info('点数过低，请及时充值')
    if points < 10:
        logger.info('点数基本耗尽，请充值后再尝试')
        return False
    logger.info(points)

    payload = OrderedDict(gt=para['gt'],challenge=para['challenge'],referer='https://www.douyu.com/', \
                          devuser='707399420',user=para['user'])
    payload['pass'] = para['pwd']
    payload['return'] = 'json'
    payload['model'] = '3'
    payload['format'] = 'utf8'
    logger.info(payload)

    r = requests.get("http://jiyanapi.c2567.com/shibie", params=payload)
    logger.info(r.url)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('get 失败,r.status_code=%d', r.status_code)
        return False
    logger.info('get 成功, r.status_code=%d', r.status_code)
    logger.info(r.text)

    data = r.json()
    status = data['status']
    if  status == 'no':
        logger.error('请重新获取验证码参数，并重新提交识别')
        return False
    elif status == 'stop':
        logger.error('请停止程序并提示错误信息。该状态是账号有问题的情况')
        return False
    else:
        logger.info('接口返回成功，请带着识别成功的数据提交')

    out = dict(challenge=data['challenge'], validate=data['validate'])
    logger.info(out)
    return out


"""
功能：接码平台登录
输入参数：user ：用户名
         pwd  ：密码    
返回值：  False ： 登录失败
         token ：
"""
def JieMaLogin(user,pwd):
    url = "http://api.jmyzm.com/http.do"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    data = "action=loginIn&uid=%s&pwd=%s" %(user, pwd)
    r = requests.post(url, data=data, headers=headers)
    logger.debug('接码平台链接: '+str(r.url))
    logger.debug('命令字: '+data)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = str(r.text)
    logger.debug('返回内容：%s', responsed)
    if responsed == 'account_is_stoped' :
        logger.error('账号被停用')
        return False
    elif responsed == 'account_is_locked':
        logger.error('账号被锁定（无法取号，充值任意金额解锁，请登录官网查看详情！）')
        return False
    elif responsed == 'account_is_closed':
        logger.error('账号被关闭（登录官网进入安全中心开启）')
        return False
    elif responsed == 'message|to_fast_try_again':
        logger.error('访问过快，限制1秒一次。')
        return False
    elif responsed == 'login_error':
        logger.error('用户名密码错误')
        return False
    responsedArray = responsed.split('|', 1 )
    logger.debug(responsedArray)
    acc = responsedArray[0]
    token = responsedArray[1]
    if acc.lower() == user.lower():
        logger.debug('Token 获取成功：'+token)
        return token


"""
功能：获取本机IP
输入参数：无
输出参数：False :获取IP失败
         IP    :获取IP,返回字符串
"""
def GetCurrentIP():
    r = requests.get(r"http://jsonip.com")
    if r.status_code != requests.codes.ok:
        logger.error('get 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('get 成功, r.status_code=%d', r.status_code)
    logger.debug(r.text)
    ip = str(r.json()['ip'])
    logger.debug('本机IP：'+ip)
    return ip

"""
功能：获取地理位置
输入参数：ip
输出参数：False  :获取地理位置失败
         location ：dict
            location.province  :省份
            location.city      :城市
"""
def GetCurrentLocation(ip):
    ###post方式不太稳定，故使用get方式
    #url = "http://ip.taobao.com/service/getIpInfo.php"
    #headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    #data = 'ip=' + ip
    #r = requests.post(url, data=data, headers=headers)
    r = requests.get('http://ip.taobao.com/service/getIpInfo.php?ip=%s' % ip)
    logger.debug(r.url)
    if r.status_code != requests.codes.ok:
        logger.error('get 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('get 成功, r.status_code=%d', r.status_code)
    logger.debug(r.text)
    responsed = r.json()
    statusCode = str(responsed['code'])
    if statusCode != '0':
        logger.error("返回信息错误")
        return False
    region = responsed['data']['region'].encode("utf-8")
    city   = responsed['data']['city'].encode("utf-8")
    location = dict(province=region,city=city)
    logger.debug('location : %s', location)
    logger.debug(urlencode(location))
    return location

"""
功能：从接码平台获取手机号码
输入参数：user  : 用户名，字符串
         token ：用户登陆返回的token，字符串
         pid   ：项目id，string格式
         location：字典，格式{'province': '\xe6\xb9\x96\xe5\x8c\x97', 'city': '\xe6\xad\xa6\xe6\xb1\x89'}
返回值：  False ： 登录失败
         phone ： 如果成功，返回手机号码
"""
def JieMaGetPhone(user,token,pid,location):
    url = "http://api.jmyzm.com/http.do"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    #data = "action=getMobilenum&uid=%s&token=%s&pid=%s&%s&vno=0" % (user, token,pid,urlencode(location))
    data = "action=getMobilenum&uid=%s&token=%s&pid=%s&vno=0" % (user, token, pid )
    r = requests.post(url, data=data, headers=headers)
    logger.debug('接码平台链接: ' + str(r.url))
    logger.debug('命令字: ' + data)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = str(r.text)
    logger.debug('返回内容：%s', responsed)
    if responsed == 'account_is_stoped':
        logger.error('账号被停用')
        return False
    elif responsed == 'account_is_locked':
        logger.error('账号被锁定（无法取号，充值任意金额解锁，请登录官网查看详情！）')
        return False
    elif responsed == 'account_is_closed':
        logger.error('账号被关闭（登录官网进入安全中心开启）')
        return False
    elif responsed == 'message|to_fast_try_again':
        logger.error('访问过快，限制1秒一次。')
        return False
    elif responsed == 'no_data':
        logger.error('系统暂时没有可用号码了')
        return False
    elif responsed == 'parameter_error':
        logger.error('传入参数错误')
        return False
    elif responsed == 'not_login':
        logger.error('没有登录,在没有登录下去访问需要登录的资源，忘记传入uid,token,或者传入token值错误，请登录获得最新token值')
        return False
    elif responsed == 'you_cannot_get':
        logger.error('使用了项目绑定（登录官网进入安全中心解除绑定或添加该项目绑定）')
        return False
    elif responsed == 'not_found_project':
        logger.error('没有找到项目,项目ID不正确')
        return False
    elif responsed == 'Lack_of_balance':
        logger.error('可使用余额不足')
        return False
    elif responsed == 'max_count_disable':
        logger.error('已经达到了当前等级可以获取手机号的最大数量，请先处理完您手上的号码再获取新的号码（处理方式：能用的号码就获取验证码，不能用的号码就加黑）')
        return False
    elif responsed == 'unknow_error':
        logger.error('未知错误,再次请求就会正确返回')
        return False
    responsedArray = responsed.split('|', 1)
    logger.debug(responsedArray)
    phone = responsedArray[0]
    token = responsedArray[1]
    if token.lower() == token.lower():
        logger.debug('手机号码获取成功：' + phone)
        return phone

"""
功能：从接码平台获取验证码并不再使用本号码
输入参数：user  : 用户名，字符串
         token ：用户登陆返回的token，字符串
         phone ：手机号码
返回值：  False ： 登录失败
         code ： 如果成功，返回验证码，并释放手机号码
"""
def JieMaGetCode(user,token,phone):
    url = "http://api.jmyzm.com/http.do"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    data = "action=getVcodeAndReleaseMobile&uid=%s&token=%s&mobile=%s" % (user, token, phone)
    r = requests.post(url, data=data, headers=headers)
    logger.debug('接码平台链接: ' + str(r.url))
    logger.debug('命令字: ' + data)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = str(r.text)
    logger.debug('返回内容：%s', responsed)
    if responsed == 'account_is_stoped':
        logger.error('账号被停用')
        return False
    elif responsed == 'account_is_locked':
        logger.error('账号被锁定（无法取号，充值任意金额解锁，请登录官网查看详情！）')
        return False
    elif responsed == 'account_is_closed':
        logger.error('账号被关闭（登录官网进入安全中心开启）')
        return False
    elif responsed == 'message|to_fast_try_again':
        logger.error('访问过快，限制1秒一次。')
        return False
    elif responsed == 'not_receive':
        logger.error('还没有接收到验证码,请让程序等待几秒后再次尝试')
        return False
    elif responsed == 'parameter_error':
        logger.error('传入参数错误')
        return False
    elif responsed == 'not_login':
        logger.error('没有登录,在没有登录下去访问需要登录的资源，忘记传入uid,token,或者传入token值错误，请登录获得最新token值')
        return False
    elif responsed == 'not_found_moblie':
        logger.error('没有找到手机号')
        return False
    elif responsed == 'not_found_project':
        logger.error('没有找到项目,项目ID不正确')
        return False
    elif responsed == 'Lack_of_balance':
        logger.error('可使用余额不足')
        return False
    responsedArray = responsed.split('|', 1)
    logger.debug(responsedArray)
    mobile = responsedArray[0]
    code = responsedArray[1]
    if phone.lower() == mobile.lower():
        logger.debug('验证码获取成功：' + code)
        return code


"""
功能：在接码平台加黑无用号码
输入参数：user  : 用户名，字符串
         token ：用户登陆返回的token，字符串
         phone ：手机号码
返回值：  False ： 登录失败
         code ： 如果成功，返回验证码，并释放手机号码
"""
def JieMaAddIgnoreList(user,token,phone):
    url = "http://api.jmyzm.com/http.do"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    data = "action=addIgnoreList&uid=%s&token=%s&mobile=%s" % (user, token, phone)
    r = requests.post(url, data=data, headers=headers)
    logger.debug('接码平台链接: ' + str(r.url))
    logger.debug('命令字: ' + data)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = str(r.text)
    logger.debug('返回内容：%s', responsed)
    if responsed == 'account_is_stoped':
        logger.error('账号被停用')
        return False
    elif responsed == 'account_is_locked':
        logger.error('账号被锁定（无法取号，充值任意金额解锁，请登录官网查看详情！）')
        return False
    elif responsed == 'account_is_closed':
        logger.error('账号被关闭（登录官网进入安全中心开启）')
        return False
    elif responsed == 'message|to_fast_try_again':
        logger.error('访问过快，限制1秒一次。')
        return False
    elif responsed == 'parameter_error':
        logger.error('传入参数错误')
        return False
    elif responsed == 'not_login':
        logger.error('没有登录,在没有登录下去访问需要登录的资源，忘记传入uid,token,或者传入token值错误，请登录获得最新token值')
        return False
    elif responsed == 'unknow_error':
        logger.error('未知错误,再次请求就会正确返回')
        return False
    logger.debug('加黑成功的号码数量：' + responsed)
    return responsed

"""
功能：从接码平台获取用户点数
输入参数： user:  用户ID
         token ：用户登陆返回的token    
返回值：  False ： 登录失败
         points ： 如果成功，返回用户点数
"""
def JieMaetGetUserPoints(user, token):
    url = "http://api.jmyzm.com/http.do"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    data = "action=getUserInfos&uid=%s&token=%s" % (user, token)
    r = requests.post(url, data=data, headers=headers)
    logger.debug('接码平台链接: ' + str(r.url))
    logger.debug('命令字: ' + data)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = str(r.text)
    logger.debug('返回内容：%s', responsed)
    if responsed == 'account_is_stoped':
        logger.error('账号被停用')
        return False
    elif responsed == 'account_is_locked':
        logger.error('账号被锁定（无法取号，充值任意金额解锁，请登录官网查看详情！）')
        return False
    elif responsed == 'account_is_closed':
        logger.error('账号被关闭（登录官网进入安全中心开启）')
        return False
    elif responsed == 'message|to_fast_try_again':
        logger.error('访问过快，限制1秒一次。')
        return False
    elif responsed == 'not_login':
        logger.error('没有登录,在没有登录下去访问需要登录的资源，忘记传入uid,token,或者传入token值错误，请登录获得最新token值')
        return False
    elif responsed == 'parameter_error':
        logger.error('传入参数错误')
        return False
    elif responsed == 'unknow_error':
        logger.error('未知错误,再次请求就会正确返回')
        return False
    responsedArray = responsed.split(';')
    logger.debug(responsedArray)
    logger.debug('用户名：'+responsedArray[0]+', 积分：'+responsedArray[1]+', 用户币：'+responsedArray[2]+', 可同时获取号码数：'+responsedArray[3])
    return responsedArray[2]  #返回用户币数


"""
功能：从接码平台获取已经获取的号码列表
输入参数： user:  用户ID
         token ：用户登陆返回的token 
         pid   : 项目ID   
返回值：  False ： 登录失败
         info ： 如果成功，返回：
                没有获取号码记录:[ ] 
                单条：[{"Pid":项目ID,"Recnum":号码,"Timeout":号码超时释放所剩时间,"Start_time":获取号码时间}]（*"Timeout"以秒为单位） 
                多条：[{"Pid":项目ID,"Recnum":号码,"Timeout":号码超时释放所剩时间,"Start_time":获取号码时间},
                      {"Pid":项目ID,"Recnum":号码,"Timeout":号码超时释放所剩时间,"Start_time":获取号码时间},
                      {...}
                     ]（*"Timeout"以秒为单位）
"""
def JieMaetGetRecvInfo(user, token, pid):
    url = "http://api.jmyzm.com/http.do"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    data = "action=getRecvingInfo&uid=%s&token=%s&pid=%s" % (user, token,pid)
    r = requests.post(url, data=data, headers=headers)
    logger.debug('接码平台链接: ' + str(r.url))
    logger.debug('命令字: ' + data)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = str(r.text)
    logger.debug('返回内容：%s', responsed)
    if responsed == 'account_is_stoped':
        logger.error('账号被停用')
        return False
    elif responsed == 'account_is_locked':
        logger.error('账号被锁定（无法取号，充值任意金额解锁，请登录官网查看详情！）')
        return False
    elif responsed == 'account_is_closed':
        logger.error('账号被关闭（登录官网进入安全中心开启）')
        return False
    elif responsed == 'message|to_fast_try_again':
        logger.error('访问过快，限制1秒一次。')
        return False
    elif responsed == 'not_login':
        logger.error('没有登录,在没有登录下去访问需要登录的资源，忘记传入uid,token,或者传入token值错误，请登录获得最新token值')
        return False
    elif responsed == 'parameter_error':
        logger.error('传入参数错误')
        return False
    elif responsed == 'unknow_error':
        logger.error('未知错误,再次请求就会正确返回')
        return False
    #logger.debug('用户名：'+responsedArray[0]+', 积分：'+responsedArray[1]+', 用户币：'+responsedArray[2]+', 可同时获取号码数：'+responsedArray[3])
    #return responsedArray[2]  #返回用户币数

"""
功能：判断手机号码有消息
输入参数：phone：手机号码，字符串
输出参数：False：无效
         True： 有效
"""
def PhoneJudge(phone):
    n = phone
    logger.debug(type(n))
    if re.match(r'1[3,4,5,7,8]\d{9}', n):
        print("您输入的的手机号码是："+n)
        # 中国联通：
        # 130，131，132，155，156，185，186，145，176
        if re.match(r'13[0,1,2]\d{8}', n) or \
                re.match(r"15[5,6]\d{8}", n) or \
                re.match(r"18[5,6]", n) or \
                re.match(r"145\d{8}", n) or \
                re.match(r"176\d{8}", n):
            print("该号码属于：中国联通")
        # 中国移动
        # 134, 135 , 136, 137, 138, 139, 147, 150, 151,
        # 152, 157, 158, 159, 178, 182, 183, 184, 187, 188；
        elif re.match(r"13[4,5,6,7,8,9]\d{8}", n) or \
                re.match(r"147\d{8}|178\d{8}", n) or \
                re.match(r"15[0,1,2,7,8,9]\d{8}", n) or \
                re.match(r"18[2,3,4,7,8]\d{8}", n):
            print("该号码属于：中国移动")
        else:
            # 中国电信
            # 133,153,189
            print("该号码属于：中国电信")
    else:
        print("请输入正确的手机号")
        return False
    return True

"""
功能：     获取手机号码
输入参数：  无
返回值：  phone:  手机号码，类型字符串
"""
def RecvCodeGetPhone():
    user = CONF['jiema']['user']
    pwd = CONF['jiema']['pwd']
    token = JieMaLogin(user, pwd)
    if token == False:
        logger.error('接码平台Token获取失败')
        return False
    ip = GetCurrentIP()
    if ip == False:
        logger.error('获取本机IP失败')
        return False
    logger.info("客户端本机IP获取成功")
    location = ''
    #location = GetCurrentLocation(ip)
    #if location == False:
    #    logger.error('获取本机位置失败')
    #    return False
    phone = JieMaGetPhone(user, token, '3737', location)
    if phone == False:
        logger.error('获取手机号码失败')
        return False
    #if PhoneJudge(phone) == False:
    #    logger.error('手机号码无效')
    #    return False
    return phone

"""
功能：     注册时触发发送验证码
输入参数：  
            phone  :手机号码,字符串
            challenge :斗鱼平台分配给客户端的ID，字符串
            validate  :第三方平台返回的识别号，字符串
输出参数：  
            False :错误
            True  :成功
"""
def RegisterSendSecurityCode(phone,challenge,validate):
    url="https://passport.douyu.com/iframe/registerCaptcha"
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    data = "phoneNum=" + phone + "&areaCode=0086&lang=cn&geetest_challenge=" + challenge + "&geetest_validate=" + validate + \
           "&geetest_seccode=" + validate + "%7Cjordan"
    logger.info(data)
    r = requests.post(url, data=data,headers=headers)
    logger.info(r.url)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.info('post 成功, r.status_code=%d', r.status_code)

    responsed = r.json()
    logger.debug(responsed)
    if responsed.has_key('msg'):
        logger.debug('msg:'+ responsed['msg'])
    error = str(responsed['error'])

    if error == '130018':
        logger.info('二次发送')
        data2 = data + "&confirm=1"
        logger.debug(data2)
        r = requests.post(url, data=data2, headers=headers)
        ##判断http post返回值
        if r.status_code != requests.codes.ok:
            logger.error('二次post 失败,r.status_code=%d', r.status_code)
            return False
        logger.info('二次post 成功, r.status_code=%d', r.status_code)
        responsed = r.json()
        logger.debug(responsed)
        logger.error('msg:' + responsed['msg'])
        error = str(responsed['error'])

    if error == '0':
        logger.info('发送成功')
        return True
    else:
        logger.error('发送失败')
        return False


"""
功能：     获取手机的验证码
输入参数：  无
输出参数：  code:  类型字符串
"""
def RecvCodeGetSecurityCode():
    inp = input("请输入验证码：")
    code = str(inp)
    logger.info('手机验证码：%s', code)
    return code

"""
功能：产生随机密码
输入参数：
输出参数：str_pass: 字符串8位
"""
def RandomPasswordGetOne():
    src = string.ascii_letters + string.digits
    rpasswd = random.sample(src, 5)  # 从字母和数字中随机取5位
    rpasswd.extend(random.sample(string.digits, 1))  # 让密码中一定包含数字
    rpasswd.extend(random.sample(string.ascii_lowercase, 1))  # 让密码中一定包含小写字母
    rpasswd.extend(random.sample(string.ascii_uppercase, 1))  # 让密码中一定包含大写字母
    random.shuffle(rpasswd)  # 打乱列表顺序
    str_passwd = ''.join(rpasswd)  # 将列表转化为字符串
    logger.info("产生随机密码：%s", str_passwd)
    return str_passwd


"""
功能：     向斗鱼提交注册账号
输入参数：  字典，包含注册信息
                phone  :手机号
                code   :验证码
                pwd    :随机密码
                challenge  :斗鱼分配给客户端的ID
                validate   :第三方极验平台返回的识别码
输出参数：  False : 失败，返回False，并有日志记录
           ou    : 成功，返回数据字典，字典内容如下
                ou.gt ：斗鱼官方的GeeTest ID，不变
                ou.challenge：斗鱼分配给客户端的ID,每次都会变化
"""
def RegisterSubmit(phone,code,pwd,challenge,validate):
    url = "https://passport.douyu.com/iframe/register"
    headers = {'Content-Type'   : 'application/x-www-form-urlencoded',
               'User-Agent'     : 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Mobile Safari/537.36',
               'accept'         : 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
               'accept-encoding': 'gzip, deflate, br',
               'accept-language': 'zh-CN,zh;q=0.9,en;q=0.8,zh-TW;q=0.7'}
    data = "areaCode=0086&phoneNum=" + phone + "&password=" + pwd + "&geetest_challenge="\
           + challenge + "&geetest_validate=" + validate + "&geetest_seccode=" + validate + \
           "%7Cjordan&phoneCaptcha=" + code + "&protocol=on&pwdStrength=20&password2=" + pwd+ \
           "&redirect_url=https%3A%2F%2Fpassport.douyu.com%2Fmember%2FregNew%3Fclient_id%3D1%26lang%3Dcn&client_id=1&reg_src=web&cpsid=0&room_id=0&cate_id=0&tag_id=0&child_id=0&vid=0&fac=&sm_did=&did=&lang=cn"
    logger.debug(data)
    r = requests.post(url, data=data,headers=headers)
    logger.debug(r.url)
    ##判断http post返回值
    if r.status_code != requests.codes.ok:
        logger.error('post 失败,r.status_code=%d', r.status_code)
        return False
    logger.debug('post 成功, r.status_code=%d', r.status_code)

    responsed = r.json()
    if responsed.has_key('msg'):
        logger.debug('msg:'+ responsed['msg'])
    logger.debug(responsed)
    error = str(responsed['error'])

    if error == '0':
        logger.debug('获取数据成功')
        return True
    else:
        logger.error('获取数据失败')
        return False

class Cli(Cmd):
    u"""help
    这是doc
     """
    prompt = 'douYu>'
    intro = 'Welcom!'

    def __init(self):
        Cmd.__init__(self)

    def preloop(self):
        print "欢迎进入斗鱼注册程序命令行"

    def postloop(self):
        print 'Bye!'
        print "退出程序"

    def do_exit(self, arg):
        return True  # 返回True，直接输入exit命令将会退出
    def help_exit(self):
        print '退出命令行'

    ##获取本机的IP地址，内部调试使用
    def do_ip(self,line):
        ip = GetCurrentIP()
        if ip == False:
            print('获取本机IP失败')
            return
        print("客户端本机IP获取成功:"+ip)
    def help_ip(self):
        print '获取本机公网IP地址'

    ##获取本机的地理位置，省份和城市
    def do_location(self,line):
        ip = GetCurrentIP()
        if ip == False:
            print('获取本机IP失败')
            return
        logger.info("客户端本机IP获取成功")
        location = GetCurrentLocation(ip)
        if location == False:
            print('获取本机位置失败')
            return
        print('客户端本机位置获取成功: ' + unquote(urlencode(location)))
    def help_location(self):
        print '获取当前客户端IP地址所在的省份及城市'

    ##打印配置文件新
    def do_conf(self,line):
        print(CONF)
    def help_conf(self):
        print '打印配置文件信息'

    ##接码平台登录获取token
    def do_jmlogin(self,line):
        user = CONF['jiema']['user']
        pwd  = CONF['jiema']['pwd']
        token = JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        print('接码平台Token获取成功: token='+token)
    def help_jmlogin(self):
        print '接码平台登录获取token'

    ##接码平台获取点数
    def do_jmpoints(self,line):
        user = CONF['jiema']['user']
        pwd  = CONF['jiema']['pwd']
        token = JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        points = JieMaetGetUserPoints(user, token)
        if points == False:
            print('接码平台点数获取失败')
            return
        print('接码平台点数获取成功，点数：' + points)
    def help_jmpoints(self):
        print '接码平台登录获取点数'

    ##获取接码平台的信息
    def do_jminfos(self,line):
        user = CONF['jiema']['user']
        pwd  = CONF['jiema']['pwd']
        token = JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        info = JieMaetGetRecvInfo(user, token, '0')
        if info == False:
            print('接码平台获取信息失败')
            return
        print info
    def help_jminfos(self):
        print('打印接码平台获取的信息')

    ##获取接码平台手机号码
    def do_jmgetphone(self,line):
        phone = RecvCodeGetPhone()
        if phone == False:
            print('获取手机号码失败')
            return
        print('phone: '+phone)
    def help_jmgetphone(self):
        print('获取接码平台手机号码')

    ##接码平台加黑无用号码
    def do_jmignorephone(self,phone):
        user = CONF['jiema']['user']
        pwd = CONF['jiema']['pwd']
        token = JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        num = JieMaAddIgnoreList(user, token, phone)
        if num == False:
            print('加黑无用号码失败')
            return
        print('加黑号码成功数：',num)
    def help_jmignorephone(self):
        print('接码平台加黑无用号码')

    ##接码平台获取验证码
    def do_jmgetcode(self,phone):
        if phone == '':
            print('请输入手机号码作为参数')
            return
        user = CONF['jiema']['user']
        pwd = CONF['jiema']['pwd']
        token = JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        code = JieMaGetCode(user, token, phone)
        if code == False:
            print('接码平台验证码获取失败')
            return
        print('接码平台验证码获取成功, '+phone+' '+code)
    def help_jmgetcode(self):
        print('接码平台获取验证码,执行命令时，需输入参数：手机号码')

    ##注册账号签获取gt和challenge
    def do_reggetgt(self,line):
        ##获取gt和challenge
        gt = RegisterGetChallenge()
        if gt == False:
            print('获取数据失败')
            return
        print('获取gt和challenge数据成功')
        print(gt)
    def help_reggetgt(self):
        print('注册账号签获取gt和challenge')

    ##极验平台获取极验码
    def do_jyvalidate(self,line):
        ##获取gt和challenge
        gt = RegisterGetChallenge()
        if gt == False:
            print('获取数据失败')
            return
        GetGeetestPara = dict()
        GetGeetestPara.update(gt)
        GetGeetestPara.update(CONF['jiyan'])
        logger.debug(GetGeetestPara)
        ou = JiYanGeetest(GetGeetestPara)
        if ou == False:
            print('获取数据失败')
            return
        print('获取识别码validate和challenge成功')
        print(ou)
    def help_jyvalidate(self):
        print('极验平台获取极验码')

    ##发送验证码
    def do_sendcode(self,phone):
        if PhoneJudge(phone) == False:
            print('手机号码无效')
            return
        ##获取gt和challenge
        gt = RegisterGetChallenge()
        if gt == False:
            print('获取gt数据失败')
            return
        GetGeetestPara = dict()
        GetGeetestPara.update(gt)
        GetGeetestPara.update(CONF['jiyan'])
        logger.debug(GetGeetestPara)
        ou = JiYanGeetest(GetGeetestPara)
        if ou == False:
            print('获取极验数据失败')
            return
        challenge = ou['challenge']
        validate = ou['validate']
        if RegisterSendSecurityCode(phone, challenge, validate) != True:
            print('发送验证码失败')
            return
        print("发送验证码成功")

    def help_sendcode(self):
        print('发送验证码，执行命令需输入参数：手机号码')

    def do_regsubmit(self, line):
        if line == '':
            print '请输入参数：phone code'
            return
        arg = line.split()
        phone = arg[0]
        code  = arg[1]
        if phone == '' or code == '':
            print '请输入参数：phone code'
            return
        ##产生随机密码
        pwd = RandomPasswordGetOne()

        ## 产生challenge和validate
        ##获取gt和challenge
        gt = RegisterGetChallenge()
        if gt == False:
            print('获取数据失败')
            return
        GetGeetestPara = dict()
        GetGeetestPara.update(gt)
        GetGeetestPara.update(CONF['jiyan'])
        ou = JiYanGeetest(GetGeetestPara)
        if ou == False:
            print('获取数据失败')
            return
        challenge = ou['challenge']
        validate  = ou['validate']
        print('phone:%s,code:%s,pwd:%s,challenge:%s,validate:%s', phone,code,pwd,challenge,validate)
        msg = RegisterSubmit(phone,code,pwd,challenge,validate)
        if msg != True:
            print('提交注册失败')
            return
        print("提交注册成功")
        print(msg)
    def help_regsubmit(self):
        print('提交注册，需要输入参数：手机号码 验证码')


if __name__ == "__main__":
    ##日志初始化
    logger = log_init('DY', './DY.log')
    CONF = read_yaml()
    cli = Cli()
    cli.cmdloop()

    ##获取gt和challenge
    rv = RegisterGetChallenge()
    if rv == False:
        logger.error('获取数据失败：%s',rv)
        sys.exit()
    logger.info('获取gt和challenge数据成功', rv)

    ##获取识别码
    GetGeetestPara = dict(user='164515366',pwd='a164515366')
    GetGeetestPara.update(rv)
    logger.info(GetGeetestPara)
    ou = JiYanGeetest(GetGeetestPara)
    if ou == False:
        logger.error('获取数据失败：%s', rv)
    logger.info('获取识别码validate和challenge成功')

    ##发送验证码
    #获取手机号码
    phone = RecvCodeGetPhone()
    challenge = ou['challenge']
    validate  = ou['validate']
    time.sleep(3)
    if RegisterSendSecurityCode(phone,challenge,validate) != True:
        logger.error('发送验证码失败')
        sys.exit()
    logger.info("发送验证码成功")

    ##提交注册
    #获取随机密码
    #rpass = RandomPasswordGetOne()
    """
    rpass = 'xmq28015'
    seccode = RecvCodeGetSecurityCode()
    RegInfo = dict(pwd=rpass,code=seccode)
    RegInfo.update(SendCodePara)
    logger.debug('注册参数：'+str(RegInfo))
    if RegisterSubmit(RegInfo) != True:
        logger.error('提交注册失败')
        sys.exit()
    logger.info("提交注册成功")
    """
