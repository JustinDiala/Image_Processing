#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

#include "CONSTS.h"
#include "WEIGHTED_BIPMATCH_V4.h"

typedef unsigned char uchar;
int ICON1[MAXD*MAXD][3];
int ICON2[MAXD*MAXD][3];
int OUT[MAXD*MAXD][3];

int width, height;

/*

	Description: This main function will perform the reading of the file from the textfile.
	This will then assign values to the variables located in the CONSTS.h header file
	and then it will perform the call to the Hungarian algorithm

	Arguments:
		- None

	Returns:
		- 0: returned for convention

*/
int main () {

	cin.tie(0);
	ios_base::sync_with_stdio(0);

	// the two for loops located from lines 35 to 41 are reading in the pixel rgb values from image1 and image2 respectively
	// then assigning the pixel rgb values into the ICON1 matrix and ICON2 matrix for image1 and image2 respectively.
	cin >> width >> height;
	for (int i = 0; i < width*height; ++i)
		cin >> ICON1[i][0] >> ICON1[i][1] >> ICON1[i][2];

	cin >> width >> height;
	for (int i = 0; i < width*height; ++i)
		cin >> ICON2[i][0] >> ICON2[i][1] >> ICON2[i][2];

	int block_size = min(280.0, sqrt(width*height));
	n = m = block_size;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, width*height-1);

	// randomly shuffle the pixels to ensure the colors are evenly distributed
	for (int i = 0; i < width*height; ++i) {
		int j = dis(gen);
		swap(ICON1[i][0], ICON1[j][0]);
		swap(ICON1[i][1], ICON1[j][1]);
		swap(ICON1[i][2], ICON1[j][2]);
	}

	// find the edge costs and negate them, since we want a min-cost perfect matching and the Hungarian algorithm finds the max-cost perfect matching
	for (int k = 0; k < width*height; k += block_size) {
		for (int i = 0; i < min(width*height, k + block_size) - k; ++i)
			for (int j = 0; j < min(width*height, k + block_size) - k; ++j)
				c[i][j] = -1 * sqrsum(ICON1[i+k][0] - ICON2[j+k][0], ICON1[i+k][1] - ICON2[j+k][1], ICON1[i+k][2] - ICON2[j+k][2]);

		hungarian(); // once everything has been initialized. This call to Hungarian algorithm will find the minimum cost matching

		// after the Hungarian algorithm finishes we store the values to a matrix called OUT.
		// This matrix contains the final image's rgb pixel values.
		for (int j = 0; j < block_size; ++j) {
			OUT[j+k][0] = ICON1[mate_r[j] + k][0];
			OUT[j+k][1] = ICON1[mate_r[j] + k][1];
			OUT[j+k][2] = ICON1[mate_r[j] + k][2];
		}
	}

	// we write the matrix out to a textfile which will be converted to an image file
	cout << width << " " << height << endl;
	for (int i = 0; i < width*height; ++i)
		cout << OUT[i][0] << " " << OUT[i][1] << " " << OUT[i][2] << endl;

	return 0;
}