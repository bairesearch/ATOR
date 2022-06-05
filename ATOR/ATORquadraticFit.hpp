/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORquadraticFit.hpp (based on EdgiseFrame.java, version 1.17 (26-02-04) CSEM)
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3p4c 07-August-2021
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_QUADRATIC_FIT
#define HEADER_ATOR_QUADRATIC_FIT

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "SHAREDvector.hpp"
#include "RTpixelMaps.hpp"
#include "ATORpixelMaps.hpp"
#include "RTscene.hpp"
#include "float.h"

	//For a quadratic fit there are 7 coefficients; a[0]..a[6]. but a[5] = a[6])
#define NUMBER_OF_COEFFICIENTS (6)


#define QUADRATIC_FIT_COLOUR_ZERO_CROSSING_R (255)
#define QUADRATIC_FIT_COLOUR_ZERO_CROSSING_G (0)
#define QUADRATIC_FIT_COLOUR_ZERO_CROSSING_B (0)
#define QUADRATIC_FIT_COLOUR_EDGE_R (0)
#define QUADRATIC_FIT_COLOUR_EDGE_G (192)
#define QUADRATIC_FIT_COLOUR_EDGE_B (0)
#define QUADRATIC_FIT_COLOUR_ZERO_CROSSING_ILLEGAL_R (255)
#define QUADRATIC_FIT_COLOUR_ZERO_CROSSING_ILLEGAL_G (0)
#define QUADRATIC_FIT_COLOUR_ZERO_CROSSING_ILLEGAL_B (255)


/*
#define FIX_NEW_HEITGER_CONSTRAST_THRESHOLD_ISSUE_MAYBE_DUE_TO_DEPTH_MAP_CHECK
#ifdef ATOR_USE_AMSTERDAM_TEST_IMAGE_DATA
	#define HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD (256*1.5)
	#define HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/4.0)
	#define HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD*10.0)
#elif defined ATOR_USE_GOOGLE_MAPS
	#define HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD (100)	//50
	#define HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/2.0)
	#define HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD*10.0)	//2.5
#elif defined ATOR_USE_STAR_MAPS
	#define HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD (256*1)
	#define HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/4.0)
	#define HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD*10.0)
#else
	#ifdef FIX_NEW_HEITGER_CONSTRAST_THRESHOLD_ISSUE_MAYBE_DUE_TO_DEPTH_MAP_CHECK
		#define HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD (256)
		#define HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/5.0)
		#define HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD*10.0)
	#else
		#define HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD (570)
		#define HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/5.0)
		#define HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD*10.0)
	#endif

	//#define HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD (256*3/1.5)
	//#define HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD*3.5)
#endif
*/


#define EDGE_DETECT (true)
#define POINT_DETECT (false)


class ATORQFzeroCrossing{

	private:
		/*There are currently no attributes of this class*/
	public:

		ATORQFzeroCrossing(void); // constructor declaration
		~ATORQFzeroCrossing();	//  and destructor.

		int x;
		int y;
		float zeroCrossingValueX;
		float zeroCrossingValueY;
		float alpha;
		float beta;
		float coefficient[NUMBER_OF_COEFFICIENTS];
		ATORQFzeroCrossing* next;

		float dzAlpha;
		float dzBeta;
		float dzTwoAlpha;
		float dzTwoBeta;
		float confidence;


		double depth;	//for 3DOD only
		#ifndef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
		int nearbyHitValueX;
		int nearbyHitValueY;
		#endif
		//#ifdef ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP
		vec point;
		//#endif
};

class ATORquadraticFitClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: RTpixelMapsClass RTpixelMaps;
	private: ATORpixelMapsClass ATORpixelMaps;
	public: double calculateAreaOfOneSideOfEdgeInPixel(const int xDevPointOnSide, const int yDevPointOnSide, const double zeroCrossingValueX, const double zeroCrossingValueY, const double alpha);


/*************************************** Edgise Frame High Level ('EdgiseFrameStandard') Methods* ***********************************/
		public: void generateZeroCrossingList(const double* luminosityContrastMap, const int imageWidth, const int imageHeight, ATORQFzeroCrossing* firstZeroCrossingInList, const bool edgeDetect, const double sensitivity, const int dimension, const double* pointMap, const double* depthMap, const int zoom, const int interpixelMapType);
			private: void edgiseData(const bool edgeDetect, const bool createEnhancedImageDisplayingQuadraticFitInfo, ATORQFzeroCrossing* currentZeroCrossingInList, const int imageWidth, const int imageHeight, const double luminosityContrastMap[], const double sensitivity, const int dimension, const double* pointMap, const double* depthMap, const int zoom, const int interpixelMapType);
				private: bool calculateZeroCrossingAndOrientation(int x, int y, float coefficient[], const bool edgeDetect, const bool createEnhancedImageDisplayingQuadraticFitInfo, ATORQFzeroCrossing* currentZeroCrossingInList);
/************************************ End Edgise Frame High Level ('EdgiseFrameStandard') Methods* **********************************/



/*********************************** Fit Methods* ******************************/
	private: void calculateQuadraticFitCoefficients(const int x, const int y, float coefficient[], const double luminosityContrastMap[]);
	private: void calculateQuadraticFitCoefficients3x3(const int x, const int y, float coefficient[], const double luminosityContrastMap[]);
	private: void calculateQuadraticFitCoefficients5x5(const int x, const int y, float coefficient[], const double luminosityContrastMap[]);
/*********************************** End Fit Methods* ******************************/

/********************************** Pixel Checking methods* ******************************/
	private: bool checkForTotalPositiveCurvature(const float a3, const float a4);
	private: bool checkForTotalNegativeCurvature(const float a3, const float a4);
	private: bool checkTotalNegativeCurvatureAbovePointThreshold(const float a3, const float a4);
/********************************** End Pixel Checking methods* ******************************/

/********************************** Zero Crossing Checking methods* **********************************/
	private: bool checkEdgeZeroCrossingObjectPassesThreshold(ATORQFzeroCrossing* zc);
		private: bool checkEdgeZeroCrossingObjectContrastGradients(ATORQFzeroCrossing* zc);
	private: bool checkPointZeroCrossingObjectPassesThreshold(const ATORQFzeroCrossing* zc);
	//	private: bool checkTotalNegativeCurvatureAbovePointThreshold(const float a3, const float a4);

/********************************** End Zero Crossing Checking methods* **********************************/

/********************************** Matrix Manipulation methods* **********************************/
	private: float getSmallestEigenValue(const float eigenValue[]);
	private: void calculateEdgeZeroCrossing(const float coefficient[], const float beta, float zc[]);
	private: void calculatePointZeroCrossing(const float coefficient[], float zc[]);
	private: void calculateEigenVector(const float coefficient[], const float ev, float eigenVector[]);
	private: void calculateEigenValues(const float coefficient[], float ev[]);
/********************************** End Matrix Manipulation methods* **********************************/

/*************************************** Get/Set Methods* ***********************************/
	private: float getPixelFloat(const int x, const int y, const double luminosityContrastMap[]);
/*************************************** End Get/Set Methods* *******************************/

};







#endif
