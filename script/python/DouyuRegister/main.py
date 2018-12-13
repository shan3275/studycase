#!/usr/bin/python
#-*- coding: UTF-8 -*-
# FileName : main.py
# Author   : Shan
# DateTime : 2018/11/14
# SoftWare : PyCharm

import re
import random,string
import sys
from urllib import urlencode
from urllib import unquote
from cmd import Cmd
import time
import inits
import douyu as douyu
import jiema as jiema
import jiyan as jiyan
import globalvar as gl
import airtel    as airtel
import douyuRegister as douyuRegister
import douyuLogin    as douyuLogin
import jsyzm         as jsyzm

global CONF
global logger
Version = "V0.1.38.c2cd367-2018-12-12"
"""
功能：判断手机号码是否有效
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
功能：打印返回结果
输入参数：
        ou  ：字典，包含信息
           ou['data']['challenge']  :challenge信息
           ou['data']['validate']   :validate信息
           ou['msg']                :信息
           ou['error']              : 0 ok
                                    : 1 手机号码无效
                                    : 2 获取gt和challenge失败
                                    : 3 极验获取challenge和validate失败
                                    : 4 验证码发送失败
输出参数：ou_str ：字符串
"""

"""
功能: 发送验证码
输入参数：phone ：手机号码
输出参数：ou  ：字典，包含信息
           ou['data']['challenge']  :challenge信息
           ou['data']['validate']   :validate信息
           ou['msg']                :信息
           ou['error']              : 0 ok
                                    : 1 手机号码无效
                                    : 2 获取gt和challenge失败
                                    : 3 极验获取challenge和validate失败
                                    : 4 验证码发送失败
"""
def SendCode(phone):
    ou=dict(error=0,data=dict(),msg='ok')
    if PhoneJudge(phone) == False:
        ou['error'] = 1
        ou['msg']   = '手机号码无效'
        return ou
        ##获取gt和challenge
    gt = douyu.RegisterGetChallenge()
    if gt == False:
        ou['error'] = 2
        ou['msg']   = '获取gt和challenge失败'
        return ou
    GetGeetestPara = dict()
    GetGeetestPara.update(gt)
    GetGeetestPara.update(CONF['jiyan'])
    logger.debug(GetGeetestPara)
    jiyanres = jiyan.JiYanGeetest(GetGeetestPara)
    if jiyanres == False:
        ou['error'] = 3
        ou['msg']   = '极验获取challenge和validate失败'
        return ou
    challenge = jiyanres['challenge']
    validate = jiyanres['validate']
    rv = douyu.RegisterSendSecurityCode(phone, challenge, validate)
    if rv == 0:
        ou['error'] = 0
        ou['msg']   = '验证码发送成功，请及时接收'
        ou['data']['challenge'] = challenge
        ou['data']['validate']  = validate
        return ou
    if rv != 2:
        ou['error'] = 4
        ou['msg']   = '验证码发送失败'
        return
    ## rv == 2的情况
    ##获取gt和challenge
    gt = douyu.RegisterGetChallenge()
    if gt == False:
        ou['error'] = 2
        ou['msg']   = '获取gt和challenge失败'
        return ou
    GetGeetestPara = dict()
    GetGeetestPara.update(gt)
    GetGeetestPara.update(CONF['jiyan'])
    logger.debug(GetGeetestPara)
    jiyanres = jiyan.JiYanGeetest(GetGeetestPara)
    if jiyanres == False:
        ou['error'] = 3
        ou['msg'] = '极验获取challenge和validate失败'
        return ou
    challenge = jiyanres['challenge']
    validate = jiyanres['validate']
    rv = douyu.RegisterSecondSendSecurityCode(phone, challenge, validate)
    if rv != 0:
        ou['error'] = 4
        ou['msg']   = '验证码再次发送失败'
        ou['data'].clear()
        return ou

    ou['data'].clear()
    ou['error'] = 0
    ou['msg'] = '验证码发送成功，请及时接收'
    ou['data']['challenge'] = challenge
    ou['data']['validate']  = validate
    return ou

