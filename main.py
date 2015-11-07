# coding=utf-8

from time import gmtime, strftime
import os
# ------------------------------------

from utility.creazioneTestSet import calcoloTestSet
from utility.calcMatrixSimilarity import calcMatrixSimilarityUser, calcMatrixSimilarityItem
from utility.calcRatingPrediction import calcRatingPrediction

numeroReviewDaControllare = 100
percentualeTestSet = 0.9
numeroAmici = 10  # mettere a None per non avere un minimo
faseFinale = False
filename = None  # "dataset/test/20151106072709"  # se faseFinale=True => bisogna mettere cartella
quanteReviewVoglioTogliere = 5
ratingPrediction = True

if not faseFinale:

    filename = "dataset/test/" + strftime("%Y%m%d%H%M%S", gmtime())
    print filename
    os.makedirs(filename)

    quantiNelTestSet, listaUtentiTestSet, numeroUtentiRispettoReview = calcoloTestSet(filename, percentualeTestSet, numeroReviewDaControllare, quanteReviewVoglioTogliere)

    # calcMatrixSimilarityUser(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet)
    calcMatrixSimilarityItem(filename, listaUtentiTestSet, numeroAmici, quantiNelTestSet)

if ratingPrediction:
    calcRatingPrediction(filename, False)

    # # Scrittura Report
    # print "Scrittura Report ..."
    # f = open(filename + "/report", 'ab')
    # f.write("RatingPrediction (" + filename.replace("dataset/test", "") + ")\nNumero review minimo richiesto: " + str(numeroReviewDaControllare))
    # f.write("\nNumero utenti con quelle review: " + str(numeroUtentiRispettoReview))
    # f.write("\npercentuale scelta testSet: " + str(percentualeTestSet))
    # try:
    #     f.write("\nUtenti nel TestSet: " + str(quantiNelTestSet))
    # except NameError:
    #     pass
    # f.write("\nQuante review Voglio togliere: " + str(quanteReviewVoglioTogliere))
    # f.write("\nNumero amici tenuto: " + str(numeroAmici))


# ###########################à reccomender system ??? probabilmente da buttare
# if not ratingPrediction:
#     print "Calcolo lista consigliati"
#     j = 0
#     laForseFinalmenteFinale = {}
#     z = 0
#     while True:
#         try:
#             for line in open(filename + '/similarityMatrix' + str(z) + '.json', 'r'):
#                 j += 1
#                 try:
#                     attuale = json.loads(line)  # {"u": , "m_s": }
#                     chiSono = attuale["u"]
#                     leMieReview = None
#                     try:
#                         with open('dataset/dataset/reviewUserRistoranti2.json') as data_file:
#                             db = json.load(data_file)  # {"37zX6lAjvAFfgVuVaZ0OiQ": {"review": {"CVos739DJ06t8-dNiRMyeQ": 5, ... "9xoEUUPmh3JnSl9B9IglPA": 3}}
#                             leMieReview = db[chiSono]["review"]
#                     except Exception, e:
#                         raise e

#                         # la parte sotto assumo si possa riscrivere più velocmenete come sopra set(cosa.keys())
#                     restToCheck = set()
#                     leMieReviewSet = set()
#                     for miaR in leMieReview:
#                         leMieReviewSet.add(miaR)

#                     for vicino in attuale["m_s"]:
#                         for film in db[vicino[0]]["review"]:
#                             restToCheck.add(film)
#                     restToCheck = restToCheck.difference(leMieReviewSet)

#                     laMiaMedia = mean(leMieReview)
#                     dentroSum = 0.0
#                     sumSotto = 0.0
#                     listaAcasoHoFinitoINomi = []

#                     for toCheck in restToCheck:
#                         for vicino in attuale["m_s"]:
#                             try:
#                                 dentroSum += vicino[1] * (db[vicino[0]]["review"][toCheck] - mean(db[vicino[0]]["review"]))
#                                 sumSotto += abs(vicino[1])
#                             except KeyError:
#                                 pass
#                         try:
#                             p_u_i = laMiaMedia + (dentroSum)/(sumSotto)
#                             listaAcasoHoFinitoINomi.append((toCheck, p_u_i))
#                         except ZeroDivisionError, err:
#                             raise err

#                         if len(listaAcasoHoFinitoINomi) > 3:
#                             listaAcasoHoFinitoINomi = sorted(listaAcasoHoFinitoINomi, key=operator.itemgetter(1), reverse=True)
#                             listaAcasoHoFinitoINomi.pop()
#                     if len(listaAcasoHoFinitoINomi) is not 0:
#                         # print len(restToCheck), len(attuale["m_s"])
#                         # print listaAcasoHoFinitoINomi
#                         laForseFinalmenteFinale[chiSono] = listaAcasoHoFinitoINomi

#                     sys.stdout.write("\rn. %s  " % str(quantiNelTestSet - j))  # qualcosa non quadra in questi numeri quando si scende sotto il centinaio sembra
#                     sys.stdout.flush()
#                 except Exception, e:
#                     pass
#         except Exception:
#             break
#         z += 1
# # ########################################################################################

#     print "\ninizio controllo qualita", "len(laForseFinalmenteFinale):", len(laForseFinalmenteFinale)
#     good = 0
#     bad = 0
#     totali = 0
#     for line in open(filename + '/testSet.json', 'r'):
#         totali += 1
#         attuale = json.loads(line)  # {"IPnjMf7DdoYCJdKyYVEHOw": {"review": {"wcrmyGHXNWFXU0DiLq_Miw": 2,  ... "tLzitjzjjBSgdBpEE7Ae9w": 4}}, "keyRem": [["xXPf6scUPzkoQ4uBvqzy_w", 2], ["6ii5JM4zVaonwvgVcj9mfQ", 4], ["aidjBZ-cLeRmrsAPM6ZkcA", 4]]}

#         for x in attuale:  # O(2)
#             if x is not u"keyRem":
#                 try:
#                     quelliCheTiHoDato = laForseFinalmenteFinale[x]  # [(u'id_rest', voto), ...]
#                     listQuelliTolti = []
#                     for iTolti in attuale["keyRem"]:
#                         listQuelliTolti.append(iTolti[0])
#                     for unoDiQuelli in quelliCheTiHoDato:  # O(3)
#                         if unoDiQuelli[0] in listQuelliTolti:
#                             good += 1
#                         else:
#                             bad += 1
#                 except Exception, e:
#                     # print attuale
#                     # print laForseFinalmenteFinale
#                     pass

#     print "\n\ntot", totali*3, "g", good, "b", bad
#     f = open(filename + "/report", 'ab')
#     f.write("Reccomender\nNumero review minimo richiesto" + str(numeroReviewDaControllare))
#     f.write("Numero utenti con quelle review" + str(numeroUtentiRispettoReview))
#     f.write("percentuale scelta testSet" + str(percentualeTestSet))
#     f.write("Numero amici tenuto" + str(numeroAmici))
#     f.write(str(quanteReviewVoglioTogliere))
#     f.write("g" + str(good) + "b" + str(bad))