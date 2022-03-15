#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <enerc.h>

#include "datatype.h"
#include "config.h"
#include "prototype.h"

#include "rgbimage.h"


#define OUT_BUFFER_SIZE 262144 // in bytes

int main (int argc, const char* argv[]) {
	char inputFileName[256], outputFileName[256];
	UINT32 qualityFactor;
	UINT32 imageFormat;

	UINT8 *outputBuffer;
	FILE *fp;

	UINT8 *outputBufferPtr;

	if (argc > 1) {
		strcpy(inputFileName, argv[1]);
		strcpy(outputFileName, argv[2]);
	} else {
		strcpy(inputFileName, "./data/baboon-220px.rgb");
		strcpy(outputFileName, "./data/baboon-220px.rgb.jpg");
	}

	qualityFactor = 1024;
	imageFormat = GRAY;

	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image!\n");

		return -1;
	}

	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));


	outputBufferPtr = outputBuffer;
	//accept_roi_begin();
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);
	//accept_roi_end();


	freeRgbImage(&srcImage);

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
	free(outputBuffer);

	return 0;
}

