 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORfeatureGeneration.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * Requirements: Assumes that depth information is less accurate than image information
 * /
 *******************************************************************************/


#include "ATORfeatureGeneration.hpp"

#define MASK_3X3_PIXEL_1 (1)	//row 1, col 1
#define MASK_3X3_PIXEL_1_X_REL (-1)
#define MASK_3X3_PIXEL_1_Y_REL (-1)
#define MASK_3X3_PIXEL_2 (2)	//row 1, col 2
#define MASK_3X3_PIXEL_1_X_REL (0)
#define MASK_3X3_PIXEL_1_Y_REL (-1)
#define MASK_3X3_PIXEL_3 (3)	//row 1, col 3
#define MASK_3X3_PIXEL_1_X_REL (1)
#define MASK_3X3_PIXEL_1_Y_REL (-1)
#define MASK_3X3_PIXEL_4 (4)	//row 2, col 1
#define MASK_3X3_PIXEL_1_X_REL (-1)
#define MASK_3X3_PIXEL_1_Y_REL (0)
#define MASK_3X3_PIXEL_5 (5)	//row 2, col 2
#define MASK_3X3_PIXEL_1_X_REL (0)
#define MASK_3X3_PIXEL_1_Y_REL (0)
#define MASK_3X3_PIXEL_6 (6)	//row 2, col 3
#define MASK_3X3_PIXEL_1_X_REL (1)
#define MASK_3X3_PIXEL_1_Y_REL (0)
#define MASK_3X3_PIXEL_7 (7)	//row 3, col 1
#define MASK_3X3_PIXEL_1_X_REL (-1)
#define MASK_3X3_PIXEL_1_Y_REL (1)
#define MASK_3X3_PIXEL_8 (8)	//row 3, col 2
#define MASK_3X3_PIXEL_1_X_REL (0)
#define MASK_3X3_PIXEL_1_Y_REL (1)
#define MASK_3X3_PIXEL_9 (9)	//row 3, col 3
#define MASK_3X3_PIXEL_1_X_REL (1)
#define MASK_3X3_PIXEL_1_Y_REL (1)


#define MASK_3X3_ARRAY_1_LOW_CONTRAST_SIZE (5)
#define MASK_3X3_ARRAY_1_HIGH_CONTRAST_SIZE (5)
#define MASK_3X3_ARRAY_2_LOW_CONTRAST_SIZE (5)
#define MASK_3X3_ARRAY_2_HIGH_CONTRAST_SIZE (5)
#define MASK_3X3_ARRAY_3_LOW_CONTRAST_SIZE (3)
#define MASK_3X3_ARRAY_3_HIGH_CONTRAST_SIZE (7)
#define MASK_3X3_ARRAY_4_LOW_CONTRAST_SIZE (2)
#define MASK_3X3_ARRAY_4_HIGH_CONTRAST_SIZE (7)
#define MASK_3X3_ARRAY_5_LOW_CONTRAST_SIZE (2)
#define MASK_3X3_ARRAY_5_HIGH_CONTRAST_SIZE (7)
#define MASK_3X3_ARRAY_6_LOW_CONTRAST_SIZE (4)
#define MASK_3X3_ARRAY_6_HIGH_CONTRAST_SIZE (5)




#ifdef TH_ATOR_USE_SHIELD_LDR_FILE
	#define COLOUR_HUE_CONTRAST_RADIAL_SUM_THRESHOLD (135.0) //(135.0) //135.0
	#define COLOUR_HUE_CONTRAST_BASIC_SUM_THRESHOLD (50.0)	//50.0
#elif defined ATOR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA
	#define COLOUR_HUE_CONTRAST_RADIAL_SUM_THRESHOLD (75.0)
	#define COLOUR_HUE_CONTRAST_BASIC_SUM_THRESHOLD (30.0)	//50.0
#else
	#define COLOUR_HUE_CONTRAST_RADIAL_SUM_THRESHOLD (100.0)
	#define COLOUR_HUE_CONTRAST_BASIC_SUM_THRESHOLD (50.0)	//50.0
#endif


#define COLOUR_HUE_CONTRAST_RADIAL_SUM_RECONCILIATION_KERNEL_HEIGHT (3)
#define COLOUR_HUE_CONTRAST_RADIAL_SUM_RECONCILIATION_KERNEL_WIDTH (3)



#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
uchar* contiguousRegionDebugrgbMap;
int contiguousRegionNumber = 0;
int tempimageWidth;
int tempimageHeight;
#endif


void ATORfeatureGenerationClass::generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit(ATORfeature* firstFeatureInFeatureList, uchar* featureRgbMap, int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const bool interpixelRGBmapType)
{
	double pixelXOffset;
	double pixelYOffset;
	int xBoundaryMax;
	int xBoundaryMin;
	int yBoundaryMax;
	int yBoundaryMin;
	if(interpixelRGBmapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
	{
		//without ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		xBoundaryMax = imageWidth-1;
		xBoundaryMin = 0;
		yBoundaryMax = imageHeight-1;
		yBoundaryMin = 0;
	}
	else if(interpixelRGBmapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
	{
		//such as luminosityContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0.5;
		pixelYOffset = 0.5;
		xBoundaryMax = imageWidth-2;
		xBoundaryMin = 0;
		yBoundaryMax = imageHeight-2;
		yBoundaryMin = 0;
	}
	else if(interpixelRGBmapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
	{
		//such as gradientContrast or pointNormalContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		xBoundaryMax = imageWidth-2;
		xBoundaryMin = 1;
		yBoundaryMax = imageHeight-2;
		yBoundaryMin = 1;
	}

	double* luminosityMap = new double[imageWidth* imageHeight];
	RTpixelMaps.createLuminosityMapFromRGBMap(imageWidth, imageHeight, featureRgbMap, luminosityMap);


	ATORQFzeroCrossing* firstZeroCrossingInList = new ATORQFzeroCrossing();
	ATORquadraticFit.generateZeroCrossingList(luminosityMap, imageWidth, imageHeight, firstZeroCrossingInList, POINT_DETECT, sensitivity, dimension, pointMap, depthMap, zoom, interpixelRGBmapType);	//need to check that an interpixel rgbmap will not cause issues here - I dont think so considering are taking the lowest depth map value within the 3x3 kernel anyway


	//add zerocrossing data to feature list
	ATORfeature* currentFeatureInList = firstFeatureInFeatureList;

	ATORQFzeroCrossing* currentZeroCrossingInList = firstZeroCrossingInList;
	while(currentZeroCrossingInList->next != NULL)
	{
		if((currentZeroCrossingInList->x >= xBoundaryMin) && (currentZeroCrossingInList->x <= xBoundaryMax) && (currentZeroCrossingInList->y >= yBoundaryMin) && (currentZeroCrossingInList->y <= yBoundaryMax))
		{
			double xPos = (pixelXOffset + double(currentZeroCrossingInList->x) + currentZeroCrossingInList->zeroCrossingValueX)*zoom;
			double yPos = (pixelYOffset + double(currentZeroCrossingInList->y) + currentZeroCrossingInList->zeroCrossingValueY)*zoom;

			currentFeatureInList->xViewport = int(xPos);
			currentFeatureInList->yViewport = int(yPos);

			if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
			{
				currentFeatureInList->pointNonWorldCoord.x = xPos;
				currentFeatureInList->pointNonWorldCoord.y = yPos;

				#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
				double depth = currentZeroCrossingInList->depth;
				#else
				double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(int(xPos), int(yPos), imageWidth, depthMap);
				#endif
				currentFeatureInList->pointNonWorldCoord.z = depth;

				#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
					vec xyzWorld;

					RTscene.calculatePointMapValue(xPos, yPos, depth, &xyzWorld, vi);

					currentFeatureInList->point.x = xyzWorld.x;
					currentFeatureInList->point.y = xyzWorld.y;
					currentFeatureInList->point.z = xyzWorld.z;
				#else
					cout << "error: generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit{} requires ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH when compiling with 3DOD/use3DODonObjectForegroundDepthCheck" << endl;
					vec xyzWorld;
					RTpixelMaps.getPointMapValue(int(xPos), int(yPos), imageWidth*zoom, pointMap, &xyzWorld);
					currentFeatureInList->point.x = xyzWorld.x;
					currentFeatureInList->point.y = xyzWorld.y;
					currentFeatureInList->point.z = xyzWorld.z;


				#endif
			}
			else
			{
				currentFeatureInList->point.x = xPos;
				currentFeatureInList->point.y = yPos;
			}

			ATORfeature* newFeature = new ATORfeature();
			currentFeatureInList->next = newFeature;
			currentFeatureInList = currentFeatureInList->next;
		}

		currentZeroCrossingInList=currentZeroCrossingInList->next;
	}

	delete luminosityMap;
	delete firstZeroCrossingInList;
}



void ATORfeatureGenerationClass::generateEdgeListFromRGBmapWithQuadraticFit(uchar* rgbMap, bool* edgeBoolMap, ATORQFzeroCrossing* edgeZeroCrossingMap[], int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const int interpixelContrastMapType)
{
	double* luminosityMap = new double[imageWidth* imageHeight];
	double* luminosityContrastMap = new double[imageWidth* imageHeight];

	RTpixelMaps.createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMap, luminosityMap);
	RTpixelMaps.createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);

	generateEdgeListFromContrastMapWithQuadraticFit(luminosityContrastMap, edgeBoolMap, edgeZeroCrossingMap, imageWidth, imageHeight, sensitivity, dimension, pointMap, depthMap, zoom, vi, interpixelContrastMapType);

	delete luminosityMap;
	delete luminosityContrastMap;
}

//currently assumes zoom = 1
void ATORfeatureGenerationClass::generateEdgeListFromContrastMapWithQuadraticFit(const double* contrastMap, bool* edgeBoolMap, ATORQFzeroCrossing* edgeZeroCrossingMap[], int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const int interpixelContrastMapType)
{
	double pixelXOffset;
	double pixelYOffset;
	if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
	{
		//without ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
	{
		//such as luminosityContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0.5;
		pixelYOffset = 0.5;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
	{
		//such as gradientContrast or pointNormalContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
	}


	ATORQFzeroCrossing* firstZeroCrossingInList = new ATORQFzeroCrossing();
	ATORquadraticFit.generateZeroCrossingList(contrastMap, imageWidth, imageHeight, firstZeroCrossingInList, EDGE_DETECT, sensitivity, dimension, pointMap, depthMap, zoom, interpixelContrastMapType);		//need to check that an interpixel rgbmap will not cause issues here - I dont think so considering are taking the lowest depth map value within the 3x3 kernel anyway

	 for(int y=0; y < imageHeight; y++)
	 {
		 for(int x=0; x < imageWidth; x++)
		 {
			edgeBoolMap[y*imageWidth + x] = false;
		 }
	 }

	//add zerocrossing data to edge map/list

	ATORQFzeroCrossing* currentZeroCrossingInList = firstZeroCrossingInList;
	while(currentZeroCrossingInList->next != NULL)
	{
		//check zoom
		int x = currentZeroCrossingInList->x;
		int y = currentZeroCrossingInList->y;
		edgeBoolMap[y*imageWidth + x] = true;
		edgeZeroCrossingMap[y*imageWidth + x] = currentZeroCrossingInList;

		if(ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
		{
			if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
			{
				double xPos = (pixelXOffset + double(currentZeroCrossingInList->x) + currentZeroCrossingInList->zeroCrossingValueX)*zoom;
				double yPos = (pixelYOffset + double(currentZeroCrossingInList->y) + currentZeroCrossingInList->zeroCrossingValueY)*zoom;

				#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
				double depth = currentZeroCrossingInList->depth;
				#else
				double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(int(xPos), int(yPos), imageWidth, depthMap);
				#endif

				#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
				vec xyzWorld;
				RTscene.calculatePointMapValue(xPos, yPos, depth, &xyzWorld, vi);
				currentZeroCrossingInList->point.x = xyzWorld.x;
				currentZeroCrossingInList->point.y = xyzWorld.y;
				currentZeroCrossingInList->point.z = xyzWorld.z;
				#else
				cout << "error: generateEdgeListFromContrastMapWithQuadraticFit{} requires ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH when compiling with 3DOD/use3DODonObjectForegroundDepthCheck" << endl;
				vec xyzWorld;
				RTpixelMaps.getPointMapValue(int(yPos), int(yPos), imageWidth*zoom, pointMap, &xyzWorld);
				currentZeroCrossingInList->point.x = xyzWorld.x;
				currentZeroCrossingInList->point.y = xyzWorld.y;
				currentZeroCrossingInList->point.z = xyzWorld.z;
				#endif
			}
		}

		currentZeroCrossingInList=currentZeroCrossingInList->next;
	}

}




#define DISTANCE_TO_CENTEROID_UNCHANGING (0)
#define DISTANCE_TO_CENTEROID_DECREASING (1)
#define DISTANCE_TO_CENTEROID_INCREASING (2)

ATORfeature* ATORfeatureGenerationClass::traceEdgeAndAddMinimaAndMaximaFeatures(ATORfeature* firstNewFeatureInList, const double centroidX, const double centroidY, const vec* centroidPos, ATORpixelContiguous* firstInPixelContiguousBoundaryStack, const int dimension, int zoom, const int circumferenceForRegion)
{
	double maxNoise;
	double maxNoise2;
	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		maxNoise = ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE;
		maxNoise2 = ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2;
	}
	else
	{
		maxNoise = ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE;
		maxNoise2 = ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2;
	}

	ATORfeature* currentFeatureInList = firstNewFeatureInList;

	ATORpixelContiguous* currentInPixelContiguousBoundaryStack = firstInPixelContiguousBoundaryStack;

	int distanceToCenteroidType = DISTANCE_TO_CENTEROID_UNCHANGING;

	vec centroidposForTest;

	double previousRelativeMinimaDistanceToCenteroid;
	double previousRelativeMaximaDistanceToCenteroid;
	vec pointAtpreviousRelativeMinimaDistanceToCenteroid;
	vec pointAtpreviousRelativeMaximaDistanceToCenteroid;
	vec pointWorldAtpreviousRelativeMinimaDistanceToCenteroid;
	vec pointWorldAtpreviousRelativeMaximaDistanceToCenteroid;
	vec pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid;
	vec pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid;
	int xAtpreviousRelativeMinimaDistanceToCenteroid;
	int yAtpreviousRelativeMinimaDistanceToCenteroid;
	int xAtpreviousRelativeMaximaDistanceToCenteroid;
	int yAtpreviousRelativeMaximaDistanceToCenteroid;


	vec currentPointForTest;
	vec previousPointForTest;	//used for discontinous boundary edge check
	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		currentPointForTest.x = currentInPixelContiguousBoundaryStack->point.x;
		currentPointForTest.y = currentInPixelContiguousBoundaryStack->point.y;
		currentPointForTest.z = currentInPixelContiguousBoundaryStack->point.z;
		previousPointForTest.x = currentPointForTest.x;
		previousPointForTest.y = currentPointForTest.y;
		previousPointForTest.z = currentPointForTest.z;
		centroidposForTest.x = centroidPos->x;
		centroidposForTest.y = centroidPos->y;
		centroidposForTest.z = centroidPos->z;
	}
	else
	{
		currentPointForTest.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
		currentPointForTest.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
		currentPointForTest.z = 0.0;
		centroidposForTest.x = centroidX;
		centroidposForTest.y = centroidY;
		centroidposForTest.z = 0.0;

	}

	vec initialPointOnBoundary;
	initialPointOnBoundary.x = currentPointForTest.x;
	initialPointOnBoundary.y = currentPointForTest.y;
	initialPointOnBoundary.z = currentPointForTest.z;


	double previousUnchangingDistanceToCenteroid;
	previousUnchangingDistanceToCenteroid = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&currentPointForTest, &centroidposForTest);

	int numberOfBoundaryOrFakeBoundaryPixelsTraced = 0;

	bool stillTracingPath2 = true;
	bool foundATracePath2 = false;

	#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
	bool discontinuousTemp = false;
	#endif

	while(stillTracingPath2)
	{

		#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
		//mark boundary point
		int nonworldx = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
		int nonworldy = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
		#endif

		double currentDistanceToCenteroid;
		if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
		{
			currentPointForTest.x = currentInPixelContiguousBoundaryStack->point.x;
			currentPointForTest.y = currentInPixelContiguousBoundaryStack->point.y;
			currentPointForTest.z = currentInPixelContiguousBoundaryStack->point.z;


			if(SHAREDvector.calculateTheDistanceBetweenTwoPoints(&previousPointForTest, &currentPointForTest) > ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT)
			{
				#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
				discontinuousTemp = true;
				//mark boundary point
				contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 0] = 255;
				contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 1] = 0;
				contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 2] = 0;
				#endif

				cout << "error: discontinous edge" << endl;
				cout << "currentPointForTest.x = " << currentPointForTest.x << endl;
				cout << "currentPointForTest.y = " << currentPointForTest.y << endl;
				cout << "currentPointForTest.z = " << currentPointForTest.z << endl;
				cout << "previousPointForTest.x = " << previousPointForTest.x << endl;
				cout << "previousPointForTest.y = " << previousPointForTest.y << endl;
				cout << "previousPointForTest.z = " << previousPointForTest.z << endl;
			}
			previousPointForTest.x = currentPointForTest.x;
			previousPointForTest.y = currentPointForTest.y;
			previousPointForTest.z = currentPointForTest.z;
		}
		else
		{
			currentPointForTest.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
			currentPointForTest.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
			currentPointForTest.z = 0.0;
		}

		#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
		//mark boundary point
		contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 0] = 0;
		contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 1] = 255;
		contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 2] = 255;
		//generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", tempimageWidth, tempimageHeight, contiguousRegionDebugrgbMap);
		#endif

		currentDistanceToCenteroid = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&currentPointForTest, &centroidposForTest);


		if((numberOfBoundaryOrFakeBoundaryPixelsTraced > (MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY)) && (SHAREDvector.compareVectors(&currentPointForTest, &initialPointOnBoundary)))
		{
			stillTracingPath2 = false;
			foundATracePath2 = true;
		}
		else
		{
			if(distanceToCenteroidType == DISTANCE_TO_CENTEROID_DECREASING)
			{
				#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
				//cout << "DISTANCE_TO_CENTEROID_DECREASING" << endl;
				#endif
				if(currentDistanceToCenteroid > (previousRelativeMinimaDistanceToCenteroid+maxNoise))
				{
					#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
					//cout << "1" << endl;
					//cout << "Trace Minima Found" << endl;
					//mark boundary point
					if(!discontinuousTemp)
					{
						contiguousRegionDebugrgbMap[yAtpreviousRelativeMinimaDistanceToCenteroid*tempimageWidth*RGB_NUM + xAtpreviousRelativeMinimaDistanceToCenteroid*RGB_NUM + 0] = 255;
						contiguousRegionDebugrgbMap[yAtpreviousRelativeMinimaDistanceToCenteroid*tempimageWidth*RGB_NUM + xAtpreviousRelativeMinimaDistanceToCenteroid*RGB_NUM + 1] = 255;
						contiguousRegionDebugrgbMap[yAtpreviousRelativeMinimaDistanceToCenteroid*tempimageWidth*RGB_NUM + xAtpreviousRelativeMinimaDistanceToCenteroid*RGB_NUM + 2] = 0;
					}
					#endif

					currentFeatureInList->xViewport = xAtpreviousRelativeMinimaDistanceToCenteroid*zoom;
					currentFeatureInList->yViewport = yAtpreviousRelativeMinimaDistanceToCenteroid*zoom;
					if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
					{
						currentFeatureInList->pointNonWorldCoord.x = pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.x*zoom;
						currentFeatureInList->pointNonWorldCoord.y = pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.y*zoom;

						currentFeatureInList->point.x = pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.x;
						currentFeatureInList->point.y = pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.y;
						currentFeatureInList->point.z = pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.z;
					}
					else
					{
						currentFeatureInList->point.x = (pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.x)*zoom;
						currentFeatureInList->point.y = (pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.y)*zoom;
					}

					ATORfeature* newFeature = new ATORfeature();
					currentFeatureInList->next = newFeature;
					currentFeatureInList = currentFeatureInList->next;


					if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE)
					{
						distanceToCenteroidType = DISTANCE_TO_CENTEROID_INCREASING;
						previousRelativeMaximaDistanceToCenteroid = currentDistanceToCenteroid;
						pointAtpreviousRelativeMaximaDistanceToCenteroid.x = currentPointForTest.x;
						pointAtpreviousRelativeMaximaDistanceToCenteroid.y = currentPointForTest.y;
						pointAtpreviousRelativeMaximaDistanceToCenteroid.z = currentPointForTest.z;
						pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->point.x;
						pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->point.y;
						pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->point.z;
						pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
						pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
						pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.z;
						xAtpreviousRelativeMaximaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->xInt;
						yAtpreviousRelativeMaximaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->yInt;
					}
					else
					{
						distanceToCenteroidType = DISTANCE_TO_CENTEROID_UNCHANGING;
						previousUnchangingDistanceToCenteroid = currentDistanceToCenteroid;
					}


				}
				else if(currentDistanceToCenteroid >= previousRelativeMinimaDistanceToCenteroid)
				{
				}
				else
				{//currentDistanceToPoint < previousRelativeMinimaDistanceToCenteroid


					previousRelativeMinimaDistanceToCenteroid = currentDistanceToCenteroid;
					pointAtpreviousRelativeMinimaDistanceToCenteroid.x = currentPointForTest.x;
					pointAtpreviousRelativeMinimaDistanceToCenteroid.y = currentPointForTest.y;
					pointAtpreviousRelativeMinimaDistanceToCenteroid.z = currentPointForTest.z;
					pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->point.x;
					pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->point.y;
					pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->point.z;
					pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
					pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
					pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.z;
					xAtpreviousRelativeMinimaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->xInt;
					yAtpreviousRelativeMinimaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->yInt;
				}
			}
			else if(distanceToCenteroidType == DISTANCE_TO_CENTEROID_INCREASING)
			{
				#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
				//cout << "DISTANCE_TO_CENTEROID_INCREASING" << endl;
				#endif

				if(currentDistanceToCenteroid < (previousRelativeMaximaDistanceToCenteroid-maxNoise))
				{
					#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
					//cout << "4" << endl;
					//cout << "Trace Maxima Found" << endl;
					//mark boundary point
					if(!discontinuousTemp)
					{
						contiguousRegionDebugrgbMap[yAtpreviousRelativeMaximaDistanceToCenteroid*tempimageWidth*RGB_NUM + xAtpreviousRelativeMaximaDistanceToCenteroid*RGB_NUM + 0] = 255;
						contiguousRegionDebugrgbMap[yAtpreviousRelativeMaximaDistanceToCenteroid*tempimageWidth*RGB_NUM + xAtpreviousRelativeMaximaDistanceToCenteroid*RGB_NUM + 1] = 255;
						contiguousRegionDebugrgbMap[yAtpreviousRelativeMaximaDistanceToCenteroid*tempimageWidth*RGB_NUM + xAtpreviousRelativeMaximaDistanceToCenteroid*RGB_NUM + 2] = 0;
					}
					#endif

					currentFeatureInList->xViewport = xAtpreviousRelativeMaximaDistanceToCenteroid;
					currentFeatureInList->yViewport = yAtpreviousRelativeMaximaDistanceToCenteroid;
					if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
					{
						currentFeatureInList->pointNonWorldCoord.x = pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.x*zoom;
						currentFeatureInList->pointNonWorldCoord.y = pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.y*zoom;

						currentFeatureInList->point.x = pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.x;
						currentFeatureInList->point.y = pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.y;
						currentFeatureInList->point.z = pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.z;
					}
					else
					{
						currentFeatureInList->point.x = (pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.x)*zoom;
						currentFeatureInList->point.y = (pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.y)*zoom;
					}
					ATORfeature* newFeature = new ATORfeature();
					currentFeatureInList->next = newFeature;
					currentFeatureInList = currentFeatureInList->next;


					if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE)
					{
						distanceToCenteroidType = DISTANCE_TO_CENTEROID_DECREASING;
						previousRelativeMinimaDistanceToCenteroid = currentDistanceToCenteroid;
						pointAtpreviousRelativeMinimaDistanceToCenteroid.x = currentPointForTest.x;
						pointAtpreviousRelativeMinimaDistanceToCenteroid.y = currentPointForTest.y;
						pointAtpreviousRelativeMinimaDistanceToCenteroid.z = currentPointForTest.z;
						pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->point.x;
						pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->point.y;
						pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->point.z;
						pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
						pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
						pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.z;
						xAtpreviousRelativeMinimaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->xInt;
						yAtpreviousRelativeMinimaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->yInt;
					}
					else
					{
						distanceToCenteroidType = DISTANCE_TO_CENTEROID_UNCHANGING;	//decreasing
						previousUnchangingDistanceToCenteroid = currentDistanceToCenteroid;
					}


				}
				else if(currentDistanceToCenteroid <= previousRelativeMaximaDistanceToCenteroid)
				{
				}
				else
				{//currentDistanceToPoint > previousRelativeMaximaDistanceToCenteroid


					previousRelativeMaximaDistanceToCenteroid = currentDistanceToCenteroid;
					pointAtpreviousRelativeMaximaDistanceToCenteroid.x = currentPointForTest.x;
					pointAtpreviousRelativeMaximaDistanceToCenteroid.y = currentPointForTest.y;
					pointAtpreviousRelativeMaximaDistanceToCenteroid.z = currentPointForTest.z;
					pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->point.x;
					pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->point.y;
					pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->point.z;
					pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
					pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
					pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.z;
					xAtpreviousRelativeMaximaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->xInt;
					yAtpreviousRelativeMaximaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->yInt;
				}
			}
			else if(distanceToCenteroidType == DISTANCE_TO_CENTEROID_UNCHANGING)
			{
				#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
				//cout << "DISTANCE_TO_CENTEROID_UNCHANGING" << endl;
				/*
				cout << "currentDistanceToCenteroid = " << currentDistanceToCenteroid << endl;
				cout << "previousUnchangingDistanceToCenteroid = " << previousUnchangingDistanceToCenteroid << endl;
				cout << "maxNoise2 = " << maxNoise2 << endl;
				*/
				#endif

				if(currentDistanceToCenteroid < (previousUnchangingDistanceToCenteroid-maxNoise2))
				{

					distanceToCenteroidType = DISTANCE_TO_CENTEROID_DECREASING;

					previousRelativeMinimaDistanceToCenteroid = currentDistanceToCenteroid;
					pointAtpreviousRelativeMinimaDistanceToCenteroid.x = currentPointForTest.x;
					pointAtpreviousRelativeMinimaDistanceToCenteroid.y = currentPointForTest.y;
					pointAtpreviousRelativeMinimaDistanceToCenteroid.z = currentPointForTest.z;
					pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->point.x;
					pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->point.y;
					pointWorldAtpreviousRelativeMinimaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->point.z;
					pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
					pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
					pointNonWorldAtpreviousRelativeMinimaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.z;
					xAtpreviousRelativeMinimaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->xInt;
					yAtpreviousRelativeMinimaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->yInt;
				}
				else if(currentDistanceToCenteroid > (previousUnchangingDistanceToCenteroid+maxNoise2))
				{

					distanceToCenteroidType = DISTANCE_TO_CENTEROID_INCREASING;

					previousRelativeMaximaDistanceToCenteroid = currentDistanceToCenteroid;
					pointAtpreviousRelativeMaximaDistanceToCenteroid.x = currentPointForTest.x;
					pointAtpreviousRelativeMaximaDistanceToCenteroid.y = currentPointForTest.y;
					pointAtpreviousRelativeMaximaDistanceToCenteroid.z = currentPointForTest.z;
					pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->point.x;
					pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->point.y;
					pointWorldAtpreviousRelativeMaximaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->point.z;
					pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.x = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.x;
					pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.y = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.y;
					pointNonWorldAtpreviousRelativeMaximaDistanceToCenteroid.z = currentInPixelContiguousBoundaryStack->pointNonWorldCoord.z;
					xAtpreviousRelativeMaximaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->xInt;
					yAtpreviousRelativeMaximaDistanceToCenteroid = currentInPixelContiguousBoundaryStack->yInt;
				}
				else
				{
				}
			}
			else
			{
				cerr << "trace error" << endl;
				exit(EXIT_ERROR);
			}

			#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
			//remark boundary point
			if(numberOfBoundaryOrFakeBoundaryPixelsTraced == 0)
			{
				contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 0] = 128;
				contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 1] = 128;
				contiguousRegionDebugrgbMap[nonworldy*tempimageWidth*RGB_NUM + nonworldx*RGB_NUM + 2] = 128;
			}
			#endif

			int numberOfPossibleCrawlPaths = 0;
			int nextPixelToCrawl;
			for(int q=0;q<9;q++)
			{
				if(q != 4)
				{
					if(currentInPixelContiguousBoundaryStack->next[q] != NULL)
					{
						vec possiblePointOnBoundary;
						if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
							possiblePointOnBoundary.x = currentInPixelContiguousBoundaryStack->next[q]->point.x;
							possiblePointOnBoundary.y = currentInPixelContiguousBoundaryStack->next[q]->point.y;
							possiblePointOnBoundary.z = currentInPixelContiguousBoundaryStack->next[q]->point.z;

						}
						else
						{
							possiblePointOnBoundary.x = currentInPixelContiguousBoundaryStack->next[q]->pointNonWorldCoord.x;
							possiblePointOnBoundary.y = currentInPixelContiguousBoundaryStack->next[q]->pointNonWorldCoord.y;
							possiblePointOnBoundary.z = 0.0;
						}


						if((numberOfBoundaryOrFakeBoundaryPixelsTraced > (MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY)) && (SHAREDvector.compareVectors(&(possiblePointOnBoundary), &initialPointOnBoundary)))
						{
							stillTracingPath2 = false;
							foundATracePath2 = true;
						}
						#ifdef NEWCHECKTHIS
						else if(SHAREDvector.compareVectors(&(possiblePointOnBoundary), &initialPointOnBoundary))
						{
							#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
							//cout << "orig" << endl;
							#endif

						}
						#endif
						else
						{

							if(currentInPixelContiguousBoundaryStack->next[q]->pathAlreadyCrawled == true)
							{//this test is probably not required

								if(currentInPixelContiguousBoundaryStack->next[q]->finalPathAlreadyCrawled == false)
								{
									nextPixelToCrawl = q;
									numberOfPossibleCrawlPaths++;
								}
							}
						}
					}
				}
			}

			if(stillTracingPath2)
			{
				if(numberOfPossibleCrawlPaths == 0)
				{
					cerr << "circumferenceForRegion = " << circumferenceForRegion << endl;
					cerr << "numberOfBoundaryOrFakeBoundaryPixelsTraced = " << numberOfBoundaryOrFakeBoundaryPixelsTraced << endl;
					cerr << "error; numberOfPossibleCrawlPaths == 0" << endl;
					exit(EXIT_ERROR);
				}
				else if(numberOfPossibleCrawlPaths > 1)
				{
					cerr << "circumferenceForRegion = " << circumferenceForRegion << endl;
					cerr << "numberOfBoundaryOrFakeBoundaryPixelsTraced = " << numberOfBoundaryOrFakeBoundaryPixelsTraced << endl;
					cerr << "error; numberOfPossibleCrawlPaths > 1" << endl;
					exit(EXIT_ERROR);
				}
				else
				{
					#ifdef NEWCHECKTHIS2
					currentInPixelContiguousBoundaryStack->finalPathAlreadyCrawled = true;
					#endif
					currentInPixelContiguousBoundaryStack = currentInPixelContiguousBoundaryStack->next[nextPixelToCrawl];
				}
			}

		}
		numberOfBoundaryOrFakeBoundaryPixelsTraced++;
	}

	return currentFeatureInList;
}





