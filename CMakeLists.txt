cmake_minimum_required(VERSION 3.6)
project(SVM)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SVM.cpp Attributes.h HOG.h MNIST.h MNISTpicture.h Palette.h Picture.h SVM.h)
add_executable(SVM ${SOURCE_FILES})
