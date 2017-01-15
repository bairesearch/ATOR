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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3j1a 14-January-2017
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



void createPointNormalMapFromPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointNormalMap);
	void calculateMeshPointNormalsUsingPointMap(int x, int y, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight, double* pointMap, vec* meshPointNormal, vec meshPointNormals[]);
void createPointNormalContrastMapFromPointNormalMap(int imageWidth, const int imageHeight, double* pointNormalMap, double* pointNormalContrastMap);
	double calculatePointNormalContrastLevelWithinKernel(int pixelX, int pixelY, double* pointNormalMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);


void setXYvectorMapValue(const int x, const int y, const int imageWidth, vec* XYvectorVal, double* XYvectorMap);
void getXYvectorMapValue(int x, int y, int imageWidth, double* XYvectorMap, vec* XYvectorVal);

void createDepthContrastBooleanMap(const int imageWidth, const int imageHeight, const double* depthContrastMap, bool* depthContrastBooleanMap);

void generatePixmapFromDepthGradientMap(const string imageFileName, int imageWidth, const int imageHeight, double* depthGradientMap);
void generateRGBmapFromDepthGradientMap(int imageWidth, const int imageHeight, double* depthGradientMap, unsigned char* rgbMap);
void generatePixmapFromDepthGradientContrastMap(const string imageFileName, const int imageWidth, const int imageHeight, const double* depthGradientContrastMap);
void generateRGBmapFromDepthGradientContrastMap(const int imageWidth, const int imageHeight, const double* depthGradientContrastMap, unsigned char* rgbMap);
void createDepthGradientContrastBooleanMap(const int imageWidth, const int imageHeight, const double* depthGradientContrastMap, bool* depthGradientContrastBooleanMap);

void generatePixmapFromPointNormalContrastMap(const string imageFileName, const int imageWidth, const int imageHeight, const double* pointNormalContrastMap);
void generateRGBmapFromPointNormalContrastMap(const int imageWidth, const int imageHeight, const double* pointNormalContrastMap, unsigned char* rgbMap);
void createPointNormalContrastBooleanMap(const int imageWidth, const int imageHeight, const double* pointNormalContrastMap, bool* pointNormalContrastBooleanMap);




//methods taken from ORoperations.cpp;


double calculateForegroundMinimumDepthWithinKernel(const int pixelX, const int pixelY, const int imageWidth, const int imageHeight, const int kernelWidth, const int kernelHeight, const double* depthMap, vec* nearbyPointOfMinimumDepth, const int zoom);

void createContrastMapFromMapWithForegroundDepthCheckOLD(const int imageWidth, const int imageHeight, const double* luminosityOrDepthMap, const double* depthMap, double* contrastMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, const double mapThreshold);
	double calculateContrastLevelWithinKernelWithForegroundDepthCheckOLD(const int pixelX, const int pixelY, const double* luminosityOrDepthMap, const double* depthMap, const int kernelWidth, const int kernelHeight, const int imageWidth, const int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, const double mapThreshold);
/*
void createContrastBooleanMapFromContrastMapWithForegroundDepthCheck(int imageWidth, int imageHeight, double* contrastMap, double* depthMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
	double getContrastLevelWithinKernelWithForegroundDepthCheck(int pixelX, int pixelY, double* contrastMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
*/



void createDepthGradientMapFromDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, double* depthGradientMap);
	double calculateDepthGradientValueWithinKernel(const int pixelX, const int pixelY, const double* depthMap, const int kernelWidth, const int kernelHeight, const int imageWidth, const int imageHeight, vec* depthGradientVal);
void createDepthGradientContrastMapFromDepthGradientMap(int imageWidth, const int imageHeight, double* depthGradientMap, double* depthGradientContrastMap);
	void createDepthGradientContrastMapFromMap(int imageWidth, const int imageHeight, double* depthGradientMap, double* depthGradientContrastMap);
		double calculateDepthGradientContrastValueWithinKernel(int pixelX, int pixelY, double* depthGradientMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);
		double calculateDepthGradientContrastValueWithinKernelWRONG(const int pixelX, const int pixelY, double* depthGradientMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);
			bool checkIfMeetDepthGradientContrastThreshold(const double depthGradientContrastValue);


void subtractBooleanMaps(const int imageWidth, const int imageHeight, const bool* booleanMap1, const bool* booleanMap2, bool* booleanMap1MinusBooleanMap2);
void addBooleanMaps(const int imageWidth, const int imageHeight, const bool* booleanMap1, const bool* booleanMap2, bool* booleanMap1PlusBooleanMap2);


	//NOT YET FINISHED
void createNormalMap(const int imageWidth, const int imageHeight, const double* luminosityContrastMapEye1, const double* depthMap, const double* depthContrastMap, const double* depthGradientContrastMap, const double* normalMap);

	//NOT YET FINISHED
double findDepthOfGivenPixel(int xEye1, int yEye1, int imageWidth, const int imageHeight, const double* luminosityContrastMapEye1, const double* luminosityContrastMapEye2, unsigned char* rgbMapEye1, const unsigned char* rgbMapEye2, const double* calculatedxEye2, const double* calculatedyEye2);


void cropRGBmap(int originalImageWidth, const int originalImageHeight, const int cropXPos, const int cropYPos, const int croppedWidth, const int croppedHeight, unsigned char* rgbMapUncropped, unsigned char* rgbMap);
void cropDepthMap(const int originalImageWidth, const int originalImageHeight, const int cropXPos, const int cropYPos, const int croppedWidth, const int croppedHeight, const double* depthMapUncropped, double* depthMap);

void resampleRGBmap(unsigned char* rgbMap, const int imageWidth, const int imageHeight, unsigned char* resampledRGBmapAtDesiredZoomChar, const int zoom, const int ignoreBackgroundComparisonMethod);
void resampleLumOrContrastOrDepthMap(double* lumOrContrastOrDepthMap, const int imageWidth, const int imageHeight, double* resampledLumOrContrastOrDepthMapAtDesiredZoomChar, const int zoom, const double offMapValue);

void createArbitraryContrastBooleanMap(const int imageWidth, const int imageHeight, const double* contrastMap, bool* contrastBooleanMap, const double sensitivity);



#endif
