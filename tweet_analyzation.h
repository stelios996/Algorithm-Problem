#ifndef TWEET_ANALYZATION_H
#define TWEET_ANALYZATION_H

#include <iostream>
#include <vector>
#include <map>
#include "classes.h"

using namespace std;

double getSentiment(string word, map<string, double> &lexicon);
int getCrypto(string word, vector<string> cryptos[], int cryptosNo);
vector<double> addVectors(vector<double> &uvector, vector<double> &luvector);

void createUserCryptoVectors(Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &tweetsNo, int &cryptosNo, 
int &alpha, vector< UserCryptoVector > &retVector, vector< UserKnownCryptos > &knownCryptos);

double getVectorAverage(vector< double > &vec);
void normalUserCryptoVectors(vector< UserCryptoVector > &ucv, vector< UserCryptoVector > &normal_ucv);

int checkAllZero(vector<double> &vec);

void getNNInfo(vector< UserCryptoVector > &normal_ucv, vector< UserKnownCryptos > &knownCryptos, vector<int> &nearestNIDs, 
vector< UserCryptoVector > &NNnucv, UserKnownCryptos &qukc, int queryID);

vector<double> predictUknownCryptos(UserCryptoVector &normal_ucv, vector< UserCryptoVector > &NNnucv, UserKnownCryptos &qukc);
vector<int> getCryptos(vector<double> &predicted, UserKnownCryptos &qukc, int c);

void writeToFile(char *outputFile, vector< UserKnownCryptos > &cryptosIDs, double duration, vector<string> crypto[], int mode);

vector< UserCryptoVector > getVirtualUsers(Hashtable2 &clusters, Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &alpha, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int tweetsNo);

vector< UserCryptoVector > getClusterMeans(Hashtable &clusters, int cryptosNo);

vector< UserKnownCryptos > recommendCluster(vector< UserCryptoVector > &normal_ucv, vector< UserCryptoVector > &virtualUsers, vector< UserKnownCryptos > &knownCryptos, vector<double> centroidsVectors[], int k, int n);

vector< UserKnownCryptos > recomCluster(vector< UserCryptoVector > &normal_ucv, Hashtable &clusters, vector< UserKnownCryptos > &knownCryptos, vector<double> centroidsVectors[], int k, int n);

vector< UserKnownCryptos > recommendCosineLSH(vector< UserCryptoVector > &normal_ucv, vector< UserCryptoVector > &virtualUsers, vector< UserKnownCryptos > &knownCryptos, int k, int P, int n);

#endif
