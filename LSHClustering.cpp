#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <cstring>
#include <chrono>
#include <random>
#include <algorithm>
#include "classes.h"
#include "distance.h"
#include "LSHClustering.h"
#include "search.h"

using namespace std;

vector<int> randomSelection(int k, int numberOfElements, default_random_engine &gen){

	vector<int> centroidIDs;
	uniform_int_distribution<int> distribution(0, numberOfElements);				//epilegw k tuxaia stoixeia apo to dataset kai 
	int ik=0;																		//epistrefw ta ids tous
	while(ik!=k){
		int candidateID = distribution(gen); 
		
		vector<int>::iterator it=find(centroidIDs.begin(), centroidIDs.end(), candidateID);
		if(it!=centroidIDs.end()){	//uparxei hdh
			continue;
		}else{						//den uparxei
			ik++;
			centroidIDs.push_back(candidateID);
		}
	}

	return centroidIDs;
}

void LloydsAssignment(Hashtable &clusters, vector<double> centroidsVectors[], vector< UserCryptoVector > &normal_ucv, char *metric, int k, int numberOfElements){

	for(int i=0; i<numberOfElements; i++){						//upologizw thn min apostash kathe stoixeiou apo ta uparxonta centroids
																//kai kataxwrw to stoixeio sto cluster me th mikroterh apostash 
																//stoixeiou-centroid
		UserCryptoVector elem = normal_ucv[i];
		vector<double> elemVector = elem.getCVector();

		vector<double> distances;
		if(strcmp(metric,"euclidean")==0){
			//cout<<"euclidean"<<endl;
			for(int j=0; j<k; j++){
				double euclideanDist = euclidean( centroidsVectors[j], elemVector );
				distances.push_back(euclideanDist);
			}
		}else if(strcmp(metric,"cosine")==0){
			//cout<<"cosine"<<endl;
			for(int j=0; j<k; j++){
				double cosineDist = 1-cosineSim( centroidsVectors[j], elemVector );//1-cosineSim
				//cout<<cosineDist<<endl;
				distances.push_back(cosineDist);
			}
		}
	
		double minDist = pow(2.0, (double)(sizeof(double)*8));
		int minPos=k+1;

		for(int l=0; l<k; l++){
			if( distances[l] < minDist ){
				minDist = distances[l];
				minPos = l;
			}
		}
		//cout<<"MINPOS: "<<minPos<<endl;
	
		UserCryptoVector *pElem = &normal_ucv[i];
		clusters.insert(minPos, pElem);

		distances.clear();
	}
	//clusters.printHashTable();
}

vector< vector<double> > KmeansUpdate(Hashtable &clusters, vector< UserCryptoVector > &normal_ucv, int k){

	vector< vector<double> > clusterMeans;

	int vsize = normal_ucv[0].getCVector().size();
	for(int i=0; i<k; i++){
		vector<double> vm;
		for(int j=0; j<vsize; j++){
			vm.push_back(0.0);
		}
		clusterMeans.push_back(vm);
	}

	for(int i=0; i<k; i++){														//briskw to meso dianusma kathe cluster
		list< UserCryptoVector* > clusterList = clusters.getList(i);
		int clusterSize = clusterList.size();
		//cout<<"CLUSTERSIZE:  "<<clusterSize<<endl;
		if(clusterSize != 0){
			for(list< UserCryptoVector* >::iterator it=clusterList.begin(); it!=clusterList.end(); ++it){
				UserCryptoVector *p = *it;
				vector<double> v = p->getCVector();									
			
				vector<double>::iterator it1, it2;									
				for( it1=v.begin(), it2=clusterMeans[i].begin(); it1!=v.end(), it2!=clusterMeans[i].end(); ++it1, ++it2){	
					(*it2) += (*it1);
				}
			}
			
			for(vector<double>::iterator it3=clusterMeans[i].begin(); it3!=clusterMeans[i].end(); ++it3){
				(*it3) = (*it3)/clusterSize; 
			}
		}
	}

	return clusterMeans;
}

