import json

# try:
#     with open('bussinessRestaurant.json') as data_file:
#         db = json.load(data_file)
# except Exception, e:
#     print e

# print "done"

# for x in db:
#     var = json.load(x)
#     print var
#     # try:
#     #     cityList[var["city"]] += 1
#     # except Exception, e:
#     #     cityList[var["city"]] = 1

cityList = {}
stateList = {}

for line in open('yelp_academic_dataset_business.json', 'r'):
    var = json.loads(line)
    try:
        cityList[var["city"]] += 1
    except Exception, e:
        cityList[var["city"]] = 1
    try:
        stateList[var["state"]] += 1
    except Exception, e:
        stateList[var["state"]] = 1
print "done", len(cityList), len(stateList)
