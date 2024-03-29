 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORquadraticFit.cpp (based on EdgiseFrame.java, version 1.17 (26-02-04) CSEM)
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3r1a 29-February-2024
 * Requirements: Assumes that depth information is less accurate than image information
 * /
 *******************************************************************************/


#include "ATORquadraticFit.hpp"


#ifdef ATOR_QUADRATIC_FIT_VERBOSE
#endif

#ifndef LINUX
int isnan(double d)
{
	return _isnan(d);
}
#endif


ATORQFzeroCrossing::ATORQFzeroCrossing(void)
{
	x = 0;	//x pos pixel
	y = 0;	//y pos pixel
	zeroCrossingValueX = 0.0;	// exact x pos
	zeroCrossingValueY = 0.0;	// exact y pos
	alpha = 0.0;
	beta = 0.0;
	for(int i=0; i<NUMBER_OF_COEFFICIENTS; i++)
	{
		coefficient[i] = 0.0;
	}
	dzAlpha = 0.0;
	dzBeta = 0.0;
	dzTwoAlpha = 0.0;
	dzTwoBeta = 0.0;
	confidence = 0.0;


	depth = 0.0;	//for 3DOD only
	#ifndef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
	nearbyHitValueX = 0;
	nearbyHitValueY = 0;
	#endif
	//#ifdef ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP
	point.x = 0.0;
	point.y = 0.0;
	point.z = 0.0;
	//#endif


	next = NULL;
}

ATORQFzeroCrossing::~ATORQFzeroCrossing()
{
	if(next != NULL)
	{
		delete next;
	}
}




static int globalImageWidth;





void ATORquadraticFitClass::generateZeroCrossingList(const double* luminosityContrastMap, const int imageWidth, const int imageHeight, ATORQFzeroCrossing* firstZeroCrossingInList, const bool edgeDetect, const double sensitivity, const int dimension, const double* pointMap, const double* depthMap, const int zoom, const int interpixelMapType)
{
	globalImageWidth = imageWidth;

	bool createEnhancedImageDisplayingQuadraticFitInfo;
	#ifdef ATOR_QUADRATIC_FIT_VERBOSE
	createEnhancedImageDisplayingQuadraticFitInfo = true;
	#else
	createEnhancedImageDisplayingQuadraticFitInfo = false;
	#endif

	edgiseData(edgeDetect, createEnhancedImageDisplayingQuadraticFitInfo, firstZeroCrossingInList, imageWidth, imageHeight, luminosityContrastMap, sensitivity, dimension, pointMap, depthMap, zoom, interpixelMapType);

}

