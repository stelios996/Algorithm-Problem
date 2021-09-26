#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <cmath>
#include "classes.h"
#include "recommendation.h"
#include "tweet_analyzation.h"
#include "hash.h"
#include "search.h"
#include "LSHClustering.h"

using namespace std;

vector< UserKnownCryptos > CosineLSHRecommendation( vector< UserCryptoVector > &normal_ucv, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int P ){

	vector< UserKnownCryptos > ret;

	cout<<"Starting LSH with Cosine Metric"<<endl<<endl;

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();			//arxikopoihsh gennhtrias paragwghs tuxaiwn ari8mwn
	default_random_engine generator(seed);

	int L=5;
	int hf=4;			
	gFamily gfamily[L](hf);
	int vectorSize = cryptosNo;						
	for(int i=0; i<L; i++){									//dhmiourgw ta tuxaia dianusmata ri(gia ton upologismo tou ri*x)
		gfamily[i].generateV(generator, vectorSize);	
	}

	int usersNo = normal_ucv.size();
	int tableSize = (int)pow(2.0, (double)hf);				//8etw tableSize pinaka katakermatismou 2^hf
	Hashtable hts[L](tableSize);							//dhmiourgia pinakwn katakermatismou
	for(int i=0;i<L;i++){
		for(int j=0; j<usersNo; j++){						//apo8hkeush dedomenwn arxeiou eisodou stous pinakes katakermatismou
			UserCryptoVector *pElem = &normal_ucv[j];
			int index = hashfunctionCosine(hf, gfamily[i], pElem);	
			//cout<<"index= "<<index<<endl;
			hts[i].insert(index, pElem);					//apothikeush dedomenou sto index bucket tou ekastote pinaka katakermatismou
		}
		//hts[i].printHashTable();
	}

	for(int i=0; i<usersNo; i++){
		vector<int> nearestNIDs = searchRNNCosine(L, hf, tableSize, gfamily, hts, normal_ucv[i], P);	//briskw  ID plhsiesterous geitones
		cout<<"Cosine LSH Query: "<<i<<endl;
		//for(int j=0; j<nearestNIDs.size(); j++){
		//	cout<< nearestNIDs[j]<< flush << " ";
		//}cout<<endl;

		vector< UserCryptoVector > NNnucv;
		UserKnownCryptos qukc;
		getNNInfo(normal_ucv, knownCryptos, nearestNIDs, NNnucv, qukc, normal_ucv[i].getUserID());		//pairnw ta info tous me bash ta ID
		vector<double> predicted = predictUknownCryptos(normal_ucv[i], NNnucv, qukc);			//problepw agnwsta cryptos tou query
		vector<int> recommended = getCryptos(predicted, qukc, 5);								//pairnw ta ID twn 5 beltistwn cryptos

		UserKnownCryptos rec;
		rec.setUserID( normal_ucv[i].getUserID() );
		rec.setKCVector( recommended );
		ret.push_back( rec );
	}

	return ret;
}


vector< UserKnownCryptos > ClusteringRecommendation( vector< UserCryptoVector > &normal_ucv, vector< UserKnownCryptos > &knownCryptos, int cryptosNo ){

	cout<<"Starting Clustering Recommendation..."<<endl;
	vector<UserKnownCryptos> ret;
	//Random Selection Init - Lloyds Assignment - K-means Update

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();			//arxikopoihsh gennhtrias paragwghs tuxaiwn ari8mwn
	default_random_engine generator(seed);

	int k = 10;
	vector<int> centroids = randomSelection(k, normal_ucv.size(), generator);	//pairnw ta id twn centroids
	//cout<<"Centroids ID: ";
	//for(int i=0; i<k; i++)
	//	cout<<centroids[i]<<" ";
	//cout<<endl<<endl;

	vector<double> centroidsVectors[k];											//pairnw ta shmeia tou kathe centroid
	for(int i=0; i<k; i++){
		centroidsVectors[i] = normal_ucv[ centroids[i] ].getCVector();
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
	double dev=0.001;
	while(loopCond==1){
		cout<<endl<<"LOOP: "<<counter<<endl;
		Hashtable clusters(k);
		LloydsAssignment(clusters, centroidsVectors, normal_ucv, metric, k, normal_ucv.size());		//lloyds assignment
	
		vector< vector<double> > clusterMeans = KmeansUpdate(clusters, normal_ucv, k);				//k-means update
	
		for(int i=0; i<k; i++){
			centroidsVectors[i] = clusterMeans[i];
		}
		double objective = kmeansObj(clusters, metric, centroidsVectors);						//upologizw to objective
		
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
			//cout<<"CLUSTERS: "<<endl;
			//for(int j=0; j<k; j++){
			//	cout<<clusters.getList(j).size()<< flush<< " "<<flush;
			//}cout<<endl;

			//vector< UserCryptoVector > meansUsers = getClusterMeans(clusters, cryptosNo);		//pairnw meso dianusma xrhsth ka8e cluster
			
			//ret = recommendCluster(normal_ucv, meansUsers, knownCryptos, centroidsVectors, k, 5);	//pairnw 5 beltista cryptos
			ret = recomCluster(normal_ucv, clusters, knownCryptos, centroidsVectors, k, 5);
		}
		previous_obj = objective;
		counter++;

	}

	return ret;
}

