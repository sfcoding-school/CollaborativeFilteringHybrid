#include "createTestSet.h"
#include "reccomender.h"
#include "matrixUser.h"
#include "matrixItem.h"
#include "ratingPrediction.h"
#include "cineseMethod.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <math.h>  
#include <iterator>

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1,100);
auto dice = std::bind ( distribution, generator );

int myRandom(){
  return (dice()+dice()+dice())%100;
}

void loadFile(std::string who, std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview){
  std::ifstream infile(who); // reviewUserRistorantiCpp50
    std::string s;
    while (std::getline(infile, s))
    {
        std::istringstream ss( s );
        std::unordered_map<std::string, double> vettoreIdReVoto;
        int booleanoIdUtente = 1;
        std::string idUt;
        while (!ss.eof())         
        {
          std::string x;               
          getline( ss, x, ';' );  
          std::istringstream ss2( x );

          if (booleanoIdUtente)
          {
              idUt = x;
              booleanoIdUtente = 0;
          } else {

              std::string idRest;
              int voto;
              int booleanoVotoIdR = 0;
                while (!ss2.eof())         
                {
                  std::string x2;               
                  getline( ss2, x2, ',' );  
                  if (booleanoVotoIdR)
                  {
                      voto = atoi(x2.c_str());
                      booleanoVotoIdR = 0;
                  } else {
                    idRest = x2;
                    booleanoVotoIdR = 1;
                  }
                }
                vettoreIdReVoto.insert({idRest, voto});
            }
        }
        vettoreUserReview.insert({idUt, vettoreIdReVoto});
        //std::cout << vettoreUserReview[0].first << " " << vettoreUserReview[0].second[0].first << " " << vettoreUserReview[0].second[0].second << "\n";    
    }
}

void odioTutti(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
               double A[], std::ofstream &log){

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUser;
    log << "Inizio a creare MatrixSimilarityUser";
    creazioneMatriceUser(testSet, vettoreUserReview, matrixSimilarityUser);
    log << "\tFinito\n";

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
    log << "Creata listaRistoranti: " << listaRistoranti.size() << "\n";

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItem;
    log << "Inizio a creare MatrixSimilarityItem";
    creazioneMatriceItem(testSet, matrixSimilarityItem, listaRistoranti);
    log << "\tFinito\n";

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUser;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItem;

    bool ratingPredictionBase = false;
    if (ratingPredictionBase)
    {
      log << "Inizio a calcolare calcoloRatingPrediction1";
      calcoloRatingPrediction(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, 
                              matrixSimilarityItem, tolti, listaRistoranti);
      log << "\tFinito\n";
    } else {
      //double mu = overallAverageRating(vettoreUserReview);
      log << "Inizio a calcolare calcoloRatingPrediction2";// (mu= " << mu << " )";
      calcoloRatingPrediction2(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, 
                              matrixSimilarityItem, tolti, listaRistoranti, 0);
      log << "\tFinito\n";
    }
    

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybrid;

    for (double i = 0.0; i <= 1; i+=0.2)
    {
        log << "alpha: " << i;
        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, i);
        calcolaQuantoSeiAndatoMale(predizioniHybrid, tolti, log);
        A[(int)(i*5)] += reccomender(predizioniHybrid, tolti, log);
        predizioniHybrid.clear();
    }
}

void odioTuttiCinesi(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
               double A[], std::ofstream &log){
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
        log << "Creata listaRistoranti: " << listaRistoranti.size() << "\n";

        std::unordered_map<std::string, std::tuple<double, double, double> > matriceVettoreCineseUser;
        std::unordered_map<std::string, std::tuple<double, double, double> > matriceVettoreCineseItem;
        creazioneVettoreCinese(testSet, matriceVettoreCineseUser, matriceVettoreCineseItem, listaRistoranti);
        log << "Ho finito di calcolare matriceVettoreCineseUser\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUserCinese;
        creazioneMatrixSimilarityCinese(matriceVettoreCineseUser, matrixSimilarityUserCinese);
        log << "Ho finito di calcolare matrixSimilarityUserCinese\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItemCinese;
        creazioneMatrixSimilarityCinese(matriceVettoreCineseItem, matrixSimilarityItemCinese);
        log << "Ho finito di calcolare matrixSimilarityItemCinese\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUserCinese;
        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItemCinese;
        calcoloRatingPredictionCinese(predizioniUserCinese, testSet, matrixSimilarityUserCinese, 
                                            matrixSimilarityItemCinese, predizioniItemCinese, tolti, listaRistoranti);
        log << "Ho finito calcoloRatingPredictionCinese\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybridCinese;

        for (double i = 0.0; i <= 1; i+=0.2)
        {
            log << "alpha: " << i;
            ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, i);
            calcolaQuantoSeiAndatoMale(predizioniHybridCinese, tolti, log);
            A[(int)(i*5)] += reccomender(predizioniHybridCinese, tolti, log);
            predizioniHybridCinese.clear();
        }
}

