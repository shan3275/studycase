#!/usr/bin/python
#-*- coding: UTF-8 -*-
# FileName : DouyuLogin.py
# Author   : Shan
# DateTime : 2018/11/13
# SoftWare : PyCharm

from selenium import webdriver
from selenium.webdriver.common.by import By

import time

def login():
    url = 'https://passport.douyu.com/member/regNew?client_id=1&lang=cn'
    username = "15915492613"
    password = "abc456RST&"
    driver = webdriver.Chrome('/Applications/chromedriver')
    driver.get(url)
    driver.find_element(By.XPATH, "//input[@placeholder='请输入手机号码']").send_keys(username)
    time.sleep(0.1)
    driver.find_element(By.XPATH, "//input[@placeholder='输入密码']").send_keys(password)
    time.sleep(0.1)
    driver.find_element(By.XPATH, "//input[@value='短信验证']").click()
    time.sleep(2000)
    driver.quit()


if __name__ == "__main__":

    login()