//DO: write a new identical version of addCentredFeaturesToFeatureList that instead operates on a ATORmeshPoint list using 1. meshPointNormalContrast and then 2. meshPointLuminosityContrast values

//nb this routine should work where contrastMap is either the luminosityContrastMap [or the depthContrastMap or the depthGradientContrastMap (3DOD only)]
//this routine has not yet been tested
bool ATORfeatureGenerationClass::addCentredFeaturesToFeatureListUsingContrastMap(ATORfeature* firstFeatureInFeatureList, int imageWidth, int imageHeight, const double contrastMap[], const double sensitivity, int dimension, double pointMap[], double depthMap[], int zoom, RTviewInfo* vi, const bool useEdgeZeroCrossingMap, const int interpixelContrastMapType)
{
	bool result = true;

	double pixelXOffset;
	double pixelYOffset;
	int kernelWidthForegroundCheck;
	int kernelHeightForegroundCheck;
	int xBoundaryMax;
	int xBoundaryMin;
	int yBoundaryMax;
	int yBoundaryMin;
	if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
	{
		//without ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		xBoundaryMax = imageWidth-1;
		xBoundaryMin = 0;
		yBoundaryMax = imageHeight-1;
		yBoundaryMin = 0;
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
	{
		//such as luminosityContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0.5;
		pixelYOffset = 0.5;
		xBoundaryMax = imageWidth-2;
		xBoundaryMin = 0;
		yBoundaryMax = imageHeight-2;
		yBoundaryMin = 0;
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
	{
		//such as gradientContrast or pointNormalContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		xBoundaryMax = imageWidth-2;
		xBoundaryMin = 1;
		yBoundaryMax = imageHeight-2;
		yBoundaryMin = 1;
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_NO_INTERPIXEL;
	}




	//added by RBB 3 Oct 09
	ATORfeature* currentFeatureInList = firstFeatureInFeatureList;
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}


	//1. edge process image using ATORquadraticFit
	bool* edgeBoolMap = new bool[imageWidth*imageHeight];
#ifndef LINUX
	ATORQFzeroCrossing** edgeZeroCrossingMap = new ATORQFzeroCrossing* [imageWidth*imageHeight];
#else
	ATORQFzeroCrossing* edgeZeroCrossingMap[imageWidth*imageHeight];
#endif
	if(useEdgeZeroCrossingMap)
	{
		for(int i=0; i<imageWidth*imageHeight; i++)
		{
			edgeZeroCrossingMap[i] = NULL;
		}
		generateEdgeListFromContrastMapWithQuadraticFit(contrastMap, edgeBoolMap, edgeZeroCrossingMap, imageWidth, imageHeight, sensitivity, dimension, pointMap, depthMap, zoom, vi, interpixelContrastMapType);
	}
	else
	{
		if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
		{
			ATORpixelMaps.createArbitraryContrastBooleanMap(imageWidth, imageHeight, contrastMap, edgeBoolMap, sensitivity);
		}
		else if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
		{
			ATORpixelMaps.createArbitraryContrastBooleanMap(imageWidth, imageHeight, contrastMap, edgeBoolMap, sensitivity);
		}
		else
		{
			cerr << "error: illegal dimension" << endl;
			exit(EXIT_ERROR);
		}

		//generate edge map by creating a boolean map from the contrast map.
	}

	#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE

	contiguousRegionDebugrgbMap = new uchar[imageWidth*imageHeight*RGB_NUM];
	tempimageWidth = imageWidth;
	tempimageHeight = imageHeight;
	contiguousRegionNumber = 3;
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			//set black
			if(edgeBoolMap[y*imageWidth +x])
			{
				contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 255;
				contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 255;
				contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 255;
			}
			else
			{
				contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 0;
				contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
				contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 0;
			}
		}
	}
	#endif


	//calculate boolean edge map using edge zerocrossing list.

	//bool* alreadyProcessedInSearchForTheOuterBoundary = new bool[imageWidth*imageHeight];
	//2. set alreadyProcessed status to false
	int* alreadyProcessed = new int[imageWidth*imageHeight];
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			alreadyProcessed[y*imageWidth + x] = NOT_PROCESSED;
			//alreadyProcessedInSearchForTheOuterBoundary[y*imageWidth + x] = false;
		}
	}

	//find centred features
	for(int y = yBoundaryMin; y <= yBoundaryMax; y++)
	{
  		for(int x = xBoundaryMin; x <= xBoundaryMax; x++)
		{
			/*
			#ifndef ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OLD
			//must remove all edges from list
			for(int y=0; y<imageHeight; y++)
			{
				for(int x=0; x<imageWidth; x++)
				{
					if(alreadyProcessed[y*imageWidth + x] == EDGE_FOUND)
					{
						alreadyProcessed[y*imageWidth + x] = NOT_PROCESSED;
					}
				}
			}
			#endif
			*/

			if(alreadyProcessed[y*imageWidth + x] == NOT_PROCESSED)
			{
				if(edgeBoolMap[y*imageWidth + x] != true)
				{

					//1. perform region definition

					int regionsumx = 0;
					int regionsumy = 0;
					vec regionsumpos;
					regionsumpos.x = 0.0;
					regionsumpos.y = 0.0;
					regionsumpos.z = 0.0;
					double regionSize = 0.0;
					int maxXx;
					int maxXy;
					bool contiguousRegionFound;
					ATORpixelContiguous* firstInPixelContiguousStack = new ATORpixelContiguous();
					contiguousRegionFound = defineRegionCheckNextPixelNonRecursive(firstInPixelContiguousStack, x, y, edgeBoolMap, edgeZeroCrossingMap, alreadyProcessed, imageWidth, imageHeight, &regionSize, &regionsumx, &regionsumy, &regionsumpos, dimension, pointMap, depthMap, zoom, useEdgeZeroCrossingMap, vi, &maxXx, &maxXy, interpixelContrastMapType);


					#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
					//temp; generate the display of the unbounded contiguous area;
					if(contiguousRegionNumber == 1)
					{
						contiguousRegionNumber++;

					}
					#endif

					if(contiguousRegionFound && (regionSize > (MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE/zoom)))
					{
						#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
						contiguousRegionNumber++;
						#endif

						#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
						RTpixelMaps.generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
						#endif

						double regionaveragex = double(regionsumx)/double(regionSize);
						double regionaveragey = double(regionsumy)/double(regionSize);

						regionsumpos.x = regionsumpos.x / double(regionSize);
						regionsumpos.y = regionsumpos.y / double(regionSize);
						regionsumpos.z = regionsumpos.z / double(regionSize);



						if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES)
						{


							//now execute shortcut place aMeshPointOnTheBoundary on the actual boundary (edge) of the contiguous region
							int edgeXDev;
							if(maxXx > regionaveragex)
							{
								edgeXDev = 1;
							}
							else
							{
								edgeXDev = -1;
							}
							int edgeYDev;
							if(maxXy > regionaveragey)
							{
								edgeYDev = 0;
							}
							else
							{
								edgeYDev = 0;
							}
							maxXx = maxXx + edgeXDev;
							maxXy = maxXy + edgeYDev;

							#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
							//mark boundary point
							contiguousRegionDebugrgbMap[maxXy*imageWidth*RGB_NUM + maxXx*RGB_NUM + 0] = 128;
							contiguousRegionDebugrgbMap[maxXy*imageWidth*RGB_NUM + maxXx*RGB_NUM + 1] = 128;
							contiguousRegionDebugrgbMap[maxXy*imageWidth*RGB_NUM + maxXx*RGB_NUM + 2] = 128;

							//mark central point
							contiguousRegionDebugrgbMap[int(regionaveragey)*imageWidth*RGB_NUM + int(regionaveragex)*RGB_NUM + 0] = 255;
							contiguousRegionDebugrgbMap[int(regionaveragey)*imageWidth*RGB_NUM + int(regionaveragex)*RGB_NUM + 1] = 255;
							contiguousRegionDebugrgbMap[int(regionaveragey)*imageWidth*RGB_NUM + int(regionaveragex)*RGB_NUM + 2] = 0;
							#endif

							if(useEdgeZeroCrossingMap)
							{
								if(alreadyProcessed[maxXy*imageWidth + maxXx] != EDGE_FOUND)
								{
									cerr << "maxXx = " << maxXx << endl;
									cerr << "maxXy = " << maxXy << endl;
									cerr << "error - boundary edge false identification" << endl;
									exit(EXIT_ERROR);
								}
							}
							else
							{
								if(alreadyProcessed[maxXy*imageWidth + maxXx] != EDGE_FOUND)
								{
									cerr << "maxXx = " << maxXx << endl;
									cerr << "maxXy = " << maxXy << endl;
									cerr << "error - boundary edge false identification" << endl;
									exit(EXIT_ERROR);
								}
								/*
								if(edgeBoolMap[maxXy*imageWidth + maxXx] != true)
								{
									cerr << "maxXx = " << maxXx << endl;
									cerr << "maxXy = " << maxXy << endl;
									cerr << "error - boundary edge false identification" << endl;
									exit(EXIT_ERROR);
								}
								*/
							}


							/*
							4/3*PI*r^2 = Area
							assume Area = 1x1 = 1 [square]
							1 = 4/3*PI*r^2
							3/4 = PI*r^2
							r = sqrt((3/4)/PI)
							r = 0.488602512

							circumference of square region = 1*4 = 4
							circumference of circular region = 2*PI*r = 2*PI*0.488602512 = 3.069980124

							now take into account all pixels may be diagonally aligned, so multiple this by 1/sqrt(2)

							so for a region of given area A, minimum circumference = C = sqrt(A)*3.069980124*(1/sqrt(2))
							*/

							double minCircumferenceOfRegion = sqrt(regionSize)*3.069980124*(1.0/sqrt(2.0));


							//2+3. find boundary region and trace line of contrast until return to start position [if not repeat for a different edge start pos], then calculate the average x and y values to find the centre feature

							/*
							#ifdef ATOR_USE_OLD_RECURSIVE_CONTIGIOUS_REGION_METHODS_WHICH_FAIL_DUE_TO_LIMITED_C_RECURSION_STACK
							int xStart;
							int yStart;
							int boundarysumx = 0;
							int boundarysumy = 0;
							vec boundarysumpos;
							boundarysumpos.x = 0.0;
							boundarysumpos.y = 0.0;
							boundarysumpos.z = 0.0;
							bool foundStart = false;
							bool foundStartAgain = false;
							int circumferenceForRegion;
							traceEdgeCheckNextPixelRecursive(x, y, alreadyProcessed, imageWidth, imageHeight, &boundarysumx, &boundarysumy, &boundarysumpos, &xStart, &yStart, &foundStartAgain, &foundStart, &circumferenceForRegion, minCircumferenceOfRegion, dimension, pointMap, depthMap, zoom, alreadyProcessedInSearchForTheOuterBoundary);
							#else
							*/
							double boundarysumx = 0;
							double boundarysumy = 0;
							vec boundarysumpos;
							boundarysumpos.x = 0.0;
							boundarysumpos.y = 0.0;
							boundarysumpos.z = 0.0;
							int circumferenceForRegion;

							ATORpixelContiguous* firstInPixelContiguousBoundaryStack = new ATORpixelContiguous();
							bool traceSuccessful = false;

							traceSuccessful = traceEdgeCheckNextPixelNonRecursive(maxXx, maxXy, alreadyProcessed, edgeZeroCrossingMap, useEdgeZeroCrossingMap, imageWidth, imageHeight, &boundarysumx, &boundarysumy, &boundarysumpos, &circumferenceForRegion, minCircumferenceOfRegion, dimension, pointMap, depthMap, zoom, firstInPixelContiguousBoundaryStack, vi, interpixelContrastMapType);

							#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
							RTpixelMaps.generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
							#endif

							/*
							#endif
							*/

							if(traceSuccessful && (circumferenceForRegion > MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE))
							{




								double boundaryaveragex = double(boundarysumx)/double(circumferenceForRegion);
								double boundaryaveragey = double(boundarysumy)/double(circumferenceForRegion);

								boundarysumpos.x = boundarysumpos.x / double(circumferenceForRegion);
								boundarysumpos.y = boundarysumpos.y / double(circumferenceForRegion);
								boundarysumpos.z = boundarysumpos.z / double(circumferenceForRegion);


								#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
								//mark central point [orange]
								contiguousRegionDebugrgbMap[int(boundaryaveragey)*imageWidth*RGB_NUM + int(boundaryaveragex)*RGB_NUM + 0] = 255;
								contiguousRegionDebugrgbMap[int(boundaryaveragey)*imageWidth*RGB_NUM + int(boundaryaveragex)*RGB_NUM + 1] = 128;
								contiguousRegionDebugrgbMap[int(boundaryaveragey)*imageWidth*RGB_NUM + int(boundaryaveragex)*RGB_NUM + 2] = 0;
								#endif

								#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY_2
									currentFeatureInList->xViewport = boundaryaveragex*zoom;
									currentFeatureInList->yViewport = boundaryaveragey*zoom;

									if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
									{
										currentFeatureInList->pointNonWorldCoord.x = (boundaryaveragex)*zoom;
										currentFeatureInList->pointNonWorldCoord.y = (boundaryaveragey)*zoom;
										currentFeatureInList->pointNonWorldCoord.z = getDepthValueWithOrWithoutForegroundCheck(currentFeatureInList->pointNonWorldCoord.x, currentFeatureInList->pointNonWorldCoord.y, imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, zoom);

										currentFeatureInList->point.x = boundarysumpos.x;
										currentFeatureInList->point.y = boundarysumpos.y;
										currentFeatureInList->point.z = boundarysumpos.z;
									}
									else
									{
										currentFeatureInList->point.x = (boundaryaveragex)*zoom;
										currentFeatureInList->point.y = (boundaryaveragey)*zoom;
									}
								#else
									currentFeatureInList->xViewport = regionaveragex*zoom;
									currentFeatureInList->yViewport = regionaveragey*zoom;

									if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
									{
										currentFeatureInList->pointNonWorldCoord.x = (regionaveragex)*zoom;
										currentFeatureInList->pointNonWorldCoord.y = (regionaveragey)*zoom;
										currentFeatureInList->pointNonWorldCoord.z = getDepthValueWithOrWithoutForegroundCheck(currentFeatureInList->pointNonWorldCoord.x, currentFeatureInList->pointNonWorldCoord.y, imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, zoom);

										currentFeatureInList->point.x = regionsumpos.x;
										currentFeatureInList->point.y = regionsumpos.y;
										currentFeatureInList->point.z = regionsumpos.z;
									}
									else
									{
										currentFeatureInList->point.x = (regionaveragex)*zoom;
										currentFeatureInList->point.y = (regionaveragey)*zoom;
									}
								#endif

								ATORfeature* newFeature = new ATORfeature();
								currentFeatureInList->next = newFeature;
								currentFeatureInList = currentFeatureInList->next;

								if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET)
								{
									currentFeatureInList = traceEdgeAndAddMinimaAndMaximaFeatures(currentFeatureInList, boundaryaveragex, boundaryaveragey, &boundarysumpos, firstInPixelContiguousBoundaryStack, dimension, zoom, circumferenceForRegion);
								}

								#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
								RTpixelMaps.generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
								#endif
							}

							if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT)
							{
								deleteContiguousStackAndResetEdgesNonRecursive(firstInPixelContiguousBoundaryStack, alreadyProcessed, imageWidth, imageHeight);
							}
							delete firstInPixelContiguousBoundaryStack;
						}
						else
						{
							currentFeatureInList->xViewport = regionaveragex*zoom;
							currentFeatureInList->yViewport = regionaveragey*zoom;

							if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
							{
								currentFeatureInList->point.x = regionsumpos.x;
								currentFeatureInList->point.y = regionsumpos.y;
								currentFeatureInList->point.z = regionsumpos.z;

								currentFeatureInList->pointNonWorldCoord.x = (regionaveragex)*zoom;
								currentFeatureInList->pointNonWorldCoord.y = (regionaveragey)*zoom;
								currentFeatureInList->pointNonWorldCoord.z = getDepthValueWithOrWithoutForegroundCheck(currentFeatureInList->pointNonWorldCoord.x, currentFeatureInList->pointNonWorldCoord.y, imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, zoom);
							}
							else
							{
								currentFeatureInList->point.x = (regionaveragex)*zoom;
								currentFeatureInList->point.y = (regionaveragey)*zoom;
							}

							ATORfeature* newFeature = new ATORfeature();
							currentFeatureInList->next = newFeature;
							currentFeatureInList = currentFeatureInList->next;

						}

					}
					if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT)
					{
						deleteContiguousStackAndResetEdgesNonRecursive(firstInPixelContiguousStack, alreadyProcessed, imageWidth, imageHeight);

					}
					delete firstInPixelContiguousStack;
				}
			}
		}
	}

	#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
	delete contiguousRegionDebugrgbMap;
	#endif

	delete edgeBoolMap;

	#ifndef LINUX
	delete edgeZeroCrossingMap;
	#endif

	return result;
}

