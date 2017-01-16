#include "SVM.h"
#include <fstream>

void SVM::read(int N, int number){

	char thisName[512];
	sprintf(thisName, "%s%dClassifier.svm", path_to_classifire,number);

	FILE *fp = fopen(thisName, "rb");

	if (fp == nullptr){
		cout << "Error, can not open " << thisName;
		getchar();
		exit(2);
	}

	fread(&magicNumber, sizeof(int), 1, fp);
	fread(&C, sizeof(size_t), 1, fp);
	fread(&Nu, sizeof(double), 1, fp);
	fread(&learnSize, sizeof(size_t), 1, fp);
	fread(&iterNum, sizeof(size_t), 1, fp);
	fread(&bin, sizeof(size_t), 1, fp);

	double *buf = nullptr;

	for (size_t k = 0; k < 10; k++){
		classifier.push_back(new classifiers());
		buf = new double[N];
		fread(buf, sizeof(double), N, fp);

		for (size_t i = 0; i < N; i++){
			classifier.at(k)->omega.push_back(buf[i]);
		}
	}

	fclose(fp);
}

void SVM::readparam(int number){

	char thisName[512];
	sprintf(thisName, "%s%dClassifier.svm",path_to_classifire ,number);

	FILE *fp = fopen(thisName, "rb");

	if (fp == nullptr){
		cout << "Error, can not open " << thisName;
		getchar();
		exit(2);
	}

	fread(&magicNumber, sizeof(int), 1, fp);
	fread(&C, sizeof(size_t), 1, fp);
	fread(&Nu, sizeof(double), 1, fp);
	fread(&learnSize, sizeof(size_t), 1, fp);
	fread(&iterNum, sizeof(size_t), 1, fp);
	fread(&bin, sizeof(size_t), 1, fp);

	fclose(fp);
}

void SVM::write(const int magicNumber){

	char thisName[512];
	sprintf(thisName, "%s%dClassifier.svm",path_to_classifire, magicNumber);

	FILE *fp = fopen(thisName, "wb");

	if (fp == nullptr){
		cout << "Error, can not open " << thisName;
		getchar();
		exit(2);
	}

	fwrite(&magicNumber, sizeof(int), 1, fp);
	fwrite(&C, sizeof(size_t), 1, fp);
	fwrite(&Nu, sizeof(double), 1, fp);
	fwrite(&learnSize, sizeof(size_t), 1, fp);
	fwrite(&iterNum, sizeof(size_t), 1, fp);
	fwrite(&bin, sizeof(size_t), 1, fp);

	double *buf = nullptr;

	buf = new double[_omega.size()];
	for (size_t i = 0; i < _omega.size(); i++){
		buf[i] = _omega.at(i);
	}

	fwrite(buf, sizeof(double), _omega.size(), fp);

	delete[] buf;

	fclose(fp);
}

double SVM::sign(const double value){
	return (value < 0) ? -1.0 : 1.0;
}

double SVM::calculateFy(vector<double>x, vector<double> omega){

	double Y = 0;

	for (int i = 0; i < omega.size(); i++)
		Y += omega.at(i)*x.at(i);
	
	return Y;
}

double SVM::calculateFy(vector<double>x){

	double Y = 0.0;

	for (int i = 0; i < omega.size(); i++){
		Y += omega.at(i)*x.at(i);
	}

	return Y;
}

void SVM::subGradient(vector<double> x, const int label){

	double fx = calculateFy(x) * label;

#pragma omp parallel for 
		for (int j = 0; j < omega.size(); j++) 
			omega[j] = fx < 1 ? omega.at(j) - Nu * (lambda*omega.at(j) - label*x.at(j)) : omega.at(j) - Nu*lambda*omega.at(j);
}

