# ##################################################################################################################
# FUNZIONI UTILI
# ##################################################################################################################

import math


# cosine similarity
def cosine_similarity(listaComune):
    sumxx = sumxy = sumyy = 0

    for i in range(len(listaComune)):
        temp1 = listaComune[i][0]
        temp2 = listaComune[i][1]
        sumxx += temp1*temp1
        sumyy += temp2*temp2
        sumxy += temp1*temp2
    return sumxy/math.sqrt(sumxx*sumyy)


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

    robaX = set(x.keys())
    robaY = set(y.keys())

    robaComune = robaX.intersection(robaY)

    if len(robaComune) is 0:
        return 0

    listaTemp = list(robaComune)

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
