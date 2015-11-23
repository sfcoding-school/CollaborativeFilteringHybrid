#include <unordered_map>
#include <set>

void ratingPredictionHybrid(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniHybrid,
                             double alpha);

void calcolaQuantoSeiAndatoMale(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioni,
    std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, std::ofstream &log);

void calcoloRatingPrediction(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityUser,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &matrixSimilarityItem,
                             std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, bool ratPred2
    );

double overallAverageRating(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview);