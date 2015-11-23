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

double calcoloSimilaritaItem(std::string rist_i, std::string rist_j, 
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet)
{ 
  std::vector< std::pair<int, int> > vettoriVotiRistorantiInComune;
  for ( auto it = testSet.begin(); it != testSet.end(); ++it )
  {
    int voto1 = 0, voto2 = 0; 
    std::unordered_map<std::string, double>::const_iterator got = (it->second).find (rist_i);
    std::unordered_map<std::string, double>::const_iterator got2 = (it->second).find (rist_j);

    if ( got != (it->second).end() && got2 != (it->second).end() )
    {
      voto1 = got->second;
      voto2 = got2->second;
      vettoriVotiRistorantiInComune.push_back({voto1, voto2});
    }
  }

  if(!vettoriVotiRistorantiInComune.size()){
    return -99;
  }

  return cosineSimilarity(vettoriVotiRistorantiInComune);
}

void creazioneMatriceItem(std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                          std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                          std::set<std::string> &listaRistoranti)
{
    for(auto rist_i : listaRistoranti) {
      std::unordered_map<std::string, double> hashTableMieiSimili;
      for(auto rist_j : listaRistoranti) {
        if (rist_j != "" && rist_i != rist_j)
        {
          double cosine_i_j = calcoloSimilaritaItem(rist_i, rist_j, testSet);
          
          if ( cosine_i_j != -99)
          {
              hashTableMieiSimili.insert({rist_j, cosine_i_j});
          }
        }
      }
      matrixSimilarityItem.insert({rist_i, hashTableMieiSimili});
    } 
}