void SVM::subGradient(){

	double* sum = new double[omega.size()];
	memset(sum, 0, omega.size() * sizeof(double));

	for (int i = 0; i < x.size(); i++){

		double fx = calculateFy(x.at(i)->mas) * x.at(i)->label;

		for (int j = 0; j < omega.size(); j++)
			sum[j] += fx < 1 ? (lambda * omega.at(j) - x.at(i)->label * x.at(i)->mas.at(j)) : lambda * omega.at(j);
	}

#pragma omp parallel for 
	for (int j = 0; j < omega.size(); j++)
		omega[j] = omega.at(j) - Nu * (sum[j] / x.size());


	delete[] sum;
}

void SVM::startLearning(int _C, double _Nu, size_t _learnSize, int magicNumber, short features, size_t _bin){
	cout << "*****************************Start SVM Learning*****************************" << std::endl;

	unsigned int start_time = clock();

	C = _C;
	Nu = _Nu;
	learnSize = _learnSize;
	bin = _bin;

	char thisName[512], thisName1[512];
	sprintf(thisName, "%strain-images.idx3-ubyte", path);
	sprintf(thisName1, "%strain-labels.idx1-ubyte", path);

	MNIST mnist(thisName, thisName1);

	base = mnist.getBase();

	std::ofstream out, err, count;

	char logName[512],lr_count[512], err_csv[512];
	sprintf(logName, "%s/logs/log_learn.txt", path_to_classifire);
	sprintf(lr_count, "%s/logs/LearnCount.txt", path_to_classifire);
	sprintf(err_csv, "%s/logs/%d_err.CSV", path_to_classifire, magicNumber);

	out.open(logName, std::ios::app);

	if (!out.is_open()){
		cout << "open log file Error!" << std::endl;
	}
	count.open(lr_count);
	if (!count.is_open()){
		cout << "open lr_count file Error!"<< std::endl;
	}
	err.open(err_csv);
	if (!err.is_open()){
		cout << "open err_csv file Error!"<< std::endl;
	}

	int iter[10] = { 0x00 };
	int iter_time[10] = { 0x00 };
	int digitCount[10] = { 0x00 };

	switch (features)
	{
	case 1: 
		attributes.push_back(new Palette());
	case 2: 
		attributes.push_back(new HOG(bin));
	default:
		break;
	}


	cout << "\nfeatures selection: " << attributes.at(0)->getName() << "\n";
	for (int k = 0, proc = 0; k < learnSize; k++){
		X* Xi = new X();
		Xi->mas.push_back(1.0);
		Xi->mark = base.at(k)->getLabel();
		digitCount[Xi->mark]++;
		for (int i = 0; i < attributes.size(); i++){
			attributes.at(i)->setPicture(base.at(k));
			attributes.at(i)->calculateAttribure();
			for (int j = 0; j < attributes.at(i)->getAttribure().size(); j++){
				Xi->mas.push_back(attributes.at(i)->getAttribure().at(j));

			}
			attributes.at(i)->clear();
		}

		x.push_back(Xi);
	}
	

	for (int i = 0; i < 10; i++){
		cout << i << "  :  " << digitCount[i] << std::endl;
		count << i << "  :  " << digitCount[i] << std::endl;
	}

	for (int cl = 0; cl < 10; cl++){

		cout << "\nLearn " << cl << " classifiers. Please wait!\n";
		unsigned int lear_start_time = clock();
		omega.clear();
		N = (int)x.at(0)->mas.size();
		lambda = 2.0 / (double)(C * N);


#pragma omp parallel for
		for (int i = 0; i < x.size(); i++)
			x.at(i)->label = x.at(i)->mark == cl ? 1 : -1;


		for (int i = 0; i < N; i++)
			omega.push_back(rand() % 10);


		for (int t = 0; t < iterNum; t++){

			for (int i = 0; i < x.size(); i++)
				subGradient(x.at(i)->mas, x.at(i)->label);

	
			newErr = 0.0;

			for (int i = 0; i < x.size(); i++)
				x.at(i)->label != sign(calculateFy(x.at(i)->mas)) ? newErr += 1.0 : 0;

			newErr /= x.size();

			char str[512];
			sprintf(str, "%d classifier %d iter;%f;%f", cl, t, newErr, Nu);

			err << str << std::endl;

			if (abs(currErr - newErr) < 0.0000001){
				iter[cl] = t;
				unsigned int lear_end_time = clock();
				iter_time[cl] = lear_end_time - lear_start_time;
				break;
			}

			currErr = newErr;

		}

		for (int i = 0; i < omega.size(); i++)
			_omega.push_back(omega.at(i));


	}
	
	unsigned int end_time = clock();

	write(magicNumber);

	out << "//////////////////////////////////////////////////////\n"
		<< "Learning " << magicNumber << " classifier!" << std::endl;

	for (int i = 0; i < 10; i++)
		out << i << "classifier" << " iterNum = " << iter[i] <<"	:	Learn_Time = " << iter_time[i] << std::endl;

	out << "Work time: " << end_time - start_time << std::endl;
	out.close();
	count.close();
	err.close();
}

