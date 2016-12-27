cmake_minimum_required(VERSION 3.6)
project(svm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    Attributes.h
    HOG.h
    main.cpp
    MNIST.h
    MNISTpicture.h
    Palette.h
    Picture.h
    SVM.cpp
    SVM.h)

add_executable(svm ${SOURCE_FILES})