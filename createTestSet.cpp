#include "createTestSet.h"
#include "reccomender.h"
#include "matrixUser.h"
#include "matrixItem.h"
#include "ratingPrediction.h"
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

// creazione e inizializzazione della funzione random
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1, 100);
auto dice = std::bind(distribution, generator);
int myRandom(){
  return (dice()+dice()+dice())%100;
}

// questa funzione deve semplicemente caricare il file contenente il dataset e fare il parsing, caricandolo in memoria
//  sull'hashTable hashTableUserReview
// il dataset deve essere del tipo:
//  id_Utente;id_bussiness,voto_dato; ...
// un utente per ogni riga
void loadDataset(std::string who, std::unordered_map<std::string, std::unordered_map<std::string, double> > &hashTableUserReview){
    std::ifstream infile(who);
    std::string s;
    while (std::getline(infile, s))
    {
        std::istringstream ss(s);
        std::unordered_map<std::string, double> hashTableIdB_voto;
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

                std::string idBussiness;
                int voto;
                int booleanoVotoIdR = 0;
                while (!ss2.eof())         
                {
                    std::string x2;               
                    getline( ss2, x2, ',');  
                    if (booleanoVotoIdR)
                    {
                        voto = atoi(x2.c_str());
                        booleanoVotoIdR = 0;
                    } else {
                        idBussiness = x2;
                        booleanoVotoIdR = 1;
                    }
                }
                hashTableIdB_voto.insert({idBussiness, voto});
            }
        }
        hashTableUserReview.insert({idUt, hashTableIdB_voto});
    }
}

// reccomender system basato sulla rating prediction, verranno quindi calcolate prima tutte le predizioni e poi unite
//  tramite formula: \alpha rU + (1 - alpha) rI
void reccomenderRatingPredictionBased(std::unordered_map<std::string, std::unordered_map<std::string, double> > &hashTableUserReview,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
               double A[], std::ofstream &log){

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUser;
    log << "Inizio a creare MatrixSimilarityUser";
    creazioneMatriceUser(testSet, hashTableUserReview, matrixSimilarityUser);
    log << "\tFinito\n";

    std::set<std::string> listaBussiness;
    for ( auto it = hashTableUserReview.begin(); it != hashTableUserReview.end(); ++it )
    {
      for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
      {
        listaBussiness.insert(it2->first);
      }
    }

    log << "Creata listaBussiness: " << listaBussiness.size() << "\n";

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItem;
    log << "Inizio a creare MatrixSimilarityItem";
    creazioneMatriceItem(hashTableUserReview, matrixSimilarityItem, listaBussiness);
    log << "\tFinito\n";

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUser;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItem;

    // da settare localmente se si vuole usare la rating prediction user di tipo diverso
    //  se a false utilizza quella con l'aggiunta della standard deviation
    bool ratingPredictionBase = false; 
    log << "Inizio a calcolare calcoloRatingPrediction";
    calcoloRatingPrediction(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, 
                              matrixSimilarityItem, tolti, ratingPredictionBase);
    log << "\tFinito\n";    

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybrid;
    for (double i = 0.0; i <= 1; i+=0.2)
    {
        log << "alpha: " << i;
        ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, i);
        // calcolaQuantoSeiAndatoMale(predizioniHybrid, tolti, log);
        A[(int)(i*5)] += reccomender(predizioniHybrid, tolti, log);
        predizioniHybrid.clear();
    }
}

// questa funzione è praticamente come quella sopra ma calcola la rating prediction ibrida con la
//  formula inventata basata sulla grandezza del test set
void testAlphaParametrico(std::unordered_map<std::string, std::unordered_map<std::string, double> > &hashTableUserReview,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
               std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
               double A[], std::ofstream &log){

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityUser;
    creazioneMatriceUser(testSet, hashTableUserReview, matrixSimilarityUser);

    std::set<std::string> listaBussiness;
    for ( auto it = hashTableUserReview.begin(); it != hashTableUserReview.end(); ++it )
    {
      for ( auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2 )
      {
        listaBussiness.insert(it2->first);
      }
    }

    std::unordered_map<std::string, std::unordered_map<std::string, double> > matrixSimilarityItem;
    creazioneMatriceItem(hashTableUserReview, matrixSimilarityItem, listaBussiness);
    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniUser;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniItem;

    bool ratingPredictionBase = false;
    calcoloRatingPrediction(predizioniUser, predizioniItem, testSet, matrixSimilarityUser, 
    matrixSimilarityItem, tolti, ratingPredictionBase);

    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybrid;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > predizioniHybridHybrid;

    double gamma = std::pow( 0.5, ((double)testSet.size()/100)/10 );

    ratingPredictionHybrid(predizioniUser, predizioniItem, predizioniHybrid, 1 - (gamma/10));
    A[0] += reccomender(predizioniHybrid, tolti, log);    
}

