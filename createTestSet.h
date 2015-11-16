#include <unordered_map>
#include <string>

void creazioneTestSet(std::unordered_map<std::string, std::unordered_map<std::string, double> > &vettoreUserReview,
                      std::unordered_map<std::string, std::unordered_map<std::string, double> > &testSet,
                      std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti,
                      int percentualeTestSet, int start);