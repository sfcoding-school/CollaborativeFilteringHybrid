# coding=utf-8

import json  # The functions with an s take string parameters. The others take file streams
import random


def calcoloTestSet(filename, percentualeTestSet, numeroReviewDaControllare, quanteReviewVoglioTogliere):
    dbReviewUser = None
    try:
        with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
            dbReviewUser = json.load(data_file)
    except Exception, erx:
        raise erx

    print "ho aperto il file reviewUserRistoranti2.json"

    try:
        f = open(filename + "/testSet.json", 'ab')
        listaUtentiTestSet = []
        userAttuale = {}
        listUserId = {}
        i = 0  # mi serve a contare "l'ogni diecimila"
        j = 0  # server per salvare con nomi diversi i file

        # Scorro tutti gli utenti e li divido in sotto file di 10k utenti tagliando
        #  quelli con meno di 'numeroReviewDaControllare' e con probabilita data
        #  quelli ne metto alcuni nel testSet togliendo loro alcune recensioni

        for x in dbReviewUser:
            if i == 10000:
                text_file = open(filename + "/reviewUserRistorantiNew10_" + str(j) + ".json", "w")
                text_file.write(json.dumps(listUserId))
                text_file.close()
                i = 0
                j += 1
                print j
                listUserId = {}
            if random.random() > percentualeTestSet and len(dbReviewUser[x]["review"]) > numeroReviewDaControllare:
                userAttuale = {x: dbReviewUser[x], "keyRem": []}
                hoFinito = True

                # possibile problema è che io tolgo cose casuali, potrebbe avere senso togliere
                # solo cose con voti alti

                while hoFinito:
                    for rev in dbReviewUser[x]["review"]:
                        if random.random() > 0.7:
                            userAttuale["keyRem"].append((rev, dbReviewUser[x]["review"][rev]))
                            dbReviewUser[x]["review"].pop(rev)
                            if len(userAttuale["keyRem"]) == quanteReviewVoglioTogliere:
                                hoFinito = False
                            break

                f.write(json.dumps(userAttuale) + "\n")  # aggiungo gli utenti al testSet
                userAttuale = {}
                listaUtentiTestSet.append(x)
                listUserId[x] = dbReviewUser[x]
                i += 1
            elif len(dbReviewUser[x]["review"]) > numeroReviewDaControllare:
                listUserId[x] = dbReviewUser[x]
                i += 1

        text_file = open(filename + "/reviewUserRistorantiNew10_" + str(j) + ".json", "w")
        text_file.write(json.dumps(listUserId))
        text_file.close()
        quantiNelTestSet = len(listaUtentiTestSet)
        numeroUtentiRispettoReview = 10000*j + len(listUserId)

    except Exception, e:
        raise e

    quantiNelTestSet = len(listaUtentiTestSet)
    numeroUtentiRispettoReview = 10000*j + len(listUserId)

    print "Numero utenti nel db con piu\' di " + str(numeroReviewDaControllare) + " review:", numeroUtentiRispettoReview
    print "Numero utenti nel TestSet:", quantiNelTestSet
    return quantiNelTestSet, listaUtentiTestSet, numeroUtentiRispettoReview
