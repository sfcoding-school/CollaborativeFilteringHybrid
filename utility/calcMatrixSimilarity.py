# ##################################################################################################################
# CALCOLO MATRICE SIMILARITA'
# ##################################################################################################################

import sys
import json
import operator
from utility import *


def calcMatrixSimilarityUser(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet):
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
    print "Ho finito di calcolare la matrice di similarita per UserBased"


def calcMatrixSimilarityItem(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet):
    f = open(filename + "/similarityMatrixItemBased0.json", 'ab')

    listaRistoranti = set()
    for lineTestSet in open(filename + '/testSet.json', 'r'):
        utenteAttualeTestSet = json.loads(lineTestSet)
        idUtenteAttuale = [key for key in utenteAttualeTestSet if key != 'keyRem'][0]
        listaRistoranti |= set(utenteAttualeTestSet[idUtenteAttuale]["review"].keys())
        for idRistoranteRimosso in utenteAttualeTestSet["keyRem"]:
            listaRistoranti.add(idRistoranteRimosso[0])

    lenListaRistoranti = len(listaRistoranti)
    listaRistoranti = list(listaRistoranti)

    contents = open(filename + '/testSet.json', 'r').read()
    testSetUnito = [json.loads(str(item)) for item in contents.strip().split('\n')]

    i = 0
    for ristorante1 in listaRistoranti:
        i += 1
        sys.stdout.write("\rn. %s     " % str(lenListaRistoranti - i))
        sys.stdout.flush()
        listaMieiSimili = []
        for ristorante2 in listaRistoranti:
            if ristorante1 != ristorante2:
                listaComune = []
                for utente in testSetUnito:
                    idUtenteAttuale = [key for key in utente if key != 'keyRem'][0]
                    voto1 = voto2 = 0
                    try:
                        voto1 = utente[idUtenteAttuale]["review"][ristorante1]
                        voto2 = utente[idUtenteAttuale]["review"][ristorante2]
                        listaComune.append((voto1, voto2))
                    except Exception:
                        pass
                if len(listaComune) is not 0:
                    cosSim = cosine_similarity(listaComune)
                    # if cosSim is not 1.0:
                    listaMieiSimili.append((ristorante2, cosSim))
        f.write(json.dumps({"u": ristorante1, "m_s": listaMieiSimili}) + "\n")
    print
    print "Ho finito di calcolare la matrice di similarita per ItemBased"