double ATORfeatureGenerationClass::getDepthValueWithOrWithoutForegroundCheck(const double pointNonWorldCoordx, const double pointNonWorldCoordy, const int imageWidth, const int imageHeight, const int kernelWidthForegroundCheck, const int kernelHeightForegroundCheck, const double depthMap[], const int zoom)
{
	//depth calculations added 13 June 2012 - check these....
	#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
	vec xyzNearbyPointOnObject;
	double depth = ATORpixelMaps.calculateForegroundMinimumDepthWithinKernel(int(pointNonWorldCoordx), int(pointNonWorldCoordy), imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, &xyzNearbyPointOnObject, zoom);
	#else
	double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(int(pointNonWorldCoordx), int(pointNonWorldCoordy), imageWidth, depthMap);
	#endif
	return depth;
}

/*
void calculateDepthMapValue(vec* xyzWorld, vec* pointNonWorldCoord, RTviewInfo* vi)
{	//see calculatePointMapValue

}
*/



bool ATORfeatureGenerationClass::addCentredFeaturesToFeatureListUsingMeshList(ATORfeature* firstFeatureInFeatureList, const double sensitivity, int dimension, ATORmeshPoint* firstMeshPointInMeshList, const int contrastValChosen, const bool useEdgeZeroCrossingMap)
{
	bool result;
	//added by RBB 3 Oct 09
	ATORfeature* currentFeatureInList = firstFeatureInFeatureList;
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}

	if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
	{
		//1. assume edges have already been processed on the raster image before the meshlist was created
		//if(useEdgeZeroCrossingMap)
	}

	//calculate boolean edge map using edge zerocrossing list.
	//2. set alreadyProcessed status to false (assume  this is the case already

	//find centred features
	ATORmeshPoint* currentMeshPointInMesh = firstMeshPointInMeshList;
	while(currentMeshPointInMesh->next != NULL)
	{
		if(currentMeshPointInMesh->alreadyProcessed == NOT_PROCESSED)
		{
			if(currentMeshPointInMesh->edge != true)
			{
				//1. perform region definition
				/*
				#ifdef ATOR_USE_OLD_RECURSIVE_CONTIGIOUS_REGION_METHODS_WHICH_FAIL_DUE_TO_LIMITED_C_RECURSION_STACK
				defineRegionCheckNextPixelUsingMeshPointRecursive(currentMeshPointInMesh, &regionSize, contrastValChosen, 0);
				#else
				*/
				int regionsumx = 0;
				int regionsumy = 0;
				vec regionsumpos;
				regionsumpos.x = 0.0;
				regionsumpos.y = 0.0;
				regionsumpos.z = 0.0;
				double regionSize = 0.0;
				ATORmeshPoint* aMeshPointOnTheBoundary;
				bool contiguousRegionFound;
				ATORpixelContiguous* firstInPixelContiguousStack = new ATORpixelContiguous();
				contiguousRegionFound = defineRegionCheckNextPixelUsingMeshPointNonRecursive(firstInPixelContiguousStack, currentMeshPointInMesh, &regionSize, contrastValChosen, &regionsumx, &regionsumy, &regionsumpos, useEdgeZeroCrossingMap, aMeshPointOnTheBoundary, sensitivity);

				if(contiguousRegionFound && (regionSize > (MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE)))
				{
					double regionaveragex = double(regionsumx)/double(regionSize);
					double regionaveragey = double(regionsumy)/double(regionSize);

					regionsumpos.x = regionsumpos.x / double(regionSize);
					regionsumpos.y = regionsumpos.y / double(regionSize);
					regionsumpos.z = regionsumpos.z / double(regionSize);


					if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES)
					{
						//now execute shortcut place aMeshPointOnTheBoundary on the actual boundary (edge) of the contiguous region
						int edgeXDev;
						if(aMeshPointOnTheBoundary->xInt > regionaveragex)
						{
							edgeXDev = 1;
						}
						else
						{
							edgeXDev = -1;
						}
						int edgeYDev;
						if(aMeshPointOnTheBoundary->yInt > regionaveragey)
						{
							edgeYDev = 1;
						}
						else
						{
							edgeYDev = -1;
						}
						int s = edgeXDev+1;
						int t = edgeYDev+1;
						int q = edgeYDev*3 + edgeXDev;
						aMeshPointOnTheBoundary = aMeshPointOnTheBoundary->adjacentMeshPoint[q];

						/*
						if(aMeshPointOnTheBoundary->edge != true)
						{
							cerr << "error - boundary edge false identification" << endl;
							exit(EXIT_ERROR);
						}
						*/
						if(aMeshPointOnTheBoundary->alreadyProcessed != EDGE_FOUND)
						{
							cerr << "error - boundary edge false identification" << endl;
							exit(EXIT_ERROR);
						}

						/*
						4/3*PI*r^2 = Area
						assume Area = 1x1 = 1 [square]
						1 = 4/3*PI*r^2
						3/4 = PI*r^2
						r = sqrt((3/4)/PI)
						r = 0.488602512

						circumference of square region = 1*4 = 4
						circumference of circular region = 2*PI*r = 2*PI*0.488602512 = 3.069980124

						now take into account all pixels may be diagonally aligned, so multiple this by 1/sqrt(2)

						so for a region of given area A, minimum circumference = C = sqrt(A)*3.069980124*(1/sqrt(2))
						*/

						double minCircumferenceOfRegion = sqrt(regionSize)*3.069980124*(1.0/sqrt(2.0));

						cout << "regionSize = " << regionSize << endl;
						cout << "minCircumferenceOfRegion = " << minCircumferenceOfRegion << endl;


						/*
						//2+3. find boundary region and trace line of contrast until return to start position [if not repeat for a different edge start pos], then calculate the average x and y values to find the centre feature

						/*
						#ifdef ATOR_USE_OLD_RECURSIVE_CONTIGIOUS_REGION_METHODS_WHICH_FAIL_DUE_TO_LIMITED_C_RECURSION_STACK
						int xStart;
						int yStart;
						int boundarysumx = 0;
						int boundarysumy = 0;
						vec boundarysumpos;
						boundarysumpos.x = 0.0;
						boundarysumpos.y = 0.0;
						boundarysumpos.z = 0.0;
						bool foundStart = false;
						bool foundStartAgain = false;
						int circumferenceForRegion;
						traceEdgeCheckNextPixelUsingMeshPointRecursive(currentMeshPointInMesh, &sumX, &sumY, &sumPos, &xStart, &yStart, &foundStartAgain, &foundStart, &circumferenceForRegion, minCircumferenceOfRegion);
						#else
						*/
						double boundarysumx = 0;
						double boundarysumy = 0;
						vec boundarysumpos;
						boundarysumpos.x = 0.0;
						boundarysumpos.y = 0.0;
						boundarysumpos.z = 0.0;
						int circumferenceForRegion;

						ATORpixelContiguous* firstInPixelContiguousBoundaryStack = new ATORpixelContiguous();
						bool traceSuccessful = false;

						traceSuccessful = traceEdgeCheckNextPixelUsingMeshPointNonRecursive(aMeshPointOnTheBoundary, &boundarysumx, &boundarysumy, &boundarysumpos, &circumferenceForRegion, minCircumferenceOfRegion, firstInPixelContiguousBoundaryStack, useEdgeZeroCrossingMap);


						if(traceSuccessful)
						{



							double boundaryaveragex = double(boundarysumx)/double(circumferenceForRegion);
							double boundaryaveragey = double(boundarysumy)/double(circumferenceForRegion);

							boundarysumpos.x = boundarysumpos.x / double(circumferenceForRegion);
							boundarysumpos.y = boundarysumpos.y / double(circumferenceForRegion);
							boundarysumpos.z = boundarysumpos.z / double(circumferenceForRegion);


							#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY_2
								currentFeatureInList->xViewport = boundaryaveragex;
								currentFeatureInList->yViewport = boundaryaveragey;

								if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
								{
									currentFeatureInList->pointNonWorldCoord.x = boundaryaveragex;
									currentFeatureInList->pointNonWorldCoord.y = boundaryaveragey;

									currentFeatureInList->point.x = boundarysumpos.x;
									currentFeatureInList->point.y = boundarysumpos.y;
									currentFeatureInList->point.z = boundarysumpos.z;
								}
								else
								{
									currentFeatureInList->point.x = boundaryaveragex;
									currentFeatureInList->point.y = boundaryaveragey;
								}
							#else
								currentFeatureInList->xViewport = regionaveragex;
								currentFeatureInList->yViewport = regionaveragey;

								if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
								{
									currentFeatureInList->pointNonWorldCoord.x = regionaveragex;
									currentFeatureInList->pointNonWorldCoord.y = regionaveragey;

									currentFeatureInList->point.x = regionsumpos.x;
									currentFeatureInList->point.y = regionsumpos.y;
									currentFeatureInList->point.z = regionsumpos.z;
								}
								else
								{
									currentFeatureInList->point.x = regionaveragex;
									currentFeatureInList->point.y = regionaveragey;
								}
							#endif

							ATORfeature* newFeature = new ATORfeature();
							currentFeatureInList->next = newFeature;
							currentFeatureInList = currentFeatureInList->next;

							if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET)
							{
								currentFeatureInList = traceEdgeAndAddMinimaAndMaximaFeatures(currentFeatureInList, boundaryaveragex, boundaryaveragey, &boundarysumpos, firstInPixelContiguousBoundaryStack, dimension, 1.0, circumferenceForRegion);
							}
						}

						if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT)
						{
							deleteContiguousStackAndResetEdgesNonRecursive(firstInPixelContiguousBoundaryStack);
						}
						delete firstInPixelContiguousBoundaryStack;
					}
					else
					{

						currentFeatureInList->xViewport = regionaveragex;
						currentFeatureInList->yViewport = regionaveragey;

						if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
							currentFeatureInList->pointNonWorldCoord.x = regionaveragex;
							currentFeatureInList->pointNonWorldCoord.y = regionaveragey;

							currentFeatureInList->point.x = regionsumpos.x;
							currentFeatureInList->point.y = regionsumpos.y;
							currentFeatureInList->point.z = regionsumpos.z;
						}
						else
						{
							currentFeatureInList->point.x = regionaveragex;
							currentFeatureInList->point.y = regionaveragey;
						}

						ATORfeature* newFeature = new ATORfeature();
						currentFeatureInList->next = newFeature;
						currentFeatureInList = currentFeatureInList->next;

					}


				}
				if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT)
				{
					deleteContiguousStackAndResetEdgesNonRecursive(firstInPixelContiguousStack);
				}
				delete firstInPixelContiguousStack;

			}
		}
		currentMeshPointInMesh = currentMeshPointInMesh->next;
	}

	return result;
}