double ATORquadraticFitClass::calculateAreaOfOneSideOfEdgeInPixel(const int xDevPointOnSide, const int yDevPointOnSide, const double zeroCrossingValueX, const double zeroCrossingValueY, const double alpha)
{
	bool areaIsJustTriangle = true;
	//calculate area inside of triangle made up by edge (areaIsJustTriangle) [+ rect adjacent this if it exists (!areaIsJustTriangle)]

	vec edgeIntersectBoundary[2];
	edgeIntersectBoundary[0].x = 0.0;
	edgeIntersectBoundary[0].y = 0.0;
	edgeIntersectBoundary[0].z = 0.0;
	edgeIntersectBoundary[1].x = 0.0;
	edgeIntersectBoundary[1].y = 0.0;
	edgeIntersectBoundary[1].z = 0.0;

	vec tempIntersectBoundary;
	tempIntersectBoundary.x = 0.0;
	tempIntersectBoundary.y = 0.0;
	tempIntersectBoundary.z = 0.0;




	//calculate intersection of theoretical edge line with pixel boundary

	int numberEdgeIntersectBoundariesFound = 0;

	double edgeValueX;
	double edgeValueY;

	edgeValueX = -HALF_PIXEL_WIDTH;
	edgeValueY = edgeValueX*tan(alpha) + zeroCrossingValueY;
	if((edgeValueY >= - HALF_PIXEL_WIDTH) && (edgeValueY <= HALF_PIXEL_WIDTH))
	{
		tempIntersectBoundary.x = edgeValueX;
		tempIntersectBoundary.y = edgeValueY;

		bool alreadyAdded = false;
		for(int i=0; i<numberEdgeIntersectBoundariesFound; i++)
		{
			if(SHAREDvector.compareVectors(&tempIntersectBoundary, &(edgeIntersectBoundary[i])))
			{
				alreadyAdded = true;
			}
		}
		if(!alreadyAdded)
		{
			SHAREDvector.copyVectors(&(edgeIntersectBoundary[numberEdgeIntersectBoundariesFound]), &tempIntersectBoundary);
			numberEdgeIntersectBoundariesFound++;
		}
	}

	edgeValueX = HALF_PIXEL_WIDTH;
	edgeValueY = edgeValueX*tan(alpha) + zeroCrossingValueY;
	if((edgeValueY >= - HALF_PIXEL_WIDTH) && (edgeValueY <= HALF_PIXEL_WIDTH))
	{
		tempIntersectBoundary.x = edgeValueX;
		tempIntersectBoundary.y = edgeValueY;

		bool alreadyAdded = false;
		for(int i=0; i<numberEdgeIntersectBoundariesFound; i++)
		{
			if(SHAREDvector.compareVectors(&tempIntersectBoundary, &(edgeIntersectBoundary[i])))
			{
				alreadyAdded = true;
			}
		}
		if(!alreadyAdded)
		{
			SHAREDvector.copyVectors(&(edgeIntersectBoundary[numberEdgeIntersectBoundariesFound]), &tempIntersectBoundary);
			numberEdgeIntersectBoundariesFound++;
		}
	}

	edgeValueY = -HALF_PIXEL_WIDTH;
	if(tan(alpha) != 0)
	{
		edgeValueX = (edgeValueY - zeroCrossingValueY) / tan(alpha);
		if((edgeValueX >= - HALF_PIXEL_WIDTH) && (edgeValueX <= HALF_PIXEL_WIDTH))
		{
			tempIntersectBoundary.x = edgeValueX;
			tempIntersectBoundary.y = edgeValueY;

			bool alreadyAdded = false;
			for(int i=0; i<numberEdgeIntersectBoundariesFound; i++)
			{
				if(SHAREDvector.compareVectors(&tempIntersectBoundary, &(edgeIntersectBoundary[i])))
				{
					alreadyAdded = true;
				}
			}
			if(!alreadyAdded)
			{
				SHAREDvector.copyVectors(&(edgeIntersectBoundary[numberEdgeIntersectBoundariesFound]), &tempIntersectBoundary);
				numberEdgeIntersectBoundariesFound++;
			}
		}
	}
	edgeValueY = HALF_PIXEL_WIDTH;
	if(tan(alpha) != 0)
	{
		edgeValueX = (edgeValueY - zeroCrossingValueY) / tan(alpha);
		if((edgeValueX >= - HALF_PIXEL_WIDTH) && (edgeValueX <= HALF_PIXEL_WIDTH))
		{
			tempIntersectBoundary.x = edgeValueX;
			tempIntersectBoundary.y = edgeValueY;

			bool alreadyAdded = false;
			for(int i=0; i<numberEdgeIntersectBoundariesFound; i++)
			{
				if(SHAREDvector.compareVectors(&tempIntersectBoundary, &(edgeIntersectBoundary[i])))
				{
					alreadyAdded = true;
				}
			}
			if(!alreadyAdded)
			{
				SHAREDvector.copyVectors(&(edgeIntersectBoundary[numberEdgeIntersectBoundariesFound]), &tempIntersectBoundary);
				numberEdgeIntersectBoundariesFound++;
			}
		}
	}
	if(numberEdgeIntersectBoundariesFound != 2)
	{
		cerr << "error: numberEdgeIntersectBoundariesFound != 2" << endl;
		exit(EXIT_ERROR);
	}


	vec testPoint;
	testPoint.x = double(xDevPointOnSide)*HALF_PIXEL_WIDTH;
	testPoint.y = double(yDevPointOnSide)*HALF_PIXEL_WIDTH;
	testPoint.z = 0.0;

	double area;


	/*Edge Type 1 - forms quad with pixel corners*/
	int scenarioChosen;
	double areaPart1;
	double areaPart2;
	if(((edgeIntersectBoundary[0].x == -HALF_PIXEL_WIDTH) && (edgeIntersectBoundary[1].x == HALF_PIXEL_WIDTH))
	|| ((edgeIntersectBoundary[0].x == HALF_PIXEL_WIDTH) && (edgeIntersectBoundary[1].x == -HALF_PIXEL_WIDTH)))
	{
		int side1;
		int side2;
		if((edgeIntersectBoundary[0].x == -HALF_PIXEL_WIDTH) && (edgeIntersectBoundary[1].x == HALF_PIXEL_WIDTH))
		{
			side1 = 0;
			side2 = 1;
		}
		else
		{
			side1 = 1;
			side2 = 0;
		}

		if(testPoint.y == HALF_PIXEL_WIDTH)
		{
			scenarioChosen = 0;
		}
		else if(testPoint.y == -HALF_PIXEL_WIDTH)
		{
			scenarioChosen = 1;
		}
		else if(testPoint.x == -HALF_PIXEL_WIDTH)
		{
			if(testPoint.y > edgeIntersectBoundary[side1].y)
			{
				scenarioChosen = 0;
			}
			else
			{
				scenarioChosen = 1;
			}

		}
		else if(testPoint.x == HALF_PIXEL_WIDTH)
		{
			if(testPoint.y > edgeIntersectBoundary[side2].y)
			{
				scenarioChosen = 0;
			}
			else
			{
				scenarioChosen = 1;
			}
		}
		else
		{
			cerr << "error: invalid testpoint" << endl;
			exit(EXIT_ERROR);
		}

		if(scenarioChosen == 0)
		{
			double triangleBoundaryY = SHAREDvars.maxDouble(edgeIntersectBoundary[0].y, edgeIntersectBoundary[1].y);
			double heightPart1 = SHAREDvars.absDouble(edgeIntersectBoundary[0].y - edgeIntersectBoundary[1].y);
			areaPart1 = 0.5* heightPart1* (HALF_PIXEL_WIDTH*2.0);	//0.5B*W
			areaPart2 = (HALF_PIXEL_WIDTH-triangleBoundaryY)* (HALF_PIXEL_WIDTH*2.0);	//B*W
		}
		else
		{
			double triangleBoundaryY = SHAREDvars.minDouble(edgeIntersectBoundary[0].y, edgeIntersectBoundary[1].y);
			double heightPart1 = SHAREDvars.absDouble(edgeIntersectBoundary[0].y - edgeIntersectBoundary[1].y);
			areaPart1 = 0.5* heightPart1* (HALF_PIXEL_WIDTH*2.0);	//0.5B*W
			areaPart2 = (HALF_PIXEL_WIDTH+triangleBoundaryY)* (HALF_PIXEL_WIDTH*2.0);	//B*W
		}
		area = areaPart1 + areaPart2;
		areaIsJustTriangle = false;
	}
	else if(((edgeIntersectBoundary[0].y == -HALF_PIXEL_WIDTH) && (edgeIntersectBoundary[1].y == HALF_PIXEL_WIDTH))
	|| ((edgeIntersectBoundary[0].y == HALF_PIXEL_WIDTH) && (edgeIntersectBoundary[1].y == -HALF_PIXEL_WIDTH)))
	{
		int side1;
		int side2;
		if((edgeIntersectBoundary[0].y == -HALF_PIXEL_WIDTH) && (edgeIntersectBoundary[1].y == HALF_PIXEL_WIDTH))
		{
			side1 = 0;
			side2 = 1;
		}
		else
		{
			side1 = 1;
			side2 = 0;
		}

		if(testPoint.x == HALF_PIXEL_WIDTH)
		{
			scenarioChosen = 0;
		}
		else if(testPoint.x == -HALF_PIXEL_WIDTH)
		{
			scenarioChosen = 1;
		}
		else if(testPoint.y == -HALF_PIXEL_WIDTH)
		{
			if(testPoint.x > edgeIntersectBoundary[side1].x)
			{
				scenarioChosen = 0;
			}
			else
			{
				scenarioChosen = 1;
			}

		}
		else if(testPoint.y == HALF_PIXEL_WIDTH)
		{
			if(testPoint.x > edgeIntersectBoundary[side2].x)
			{
				scenarioChosen = 0;
			}
			else
			{
				scenarioChosen = 1;
			}

		}
		else
		{
			cerr << "error: invalid testpoint" << endl;
			exit(EXIT_ERROR);
		}
		if(scenarioChosen == 0)
		{
			double triangleBoundaryX = SHAREDvars.maxDouble(edgeIntersectBoundary[0].x, edgeIntersectBoundary[1].x);
			double widthPart1 = SHAREDvars.absDouble(edgeIntersectBoundary[0].x - edgeIntersectBoundary[1].x);
			areaPart1 = 0.5* widthPart1* (HALF_PIXEL_WIDTH*2.0);	//0.5B*W
			areaPart2 = (HALF_PIXEL_WIDTH-triangleBoundaryX)* (HALF_PIXEL_WIDTH*2.0);	//B*W
		}
		else
		{
			double triangleBoundaryX = SHAREDvars.minDouble(edgeIntersectBoundary[0].x, edgeIntersectBoundary[1].x);
			double widthPart1 = SHAREDvars.absDouble(edgeIntersectBoundary[0].x - edgeIntersectBoundary[1].x);
			areaPart1 = 0.5* widthPart1* (HALF_PIXEL_WIDTH*2.0);	//0.5B*W
			areaPart2 = (HALF_PIXEL_WIDTH-triangleBoundaryX)* (HALF_PIXEL_WIDTH*2.0);	//B*W
		}
		area = areaPart1 + areaPart2;
		areaIsJustTriangle = false;
	}


	/*Edge Type 2 - forms triangle with pixel corners*/
	if(areaIsJustTriangle)
	{
		double smallerArea;

		double width;
		double height;
		vec thirdPoint;
		thirdPoint.x = 0.0;
		thirdPoint.y = 0.0;
		thirdPoint.z = 0.0;

		int sidex1;
		int sidex2;
		if(SHAREDvars.absDouble(edgeIntersectBoundary[0].x ) != HALF_PIXEL_WIDTH)
		{
			sidex1 = 0;
			sidex2 = 1;
		}
		else if(SHAREDvars.absDouble(edgeIntersectBoundary[1].x) != HALF_PIXEL_WIDTH)
		{
			sidex1 = 1;
			sidex2 = 0;
		}
		else
		{
			cerr << "error: areaIsJustTriangle and no edgeIntersectBoundary @ x = +/-HALF_PIXEL_WIDTH" << endl;
			exit(EXIT_ERROR);
		}
		if(edgeIntersectBoundary[sidex2].x == HALF_PIXEL_WIDTH)
		{
			width = HALF_PIXEL_WIDTH - edgeIntersectBoundary[sidex1].x;
			thirdPoint.x = HALF_PIXEL_WIDTH;
		}
		else if(edgeIntersectBoundary[sidex2].x == -HALF_PIXEL_WIDTH)
		{
			width = HALF_PIXEL_WIDTH + edgeIntersectBoundary[sidex1].x;
			thirdPoint.x = -HALF_PIXEL_WIDTH;
		}
		else
		{
			cerr << "error: areaIsJustTriangle and x mismatch " << sidex1 << endl;
			exit(EXIT_ERROR);
		}


		int sidey1;
		int sidey2;
		if(SHAREDvars.absDouble(edgeIntersectBoundary[0].y ) != HALF_PIXEL_WIDTH)
		{
			sidey1 = 0;
			sidey2 = 1;
		}
		else if(SHAREDvars.absDouble(edgeIntersectBoundary[1].y) != HALF_PIXEL_WIDTH)
		{
			sidey1 = 1;
			sidey2 = 0;
		}
		else
		{
			cerr << "error: areaIsJustTriangle and no edgeIntersectBoundary @ y = +/-HALF_PIXEL_WIDTH" << endl;
			exit(EXIT_ERROR);
		}
		if(edgeIntersectBoundary[sidey2].y == HALF_PIXEL_WIDTH)
		{
			height = HALF_PIXEL_WIDTH - edgeIntersectBoundary[sidey1].y;
			thirdPoint.y = HALF_PIXEL_WIDTH;
		}
		else if(edgeIntersectBoundary[sidey2].y == -HALF_PIXEL_WIDTH)
		{
			height = HALF_PIXEL_WIDTH + edgeIntersectBoundary[sidey1].y;
			thirdPoint.y = -HALF_PIXEL_WIDTH;
		}
		else
		{
			cerr << "error: areaIsJustTriangle and y mismatch " << sidey1 << endl;
			exit(EXIT_ERROR);
		}

		smallerArea = 0.5*width*height;

		double distanceFromTestPointToThirdPoint = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&testPoint, &thirdPoint);
		double distanceFromEdgeIntersectBoundary1ToThirdPoint = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(edgeIntersectBoundary[0]), &thirdPoint);
		double distanceFromEdgeIntersectBoundary2ToThirdPoint = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(edgeIntersectBoundary[1]), &thirdPoint);

		if(distanceFromTestPointToThirdPoint < SHAREDvars.minDouble(distanceFromEdgeIntersectBoundary1ToThirdPoint, distanceFromEdgeIntersectBoundary2ToThirdPoint))
		{
			area = smallerArea;
		}
		else
		{
			area = (HALF_PIXEL_WIDTH*2.0) - smallerArea;
		}

	}

	return area;
}


/*************************************** Edgise Frame High Level ('EdgiseFrameStandard') Methods* ***********************************/

/****
 * edgiseData()
 *
 * preconditions:	-relevant rasterArrays are filled
 *
 * postconditions: 	-produces zero crossing information after applying a quadratic fit
 *					is stored/drawed/compared depending on how the constructor was called
 *
 ****/

