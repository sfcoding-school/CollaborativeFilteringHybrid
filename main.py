# coding=utf-8

from time import gmtime, strftime
import os
# ------------------------------------

from utility.creazioneTestSet import calcoloTestSet
from utility.calcMatrixSimilarity import calcMatrixSimilarityUser, calcMatrixSimilarityItem
from utility.calcRatingPrediction import calcRatingPrediction
from utility.reccomenderHybrid import reccomenderHybrid, reccomenderHybridCheck, avreiPotutoCalc
from utility.report import scriviLog

numeroReviewDaControllare = 100
percentualeTestSet = 0.7
numeroAmici = 50  # mettere a None per non avere un minimo
faseFinale = True
filename = None if not faseFinale else "dataset/test/20151111115107"  # "dataset/test/20151109075839"  # se faseFinale=True => bisogna mettere cartella
quanteReviewVoglioTogliere = 5
ratingPrediction = False
reccomender = True
alpha1 = 1
alpha2 = 1

if not faseFinale:

    filename = "dataset/test/" + strftime("%Y%m%d%H%M%S", gmtime())

    scriviLog(filename, filename.replace("dataset/test/", "") + "\nNumero review minimo richiesto: " + str(numeroReviewDaControllare) +
    "\npercentualeTestSet: " + str(percentualeTestSet) +
    "\nNumero amici tenuto: " + str(numeroAmici) +
    "\nQuante review Voglio togliere: " + str(quanteReviewVoglioTogliere) +
    "\nAlpha1, Alpha2: " + str(alpha1) + ", " + str(alpha2)
    )

    print filename
    os.makedirs(filename)

    quantiNelTestSet, listaUtentiTestSet, numeroUtentiRispettoReview = calcoloTestSet(filename, percentualeTestSet, numeroReviewDaControllare, quanteReviewVoglioTogliere)

    scriviLog(filename, "numeroUtentiRispettoReview: " + str(numeroUtentiRispettoReview) + "\nquantiNelTestSet: " + str(quantiNelTestSet))

    calcMatrixSimilarityUser(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet)
    calcMatrixSimilarityItem(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet)

if ratingPrediction:
    calcRatingPrediction(filename, True)
    calcRatingPrediction(filename, False)

if reccomender:
    avreiPotuto = avreiPotutoCalc(filename)
    scriviLog(filename, "avreiPotuto" + str(avreiPotuto))
    print "avreiPotuto: " + str(avreiPotuto)
    # reccomenderHybrid(filename, quanteReviewVoglioTogliere, alpha1, alpha2)
    reccomenderHybridCheck(filename, quanteReviewVoglioTogliere, 1, 10)
    reccomenderHybridCheck(filename, quanteReviewVoglioTogliere, 10, 1)
    reccomenderHybridCheck(filename, quanteReviewVoglioTogliere, 5, 1)
    reccomenderHybridCheck(filename, quanteReviewVoglioTogliere, 1, 5)


print int(strftime("%Y%m%d%H%M%S", gmtime())) - int(filename.replace("dataset/test/", ""))
