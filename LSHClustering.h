#ifndef LSHCLUSTERING_H
#define LSHCLUSTERING_H

#include <iostream>
#include <vector>
#include <random>
#include "classes.h"

using namespace std;

vector<int> randomSelection(int k, int numberOfElements, default_random_engine &gen);

void LloydsAssignment(Hashtable &clusters, vector<double> centroidsVectors[], vector< UserCryptoVector > &normal_ucv, char *metric, int k, int numberOfElements);

vector< vector<double> > KmeansUpdate(Hashtable &clusters, vector< UserCryptoVector > &normal_ucv, int k);

double kmeansObj(Hashtable &clusters, char *metric, vector<double> centroidsVectors[]);

vector<double> Silhouette(Hashtable &clusters, int k, char *metric, vector<double> centroidsVectors[]);

/////////For Class Element/////////////////
void LloydsAssignment2(Hashtable2 &clusters, vector<double> centroidsVectors[], Element data[], char *metric, int k, int numberOfElements);
vector< vector<double> > KmeansUpdate2(Hashtable2 &clusters, Element data[], int k);
double kmeansObj2(Hashtable2 &clusters, char *metric, vector<double> centroidsVectors[]);

#endif
