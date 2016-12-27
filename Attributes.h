#ifndef ATTRIBUTES_H_INCLUDED
#define ATTRIBUTES_H_INCLUDED
#include "Picture.h"
 

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