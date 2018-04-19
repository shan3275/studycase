# -*- coding: utf-8 -*-  
import random, base64  
from hashlib import sha1  
  
def crypt(data, key):  
    """RC4 algorithm"""  
    x = 0  
    box = range(256)  
    for i in range(256):  
        x = (x + box[i] + ord(key[i % len(key)])) % 256  
        box[i], box[x] = box[x], box[i]  
    x = y = 0  
    out = []  
    for char in data:  
        x = (x + 1) % 256  
        y = (y + box[x]) % 256  
        box[x], box[y] = box[y], box[x]  
        out.append(chr(ord(char) ^ box[(box[x] + box[y]) % 256]))  
  
    return ''.join(out)  
  
def tencode(data, key, encode=base64.b64encode, salt_length=16):  
    """RC4 encryption with random salt and final encoding"""  
    salt = ''  
    for n in range(salt_length):  
        salt += chr(random.randrange(256))  
    data = salt + crypt(data, sha1(key + salt).digest())  
    if encode:  
        data = encode(data)  
    return data  
  
def tdecode(data, key, decode=base64.b64decode, salt_length=16):  
    """RC4 decryption of encoded data"""  
    if decode:  
        data = decode(data)  
    salt = data[:salt_length]  
    return crypt(data[salt_length:], sha1(key + salt).digest())  
  
  
if __name__=='__main__':  
    # 需要加密的数据  
    data = 'hello python 爱的实打实大师大师大师的发送到发送到'  
    # 密钥  
    key = '123456'  
  
    # 加码  
    encoded_data = tencode(data=data, key=key)  
    print encoded_data  
    encoded_data = 'U2FsdGVkX1+PZINaw/qokNCF2TVjRndc1KGdSeUa3xFqA5SXCYgTJN5ajEKjjGqUAtF8EEYW+8oeqE889SJPv+BxBrNXHZWQEvhy98iAokqBJedw+I+PSHXfH5Qb2XCeLyxMovNJJ+pn40DaG2fWmoi+rbKw2hfdv6261Z+sZcgEP9Yrm0CyoB5ugXspDvLK9sV/sNuLClg5mOpzLJh0OQgnL9VnHInPTMhcKAYmIRGc0HzV9RzIbXNmixR3ShEZBHB75K/STbf8+CSxVQdUWWpa7R4qMLpeXr10uPG5tH8WDE24D1RvCLmfbPur+QJztdUNO9SWYgYuZFu2jYd+d3kplcsPHA2LO6UDuM4EAg7Mtjb9'
    # 解码  
    decoded_data = tdecode(data=encoded_data, key='dc')  
    print decoded_data  
