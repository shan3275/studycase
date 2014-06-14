#!/usr/bin/env python

import pexpect
import getpass, os
def ssh_command (user, host, password,command):
    """
    This runs a command on the remote host. This could also be done with the
    pxssh class, but this demonstrates what that class does at a simpler level.
    This returns a pexpect.spawn object. This handles the case when you try to
    connect to a new host and ssh asks you if you want to accept the public key
    fingerprint and continue connecting.
    """
    ssh_newkey = 'Are you sure you want to continue connecting'
    child = pexpect.spawn('/usr/local/newssh/bin/ssh -N -v -Z diyssh -p 2222 %s@%s -D 127.0.0.1:7070 %s'%(user, host, command))
    i = child.expect([pexpect.TIMEOUT, ssh_newkey, 'password: '])
    if i == 0: 
        print 'ERROR!'
        print 'SSH could not login. Here is what SSH said:'
        print child.before, child.after
        return None
    if i == 1: 
	print 'yes'
        child.sendline ('yes')
        #child.expect ('password:')
        i = child.expect([pexpect.TIMEOUT, 'password:'])
        if i == 0: 
         print child.before, child.after
         return None
    child.sendline(password)
    print 'good'
    return child
def main ():
    host = 's1.enshi.us'
    user = 'psitvsvu'
    password = 'whV6Vat237'
    command = ' '
    child = ssh_command (user, host, password, command)
    #child.expect(pexpect.EOF)
    #print child.before
    child.interact()

main()

