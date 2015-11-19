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

double calcolaMedia(std::unordered_map<std::string, double> leMieReview){
  int somma = 0;
   for ( auto it = leMieReview.begin(); it != leMieReview.end(); ++it )
  {
    somma += it->second;
  }
  return (double)somma/leMieReview.size();
}

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

void calcoloRatingPrediction(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
                             std::set<std::string> &listaRistoranti){
    
    for ( auto utenteNelTestSet = testSet.begin(); utenteNelTestSet != testSet.end(); ++utenteNelTestSet )
    {
        std::unordered_map<std::string, double> predizioniUserBasedTemp;
        std::unordered_map<std::string, double> predizioniItemBasedTemp;
        double media_u = calcolaMedia(utenteNelTestSet->second);

          // tolgo i miei ristoranti
        std::set<std::string> listaMieiRistoranti;
        std::set<std::string> listaRistorantiDifference;
        for ( auto ttt = (utenteNelTestSet->second).begin(); ttt != (utenteNelTestSet->second).end(); ++ttt )
        {
          listaMieiRistoranti.insert(ttt->first);
        }

        std::set_difference(listaRistoranti.begin(), listaRistoranti.end(), 
                        listaMieiRistoranti.begin(), listaMieiRistoranti.end(), 
                        std::inserter(listaRistorantiDifference, listaRistorantiDifference.begin()));

        // ----- ----- ----- ------- ----- 
        std::set<std::string>::iterator ristIt;
        for (ristIt = std::begin(listaRistorantiDifference); ristIt != std::end(listaRistorantiDifference); ++ristIt)
        {
            std::string ristF = *ristIt;
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
                            //qui ci entro
                            double simil = mioSimile->second;
                            double laSuaMedia = calcolaMedia(mioSimileNelTestSet->second);
                            double ilSuoVoto = laSuaReview->second;
                            sopra += simil * (ilSuoVoto - laSuaMedia);
                            sotto += std::abs(simil);
                        }
                    }
                }
                double p_u_i;
                if (sotto != 0 &&  sopra != 0)
                {
                  p_u_i = media_u + sopra/sotto;
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
                        sopraI += simil * ilMioVotoSuQuelRistorante->second ;
                        sottoI += std::abs(simil);
                    }
                }
            }

            double p_u_i;
            if (sottoI != 0 &&  sopraI != 0)
            {
                p_u_i = sopraI/sottoI;
                predizioniItemBasedTemp.insert({ristF, p_u_i});
            }
        }
        predizioniItem.insert({utenteNelTestSet->first, predizioniItemBasedTemp});
        predizioniUser.insert({utenteNelTestSet->first, predizioniUserBasedTemp});
    }
    //-------------------------------------------------------------------------------
    // std::cout << "Rating Prediction USERBASED";
    // calcolaQuantoSeiAndatoMale(predizioniUser, tolti);
    // std::cout << "\nRating Prediction ITEMBASED";
    // calcolaQuantoSeiAndatoMale(predizioniItem, tolti);
}

double standardDeviation(std::unordered_map<std::string, double> leMieReview, double laMiaMedia){
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

void calcoloRatingPrediction2(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
                             std::set<std::string> &listaRistoranti, double mu
    ){

    for ( auto utenteNelTestSet = testSet.begin(); utenteNelTestSet != testSet.end(); ++utenteNelTestSet )
    {
        std::unordered_map<std::string, double> predizioniUserBasedTemp;
        std::unordered_map<std::string, double> predizioniItemBasedTemp;
        double media_u = calcolaMedia(utenteNelTestSet->second);

          // tolgo i miei ristoranti
        std::set<std::string> listaMieiRistoranti;
        std::set<std::string> listaRistorantiDifference;
        for ( auto ttt = (utenteNelTestSet->second).begin(); ttt != (utenteNelTestSet->second).end(); ++ttt )
        {
          listaMieiRistoranti.insert(ttt->first);
        }

        std::set_difference(listaRistoranti.begin(), listaRistoranti.end(), 
                        listaMieiRistoranti.begin(), listaMieiRistoranti.end(), 
                        std::inserter(listaRistorantiDifference, listaRistorantiDifference.begin()));

        // ----- ----- ----- ------- ----- 
        std::set<std::string>::iterator ristIt;
        for (ristIt = std::begin(listaRistorantiDifference); ristIt != std::end(listaRistorantiDifference); ++ristIt)
        {
            std::string ristF = *ristIt;
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
                            //qui ci entro
                            double simil = mioSimile->second;
                            double laSuaMedia = calcolaMedia(mioSimileNelTestSet->second);
                            double ilSuoVoto = laSuaReview->second;
                            sopra += simil * ((ilSuoVoto - laSuaMedia)/standardDeviation(mioSimileNelTestSet->second, laSuaMedia));
                            sotto += std::abs(simil);
                        }
                    }
                }
                double p_u_i;
                if (sotto != 0 &&  sopra != 0)
                {
                  p_u_i = media_u + standardDeviation(utenteNelTestSet->second, media_u) * (sopra/sotto);
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
    //-------------------------------------------------------------------------------
    // std::cout << "Rating Prediction USERBASED";
    // calcolaQuantoSeiAndatoMale(predizioniUser, tolti);
    // std::cout << "\nRating Prediction ITEMBASED";
    // calcolaQuantoSeiAndatoMale(predizioniItem, tolti);
}

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