# -*- coding: UTF-8 -*
#!/usr/bin/python
#author:www.py40.com

#二分法查找
  
def search2(a,m):  
    low = 0  
    high = len(a) - 1  
    while low<=high:  
        mid = (int)((low + high)/2)  
        print "mid:%u" %(mid)
        midval = a[mid]  
        if midval<m:  
            low = mid + 1  
        elif midval>m:  
            high = mid-1  
        else:  
            print(mid)
            return(mid) 
    print(-1)
    return(-1)  
  
if __name__ == "__main__":
    a = [1,3,45,12,3,1,3,5,12,6,74,32,6,3]
    a.sort()
    print(a)
    search2(a,12)  
