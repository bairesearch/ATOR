/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: ATORimagecomparison.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3k4a 21-May-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_IMAGE_COMPARISON
#define HEADER_OR_IMAGE_COMPARISON

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
	#define IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR (IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR_BASE* 3)		//see calcaverageerror.cpp/genHueCompAvgErr.cpp for derivation
	#define IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING (1.0)	//1.0

#endif
#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	#define IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR (0.166667* 2)		//see calcaverageerror.cpp for derivation: only correct for kernel width = 3, should be fine though since errors are relative
#endif





#define OR_IMAGE_COMPARISON_MAX_SD_FROM_AVG_HUE_DEVIATION (1)


#define RADIAL_CONTRAST_SUB_KERNEL_WIDTH (3)
#define RADIAL_CONTRAST_SUB_KERNEL_HEIGHT (3)

class ORimagecomparisonClass
{
	private: RTpixelMapsClass RTpixelMaps;
	private: SHAREDvarsClass SHAREDvars;
	private: ORpixelMapsClass ORpixelMaps;
	public: void disablePixelsThatAreNotContainedInTheObjectTriangle2DOD(const int imageWidth, const int imageHeight, unsigned char* rgbMap);
	public: void applyGaussianKernelToRGBmap(int imageWidth, const int imageHeight, const double sigma, const unsigned char* rgbMap, unsigned char* rgbMapWithGaussianApplied);
	private: void applyGaussianKernelToLuminosityMapOrRGBmapComponent(int imageWidth, const int imageHeight, const double sigma, const double* luminosityMap, double* luminosityMapWithGaussianApplied, const unsigned char* rgbMap, unsigned char* rgbMapWithGaussianApplied, const bool useRGBmapComponent, const int rgbMapComponentNumber);


	public: void createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(int imageWidth, int imageHeight, unsigned char* rgbMap, double* normalisedHueContrastMap, double* normalisedAverageHueContrastR, double* normalisedAverageHueContrastG, double* normalisedAverageHueContrastB, double* averageContrastWithRespectToAverageColour, double* averageStarkContrastWithRespectToAverageColour, double* averageLocalContrast, double* averageLocalStarkContrast);
	public: bool checkImageHasContrastValuesOnly(const double averageContrastWithRespectToAverageColour, const double averageStarkContrastWithRespectToAverageColour, const double averageLocalContrast, const double averageLocalStarkContrast);



	public: void calculateAverageColour(int imageWidth, int imageHeight, unsigned char* rgbMap, colour* avgCol);

	public: double compareImagesRGBsmallNoKernel(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain);

	#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
	private: double compareImagesRGBwithPosDevAndLocalStarkCont(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain);
	public: double compareImagesRGBwithPosDev(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain);				//OLD
	#endif
	#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
	public: bool checkImageHasContrast(int imageWidth, const int imageHeight, unsigned char* rgbMap);
	#endif
	public: double compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain);
		private: double compareImagesRGBwithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain, double* averageHueDeviationR, double* averageHueDeviationG, double* averageHueDeviationB, double* averageXkernelRelativePositionForLowestErrorMatch, double* averageYkernelRelativePositionForLowestErrorMatch);


		private: void createColourHueContrastVecMapFromRGBMapNOTUSED(int imageWidth, const int imageHeight, unsigned char* rgbMap, double* colourHueContrastVectorMap, int* colourHueContrastCategoryIntMap);
			private: void calculateColourHueContrastVecLevelWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight, vec* colourHueContrastVecVal, int* colourHueContrastCategoryIntVal);
		private: double calculateColourHueSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);
		private: double calculateColourHueRadialSumContrastWithinKernelNOTUSED(const int pixelX, const int pixelY, unsigned char* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);


	#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	private: double compareRGBpixelsForMatchHueOnly(colour* testImagePixelColour, colour* trainImagePixelColour);
	public: double compareRGBpixelsForMatchLumContrastOnly(colour* testImagePixelColour, colour* trainImagePixelColour);
		private: void calculateHueError(colour* testImagePixelColour, colour* trainImagePixelColour, double* hueError, double* nonHueError);
	#endif

	#ifdef OR_IMAGE_COMPARISON_USE_BAD_CODE
	private: double compareRGBpixelsForMatchBAD(const colour* testImagePixelColour, const colour* trainImagePixelColour);
		private: double calculateHueErrorNormalisedBAD(const colour* testImagePixelColour, const colour* trainImagePixelColour, double* hueError, double* nonHueErrorNormalised);
	#endif

};


#endif




