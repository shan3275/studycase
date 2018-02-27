#!/usr/bin/python

from __future__ import division

import sys
import re
import threading
import time
from optparse import OptionParser
import shutil
from datetime import datetime
from urlparse import *
import urllib

VERSION="LOGANA V1.11, 2018/1/11; copyright by less (less@beescast.com)"

CNT   = 0
HIT   = 0
END   = 0
STIME = 0

DD_TAB ={}
DU_TAB ={}
RD_TAB ={}
RU_TAB ={}

DUPAT = []

def str_hit(tab, str):
    tab[str] = tab.get(str, 0) + 1

def host_hit(tab, url):
    try:
        r = urlparse(url)
        host = r.netloc
    except:
        host = "ERROR"

    tab[host] = tab.get(host, 0) + 1

def classify_field(tab, pattern, url):
    fields = re.findall(pattern, url)
    if len(fields) > 0:
        tab[fields[0]] = tab.get(fields[0], 0) + 1

BIZ_TAB = {}
def classify_biz(url):
    classify_field(BIZ_TAB, "__biz=(.+?)==", url)

TTH_TAB = {}
def classify_tth(url):
    classify_field(TTH_TAB, "toutiao\.com\/a(.+?)\/", url)

QUERY_TAB = {}
def classify_query(url):
    classify_field(QUERY_TAB, r"query=(.+?)&", url)

def dump_tab_topx(tabname, tab, topx, ofile):
    IDX = 0
    ITEMS = sorted(tab.items(), key=lambda T: T[1], reverse=True)
    print "\n ----------------------------------------------------------------------------------------"
    print >> ofile, "\n ----------------------------------------------------------------------------------------"
    print " %s:" %(tabname)
    print >> ofile, " %s:" %(tabname)
    for UVAL, NUM in ITEMS:
        try:
            VAL = urllib.unquote(UVAL)
        except:
            VAL = UVAL

        print " %-6d          %-10d      %-80s" % (IDX, NUM, VAL)
        print >> ofile, " %-6d          %-10d      %-80s" % (IDX, NUM, VAL)
        IDX += 1

        if IDX >= topx:
            break

def stat_dump(ofile):
    global CNT, HIT, STIME

    ntime = datetime.now()

    sspend = (ntime - STIME).seconds

    mspend = (ntime - STIME).microseconds

    spend = sspend * 1000000 + mspend

    #print "sspend=%d , mspend=%d" % (sspend, mspend)
    #print "spend=%d" %(spend)

    if spend < 1:
        spend = 1

    speed = (CNT * 1000000) / spend

    if CNT == 0:
        rate = 0
    else:
        rate = (HIT * 100) / CNT

    secs = spend / 1000000

    print "\r CNT %-10d  HIT %-10d  RATE %.4f%%          %-5.2f seconds  %4d/s; " % (CNT, HIT, rate, secs, speed),
    sys.stdout.flush()

    if ofile != None:
        print >> ofile, " ----------------------------------------------------------------------------------------"
        print >> ofile, " CNT %-10d  HIT %-10d  RATE %.4f%%          %-5.2f seconds  %4d/s; " % (CNT, HIT, rate, secs, speed)

def stat_thread(period, ofile):
    global END, STIME
    stop = 0
    print "\n"

    STIME = datetime.now()
    time.sleep(period)
    while stop == 0:
        stat_dump(None)

        if END == 0:
            time.sleep(period)
        else:
            stat_dump(ofile)
            stop = 1

def field_compare(pstr, vstr):
    result = re.search(pstr, vstr)

    if result:
        return True

    return False

def uri_compare(pstr, vstr):
    uri = vstr

    npos = vstr.find('?')

    if npos > 0:
        uri = vstr[0:npos]

    return field_compare(pstr, uri)



