#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <complex.h>

#include "include/plot.h"
#include "include/complexMath.h"
#include "include/arraysOps.h"
#include "include/treeExploration.h"
#include "include/debugTools.h"

#define SIZEARR 10000
#define HEIGHT 1000 
#define WIDTH  1000 
#define BOUNDS 2 
#define EPSI  0.001 
#define LINE 0 

int LEVMAX = 8;  

#define DEBUG 0

int main(){
	double complex ta = 2.2;
	double complex tb = 2.2;
	int numIm = 0;
	float theta = 0;

	srand(time(NULL));

	double PARAMS[10];
	PARAMS[0] = 10; //levmax
	PARAMS[1] = 0.01; //epsilon
	PARAMS[2] = 1; //bounds
	PARAMS[3] = 1000; //x resolution
	PARAMS[4] = 1000; //y resolution
	PARAMS[5] = 1; //Draw line (1 = yes)
	PARAMS[6] = 0; //Debug mode (1 = yes)

	float *** imgArr = NULL;
	imgArr = (float***)malloc(WIDTH*sizeof(float**));
	for (int i = 0; i< WIDTH; i++) {
		imgArr[i] = (float **) malloc(HEIGHT*sizeof(float *));
		for (int j = 0; j < HEIGHT; j++) 
			imgArr[i][j] = (float *) malloc(3 *sizeof(float));
	}

	if (imgArr == NULL){
		printf("Could not allocate memory for the image array !\nExiting...\n");
		exit(-1);
	}
	while(1){

		for (int i = 0; i< WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				imgArr[i][j][0] = 0;
				imgArr[i][j][1] = 0;
				imgArr[i][j][2] = 0;
			}
		}

		//ta = 2 + 2 * I * -sin(theta);
		//tb = 2*sin(theta) + I * cos(theta);
		//ta = 1.91 + cos(theta)*I;
		//tb = 1.91 + sin(theta)*I;
		ta  = (float)rand()/(float)(RAND_MAX/2) +  -I +(float)rand()/(float)(RAND_MAX/2)*I;
		tb  = (float)rand()/(float)(RAND_MAX/2) +  -I +(float)rand()/(float)(RAND_MAX/2)*I;
		//ta = 2 + cos(theta) + 2*I + I*sin(theta);
		//tb = 2 + cos(theta) + 2*I + I*sin(theta);
		ta = 2.2;
		tb = 2.2;
		computeDepthFirst(PARAMS, ta, tb, imgArr, numIm);
		numIm++;
		//	LEVMAX++;
		theta += 0.05;
		if (theta > 3.1415928) exit(1);
	}
	return 0;
}
