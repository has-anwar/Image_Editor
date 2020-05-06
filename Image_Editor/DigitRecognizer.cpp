#include "DigitRecognizer.h"



FR DigitRecognizer::frequency(int row)
{	
	FR fr;
	fr.f = 0;
	fr.r = row;
	Image img = this->get_img();
	byte*** pic = img.get_pic();
	int R = img.get_R();
	int C = img.get_C();


	for(int c = 0; c < C - 1; c++) {
		if (pic[fr.r][c][0] != pic[fr.r][c+1][0] && pic[fr.r][c][0] == 255) {
			fr.f += 1;
		}
	}

	return fr;

}

Frequencies DigitRecognizer::frequency() 
{
	int freq = 0;
	Image img = this->get_img();
	byte*** pic = img.get_pic();
	int R = img.get_R();
	int C = img.get_C();
	

	vector <int> hor;
	vector <int> ver;
	vector <int> diag;
	vector <int> anti_diag;

	for (int r = 0; r < R - 1; r++) {
		for (int c = 0; c < C - 1; c++) {
			if (pic[r][c][0] != pic[r][c + 1][0] && pic[r][c][0] == 255) {
				freq += 1;
			}
		}
		if (freq != 0) {
			hor.push_back(freq);
			freq = 0;
		}
	}
	
	freq = 0;

	for (int c = 0; c < C- 1; c++) {
		for (int r = 0; r < R - 1; r++) {
			if (pic[r][c][0] != pic[r + 1][c][0] && pic[r][c][0] == 255) {
				freq += 1;
			}
		}
		if (freq != 0) {
			ver.push_back(freq);
			freq = 0;
		}
	}

	freq = 0;
	/*
	for (int r = 0; r < img.get_R() - 1; r++) {
		for (int c = 0; c < img.get_C() - 1; c++) {
			if (pic[r][c][0] != pic[r + 1][c + 1][0] && pic[r][c][0] == 255) {
				freq += 1;
			}
		}
		if (freq != 0) {
			diag.push_back(freq);
			freq = 0;
		}
	}
	*/

	int i, j, k;

	for (k = 0; k <= R - 2; k++) {
		i = k;
		j = 0;
		while(i >= 0) {
			if (i > 0 && j > C - 2)
			{
				if (pic[i][j][0] != pic[i - 1][j + 1][0] && pic[i][j][0] == 255) {
					freq += 1;
				}
			}

			i = i - 1;
			j = j + 1;
		}
		if (freq != 0) {
			diag.push_back(freq);
			freq = 0;
		}
		for (k = 1; k <= R - 2; k++) {
			i = R - 1;
			j = k;
			while (j <= C - 1)
			{
				if (i > 3 && j > C - 3) {
					if (pic[i][j][0] != pic[i - 1][j + 1][0] && pic[i][j][0] == 255) {
						freq += 1;
					}
				}
				i = i - 1;
				j = j + 1;
			}
			if (freq != 0) {
				diag.push_back(freq);
				freq = 0;
			}
		}
	}
	cout << k << " " << j;
	freq = 0;
	/*
	for (int r = img.get_R() - 1; r >= 1; r--) {
		for (int c = 0; c < img.get_C() - 1; c++) {
			if (pic[r][c][0] != pic[r - 1][c + 1][0] && pic[r][c][0] == 255) {
				freq += 1;
			}
		}
		if (freq != 0) {
			anti_diag.push_back(freq);
			freq = 0;
		}
	}
	*/
	Frequencies frequencies;
	
	for (int i = 0; i <hor.size() - 1; i++) {
		if (hor[i] != hor[i + 1]) {
			frequencies.hor.push_back(hor[i]);
		}
	}
	frequencies.hor.push_back(hor[hor.size() - 1]);

	for (int i = 0; i < ver.size() - 1; i++) {
		if (ver[i] != ver[i + 1]) {
			frequencies.ver.push_back(ver[i]);
		}
	}
	frequencies.ver.push_back(ver[ver.size() - 1]);

	for (int i = 0; i < diag.size() - 1; i++) {
		if (diag[i] != diag[i + 1]) {
			frequencies.diag.push_back(diag[i]);
		}
	}
	frequencies.diag.push_back(diag[diag.size() - 1]);

	for (int i = 0; i < anti_diag.size() - 1; i++) {
		if (anti_diag[i] != anti_diag[i + 1]) {
			frequencies.anti_diag.push_back(anti_diag[i]);
		}
	}
	frequencies.anti_diag.push_back(anti_diag[anti_diag.size() - 1]);

	return frequencies;

}