def log_filter(ifile, ofile, kdicts, options):
    global CNT, HIT, END
    #for LINE in ifile.readlines():

    #print "LOG=%s" %(options.log)
    #print "kdicts=", kdicts

    for LINE in ifile:
        LINE = LINE.strip('\n')
        FIELD = LINE.split('|')

        if len(FIELD) < 5:
            continue

        TIME = FIELD[0]
        SIP  = FIELD[3]
        DIF  = FIELD[4]
        DUF  = FIELD[9]
        RUF  = FIELD[11]
        UAF  = FIELD[10]

        CNT += 1

        if kdicts != None:

            KHIT = 0

            for kdict in kdicts:
                if kdict.has_key('di') and uri_compare(kdict['di'], DUF) == True:
                    KHIT = 1
                    break

                if kdict.has_key('du') and field_compare(kdict['du'], DUF) == True:
                    KHIT = 1
                    break

                #print "ru=%s, RUF=%s" %(kdict['ru'], RUF)
                if kdict.has_key('ru') and field_compare(kdict['ru'], RUF) == True:
                    KHIT = 1
                    break

                if kdict.has_key('ua') and field_compare(kdict['ua'], UAF) == True:
                    KHIT = 1
                    break

            if KHIT == 0:
                continue

        if options.log:
            print >> ofile, LINE

        HIT += 1

        if options.classify == "biz":
            classify_biz(DUF)

        if options.classify == "tth":
            classify_tth(DUF)

        if options.classify == "query":
            classify_query(RUF)

        # Classify dest host
        if options.ddtopx > 0:
            durl = 'http://' + DUF
            host_hit(DD_TAB, durl)

        # classify dest url
        if options.dutopx > 0:
            str_hit(DU_TAB, DUF)

        # classify ref host
        if options.rdtopx > 0:
            host_hit(RD_TAB, RUF)

        # classify ref url
        if options.rutopx > 0:
            str_hit(RU_TAB, RUF)

    END = 1


def kstr_parser(kstr):
    kdict = {}

    if kstr == None:
        return None

    pairs = kstr.split(",")

    for pair in pairs:
        ep = pair.find('=')

        if ep < 0:
            return None
        key = pair[0:ep]
        val = pair[ep+1:]
        #print "key=%s, val=%s" % (key, val)

        kdict[key] = val

    return kdict



def logs_filter(ofile, kdicts, options):

    ipaths = options.ipath.split(",")

    for ipath in ipaths:
        ifile = open(ipath, "r")

        log_filter(ifile, ofile, kdicts, options)

        ifile.close()

