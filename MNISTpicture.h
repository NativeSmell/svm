#ifndef MNISTpicture_H_INCLUDED 
#define MNISTpicture_H_INCLUDED

#include <stdlib.h>
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

    void save(char* path,char* name) {

        FILE *out,*src;

        char thisName[512];
        sprintf(thisName, "%smnist_example/0_1.bmp", path);

        src = fopen(thisName, "r+b");
        out = fopen(name, "w+b");

        unsigned char info[54];
        fread(info, sizeof(unsigned char), 54, src);

        fwrite(info, sizeof(unsigned char), 54, out);
        fwrite(palette, sizeof(RGBTRIPLE), width*height, out);

        fclose(out);
        fclose(src);

    }

};

#endif // MNISTpicture_H_INCLUDED