double kmeansObj(Hashtable &clusters, char *metric, vector<double> centroidsVectors[]){

	int htsize = clusters.getTableSize();
	//cout<<"TABLESIZE: "<<htsize<<endl;

	double sum = 0.0;														//upologizw to objective me ton tupo apo tis diafaneies
	for(int i=0; i<htsize; i++){
		list< UserCryptoVector* > clusterList = clusters.getList(i);
		int clusterSize = clusterList.size();

		if(clusterSize != 0){
			
			for(list< UserCryptoVector* >::iterator it=clusterList.begin(); it!=clusterList.end(); ++it){
				UserCryptoVector *p = *it;
				vector<double> v= p->getCVector();									

				if(strcmp(metric,"euclidean")==0){
					//cout<<"euclidean"<<endl;
						double euclideanDist = euclidean( centroidsVectors[i], v );
						sum += euclideanDist*euclideanDist;
				}else if(strcmp(metric,"cosine")==0){
					//cout<<"cosine"<<endl;
						double cosineDist = 1-cosineSim( centroidsVectors[i], v );//1-cosineSim
						sum += cosineDist*cosineDist;
				}
					
			}	
		}
	}
	
	//cout<<"SUM_OBJ: "<<sum<<endl;
	return sum;	
}

vector<double> Silhouette(Hashtable &clusters, int k, char *metric, vector<double> centroidsVectors[]){

	vector<double> silh;
	for(int i=0; i<k; i++){											//to silhouette gia ka8e cluster
		list< UserCryptoVector* > clusterList = clusters.getList(i);
		vector<double> clusterSilh;

		if(clusterList.size() != 0){
			for(list< UserCryptoVector* >::iterator itlist=clusterList.begin(); itlist!=clusterList.end(); itlist++){
				UserCryptoVector *elem = *itlist;
				int elemID = elem->getUserID();
				vector<double> elemVector = elem->getCVector();
				
				double a=0.0;													//upologizw to a(i)
				for(list< UserCryptoVector* >::iterator itlist2=clusterList.begin(); itlist2!=clusterList.end(); itlist2++){
					UserCryptoVector *p = *itlist2;
					int pID = p->getUserID();
					vector<double> pVector = p->getCVector();
		
					if(pID != elemID){
						if( strcmp(metric,"euclidean")==0 ){
							double euclideanDist = euclidean(elemVector, pVector);
							a += euclideanDist;
						}else if( strcmp(metric,"cosine")==0 ){
							double cosineDist = 1-cosineSim(elemVector, pVector);
							a += cosineDist;
						}
					}
				}
				a = a/clusterList.size();

				vector<double> distances;										//briskw to amesws kontinotero cluster
				for(int l=0; l<k; l++){
					if( strcmp(metric,"euclidean")==0 ){
						double euclideanDist = euclidean(elemVector, centroidsVectors[l]);
						distances.push_back(euclideanDist);
					}else if( strcmp(metric,"cosine")==0 ){
						double cosineDist = 1-cosineSim(elemVector, centroidsVectors[l]);
						distances.push_back(cosineDist);
					}
				}
				
				double mindist = pow(2.0, (double)(sizeof(double)*8));
				int minPos=k+1;

				for(int l=0; l<k; l++){
					if( distances[l] < mindist  && i!=l){
						mindist = distances[l];
						minPos = l;
					}
				}

				list< UserCryptoVector* > clusterList2 = clusters.getList(minPos);			//upologizw to b(i)
				double b=0.0;
				if(clusterList2.size()!=0){
					for(list< UserCryptoVector* >::iterator itlist3=clusterList2.begin(); itlist3!=clusterList2.end(); itlist3++){
						UserCryptoVector *p2= *itlist3;
						//int pID2 = p2->getID();
						vector<double> pVector2 = p2->getCVector();
		
						if( strcmp(metric,"euclidean")==0 ){
							double euclideanDist2 = euclidean(elemVector, pVector2);
							b += euclideanDist2;
						}else if( strcmp(metric,"cosine")==0 ){
							double cosineDist2 = 1-cosineSim(elemVector, pVector2);
							b += cosineDist2;
						}
					}
					b = b/clusterList2.size();
				}
		
				double s;									//to silhouette ka8e stoixeiou
				if(a>b){
					s=(b-a)/a;
				}else if(a<b){
					s=(b-a)/b;
				}
				clusterSilh.push_back(s);

			}

		}else{
			silh.push_back(0);
			continue;
		}

		double sum=0.0;										//to silhouette tou cluster
		for(int j=0; j<clusterSilh.size(); j++){
			sum += clusterSilh[j];
		}
		sum = sum/clusterSilh.size();
		silh.push_back(sum);

		clusterSilh.clear();
	}
	
	double total=0.0;										//to total silhouette apo ola ta clusters
	for(int i=0; i<silh.size(); i++){
		total += silh[i];
	}
	total = total/silh.size();
	silh.push_back(total);
	
	return silh;
}

