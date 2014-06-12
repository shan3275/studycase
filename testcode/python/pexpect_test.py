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
    user = 'shan'
    ip = '192.168.1.35'
    mypassword = 'shan'
 
    pexpect.run('ls -la')
    print user
    child = pexpect.spawn('ssh %s@%s' % (user,ip))
    child.expect ('password:')
    child.sendline (mypassword)
 
    child.expect('$')
    child.sendline('ls -la')
    '''
    child.sendline('sudo -s')
    child.expect (':')
    child.sendline (mypassword)
    child.expect('#')
    child.sendline('ls -la')
    child.expect('#')
    print child.before   # Print the result of the ls command.
    child.sendline("echo '112' >> /home/shan/1.txt ")
    '''
    child.interact()     # Give control of the child to the user.
 
    pass
