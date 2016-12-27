# svm
The implementation of Support Vector Machine for recognition of handwritten digits

This implementation contains a two methods of extracting features. To select the desired feature you need add argument of command line.

This tool takes 7 arguments of command lime:

1) Path to MNIST data base
2) C - normalizing factor
3) Nu - step of gradient
4) LearnSize - Size of learn samples (Max for MNIST is 60000)
5) MagicNumber - Version of classifire (any number)
6) Features - Method of features selection
   1 - Clear palette
   2 -  Histogram of Oriented Gradients, HOG
 7) If you using HOG - you must specify the number of intervals for quantization
 
 
 Example for Windows:
    start SVM_Learn.exe MNIST\ 25 0.0075 38000 1 2 15
   
    to learn and test SVM with HOG method (15 intervals for quantization) on 380000 pictures of handwritten digits and with step of gradient equels 0.075
    
    start SVM_Learn.exe MNIST\ 25 0.0075 38000 1 1 0
     
     to learn and test SVM with standart picture Palette on 380000 pictures of handwritten digits and with step of gradient equels 0.075
