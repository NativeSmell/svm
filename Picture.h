#ifndef PICTURE_H_INCLUDED
#define PICTURE_H_INCLUDED

#include <iostream>
#include <vector>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

using std::cout;
using std::vector;

typedef unsigned char BYTE;

typedef struct RGBTRIPLE{
	BYTE rgbtBlue;
	BYTE rgbtGreen;
	BYTE rgbtRed;
}RGBTRIPLE;

const BYTE WHITE = 255;
const BYTE BLACK = 0;

class Picture{

protected:
	const char* filename;
	const char* ext;
	int width, height;
	RGBTRIPLE rgb_l, *palette;

public:
	Picture(char* name) : filename(name) {}
	Picture(std::string name) { filename = name.c_str(); }
	~Picture() { delete palette; }

	int getHeight(){ return this->height; }
	int getWidth() { return this->width; }
	RGBTRIPLE* getPalette(){ return this->palette; }
};

#endif // PICTURE_H_INCLUDED