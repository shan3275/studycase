#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys,os,hashlib,time,base64
import binascii

class RC4:
    def __init__(self, keyStr, keyLen=32):
        self.keyLen = keyLen
        self.keyStr = keyStr or 'none_key'
        key = hashlib.md5(self.keyStr).hexdigest()   #32位
        self.keya = hashlib.md5(key[0:16]).hexdigest()
        self.keyb = hashlib.md5(key[16:32]).hexdigest()
        self.keyc = ''

    #加密
    def encrypt(self, data):
        self.keyc = hashlib.md5(str(time.time())).hexdigest()[(32-self.keyLen):32]
        data = '0000000000' + hashlib.md5(data + self.keyb).hexdigest()[0:16] + data
        result = self.rc4Algorithm(data)
        return self.keyc + base64.b64encode(result)

    #解密
    def decrypt(self, data):
        self.keyc = data[0:self.keyLen]
        data = base64.b64decode(data[self.keyLen:])
        result = self.rc4Algorithm(data)
        if (result[0:10] == '0000000000' or int(result[0:10]) - int(time.time()) > 0) and result[10:26] == hashlib.md5(result[26:] + self.keyb).hexdigest()[0:16]:
            return result[26:]
        else:
            return None

    #rc4算法实现
    def rc4Algorithm(self, data):
        dataLen = len(data)
        result = ''
        box = list(range(256))
        randKey = []
        cryptKey = self.keya + hashlib.md5(self.keya + self.keyc).hexdigest()
        keyLen = len(cryptKey)
        for i in xrange(255):
            randKey.append(ord(cryptKey[i % keyLen]))

        for i in xrange(255):
            j = 0
            j = (j + box[i] + randKey[i]) % 256
            tmp = box[i]
            box[i] = box[j]
            box[j] = tmp

        for i in xrange(dataLen):
            a = j = 0
            a = (a + 1) % 256
            j = (j + box[a]) % 256
            tmp = box[a]
            box[a] = box[j]
            box[j] = tmp
            result += chr(ord(data[i]) ^ (box[(box[a] + box[j]) % 256]))
        return  result

#RC4测试
if __name__=='__main__':
    rc4 = RC4('dc')
    data = 'hello script, i love python and lua'
    print data
    str = rc4.encrypt(data)
    print binascii.b2a_hex(str)
    print str
    str = rc4.decrypt(str)
    print str


# hello script, i love python and lua
# e59f7b8e4271a596+DJzDKYfCh9tbh2cP22cMaGDFvGntU58QdR71Rd4dW4BwayP0g8E0IF2PcMPNz9zo9tmi8NhwyZwxSJj4Q==
# hello script, i love python and lua

