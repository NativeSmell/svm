#include "SVM.h"

int main(int argc, char *argv[]){

	argc != 8 ? cout << "Error! Check arguments\n" , system("pause") ,exit(1) : 0;

	SVM svm(argv[1]);
	
	svm.startLearning(atoi(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
	svm.clear();
	svm.startValidation(atoi(argv[5]), atoi(argv[6]));
	svm.clear();
	svm.startTest(atoi(argv[5]),atoi(argv[6]));
	svm.clear();

	cout << "Done!";
	getchar();
	return 0;
}