int DigitRecognizer::digit()
{

	int row = 0;
	vector <int> values;
	int val;

	FR fr = this->frequency(row);



	for (int r = 0; r < img.get_R()-1; r++) {
		fr = this->frequency(r);
		val = fr.f;
		fr = this->frequency(r+1);
		if (fr.f != val) {
			values.push_back(val);
		}

	}

	for (auto i = values.begin(); i != values.end(); ++i)
		cout << *i << " ";

	cout << endl;

	if (values[0] == 0 && values[1] == 1 && values[2] == 2 && values[3] == 1) {
		return 0;
	}
	else return 1;



}

Image DigitRecognizer::get_img()
{
	return this->img;
}

void DigitRecognizer::radon_transform(Image img)
{
	int R = img.get_R();
	int C = img.get_C();
	byte*** pic = img.get_pic();

	int F = 0;
	vector <int> freq;
	unordered_map<int, vector<int>> freq_map;

	float x0 = 0.0f;
	float y0 = 0.0f;

	float x1 = 0.0f;
	float y1 = 0.0f;

	int angle = 10;
	int temp = angle;
	float rad = angle * 3.14 / 180;
	int len = 1;

	
	while (angle <= 180) {


		F = 0;
		rad = angle * 3.14 / 180;
		freq.clear();

		if ((angle > 0 && angle < 180) && (angle != 90 && angle != 180)) {

			

			for (int row = 0; row < R; row++) {
				int col = 0;
				x0 = row;
				y0 = col;

				while (x0 < R - 1 && y0 < C - 1) {


					x1 = x0 + abs(cos(rad)) * len;
					y1 = y0 + abs(sin(rad)) * len;

					if (pic[int(x0)][int(y0)][0] != pic[int(x1)][int(y1)][0] && pic[int(x0)][int(y0)][0] == 255) {
						F++;
					}

					x0 = x1;
					y0 = y1;
					F = 0;
				}
				//F = 0;
				if (F != 0) {
					freq.push_back(F);
					F = 0;
				}
			}
			
			for (int col = 1; col < C; col++) {
				int row = 0;
				x0 = row;
				y0 = col;

				while (x0 < R - 1 && y0 < C - 1) {


					x1 = x0 + abs(cos(rad)) * len;
					y1 = y0 + abs(sin(rad)) * len;

					if (pic[int(x0)][int(y0)][0] != pic[int(x1)][int(y1)][0] && pic[int(x0)][int(y0)][0] == 255) {
						F++;
					}

					x0 = x1;
					y0 = y1;

				}
				if (F != 0) {
					freq.push_back(F);
					F = 0;
				}
			}
			freq_map[angle] = freq;
		}

		else if (angle == 90) {
			for (int r = 0; r < R - 1; r++) {
				for (int c = 0; c < C - 1; c++) {
					if (pic[r][c][0] != pic[r][c + 1][0] && pic[r][c][0] == 255) {
						F++;
					}
				}
				if (F != 0) {
					freq.push_back(F);
					F = 0;
				}
			}
			freq_map[angle] = freq;
		}

		else if (angle == 180) {
			for (int c = 0; c < C - 1; c++) {
				for (int r = 0; r < R - 1; r++) {
					if (pic[r][c][0] != pic[r + 1][c][0] && pic[r][c][0] == 255) {
						F++;
					}
				}
				if (F != 0) {
					freq.push_back(F);
					F = 0;
				}
			}
			freq_map[angle] = freq;
		}

		else cout << "Angle value " << angle << " not allowed";

		angle += temp;
	}

	vector <int> vec;
	for (auto x : freq_map) {
		cout <<"At angle: "<< x.first << endl;
		vec = x.second;

		for (auto i = vec.begin(); i != vec.end(); ++i)
			cout << *i << " ";
		cout << endl;
	}
}

Coordinates DigitRecognizer::get_coordinates(int start_x, int start_y, int R, int C)
{

	int x2, y2;
	Coordinates p1;
	int angle = 108;
	long rads = angle * 3.14 / 180;

	x2 = (int)round(start_x + abs(cos(rads)) * (1));
	y2 = (int)round(start_y + abs(sin(rads)) * (1));
	p1.x2 = x2;
	p1.y2 = y2;

	return p1;
}

bool DigitRecognizer::isValid(int x, int y)
{
	if (true)
		return true;
	
	else return false;
}