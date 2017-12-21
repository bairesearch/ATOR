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
 * File Name: ATORpixelMaps.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3m6a 10-December-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_PIXELMAPS
#define HEADER_OR_PIXELMAPS

#include "SHAREDvars.hpp"

#include "RTpixelMaps.hpp"
#include "ATORglobalDefs.hpp"
#include "SHAREDvector.hpp"



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



class ORpixelMapsClass
{
	private: RTpixelMapsClass RTpixelMaps;
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	private: RTppmClass RTppm;
	public: void createPointNormalMapFromPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointNormalMap);
		private: void calculateMeshPointNormalsUsingPointMap(int x, int y, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight, double* pointMap, vec* meshPointNormal, vec meshPointNormals[]);
	public: void createPointNormalContrastMapFromPointNormalMap(int imageWidth, const int imageHeight, double* pointNormalMap, double* pointNormalContrastMap);
		private: double calculatePointNormalContrastLevelWithinKernel(int pixelX, int pixelY, double* pointNormalMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);


	public: void setXYvectorMapValue(const int x, const int y, const int imageWidth, vec* XYvectorVal, double* XYvectorMap);
	private: void getXYvectorMapValue(int x, int y, int imageWidth, double* XYvectorMap, vec* XYvectorVal);

	public: void createDepthContrastBooleanMap(const int imageWidth, const int imageHeight, const double* depthContrastMap, bool* depthContrastBooleanMap);

	public: void generatePixmapFromDepthGradientMap(const string imageFileName, int imageWidth, const int imageHeight, double* depthGradientMap);
	public: void generateRGBmapFromDepthGradientMap(int imageWidth, const int imageHeight, double* depthGradientMap, unsigned char* rgbMap);
	public: void generatePixmapFromDepthGradientContrastMap(const string imageFileName, const int imageWidth, const int imageHeight, const double* depthGradientContrastMap);
	private: void generateRGBmapFromDepthGradientContrastMap(const int imageWidth, const int imageHeight, const double* depthGradientContrastMap, unsigned char* rgbMap);
	public: void createDepthGradientContrastBooleanMap(const int imageWidth, const int imageHeight, const double* depthGradientContrastMap, bool* depthGradientContrastBooleanMap);

	public: void generatePixmapFromPointNormalContrastMap(const string imageFileName, const int imageWidth, const int imageHeight, const double* pointNormalContrastMap);
	public: void generateRGBmapFromPointNormalContrastMap(const int imageWidth, const int imageHeight, const double* pointNormalContrastMap, unsigned char* rgbMap);
	public: void createPointNormalContrastBooleanMap(const int imageWidth, const int imageHeight, const double* pointNormalContrastMap, bool* pointNormalContrastBooleanMap);




	//methods taken from ORoperations.cpp;


	public: double calculateForegroundMinimumDepthWithinKernel(const int pixelX, const int pixelY, const int imageWidth, const int imageHeight, const int kernelWidth, const int kernelHeight, const double* depthMap, vec* nearbyPointOfMinimumDepth, const int zoom);

	private: void createContrastMapFromMapWithForegroundDepthCheckOLD(const int imageWidth, const int imageHeight, const double* luminosityOrDepthMap, const double* depthMap, double* contrastMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, const double mapThreshold);
		private: double calculateContrastLevelWithinKernelWithForegroundDepthCheckOLD(const int pixelX, const int pixelY, const double* luminosityOrDepthMap, const double* depthMap, const int kernelWidth, const int kernelHeight, const int imageWidth, const int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, const double mapThreshold);
	/*
	void createContrastBooleanMapFromContrastMapWithForegroundDepthCheck(int imageWidth, int imageHeight, double* contrastMap, double* depthMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
		double getContrastLevelWithinKernelWithForegroundDepthCheck(int pixelX, int pixelY, double* contrastMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold);
	*/



	public: void createDepthGradientMapFromDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, double* depthGradientMap);
		private: double calculateDepthGradientValueWithinKernel(const int pixelX, const int pixelY, const double* depthMap, const int kernelWidth, const int kernelHeight, const int imageWidth, const int imageHeight, vec* depthGradientVal);
	public: void createDepthGradientContrastMapFromDepthGradientMap(int imageWidth, const int imageHeight, double* depthGradientMap, double* depthGradientContrastMap);
		private: void createDepthGradientContrastMapFromMap(int imageWidth, const int imageHeight, double* depthGradientMap, double* depthGradientContrastMap);
			private: double calculateDepthGradientContrastValueWithinKernel(int pixelX, int pixelY, double* depthGradientMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);
			private: double calculateDepthGradientContrastValueWithinKernelWRONG(const int pixelX, const int pixelY, double* depthGradientMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight);
				private: bool checkIfMeetDepthGradientContrastThreshold(const double depthGradientContrastValue);


	public: void subtractBooleanMaps(const int imageWidth, const int imageHeight, const bool* booleanMap1, const bool* booleanMap2, bool* booleanMap1MinusBooleanMap2);
	public: void addBooleanMaps(const int imageWidth, const int imageHeight, const bool* booleanMap1, const bool* booleanMap2, bool* booleanMap1PlusBooleanMap2);


	//NOT YET FINISHED
	private: void createNormalMap(const int imageWidth, const int imageHeight, const double* luminosityContrastMapEye1, const double* depthMap, const double* depthContrastMap, const double* depthGradientContrastMap, const double* normalMap);

	//NOT YET FINISHED
	private: double findDepthOfGivenPixel(int xEye1, int yEye1, int imageWidth, const int imageHeight, const double* luminosityContrastMapEye1, const double* luminosityContrastMapEye2, unsigned char* rgbMapEye1, const unsigned char* rgbMapEye2, const double* calculatedxEye2, const double* calculatedyEye2);


	public: void cropRGBmap(int originalImageWidth, const int originalImageHeight, const int cropXPos, const int cropYPos, const int croppedWidth, const int croppedHeight, unsigned char* rgbMapUncropped, unsigned char* rgbMap);
	public: void cropDepthMap(const int originalImageWidth, const int originalImageHeight, const int cropXPos, const int cropYPos, const int croppedWidth, const int croppedHeight, const double* depthMapUncropped, double* depthMap);

	public: void resampleRGBmap(unsigned char* rgbMap, const int imageWidth, const int imageHeight, unsigned char* resampledRGBmapAtDesiredZoomChar, const int zoom, const int ignoreBackgroundComparisonMethod);
	public: void resampleLumOrContrastOrDepthMap(double* lumOrContrastOrDepthMap, const int imageWidth, const int imageHeight, double* resampledLumOrContrastOrDepthMapAtDesiredZoomChar, const int zoom, const double offMapValue);

	public: void createArbitraryContrastBooleanMap(const int imageWidth, const int imageHeight, const double* contrastMap, bool* contrastBooleanMap, const double sensitivity);
};



#endif
