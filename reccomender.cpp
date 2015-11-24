#include "reccomender.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm> // std::sort

double reccomender(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniHybrid,
                   std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, std::ofstream &log
    ){

    std::unordered_map<std::string, std::vector<std::pair<std::string, double> > > iRaccomandati;

    // estrazione diversa ha il compito di estrarre non i primi (rispetto al voto) 5 tra i tolti
    //  ma tutti quelli con voto alto uguale purchè siano almeno 5
    bool estrazioneDiversa = false;
    if (estrazioneDiversa)
    {
        log << "reccomenderAlternativo!!!!!!!\n";
        for ( auto it = predizioniHybrid.begin(); it != predizioniHybrid.end(); ++it )
        {
            std::vector<std::pair<std::string, double> > v;
            for ( auto predizioni = (it->second).begin(); predizioni != (it->second).end(); ++predizioni )
                v.push_back({predizioni->first, predizioni->second});
            
            std::sort(v.begin(), v.end(), [](const std::pair<std::string, double>& lhs, const std::pair<std::string, double>& rhs) {
                                                return lhs.second < rhs.second; });
            std::reverse(v.begin(), v.end());
            std::vector<std::pair<std::string, double> > v2;
            double max = v[0].second;
            for (int i = 0; i < v.size(); ++i)
            {
                if ( v[i].second == max || i <= 5 )
                    v2.push_back(v[i]);
            }
            iRaccomandati.insert({it->first, v2});
        }
    } else {
        // estraggo i primi 5 una volta ordinato il tutto
        for ( auto it = predizioniHybrid.begin(); it != predizioniHybrid.end(); ++it )
        {
            std::vector<std::pair<std::string, double> > v;
            for ( auto predizioni = (it->second).begin(); predizioni != (it->second).end(); ++predizioni )
            {
                v.push_back({predizioni->first, predizioni->second});
                // lavoro sulla grandezza del vettore v (fissata a 5)
                //  solo se la sua size è maggiore di 5 allora ordino e faccio il pop (estraggo l'ultimo)
                //  in questo modo si ha: n·5·log(5) invece che n + n log(n) + 5
                if (v.size() > 5)
                {
                    if (v.size() > 5)
                    {
                        std::sort(v.begin(), v.end(), [](const std::pair<std::string, double>& lhs, const std::pair<std::string, double>& rhs) {
                                                            return lhs.second < rhs.second; });
                        std::reverse(v.begin(), v.end());
                        v.pop_back(); //tolgo l'ultimo
                    }
                }
            }
            iRaccomandati.insert({it->first, v});
        }
    }

    // estraggo i primi 5, solo questi verranno controllati per sapere se ho "consigliato"
    //  il giusto, in questo caso si è obbligati ad estrarre tutti i bussiness con voto
    //  maggiore, purchè siano almeno 5
    std::unordered_map<std::string, std::vector<std::pair<std::string, int> > > iToltiDaControllare;
    for ( auto it = tolti.begin(); it != tolti.end(); ++it )
    {
        std::vector<std::pair<std::string, int> > v;
        std::vector<std::pair<std::string, int> > quelliDaControllare;
        for ( auto tolte = (it->second).begin(); tolte != (it->second).end(); ++tolte )
            v.push_back({tolte->first, tolte->second});

        std::sort(v.begin(), v.end(), [](const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs) {
                                            return lhs.second < rhs.second; });
        std::reverse(v.begin(), v.end());
        int max = v[0].second;
        for (int i = 0; i < v.size(); ++i)
        {
            if (v[i].second == max || quelliDaControllare.size() < 6)
                quelliDaControllare.push_back(v[i]);
        }
        iToltiDaControllare.insert({it->first, quelliDaControllare});
    }

    // qui si controllano i TP e i FP
    //  fissato un utente lo si cerca nel hashTable iToltiDaControllare
    //  vengono poi creati due vettori di stringhe con solo i nomi dei bussiness, quelli tolti
    //  e quelli consigliati (oramai del voto ci interessa poco) (fissiamo tali grandezze n_1 e n_2)
    //  vengono poi creati altri due vettori (v1, v2) che dovranno essere esattamente della grandezza del 
    //  più grande dei 2, questo perchè con la modifica non possiamo assumere i vettori di lunghezza 
    //  fissata 5. Da qui si ordinano entrambi (il cui costo sarà ridotto a poichè "n_i log(n_i)" con n_i numero
    //  piccolo non ci preoccupa).
    //  Ora si fa l'intersezione e la differenza (insiemisticamente parlando) tra i 2 originali 
    //  usando v_1 e v_2 come contenitori del risultato
    //  ovviamente la grandezza dell'intersezione saranno i TP e quella della differenza i FP
    //  NB: .resize(...) è utile poichè il vettore si "sporca", guardare documentazione ufficiale per
    //      ulteriori chiarimenti
    int good = 0, bad = 0;
    for ( auto it = iRaccomandati.begin(); it != iRaccomandati.end(); ++it )
    {
        std::unordered_map<std::string, std::vector<std::pair<std::string, int> > > ::const_iterator got = iToltiDaControllare.find (it->first);
        if ( got != iToltiDaControllare.end() )
        {
            std::vector<std::string> iConsigliati;
            std::vector<std::string> iTolti;
            for (int i = 0; i < (it->second).size(); ++i)
                iConsigliati.push_back((it->second)[i].first);
            
            for (int i = 0; i < (got->second).size(); ++i)
                iTolti.push_back((got->second)[i].first);

            std::vector<std::string> v(((iConsigliati.size() < iTolti.size()) ? iTolti.size() : iConsigliati.size()));
            std::vector<std::string> v2(((iConsigliati.size() < iTolti.size()) ? iTolti.size() : iConsigliati.size()));                      // 0  0  0  0  0  0  0  0  0  0
            std::vector<std::string>::iterator it;
            std::sort (iConsigliati.begin(),iConsigliati.end());
            std::sort (iTolti.begin(),iTolti.end());

            it=std::set_intersection (iConsigliati.begin(), iConsigliati.end(), iTolti.begin(), iTolti.end(), v.begin());
            v.resize(it-v.begin());

            std::vector<std::string>::iterator it2;
            it2=std::set_difference(iConsigliati.begin(), iConsigliati.end(), 
            iTolti.begin(), iTolti.end(), 
            v2.begin());
            v2.resize(it2-v2.begin());
            good += v.size();
            bad += v2.size();
        }
    }
    log << "TP: " << good << " FP: " << bad << " " << ((double)100*good)/((double)good + bad) << std::endl;
    return ((double)100*good)/((double)good + bad);
}