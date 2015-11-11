# coding=utf-8

import json
import sys
from utility import *
import operator
from report import scriviLog


def reccomenderHybrid(filename, quantiNeHoTolti, alpha1, alpha2):
    reccomenderHybridCalcolo(filename, quantiNeHoTolti, True)
    reccomenderHybridCalcolo(filename, quantiNeHoTolti, False)
    reccomenderHybridCheck(filename, quantiNeHoTolti, alpha1, alpha2)


def avreiPotutoCalc(filename):
    try:
        with open(filename + "/testP_UserBased.json") as data_file:
            # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
            userBasedPred = json.load(data_file)
        with open(filename + "/testP_ItemBased.json") as data_file:
            # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
            itemBasedPred = json.load(data_file)
    except Exception, e:
        raise e

    possibiliRaccomandabili = {}
    for user1 in userBasedPred:
        listaPossibili = []
        for ristRatPred in userBasedPred[user1]:
            for ristRatPred2 in itemBasedPred[user1]:
                if ristRatPred[0] == ristRatPred2[0]:
                    listaPossibili.append(ristRatPred[0])
        possibiliRaccomandabili[user1] = listaPossibili

    avreiPotuto = 0
    for lineTestSet in open(filename + '/testSet.json', 'r'):
        utenteAttualeTestSet = json.loads(lineTestSet)
        idUtenteAttuale = [key for key in utenteAttualeTestSet if key != 'keyRem'][0]
        ristorantiRimossi = []
        for x in utenteAttualeTestSet["keyRem"]:
            ristorantiRimossi.append(x[0])
        for x in possibiliRaccomandabili[idUtenteAttuale]:
            if x in ristorantiRimossi:
                avreiPotuto += 1
    return avreiPotuto


def reccomenderHybridCheck(filename, quantiNeHoTolti, alpha1, alpha2):
    try:
        with open(filename + "/testP_UserBased.json") as data_file:
            # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
            userBasedPred = json.load(data_file)
        with open(filename + "/testP_ItemBased.json") as data_file:
            # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
            itemBasedPred = json.load(data_file)
    except Exception, e:
        raise e

    raccomandati = {}
    for user1 in userBasedPred:
        listaFuturiRaccomandati = []
        for ristRatPred in userBasedPred[user1]:
            for ristRatPred2 in itemBasedPred[user1]:
                if ristRatPred[0] == ristRatPred2[0]:
                    listaFuturiRaccomandati.append((ristRatPred[0], alpha1 * ristRatPred[1] + alpha2 * ristRatPred2[1]))
                    if len(listaFuturiRaccomandati) > quantiNeHoTolti:
                        listaFuturiRaccomandati = sorted(listaFuturiRaccomandati, key=operator.itemgetter(1), reverse=True)
                        listaFuturiRaccomandati.pop()
        raccomandati[user1] = listaFuturiRaccomandati

    good = bad = 0
    for lineTestSet in open(filename + '/testSet.json', 'r'):
        utenteAttualeTestSet = json.loads(lineTestSet)
        idUtenteAttuale = [key for key in utenteAttualeTestSet if key != 'keyRem'][0]
        ristorantiRimossi = []
        for x in utenteAttualeTestSet["keyRem"]:
            ristorantiRimossi.append(x[0])
        for x in raccomandati[idUtenteAttuale]:
            if x[0] in ristorantiRimossi:
                good += 1
            else:
                bad += 1
    print "alpha1", str(alpha1), " alpha2", str(alpha2), "\tg", good, "b", bad
    scriviLog(filename, "\nalpha1 " + str(alpha1) + " alpha2 " + str(alpha2) + " \n" + "g " + str(good) + " b " + str(bad))


def reccomenderHybridCalcolo(filename, quantiNeHoTolti, userBased):
    print "Calcolo Rating Prediction per ReccomenderHybrid" + ("UserBased" if userBased else "ItemBased")
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

    listaRistoranti = set()
    for lineTestSet in open(filename + '/testSet.json', 'r'):
        utenteAttualeTestSet = json.loads(lineTestSet)
        idUtenteAttuale = [key for key in utenteAttualeTestSet if key != 'keyRem'][0]
        listaRistoranti |= set(utenteAttualeTestSet[idUtenteAttuale]["review"].keys())
        for idRistoranteRimosso in utenteAttualeTestSet["keyRem"]:
            listaRistoranti.add(idRistoranteRimosso[0])
    ristorantiTolti = list(listaRistoranti)
    listaRistoranti.clear()

    similarityMatrix = {}
    with open(filename + qualeMatrice + '0.json') as f:
        for line in f:
            temp = json.loads(line)
            similarityMatrix[temp["u"]] = temp["m_s"]

    print "Ho caricato tutto in memoria"

    predizioni = {}

    for utente in testSetUnito:
        sys.stdout.write("\rn. %s  " % j)
        sys.stdout.flush()
        j += 1
        idTizioAttualeTestSet = [key for key in utente if key != 'keyRem'][0]
        laMiaMedia = 0.0

        predizioni[idTizioAttualeTestSet] = []

        if userBased:
            laMiaMedia = mean(utente[idTizioAttualeTestSet]["review"])

        for ristorante in ristorantiTolti:
            altro = sumSotto = 0.0
            try:
                for vicino in similarityMatrix[(ristorante if not userBased else idTizioAttualeTestSet)]:
                    try:
                        if userBased:
                            altro += db[vicino[0]]["review"][ristorante] - mean(db[vicino[0]]["review"])  # da ricontrollare !!!!!!!!!!!!!!!!!!!!!!!!!!!
                        else:
                            altro += vicino[1] * utente[idTizioAttualeTestSet]["review"][vicino[0]]
                        sumSotto += abs(vicino[1])
                    except Exception:
                        pass
                try:
                    p_u_i = laMiaMedia + (altro)/(sumSotto)
                    predizioni[idTizioAttualeTestSet].append((ristorante, p_u_i))
                    # if len(predizioni[idTizioAttualeTestSet]) > quantiNeHoTolti*10:
                    #     predizioni[idTizioAttualeTestSet] = sorted(predizioni[idTizioAttualeTestSet], key=operator.itemgetter(1), reverse=True)
                    #     predizioni[idTizioAttualeTestSet].pop()
                except ZeroDivisionError:
                    pass
            except Exception, e:
                pass

    text_file = open(filename + "/testP_" + ("UserBased" if userBased else "ItemBased") + ".json", "w")
    text_file.write(json.dumps(predizioni))
    text_file.close()
    print
