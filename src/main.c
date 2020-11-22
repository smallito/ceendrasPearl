#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <complex.h>

#include "include/complexMath.h"
#include "include/plot.h"
#include "include/arraysOps.h"
#include "include/treeExploration.h"
#include "include/debugTools.h"
#include "include/easing.h"

#define SIZEARR 1000
#define ANTIALPOW 4
#define WIDTH  1920 * ANTIALPOW 
#define HEIGHT 1080 * ANTIALPOW
#define BOUNDS 1 
#define RANDBOUNDS 0 + 1 * I 
#define EPSI  0.005 
#define LEVMAX 15 
#define LINE 0 
#define BITWISE 1
#define DEBUG 0



int main(){
	time_t pt;
	srand((unsigned) time(&pt));

	double complex ta = 0.;
	double complex tb = 0.;
	double complex tab = 0.;

	double complex taBeg = 0.;
	double complex tbBeg = 0.;
	double complex tabBeg = 0.;

	double complex taEnd =  0.;
	double complex tbEnd = 0.;
	double complex tabEnd =  0.;

	double complex taInit = 0.;
	double complex tbInit = 0.;
	double complex tabInit = 0.;

	int numIm = 0;

	taBeg  = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);
	tbBeg  = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);
	tabBeg = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);

	taEnd  =  randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);
	tbEnd  = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);
	tabEnd = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);

	tbBeg  = 2;
	tbEnd  = 2;

	taInit = taBeg;
	tbInit = tbBeg;
	tabInit = tabBeg;

	printf("taBeg: %lf + %lf\n", creal(taBeg), cimag(taBeg));
	printf("taEnd: %lf + %lf\n", creal(taEnd), cimag(taEnd));
	printf("tbBeg: %lf + %lf\n", creal(tbBeg), cimag(tbBeg));
	printf("tbEnd: %lf + %lf\n", creal(tbEnd), cimag(tbEnd));
	printf("tabBeg: %lf + %lf\n", creal(tabBeg), cimag(tabBeg));
	printf("tabEnd: %lf + %lf\n\n", creal(tabEnd), cimag(tabEnd));

	int fps = 30;
	int duration = 3;
	int lengthAnim = 6;

	image_t img;
	image_t* pImg = &img;

	pImg->w      = WIDTH;
	pImg->h      = HEIGHT;
	pImg->bounds = BOUNDS;
	pImg->epsi   = EPSI;
	pImg->line   = LINE;
	pImg->levmax = LEVMAX;
	pImg->antialiasingPow = ANTIALPOW;
	pImg->debug  = DEBUG;
	pImg->bitwise= BITWISE;
	pImg->filename = malloc(256* sizeof(char));

	pImg->pointArr = NULL;
	pImg->bitArray = NULL;
	pImg->pointArr = (int*)calloc(pImg->w*pImg->h, sizeof(int));
	unsigned int allocSize = (ceil(pImg->w/64.0))* pImg->h;
	pImg->bitArray = (unsigned long long int*)calloc(allocSize, sizeof(unsigned long long int));

	if (pImg->pointArr == NULL){
		printf("Could not allocate memory for the image array !\nExiting...\n");
		exit(-1);
	}
	
	printf("levmax %d\n", pImg->levmax);
	
	char prefix[100] = "out/img_";
	char imageNum[5];  


	while(1){
		srand((unsigned) time(&pt));
		//Create a filename for the image based on the number of image processed
		sprintf(imageNum, "%d", numIm);
		strcat(prefix, imageNum);
		strcat(prefix, ".bmp");
		strcpy(pImg->filename, prefix);
		strcpy(prefix, "out/img_");
		printf("Image: %s\n\n", pImg->filename);

		//Here, we interpolate between two values of ta,tb using an easing function
		//The easing function takes a starting value and the value that needs to be added
		//To get the value that needs to be added we extract the distance between the two traces using copysign
		//And multiply by minus one to add. We need to do that for the real and complex part so we get this loooong line :)
		ta = InOutQuadComplex((float)(numIm%(fps*duration)), taBeg, -copysign(creal(taBeg- taEnd), creal(taBeg- taEnd)) + I*-copysign(cimag(taBeg- taEnd), cimag(taBeg- taEnd)), (float)fps * duration); tb = InOutQuadComplex((float)(numIm%(fps*duration)), tbBeg, -copysign(creal(tbBeg- tbEnd), creal(tbBeg- tbEnd)) + I*-copysign(cimag(tbBeg- tbEnd), cimag(tbBeg- tbEnd)), (float)fps * duration);
		tb = InOutQuadComplex((float)(numIm%(fps*duration)), tbBeg, -copysign(creal(tbBeg- tbEnd), creal(tbBeg- tbEnd)) + I*-copysign(cimag(tbBeg- tbEnd), cimag(tbBeg- tbEnd)), (float)fps * duration);
		tab = InOutQuadComplex((float)(numIm%(fps*duration)), tabBeg, -copysign(creal(tabBeg- tabEnd), creal(tabBeg- tabEnd)) + I*-copysign(cimag(tabBeg- tabEnd), cimag(tabBeg- tabEnd)), (float)fps * duration);

		computeDepthFirst(ta, tb, tab, pImg, numIm);
		saveArrayAsBMP(pImg);
		numIm++;
		printf("ta:  %lf + I %lf\n", creal(ta), cimag(ta));
		printf("tb:  %lf + I %lf\n", creal(tb), cimag(tb));
		printf("tab: %lf + I %lf\n", creal(tab), cimag(tab));
		if (numIm % (fps * duration) == 0 ){
			taBeg = taEnd;
			tbBeg = tbEnd;
			tabBeg = tabEnd;


			taEnd  = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);
			tbEnd  = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);
			tabEnd = randomComplex(-3 - 1.5 * I, 3 + 1.5 * I);


			if (numIm >= fps * lengthAnim - fps * duration ){//loop by ending up at the begining traces
				taEnd = taInit;
				tbEnd = tbInit;
				tabEnd = tabInit;
			}
		}
		if (numIm >= fps * lengthAnim) return(1);
	}
	return 0;
}
