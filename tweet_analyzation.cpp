#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <cmath>
#include "classes.h"
#include "tweet_analyzation.h"
#include "distance.h"
#include "search.h"

using namespace std;

double getSentiment(string word, map<string, double> &lexicon){

	map<string, double>::iterator it = lexicon.find(word);		//anazhthsh sto le3iko
	if(it==lexicon.end()){	//not found							//den uparxei-epistrefw 0
		return (double)0;
	}else{					//found								//uparxei-epistrefw to sentiment ths le3hs
		return it->second;
	}
}

int getCrypto(string word, vector<string> cryptos[], int cryptosNo){

	int pos=-1;													//briskw an to word einai crypto, kai an einai se poio anaferetai
	for(int i=0; i<cryptosNo; i++){
		if( find(cryptos[i].begin(), cryptos[i].end(), word) != cryptos[i].end()){
			pos=i;
			break;		
		}
	}
	return pos;
}

vector<double> addVectors(vector<double> &uvector, vector<double> &luvector){	//pros8esh dianusmatwn

	double inf= numeric_limits<double>::infinity();

	vector<double> sum;
	vector<double>::iterator it1;
	vector<double>::iterator it2;
	for( it1=uvector.begin(), it2=luvector.begin(); it1!=uvector.end(), it2!=luvector.end(); ++it1, ++it2){
		if( (*it1)!=inf && (*it2)!=inf ){
			double s = (*it1)+(*it2);
			sum.push_back( s );

		}else if( (*it1)==inf && (*it2)==inf ){
			sum.push_back( inf );			

		}else if( (*it1)!=inf && (*it2)==inf ){
			sum.push_back( (*it1) );

		}else if( (*it1)==inf && (*it2)!=inf ){
			sum.push_back( (*it2) );

		}
	}
	return sum;
}


void createUserCryptoVectors(Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &tweetsNo, int &cryptosNo,
int &alpha, vector< UserCryptoVector > &retVector, vector< UserKnownCryptos > &knownCryptos){

	int currID, prevID=-1;
	for(int i=0; i<tweetsNo; i++){														//gia ka8e tweet
		
		currID = tweets[i].getUserID();
		vector<string> t=tweets[i].getTweet();											

		double sentimentVal=0.0;
		vector<int> tcryptos;	
		for(int j=0; j<t.size(); j++){													//gia ka8e le3h tou tweet
			double val = getSentiment( t[j], vaderLexicon );							//pairnw to sentiment ths le3hs
			if(val==0.0){																//an den uparxei sto le3iko h le3h
				int pos=getCrypto( t[j], cryptos, cryptosNo );								//elegxw an h le3h einai cryptonomisma
			//cout<<pos<<endl;
				if(pos!=-1){ 
					tcryptos.push_back(pos);												//an einai to krataw
				}
				//cout<<(t[i])<<endl;
				//cout<<" : "<<val<<endl;
			}else{																		//an uparxei sto le3iko h le3h
				sentimentVal += val;														//upologizw to sentiment tou tweet
			}
		}
		sentimentVal = sentimentVal/sqrt( sentimentVal*sentimentVal + alpha);			//kanonikopoiw to sentiment
		tweets[i].setSentiment(sentimentVal);
		//cout<<"SENTIMENT SCORE: "<< sentimentVal<<endl;
		/*cout<<"CRYPTOS POSITION"<<endl;
		for(vector<int>::iterator it1=tcryptos.begin(); it1!=tcryptos.end(); it1++){
			cout<< (*it1)<<endl;
		}*/

		UserCryptoVector user(cryptosNo);
		user.setUserID( currID );
		for(int k=0; k<tcryptos.size(); k++){											//8etw to sentiment se ka8e cryptonomisma pou exei 
			user.setCVector( sentimentVal, tcryptos[k] );								//anafer8ei to tweet
		}
			
		if(currID != prevID){											//an o trexon xrhsths exei diaforetiko ID apo ton prohgoumeno
			
			retVector.push_back(user);									//krataw ta dedomena tou

			UserKnownCryptos ukc;
			ukc.setUserID( currID );
			ukc.setKCVector( tcryptos );
			knownCryptos.push_back( ukc );
		}else{															//an o trexon xrhsths exei to idio ID me ton prohgoumeno
			
			UserCryptoVector lastUser = retVector.back();				//prosthetw ta dedomena tou sta prohgoumena dedomena
			retVector.pop_back();

			vector<double> uvector = user.getCVector();
			vector<double> luvector = lastUser.getCVector();
			vector<double> addVector = addVectors(uvector, luvector);

			UserCryptoVector newUser(cryptosNo);
			newUser.setUserID( currID );
			newUser.setCVectorWhole( addVector );
			retVector.push_back( newUser );

			UserKnownCryptos lukc = knownCryptos.back();
			knownCryptos.pop_back();
			
			vector<int> lukcvector = lukc.getKCVector();
			lukcvector.insert( lukcvector.end(), tcryptos.begin(), tcryptos.end() );
			lukc.setKCVector( lukcvector );
			knownCryptos.push_back( lukc );
		}

		prevID = currID;
	}

}

