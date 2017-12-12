# -*- coding: UTF-8 -*
#!/usr/bin/python

#将n个盘子从x通过y移动到z上
def move(n, x, y, z):
    if(n == 1):
        print("{}:{}->{}".format(n,x,z))
        return
    move(n-1, x, z, y)#将前n-1个盘子从x通过z移动到y上
    print("{}:{}->{}".format(n,x,z))#将第n个盘子从x移动到z上
    move(n-1, y, x, z)#将前n-1个盘子从y通过x移动到z上
    
move(8,"x","y","z");
