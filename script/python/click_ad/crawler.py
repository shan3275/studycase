#!/usr/bin/python
# -*- coding:utf-8 -*-


from __future__ import division

import sys
import re
import threading
import time
from datetime import datetime
import urllib
import urllib2
import os


from optparse import OptionParser
import shutil

from urlparse import *

import traceback


VERSION = "LOGANA V1.03, 2017/12/01; copyright by less (less@beescast.com)"

CNT = 0
HIT = 0
ERR = 0
END = 0
STM = 0


def stat_dump(ofile):
    global CNT, HIT, STM

    ntime = datetime.now()

    sspend = (ntime - STM).seconds

    mspend = (ntime - STM).microseconds

    spend = sspend * 1000000 + mspend

    # print "sspend=%d , mspend=%d" % (sspend, mspend)
    # print "spend=%d" %(spend)

    if spend < 1:
        spend = 1

    speed = (CNT * 1000000) / spend

    if CNT == 0:
        rate = 0
    else:
        rate = (HIT * 100) / CNT

    secs = spend / 1000000

    print "\r CNT %-10d  HIT %-10d  ERR %-10d  RATE %.4f%%          %-5.2f seconds  %4d/s; " % (CNT, HIT, ERR, rate, secs, speed),
    sys.stdout.flush()

    if ofile != None:
        print >> ofile, " ----------------------------------------------------------------------------------------"
        print >> ofile, " CNT %-10d  HIT %-10d  ERR %-10d RATE %.4f%%          %-5.2f seconds  %4d/s; " % (
        CNT, HIT, ERR, rate, secs, speed)


def stat_thread(period, ofile):
    global END, STM
    stop = 0
    print "\n"

    STM = datetime.now()
    time.sleep(period)
    while stop == 0:
        stat_dump(None)

        if END == 0:
            time.sleep(period)
        else:
            stat_dump(ofile)
            stop = 1


def dump_base_info(ofile):
    global VERSION, STM
    print >> ofile, "\n ----------------------------------------------------------------------------------------"
    print >> ofile, " %s" % (VERSION)
    print >> ofile, " %s" % (STM.strftime('%Y-%m-%d %H:%M:%S'))
    print >> ofile, " %s" % (' '.join(sys.argv))

    print "\n ----------------------------------------------------------------------------------------"
    print " %s" % (VERSION)
    print " %s  <->  %s" % (STM.strftime('%Y-%m-%d %H:%M:%S'), datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
    print " %s" % (' '.join(sys.argv))


# End of file


TNUM = 0
TMAX = 50

class SaveThread(threading.Thread):
    def __init__(self, url, filename):
        threading.Thread.__init__(self)
        self.url = url
        self.filename = filename


    def saveHtml(self, file_name, file_content):
        #    注意windows文件命名的禁用符，比如 /
        # with    open(file_name.replace('/', '_') + ".html", "wb") as f:
        #   f.write(file_content)
        with    open(file_name, "wb") as f:
            f.write(file_content)

    def run(self):

        #print "url=%s, filename=%s" %(self.url, self.filename)
        global CNT, HIT, ERR, TNUM

        TNUM += 1
        try:
            headers = {'User-Agent': 'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'}

            req = urllib2.Request(self.url, headers=headers)
            response = urllib2.urlopen(req)
            html = response.read()

            self.saveHtml(self.filename, html)
            HIT += 1
        except Exception, e:
            print 'str(e)', str(e)
            ERR += 1

        CNT += 1
        TNUM -= 1


class MyThread(threading.Thread):
    def __init__(self, ifile, ofile, odir):
        threading.Thread.__init__(self)
        self.ifile = ifile
        self.ofile = ofile
        self.odir  = odir

    def run(self):

        global TNUM, TMAX, END

        TNUM = 0

        LCNT = 0

        for LINE in self.ifile.readlines():
            LINE = LINE.strip('\n')
            FIELD = LINE.split('|')
            if len(FIELD) < 5:
                continue

            SPF = FIELD[0]
            DPF = FIELD[1]
            DUF = FIELD[2]
            RUF = FIELD[3]
            UAF  = FIELD[4]

            SPV = SPF.split(' ')

            TIME = SPV[0]
            SIP =  SPV[4]

            #duc = re.findall(r"^(.+?)&", DUF)[0]
            #url = "http://%s" % (duc)
            url = "http://%s" % (DUF)
            #print "DUF=%s, url=%s" % (DUF, url)

            #gid = re.findall(r"goods_id=(.+?)&", DUF)[0]
            #web_file = "%s/%s.html" % (self.odir, gid)
            web_file = "%s/%d.html" % (self.odir, LCNT)

            while TNUM >= TMAX:
                time.sleep(0.01)

            save_thread = SaveThread(url, web_file)
            save_thread.start()

            print >> self.ofile, "%-10d    %s    %s" % (LCNT, url, web_file)
            LCNT += 1

        END = 1

def main():
    global END
    usage = "usage: %prog [options]"
    parser = OptionParser(usage)
    parser.add_option("-i", "--ipath", type="string", dest="ipath", help="read log from FILENAME")
    parser.add_option("-o", "--odir", type="string", dest="odir", help="write result to FILENAME")
    parser.add_option("-v", "--verbose", action="store_true", dest="version")

    (options, args) = parser.parse_args()

    #print "options", optionsu

    if options.version:
        print "%s." % VERSION
        exit(1)

    if options.ipath == None or options.odir == None:
        parser.error("Invaild param")

    ipath = options.ipath

    odir = options.odir

    ifilename = os.path.basename(ipath)

    opath = odir + "/" + ifilename + '.report'

    isExists = os.path.exists(odir)
    if not isExists:
        os.makedirs(odir)

    ifile = open(ipath, "r")
    ofile = open(opath, "w")

    #t = threading.Thread(target=log_filter, args=(ifile, ofile, kdict, destopx, reftopx, classify, log, ))
    #t.start()

    my_thread = MyThread(ifile, ofile, odir)
    my_thread.start()

    stat_thread(0.25, ofile)

    ifile.close()

    dump_base_info(ofile)

    ofile.close()

if __name__ == "__main__":
    main()

# End of file

# Demo command:
# python crawler.py -i /nwh/logana/result/206.log20171026_mobile.yangkeduo_goods.log -o /nwh/logana/html/yangkeduo_goods

