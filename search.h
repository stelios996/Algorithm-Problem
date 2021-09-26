#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include "hash.h"
#include "preprocess.h"
#include "classes.h"
#include "distance.h"
#include "search.h"

using namespace std;

bool mysort( const pair<int,double> &a, const pair<int,double> &b);
vector< int > searchRNNCosine(int L, int k, int tableSize, gFamily gfamily[], Hashtable hts[], UserCryptoVector nucv, int P);

#endif