//pointMap not currently used
void ATORquadraticFitClass::edgiseData(const bool edgeDetect, const bool createEnhancedImageDisplayingQuadraticFitInfo, ATORQFzeroCrossing* currentZeroCrossingInList, const int imageWidth, const int imageHeight, const double luminosityContrastMap[], const double sensitivity, const int dimension, const double* pointMap, const double* depthMap, const int zoom, const int interpixelMapType)
{
	int kernelWidthForegroundCheck;
	int kernelHeightForegroundCheck;
	if(interpixelMapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
	{
		//without ATOR_USE_CONTRAST_CALC_METHOD_C
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
	}
	else if(interpixelMapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
	{
		//such as luminosityContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL;
	}
	else if(interpixelMapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
	{
		//such as gradientContrast or pointNormalContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_NO_INTERPIXEL;
	}


	#ifdef ATOR_QUADRATIC_FIT_VERBOSE

	#define ENHANCEMENT_FACTOR (5)	//must be odd number
	int enhancementFactor = ENHANCEMENT_FACTOR;
	int enhancedImageWidth = imageWidth*enhancementFactor;
	int enhancedImageHeight = imageHeight*enhancementFactor;
	int enhancedImageSize = enhancedImageWidth*enhancedImageHeight;
	uchar* quadraticFitMapEnhancedRGB = new uchar[enhancedImageSize*RGB_NUM];

	if(createEnhancedImageDisplayingQuadraticFitInfo)
	{
		 for(int y=0; y < imageHeight; y++)
		 {
			 for(int x=0; x < imageWidth; x++)
			 {
				 for(int ey=0; ey < enhancementFactor; ey++)
				 {
					 for(int ex=0; ex < enhancementFactor; ex++)
					 {
						quadraticFitMapEnhancedRGB[((y*enhancementFactor)+ey)*enhancedImageWidth*RGB_NUM + ((x*enhancementFactor) + ex)*RGB_NUM + RGB_RED] = luminosityContrastMap[y*imageWidth  + x] / MAX_LUMINOSITY_CONTRAST* MAX_RGB_VAL;
				 		quadraticFitMapEnhancedRGB[((y*enhancementFactor)+ey)*enhancedImageWidth*RGB_NUM + ((x*enhancementFactor) + ex)*RGB_NUM + RGB_GREEN] = luminosityContrastMap[y*imageWidth  + x] / MAX_LUMINOSITY_CONTRAST* MAX_RGB_VAL;
						quadraticFitMapEnhancedRGB[((y*enhancementFactor)+ey)*enhancedImageWidth*RGB_NUM + ((x*enhancementFactor) + ex)*RGB_NUM + RGB_BLUE] = luminosityContrastMap[y*imageWidth  + x] / MAX_LUMINOSITY_CONTRAST* MAX_RGB_VAL;
					 }
				 }
			 }
		 }
	}
	#endif


	int robotTerritoryHeight = imageHeight - (QUADRATIC_FIT_KERNEL_SIZE-1);
	int robotTerritoryWidth = imageWidth - (QUADRATIC_FIT_KERNEL_SIZE-1);

	int x,y;

	/*finished clearing all lists*/

	float* coefficient = new float[NUMBER_OF_COEFFICIENTS];

	//This loop determines the coefficients of the fit for each pixel in the AdviseFrame
	//If the edgise process is enabled, this loop fills in the edgisedData Array necessary
	//for display output by EdigiseDisplay.

	for(y=0; y < robotTerritoryHeight; y++)	//Eg y:0->126
	{
		for(x=0; x < robotTerritoryWidth; x++)	//Eg x:0->126
		{
			float featureProbabilityLevelCentrePixel = getPixelFloat(x+(QUADRATIC_FIT_KERNEL_SIZE/2), y+(QUADRATIC_FIT_KERNEL_SIZE/2), luminosityContrastMap);

			bool centreContrastThreshold;
			if(edgeDetect == POINT_DETECT)
			{
				if(featureProbabilityLevelCentrePixel >= HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD*sensitivity)
				{
					//pixel is above ZC contrast threshold
					centreContrastThreshold = true;
				}
				else
				{
					centreContrastThreshold = false;
				}
			}
			else if(edgeDetect == EDGE_DETECT)
			{
				if(featureProbabilityLevelCentrePixel > sensitivity)
				{
					//pixel is above ZC contrast threshold
					centreContrastThreshold = true;
				}
				else
				{
					centreContrastThreshold = false;
				}
			}

			bool centreFeatureFound = true;
			double featureProbabilityLevelKernelPixels = 0.0;

			if((centreContrastThreshold == true) && (edgeDetect == POINT_DETECT))
			{
				for(int ky = y+(QUADRATIC_FIT_KERNEL_SIZE/2)-(QUADRATIC_FIT_KERNEL_SIZE/2); ky <= y+(QUADRATIC_FIT_KERNEL_SIZE/2)+(QUADRATIC_FIT_KERNEL_SIZE/2); ky++)
				{
					for(int kx = x+(QUADRATIC_FIT_KERNEL_SIZE/2)-(QUADRATIC_FIT_KERNEL_SIZE/2); kx <= x+(QUADRATIC_FIT_KERNEL_SIZE/2)+(QUADRATIC_FIT_KERNEL_SIZE/2); kx++)
					{
						if(edgeDetect == POINT_DETECT)
						{

							double currentFeatureProbabilityLevelKernelPixel = getPixelFloat(kx, ky, luminosityContrastMap);
							featureProbabilityLevelKernelPixels = featureProbabilityLevelKernelPixels + currentFeatureProbabilityLevelKernelPixel;

							if(currentFeatureProbabilityLevelKernelPixel > featureProbabilityLevelCentrePixel)
							{
								centreFeatureFound = false;
							}
						}

					}
				}
			}

		#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
			//3DOD only;
			vec xyzNearbyPointOnObject;
			xyzNearbyPointOnObject.x = 0.0;
			xyzNearbyPointOnObject.y = 0.0;
			xyzNearbyPointOnObject.z = 0.0;
			double minDepthForNearbyPoints = REALLY_FAR_AWAY;

			if((centreContrastThreshold == true) && (dimension == ATOR_METHOD3DOD_DIMENSIONS))
			{
				minDepthForNearbyPoints = ATORpixelMaps.calculateForegroundMinimumDepthWithinKernel(x+(QUADRATIC_FIT_KERNEL_SIZE/2), y+(QUADRATIC_FIT_KERNEL_SIZE/2), imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, &xyzNearbyPointOnObject, zoom);
			}
		#endif

			bool centreContrastThresholdAdvanced = false;
			if(edgeDetect == POINT_DETECT)
			{
				if((centreContrastThreshold == true) && (centreFeatureFound == true))
				{
					if(featureProbabilityLevelKernelPixels >= HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD*sensitivity)
					{
						centreContrastThresholdAdvanced = true;
					}
				}

			}
			else
			{
				if(centreContrastThreshold == true)
				{//pixel is above ZC contrast threshold
					centreContrastThresholdAdvanced = true;
				}
			}

			if(centreContrastThresholdAdvanced == true)
			{//pixel is above ZC contrast threshold


				calculateQuadraticFitCoefficients((x+(QUADRATIC_FIT_KERNEL_SIZE/2)), (y+(QUADRATIC_FIT_KERNEL_SIZE/2)), coefficient, luminosityContrastMap);	//+1!
						//determines Coefficients for rasterArray1 from pixels 1->127

				if(edgeDetect)
				{
					#ifndef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ
					if(!checkForTotalPositiveCurvature(coefficient[3], coefficient[4]))
					{
					#endif

						bool passZeroCrossingTest = false;
						if(calculateZeroCrossingAndOrientation((x+(QUADRATIC_FIT_KERNEL_SIZE/2)), (y+(QUADRATIC_FIT_KERNEL_SIZE/2)), coefficient, edgeDetect, createEnhancedImageDisplayingQuadraticFitInfo, currentZeroCrossingInList))
						{
							passZeroCrossingTest = true;
						}

						#ifdef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ_2
						bool zeroCrossingIllegal = false;
						if(!passZeroCrossingTest)
						{
							if(isnan(currentZeroCrossingInList->zeroCrossingValueX) || isnan(currentZeroCrossingInList->zeroCrossingValueY))
							{
								zeroCrossingIllegal = true;

								currentZeroCrossingInList->zeroCrossingValueX = 0.0;
								currentZeroCrossingInList->zeroCrossingValueY = 0.0;
								currentZeroCrossingInList->alpha = 0.0;
								currentZeroCrossingInList->beta = 0.0;

								currentZeroCrossingInList->dzAlpha = 0.0;
								currentZeroCrossingInList->dzBeta = 0.0;
								currentZeroCrossingInList->dzTwoAlpha = 0.0;
								currentZeroCrossingInList->dzTwoBeta = 0.0;
								currentZeroCrossingInList->confidence = 0.0;

								passZeroCrossingTest = true;

							}
						}
						#endif

						if(passZeroCrossingTest)
						{
							#ifdef ATOR_QUADRATIC_FIT_VERBOSE
							if(createEnhancedImageDisplayingQuadraticFitInfo)
							{

								/*
								cout << "currentZeroCrossingInList->zeroCrossingValueX = " << currentZeroCrossingInList->zeroCrossingValueX << endl;
								cout << "currentZeroCrossingInList->zeroCrossingValueY = " << currentZeroCrossingInList->zeroCrossingValueY << endl;
								cout << "currentZeroCrossingInList->x = " << currentZeroCrossingInList->x << endl;
								cout << "currentZeroCrossingInList->y = " << currentZeroCrossingInList->y << endl;
								cout << "currentZeroCrossingInList->dzTwoAlpha = " << currentZeroCrossingInList->dzTwoAlpha << endl;
								cout << "currentZeroCrossingInList->dzTwoBeta = " << currentZeroCrossingInList->dzTwoBeta << endl;
								cout << "currentZeroCrossingInList->dzTwox = " << currentZeroCrossingInList->coefficient[3] << endl;
								cout << "currentZeroCrossingInList->dzTwoy = " << currentZeroCrossingInList->coefficient[4] << endl;
								cout << "currentZeroCrossingInList->beta = " << currentZeroCrossingInList->beta << endl;
								*/

								//then draw edge;
								//tan(alpha) = y/x
								//y = x*tan(alpha)
								//edge y = x*tan(alpha) + zerocrossingyval
								//edge x = (edge y - zerocrossingyval)/ tan(alpha)

								for(int e=0; e<=enhancementFactor; e++)
								{
									double edgeValueX = (double(e)/double(enhancementFactor)) - HALF_PIXEL_WIDTH;
									double edgeValueY = edgeValueX*tan(currentZeroCrossingInList->alpha) + currentZeroCrossingInList->zeroCrossingValueY;

									if((edgeValueY >= -HALF_PIXEL_WIDTH) && (edgeValueY <= HALF_PIXEL_WIDTH))
									{
										quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(edgeValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(edgeValueX*enhancementFactor)))*RGB_NUM + RGB_RED] = QUADRATIC_FIT_COLOUR_EDGE_R;
				 						quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(edgeValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(edgeValueX*enhancementFactor)))*RGB_NUM + RGB_GREEN] = QUADRATIC_FIT_COLOUR_EDGE_G;
										quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(edgeValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(edgeValueX*enhancementFactor)))*RGB_NUM + RGB_BLUE] = QUADRATIC_FIT_COLOUR_EDGE_B;
									}
								}

								for(int e=0; e<=enhancementFactor; e++)
								{
									double edgeValueY = (double(e)/double(enhancementFactor)) - HALF_PIXEL_WIDTH;
									if(tan(currentZeroCrossingInList->alpha) != 0)
									{
										double edgeValueX = (edgeValueY - currentZeroCrossingInList->zeroCrossingValueY) / tan(currentZeroCrossingInList->alpha);

										if((edgeValueX >= -HALF_PIXEL_WIDTH) && (edgeValueX <= HALF_PIXEL_WIDTH))
										{
											quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(edgeValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(edgeValueX*enhancementFactor)))*RGB_NUM + RGB_RED] = QUADRATIC_FIT_COLOUR_EDGE_R;
				 							quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(edgeValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(edgeValueX*enhancementFactor)))*RGB_NUM + RGB_GREEN] = QUADRATIC_FIT_COLOUR_EDGE_G;
											quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(edgeValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(edgeValueX*enhancementFactor)))*RGB_NUM + RGB_BLUE] = QUADRATIC_FIT_COLOUR_EDGE_B;
										}
									}
								}

								#ifdef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ_2
								if(zeroCrossingIllegal)
								{
									quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_RED] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_ILLEGAL_R;
				 					quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_GREEN] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_ILLEGAL_G;
									quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_BLUE] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_ILLEGAL_B;
								}
								else
								{
								#endif
									quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_RED] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_R;
				 					quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_GREEN] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_G;
									quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_BLUE] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_B;
								#ifdef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ_2
								}
								#endif
							}
							#endif


							#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
							if(dimension == ATOR_METHOD3DOD_DIMENSIONS)	//3DOD only;
							{
								double depthVal = RTpixelMaps.getLumOrContrastOrDepthMapValue(x*zoom, y*zoom, imageWidth*zoom, depthMap);
								bool onObject = false;

								#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
								if(!SHAREDvars.compareDoubles(depthVal, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW 18 Nov 09 {ensures the pixel is a foreground pixel}
								{//off object
								#endif
									if(depthVal < minDepthForNearbyPoints)
									{
										onObject = true;

									}
								#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
								}
								#endif

								if(onObject)
								{

									currentZeroCrossingInList->depth = depthVal;
									#ifndef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
									currentZeroCrossingInList->nearbyHitValueX = x;
									currentZeroCrossingInList->nearbyHitValueY = y;
									#endif
								}
								else
								{
									/*
									cout << "feature off object" << endl;
									cout << "xyzNearbyPointOnObject.x = " << xyzNearbyPointOnObject.x << endl;
									cout << "xyzNearbyPointOnObject.y = " << xyzNearbyPointOnObject.y << endl;
									cout << "xyzNearbyPointOnObject.z = " << xyzNearbyPointOnObject.z << endl;
									cout << "depthVal = " << depthVal << endl;
									cout << "minDepthForNearbyPoints = " << minDepthForNearbyPoints << endl;
									*/
									currentZeroCrossingInList->depth = minDepthForNearbyPoints;
									#ifndef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
									currentZeroCrossingInList->nearbyHitValueX = xyzNearbyPointOnObject.x;
									currentZeroCrossingInList->nearbyHitValueY = xyzNearbyPointOnObject.y;
									#endif
								}
							}
							#endif

							ATORQFzeroCrossing* zc = new ATORQFzeroCrossing();
							currentZeroCrossingInList->next = zc;
							currentZeroCrossingInList = currentZeroCrossingInList->next;
						}

					#ifndef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ
					}
					#endif
				}
				else
				{//feature detect

					/*
					if(checkForTotalNegativeCurvature(coefficient[3], coefficient[4]))
					{
					*/
					//optimisation;
					if(checkTotalNegativeCurvatureAbovePointThreshold(coefficient[3], coefficient[4]))
					{

						if(calculateZeroCrossingAndOrientation((x+(QUADRATIC_FIT_KERNEL_SIZE/2)), (y+(QUADRATIC_FIT_KERNEL_SIZE/2)), coefficient, edgeDetect, createEnhancedImageDisplayingQuadraticFitInfo, currentZeroCrossingInList))
						{
							#ifdef ATOR_QUADRATIC_FIT_VERBOSE
							if(createEnhancedImageDisplayingQuadraticFitInfo)
							{
								/*
								cout << "currentZeroCrossingInList->zeroCrossingValueX = " << currentZeroCrossingInList->zeroCrossingValueX << endl;
								cout << "currentZeroCrossingInList->zeroCrossingValueY = " << currentZeroCrossingInList->zeroCrossingValueY << endl;
								cout << "currentZeroCrossingInList->x = " << currentZeroCrossingInList->x << endl;
								cout << "currentZeroCrossingInList->y = " << currentZeroCrossingInList->y << endl;
								//cout << "currentZeroCrossingInList->dzTwoAlpha = " << currentZeroCrossingInList->dzTwoAlpha << endl;
								//cout << "currentZeroCrossingInList->dzTwoBeta = " << currentZeroCrossingInList->dzTwoBeta << endl;
								//cout << "currentZeroCrossingInList->dzTwox = " << currentZeroCrossingInList->coefficient[3] << endl;
								//cout << "currentZeroCrossingInList->dzTwoy = " << currentZeroCrossingInList->coefficient[4] << endl;
								*/

								quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_RED] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_R;
				 				quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_GREEN] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_G;
								quadraticFitMapEnhancedRGB[(((y+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor)+int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueY*enhancementFactor)))*enhancedImageWidth*RGB_NUM + (((x+(QUADRATIC_FIT_KERNEL_SIZE/2))*enhancementFactor) + int(int(enhancementFactor/2) + int(currentZeroCrossingInList->zeroCrossingValueX*enhancementFactor)))*RGB_NUM + RGB_BLUE] = QUADRATIC_FIT_COLOUR_ZERO_CROSSING_B;
							}
							#endif

							#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
							if(dimension == ATOR_METHOD3DOD_DIMENSIONS)	//3DOD only;
							{
								double depthVal = RTpixelMaps.getLumOrContrastOrDepthMapValue(x*zoom, y*zoom, imageWidth*zoom, depthMap);
								bool onObject = false;

								#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
								if(!SHAREDvars.compareDoubles(depthVal, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW 18 Nov 09 {ensures the pixel is a foreground pixel}
								{//off object
								#endif
									if(depthVal < minDepthForNearbyPoints)
									{
										onObject = true;
									}
								#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
								}
								#endif

								if(onObject)
								{

									currentZeroCrossingInList->depth = depthVal;
									#ifndef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
									currentZeroCrossingInList->nearbyHitValueX = x;
									currentZeroCrossingInList->nearbyHitValueY = y;
									#endif
								}
								else
								{

									currentZeroCrossingInList->depth = minDepthForNearbyPoints;
									#ifndef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
									currentZeroCrossingInList->nearbyHitValueX = xyzNearbyPointOnObject.x;
									currentZeroCrossingInList->nearbyHitValueY = xyzNearbyPointOnObject.y;
									#endif
								}
							}
							#endif

							ATORQFzeroCrossing* zc = new ATORQFzeroCrossing();
							currentZeroCrossingInList->next = zc;
							currentZeroCrossingInList = currentZeroCrossingInList->next;
						}
					}

				}
				//checks for individual bumps of high contrast (considered noise)
			}
		}
	}

	#ifdef ATOR_QUADRATIC_FIT_VERBOSE
	if(createEnhancedImageDisplayingQuadraticFitInfo)
	{
		if(edgeDetect)
		{
			RTpixelMaps.generatePixmapFromRGBmap("debugEdgeDetectionQuadraticFitMapEnhancedRGB.ppm", enhancedImageWidth, enhancedImageHeight, quadraticFitMapEnhancedRGB);
		}
		else
		{
			RTpixelMaps.generatePixmapFromRGBmap("debugPointDetectionQuadraticFitMapEnhancedRGB.ppm", enhancedImageWidth, enhancedImageHeight, quadraticFitMapEnhancedRGB);
		}
		//exit(EXIT_ERROR);
	}
	#endif
}







	//x, y are redundant