"""
功能：注册一个账号，使用api方式，返回账号信息
输入产生：无
输出参数：ou：字典，包含账号信息
            ou['data']['nickname'] : 用户名
            ou['data']['pwd']      : 密码
            ou['data']['phone']    : 绑定的手机号码
            ou['msg']      : 信息
            ou['error']             : 0 ok
                                    : 1 手机号码无效
                                    : 2 获取gt和challenge失败
                                    : 3 极验获取challenge和validate失败
                                    : 4 验证码发送失败
                                    : 5 获取手机号码失败
                                    : 6 获取验证码失败
                                    : 7 注册提交失败
"""
def RegisterOneAccount():
    ou = dict(error=0,data=dict(),msg='ok')
    ##注册前 重启modem
    airtel.AirtelReboot()

    ##第一步 获取手机号码
    phone = jiema.RecvCodeGetPhone()
    if phone == False:
        ou['error'] = 5
        ou['msg']   = '获取手机号码失败'
        return ou
    print('phone: ' + phone)

    ##第二步 发送验证码
    ou = SendCode(phone)
    if ou['error'] != 0:
        return ou

    time.sleep(20)
    ##第三步 接收验证码
    code = jiema.RecvCodeGetCode(phone)
    if code == False:
        ou['error'] = 6
        ou['msg']   = '获取验证码失败'
        ou['data'].clear()
        return ou

    ##第四步 注册
    #产生随机密码
    pwd = RandomPasswordGetOne()
    #第二步返回结果中的数值
    challenge = ou['data']['challenge']
    validate  = ou['data']['validate']
    ou['data'].clear()
    logger.debug('phone:%s,code:%s,pwd:%s,challenge:%s,validate:%s',phone,code,pwd,challenge,validate)
    msg = douyu.RegisterSubmit(phone,code,pwd,challenge,validate)
    if msg == False:
        ou['error'] = 7
        ou['msg']   = '注册提交失败'
        ou['data'].clear()
        return ou
    ou['error'] = 0
    ou['msg']   = '提交注册成功'
    ou['data']['nickname']  = msg['nickname']
    ou['data']['pwd']       = pwd
    ou['data']['phone']     = phone
    logger.debug("提交注册成功")
    logger.debug('nickname:%s,phone:%s, pwd:%s', msg['nickname'], phone, pwd)

    return ou


