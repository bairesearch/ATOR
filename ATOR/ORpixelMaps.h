/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ORpixelMaps.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3h9b 09-December-2015
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



void createPointNormalMapFromPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointNormalMap);
	void calculateMeshPointNormalsUsingPointMap(int x, int y, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, double* pointMap, vec* meshPointNormal, vec meshPointNormals[]);
void createPointNormalContrastMapFromPointNormalMap(int imageWidth, int imageHeight, double* pointNormalMap, double* pointNormalContrastMap);
	double calculatePointNormalContrastLevelWithinKernel(int pixelX, int pixelY, double* pointNormalMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);


void setXYvectorMapValue(int x, int y, int imageWidth, vec* XYvectorVal, double* XYvectorMap);
void getXYvectorMapValue(int x, int y, int imageWidth, double* XYvectorMap, vec* XYvectorVal);

void createDepthContrastBooleanMap(int imageWidth, int imageHeight, double* depthContrastMap, bool* depthContrastBooleanMap);

void generatePixmapFromDepthGradientMap(string imageFileName, int imageWidth, int imageHeight, double* depthGradientMap);
void generateRGBmapFromDepthGradientMap(int imageWidth, int imageHeight, double* depthGradientMap, unsigned char* rgbMap);
void generatePixmapFromDepthGradientContrastMap(string imageFileName, int imageWidth, int imageHeight, double* depthGradientContrastMap);
void generateRGBmapFromDepthGradientContrastMap(int imageWidth, int imageHeight, double* depthGradientContrastMap, unsigned char* rgbMap);
void createDepthGradientContrastBooleanMap(int imageWidth, int imageHeight, double* depthGradientContrastMap, bool* depthGradientContrastBooleanMap);

void generatePixmapFromPointNormalContrastMap(string imageFileName, int imageWidth, int imageHeight, double* pointNormalContrastMap);
void generateRGBmapFromPointNormalContrastMap(int imageWidth, int imageHeight, double* pointNormalContrastMap, unsigned char* rgbMap);
void createPointNormalContrastBooleanMap(int imageWidth, int imageHeight, double* pointNormalContrastMap, bool* pointNormalContrastBooleanMap);




//methods taken from ORoperations.cpp;


double calculateForegroundMinimumDepthWithinKernel(int pixelX, int pixelY, int imageWidth, int imageHeight, int kernelWidth, int kernelHeight, double* depthMap, vec* nearbyPointOfMinimumDepth, int zoom);

void createContrastMapFromMapWithForegroundDepthCheckOLD(int imageWidth, int imageHeight, double* luminosityOrDepthMap, double* depthMap, double* contrastMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
	double calculateContrastLevelWithinKernelWithForegroundDepthCheckOLD(int pixelX, int pixelY, double* luminosityOrDepthMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
/*
void createContrastBooleanMapFromContrastMapWithForegroundDepthCheck(int imageWidth, int imageHeight, double* contrastMap, double* depthMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
	double getContrastLevelWithinKernelWithForegroundDepthCheck(int pixelX, int pixelY, double* contrastMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
*/



void createDepthGradientMapFromDepthMap(int imageWidth, int imageHeight, double* depthMap, double* depthGradientMap);
	double calculateDepthGradientValueWithinKernel(int pixelX, int pixelY, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, vec* depthGradientVal);
void createDepthGradientContrastMapFromDepthGradientMap(int imageWidth, int imageHeight, double* depthGradientMap, double* depthGradientContrastMap);
	void createDepthGradientContrastMapFromMap(int imageWidth, int imageHeight, double* depthGradientMap, double* depthGradientContrastMap);
		double calculateDepthGradientContrastValueWithinKernel(int pixelX, int pixelY, double* depthGradientMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);
		double calculateDepthGradientContrastValueWithinKernelWRONG(int pixelX, int pixelY, double* depthGradientMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);
			bool checkIfMeetDepthGradientContrastThreshold(double depthGradientContrastValue);


void subtractBooleanMaps(int imageWidth, int imageHeight, bool* booleanMap1, bool* booleanMap2, bool* booleanMap1MinusBooleanMap2);
void addBooleanMaps(int imageWidth, int imageHeight, bool* booleanMap1, bool* booleanMap2, bool* booleanMap1PlusBooleanMap2);


	//NOT YET FINISHED
void createNormalMap(int imageWidth, int imageHeight, double* luminosityContrastMapEye1, double* depthMap, double* depthContrastMap, double* depthGradientContrastMap, double* normalMap);

	//NOT YET FINISHED
double findDepthOfGivenPixel(int xEye1, int yEye1, int imageWidth, int imageHeight, double* luminosityContrastMapEye1, double* luminosityContrastMapEye2, unsigned char* rgbMapEye1, unsigned char* rgbMapEye2, double* calculatedxEye2, double* calculatedyEye2);


void cropRGBmap(int originalImageWidth, int originalImageHeight, int cropXPos, int cropYPos, int croppedWidth, int croppedHeight, unsigned char* rgbMapUncropped, unsigned char* rgbMap);
void cropDepthMap(int originalImageWidth, int originalImageHeight, int cropXPos, int cropYPos, int croppedWidth, int croppedHeight, double* depthMapUncropped, double* depthMap);

void resampleRGBmap(unsigned char* rgbMap, int imageWidth, int imageHeight, unsigned char* resampledRGBmapAtDesiredZoomChar, int zoom, int ignoreBackgroundComparisonMethod);
void resampleLumOrContrastOrDepthMap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double* resampledLumOrContrastOrDepthMapAtDesiredZoomChar, int zoom, double offMapValue);

void createArbitraryContrastBooleanMap(int imageWidth, int imageHeight, double* contrastMap, bool* contrastBooleanMap, double sensitivity);



#endif
