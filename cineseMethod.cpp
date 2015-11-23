#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <math.h>  
#include <iterator>
#include <unordered_map>

double calcolaMediaQ(std::unordered_map<std::string, double> leMieReview){
  // std::cout << leMieReview.size() <<"\n";
  int somma = 0;
   for ( auto it = leMieReview.begin(); it != leMieReview.end(); ++it )
  {
    somma += it->second;
  }
  // std::cout << (double)somma/leMieReview.size() << "\n" ;
  return (double)somma/leMieReview.size();
}

double calcolaMediaR(std::string qualeRistorante, std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet){
    double somma = 0, counter = 0;

    for ( auto it = testSet.begin(); it != testSet.end(); ++it )
    {
        std::unordered_map<std::string, double>::const_iterator got = (it->second).find (qualeRistorante);
        if (got != (it->second).end())
        {
            somma += got->second;
            counter++;
        }
    }


    return (double)somma/counter;
}

double cosineSimilarity(std::tuple<double, double, double> a, std::tuple<double, double, double> b){
  double sotto_l = 0, sotto_r = 0, sopra = 0;
  
    double pezzo1 = std::get<0>(a);
    double pezzo2 = std::get<0>(b);
    sopra += pezzo1 * pezzo2;
    sotto_l += pezzo1*pezzo1;
    sotto_r += pezzo2*pezzo2;

    pezzo1 = std::get<1>(a);
    pezzo2 = std::get<1>(b);
    sopra += pezzo1 * pezzo2;
    sotto_l += pezzo1*pezzo1;
    sotto_r += pezzo2*pezzo2;

    pezzo1 = std::get<2>(a);
    pezzo2 = std::get<2>(b);
    sopra += pezzo1 * pezzo2;
    sotto_l += pezzo1*pezzo1;
    sotto_r += pezzo2*pezzo2;
  
//std::cout << sopra << " " << sotto_l << " " << sotto_r  << " " << (std::sqrt(sotto_l)) << " " << (std::sqrt(sotto_r)) << "\n";
  
  return (sopra)/((double)(std::sqrt(sotto_l))*(std::sqrt(sotto_r)));
}

void creazioneVettoreCinese(std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet, 
                            std::unordered_map<std::string, std::tuple<double, double, double> > &matriceVettoreCineseUser,
                            std::unordered_map<std::string, std::tuple<double, double, double> > &matriceVettoreCineseItem,
                            std::set<std::string> &listaRistoranti){

    for ( auto it = testSet.begin(); it != testSet.end(); ++it )
    {
        double mean = calcolaMediaQ(it->second), variance = 0;
        int counter = 0, max = 0, min = 0;
        for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
        {
            counter++;
            variance += (it2->second - mean) * (it2->second - mean);
            if (it2->second > max)
            {
                max = it2->second;
            }
            if (it2->second < min)
            {
                min = it2->second;
            }
        }

        variance = ((double)1/counter) * variance;
        double range = max - min;
        
        matriceVettoreCineseUser.insert({it->first, std::make_tuple(mean, variance, range)});
    }
    //----------------------------------
    for(auto rist_i : listaRistoranti) {
        //devo prima calcolare media
        double mean = calcolaMediaR(rist_i, testSet), variance = 0;
        int counter = 0, max = 0, min = 0;
        for ( auto it = testSet.begin(); it != testSet.end(); ++it )
        {
            std::unordered_map<std::string, double>::const_iterator got = (it->second).find (rist_i);
            if (got != (it->second).end())
            {
                variance += (got->second - mean) * (got->second - mean);
                counter++;
                //nel frattempo cerco max e min
                if (got->second > max)
                {
                    max = got->second;
                }
                if (got->second < min)
                {
                    min = got->second;
                }
            } 
        }

        if (counter != 0) // non tutti possono avere quel ristorante (?)
        {
            variance = ((double)1/counter) * variance;
            double range = max - min;
            matriceVettoreCineseItem.insert({rist_i, std::make_tuple(mean, variance, range)});
        }
    }
}

