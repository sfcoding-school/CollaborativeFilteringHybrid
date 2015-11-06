# coding=utf-8
import json  # The functions with an s take string parameters. The others take file streams
import random
import math
import sys
import operator
import os
from time import gmtime, strftime

numeroReviewDaControllare = 100
percentualeTestSet = 0.9
numeroAmici = 10  # mettere a None per non avere un minimo
faseFinale = False
filename = None  # "dataset/test/20151106072709"  # se faseFinale=True => bisogna mettere cartella
quanteReviewVoglioTogliere = 5

ratingPrediction = True

# ##################################################################################################################
# FUNZIONI UTILI
# ##################################################################################################################


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

# ##################################################################################################################
# CALCOLO TEST SET E SCOMPOSIZIONE IN FILE
# ##################################################################################################################

if not faseFinale:
    filename = "dataset/test/" + strftime("%Y%m%d%H%M%S", gmtime())
    print filename
    os.makedirs(filename)
    db = None
    try:
        with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
            db = json.load(data_file)
    except Exception, erx:
        raise erx

    print "ho aperto il file reviewUserRistoranti2.json"

    f = open(filename + "/testSet.json", 'ab')
    listaUtentiTestSet = []
    userAttuale = {}
    listUserId = {}
    i = 0  # mi serve a contare "l'ogni diecimila"
    j = 0  # server per salvare con nomi diversi i file

    # Scorro tutti gli utenti e li divido in sotto file di 10k utenti tagliando
    #  quelli con meno di 'numeroReviewDaControllare' e con probabilita data
    #  quelli ne metto alcuni nel testSet togliendo loro alcune recensioni

    for x in db:
        if i == 10000:
            text_file = open(filename + "/reviewUserRistorantiNew10_" + str(j) + ".json", "w")
            text_file.write(json.dumps(listUserId))
            text_file.close()
            i = 0
            j += 1
            print j
            listUserId = {}
        if random.random() > percentualeTestSet and len(db[x]["review"]) > numeroReviewDaControllare:
            userAttuale = {x: db[x], "keyRem": []}
            hoFinito = True

            # possibile problema è che io tolgo cose casuali, potrebbe avere senso togliere
            # solo cose con voti alti

            while hoFinito:
                for rev in db[x]["review"]:
                    if random.random() > 0.7:
                        userAttuale["keyRem"].append((rev, db[x]["review"][rev]))
                        db[x]["review"].pop(rev)
                        if len(userAttuale["keyRem"]) == quanteReviewVoglioTogliere:
                            hoFinito = False
                        break

            f.write(json.dumps(userAttuale) + "\n")  # aggiungo gli utenti al testSet
            userAttuale = {}
            listaUtentiTestSet.append(x)
            listUserId[x] = db[x]
            i += 1
        elif len(db[x]["review"]) > numeroReviewDaControllare:
            listUserId[x] = db[x]
            i += 1

    text_file = open(filename + "/reviewUserRistorantiNew10_" + str(j) + ".json", "w")
    text_file.write(json.dumps(listUserId))
    text_file.close()
    quantiNelTestSet = len(listaUtentiTestSet)
    numeroUtentiRispettoReview = 10000*j + len(listUserId)

    print "Numero utenti nel db con piu\' di " + str(numeroReviewDaControllare) + " review:", numeroUtentiRispettoReview
    print "Numero utenti nel TestSet:", quantiNelTestSet

    userAttuale = None
    listUserId = None

    # ##################################################################################################################
    # CALCOLO MATRICE SIMILARITA'
    # ##################################################################################################################

    f = open(filename + "/similarityMatrix0.json", 'ab')
    similarityMatrix = []
    quelloAttuale = {}
    similForJustOneUser = []
    miServivaUnBooleano = True
    mediaAttuale = 0.0
    who_2 = None
    i = j = k = z = 0
    while True:
        try:
            with open(filename + '/reviewUserRistorantiNew10_' + str(i) + '.json') as data_file:
                file_temp = json.load(data_file)
                for who_1 in file_temp:
                    if miServivaUnBooleano is True:
                        if (who_1 not in similarityMatrix) and (who_1 in listaUtentiTestSet):
                            j += 1
                            z += 1
                            who_2 = who_1
                            quelloAttuale = file_temp[who_2]
                            mediaAttuale = mean(file_temp[who_2]["review"])
                            miServivaUnBooleano = False
                    else:
                        similForJustOneUser.append((who_1, pearson(file_temp[who_1]["review"], quelloAttuale["review"], mean(file_temp[who_1]["review"]), mediaAttuale)))
                        if numeroAmici is not None and len(similForJustOneUser) > numeroAmici:
                            similForJustOneUser = sorted(similForJustOneUser, key=operator.itemgetter(1), reverse=True)
                            similForJustOneUser.pop()
                file_temp.clear()
            i += 1
            # print(i)
        except Exception:
            sys.stdout.write("\rn. %s  " % str(quantiNelTestSet - j))  # qualcosa non quadra in questi numeri quando si scende sotto il centinaio sembra
            sys.stdout.flush()
            f.write(json.dumps({"u": who_2, "m_s": similForJustOneUser}) + "\n")
            if z == 500:
                z = 0
                f.close()
                k += 1
                f = open(filename + "/similarityMatrix" + str(k) + ".json", 'ab')
            if j == quantiNelTestSet:
                break

            similarityMatrix.append(who_2)
            i = 0
            quelloAttuale = {}
            similForJustOneUser = []
            miServivaUnBooleano = True
            mediaAttuale = 0.0
            who_2 = None
            # dovrei salvare e svuotare la lista di tuple di quelli scelti

    print
    print "Ho finito di calcolare la matrice di similarita"

