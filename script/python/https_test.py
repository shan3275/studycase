#!/usr/bin/python
#coding=utf-8
#Filename : addr_book.py
#Function : 通过钉钉机器人发送消息给群指定成员,测试OK
#History  : 2017/11/15     shan      V0.1

import os
import sys
import requests
import json

url = 'https://oapi.dingtalk.com/robot/send?access_token=48acc252d0a8f42ef84766d719662c4034086333e246eb01d07f2ca26874a602'
def send_warn(recv_data, send_data):
    content = "SC服务器一分钟流量警告,recv_data: %s kb, send_data: %s kb"%(recv_data, send_data)
    text = {
                "msgtype": "text",
                    "text": {
                        "content": content
                    },
                    "at": {
                        "atMobiles": [
                            "15915492613",
                            "189xxxx8325"
                        ],
                        "isAtAll": False
                    }
                }
    headers = {'content-type': 'application/json'}

    r = requests.post(url, data=json.dumps(text), headers=headers)
    print r.text

if __name__ == "__main__":
    send_warn(600,300)


'''
#get access_token
getAccessTokenPayload = {'corpid': 'ding83b5ee5f25485d98', 'corpsecret': '39YLgMCDeyClnjIyFzoDD9sUZhZX2C32LC1BJ0IxwYZ0Ta-IpMlazRZMFSkpQvmF'}
r = requests.get("https://oapi.dingtalk.com/gettoken", params=getAccessTokenPayload)
if (r.status_code == requests.codes.ok):
    issue = json.loads(r.text)
    print json.dumps(issue,ensure_ascii=False)
    access_token = issue[u'access_token']
    print access_token
else:
    sys.exit()

print ''

#get userID
getUserIdPayload = {}
getUserIdPayload['access_token'] = access_token
getUserIdPayload['mobile'] = '15915492613'
print getUserIdPayload

r = requests.get("https://oapi.dingtalk.com/user/get_by_mobile", params=getUserIdPayload)
if (r.status_code == requests.codes.ok):
    issue = json.loads(r.text)
    print issue
else:
    sys.exit()

#get department list
getListPayload = {}
getListPayload['access_token'] = access_token
print getListPayload

r = requests.get("https://oapi.dingtalk.com/department/list", params=getListPayload)
if (r.status_code == requests.codes.ok):
    issue = json.loads(r.text)
    print json.dumps(issue,ensure_ascii=False)
else:
    sys.exit()

print ''

#get department datail
getDepartmentDetailPayload = {}
getDepartmentDetailPayload['access_token'] = access_token
getDepartmentDetailPayload['id'] = '1'
print getDepartmentDetailPayload

r = requests.get("https://oapi.dingtalk.com/department/get", params=getDepartmentDetailPayload)
if (r.status_code == requests.codes.ok):
    issue = json.loads(r.text)
    print json.dumps(issue,ensure_ascii=False)
else:
    sys.exit()

print ''

#get user info
getUserInfoPayload = {}
getUserInfoPayload['access_token'] = access_token
getUserInfoPayload['userid'] = '0166483111747'
print getUserInfoPayload 

r = requests.get("https://oapi.dingtalk.com/user/get", params=getUserInfoPayload)
if (r.status_code == requests.codes.ok):
    issue = json.loads(r.text)
    print json.dumps(issue,ensure_ascii=False)
else:
    sys.exit()


'''



