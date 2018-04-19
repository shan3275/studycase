# coding: utf-8

from multiprocessing import Process
import time
import os

def task(msg):
    print 'hello, %s' % msg
    print("task ",os.getpid(),os.getppid())
    time.sleep(1)


if __name__ == '__main__':
    p = Process(target=task, args=('world',))

    p.start()
    if p.is_alive():
        print 'Process: %s is running' % p.pid
    p.join()
    print("master ",os.getpid(),os.getppid())
