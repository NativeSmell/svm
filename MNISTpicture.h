#ifndef MNISTpicture_H_INCLUDED 
#define MNISTpicture_H_INCLUDED

#include "Picture.h"

class MNISTpicture : public Picture
{
private:

	int label;

public:
	MNISTpicture(char* fileName, int label, int r, int c) : Picture(fileName), label(label) { width = c; height = r; ext = ".bmp"; }
	MNISTpicture(char* fileName, int r, int c) : Picture(fileName) { width = c; height = r; ext = ".bmp"; }
	MNISTpicture(std::string fileName, int label, int r, int c) : Picture(fileName), label(label) { width = c; height = r; ext = ".bmp"; }
	~MNISTpicture(){}

	void setPalette(RGBTRIPLE* palette){
		this->palette = palette;
	}

	void setLabel(const int value){ this->label = value; }
	int getLabel() { return this->label; }

};

#endif // MNISTpicture_H_INCLUDED