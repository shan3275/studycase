#!/usr/bin/python
#-*-coding:utf-8-*-
# FileName : Wm_Cookie_Login.py
# Author   : Adil
# DateTime : 2018/3/20 19:47
# SoftWare : PyCharm

from selenium import webdriver

import time

#url = 'http://192.168.1.13:5000/login'
url = 'https://www.douyu.com/'
#cookies = 'acf_did=5e33581fbb6a925255e20e8c00041501; acf_uid=126078070; acf_username=126078070; acf_nickname=%E8%BF%87%E5%A4%9C800%E5%85%83RMB; acf_own_room=0; acf_groupid=1; acf_phonestatus=1; acf_avatar=https%3A%2F%2Fapic.douyucdn.cn%2Fupload%2Favatar%2Fdefault%2F09_; acf_ct=0; acf_ltkid=14768355; acf_biz=1; acf_stk=5232bc6c50c7f8ac; dy_did=5e33581fbb6a925255e20e8c00041501; Hm_lvt_e99aee90ec1b2106afe7ec3b199020a7=1532482084,1532582556; Hm_lpvt_e99aee90ec1b2106afe7ec3b199020a7=1532582556'
cookies = {'acf_did':'5e33581fbb6a925255e20e8c00041501', 'acf_uid':'126078070', 'acf_username':'126078070', 'acf_nickname':'%E8%BF%87%E5%A4%9C800%E5%85%83RMB', 'acf_own_room':'0', 'acf_groupid':'1', 'acf_phonestatus':'1', 'acf_avatar':'https%3A%2F%2Fapic.douyucdn.cn%2Fupload%2Favatar%2Fdefault%2F09_', 'acf_ct':'0', 'acf_ltkid':'14768355', 'acf_biz':'1', 'acf_stk':'5232bc6c50c7f8ac', 'dy_did':'5e33581fbb6a925255e20e8c00041501', 'Hm_lvt_e99aee90ec1b2106afe7ec3b199020a7':['1532482084', '1532582556'], 'Hm_lpvt_e99aee90ec1b2106afe7ec3b199020a7':'1532582556'}

def login():
    '''先定义一个正常登录的方法，获取登录前和登录后的cookie'''

    driver = webdriver.Chrome('/Applications/chromedriver')

    driver.get(url)
    driver.maximize_window()
    cookieBefore = driver.get_cookies()
    # 打印登录前的cookie
    print(cookieBefore)
    time.sleep(2)
    driver.find_element_by_link_text("登录").click()
    time.sleep(2000)
    #driver.find_element_by_name("username").clear()
    #driver.find_element_by_name("username").send_keys("shan275")
    #driver.implicitly_wait(5)
    #driver.find_element_by_name("password").clear()
    #driver.find_element_by_name("password").send_keys("a123456")
    #driver.find_element_by_name("password").submit()
    ##driver.find_element_by_class_name('btn btn-primary btn-block btn-flat').click()
    #driver.implicitly_wait(5)
    ## 加一个休眠，这样得到的cookie 才是登录后的cookie,否则可能打印的还是登录前的cookie
    #time.sleep(5)
    #print("登录后！")
    #cookiesAfter = driver.get_cookies()
    #print("cookiesAfter:")
    #print(cookiesAfter)
    ## cookie 存放到了list,其中是dict
    ## 对比发现登录后的cookie比登录前多了4个dict。
    ## 如下代码分别是  1、4 、7、 8 
    #len1 = len(cookiesAfter)
    #print("len:%d" %len1)
    #cookie1 = cookiesAfter[0]
    #cookie2 = cookiesAfter[1]
    #print("cookie1:%s" %cookie1)
    #print("cookie2:%s" %cookie2)

    #driver.quit()
    ## 将获取的这四个cookie作为参数，传递给，使用cookie登录的函数，如下
    #cookieLogin(cookie1,cookie2)
#def cookieLogin(cookie1,cookie2):
def cookieLogin(cookie1):
    print("+++++++++++++++++++++++++")
    print("cookieLogin")
    print("cookie2:%s" % cookie1)
    driver = webdriver.Chrome('/Applications/chromedriver')
    driver.maximize_window()
    # 清除一下cookie
    driver.delete_all_cookies()
    time.sleep(3)
    driver.get(url)
    # 打开浏览器后添加访问地址后，添加cookie
    driver.add_cookie(cookie1)
    #driver.add_cookie(cookie2)
    print("cookies")
    # 打印一下cookie,与上面正常登录的cookie对比一下
    print(driver.get_cookies())
    time.sleep(5)
    # 刷新页面，可以看到已经是登录状态了，至此完成的使用cookie 的登录。
    driver.refresh()
    time.sleep(20)
    driver.quit()


if __name__ == "__main__":

    login()
    #cookieLogin(cookies)
