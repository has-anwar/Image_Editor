#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map> 

#include "Image.h"
#include "Frequencies.h"

#define byte        unsigned char

using namespace std;

struct  FR {
	int f = 0;
	int r = 0;
};

struct Coordinates {
	int x2;
	int y2;
};

class DigitRecognizer
{
private: 
	Image img;

public:

	DigitRecognizer(Image img) {
		this->img = img;
	}

	FR frequency(int r);
	Frequencies frequency();
	int digit();
	Image get_img();
	map<int, vector<int>> radon_transform(Image img);

	Coordinates get_coordinates(int start_x, int start_y, int R, int C);

	bool isValid(int x, int y);


};

