import json

dbUser = []

for line in open('yelp_academic_dataset_user.json', 'r'):
    dbUser.append(json.loads(line))
print "done"

# {
#   "yelping_since": "2010-10",
#   "votes": {"funny": 0,"useful": 6,"cool": 3},
#   "review_count": 4,
#   "name": "C",
#   "user_id": "KC6-fxngSzHFI37Z5R-1Jw",
#   "friends": [],
#   "fans": 0,
#   "average_stars": 2.25,
#   "type": "user", "compliments": {}, "elite": []}

dbUserNew = []

for x in dbUser:
    dbUserNew.append({x["user_id"]: {"av_star": x["average_stars"], "rev_count": x["review_count"]}})

print "done"

text_file = open("userNew.json", "w")
text_file.write(json.dumps(dbUserNew))
text_file.close()

print "saved"
