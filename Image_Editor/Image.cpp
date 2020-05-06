#include "Image.h"

void Image::set_pic(byte*** new_pic)
{
	this->pic = new_pic;
}

void Image::set_data_header(FILE* fileptr) {
	int bytes = fread(&data_header[0], 1, 54, fileptr);
	this->bytes = bytes;
}

int Image::get_bytes() {
	return bytes;
}

char* Image::get_header() {
	return data_header;
}

bool Image::check_header() {
	if (data_header[0] == 0x42 && data_header[1] == 0x4d) {
		cout << "This is a BMP File" << endl;
		return 1;
	}
	else return 0;
}

bool Image::check_bits() {
	if (data_header[28] == 0x18) {
		cout << "File is 24 bit" << endl;
		return 1;
	}
	else return 0;
}

void Image::set_RC() {
	R = *((int*)&data_header[22]);
	C = *((int*)&data_header[18]);
}

int Image::get_R() {
	return R;
}

int Image::get_C() {
	return C;
}

void Image::set_pixel_offset() { pixel_offset = *((int*)&data_header[10]); }

int Image::get_pixel_offset() { return pixel_offset; }

void Image::set_row_padding() {
	if ((C * 3) % 4 != 0) row_padding = 4 - ((C * 3) % 4);
}

int Image::get_row_padding() {
	return row_padding;
}

void Image::create_data_field() {
	data = new char[R * (3 * C + row_padding)];
}

void Image::create_pic_field() {
	pic = new byte * *[R];
	for (r = 0; r < R; r++) {
		pic[r] = new byte * [C];
		for (c = 0; c < C; c++) {
			pic[r][c] = new byte[3];
		}
	}
	cout << "Size of pic: " << sizeof(pic) << " bytes" << endl;
}

int Image::read_into_data(FILE* fileptr) {
	bytes = R * (3 * C + row_padding);

	int bread = 0;
	while (bread != bytes) {
		bread = bread + fread(data + bread, 1, 1, fileptr);
	}
	cout << "Bytes read " << bread << endl;

	if (bytes != R * (3 * C + row_padding)) {
		cout << "error reading pixel data complete!" << bytes << "   " << R * (3 * C + row_padding) << endl;
		return -1;
	}
	return 0;
}

void Image::read_into_pic() {
	i = 0;
	for (r = 0; r < R; r++) {
		for (c = 0; c < C; c++) {
			pic[r][c][0] = data[(r * ((3 * C) + row_padding)) + (3 * c)];
			pic[r][c][1] = data[(r * ((3 * C) + row_padding)) + (3 * c) + 1];
			pic[r][c][2] = data[(r * ((3 * C) + row_padding)) + (3 * c) + 2];
		}
	}
}

byte*** Image ::get_pic() {
	return pic;
}

int Image::reassemble(FILE* fileptr) {
	bytes = fwrite(data_header, 1, 54, fileptr);
	if (bytes == 0) {
		cout << "output.bmp could note be RWEAD" << endl;
		return -1;
	}

	i = 0;
	for (r = 0; r < R; r++) {
		for (c = 0; c < C; c++) {
			bytes = fwrite(pic[r][c], 1, 3, fileptr);
			if (bytes != 3) cout << "ERROR WRITING" << endl;
		}
		bytes = fwrite(padding, 1, row_padding, fileptr);
	}
	
	delete data;
	delete pic;

	return 0;
}