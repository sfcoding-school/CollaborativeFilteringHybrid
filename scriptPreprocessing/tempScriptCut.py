import json

try:
    with open('reviewUserRistoranti2.json') as data_file:
        db = json.load(data_file)
except Exception, e:
    print e

print "done"

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
    if len(db[x]["review"]) is not 0:
        listUserId[x] = db[x]
    i += 1

text_file = open("dataset/reviewUserRistoranti/reviewUserRistorantiNew" + str(j) + ".json", "w")
text_file.write(json.dumps(listUserId))
text_file.close()
print i, len(listUserId)

print "done"
