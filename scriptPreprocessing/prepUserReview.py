# ## reviewNew
# {"15SdjuK7DmYqUAj6rjGowg": {"buss_id": "vcNAWiLM4dR7D2nwwJ7nCA", "usr_id": "Xqd0DzHaiyRqVH3WRG7hzg", "star": 5}}
# ## userNew
# {"QlECcMtGvK4tao0bhW7w-Q": {"rev_count": 9, "av_star": 2.75}}

# ## cosa voglio creare
# {"id_utente": {"rev_count": 9, "av_star": 2.75, "review": {"id_rest": votoDato}}}

import json

listaUser = {}
i = 0
while True:
    try:
        with open('dataset/userNewFolder/userNew' + str(i) + '.json') as data_file:
            # {"18kPq7GPye-YQ3LyKyAZPw": {"rev_count": 108, "av_star": 4.14}}
            for review in json.load(data_file):
                for x in review:  # O(1)
                    listaUser[x] = {"review": {}}
        i += 1
        print 8-i
    except Exception, e:
        break

print "-------"
i = 0
while True:
    try:
        with open('dataset/reviewSoloRistoranti/reviewSoloRistorantiNew' + str(i) + '.json') as data_file:
            # {"15SdjuK7DmYqUAj6rjGowg": {"buss_id": "vcNAWiLM4dR7D2nwwJ7nCA", "usr_id": "Xqd0DzHaiyRqVH3WRG7hzg", "star": 5}}
            for rest in json.load(data_file):
                for x in rest:  # O(1)
                    listaUser[rest[x]["usr_id"]]["review"][rest[x]["buss_id"]] = rest[x]["star"]
        i += 1
        print 20-i
    except Exception, e:
        break

print "done"

text_file = open("reviewUserRistoranti2.json", "w")
text_file.write(json.dumps(listaUser))
text_file.close()

print "saved"
