import math
import numpy as np
from sklearn.svm import LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import GradientBoostingClassifier

heroes = {"Ursa":0,"Magnus":1,"Rubick":2,"Spectre":3,"Brewmaster":4,"Tidehunter":5,"Riki":6,"Lone Druid":7,"Undying":8,"Sven":9,"Troll Warlord":10,"Medusa":11,"Slardar":12,"Viper":13,"Leshrac":14,"Tiny":15,"Spirit Breaker":16,"Dragon Knight":17,"Queen of Pain":18,"Sniper":19,"Beastmaster":20,"Razor":21,"Ancient Apparition":22,"Chen":23,"Timbersaw":24,"Storm Spirit":25,"Slark":26,"Naga Siren":27,"Doom":28,"Gyrocopter":29,"Bane":30,"Nature's Prophet":31,"Batrider":32,"Puck":33,"Zeus":34,"Jakiro":35,"Lina":36,"Shadow Demon":37,"Dazzle":38,"Anti-Mage":39,"Wisp":40,"Nyx Assassin":41,"Huskar":42,"Faceless Void":43,"Meepo":44,"Kunkka":45,"Bounty Hunter":46,"Axe":47,"Enigma":48,"Visage":49,"Lycanthrope":50,"Dark Seer":51,"Lifestealer":52,"Witch Doctor":53,"Broodmother":54,"Bloodseeker":55,"Silencer":56,"Lion":57,"Enchantress":58,"Night Stalker":59,"Chaos Knight":60,"Phantom Lancer":61,"Disruptor":62,"Necrolyte":63,"Death Prophet":64,"Luna":65,"Templar Assassin":66,"Juggernaut":67,"Sand King":68,"Morphling":69,"Warlock":70,"Tinker":71,"Crystal Maiden":72,"Phantom Assassin":73,"Pudge":74,"Skeleton King":75,"Treant Protector":76,"Earthshaker":77,"Venomancer":78,"Pugna":79,"Weaver":80,"Outworld Devourer":81,"Centaur Warrunner":82,"Invoker":83,"Ogre Magi":84,"Clockwerk":85,"Mirana":86,"Clinkz":87,"Keeper of the Light":88,"Vengeful Spirit":89,"Lich":90,"Drow Ranger":91,"Shadow Shaman":92,"Shadow Fiend":93,"Omniknight":94,"Windrunner":95,"Alchemist":96}
dims = len(heroes)
traits = {16:[1, 2, 2, 0, 0],68:[1, 3, 1, 0, 0],3:[5, 2, 1, 2, 0],72:[0, 1, 1, 0, 0],15:[4, 2, 1, 0, 1],56:[2, 2, 1, 0, 0],38:[0, 0, 1, 0, 0],39:[3, 4, 1, 1, 0],45:[2, 2, 1, 0, 1],35:[0, 1, 1, 0, 0],42:[1, 2, 2, 0, 0],32:[2, 2, 1, 0, 0],13:[2, 3, 1, 0, 0],34:[2, 3, 1, 0, 0],10:[3, 3, 1, 0, 0],53:[2, 2, 1, 0, 0],18:[3, 3, 1, 0, 1],69:[3, 2, 1, 0, 0],20:[2, 2, 1, 0, 0],82:[2, 2, 2, 0, 0],75:[3, 2, 1, 0, 0],12:[3, 3, 1, 0, 0],22:[2, 1, 1, 0, 0],27:[4, 2, 1, 2, 0],25:[2, 3, 1, 0, 0],58:[2, 2, 3, 1, 0],57:[1, 2, 1, 0, 0],14:[2, 3, 1, 0, 0],37:[0, 1, 1, 0, 0],94:[0, 1, 0, 0, 0],0:[2, 3, 3, 0, 0],48:[2, 2, 1, 1, 0],55:[2, 3, 1, 0, 0],11:[4, 1, 0, 0, 1],74:[1, 2, 1, 0, 0],91:[3, 3, 2, 0, 0],92:[1, 2, 1, 0, 0],28:[5, 2, 0, 0, 0],41:[1, 2, 1, 0, 0],7:[3, 3, 1, 0, 0],71:[3, 3, 1, 0, 0],50:[3, 3, 1, 1, 0],64:[3, 3, 1, 0, 0],80:[3, 2, 1, 0, 0],47:[1, 3, 3, 0, 0],73:[3, 3, 1, 0, 0],36:[2, 3, 1, 0, 0],40:[0, 1, 1, 0, 0],51:[3, 2, 1, 0, 0],21:[2, 3, 1, 0, 0],44:[2, 4, 1, 2, 0],6:[2, 3, 0, 0, 0],62:[1, 1, 1, 0, 0],86:[2, 2, 1, 0, 0],24:[2, 3, 1, 0, 1],65:[3, 2, 1, 1, 0],89:[2, 1, 1, 0, 0],30:[2, 2, 1, 0, 0],90:[1, 2, 1, 0, 0],1:[2, 3, 1, 0, 0],59:[2, 3, 2, 0, 0],81:[3, 2, 0, 0, 0],84:[1, 1, 1, 0, 0],31:[3, 2, 1, 1, 0],78:[2, 2, 1, 0, 0],26:[3, 2, 1, 0, 0],8:[0, 1, 2, 0, 0],76:[0, 0, 1, 0, 0],60:[4, 2, 1, 2, 0],29:[3, 2, 1, 0, 0],95:[2, 2, 1, 0, 0],88:[0, 1, 1, 0, 1],61:[4, 3, 1, 2, 0],87:[3, 3, 1, 0, 0],83:[3, 3, 1, 0, 1],46:[2, 2, 1, 0, 0],54:[2, 2, 1, 1, 0],93:[3, 3, 1, 0, 1],5:[1, 2, 1, 0, 0],63:[3, 2, 0, 0, 0],96:[3, 3, 1, 0, 0],4:[2, 3, 1, 0, 0],23:[0, 2, 3, 1, 0],66:[3, 3, 1, 0, 0],9:[3, 2, 1, 0, 1],85:[2, 2, 2, 0, 0],52:[3, 3, 1, 0, 0],70:[2, 2, 0, 0, 0],79:[2, 2, 1, 0, 0],17:[3, 3, 1, 0, 0],2:[0, 1, 1, 0, 0],19:[3, 2, 1, 0, 0],33:[2, 3, 1, 0, 0],67:[3, 3, 1, 0, 0],77:[2, 2, 1, 0, 2],49:[1, 2, 1, 1, 0],43:[4, 2, 1, 0, 0]}

