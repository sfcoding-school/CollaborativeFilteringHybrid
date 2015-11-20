import json

dbReview = []

for line in open('yelp_academic_dataset_review.json', 'r'):
    dbReview.append(json.loads(line))
print "done"

dbReviewNew = []

# {"votes": {"funny": 0, "useful": 2, "cool": 1},
# "user_id": "Xqd0DzHaiyRqVH3WRG7hzg",
# "review_id": "15SdjuK7DmYqUAj6rjGowg",
# "stars": 5,
# "date": "2007-05-17",
# "text": "dr. goldberg offers everything i look for in a general practitioner.  he's nice and easy to talk to without being patronizing; he's always on time in seeing his patients; he's affiliated with a top-notch hospital (nyu) which my parents have explained to me is very important in case something happens and you need surgery; and you can get referrals to see specialists without having to see him first.  really, what more do you need?  i'm sitting here trying to think of any complaints i have about him, but i'm really drawing a blank.",
# "type": "review", "business_id": "vcNAWiLM4dR7D2nwwJ7nCA"}


for x in dbReview:
    dbReviewNew.append({x["review_id"]: {"star": x["stars"], "buss_id": x["business_id"], "usr_id": x["user_id"]}})

print "done"

text_file = open("reviewNew.json", "w")
text_file.write(json.dumps(dbReviewNew))
text_file.close()

print "saved"
