#ifndef SVM_H_INCLUDED
#define SVM_H_INCLUDED

#include"HOG.h"
#include"Palette.h"
#include "Picture.h"
#include <iostream>
#include "MNIST.h"
#include <ctime>

#pragma warning(disable : 4996)

typedef struct X{
	vector<double> mas;
	int label;
	int mark;
}X;

typedef struct classifiers{
	double result;
	vector<double> omega;
}classifiers;

class SVM {
private:
	vector<X*> x; 
	
	vector<classifiers*> classifier;
	vector<double> omega;
	vector<double> _omega;
	vector<MNISTpicture*> base;
	X thisX;

	vector<Attributes*> attributes;

	int C;
	double Nu; 
	size_t learnSize;
	size_t validationSize;
	size_t iterNum = 100000;
	size_t bin;


	int N;
	double lambda;
	double LNorm;


	double P = 0.0;
	double newErr = 0.0;
	double currErr = 0.0;

	int magicNumber;

	char* path;
	char* path_to_classifire;

	double calculateFy(vector<double>x, vector<double> omega);
	void subGradient(vector<double> x, const int label);
	void subGradient();
	double calculateFy(vector<double>x);
	double sign(const double value);
	void write(const int magicNumber);
	void read(int N, int magicNumber);
	void readparam(int magicNumber);

public:

	SVM(char* path, char* path_to_classifire) : path(path) , path_to_classifire(path_to_classifire){ }
	SVM(char* path_to_classifire) : path_to_classifire(path_to_classifire){}


	~SVM(){}
	
	void clear(){
		x.clear();
		classifier.clear();
		base.clear();
		P = 0.0;
		C = 0;
		Nu = 0;
		learnSize = 0;
		attributes.clear();
	}

	void startLearning(int _C, double _Nu, size_t _learnSize, int magicNumber, short features, size_t _bin);
	void startValidation(int magicNumber, short features);
	void startTest(int magicNumber, short features);
	void checkClassifier(int magicNumber, short features, char* path_to_image);
};

#endif // SVM_H_INCLUDED