#hhash = {}
#with open("dotaheroestraits.csv", 'r') as f:
#    for i in range(110):
#        cols = f.readline().strip().split(",")
#        hhash[cols[0]] = [int(x) for x in cols[1:6]]

#for k in heroes:
#    print("{0}:{1},".format(heroes[k], hhash[k]))
#print("ok")

trainingSize = 15000

def getFeatureVector(cols):
    row = [0] * (dims + 10)
    team1 = [0] * 5
    team2 = [0] * 5
    for c in cols[:5]:
        row[heroes[c]] = 1
        team1 = [ x+y for x,y in zip(team1,traits[heroes[c]])]
    for c in cols[5:10]:
        row[heroes[c]] = -1
        team2 = [ x+y for x,y in zip(team1,traits[heroes[c]])]
    row[dims:dims+5] = team1
    row[dims+5:dims+10] = team2
    return row
        

trainingX = []
trainingY = []

with open("dota2trainingdata.txt", 'r') as f:
    for i in range(trainingSize):
        cols = f.readline().strip().split(",")
        trainingX.append(getFeatureVector(cols))
        trainingY.append(int(cols[10]))

#print("training...")
#clf = LinearSVC(C=1.0, penalty='l2', dual=False)
#rfc = RandomForestClassifier(n_estimators=10, criterion='gini', min_samples_split=7, min_samples_leaf=4, max_features='auto')
#clf = AdaBoostClassifier(base_estimator=rfc, n_estimators=5, learning_rate=1.0, algorithm='SAMME.R', random_state=True)
clf = GradientBoostingClassifier(n_estimators=200, learning_rate=.7, max_depth=1)
clf.fit(trainingX, trainingY)
#print("done!")

n = int(input())
for i in range(n):
    cols = input().strip().split(",")
    print(clf.predict(getFeatureVector(cols))[0])
