#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include <random>
#include "hash.h"
#include "preprocess.h"
#include "classes.h"
#include "distance.h"
#include "search.h"

using namespace std;

bool mysort( const pair<int,double> &a, const pair<int,double> &b){
	return a.second>b.second;
}

//LSH COSINE SEARCH
vector< int > searchRNNCosine(int L, int k, int tableSize, gFamily gfamily[], Hashtable hts[], UserCryptoVector nucv, int P){

	UserCryptoVector *qElemp = &nucv;
	vector<double> qelemvec = nucv.getCVector();
	int qelemID = nucv.getUserID();

	vector< pair<int, double> > pairs;

	//LSH search
	for(int i=0; i<L; i++){											//search ola ta hashtables

		int qphi = hashfunctionCosine(k, gfamily[i], qElemp);		//briskw to bucket pou tha mpei
		//cout<<"qphi: "<<qphi<<endl;

		list< UserCryptoVector* >  phibucket = hts[i].getList(qphi);			//pairnw to sugkekrimeno bucket tou hashtable
		if(phibucket.size() != 0){
			//cout<<"phibucket size: "<<phibucket.size()<<endl;
			for(list< UserCryptoVector* >::iterator itlist=phibucket.begin(); itlist!=phibucket.end(); itlist++){	//e3etazw ta stoixeia tou
				UserCryptoVector *p = *itlist;
				int id = p->getUserID();
				if(id!=qelemID){
					vector<double> pvec = p->getCVector();

					double cosineDistance = 1-cosineSim(pvec, qelemvec);
					
					pairs.push_back( make_pair(id, cosineDistance) );					
				}
			}
		}else{
			;//cout<<"-"<<countQuery<<", L: "<<i<<", Bucket is empty"<<endl;
		}
	}

	
	sort( pairs.begin(), pairs.end(), mysort);

	vector<int> retIDs;				//pairnw ta P kalutera
	for(int i=0; i<P; i++){
		retIDs.push_back( pairs[i].first );
	}		

	return retIDs;
}


