import json
import sys
from utility import *
import math
from report import scriviLog


def calcRatingPrediction(filename, userBased):
    # Measuring Ratings Prediction Accuracy by RMSE (Root Mean Squared Error) and MAE (Mean Absolute Error)
    print "Rating Prediction " + ("UserBased" if userBased else "ItemBased")
    scriviLog(filename, "Rating Prediction " + ("UserBased" if userBased else "ItemBased"))

    counterLine = 0
    RMSE = MAE = 0.0
    j = 1

    contents = open(filename + '/testSet.json', 'r').read()
    testSetUnito = [json.loads(str(item)) for item in contents.strip().split('\n')]

    qualeMatrice = "/similarityMatrixItemBased"
    if userBased:
        try:
            with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
                # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
                db = json.load(data_file)
        except Exception, e:
            raise e
        qualeMatrice = "/similarityMatrixUserBased"

    similarityMatrix = {}
    with open(filename + qualeMatrice + '0.json') as f:
        for line in f:
            temp = json.loads(line)
            similarityMatrix[temp["u"]] = temp["m_s"]

    print "Ho caricato tutto in memoria"

    for utente in testSetUnito:
        sys.stdout.write("\rn. %s  " % j)
        sys.stdout.flush()
        j += 1
        idTizioAttualeTestSet = [key for key in utente if key != 'keyRem'][0]
        laMiaMedia = 0.0

        ristorantiTolti = []
        for keyRimossa in utente["keyRem"]:
            ristorantiTolti.append((keyRimossa[0], keyRimossa[1]))  # (nome_rist, votoMio)

        if userBased:
            laMiaMedia = mean(utente[idTizioAttualeTestSet]["review"])

        for ristorante in ristorantiTolti:  # utente[idTizioAttualeTestSet]["review"]:
            altro = sumSotto = 0.0
            try:
                for vicino in similarityMatrix[(ristorante[0] if not userBased else idTizioAttualeTestSet)]:
                    try:
                        if userBased:
                            altro += db[vicino[0]]["review"][ristorante[0]] - mean(db[vicino[0]]["review"])  # da ricontrollare !!!!!!!!!!!!!!!!!!!!!!!!!!!
                        else:
                            altro += vicino[1] * utente[idTizioAttualeTestSet]["review"][vicino[0]]
                        sumSotto += abs(vicino[1])
                    except Exception:
                        pass
                try:
                    p_u_i = laMiaMedia + (altro)/(sumSotto)
                    temp = float(p_u_i) - int(ristorante[1])
                    MAE += abs(temp)
                    RMSE += temp**2
                    counterLine += 1
                except ZeroDivisionError:
                    pass
            except Exception, e:
                pass

    try:
        RMSE = math.sqrt(1.0/counterLine*RMSE)
        MAE = 1.0/counterLine*MAE
    except ZeroDivisionError:
        print "ZeroDivisionError", counterLine, RMSE, MAE
    # an MAE of 0.7 means that the algorithm, on average, was off by 0.7 stars
    sol = 'RMSE = ' + str(RMSE) + ' NRMSE = ' + str(RMSE/(5-1)) + ' MAE = ' + str(MAE) + ' NMAE = ' + str(MAE/(5-1))
    print
    print sol
    scriviLog(filename, sol)
