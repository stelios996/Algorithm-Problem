CC=g++
CFLAGS= -c -std=c++11

all: recommendation

recommendation: main.o preprocess.o classes.o tweet_analyzation.o distance.o hash.o recommendation.o search.o LSHClustering.o Brecommendation.o
	$(CC) -o recommendation main.o preprocess.o classes.o tweet_analyzation.o distance.o hash.o recommendation.o search.o LSHClustering.o Brecommendation.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

preprocess.o: preprocess.cpp
	$(CC) $(CFLAGS) preprocess.cpp

classes.o: classes.cpp
	$(CC) $(CFLAGS) classes.cpp

tweet_analyzation.o: tweet_analyzation.cpp
	$(CC) $(CFLAGS) tweet_analyzation.cpp

distance.o: distance.cpp
	$(CC) $(CFLAGS) distance.cpp

hash.o: hash.cpp
	$(CC) $(CFLAGS) hash.cpp

recommendation.o: recommendation.cpp
	$(CC) $(CFLAGS) recommendation.cpp

search.o: search.cpp
	$(CC) $(CFLAGS) search.cpp

LSHClustering.o: LSHClustering.cpp
	$(CC) $(CFLAGS) LSHClustering.cpp

Brecommendation.o: Brecommendation.cpp
	$(CC) $(CFLAGS) Brecommendation.cpp

clean:
	rm -f *.o recommendation
