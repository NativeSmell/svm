#ifndef MNIST_H_INCLUDED
#define MNIST_H_INCLUDED

#include "MNISTpicture.h"
#include "vector"
#include <fstream>

class MNIST {
private:

	char* pictureFilename;
	char* labelsFilename;
	vector<MNISTpicture*> base;
	int magic_number = 0;
	int number_of_images = 0;
	int magic_number_l = 0;
	int number_of_images_l = 0;
	int n_rows = 0;
	int n_cols = 0;

public:
	MNIST(char* pictureFilename, char* labelsFilename) : pictureFilename(pictureFilename), labelsFilename(labelsFilename){ readMNIST(); }
	~MNIST(){}

	vector<MNISTpicture*> getBase(){
		return this->base;
	}

	int ReverseInt(int i)
	{
		unsigned char ch1, ch2, ch3, ch4;
		ch1 = i & 255;
		ch2 = (i >> 8) & 255;
		ch3 = (i >> 16) & 255;
		ch4 = (i >> 24) & 255;
		return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
	}
	void readMNIST(){

		std::ifstream picture(pictureFilename, std::ios::binary);

		if (picture.fail()){
			cout << "File open error: " << pictureFilename << std::endl;
			picture.close();
		}
		std::ifstream labels(labelsFilename, std::ios::binary);

		if (labels.fail()){
			cout << "File open error: " << labelsFilename << std::endl;
			labels.close();
		}

		picture.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		picture.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		picture.read((char*)&n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		picture.read((char*)&n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);

		labels.read((char*)&magic_number_l, sizeof(magic_number_l));
		magic_number_l = ReverseInt(magic_number_l);
		labels.read((char*)&number_of_images_l, sizeof(number_of_images_l));
		number_of_images_l = ReverseInt(number_of_images_l);

		if (number_of_images_l == number_of_images){
			for (int i = 0, proc = 0; i < number_of_images; ++i)
			{
				RGBTRIPLE* tempPalette = new RGBTRIPLE[n_rows*n_cols];
				for (int r = 0; r < n_rows; ++r)
				{
					for (int c = 0; c < n_cols; ++c)
					{
						unsigned char temp = 0;
						picture.read((char*)&temp, sizeof(temp));
						tempPalette[(n_rows*r) + c].rgbtRed = tempPalette[(n_rows*r) + c].rgbtBlue = tempPalette[(n_rows*r) + c].rgbtGreen = temp;
					}
				}

				MNISTpicture* buf = new MNISTpicture((char*)"mnist", n_rows, n_cols);
				buf->setPalette(tempPalette);
				base.push_back(buf);

				unsigned char temp = 0;
				labels.read((char*)&temp, sizeof(temp));
				base.at(i)->setLabel((int)temp);

				if (proc < 100 && ((int)((i * 100 / (number_of_images)) + 0.5)) == proc){
					cout <<"\t" << proc << "% Reading MNIST. Please wait!" << std::endl;
					proc += 5;
				}
			}
		}


		labels.close();
		picture.close();
	}

};

#endif // MNIST_H_INCLUDED

