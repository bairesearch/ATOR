/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ORimagecomparison.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3c5c 21-December-2012
 *
 *******************************************************************************/



#ifndef HEADER_OR_IMAGE_COMPARISON
#define HEADER_OR_IMAGE_COMPARISON



#include "ORglobalDefs.h"
#include "SHAREDvars.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <time.h>
	#include <math.h>
	using namespace std;


#define SMALL_MAP_EXTENSION_PART ".small"
#define TRAIN_STRING ".train"
#define TEST_STRING ".test"
#define PNG_EXTENSION ".png"
#define JPG_EXTENSION ".jpg"


	//primary thresholds

#define IMAGE_COMPARISON_KERNEL_WIDTH (3)	//5
#define IMAGE_COMPARISON_KERNEL_HEIGHT (3)	//5


#define OR_IMAGE_COMPARISON_NO_CONTRAST_ERROR_KEY (9999)




#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
	//not used anymore
	#ifdef OR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
		#define OR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION (2000.0/(28*28)) //500
	#else
		#define OR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION (2000.0/(28*28/2)) //500
	#endif
#endif

#define VERY_HIGH_ERROR (99999999999999.0)

#define COLOUR_HUE_CONTRAST_CATEGORY_0 (0)
#define COLOUR_HUE_CONTRAST_CATEGORY_1 (1)	//edge aligned horizontally
#define COLOUR_HUE_CONTRAST_CATEGORY_2 (2)	//edge aligned vertically
#define COLOUR_HUE_CONTRAST_CATEGORY_3 (3)	//edge with negative grad
#define COLOUR_HUE_CONTRAST_CATEGORY_4 (4)	//edge with positive grad
#define COLOUR_HUE_CONTRAST_CATEGORY_5 (5)	//edge with negative grad
#define COLOUR_HUE_CONTRAST_CATEGORY_6 (6)	//edge with positive grad
#define COLOUR_HUE_CONTRAST_CATEGORY_7 (7)	//no obvious edge
#define COLOUR_HUE_CONTRAST_CATEGORY_8 (8)	//no obvious edge


#define HUE_CONTRAST_VECTOR_MAP_GENERATION_KERNEL_WIDTH (2)
#define HUE_CONTRAST_VECTOR_MAP_GENERATION_KERNEL_HEIGHT (2)



//not used anymore
#define COLOUR_HUE_CONTRAST_THRESHOLD (0.10)
#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	#define OR_IMAGE_COMPARISON_USE_OLD_OUTDATED_VARS
/*
#elif not defined OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	#define OR_IMAGE_COMPARISON_USE_OLD_OUTDATED_VARS
*/
#endif
#ifdef OR_IMAGE_COMPARISON_USE_OLD_OUTDATED_VARS
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING (1.0)
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR_BASE (85.332)
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR (IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR_BASE * 3)		//see calcaverageerror.cpp/genHueCompAvgErr.cpp for derivation
	#define IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING (1.0)	//1.0

#endif
#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	#define IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR (0.166667 * 2)		//see calcaverageerror.cpp for derivation: only correct for kernel width = 3, should be fine though since errors are relative
#endif





#define OR_IMAGE_COMPARISON_MAX_SD_FROM_AVG_HUE_DEVIATION (1)


#define RADIAL_CONTRAST_SUB_KERNEL_WIDTH (3)
#define RADIAL_CONTRAST_SUB_KERNEL_HEIGHT (3)

void disablePixelsThatAreNotContainedInTheObjectTriangle2DOD(int imageWidth, int imageHeight, unsigned char * rgbMap);
void applyGaussianKernelToRGBmap(int imageWidth, int imageHeight, double sigma, unsigned char * rgbMap, unsigned char * rgbMapWithGaussianApplied);
void applyGaussianKernelToLuminosityMapOrRGBmapComponent(int imageWidth, int imageHeight, double sigma, double * luminosityMap, double * luminosityMapWithGaussianApplied, unsigned char * rgbMap, unsigned char * rgbMapWithGaussianApplied, bool useRGBmapComponent, int rgbMapComponentNumber);


void createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(int imageWidth, int imageHeight, unsigned char * rgbMap, double * normalisedHueContrastMap, double * normalisedAverageHueContrastR, double * normalisedAverageHueContrastG, double * normalisedAverageHueContrastB, double * averageContrastWithRespectToAverageColour, double * averageStarkContrastWithRespectToAverageColour, double * averageLocalContrast, double * averageLocalStarkContrast);
bool checkImageHasContrastValuesOnly(double averageContrastWithRespectToAverageColour, double averageStarkContrastWithRespectToAverageColour, double averageLocalContrast, double averageLocalStarkContrast);



void calculateAverageColour(int imageWidth, int imageHeight, unsigned char * rgbMap, colour * avgCol);

double compareImagesRGBsmallNoKernel(int imageWidth, int imageHeight, unsigned char * rgbMapTest, unsigned char * rgbMapTrain);

#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
double compareImagesRGBwithPosDevAndLocalStarkCont(int imageWidth, int imageHeight, unsigned char * rgbMapTest, unsigned char * rgbMapTrain);
double compareImagesRGBwithPosDev(int imageWidth, int imageHeight, unsigned char * rgbMapTest, unsigned char * rgbMapTrain);				//OLD
#endif
#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
bool checkImageHasContrast(int imageWidth, int imageHeight, unsigned char * rgbMap);
#endif
double compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(int imageWidth, int imageHeight, unsigned char * rgbMapTest, unsigned char * rgbMapTrain);
	double compareImagesRGBwithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(int imageWidth, int imageHeight, unsigned char * rgbMapTest, unsigned char * rgbMapTrain, double * averageHueDeviationR, double * averageHueDeviationG, double * averageHueDeviationB, double * averageXkernelRelativePositionForLowestErrorMatch, double * averageYkernelRelativePositionForLowestErrorMatch);


	void createColourHueContrastVecMapFromRGBMapNOTUSED(int imageWidth, int imageHeight, unsigned char * rgbMap, double * colourHueContrastVectorMap, int * colourHueContrastCategoryIntMap);
		void calculateColourHueContrastVecLevelWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, vec * colourHueContrastVecVal, int * colourHueContrastCategoryIntVal);
	double calculateColourHueSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);
	double calculateColourHueRadialSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight);


#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
double compareRGBpixelsForMatchHueOnly(colour * testImagePixelColour, colour * trainImagePixelColour);
double compareRGBpixelsForMatchLumContrastOnly(colour * testImagePixelColour, colour * trainImagePixelColour);
	void calculateHueError(colour * testImagePixelColour, colour * trainImagePixelColour, double * hueError, double * nonHueError);
#endif

#ifdef OR_IMAGE_COMPARISON_USE_BAD_CODE
double compareRGBpixelsForMatchBAD(colour * testImagePixelColour, colour * trainImagePixelColour);
	double calculateHueErrorNormalisedBAD(colour * testImagePixelColour, colour * trainImagePixelColour, double * hueError, double * nonHueErrorNormalised);
#endif


#endif