bool ATORquadraticFitClass::calculateZeroCrossingAndOrientation(int x, int y, float coefficient[], const bool edgeDetect, const bool createEnhancedImageDisplayingQuadraticFitInfo, ATORQFzeroCrossing* currentZeroCrossingInList)
{
	//Summary:
	// calculates eigen values of the C coefficient in coefficients[], an eigen vector, alpha, beta,
	// zeroCrossings, and checks whether the zeroCrossing is valid.
	// depending on implementation, the zero crossing can be recorded into a list while
	// the zerocrossing and its orientation, the associated solution, and the oMap orientation
	// can be made ready for display.

		//passed Threshold variable is used to note whether the any data from
		//the fit will be recorded/displayed..
	bool passedThreshold;

	float* eigenValues = new float[2];

	calculateEigenValues(coefficient, eigenValues);

	float minEigenValue = getSmallestEigenValue(eigenValues);

	float* eigenVector = new float[2];
	calculateEigenVector(coefficient, minEigenValue, eigenVector);

	float alpha = (float)atan2(eigenVector[1], eigenVector[0]);	//tan(^-1)(y/x)

	float beta = alpha + ((float)PI)/2.0;

	float* zeroCrossing = new float[2];
	if(edgeDetect)
	{
		calculateEdgeZeroCrossing(coefficient, beta, zeroCrossing);
	}
	else
	{
		calculatePointZeroCrossing(coefficient, zeroCrossing);
	}

	float zeroCrossingValueX = zeroCrossing[0];
	float zeroCrossingValueY = zeroCrossing[1];

	currentZeroCrossingInList->x = x;
	currentZeroCrossingInList->y = y;
	currentZeroCrossingInList->zeroCrossingValueX = zeroCrossingValueX;
	currentZeroCrossingInList->zeroCrossingValueY = zeroCrossingValueY;
	currentZeroCrossingInList->alpha = alpha;
	currentZeroCrossingInList->beta = beta;
	currentZeroCrossingInList->coefficient[0] = coefficient[0];
	currentZeroCrossingInList->coefficient[1] = coefficient[1];
	currentZeroCrossingInList->coefficient[2] = coefficient[2];
	currentZeroCrossingInList->coefficient[3] = coefficient[3];
	currentZeroCrossingInList->coefficient[4] = coefficient[4];
	currentZeroCrossingInList->coefficient[5] = coefficient[5];

	//Passes ZeroCrossing zc so it can be filled with all available data
	if(edgeDetect)
	{
		passedThreshold = checkEdgeZeroCrossingObjectPassesThreshold(currentZeroCrossingInList);
		//passedThreshold = checkEdgeZeroCrossingFloatPassesThreshold(x, y, zeroCrossingValueX, zeroCrossingValueY, coefficient, alpha)
	}
	else
	{//point
		passedThreshold = checkPointZeroCrossingObjectPassesThreshold(currentZeroCrossingInList);
		//passedThreshold = checkPointZeroCrossingFloatPassesThreshold(x, y, zeroCrossingValueX, zeroCrossingValueY, coefficient, alpha)

	}

	return passedThreshold;
}