double getVectorAverage(vector< double > &vec){			//upologizw to meso oro tou dianusmatos

	double inf= numeric_limits<double>::infinity();

	int vsize = 0;
	double sum = 0.0;
	for(vector<double>::iterator it=vec.begin(); it!=vec.end(); ++it){
		if( (*it)!=inf ){
			sum += (*it);
			vsize++;
		}		
	}
	double avg;
	if( vsize!=0 ){
		avg = sum/((double) vsize);
	}else{
		avg = 0.0;
	}
	//cout<<"AVERAGE: "<< avg <<endl;
	return avg;
}

void normalUserCryptoVectors(vector< UserCryptoVector > &ucv, vector< UserCryptoVector > &normal_ucv){

	double inf= numeric_limits<double>::infinity();				//kanonikopoiw ta dianusmata twn xrhstwn sumfwna me to meso oro tou ka8enos 
																//sta agnwsta cryptonomismata bazw 0
	int ucv_size = ucv.size();
	for(int i=0; i<ucv_size; i++){
		UserCryptoVector user = ucv[i];
		int userID = user.getUserID();
		vector<double> userV = user.getCVector();
		int userVSize = user.getVSize();

		double avg_vec = getVectorAverage(userV);

		vector<double> normal_ucv1;
		for(vector<double>::iterator it=userV.begin(); it!=userV.end(); ++it){
			if( (*it)==inf ){
				normal_ucv1.push_back(0.0);
			}else{
				normal_ucv1.push_back( (*it)-avg_vec );
			}
		}
		
		UserCryptoVector normal_user(userVSize);
		normal_user.setUserID(userID);
		normal_user.setCVectorWhole(normal_ucv1);

		normal_ucv.push_back(normal_user);
		normal_ucv1.clear();
	}
}

int checkAllZero(vector<double> &vec){	//elegxw an ola ta dianusmata einai mhdenika dianusmata

	int flag=0;
	for(int i=0; i<vec.size(); i++){
		if(vec[i]!=0.0){
			flag=1;
			break;
		}
	}
	if(flag==1){
		return 0;	//not all zero	
	}else{
		return 1;	//all zero
	}
}


void getNNInfo(vector< UserCryptoVector > &normal_ucv, vector< UserKnownCryptos > &knownCryptos, vector<int> &nearestNIDs, 
vector< UserCryptoVector > &NNnucv, UserKnownCryptos &qukc, int queryID){

	for(int k=0; k<knownCryptos.size(); k++){			//briskw ta cryptos pou exei anafer8ei to query
		UserKnownCryptos currUKC = knownCryptos[k];
		if( currUKC.getUserID() == queryID ){
			qukc = currUKC;
			break;
		}
	}

	for(int i=0; i<nearestNIDs.size(); i++){			//briskw ta dianusmara xrhstwn twn plhsiesterwn geitonwn
		int currID = nearestNIDs[i];

		for(int j=0; j<normal_ucv.size(); j++){
			UserCryptoVector currUCV = normal_ucv[j];
			if( currUCV.getUserID() == currID ){
				NNnucv.push_back( currUCV );
				break;
			}
		}
	}
}

vector<double> predictUknownCryptos(UserCryptoVector &query, vector< UserCryptoVector > &neighbors, UserKnownCryptos &qukc){

	vector<double> predicted;
	
	vector<double> simV;										//kanw thn problepsh twn agnwstwn cryptonomismatwn tou query 
	double sumSim = 0.0;										//sumfwna me ton tupo apo tis diafaneies
	vector<double> qvector = query.getCVector();
	for(int i=0; i<neighbors.size(); i++){
		UserCryptoVector current = neighbors[i];
		vector<double> currV = current.getCVector();
		
		double sim = cosineSim(qvector, currV);
		simV.push_back(sim);
		sumSim += abs(sim);
	}
	//cout<<"SUM SIM: "<<sumSim<<endl;
	double z;
	if(sumSim==0){
		z=1;
	}else{
		z=sumSim;
	}

	vector<int> knownCryptos = qukc.getKCVector();		
	for(int i=0; i<qvector.size(); i++){
		
		if( find( knownCryptos.begin(), knownCryptos.end(), i)!=knownCryptos.end() ){
			predicted.push_back( qvector[i] );
		}else{
			double sumitem=0.0;
			for(int j=0; j<neighbors.size(); j++){
				UserCryptoVector current = neighbors[j];
				vector<double> currV = current.getCVector();
				
				sumitem += currV[i]*simV[i];
			}
			double r = z*sumitem;
			//cout<<"R(u,i): "<< i<< flush<<" "<<r<<endl;
			predicted.push_back(r);
		}
	}

	//cout<<"Predicted: "<<endl;
	//for(int i=0; i<qvector.size(); i++){
	//	cout<< predicted[i] <<endl;
	//}

	return predicted;
}

