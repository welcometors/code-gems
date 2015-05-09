from bs4 import BeautifulSoup
from unidecode import unidecode
import requests

def makelist(table, title):
    result = []
    allrows = table.findAll('tr')
    idx = 0
    for row in allrows:
        rowData = []
        allcols = row.findAll('td')
        if len(allcols) < 1:
            allcols = row.findAll('th')
        for col in allcols:
            thestrings = [s.encode('ascii').decode('utf-8').strip() for s in col.findAll(text=True)]
            thetext = ''.join(thestrings)
            rowData.append(thetext)
        rowData.insert(0, title + ' ' + str(idx))
        idx += 1
        result.append(rowData)
    return result


def getInfo(url):
    r  = requests.get(url)
    data = r.text
    
    soup = BeautifulSoup(data)
    text = soup.find(id="mw-content-text")
    header = unidecode(soup.find(id="WikiaPageHeader").find("h1").text)

    res = []
    for table in soup.find_all("table", { "class" : "wikitable center" }):
        isStats = 0
        for th in table.find_all('th'):
            if th.find('a') is not None and th.find('a').get('title') is not None:
                title = th.find('a').get('title')
                if title == "Databook":
                    isStats = True
                    break 
        if isStats:
            res = makelist(table, header)

    return (header, res)

base = "http://naruto.wikia.com"
books = [ "/wiki/Rin_no_Sho", "/wiki/T%C5%8D_no_Sho", "/wiki/Sha_no_Sho"]
characters = set()
for book in books:
    r  = requests.get(base + book)
    data = r.text
    soup = BeautifulSoup(data)
    list = soup.find(id="mw-content-text").find_all("ol")[1].find_all("li")
    print("{0} characters fount in book '{1}'".format(len(list), book))
    for item in list:
        text = item.find("a").get("href") #.text.encode('ascii', errors='replace').decode('utf-8').strip()
        characters.add(text)
        #print(text)

print("Total {0} unique characters are found.".format(len(characters)))

ctr = 0
dict = {}
for char in characters:
    print("{0} > searching for '{1}' ...".format(ctr, char))
    title, table = getInfo(base + char)
    if len(table) > 1: 
        print("{0} rows added for {1}.".format(len(table)-1, title))
        dict[title] = table[1:]
    else:
        print("{0} no info found.".format(title))
    ctr += 1

f = open("narutoAllCharStat.csv", "w")
f.write("Character, Ninjutsu, Taijutsu, Genjutsu, Intelligence, Strength, Speed, Stamina, Hand seals, Total\n")
for k,v in dict.items():
    for r in v:
        f.write(",".join(r) + "\n")
f.close()

#url = "/wiki/Neji_Hy%C5%ABga"
#url = "http://naruto.wikia.com/wiki/One-Tailed_Shukaku"
#url = "http://naruto.wikia.com/wiki/Unnamed_Nara_Clan_Member"
#title, table = getInfo(base + url)

#print(title)
#print(table)