/************************************ End Edgise Frame High Level ('EdgiseFrameStandard') Methods* **********************************/





//returns float value of byte at a particular index
float ATORquadraticFitClass::getPixelFloat(const int x, const int y, const double luminosityContrastMap[])
{
	#ifdef EMULATE_EDGISE
	return float((float(luminosityContrastMap[y*globalImageWidth  + x])/float(MAX_LUMINOSITY_CONTRAST))*128.0);
	#else
	return float(luminosityContrastMap[y*globalImageWidth  + x]);
	#endif
}






/*********************************** Fit Methods* ******************************/



//determines coefficients of some x, y coordinates of a frame which is being fitted
void ATORquadraticFitClass::calculateQuadraticFitCoefficients(const int x, const int y, float coefficient[], const double luminosityContrastMap[])
{
	if(QUADRATIC_FIT_KERNEL_SIZE == QUADRATIC_FIT_KERNEL_SIZE_3X3)
	{
		calculateQuadraticFitCoefficients3x3(x, y, coefficient, luminosityContrastMap);
	}
	else if(QUADRATIC_FIT_KERNEL_SIZE == QUADRATIC_FIT_KERNEL_SIZE_5X5)
	{//5x5 kernel
		calculateQuadraticFitCoefficients5x5(x, y, coefficient, luminosityContrastMap);
	}
	else
	{
		cerr << "invalid quadratic fit kernel size, QUADRATIC_FIT_KERNEL_SIZE = " << QUADRATIC_FIT_KERNEL_SIZE << endl;
		exit(EXIT_ERROR);
	}
}



void ATORquadraticFitClass::calculateQuadraticFitCoefficients3x3(const int x, const int y, float coefficient[], const double luminosityContrastMap[])
{
	/*
	@author Dr Friedrich Heitger
	(this was transferred from C to Java)
	the naming of the coefficients a[] has been kept)

	2D second order fit in 3x3 neighbourhood
	f(x,y) = a0 + a1 x + a2 y + a3 x(squared) + a4 y(squared) + a5 xy
	3x3 surround index definition and offsets

	4	3	2		x-1, y+1		x, y+1		x+1, y+1
	5	0	1		x-1, y			x, y		x+1, y
	6	7	8		x-1, y-1		x, y-1		x+1, y-1
	*/

	// float coefficient[6] are coefficients of fit equations around pixel centred x,y


	float sxp, sxn, syp, syn, stot;
	float zeroPointFive = 0.5555555555F;

	sxp = 	getPixelFloat((x+1),(y),luminosityContrastMap) 	+ getPixelFloat((x+1),(y+1),luminosityContrastMap)+ getPixelFloat((x+1),(y-1),luminosityContrastMap);
			//1,2,8
	sxn = 	getPixelFloat((x-1),(y+1),luminosityContrastMap) 	+ getPixelFloat((x-1),(y),luminosityContrastMap) 	+ getPixelFloat((x-1),(y-1),luminosityContrastMap);
			//4,5,6
	syp = 	getPixelFloat((x+1),(y+1),luminosityContrastMap) 	+ getPixelFloat((x),(y+1),luminosityContrastMap) 	+ getPixelFloat((x-1),(y+1),luminosityContrastMap);
			//2,3,4
	syn = 	getPixelFloat((x-1),(y-1),luminosityContrastMap) 	+ getPixelFloat((x),(y-1),luminosityContrastMap) 	+ getPixelFloat((x+1),(y-1),luminosityContrastMap);
			//6,7,8
	stot = 	getPixelFloat((x),(y),luminosityContrastMap) 		+ getPixelFloat((x),(y+1),luminosityContrastMap) 	+ getPixelFloat((x),(y-1),luminosityContrastMap)
			+ sxp + sxn;
			//0,3,7

	coefficient[0] = zeroPointFive* stot - (sxn+sxp+syn+syp)/3.0;
	coefficient[1] = (sxp - sxn)/6.0;
	coefficient[2] = (syp - syn)/6.0;
	coefficient[3] = (sxp + sxn)/2.0 - stot/3.0;
	coefficient[4] = (syp + syn)/2.0 - stot/3.0;
	coefficient[5] = (	getPixelFloat((x+1),	(y+1),luminosityContrastMap) + getPixelFloat((x-1),	(y-1),luminosityContrastMap)			//2,6
					-	getPixelFloat((x-1),	(y+1),luminosityContrastMap) - getPixelFloat((x+1),	(y-1),luminosityContrastMap))/4.0;		//4,8

}

