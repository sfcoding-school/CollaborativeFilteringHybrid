import json
import csv
from scipy.stats import t
import numpy as np
from numpy import average, std
from math import sqrt
from sys import argv

dbReviewUser = {}

with open("ml-100k/u.data", 'r') as SM:
    for line in csv.reader(SM, delimiter='\t',):
        try:
            dbReviewUser[line[0]].append((line[1], line[2]))
        except Exception, e:
            dbReviewUser[line[0]] = [(line[1], line[2])]

f = open("reviewUserMov100k100", 'ab')
i = 0
for utente in dbReviewUser:
    var = ""
    if len(dbReviewUser[utente]) > 100:
        i += 1
        var += str(utente) + ";"
        for review in dbReviewUser[utente]:
            var += str(review[0]) + "," + str(review[1]) + ";"
        f.write(var + "\n")
print i

# data = sorted(data, reverse=True)

# i = 1
# data2 = []
# f = open("casuale", 'ab')
# for x in data:
#     # data2.append((i, x))
#     # print "(" + i + "," + x + ")",
#     f.write("(" + str(i) + "," + str(x) + ")")
    # i += 1


# with open('file.csv', 'w') as f:
#     w = csv.writer(f)
#     for row in data2:
#         w.writerow(row)

# mean = average(data)
# # evaluate sample variance by setting delta degrees of freedom (ddof) to
# # 1. The degree used in calculations is N - ddof
# stddev = std(data, ddof=1)
# # Get the endpoints of the range that contains 95% of the distribution
# t_bounds = t.interval(0.95, len(data) - 1)
# # sum mean to the confidence interval
# ci = [mean + critval * stddev / sqrt(len(data)) for critval in t_bounds]
# # print "all Data:", data
# print "Mean:", mean
# print "Confidence Interval 95%%: %f, %f" % (ci[0], ci[1])

# a = np.array(data)
# print "Min:", np.min(a), "Max:", np.max(a)
# print "Percentile 25%", np.percentile(a, 25)
# print "Percentile 75%", np.percentile(a, 75)
# print 'Mediana', np.median(a)


# import json
# import csv
# import operator
# from scipy.stats import t
# import numpy as np
# from numpy import average, std
# from math import sqrt
# import csv
# from sys import argv

# try:
#     with open('dataset/reviewUserRistoranti2.json') as data_file:
#         dbReviewUser = json.load(data_file)
# except Exception, erx:
#     raise erx

# f = open("reviewUserRistorantiCpp50", 'ab')
# i = 0
# for utente in dbReviewUser:
#     var = ""
#     if len(dbReviewUser[utente]["review"]) > 50:
#         i += 1
#         var += utente + ";"
#         for review in dbReviewUser[utente]["review"]:
#             var += review + "," + str(dbReviewUser[utente]["review"][review]) + ";"
#         f.write(var + "\n")
# print i    

# # data = sorted(data, reverse=True)

# # i = 1
# # data2 = []
# # f = open("casuale", 'ab')
# # for x in data:
# #     # data2.append((i, x))
# #     # print "(" + i + "," + x + ")",
# #     f.write("(" + str(i) + "," + str(x) + ")")
#     # i += 1


# # with open('file.csv', 'w') as f:
# #     w = csv.writer(f)
# #     for row in data2:
# #         w.writerow(row)

# # mean = average(data)
# # # evaluate sample variance by setting delta degrees of freedom (ddof) to
# # # 1. The degree used in calculations is N - ddof
# # stddev = std(data, ddof=1)
# # # Get the endpoints of the range that contains 95% of the distribution
# # t_bounds = t.interval(0.95, len(data) - 1)
# # # sum mean to the confidence interval
# # ci = [mean + critval * stddev / sqrt(len(data)) for critval in t_bounds]
# # # print "all Data:", data
# # print "Mean:", mean
# # print "Confidence Interval 95%%: %f, %f" % (ci[0], ci[1])

# # a = np.array(data)
# # print "Min:", np.min(a), "Max:", np.max(a)
# # print "Percentile 25%", np.percentile(a, 25)
# # print "Percentile 75%", np.percentile(a, 75)
# # print 'Mediana', np.median(a)
