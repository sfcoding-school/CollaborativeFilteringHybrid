void creazioneVettoreCinese(std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet, 
                            std::unordered_map<std::string, std::tuple<double, double, double> > &matriceVettoreCineseUser,
                            std::unordered_map<std::string, std::tuple<double, double, double> > &matriceVettoreCineseItem,
                            std::set<std::string> &listaRistoranti);

void creazioneMatrixSimilarityCinese(std::unordered_map<std::string, std::tuple<double, double, double> > &matriceVettoreCinese,
                                    std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityCinese);

void calcoloRatingPredictionCinese(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUserCinese,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUserCinese,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti
    );