vector<int> getCryptos(vector<double> &predicted, UserKnownCryptos &qukc, int c){

	vector<int> recommended;
	int count=0;

	vector<int> knownCryptos = qukc.getKCVector();			//pairnw ta c beltista cryptos sta opoia den exei anafer8ei o xrhsths
	double lowest = numeric_limits<double>::lowest();
	while(count!=c){
		double max = lowest;
		int maxpos = -1;
		for(int i=0; i<predicted.size(); i++){
			if( (find(knownCryptos.begin(), knownCryptos.end(), i) == knownCryptos.end()) 
				&& (find(recommended.begin(), recommended.end(), i) == recommended.end()) ){
				if(predicted[i]>max){
					max = predicted[i];
					maxpos = i;
				}
			}
		}
		recommended.push_back( maxpos );
		count++;
	}
	
	return recommended;
}

void writeToFile(char *outputFile, vector< UserKnownCryptos > &cryptosIDs, double duration, vector<string> crypto[], int mode){

	if(mode==0){													//grafw ta apotelesmata sto arxeio e3odou
		ofstream outfile(outputFile, ios::out);
		outfile << "Cosine LSH\n";

		for(int i=0; i<cryptosIDs.size(); i++){
			UserKnownCryptos user=cryptosIDs[i];
			outfile<< user.getUserID();
			//cout<< user.getUserID()<<flush;
			vector<int> userC = user.getKCVector();
			for(int j=0; j<userC.size(); j++){
				vector<string> cryp = crypto[ userC[j] ];
				if(cryp.size() == 4){
					outfile<<" "<<cryp[0]<<flush;
				}else{
					outfile<<" "<<cryp[4]<<flush;
				}
			}
			outfile<<"\n";
		}		
		outfile<<"Execution Time: "<<duration<<" secs\n\n"; 
		outfile.close();
	}else if(mode==1){
		ofstream outfile(outputFile, ios::app);
		for(int i=0; i<cryptosIDs.size(); i++){
			UserKnownCryptos user=cryptosIDs[i];
			outfile<< user.getUserID();
			//cout<< user.getUserID()<<flush;
			vector<int> userC = user.getKCVector();
			for(int j=0; j<userC.size(); j++){
				vector<string> cryp = crypto[ userC[j] ];
				if(cryp.size() == 4){
					outfile<<" "<<cryp[0]<<flush;
				}else{
					outfile<<" "<<cryp[4]<<flush;
				}
			}
			outfile<<"\n";
		}		
		outfile<<"Execution Time: "<<duration<<" secs\n\n"; 
		outfile.close();
	}	
}

