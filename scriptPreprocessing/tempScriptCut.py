import json

try:
    with open('userNew.json') as data_file:
        db = json.load(data_file)
except Exception, e:
    print e

print "done"

listUserId = []
i = 0
j = 0
for x in db:
    if i == 50000:
        text_file = open("userNewFolder/userNew" + str(j) + ".json", "w")
        text_file.write(json.dumps(listUserId))
        text_file.close()
        i = 0
        j += 1
        print j
        listUserId = []
    listUserId.append(x)
    i += 1

text_file = open("userNewFolder/userNew" + str(j) + ".json", "w")
text_file.write(json.dumps(listUserId))
text_file.close()
print i, len(listUserId)

print "done"
