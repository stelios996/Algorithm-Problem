#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

double euclidean(vector<double> &v1, vector<double> &v2){		//upologizw thn eukleidia apostash duo vectors

	double dif;
	double sum = 0.0;
	vector<double>::iterator itv1, itv2;
	for(itv1=v1.begin(), itv2 = v2.begin(); itv1!=v1.end() && itv2!=v2.end(); itv1++, itv2++){

		dif = (*itv1)-(*itv2);
		sum += dif*dif;
	}
	return sqrt(sum);
}

double cosine(vector<double> &v1, vector<double> &v2){		//upologizw thn cosine apostash duo vectors

	double product = 0.0;
	double v1norm=0.0, v2norm=0.0;
	vector<double>::iterator itv1, itv2;
	for(itv1=v1.begin(), itv2 = v2.begin(); itv1!=v1.end() && itv2!=v2.end(); itv1++, itv2++){
		product += (*itv1)*(*itv2);
		v1norm += (*itv1)*(*itv1);
		v2norm += (*itv2)*(*itv2);
	}
	v1norm = sqrt(v1norm);
	v2norm = sqrt(v2norm);

	double cosine = 1 - product/(v1norm * v2norm);
	return cosine;	
}

double cosineSim(vector<double> &v1, vector<double> &v2){		//upologizw thn cosine omoiothta duo vectors

	double product = 0.0;
	double v1norm=0.0, v2norm=0.0;
	vector<double>::iterator itv1, itv2;
	for(itv1=v1.begin(), itv2 = v2.begin(); itv1!=v1.end() && itv2!=v2.end(); itv1++, itv2++){
		product += (*itv1)*(*itv2);
		v1norm += (*itv1)*(*itv1);
		v2norm += (*itv2)*(*itv2);
	}
	v1norm = sqrt(v1norm);
	v2norm = sqrt(v2norm);

	double sim;
	if(v1norm!=0 && v2norm!=0){	
		sim = product/(v1norm * v2norm);
	}else{
		sim = 0;
	}
	return sim;	

}





