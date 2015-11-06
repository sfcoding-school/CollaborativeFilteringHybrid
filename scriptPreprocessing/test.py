import json
import math
import operator


# calcolo la media dei voti di un utente
def mean(x):
    sum = 0.0
    for i in x:
        sum += x[i]
    return sum / len(x)


# calcolo la sample standard deviation
def sampleStandardDeviation(x, mean_x, listaComune):
    sumv = 0.0
    for i in listaComune:
        sumv += (x[i] - mean_x)**2
    # return math.sqrt(sumv/(len(x)-1))
    return math.sqrt(sumv/(len(x)-1)) if len(x)-1 is not 0 else 0


# calcolo il PCC
def pearson(x, y, mean_x, mean_y):
    scorex = []
    scorey = []

    listaTemp = []
    for bssns in x:
        if bssns in y:
            listaTemp.append(bssns)

    if len(listaTemp) is 0:
        return 0

    stdDev_x = sampleStandardDeviation(x, mean_x, listaTemp)
    stdDev_y = sampleStandardDeviation(y, mean_y, listaTemp)

    for comune in listaTemp:
        try:
            scorex.append((x[comune] - mean_x)/stdDev_x)
            scorey.append((y[comune] - mean_y)/stdDev_y)
        except ZeroDivisionError:
            # print "we got a problem !!!!!!!!!!!!", e
            return 0

    # multiplies both lists together into 1 list (hence zip) and sums the whole list
    # using zip to iterate over two lists in parallel
    valoreDaRitornare = 0
    try:
        valoreDaRitornare = (sum([i*j for i, j in zip(scorex, scorey)]))/(len(x)-1)
    except Exception:
        pass
    return valoreDaRitornare

# primo = {
#     "NFBQGT-YmMCHvTSIEyX_1g": {
#     'review': {'ubfD6-I73lP00v0fkx-keQ': 5, 'rZbHg4ACfN3iShdsT47WKQ': 4, 'togliamoanchequesto': 4, 'KleJUKZjGFFhybrcpZIyHQ': 5}},
#     "bho": {
#     'review': {'ubfD6-I73lP00v0fkx-keQ': 5, 'rZbHg4ACfN3iShdsT47WKQ': 4, 'togliamosoloquesto': 4, 'KleJUKZjGFFhybrcpZIyHQ': 5}},
#     "asdrubale": {
#     'review': {'ubfD6-I73lP00v0fkx-keQ': 5, 'unaltrodifferente': 4, 'sWCH3c8XxBrF7aODVgQeQw': 4, 'KleJUKZjGFFhybrcpZIyHQ': 5}},
#     "ciao": {
#     'review': {'ubfD6-diverso-keQ': 3,  'sWCH3c8XxBrF7aODVgQeQw': 4, 'KleJUKZjGFFhybrcpZIyHQ': 5}}
#     }

# similarityMatrix = {}

# # # roba utile per dopo
# i = 0
# while True:
#     try:
#         with open('dataset/reviewUserRistoranti/reviewUserRistorantiNew' + str(i) + '.json') as data_file:
#             file_temp = json.load(data_file)
#             for who_1 in file_temp:  # O(n^2 - n)
#                 for who_2 in file_temp:
#                     if who_1 is not who_2:
#                         try:
#                             similarityMatrix[bytes(who_1, encoding="UTF-8")][bytes(who_2, encoding="UTF-8")] = pearson(file_temp[who_1]["review"], file_temp[who_2]["review"], mean(file_temp[who_1]["review"]), mean(file_temp[who_2]["review"]))
#                         except Exception:
#                             similarityMatrix[bytes(who_2, encoding="UTF-8")] = {bytes(who_2, encoding="UTF-8"): pearson(file_temp[who_1]["review"], file_temp[who_2]["review"], mean(file_temp[who_1]["review"]), mean(file_temp[who_2]["review"]))}
#             file_temp.clear()
#             data_file.clear()
#         # print similarityMatrix
#         i += 1
#         print(i)
#     except Exception:
#         break

f = open("dataset/similarityMatrix0.json", 'ab')
chiHoGiaFatto = open("chiHoGiaFatto.json", 'ab')

similarityMatrix = []
quelloAttuale = {}
similForJustOneUser = []
miServivaUnBooleano = True
mediaAttuale = 0.0
who_2 = None
# # roba utile per dopo
i = j = k = z = 0
while True:
    try:
        with open('dataset/reviewUserRistoranti/reviewUserRistorantiNew10_' + str(i) + '.json') as data_file:
            file_temp = json.load(data_file)
            for who_1 in file_temp:  # O(n^2 - n)
                if miServivaUnBooleano is True:
                    if who_1 not in similarityMatrix:
                        j += 1
                        z += 1
                        who_2 = who_1
                        quelloAttuale = file_temp[who_2]
                        mediaAttuale = mean(file_temp[who_2]["review"])
                        miServivaUnBooleano = False
                else:
                    similForJustOneUser.append((who_1, pearson(file_temp[who_1]["review"], quelloAttuale["review"], mean(file_temp[who_1]["review"]), mediaAttuale)))
                    # similForJustOneUser = sorted(similForJustOneUser, key=operator.itemgetter(1), reverse=True)
                    # if len(similForJustOneUser) > 1000: # ordinarla solo se e' grande senno' sto sprecando cicli cpu per niente
                    #     similForJustOneUser.pop()
            file_temp.clear()
        i += 1
        # print(i)
    except Exception, err:
        print j
        f.write(json.dumps({"u": who_2, "m_s": similForJustOneUser}) + "\n")
        if z == 500:
            z = 0
            f.close()
            k += 1
            f = open("dataset/similarityMatrix" + str(k) + ".json", 'ab')

        similarityMatrix.append(who_2)
        chiHoGiaFatto.write(str(who_2))
        i = 0
        quelloAttuale = {}
        similForJustOneUser = []
        miServivaUnBooleano = True
        mediaAttuale = 0.0
        who_2 = None
        # dovrei salvare e svuotare la lista di tuple di quelli scelti

print "-------", len(similarityMatrix)