void stoCreandoUnMostroMezzoCinese(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
               double A[], std::ofstream &log){

  //------------------------------------------
  std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUser;
    log << "Inizio a creare MatrixSimilarityUser";
    creazioneMatriceUser(testSet, vettoreUserReview, matrixSimilarityUser);
    log << "\tFinito\n";

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
    log << "Creata listaRistoranti: " << listaRistoranti.size() << "\n";

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItem;
    log << "Inizio a creare MatrixSimilarityItem";
    creazioneMatriceItem(testSet, matrixSimilarityItem, listaRistoranti);
    log << "\tFinito\n";

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUser;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItem;

    bool ratingPredictionBase = false;
    if (ratingPredictionBase)
    {
      log << "Inizio a calcolare calcoloRatingPrediction1";
      calcoloRatingPrediction(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, 
                              matrixSimilarityItem, tolti, listaRistoranti);
      log << "\tFinito\n";
    } else {
      //double mu = overallAverageRating(vettoreUserReview);
      log << "Inizio a calcolare calcoloRatingPrediction2";// (mu= " << mu << " )";
      calcoloRatingPrediction2(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, 
                              matrixSimilarityItem, tolti, listaRistoranti, 0);
      log << "\tFinito\n";
    }
    

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybrid;

    
    
  //------------------------------------------

            std::unordered_map<std::string, std::tuple<double, double, double> > matriceVettoreCineseUser;
        std::unordered_map<std::string, std::tuple<double, double, double> > matriceVettoreCineseItem;
        creazioneVettoreCinese(testSet, matriceVettoreCineseUser, matriceVettoreCineseItem, listaRistoranti);
        log << "Ho finito di calcolare matriceVettoreCineseUser\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUserCinese;
        creazioneMatrixSimilarityCinese(matriceVettoreCineseUser, matrixSimilarityUserCinese);
        log << "Ho finito di calcolare matrixSimilarityUserCinese\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItemCinese;
        creazioneMatrixSimilarityCinese(matriceVettoreCineseItem, matrixSimilarityItemCinese);
        log << "Ho finito di calcolare matrixSimilarityItemCinese\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUserCinese;
        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItemCinese;
        calcoloRatingPredictionCinese(predizioniUserCinese, testSet, matrixSimilarityUserCinese, 
                                            matrixSimilarityItemCinese, predizioniItemCinese, tolti, listaRistoranti);
        log << "Ho finito calcoloRatingPredictionCinese\n";

        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybridCinese;
        

        //--------------------------------------------------------------
        std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybridHybrid;

        // for (double i = 0.0; i <= 1; i+=0.2)
        // {

        // }


        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, 0);
        ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, 0.2);
        ratingPredictionHybrid(predizioniHybrid, predizioniHybridCinese, predizioniHybridHybrid, 0.6);
        calcolaQuantoSeiAndatoMale(predizioniHybridHybrid, tolti, log);
        A[0] += reccomender(predizioniHybridHybrid, tolti, log);

        predizioniHybrid.clear();
        predizioniHybridCinese.clear();
        predizioniHybridHybrid.clear();

        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, 0.2);
        ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, 0.4);
        ratingPredictionHybrid(predizioniHybrid, predizioniHybridCinese, predizioniHybridHybrid, 0.8);
        calcolaQuantoSeiAndatoMale(predizioniHybridHybrid, tolti, log);
        A[1] += reccomender(predizioniHybridHybrid, tolti, log);

        predizioniHybrid.clear();
        predizioniHybridCinese.clear();
        predizioniHybridHybrid.clear();

        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, 0.8);
        ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, 0.4);
        ratingPredictionHybrid(predizioniHybrid, predizioniHybridCinese, predizioniHybridHybrid, 0.4);
        calcolaQuantoSeiAndatoMale(predizioniHybridHybrid, tolti, log);
        A[2] += reccomender(predizioniHybridHybrid, tolti, log);

        predizioniHybrid.clear();
        predizioniHybridCinese.clear();
        predizioniHybridHybrid.clear();

        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, 0.8);
        ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, 0.2);
        ratingPredictionHybrid(predizioniHybrid, predizioniHybridCinese, predizioniHybridHybrid, 0.6);
        calcolaQuantoSeiAndatoMale(predizioniHybridHybrid, tolti, log);
        A[3] += reccomender(predizioniHybridHybrid, tolti, log);

        predizioniHybrid.clear();
        predizioniHybridCinese.clear();
        predizioniHybridHybrid.clear();

        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, 0.8);
        ratingPredictionHybrid(predizioniUserCinese, predizioniItemCinese, predizioniHybridCinese, 0.2);
        ratingPredictionHybrid(predizioniHybrid, predizioniHybridCinese, predizioniHybridHybrid, 0.2);
        calcolaQuantoSeiAndatoMale(predizioniHybridHybrid, tolti, log);
        A[4] += reccomender(predizioniHybridHybrid, tolti, log);
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

