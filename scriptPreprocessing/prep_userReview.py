import json

qualiSonoRistoranti = []
with open('dataset/bussinessRestaurant.json') as data_file:
    qualiSonoRistoranti = (json.load(data_file))["bss_id"]

print len(qualiSonoRistoranti)

reviewSoloRistoranti = []
i = 0
while True:
    try:
        with open('dataset/reviewNewFolder/reviewNew' + str(i) + '.json') as data_file:
            # {"15SdjuK7DmYqUAj6rjGowg": {"buss_id": "vcNAWiLM4dR7D2nwwJ7nCA", "usr_id": "Xqd0DzHaiyRqVH3WRG7hzg", "star": 5}}
            for review in json.load(data_file):
                for x in review:  # O(1)
                    if review[x]["buss_id"] in qualiSonoRistoranti:
                        reviewSoloRistoranti.append(review)
        i += 1
        print i
    except Exception, e:
        print e
        break

print "done", i

text_file = open("userNew.json", "w")
text_file.write(json.dumps(reviewSoloRistoranti))
text_file.close()

print "saved", len(reviewSoloRistoranti)
