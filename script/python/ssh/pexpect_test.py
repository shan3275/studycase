#!/usr/bin/env python
# -*- coding: utf-8 -*-
# filename: pexpect_test.py
# Version 0.1
'''
Created on 2010-7-2
 
@author: forever
@modified: shan
'''
import pexpect
 
if __name__ == '__main__':
    user = 'root'
    ip = 'admartzone.com'
    mypassword = 'csE)5ZbMXX$c*rPPY.4UfG*aNM(IqFkJ'
 
    pexpect.run('ls -la')
    print user
    child = pexpect.spawn('ssh %s@%s -p 65522' % (user,ip))
    child.expect ('password:')
    child.sendline (mypassword)
 
    child.expect('#')
    child.sendline(' echo '' > /var/log/wtmp')
    child.sendline(' ls -la')
    child.expect('#')
    print child.before
    child.sendline(' exit')

    child.interact()     # Give control of the child to the user.
 
    pass
