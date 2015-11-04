import json
import random

try:
    with open('dataset/reviewUserRistoranti2.json') as data_file:
        db = json.load(data_file)
except Exception, e:
    print e

print "done"

f = open("dataset/reviewUserRistoranti/testSet.json", 'ab')
userAttuale = {}
listUserId = {}
i = 0
j = 0
for x in db:
    if i == 10000:
        text_file = open("dataset/reviewUserRistoranti/reviewUserRistorantiNew" + str(j) + ".json", "w")
        text_file.write(json.dumps(listUserId))
        text_file.close()
        i = 0
        j += 1
        print j
        listUserId = {}
    if random.random() > 0.9:
        if len(db[x]["review"]) > 10:
            userAttuale = {x: db[x], "keyRem": []}
            hoFinito = True
            while hoFinito:
                for rev in db[x]["review"]:
                    print len(userAttuale["keyRem"])
                    if random.random() > 0.8:
                        userAttuale["keyRem"].append((rev, db[x]["review"][rev]))
                        db[x]["review"].pop(rev)
                        if len(userAttuale["keyRem"]) == 3:
                            hoFinito = False
                        break
                    
            f.write(json.dumps(userAttuale) + "\n")
            userAttuale = {}
    if len(db[x]["review"]) > 3:
        listUserId[x] = db[x]
        i += 1

text_file = open("dataset/reviewUserRistoranti/reviewUserRistorantiNew" + str(j) + ".json", "w")
text_file.write(json.dumps(listUserId))
text_file.close()
print i, len(listUserId)

print "done"
