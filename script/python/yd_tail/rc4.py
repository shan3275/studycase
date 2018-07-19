#!/usr/bin/env python

import hashlib,time,random, base64
import sys,os


if len(sys.argv) != 4:
    print("USAGE: %s  (encode|decode) input_file output_file" % sys.argv[0])
    exit()
method=sys.argv[1]
input_file=sys.argv[2]
output_file=sys.argv[3]

if method != 'decode' and method != 'encode':
    print("USAGE: %s  (encode|decode) input_file output_file" % sys.argv[0])


#key value
key='women@qianglie##fandui**meiguo__xiji<>xuliya'

#defind class for RC4
class rc4:
    def __init__(self,public_key):
        self.public_key = public_key
        #undo key vale hash
        #self.public_key = hashlib.md5(public_key).hexdigest()
    def encode(self,string):
        self.result = bytearray()
        self.docrypt(string)
        #undo result base64
        #self.result = base64.b32encode(self.result)
        return self.result
    def decode(self,string):
        self.result = bytearray()
        #string = base64.b32decode(string)
        self.docrypt(string)
        return self.result

    def docrypt(self,string):
        string_lenth = len(string)
        box = []
        randkey = []
        key_lenth = len(self.public_key)
        index1 = 0
        index2 = 0

        for i in xrange(256):
            box.append(i)

        for i in xrange(key_lenth):
            randkey.append(ord(self.public_key[i]))

        for i in xrange(256):
            index2 = ((randkey[index1] & 0xff) + (box[i] & 0xff) + index2) & 0xff
            tmp = box[i]
            box[i] = box[index2]
            box[index2] = tmp
            index1 = (index1 + 1) % key_lenth

        print string_lenth
        print box

        x = 0
        y = 0
        for i in xrange(string_lenth):
            x = (x + 1) & 0xff
            y = ((box[x] & 0xff) + y) & 0xff
            tmp = box[x]
            box[x] = box[y]
            box[y] = tmp
            xorIndex = ((box[x] & 0xff) + (box[y] & 0xff)) & 0xff
            self.result.append(ord(string[i]) ^ box[xorIndex])

#create object 
method_rc4=rc4(key)

#read input source
with open(input_file) as rf:
    data=rf.read()

#code data
data_method =getattr(method_rc4,method) 
rc4_data = data_method(data)

#wirte data to outfile
with open(output_file,'wb') as wf:
    wf.write(rc4_data)