if not ratingPrediction:
    print "Calcolo lista consigliati"
    j = 0
    laForseFinalmenteFinale = {}
    z = 0
    while True:
        try:
            for line in open(filename + '/similarityMatrix' + str(z) + '.json', 'r'):
                j += 1
                try:
                    attuale = json.loads(line)  # {"u": , "m_s": }
                    chiSono = attuale["u"]
                    leMieReview = None
                    try:
                        with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
                            db = json.load(data_file)  # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
                            leMieReview = db[chiSono]["review"]
                    except Exception, e:
                        raise e

                        # la parte sotto assumo si possa riscrivere più velocmenete come sopra set(cosa.keys())
                    restToCheck = set()
                    leMieReviewSet = set()
                    for miaR in leMieReview:
                        leMieReviewSet.add(miaR)

                    for vicino in attuale["m_s"]:
                        for film in db[vicino[0]]["review"]:
                            restToCheck.add(film)
                    restToCheck = restToCheck.difference(leMieReviewSet)

                    laMiaMedia = mean(leMieReview)
                    dentroSum = 0.0
                    sumSotto = 0.0
                    listaAcasoHoFinitoINomi = []

                    for toCheck in restToCheck:
                        for vicino in attuale["m_s"]:
                            try:
                                dentroSum += vicino[1] * (db[vicino[0]]["review"][toCheck] - mean(db[vicino[0]]["review"]))
                                sumSotto += abs(vicino[1])
                            except KeyError:
                                pass
                        try:
                            p_u_i = laMiaMedia + (dentroSum)/(sumSotto)
                            listaAcasoHoFinitoINomi.append((toCheck, p_u_i))
                        except ZeroDivisionError, err:
                            raise err

                        if len(listaAcasoHoFinitoINomi) > 3:
                            listaAcasoHoFinitoINomi = sorted(listaAcasoHoFinitoINomi, key=operator.itemgetter(1), reverse=True)
                            listaAcasoHoFinitoINomi.pop()
                    if len(listaAcasoHoFinitoINomi) is not 0:
                        # print len(restToCheck), len(attuale["m_s"])
                        # print listaAcasoHoFinitoINomi
                        laForseFinalmenteFinale[chiSono] = listaAcasoHoFinitoINomi

                    sys.stdout.write("\rn. %s  " % str(quantiNelTestSet - j))  # qualcosa non quadra in questi numeri quando si scende sotto il centinaio sembra
                    sys.stdout.flush()
                except Exception, e:
                    pass
        except Exception:
            break
        z += 1
# ########################################################################################

    print "\ninizio controllo qualita", "len(laForseFinalmenteFinale):", len(laForseFinalmenteFinale)
    good = 0
    bad = 0
    totali = 0
    for line in open(filename + '/testSet.json', 'r'):
        totali += 1
        attuale = json.loads(line)  # {"IPnjMf7DdoYCJdKyYVEHOw": {"review": {"wcrmyGHXNWFXU0DiLq_Miw": 2,  ... "tLzitjzjjBSgdBpEE7Ae9w": 4}}, "keyRem": [["xXPf6scUPzkoQ4uBvqzy_w", 2], ["6ii5JM4zVaonwvgVcj9mfQ", 4], ["aidjBZ-cLeRmrsAPM6ZkcA", 4]]}

        for x in attuale:  # O(2)
            if x is not u"keyRem":
                try:
                    quelliCheTiHoDato = laForseFinalmenteFinale[x]  # [(u'id_rest', voto), ...]
                    listQuelliTolti = []
                    for iTolti in attuale["keyRem"]:
                        listQuelliTolti.append(iTolti[0])
                    for unoDiQuelli in quelliCheTiHoDato:  # O(3)
                        if unoDiQuelli[0] in listQuelliTolti:
                            good += 1
                        else:
                            bad += 1
                except Exception, e:
                    # print attuale
                    # print laForseFinalmenteFinale
                    pass

    print "\n\ntot", totali*3, "g", good, "b", bad
    f = open(filename + "/report", 'ab')
    f.write("Reccomender\nNumero review minimo richiesto" + str(numeroReviewDaControllare))
    f.write("Numero utenti con quelle review" + str(numeroUtentiRispettoReview))
    f.write("percentuale scelta testSet" + str(percentualeTestSet))
    f.write("Numero amici tenuto" + str(numeroAmici))
    f.write(str(quanteReviewVoglioTogliere))
    f.write("g" + str(good) + "b" + str(bad))

