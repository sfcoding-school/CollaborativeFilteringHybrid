import json
import sys
from utility import *
import math


def calcRatingPrediction(filename, userBased):
    # Measuring Ratings Prediction Accuracy by RMSE (Root Mean Squared Error) and MAE (Mean Absolute Error)
    print "Rating Prediction"
    counterLine = 0
    RMSE = MAE = 0.0
    j = 1

    try:
        with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
            # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
            db = json.load(data_file)
    except Exception, e:
        raise e

    for lineTestSet in open(filename + '/testSet.json', 'r'):
        # Serve solo per bellezza
        sys.stdout.write("\rn. %s  " % j)
        sys.stdout.flush()
        j += 1

        # {"IPnjMf7DdoYCJdKyYVEHOw": {"review": {"wcrmyGHXNWFXU0DiLq_Miw": 2, ...}}, "keyRem": [["xXPf6scUPzkoQ4uBvqzy_w", 2], ...]}
        tizioTestSet = json.loads(lineTestSet)
        idTizioAttualeTestSet = None
        laMiaMedia = 0.0
        for key in tizioTestSet:  # O(2)
            if key != "keyRem":
                idTizioAttualeTestSet = key

        z = 0
        # for lineSimilMatrix in open(filename + '/similarityMatrix' + str(z) + '.json', 'r'):
        # # {"u": stringaIdUser, "m_s": [[u'fwEPqYiyZCaB7QKY6a7h5g', 1.0691015366119752], ...]}
        qualeMatrice = "/similarityMatrixUserBased"
        if not userBased:
            qualeMatrice = "/similarityMatrixItemBased"

        with open(filename + qualeMatrice + str(z) + '.json') as f:
            for lineSimilMatrix in f:
                rigaAttualeSimilMatrix = json.loads(lineSimilMatrix)

                if idTizioAttualeTestSet == rigaAttualeSimilMatrix["u"]:
                    ristorantiTolti = []
                    for keyRimossa in tizioTestSet["keyRem"]:
                        ristorantiTolti.append((keyRimossa[0], keyRimossa[1]))  # (nome_rist, votoMio)

                    if userBased:
                        laMiaMedia = mean(tizioTestSet[idTizioAttualeTestSet]["review"])

                    for ristoranteTolto in ristorantiTolti:
                        dentroSum = sumSotto = 0.0
                        for vicino in rigaAttualeSimilMatrix["m_s"]:
                            try:
                                if userBased:
                                    altro = db[vicino[0]]["review"][ristoranteTolto[0]] - mean(db[vicino[0]]["review"])
                                else:
                                    altro = db[idTizioAttualeTestSet]["review"][ristoranteTolto[0]]

                                dentroSum += vicino[1] * altro
                                sumSotto += abs(vicino[1])
                                p_u_i = laMiaMedia + (dentroSum)/(sumSotto)  # RICONTROLLARE
                                temp = float(p_u_i) - int(ristoranteTolto[1])
                                MAE += abs(temp)
                                RMSE += temp**2
                                counterLine += 1
                            except Exception, e:
                                pass  # non tutti il mio vicinato potrebbe aver recensito il mio stesso ristorante

    try:
        RMSE = math.sqrt(1.0/counterLine*RMSE)
        MAE = 1.0/counterLine*MAE
    except ZeroDivisionError:
        print "ZeroDivisionError"
    # an MAE of 0.7 means that the algorithm, on average, was off by 0.7 stars
    sol = '\nRMSE = ' + str(RMSE) + ' NRMSE = ' + str(RMSE/(5-1)) + ' MAE = ' + str(MAE) + ' NMAE = ' + str(MAE/(5-1))
    print sol