void ATORquadraticFitClass::calculateQuadraticFitCoefficients5x5(const int x, const int y, float coefficient[], const double luminosityContrastMap[])
{
	/*
	@author Dr Friedrich Heitger
	(this was transferred from C to Java)
	the naming of the coefficients a[] has been kept)
	*/


	/*--------------------------------------------------------------------------*/
	/* 2D second order fit in 5x5 neighbourhood                                */
	/* f(x,y) = a0 + a1 x* a2 y + a3 x^2 + a4 y^2 + a5 xy                     */
	/* 5x5 surround index definition and offsets                               */
	/* -------------------------                                              */
	/* | 15 | 14 | 13 | 12 | 11 |                                              */
	/* | 16 |  4 |  3 |  2 | 10 |                                              */
	/* | 17 |  5 |  0 |  1 |  9 |          Matrix Indices                      */
	/* | 18 |  6 |  7 |  8 | 24 |                                              */
	/* | 19 | 20 | 21 | 22 | 23 |                                              */
	/* -------------------------                                               */
	/* Constants:                                                              */
	/* 27/175 = 0.1542857143                                                   */
	/* 1/35   = 0.0285714286                                                   */
	/* 1/50   = 0.02                                                           */
	/* 1/70   = 0.0142857143                                                   */


	// float coefficient[6] are coefficients of fit equations around pixel centred x,y


	float sx0, sxp1, sxn1, syp1, syn1,  sxp2, sxn2, syp2, syn2, stot;


	sx0  = getPixelFloat(x   ,  y   ,luminosityContrastMap)	+getPixelFloat(x   , (y+1),luminosityContrastMap)+getPixelFloat(x   , (y-1),luminosityContrastMap) + /* 0,3,7  */
					getPixelFloat(x   , (y+2),luminosityContrastMap)+getPixelFloat(x   , (y-2),luminosityContrastMap);                    /* 13,21  */

	sxp1 = getPixelFloat(x+1 ,  y   ,luminosityContrastMap)+getPixelFloat(x+1 , (y+1),luminosityContrastMap)+getPixelFloat(x+1 , (y-1),luminosityContrastMap) + /* 1,2,8  */
					getPixelFloat(x+1 , (y+2),luminosityContrastMap)+getPixelFloat(x+1 , (y-2), luminosityContrastMap);                    /* 12,22  */
	sxn1 = getPixelFloat(x-1 , (y+1),luminosityContrastMap)+getPixelFloat(x-1 ,  y   ,luminosityContrastMap)+getPixelFloat(x-1 , (y-1),luminosityContrastMap) + /* 4,5,6  */
					getPixelFloat(x-1 , (y+2),luminosityContrastMap)+getPixelFloat(x-1 , (y-2),luminosityContrastMap);                    /* 14,20  */
	sxp2 = getPixelFloat(x+2 , (y+2),luminosityContrastMap)+getPixelFloat(x+2 , (y+1),luminosityContrastMap)+getPixelFloat(x+2 ,  y   ,luminosityContrastMap) + /* 11,10,9*/
			 		getPixelFloat(x+2 , (y-1),luminosityContrastMap)+getPixelFloat(x+2 , (y-2),luminosityContrastMap);                    /* 24,23  */
	sxn2 = getPixelFloat(x-2 , (y+2),luminosityContrastMap)+getPixelFloat(x-2 , (y+1),luminosityContrastMap)+getPixelFloat(x-2 ,  y   ,luminosityContrastMap) + /* 15,16,17*/
			 		getPixelFloat(x-2 , (y-1),luminosityContrastMap)+getPixelFloat(x-2 , (y-2),luminosityContrastMap);                    /* 18,19  */

	syp1 = getPixelFloat(x-1 , (y+1),luminosityContrastMap)+getPixelFloat(x   , (y+1),luminosityContrastMap)+getPixelFloat(x+1 , (y+1),luminosityContrastMap) + /* 4,3,2  */
			 		getPixelFloat(x-2 , (y+1),luminosityContrastMap)+getPixelFloat(x+2 , (y+1),luminosityContrastMap);                    /* 16,10  */
	syn1 = getPixelFloat(x-1 , (y-1),luminosityContrastMap)+getPixelFloat(x   , (y-1),luminosityContrastMap)+getPixelFloat(x+1 , (y-1),luminosityContrastMap) + /* 6,7,8  */
			 		getPixelFloat(x-2 , (y-1),luminosityContrastMap)+getPixelFloat(x+2 , (y-1),luminosityContrastMap);                    /* 18,24  */
	syp2 = getPixelFloat(x-2 , (y+2),luminosityContrastMap)+getPixelFloat(x-1 , (y+2),luminosityContrastMap)+getPixelFloat(x   , (y+2),luminosityContrastMap) + /* 15,14,13*/
			 		getPixelFloat(x+1 , (y+2),luminosityContrastMap)+getPixelFloat(x+2 , (y+2),luminosityContrastMap);                    /* 12,11  */
	syn2 = getPixelFloat(x-2 , (y-2),luminosityContrastMap)+getPixelFloat(x-1 , (y-2),luminosityContrastMap)+getPixelFloat(x   , (y-2),luminosityContrastMap) + /* 19,20,21*/
			 		getPixelFloat(x+1 , (y-2),luminosityContrastMap)+getPixelFloat(x+2 , (y-2),luminosityContrastMap);                    /* 22,23  */

	stot = sx0 + sxp1 + sxp2 + sxn1 + sxn2;

	coefficient[0] =  0.1542857143* stot - 0.0285714286* (sxn1+sxp1+syn1+syp1 + 4.0*(sxn2+sxp2+syn2+syp2));
	coefficient[1] =  0.02* (sxp1 - sxn1 + 2.0*(sxp2 - sxn2));
	coefficient[2] =  0.02* (syp1 - syn1 + 2.0*(syp2 - syn2));
	coefficient[3] =  0.0142857143* (sxp1 + sxn1 + 4.0*(sxp2 + sxn2)) - 0.0285714286* stot;
	coefficient[4] =  0.0142857143* (syp1 + syn1 + 4.0*(syp2 + syn2)) - 0.0285714286* stot;
	coefficient[5] = 0.01* (4.0* (getPixelFloat(x-2 , (y-2),luminosityContrastMap) + getPixelFloat(x+2 , (y+2),luminosityContrastMap) - 		/* 19,11*/
						getPixelFloat(x-2 , (y+2),luminosityContrastMap) - getPixelFloat(x+2 , (y-2),luminosityContrastMap)) + 			/* 15,23*/
						2.0* (getPixelFloat(x-2 , (y-1),luminosityContrastMap) + getPixelFloat(x-1 , (y-2),luminosityContrastMap) + 	/* 18,20*/
						getPixelFloat(x+1 , (y+2),luminosityContrastMap) + getPixelFloat(x+2 , (y+1),luminosityContrastMap) -  			/* 12,10*/
						getPixelFloat(x-1 , (y+2),luminosityContrastMap) - getPixelFloat(x-2 , (y+1),luminosityContrastMap) -  			/* 14,16*/
						getPixelFloat(x+1 , (y-2),luminosityContrastMap) - getPixelFloat(x+2 , (y-1),luminosityContrastMap)) + 			/* 22,24*/
						getPixelFloat(x+1 , (y+1),luminosityContrastMap) + getPixelFloat(x-1 , (y-1),luminosityContrastMap) -  			/* 2,6*/
						getPixelFloat(x-1 , (y+1),luminosityContrastMap) - getPixelFloat(x+1 , (y-1),luminosityContrastMap));  			/* 4,8*/

}

/*********************************** End Fit Methods* ******************************/






/********************************** Pixel Checking methods* ******************************/