void calculateNewXYbasedOnDirection(int xCurrent, int yCurrent, int q, int* x, int* y)
{
       * x = xCurrent + q%3 - 1;
	*y = yCurrent + q/3 - 1;
}

bool ATORfeatureGenerationClass::defineRegionCheckNextPixelNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack, int xInitial, int yInitial, const bool edgeBoolMap[], constEffective ATORQFzeroCrossing* edgeZeroCrossingMap[], int alreadyProcessed[], int imageWidth, const int imageHeight, double* regionSize, int* sumX, int* sumY, vec* sumPos, const int dimension, double pointMap[], const double depthMap[], int zoom, const bool useEdgeZeroCrossingMap, RTviewInfo* vi, int* maxXx, int* maxXy, const int interpixelContrastMapType)
{
	double pixelXOffset;
	double pixelYOffset;
	int xBoundaryMax;
	int xBoundaryMin;
	int yBoundaryMax;
	int yBoundaryMin;
	if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
	{
		//without ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		xBoundaryMax = imageWidth-1;
		xBoundaryMin = 0;
		yBoundaryMax = imageHeight-1;
		yBoundaryMin = 0;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
	{
		//such as luminosityContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0.5;
		pixelYOffset = 0.5;
		xBoundaryMax = imageWidth-2;
		xBoundaryMin = 0;
		yBoundaryMax = imageHeight-2;
		yBoundaryMin = 0;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
	{
		//such as gradientContrast or pointNormalContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		xBoundaryMax = imageWidth-2;
		xBoundaryMin = 1;
		yBoundaryMax = imageHeight-2;
		yBoundaryMin = 1;
	}

	bool contiguousRegionFound = true;
	bool unboundedContiguousRegionFound = false;

	//shortcut to define a pixel on the boundary of the contiguous region
	*maxXx = xInitial;
	*maxXy = yInitial;


	firstInPixelContiguousStack->xInt = xInitial;
	firstInPixelContiguousStack->yInt = yInitial;
	ATORpixelContiguous* currentInPixelContiguousStack = firstInPixelContiguousStack;

	bool stillProcessing = true;
	while(stillProcessing)
	{
		int xCurrent = currentInPixelContiguousStack->xInt;
		int yCurrent = currentInPixelContiguousStack->yInt;
		int numberOfNewBranchesFromThisPixel = 0;

		for(int q2=1; q2<9; q2=q2+2)
		{
			int q = (q2+2)%8;	//required such that -x, +x crawing occurs before -y,+y crawing to enable tight zigzagging during crawl

			int xDev;
			int yDev;

			int x;
			int y;

			xDev = q%3 - 1;
			yDev = q/3 - 1;

			x = xCurrent + xDev;
			y = yCurrent + yDev;


			if((x >= xBoundaryMin) && (x <= xBoundaryMax) && (y >= yBoundaryMin) && (y <= yBoundaryMax))
			{//map boundary check


				int pixelStatus = alreadyProcessed[(y*imageWidth) + x];
				if(pixelStatus == NOT_PROCESSED)
				{
					if(edgeBoolMap[y*imageWidth + x] == true)
					{
						alreadyProcessed[y*imageWidth + x] = EDGE_FOUND;

						#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE

						#ifndef ATOR_CONTIGUOUS_REGION_VERBOSE3
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 255;
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 255;
						#endif

						#endif

						//PUT THIS CODE BACK IN - TEMP REMOVED FOR DEBUG
						if(useEdgeZeroCrossingMap)
						{
							double subpixelArea = ATORquadraticFit.calculateAreaOfOneSideOfEdgeInPixel(-(x-xCurrent), -(y-yCurrent), edgeZeroCrossingMap[y*imageWidth + x]->zeroCrossingValueX, edgeZeroCrossingMap[y*imageWidth + x]->zeroCrossingValueY, edgeZeroCrossingMap[y*imageWidth + x]->alpha);
							#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY
							*regionSize = *regionSize + subpixelArea;
							#endif

							//now add subpixel area for a diagonal adjacent pixels if necessary

							int xAdjacentDev1;
							int yAdjacentDev1;
							int xAdjacentDev2;
							int yAdjacentDev2;
							int xDiagDev1;
							int yDiagDev1;
							int xDiagDev2;
							int yDiagDev2;
							int xDiagonalPixel1;
							int yDiagonalPixel1;
							int xDiagonalPixel2;
							int yDiagonalPixel2;
							int xAdjacentPixel1;
							int yAdjacentPixel1;
							int xAdjacentPixel2;
							int yAdjacentPixel2;
							if((xDev > 0) || (xDev < 0))
							{
								xDiagDev1 = 0;
								yDiagDev1 = 1;
								xDiagDev2 = 0;
								yDiagDev2 = -1;
								xAdjacentDev1 = x-xCurrent;
								yAdjacentDev1 = yDiagDev1;
								xAdjacentDev2 = x-xCurrent;
								yAdjacentDev2 = yDiagDev2;
								xDiagonalPixel1 = xCurrent + xDiagDev1;
								yDiagonalPixel1 = yCurrent + yDiagDev1;
								xDiagonalPixel2 = xCurrent + xDiagDev2;
								yDiagonalPixel2 = yCurrent + yDiagDev2;
								xAdjacentPixel1 = x;
								yAdjacentPixel1 = yDiagonalPixel1;
								xAdjacentPixel2 = x;
								yAdjacentPixel2 = yDiagonalPixel2;

							}
							else
							{
								xDiagDev1 = 1;
								yDiagDev1 = 0;
								xDiagDev2 = -1;
								yDiagDev2 = 0;
								xAdjacentDev1 = xDiagDev1;
								yAdjacentDev1 = y-yCurrent;
								xAdjacentDev2 = xDiagDev2;
								yAdjacentDev2 = y-yCurrent;
								xDiagonalPixel1 = xCurrent + xDiagDev1;
								yDiagonalPixel1 = yCurrent + yDiagDev1;
								xDiagonalPixel2 = xCurrent + xDiagDev2;
								yDiagonalPixel2 = yCurrent + yDiagDev2;
								xAdjacentPixel1 = xDiagonalPixel1;
								yAdjacentPixel1 = y;
								xAdjacentPixel2 = xDiagonalPixel2;
								yAdjacentPixel2 = y;
							}

							if(edgeBoolMap[yDiagonalPixel1*imageWidth + xDiagonalPixel1] == true)
							{
								if((alreadyProcessed[(yAdjacentPixel1*imageWidth) + xAdjacentPixel1] == NOT_PROCESSED) && (edgeBoolMap[yAdjacentPixel1*imageWidth + xAdjacentPixel1] == true))
								{
									alreadyProcessed[(yAdjacentPixel1*imageWidth) + xAdjacentPixel1] = EDGE_FOUND;
									#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
									#ifndef ATOR_CONTIGUOUS_REGION_VERBOSE3
									contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 255;
									contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
									contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 255;
									#endif
									#endif

									double subpixelAreaAdjacent = ATORquadraticFit.calculateAreaOfOneSideOfEdgeInPixel(-xAdjacentDev1, -yAdjacentDev1, edgeZeroCrossingMap[yAdjacentPixel1*imageWidth + xAdjacentPixel1]->zeroCrossingValueX, edgeZeroCrossingMap[yAdjacentPixel1*imageWidth + xAdjacentPixel1]->zeroCrossingValueY, edgeZeroCrossingMap[yAdjacentPixel1*imageWidth + xAdjacentPixel1]->alpha);
									#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY
									*regionSize = *regionSize + subpixelAreaAdjacent;
									#endif

								}
							}
							if(edgeBoolMap[yDiagonalPixel2*imageWidth + xDiagonalPixel2] == true)
							{
								if((alreadyProcessed[(yAdjacentPixel2*imageWidth) + xAdjacentPixel2] == NOT_PROCESSED) && (edgeBoolMap[yAdjacentPixel2*imageWidth + xAdjacentPixel2] == true))
								{
									alreadyProcessed[(yAdjacentPixel2*imageWidth) + xAdjacentPixel2] = EDGE_FOUND;
									#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
									#ifndef ATOR_CONTIGUOUS_REGION_VERBOSE3
									contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 255;
									contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
									contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 255;
									#endif
									#endif

									double subpixelAreaAdjacent = ATORquadraticFit.calculateAreaOfOneSideOfEdgeInPixel(-xAdjacentDev2, -yAdjacentDev2, edgeZeroCrossingMap[yAdjacentPixel2*imageWidth + xAdjacentPixel2]->zeroCrossingValueX, edgeZeroCrossingMap[yAdjacentPixel2*imageWidth + xAdjacentPixel2]->zeroCrossingValueY, edgeZeroCrossingMap[yAdjacentPixel2*imageWidth + xAdjacentPixel2]->alpha);
									#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY
									*regionSize = *regionSize + subpixelAreaAdjacent;
									#endif
								}
							}
						}

						//stop crawl here
					}
					else
					{
						ATORpixelContiguous* newPixel = new ATORpixelContiguous(x, y, currentInPixelContiguousStack);
						currentInPixelContiguousStack->next[q2] = newPixel;
						*regionSize = *regionSize + 1;
						alreadyProcessed[y*imageWidth + x] = NO_EDGE_FOUND;
						numberOfNewBranchesFromThisPixel++;

						#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = ((contiguousRegionNumber/1)%10* 25);
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = ((contiguousRegionNumber/10)%10* 25);
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = ((contiguousRegionNumber/100)%10* 25);
						#endif

						*sumX = *sumX + pixelXOffset + x;
						*sumY = *sumY + pixelYOffset + y;

						if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
						#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
							double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue((x+pixelXOffset)*zoom, (y+pixelXOffset)*zoom, imageWidth*zoom, depthMap);
							vec xyzWorld;
							RTscene.calculatePointMapValue((x+pixelXOffset)*zoom, (y+pixelYOffset)*zoom, depth, &xyzWorld, vi);

						#else
							cout << "error: traceEdgeCheckNextPixelRecursive{} requires ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH when compiling with 3DOD/use3DODonObjectForegroundDepthCheck" << endl;
							vec xyzWorld;
							RTpixelMaps.getPointMapValue((x+pixelXOffset)*zoom, (y+pixelYOffset)*zoom, imageWidth*zoom, pointMap, &xyzWorld);
						#endif

							sumPos->x = sumPos->x +  xyzWorld.x;
							sumPos->y = sumPos->y +  xyzWorld.y;
							sumPos->z = sumPos->z +  xyzWorld.z;
						}



						//shortcut to define a pixel on the boundary of the contiguous region
						if(x >* maxXx)
						{
							*maxXx = x;
							*maxXy = y;
						}

					}
				}
				else if(pixelStatus == NO_EDGE_FOUND)
				{
					/*
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 0;
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 255;
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 0;
					*/
					//stop crawl here
				}
				else if(pixelStatus == EDGE_FOUND)
				{
					/*
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 0;
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 255;
					*/
					//stop crawl here
				}
			}
			else
			{
				//on edge of map
				contiguousRegionFound = false;
				unboundedContiguousRegionFound = true;
				//currentInPixelContiguousStack->next[q2]->pathAlreadyCrawled = true;
				//end of crawl line
			}
		}

		bool foundAReferenceToCrawlTo = false;
		for(int q=1; q<9; q=q+2)
		{
			if((!foundAReferenceToCrawlTo) && (currentInPixelContiguousStack->next[q] != NULL))
			{
				if(currentInPixelContiguousStack->next[q]->pathAlreadyCrawled != true)
				{
					bool nextIsNotPrevious = true;
					if(currentInPixelContiguousStack->previous != NULL)
					{
						if((currentInPixelContiguousStack->next[q]->xInt == currentInPixelContiguousStack->previous->xInt) && (currentInPixelContiguousStack->next[q]->yInt == currentInPixelContiguousStack->previous->yInt))
						{
							nextIsNotPrevious = false;
						}
					}

					if(nextIsNotPrevious)
					{
						foundAReferenceToCrawlTo = true;
						currentInPixelContiguousStack = currentInPixelContiguousStack->next[q];
					}
				}
			}
		}
		if(!foundAReferenceToCrawlTo)
		{
			currentInPixelContiguousStack->pathAlreadyCrawled = true;
			if(currentInPixelContiguousStack->previous != NULL)
			{
				currentInPixelContiguousStack = currentInPixelContiguousStack->previous;
			}
			else
			{
				stillProcessing = false;
			}
		}
	}

	#ifdef ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OLD
	if(unboundedContiguousRegionFound)
	{
		cout << "unbounded region found - it is most efficient to just clear all edge pixels" << endl;
		//must remove all edges from list
		for(int y=0; y<imageHeight; y++)
		{
			for(int x=0; x<imageWidth; x++)
			{
				if(alreadyProcessed[y*imageWidth + x] == EDGE_FOUND)
				{
					alreadyProcessed[y*imageWidth + x] = NOT_PROCESSED;
				}
			}
		}
	}
	else if(!contiguousRegionFound)
	{
		cout << "contigous region not found - it is most efficient to restore all edge pixels in contigous stack" << endl;
		clearContiguousStackRecursive(firstInPixelContiguousStack, alreadyProcessed, imageWidth, imageHeight);
	}
	#endif



	return contiguousRegionFound;

}

void ATORfeatureGenerationClass::deleteContiguousStackAndResetEdgesNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack, int alreadyProcessed[], const int imageWidth, const int imageHeight)
{
	ATORpixelContiguous* currentPixelContiguous = firstInPixelContiguousStack;
	bool stillMoreToDelete = true;
	while(stillMoreToDelete)
	{
		bool foundAPixelContiguousToParseInto = false;
		ATORpixelContiguous* pixelContiguousToParseInto;
		for(int q=0;q<9;q++)
		{
			if(q != 4)
			{
				if(currentPixelContiguous->next[q] != NULL)
				{
					if(currentPixelContiguous->next[q]->readyToDelete)
					{
						for(int r=0; r<9; r++)
						{
							int xDev;
							int yDev;

							xDev = 0 + r%3 - 1;
							yDev = 0 + r/3 - 1;

							int x = currentPixelContiguous->next[q]->xInt + xDev;
							int y = currentPixelContiguous->next[q]->yInt + yDev;
							if((x >= 0) && (x <= (imageWidth-1)) && (y >= 0 ) && (y <= (imageHeight-1)))
							{//map boundary check
								if(alreadyProcessed[y*imageWidth + x] == EDGE_FOUND)
								{
									alreadyProcessed[y*imageWidth + x] = NOT_PROCESSED;
								}
							}
						}

						free(currentPixelContiguous->next[q]);
						currentPixelContiguous->next[q] = NULL;
					}
					else
					{
						if(!foundAPixelContiguousToParseInto)
						{
							foundAPixelContiguousToParseInto = true;
							pixelContiguousToParseInto = currentPixelContiguous->next[q];

						}
					}
				}
			}
		}
		if(foundAPixelContiguousToParseInto)
		{
			currentPixelContiguous = pixelContiguousToParseInto;
		}
		else
		{
			if(currentPixelContiguous->previous != NULL)
			{
				currentPixelContiguous->readyToDelete = true;
				currentPixelContiguous = currentPixelContiguous->previous;
			}
			else
			{
				stillMoreToDelete = false;
			}
		}
	}
}

