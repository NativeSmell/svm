#include <stdlib.h>
#include "SVM.h"

int main(int argc, char *argv[]){

        if(argc == 5) {
            SVM svm(argv[1]);
            svm.checkClassifier(atoi(argv[2]), atoi(argv[3]), argv[4]);
        }

    else {
            bool check_arg;
            argc != 9 ? cout << "Error! Check arguments\n", check_arg = false : check_arg = true;

            if (check_arg) {
                SVM svm(argv[1], argv[2]);

                svm.startLearning(atoi(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]));
                svm.clear();
                svm.startValidation(atoi(argv[6]), atoi(argv[7]));
                svm.clear();
                svm.startTest(atoi(argv[6]), atoi(argv[7]));
                svm.clear();

                cout << "Done!";
            }
        }
	
	cout << std::endl;
	return 0;
}