vector< UserCryptoVector > getVirtualUsers(Hashtable2 &clusters, Tweet tweets[], map<string , double> &vaderLexicon, vector<string> cryptos[], int &alpha, vector< UserKnownCryptos > &knownCryptos, int cryptosNo, int tweetsNo){

	vector<UserCryptoVector> retVector;									//upologizw tous eikonikous xrhstes sumfwna me ta tweets
	int htsize = clusters.getTableSize();								//pou periexei ka8e cluster, pou proekupsan apo th 2h ergasia
	//cout<<"TABLESIZE: "<<htsize<<endl;
	double inf= numeric_limits<double>::infinity();

	vector<double> zero;
	for(int i=0; i<cryptosNo; i++){
		zero.push_back(0.0);
	}
	
	for(int i=0; i<htsize; i++){
		UserCryptoVector vuser(cryptosNo);
		vuser.setUserID(i);
		vuser.setCVectorWhole(zero);
		retVector.push_back(vuser);
	}
	
	for(int i=0; i<htsize; i++){
		list< Element* > clusterList = clusters.getList(i);
		int clusterSize = clusterList.size();

		if(clusterSize!=0){

			vector<double> vuserV = retVector[i].getCVector();

			for(list< Element* >::iterator it=clusterList.begin(); it!=clusterList.end(); ++it){
				Element *p = *it;
				int tid = p->getID();

				vector<string> ttweet;	
				for(int j=0; j<tweetsNo; j++){
					if(tid==tweets[j].getTweetID()){
						ttweet = tweets[j].getTweet();
						break;
					}
				}
				double sentimentVal=0.0;
				vector<int> tcryptos;
				for(int j=0; j<ttweet.size(); j++){
					double val = getSentiment( ttweet[j], vaderLexicon );
					if(val==0.0){
						int pos=getCrypto( ttweet[j], cryptos, cryptosNo );
						///cout<<pos<<endl;
						if(pos!=-1){ 
							tcryptos.push_back(pos);
						}
						///cout<<(t[i])<<endl;
						///cout<<" : "<<val<<endl;
					}else{
						sentimentVal += val;
					}
				}
				sentimentVal = sentimentVal/sqrt( sentimentVal*sentimentVal + alpha);
				UserCryptoVector user(cryptosNo);
				user.setUserID( tid );
				for(int k=0; k<tcryptos.size(); k++){
					user.setCVector( sentimentVal, tcryptos[k] );
				}
				vector<double> userV = user.getCVector();
				double avg_vec = getVectorAverage(userV);

				vector<double> normal_ucv1;
				for(vector<double>::iterator it=userV.begin(); it!=userV.end(); ++it){
					if( (*it)==inf ){
						normal_ucv1.push_back(0.0);
					}else{
						normal_ucv1.push_back( (*it)-avg_vec );
					}
				}
				
				vuserV = addVectors(vuserV, normal_ucv1);
			}

			retVector[i].setCVectorWhole(vuserV);
		}
	}
	
	return retVector;
}

vector< UserCryptoVector > getClusterMeans(Hashtable &clusters, int cryptosNo){

	vector<UserCryptoVector> retVector;							//pairnw to meso dianusma xrhstwn ka8e cluster
	int htsize = clusters.getTableSize();
	//cout<<"TABLESIZE: "<<htsize<<endl;

	vector<double> zero;
	for(int i=0; i<cryptosNo; i++){
		zero.push_back(0.0);
	}
	
	for(int i=0; i<htsize; i++){
		UserCryptoVector vuser(cryptosNo);
		vuser.setUserID(i);
		vuser.setCVectorWhole(zero);
		retVector.push_back(vuser);
	}
	
	for(int i=0; i<htsize; i++){
		list< UserCryptoVector* > clusterList = clusters.getList(i);
		int clusterSize = clusterList.size();

		if(clusterSize!=0){

			vector<double> vuserV = retVector[i].getCVector();

			for(list< UserCryptoVector* >::iterator it=clusterList.begin(); it!=clusterList.end(); ++it){
				UserCryptoVector *p = *it;
				vector<double> v= p->getCVector();

				vuserV = addVectors(vuserV, v);
			}

			retVector[i].setCVectorWhole(vuserV);
		}
	}
	
	return retVector;

}

vector< UserKnownCryptos > recommendCluster(vector< UserCryptoVector > &normal_ucv, vector< UserCryptoVector > &virtualUsers, vector< UserKnownCryptos > &knownCryptos, vector<double> centroidsVectors[], int k, int n){

	vector< UserKnownCryptos > ret;

	for(int i=0; i<normal_ucv.size(); i++){										//gia ka8e dianusma xrhsth
		UserCryptoVector user=normal_ucv[i];
		int id = user.getUserID();
		vector<double> vec = user.getCVector();		
		
		vector<double> distances;												//upologizw tis apostaseis tou apo ka8e centroid
		for(int j=0; j<k; j++){
			double euclideanDist = euclidean(vec, centroidsVectors[j]);
			distances.push_back(euclideanDist);
		}
		
		double mindist = pow(2.0, (double)(sizeof(double)*8));					//briskw to cluster pou tha pesei
		int minPos=k+1;

		for(int l=0; l<k; l++){
			if( distances[l] < mindist ){
				mindist = distances[l];
				minPos = l;
			}
		}		

		UserCryptoVector vuser = virtualUsers[minPos];
		vector<UserCryptoVector> virt;
		virt.push_back(vuser);

		UserKnownCryptos qukc;													//briskw ta cryptos pou exei anafer8ei to query
		for(int j=0; j<knownCryptos.size(); j++){
			int uid = knownCryptos[j].getUserID();
			if(uid==id){
				qukc = knownCryptos[j];
				break;
			}
		}

		vector<double> predicted = predictUknownCryptos(user, virt, qukc);		//kanw problepsh twn agnwstwn
		vector<int> recommended = getCryptos(predicted, qukc, n);				//pairnw ta n beltista

		UserKnownCryptos rec;
		rec.setUserID( id );
		rec.setKCVector( recommended );
		ret.push_back( rec );

		distances.clear();
		virt.clear();
	}

	return ret;
}

