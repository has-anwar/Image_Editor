#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

#include "Image.h"
#include "Editor.h"
#include "DigitRecognizer.h"
#include "Frequencies.h"

using namespace std;
using namespace cv;

#define byte        unsigned char

Image dismantel(FILE* fileptr, Image img);
void display_output(string output);

int main()
{

	char file_name[]= "sprint-zero.bmp";
	FILE* fileptr = fopen(file_name, "rb");
	if (!fileptr){
		cout << file_name << " could not be opened" << endl;
		return -1;
	}

	Image img;
	Editor edit;
	img = dismantel(fileptr, img);
	byte*** pic = img.get_pic();
	

	int R = img.get_R();
	int C = img.get_C();

	//pic = edit.crop(pic, R, C);
	
	
	//pic = edit.BGR2GREY(pic, R, C);
	 //edit.GREY2BINARY(pic, R, C);             
	//pic = edit.filter(pic, R, C);
	

	byte*** res_pic;
	//res_pic = edit.scale(pic, R, C);

	//img.set_pic(res_pic);
	DigitRecognizer rec(img);

	rec.radon_transform(img);

	/*
	DigitRecognizer digir_r(img);
	
	int digit = digir_r.digit();
	cout << "Digit is " << digit << endl;
	*/
	/*
	 Frequencies freq = digir_r.frequency();
	 
	 cout << "Horizontal Frequency: ";
	for (int i = 0; i <= freq.hor.size()-1 ; i++) {
		cout << freq.hor[i] << " ";
	}
	cout << endl;
	
	cout << "Vertical Frequency: ";
	for (int i = 0; i <= freq.ver.size() - 1; i++) {
		cout << freq.ver[i] << " ";
	}
	cout << endl;

	cout << "Diagonal Frequency: ";
	for (int i = 0; i <= freq.diag.size() - 1; i++) {
		cout << freq.diag[i] << " ";
	}
	cout << endl;

	cout << "Anti-Diagonal Frequency: ";
	for (int i = 0; i <= freq.anti_diag.size() - 1; i++) {
		cout << freq.anti_diag[i] << " ";
	}
	cout << endl;
	*/
	fclose(fileptr);

	string output = "output.bmp";
	char* cstr = new char [output.size() + 1];
	strcpy(cstr, output.c_str());
	fileptr = fopen(cstr, "wb");

	if (!fileptr) {
		cout << "output. bmp could not be opened" << endl;
		return -1;
	}

	img.reassemble(fileptr);
	fclose(fileptr);

	display_output(output);

	return 0;
}

Image dismantel(FILE* fileptr, Image img) {
	img.set_data_header(fileptr);
	int bytes = img.get_bytes();
	cout << "Bytes read are: " << bytes << endl;

	img.check_header();
	img.check_bits();
	img.set_RC();
	img.set_pixel_offset();
	if (img.get_pixel_offset() != 54)
		cout << "Pixel NOT starting at 54" << endl;
	img.set_row_padding();

	printf("Rows:%08d ", img.get_R());
	printf("Cols:%08d\n", img.get_C());
	printf("Pixel data start from :%08d\n", img.get_pixel_offset());
	printf("Padding per row= %d bytes\n", img.get_row_padding());

	img.create_data_field();
	img.create_pic_field();
	img.read_into_data(fileptr);
	img.read_into_pic();

	return img;
}

void display_output(string output) {
	Mat image;
	image = imread(output);
	namedWindow("Window", WINDOW_AUTOSIZE);
	imshow("Window", image);
	waitKey(0);
}