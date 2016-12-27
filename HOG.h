#ifndef HOG_H_INCLUDED
#define HOG_H_INCLUDED

#include "Attributes.h"
#include <math.h>

class cell{
public:
	int* mas;

	cell(int bin) {
		mas = new int[bin];
		for (int i = 0; i < bin; i++){
			mas[i] = 0;
		}
	}
	~cell() { delete[] mas; }
};

class HOG : public Attributes {
private:
	int newH = 28;
	int newW = 28;
	int bin;
	int cell_size = 4;

public:
	HOG(Picture* picture, int bin) : Attributes(picture), bin(bin) { name = (char*)"HOG "; }
	HOG(int bin) : bin(bin){ name = (char*)"HOG "; }

	void calculateAttribure(){

		int GX[3][3] = { { -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 } };

		int GY[3][3] = { { 1, 2, 1 },
		{ 0, 0, 0 },
		{ -1, -2, -1 } };

		int w = picture->getWidth();
		int h = picture->getHeight();

		RGBTRIPLE* temp = picture->getPalette();

		BYTE **matrix = new BYTE*[newH];
		for (int i = 0; i < newH; i++){
			matrix[i] = new BYTE[newW];
			for (int j = 0; j < newW; j++){
				matrix[i][j] = temp[i*newW + j].rgbtRed;
			}
		}

		vector<int> orient;

		for (int i = 0; i < newH; i++){
			for (int j = 0; j < newW; j++){
				int sumX = 0;
				int sumY = 0;
				for (int k = 0; k < 3; k++){
					for (int l = 0; l < 3; l++){
						BYTE temp = 0;
						if (i - 1 + k < 0 || j - 1 + l < 0) temp = matrix[0][0];
						else if (i - 1 + k >= newH || j - 1 + l >= newW) temp = matrix[0][0];
						else temp = matrix[i - 1 + k][j - 1 + l];
						sumY += temp * GY[k][l];
						sumX += temp * GX[k][l];
					}
				}

				double temp = atan2(sumY, sumX)*(180 / 3.14);
				temp += temp >= 0 ? 0.5 : 360.5;
				orient.push_back((int)(temp / (360 / bin)));
			}
		}

		Histagram(orient);

		for (int i = 0; i < h; i++)
			delete[] matrix[i];

		normalization();
	}

	void Histagram(vector<int> orient){

		vector<cell*> cells;

		int i, j, k, c, x = 0;
		for (i = 0; i < newH; i += cell_size){
			for (j = 0; j < newW; j += cell_size, x++){
				cells.push_back(new cell(bin));
				for (k = 0; k < cell_size; k++){
					for (c = 0; c < cell_size; c++){
						cells.at(x)->mas[orient.at((i + k)*newW + j + c)]++;
					}
				}
				for (int p = 0; p < bin; p++)
					attributes.push_back((double)cells.at(x)->mas[p]);
			}
		}
	}
};



#endif //HOG_H_INCLUDED