#include "createTestSet.h"
#include <iostream>
#include <string>
#include <ios>
#include <fstream>
#include <time.h>       /* time_t, time, ctime */

int main(int argc, char *argv[])
{
    std::string logFile = "logfile.txt";

    time_t rawtime;
    time (&rawtime);

    int percentualeTestSet = 95;
    std::string who = "dataset/reviewUserRistorantiCpp";
    bool chineseMethod = 0;
    if (argc != 1)
    {
        who = argv[1];
        logFile = argv[2];
        
        if (argc > 3)
        {
           // percentualeTestSet = std::atoi(argv[3]);
            chineseMethod = argv[3];
        }
    }

    std::ofstream log(logFile, std::ios_base::app | std::ios_base::out);
    log << "ratPred 2 e tiro fuori best\n";
    for (int i = 95; i > 75; i-=5)
    {
        log << "-----------------------------------------------------------\n" << ctime (&rawtime) << std::endl;
        log << "who: " << who << std::endl;    
        log << "percentualeTestSet: " << i << std::endl;
        log << "chineseMethod: " << chineseMethod << std::endl;

        creazioneTestSet(who, i, chineseMethod, log);
        time (&rawtime);
        log << std::endl << ctime (&rawtime) << std::endl << "-----------------------------------------------------------\n";  
    }

    return 0;

    
    // if (chineseMethod)
    // {
    //     creazioneTestSet(who, vettoreUserReview, testSet, tolti, percentualeTestSet, 0);


    //     std::set<std::string> listaRistoranti;
    //     for ( auto it = testSet.begin(); it != testSet.end(); ++it )
    //     {
    //         for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
    //         {
    //             listaRistoranti.insert(it2->first);
    //         }
    //     }
    //     for ( auto it = tolti.begin(); it != tolti.end(); ++it )
    //     {
    //         for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
    //         {
    //             listaRistoranti.insert(it2->first);
    //         }
    //     }
    //     std::cout << "Creata listaRistoranti: " << listaRistoranti.size() << "\n";

    //     std::unordered_map<std::string, std::tuple<double, double, double> > matriceVettoreCineseUser;
    //     std::unordered_map<std::string, std::tuple<double, double, double> > matriceVettoreCineseItem;
    //     creazioneVettoreCinese(testSet, matriceVettoreCineseUser, matriceVettoreCineseItem, listaRistoranti);
    //     std::cout << "Ho finito di calcolare matriceVettoreCineseUser\n";

    //     std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUserCinese;
    //     creazioneMatrixSimilarityCinese(matriceVettoreCineseUser, matrixSimilarityUserCinese);
    //     std::cout << "Ho finito di calcolare matrixSimilarityUserCinese\n";

    //     std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItemCinese;
    //     creazioneMatrixSimilarityCinese(matriceVettoreCineseItem, matrixSimilarityItemCinese);
    //     std::cout << "Ho finito di calcolare matrixSimilarityItemCinese\n";

    //     std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUserCinese;
    //     std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItemCinese;
    //     calcoloRatingPredictionCinese(predizioniUserCinese, testSet, matrixSimilarityUserCinese, 
    //                                         matrixSimilarityItemCinese, predizioniItemCinese, tolti, listaRistoranti);
    //     std::cout << "Ho finito calcoloRatingPredictionCinese\n";

    //     std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybridCinese;

    //     for (double i = 0.0; i <= 1; i+=0.2)
    //     {
    //         std::cout << "alpha: " << i;
    //         ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, i);
    //         calcolaQuantoSeiAndatoMale(predizioniHybridCinese, tolti);
    //         reccomender(predizioniHybridCinese, tolti);
    //         predizioniHybridCinese.clear();
    //     }

    // }else{
       
    //         creazioneTestSet(who, vettoreUserReview, testSet, tolti, percentualeTestSet, quantiFold);
    //         std::cout << "-----------------------------------------------------------\n";     
    // }
}