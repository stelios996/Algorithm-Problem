#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <cmath>
#include <limits>
#include "classes.h"

using namespace std;

//###################~~Tweet~~###################
int Tweet::getUserID(){
	return this->userID;
}

int Tweet::getTweetID(){
	return this->tweetID;
}

vector<string> Tweet::getTweet(){
	return this->tweet;
}

double Tweet::getSentiment(){
	return this->sentimentScore;
}
		
void Tweet::setUserID(int uid){
	this->userID = uid;
}

void Tweet::setTweetID(int tid){
	this->tweetID = tid;
}
		
void Tweet::setTweet(vector<string> &t){
	this->tweet = t;
}

void Tweet::setSentiment(double score){
	this->sentimentScore = score;
}
//################################################

//##############~~UserCryptoVector~~##############
UserCryptoVector::UserCryptoVector(int cryptosNo){
	
	this->vsize = cryptosNo;
	double inf= numeric_limits<double>::infinity();
	for(int i=0; i<(this->vsize); i++){
		this->cvector.push_back( inf );
	}
	//cout<<"Array of size "<<this->vsize<<" has been constructed!!!"<<endl;
}

UserCryptoVector::~UserCryptoVector(){
	//delete[] this->cvector;
	//cout<<"Array of size "<<this->vsize<<" has been destructed!!!"<<endl;
}

int UserCryptoVector::getUserID(){
	return this->userID;
}

vector<double> UserCryptoVector::getCVector(){
	return this->cvector;
}

int UserCryptoVector::getVSize(){
	return this->vsize;
}

void UserCryptoVector::setUserID(int uid){
	this->userID = uid;
}

void UserCryptoVector::setCVector(double val, int pos){
	this->cvector[pos] = val;
}

void UserCryptoVector::setCVectorWhole( vector<double> &vec){
	this->cvector = vec;
}

//void UserCryptoVector::setVSize(int vs){
//	this->vsize = vs;
//}
//################################################

//##############~~UserKnownCryptos~~##############
int UserKnownCryptos::getUserID(){
	return this->userID;
}

vector<int> UserKnownCryptos::getKCVector(){
	return this->kcvector;
}

void UserKnownCryptos::setUserID(int uid){
	this->userID = uid;
}

void UserKnownCryptos::setKCVector( vector<int> &kcvec){
	this->kcvector = kcvec;
}

//##################~~Element~~##################

Element::Element(){
	this->cluster = -1;
}

void Element::setID(int id){
	this->id = id;
}

void Element::setVector(vector<double> &v){	
	this->vec = v;
}

int Element::getID(){
	return this->id;
}
	
vector<double> Element::getVector(){		
	return this->vec;
}

void Element::setCluster(int i){
	this->cluster = i;
}

int Element::getCluster(){
	return this->cluster;
}
//################################################

//##################~~HashTable~~#################

Hashtable::Hashtable(int tableSize){
	this->tableSize = tableSize;
	this->htable = new list< UserCryptoVector* >[tableSize];
	//cout<<"Hashtable constructed size: "<< this->tableSize <<endl;
}

Hashtable::~Hashtable(){
	delete[] this->htable;
	//cout<<"Hashtable Destructor"<<endl;
}

int Hashtable::getTableSize(){
	return this->tableSize;
}

list< UserCryptoVector* > Hashtable::getList(int pos){
	return this->htable[pos];
}

void Hashtable::insert(int pos, UserCryptoVector *&v){
	this->htable[pos].push_back(v);
}

void Hashtable::printHashTable(){
	int size=0;
	for(int k=0;k<this->tableSize;k++){
		//list< Element* > ll = this->htable[k];
		list< UserCryptoVector* > ll = this->htable[k];
		if(ll.size() != 0){
			//cout<<k<<":"<<endl;
			//for(list< Element* >::iterator it=ll.begin(); it!=ll.end();it++){
			for(list< UserCryptoVector* >::iterator it=ll.begin(); it!=ll.end();it++){
				//Element *p1 = *it;
				//vector<double> v= p1->getVector();
				UserCryptoVector *p1 = *it;
				vector<double> v= p1->getCVector();								
				//for(vector<double>::iterator it1=v.begin(); it1!=v.end(); ++it1){	
				//	cout<<*it1<<" ";
				//}
				//cout<<endl;
				size++;
			}
			//cout<<endl;
		}
	}
	cout<<"Elements in hashtable= "<<size<<endl<<endl;
}
//################################################

//##################~~HashTable2~~#################

Hashtable2::Hashtable2(int tableSize){
	this->tableSize = tableSize;
	this->htable = new list< Element* >[tableSize];
	//cout<<"Hashtable constructed size: "<< this->tableSize <<endl;
}

Hashtable2::~Hashtable2(){
	delete[] this->htable;
	//cout<<"Hashtable Destructor"<<endl;
}

int Hashtable2::getTableSize(){
	return this->tableSize;
}

list< Element* > Hashtable2::getList(int pos){
	return this->htable[pos];
}

void Hashtable2::insert(int pos, Element *&v){
	this->htable[pos].push_back(v);
}

void Hashtable2::printHashTable(){
	int size=0;
	for(int k=0;k<this->tableSize;k++){
		list< Element* > ll = this->htable[k];
		//list< UserCryptoVector* > ll = this->htable[k];
		if(ll.size() != 0){
			//cout<<k<<":"<<endl;
			for(list< Element* >::iterator it=ll.begin(); it!=ll.end();it++){
			//for(list< UserCryptoVector* >::iterator it=ll.begin(); it!=ll.end();it++){
				Element *p1 = *it;
				vector<double> v= p1->getVector();
				//UserCryptoVector *p1 = *it;
				//vector<double> v= p1->getCVector();								
				//for(vector<double>::iterator it1=v.begin(); it1!=v.end(); ++it1){	
				//	cout<<*it1<<" ";
				//}
				//cout<<endl;
				size++;
			}
			//cout<<endl;
		}
	}
	cout<<"Elements in hashtable= "<<size<<endl<<endl;
}
//################################################

//###################~~gFamily~~##################
gFamily::gFamily(int ksize){
	this->k = ksize;
	//cout<<"gFamily constructed size: "<< this->k <<endl;
}

gFamily::~gFamily(){
	;//cout<<"gFamily Destructor"<<endl;
}

void gFamily::generateV(default_random_engine &gen, int d){			//paragwgh tuxaiwn v
	
	normal_distribution<double> distribution(0.0 , 1.0);
	for(int i=0; i<(this->k); i++){
		vector<double> vec;
		for(int j=0; j<d; j++){
			vec.push_back( distribution(gen) );
		}
		(this->v).push_back(vec);
		vec.clear();
	}
}

//void gFamily::generateT(default_random_engine &gen, int w){			//paragwgh tuxaiwn t

//	uniform_real_distribution<double> distributionR(0,w);
//	for(int i=0; i<(this->k); i++){
//		(this->t).push_back( distributionR(gen) );
//	}
//}

vector< vector<double> > gFamily::getV(){
	return this->v;
}

//vector<double> gFamily::getT(){
//	return this->t;
//}	

vector<double> gFamily::getSpecificVector(int pos){
	return this->v[pos];
}

//double gFamily::getSpecificT(int pos){
//	return this->t[pos];
//}
//################################################