vector< UserKnownCryptos > recomCluster(vector< UserCryptoVector > &normal_ucv, Hashtable &clusters, vector< UserKnownCryptos > &knownCryptos, vector<double> centroidsVectors[], int k, int n){

	vector< UserKnownCryptos > ret;

	for(int i=0; i<normal_ucv.size(); i++){										//gia ka8e dianusma xrhsth
		UserCryptoVector user=normal_ucv[i];
		int id = user.getUserID();
		cout<<"Clustering Query: "<<i<<endl;
		vector<double> vec = user.getCVector();		
		
		vector<double> distances;												//upologizw tis apostaseis tou apo ka8e centroid
		for(int j=0; j<k; j++){
			double euclideanDist = euclidean(vec, centroidsVectors[j]);
			distances.push_back(euclideanDist);
		}
		
		double mindist = pow(2.0, (double)(sizeof(double)*8));					//briskw to cluster pou tha pesei
		int minPos=k+1;

		for(int l=0; l<k; l++){
			if( distances[l] < mindist ){
				mindist = distances[l];
				minPos = l;
			}
		}

		vector< UserCryptoVector > NNnucv;										//sumbouleuomai ola ta elements tou cluster
		list< UserCryptoVector* > bucket = clusters.getList(minPos);
		for(list< UserCryptoVector* >::iterator it=bucket.begin(); it!=bucket.end(); ++it){
			UserCryptoVector u = **it;
			int idu = u.getUserID();
			if(id!=idu){
				NNnucv.push_back( u );
			}
		}

		UserKnownCryptos qukc;													//briskw ta cryptos pou exei anafer8ei to query
		for(int j=0; j<knownCryptos.size(); j++){
			int uid = knownCryptos[j].getUserID();
			if(uid==id){
				qukc = knownCryptos[j];
				break;
			}
		}

		vector<double> predicted = predictUknownCryptos(user, NNnucv, qukc);		//kanw problepsh twn agnwstwn
		vector<int> recommended = getCryptos(predicted, qukc, n);				//pairnw ta n beltista

		UserKnownCryptos rec;
		rec.setUserID( id );
		rec.setKCVector( recommended );
		ret.push_back( rec );

		distances.clear();
		NNnucv.clear();
	}
	
	return ret;
}

vector< UserKnownCryptos > recommendCosineLSH(vector< UserCryptoVector > &normal_ucv, vector< UserCryptoVector > &virtualUsers, vector< UserKnownCryptos > &knownCryptos, int k, int P, int n){

	vector< UserKnownCryptos > ret;

	for(int i=0; i<normal_ucv.size(); i++){										//gia ka8e dianusma xrhsth
		UserCryptoVector user=normal_ucv[i];
		int id = user.getUserID();
		vector<double> vec = user.getCVector();	

		vector< pair<int, double> > distances;									//upologizw tis apostaseis apo tous eikonikous xrhstes
		for(int j=0; j<virtualUsers.size(); j++){
			UserCryptoVector vuser=virtualUsers[j];
			int vid = vuser.getUserID();
			vector<double> vvec = vuser.getCVector();	

			double cosineDist = 1-cosineSim(vec, vvec);
			distances.push_back( make_pair(vid, cosineDist) );
		}

		sort( distances.begin(), distances.end(), mysort);						//tis ta3inomw

		int selNum;
		if(k<=P){
			selNum=k;
		}else{
			selNum=P;
		}

		vector< UserCryptoVector > bestNeighbors;								//epilegw ta P kalutera
		for(int j=0; j<selNum; j++){
			UserCryptoVector u = virtualUsers[ distances[j].first ];
			bestNeighbors.push_back(u);
		}

		UserKnownCryptos qukc;													//briskw ta cryptonomismata sta opoia exei anafer8ei
		for(int j=0; j<knownCryptos.size(); j++){
			int uid = knownCryptos[j].getUserID();
			if(uid==id){
				qukc = knownCryptos[j];
				break;
			}
		}

		vector<double> predicted = predictUknownCryptos(user, bestNeighbors, qukc);		//kanw problepsh twn agnwstwn cryptonomismatwn
		vector<int> recommended = getCryptos(predicted, qukc, n);						//pairnw ta n beltista cryptonomismata

		UserKnownCryptos rec;
		rec.setUserID( id );
		rec.setKCVector( recommended );
		ret.push_back( rec );

		distances.clear();
		bestNeighbors.clear();
	}

	return ret;
}