"""
功能：注册一个账号，使用浏览器模式，人工参与，返回账号信息
输入产生：无
输出参数：ou：字典，包含账号信息
            ou['data']['nickname'] : 用户名
            ou['data']['pwd']      : 密码
            ou['data']['phone']    : 绑定的手机号码
            ou['msg']      : 信息
            ou['error']             : 0 ok
                                    : 1 手机号码无效
                                    : 2 获取gt和challenge失败
                                    : 3 极验获取challenge和validate失败
                                    : 4 验证码发送失败
                                    : 5 获取手机号码失败
                                    : 6 获取验证码失败
                                    : 7 注册提交失败
"""
def RegisterOneAccountV5():
    ou = dict(error=0,data=dict(),msg='ok')
    ##注册前 重启modem
    airtel.AirtelReboot()

    ##第一步 获取手机号码
    if CONF['useyzm'] == True:
        jm = jsyzm.JSJM()
        ou = jm.getPhone()
        if ou['error'] != '0':
            logger.error('获取号码失败，error=%s,msg=%s', ou['error'], ou['msg'])
            return ou
        phone = ou['data']['phone']
    else:
        phone = jiema.RecvCodeGetPhone()
        if phone == False:
            ou['error'] = 5
            ou['msg']   = '获取手机号码失败'
            return ou
    #print('phone: ' + phone)

    ##第二步 产生随机密码
    pwd = RandomPasswordGetOne()

    ##第三步 发送验证码
    crack = douyuRegister.DouyuRegister(phone, pwd)
    rv= crack.sendcode()
    if rv == True:
        logger.debug('验证码发送成功，准备接收验证码')
    else:
        ou['error'] = 4
        ou['msg']   = '验证码发送失败'
        return ou

    time.sleep(20)
    ##第三步 接收验证码
    if CONF['useyzm'] == True:
        ou = jm.getCode(phone)
        if ou['error'] != '0':
            logger.error('获取短信失败，error=' + ou['error'] + ' msg=' + ou['msg'])
            return ou
        code = ou['data']['code']
    else:
        code = jiema.RecvCodeGetCode(phone)
        if code == False:
            ou['error'] = 6
            ou['msg']   = '获取验证码失败'
            ou['data'].clear()
            return ou

    ##第四步 设置验证码
    crack.setcode(code)

    #第五步 提交注册
    ou['data'].clear()
    logger.debug('phone:%s,code:%s,pwd:%s',phone,code,pwd)
    rv = crack.register()
    if rv == False:
        ou['error'] = 7
        ou['msg']   = '注册提交失败'
        ou['data'].clear()
        return ou

    ##第六步 获取nickname
    nickname = crack.get_nickname()
    ##第七步 获取cookie
    cookie   = crack.get_cookie()
    ou['error'] = 0
    ou['msg'] = '注册成功'
    ou['data']['nickname'] = nickname
    ou['data']['phone']    = phone
    ou['data']['pwd']      = pwd
    ou['data']['cookie']   = cookie

    ##第八步 获取站内信
    validate = crack.get_umes()
    ou['data']['validate'] = validate
    logger.debug("注册成功")
    logger.debug('nickname:%s,phone:%s, pwd:%s, validate:%s, cooike:%s', nickname, phone, pwd, format(validate,""), cookie)

    return ou

"""
功能：使用浏览器模拟登陆，获取用户名
输入参数：nickname ：用户名
         pwd   ：密码
输出参数：ou：字典，包含账号信息
            ou['data']['nickname'] : 用户名
            ou['data']['pwd']      : 密码
            ou['data']['phone']    : 绑定的手机号码
            ou['msg']      : 信息
            ou['error']             : 0 ok
                                    : 1 手机号码无效
                                    : 2 获取gt和challenge失败
                                    : 3 极验获取challenge和validate失败
                                    : 4 验证码发送失败
                                    : 5 获取手机号码失败
                                    : 6 获取验证码失败
                                    : 7 注册提交失败
                                    : 8 登陆失败
                                    
"""
def LoginOneAccountV5(nickname, pwd):
    ou = dict(error=0, data=dict(), msg='ok')
    crack = douyuLogin.DouyuLogin(nickname,pwd)
    rv = crack.login()
    if rv == False:
        logger.error('登陆失败')
        ou['error'] = 8
        ou['msg'] = '登陆失败'
        return ou
    cookie = crack.get_cookie()
    validate = crack.get_umes()
    ou['error'] = 0
    ou['msg'] = '登陆成功'
    ou['data']['nickname'] = nickname
    ou['data']['pwd']      = pwd
    ou['data']['cookie']   = cookie
    ou['data']['validate'] = validate
    return ou

