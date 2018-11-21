#!/usr/bin/python
#-*- coding: UTF-8 -*-
# FileName : Wm_Cookie_Login.py
# Author   : Adil
# DateTime : 2018/3/20 19:47
# SoftWare : PyCharm

from selenium import webdriver

import time

url = 'http://192.168.1.1/cgi-bin/luci'

def login():
    driver = webdriver.Chrome('/Applications/chromedriver')
    driver.get(url)
    time.sleep(2)
    driver.find_element_by_id("login_password").clear()
    driver.find_element_by_id("login_password").send_keys("w2eds")
    time.sleep(2)
    driver.find_element_by_class_name('btn').click()
    time.sleep(2)
    print("已经登陆光猫")
    return driver

def switch_window(driver, now):
    all_handles = driver.window_handles                #得到当前开启的所有窗口的句柄
    for handle in all_handles:
        if handle != now:                              #获取到与当前窗口不一样的窗口
            driver.switch_to_window(handle)            #切换

def modem_reboot(driver):
    print("准备重启光猫")
    driver.find_element_by_id('menu_action_restart_hint').click()
    #driver.refresh()
    #driver.find_element_by_class_name('pop_window_action bg_color_main').click()
    now = driver.current_window_handle
    switch_window(driver, now)
    driver.find_element_by_css_selector("[class='pop_window_action bg_color_main']").click()
    time.sleep(60)
    print("重启光猫完成")
    driver.quit()


if __name__ == "__main__":

    driver = login()
    modem_reboot(driver)
