#$language = "python"
#$interface = "1.0"

import re
import sys
import os
import time

test_file_dir = 'D:\\test\\'

def portshow():
  crt.Screen.Synchronous = True
  #file_time = time.strftime('%Y-%m-%d-%H-%M-%S', time.localtime(time.time()))
  #file = open(test_file_dir+'test'+ file_time + '.txt', 'a')
  #for name, aer, block, offset in enumerate(serdes_reg): 
  
  while 1:
    crt.Screen.WaitForString("root@FS9000:~#")
    crt.Screen.Send("dtweak capm if\n")
    
  file.close()
  crt.Screen.Synchronous = False 

def main():
  for i in range(1):
    portshow()
    
main()

