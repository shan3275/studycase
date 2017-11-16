#!/usr/bin/python
#Filename: if.py
import requests
def send_simple_message():
    return requests.post(
        "https://api.mailgun.net/v3/mg.oncompress.com/messages",
        auth=("api", "key-6c77894e772490822d55b4e4b6651894"),
        data={"from": "Excited User <postmaster@mg.oncompress.com>",
              "to": ["shan275@163.com", "shan275@qq.com"],
              "subject": "Hello",
              "text": "Testing some Mailgun awesomness!"})

if __name__ == "__main__":
    send_simple_message()
