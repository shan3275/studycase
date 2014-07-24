#!/usr/bin/python

import telnetlib
import time
import sys

if __name__ == "__main__":
    try:
        server = sys.argv[1]
        port = sys.argv[2]
    except:
        print("Usage:\n\t./bcm_test.py server_ip port")

    tl = telnetlib.Telnet(server, port)
    tl.write("\n")
    tl.read_very_eager()

    while True:
        command = "ps\n"
        tl.write(command)
        tl.read_until("Forward")
        #time.sleep(2)
        print tl.read_all()
