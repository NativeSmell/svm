#ifndef ATTRIBUTES_H_INCLUDED
#define ATTRIBUTES_H_INCLUDED
#include "Picture.h"
 

/*

1 - blockSum - сред ариф по блокам 10х10          54 признаков
2 - Haar - два признака Хаара по левой и правой части        2 признаков
3 - PairCorrelation - попарная корреляция по 4 четвертям			6 признаков
4 - CountBlack - кол-во черных пикселей в блоке 10x10 деленное на 100			54 признака
5 - Dispersedness - периметр(кол-во пикселей в контуре) в квадрате деленные на площадь			1 признак
6 - HOG - Histogram of Oriented Gradients - Гистограмма направленных градиентов для блока 10 x 10 в 8 направлениях
*/


typedef struct Block{
	vector<int> mas;
}Block;

class Attributes
{
protected:
	vector<double>attributes;
	Picture* picture;
	char* name;

	double math_exp;
	double disp;

public:

	Attributes(Picture* picture) : picture(picture){}
	Attributes(){}
	~Attributes() {}

	vector<double> getAttribure(){
		return attributes;
	}

	void clear(){
		attributes.clear();
	}

	void setPicture(Picture* picture){
		this->picture = picture;
	}

	virtual void calculateAttribure() = 0;

	char* getName(){
		return this->name;
	}

	void normalization(){
		math_exp = 0.0;
		disp = 0.0;
		for (int i = 0; i < attributes.size(); i++){
			math_exp += attributes.at(i);
			disp += attributes.at(i)*attributes.at(i);
		}
		math_exp /= attributes.size();
		disp /= attributes.size();


#pragma omp parallel for
		for (int i = 0; i < attributes.size(); i++)
			attributes[i] = (attributes.at(i) - math_exp) / (disp - 1);

	}


};

#endif // ATTRIBUTES_H_INCLUDED