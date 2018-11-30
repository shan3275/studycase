#!/usr/bin/python
#-*- coding: UTF-8 -*-
# FileName : douyuRegister.py
# Author   : Shan
# DateTime : 2018/11/28
# SoftWare : PyCharm

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
class DouyuRegister():
    def __init__(self,phone,pwd):
        self.url = 'https://passport.douyu.com/member/regNew?client_id=1&lang=cn'
        self.browser = webdriver.Chrome('/Applications/chromedriver')
        self.wait = WebDriverWait(self.browser, 60)
        self.phone = phone
        self.pwd   = pwd
        self.code  = ''

    def __del__(self):
        self.browser.close()

    def get_geetest_button(self):
        """
        获取初始验证按钮
        :return:
        """
        button = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//input[@type='button' and @value='短信验证']")))
        return button

    def open(self):
        """
        打开网页输入用户名密码
        :return: None
        """
        self.browser.get(self.url)
        logger.debug('已经打开页面:%s', self.url)
        phone = self.wait.until(EC.presence_of_element_located((By.XPATH, "//input[@name='phoneNum' and @placeholder='请输入手机号码']")))
        pwd = self.wait.until(EC.presence_of_element_located((By.XPATH, "//input[@name='password' and @placeholder='输入密码']")))
        phone.send_keys(self.phone)
        pwd.send_keys(self.pwd)
        logger.debug('已经输入手机号和密码：%s，%s', self.phone,self.pwd)

    def login(self):
        """
        登录
        :return: None
        """
        submit = self.wait.until(EC.element_to_be_clickable((By.CLASS_NAME, 'login-btn')))
        submit.click()
        time.sleep(10)
        print('登录成功')

    def switch_window(self,driver, now):
        all_handles = driver.window_handles  # 得到当前开启的所有窗口的句柄
        for handle in all_handles:
            if handle != now:  # 获取到与当前窗口不一样的窗口
                driver.switch_to_window(handle)  # 切换

    def goonreg(self):
        """
        继续注册
        :return: None
        """
        logger.debug('等待2秒查询是否出现 继续注册 按钮')
        time.sleep(2)
        now = self.browser.current_window_handle
        self.switch_window(self.browser, now)
        #reg = self.wait.until(EC.element_to_be_clickable((By.CLASS_NAME, 'aui_state_highlight')))
        try:
            reg = self.browser.find_element(By.CLASS_NAME, 'aui_state_highlight')
            logger.debug('找到元素，继续注册按钮，准备点击')
            reg.click()
            now = self.browser.current_window_handle
            self.switch_window(self.browser, now)
            return True
        except:
            logger.debug('元素未找到，没有找到继续注册按钮')
            now = self.browser.current_window_handle
            self.switch_window(self.browser, now)
            return False

    def hadsend(self):
        logger.debug('等待2秒查询是否已经发送短信')
        time.sleep(2)
        try:
            hadsend = self.browser.find_element_by_xpath("//input[@class='phone-send js-sendvoice fl long']")
            logger.debug('元素已找到，已发送短信')
            return True
        except:
            logger.debug('元素未找到，没有发送短信')
            return False

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
    def EnsureSendCode(self):
        """
        检查验证码验证是否完成
        :return:
        """
        # 如果出现继续注册
        rv = self.goonreg()
        if rv == True:
            logger.info('点击继续注册成功')
            return True

        rv = self.hadsend()
        if rv == True:
            logger.info('短信已经发送，准备接收短信')
            return True
        logger.info('短信未发送，需要继续')
        logger.error('检查短信未发送')
        return False
    def sendcode(self):
        # 输入用户名密码
        self.open()
        # 点击验证按钮
        time.sleep(2)
        button = self.get_geetest_button()
        button.click()
        logger.debug('已经点击短信验证')
        #查询是否出现验证码
        rv = self.hasgeetest()
        if rv == True:
            logger.debug('已经加载了验证码，需要手动验证')
            #检测
            for i in range(1, 5, 1):
                logger.debug('循环次数：%d',i)
                rv = self.EnsureSendCode()
                if rv == True:
                    logger.info('验证码 验证通过')
                    break
                else:
                    logger.debug('验证失败')
        logger.debug('未加载验证码')
        #检测
        rv = self.EnsureSendCode()
        if rv == True:
            logger.info('验证通过')
            rv = self.hadsend()
            if rv == True:
                logger.info('短信已经发送，准备接收短信')
            return True
        return False

    def setcode(self,code):
        self.code = code
        logger.debug('收到验证码为：'+code)

    def inputcode(self):
        code = self.wait.until(EC.presence_of_element_located((By.XPATH, "//input[@name='phoneCaptcha' and @placeholder='输入验证码']")))
        code.send_keys(self.code)
        logger.debug('已输入验证码')

    def checklogin(self):
        """
        检查在验证码通过之后，登陆是否成功，
        :return:
        """
        logger.debug('等待3秒，查询注册登陆成功')
        time.sleep(3)
        self.switch_window(self.browser, self.browser.current_window_handle)
        try:
            hadsend = self.browser.find_element(By.CLASS_NAME, "l-txt")
            logger.debug('元素已找到，登陆成功')
            return True
        except:
            logger.debug('元素未找到，登陆未成功')
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

    def register(self):
        #输入验证码
        self.inputcode()

        #点击立即注册
        time.sleep(1)
        submit = self.wait.until(EC.element_to_be_clickable((By.XPATH, "//input[@data-i18n='Register' and @value='立即注册']")))
        submit.click()

        #页面跳转到登陆页面
        rv = self.checklogin()
        if rv == True:
            logger.info('注册成功')
            return True
        else:
            logger.debug('注册失败')
        return False


logger = gl.get_logger()

"""
if __name__ == '__main__':
    logger = gl.get_logger()
    crack = DouyuRegister('13658102348','aDdeS3aBC')
    rv= crack.sendcode()
    if rv == True:
        logger.info('验证码发送成功，准备接收验证码')
    crack.setcode('456723')
    crack.register()
    time.sleep(30)
"""

