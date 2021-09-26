#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include "classes.h"

using namespace std;

vector<string> tokenize(const string& str, const string& delimiters);
int getNumberOfLines(char *filepath);
void storeFileData(char *filepath, Element data[]);
void storeCrypto(char *filepath, vector<string> crypto[]);
void storeVaderLexicon(char *filepath, map<string, double> &lexicon);
void storeTweets(char *filepath, Tweet tweets[], int &p);

#endif
