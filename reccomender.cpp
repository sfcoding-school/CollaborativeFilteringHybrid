#include "reccomender.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>    // std::sort

double reccomender(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniHybrid,
                std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, std::ofstream &log
    ){
    std::unordered_map<std::string, std::vector<std::pair<std::string, double> > > iRaccomandati;
    if (true)
    {
        log << "reccomenderAlternativo!!!!!!!\n";
       for ( auto it = predizioniHybrid.begin(); it != predizioniHybrid.end(); ++it )
      {
          std::vector<std::pair<std::string, double> > v;
          for ( auto predizioni = (it->second).begin(); predizioni != (it->second).end(); ++predizioni )
          {
              v.push_back({predizioni->first, predizioni->second});
          }
          std::sort(v.begin(), v.end(), [](const std::pair<std::string, double>& lhs, const std::pair<std::string, double>& rhs) {
                          return lhs.second < rhs.second; });
          std::reverse(v.begin(), v.end());
          std::vector<std::pair<std::string, double> > v2;
          double max = v[0].second;
          for (int i = 0; i < v.size(); ++i)
          {
            if ( v[i].second == max || i <= 5 )
            {
              v2.push_back(v[i]);
            }
          }
          iRaccomandati.insert({it->first, v2});
      }
    } else {
      for ( auto it = predizioniHybrid.begin(); it != predizioniHybrid.end(); ++it )
      {
          std::vector<std::pair<std::string, double> > v;
          for ( auto predizioni = (it->second).begin(); predizioni != (it->second).end(); ++predizioni )
          {
              v.push_back({predizioni->first, predizioni->second});
              if (v.size() > 5)
              {
                  if (v.size() > 5)
                  {
                      std::sort(v.begin(), v.end(), [](const std::pair<std::string, double>& lhs, const std::pair<std::string, double>& rhs) {
                          return lhs.second < rhs.second; });
                      std::reverse(v.begin(), v.end());
                      //tolgo l'ultimo
                      v.pop_back();
                  }
              }
          }
          iRaccomandati.insert({it->first, v});
      }
    }
    std::unordered_map<std::string, std::vector<std::pair<std::string, int> > > iToltiDaControllare;
    for ( auto it = tolti.begin(); it != tolti.end(); ++it )
    {
        std::vector<std::pair<std::string, int> > v;
        std::vector<std::pair<std::string, int> > quelliDaControllare;
        for ( auto tolte = (it->second).begin(); tolte != (it->second).end(); ++tolte )
        {
            v.push_back({tolte->first, tolte->second});
        }
        std::sort(v.begin(), v.end(), [](const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs) {
                        return lhs.second < rhs.second; });
        std::reverse(v.begin(), v.end());
        int max = v[0].second;
        for (int i = 0; i < v.size(); ++i)
        {
            if (v[i].second == max || quelliDaControllare.size() < 6)
            {
                quelliDaControllare.push_back(v[i]);
            }
        }
        iToltiDaControllare.insert({it->first, quelliDaControllare});
    }

    // poi controllare
    int good = 0, bad = 0;
     for ( auto it = iRaccomandati.begin(); it != iRaccomandati.end(); ++it )
     {
        std::unordered_map<std::string, std::vector<std::pair<std::string, int> > > ::const_iterator got = iToltiDaControllare.find (it->first);
        if ( got != iToltiDaControllare.end() )
        {
            std::vector<std::string> iConsigliati;
            std::vector<std::string> iTolti;
            for (int i = 0; i < (it->second).size(); ++i)
            {
                iConsigliati.push_back((it->second)[i].first);
            }
            for (int i = 0; i < (got->second).size(); ++i)
            {
                iTolti.push_back((got->second)[i].first);
            }
        
              std::vector<std::string> v(((iConsigliati.size() < iTolti.size()) ? iTolti.size() : iConsigliati.size()));
              std::vector<std::string> v2(((iConsigliati.size() < iTolti.size()) ? iTolti.size() : iConsigliati.size()));                      // 0  0  0  0  0  0  0  0  0  0
              std::vector<std::string>::iterator it;

              std::sort (iConsigliati.begin(),iConsigliati.end());     //  5 10 15 20 25
              std::sort (iTolti.begin(),iTolti.end());   // 10 20 30 40 50

              it=std::set_intersection (iConsigliati.begin(), iConsigliati.end(), iTolti.begin(), iTolti.end(), v.begin());
                                                           // 10 20 0  0  0  0  0  0  0  0
              v.resize(it-v.begin());                      // 10 20

              std::vector<std::string>::iterator it2;
              it2=std::set_difference(iConsigliati.begin(), iConsigliati.end(), 
                                    iTolti.begin(), iTolti.end(), 
                                    v2.begin());
               v2.resize(it2-v2.begin());
               good += v.size();
               bad += v2.size();
        }
     }
     //log << "good: " << good << " bad: " << bad << " " << ((double)100*good)/((double)good + bad) << std::endl;
     return ((double)100*good)/((double)good + bad);
}