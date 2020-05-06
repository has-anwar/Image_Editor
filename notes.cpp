#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <fcntl.h>
#include <stdio.h>
using namespace std;
#define byte        unsigned char



int test()
{
    char data_header[54];
	char * data;
	byte *** pic;
	char padding[]={0,0,0};
	int R; //number of rows
	int C; //number of cols
	int row_padding; // number of dummy bytes at end of last pixel's data on each row, ignore and skip these bytes when you are processing the image.
	int pixel_offset;// index in data where the actual pixel value starts
	int r, c, i;

	FILE *fileptr=fopen("01.bmp","rb"); //The file needs to be named input exactly and is in BMP format
	if(!fileptr){
		cout<<"input.bmp could note be opened"<<endl;
		return -1;
	}
    int bytes=fread(data_header,1,54,fileptr);
	cout<<"bytes"<<bytes<<endl;

	if(bytes==0 || bytes<54){
		cout<<"bytes could not be input"<<bytes<<endl;
		return -1;
	}
	if(data_header[0]==0x42 && data_header[1]==0x4d){
		   cout<<"This is a BMP File"<<endl;
	}
	if(data_header[28]==0x18){
			cout<<"File is 24 bit"<<endl;
	}
	C = *((int*)&data_header[18]);
	R = *((int*)&data_header[22]);
	pixel_offset = *((int*)&data_header[10]);

	if(pixel_offset != 54)
		cout<<"Pixel NOT starting at 54"<<endl;

	row_padding = 0;
	if((C*3)%4!=0) row_padding = 4 - ((C*3)%4);
	printf("Rows:%08d ",R);
	printf("Cols:%08d\n",C);
	printf("Pixel data start from :%08d\n",pixel_offset);
	printf("Padding per row=%d bytes\n",row_padding);

	data = new char [R*(3*C+row_padding)];
	pic = new byte ** [R];
	for(r=0;r<R;r++){
        pic[r] = new byte*[C];
        for(c=0;c<C;c++){
            pic[r][c] = new byte[3];
        }
	}
	cout<<sizeof(pic);
	//pic = new char [R*(3*C)];

	bytes = R*(3*C+row_padding);
	int bread = 0;
	while(bread!=bytes){
        bread = bread + fread(data+bread, 1, 1, fileptr);
	}
    cout<<"Bytes read "<<bread<<endl;

	if(bytes!=R*(3*C+row_padding)){
		cout<<"error reading pixel data complete!"<<bytes<<"   "<<R*(3*C+row_padding)<<endl;
		return -1;
	}


	//read pixel data into pic[]
	i=0;
	for(r=0;r<R;r++){
		for(c=0;c<C;c++){
			pic[r][c][0] = data[(r*((3*C)+row_padding))+ (3*c)];
			pic[r][c][1] = data[(r*((3*C)+row_padding))+ (3*c) + 1];
			pic[r][c][2] = data[(r*((3*C)+row_padding))+ (3*c) + 2];
		}
	}

//A picture is made up of 3 colors in one pixel.
//A pixel is one dot on the screen.
//The order of the colors is blue then green and then red

//Your input picture must be named 'input.bmp' and must be in bmp format.
//Your output will be named 'output.bmp

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//YOUR LOGIC COMES HERE:

	//display or manipulate pixel data
	//cout<<"picture is:"<<endl;
	/*char color[3];
	color[0] = pic[0][0][0];
	color[1] = pic[0][0][1];
	color[2] = pic[0][0][2];

	for(r=0;r<R;r++){
        for(c=0;c<C;c++){
			if( (pic[r][c][0] == color[0]) &&
                (pic[r][c][1] == color[1]) &&
                (pic[r][c][2] == color[2])){
                    pic[r][c][0] = 0;
                    pic[r][c][1] = 0;
                    pic[r][c][2] = 0;
                }
            else {
                    pic[r][c][0] = 255;
                    pic[r][c][1] = 255;
                    pic[r][c][2] = 255;
            }
        }
	}*/

	for (r = 0; r < R; r++) {
		for (c = 0; c < C; c++) {
			if ((c < (5.0 /(float) 100) * C) || c> (95.0 / (float)100)* C || r<(20.0 / (float)100) * R || r>(80.0 / (float)100) * R) {
				pic[r][c][0] = 255;
				pic[r][c][1] = 255;
				pic[r][c][2] = 255;
			}
		}
	}

	for (r = 1; r < R-1; r++) {
		for (c = 1; c < C - 1; c++) {
			if(pic[r][c][0] == 0)
			{
				pic[r][c + 1];
				pic[r][c - 1];
				pic[r + 1][c]; 
				pic[r - 1][c + 1];
				pic[r + 1][c + 1];
				pic[r - 1][c - 1];
				pic[r - 1][c + 1];
				pic[r + 1][c - 1];
			}
		}
	}


//Make all the colors in all the pixels the opposite color.
//You can use: pic[r*C*3+c*3+h], with r being the row, c being the column and h being 0, 1, 2 with 0 being blue, 1 being green and 2 being red.


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// YOUR LOGIC ABOVE ONLY!

	fclose(fileptr);
	//write pixel data fro m pic[] to file output
	fileptr=fopen("output.bmp","wb");
	if(!fileptr){
		cout<<"output.bmp could note be opened"<<endl;
		return -1;
	}

	bytes=fwrite(data_header,1,54,fileptr);
	if(bytes==0){
		cout<<"output.bmp could note be RWEAD"<<endl;
		return -1;
	}

	i=0;
	for(r=0;r<R;r++){
		for(c=0;c<C;c++){
            bytes = fwrite(pic[r][c],1,3,fileptr);
            if(bytes!=3) cout<<"ERROR WRITING"<<endl;
		}
		bytes = fwrite(padding,1, row_padding,fileptr);
	}
	fclose(fileptr);
	delete data;
	delete pic; //todo delete the 2 inner dimension iteratively.
    return 0;
}