def RegisterAndLoginOneAccount():
    """
    功能：使用浏览器模拟登陆，获取用户名
    输入参数：无
    输出参数：ou：字典，包含账号信息
                ou['data']['nickname'] : 用户名
                ou['data']['pwd']      : 密码
                ou['data']['phone']    : 绑定的手机号码
                ou['msg']      : 信息
                ou['error']             : 0 ok
                                        : 1 手机号码无效
                                        : 2 获取gt和challenge失败
                                        : 3 极验获取challenge和validate失败
                                        : 4 验证码发送失败
                                        : 5 获取手机号码失败
                                        : 6 获取验证码失败
                                        : 7 注册提交失败
                                        : 8 登陆失败
    """
    ou = RegisterOneAccountV5()
    if ou['error'] == 0:
        logger.debug('注册成功，手机号：%s，密码：%s' ,ou['data']['phone'], ou['data']['pwd'])
    else:
        logger.debug('注册失败：%s', ou['msg'])
        return ou

    nickname = ou['data']['nickname']
    phone    = ou['data']['phone']
    pwd      = ou['data']['pwd']
    ou = LoginOneAccountV5(nickname, pwd)
    if ou['error'] == 0:
        logger.debug("登陆成功，昵称：%s 手机号：%s 密码：%s", nickname, phone, pwd)
        ou['data']['phone'] = phone
    else:
        logger.debug('登陆失败：%s', ou['msg'])
    return ou

def SaveAccountToFile(line):
    """
    功能：写行内容到文件中
    :param line: 账号内容
    :return: 无
    """
    if CONF['acc'] == '':
        logger.error('输出账号文件为空')
        return
    f = open(CONF['acc'], 'a+')
    f.writelines(line + '\n')
    f.close()
    logger.debug('账号写入文件：%s',line)