// ////////////////////////////////////////////////////////////////////////////////////////////
//  MAIN
// ////////////////////////////////////////////////////////////////////////////////////////////

void creazioneTestSet(std::string who, int percentualeTestSet, bool metodoAlphaGeom, std::ofstream &log) {

    std::unordered_map<std::string, std::unordered_map<std::string, double> > hashTableUserReview;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > testSet;
    std::unordered_map<std::string, std::unordered_map<std::string, double> > tolti;

    //carico il file e inizializzo hashTableUserReview
    loadDataset(who, hashTableUserReview);
    log << "Ho caricato in memoria UserReview: " << hashTableUserReview.size() << "\n";

    // seleziono random il test set
    std::vector<std::string> tempScelti;
    for ( auto it = hashTableUserReview.begin(); it != hashTableUserReview.end(); ++it )
    {
       if (myRandom() > percentualeTestSet)
      {
        tempScelti.push_back(it->first);
      }
    }
    
    // questo array mi serve per fare la media delle percentuali di TP nei vari fold
    //  ha grandezza 6 poichè tento i vari \alpha da 0 a 1 con passo 0.2
    double A[6] = {0, 0, 0, 0, 0, 0};

    for (int foldAttuale = 0; foldAttuale < 5; ++foldAttuale)
    {
        log << "Inizio fold: " << foldAttuale << std::endl;
        testSet.clear();
        tolti.clear();

        for (int i = 0; i < tempScelti.size(); ++i)
        {
            // cerco l'utente nel dataset
            std::unordered_map<std::string, std::unordered_map<std::string, double> >::const_iterator got = hashTableUserReview.find (tempScelti[i]);
            
            // per ogni sua review (che sono sempre fisse)
            std::unordered_map<std::string, double> hashTableReviewTestSet;
            std::unordered_map<std::string, double> hashTableReviewTolte;
            int foldSize = (int)((got->second).size()/5);
            int j = 0; //serve per scorrere le fold

            for ( auto it = (got->second).begin(); it != (got->second).end(); ++it )
            {

                if (foldAttuale * foldSize <= j && j < (foldAttuale+1) * foldSize)
                {
                    // pusho su "hashTableReviewTolte"
                    hashTableReviewTolte.insert({it->first, it->second});
                } else {
                    // pusho su hashTableReviewTestSet
                    hashTableReviewTestSet.insert({it->first, it->second});
                }
                j++;
            }
            // tempScelti[i] è 'id dell'utente attuale
            testSet.insert({tempScelti[i], hashTableReviewTestSet});
            tolti.insert({tempScelti[i], hashTableReviewTolte});
        }

        // controllo per sicurezza che non siano vuoti
        if (testSet.size() && tolti.size())
        {
          if (metodoAlphaGeom)
          {
            // viene richiamata la funzione che, prima, come per la reccomenderRatingPredictionBased
            //  calcolerà la rating prediction base ma poi le unirà rispetto ad un valore alpha
            //  parametrico che segue una certa funzione dipendente dalla grandezza del test set
            testAlphaParametrico(hashTableUserReview, testSet, tolti, A, log);
          } else {
            // reccomender system basato su rating prediction
            reccomenderRatingPredictionBased(hashTableUserReview, testSet, tolti, A, log);
            log << "Finita fold: " << foldAttuale << std::endl;
          }
        }
    }

    // stampo sul file di log i risultati trovati
    log << "A\n";
    for (int i = 0; i < 6; ++i)
    {
        log << " alpha " << (double)i/5 << " : " << A[i] << " " << A[i]/5;
    }

    log << "\nFor Tex\n";
    for (int i = 0; i < 6; ++i)
    {
        log << i <<": (" << 100-percentualeTestSet << ", " <<  A[i]/5 << ") | ";
    }
}