void ATORfeatureGenerationClass::deleteContiguousStackAndResetEdgesNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack)
{
	ATORpixelContiguous* currentPixelContiguous = firstInPixelContiguousStack;
	bool stillMoreToDelete = true;
	while(stillMoreToDelete)
	{
		bool foundAPixelContiguousToParseInto = false;
		ATORpixelContiguous* pixelContiguousToParseInto;
		for(int q=0;q<9;q++)
		{
			if(q != 4)
			{
				if(currentPixelContiguous->next[q] != NULL)
				{
					if(currentPixelContiguous->next[q]->readyToDelete)
					{
						if(currentPixelContiguous->next[q]->meshPoint != NULL)
						{
							if(currentPixelContiguous->next[q]->meshPoint->alreadyProcessed == EDGE_FOUND)
							{
								currentPixelContiguous->next[q]->meshPoint->alreadyProcessed = NOT_PROCESSED;
							}
						}

						free(currentPixelContiguous->next[q]);
						currentPixelContiguous->next[q] = NULL;
					}
					else
					{
						if(!foundAPixelContiguousToParseInto)
						{
							foundAPixelContiguousToParseInto = true;
							pixelContiguousToParseInto = currentPixelContiguous->next[q];

						}
					}
				}
			}
		}
		if(foundAPixelContiguousToParseInto)
		{
			currentPixelContiguous = pixelContiguousToParseInto;
		}
		else
		{
			if(currentPixelContiguous->previous != NULL)
			{
				currentPixelContiguous->readyToDelete = true;
				currentPixelContiguous = currentPixelContiguous->previous;
			}
			else
			{
				stillMoreToDelete = false;
			}
		}
	}
}

/*
//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
void clearContiguousStackRecursive(ATORpixelContiguous* currentInPixelContiguousStack, int alreadyProcessed[], int imageWidth, int imageHeight)
{
	alreadyProcessed[(currentInPixelContiguousStack->yInt)*imageWidth + (currentInPixelContiguousStack->xInt)] = NOT_PROCESSED;

	for(int q=0; q<9; q++)
	{
		if(q != 4)
		{
			if(currentInPixelContiguousStack->next[q] != NULL)
			{
				clearContiguousStackRecursive(currentInPixelContiguousStack->next[q], alreadyProcessed, imageWidth, imageHeight);
			}
		}

	}
}
//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
void clearContiguousStackRecursive(ATORpixelContiguous* currentInPixelContiguousStack, int imageWidth, int imageHeight)
{
	currentInPixelContiguousStack->meshPoint->alreadyProcessed = NOT_PROCESSED;

	for(int q=0; q<9; q++)
	{
		if(q != 4)
		{
			if(currentInPixelContiguousStack->next[q] != NULL)
			{
				clearContiguousStackRecursive(currentInPixelContiguousStack->next[q], imageWidth, imageHeight);
			}
		}

	}
}
*/



bool ATORfeatureGenerationClass::defineRegionCheckNextPixelUsingMeshPointNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack, ATORmeshPoint* firstMeshPoint, double* regionSize, const int contrastValChosen, int* sumX, int* sumY, vec* sumPos, const bool useEdgeZeroCrossingMap, ATORmeshPoint* aMeshPointOnTheBoundary, const double sensitivity)
{
	bool unboundedContiguousRegionFound = false;
	bool contiguousRegionFound = true;

	firstInPixelContiguousStack->meshPoint = firstMeshPoint;
	ATORpixelContiguous* currentInPixelContiguousStack = firstInPixelContiguousStack;

	//shortcut to define a pixel on the boundary of the contiguous region
	double maxDistanceOfPointFromStartPoint = 0.0;

	bool stillProcessing = true;
	while(stillProcessing)
	{
		ATORmeshPoint* currentMeshPoint = currentInPixelContiguousStack->meshPoint;
		int numberOfNewBranchesFromThisPixel = 0;

		for(int q2=1; q2<9; q2=q2+2)
		{
			int q = (q+2)%8;	//required such that -x, +x crawing occurs before -y,+y crawing to enable tight zigzagging during crawl

			int xDev;
			int yDev;
			if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
			{
				xDev = 0 + q%3 - 1;
				yDev = 0 + q/3 - 1;
			}

			if(currentMeshPoint->adjacentMeshPointFilled[q])
			{
				int pixelStatus = currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed;
				if(pixelStatus == NOT_PROCESSED)
				{
					bool passCondition = false;
					if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
					{
						if(currentMeshPoint->adjacentMeshPoint[q]->edge == true)	//CHECK THIS - not yet used - need to create an edgise routine for MeshLists first..
						{
							passCondition = true;
						}
					}
					else
					{
						double contrastValUsedForComparison;
						if(contrastValChosen == CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST)
						{
							contrastValUsedForComparison = currentMeshPoint->adjacentMeshPoint[q]->luminosityContrast;
						}
						else if(contrastValChosen == CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST)
						{
							contrastValUsedForComparison = currentMeshPoint->adjacentMeshPoint[q]->meshPointNormalContrast;
						}

						if(contrastValUsedForComparison > sensitivity)
						{
							passCondition = true;
						}
					}

					if(passCondition)
					{
						currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed = EDGE_FOUND;

						if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
						{
							if(useEdgeZeroCrossingMap)
							{
								double subpixelArea = ATORquadraticFit.calculateAreaOfOneSideOfEdgeInPixel(-(xDev), -(yDev), currentMeshPoint->adjacentMeshPoint[q]->zeroCrossingValueX, currentMeshPoint->adjacentMeshPoint[q]->zeroCrossingValueY, currentMeshPoint->adjacentMeshPoint[q]->alpha);
								#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY
								*regionSize = *regionSize + subpixelArea;
								#endif

								//now add subpixel area for a diagonal adjacent pixels if necessary
								int xAdjacentDev1;
								int yAdjacentDev1;
								int xAdjacentDev2;
								int yAdjacentDev2;
								int xDiagDev1;
								int yDiagDev1;
								int xDiagDev2;
								int yDiagDev2;

								int qDiag1;
								int qDiag2;
								int rAdj1;
								int rAdj2;

								if((xDev > 0) || (xDev < 0))
								{
									xDiagDev1 = 0;
									yDiagDev1 = 1;
									xDiagDev2 = 0;
									yDiagDev2 = -1;
									xAdjacentDev1 = xDev;
									yAdjacentDev1 = yDiagDev1;
									xAdjacentDev2 = xDev;
									yAdjacentDev2 = yDiagDev2;
									qDiag1 = 2;
									qDiag1 = 3;
									rAdj1 = 2;
									rAdj2 = 3;
								}
								else
								{
									xDiagDev1 = 1;
									yDiagDev1 = 0;
									xDiagDev2 = -1;
									yDiagDev2 = 0;
									xAdjacentDev1 = xDiagDev1;
									yAdjacentDev1 = yDev;
									xAdjacentDev2 = xDiagDev2;
									yAdjacentDev2 = yDev;
									qDiag1 = 0;
									qDiag1 = 1;
									rAdj1 = 0;
									rAdj2 = 1;
								}

								if(currentMeshPoint->adjacentMeshPoint[qDiag1]->edge == true)
								{
									if((currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj1]->alreadyProcessed == NOT_PROCESSED) && (currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj1]->edge == true))
									{
										currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj1]->alreadyProcessed = EDGE_FOUND;
										double subpixelAreaAdjacent = ATORquadraticFit.calculateAreaOfOneSideOfEdgeInPixel(-xAdjacentDev1, -yAdjacentDev1, currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj1]->zeroCrossingValueX, currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj1]->zeroCrossingValueY, currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj1]->alpha);
										#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY
										*regionSize = *regionSize + subpixelAreaAdjacent;
										#endif
									}
								}
								if(currentMeshPoint->adjacentMeshPoint[qDiag2]->edge == true)
								{
									if((currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj2]->alreadyProcessed == NOT_PROCESSED) && (currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj2]->edge == true))
									{
										currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj2]->alreadyProcessed = EDGE_FOUND;
										double subpixelAreaAdjacent = ATORquadraticFit.calculateAreaOfOneSideOfEdgeInPixel(-xAdjacentDev2, -yAdjacentDev2, currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj2]->zeroCrossingValueX, currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj2]->zeroCrossingValueY, currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[rAdj2]->alpha);
										#ifdef ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_EXTRA_NOT_NECESSARY_AND_BUGGY
										*regionSize = *regionSize + subpixelAreaAdjacent;
										#endif
									}
								}
							}
						}
					}
					else
					{
						ATORpixelContiguous* newPixel = new ATORpixelContiguous(currentMeshPoint->adjacentMeshPoint[q], currentInPixelContiguousStack);
						currentInPixelContiguousStack->next[q2] = newPixel;
						*regionSize = *regionSize + 1;
						currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed = NO_EDGE_FOUND;
						numberOfNewBranchesFromThisPixel++;

						*sumX = *sumX + currentMeshPoint->adjacentMeshPoint[q]->xInt;
						*sumY = *sumY + currentMeshPoint->adjacentMeshPoint[q]->yInt;
						sumPos->x = sumPos->x + currentMeshPoint->adjacentMeshPoint[q]->point.x;
						sumPos->y = sumPos->y + currentMeshPoint->adjacentMeshPoint[q]->point.y;
						sumPos->z = sumPos->z + currentMeshPoint->adjacentMeshPoint[q]->point.z;

						//shortcut to define a pixel on the boundary of the contiguous region
						vec difference;
						SHAREDvector.subtractVectors(&difference, &(currentMeshPoint->adjacentMeshPoint[q]->point), &(firstMeshPoint->point));
						if(SHAREDvector.findMagnitudeOfVector(&difference) > maxDistanceOfPointFromStartPoint)
						{
							maxDistanceOfPointFromStartPoint = SHAREDvector.findMagnitudeOfVector(&difference);
							aMeshPointOnTheBoundary = currentMeshPoint->adjacentMeshPoint[q];
						}

						//no kernel boundary check
					}
				}
				else if(pixelStatus == NO_EDGE_FOUND)
				{
					//stop crawl here
				}
				else if(pixelStatus == EDGE_FOUND)
				{
					//stop crawl here
				}
			}
			else
			{//boundary of mesh detected
				contiguousRegionFound = false;
				unboundedContiguousRegionFound = true;
			}

		}

		bool foundAReferenceToCrawlTo = false;
		for(int q=1; q<9; q=q+2)
		{
			if((!foundAReferenceToCrawlTo) && (currentInPixelContiguousStack->next[q] != NULL))
			{
				if(currentInPixelContiguousStack->next[q]->pathAlreadyCrawled != true)
				{
					bool nextIsNotPrevious = true;
					if(currentInPixelContiguousStack->previous != NULL)
					{
						if(SHAREDvector.compareVectors(&(currentInPixelContiguousStack->next[q]->meshPoint->point), &(currentInPixelContiguousStack->previous->meshPoint->point)))
						{
							nextIsNotPrevious = false;
						}
					}

					if(nextIsNotPrevious)
					{
						foundAReferenceToCrawlTo = true;
						currentInPixelContiguousStack = currentInPixelContiguousStack->next[q];
					}
				}
			}
		}
		if(!foundAReferenceToCrawlTo)
		{
			currentInPixelContiguousStack->pathAlreadyCrawled = true;
			if(currentInPixelContiguousStack->previous != NULL)
			{
				currentInPixelContiguousStack = currentInPixelContiguousStack->previous;
			}
			else
			{
				stillProcessing = false;
			}
		}
	}

	#ifdef ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OLD
	if(unboundedContiguousRegionFound)
	{
		//must remove all edges from list
		ATORmeshPoint* currentMeshPoint = firstMeshPoint;
		while(currentMeshPoint->next != NULL)
		{
			if(currentMeshPoint->alreadyProcessed == EDGE_FOUND)
			{
				currentMeshPoint->alreadyProcessed = NOT_PROCESSED;
			}
			currentMeshPoint = currentMeshPoint->next;
		}
	}
	else if(!contiguousRegionFound)
	{
		cout << "contigous region not found - it is most efficient to restore all edge pixels in contigous stack" << endl;
		clearContiguousStackRecursive(firstInPixelContiguousStack, imageWidth, imageHeight);
	}
	#endif


	return contiguousRegionFound;

}






