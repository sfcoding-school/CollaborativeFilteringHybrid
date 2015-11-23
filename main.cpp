#include "createTestSet.h"
#include <iostream>
#include <string>
#include <ios>
#include <fstream>
#include <time.h> /* time_t, time, ctime */

int main(int argc, char *argv[])
{
    // /////////////////////////////////////////////////////////////
    //  INIZIALIZZAZIONE VARIABILI E LETTURA DA TERMINALE
    // /////////////////////////////////////////////////////////////
    //standard log File, percentualeTestSet e dataset da usare
    std::string logFile = "logfile.txt";
    int percentualeTestSet = 95;
    std::string who = "dataset/reviewUserRistorantiCpp";
    bool metodoAlphaGeom = 0;

    //inizializzo output sy file
    std::ofstream log(logFile, std::ios_base::app | std::ios_base::out);

    //inizializzo per timestamp
    time_t rawtime;
    time (&rawtime);

    //recupero input
    if (argc != 1)
    {
        who = argv[1];
        logFile = argv[2];
        
        if (argc > 3)
        {
            metodoAlphaGeom = argv[3];
        }
    }

    // /////////////////////////////////////////////////////////////
    //  INIZIO FASE TEST
    // /////////////////////////////////////////////////////////////
    // for per ogni percentuale di test set desiderata
    for (int i = 95; i > 75; i-=5)
    {
        log << "-----------------------------------------------------------\n" << ctime (&rawtime) << std::endl;
        log << "who: " << who << std::endl;    
        log << "percentualeTestSet: " << i << std::endl;
        log << "metodoAlphaGeom: " << metodoAlphaGeom << std::endl;

        creazioneTestSet(who, i, metodoAlphaGeom, log);
        time (&rawtime);
        log << std::endl << ctime (&rawtime) << std::endl << "-----------------------------------------------------------\n";  
    }

    return 0;
}