import json

dbBussiness = []

for line in open('yelp_academic_dataset_business.json', 'r'):
    dbBussiness.append(json.loads(line))
print "done"

dbBussinessNew = []

# {"business_id": "vcNAWiLM4dR7D2nwwJ7nCA",
# "full_address": "4840 E Indian School Rd\nSte 101\nPhoenix, AZ 85018",
# "hours": {"Tuesday": {"close": "17:00", "open": "08:00"}, "Friday": {"close": "17:00", "open": "08:00"}, "Monday": {"close": "17:00", "open": "08:00"}, "Wednesday": {"close": "17:00", "open": "08:00"}, "Thursday": {"close": "17:00", "open": "08:00"}},
# "open": true,
# "categories": ["Doctors", "Health & Medical"],
# "city": "Phoenix", "review_count": 9, "name": "Eric Goldberg, MD",
# "neighborhoods": [],
# "longitude": -111.98375799999999,
# "state": "AZ", "stars": 3.5, "latitude": 33.499313000000001, "attributes": {"By Appointment Only": true}, "type": "business"}

for x in dbBussiness:
    dbBussinessNew.append({x["business_id"]: {"city": x["city"], "state": x["state"], "stars": x["stars"], "categories": x["categories"], "review_count": x["review_count"]}})

print "done"

text_file = open("bussinessNew.json", "w")
text_file.write(json.dumps(dbBussinessNew))
text_file.close()

print "saved"
