#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include "preprocess.h"
#include "classes.h"

using namespace std;

vector<string> tokenize(const string& str, const string& delimiters){		//tokenize ena string sumfwna me kapoois delimeters
    int prev = 0;
	int currentPos = 0;
	vector<string> tokens;

	// loop through string until we reach the end
	while((currentPos = str.find_first_of(delimiters, prev)) != string::npos){
		if(currentPos > prev){
			tokens.push_back(str.substr(prev, currentPos - prev));
		}
		prev = currentPos + 1;
	}
	// if characters are remaining, save to vector
	if(prev < str.length()){
		tokens.push_back(str.substr(prev, string::npos));
	}
	return tokens;
}

int getNumberOfLines(char *filepath){		//diabazw to plhthos twn dedomenwn apo to arxeio

	int numberOfLines=0;					
    ifstream myfile(filepath);
    string line;
    while(getline(myfile,line)){
        	++numberOfLines;
	}
    //cout<<"Number Of Elements: "<<numberOfLines<<endl;
    myfile.close();
    return numberOfLines;
}

void storeFileData(char *filepath, Element data[]){			//apo8hkeuw ta dedomena tou arxeiou eisodou ston pinaka data
	ifstream myfile(filepath);								
    string line;											
    int i=0;
    while(getline(myfile,line)){
        string delims = " ,\n\t";
        vector<string> tokens;
        vector<double> tokensDouble;
        tokens=tokenize(line, delims);

		for(vector<string>::iterator it=tokens.begin()+1; it!=tokens.end(); ++it){//tokens.end()-1
			tokensDouble.push_back( stod((*it), NULL) );
		}
        
		data[i].setID(atoi(tokens[0].c_str()));			//se periptwsh pou sto arxeio eisodou dinetai to id tou ka8e dedomenou(apasxoliaste)
		//data[i].setID(i);								//alliws bazw default enan akeraio gia id
        data[i].setVector(tokensDouble);
        i++;
    }
    myfile.close();
}

void storeCrypto(char *filepath, vector<string> crypto[]){		//apo8ikeuw cryptonomismata pou diabazw

	ifstream myfile(filepath);								
    string line;											
    int i=0;
    while(getline(myfile,line)){
        string delims = "\r\n\t";
        vector<string> tokens;
		tokens=tokenize(line, delims);

		crypto[i] = tokens;
		i++;
	}
	myfile.close();
}

void storeVaderLexicon(char *filepath, map<string, double> &lexicon){		//apo8ikeuw to le3iko pou diabazw

	ifstream myfile(filepath);								
    string line;											
    while(getline(myfile,line)){
		string delims = "\r\n\t";
		vector<string> tokens;
        tokens=tokenize(line, delims);
		lexicon.insert( pair<string, double>(tokens[0], stod(tokens[1], NULL)) );
	}
	myfile.close();
}

void storeTweets(char *filepath, Tweet tweets[], int &p){		//apo8ikeuw ta tweets pou diabazw

	ifstream myfile(filepath);								
    string line;											
    int i=0;
	p=20;
	int pflag=0;
    while(getline(myfile,line)){
        string delims = "\r\n\t";
        vector<string> tokens;	
		tokens=tokenize(line, delims);

		if(pflag==0){
			if(tokens.size()==2){
				p = atoi(tokens[1].c_str());
				continue;
			}
			pflag=1;
		}

		tweets[i].setUserID(atoi(tokens[0].c_str()));
		tweets[i].setTweetID(atoi(tokens[1].c_str()));

		vector<string> v;
		for(vector<string>::iterator it=tokens.begin()+2; it!=tokens.end(); ++it){
			v.push_back(*it);
		}
		tweets[i].setTweet(v);		

		i++;
	}
	myfile.close();
}


