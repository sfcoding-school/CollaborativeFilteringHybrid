#include "reccomender.h"
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

// implementazione della cosine similarity
//  ogni coppia è un unico bussiness (di cui qui non mi serve sapere quale)
//  e la coppia sarà il voto dei due utenti in esame su quel bussiness
double cosineSimilarity(std::vector< std::pair<int, int> > & vettoreComune){
    double sotto_l = 0, sotto_r = 0, sopra = 0;
    for (int i = 0; i < vettoreComune.size(); ++i)
    {
        double pezzo1 = (double)vettoreComune[i].first;
        double pezzo2 = (double)vettoreComune[i].second;
        sopra += pezzo1 * pezzo2;
        sotto_l += pezzo1*pezzo1;
        sotto_r += pezzo2*pezzo2;
    }
    return (sopra)/((double)(std::sqrt(sotto_l)) * (std::sqrt(sotto_r)));
}

// questo metodo ha la funzione di trovari i bussiness in comune, creare il vettore di coppie
//  da passare alla funzione cosineSimilarity e infine tornare tale valore
double calcoloSimilaritaItem(std::string buss_i, std::string buss_j, 
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet)
{ 
  std::vector< std::pair<int, int> > vettoreBussinessInComune;
  for ( auto it = testSet.begin(); it != testSet.end(); ++it )
  {
    int voto1 = 0, voto2 = 0; 
    std::unordered_map<std::string, double>::const_iterator got = (it->second).find (buss_i);
    std::unordered_map<std::string, double>::const_iterator got2 = (it->second).find (buss_j);

    if ( got != (it->second).end() && got2 != (it->second).end() )
    {
      voto1 = got->second;
      voto2 = got2->second;
      vettoreBussinessInComune.push_back({voto1, voto2});
    }
  }

  if(!vettoreBussinessInComune.size()){
    return -99; // solito flag
  }

  return cosineSimilarity(vettoreBussinessInComune);
}

// questa è la funzione principale che viene richiamata
//  la sua unica funzione è scorrere tutti i bussiness, richiamare per ognuno il metodo
//  calcoloSimilaritaItem e se non si attiva il flag lo inserisci tra i simili
void creazioneMatriceItem(std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                          std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                          std::set<std::string> &listaBussiness)
{
    for(auto buss_i : listaBussiness) {
      std::unordered_map<std::string, double> hashTableMieiSimili;
      for(auto buss_j : listaBussiness) {
        if (buss_j != "" && buss_i != buss_j)
        {
          double cosine_i_j = calcoloSimilaritaItem(buss_i, buss_j, testSet);
          if ( cosine_i_j != -99)
          {
            hashTableMieiSimili.insert({buss_j, cosine_i_j});
          }
        }
      }
      matrixSimilarityItem.insert({buss_i, hashTableMieiSimili});
    } 
}