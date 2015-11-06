# ##################################################################################################################
# CALCOLO MATRICE SIMILARITA'
# ##################################################################################################################

import sys
import json
import operator
from utility import *


def calcMatrixSimilarity(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet):
    f = open(filename + "/similarityMatrixUserBased0.json", 'ab')
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
            sys.stdout.write("\rn. %s  " % str(quantiNelTestSet - j))
            sys.stdout.flush()
            f.write(json.dumps({"u": who_2, "m_s": similForJustOneUser}) + "\n")
            if z == 500:
                z = 0
                f.close()
                k += 1
                f = open(filename + "/similarityMatrixUserBased" + str(k) + ".json", 'ab')
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
