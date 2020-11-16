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
 * File Name: ATORTHmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3o3a 16-November-2020
 * Notes: NB pointmap is a new addition for test streamlining; NB in test scenarios 2 and 3, there will be no pointmap available; the pointmap will have to be generated after depth map is obtained by using calculatePointUsingTInWorld()
 * /
 *******************************************************************************/


#include "ATORTHmain.hpp"
#include "ATORmethod.hpp"
#include "ATORglobalDefs.hpp"
#include "XMLrulesClass.hpp"
#include "ATORrules.hpp"

//#define TEMPTESTEDGEFIT
/*
#ifdef TEMPTESTEDGEFIT
#include "ATORfeatureGeneration.hpp"
#include "RTppm.hpp"
#include "RTviewinfo.hpp"
#include "RTpixelMaps.hpp"
int main
{
	int imageWidth = 768;
	int imageHeight = 576;
	uchar* rgbMap = new uchar[imageWidth*imageHeight*RGB_NUM];
	bool* edgeBoolMap = new bool[imageWidth*imageHeight];
	string rgbMapFileName = "testedge.ppm";
	pixmap* rgbPixMap;
	cout << "rgbPixMap = loadPPM(" << rgbMapFileName << ");" << endl;
	rgbPixMap = loadPPM(rgbMapFileName);
	cout << "imageWidth = " << imageWidth << endl;
	cout << "imageHeight = " << imageHeight << endl;
	cout << "createRGBMapFromPixmapImage(rgbPixMap, rgbMap);" << endl;
	createRGBMapFromPixmapImage(rgbPixMap, rgbMap);
	freePixmap(rgbPixMap);
	double* nullpointer = NULL;
	RTviewInfo* vi = NULL;

	generateEdgeListFromRGBmapWithQuadraticFit(rgbMap, edgeBoolMap, imageWidth, imageHeight, 1.0, false, nullpointer, nullpointer, 1, vi);
}
int mainTemp
#else
*/
int main()
{
	if(!parseATORrulesXMLfile())
	{
		cerr << "error: no rules file detected" << endl;
		exit(EXIT_ERROR);
	}
	fillInATORrulesExternVariables();


	#ifdef COMPILE_TH_ATOR_METHOD_2DOD

		int imageWidthFacingPoly = TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH;
		int imageHeightFacingPoly = TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT;

		int maxNumberOfPolygonsTrain = ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
		int numberOfTrainViewIndiciesPerObject = ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN;

		RTviewInfo viTrain;

		viTrain.imageWidth = TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH;
		viTrain.imageHeight = TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT;
		viTrain.viewWidth = TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH;
		viTrain.viewHeight = TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT;
		viTrain.focalLength = TH_ATOR_METHOD2DOD_DEFAULT_FOCAL;

		viTrain.eye.x = TH_ATOR_METHOD2DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		viTrain.eye.y = TH_ATOR_METHOD2DOD_DEFAULT_EYE_Y;
		viTrain.eye.z = TH_ATOR_METHOD2DOD_DEFAULT_EYE_Z;
		viTrain.viewAt.x = TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_X;
		viTrain.viewAt.y = TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Y;
		viTrain.viewAt.z = TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Z;
		viTrain.viewUp.x = TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_X;
		viTrain.viewUp.y = TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Y;
		viTrain.viewUp.z = TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Z;

		int maxNumberOfPolygonsTest = ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST;
		int numberOfTestViewIndiciesPerObject = ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST;

		RTviewInfo viTest;

		viTest.imageWidth = TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH;
		viTest.imageHeight = TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT;
		viTest.viewWidth = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH;
		viTest.viewHeight = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT;
		viTest.focalLength = TH_ATOR_METHOD2DOD_DEFAULT2_FOCAL;

		viTest.eye.x = TH_ATOR_METHOD2DOD_DEFAULT2_EYE_X;		//CHECK THIS; preferably the eye moves around the object, however not to the same locations as used during training
		viTest.eye.y = TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Y;
		viTest.eye.z = TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Z;
		viTest.viewAt.x = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_X;
		viTest.viewAt.y = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Y;
		viTest.viewAt.z = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Z;
		viTest.viewUp.x = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_X;
		viTest.viewUp.y = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Y;
		viTest.viewUp.z = TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Z;

		string trainObjectNameArray[10];
		trainObjectNameArray[0] = ATOR_METHOD2DOD_OBJECT_0_NAME;
		trainObjectNameArray[1] = ATOR_METHOD2DOD_OBJECT_1_NAME;
		trainObjectNameArray[2] = ATOR_METHOD2DOD_OBJECT_2_NAME;
		trainObjectNameArray[3] = ATOR_METHOD2DOD_OBJECT_3_NAME;
		trainObjectNameArray[4] = ATOR_METHOD2DOD_OBJECT_4_NAME;
		trainObjectNameArray[5] = ATOR_METHOD2DOD_OBJECT_5_NAME;
		trainObjectNameArray[6] = ATOR_METHOD2DOD_OBJECT_6_NAME;
		trainObjectNameArray[7] = ATOR_METHOD2DOD_OBJECT_7_NAME;
		trainObjectNameArray[8] = ATOR_METHOD2DOD_OBJECT_8_NAME;
		trainObjectNameArray[9] = ATOR_METHOD2DOD_OBJECT_9_NAME;

		string testObjectNameArray[10];
		testObjectNameArray[0] = ATOR_METHOD2DOD_OBJECT_0_NAME;

	#ifdef ATOR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA
		int objectDataSource = ATOR_OBJECT_DATA_SOURCE_PREEXISTING_DATA;
	#elif defined ATOR_METHOD2DOD_GENERATE_IMAGE_DATA
		int objectDataSource = ATOR_OBJECT_DATA_SOURCE_GENERATE_DATA;
	#endif
		int dimension = ATOR_METHOD2DOD_DIMENSIONS;
		int numberOfTrainObjects = ATOR_METHOD2DOD_NUM_OF_OBJECTS;
		int numberOfTestObjects = 1;

		int numberOfTrainZoomIndicies = ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
		int numberOfTestZoomIndicies = ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;

		int numberOfTrainViewIndiciesPerObjectWithUniquePolygons;
		int numberOfTestViewIndiciesPerObjectWithUniquePolygons;
		if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
		{
			numberOfTrainViewIndiciesPerObjectWithUniquePolygons = 1;
			numberOfTestViewIndiciesPerObjectWithUniquePolygons = 1;
		}
		else if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
		{
			numberOfTrainViewIndiciesPerObjectWithUniquePolygons = numberOfTrainViewIndiciesPerObject;
			numberOfTestViewIndiciesPerObjectWithUniquePolygons = numberOfTestViewIndiciesPerObject;
		}

		int* numberOfTrainPolys = new int[numberOfTrainObjects*numberOfTrainViewIndiciesPerObjectWithUniquePolygons*numberOfTrainZoomIndicies];
		int* numberOfTestPolys = new int[numberOfTestObjects*numberOfTestViewIndiciesPerObjectWithUniquePolygons*numberOfTestZoomIndicies];

		ORTHmethod(dimension, numberOfTrainObjects, trainObjectNameArray, numberOfTestObjects, testObjectNameArray, numberOfTrainPolys, numberOfTestPolys, objectDataSource, &viTrain, &viTest, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygonsTrain, maxNumberOfPolygonsTest, numberOfTrainViewIndiciesPerObject, numberOfTestViewIndiciesPerObject, numberOfTrainViewIndiciesPerObjectWithUniquePolygons, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTrainZoomIndicies, numberOfTestZoomIndicies);

	#elif defined COMPILE_TH_ATOR_METHOD_3DOD

		int imageWidthFacingPoly = TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH;
		int imageHeightFacingPoly = TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT;

		int maxNumberOfPolygonsTrain = ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
		int numberOfTrainViewIndiciesPerObject = ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TRAIN;

		RTviewInfo viTrain;

		viTrain.imageWidth = TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_WIDTH;
		viTrain.imageHeight = TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT;
		viTrain.viewWidth = TH_ATOR_METHOD3DOD_DEFAULT_VIEWSIZE_WIDTH;
		viTrain.viewHeight = TH_ATOR_METHOD3DOD_DEFAULT_VIEWSIZE_HEIGHT;
		viTrain.focalLength = TH_ATOR_METHOD3DOD_DEFAULT_FOCAL;

		viTrain.eye.x = TH_ATOR_METHOD3DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		viTrain.eye.y = TH_ATOR_METHOD3DOD_DEFAULT_EYE_Y;
		viTrain.eye.z = TH_ATOR_METHOD3DOD_DEFAULT_EYE_Z;
		viTrain.viewAt.x = TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_X;
		viTrain.viewAt.y = TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_Y;
		viTrain.viewAt.z = TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_Z;
		viTrain.viewUp.x = TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_X;
		viTrain.viewUp.y = TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_Y;	//[house3DOD1: 0 / 50 (view1) / 100 (view2)] / [house3DOD2: 0 / 50 (view1) / 100 (view2)]
		viTrain.viewUp.z = TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_Z;	//[house3DOD1: 100] / [house3DOD2: -100]

		int maxNumberOfPolygonsTest = ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST;
		int numberOfTestViewIndiciesPerObject = ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST;

		RTviewInfo viTest;

		viTest.imageWidth = TH_ATOR_METHOD3DOD_DEFAULT2_IMAGE_WIDTH;
		viTest.imageHeight = TH_ATOR_METHOD3DOD_DEFAULT2_IMAGE_HEIGHT;
		viTest.viewWidth = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWSIZE_WIDTH;
		viTest.viewHeight = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWSIZE_HEIGHT;
		viTest.focalLength = TH_ATOR_METHOD3DOD_DEFAULT2_FOCAL;

		viTest.eye.x = TH_ATOR_METHOD3DOD_DEFAULT2_EYE_X;		//CHECK THIS; preferably the eye moves around the object, however not to the same locations as used during training
		viTest.eye.y = TH_ATOR_METHOD3DOD_DEFAULT2_EYE_Y;
		viTest.eye.z = TH_ATOR_METHOD3DOD_DEFAULT2_EYE_Z;
		viTest.viewAt.x = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWAT_X;
		viTest.viewAt.y = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWAT_Y;
		viTest.viewAt.z = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWAT_Z;
		viTest.viewUp.x = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_X;
		viTest.viewUp.y = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_Y;
		viTest.viewUp.z = TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_Z;

		string trainObjectNameArray[10];
		trainObjectNameArray[0] = ATOR_METHOD3DOD_OBJECT_0_NAME;
		trainObjectNameArray[1] = ATOR_METHOD3DOD_OBJECT_1_NAME;
		trainObjectNameArray[2] = ATOR_METHOD3DOD_OBJECT_2_NAME;
		trainObjectNameArray[3] = ATOR_METHOD3DOD_OBJECT_3_NAME;
		trainObjectNameArray[4] = ATOR_METHOD3DOD_OBJECT_4_NAME;
		trainObjectNameArray[5] = ATOR_METHOD3DOD_OBJECT_5_NAME;
		trainObjectNameArray[6] = ATOR_METHOD3DOD_OBJECT_6_NAME;
		trainObjectNameArray[7] = ATOR_METHOD3DOD_OBJECT_7_NAME;
		trainObjectNameArray[8] = ATOR_METHOD3DOD_OBJECT_8_NAME;
		trainObjectNameArray[9] = ATOR_METHOD3DOD_OBJECT_9_NAME;

		string testObjectNameArray[10];
		testObjectNameArray[0] = ATOR_METHOD3DOD_OBJECT_0_NAME;

	#ifdef ATOR_METHOD3DOD_USE_PREEXISTING_IMAGE_DATA
		int objectDataSource = ATOR_OBJECT_DATA_SOURCE_PREEXISTING_DATA;
	#elif defined ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
		int objectDataSource = ATOR_OBJECT_DATA_SOURCE_GENERATE_DATA;
	#endif

		int dimension = ATOR_METHOD3DOD_DIMENSIONS;
		int numberOfTrainObjects = ATOR_METHOD3DOD_NUM_OF_OBJECTS;
		int numberOfTestObjects = 1;

		int numberOfTrainZoomIndicies = ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
		int numberOfTestZoomIndicies = ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;

		int numberOfTrainViewIndiciesPerObjectWithUniquePolygons;
		int numberOfTestViewIndiciesPerObjectWithUniquePolygons;
		if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
		{
			numberOfTrainViewIndiciesPerObjectWithUniquePolygons = 1;
			numberOfTestViewIndiciesPerObjectWithUniquePolygons = 1;
		}
		else if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
		{
			numberOfTrainViewIndiciesPerObjectWithUniquePolygons = numberOfTrainViewIndiciesPerObject;
			numberOfTestViewIndiciesPerObjectWithUniquePolygons = numberOfTestViewIndiciesPerObject;
		}

		int* numberOfTrainPolys = new int[numberOfTrainObjects*numberOfTrainViewIndiciesPerObjectWithUniquePolygons*numberOfTrainZoomIndicies];
		int* numberOfTestPolys = new int[numberOfTestObjects*numberOfTestViewIndiciesPerObjectWithUniquePolygons*numberOfTestZoomIndicies];

		ORTHmethod(dimension, numberOfTrainObjects, trainObjectNameArray, numberOfTestObjects, testObjectNameArray, numberOfTrainPolys, numberOfTestPolys, objectDataSource, &viTrain, &viTest, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygonsTrain, maxNumberOfPolygonsTest, numberOfTrainViewIndiciesPerObject, numberOfTestViewIndiciesPerObject, numberOfTrainViewIndiciesPerObjectWithUniquePolygons, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTrainZoomIndicies, numberOfTestZoomIndicies);
	#else
		cerr << "error: COMPILE_TH_ATOR_METHOD_3DOD or COMPILE_TH_ATOR_METHOD_2DOD must be defined for TH OR" << endl;
		exit(EXIT_ERROR);
	#endif

}