bool ATORfeatureGenerationClass::traceEdgeCheckNextPixelNonRecursive(int xInitialOnBoundary, int yInitialOnBoundary, int alreadyProcessed[], constEffective ATORQFzeroCrossing* edgeZeroCrossingMap[], const bool useEdgeZeroCrossingMap, int imageWidth, const int imageHeight, double* sumX, double* sumY, vec* sumPos, int* numberOfCounts, const int minRegionCircumferenceForRegion, const int dimension, double pointMap[], const double depthMap[], int zoom, ATORpixelContiguous* firstInPixelContiguousBoundaryStack, RTviewInfo* vi, const int interpixelContrastMapType)
{
	double pixelXOffset;
	double pixelYOffset;
	int kernelWidthForegroundCheck;
	int kernelHeightForegroundCheck;
	if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
	{
		//without ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
	{
		//such as luminosityContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0.5;
		pixelYOffset = 0.5;
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL;
	}
	else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
	{
		//such as gradientContrast or pointNormalContrast map with ATOR_USE_CONTRAST_CALC_METHOD_C
		pixelXOffset = 0;
		pixelYOffset = 0;
		kernelWidthForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL;
		kernelHeightForegroundCheck = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_NO_INTERPIXEL;
	}


	bool foundATracePath1 = false;

	vec initialPointOnBoundary;
	initialPointOnBoundary.x = xInitialOnBoundary;
	initialPointOnBoundary.y = yInitialOnBoundary;
	initialPointOnBoundary.z = 0.0;


	firstInPixelContiguousBoundaryStack->xInt = xInitialOnBoundary;
	firstInPixelContiguousBoundaryStack->yInt = yInitialOnBoundary;
	double xposInitial;
	double yposInitial;
	if(useEdgeZeroCrossingMap)
	{
		xposInitial = pixelXOffset + firstInPixelContiguousBoundaryStack->xInt + edgeZeroCrossingMap[firstInPixelContiguousBoundaryStack->yInt*imageWidth + firstInPixelContiguousBoundaryStack->xInt]->zeroCrossingValueX;
		yposInitial = pixelYOffset + firstInPixelContiguousBoundaryStack->yInt + edgeZeroCrossingMap[firstInPixelContiguousBoundaryStack->yInt*imageWidth + firstInPixelContiguousBoundaryStack->xInt]->zeroCrossingValueY;

	}
	else
	{
		xposInitial = pixelXOffset + firstInPixelContiguousBoundaryStack->xInt;
		yposInitial = pixelYOffset + firstInPixelContiguousBoundaryStack->yInt;

	}
	*sumX = *sumX + xposInitial;
	*sumY = *sumY + yposInitial;
	firstInPixelContiguousBoundaryStack->pointNonWorldCoord.x = xposInitial;
	firstInPixelContiguousBoundaryStack->pointNonWorldCoord.y = yposInitial;

	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
			vec xyzWorld;

			if(ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE)
			{

				xyzWorld.x = edgeZeroCrossingMap[firstInPixelContiguousBoundaryStack->yInt*imageWidth + firstInPixelContiguousBoundaryStack->xInt]->point.x;
				xyzWorld.y = edgeZeroCrossingMap[firstInPixelContiguousBoundaryStack->yInt*imageWidth + firstInPixelContiguousBoundaryStack->xInt]->point.y;
				xyzWorld.z = edgeZeroCrossingMap[firstInPixelContiguousBoundaryStack->yInt*imageWidth + firstInPixelContiguousBoundaryStack->xInt]->point.z;
			}
			else
			{
				#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
					#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
						vec xyzNearbyPointOnObject;
						double depth = ATORpixelMaps.calculateForegroundMinimumDepthWithinKernel(int(xposInitial), int(yposInitial), imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, &xyzNearbyPointOnObject, zoom);
					#else
						double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(int(xposInitial), int(yposInitial), imageWidth, depthMap);
					#endif

					RTscene.calculatePointMapValue(xposInitial, yposInitial, depth, &xyzWorld, vi);

				#else
					cout << "error: traceEdgeCheckNextPixelRecursive{} requires ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH when compiling with 3DOD/use3DODonObjectForegroundDepthCheck" << endl;

					RTpixelMaps.getPointMapValue(int(xposInitial), int(yposInitial), imageWidth, pointMap, &xyzWorld);
				#endif

			}

			sumPos->x = sumPos->x + xyzWorld.x;
			sumPos->y = sumPos->y + xyzWorld.y;
			sumPos->z = sumPos->z + xyzWorld.z;
			firstInPixelContiguousBoundaryStack->point.x = xyzWorld.x;
			firstInPixelContiguousBoundaryStack->point.y = xyzWorld.y;
			firstInPixelContiguousBoundaryStack->point.z = xyzWorld.z;
	}


	ATORpixelContiguous* currentInPixelContiguousStack = firstInPixelContiguousBoundaryStack;

	int numberOfBoundaryOrFakeBoundaryPixelsTraced = 0;
	bool stillTracingPath1 = true;
	while(stillTracingPath1)
	{

		int xCurrent = currentInPixelContiguousStack->xInt;
		int yCurrent = currentInPixelContiguousStack->yInt;
		int numberOfNewBranchesFromThisPixel = 0;

		currentInPixelContiguousStack->pathAlreadyCrawled = true;
		alreadyProcessed[yCurrent*imageWidth + xCurrent] = NOT_PROCESSED;

		#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
		if(numberOfBoundaryOrFakeBoundaryPixelsTraced != 0)
		{
			contiguousRegionDebugrgbMap[yCurrent*imageWidth*RGB_NUM + xCurrent*RGB_NUM + 0] = 0;
			contiguousRegionDebugrgbMap[yCurrent*imageWidth*RGB_NUM + xCurrent*RGB_NUM + 1] = 255;
			contiguousRegionDebugrgbMap[yCurrent*imageWidth*RGB_NUM + xCurrent*RGB_NUM + 2] = 0;
			//generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
		}
		//cout << "xyCurrent = " << xCurrent << "  " << yCurrent << endl;
		#endif

		vec currentPoint;
		currentPoint.x = xCurrent;
		currentPoint.y = yCurrent;
		currentPoint.z = 0.0;
		if((numberOfBoundaryOrFakeBoundaryPixelsTraced > minRegionCircumferenceForRegion) && (SHAREDvector.compareVectors(&currentPoint, &initialPointOnBoundary)))
		{
			stillTracingPath1 = false;
			foundATracePath1 = true;
		}
		else
		{
			int q2Min;
			int q2Max;
			int q2Increment;
			if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
			{
				q2Min = 0;
				q2Max = 9;
				q2Increment = 1;
			}
			else
			{
				q2Min = 1;
				q2Max = 9;
				q2Increment = 2;
			}

			for(int q2=q2Min; q2<q2Max; q2=q2+q2Increment)
			{
				bool passCondition = true;

				int q;
				if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
				{
					//try adjacents first, then diagonals
					bool skip = false;
					if(q2 < 4)
					{
						q = q2*2 + 1;	//1, 3, 5, 7
					}
					else
					{
						q = q2*2 - 8;	//0, 2, 4, 6, 8
					}

					if(q == 4)
					{
						skip = true;
					}
					if(skip)
					{
						passCondition = false;
					}

				}
				else
				{
					q = q2;
				}

				if(passCondition)
				{

					int x;
					int y;

					x = xCurrent + q%3 - 1;
					y = yCurrent + q/3 - 1;

					vec currentPointAdjacentPoint;
					currentPointAdjacentPoint.x = x;
					currentPointAdjacentPoint.y = y;
					currentPointAdjacentPoint.z = 0.0;

					/*
					#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 255;
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
					contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 255;
					//generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
					#endif
					*/

					bool nextIsNotPrevious = true;
					if(currentInPixelContiguousStack->previous != NULL)
					{
						if((x == currentInPixelContiguousStack->previous->xInt) && (y == currentInPixelContiguousStack->previous->yInt))
						{
							nextIsNotPrevious = false;
						}
					}
					bool nextHasNotAlreadyBeenCrawled = true;
					if(currentInPixelContiguousStack->next[q] != NULL)
					{

						if(currentInPixelContiguousStack->next[q]->pathAlreadyCrawled == true)
						{
							nextHasNotAlreadyBeenCrawled = false;

						}
					}

					bool nextIsInitial = false;
					if((numberOfBoundaryOrFakeBoundaryPixelsTraced > minRegionCircumferenceForRegion) && (SHAREDvector.compareVectors(&currentPointAdjacentPoint, &initialPointOnBoundary)))
					{
						nextIsInitial = true;
						stillTracingPath1 = false;
						foundATracePath1 = true;

					}


					if(((alreadyProcessed[y*imageWidth + x] == EDGE_FOUND) && nextIsNotPrevious && nextHasNotAlreadyBeenCrawled) || nextIsInitial)	//add 2 initial points to the list
					{
						#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
						//cout << "q = " << q << endl;
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 0] = 255;
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 1] = 0;
						contiguousRegionDebugrgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + 2] = 0;
						//generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
						#endif

						numberOfBoundaryOrFakeBoundaryPixelsTraced++;
						ATORpixelContiguous* newPixel = new ATORpixelContiguous(x, y, currentInPixelContiguousStack);
						currentInPixelContiguousStack->next[q] = newPixel;
						numberOfNewBranchesFromThisPixel++;

						double xpos;
						double ypos;
						if(useEdgeZeroCrossingMap)
						{
							xpos = pixelXOffset + newPixel->xInt + edgeZeroCrossingMap[newPixel->yInt*imageWidth + newPixel->xInt]->zeroCrossingValueX;
							ypos = pixelYOffset + newPixel->yInt + edgeZeroCrossingMap[newPixel->yInt*imageWidth + newPixel->xInt]->zeroCrossingValueY;

						}
						else
						{
							xpos = pixelXOffset + newPixel->xInt;
							ypos = pixelYOffset + newPixel->yInt;

						}
						*sumX = *sumX + xpos;
						*sumY = *sumY + ypos;
						newPixel->pointNonWorldCoord.x = xpos;
						newPixel->pointNonWorldCoord.y = ypos;

						if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
							vec xyzWorld;

							if(ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE)
							{
								xyzWorld.x = edgeZeroCrossingMap[newPixel->yInt*imageWidth + newPixel->xInt]->point.x;
								xyzWorld.y = edgeZeroCrossingMap[newPixel->yInt*imageWidth + newPixel->xInt]->point.y;
								xyzWorld.z = edgeZeroCrossingMap[newPixel->yInt*imageWidth + newPixel->xInt]->point.z;
							}
							else
							{
								#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
									#ifdef ATOR_USE_FOREGROUND_DEPTH_CHECKS
										vec xyzNearbyPointOnObject;
										double depth = 	ATORpixelMaps.calculateForegroundMinimumDepthWithinKernel(int(xpos), int(ypos), imageWidth, imageHeight, kernelWidthForegroundCheck, kernelHeightForegroundCheck, depthMap, &xyzNearbyPointOnObject, zoom);
									#else
										double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(int(xpos)*zoom, int(ypos)*zoom, imageWidth*zoom, depthMap);
									#endif

									RTscene.calculatePointMapValue(xpos*zoom, ypos*zoom, depth, &xyzWorld, vi);

								#else
									cout << "error: traceEdgeCheckNextPixelRecursive{} requires ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH when compiling with 3DOD/use3DODonObjectForegroundDepthCheck" << endl;

									RTpixelMaps.getPointMapValue(int(xpos)*zoom, int(ypos)*zoom, imageWidth*zoom, pointMap, &xyzWorld);
								#endif

							}

							sumPos->x = sumPos->x + xyzWorld.x;
							sumPos->y = sumPos->y + xyzWorld.y;
							sumPos->z = sumPos->z + xyzWorld.z;
							newPixel->point.x = xyzWorld.x;
							newPixel->point.y = xyzWorld.y;
							newPixel->point.z = xyzWorld.z;
						}


						if((x > 0) && (x < (imageWidth-1)) && (y > 0) && (y < (imageHeight-1)))
						{//kernel boundary check (needs 1 pixels broth)

						}
						else
						{
							cerr << "error - boundary found in trace" << endl;
							exit(EXIT_ERROR);

							//?:
							currentInPixelContiguousStack->next[q]->pathAlreadyCrawled = true;
							//end of crawl line
						}

					}
					else
					{
						/*
						if(currentInPixelContiguousStack->next[q] != NULL)
						{
							currentInPixelContiguousStack->next[q]->pathAlreadyCrawled = true;
						}
						*/
					}
				}
			}
		}

		if(foundATracePath1)
		{

		}
		else
		{
			bool foundAReferenceToCrawlTo = false;

			int q2Min;
			int q2Max;
			int q2Increment;
			if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
			{
				q2Min = 0;
				q2Max = 9;
				q2Increment = 1;
			}
			else
			{
				q2Min = 1;
				q2Max = 9;
				q2Increment = 2;
			}


			for(int q2=q2Min; q2<q2Max; q2=q2+q2Increment)
			{
				bool passCondition = true;
				int q;
				if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
				{
					//try adjacents first, then diagonals
					int skip = false;
					if(q2 < 4)
					{
						q = q2*2 + 1;	//1, 3, 5, 7
					}
					else
					{
						q = q2*2 - 8;	//0, 2, 4, 6, 8
					}
					if(q == 4)
					{
						skip = true;
					}
					if(skip)
					{
						passCondition = false;
					}
				}
				else
				{
					q = q2;
				}

				if(passCondition)
				{

					if(currentInPixelContiguousStack->next[q] != NULL)
					{
						vec currentPointAdjacentPoint;
						currentPointAdjacentPoint.x = currentInPixelContiguousStack->next[q]->xInt;
						currentPointAdjacentPoint.y = currentInPixelContiguousStack->next[q]->yInt;
						currentPointAdjacentPoint.z = 0.0;
						if((numberOfBoundaryOrFakeBoundaryPixelsTraced > minRegionCircumferenceForRegion) && (SHAREDvector.compareVectors(&currentPointAdjacentPoint, &initialPointOnBoundary)))
						{
							stillTracingPath1 = false;
							foundATracePath1 = true;
							foundAReferenceToCrawlTo = true;
							#ifdef NEWCHECKTHIS
							currentInPixelContiguousStack->finalPathAlreadyCrawled = true;
							#endif
						}
						else
						{
							if(!foundAReferenceToCrawlTo)
							{
								if(currentInPixelContiguousStack->next[q]->pathAlreadyCrawled != true)
								{
									//done above;
									bool nextIsNotPrevious = true;
									if(currentInPixelContiguousStack->previous != NULL)
									{
										if((currentInPixelContiguousStack->next[q]->xInt == currentInPixelContiguousStack->previous->xInt) && (currentInPixelContiguousStack->next[q]->yInt == currentInPixelContiguousStack->previous->yInt))
										{
											nextIsNotPrevious = false;
										}
									}
									if(nextIsNotPrevious)
									{
										foundAReferenceToCrawlTo = true;
										currentInPixelContiguousStack = currentInPixelContiguousStack->next[q];

									}
								}
							}
						}
					}
					else
					{
						/*
						if(currentInPixelContiguousStack->next[q] == NULL)
						{
							cout << "currentInPixelContiguousStack->next[q] == NULL" << endl;
						}
						if(!foundAReferenceToCrawlTo)
						{
							cout << "(!foundAReferenceToCrawlTo)" << endl;
						}
						cout << "2. q = " << q << endl;
						*/
					}

				}

			}
			if(!foundAReferenceToCrawlTo)
			{

				#ifdef ATOR_CONTIGUOUS_REGION_VERBOSE
				//cout << "retracing step; "<< endl;
				contiguousRegionDebugrgbMap[currentInPixelContiguousStack->yInt*imageWidth*RGB_NUM + currentInPixelContiguousStack->xInt*RGB_NUM + 0] = 200;
				contiguousRegionDebugrgbMap[currentInPixelContiguousStack->yInt*imageWidth*RGB_NUM + currentInPixelContiguousStack->xInt*RGB_NUM + 1] = 200;
				contiguousRegionDebugrgbMap[currentInPixelContiguousStack->yInt*imageWidth*RGB_NUM + currentInPixelContiguousStack->xInt*RGB_NUM + 2] = 200;
				//generatePixmapFromRGBmap("debugContiguousRegionDetectionRGB.ppm", imageWidth, imageHeight, contiguousRegionDebugrgbMap);
				//cout << "currentInPixelContiguousStack->xInt = " << currentInPixelContiguousStack->xInt << endl;
				//cout << "currentInPixelContiguousStack->yInt = " << currentInPixelContiguousStack->yInt << endl;
				#endif


				currentInPixelContiguousStack->finalPathAlreadyCrawled = true;
				currentInPixelContiguousStack->pathAlreadyCrawled = true;
				if(currentInPixelContiguousStack->previous != NULL)
				{
					currentInPixelContiguousStack = currentInPixelContiguousStack->previous;
				}
				else
				{
					cerr << "1. error - returned to beginning without tracing a boundary" << endl;
					cerr << "currentInPixelContiguousStack->xInt = " << currentInPixelContiguousStack->xInt << endl;
					cerr << "currentInPixelContiguousStack->yInt = " << currentInPixelContiguousStack->yInt << endl;
					exit(EXIT_ERROR);
					stillTracingPath1 = false;
				}
			}
		}

	}


	*numberOfCounts = numberOfBoundaryOrFakeBoundaryPixelsTraced;

	return foundATracePath1;

}



