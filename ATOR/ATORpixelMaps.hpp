 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORpixelMaps.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_PIXELMAPS
#define HEADER_ATOR_PIXELMAPS

#include "SHAREDvars.hpp"

#include "RTpixelMaps.hpp"
#include "ATORglobalDefs.hpp"
#include "SHAREDvector.hpp"



#define ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF (0)
#define ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE (1)
#define ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE_COMPLETELY (2)


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



class ATORpixelMapsClass
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
	public: void generateRGBmapFromDepthGradientMap(int imageWidth, const int imageHeight, double* depthGradientMap, uchar* rgbMap);
	public: void generatePixmapFromDepthGradientContrastMap(const string imageFileName, const int imageWidth, const int imageHeight, const double* depthGradientContrastMap);
	private: void generateRGBmapFromDepthGradientContrastMap(const int imageWidth, const int imageHeight, const double* depthGradientContrastMap, uchar* rgbMap);
	public: void createDepthGradientContrastBooleanMap(const int imageWidth, const int imageHeight, const double* depthGradientContrastMap, bool* depthGradientContrastBooleanMap);

	public: void generatePixmapFromPointNormalContrastMap(const string imageFileName, const int imageWidth, const int imageHeight, const double* pointNormalContrastMap);
	public: void generateRGBmapFromPointNormalContrastMap(const int imageWidth, const int imageHeight, const double* pointNormalContrastMap, uchar* rgbMap);
	public: void createPointNormalContrastBooleanMap(const int imageWidth, const int imageHeight, const double* pointNormalContrastMap, bool* pointNormalContrastBooleanMap);




	//methods taken from ATORoperations.cpp;


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
	private: double findDepthOfGivenPixel(int xEye1, int yEye1, int imageWidth, const int imageHeight, const double* luminosityContrastMapEye1, const double* luminosityContrastMapEye2, uchar* rgbMapEye1, const uchar* rgbMapEye2, const double* calculatedxEye2, const double* calculatedyEye2);


	public: void cropRGBmap(int originalImageWidth, const int originalImageHeight, const int cropXPos, const int cropYPos, const int croppedWidth, const int croppedHeight, uchar* rgbMapUncropped, uchar* rgbMap);
	public: void cropDepthMap(const int originalImageWidth, const int originalImageHeight, const int cropXPos, const int cropYPos, const int croppedWidth, const int croppedHeight, const double* depthMapUncropped, double* depthMap);

	public: void resampleRGBmap(uchar* rgbMap, const int imageWidth, const int imageHeight, uchar* resampledRGBmapAtDesiredZoomChar, const int zoom, const int ignoreBackgroundComparisonMethod);
	public: void resampleLumOrContrastOrDepthMap(double* lumOrContrastOrDepthMap, const int imageWidth, const int imageHeight, double* resampledLumOrContrastOrDepthMapAtDesiredZoomChar, const int zoom, const double offMapValue);

	public: void createArbitraryContrastBooleanMap(const int imageWidth, const int imageHeight, const double* contrastMap, bool* contrastBooleanMap, const double sensitivity);
};



#endif