void creazioneTestSet(std::string who, int percentualeTestSet, bool chineseMethod, std::ofstream &log) {

    std::unordered_map<std::string, std::unordered_map<std::string, double> > vettoreUserReview;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > testSet;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > tolti;

    loadFile(who, vettoreUserReview);
    log << "Ho caricato in memoria UserReview: " << vettoreUserReview.size() << "\n";

    std::vector<std::string> tempScelti;

    for ( auto it = vettoreUserReview.begin(); it != vettoreUserReview.end(); ++it )
    {
       if (myRandom() > percentualeTestSet)
      {
        tempScelti.push_back(it->first);
      }
    }
    
    double A[6] = {0, 0, 0, 0, 0, 0};

    for (int foldAttuale = 0; foldAttuale < 5; ++foldAttuale)
    {
        log << "Inizio fold: " << foldAttuale << std::endl;
        testSet.clear();
        tolti.clear();
        for (int i = 0; i < tempScelti.size(); ++i)
        {
            // cerco l'utente nel dataset
            std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator got = vettoreUserReview.find (tempScelti[i]);
            
            // per ogni sua review (che sono sempre fisse)
            std::unordered_map<std::string, double> vettoreReviewTestSet;
            std::unordered_map<std::string, double> vettoreReviewTolti;
            int foldSize = (int)((got->second).size()/5);
            int j = 0;

            for ( auto it = (got->second).begin(); it != (got->second).end(); ++it )
            {

                if (foldAttuale * foldSize <= j && j < (foldAttuale+1) * foldSize)
                {
                    // pusho su "vettoreReviewTolti"
                    vettoreReviewTolti.insert({it->first, it->second});
                } else {
                    // pusho su vettoreReviewTestSet
                    vettoreReviewTestSet.insert({it->first, it->second});
                }
                j++;
            }
            testSet.insert({tempScelti[i], vettoreReviewTestSet});
            tolti.insert({tempScelti[i], vettoreReviewTolti});
        }

        if (testSet.size() && tolti.size())
        {
          if (chineseMethod)
          {
            //odioTuttiCinesi(vettoreUserReview, testSet, tolti, A, log);
            stoCreandoUnMostroMezzoCinese(vettoreUserReview, testSet, tolti, A, log);
          } else {
            odioTutti(vettoreUserReview, testSet, tolti, A, log);
            log << "Finita fold: " << foldAttuale << std::endl;
          }
        }
    }

    log << "A\n";
    for (int i = 0; i < 6; ++i)
    {
        log << " alpha " << (double)i/5 << " : " << A[i] << " " << A[i]/5;
    }

    log << "\nFor Tex\n";
    for (int i = 0; i < 6; ++i)
    {
        log << "(" << 100-percentualeTestSet << ", " <<  A[i]/5 << ") ";
    }
}