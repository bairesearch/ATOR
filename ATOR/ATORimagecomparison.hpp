 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORimagecomparison.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_IMAGE_COMPARISON
#define HEADER_ATOR_IMAGE_COMPARISON

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORoperations.hpp"

#define SMALL_MAP_EXTENSION_PART ".small"
#define TRAIN_STRING ".train"
#define TEST_STRING ".test"
#define PNG_EXTENSION ".png"
#define JPG_EXTENSION ".jpg"


	//primary thresholds

#define IMAGE_COMPARISON_KERNEL_WIDTH (3)	//5
#define IMAGE_COMPARISON_KERNEL_HEIGHT (3)	//5


#define ATOR_IMAGE_COMPARISON_NO_CONTRAST_ERROR_KEY (9999)




#ifndef ATOR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
	//not used anymore
	#ifdef ATOR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
		#define ATOR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION (2000.0/(28*28)) //500
	#else
		#define ATOR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION (2000.0/(28*28/2)) //500
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
#ifndef ATOR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	#define ATOR_IMAGE_COMPARISON_USE_OLD_OUTDATED_VARS
/*
#elif not defined ATOR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	#define ATOR_IMAGE_COMPARISON_USE_OLD_OUTDATED_VARS
*/
#endif
#ifdef ATOR_IMAGE_COMPARISON_USE_OLD_OUTDATED_VARS
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING (1.0)
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR_BASE (85.332)
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR (IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR_BASE* 3)		//see calcaverageerror.cpp/genHueCompAvgErr.cpp for derivation
	#define IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING (1.0)	//1.0

#endif
#ifndef ATOR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	#define IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR (0.166667* 2)		//see calcaverageerror.cpp for derivation: only correct for kernel width = 3, should be fine though since errors are relative
#endif





#define ATOR_IMAGE_COMPARISON_MAX_SD_FROM_AVG_HUE_DEVIATION (1)


#define RADIAL_CONTRAST_SUB_KERNEL_WIDTH (3)
#define RADIAL_CONTRAST_SUB_KERNEL_HEIGHT (3)

class ATORimagecomparisonClass
{
	private: RTpixelMapsClass RTpixelMaps;
	private: SHAREDvarsClass SHAREDvars;
	private: ATORpixelMapsClass ATORpixelMaps;
	public: void disablePixelsThatAreNotContainedInTheObjectTriangle2DOD(const int imageWidth, const int imageHeight, uchar* rgbMap);
	public: void applyGaussianKernelToRGBmap(int imageWidth, const int imageHeight, const double sigma, const uchar* rgbMap, uchar* rgbMapWithGaussianApplied);
	private: void applyGaussianKernelToLuminosityMapOrRGBmapComponent(int imageWidth, const int imageHeight, const double sigma, const double* luminosityMap, double* luminosityMapWithGaussianApplied, const uchar* rgbMap, uchar* rgbMapWithGaussianApplied, const bool useRGBmapComponent, const int rgbMapComponentNumber);


	public: void createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(int imageWidth, int imageHeight, uchar* rgbMap, double* normalisedHueContrastMap, double* normalisedAverageHueContrastR, double* normalisedAverageHueContrastG, double* normalisedAverageHueContrastB, double* averageContrastWithRespectToAverageColour, double* averageStarkContrastWithRespectToAverageColour, double* averageLocalContrast, double* averageLocalStarkContrast);
	public: bool checkImageHasContrastValuesOnly(const double averageContrastWithRespectToAverageColour, const double averageStarkContrastWithRespectToAverageColour, const double averageLocalContrast, const double averageLocalStarkContrast);



	public: void calculateAverageColour(int imageWidth, int imageHeight, uchar* rgbMap, colour* avgCol);

	public: double compareImagesRGBsmallNoKernel(int imageWidth, const int imageHeight, uchar* rgbMapTest, uchar* rgbMapTrain);

	#ifndef ATOR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	private: double compareImagesRGBwithPosDevAndLocalStarkCont(int imageWidth, const int imageHeight, uchar* rgbMapTest, uchar* rgbMapTrain);
	public: double compareImagesRGBwithPosDev(int imageWidth, const int imageHeight, uchar* rgbMapTest, uchar* rgbMapTrain);				//OLD
	#endif
	#ifndef ATOR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
	public: bool checkImageHasContrast(int imageWidth, const int imageHeight, uchar* rgbMap);
	#endif
	public: double compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(int imageWidth, const int imageHeight, uchar* rgbMapTest, uchar* rgbMapTrain);
		private: double compareImagesRGBwithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(int imageWidth, const int imageHeight, uchar* rgbMapTest, uchar* rgbMapTrain, double* averageHueDeviationR, double* averageHueDeviationG, double* averageHueDeviationB, double* averageXkernelRelativePositionForLowestErrorMatch, double* averageYkernelRelativePositionForLowestErrorMatch);


		private: void createColourHueContrastVecMapFromRGBMapNOTUSED(int imageWidth, const int imageHeight, uchar* rgbMap, double* colourHueContrastVectorMap, int* colourHueContrastCategoryIntMap);
			private: void calculateColourHueContrastVecLevelWithinKernelNOTUSED(int pixelX, int pixelY, uchar* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight, vec* colourHueContrastVecVal, int* colourHueContrastCategoryIntVal);
		private: double calculateColourHueSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, uchar* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);
		private: double calculateColourHueRadialSumContrastWithinKernelNOTUSED(const int pixelX, const int pixelY, uchar* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);


	#ifndef ATOR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	private: double compareRGBpixelsForMatchHueOnly(colour* testImagePixelColour, colour* trainImagePixelColour);
	public: double compareRGBpixelsForMatchLumContrastOnly(colour* testImagePixelColour, colour* trainImagePixelColour);
		private: void calculateHueError(colour* testImagePixelColour, colour* trainImagePixelColour, double* hueError, double* nonHueError);
	#endif

	#ifdef ATOR_IMAGE_COMPARISON_USE_BAD_CODE
	private: double compareRGBpixelsForMatchBAD(const colour* testImagePixelColour, const colour* trainImagePixelColour);
		private: double calculateHueErrorNormalisedBAD(const colour* testImagePixelColour, const colour* trainImagePixelColour, double* hueError, double* nonHueErrorNormalised);
	#endif

};


#endif




