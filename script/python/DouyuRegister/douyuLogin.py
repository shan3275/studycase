#!/usr/bin/python
#-*- coding: UTF-8 -*-
# FileName : douyuLogin.py
# Author   : Shan
# DateTime : 2018/11/28
# SoftWare : PyCharm

import sys
import time
from io import BytesIO
from PIL import Image
from selenium import webdriver
from selenium.webdriver import ActionChains
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import globalvar as gl
global logger

"""
功能：使用网页进行账号注册
"""
class DouyuLogin():
    def __init__(self,nickname,pwd):
        self.url = 'https://passport.douyu.com'
        self.browser = webdriver.Chrome('/Applications/chromedriver')
        self.wait = WebDriverWait(self.browser, 60)
        self.nickname = nickname
        self.pwd   = pwd
        self.phone = ''

    def __del__(self):
        self.browser.close()

    def open(self):
        """
        打开网页输入用户名密码
        :return: None
        """
        self.browser.get(self.url)
        logger.debug('已经打开页面:%s', self.url)
        login = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//div[@class='scanicon-toLogin js-qrcode-switch']")))
        login.click()

        nicklogin = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//span[@data-i18n='Login.Subtype.nickname' and @data-subtype='login-by-nickname']")))
        nicklogin.click()

    def inputphone(self):
        phone = self.wait.until(EC.presence_of_element_located((By.XPATH, "//input[@name='phoneNum' and @placeholder='请输入手机号码']")))
        phone.send_keys(self.phone)
        logger.debug('已输入手机号码')

    def inputpwd(self):
        pwd = self.wait.until(EC.presence_of_element_located((By.XPATH, "//input[@name='password' and @placeholder='输入密码']")))
        pwd.send_keys(self.pwd)
        logger.debug('已输入密码')

    def inputnickname(self):
        nickname = self.wait.until(EC.presence_of_element_located((By.XPATH, "//input[@name='username' and @placeholder='输入昵称']")))
        nickname.send_keys(self.nickname.decode())
        logger.debug('已输入昵称')

    def get_submit(self):
        """
        获取初始验证按钮
        :return:
        """
        #button = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//input[@data-i18n='Login' and @value='登录' and @type='submit']")))
        button = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//input[@class='loginbox-sbt btn-sub' and @value='登录' and @type='submit']")))
        return button

    def hasgeetest(self):
        """
        检查是否蹦出验证的框
        :return: True  False
        """
        logger.debug('等待2秒查询是否已经加载验证码')
        time.sleep(2)
        try:
            hadsend = self.browser.find_element(By.CLASS_NAME, "geetest_window")
            logger.debug('元素已找到，已经加载了验证码')
            return True
        except:
            logger.debug('元素未找到，没有加载验证码')
            return False

    def switch_window(self,driver, now):
        all_handles = driver.window_handles  # 得到当前开启的所有窗口的句柄
        for handle in all_handles:
            if handle != now:  # 获取到与当前窗口不一样的窗口
                driver.switch_to_window(handle)  # 切换

    def checklogin(self):
        """
        检查在验证码通过之后，登陆是否成功，
        :return:
        """
        logger.debug('等待5秒，查询是否登陆成功')
        time.sleep(5)
        self.switch_window(self.browser, self.browser.current_window_handle)
        try:
            hadsend = self.browser.find_element(By.CLASS_NAME, "l-txt")
            logger.debug('元素已找到，登陆成功')
            return True
        except:
            logger.debug('元素未找到，登陆未成功')
            return False

    def get_cookie(self):
        """
        获取cooke
        :return: cookie
        """
        cookies = self.browser.get_cookies()
        logger.info(cookies)
        i=0
        cookie_str = ''
        for cookie in cookies:
            if cookie.has_key('name') == True and cookie.has_key('value') == True:
                if i != 0:
                    cookie_str = cookie_str + '; '
                cookie_str = cookie_str + cookie['name']+'='+cookie['value']
            i = i + 1
        logger.info(cookie_str)
        return cookie_str
    def login(self):
        """
        登陆斗鱼
        :return: True False
        """
        #打开页面
        self.open()

        #输入用户名和密码
        self.inputnickname()
        self.inputpwd()
        time.sleep(3)
        submit = self.get_submit()
        submit.click()

        rv = self.hasgeetest()
        if rv == True:
            logger.debug('已经加载了验证码，需要手动验证')
            for i in range(1, 5, 1):
                rv = self.checklogin()
                if rv == True:
                    logger.info('登陆成功')
                    return True
                else:
                    logger.debug('登陆失败')
        logger.debug('未加载验证码')
        rv = self.checklogin()
        if rv == True:
            logger.info('登陆成功')
            return True
        else:
            logger.debug('登陆失败')
        return False

    def get_nickname(self):
        """
        获取登陆之后的用户名
        :return: 用户名
        """
        nickname = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, "l-txt")))
        username =  nickname.text
        logger.debug("username: " + username)
        logger.debug(type(username))
        return username


logger = gl.get_logger()
"""
if __name__ == '__main__':
    logger = gl.get_logger()
    crack = DouyuLogin('18711836842','D5fYJz70')
    rv = crack.login()
    if rv == False:
        logger.error('登陆失败')
        sys.exit()
    nickname = crack.get_nickname()
    time.sleep(60)
"""
