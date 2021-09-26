#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <cmath>
#include "classes.h"
#include "Brecommendation.h"
#include "tweet_analyzation.h"
#include "hash.h"
#include "search.h"
#include "LSHClustering.h"

using namespace std;

vector< UserKnownCryptos > BCosineLSHRecom(vector< UserCryptoVector > &normal_ucv, Element data[], Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &alpha, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int p, int dataNo, int tweetsNo){

	cout<<"Starting Cosine LSH Recommendation B part..."<<endl;
	vector<UserKnownCryptos> ret;
	//Random Selection Init - Lloyds Assignment - K-means Update
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();			//arxikopoihsh gennhtrias paragwghs tuxaiwn ari8mwn
	default_random_engine generator(seed);

	int k = 100;
	vector<int> centroids = randomSelection(k, dataNo, generator);	//pairnw ta id twn centroids
	//cout<<"Centroids ID: ";
	//for(int i=0; i<k; i++)
	//	cout<<centroids[i]<<" ";
	//cout<<endl<<endl;

	vector<double> centroidsVectors[k];											//pairnw ta shmeia tou kathe centroid
	for(int i=0; i<k; i++){
		centroidsVectors[i] = data[ centroids[i] ].getVector();
	}

	/*for(int i=0; i<k; i++){
		cout<<i<<":"<<endl;
		vector<double> v=centroidsVectors[i];
		for(vector<double>::iterator it=v.begin(); it!=v.end(); ++it){
			cout<<(*it)<<" "<<flush;
		}cout<<endl;
	}*/

	char metric[20]="cosine";
	int counter=0;
	double previous_obj=-100000.0;
	int loopCond=1;
	double dev=0.001;					//akribeia 99.9%
	while(loopCond==1){
		cout<<endl<<"LOOP: "<<counter<<endl;
		Hashtable2 clusters(k);
		LloydsAssignment2(clusters, centroidsVectors, data, metric, k, dataNo);		//lloyds assignment
	
		vector< vector<double> > clusterMeans = KmeansUpdate2(clusters, data, k);				//k-means update
	
		for(int i=0; i<k; i++){
			centroidsVectors[i] = clusterMeans[i];
		}
		double objective = kmeansObj2(clusters, metric, centroidsVectors);						//upologizw to objective
		
		cout<<"OBJECTIVE: "<<objective<<endl;
		cout<<"PREVIOUS: "<<previous_obj<<endl;

		if( (objective>=((1.0-dev)*previous_obj) && objective<=previous_obj) || (objective==previous_obj) ){
			loopCond=0;											//an to objective meta3u 2 epanalhpsewn eiani idio h exei 1% apoklish 
																//stamataw ton algorithmo
			/*cout<<endl<<"Calculating Silhouette..."<<endl;
			vector<double> silhouette = Silhouette(clusters, k, metric, centroidsVectors);		//upologizw to silhouette
			int f;
			for(f=0; f<silhouette.size()-1; f++){
				cout<<silhouette[f]<<" "<<flush;
			}
			cout<<endl;
			cout<<"Total Silhouette: "<<silhouette[f]<<endl;*/

			vector< UserCryptoVector > virtualUsers = getVirtualUsers(clusters, tweets, vaderLexicon, cryptos, alpha, knownCryptos, cryptosNo, tweetsNo);																						//upologizw eikonikous xrhstes
			
			ret = recommendCosineLSH(normal_ucv, virtualUsers, knownCryptos, k, p, 2);			//briskw 2 beltista cryptos

		}
		previous_obj = objective;
		counter++;

	}

	return ret;

}

vector< UserKnownCryptos > BClusteringRecom(vector< UserCryptoVector > &normal_ucv, Element data[], Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &alpha, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int p, int dataNo, int tweetsNo){

	cout<<"Starting Clustering Recommendation B part..."<<endl;
	vector<UserKnownCryptos> ret;
	//Random Selection Init - Lloyds Assignment - K-means Update

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();			//arxikopoihsh gennhtrias paragwghs tuxaiwn ari8mwn
	default_random_engine generator(seed);

	int k = 100;
	vector<int> centroids = randomSelection(k, dataNo, generator);	//pairnw ta id twn centroids
	//cout<<"Centroids ID: ";
	//for(int i=0; i<k; i++)
	//	cout<<centroids[i]<<" ";
	//cout<<endl<<endl;

	vector<double> centroidsVectors[k];											//pairnw ta shmeia tou kathe centroid
	for(int i=0; i<k; i++){
		centroidsVectors[i] = data[ centroids[i] ].getVector();
	}

	/*for(int i=0; i<k; i++){
		cout<<i<<":"<<endl;
		vector<double> v=centroidsVectors[i];
		for(vector<double>::iterator it=v.begin(); it!=v.end(); ++it){
			cout<<(*it)<<" "<<flush;
		}cout<<endl;
	}*/

	char metric[20]="euclidean";
	int counter=0;
	double previous_obj=-100000.0;
	int loopCond=1;
	double dev=0.001;					//99.9% akribeia
	while(loopCond==1){
		cout<<endl<<"LOOP: "<<counter<<endl;
		Hashtable2 clusters(k);
		LloydsAssignment2(clusters, centroidsVectors, data, metric, k, dataNo);		//lloyds assignment
	
		vector< vector<double> > clusterMeans = KmeansUpdate2(clusters, data, k);				//k-means update
	
		for(int i=0; i<k; i++){
			centroidsVectors[i] = clusterMeans[i];
		}
		double objective = kmeansObj2(clusters, metric, centroidsVectors);						//upologizw to objective
		
		cout<<"OBJECTIVE: "<<objective<<endl;
		cout<<"PREVIOUS: "<<previous_obj<<endl;

		if( (objective>=((1.0-dev)*previous_obj) && objective<=previous_obj) || (objective==previous_obj) ){
			loopCond=0;											//an to objective meta3u 2 epanalhpsewn eiani idio h exei 1% apoklish 
																//stamataw ton algorithmo
			/*cout<<endl<<"Calculating Silhouette..."<<endl;
			vector<double> silhouette = Silhouette(clusters, k, metric, centroidsVectors);		//upologizw to silhouette
			int f;
			for(f=0; f<silhouette.size()-1; f++){
				cout<<silhouette[f]<<" "<<flush;
			}
			cout<<endl;
			cout<<"Total Silhouette: "<<silhouette[f]<<endl;*/

			vector< UserCryptoVector > virtualUsers = getVirtualUsers(clusters, tweets, vaderLexicon, cryptos, alpha, knownCryptos, cryptosNo, tweetsNo);																						//upologizw eikonikous xrhstes
			
			ret = recommendCluster(normal_ucv, virtualUsers, knownCryptos, centroidsVectors, k, 2);	//briskw 2 beltista cryptos
																								
		}
		previous_obj = objective;
		counter++;

	}

	return ret;

}


