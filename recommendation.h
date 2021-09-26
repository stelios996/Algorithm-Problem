#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include <iostream>
#include <vector>
#include <list>
#include "classes.h"

using namespace std;

vector< UserKnownCryptos > CosineLSHRecommendation( vector< UserCryptoVector > &normal_ucv, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int P );

vector< UserKnownCryptos > ClusteringRecommendation( vector< UserCryptoVector > &normal_ucv, vector< UserKnownCryptos > &knownCryptos, int cryptosNo );

#endif
