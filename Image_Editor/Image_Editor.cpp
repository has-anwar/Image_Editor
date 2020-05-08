#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <unordered_map> 


#include "Image.h"
#include "Editor.h"
#include "DigitRecognizer.h"
#include "Frequencies.h"


using namespace std;
using namespace cv;

#define byte        unsigned char

Image dismantel(FILE* fileptr, Image img);
void display_output(string output);
int driver(char file_name[]);
void save_frequencies(char file_name[], unordered_map<int, vector<int>>);

int main()
{
	vector<string> vec{ "sprint-zero.bmp" };

	for (int i = 0; i < vec.size();i++) {
		char* file_name = const_cast<char*>(vec[i].c_str());
		driver(file_name);
	}



	return 0;
}

int driver(char file_name[])
{
	FILE* fileptr = fopen(file_name, "rb");
	if (!fileptr) {
		cout << file_name << " could not be opened" << endl;
		return -1;
	}

	Image img;
	Editor edit;
	img = dismantel(fileptr, img);
	byte*** pic = img.get_pic();


	int R = img.get_R();
	int C = img.get_C();

	pic = edit.crop(pic, R, C);
	edit.BGR2GREY(pic, R, C);
	edit.GREY2BINARY(pic, R, C);
	edit.filter(pic, R, C);


	byte*** res_pic;
	res_pic = edit.scale(pic, R, C);

	img.set_pic(res_pic);
	DigitRecognizer rec(img);

	unordered_map<int, vector<int>> freq_map = rec.radon_transform(img);

	fclose(fileptr);

	string output = "output.bmp";
	char* cstr = new char[output.size() + 1];
	strcpy(cstr, output.c_str());
	fileptr = fopen(cstr, "wb");

	if (!fileptr) {
		cout << "output. bmp could not be opened" << endl;
		return -1;
	}

	img.reassemble(fileptr);
	fclose(fileptr);


	display_output(output);
	save_frequencies(file_name, freq_map);


	return 0;
}



void save_frequencies(char* file_name, unordered_map<int, vector<int>> freq_map)
{
	vector <int> vec;
	ofstream outfile;

	outfile.open("radon_vectors.txt");
	outfile << file_name << ":" << endl;


	for (auto x : freq_map) {
		outfile << "At angle: " << x.first << endl;
		vec = x.second;

		for (auto i = vec.begin(); i != vec.end(); ++i) {
			outfile << *i << " ";
		}
		outfile << endl;
		
	}

	outfile.close();
	

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

