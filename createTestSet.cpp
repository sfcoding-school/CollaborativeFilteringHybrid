#include "createTestSet.h"
#include "vectorUtility.h"
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

// -------------------------------------------------------------------------------------
void creazioneTestSet(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview,
                      std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                      std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
                      int percentualeTestSet) {

    std::ifstream infile("reviewUserRistorantiCpp");
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

    std::cout << "Ho caricato in memoria UserReview: " << vettoreUserReview.size() << "\n";

    for ( auto it = vettoreUserReview.begin(); it != vettoreUserReview.end(); ++it )
    {
       if (myRandom() > percentualeTestSet)
      {
        std::unordered_map<std::string, double> vettoreReviewTestSet;
        std::unordered_map<std::string, double> vettoreReviewTolti;
        int foldSize = (int)((it->second).size()/5);
        int j = 0;
        for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
        {
          if (j <= foldSize)
          {
            // pusho su "vettoreReviewTolti"
            vettoreReviewTolti.insert({it2->first, it2->second});
          } else {
            // pusho su vettoreReviewTestSet
            vettoreReviewTestSet.insert({it2->first, it2->second});
          }
          j++;
        }
        testSet.insert({it->first, vettoreReviewTestSet});
        tolti.insert({it->first, vettoreReviewTolti});
      }
    }

    if (testSet.size() != tolti.size() )
    {
      std::cout << "\nQualcosa è andato storto nella creazione del testSet\n";
      return;
    }
    std::cout << "Ho creato il testSet: " << testSet.size() << " ";
    return;
} // fine funzione