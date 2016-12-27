#ifndef Palette_H_INCLUDED
#define Palette_H_INCLUDED

#include "Attributes.h"

class Palette : public Attributes {

public:
	Palette(Picture* picture) : Attributes(picture) { name = (char*)"Palette"; }
	Palette(){ name = (char*)"Palette"; }

	void calculateAttribure(){
		int w = picture->getWidth();
		int h = picture->getHeight();

		RGBTRIPLE* temp = picture->getPalette();
 
		for (int i = 0; i < h; i++){
			for (int j = 0; j < w; j++){
				attributes.push_back((double)temp[i*w + j].rgbtRed);
			}
		}
	}

};



#endif //Palette_H_INCLUDED