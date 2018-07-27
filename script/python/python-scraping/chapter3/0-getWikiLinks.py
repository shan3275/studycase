from urllib.request import urlopen
from bs4 import BeautifulSoup
import datetime
import random
import re
import ssl

random.seed(datetime.datetime.now())
def getLinks(articleUrl):
    html = urlopen("https://en.wikipedia.org"+articleUrl)
    bsObj = BeautifulSoup(html, "html.parser")
    return bsObj.find("div", {"id":"bodyContent"}).findAll("a", href=re.compile("^(/wiki/)((?!:).)*$"))
links = getLinks("/wiki/Kevin_Bacon")
for link in links:
    if 'href' in link.attrs:
        print(link.attrs['href'])
