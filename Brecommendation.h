#ifndef BRECOMMENDATION_H
#define BRECOMMENDATION_H

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "classes.h"

using namespace std;

vector< UserKnownCryptos > BCosineLSHRecom(vector< UserCryptoVector > &normal_ucv, Element data[], Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &alpha, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int p, int dataNo, int tweetsNo);

vector< UserKnownCryptos > BClusteringRecom(vector< UserCryptoVector > &normal_ucv, Element data[], Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &alpha, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int p, int dataNo, int tweetsNo);



#endif
