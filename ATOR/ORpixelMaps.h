/*******************************************************************************
 *
 * File Name: ORpixelMaps.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a6c 07-May-2012
 *
 *******************************************************************************/

 #ifndef HEADER_OR_PIXELMAPS
#define HEADER_OR_PIXELMAPS

#include "SHAREDvars.h"

#include "RTpixelMaps.h"



#define OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF (0)
#define OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE (1)
#define OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE_COMPLETELY (2)


#define DEPTH_GRADIENT_MAP_VEC_NUM_DIMENSIONS (2)





#define DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES (3)
#define DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES (3)
//#define KERNEL_WIDTH_NUMBER_SAMPLES (3)
//#define KERNEL_HEIGHT_NUMBER_SAMPLES (3)



#define MATRIX_NUM_VECTORS (3)
#define MATRIX_VEC_A (0)
#define MATRIX_VEC_B (1)
#define MATRIX_VEC_C (2)

#define VECTOR_NUM_VALUES (3)
#define VECTOR_VAL_X (0)
#define VECTOR_VAL_Y (1)
#define VECTOR_VAL_Z (2)

#define XY_VECTOR_MAP_NUM_DIMENSIONS (2)
#define XY_VECTOR_MAP_VEC_X (0)
#define XY_VECTOR_MAP_VEC_Y (1)



void createPointNormalMapFromPointMap(int imageWidth, int imageHeight, double *  pointMap, double * pointNormalMap);
	void calculateMeshPointNormalsUsingPointMap(int x, int y, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, double *  pointMap, vec * meshPointNormal, vec meshPointNormals[]);
void createPointNormalContrastMapFromPointNormalMap(int imageWidth, int imageHeight, double *  pointNormalMap, double * pointNormalContrastMap);
	double calculatePointNormalContrastLevelWithinKernel(int pixelX, int pixelY, double * pointNormalMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);


void setXYVectorMapValue(int x, int y, int imageWidth, vec * XYVectorVal, double * XYVectorMap);
void getXYVectorMapValue(int x, int y, int imageWidth, double * XYVectorMap, vec * XYVectorVal);

void createDepthContrastBooleanMap(int imageWidth, int imageHeight, double * depthContrastMap, bool * depthContrastBooleanMap);

void generatePixmapFromDepthGradientMap(char * imageFileName, int imageWidth, int imageHeight, double * depthGradientMap);
void generateRGBMapFromDepthGradientMap(int imageWidth, int imageHeight, double * depthGradientMap, unsigned char * rgbMap);
void generatePixmapFromDepthGradientContrastMap(char * imageFileName, int imageWidth, int imageHeight, double * depthGradientContrastMap);
void generateRGBMapFromDepthGradientContrastMap(int imageWidth, int imageHeight, double * depthGradientContrastMap, unsigned char * rgbMap);
void createDepthGradientContrastBooleanMap(int imageWidth, int imageHeight, double * depthGradientContrastMap, bool * depthGradientContrastBooleanMap);

void generatePixmapFromPointNormalContrastMap(char * imageFileName, int imageWidth, int imageHeight, double * pointNormalContrastMap);
void generateRGBMapFromPointNormalContrastMap(int imageWidth, int imageHeight, double * pointNormalContrastMap, unsigned char * rgbMap);
void createPointNormalContrastBooleanMap(int imageWidth, int imageHeight, double * pointNormalContrastMap, bool * pointNormalContrastBooleanMap);




//methods taken from ORoperations.cpp;


double calculateForegroundMinimumDepthWithinKernel(int pixelX, int pixelY, int imageWidth, int imageHeight, int kernelWidth, int kernelHeight, double * depthMap, vec * nearbyPointOfMinimumDepth, int zoom);

void createContrastMapFromMapWithForegroundDepthCheckOLD(int imageWidth, int imageHeight, double * luminosityOrDepthMap, double * depthMap, double * contrastMap, bool * contrastBooleanMap, bool * foregroundDepthCheckContrastBooleanMap, double mapThreshold);
	double calculateContrastLevelWithinKernelWithForegroundDepthCheckOLD(int pixelX, int pixelY, double * luminosityOrDepthMap, double * depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool * foregroundDepthCheckContrastBooleanMap, double mapThreshold);
/*
void createContrastBooleanMapFromContrastMapWithForegroundDepthCheck(int imageWidth, int imageHeight, double * contrastMap, double * depthMap, bool * contrastBooleanMap, bool * foregroundDepthCheckContrastBooleanMap, double mapThreshold);
	double getContrastLevelWithinKernelWithForegroundDepthCheck(int pixelX, int pixelY, double * contrastMap, double * depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool * foregroundDepthCheckContrastBooleanMap, double mapThreshold);
*/



void createDepthGradientMapFromDepthMap(int imageWidth, int imageHeight, double * depthMap, double * depthGradientMap);
	double calculateDepthGradientValueWithinKernel(int pixelX, int pixelY, double * depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, vec * depthGradientVal);
void createDepthGradientContrastMapFromDepthGradientMap(int imageWidth, int imageHeight, double * depthGradientMap, double * depthGradientContrastMap);
	void createDepthGradientContrastMapFromMap(int imageWidth, int imageHeight, double * depthGradientMap, double * depthGradientContrastMap);
		double calculateDepthGradientContrastValueWithinKernel(int pixelX, int pixelY, double * depthGradientMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);
		double calculateDepthGradientContrastValueWithinKernelWRONG(int pixelX, int pixelY, double * depthGradientMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);
			bool checkIfMeetDepthGradientContrastThreshold(double depthGradientContrastValue);


void subtractBooleanMaps(int imageWidth, int imageHeight, bool * booleanMap1, bool * booleanMap2, bool * booleanMap1MinusBooleanMap2);
void addBooleanMaps(int imageWidth, int imageHeight, bool * booleanMap1, bool * booleanMap2, bool * booleanMap1PlusBooleanMap2);


	//NOT YET FINISHED
void createNormalMap(int imageWidth, int imageHeight, double * luminosityContrastMapEye1, double * depthMap, double * depthContrastMap, double * depthGradientContrastMap, double * normalMap);

	//NOT YET FINISHED
double findDepthOfGivenPixel(int xEye1, int yEye1, int imageWidth, int imageHeight, double * luminosityContrastMapEye1, double * luminosityContrastMapEye2, unsigned char * rgbMapEye1, unsigned char * rgbMapEye2, double * calculatedxEye2, double * calculatedyEye2);


void cropRGBMap(int originalImageWidth, int originalImageHeight, int cropXPos, int cropYPos, int croppedWidth, int croppedHeight, unsigned char * rgbMapUncropped, unsigned char * rgbMap);
void cropDepthMap(int originalImageWidth, int originalImageHeight, int cropXPos, int cropYPos, int croppedWidth, int croppedHeight, double * depthMapUncropped, double * depthMap);

void resampleRGBMap(unsigned char * rgbMap, int imageWidth, int imageHeight, unsigned char * resampledRGBMapAtDesiredzoomChar, int zoom, int ignoreBackgroundComparisonMethod);
void resampleLumOrContrastOrDepthMap(double * lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double * resampledLumOrContrastOrDepthMapAtDesiredzoomChar, int zoom, double offMapValue);

void createArbitraryContrastBooleanMap(int imageWidth, int imageHeight, double * contrastMap, bool * contrastBooleanMap, double sensitivity);



#endif
