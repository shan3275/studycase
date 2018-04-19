#/usr/bin/python
#coding=utf-8
import sys,os,hashlib,time,base64
class rc4:
    
    def __init__(self,public_key = None,ckey_lenth = 16):
        self.ckey_lenth = ckey_lenth
        self.public_key = public_key or 'none_public_key'
        key = hashlib.md5(self.public_key).hexdigest()
        self.keya = hashlib.md5(key[0:16]).hexdigest()
        self.keyb = hashlib.md5(key[16:32]).hexdigest()
        self.keyc = ''
    
    def encode(self,string):
        self.keyc = hashlib.md5(str(time.time())).hexdigest()[32 - self.ckey_lenth:32]
        string = '0000000000' + hashlib.md5(string + self.keyb).hexdigest()[0:16] + string
        self.result = ''
        self.docrypt(string)
        return self.keyc + base64.b64encode(self.result)
        
    def decode(self,string):
        self.keyc = string[0:self.ckey_lenth]
        string = base64.b64decode(string[self.ckey_lenth:])
        self.result = ''
        self.docrypt(string)
        result = self.result
        if (result[0:10] == '0000000000' or int(result[0:10]) - int(time.time()) > 0) and result[10:26] == hashlib.md5(result[26:] + self.keyb).hexdigest()[0:16]:
            return result[26:]
        else:
            return None
        
    def docrypt(self,string):
        string_lenth = len(string)
        result = ''
        box = list(range(256))
        randkey = []
        
        cryptkey = self.keya + hashlib.md5(self.keya + self.keyc).hexdigest()
        key_lenth = len(cryptkey)
        
        for i in xrange(255):
            randkey.append(ord(cryptkey[i % key_lenth]))
        
        for i in xrange(255):
            j = 0
            j = (j + box[i] + randkey[i]) % 256
            tmp = box[i]
            box[i] = box[j]
            box[j] = tmp
            
        for i in xrange(string_lenth):
            a = j = 0
            a = (a + 1) % 256
            j = (j + box[a]) % 256
            tmp = box[a]
            box[a] = box[j]
            box[j] = tmp
            self.result += chr(ord(string[i]) ^ (box[(box[a] + box[j]) % 256]))


rc = rc4('dc')
string = '1234567890'
print(string)
str = rc.encode(string)
print(str)
str = "U2FsdGVkX1+HDFjPovkCWoOlyIsJRYFQNGqpK2I0RFY="
str = rc.decode(str)
print(str)