void SVM::startValidation(int magicNumber, short features){
	cout << "*****************************Start SVM Validation*****************************" << std::endl;

	char thisName[512], thisName1[512];
	sprintf(thisName, "%strain-images.idx3-ubyte", path);
	sprintf(thisName1, "%strain-labels.idx1-ubyte", path);

	MNIST mnist(thisName, thisName1);

	base = mnist.getBase();

	char logName[512];
	sprintf(logName, "%s/logs/log.txt", path_to_classifire);

	std::ofstream out;
	out.open(logName, std::ios::app);

	if (!out.is_open()){
		cout << "open log file Error!";
	}
	
	readparam(magicNumber);

	switch (features)
	{
	case 1:
		attributes.push_back(new Palette());
	case 2:
		attributes.push_back(new HOG(bin));
	default:
		break;
	}
	validationSize = learnSize < 35000 ? (int)(learnSize * 0.7) : base.size() - learnSize;
	cout << "\nfeatures selection: " << attributes.at(0)->getName() << "\n";
	for (int k = (int)base.size() - validationSize, proc = 0; k < base.size(); k++){
		X* Xi = new X();
		Xi->mas.push_back(1.0);
		Xi->mark = base.at(k)->getLabel();
		for (int i = 0; i < attributes.size(); i++){
			attributes.at(i)->setPicture(base.at(k));
			attributes.at(i)->calculateAttribure();
			for (int j = 0; j < attributes.at(i)->getAttribure().size(); j++){
				Xi->mas.push_back(attributes.at(i)->getAttribure().at(j));
			}
			attributes.at(i)->clear();
		}
		x.push_back(Xi);

	}
	read((int)x.at(0)->mas.size(), magicNumber);

	cout << "\nStart validation\n";

	for (int k = 0; k < x.size(); k++){
		
#pragma omp parallel for
		for (int cl = 0; cl < 10; cl++)
			classifier.at(cl)->result = calculateFy(classifier.at(cl)->omega, x.at(k)->mas);

		int result = 0;
		double max = classifier.at(0)->result;
		for (int i = 1; i < 10; i++){
			max < classifier.at(i)->result ? max = classifier.at(i)->result, result = i : 0;
		}

		if (result != x.at(k)->mark)
			P++;
	}

	P /= (double)x.size();

	out << "Validation " << magicNumber << std::endl
		<< "C - " << C << " Nu - " << Nu << " learnSize - "
		<< learnSize << " iterNum - " << iterNum << " validation size -  "
		<< validationSize << " HOG - " << bin << std::endl << "accuracy percent - "
		<< (1 - P) * 100 << std::endl << std::endl;

	out.close();
}

