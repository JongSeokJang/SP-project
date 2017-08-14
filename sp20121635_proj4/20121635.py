import requests
from bs4 import BeautifulSoup

rootUrl = "http://cspro.sogang.ac.kr/~cse20121611/"
visitedPage	= [];
notGoSymbol	= [" ", "#", "?"]
fileNum = 1

def ReadPage(pageUrl) :
	global fileNum
	req = requests.get(pageUrl)
	soup = BeautifulSoup(req.content, "html.parser")
	results = soup.find_all('a')

	if req.ok == True:
		if pageUrl not in visitedPage:
			fileName	= "Output_"+ ('%04d' %fileNum) +'.txt'
			fileStream	= open(fileName,"w")
			fileStream.write(soup.text)
			fileStream.close()
			fileNum += 1
			visitedPage.append(pageUrl)

	for ii in results:
		hyperUrl = str(ii.get('href'))
		if len(hyperUrl) == 0 or hyperUrl[0] in notGoSymbol:
			continue
		else:
			if hyperUrl[0:7] == "http://":
				nextUrl = hyperUrl	
			else :
				nextUrl = rootUrl+hyperUrl
			
			if nextUrl in visitedPage:
				continue
			else:
				ReadPage(nextUrl)

ReadPage(rootUrl+"index.html")
urlFile = open("URL.txt","w")
for page in visitedPage:
	urlFile.write(page+"\n")
urlFile.close();
