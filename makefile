# Create Project_1 if you execute just "make"
all: main

#Create a rule for creating Project_1 executable
main: main.o createTestSet.o vectorUtility.o matrixUser.o matrixItem.o ratingPrediction.o
	g++ -O3 -std=c++0x main.o matrixUser.o matrixItem.o createTestSet.o vectorUtility.o ratingPrediction.o -o main 
	rm main.o createTestSet.o vectorUtility.o matrixItem.o matrixUser.o ratingPrediction.o
	
#Create rules for creating each intermediate object file
#Adding the header to the end tells make that if that file
#changes, the object needs to be recompiled
main.o: main.cpp createTestSet.o vectorUtility.o matrixUser.o matrixItem.o
	g++ -O3 -std=c++0x -c main.cpp -o main.o
	
createTestSet.o: createTestSet.cpp createTestSet.h
	g++ -O3 -std=c++0x -c createTestSet.cpp -o createTestSet.o

vectorUtility.o: vectorUtility.cpp vectorUtility.h
	g++ -O3 -std=c++0x -c vectorUtility.cpp -o vectorUtility.o

matrixUser.o: matrixUser.cpp matrixUser.h
	g++ -O3 -std=c++0x -c matrixUser.cpp -o matrixUser.o

matrixItem.o: matrixItem.cpp matrixItem.h
	g++ -O3 -std=c++0x -c matrixItem.cpp -o matrixItem.o

ratingPrediction.o: ratingPrediction.cpp ratingPrediction.h
	g++ -O3 -std=c++0x -c ratingPrediction.cpp -o ratingPrediction.o
