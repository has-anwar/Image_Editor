#pragma once
#include <iostream>
#include <string>
#define byte        unsigned char

using namespace std;

class Image
{	

private:

	char data_header[54];
	char* data;
	byte*** pic;
	char padding[3] = { 0,0,0 };
	int R;
	int C; 
	int row_padding; 
	int pixel_offset=-1;
	int bytes;
	int i, r, c;

public:

	void set_data_header(FILE* fileptr);
	void set_RC();
	void set_pixel_offset();
	void set_row_padding();
	void set_pic(byte***);

	bool check_header();
	bool check_bits();

	int get_R();
	int get_C();
	int get_pixel_offset();
	int get_row_padding();
	void create_data_field();
	void create_pic_field();
	int read_into_data(FILE*);
	void read_into_pic();
	byte*** get_pic();
	int reassemble(FILE*);
	int get_bytes();
	char* get_header();

};