//now trace edges instead of 1 pixel inside of edge
bool ATORfeatureGenerationClass::traceEdgeCheckNextPixelUsingMeshPointNonRecursive(ATORmeshPoint* aMeshPointOnTheBoundary, double* sumX, double* sumY, vec* sumPos, int* numberOfCounts, const int minRegionCircumferenceForRegion, ATORpixelContiguous* firstInPixelContiguousBoundaryStack, const bool useEdgeZeroCrossingMap)
{
	bool foundATracePath1 = false;

	firstInPixelContiguousBoundaryStack->meshPoint = aMeshPointOnTheBoundary;

	vec xyzWorld;
	vec nonWorld;

	if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
	{
		if(useEdgeZeroCrossingMap)
		{
			xyzWorld.x = firstInPixelContiguousBoundaryStack->meshPoint->quadraticFitPoint.x;
			xyzWorld.y = firstInPixelContiguousBoundaryStack->meshPoint->quadraticFitPoint.y;
			xyzWorld.z = firstInPixelContiguousBoundaryStack->meshPoint->quadraticFitPoint.z;
			nonWorld.x = firstInPixelContiguousBoundaryStack->meshPoint->xInt + firstInPixelContiguousBoundaryStack->meshPoint->zeroCrossingValueX;
			nonWorld.y = firstInPixelContiguousBoundaryStack->meshPoint->yInt + firstInPixelContiguousBoundaryStack->meshPoint->zeroCrossingValueY;
		}
	}
	else
	{
		xyzWorld.x = firstInPixelContiguousBoundaryStack->meshPoint->point.x;
		xyzWorld.y = firstInPixelContiguousBoundaryStack->meshPoint->point.y;
		xyzWorld.z = firstInPixelContiguousBoundaryStack->meshPoint->point.z;

		nonWorld.x = firstInPixelContiguousBoundaryStack->meshPoint->xInt;
		nonWorld.y = firstInPixelContiguousBoundaryStack->meshPoint->yInt;
	}

	sumPos->x = sumPos->x + xyzWorld.x;
	sumPos->y = sumPos->y + xyzWorld.y;
	sumPos->z = sumPos->z + xyzWorld.z;

	*sumX = *sumX + nonWorld.x;
	*sumY = *sumY + nonWorld.y;

	firstInPixelContiguousBoundaryStack->point.x = xyzWorld.x;
	firstInPixelContiguousBoundaryStack->point.y = xyzWorld.y;
	firstInPixelContiguousBoundaryStack->point.z = xyzWorld.z;

	firstInPixelContiguousBoundaryStack->pointNonWorldCoord.x = nonWorld.x;
	firstInPixelContiguousBoundaryStack->pointNonWorldCoord.y = nonWorld.y;


	ATORpixelContiguous* currentInPixelContiguousStack = firstInPixelContiguousBoundaryStack;

	int numberOfBoundaryOrFakeBoundaryPixelsTraced = 0;

	bool stillTracingPath1 = true;
	while(stillTracingPath1)
	{
		ATORmeshPoint* currentMeshPoint = currentInPixelContiguousStack->meshPoint;

		currentInPixelContiguousStack->pathAlreadyCrawled = true;
		currentInPixelContiguousStack->meshPoint->alreadyProcessed = NOT_PROCESSED;

		int numberOfNewBranchesFromThisPixel = 0;

		if((numberOfBoundaryOrFakeBoundaryPixelsTraced > minRegionCircumferenceForRegion) && (SHAREDvector.compareVectors(&(currentInPixelContiguousStack->meshPoint->point), &(aMeshPointOnTheBoundary->point))))
		{
			stillTracingPath1 = false;
			foundATracePath1 = true;
		}
		else
		{
			int q2Min;
			int q2Max;
			int q2Increment;
			if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
			{
				q2Min = 0;
				q2Max = 9;
				q2Increment = 1;
			}
			else
			{
				q2Min = 1;
				q2Max = 9;
				q2Increment = 2;
			}

			for(int q2=q2Min; q2<q2Max; q2=q2+q2Increment)
			{
				bool passCondition = true;

				int q;
				if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
				{
					//try adjacents first, then diagonals
					int q;
					bool skip = false;
					if(q2 < 4)
					{
						q = q2*2 + 1;	//1, 3, 5, 7
					}
					else
					{
						q = q2*2 - 8;	//0, 2, 4, 6, 8
					}
					if(q == 4)
					{
						skip = true;
					}
					if(skip)
					{
						passCondition = false;
					}

				}
				else
				{
					q = q2;
				}

				if(passCondition)
				{

					if(currentMeshPoint->adjacentMeshPointFilled[q])
					{

						bool nextIsNotPrevious = true;
						if(currentInPixelContiguousStack->previous != NULL)
						{
							if(SHAREDvector.compareVectors(&(currentMeshPoint->adjacentMeshPoint[q]->point), &(currentInPixelContiguousStack->previous->point)))	//OR;	if(SHAREDvector.compareVectors(&(currentInPixelContiguousStack->next[q]->point), &(currentInPixelContiguousStack->previous->point)))
							{
								nextIsNotPrevious = false;
							}
						}
						bool nextHasNotAlreadyBeenCrawled = true;
						if(currentInPixelContiguousStack->next[q] != NULL)
						{
							if(currentInPixelContiguousStack->next[q]->pathAlreadyCrawled)
							{
								nextHasNotAlreadyBeenCrawled = false;
							}
						}
						bool nextIsInitial = false;
						if((numberOfBoundaryOrFakeBoundaryPixelsTraced > minRegionCircumferenceForRegion) && (SHAREDvector.compareVectors(&(currentInPixelContiguousStack->next[q]->point),  &(aMeshPointOnTheBoundary->point))))
						{
							nextIsInitial = true;
							stillTracingPath1 = false;
							foundATracePath1 = true;
						}

						if(((currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed == EDGE_FOUND) && nextIsNotPrevious && nextHasNotAlreadyBeenCrawled) || nextIsInitial)	//add 2 initial points to the list
						{

							numberOfBoundaryOrFakeBoundaryPixelsTraced++;
							ATORpixelContiguous* newPixel = new ATORpixelContiguous(currentMeshPoint->adjacentMeshPoint[q], currentInPixelContiguousStack);
							currentMeshPoint->adjacentMeshPoint[q]->hasBeenProcessedInGenerationOfOuterBoundary = true;
							currentInPixelContiguousStack->next[q] = newPixel;
							numberOfNewBranchesFromThisPixel++;


							if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
							{
								if(useEdgeZeroCrossingMap)
								{
									xyzWorld.x = newPixel->meshPoint->quadraticFitPoint.x;
									xyzWorld.y = newPixel->meshPoint->quadraticFitPoint.y;
									xyzWorld.z = newPixel->meshPoint->quadraticFitPoint.z;
									nonWorld.x = newPixel->meshPoint->xInt + newPixel->meshPoint->zeroCrossingValueX;
									nonWorld.y = newPixel->meshPoint->yInt + newPixel->meshPoint->zeroCrossingValueY;


								}
							}
							else
							{

								xyzWorld.x = newPixel->meshPoint->point.x;
								xyzWorld.y = newPixel->meshPoint->point.y;
								xyzWorld.z = newPixel->meshPoint->point.z;

								nonWorld.x = newPixel->meshPoint->xInt;
								nonWorld.y = newPixel->meshPoint->yInt;
							}

							sumPos->x = sumPos->x + xyzWorld.x;
							sumPos->y = sumPos->y + xyzWorld.y;
							sumPos->z = sumPos->z + xyzWorld.z;

							*sumX = *sumX + nonWorld.x;
							*sumY = *sumY + nonWorld.y;

							newPixel->point.x = xyzWorld.x;
							newPixel->point.y = xyzWorld.y;
							newPixel->point.z = xyzWorld.z;

							newPixel->pointNonWorldCoord.x = nonWorld.x;
							newPixel->pointNonWorldCoord.y = nonWorld.y;

						}
						else
						{
							/*
							if(currentInPixelContiguousStack->next[q] != NULL)
							{
								currentInPixelContiguousStack->next[q]->pathAlreadyCrawled = true;
							}
							*/

						}

					}
				}

			}
		}

		if(foundATracePath1)
		{

		}
		else
		{

			bool foundAReferenceToCrawlTo = false;

			int q2Min;
			int q2Max;
			int q2Increment;
			if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
			{
				q2Min = 0;
				q2Max = 9;
				q2Increment = 1;
			}
			else
			{
				q2Min = 1;
				q2Max = 9;
				q2Increment = 2;
			}

			for(int q2=q2Min; q2<q2Max; q2=q2+q2Increment)
			{
				bool passCondition = true;

				int q;
				if(ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS)
				{				//try adjacents first, then diagonals
					int q;
					bool skip;
					if(q2 < 4)
					{
						q = q2*2 + 1;	//1, 3, 5, 7
					}
					else
					{
						q = q2*2 - 8;	//0, 2, 4, 6, 8
					}
					if(q == 4)
					{
						skip = true;
					}
					if(skip)
					{
						passCondition = false;
					}
				}
				else
				{
					q = q2;
				}

				if(passCondition)
				{

					if(currentInPixelContiguousStack->next[q] != NULL)
					{
						if((numberOfBoundaryOrFakeBoundaryPixelsTraced > minRegionCircumferenceForRegion) && (SHAREDvector.compareVectors(&(currentInPixelContiguousStack->next[q]->meshPoint->point), &(aMeshPointOnTheBoundary->point))))
						{
							stillTracingPath1 = false;
							foundATracePath1 = true;
							foundAReferenceToCrawlTo = true;
						}
						else
						{
							if(!foundAReferenceToCrawlTo)
							{
								if(currentInPixelContiguousStack->next[q]->pathAlreadyCrawled != true)
								{
									if(currentInPixelContiguousStack->next[q]->meshPoint->hasBeenProcessedInGenerationOfOuterBoundary != true)
									{
										//done above;
										bool nextIsNotPrevious = true;
										if(currentInPixelContiguousStack->previous != NULL)
										{
											if(SHAREDvector.compareVectors(&(currentInPixelContiguousStack->next[q]->meshPoint->point), &(currentInPixelContiguousStack->previous->meshPoint->point)))
											{
												nextIsNotPrevious = false;
											}
										}
										if(nextIsNotPrevious)
										{

											foundAReferenceToCrawlTo = true;
											currentInPixelContiguousStack = currentInPixelContiguousStack->next[q];
										}
									}
								}
							}
						}
					}
				}
			}

			if(!foundAReferenceToCrawlTo)
			{
				currentInPixelContiguousStack->finalPathAlreadyCrawled = true;
				currentInPixelContiguousStack->pathAlreadyCrawled = true;
				if(currentInPixelContiguousStack->previous != NULL)
				{
					currentInPixelContiguousStack = currentInPixelContiguousStack->previous;
				}
				else
				{
					cerr << "1. error - returned to beginning without tracing a boundary" << endl;
					exit(EXIT_ERROR);
					stillTracingPath1 = false;
				}
			}
		}
	}

	*numberOfCounts = numberOfBoundaryOrFakeBoundaryPixelsTraced;

	return foundATracePath1;
}








void ATORfeatureGenerationClass::generateFeatureListFromHeitgerFeatureRGBMap(ATORfeature* firstFeatureInFeatureList, uchar* featureRgbMap, int imageWidth, const int imageHeight, const uchar* rgbMap, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi)
{
	ATORfeature* currentFeatureInList = firstFeatureInFeatureList;

	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			double featureProbabilityLevelCentrePixel = RTpixelMaps.calculateLuminosityLevelFromRGBMap(x, y, imageWidth, featureRgbMap);
			if(featureProbabilityLevelCentrePixel >= HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD*sensitivity)
			{//centre pixel feature detection threshold passed, now calculate surrounding/kernel feature detection threshold level


				double featureProbabilityLevelKernelPixels = 0.0;
				bool centreFeatureFound = true;

				vec xyzNearbyPointOnObject;
				xyzNearbyPointOnObject.x = 0.0;
				xyzNearbyPointOnObject.y = 0.0;
				xyzNearbyPointOnObject.z = 0.0;

				bool nearbyPointOnObjectFound = false;
				double minDepthForNearbyPoints = REALLY_FAR_AWAY;

				for(int ky = y-(HEITGER_FEATURE_INTERPRETATION_KERNEL_HEIGHT/2); ky <= y+(HEITGER_FEATURE_INTERPRETATION_KERNEL_HEIGHT/2); ky++)
				{
					for(int kx = x-(HEITGER_FEATURE_INTERPRETATION_KERNEL_WIDTH/2); kx <= x+(HEITGER_FEATURE_INTERPRETATION_KERNEL_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
								{
									double depthVal = RTpixelMaps.getLumOrContrastOrDepthMapValue(kx*zoom, ky*zoom, imageWidth*zoom, depthMap);

								#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
									if(!SHAREDvars.compareDoubles(depthVal, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [2 june 09] {ensures the pixel is not a background pixel}
									{
										xyzNearbyPointOnObject.x = kx*zoom;
										xyzNearbyPointOnObject.y = ky*zoom;
										xyzNearbyPointOnObject.z = depthVal;
										nearbyPointOnObjectFound = true;
									}
								#else
									if(depthVal < minDepthForNearbyPoints)
									{
										minDepthForNearbyPoints = depthVal;
										xyzNearbyPointOnObject.x = kx*zoom;
										xyzNearbyPointOnObject.y = ky*zoom;
										xyzNearbyPointOnObject.z = depthVal;
										nearbyPointOnObjectFound = true;
									}
								#endif
								}

								double currentFeatureProbabilityLevelKernelPixel = RTpixelMaps.calculateLuminosityLevelFromRGBMap(kx, ky, imageWidth, featureRgbMap);
								featureProbabilityLevelKernelPixels = featureProbabilityLevelKernelPixels + currentFeatureProbabilityLevelKernelPixel;

								if(currentFeatureProbabilityLevelKernelPixel > featureProbabilityLevelCentrePixel)
								{
									centreFeatureFound = false;
								}
							}
						}
					}
				}

				if(centreFeatureFound)
				{
					if(featureProbabilityLevelKernelPixels >= HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD*sensitivity)
					{

						if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
							double depthVal = RTpixelMaps.getLumOrContrastOrDepthMapValue(x*zoom, y*zoom, imageWidth*zoom, depthMap);

						#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
							if(SHAREDvars.compareDoubles(depthVal, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [2 june 09] {ensures the pixel is not a background pixel}
							{//off object

								if(nearbyPointOnObjectFound)
								{
						#else
								if(minDepthForNearbyPoints < depthVal)
								{
						#endif
									currentFeatureInList->xViewport = x*zoom;
									currentFeatureInList->yViewport = y*zoom;

									currentFeatureInList->pointNonWorldCoord.x = xyzNearbyPointOnObject.x;
									currentFeatureInList->pointNonWorldCoord.y = xyzNearbyPointOnObject.y;
									currentFeatureInList->pointNonWorldCoord.z = xyzNearbyPointOnObject.z;

									vec xyzWorld;
								#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
									RTscene.calculatePointMapValue(xyzNearbyPointOnObject.x, xyzNearbyPointOnObject.y, xyzNearbyPointOnObject.z, &xyzWorld, vi);
								#else
									RTpixelMaps.getPointMapValue(int(xyzNearbyPointOnObject.x), int(xyzNearbyPointOnObject.y), imageWidth*zoom, pointMap, &xyzWorld);
								#endif
									currentFeatureInList->point.x = xyzWorld.x;
									currentFeatureInList->point.y = xyzWorld.y;
									currentFeatureInList->point.z = xyzWorld.z;


									ATORfeature* newFeature = new ATORfeature();
									currentFeatureInList->next = newFeature;
									currentFeatureInList = currentFeatureInList->next;
								}
						#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
								else
								{
									cerr << "error: feature not on object, and no nearby features on object" << endl;
									exit(EXIT_ERROR);
								}
							}
						#endif
							else
							{

								currentFeatureInList->xViewport = x*zoom;
								currentFeatureInList->yViewport = y*zoom;

								currentFeatureInList->pointNonWorldCoord.x = x*zoom;
								currentFeatureInList->pointNonWorldCoord.y = y*zoom;
								currentFeatureInList->pointNonWorldCoord.z = depthVal;

								vec xyzWorld;
							#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
								RTscene.calculatePointMapValue(x*zoom, y*zoom, depthVal, &xyzWorld, vi);
							#else
								RTpixelMaps.getPointMapValue(x*zoom, y*zoom, imageWidth*zoom, pointMap, &xyzWorld);
							#endif

								currentFeatureInList->point.x = xyzWorld.x;
								currentFeatureInList->point.y = xyzWorld.y;
								currentFeatureInList->point.z = xyzWorld.z;

								ATORfeature* newFeature = new ATORfeature();
								currentFeatureInList->next = newFeature;
								currentFeatureInList = currentFeatureInList->next;

							}
						}
						else
						{

							currentFeatureInList->xViewport = x*zoom;
							currentFeatureInList->yViewport = y*zoom;

							currentFeatureInList->point.x = x*zoom;
							currentFeatureInList->point.y = y*zoom;

							ATORfeature* newFeature = new ATORfeature();
							currentFeatureInList->next = newFeature;
							currentFeatureInList = currentFeatureInList->next;

						}
					}
				}
			}
		}
	}
}

void ATORfeatureGenerationClass::generateFeatureListFromHeitgerFeatureAsciiMap(ATORfeature* firstFeatureInFeatureList, const string featureMapFileNameAscii, int zoom, const RTviewInfo* vi)
{
	ATORfeature* currentFeatureInList = firstFeatureInFeatureList;

	ifstream parseFileObject(featureMapFileNameAscii.c_str());

	if(parseFileObject.rdbuf()->is_open())
	{
		char c;
		int charCount = 0;
		int lineCount = 0;
		bool waitingForNewLine = false;
		bool readingX = true;
		bool readingY = false;
		bool readingMagnitude = false;

		string XString = "";
		string YString = "";
		string magnitudeString = "";

		while(parseFileObject.get(c))
		{
			charCount++;

			if((waitingForNewLine) && (c == '\n'))
			{
				lineCount++;
				waitingForNewLine = false;
				readingX = true;
			}
			else if(waitingForNewLine)
			{
				//do nothing, still waiting for new line
			}
			else if((readingX) && (c == ' '))
			{
				readingX = false;
				readingY = true;
			}
			else if(readingX)
			{
				XString = XString + c;
			}
			else if((readingY) && (c == ' '))
			{
				readingY = false;
				readingMagnitude = true;
			}
			else if(readingY)
			{
				YString = YString + c;
			}
			else if((readingMagnitude) && (c == '\n'))
			{
				currentFeatureInList->point.x = (SHAREDvars.convertStringToDouble(XString))* zoom;
				currentFeatureInList->point.y = (SHAREDvars.convertStringToDouble(YString))* zoom;
				currentFeatureInList->point.z = 0.0;
				currentFeatureInList->magnitude = (SHAREDvars.convertStringToDouble(magnitudeString));
				currentFeatureInList->xViewport = currentFeatureInList->point.x;
				currentFeatureInList->yViewport = currentFeatureInList->point.y;


				ATORfeature* newFeature = new ATORfeature();
				currentFeatureInList->next = newFeature;
				currentFeatureInList=currentFeatureInList->next;

				XString = "";
				YString = "";
				magnitudeString = "";

				lineCount++;
				readingMagnitude = false;
				readingX = true;
			}
			else if(readingMagnitude)
			{
				magnitudeString = magnitudeString + c;
			}
			else
			{
				//cerr << "file i/o error" << endl;
				//exit(EXIT_ERROR);
			}


		}

		parseFileObject.close();
	}
	else
	{
		cout << "error opening file, " << featureMapFileNameAscii << endl;
	}
}





#ifndef ATOR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
void ATORfeatureGenerationClass::generateFeatureListFromRGBMap(ATORfeature* firstFeatureInFeatureList, uchar* rgbMap, int imageWidth, const int imageHeight, int zoom, const string trainOrTestString, const string mapFileName, const RTviewInfo* vi)
{
	/*
	int resampledWidth = (imageWidth/zoom);
	int resampledHeight = (imageHeight/zoom);

	uchar* resampledRGBAtDesiredzoomChar = new uchar[resampledWidth*resampledHeight*RGB_NUM];
	resampleRGBmap(rgbMap, imageWidth, imageHeight, resampledRGBAtDesiredzoomChar, zoom);
	*/

	int* kernelOrientationHighContrastFoundMap = new int[imageWidth*imageHeight];
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			kernelOrientationHighContrastFoundMap[y*imageWidth + x] = 0;
		}
	}


	//3 orientations, 45 to vertical, vertical, and 45 past vertical
	for(int kernelOrientation = 0; kernelOrientation < 180; kernelOrientation=kernelOrientation+45)
	{
		//cout << "\n\n kernelOrientation = " << kernelOrientation << "\n\n" << endl;

		int xIncrement;
		int yIncrement;
		if(kernelOrientation == 0)
		{
			xIncrement = 1;
			yIncrement = 0;
		}
		else if(kernelOrientation == 45)
		{
			xIncrement = -1;
			yIncrement = 1;
		}
		else if(kernelOrientation == 90)
		{
			xIncrement = 0;
			yIncrement = 1;
		}
		else if(kernelOrientation == 135)
		{
			xIncrement = 1;
			yIncrement = 1;
		}
		else
		{
			cerr << "invalid kernelOrientation" << endl;
			exit(EXIT_ERROR);
		}

		int numberOfScanLines;
		if(kernelOrientation == 0)
		{
			numberOfScanLines = imageHeight;
		}
		else
		{
			numberOfScanLines = imageWidth;
		}


		for(int scanLine = 0; scanLine < numberOfScanLines; scanLine++)	//xbase
		{
			//uchar* colourStack = new uchar[ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH][RGB_NUM];
			colour previousColourInStack;
			double* contrastStack = new double[ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_CONTRAST_STACK_SIZE];	//4
			int stackIndex = 0;
			bool haveAFullStack = false;

			int y;
			int x;
			if(kernelOrientation == 0)
			{
				y = scanLine;
				x = 0;
			}
			else
			{
				y = 0;
				x = scanLine;
			}

			while((y < imageHeight) && (x < imageWidth) && (x >= 0))
			{
				colour kernelCurrentPixelPositionInRGBColour;
				RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &kernelCurrentPixelPositionInRGBColour);

				if(((kernelOrientation == 0) && (x > 0)) || ((kernelOrientation > 0) && (y > 0)))
				{
					contrastStack[stackIndex] = ATORimagecomparison.compareRGBpixelsForMatchLumContrastOnly(&kernelCurrentPixelPositionInRGBColour, &previousColourInStack);

					if(stackIndex == (ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_CONTRAST_STACK_SIZE-1))
					{
						haveAFullStack = true;
					}
					if(haveAFullStack)
					{


						if((contrastStack[stackIndex] < ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_NO_CONTRAST_MAX_THRESHOLD) &&
						(contrastStack[(stackIndex+1)%4] < ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_NO_CONTRAST_MAX_THRESHOLD) &&
						((contrastStack[(stackIndex+2)%4] >= ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_HIGH_CONTRAST_MIN_THRESHOLD) ||
						(contrastStack[(stackIndex+3)%4] >= ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_HIGH_CONTRAST_MIN_THRESHOLD)))
						{

							kernelOrientationHighContrastFoundMap[(y-(yIncrement*(ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH/2)))*imageWidth + (x-(xIncrement*(ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH/2)))] = kernelOrientationHighContrastFoundMap[(y-(yIncrement*(ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH/2)))*imageWidth + (x-(xIncrement*(ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH/2)))] + 1;

						}
					}

					if(stackIndex == ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_CONTRAST_STACK_SIZE-1)
					{
						stackIndex = 0;
					}
					else
					{
						stackIndex++;
					}
				}

				previousColourInStack.r = kernelCurrentPixelPositionInRGBColour.r;
				previousColourInStack.g = kernelCurrentPixelPositionInRGBColour.g;
				previousColourInStack.b = kernelCurrentPixelPositionInRGBColour.b;

				x = x + xIncrement;
				y = y + yIncrement;

			}

		}
	}


	ATORfeature* currentFeatureInList = firstFeatureInFeatureList;

	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			if(kernelOrientationHighContrastFoundMap[y*imageWidth + x] >= (ATOR_FEATURE_DETECTION_MIN_ORIENTATIONS_WITH_HIGH_CONTRAST))
			{
				/*
				vec proposedFeature;
				proposedFeature.x = x*zoom;
				proposedFeature.y = y*zoom;
				proposedFeature.z = 0.0;
				if(!addFeatureToListAndIfCommonFeatureExistsTakeAverage(&proposedFeature, firstFeatureInFeatureList, COLOUR_HUE_CONTRAST_RADIAL_SUM_RECONCILIATION_KERNEL_WIDTH, false))
				{
				      //cout << "!foundCommonFeature" << endl;
				}
				*/

				currentFeatureInList->xViewport = x*zoom;
				currentFeatureInList->yViewport = y*zoom;
				currentFeatureInList->point.x = x*zoom;
				currentFeatureInList->point.y = y*zoom;
				currentFeatureInList->point.z = 0.0;
				ATORfeature* newFeature = new ATORfeature();
				currentFeatureInList->next = newFeature;
				currentFeatureInList = currentFeatureInList->next;
			}
		}
	}

	//check high contrast at particular points within kernel to determine whether a pixel is a feature.

	delete kernelOrientationHighContrastFoundMap;
}

