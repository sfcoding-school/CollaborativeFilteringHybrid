#include "createTestSet.h"
#include "vectorUtility.h"
#include "matrixUser.h"
#include "matrixItem.h"
#include "ratingPrediction.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <algorithm>    // std::sort

int main() {

    int percentualeTestSet = 90;

    std::unordered_map<std::string, std::unordered_map<std::string, double> > vettoreUserReview;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > testSet;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > tolti;

    std::cout << "percentualeTestSet: " << percentualeTestSet << std::endl;

    for (int quantiFold = 0; quantiFold < 5; ++quantiFold)
    {
        creazioneTestSet(vettoreUserReview, testSet, tolti, percentualeTestSet, quantiFold);
        std::cout << testSet.size() <<"\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUser;
        creazioneMatriceUser(testSet, vettoreUserReview, matrixSimilarityUser);

        std::set<std::string> listaRistoranti;
        for ( auto it = testSet.begin(); it != testSet.end(); ++it )
        {
          for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
          {
            listaRistoranti.insert(it2->first);
          }
        }
        for ( auto it = tolti.begin(); it != tolti.end(); ++it )
        {
          for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
          {
            listaRistoranti.insert(it2->first);
          }
        }
        std::cout << "Creata listaRistoranti: " << listaRistoranti.size() << "\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItem;
        creazioneMatriceItem(testSet, matrixSimilarityItem, listaRistoranti);

        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUser;
        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItem;
        calcoloRatingPrediction(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, matrixSimilarityItem, tolti, listaRistoranti);

        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybrid;

        for (double i = 0.0; i <= 1; i+=0.2)
        {
            std::cout << "alpha: " << i;
            ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, i);
            calcolaQuantoSeiAndatoMale(predizioniHybrid, tolti);
            reccomender(predizioniHybrid, tolti);
            predizioniHybrid.clear();
        }

        std::cout << "-----------------------------------------------------------\n";
    }
}   

// creazioneTestSet(vettoreUserReview, testSet, tolti, percentualeTestSet, 0);
//     std::cout << testSet.size() <<"\n";

//     std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUser;
//     creazioneMatriceUser(testSet, vettoreUserReview, matrixSimilarityUser);

//     std::set<std::string> listaRistoranti;
//     for ( auto it = testSet.begin(); it != testSet.end(); ++it )
//     {
//       for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
//       {
//         listaRistoranti.insert(it2->first);
//       }
//     }
//     for ( auto it = tolti.begin(); it != tolti.end(); ++it )
//     {
//       std::unordered_map<std::string, double> vettoreMieiSimili;
//       for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
//       {
//         listaRistoranti.insert(it2->first);
//       }
//     }
//     std::cout << "Creata listaRistoranti: " << listaRistoranti.size() << "\n";

//     std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItem;
//     creazioneMatriceItem(testSet, matrixSimilarityItem, listaRistoranti);

//     std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUser;
//     std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItem;
//     calcoloRatingPrediction(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, matrixSimilarityItem, tolti, listaRistoranti);

//     std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybrid;

//     for (double i = 0.0; i <= 1; i+=0.2)
//     {
//         std::cout << "alpha: " << i;
//         ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, i);
//         calcolaQuantoSeiAndatoMale(predizioniHybrid, tolti);
//         reccomender(predizioniHybrid, tolti);
//         predizioniHybrid.clear();
//     }