else:
    # Measuring Ratings Prediction Accuracy by RMSE (Root Mean Squared Error) and MAE (Mean Absolute Error)
    print "Rating Prediction"
    counterLine = 0
    RMSE = MAE = 0.0
    j = 1
    for lineTestSet in open(filename + '/testSet.json', 'r'):
        # Serve solo per bellezza
        sys.stdout.write("\rn. %s  " % j)
        sys.stdout.flush()
        j += 1

        # {"IPnjMf7DdoYCJdKyYVEHOw": {"review": {"wcrmyGHXNWFXU0DiLq_Miw": 2, ...}}, "keyRem": [["xXPf6scUPzkoQ4uBvqzy_w", 2], ...]}
        tizioTestSet = json.loads(lineTestSet)
        idTizioAttualeTestSet = None
        for key in tizioTestSet:  # O(2)
            if key != "keyRem":
                idTizioAttualeTestSet = key

        z = 0
        #for lineSimilMatrix in open(filename + '/similarityMatrix' + str(z) + '.json', 'r'):
        # # {"u": stringaIdUser, "m_s": [[u'fwEPqYiyZCaB7QKY6a7h5g', 1.0691015366119752], ...]}
        with open(filename + '/similarityMatrix' + str(z) + '.json') as f:
            for lineSimilMatrix in f:
                rigaAttualeSimilMatrix = json.loads(lineSimilMatrix)

                if idTizioAttualeTestSet == rigaAttualeSimilMatrix["u"]:
                    try:
                        with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
                            # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
                            db = json.load(data_file)
                    except Exception, e:
                        raise e

                    ristorantiTolti = []
                    for keyRimossa in tizioTestSet["keyRem"]:
                        ristorantiTolti.append((keyRimossa[0], keyRimossa[1]))  # (nome_rist, votoMio)

                    laMiaMedia = mean(tizioTestSet[idTizioAttualeTestSet]["review"])
                    for ristoranteTolto in ristorantiTolti:
                        dentroSum = sumSotto = 0.0
                        for vicino in rigaAttualeSimilMatrix["m_s"]:
                            try:
                                dentroSum += vicino[1] * (db[vicino[0]]["review"][ristoranteTolto[0]] - mean(db[vicino[0]]["review"]))
                                sumSotto += abs(vicino[1])
                                p_u_i = laMiaMedia + (dentroSum)/(sumSotto)
                                temp = float(p_u_i) - int(ristoranteTolto[1])
                                MAE += abs(temp)
                                RMSE += temp**2
                                counterLine += 1
                            except Exception, e:
                                pass  # non tutti il mio vicinato potrebbe aver recensito il mio stesso ristorante

    RMSE = math.sqrt(1.0/counterLine*RMSE)
    MAE = 1.0/counterLine*MAE
    # an MAE of 0.7 means that the algorithm, on average, was off by 0.7 stars
    sol = 'RMSE = ' + str(RMSE) + ' NRMSE = ' + str(RMSE/(5-1)) + ' MAE = ' + str(MAE) + ' NMAE = ' + str(MAE/(5-1))
    print sol
    # Scrittura Report
    print "Scrittura Report ..."
    f = open(filename + "/report", 'ab')
    f.write("RatingPrediction (" + filename.replace("dataset/test", "") + ")\nNumero review minimo richiesto: " + str(numeroReviewDaControllare))
    f.write("\nNumero utenti con quelle review: " + str(numeroUtentiRispettoReview))
    f.write("\npercentuale scelta testSet: " + str(percentualeTestSet))
    try:
        f.write("\nUtenti nel TestSet: " + str(quantiNelTestSet))
    except NameError:
        pass
    f.write("\nQuante review Voglio togliere: " + str(quanteReviewVoglioTogliere))
    f.write("\nNumero amici tenuto: " + str(numeroAmici))
    f.write("\n\nRisultato:\n" + sol)