bool ATORquadraticFitClass::checkForTotalPositiveCurvature(const float a3, const float a4)
{
	if((a3 > 0) && (a4 > 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ATORquadraticFitClass::checkForTotalNegativeCurvature(const float a3, const float a4)
{
	if((a3 < 0) && (a4 < 0))
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}


bool ATORquadraticFitClass::checkTotalNegativeCurvatureAbovePointThreshold(const float a3, const float a4)
{
	if((a3 < A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD) && (a4 < A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD))
	{
		return true;
	}
	else
	{
		return false;
	}
}






/********************************** End Pixel Checking methods* ******************************/












/********************************** Zero Crossing Checking methods* **********************************/


/* 2 kinds of methods are provided. Part A for Object Passing, Part B other for non-object passing*/



/********************************** Part A (object passing)* ******************************/









bool ATORquadraticFitClass::checkPointZeroCrossingObjectPassesThreshold(const ATORQFzeroCrossing* zc)
{
	bool passedThreshold;

	//check zeroCrossing is within position threshold
	//[= 0.5 . that is, is the zeroCrossing within the current pixel?]
	if((SHAREDvars.absDouble(zc->zeroCrossingValueX) <= ZERO_CROSSING_POSITION_THRESHOLD)
	&& (SHAREDvars.absDouble(zc->zeroCrossingValueY) <= ZERO_CROSSING_POSITION_THRESHOLD))
	{
		//check the zeroCrossing is not undefined (not a number / infinite)
		if((SHAREDvars.absDouble(zc->zeroCrossingValueX) >= 0.0)
		&& (SHAREDvars.absDouble(zc->zeroCrossingValueY) >= 0.0))
		/*
		if((SHAREDvars.absDouble(zc->zeroCrossingValueX) < MAX_ZERO_CROSSING_NOT_NAN_OR_INFINITE)
		&& (SHAREDvars.absDouble(zc->zeroCrossingValueY) > MIN_ZERO_CROSSING_NOT_NAN_OR_INFINITE))
		*/
		{

			if(checkTotalNegativeCurvatureAbovePointThreshold(zc->coefficient[3], zc->coefficient[4]))
			{

				passedThreshold = true;

			}
			else
			{	//error which creates a zeroCrossing off the edge by a factor of 1 and
				//a corresponding zeroCrossingLine perpendicular to Edge this error
				//happens only very rarely but can significantly effect results
				passedThreshold = false;

			}

		}
		else
		{
			//thresholdValues are either NaN or Infinity
			passedThreshold = false;
		}
	}
	else
	{
		passedThreshold = false;
	}


	return passedThreshold;
}









/***
 * FOR LOW EFFICIENCY ZEROCROSSING CALCULATIONS (with passing of zeroCrossing object)
 *
 * checkEdgeZeroCrossingObjectPassesThreshold(ATORQFzeroCrossing* zc)
 */

bool ATORquadraticFitClass::checkEdgeZeroCrossingObjectPassesThreshold(ATORQFzeroCrossing* zc)
{
	bool passedThreshold;

	//check zeroCrossing is within position threshold
	//[= 0.5 . that is, is the zeroCrossing within the current pixel?]
	if((SHAREDvars.absDouble(zc->zeroCrossingValueX) <= ZERO_CROSSING_POSITION_THRESHOLD)
	&& (SHAREDvars.absDouble(zc->zeroCrossingValueY) <= ZERO_CROSSING_POSITION_THRESHOLD))
	{
		#ifndef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ
		//check the zeroCrossing is not undefined (not a number / infinite)
		if((SHAREDvars.absDouble(zc->zeroCrossingValueX) >= 0.0)
		&& (SHAREDvars.absDouble(zc->zeroCrossingValueY) >= 0.0))
		{
		#endif
			//check orientation and magnitude of gradient of contrast

			#ifndef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ
			if(checkEdgeZeroCrossingObjectContrastGradients(zc))
			{
			#endif

				passedThreshold = true;

			#ifndef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ
			}
			else
			{	//error which creates a zeroCrossing off the edge by a factor of 1 and
				//a corresponding zeroCrossingLine perpendicular to Edge this error
				//happens only very rarely but can significantly effect results
				passedThreshold = false;

			}
			#endif

		#ifndef ATOR_QUADRATIC_FIT_EDGE_RELAXED_REQ
		}
		else
		{
			//thresholdValues are either NaN or Infinity
			passedThreshold = false;
		}
		#endif
	}
	else
	{
		passedThreshold = false;
	}

	return passedThreshold;
}



/***
 * FOR LOW EFFICIENCY ZEROCROSSING CALCULATIONS (with passing of zeroCrossing object)
 *
 * checkEdgeZeroCrossingObjectContrastGradients(ATORQFzeroCrossing* zc)
 */

bool ATORquadraticFitClass::checkEdgeZeroCrossingObjectContrastGradients(ATORQFzeroCrossing* zc)
{

	/********************THEORY***********************************************************

		[related to THEORY for calculateEdgeZeroCrossing()]

			d(z)/d(orientation) = d(z)/d(x)*cos(orientation) + d(z)/d(y)*sin(orientation)

		so,

			d(z)/d(alpha) = d(z)/d(x)*cos(alpha) + d(z)/d(y)*sin(alpha)


	METHOD 1	(currently implemented method)

			Must solve for d(z)/d(alpha)

		If d(z)/d(alpha) is large, then the zerocrossing has been calculated nearby
		most likely due to a noisy distribution along the axis of the orientation of
		the curve. (while on a steep edge of the curve)

	METHOD 2 	(this method was implemented for [straight] lines)

			Must solve for d^2(z)/d(alpha)^2 , d^2(z)/d(beta)^2

			if (d^2(z)/d(alpha)^2) / (d^2(z)/d(beta)^2) is small, then (d^2(z)/d(alpha)^2)
			is small

			therefore curvature in alpha direction is small

			therefore there is a constant gradient in alpha direction

			therefore the zero crossing lies on the edge of a [straight] line.

			[method 2 does not take into account the slope of the line at the zerocrossing]

	 ////////////////////////NOTES/////////////////////////////////////////////////////////
	 //
	 // [related to NOTES for calculateEdgeZeroCrossing(). Where a[] = coefficient[]]
	 //
	 // METHOD 1
	 //
	 // f(x,y) = z = a[0] + a[1]*x + a[2]*y + a[3]*x(squared) + a[4]*y(squared) + a[5]*xy
	 //
	 // d(z)/d(x) = a[1] + 2*a[3]*x + a[5]*y
	 // d(z)/d(y) = a[2] + 2*a[4]*y + a[5]*x
	 //
	 // d(z)/d(alpha) = (a[1] + 2*a[3]*x + a[5]*y)*cos(alpha)
	 //					+ (a[2] + 2*a[4]*y + a[5]*x)*sin(alpha)
	 //
	 //
	 //	float dzdalpha = (a[1] 	+ 2*a[3]*zeroCrossingValueX
	 //								+ a[5]*zeroCrossingValueY*(float)(cos(alpha))
	 //
	 //						+ (a[2] + 2*a[4]*zeroCrossingValueY
	 //								+ a[5]*zeroCrossingValueX*(float)(sin(alpha));
	 //
	 //	if(SHAREDvars.absDouble(dzdalpha) <= MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE)
	 //	{
	 //		pass
	 //	}
	 //	else
	 //	{
	 //		fail
	 //	}
	 //
	 // METHOD 2 (has problems...)
	 //
	 // dz/dx^2 = a[3], dz/dy^2 = a[4], dz/(dydx) = a[5]/2
	 //
	 //	d/d(orientation)(dz/d(orientation))
	 //
	 //		=	d/dx{(dz/dx)*cos(orientation) + (dz/dy)*sin(orientation)}*cos(orientation)
	 //		+	d/dy{(dz/dx)*cos(orientation) + (dz/dy)*sin(orientation)}*sin(orientation)
	 //
	 //
	 //		=	d/dx{(dz/dx)*cos(orientation)			+ (dz/dy)*sin(orientation)}
	 //																*cos(orientation)
	 //		+	d/dy{(dz/dx)*cos(orientation) 			+ (dz/dy)*sin(orientation)}
	 //																*sin(orientation)
	 //
	 //		= 	a[3]*cos^2(orientation) 				+ a[5]/2*sin(orientation)*cos(orientation)
	 //
	 //		+	a[5]/2*cos(orientation)*sin(orientation)+ a[4]*sin^2(orientation)
	 //
	 //	if (d^2(z)/d(beta)^2) / (d^2(z)/d(alpha)^2) is large, then (d^2(z)/d(alpha)^2) is small.
	 //
	 // therefore dz/d(alpha) is linear
	 //
	 // float dzTwodAlpha 	= 		a[3]*cos^2(alpha) 			+ a[5]/2*cos(alpha)*sin(alpha) +
	 //								a[5]/2*cos(alpha)*sin(alpha)+ a[4]*sin^2(alpha);
	 // float dzTwodBeta	= 		a[3]*cos^2(beta) 			+ a[5]/2*cos(beta)*sin(beta) +
	 //								a[5]/2*cos(beta)*sin(beta)	+ a[4]*sin^2(beta);
	 //
	 // if(((dzTwodAlpha) / (dzTwodBeta)) <= 0.5)
	 // {
	 //		pass
	 // }
	 // else
	 // {
	 //		fail
	 // }
	 //
	 // METHOD 3
	 //
	 // checks that dz2dbeta is not positive.
	 //
	 //	if(dzTwodBeta < 0)
	 //	{
	 //		pass
	 //	}
	 //	else
	 //	{
	 //		fail
	 //	}
	 //
	 ///////////////////////////////////////////////////////////////////////////////////
 *
 *******************************************************************************/


	float alpha = zc->alpha;
	float coefficient[6];
	coefficient[0] = zc->coefficient[0];
	coefficient[1] = zc->coefficient[1];
	coefficient[2] = zc->coefficient[2];
	coefficient[3] = zc->coefficient[3];
	coefficient[4] = zc->coefficient[4];
	coefficient[5] = zc->coefficient[5];
	int x = zc->x;
	int y = zc->y;
	float zeroCrossingValueX = zc->zeroCrossingValueX;
	float zeroCrossingValueY = zc->zeroCrossingValueY;

	float beta = alpha + (float)PI/2.0;

		//at the zero crossing point...
	float dzdalpha = (coefficient[1] + 2.0*coefficient[3]*zeroCrossingValueX + coefficient[5]*zeroCrossingValueY)
					*(float)(cos(alpha))
					+ (coefficient[2] + 2.0*coefficient[4]*zeroCrossingValueY + coefficient[5]*zeroCrossingValueX)
					*(float)(sin(alpha));


		//FOR RESEARCH
	float dzdbeta = (coefficient[1] + 2.0*coefficient[3]*zeroCrossingValueX + coefficient[5]*zeroCrossingValueY)
						*(float)(cos(beta))
						+ (coefficient[2] + 2.0*coefficient[4]*zeroCrossingValueY + coefficient[5]*zeroCrossingValueX)
						*(float)(sin(beta));

	float dzTwodAlpha 	= 		coefficient[3]*((float)cos(alpha))*((float)cos(alpha))
							+ coefficient[5]/2.0*((float)cos(alpha))*((float)sin(alpha))
							+ coefficient[5]/2.0*((float)cos(alpha))*((float)sin(alpha))
							+ coefficient[4]*((float)sin(alpha))*((float)sin(alpha));
	float dzTwodBeta	= 		coefficient[3]*((float)cos(beta))*((float)cos(beta))
							+ coefficient[5]/2.0*((float)cos(beta))*((float)sin(beta))
							+ coefficient[5]/2.0*((float)cos(beta))*((float)sin(beta))
							+ coefficient[4]*((float)sin(beta))*((float)sin(beta));

	zc->dzAlpha = dzdalpha;
	zc->dzBeta = dzdbeta;
	zc->dzTwoAlpha = dzTwodAlpha;
	zc->dzTwoBeta = dzTwodBeta;

	//method 1
	if(SHAREDvars.absDouble(dzdalpha) <= MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE)
	{
			//if orientation of zerocrossing fit is perpendicular to a large contrast gradient, then
			//disregard zerocrossing


			//method 3

			//checks that dz2dbeta is not positive. this cannot be done directly via the coefficient[3] and coefficient[4] coefficient.
			if(dzTwodBeta < 0)
			{
				//method 2

				float alphaToBetaGradientRatio = (float)SHAREDvars.absDouble(dzTwodAlpha) / (float)SHAREDvars.absDouble(dzTwodBeta);

				float betaToAlphaGradientRatio =  (float)SHAREDvars.absDouble(dzTwodBeta) / (float)SHAREDvars.absDouble(dzTwodAlpha);
					//not currently required.

				if(alphaToBetaGradientRatio <= MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO)
				{
					//zc.setFilled();

					//confidence = (|max(c)| - |min(c)|) / (|max(c)| + |min(c)|)
					zc->confidence = ((SHAREDvars.absDouble(dzTwodBeta) - SHAREDvars.absDouble(dzTwodAlpha)) / (SHAREDvars.absDouble(dzTwodBeta) + SHAREDvars.absDouble(dzTwodAlpha)));
					//zc.setConfidence((SHAREDvars.absDouble(dzTwodAlpha)) / (SHAREDvars.absDouble(dzTwodBeta)));


					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
	}
	else
	{
		return false;
	}
}







/********************************** Matrix Manipulation methods* **********************************/


float ATORquadraticFitClass::getSmallestEigenValue(const float eigenValue[])
{
	float minEigenValue;

	if(SHAREDvars.absDouble(eigenValue[1]) > SHAREDvars.absDouble(eigenValue[0]))
	{
		minEigenValue = eigenValue[0];
	}
	else
	{
		minEigenValue = eigenValue[1];
	}

	return minEigenValue;
}



//(this is the new method of discriminating data based on where the pixel
//solution derivative with respect to it's orientation of curvature is zero)
void ATORquadraticFitClass::calculateEdgeZeroCrossing(const float coefficient[], const float beta, float zc[])
{
	/********************THEORY***********************************************************

							y = mx + b												(1)

																(straight line formula)

		so						where, b = y-intercept = 0

								and, m = delta(y) / delta(x) = tan(beta)

		therefore, 			y = tan(beta)x

		now let f(x,y) = z

		by definition (NB these are paritial derivatives),

			d(z)/d(orientation) = d(z)/d(x)*cos(orientation) + d(z)/d(y)*sin(orientation)

				d(z)/d(beta)	= d(z)/d(x)*cos(beta) + d(z)/d(y)*sin(beta) 		(2)

															(area orientation formula)


		but where y = mx, 	d(z)/d(beta) = 0

		so, values for x and y can be solved based on these simultaneous equations
		[(1) and (2)]


	 ////////////////////////NOTES////////////////////////////////////////////////////////
	 //
	 // Where a[] = coefficient[]
	 //
	 // f(x,y) = z = a[0] + a[1]*x + a[2]*y + a[3]*x(squared) + a[4]*y(squared) + a[5]*xy
	 //
	 // d(z)/d(x) = a[1] + 2*a[3]*x + a[5]*y
	 // d(z)/d(y) = a[2] + 2*a[4]*y + a[5]*x
	 //
	 // let y = tan(beta)*x,
	 // d(z)/d(beta) = ((a[1] + 2*a[3]*x + a[5]*tan(beta)*x)*cos(beta))
	 //										+ (a[2] + 2*a[4]*tan(beta)*x + a[5]*x)*sin(beta))
	 //
	 // let d(z)/d(beta) = 0, and solve for x:
	 //
	 // 	0 = cos(beta)*a[1] + x*cos(beta)*(2*a[3] + a[5]*tan(beta)) + a[2]*sin(beta)
	 //												+ x*sin(beta)*(2*a[4]*tan(beta) + a[5])
	 //
	 //		x = -(cos(beta)*a[1] + sin(beta)*a[2]) / (((cos(beta)*(2*a[3] + a[5]*tan(beta)))
	 //												+ (sin(beta)*(2*a[4]*tan(beta) + a[5])))
	 //
	 /////////////////////////////////////////////////////////////////////////////////////
	* ***********************************************************************************/

	//float alpha = beta - ((float)PI)/2.0;

	float x = -((float)cos(beta)*coefficient[1] + (float)sin(beta)*coefficient[2]) /
								(((float)cos(beta)*(2.0*coefficient[3] + coefficient[5]*(float)tan(beta)))
								+ ((float)sin(beta)*(2.0*coefficient[4]*(float)tan(beta) + coefficient[5])));

	float y = (float)tan(beta)*x;

	zc[0] = x;
	zc[1] = y;

}



void ATORquadraticFitClass::calculatePointZeroCrossing(const float coefficient[], float zc[])
{
	/********************THEORY***********************************************************

	 ////////////////////////NOTES////////////////////////////////////////////////////////
	 //
	 // Where a[] = coefficient[]
	 //
	 // f(x,y) = z = a[0] + a[1]*x + a[2]*y + a[3]*x(squared) + a[4]*y(squared) + a[5]*xy
	 //
	 // d(z)/d(x) = a[1] + 2*a[3]*x + a[5]*y
	 // d(z)/d(y) = a[2] + 2*a[4]*y + a[5]*x
	 //
	 // let d(z)/d(x) = 0
	 //             0 = a[1] + 2*a[3]*x + a[5]*y
	 //      2*a[3]*x = -a[1] - a[5]*y
	 //             x = -a[1]/(2*a[3]) - (a[5]*y)/(2*a[3])	(1)
	 //
	 // let d(z)/d(y) = 0
	 //             0 = a[2] + 2*a[4]*y + a[5]*x
	 //        a[5]*x = -a[2] - 2*a[4]*y
	 //             x =  -a[2]/a[5] - (2*a[4]*y)/a[5]	(2)
	 //
	 //	(2)-(1)
	 //
	 //             0 =  -a[2]/a[5] - a[1]/(2*a[3]) - (2*a[4]*y)/a[5] + (a[5]*y)/(2*a[3])
	 //             y =  (a[2]/a[5] + a[1]/(2*a[3])) / -((2*a[4])/a[5] + a[5]/(2*a[3]))
	 //
	 //             x = -a[1]/(2*a[3]) - (a[5]*y)/(2*a[3])
	 //
	 /////////////////////////////////////////////////////////////////////////////////////
	* ***********************************************************************************/

	float y = (coefficient[2]/coefficient[5] + coefficient[1]/(2.0*coefficient[3])) / (-((2.0*coefficient[4])/coefficient[5] + coefficient[5]/(2.0*coefficient[3])));
	float x = (-coefficient[1])/(2.0*coefficient[3]) - (coefficient[5]*y)/(2.0*coefficient[3]);

	zc[0] = x;
	zc[1] = y;
}





//This method is specifically for Edgise Calculations

	//NB this method only calculates one eigenvector as it accepts only one eigenvalue
void ATORquadraticFitClass::calculateEigenVector(const float coefficient[], const float ev, float eigenVector[])
{
	/*********************THEORY**********************************************************

		[related to THEORY for calculateEigenValues()]

							A-(I*Lambda) 	= 	a-Lamba	b
												c		d-Lambda

		Now by definition,

							A*eigenVector	=	Lambda*eigenVector

			[A-(I*Lambda)]* eigenVector	= 	0

		therefore;

				a-Lamba	b			* 	x1	= 	0
				c		d-Lambda		x2

	//////////////////////NOTES///////////////////////////////////////////////////////////
	//
	// Where a[] = coefficient[]
	//
	// a[3] - ev	a[5]/2		* 	x1				= 0
	// a[5]/2		a[4] - ev		x2
	//
	// a[3] - ev	* x1 	+ 	a[5]/2 		* x2 	= 0
	// a[5]/2 		* x1 	+ 	a[4] - ev	* x2 	= 0
	//
	// (the above are two duplicate equations)
	//
	// let x1 = 1
	//
	// then from equation 1, 					x2 	= (0 - ((a[3] - ev)* 1)) / (a[5]/2)
	//
	//////////////////////////////////////////////////////////////////////////////////////
	*************************************************************************************/

	float x1 = 1.0;
	float x2 = -(coefficient[3] - ev) / (coefficient[5]/2);

	eigenVector[0] = x1;
	eigenVector[1] = x2;

}





//This method is specifically for Edgise Calculations
void ATORquadraticFitClass::calculateEigenValues(const float coefficient[], float ev[])
{


	/*********************THEORY**********************************************************

		In a Matrix, A, the eigen values can be found by first subtracting A
		by it�s identity matrix multiplied by Lambda (Q)

		where A			= 	a	b				and I 		=	1	0
							c	d								0	1

		I*Lambda		=	1	0* Lambda 	=	Lambda 	 0
							0	1				0  		 Lambda

		A-(I*Lambda) 	= 	a-Lamba	b
							c		d-Lambda

		Now taking the determinant of A-(I*Lambda) and setting it to zero, we can solve for
		Lambda, A`s eigen value(s).

	//////////////////////NOTES///////////////////////////////////////////////////////////
	//
	// Eg where  f(x,y) = a[0] + a[1]*x + a[2]*y + a[3]*x(squared) + a[4]*y(squared) + a[5]*xy
	//
	// or     	 f(x,y) = a[0] + transformed(a[1])*Vector - transformed(Vector)*(a[3]   a[5]/2)*Vector
	//		                               	 a[2]                                a[5]/2 a[4]
	//
	// if    Q =  a[3]	 - ev		a[5]/2
	//            a[5]/2			a[4] - ev
	//
	// then  det(Q)   =   	(a[3] - ev)(a[4] - ev) - (a[5]/2)(sqared).
	//                =   	ev(squared)   -  ( a[3] + a[4])*ev     + a[3]*a[4] - (a[5]/2)(squared)
	//				  =   	ev(squared)   +  (-a[3] - a[4])*ev     + a[3]*a[4] - (a[5]/2)(squared)
	//                = (a)*ev(squared)   +             (b)*ev     + (c)
	//
	// set det(Q) = 0.
	//
	// Therefore,  ev = [-b +/- squareroot(b(squared) - 4*a*c)] / 2*a
	//
	// 								where a = 1, b = -a[3] -a[4], c = a[3]*a[4] - (a[5]/2)(squared)
	//
	//////////////////////////////////////////////////////////////////////////////////////
	*************************************************************************************/

	//calc eigen values, ev[] (mathematical theory explained above)

	float coeffA = 1;
	float coeffB = -coefficient[3]  -coefficient[4];
	float coeffC =  coefficient[3]* coefficient[4] - (coefficient[5]/2.0)* (coefficient[5]/2.0);

	ev[0]   = (-coeffB + (float)sqrt(coeffB*coeffB - 4.0*coeffA*coeffC)) / (2.0*coeffA);
	ev[1]   = (-coeffB - (float)sqrt(coeffB*coeffB - 4.0*coeffA*coeffC)) / (2.0*coeffA);

}

/********************************** End Matrix Manipulation methods* **********************************/

































