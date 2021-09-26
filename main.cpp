#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include "preprocess.h"
#include "classes.h"
#include "tweet_analyzation.h"
#include "recommendation.h"
#include "Brecommendation.h"

using namespace std;

int main(int argc, char* argv[]){

	char inputFile[500];
	char outputFile[500];
	int validate=0;

	int inputFlag=0, outputFlag=0;

	if(argc!=5 && argc!=6){												//diabazw ta orismata apo th grammh entolwn
		cerr<< "Usage: " << argv[0] << " NAME"<<endl;			//an o ari8mos twn orismatwn den upakouei, epistrefei error
		return -1;
	}else{
		for(int i=1; i<argc; i++){
			if(strcmp(argv[i],"-d") == 0){
				strcpy(inputFile , argv[i+1]);
				inputFlag=1;

			}else if(strcmp(argv[i],"-o") == 0){
				strcpy(outputFile , argv[i+1]);
				outputFlag=1;			

			}else if(strcmp(argv[i],"-validate") == 0){
				validate=1;
			}
		}
	}
	if(inputFlag==0 || outputFlag==0){
		cerr<< "Usage: " << argv[0] << " NAME"<<endl;			//an o ari8mos twn orismatwn den upakouei, epistrefei error
		return -2;
	}
	
	cout<< "INPUT FILE: " << inputFile << endl;
	cout<< "OUTPUT FILE: " << outputFile << endl;
	cout<< "VALIDATE: " << validate << endl <<endl;

	char currencyFile[100];
	strcpy(currencyFile , "./InputFiles/coins_queries.csv");
	char vaderFile[100];
	strcpy(vaderFile, "./InputFiles/vader_lexicon.csv");

	cout<< "CRYPTO FILE: " << currencyFile << endl;
	cout<< "VADER FILE: " << vaderFile << endl << endl;
	

	cout<< "Preprocessing data:" << endl;
	cout<< "-Reading cryptocurrency data ..." << endl;	
	int currencyData = getNumberOfLines(currencyFile);					//diabazw to plh8os twn cryptonomismatwn
	cout << "CURRENCY DATA: " << currencyData << endl;
	cout << "-Storing cryptocurrency data to memory ..." << endl << endl;
	vector<string> *crypto = new vector<string>[currencyData];			//apothikeuw ta cryptonomismata
	storeCrypto(currencyFile, crypto);

	cout << "-Reading Vader lexicon ..." << endl;
	int vaderData = getNumberOfLines(vaderFile);						//diabazw to plh8os dedomenwn tou le3ikou
	cout << "VADER LEXICON DATA: " << vaderData << endl;
	cout << "-Storing Vader lexicon to memory ..." << endl << endl;
	map<string, double> vaderLexicon;
	storeVaderLexicon(vaderFile, vaderLexicon);							//apo8ikeuw to le3iko se ena map

	cout << "-Reading input file data..." << endl;
	int tweetsNo = getNumberOfLines(inputFile);							//diabazw to plh8os twn tweets
	cout << "TWEETS DATA: " << tweetsNo << endl;
	cout << "-Storing input file data to memory ..." << endl;
	Tweet *tweets = new Tweet[tweetsNo];
	int p;
	storeTweets(inputFile, tweets, p);									//apo8ikeuw ta tweets
	cout<<"P: "<<p<<endl<<endl;
	
	char pr2_dataset[100] = "./InputFiles/twitter_dataset_small_v2.csv";
	cout<<"-Reading input file data from Proj2..."<<endl;
	int dataNo = getNumberOfLines(pr2_dataset);							//diabazw ton ari8mo twn dedomenwn ths 2hs ergasias
	cout<<"---number_of_data: "<< dataNo <<endl;    
	cout<<"-Storing input file data from Proj2..."<<endl;
	Element *data2 = new Element[dataNo];
    storeFileData(pr2_dataset, data2);									//apo8ikeuw ta dedomena ths 2hs ergasias

	cout << "Data preprocessing successful" << endl << endl;


	int alpha=15;
	vector< UserCryptoVector > ucv;
	vector< UserKnownCryptos > knownCryptos;		//dhmiourgw ta dianusmata xrhsth kai krataw ta cryptos pou anaferetai ka8e tweet
	createUserCryptoVectors(tweets, vaderLexicon, crypto, tweetsNo, currencyData, alpha, ucv, knownCryptos);

	vector< UserCryptoVector > normal_ucv;
	normalUserCryptoVectors(ucv, normal_ucv);		//kanonikopoiw ta dianusmata xrhsth

	//~~~~~~~~~~~~LSH Recommendation~~~~~~~~~~~~~~~
	int flag=0;
	while(1){

		if(flag==1){
			cout<<"Do you want to continue? (y or n)"<<endl<<">>";					//dinetai dunatothta ekteleshs allou sunduasmou sta
			char ch;																//idia dedomena
			cin>>ch;
			while(ch!='y' && ch!='n'){
				cout<<">>";
				cin>>ch;
			}
			if(ch=='n'){
				break;
			}
		}

		cout<<" 1. Cosine LSH Recommendation"<<endl;
		cout<<" 2. Clustering Recommendation"<<endl;
	
		cout<<endl<<"Select Algorithm (1-2): "<<endl<<">>";
		int comb;
		cin>>comb;
		while(comb<=0 || comb>2){
			cout<<">>";
			cin>>comb;
		}

		if(comb==1){

			clock_t start,end;
			start = clock();

			vector< UserKnownCryptos > cryptosIDs = CosineLSHRecommendation(normal_ucv, knownCryptos, currencyData, p);		//1A

			end = clock();
			double duration = (end-start)/(double)(CLOCKS_PER_SEC/1000);

			//cout<<"CRYPTOSIDS SIZE: "<<cryptosIDs.size()<<endl;
		
			cout<<"Writing results to output file..."<<endl;
			ofstream outfile(outputFile, ios::out);
			outfile << "Cosine LSH Recommendation PartA\n";
			outfile.close();
			writeToFile(outputFile, cryptosIDs, duration, crypto, 1);		//grafw ta apotelesmata sto arxeio
	
			clock_t start4,end4;
			start4 = clock();
	
			vector< UserKnownCryptos > cryptosIDs4 = BCosineLSHRecom(normal_ucv, data2, tweets, vaderLexicon, crypto, alpha, knownCryptos, currencyData, p, dataNo, tweetsNo);																					//1B

			end4 = clock();
			double duration4 = (end4-start4)/(double)(CLOCKS_PER_SEC/1000);

			//cout<<"CRYPTOSIDS4 SIZE: "<<cryptosIDs4.size()<<endl;
			cout<<"Writing results to output file..."<<endl;
			ofstream outfile4(outputFile, ios::app);
			outfile4 << "Cosine LSH Recommendation PartB\n";
			outfile4.close();
			writeToFile(outputFile, cryptosIDs4, duration4, crypto, 1);		//grafw ta apotelesmata sto arxeio

			cout<<endl<<"Cosine LSH Recommendation complete..."<<endl<<endl;

	//~~~~~~~~Clustering Recommendation~~~~~~~~~~~~~
		}else if(comb==2){
			clock_t start2,end2;
			start2 = clock();

			vector<UserKnownCryptos> cryptosIDs2 = ClusteringRecommendation(normal_ucv, knownCryptos, currencyData);		//2A

			end2 = clock();
			double duration2 = (end2-start2)/(double)(CLOCKS_PER_SEC/1000);

			//cout<<"CRYPTOSIDS2 SIZE: "<<cryptosIDs2.size()<<endl;
			cout<<"Writing results to output file..."<<endl;
			ofstream outfile2(outputFile, ios::out);
			outfile2 << "Clustering Recommendation PartA\n";
			outfile2.close();
			writeToFile(outputFile, cryptosIDs2, duration2, crypto, 1);		//grafw ta apotelesmata sto arxeio

			clock_t start3,end3;
			start3 = clock();

			vector< UserKnownCryptos > cryptosIDs3 = BClusteringRecom(normal_ucv, data2, tweets, vaderLexicon, crypto, alpha, knownCryptos, currencyData, p, dataNo, tweetsNo);																					//2B

			end3 = clock();
			double duration3 = (end3-start3)/(double)(CLOCKS_PER_SEC/1000);

			//cout<<"CRYPTOSIDS3 SIZE: "<<cryptosIDs3.size()<<endl;
			cout<<"Writing results to output file..."<<endl;
			ofstream outfile3(outputFile, ios::app);
			outfile3 << "Clustering Recommendation PartB\n";
			outfile3.close();
			writeToFile(outputFile, cryptosIDs3, duration3, crypto, 1);		//grafw ta apotelesmata sto arxeio


			cout<<endl<<"Clustering Recommendation complete..."<<endl<<endl;
		}
		
		if(flag==0){
			flag=1;
		}
	}

	delete[] data2;
	delete[] crypto;
	delete[] tweets;
	return 0;
}