def dump_base_info(ofile):
    global VERSION, STIME
    print >> ofile, "\n ----------------------------------------------------------------------------------------"
    print >> ofile, " %s" % (VERSION)
    print >> ofile, " %s" % (STIME.strftime('%Y-%m-%d %H:%M:%S'))
    print >> ofile, " %s" % (' '.join(sys.argv))

    print "\n ----------------------------------------------------------------------------------------"
    print " %s" % (VERSION)
    print " %s  <->  %s" % (STIME.strftime('%Y-%m-%d %H:%M:%S'), datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
    print " %s" % (' '.join(sys.argv))



def main():
    global END
    usage = "usage: %prog [options]"
    parser = OptionParser(usage)
    parser.add_option("-i", "--ipath", type="string", dest="ipath", help="read log from FILENAME")
    parser.add_option("-o", "--opath", type="string", dest="opath", help="write result to FILENAME")
    parser.add_option("-K", "--kpath", type="string", dest="kpath", help="hosts list from FILENAME")
    parser.add_option("-d", "--dest_domain", type="int", dest="ddtopx", default=0, help="classify dest domain")
    parser.add_option("-D", "--dest_url", type="int", dest="dutopx", default=0, help="classify dest url")
    parser.add_option("-r", "--refer_domain", type="int", dest="rdtopx", default=0, help="classify refer domain")
    parser.add_option("-R", "--refer_url", type="int", dest="rutopx", default=0, help="classify refer url")
    parser.add_option("-c", "--classify", type="string", dest="classify", help="classify biz,tth or query in url")
    parser.add_option("-k", "--key", type="string", dest="kstr",help="match key & value")
    parser.add_option("-v", "--verbose", action="store_true", dest="version")
    parser.add_option("-l", "--log", action="store_true", dest="log")

    (options, args) = parser.parse_args()

    #print "options", optionsu

    if options.version:
        print "%s." % VERSION
        exit(1)

    if options.ipath == None or options.opath == None:
        parser.error("Invaild param")

    kdicts = []
    if options.kpath != None:
        #if kdict != None:
        #    kdicts.append(kdict)
        hfile = open(options.kpath, "r")

        for LINE in hfile:
            LINE = LINE.strip('\n')
            kdict = {}
            PATSTR = LINE + "(.*?)\.s?html"
            kdict['di'] = PATSTR
            kdicts.append(kdict)
    elif options.kstr != None:
        kdict = kstr_parser(options.kstr)

        hfile.close()

    else:
        kdicts = None

    ofile = open(options.opath, "w")

    t = threading.Thread(target=logs_filter, args=(ofile, kdicts, options, ))
    t.start()

    stat_thread(0.25, ofile)

    if kdicts !=None and len(kdicts) > 0:
        print >> ofile, "\n ----------------------------------------------------------------------------------------"
        print "\n ----------------------------------------------------------------------------------------"

        print >> ofile, "KDICTS:"
        print " KDICTS:"
        KCNT = 0

        for kdict in kdicts:
            print >> ofile, " K[%d]: " % (KCNT),
            print " K[%.2d]: " % (KCNT),
            if kdict.has_key('di'):
                print >> ofile, " di=%s" % (kdict['di']),
                print " di=%s" % (kdict['di']),

            if kdict.has_key('du'):
                print >> ofile, " du=%s" % (kdict['du']),
                print " du=%s" % (kdict['du']),

            if kdict.has_key('ru'):
                print >> ofile, " ru=%s" % (kdict['ru']),
                print " ru=%s" % (kdict['ru']),

            if kdict.has_key('ua'):
                print >> ofile, " ua=%s" % (kdict['ua']),
                print " ua=%s" % (kdict['ua']),

            print >> ofile, ""
            print ""

            KCNT += 1

    if options.ddtopx > 0:
        dump_tab_topx("Dest host",  DD_TAB, options.ddtopx, ofile)

    if options.dutopx > 0:
        dump_tab_topx("Dest url",   DU_TAB, options.dutopx, ofile)

    if options.rdtopx > 0:
        dump_tab_topx("Refer host", RD_TAB, options.rdtopx, ofile)

    if options.rutopx > 0:
        dump_tab_topx("Refer url",  RU_TAB, options.rutopx, ofile)

    if options.classify == "biz":
        dump_tab_topx("BIZ", BIZ_TAB, 1000, ofile)

    if options.classify == "tth":
        dump_tab_topx("TTH", TTH_TAB, 1000, ofile)

    if options.classify == "query":
        dump_tab_topx("QUERY", QUERY_TAB, 1000, ofile)


    time.sleep(3)
    dump_base_info(ofile)

    ofile.close()

if __name__ == "__main__":
    main()

# End of file
# Test command
# python logana.py -i /nwh/logana/log/zebra.log20170814  -o /nwh/logana/result/zebra.log20170814_query.log -k ru="weixin\.sogou\.com" -c query
# python logana.py -i /nwh/logana/log/202.log20171028  -o /nwh/logana/result/202.log20171028_tth.log  -k du="www\.toutiao.\com" -c tth -r 50
# python logana.py -i /nwh/logana/result/206.log20171026_yangkeduo.log  -o /nwh/logana/result/206.log20171026_mobile.yangkeduo.log -k du="^mobile\.yangkeduo\.com" -r 50 -l
# python logana.py -i /nwh/logana/result/206.log20171026_mobile.yangkeduo.log  -o /nwh/logana/result/206.log20171026_mobile.yangkeduo_goods.log -k du="^mobile\.yangkeduo\.com\/goods\.html" -r 50 -l
