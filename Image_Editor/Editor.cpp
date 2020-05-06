#include "Editor.h"

byte*** Editor::BGR2GREY(byte*** pic,int R, int C)
{
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			unsigned int grey_scale = (int)((0.11 * pic[r][c][0]) + (0.11 * pic[r][c][1]) + (0.11 * pic[r][c][2]));
			pic[r][c][0] = grey_scale;
			pic[r][c][1] = grey_scale;
			pic[r][c][2] = grey_scale;
		}
	}
	return pic;
}

byte*** Editor::GREY2BINARY(byte*** pic, int R, int C)
{
	unsigned int thr = 100;

	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			unsigned int val = pic[r][c][0] + pic[r][c][1] + pic[r][c][2];
			if (val <= thr) {
				pic[r][c][0] = 255;
				pic[r][c][1] = 255;
				pic[r][c][2] = 255;
			}
			else {
				pic[r][c][0] = 0;
				pic[r][c][1] = 0;
				pic[r][c][2] = 0;
			}
		}
	}
	return pic;
}

byte*** Editor::crop(byte*** pic, int R, int C)
{
	for (int r = 0; r < R; r++) {
		for (int c = 0; c < C; c++) {
			if ((c < (5.0 / (float)100) * C) || c > (95.0 / (float)100) * C || r<(20.0 / (float)100) * R || r>(80.0 / (float)100) * R) {
				pic[r][c][0] = 255;
				pic[r][c][1] = 255;
				pic[r][c][2] = 255;
			}
		}
	}
	return pic;
}

byte*** Editor::filter(byte*** pic, int R, int C)
{
	unsigned int val = 0;
	int thr = 2100;	//1150, 1479, 1900
	int size = 9;	//7, 9, 9

	for (int r = 0; r < R - size; r++) {
		for (int c = 0; c < C - size; c++) {
			
			for (int s_r = 0; s_r <= size; s_r++) {
				for (int s_c = 0; s_c <= size; s_c++) {
					val += pic[r+s_r][c+s_c][0];
				}
			
				if (val <= thr) {
					pic[r][c][0] = 0;
					pic[r][c][1] = 0;
					pic[r][c][2] = 0;
				}
			
			}
			val = 0;
		}
	}
	return pic;
}

byte*** Editor::scale(byte*** pic, int R, int C)
{
	byte*** new_pic = 0;
	float resize_value = 100.0f;
	float resize_factor = resize_value / R;
	cout << "Resize factor is: " << resize_factor << endl;
	
	new_pic = new byte * *[R];
	for (int r = 0; r < R; r++) {
		new_pic[r] = new byte * [C];
		for (int c = 0; c < C; c++) {
			new_pic[r][c] = new byte[3];
		}
	}
	cout << "Size of pic is: " << sizeof(new_pic) << " bytes" << endl;

	for (int i = 0; i <= R-1; i++) {
		for (int j = 0; j <= C-1; j++) {
			new_pic[i][j][0] = 0;
			new_pic[i][j][1] = 0;
			new_pic[i][j][2] = 0;
		}
	}
	
	for (int i = 0; i <= R - 1; i++) {
		for (int j = 0; j <= C - 1; j++) {
			for (int rgb = 0; rgb < 3; rgb++) {
				int i1 = i * resize_factor;
				int j1 = j * resize_factor;
				new_pic[i1][j1][rgb] = pic[i][j][rgb];
			}
		}
	}
	
	return new_pic;
}
