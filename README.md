# svm
The implementation of Support Vector Machine for recognition of handwritten digits

This implementation contains two methods of extracting features. To select the desired feature you need to add an argument of command line.

This tool takes 8 arguments of command line:

   1) Path to MNIST data base (http://yann.lecun.com/exdb/mnist/)

   2) Path to the folder where the results will be recorded (Create in this directory "logs" folder if you wanna see logs and test results)
   
   3) C - normalizing factor
   
   4) Nu - step of gradient
   
   5) LearnSize - Size of learn samples (Max for MNIST is 60000)
   
   6) MagicNumber - Version of classifire (any number)
   
   7) Features - Method of features selection
   
      1 - Clear palette
      
      2 -  Histogram of Oriented Gradients, HOG
      
   8) If you are using HOG - you must specify the number of intervals for quantization
 
 
Example:
 
      Windows: start SVM_Learn.exe MNIST/ SVMClassifiers/ 25 0.0075 38000 1 2 15
    
      Linux: svm /home/home_pc/data/MNIST/ /home/home_pc/data/SVMClassifiers/ 25 0.0075 38000 1 2 15

To learn and test SVM with HOG method (15 intervals for quantization) on 380000 pictures of handwritten digits and with step of gradient equal 0.075
    
      Windows: start SVM_Learn.exe MNIST/ SVMClassifiers/ 25 0.0075 38000 1 2 15
    
      Linux: svm /home/home_pc/data/MNIST/ /home/home_pc/data/SVMClassifiers/ 25 0.0075 38000 1 2 15
     
To learn and test SVM with standart picture Palette on 380000 pictures of handwritten digits and with step of gradient equal 0.075

