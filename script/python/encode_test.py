#!/usr/bin/python
#str = "this is string example....wow!!!"
str = 'irs01.com/irt?_imt_id=oROptOEV3VF5ZG0vR1XrMQA&_iwt_UA=ua-youku-130001&_t=i&_iwt_p1=B-0-0&_iwt_p2=D:/Youku%20Files/download/\xbe\xad\xc2\xe7\xeb\xf2\xd1\xa8\xd1\xa7\xc3\xcf\xc0\xcf\xca\xa6/\xbe\xad\xc2\xe7\xeb\xf2\xd1\xa8\xd1\xa703_\xb8\xdf\xc7\xe5.kux&_iwt_p3=1677-1525-19-28&_iwt_p4=youku&_iwt_p6=win_10.0&_iwt_muid=5385'
print 'str:%s' %(str)
print "Encoded String(base64): " + str.encode('base64','strict')
print "Encoded String(utf-8): "  + str.decode('utf-8', 'ignore')