void creazioneMatrixSimilarityCinese(std::unordered_map<std::string, std::tuple<double, double, double> > &matriceVettoreCinese,
                                    std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityCinese){
    for ( auto it = matriceVettoreCinese.begin(); it != matriceVettoreCinese.end(); ++it )
    {
        std::unordered_map<std::string, double> temp;
        for ( auto it2 = matriceVettoreCinese.begin(); it2 != matriceVettoreCinese.end(); ++it2 )
        {
            if (it->first != it2->first)
            {
                double val = cosineSimilarity(it->second, it2->second);
                temp.insert({it2->first, val});
            }
        }
        matrixSimilarityCinese.insert({it->first, temp});
    }
}

void calcolaQuantoSeiAndatoMaleC(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioni,
    std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti){
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
    // std::cout << "\nRMSE: " << RMSE << " NRMSE: " << ((1.0/4)*RMSE) << " MAE: " << MAE << " NMAE: " << ((1.0/4)*MAE) << "\n";
}

double standardDeviation2(std::unordered_map<std::string, double> leMieReview, double laMiaMedia){
    double somma = 0;
    for ( auto it = leMieReview.begin(); it != leMieReview.end(); ++it )
    {
        somma += (it->second - laMiaMedia) * (it->second - laMiaMedia) ;
    }
    return std::sqrt((1.0/leMieReview.size()) * somma );
}

void calcoloRatingPredictionCinese(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUserCinese,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUserCinese,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItemCinese,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItemCinese,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti
    ){
    // std::cout << "Inizio a calcolare rating prediction Cinese";

    for ( auto utenteNelTestSet = testSet.begin(); utenteNelTestSet != testSet.end(); ++utenteNelTestSet )
    {
        std::unordered_map<std::string, double> predizioniUserBasedTemp;
        std::unordered_map<std::string, double> predizioniItemBasedTemp;
        double media_u = calcolaMediaQ(utenteNelTestSet->second);

          std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator doveSono = tolti.find (utenteNelTestSet->first);
        // ----- ----- ----- ------- ----- 
        for(auto cosoTolto = (doveSono->second).begin(); cosoTolto != (doveSono->second).end(); ++cosoTolto)
        {
            std::string ristF = cosoTolto->first;
            //---- BLOCCO USERBASED
            double sopra = 0, sotto = 0;
            std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator got = matrixSimilarityUserCinese.find (utenteNelTestSet->first);
            if ( got != matrixSimilarityUserCinese.end() ){
                for ( auto mioSimile = (got->second).begin(); mioSimile != (got->second).end(); ++mioSimile )
                {
                    std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator mioSimileNelTestSet = testSet.find (mioSimile->first);
                    if ( mioSimileNelTestSet != testSet.end() ){
                        std::unordered_map<std::string, double>::const_iterator laSuaReview = (mioSimileNelTestSet->second).find (ristF);
                        if ( laSuaReview != (mioSimileNelTestSet->second).end() ){
                            //qui ci entro
                            double simil = mioSimile->second;
                            double laSuaMedia = calcolaMediaQ(mioSimileNelTestSet->second);
                            double ilSuoVoto = laSuaReview->second;
                            sopra += simil * ((ilSuoVoto - laSuaMedia)/standardDeviation2(mioSimileNelTestSet->second, laSuaMedia));
                            sotto += std::abs(simil);
                        }
                    }
                }
                double p_u_i;
                if (sotto != 0 &&  sopra != 0)
                {
                  p_u_i = media_u + standardDeviation2(utenteNelTestSet->second, media_u) * (sopra/sotto);
                  // std::cout << p_u_i << "\n";
                  predizioniUserBasedTemp.insert({ristF, p_u_i});
                }
            }

            // //---- BLOCCO ITEMBASED
            double sopraI = 0, sottoI = 0;
            got = matrixSimilarityItemCinese.find (ristF);
            if ( got != matrixSimilarityItemCinese.end() ){

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
        predizioniItemCinese.insert({utenteNelTestSet->first, predizioniItemBasedTemp});
        predizioniUserCinese.insert({utenteNelTestSet->first, predizioniUserBasedTemp});
    }
    // std::cout << "\nFinito\n\n";
    //-------------------------------------------------------------------------------
    // std::cout << "Rating Prediction USERBASED";
    // calcolaQuantoSeiAndatoMaleC(predizioniUserCinese, tolti);
    // std::cout << "\nRating Prediction ITEMBASED";
    // calcolaQuantoSeiAndatoMaleC(predizioniItemCinese, tolti);
}