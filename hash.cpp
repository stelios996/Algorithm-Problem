#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <math.h>
#include "classes.h"

using namespace std;

int hashfunctionCosine(int k, gFamily &gfam, UserCryptoVector *&elem){		//cosine hash function

	//vector<double> x = elem->getVector();
	vector<double> x = elem->getCVector();
	vector<int> g;
	
	for(int i=0; i<k; i++){											//upologizw to g={h1,...hk}

		vector<double> r = gfam.getSpecificVector(i); 

		long double product=0.0;
		vector<double>::iterator itx;
		vector<double>::iterator itr;
		for(itx=x.begin(), itr = r.begin(); itx!=x.end() && itr!=r.end(); itx++, itr++){
			product += (long double)(*itx)*(*itr);
		}
		
		int hi;
		if(product>=0.0){
			hi = 1;
		}else if(product<0.0){
			hi = 0;
		}
		g.push_back(hi);
	}
	
	int decimal=0;													//convert binary bitstring g to decimal
	double j=k-1;
	for(int l=0; l<k; l++, j--){
		if(g[l]==1){
			decimal += (int)pow(2.0, j);
		}
	}
	//cout<<"index: "<<decimal<<endl;
	return decimal;
}





