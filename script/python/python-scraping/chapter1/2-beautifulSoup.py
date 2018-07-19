from urllib.request import urlopen
from bs4 import BeautifulSoup

#html = urlopen("http://www.pythonscraping.com/exercises/exercise1.html")
html = urlopen("http://www.163.com")
bsObj = BeautifulSoup(html.read(), "lxml")
print(bsObj.h1)
print (" ")
print(bsObj.title)
print (" ")
print(bsObj.head)
print (" ")
print(bsObj.div)
print (" ")
#print(bsObj)
