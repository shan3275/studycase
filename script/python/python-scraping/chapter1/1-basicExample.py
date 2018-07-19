from urllib.request import urlopen
#Retrieve HTML string from the URL
#html = urlopen("http://www.pythonscraping.com/exercises/exercise1.html")
html = urlopen("http://www.163.com")
print(html.read())
