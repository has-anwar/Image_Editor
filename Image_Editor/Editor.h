#pragma once
#include <iostream>

#include "Image.h"

#define byte        unsigned char

using namespace std;

class Editor
{
private:

public:

	byte*** BGR2GREY(byte***, int, int);
	byte*** GREY2BINARY(byte***, int, int);
	byte*** crop(byte***, int, int);
	byte*** filter(byte*** , int , int );
	byte*** scale(byte***, int R, int C);

};

