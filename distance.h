#ifndef DISTANCE_H
#define DISTANCE_H

#include <iostream>
#include <vector>

using namespace std;

double euclidean(vector<double> &v1, vector<double> &v2);	
double cosine(vector<double> &v1, vector<double> &v2);
double cosineSim(vector<double> &v1, vector<double> &v2);	

#endif
