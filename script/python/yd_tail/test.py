#!/usr/bin/python
#Filename:for.py
import time
import sys, os

ip = 0
for i in range(1,1000000, 1):
        ip += 1
        ip_str = "10.%d.%d.%d" %((ip>>16)&0xff, (ip>>8)&0xff, (ip>>0)&0xff)
        times = time.strftime('%Y-%m-%d %H:%M:%S')
        #url = "api.weibo.cn/2/friendships/create?trim_level=1&networktype=wifi&cardid=2302831001_0_45.3_5901359283_3159686244&invite=0&able_recommend=0&uicode=10000198&moduleID=700&featurecode=10000326&wb_version=3557&c=android&i=e3f2980&s=dcc5b47f&ft=0&ua=samsung-GT-I9300__weibo__8.1.1__android__android4.4.2&wm=2468_1001&aid=01AsQCFpPSpNq_4FHxpjLlZEs6HaOh9yt1F8wZ3XOuWteXmKU.&fid=2302835901359283&uid=3159686244&v_f=2&v_p=58&from=1081195010&gsid=_2A253dH0xDeTxGeBN41UZ-S3Nzz2IHXVSIPf5rDV6PUJbkdANLUX1kWpNRDQNKXsxRoSwjz9OGi5PC2Map7qtkAzS&imei=867075010921975&lang=zh_CN&lfid=100016387893141&skin=default&trim=1&oldwm=2468_1001&sflag=1&luicode=10000001"
        #url = "api.weibo.cn/2/friendships/create?trim_level=1&networktype=wifi&cardid=2302831001_0_45.3_5901359283_3159686244&invite=0&able_recommend=0&uicode=10000198&moduleID=700&featurecode=10000326&wb_version=3557&c=android&i=e3f2980&s=dcc5b47f&ft=0&ua=samsung-GT-I9300__weibo__8.1.1__android__android4.4.2&wm=2468_1001&aid=01AsQCFpPSpNq_4FHxpjLlZEs6HaOh9yt1F8wZ3XOuWteXmKU.&fid=2302835901359283&uid=&v_f=2&v_p=58&from=&gsid=_2A253dH0xDeTxGeBN41UZ-S3Nzz2IHXVSIPf5rDV6PUJbkdANLUX1kWpNRDQNKXsxRoSwjz9OGi5PC2Map7qtkAzS&imei=867075010921975&lang=zh_CN&lfid=100016387893141&skin=default&trim=1&oldwm=2468_1001&sflag=1&luicode=10000001"
        url ="api.weibo.cn/2/profile/dealatt?trim_level=1&networktype=wifi&cardid=0001980001_1892463935&invite=0&able_recommend=0&uicode=10000198&moduleID=700&featurecode=10000001&wb_version=3557&c=android&i=e3f2980&s=dcc5b47f&ft=0&ua=samsung-GT-I9300__weibo__8.1.1__android__android4.4.2&wm=2468_1001&aid=01AasQCFpPSpNq_4FHxpjLlZEs6HaOh9yt1F8wZ3XOuWteXmKU.&uid=1892463935&v_f=2&v_p=58&from=1081195010&gsid=_2A253dH0xDeTxGeBN41UZ-S3Nzz2IHXVSIPf5rDV6PUJbkdANLUX1kWpNRDQNKXsxRoSwjz9OGi5PC2Map7qtkAzS&imei=867075010921975&uid=2371980&lang=zh_CN&lfid=100016387893141&skin=default&trim=1&oldwm=2468_1001&sflag=1&luicode=10000001"
        other = "|    Mozilla/5.0 (Linux; U; Android 7.1.1; zh-CN; OPPO R11t Build/NMF26X) AppleWebKit/537.3    6 (KHTML, like Gecko) Version/4.0 Chrome/57.0.2987.108 UCBrowser/11.9.3.973 Mobile Saf    ari/537.36|https://so.m.sm.cn/s?q=%E8%A5%BF%E8%A5%BF%E7%89%B9%E4%BD%9C%E5%93%81%E9%9B%    86txt%E7%99%BE%E5%BA%A6%E4%BA%91&uc_param_str=dnntnwvepffrgibijbprsvdsme&from=wh20000&    uc_sm=1"
        str = '%s|ZEBRA: |GET|%s|101.206.162.223|39144|80|2534350363|3310829935|%s%s\n' %(times,ip_str,url,other)
        print str
        time.sleep(1)
        f = open("naga.log"+time.strftime('%Y%m%d'), 'a')
        f.write(str)
        f.flush()
        f.close()
else:
	print 'The for loop is over'
