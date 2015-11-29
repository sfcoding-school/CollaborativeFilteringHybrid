#include "reccomender.h"
#include "matrixUser.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <math.h>  
#include <iterator>

double calcolaMedia(std::unordered_map<std::string, double> &leMieReview){
  int somma = 0;
  for ( auto it = leMieReview.begin(); it != leMieReview.end(); ++it )
  {
    somma += it->second;
  }
  return (double)somma/leMieReview.size();
}

// implementazione della correlazione di pearson
double pearsonCorrelation(std::unordered_map<std::string, double> & reviewU_i, std::unordered_map<std::string, double> & reviewU_j)
{
    double mediaU_i = calcolaMedia(reviewU_i);
    double mediaU_j = calcolaMedia(reviewU_j);
    double sotto_l = 0, sotto_r = 0, sopra = 0;
    bool nonAvevamoNienteInComune = false;

    for ( auto it = reviewU_i.begin(); it != reviewU_i.end(); ++it )
    {
        std::unordered_map<std::string, double>::const_iterator got = reviewU_i.find (it->first);

        if ( got == reviewU_i.end() )
            nonAvevamoNienteInComune = true;
        else{
            double pezzo1 = it->second - mediaU_i;
            double pezzo2 = got->second - mediaU_j;
            sopra += pezzo1 * pezzo2;
            sotto_l += pezzo1*pezzo1;
            sotto_r += pezzo2*pezzo2;
        }
    }

    if (nonAvevamoNienteInComune)
    {
        return -99; // flag qualsiasi 
    }

    return (sopra)/((double)(std::sqrt(sotto_l)) * (std::sqrt(sotto_r)));
} 

// per ogni utente nel test set (calcolo solo le righe del test set poichè a livello progetto sono le uniche
//  che bisogna controllare in caso contrario il primo for andrebbe cambiato) si calcola la sua correlazione
//  di pearson con qualunque (qui invece sono tutti!) altro utente 
void creazioneMatriceUser(std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                          std::unordered_map<std::string, std::unordered_map<std::string, double> > &hashTableUserReview,
                          std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUser)
{
    for ( auto it = testSet.begin(); it != testSet.end(); ++it )
    {
        std::unordered_map<std::string, double> vettoreMieiSimili;
        for ( auto it2 = hashTableUserReview.begin(); it2 != hashTableUserReview.end(); ++it2 )
        {
            if (it->first != it2->first)
            {
                double pearson_i_j = pearsonCorrelation(it->second, it2->second);
                if (pearson_i_j != -99)
                    vettoreMieiSimili.insert({it2->first, pearson_i_j, });
            }
        }
        matrixSimilarityUser.insert({it->first, vettoreMieiSimili});
    }
    return;
}