void SVM::startTest(int magicNumber, short features){
	cout << "*****************************Start SVM Test*****************************" << std::endl;

	char thisName[512], thisName1[512];
	sprintf(thisName, "%st10k-images.idx3-ubyte", path);
	sprintf(thisName1, "%st10k-labels.idx1-ubyte", path);

	MNIST mnist(thisName, thisName1);

	base = mnist.getBase();

	readparam(magicNumber);

	switch (features)
	{
	case 1:
		attributes.push_back(new Palette());
	case 2:
		attributes.push_back(new HOG(bin));
	default:
		break;
	}
	
	cout << "\nfeatures selection: " << attributes.at(0)->getName() << "\n";
	for (int k = 0, proc = 0; k < base.size(); k++){
		X* Xi = new X();
		Xi->mas.push_back(1.0);
		Xi->mark = base.at(k)->getLabel();

		//char thisName[512];
		//sprintf(thisName, "%s/mnist_example/%d_%d.bmp", path,k, base.at(k)->getLabel());

		//base.at(k)->save(path,thisName);

		for (int i = 0; i < attributes.size(); i++){
			attributes.at(i)->setPicture(base.at(k));
			attributes.at(i)->calculateAttribure();
			for (int j = 0; j < attributes.at(i)->getAttribure().size(); j++){
				Xi->mas.push_back(attributes.at(i)->getAttribure().at(j));
			}

			attributes.at(i)->clear();
		}

		x.push_back(Xi);
	}

	char logName[512];
	sprintf(logName, "%s/logs/log.txt", path_to_classifire);

	std::ofstream out;
	out.open(logName, std::ios::app);

	if (!out.is_open()){
		cout << "open log file Error!";
	}

	read((int)x.at(0)->mas.size(), magicNumber);

	cout << "\nStart test\n";

	for (int k = 0; k < x.size(); k++){

#pragma omp parallel for
		for (int cl = 0; cl < 10; cl++)
			classifier.at(cl)->result = calculateFy(classifier.at(cl)->omega, x.at(k)->mas);

		int result = 0;
		double max = classifier.at(0)->result;

		for (int i = 1; i < 10; i++){
			max < classifier.at(i)->result ? max = classifier.at(i)->result, result = i : 0;
		}

		if (result != x.at(k)->mark)
			P++;
	}

	P /= (double)x.size();

	out << "Testing " << magicNumber << std::endl
		<< "C - " << C << " Nu - " << Nu << " learnSize - "
		<< learnSize << " iterNum - " << iterNum << " Test size -  "
		<< base.size() << "HOG -" << bin << std::endl << "accuracy percent - "
		<< (1 - P) * 100 << std::endl << std::endl;

	out.close();
}


void SVM::checkClassifier(int magicNumber, short features, char* path_to_image) {

	readparam(magicNumber);

	Picture* picture = new Picture();

	picture->load_bmp(path_to_image);

	switch (features) {
		case 1:
			attributes.push_back(new Palette());
		case 2:
			attributes.push_back(new HOG(bin));
		default:
			break;
	}

	cout << "\nfeatures selection: " << attributes.at(0)->getName() << "\n";

	X *Xi = new X();
	Xi->mas.push_back(1.0);
	for (int i = 0; i < attributes.size(); i++) {
		attributes.at(i)->setPicture(picture);
		attributes.at(i)->calculateAttribure();
		for (int j = 0; j < attributes.at(i)->getAttribure().size(); j++) {
			Xi->mas.push_back(attributes.at(i)->getAttribure().at(j));
		}

		attributes.at(i)->clear();
	}
	read(Xi->mas.size(), magicNumber);


	for (int cl = 0; cl < 10; cl++)
		classifier.at(cl)->result = calculateFy(classifier.at(cl)->omega, Xi->mas);

	int result = 0;
	double max = classifier.at(0)->result;

	for (int i = 1; i < 10; i++){
		max < classifier.at(i)->result ? max = classifier.at(i)->result, result = i : 0;
	}

	cout << "It'is - " << result;

}