def RegisterAccount(num):
    """
    功能：注册多个账号，并记录日志文件
    :param num:
    :return:
    """
    success = 0
    fail    = 0
    while success < num:
        result_str = '需求数量：' +str(num) + ' 已注册成功：' + str(success) + '失败数量：' + str(fail)
        sys.stdout.write("\r{0}".format(result_str))
        sys.stdout.flush()
        ou = RegisterOneAccountV5()
        if ou['error'] != 0:
            fail = fail + 1
            continue
        #成功
        acc_str =  ou['data']['nickname']  + "|" + ou['data']['pwd']+ "|" + format(ou['data']['validate'],"d")+"|"+ou['data']['cookie']
        SaveAccountToFile(acc_str)
        #自加一
        success = success +1
    else:
        result_str = '需求数量：' +str(num) + ' 已注册成功：' + str(success) + '失败数量：' + str(fail)
        sys.stdout.write("\r{0}".format(result_str))
        sys.stdout.flush()
        print('\n')

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

    #版本号
    def do_version(self,arg):
        print(Version)
    def help_version(self):
        print("显示版本号")

    ##获取本机的IP地址，内部调试使用
    def do_ip(self,line):
        ip = jiema.GetCurrentIP()
        if ip == False:
            print('获取本机IP失败')
            return
        print("客户端本机IP获取成功:"+ip)
    def help_ip(self):
        print '获取本机公网IP地址'

    ##获取本机的地理位置，省份和城市
    def do_location(self,line):
        ip = jiema.GetCurrentIP()
        if ip == False:
            print('获取本机IP失败')
            return
        logger.debug("客户端本机IP获取成功")
        location = jiema.GetCurrentLocation(ip)
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
        if CONF['useyzm'] == True:
            #use jsyzm
            jm = jsyzm.JSJM()
            token = jm.token
        else:
            user = CONF['jiema']['user']
            pwd  = CONF['jiema']['pwd']
            token = jiema.JieMaLogin(user, pwd)
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
        token = jiema.JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        points = jiema.JieMaetGetUserPoints(user, token)
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
        token = jiema.JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        info = jiema.JieMaetGetRecvInfo(user, token, '0')
        if info == False:
            print('接码平台获取信息失败')
            return
        print info
    def help_jminfos(self):
        print('打印接码平台获取的信息')

    ##获取接码平台手机号码
    def do_getphone(self,line):
        if CONF['useyzm'] == True:
            jm = jsyzm.JSJM()
            ou = jm.getPhone()
            if ou['error'] != '0':
                print('获取号码失败，error=%s,msg=%s', ou['error'], ou['msg'])
                return
            phone = ou['data']['phone']
        else:
            phone = jiema.RecvCodeGetPhone()
            if phone == False:
                print('获取手机号码失败')
                return
        print('phone: '+phone)
    def help_getphone(self):
        print('获取接码平台手机号码')

    ##释放接码平台手机号码
    def do_releasephone(self,line):
        phone = line
        if CONF['useyzm'] == True:
            jm = jsyzm.JSJM()
            ou = jm.releasePhone(phone)
            if ou['error'] != '0':
                print('获取号码失败，error=%s,msg=%s', ou['error'], ou['msg'])
                return
        else:
            print('不用释放号码')
            return
        print('释放号码 %s 成功',phone)
    def help_releasephone(self):
        print('释放单个手机号码, 需要输入参数')

    ##释放接码平台手机号码
    def do_releaseallphone(self, line):
        phone = line
        if CONF['useyzm'] == True:
            jm = jsyzm.JSJM()
            ou = jm.releaseAllPhone()
            if ou['error'] != '0':
                print('获取号码失败，error=%s,msg=%s', ou['error'], ou['msg'])
                return
        else:
            print('不用释放号码')
            return
        print('释放号码全部号码成功')

    def help_releaseallphone(self):
        print('释放全部获取的手机号码')


    ##接码平台加黑无用号码
    def do_jmignorephone(self,phone):
        user = CONF['jiema']['user']
        pwd = CONF['jiema']['pwd']
        token = jiema.JieMaLogin(user, pwd)
        if token == False:
            print('接码平台Token获取失败')
            return
        num = jiema.JieMaAddIgnoreList(user, token, phone)
        if num == False:
            print('加黑无用号码失败')
            return
        print('加黑号码成功数：',num)
    def help_jmignorephone(self):
        print('接码平台加黑无用号码')

    ##接码平台获取验证码
    def do_getcode(self,phone):
        if phone == '':
            print('请输入手机号码作为参数')
            return
        if CONF['useyzm'] == True:
            jm = jsyzm.JSJM()
            ou = jm.getCode(phone)
            if ou['error'] != '0':
                print('获取短信失败，error='+ou['error'] +' msg='+ou['msg'])
                return
            code = ou['data']['code']
        else:
            code = jiema.RecvCodeGetCode(phone)
            if code == False:
                print('获取验证码失败')
                return
        print('接码平台验证码获取成功, '+phone+' '+code)
    def help_getcode(self):
        print('接码平台获取验证码,执行命令时，需输入参数：手机号码')

    ##注册账号签获取gt和challenge
    def do_reggetgt(self,line):
        ##获取gt和challenge
        gt = douyu.RegisterGetChallenge()
        if gt == False:
            print('获取数据失败')
            return
        print('获取gt和challenge数据成功')
        for key, value in gt.items():
            print('{key}:{value}'.format(key=key, value=value))
    def help_reggetgt(self):
        print('注册账号签获取gt和challenge')

    ##极验平台获取极验码
    def do_jyvalidate(self,line):
        ##获取gt和challenge
        gt = douyu.RegisterGetChallenge()
        if gt == False:
            print('获取数据失败')
            return
        GetGeetestPara = dict()
        GetGeetestPara.update(gt)
        GetGeetestPara.update(CONF['jiyan'])
        logger.debug(GetGeetestPara)
        ou = jiyan.JiYanGeetest(GetGeetestPara)
        if ou == False:
            print('获取数据失败')
            return
        print('获取识别码validate和challenge成功')
        print(ou)
    def help_jyvalidate(self):
        print('极验平台获取极验码')

    ##发送验证码
    def do_sendcode(self,phone):
        ou = SendCode(phone)
        print(ou)
        if ou['error'] == 0:
            CONF['tmp']['challenge'] = ou['data']['challenge']
            CONF['tmp']['validate']  = ou['data']['validate']
            print('验证码发送成功')
        print 'msg: %s' %(ou['msg'])

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
        challenge = CONF['tmp']['challenge']
        validate  = CONF['tmp']['validate']
        print 'phone:%s,code:%s,pwd:%s,challenge:%s,validate:%s'  %(phone,code,pwd,challenge,validate)
        msg = douyu.RegisterSubmit(phone,code,pwd,challenge,validate)
        if msg == False:
            print('提交注册失败')
            return
        print("提交注册成功")
        print 'nickname:%s,phone:%s, pwd:%s' %(msg['nickname'], phone, pwd)
    def help_regsubmit(self):
        print('提交注册，需要输入参数：手机号码 验证码')

    ##重启4G无线网卡
    def do_modemreboot(self,line):
        airtel.AirtelReboot()
    def help_modemreboot(self):
        print('重启4G无线网卡')

    ##注册账号
    def do_regoneacc(self,line):
        ou = RegisterOneAccount()
        print(ou)
        if ou['error'] == 0:
            print '注册成功，昵称：%s，手机号：%s，密码：%s' %(ou['data']['nickname'], ou['data']['phone'], ou['data']['pwd'])
        else:
            print '注册失败：%s' %(ou['msg'])
    def help_regoneacc(self):
        print('注册一个账号')

    ##最新版本中获取gt和challenge值（code_data_id,code_token,code_type）
    def do_v4reggetgt(self,line):
        ou = douyu.CheckGeeTest()
        if ou == False:
            print('获取数据失败')
            return
        print('获取gt和code_token数据成功')
        for key, value in ou.items():
            print('{key}:{value}'.format(key=key, value=value))

    def help_v4reggetgt(self):
        print('最新版本中获取gt和challenge值（code_data_id,code_token,code_type）')

    ##极验平台获取极验码
    def do_v4jyvalidate(self,line):
        ##获取gt和challenge
        gt = douyu.CheckGeeTest()
        if gt == False:
            print('获取数据失败')
            return
        browserinfo = douyu.RegisterGetBrowserInfo()
        ou = jiyan.JiYanDeepKnowGeetest(CONF['jiyan']['user'],CONF['jiyan']['pwd'],gt['code_data_id'],browserinfo)
        if ou == False:
            print('获取数据失败')
            return
        print('获取识别码validate和challenge成功')
        print(ou)
    def help_v4jyvalidate(self):
        print('极验平台获取极验码')

    def do_v5regoneacc(self,line):
        ou = RegisterOneAccountV5()
        if ou['error'] == 0:
            print "登陆成功，昵称："+ou['data']['nickname']+" 手机号："+ou['data']['phone']+" 密码："+ou['data']['pwd']
        else:
            print '登陆失败：%s' %(ou['msg'])
    def help_v5regoneacc(self):
        print('使用浏览器模拟，注册一个账号,并登陆')

    def do_v5loginoneacc(self, line):
        if line == '':
            print '请输入参数：nickname pwd'
            return
        arg = line.split()
        nickname = arg[0]
        pwd  = arg[1]
        ou = LoginOneAccountV5(nickname, pwd)
        if ou['error'] == 0:
            print "登陆成功，昵称：" +nickname + " 密码：" +pwd + ' cookie:' + ou['data']['cookie']
            print "账号有效：" + format(ou['data']['validate'],"")
        else:
            print '登陆失败：%s' % (ou['msg'])
            return
    def help_v5loginoneacc(self):
        print('使用浏览器，模拟登陆，获取用户名,输入参数： nickname  password')

    def do_v5regacc(self,line):
        if line == '':
            print '请输入参数：num'
            return
        num = int(line)
        RegisterAccount(num)
    def help_v5regacc(self):
        print('批量注册账号，并保存文件，需要输入注册数量，参数：num')

if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf8')
    logger = gl.get_logger()
    CONF   = gl.get_conf()
    cli = Cli()
    cli.cmdloop()