#endif
























/*
#ifdef ATOR_USE_OLD_RECURSIVE_CONTIGIOUS_REGION_METHODS_WHICH_FAIL_DUE_TO_LIMITED_C_RECURSION_STACK
void defineRegionCheckNextPixelRecursive(int xCurrent, int yCurrent, bool edgeBoolMap[], int alreadyProcessed[], int imageWidth, int imageHeight, int* regionSize)
{
	for(int q=0; q<4; q++)
	{
		int x;
		int y;
		if(q < 2)
		{
			x = xCurrent + (q*2)-1;
			y = yCurrent;
		}
		else
		{
			x = xCurrent;
			y = yCurrent + ((q-2)*2)-1;
		}

		int pixelStatus = alreadyProcessed[(y*imageWidth) + x];
		if(pixelStatus == NOT_PROCESSED)
		{
			if(edgeBoolMap[y*imageWidth + x] == true)
			{
				alreadyProcessed[y*imageWidth + x] = EDGE_FOUND;
			}
			else
			{
				*regionSize = *regionSize + 1;
				alreadyProcessed[y*imageWidth + x] = NO_EDGE_FOUND;

				if((x > 0) && (x < (imageWidth-1)) && (y > 0 ) && (y < (imageHeight-1)))
				{//kernel boundary check
					defineRegionCheckNextPixelRecursive(x, y, edgeBoolMap, alreadyProcessed, imageWidth, imageHeight, regionSize);
				}
			}
		}
		else if(pixelStatus == NO_EDGE_FOUND)
		{
			//stop crawl here
		}
		else if(pixelStatus == EDGE_FOUND)
		{
			//stop crawl here
		}
	}
}


void defineRegionCheckNextPixelUsingMeshPointRecursive(ATORmeshPoint* currentMeshPoint, int* regionSize, int contrastValChosen, int recursionLimit)
{
	for(int q=0; q<4; q++)
	{
		if(currentMeshPoint->adjacentMeshPointFilled[q])
		{
			//cout << "h10" << endl;

			int pixelStatus = currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed;
			if(pixelStatus == NOT_PROCESSED)
			{
				//cout << "h11" << endl;
				if(ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
				{
					if(currentMeshPoint->adjacentMeshPoint[q]->edge == true)	//CHECK THIS - not yet used - need to create an edgise routine for MeshLists first..
				}
				else
				{

					double contrastValUsedForComparison;
					double minContrastValueUsedForComparison;
					if(contrastValUsedForComparison = CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST)
					{
						contrastValUsedForComparison = currentMeshPoint->adjacentMeshPoint[q]->luminosityContrast;
						minContrastValueUsedForComparison = EDGE_LUMINOSITY_CONTRAST_THRESHOLD;
					}
					else if(contrastValUsedForComparison = CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST)
					{
						contrastValUsedForComparison = currentMeshPoint->adjacentMeshPoint[q]->meshPointNormalContrast;
						minContrastValueUsedForComparison = EDGE_NORMAL_CONTRAST_THRESHOLD;
					}
					if(contrastValUsedForComparison > minContrastValueUsedForComparison)
				}


				{
					//cout << "edge found" << endl;
					currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed = EDGE_FOUND;
				}
				else
				{
					//cout << "no edge found" << endl;
					*regionSize = *regionSize + 1;
					currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed = NO_EDGE_FOUND;

					//no kernel boundary check
					cout << "recursionLimit = " << recursionLimit << endl;
					defineRegionCheckNextPixelUsingMeshPointRecursive(currentMeshPoint->adjacentMeshPoint[q], regionSize, contrastValChosen, ++recursionLimit);
				}
			}
			else if(pixelStatus == NO_EDGE_FOUND)
			{
				//stop crawl here
			}
			else if(pixelStatus == EDGE_FOUND)
			{
				//stop crawl here
			}
		}
		else
		{
			//stop crawl here
		}
	}
}



void traceEdgeCheckNextPixelRecursive(int xCurrent, int yCurrent, int alreadyProcessed[], int imageWidth, int imageHeight, int* sumX, int* sumY, vec* sumPos, int* xStart, int* yStart, bool* foundStartAgain, bool* foundStart, int* numberOfCounts, int minRegionCircumferenceForRegion, int dimension, double pointMap[], double depthMap[], int zoom, bool alreadyProcessedInSearchForTheOuterBoundary[])
{
	bool currentpixelStatus = alreadyProcessed[(yCurrent*imageWidth) + xCurrent];

	if(foundStart && (xCurrent == *xStart) && (yCurrent == *yStart))
	{
		if(*foundStartAgain)
		{
			cerr << "found start multiple times - going around in loops" << endl;
			exit(EXIT_ERROR);
		}
		*foundStartAgain = true;
	}
	else
	{

		double localsumx = 0.0;
		double localsumy = 0.0;
		vec localsumpos;
		localsumpos.x = 0.0;
		localsumpos.y = 0.0;
		localsumpos.z = 0.0;
		int localsumnumberofhits = 0;
		bool edgeFoundInNeighborhood = false;
		for(int x = xCurrent-1; x<=xCurrent+1;x++)
		{
			for(int y = yCurrent-1; y<=yCurrent+1;y++)
			{
				if(!((x == xCurrent) && (y == yCurrent)))
				{
					int pixelStatus = alreadyProcessed[(y*imageWidth) + x];
					if(pixelStatus == EDGE_FOUND)
					{//do: need to check this - need to only record a single hit
						edgeFoundInNeighborhood = true;
						localsumnumberofhits++;

						if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
						#ifdef ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
							double depth = getLumOrContrastOrDepthMapValue(x*zoom, y*zoom, imageWidth*zoom, depthMap);
							vec xyzWorld;
							calculatePointMapValue(x*zoom, y*zoom, depth, &xyzWorld, vi);

						#else
							cout << "error: traceEdgeCheckNextPixelRecursive{} requires ATOR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH when compiling with 3DOD/use3DODonObjectForegroundDepthCheck" << endl;
							vec xyzWorld;
							getPointMapValue(x*zoom, y*zoom, imageWidth*zoom, pointMap, &xyzWorld);
						#endif
							localsumx = localsumx + x;
							localsumy = localsumy + y;
							localsumpos.x = localsumpos.x +  xyzWorld.x;
							localsumpos.y = localsumpos.y +  xyzWorld.y;
							localsumpos.z = localsumpos.z +  xyzWorld.z;
						}
						else
						{
							localsumx = localsumx + x;
							localsumy = localsumy + y;
						}
					}
				}
			}
		}

		int tempstartx;
		int tempstarty;
		bool tempfoundStart;
		if(edgeFoundInNeighborhood == true)
		{
			if(*foundStart == false)
			{
				tempstartx = xCurrent;
				tempstarty = yCurrent;
				tempfoundStart = true;
			}
			else
			{
				tempstartx = *xStart;
				tempstarty = *yStart;
				tempfoundStart = foundStart;
			}
		}
		else
		{
			if(*foundStart == false)
			{
				tempstartx = xCurrent;
				tempstarty = yCurrent;
				tempfoundStart = false;
			}
		}

		if((edgeFoundInNeighborhood == true) || (tempfoundStart == false))
		{
			for(int q=0; q<4; q++)
			{
				int x;
				int y;
				if(q < 2)
				{
					x = xCurrent + (q*2)-1;
					y = yCurrent;
				}
				else
				{
					x = xCurrent;
					y = yCurrent + ((q-2)*2)-1;
				}

				int pixelStatus = alreadyProcessed[(y*imageWidth) + x];

				bool passedChecks = false;
				if(pixelStatus == NO_EDGE_FOUND)
				{
					if(tempfoundStart)
					{
						passedChecks = true;
					}
					else
					{
						if(alreadyProcessedInSearchForTheOuterBoundary[(y*imageWidth) + x] == false)
						{
							passedChecks = true;
							alreadyProcessedInSearchForTheOuterBoundary[(y*imageWidth) + x] = true;
						}
					}
				}

				if(passedChecks)
				{
					if((x > 0) && (x < (imageWidth-1)) && (y > 0 ) && (y < (imageHeight-1)))
					{//kernel boundary check
						if(*foundStartAgain != true)
						{//only find a single valid region
							int tempsumx = 0;
							int tempsumy = 0;
							vec templocalsumpos;
							templocalsumpos.x = 0.0;
							templocalsumpos.y = 0.0;
							templocalsumpos.z = 0.0;
							bool tempFoundStartAgain = false;
							int tempnumberOfCounts = 0;

							traceEdgeCheckNextPixelRecursive(x, y, alreadyProcessed, imageWidth, imageHeight, &tempsumx, &tempsumy, &templocalsumpos, &tempstartx, &tempstarty, &tempFoundStartAgain, &tempfoundStart, &tempnumberOfCounts, minRegionCircumferenceForRegion, dimension, pointMap, depthMap, zoom, alreadyProcessedInSearchForTheOuterBoundary);
							if((tempFoundStartAgain == true) && (tempfoundStart == true) && (tempnumberOfCounts > minRegionCircumferenceForRegion))
							{
								*foundStartAgain = true;
								*sumX = localsumx + tempsumx;
								*sumY = localsumy + tempsumy;
								sumPos->x = localsumpos.x + templocalsumpos.x;
								sumPos->y = localsumpos.y + templocalsumpos.y;
								sumPos->z = localsumpos.z + templocalsumpos.z;
								*numberOfCounts = localsumnumberofhits + tempnumberOfCounts;
							}
						}
					}
				}

			}

			if((*foundStart == false) && (tempfoundStart == true))
			{
				if(*foundStartAgain == true)
				{
					*xStart = tempstartx;
					*yStart = tempstarty;
					*foundStart = true;
				}
			}
		}
	}
}





void traceEdgeCheckNextPixelUsingMeshPointRecursive(ATORmeshPoint* currentMeshPoint, int* sumX, int* sumY, vec* sumPos, int* xStart, int* yStart, bool* foundStartAgain, bool* foundStart, int* numberOfCounts, int minRegionCircumferenceForRegion)
{
	bool currentpixelStatus = currentMeshPoint->alreadyProcessed;
	int xCurrent = currentMeshPoint->xInt;
	int yCurrent = currentMeshPoint->yInt;
	if(foundStart && (xCurrent == *xStart) && (yCurrent == *yStart))
	{
		if(*foundStartAgain)
		{
			cerr << "found start multiple times - going around in loops" << endl;
			exit(EXIT_ERROR);
		}
		*foundStartAgain = true;
	}
	else
	{

		double localsumx = 0.0;
		double localsumy = 0.0;
		vec localsumpos;
		localsumpos.x = 0.0;
		localsumpos.y = 0.0;
		localsumpos.z = 0.0;
		int localsumnumberofhits = 0;
		bool edgeFoundInNeighborhood = false;
		//must check all 9 pixels around pixel
		//up, down, left, right
		for(int q = 0; q < 4; q++)
		{
			if(currentMeshPoint->adjacentMeshPointFilled[q] == true)
			{
				if(currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed == EDGE_FOUND)
				{
					localsumnumberofhits++;
					edgeFoundInNeighborhood = true;
					localsumx = localsumx + currentMeshPoint->adjacentMeshPoint[q]->xInt;
					localsumy = localsumy + currentMeshPoint->adjacentMeshPoint[q]->yInt;
					localsumpos.x = localsumpos.x + currentMeshPoint->adjacentMeshPoint[q]->point.x;
					localsumpos.y = localsumpos.y + currentMeshPoint->adjacentMeshPoint[q]->point.y;
					localsumpos.z = localsumpos.z + currentMeshPoint->adjacentMeshPoint[q]->point.z;
				}
			}
		}
		//2 pixels left and right of up and down
		for(int q = 2; q < 4; q++)
		{//up + down
			for(int r = 0; r < 2; r++)
			{//left + right
				if(currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPointFilled[r] == true)
				{
					if(currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[r]->alreadyProcessed == EDGE_FOUND)
					{
						localsumnumberofhits++;
						edgeFoundInNeighborhood = true;
						localsumx = localsumx + currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[r]->xInt;
						localsumy = localsumy + currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[r]->yInt;
						localsumpos.x = localsumpos.x + currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[r]->point.x;
						localsumpos.y = localsumpos.y + currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[r]->point.y;
						localsumpos.z = localsumpos.z + currentMeshPoint->adjacentMeshPoint[q]->adjacentMeshPoint[r]->point.z;
					}
				}
			}
		}


		ATORmeshPoint* tempStartMeshPoint;
		int tempstartx;
		int tempstarty;
		bool tempfoundStart;
		if(edgeFoundInNeighborhood == true)
		{
			if(*foundStart == false)
			{
				tempstartx = xCurrent;
				tempstarty = yCurrent;
				tempfoundStart = true;
			}
			else
			{
				tempstartx = *xStart;
				tempstarty = *yStart;
				tempfoundStart = true; //foundStart;
			}
		}
		else
		{
			if(*foundStart == false)
			{
				tempstartx = xCurrent;
				tempstarty = yCurrent;
				tempfoundStart = false;
			}
		}

		if((edgeFoundInNeighborhood == true) || (tempfoundStart == false))
		{
			for(int q=0; q<4; q++)
			{
				if(currentMeshPoint->adjacentMeshPointFilled[q])
				{
					bool passedChecks = false;

					int pixelStatus = currentMeshPoint->adjacentMeshPoint[q]->alreadyProcessed;
					if(pixelStatus == NO_EDGE_FOUND)
					{
						if(tempfoundStart)
						{
							passedChecks = true;
						}
						else
						{
							if(currentMeshPoint->hasBeenProcessedInSearchForTheOuterBoundary == false)
							{
								passedChecks = true;
								currentMeshPoint->hasBeenProcessedInSearchForTheOuterBoundary = true;
							}
						}
					}

					if(passedChecks)
					{
						//no kernel boundary check
						if(*foundStartAgain != true)
						{//only find a single valid region
							int tempsumx = 0;
							int tempsumy = 0;
							vec templocalsumpos;
							templocalsumpos.x = 0.0;
							templocalsumpos.y = 0.0;
							templocalsumpos.z = 0.0;
							bool tempFoundStartAgain = false;
							int tempnumberOfCounts = 0;

							traceEdgeCheckNextPixelUsingMeshPointRecursive(currentMeshPoint->adjacentMeshPoint[q], &tempsumx, &tempsumy, &templocalsumpos, &tempstartx, &tempstarty, &tempFoundStartAgain, &tempfoundStart, &tempnumberOfCounts, minRegionCircumferenceForRegion);
							if((tempFoundStartAgain == true) && (tempfoundStart == true) && (tempnumberOfCounts > minRegionCircumferenceForRegion))
							{
								*foundStartAgain = true;
								*sumX = localsumx + tempsumx;
								*sumY = localsumy + tempsumy;
								sumPos->x = localsumpos.x + templocalsumpos.x;
								sumPos->y = localsumpos.y + templocalsumpos.y;
								sumPos->z = localsumpos.z + templocalsumpos.z;
								*numberOfCounts = localsumnumberofhits + tempnumberOfCounts;
							}
						}

					}
				}

			}

			if((*foundStart == false) && (tempfoundStart == true))
			{
				if(*foundStartAgain == true)
				{
					*xStart = tempstartx;
					*yStart = tempstarty;
					*foundStart = true;
				}
			}
		}
	}
}
#endif
*/








