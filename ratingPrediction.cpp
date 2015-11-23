#include "ratingPrediction.h"
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <math.h>  
#include <iterator>
#include <cmath> 
#include <fstream>

// //////////////////////////////////////////////////////////////////////
// FUNZIONI DI SUPPORTO
// //////////////////////////////////////////////////////////////////////

double calcolaMedia(std::unordered_map<std::string, double> leMieReview){
  int somma = 0;
   for ( auto it = leMieReview.begin(); it != leMieReview.end(); ++it )
  {
    somma += it->second;
  }
  return (double)somma/leMieReview.size();
}

double standardDeviation(std::unordered_map<std::string, double> leMieReview, double laMiaMedia, bool ratPred2){
    if (ratPred2)   return 1;
    double somma = 0;
    for ( auto it = leMieReview.begin(); it != leMieReview.end(); ++it )
    {
        somma += (it->second - laMiaMedia) * (it->second - laMiaMedia) ;
    }
    return std::sqrt((1.0/leMieReview.size()) * somma );
}

double overallAverageRating(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview){
    double somma = 0;
    double counter = 0;
    for ( auto it = vettoreUserReview.begin(); it != vettoreUserReview.end(); ++it )
    {
        for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
        {
            somma += it2->second;
            counter++;
        }
    }
    return somma/counter;
}

// serve a colcolare i valori RMSE e MAE
void calcolaQuantoSeiAndatoMale(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioni,
    std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, std::ofstream &log){
    double RMSE = 0, MAE = 0;
    int counterLine = 0;

    for ( auto utenteInPredizioni = predizioni.begin(); utenteInPredizioni != predizioni.end(); ++utenteInPredizioni )
    {
        std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator got = tolti.find (utenteInPredizioni->first);
        if ( got != tolti.end() ){
            for ( auto reviewTolte = (got->second).begin(); reviewTolte != (got->second).end(); ++reviewTolte )
            {
                std::unordered_map<std::string, double>::const_iterator got2 = (utenteInPredizioni->second).find (reviewTolte->first);
                if ( got2 != (utenteInPredizioni->second).end() ){
                    double tempPred = got2->second - reviewTolte->second;
                    MAE += std::abs(tempPred);
                    RMSE += tempPred * tempPred;
                    counterLine++;
                }
            }
        }
    }

    RMSE = std::sqrt((1.0/counterLine)*RMSE);
    MAE = (1.0/counterLine)*MAE;
    log << "\nRMSE: " << RMSE << " NRMSE: " << ((1.0/4)*RMSE) << " MAE: " << MAE << " NMAE: " << ((1.0/4)*MAE) << "\n";
}

// //////////////////////////////////////////////////////////////////////
// CALCOLO PREDIZIONE
// //////////////////////////////////////////////////////////////////////

void calcoloRatingPrediction(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, bool ratPred2
    ){

    for ( auto utenteNelTestSet = testSet.begin(); utenteNelTestSet != testSet.end(); ++utenteNelTestSet )
    {
        std::unordered_map<std::string, double> predizioniUserBasedTemp;
        std::unordered_map<std::string, double> predizioniItemBasedTemp;
        double media_u = calcolaMedia(utenteNelTestSet->second);

        // tolgo i miei ristoranti
        std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator doveSono = tolti.find (utenteNelTestSet->first);

        for(auto cosoTolto = (doveSono->second).begin(); cosoTolto != (doveSono->second).end(); ++cosoTolto)
        {
            std::string ristF = cosoTolto->first;
            //---- BLOCCO USERBASED
            double sopra = 0, sotto = 0;
            std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator got = matrixSimilarityUser.find (utenteNelTestSet->first);
            if ( got != matrixSimilarityUser.end() ){
                for ( auto mioSimile = (got->second).begin(); mioSimile != (got->second).end(); ++mioSimile )
                {
                    std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator mioSimileNelTestSet = testSet.find (mioSimile->first);
                    if ( mioSimileNelTestSet != testSet.end() ){
                        std::unordered_map<std::string, double>::const_iterator laSuaReview = (mioSimileNelTestSet->second).find (ristF);
                        if ( laSuaReview != (mioSimileNelTestSet->second).end() ){
                            double simil = mioSimile->second;
                            double laSuaMedia = calcolaMedia(mioSimileNelTestSet->second);
                            double ilSuoVoto = laSuaReview->second;
                            sopra += simil * ((ilSuoVoto - laSuaMedia)/standardDeviation(mioSimileNelTestSet->second, laSuaMedia, ratPred2));
                            sotto += std::abs(simil);
                        }
                    }
                }
                double p_u_i;
                if (sotto != 0 &&  sopra != 0)
                {
                  p_u_i = media_u + standardDeviation(utenteNelTestSet->second, media_u, ratPred2) * (sopra/sotto);
                  predizioniUserBasedTemp.insert({ristF, p_u_i});
                }
            }

            //---- BLOCCO ITEMBASED
            double sopraI = 0, sottoI = 0;
            got = matrixSimilarityItem.find (ristF);
            if ( got != matrixSimilarityItem.end() ){
                for ( auto mioSimile = (got->second).begin(); mioSimile != (got->second).end(); ++mioSimile )
                {
                    std::unordered_map<std::string, double>::const_iterator ilMioVotoSuQuelRistorante = (utenteNelTestSet->second).find (mioSimile->first);
                    if (ilMioVotoSuQuelRistorante != (utenteNelTestSet->second).end())
                    {
                        double simil = mioSimile->second;
                        sopraI += simil * (ilMioVotoSuQuelRistorante->second) ;
                        sottoI += std::abs(simil);
                    }
                }
            }

            double p_u_i;
            if (sottoI != 0 &&  sopraI != 0)
            {
                p_u_i = (sopraI/sottoI);
                predizioniItemBasedTemp.insert({ristF, p_u_i});
            }
        }
        predizioniItem.insert({utenteNelTestSet->first, predizioniItemBasedTemp});
        predizioniUser.insert({utenteNelTestSet->first, predizioniUserBasedTemp});
    }
}

// semplicemente scorre le predizioni fatte tramite user, le cerca tra le item e le unisce rispetto ad alpha
//  sull'hash table hybrid
void ratingPredictionHybrid(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniHybrid,
                             double alpha){
    for ( auto utenteInPredizioni = predizioniUser.begin(); utenteInPredizioni != predizioniUser.end(); ++utenteInPredizioni )
    {
        std::unordered_map<std::string, double> predTemp;
        std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator got = predizioniItem.find (utenteInPredizioni->first);
        if ( got != predizioniItem.end() ){
            for ( auto predizione = (utenteInPredizioni->second).begin(); predizione != (utenteInPredizioni->second).end(); ++predizione )
            {
                std::unordered_map<std::string, double>::const_iterator got2 = (got->second).find (predizione->first);
                if ( got2 != got->second.end() ){
                    double tttt = alpha * (predizione->second) + (1-alpha) * (got2->second);
                    predTemp.insert({predizione->first, tttt});
                }
            }
        }
        predizioniHybrid.insert({utenteInPredizioni->first, predTemp});
    } 
}