///////////////////////////Assign for Element///////////////////////////////////////////////////
void LloydsAssignment2(Hashtable2 &clusters, vector<double> centroidsVectors[], Element data[], char *metric, int k, int numberOfElements){

	for(int i=0; i<numberOfElements; i++){						//upologizw thn min apostash kathe stoixeiou apo ta uparxonta centroids
																//kai kataxwrw to stoixeio sto cluster me th mikroterh apostash 
		Element elem = data[i];									//stoixeiou-centroid	
		vector<double> elemVector = elem.getVector();

		vector<double> distances;
		for(int j=0; j<k; j++){
			if( strcmp(metric,"euclidean")==0 ){
				double euclideanDist = euclidean( centroidsVectors[j], elemVector );
				//cout<<cosineDist<<endl;
				distances.push_back(euclideanDist);
			}else if( strcmp(metric,"cosine")==0 ){
				double cosineDist = 1-cosineSim( centroidsVectors[j], elemVector );
				//cout<<cosineDist<<endl;
				distances.push_back(cosineDist);
			}
		}

	
		double minDist = pow(2.0, (double)(sizeof(double)*8));
		int minPos=k+1;

		for(int l=0; l<k; l++){
			if( distances[l] < minDist ){
				minDist = distances[l];
				minPos = l;
			}
		}
		//cout<<"MINPOS: "<<minPos<<endl;
	
		Element *pElem = &data[i];
		clusters.insert(minPos, pElem);

		distances.clear();
	}
	//clusters.printHashTable();
}

vector< vector<double> > KmeansUpdate2(Hashtable2 &clusters, Element data[], int k){

	vector< vector<double> > clusterMeans;

	int vsize = data[0].getVector().size();
	for(int i=0; i<k; i++){
		vector<double> vm;
		for(int j=0; j<vsize; j++){
			vm.push_back(0.0);
		}
		clusterMeans.push_back(vm);
	}

	for(int i=0; i<k; i++){														//briskw to meso dianusma kathe cluster
		list< Element* > clusterList = clusters.getList(i);
		int clusterSize = clusterList.size();
		//cout<<"CLUSTERSIZE:  "<<clusterSize<<endl;
		if(clusterSize != 0){
			for(list< Element* >::iterator it=clusterList.begin(); it!=clusterList.end(); ++it){
				Element *p = *it;
				vector<double> v = p->getVector();									
			
				vector<double>::iterator it1, it2;									
				for( it1=v.begin(), it2=clusterMeans[i].begin(); it1!=v.end(), it2!=clusterMeans[i].end(); ++it1, ++it2){	
					(*it2) += (*it1);
				}
			}
			
			for(vector<double>::iterator it3=clusterMeans[i].begin(); it3!=clusterMeans[i].end(); ++it3){
				(*it3) = (*it3)/clusterSize; 
			}
		}
	}

	return clusterMeans;
}

double kmeansObj2(Hashtable2 &clusters, char *metric, vector<double> centroidsVectors[]){

	int htsize = clusters.getTableSize();
	//cout<<"TABLESIZE: "<<htsize<<endl;

	double sum = 0.0;														//upologizw to objective me ton tupo apo tis diafaneies
	for(int i=0; i<htsize; i++){
		list< Element* > clusterList = clusters.getList(i);
		int clusterSize = clusterList.size();

		if(clusterSize != 0){
			
			for(list< Element* >::iterator it=clusterList.begin(); it!=clusterList.end(); ++it){
				Element *p = *it;
				vector<double> v= p->getVector();									

				if( strcmp(metric,"euclidean")==0 ){
					double euclideanDist = euclidean( centroidsVectors[i], v );
					sum += euclideanDist*euclideanDist;
				}else if( strcmp(metric,"cosine")==0 ){
					double cosineDist = 1-cosineSim( centroidsVectors[i], v );
					sum += cosineDist*cosineDist;
				}	
			}	
		}
	}
	
	//cout<<"SUM_OBJ: "<<sum<<endl;
	return sum;	
}
