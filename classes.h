#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>

using namespace std;

class Tweet{
	private:
		int userID;
		int tweetID;
		vector<string> tweet;
		double sentimentScore;
	public:
		Tweet(){};
		~Tweet(){};
		int getUserID();
		int getTweetID();
		vector<string> getTweet();
		double getSentiment();
		void setUserID(int uid);
		void setTweetID(int tid);
		void setTweet(vector<string> &t);	
		void setSentiment(double score);
};

class UserCryptoVector{
	private:
		int userID;
		vector<double> cvector;
		int vsize;
	public:
		UserCryptoVector(int cryptosNo);
		~UserCryptoVector();
		int getUserID();
		vector<double> getCVector();
		int getVSize();
		void setUserID(int uid);
		void setCVector(double val, int pos);
		void setCVectorWhole( vector<double> &vec);
		//void setVSize(int vs);
};

class UserKnownCryptos{
	private:
		int userID;
		vector<int> kcvector;
	public:
		UserKnownCryptos(){};
		~UserKnownCryptos(){};
		int getUserID();
		vector<int> getKCVector();
		void setUserID(int uid);
		void setKCVector( vector<int> &kcvec);
};

class Element{						//klash stoixeiou, apoteleitai apo ena id kai ena vector pou perilambanei tis suntetagmenes
	private:
		int id;
		vector<double> vec;
		int cluster;
	public:
		Element();
		~Element(){};
		void setID(int id);
		void setVector(vector<double> &v);
		int getID();
		vector<double> getVector();
		void setCluster(int i);
		int getCluster();
};

class Hashtable{				//klash pinaka katakermatismou, apoteleitai apo to megethos tou pinaka kai ena pinaka listwn pointer se data
	private:
		int tableSize;
		list< UserCryptoVector* > *htable;
	public:
		Hashtable(int tableSize);
		~Hashtable();

		int getTableSize();
		list< UserCryptoVector* > getList(int pos);
		void insert(int pos, UserCryptoVector *&v);
		void printHashTable();

};

class Hashtable2{				//klash pinaka katakermatismou, apoteleitai apo to megethos tou pinaka kai ena pinaka listwn pointer se data
	private:
		int tableSize;
		list< Element* > *htable;
	public:
		Hashtable2(int tableSize);
		~Hashtable2();

		int getTableSize();
		list< Element* > getList(int pos);
		void insert(int pos, Element *&v);
		void printHashTable();

};


class gFamily{					//klash oikogeneias g
	private:
		int k;
		vector< vector<double> > v;
		//vector<double> t;
	public:
		gFamily(int ksize);
		~gFamily();

		void generateV(default_random_engine &gen, int d);
		//void generateT(default_random_engine &gen, int w);	
		vector< vector<double> > getV();
		//vector<double> getT();	
		vector<double> getSpecificVector(int pos);
		//double getSpecificT(int pos);
};

#endif
