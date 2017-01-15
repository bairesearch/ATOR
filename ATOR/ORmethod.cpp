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
 * File Name: ORmethod.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3f2b 22-June-2015
 * NB pointmap is a new addition for test streamlining; NB in test scenarios 2 and 3, there will be no pointmap available; the pointmap will have to be generated after depth map is obtained by using calculatePointUsingTInWorld()
 *******************************************************************************/


#include "ORmethod.h"
#include "ORglobalDefs.h"
#include "ORmethod3DOD.h"
#include "ORmethod2DOD.h"
#include "ORpixelMaps.h"
#include "ORoperations.h"
#include "ORfeature.h"
#include "ORquadraticFit.h"
#include "RTscene.h"
#include "LDparser.h"
#include "LDreferenceManipulation.h"
#include "RTreferenceManipulation.h"
#include "SHAREDvector.h"
#include "ORcomparison.h"
#include "ORimagecomparison.h"
#include "ORdatabaseFileIO.h"

#ifdef OR_IMAGE_COMPARISON_SQL
#include "LDmysql.h";
#include "LDjpeg.h";
#include "ORdatabaseSQL.h"
#include "ORdatabaseDecisionTree.h"
#endif

/*
#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
		//for neural network;
	#include "ANNFormation.h"
	#include "ANNTraining.h"
	#include "ANNUpdateAlgorithm.h"
	#include "ANNdisplay.h"
#endif
*/
#ifdef USE_OPENGL
	#include "LDopengl.h"
#endif

#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
static double oriNoiseArray[OR_METHOD3DOD_NUM_ORI_NOISE_VALUES][3];
static double posNoiseArray[OR_METHOD3DOD_NUM_POS_NOISE_VALUES][3];
static double featurePosNoise1Array[OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS][2];
static double featurePosNoise2Array[OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS][2];
static double featurePosNoise3Array[OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS][2];

#endif



//#ifdef OR_PRINT_ALGORITHM_AND_TIME_DETAILS
long time1aALTERNATEODgenerationParseVectorGraphicsFileTotal = 0.0;
long time1aALTERNATEODgenerationParseVectorGraphicsFileIndex = 0.0;

long time1ODgenerationTotalTrain = 0.0;
long time1ODgenerationIndexTrain = 0.0;
long time1ODgenerationTotalTest = 0.0;
long time1ODgenerationIndexTest = 0.0;

long time1aODgenerationLoadImageFileTotalTrain = 0.0;
long time1aODgenerationLoadImageFileIndexTrain = 0.0;
long time1aODgenerationLoadImageFileTotalTest = 0.0;
long time1aODgenerationLoadImageFileIndexTest = 0.0;


long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTrain = 0.0;
long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTrain = 0.0;
long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTest = 0.0;
long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTest = 0.0;

long time1bODgenerationCreateInterpolatedMeshTotalTrain = 0.0;
long time1bODgenerationCreateInterpolatedMeshIndexTrain = 0.0;
long time1bODgenerationCreateInterpolatedMeshTotalTest = 0.0;
long time1bODgenerationCreateInterpolatedMeshIndexTest = 0.0;

long time1cODgenerationFeatureListCreationTotalTrain = 0.0;
long time1cODgenerationFeatureListCreationIndexTrain = 0.0;
long time1cODgenerationFeatureListCreationTotalTest = 0.0;
long time1cODgenerationFeatureListCreationIndexTest = 0.0;

long time2ObjectTriangleGenerationPolygonListTotalTrain = 0.0;
long time2ObjectTriangleGenerationPolygonListIndexTrain = 0.0;
long time2ObjectTriangleGenerationPolygonListTotalTest = 0.0;
long time2ObjectTriangleGenerationPolygonListIndexTest = 0.0;

long time3NormalisedSnapshotGenerationTotalTrain = 0.0;
long time3NormalisedSnapshotGenerationIndexTrain = 0.0;
long time3NormalisedSnapshotGenerationTotalTest = 0.0;
long time3NormalisedSnapshotGenerationIndexTest = 0.0;


long time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTrain = 0.0;
long time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTrain = 0.0;
long time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTest = 0.0;
long time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTest = 0.0;

long time3aNormalisedSnapshotGenerationTransformDataWRTPolygonTotalTrain = 0.0;
long time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTrain = 0.0;
long time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonTotalTest = 0.0;
long time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTest = 0.0;

long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTrain = 0.0;
long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTrain = 0.0;
long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTest = 0.0;
long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTest = 0.0;

long time5NormalisedSnapshotComparisonTotal = 0.0;
long time5NormalisedSnapshotComparisonIndex = 0.0;


//#endif






bool ORTHmethod(int dimension, int numberOfTrainObjects, string trainObjectNameArray[], int numberOfTestObjects, string testObjectNameArray[], int* numberOfTrainPolys, int* numberOfTestPolys, int objectDataSource, RTviewInfo* viTrain, RTviewInfo* viTest, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrain, int maxNumberOfPolygonsTest, int numberOfTrainViewIndiciesPerObject, int numberOfTestViewIndiciesPerObject, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies)
{
	bool result = true;

	string multiViewListStringNotUsed = "";
	int firstViewNumberNotUsed = 0;

	#define OR_MYSQL_IP_ADDRESS_DEFAULT_NULL "null"
	#define OR_MYSQL_USER_NAME_DEFAULT_NULL "null"
	#define OR_MYSQL_USER_PASSWORD_DEFAULT_NULL "null"

	if(!ORmethodInitialise(imageWidthFacingPoly, imageHeightFacingPoly, true, true, true, dimension, OR_MYSQL_IP_ADDRESS_DEFAULT_NULL, OR_MYSQL_USER_NAME_DEFAULT_NULL, OR_MYSQL_USER_PASSWORD_DEFAULT_NULL))
	{
		result = false;
	}


			#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
			if(dimension == OR_METHOD2DOD_DIMENSIONS)
			{
				setNoiseArraysMethod2DOD();		//Advded 5 Nov 08
			}
			else if(dimension == OR_METHOD3DOD_DIMENSIONS)
			{
				setNoiseArraysMethodA();		//Advded 5 Nov 08
			}
			#endif

	string testUI2;

#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
	ANNexperience* firstExperienceInTrainList = new ANNexperience();
	ANNexperience* firstExperienceInTestList = new ANNexperience();
#endif

	//train
	if(!ORmethodTrainOrTest(dimension, numberOfTrainObjects, trainObjectNameArray, objectDataSource, viTrain, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygonsTrain, numberOfTrainViewIndiciesPerObject, numberOfTrainViewIndiciesPerObjectWithUniquePolygons, numberOfTrainPolys, true, numberOfTrainZoomIndicies, firstViewNumberNotUsed, multiViewListStringNotUsed))
	{
		result = false;
	}

				#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR

					//int numSidesPerPolygon = OR_METHOD_POLYGON_NUMBER_OF_SIDES;
					ANNneuronContainer* firstInputNeuronInNetwork = new ANNneuronContainer();
					ANNneuronContainer* firstOutputNeuronInNetwork;
					int numberOfInputNeurons;
					int numberOfOutputNeurons = numberOfTrainPolys*OR_METHOD_POLYGON_NUMBER_OF_SIDES;
				#ifdef OR_DEBUG_WORRY_ABOUT_DISPLAYING_2D_NET_PROPERLY
					numberOfOutputNeurons =	(int)pow(((int)sqrt(numberOfOutputNeurons)+1), 2.0);	//this is used for 2D visualisation purposes only									//required for 2D visualisation
				#endif
					firstOutputNeuronInNetwork = initialiseNormalisedSnapshotNeuralNetwork(firstInputNeuronInNetwork, &numberOfInputNeurons, numberOfOutputNeurons, imageWidthFacingPoly, imageHeightFacingPoly);

					cout << "numberOfInputNeurons = " << numberOfInputNeurons << endl;
					cout << "numberOfOutputNeurons = " << numberOfOutputNeurons << endl;

					bool addSprites = false;
					bool allowRaytrace = false;
					string XMLNNSceneFileName = "ORNN.xml";
					char* charstarvectorGraphicsLDRNNSceneFileName = "ORNN.ldr";
					char* charstarvectorGraphicsLDRNNSceneFileNameWithSprites = "ORNNwithSprites.ldr";
					char* charstarvectorGraphicsTALNNSceneFileName = "ORNN.tal";
					char* charstarraytracedImagePPMNNSceneFileName = "ORNN.ppm";
					char* charstarexperienceNNSceneFileName = "ORNN.data";
					bool useFoldsDuringTraining;
					int maxOrSetNumEpochs;
					useFoldsDuringTraining = false;
					maxOrSetNumEpochs = OR_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
					trainAndOutputNeuralNetworkWithFileNames(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, firstExperienceInTrainList, addSprites, allowRaytrace, &XMLNNSceneFileName, charstarvectorGraphicsLDRNNSceneFileName, charstarvectorGraphicsLDRNNSceneFileNameWithSprites, charstarvectorGraphicsTALNNSceneFileName, charstarraytracedImagePPMNNSceneFileName, charstarexperienceNNSceneFileName, useFoldsDuringTraining, maxOrSetNumEpochs);

					cout << "copy NN results to clipboard and ENTER SOME TEXT TO CONTINUE" << endl;
					cin >> testUI2;
				#endif

	//test
	if(!ORmethodTrainOrTest(dimension, numberOfTestObjects, testObjectNameArray, objectDataSource, viTest, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygonsTest, numberOfTestViewIndiciesPerObject, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTestPolys, false, numberOfTestZoomIndicies, firstViewNumberNotUsed, multiViewListStringNotUsed))
	{
		result = false;
	}

	#ifdef OR_IMAGE_COMPARISON_SQL
	if(!ORmethodCompareTestWithTrain(dimension, numberOfTestObjects, testObjectNameArray, imageWidthFacingPoly, imageHeightFacingPoly, numberOfTestPolys, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTestZoomIndicies, TEST, 0))
	#else
	if(!ORmethodCompareTestWithTrain(dimension, numberOfTrainObjects, trainObjectNameArray, numberOfTestObjects, testObjectNameArray, imageWidthFacingPoly, imageHeightFacingPoly, numberOfTrainPolys, numberOfTestPolys, numberOfTrainViewIndiciesPerObjectWithUniquePolygons, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTrainZoomIndicies, numberOfTestZoomIndicies, 0))
	#endif
	{
		result = false;
	}

	if(!ORmethodExit())
	{
		result = false;
	}

	return result;
}

#ifdef OR_IMAGE_COMPARISON_SQL


bool ORmethodTrain(int dimension, int numberOfTrainObjects, string trainObjectNameArray[], int* numberOfTrainPolys, int objectDataSource, RTviewInfo* viTrain, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrain, int numberOfTrainViewIndiciesPerObject, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int trainOrTest, string sqlIPaddress, string sqlUsername, string sqlPassword, bool clearTrainTable, int viewNumber, string multViewListFileName)
{
	bool result = true;

	if(!ORmethodInitialise(imageWidthFacingPoly, imageHeightFacingPoly, true, true, clearTrainTable, dimension, sqlIPaddress, sqlUsername, sqlPassword))
	{
		result = false;
	}

	//train
	if(!ORmethodTrainOrTest(dimension, numberOfTrainObjects, trainObjectNameArray, objectDataSource, viTrain, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygonsTrain, numberOfTrainViewIndiciesPerObject, numberOfTrainViewIndiciesPerObjectWithUniquePolygons, numberOfTrainPolys, trainOrTest, numberOfTrainZoomIndicies, viewNumber, multViewListFileName))
	{
		result = false;
	}


	if(!ORmethodExit())
	{
		result = false;
	}

	return result;
}

bool ORmethodTest(int dimension, int numberOfTestObjects, string testObjectNameArray[], int* numberOfTestPolys, int objectDataSource, RTviewInfo* viTest, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTest, int numberOfTestViewIndiciesPerObject, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTestZoomIndicies, int trainOrTest, string sqlIPaddress, string sqlUsername, string sqlPassword, bool clearTrainTable, int viewNumber, string multViewListFileName)
{
	bool result = true;

	if(!ORmethodInitialise(imageWidthFacingPoly, imageHeightFacingPoly, true, true, false, dimension, sqlIPaddress, sqlUsername, sqlPassword))
	{
		result = false;
	}

	//test
	if(!ORmethodTrainOrTest(dimension, numberOfTestObjects, testObjectNameArray, objectDataSource, viTest, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygonsTest, numberOfTestViewIndiciesPerObject, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTestPolys, trainOrTest, numberOfTestZoomIndicies, viewNumber, multViewListFileName))
	{
		result = false;
	}
	cout << "ORmethodTrainOrTest complete" << endl;

	if(!ORmethodCompareTestWithTrain(dimension, numberOfTestObjects, testObjectNameArray, imageWidthFacingPoly, imageHeightFacingPoly, numberOfTestPolys, numberOfTestViewIndiciesPerObjectWithUniquePolygons, numberOfTestZoomIndicies, trainOrTest, viewNumber))
	{
		result = false;
	}

	if(!ORmethodExit())
	{
		result = false;
	}

	return result;
}


#endif




bool ORmethodInitialise(int imageWidthFacingPoly, int imageHeightFacingPoly, bool initialiseTrain, bool initialiseTest, bool clearTrainTable, int dimension, string sqlIPaddress, string sqlUsername, string sqlPassword)
{
	int cropWidth;
	int cropHeight;
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		cropWidth = OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X;
		cropHeight = OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y;
	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		cropWidth = OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X;
		cropHeight = OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y;
	}

	#ifdef USE_OPENGL
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "OpenGL Hardware Acceleration: preconditions" << endl;
		cout << "\t 1. freeglut is installed" << endl;
	}
	initiateOpenGL(imageWidthFacingPoly+(cropWidth*2), imageHeightFacingPoly+(cropHeight*2), OR_SNAPSHOT_WINDOW_POSITION_X, OR_SNAPSHOT_WINDOW_POSITION_Y, OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS);
	#endif

	if(OR_IMAGE_COMPARISON_DECISION_TREE)
	{
		#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		imageComparisonTreeBaseDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_BASE_DIRECTORY;
		imageComparisonTreeName = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_NAME;
		#endif
	}

	#ifndef OR_METHOD3DOD_TEST
	#ifdef OR_IMAGE_COMPARISON_SQL
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "SQL Database: preconditions" << endl;
		cout << "\t 1. MySQL server is installed on a PC on the network with the default BAI OR layout" << endl;
		cout << "\t 2. MySQL client is installed on this PC, including development files (headers and libraries)" << endl;
	}
	char* sqlipaddressCharStar = const_cast<char*>(sqlIPaddress.c_str());
	char* sqlusernameCharStar = const_cast<char*>(sqlUsername.c_str());
	char* sqlpasswordCharStar = const_cast<char*>(sqlPassword.c_str());
	initiateMySQLserverConnection(sqlipaddressCharStar, sqlusernameCharStar, sqlpasswordCharStar, OR_MYSQL_DATABASE_NAME);
	performSQLdeleteAllRowsQuery(OR_MYSQL_TABLE_NAME_TEST);
	databaseTableSizeTest = 0;
	if(clearTrainTable)
	{
		performSQLdeleteAllRowsQuery(OR_MYSQL_TABLE_NAME_TRAIN);
		performSQLdeleteAllRowsQuery(OR_MYSQL_TABLE_NAME_DECISIONTREE);
		databaseTableSizeTrain = 0;
		databaseTableSizeTrainInitial = 0;
		databaseTableSizeDecisionTree = 0;
		databaseTableSizeDecisionTreeInitial = 0;
	}
	else
	{
		databaseTableSizeTrain = performSQLgetNumRowsQuery(OR_MYSQL_TABLE_NAME_TRAIN);
		databaseTableSizeTrainInitial = databaseTableSizeTrain;
		databaseTableSizeDecisionTree = performSQLgetNumRowsQuery(OR_MYSQL_TABLE_NAME_DECISIONTREE);
		databaseTableSizeDecisionTreeInitial = databaseTableSizeDecisionTree;
	}
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "databaseTableSizeTrain = " << databaseTableSizeTrain << endl;
		cout << "databaseTableSizeTest = " << databaseTableSizeTest << endl;
		cout << "databaseTableSizeDecisionTree = " << databaseTableSizeDecisionTree << endl;
	}
	#endif
	#endif

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		fillDCTcoeffSelectionArrays();
	}

	return true;
}

bool ORmethodExit()
{
	#ifdef OR_IMAGE_COMPARISON_SQL
	endMySQLserverConnection();
	#endif

	#ifdef USE_OPENGL
	exitOpenGL();
	#endif
	
	return true;
}



#ifdef OR_IMAGE_COMPARISON_SQL
bool ORmethodCompareTestWithTrain(int dimension, int numberOfTestObjects, string testObjectNameArray[], int imageWidthFacingPoly, int imageHeightFacingPoly, int* numberOfTestPolys, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTestZoomIndicies, int trainOrTest, int testViewNumber)
#else
bool ORmethodCompareTestWithTrain(int dimension, int numberOfTrainObjects, string trainObjectNameArray[], int numberOfTestObjects, string testObjectNameArray[], int imageWidthFacingPoly, int imageHeightFacingPoly, int* numberOfTrainPolys, int* numberOfTestPolys, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies, int testViewNumber)
#endif
{
	bool result = true;

	long time5NormalisedSnapshotComparisonStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t start: 5. normalised snapshot comparison" << endl;
		time5NormalisedSnapshotComparisonStart = getTimeAsLong();
	}
	
	double averageMatchErrorAcrossAllObjects;
#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
	averageMatchErrorAcrossAllObjects = compareNormalisedSnapshotExperienceListWithNeuralNetwork(firstExperienceInTestList, firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfTrainPolys);
#else
	#ifdef OR_IMAGE_COMPARISON_SQL
	averageMatchErrorAcrossAllObjects = compareNormalisedSnapshots(numberOfTestPolys, numberOfTestViewIndiciesPerObjectWithUniquePolygons, imageWidthFacingPoly, imageHeightFacingPoly, testObjectNameArray, numberOfTestObjects, dimension, numberOfTestZoomIndicies, trainOrTest, testViewNumber);
	#else
	averageMatchErrorAcrossAllObjects = compareNormalisedSnapshots(numberOfTrainPolys, numberOfTestPolys, numberOfTrainViewIndiciesPerObjectWithUniquePolygons, numberOfTestViewIndiciesPerObjectWithUniquePolygons, imageWidthFacingPoly, imageHeightFacingPoly, trainObjectNameArray, numberOfTrainObjects, testObjectNameArray, numberOfTestObjects, dimension, numberOfTrainZoomIndicies, numberOfTestZoomIndicies, testViewNumber);
	#endif
#endif

	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "averageMatchErrorAcrossAllObjects error = " << averageMatchErrorAcrossAllObjects << endl;
	}


	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t end: 5. normalised snapshot comparison" << endl;
		long time5NormalisedSnapshotComparisonEnd;
		time5NormalisedSnapshotComparisonEnd = getTimeAsLong();
		time5NormalisedSnapshotComparisonTotal = time5NormalisedSnapshotComparisonTotal + time5NormalisedSnapshotComparisonEnd-time5NormalisedSnapshotComparisonStart;
		time5NormalisedSnapshotComparisonIndex++;
		cout << "\t time5NormalisedSnapshotComparison = " << time5NormalisedSnapshotComparisonEnd-time5NormalisedSnapshotComparisonStart << endl;
	}


	long time1aALTERNATEODgenerationParseVectorGraphicsFileAverage = 0;

	long time1ODgenerationAverageTrain = 0;
	long time1ODgenerationAverageTest = 0;

	long time1aODgenerationLoadImageFileAverageTrain = 0;
	long time1aODgenerationLoadImageFileAverageTest = 0;

	long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTrain = 0;
	long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTest = 0;

	long time1bODgenerationCreateInterpolatedMeshAverageTrain = 0;
	long time1bODgenerationCreateInterpolatedMeshAverageTest = 0;

	long time1cODgenerationFeatureListCreationAverageTrain = 0;
	long time1cODgenerationFeatureListCreationAverageTest = 0;

	long time2ObjectTriangleGenerationPolygonListAverageTrain = 0;
	long time2ObjectTriangleGenerationPolygonListAverageTest = 0;

	long time3NormalisedSnapshotGenerationAverageTrain = 0;
	long time3NormalisedSnapshotGenerationAverageTest = 0;

	long time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTrain = 0;
	long time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTest = 0;

	long time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTrain = 0;
	long time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTest = 0;

	long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTrain = 0;
	long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTest = 0;

	long time5NormalisedSnapshotComparisonAverage = 0;

	long time5aNormalisedSnapshotComparisonLoadComparisonDataAverage = 0;
	long time5bNormalisedSnapshotComparisonCompareSnapshotDataAverage = 0;

	long timeTotalTrain = 0;
	long timeTotalTest = 0;
	long timeTotalCompare = 0;

	if(time1aALTERNATEODgenerationParseVectorGraphicsFileIndex > 0)
	{
		time1aALTERNATEODgenerationParseVectorGraphicsFileAverage = time1aALTERNATEODgenerationParseVectorGraphicsFileTotal/time1aALTERNATEODgenerationParseVectorGraphicsFileIndex;
	}
	if(time1ODgenerationIndexTrain > 0)
	{
		time1ODgenerationAverageTrain = time1ODgenerationTotalTrain / time1ODgenerationIndexTrain;
	}
	if(time1ODgenerationIndexTest > 0)
	{
		time1ODgenerationAverageTest = time1ODgenerationTotalTest / time1ODgenerationIndexTest;
	}

	if(time1aODgenerationLoadImageFileIndexTrain > 0)
	{
		time1aODgenerationLoadImageFileAverageTrain = time1aODgenerationLoadImageFileTotalTrain / time1aODgenerationLoadImageFileIndexTrain;
	}
	if(time1aODgenerationLoadImageFileIndexTest > 0)
	{
		time1aODgenerationLoadImageFileAverageTest = time1aODgenerationLoadImageFileTotalTest / time1aODgenerationLoadImageFileIndexTest;
	}

	if(time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTrain > 0)
	{
		time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTrain = time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTrain / time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTrain;
	}
	if(time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTest > 0)
	{
		time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTest = time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTest / time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTest;
	}

	if(time1bODgenerationCreateInterpolatedMeshIndexTrain > 0)
	{
		time1bODgenerationCreateInterpolatedMeshAverageTrain = time1bODgenerationCreateInterpolatedMeshTotalTrain / time1bODgenerationCreateInterpolatedMeshIndexTrain;
	}
	if(time1bODgenerationCreateInterpolatedMeshIndexTest > 0)
	{
		time1bODgenerationCreateInterpolatedMeshAverageTest = time1bODgenerationCreateInterpolatedMeshTotalTest / time1bODgenerationCreateInterpolatedMeshIndexTest;
	}

	if(time1cODgenerationFeatureListCreationIndexTrain > 0)
	{
		time1cODgenerationFeatureListCreationAverageTrain = time1cODgenerationFeatureListCreationTotalTrain / time1cODgenerationFeatureListCreationIndexTrain;
	}
	if(time1cODgenerationFeatureListCreationIndexTest > 0)
	{
		time1cODgenerationFeatureListCreationAverageTest = time1cODgenerationFeatureListCreationTotalTest / time1cODgenerationFeatureListCreationIndexTest;
	}

	if(time2ObjectTriangleGenerationPolygonListIndexTrain > 0)
	{
		time2ObjectTriangleGenerationPolygonListAverageTrain = time2ObjectTriangleGenerationPolygonListTotalTrain / time2ObjectTriangleGenerationPolygonListIndexTrain;
	}
	if(time2ObjectTriangleGenerationPolygonListIndexTest > 0)
	{
		time2ObjectTriangleGenerationPolygonListAverageTest = time2ObjectTriangleGenerationPolygonListTotalTest / time2ObjectTriangleGenerationPolygonListIndexTest;
	}

	if(time3NormalisedSnapshotGenerationIndexTrain > 0)
	{
		time3NormalisedSnapshotGenerationAverageTrain = time3NormalisedSnapshotGenerationTotalTrain / time3NormalisedSnapshotGenerationIndexTrain;
	}
	if(time3NormalisedSnapshotGenerationIndexTest > 0)
	{
		time3NormalisedSnapshotGenerationAverageTest = time3NormalisedSnapshotGenerationTotalTest / time3NormalisedSnapshotGenerationIndexTest;
	}

	if(time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTrain > 0)
	{
		time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTrain = time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTrain / time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTrain;
	}
	if(time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTest > 0)
	{
		time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTest = time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTest / time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTest;
	}

	if(time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTrain > 0)
	{
		time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTrain = time3aNormalisedSnapshotGenerationTransformDataWRTPolygonTotalTrain / time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTrain;
	}
	if(time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTest > 0)
	{
		time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTest = time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonTotalTest / time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTest;
	}

	if(time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTrain > 0)
	{
		time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTrain = time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTrain / time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTrain;
	}
	if(time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTest > 0)
	{
		time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTest = time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTest / time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTest;
	}

	if(time5NormalisedSnapshotComparisonIndex > 0)
	{
		time5NormalisedSnapshotComparisonAverage = time5NormalisedSnapshotComparisonTotal/time5NormalisedSnapshotComparisonIndex;
	}

	if(time5aNormalisedSnapshotComparisonLoadComparisonDataIndex > 0)
	{
		time5aNormalisedSnapshotComparisonLoadComparisonDataAverage = time5aNormalisedSnapshotComparisonLoadComparisonDataTotal / time5aNormalisedSnapshotComparisonLoadComparisonDataIndex;
	}
	if(time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex > 0)
	{
		time5bNormalisedSnapshotComparisonCompareSnapshotDataAverage = time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal / time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex;
	}

//exitOpenGL();
//exit(0);

	timeTotalTrain = time1aALTERNATEODgenerationParseVectorGraphicsFileTotal+time1ODgenerationTotalTrain+time2ObjectTriangleGenerationPolygonListTotalTrain+time3NormalisedSnapshotGenerationTotalTrain;
	timeTotalTest = time1aALTERNATEODgenerationParseVectorGraphicsFileTotal+time1ODgenerationTotalTest+time2ObjectTriangleGenerationPolygonListTotalTest+time3NormalisedSnapshotGenerationTotalTest;
	timeTotalCompare = time5NormalisedSnapshotComparisonTotal;


	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "time1aALTERNATEODgenerationParseVectorGraphicsFileAverage = " << time1aALTERNATEODgenerationParseVectorGraphicsFileAverage << endl;
		cout << "time1aALTERNATEODgenerationParseVectorGraphicsFileTotal = " << time1aALTERNATEODgenerationParseVectorGraphicsFileTotal << endl;
		cout << "time1aALTERNATEODgenerationParseVectorGraphicsFileIndex = " << time1aALTERNATEODgenerationParseVectorGraphicsFileIndex << endl;

		cout << "time1ODgenerationAverageTrain = " << time1ODgenerationAverageTrain << endl;
		cout << "time1ODgenerationTotalTrain = " << time1ODgenerationTotalTrain << endl;
		cout << "time1ODgenerationIndexTrain = " << time1ODgenerationIndexTrain << endl;
		cout << "time1ODgenerationAverageTest = " << time1ODgenerationAverageTest << endl;
		cout << "time1ODgenerationTotalTest = " << time1ODgenerationTotalTest << endl;
		cout << "time1ODgenerationIndexTest = " << time1ODgenerationIndexTest << endl;

		cout << "time1aODgenerationLoadImageFileAverageTrain = " << time1aODgenerationLoadImageFileAverageTrain << endl;
		cout << "time1aODgenerationLoadImageFileTotalTrain = " << time1aODgenerationLoadImageFileTotalTrain << endl;
		cout << "time1aODgenerationLoadImageFileIndexTrain = " << time1aODgenerationLoadImageFileIndexTrain << endl;
		cout << "time1aODgenerationLoadImageFileAverageTest = " << time1aODgenerationLoadImageFileAverageTest << endl;
		cout << "time1aODgenerationLoadImageFileTotalTest = " << time1aODgenerationLoadImageFileTotalTest << endl;
		cout << "time1aODgenerationLoadImageFileIndexTest = " << time1aODgenerationLoadImageFileIndexTest << endl;

		cout << "time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTrain = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTrain << endl;
		cout << "time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTrain = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTrain << endl;
		cout << "time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTrain = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTrain << endl;
		cout << "time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTest = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileAverageTest << endl;
		cout << "time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTest = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTest << endl;
		cout << "time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTest = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTest << endl;

		cout << "time1bODgenerationCreateInterpolatedMeshAverageTrain = " << time1bODgenerationCreateInterpolatedMeshAverageTrain << endl;
		cout << "time1bODgenerationCreateInterpolatedMeshTotalTrain = " << time1bODgenerationCreateInterpolatedMeshTotalTrain << endl;
		cout << "time1bODgenerationCreateInterpolatedMeshIndexTrain = " << time1bODgenerationCreateInterpolatedMeshIndexTrain << endl;
		cout << "time1bODgenerationCreateInterpolatedMeshAverageTest = " << time1bODgenerationCreateInterpolatedMeshAverageTest << endl;
		cout << "time1bODgenerationCreateInterpolatedMeshTotalTest = " << time1bODgenerationCreateInterpolatedMeshTotalTest << endl;
		cout << "time1bODgenerationCreateInterpolatedMeshIndexTest = " << time1bODgenerationCreateInterpolatedMeshIndexTest << endl;

		cout << "time1cODgenerationFeatureListCreationAverageTrain = " << time1cODgenerationFeatureListCreationAverageTrain << endl;
		cout << "time1cODgenerationFeatureListCreationTotalTrain = " << time1cODgenerationFeatureListCreationTotalTrain << endl;
		cout << "time1cODgenerationFeatureListCreationIndexTrain = " << time1cODgenerationFeatureListCreationIndexTrain << endl;
		cout << "time1cODgenerationFeatureListCreationAverageTest = " << time1cODgenerationFeatureListCreationAverageTest << endl;
		cout << "time1cODgenerationFeatureListCreationTotalTest = " << time1cODgenerationFeatureListCreationTotalTest << endl;
		cout << "time1cODgenerationFeatureListCreationIndexTest = " << time1cODgenerationFeatureListCreationIndexTest << endl;

		cout << "time2ObjectTriangleGenerationPolygonListAverageTrain = " << time2ObjectTriangleGenerationPolygonListAverageTrain << endl;
		cout << "time2ObjectTriangleGenerationPolygonListTotalTrain = " << time2ObjectTriangleGenerationPolygonListTotalTrain << endl;
		cout << "time2ObjectTriangleGenerationPolygonListIndexTrain = " << time2ObjectTriangleGenerationPolygonListIndexTrain << endl;
		cout << "time2ObjectTriangleGenerationPolygonListAverageTest = " << time2ObjectTriangleGenerationPolygonListAverageTest << endl;
		cout << "time2ObjectTriangleGenerationPolygonListTotalTest = " << time2ObjectTriangleGenerationPolygonListTotalTest << endl;
		cout << "time2ObjectTriangleGenerationPolygonListIndexTest = " << time2ObjectTriangleGenerationPolygonListIndexTest << endl;

		cout << "time3NormalisedSnapshotGenerationAverageTrain = " << time3NormalisedSnapshotGenerationAverageTrain << endl;
		cout << "time3NormalisedSnapshotGenerationTotalTrain = " << time3NormalisedSnapshotGenerationTotalTrain << endl;
		cout << "time3NormalisedSnapshotGenerationIndexTrain = " << time3NormalisedSnapshotGenerationIndexTrain << endl;
		cout << "time3NormalisedSnapshotGenerationAverageTest = " << time3NormalisedSnapshotGenerationAverageTest << endl;
		cout << "time3NormalisedSnapshotGenerationTotalTest = " << time3NormalisedSnapshotGenerationTotalTest << endl;
		cout << "time3NormalisedSnapshotGenerationIndexTest = " << time3NormalisedSnapshotGenerationIndexTest << endl;

		cout << "time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTrain = " << time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTrain << endl;
		cout << "time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTrain = " << time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTrain << endl;
		cout << "time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTrain = " << time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTrain << endl;
		cout << "time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTest = " << time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonAverageTest << endl;
		cout << "time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTest = " << time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTest << endl;
		cout << "time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTest = " << time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTest << endl;

		cout << "time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTrain = " << time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTrain << endl;
		cout << "time3aNormalisedSnapshotGenerationTransformDataWRTPolygonTotalTrain = " << time3aNormalisedSnapshotGenerationTransformDataWRTPolygonTotalTrain << endl;
		cout << "time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTrain = " << time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTrain << endl;
		cout << "time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTest = " << time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonAverageTest << endl;
		cout << "time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonTotalTest = " << time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonTotalTest << endl;
		cout << "time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTest = " << time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTest << endl;

		cout << "time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTrain = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTrain << endl;
		cout << "time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTrain = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTrain << endl;
		cout << "time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTrain = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTrain << endl;
		cout << "time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTest = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotAverageTest << endl;
		cout << "time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTest = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTest << endl;
		cout << "time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTest = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTest << endl;

		cout << "time5NormalisedSnapshotComparisonAverage = " << time5NormalisedSnapshotComparisonAverage << endl;
		cout << "time5NormalisedSnapshotComparisonTotal = " << time5NormalisedSnapshotComparisonTotal << endl;
		cout << "time5NormalisedSnapshotComparisonIndex = " << time5NormalisedSnapshotComparisonIndex << endl;


		cout << "time5aNormalisedSnapshotComparisonLoadComparisonDataAverage = " << time5aNormalisedSnapshotComparisonLoadComparisonDataAverage << endl;
		cout << "time5aNormalisedSnapshotComparisonLoadComparisonDataTotal = " << time5aNormalisedSnapshotComparisonLoadComparisonDataTotal << endl;
		cout << "time5aNormalisedSnapshotComparisonLoadComparisonDataIndex = " << time5aNormalisedSnapshotComparisonLoadComparisonDataIndex << endl;

		cout << "time5bNormalisedSnapshotComparisonCompareSnapshotDataAverage = " << time5bNormalisedSnapshotComparisonCompareSnapshotDataAverage << endl;
		cout << "time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal = " << time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal << endl;
		cout << "time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex = " << time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex << endl;


		cout << "timeTotalTrain(1->3) = " << timeTotalTrain << endl;
		cout << "timeTotalTest(1->3) = " << timeTotalTest << endl;
		cout << "timeTotalCompare(5) = " << timeTotalCompare << endl;

		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << endl;
		}
	}
	//#endif


	return result;
}



bool ORmethodTrainOrTest(int dimension, int numberOfObjects, string objectNameArray[], int objectDataSource, RTviewInfo* vi, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygons, int numberOfViewIndiciesPerObject, int numberOfViewIndiciesPerObjectWithUniquePolygons, int* numberOfPolys, int trainOrTest, int numberOfZoomIndicies, int viewNumber, string multViewListFileName)
{
	bool result = true;

	for(int o=0; o<numberOfObjects; o++)		//only used for test harness
	{
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << " objectIndex = " << o << endl;
		}

		#ifdef OR_USE_DATABASE
		#ifdef OR_DATABASE_DEBUG
		cout << "DBgenerateFolderName: objectNameArray[o] = " << objectNameArray[o] << endl;
		#endif
		DBgenerateFolderName(&(objectNameArray[o]), trainOrTest);
		#endif
		string objectSceneFileName = objectNameArray[o] + SCENE_FILE_NAME_EXTENSION;
		string objectSceneFileNameCollapsed = objectNameArray[o] + "CollapsedForRaytracing" + SCENE_FILE_NAME_EXTENSION;
		string topLevelSceneFileName = objectSceneFileName;
		string topLevelSceneFileNameCollapsed = objectSceneFileName;
		LDreference* initialReferenceInSceneFile = new LDreference();
		LDreference* topLevelReferenceInSceneFile = new LDreference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(objectDataSource == OR_OBJECT_DATA_SOURCE_GENERATE_DATA)
		{
			long time1aALTERNATEODgenerationParseVectorGraphicsFileStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "start: 1a. object data generation (ALTERNATE METHOD - part i) - parse vector graphics file" << endl;
				time1aALTERNATEODgenerationParseVectorGraphicsFileStart = getTimeAsLong();
			}

			if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				result = false;
			}
			write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "end: 1a. object data generation (ALTERNATE METHOD - part i) - parse vector graphics file" << endl;
				long time1aALTERNATEODgenerationParseVectorGraphicsFileEnd;
				time1aALTERNATEODgenerationParseVectorGraphicsFileEnd = getTimeAsLong();
				time1aALTERNATEODgenerationParseVectorGraphicsFileTotal = time1aALTERNATEODgenerationParseVectorGraphicsFileTotal + time1aALTERNATEODgenerationParseVectorGraphicsFileEnd-time1aALTERNATEODgenerationParseVectorGraphicsFileStart;
				time1aALTERNATEODgenerationParseVectorGraphicsFileIndex++;
				cout << "timeODgeneration1aALTERNATEParseVectorGraphicsFile = " << time1aALTERNATEODgenerationParseVectorGraphicsFileEnd-time1aALTERNATEODgenerationParseVectorGraphicsFileStart << endl;
			}
		}

	#ifndef OR_DEBUG_METHOD_ASSUME_TRAIN_AND_TEST_WITH_SINGLE_VIEWPOINT

		double viviewupInitial;
		if(objectDataSource == OR_OBJECT_DATA_SOURCE_GENERATE_DATA)
		{
			viviewupInitial = vi->viewUp.x;
		}

		for(int viewIndex = viewNumber; viewIndex < viewNumber+numberOfViewIndiciesPerObjectWithUniquePolygons; viewIndex++)	//only used for test harness
		{
			long time1ODgenerationStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t start: 1. object data generation" << endl;
				time1ODgenerationStart = getTimeAsLong();
			}

			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				if(numberOfViewIndiciesPerObjectWithUniquePolygons > 1)
				{
					cout << "\t\t  viewIndex = " << viewIndex << endl;
				}
			}

			LDreference* firstReferenceInInterpolatedMesh = new LDreference();
			ORmeshPoint* firstMeshPointInMeshList = new ORmeshPoint();
			ORfeature* firstFeatureInList = new ORfeature[numberOfZoomIndicies];
			ORpolygon* firstPolygonInList = new ORpolygon[numberOfZoomIndicies];

			for(int multiViewViewIndex = 0; multiViewViewIndex < numberOfViewIndiciesPerObject; multiViewViewIndex++)
			{
				if(OR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "\t\t  multiViewViewIndex = " << multiViewViewIndex << endl;
				}

				RTviewInfo* viMultiView;
				int objectDataSourceForThisView = objectDataSource;

				if(objectDataSource == OR_OBJECT_DATA_SOURCE_GENERATE_DATA)
				{
					//used to generate multiple views of object defined in 3D from different angles
					double viviewupInitialSub = viviewupInitial + (viewIndex*10);
					vi->viewUp.x = viviewupInitialSub + (multiViewViewIndex*10);
					viMultiView = vi;
				}
				else if(objectDataSource == OR_OBJECT_DATA_SOURCE_USER_LIST)
				{
					//overwrite view info with multiview reference;
					viMultiView = new RTviewInfo();
					string multViewListFileNameWithFullPath = "";
					multViewListFileNameWithFullPath = multViewListFileNameWithFullPath + workingFolderCharStar + "/" + multViewListFileName;
					createViFromMultiViewList(viMultiView, multViewListFileNameWithFullPath, multiViewViewIndex, dimension);
					objectDataSourceForThisView = OR_OBJECT_DATA_SOURCE_USER_FILE;
				}
				else if(objectDataSource == OR_OBJECT_DATA_SOURCE_USER_FILE)
				{
					viMultiView = vi;
				}
				else if(objectDataSource == OR_OBJECT_DATA_SOURCE_PREEXISTING_DATA)
				{
					viMultiView = vi;
				}

				//add to interpolated 3D Map and add to cornerlist
				bool useQuadraticFitEdgeZeroCrossingMap = false;
				if(dimension == OR_METHOD3DOD_DIMENSIONS)
				{
					useQuadraticFitEdgeZeroCrossingMap = OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
				}
				else if(dimension == OR_METHOD2DOD_DIMENSIONS)
				{
					useQuadraticFitEdgeZeroCrossingMap = OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
				}
				else
				{
					cout << "Error: illegal number of dimensions" << endl;
					exit(0);
				}
				if(!createOrAddToInterpolatedMeshAndFeaturesList(initialReferenceInSceneFile, viMultiView, firstReferenceInInterpolatedMesh, firstMeshPointInMeshList, firstFeatureInList, trainOrTest, viewIndex, objectNameArray[o], dimension, objectDataSourceForThisView, numberOfZoomIndicies, useQuadraticFitEdgeZeroCrossingMap))
				{
					result = false;
				}

			}

			if(OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST)
			{
				if(!createInterpolatedMeshReferenceListUsingMeshList(firstMeshPointInMeshList, firstReferenceInInterpolatedMesh, vi, objectNameArray[o], trainOrTest, dimension))
				{
					result = false;
				}
			}

			if(OR_METHOD_USE_MESH_LISTS)
			{
				if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
				{
					if(dimension == OR_METHOD3DOD_DIMENSIONS)
					{
						if(OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED)
						{
							//now find centred features using entire 3Dmesh
							int sensitivity = 1.0;
							ORmeshPoint* firstMeshPointUsedToCalculateCentredFeatures;
							if(OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
							{
								if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
								{
									if(OR_USE_CONTRAST_CALC_METHOD_C)
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
									}
									else
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
									}
								}
								else
								{
									if(OR_USE_CONTRAST_CALC_METHOD_C)
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList->interpixelMeshPoint;
									}
									else
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
									}
								}
								if(!addCentredFeaturesToFeatureListUsingMeshList(firstFeatureInList, IRRELEVANT, dimension, firstMeshPointUsedToCalculateCentredFeatures, IRRELEVANT, OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL))
								{
									result = false;
								}
							}
							else
							{
								if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
								{
									if(OR_USE_CONTRAST_CALC_METHOD_C)
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
									}
									else
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
									}
									if(!addCentredFeaturesToFeatureListUsingMeshList(firstFeatureInList, EDGE_NORMAL_CONTRAST_THRESHOLD, dimension, firstMeshPointUsedToCalculateCentredFeatures, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST, OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL))
									{
										result = false;
									}
								}
								else
								{
									if(OR_USE_CONTRAST_CALC_METHOD_C)	//check this...
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList->interpixelMeshPoint;
									}
									else
									{
										firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
									}
									if(!addCentredFeaturesToFeatureListUsingMeshList(firstFeatureInList, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, firstMeshPointUsedToCalculateCentredFeatures, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST, OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL))
									{
										result = false;
									}


								}
							}
						}
					}
					else
					{
						if(OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED)
						{
							//now find centred features using entire 2Dmesh
							int sensitivity = 1.0;
							ORmeshPoint* firstMeshPointUsedToCalculateCentredFeatures;

							if(OR_USE_CONTRAST_CALC_METHOD_C)
							{
								firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList->interpixelMeshPoint;
							}
							else
							{
								firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
							}
							if(!addCentredFeaturesToFeatureListUsingMeshList(firstFeatureInList, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, firstMeshPointInMeshList, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST, OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL))
							{
								result = false;
							}
						}
					}
				}
			}

			#ifdef OR_METHOD_ASSUME_TRAIN_AND_TEST_WITH_MULTI_VIEWPOINT_ADVANCED_VERSION
				//not yet coded.
			//really should be adding proper 3DOD corner detection code here (using generateFeatureListUsing3D data instead of addCornerFeaturesToFeatureListUsingRGBmap);
			cout << "error: OR_METHOD_ASSUME_TRAIN_AND_TEST_WITH_MULTI_VIEWPOINT_ADVANCED_VERSION not yet coded" << endl;
			//generateFeatureListUsing3DSnapshot(vi, rgbMap, firstFeatureInList, trainOrTest, mapFileName, sensitivity)
			#endif

			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t\t end: 1. object data generation" << endl;
				long time1ODgenerationEnd;
				time1ODgenerationEnd = getTimeAsLong();

				if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
				{
					time1ODgenerationTotalTrain = time1ODgenerationTotalTrain+ time1ODgenerationEnd-time1ODgenerationStart;
					time1ODgenerationIndexTrain++;
				}
				else if(trainOrTest == TEST)
				{
					time1ODgenerationTotalTest = time1ODgenerationTotalTest+ time1ODgenerationEnd-time1ODgenerationStart;
					time1ODgenerationIndexTest++;
				}

				cout << "\t time1ODgeneration = " << time1ODgenerationEnd-time1ODgenerationStart << endl;

			}


			#ifndef DEBUG_ONLY_GENERATE_FEATURE_LISTS_NOTHING_ELSE

			long time2ObjectTriangleGenerationPolygonListStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t start: 2. object triangle generation - polygon list creation" << endl;
				time2ObjectTriangleGenerationPolygonListStart = getTimeAsLong();
			}

			if(!generatePolygonListUsingFeatureListLocalised(vi->imageWidth, vi->imageHeight, firstFeatureInList, firstPolygonInList, numberOfZoomIndicies, dimension))
			//if(!generatePolygonListUsingFeatureList(vi->imageWidth, vi->imageHeight, firstFeatureInList, firstPolygonInList))
			{
				result = false;
			}

			long time3NormalisedSnapshotGenerationStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t end: 2. object triangle generation - polygon list creation" << endl;
				long time2ObjectTriangleGenerationPolygonListEnd;
				time2ObjectTriangleGenerationPolygonListEnd = getTimeAsLong();

				if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
				{
					time2ObjectTriangleGenerationPolygonListTotalTrain = time2ObjectTriangleGenerationPolygonListTotalTrain + time2ObjectTriangleGenerationPolygonListEnd-time2ObjectTriangleGenerationPolygonListStart;
					time2ObjectTriangleGenerationPolygonListIndexTrain++;
				}
				else if(trainOrTest == TEST)
				{
					time2ObjectTriangleGenerationPolygonListTotalTest = time2ObjectTriangleGenerationPolygonListTotalTest + time2ObjectTriangleGenerationPolygonListEnd-time2ObjectTriangleGenerationPolygonListStart;
					time2ObjectTriangleGenerationPolygonListIndexTest++;
				}

				cout << "\t time2ObjectTriangleGenerationPolygonList = " << time2ObjectTriangleGenerationPolygonListEnd-time2ObjectTriangleGenerationPolygonListStart << endl;
				cout << "\t start: 3. normalised snapshot generation" << endl;
				time3NormalisedSnapshotGenerationStart = getTimeAsLong();
			}
				//the following is required to be used instead for shapes with rounded edges [eg cylinders] - ie shapes with large numbers of polygons;
				//void generatePolygonsUsingFeatureArraysEfficientNOTCOMPLETE(int imageWidth, int imageHeight, double* depthMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3])
			#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
			if(!generateNormalisedSnapshotsExperienceListUsingPolyList(firstReferenceInInterpolatedMesh, firstPolygonInList, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, firstExperienceInList, &(numberOfPolys[o*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies+0*numberOfZoomIndicies]), trainOrTest, viewIndex, objectNameArray[o], dimension, firstFeatureInList))
			#else
			if(!generateNormalisedSnapshotsUsingPolyList(firstReferenceInInterpolatedMesh, firstPolygonInList, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, &(numberOfPolys[o*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies+0*numberOfZoomIndicies]), trainOrTest, viewIndex, objectNameArray[o], dimension, firstFeatureInList, numberOfZoomIndicies))
			#endif
			{
				result = false;
			}


			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t end: 3. normalised snapshot generation" << endl;
				long time3NormalisedSnapshotGenerationEnd;
				time3NormalisedSnapshotGenerationEnd = getTimeAsLong();

				if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
				{
					time3NormalisedSnapshotGenerationTotalTrain = time3NormalisedSnapshotGenerationTotalTrain+ time3NormalisedSnapshotGenerationEnd-time3NormalisedSnapshotGenerationStart;
					time3NormalisedSnapshotGenerationIndexTrain++;
				}
				else if(trainOrTest == TEST)
				{
					time3NormalisedSnapshotGenerationTotalTest = time3NormalisedSnapshotGenerationTotalTest+ time3NormalisedSnapshotGenerationEnd-time3NormalisedSnapshotGenerationStart;
					time3NormalisedSnapshotGenerationIndexTest++;
				}

				cout << "\t time3NormalisedSnapshotGeneration1 = " << time3NormalisedSnapshotGenerationEnd-time3NormalisedSnapshotGenerationStart << endl;

			}
			#endif

			//delete firstReferenceInInterpolatedMesh;	//this cannot be deleted because it is still used by glutDisplayFunc
			//cout << "deleted firstReferenceInInterpolatedMesh" << endl;

			/*
			//NEED TO FIX THIS - MEM ISSUE
			for(int zoomIndex=0; zoomIndex<numberOfZoomIndicies; zoomIndex++)
			{
				delete &(firstFeatureInList[zoomIndex]);
				delete &(firstPolygonInList[zoomIndex]);
			}
			*/
			/*
			delete firstFeatureInList;
			delete firstPolygonInList;
			*/
		}
	#else

		double viviewupInitial = vi->viewUp.x;

		for(int viewIndex = viewNumber; viewIndex < viewNumber+numberOfViewIndiciesPerObjectWithUniquePolygons; viewIndex++)
		{
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "\t\t  viewIndex = " << viewIndex << endl;
			}

			long time1ODgenerationStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t\t start: 1. object data generation" << endl;
				time1ODgenerationStart = getTimeAsLong();
			}

			LDreference* firstReferenceInInterpolatedMesh = new LDreference();
			ORmeshPoint* firstMeshPointInMeshList = new ORmeshPoint();
			ORfeature* firstFeatureInList = new ORfeature[numberOfZoomIndicies];
			ORpolygon* firstPolygonInList = new ORpolygon[numberOfZoomIndicies];

			vi->viewUp.x = viviewupInitial + (viewIndex*10);

			//add to interpolated 2D/3D Map and add to feature list
			bool useQuadraticFitEdgeZeroCrossingMap = false;
			if(dimension == OR_METHOD3DOD_DIMENSIONS)
			{
				useQuadraticFitEdgeZeroCrossingMap = OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
			}
			else if(dimension == OR_METHOD2DOD_DIMENSIONS)
			{
				useQuadraticFitEdgeZeroCrossingMap = OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
			}
			else
			{
				cout << "Error: illegal number of dimensions" << endl;
				exit(0);
			}
			if(!createOrAddToInterpolatedMeshAndFeaturesList(initialReferenceInSceneFile, vi, firstReferenceInInterpolatedMesh, firstMeshPointInMeshList, firstFeatureInList, trainOrTest, viewIndex, objectNameArray[o], dimension, objectDataSource, numberOfZoomIndicies, useQuadraticFitEdgeZeroCrossingMap))
			{
				result = false;
			}

			if(OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST)
			{
				if(!createInterpolatedMeshReferenceListUsingMeshList(firstMeshPointInMeshList, firstReferenceInInterpolatedMesh, vi, objectNameArray[o], trainOrTest, dimension))
				{
					result = false;
				}
			}


			//now find centred features using entire 3Dmesh
			if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
			{

			}

			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t\t end: 1. object data generation" << endl;
				long time1ODgenerationEnd;
				time1ODgenerationEnd = getTimeAsLong();

				if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
				{
					time1ODgenerationTotalTrain = time1ODgenerationTotalTrain+ time1ODgenerationEnd-time1ODgenerationStart;
					time1ODgenerationIndexTrain++;
				}
				else if(trainOrTest == TEST)
				{
					time1ODgenerationTotalTest = time1ODgenerationTotalTest+ time1ODgenerationEnd-time1ODgenerationStart;
					time1ODgenerationIndexTest++;
				}

				cout << "\t\t time1ODgeneration = " << time1ODgenerationEnd-time1ODgenerationStart << endl;
			}

		#ifndef DEBUG_ONLY_GENERATE_FEATURE_LISTS_NOTHING_ELSE

			long time2ObjectTriangleGenerationPolygonListStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t\t start: 2. object triangle generation - polygon list creation" << endl;
				time2ObjectTriangleGenerationPolygonListStart = getTimeAsLong();
			}

			if(!generatePolygonListUsingFeatureListLocalised(vi->imageWidth, vi->imageHeight, firstFeatureInList, firstPolygonInList, numberOfZoomIndicies, dimension))
			{
				result = false;
			}

			long time3NormalisedSnapshotGenerationStart;
			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t\t end: 2. object triangle generation - polygon list creation" << endl;
				long time2ObjectTriangleGenerationPolygonListEnd;
				time2ObjectTriangleGenerationPolygonListEnd = getTimeAsLong();

				if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
				{
					time2ObjectTriangleGenerationPolygonListTotalTrain = time2ObjectTriangleGenerationPolygonListTotalTrain + time2ObjectTriangleGenerationPolygonListEnd-time2ObjectTriangleGenerationPolygonListStart;
					time2ObjectTriangleGenerationPolygonListIndexTrain++;
				}
				else if(trainOrTest == TEST)
				{
					time2ObjectTriangleGenerationPolygonListTotalTest = time2ObjectTriangleGenerationPolygonListTotalTest + time2ObjectTriangleGenerationPolygonListEnd-time2ObjectTriangleGenerationPolygonListStart;
					time2ObjectTriangleGenerationPolygonListIndexTest++;
				}

				cout << "\t\t time2ObjectTriangleGenerationPolygonList = " << time2ObjectTriangleGenerationPolygonListEnd-time2ObjectTriangleGenerationPolygonListStart << endl;
				cout << "\t\t start: 3. normalised snapshot generation" << endl;

				time3NormalisedSnapshotGenerationStart = getTimeAsLong();
			}

				//the following is required to be used instead for shapes with rounded edges [eg cylinders] - ie shapes with large numbers of polygons;
				//void generatePolygonsUsingFeatureArraysEfficientNOTCOMPLETE(int imageWidth, int imageHeight, double* depthMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3])
			#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
			if(!generateNormalisedSnapshotsExperienceListUsingPolyList(firstReferenceInInterpolatedMesh, firstPolygonInList, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, firstExperienceInList, &(numberOfPolys[o*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies+viewIndex*numberOfZoomIndicies]), trainOrTest, viewIndex, objectNameArray[o], dimension, firstFeatureInList))
			#else
			if(!generateNormalisedSnapshotsUsingPolyList(firstReferenceInInterpolatedMesh, firstPolygonInList, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, &(numberOfPolys[o*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies+viewIndex*numberOfZoomIndicies]), trainOrTest, viewIndex, objectNameArray[o], dimension, firstFeatureInList, numberOfZoomIndicies))
			#endif
			{
				result = false;
			}

			if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
			{
				cout << "\t\t end: 3. normalised snapshot generation" << endl;
				long time3NormalisedSnapshotGenerationEnd;
				time3NormalisedSnapshotGenerationEnd = getTimeAsLong();

				if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
				{
					time3NormalisedSnapshotGenerationTotalTrain = time3NormalisedSnapshotGenerationTotalTrain+ time3NormalisedSnapshotGenerationEnd-time3NormalisedSnapshotGenerationStart;
					time3NormalisedSnapshotGenerationIndexTrain++;
				}
				else if(trainOrTest == TEST)
				{
					time3NormalisedSnapshotGenerationTotalTest = time3NormalisedSnapshotGenerationTotalTest+ time3NormalisedSnapshotGenerationEnd-time3NormalisedSnapshotGenerationStart;
					time3NormalisedSnapshotGenerationIndexTest++;
				}

				cout << "\t\t time3NormalisedSnapshotGeneration2 = " << time3NormalisedSnapshotGenerationEnd-time3NormalisedSnapshotGenerationStart << endl;

			}
		#endif

			/*
			//NEED TO FIX THIS - MEM ISSUE
			for(int zoomIndex=0; zoomIndex<numberOfZoomIndicies; zoomIndex++)
			{
				delete &(firstFeatureInList[zoomIndex]);
				delete &(firstPolygonInList[zoomIndex]);
			}
			*/
			/*
			delete firstFeatureInList;
			delete firstPolygonInList;
			*/

			delete firstReferenceInInterpolatedMesh;
		}
	#endif

	}

	return result;
}








bool createOrAddToInterpolatedMeshAndFeaturesList(LDreference* initialReferenceInSceneFile, RTviewInfo* vi, LDreference* firstReferenceInInterpolatedMesh, ORmeshPoint* firstMeshPointInMeshList, ORfeature firstFeatureInList[], int trainOrTest, int viewIndex, string objectName, int dimension, int objectDataSource, int numberOfZoomIndicies, bool useEdgeZeroCrossingMap)
{
	bool result = true;

	int imageWidth = vi->imageWidth;
	int imageHeight = vi->imageHeight;

	double* pointMap = new double[imageWidth*imageHeight*VEC_MAP_VEC_NUM_DIMENSIONS];	//NOT USED for 2D!
	unsigned char* rgbMap = new unsigned char[imageWidth*imageHeight*RGB_NUM];
	double* depthMap = new double[imageWidth*imageHeight];					//NOT USED for 2D!

	ORmeshPoint* firstMeshPointUsedToCalculateCentredFeatures = NULL;

	if(!createRGBandPointMap(initialReferenceInSceneFile, pointMap, rgbMap, depthMap, vi, trainOrTest, viewIndex, objectName, dimension, objectDataSource))
	{
		result = false;
	}

	if(!OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST)
	{
		if(!createOrAddToInterpolatedMeshReferenceListUsingPointAndRGBMap(pointMap, rgbMap, firstReferenceInInterpolatedMesh, vi, objectName, trainOrTest, dimension, viewIndex))
		{
			result = false;
		}
	}


#ifndef LINUX
	ORmeshPoint** meshPointArray = new ORmeshPoint* [imageWidth*imageHeight];
#else
	ORmeshPoint* meshPointArray[imageWidth*imageHeight];	//not currently used in ORmethod.cpp - could be used in sub function to enhance efficiency of FD algorithms [contains same MeshPoints as Mesh list]
#endif
	if(OR_METHOD_USE_MESH_LISTS)
	{
		if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{
			if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
			{
				create3DmeshUsingPointMap3DOD(imageWidth, imageHeight, pointMap, depthMap, rgbMap, firstMeshPointInMeshList, meshPointArray, useEdgeZeroCrossingMap, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST, vi);
				firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
			}
			else
			{
				create3DmeshUsingPointMap3DOD(imageWidth, imageHeight, pointMap, depthMap, rgbMap, firstMeshPointInMeshList, meshPointArray, useEdgeZeroCrossingMap, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST, vi);
				if(OR_USE_CONTRAST_CALC_METHOD_C)
				{
					firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList->interpixelMeshPoint;
				}
				else
				{
					firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
				}
			}
		}
		else if(dimension == OR_METHOD2DOD_DIMENSIONS)
		{
			create2DmeshUsingRGBmap2DOD(imageWidth, imageHeight, vi->xOffset, vi->yOffset, rgbMap, firstMeshPointInMeshList, meshPointArray, useEdgeZeroCrossingMap);
			if(OR_USE_CONTRAST_CALC_METHOD_C)
			{
				firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList->interpixelMeshPoint;
			}
			else
			{
				firstMeshPointUsedToCalculateCentredFeatures = firstMeshPointInMeshList;
			}
		}
	}

	ORmeshPoint* currentMeshPointInMeshList = firstMeshPointInMeshList;

	#ifdef OR_DEBUG
	/*
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double depth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			cout << depth << " ";
		}
		cout << "\n" << endl;
	}
	*/
	#endif

	if(!createOrAddPointsToFeaturesList(pointMap, rgbMap, depthMap, firstFeatureInList, vi, trainOrTest, viewIndex, objectName, dimension, numberOfZoomIndicies, firstMeshPointUsedToCalculateCentredFeatures, meshPointArray, useEdgeZeroCrossingMap))
	{
		result = false;
	}

	delete pointMap;
	delete rgbMap;
	delete depthMap;

	#ifndef LINUX
	delete meshPointArray;
	#endif

	return result;
}



bool createRGBandPointMap(LDreference* initialReferenceInSceneFile, double* pointMap, unsigned char* rgbMap, double* depthMap, RTviewInfo* vi, int trainOrTest, int viewIndex, string objectName, int dimension, int objectDataSource)
{
	bool result = true;

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	char viewIndexString[10];
	sprintf(viewIndexString, "%d",  viewIndex);

	string mapFileName = objectName + "initialViewMap" + "ViewIndex" + viewIndexString + "ZoomIndex" + "0";

	int imageWidth = vi->imageWidth;
	int imageHeight = vi->imageHeight;

	double* normalMap = new double[imageWidth*imageHeight*VEC_MAP_VEC_NUM_DIMENSIONS];	//NOT USED for 2D!
	double* luminosityMap = new double[imageWidth*imageHeight];
	bool* luminosityBooleanMap = new bool[imageWidth*imageHeight];
	double* luminosityContrastMap = new double[imageWidth* imageHeight];
	bool* luminosityContrastBooleanMap = new bool[imageWidth*imageHeight];
	bool* foregroundDepthCheckLuminosityContrastBooleanMap = new bool[imageWidth*imageHeight];

	double* depthContrastMap = new double[imageWidth* imageHeight];	//NOT USED FOR 2D!
	bool* depthContrastBooleanMap = new bool[imageWidth*imageHeight];	//NOT USED FOR 2D!
	bool* foregroundDepthCheckDepthContrastBooleanMap = new bool[imageWidth*imageHeight];
	double* depthGradientMap = new double[imageWidth*imageHeight*DEPTH_GRADIENT_MAP_VEC_NUM_DIMENSIONS];	//NOT USED FOR 2D!
	double* depthGradientContrastMap = new double[imageWidth*imageHeight];		//NOT USED FOR 2D!
	bool* depthGradientContrastBooleanMap = new bool[imageWidth*imageHeight];		//NOT USED FOR 2D!
	bool* luminosityContrastMapMinusDepthContrastMap = new bool[imageWidth*imageHeight];	//NOT USED FOR 2D!

	double* pointNormalMap = new double[imageWidth*imageHeight*VECTOR_NUM_VALUES];		//NOT USED FOR 2D!
	double* pointNormalContrastMap = new double[imageWidth*imageHeight];			//NOT USED FOR 2D!
	bool* pointNormalContrastBooleanMap = new bool[imageWidth*imageHeight];		//NOT USED FOR 2D!

	if(objectDataSource == OR_OBJECT_DATA_SOURCE_PREEXISTING_DATA)
	{
		long time1aODgenerationLoadImageFileStart;
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
		{
			cout << "\t\t\t start: 1a. object data generation - load image file" << endl;
			time1aODgenerationLoadImageFileStart = getTimeAsLong();
		}

		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "Preexisting image dataset: preconditions" << endl;
			cout << "\t 1. imagemagik is installed (for png to ppm conversion)" << endl;
			cout << "\t 2. preexisting images are placed in current directory; eg 868_r0.png, 868_r5.png, 868_r10.png, ... etc" << endl;
		}

		char rotationviewIndexString[10];
		sprintf(rotationviewIndexString, "%d",  (viewIndex*PREEXISTING_IMAGE_DATA_VIEW_INDEX_DEGREES));


	#ifdef OR_USE_SINGLE_TRAIN_STAR_MAP
		string preexistingImageFileName;
		if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
		{
			preexistingImageFileName = objectName + PNG_EXTENSION;
		}
		else if(trainOrTest == TEST)
		{
			preexistingImageFileName = objectName + "_r" + rotationviewIndexString + PNG_EXTENSION;
		}
	#else
		string preexistingImageFileName = objectName + "_r" + rotationviewIndexString + PNG_EXTENSION;
	#endif

		#ifdef OR_DEBUG
		//cout << "preexistingImageFileName = " << preexistingImageFileName << endl;
		#endif

		string rgbMapFileName = mapFileName + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;

		string convertPNGtoPPMCommand = "";
		convertPNGtoPPMCommand = convertPNGtoPPMCommand + "convert " + workingFolderCharStar + "/" + preexistingImageFileName + " " + rgbMapFileName;
		cout << "system(" << convertPNGtoPPMCommand << ");" << endl;
		system(convertPNGtoPPMCommand.c_str());


		pixmap* rgbPixMap;
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "rgbPixMap = loadPPM(" << rgbMapFileName << ");" << endl;
		}
		rgbPixMap = loadPPM(rgbMapFileName);
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "imageWidth = " << imageWidth << endl;
			cout << "imageHeight = " << imageHeight << endl;
			cout << "createRGBMapFromPixmapImage(rgbPixMap, rgbMap);" << endl;
		}
		createRGBMapFromPixmapImage(rgbPixMap, rgbMap);
		freePixmap(rgbPixMap);

		if(dimension != OR_METHOD2DOD_DIMENSIONS)
		{
			cout << "error: OR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA requires OR_METHOD2DOD_DIMENSIONS" << endl;
		}

		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
		{
			cout << "\t\t\t end: 1a. object data generation - load image file" << endl;
			long time1aODgenerationLoadImageFileEnd;
			time1aODgenerationLoadImageFileEnd = getTimeAsLong();
			if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
			{
				time1aODgenerationLoadImageFileTotalTrain = time1aODgenerationLoadImageFileTotalTrain+ time1aODgenerationLoadImageFileEnd-time1aODgenerationLoadImageFileStart;
				time1aODgenerationLoadImageFileIndexTrain++;
			}
			else if(trainOrTest == TEST)
			{
				time1aODgenerationLoadImageFileTotalTest = time1aODgenerationLoadImageFileTotalTest+ time1aODgenerationLoadImageFileEnd-time1aODgenerationLoadImageFileStart;
				time1aODgenerationLoadImageFileIndexTest++;
			}
			cout << "\t\t\t time1aODgenerationLoadImageFile = " << time1aODgenerationLoadImageFileEnd-time1aODgenerationLoadImageFileStart << endl;
		}

	}
	else if(objectDataSource == OR_OBJECT_DATA_SOURCE_USER_FILE)
	{
		long time1aODgenerationLoadImageFileStart;
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
		{
			cout << "\t\t\t start: 1a. object data generation - load image file" << endl;
			time1aODgenerationLoadImageFileStart = getTimeAsLong();
		}

		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "Preexisting image dataset: preconditions" << endl;
			cout << "\t 1. imagemagik is installed (for png to ppm conversion)" << endl;
			cout << "\t 2. preexisting images are placed in current directory; eg house.png, house.depth.png" << endl;
		}

		string objectImageName;

		if(viewIndex > 0)
		{
			//must have view number appending file name
			objectImageName = vi->objectName + ".view" + viewIndexString + vi->imageExtensionName;

		}
		else
		{
			objectImageName = vi->objectName + vi->imageExtensionName;
		}

		string rgbMapFileName = mapFileName + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;

		string convertPNGtoPPMCommand = "";
		convertPNGtoPPMCommand = convertPNGtoPPMCommand + "convert " + workingFolderCharStar + "/" + objectImageName + " " + rgbMapFileName;
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "system(" << convertPNGtoPPMCommand << ");" << endl;
		}
		system(convertPNGtoPPMCommand.c_str());

		pixmap* rgbPixMap;
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "rgbPixMap = loadPPM(" << rgbMapFileName << ");" << endl;
		}
		rgbPixMap = loadPPM(rgbMapFileName);
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "imageWidth = " << imageWidth << endl;
			cout << "imageHeight = " << imageHeight << endl;
			cout << "createRGBMapFromPixmapImage(rgbPixMap, rgbMap);" << endl;
		}
		createRGBMapFromPixmapImage(rgbPixMap, rgbMap);
		freePixmap(rgbPixMap);


		if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{
			string objectDepthName = "";

			if(viewIndex > 0)
			{
				//must have view number appending file name
				objectDepthName = objectDepthName + vi->objectName + ".view" + viewIndexString + vi->depthExtensionName;
			}
			else
			{
				objectDepthName = objectDepthName + vi->objectName + vi->depthExtensionName;
			}

			string depthMap24BitFileName = mapFileName + DEPTHMAP24BIT_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;

			string convertPNGtoPPMCommand = "";
			convertPNGtoPPMCommand = convertPNGtoPPMCommand + "convert " + workingFolderCharStar + "/" + objectDepthName + " " + depthMap24BitFileName;
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "system(" << convertPNGtoPPMCommand << ");" << endl;
			}
			system(convertPNGtoPPMCommand.c_str());


			pixmap* depth24BitPixMap;
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "depth24BitPixMap = loadPPM(" << depthMap24BitFileName << ");" << endl;
			}
			depth24BitPixMap = loadPPM(depthMap24BitFileName);
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "imageWidth = " << imageWidth << endl;
				cout << "imageHeight = " << imageHeight << endl;
				cout << "createDepthMapFromDepth24BitPixmapImage(depth24BitPixMap, depthMap);" << endl;
			}
			createDepthMapFromDepth24BitPixmapImage(depth24BitPixMap, depthMap, vi->depthScale, 0.0);
			freePixmap(depth24BitPixMap);

				//debug;
			string depthMapTempTestName = mapFileName + "temptest" + DEPTHMAP24BIT_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
			string depthMap24BitTempTestFileName = mapFileName + "temptest" + DEPTHMAP24BIT_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
			generatePixmapFromDepthMapOrDepthContrastMap(depthMapTempTestName, imageWidth, imageHeight, depthMap);
			generatePixmapFromDepthMap24Bit(depthMap24BitTempTestFileName, imageWidth, imageHeight, depthMap, vi->depthScale, 0.0);


			createPointMapFromDepthMap(imageWidth, imageHeight, depthMap, pointMap, vi);

			#ifdef OR_DEBUG_3DOD_POV
			printPointMap(imageWidth, imageHeight, pointMap);
			printDepthMap(imageWidth, imageHeight, depthMap);
			printvi(vi);
			#endif


		}



		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
		{
			cout << "\t\t\t end: 1a. object data generation - load image file" << endl;
			long time1aODgenerationLoadImageFileEnd;
			time1aODgenerationLoadImageFileEnd = getTimeAsLong();
			if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
			{
				time1aODgenerationLoadImageFileTotalTrain = time1aODgenerationLoadImageFileTotalTrain+ time1aODgenerationLoadImageFileEnd-time1aODgenerationLoadImageFileStart;
				time1aODgenerationLoadImageFileIndexTrain++;
			}
			else if(trainOrTest == TEST)
			{
				time1aODgenerationLoadImageFileTotalTest = time1aODgenerationLoadImageFileTotalTest+ time1aODgenerationLoadImageFileEnd-time1aODgenerationLoadImageFileStart;
				time1aODgenerationLoadImageFileIndexTest++;
			}
			cout << "\t\t\t time1aODgenerationLoadImageFile = " << time1aODgenerationLoadImageFileEnd-time1aODgenerationLoadImageFileStart << endl;
		}
	}
	else if(objectDataSource == OR_OBJECT_DATA_SOURCE_GENERATE_DATA)
	{
		long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileStart;
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
		{
			cout << "\t\t\t start: 1a. object data generation (ALTERNATE METHOD - part ii) - raytrace vector graphics file" << endl;
			time1aALTERNATEODgenerationRaytraceVectorGraphicsFileStart = getTimeAsLong();
		}

		string interpolatedMapFileNameForRayTracingTAL = mapFileName + trainOrTestString + TAL_EXTENSION;
		/*//TEMP REPLACED FOR DEBUG
		string interpolatedMapFileNameForRayTracingTAL = mapFileName + TAL_EXTENSION;
		*/
		char* charstarinterpolatedMapFileNameForRayTracingTAL = const_cast<char*>(interpolatedMapFileNameForRayTracingTAL.c_str());



		write2DReferenceListCollapsedTo1DToFileRayTraceFormat(charstarinterpolatedMapFileNameForRayTracingTAL, initialReferenceInSceneFile, true, vi, false, NULL, NULL);



		setLightingMode(LIGHTING_MODE_AMBIENT_DIFFUSE_SPECULAR);	//TEMP: LIGHTING_MODE_BASIC	//DEFAULT: LIGHTING_MODE_AMBIENT_DIFFUSE_SPECULAR
	#ifdef TH_OR_USE_SHIELD_LDR_FILE
		setSceneLightingConditions(AMBIENT_RED, AMBIENT_GREEN, AMBIENT_BLUE, 0.0, DIFFUSE);	//SPECULAR //DIFFUSE
	#endif


		rayTraceScene(charstarinterpolatedMapFileNameForRayTracingTAL, NULL, false, true, rgbMap, depthMap, normalMap, pointMap);
		#ifdef OR_OVERWRITE_POINT_MAP_USING_CONSISTENT_POINT_MAP_CREATION
		createPointMapFromDepthMap(imageWidth, imageHeight, depthMap, pointMap, vi);		//this is to overwrite the default point map using a consistent point map generation
		#endif

		string rgbMapFileName = mapFileName + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;

		createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMap, luminosityMap);
		createLuminosityBooleanMap(imageWidth, imageHeight, luminosityMap, luminosityBooleanMap);


		if(dimension == OR_METHOD2DOD_DIMENSIONS)
		{
			createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);
			createLuminosityContrastBooleanMap(imageWidth, imageHeight, luminosityContrastMap, luminosityContrastBooleanMap);
		}
		else if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{
			#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
			createContrastMapFromMapWithForegroundDepthCheck(imageWidth, imageHeight, luminosityMap, depthMap, luminosityContrastMap, luminosityContrastBooleanMap, foregroundDepthCheckLuminosityContrastBooleanMap, EDGE_LUMINOSITY_CONTRAST_THRESHOLD);
			#else
			createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);
			createLuminosityContrastBooleanMap(imageWidth, imageHeight, luminosityContrastMap, luminosityContrastBooleanMap);
			#endif
		}
		else
		{
			cout << "Error: illegal number of dimensions" << endl;
			exit(0);
		}

		if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{
			#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
				createContrastMapFromMapWithForegroundDepthCheck(imageWidth, imageHeight, depthMap, depthMap, depthContrastMap, depthContrastBooleanMap, foregroundDepthCheckDepthContrastBooleanMap, EDGE_DEPTH_CONTRAST_THRESHOLD);
			#else
				createContrastMapFromMap(imageWidth, imageHeight, depthMap, depthContrastMap);
				createDepthContrastBooleanMap(imageWidth, imageHeight, depthContrastMap, depthContrastBooleanMap);
			#endif
			createDepthGradientMapFromDepthMap(imageWidth, imageHeight, depthMap, depthGradientMap);
			createDepthGradientContrastMapFromDepthGradientMap(imageWidth, imageHeight, depthGradientMap, depthGradientContrastMap);
			subtractBooleanMaps(imageWidth, imageHeight, luminosityContrastBooleanMap, depthContrastBooleanMap, luminosityContrastMapMinusDepthContrastMap);
			createDepthGradientContrastBooleanMap(imageWidth, imageHeight, depthGradientContrastMap, depthGradientContrastBooleanMap);
			//subtractBooleanMaps(imageWidth, imageHeight, depthGradientContrastBooleanMap, depthContrastBooleanMap, depthGradientContrastMapMinusDepthContrastMap);
			//addBooleanMaps(imageWidth, imageHeight, depthGradientContrastBooleanMap, depthContrastBooleanMap, depthGradientContrastMapPlusDepthContrastMap);

			createPointNormalMapFromPointMap(imageWidth, imageHeight, pointMap, pointNormalMap);
			createPointNormalContrastMapFromPointNormalMap(imageWidth, imageHeight, pointNormalMap, pointNormalContrastMap);
			createPointNormalContrastBooleanMap(imageWidth, imageHeight, pointNormalContrastMap, pointNormalContrastBooleanMap);

		}


		string luminosityMapFileName = mapFileName + LUMINOSITY_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
		string luminosityBooleanMapFileName = mapFileName + LUMINOSITY_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
		string luminosityContrastMapFileName = mapFileName + LUMINOSITY_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
		string luminosityContrastBooleanMapFileName = mapFileName + LUMINOSITY_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;

		string depthMap24BitFileName = mapFileName + DEPTHMAP24BIT_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;

		string normalMapFileName = mapFileName + NORMALMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string depthMapFileName = mapFileName + DEPTHMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string depthContrastMapFileName = mapFileName + DEPTH_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string depthContrastBooleanMapFileName = mapFileName + DEPTH_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string depthGradientMapFileName = mapFileName + DEPTH_GRADIENT_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string depthGradientContrastMapFileName = mapFileName + DEPTH_GRADIENT_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string depthGradientContrastBooleanMapFileName = mapFileName + DEPTH_GRADIENT_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string luminosityContrastMapMinusDepthContrastMapFileName = mapFileName + LUMINOSITY_CONTRAST_MINUS_DEPTH_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!

		string pointNormalContrastMapFileName = mapFileName + POINT_NORMAL_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!
		string pointNormalContrastBooleanMapFileName = mapFileName + POINT_NORMAL_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED FOR 2D!

		generatePixmapFromRGBmap(rgbMapFileName, imageWidth, imageHeight, rgbMap);
		generatePixmapFromBooleanMap(luminosityBooleanMapFileName, imageWidth, imageHeight, luminosityBooleanMap);
		generatePixmapFromLuminosityContrastMap(luminosityContrastMapFileName, imageWidth, imageHeight, luminosityContrastMap);
		generatePixmapFromBooleanMap(luminosityContrastBooleanMapFileName, imageWidth, imageHeight, luminosityContrastBooleanMap);

		if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{
			generatePixmapFromNormalMap(normalMapFileName, imageWidth, imageHeight, normalMap);
			generatePixmapFromDepthMapOrDepthContrastMap(depthMapFileName, imageWidth, imageHeight, depthMap);
			if(objectDataSource == OR_OBJECT_DATA_SOURCE_GENERATE_DATA)
			{
				vi->depthScale = OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE;
				generatePixmapFromDepthMap24Bit(depthMap24BitFileName, imageWidth, imageHeight, depthMap, vi->depthScale, 0.0);
				#ifdef OR_DEBUG_3DOD_POV
				printPointMap(imageWidth, imageHeight, pointMap);
				printDepthMap(imageWidth, imageHeight, depthMap);
				printvi(vi);
				#endif

			}
			generatePixmapFromDepthMapOrDepthContrastMap(depthContrastMapFileName, imageWidth, imageHeight, depthContrastMap);
			generatePixmapFromBooleanMap(depthContrastBooleanMapFileName, imageWidth, imageHeight, depthContrastBooleanMap);
			generatePixmapFromDepthGradientMap(depthGradientMapFileName, imageWidth, imageHeight, depthGradientMap);
			generatePixmapFromDepthGradientContrastMap(depthGradientContrastMapFileName, imageWidth, imageHeight, depthGradientContrastMap);
			generatePixmapFromBooleanMap(depthGradientContrastBooleanMapFileName, imageWidth, imageHeight, depthGradientContrastBooleanMap);
			generatePixmapFromBooleanMap(luminosityContrastMapMinusDepthContrastMapFileName, imageWidth, imageHeight, luminosityContrastMapMinusDepthContrastMap);
			//generatePixmapFromBooleanMap(depthGradientContrastMapMinusDepthContrastMapFileName, imageWidth, imageHeight, depthGradientContrastMapMinusDepthContrastMap);

			generatePixmapFromPointNormalContrastMap(pointNormalContrastMapFileName, imageWidth, imageHeight, pointNormalContrastMap);
			generatePixmapFromBooleanMap(pointNormalContrastBooleanMapFileName, imageWidth, imageHeight, pointNormalContrastBooleanMap);


		}

		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
		{
			cout << "\t\t\t end: 1a. object data generation (ALTERNATE METHOD - part ii) - raytrace vector graphics file" << endl;
			long time1aALTERNATEODgenerationRaytraceVectorGraphicsFileEnd;
			time1aALTERNATEODgenerationRaytraceVectorGraphicsFileEnd = getTimeAsLong();
			if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
			{
				time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTrain = time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTrain+ time1aALTERNATEODgenerationRaytraceVectorGraphicsFileEnd-time1aALTERNATEODgenerationRaytraceVectorGraphicsFileStart;
				time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTrain++;
			}
			else if(trainOrTest == TEST)
			{
				time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTest = time1aALTERNATEODgenerationRaytraceVectorGraphicsFileTotalTest+ time1aALTERNATEODgenerationRaytraceVectorGraphicsFileEnd-time1aALTERNATEODgenerationRaytraceVectorGraphicsFileStart;
				time1aALTERNATEODgenerationRaytraceVectorGraphicsFileIndexTest++;
			}
			cout << "\t\t\t time1aALTERNATEODgenerationRaytraceVectorGraphicsFile = " << time1aALTERNATEODgenerationRaytraceVectorGraphicsFileEnd-time1aALTERNATEODgenerationRaytraceVectorGraphicsFileStart << endl;
		}
	}

	#ifdef OR_DEBUG
	//TEMPprintReferenceListVertexPositions2DOD(firstReferenceInInterpolatedMesh);
	#endif

	delete normalMap;
	delete luminosityMap;
	delete luminosityBooleanMap;
	delete luminosityContrastMap;
	delete luminosityContrastBooleanMap;

	delete depthContrastMap;
	delete depthContrastBooleanMap;
	delete depthGradientMap;
	delete depthGradientContrastMap;
	delete depthGradientContrastBooleanMap;
	delete luminosityContrastMapMinusDepthContrastMap;

	return result;
}




bool createOrAddToInterpolatedMeshReferenceListUsingPointAndRGBMap(double* pointMap, unsigned char* rgbMap, LDreference* firstReferenceInInterpolatedMesh, RTviewInfo* vi, string objectName, int trainOrTest, int dimension, int viewIndex)
{
	bool result = true;

	long time1bODgenerationCreateInterpolatedMeshStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t\t\t start: 1b. object data generation - create interpolated mesh" << endl;
		time1bODgenerationCreateInterpolatedMeshStart = getTimeAsLong();
	}

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	int imageWidth = vi->imageWidth;
	int imageHeight = vi->imageHeight;


	double* pointMapInterpolated = new double[(imageWidth+1)*(imageHeight+1)*VEC_MAP_VEC_NUM_DIMENSIONS];	//NOT USED FOR 2D!

	//set reference to last reference in list [so as to add references onto reference list];
	LDreference* currentReferenceInList = firstReferenceInInterpolatedMesh;
	while(currentReferenceInList->next != NULL)
	{
		//cout << "currentReferenceInList->name = " << currentReferenceInList->name << endl;
		currentReferenceInList = currentReferenceInList->next;
	}
	LDreference* firstNewReferenceInList = currentReferenceInList;


	if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		createInterpolatedPointMap(imageWidth, imageHeight, pointMap, pointMapInterpolated);
		createInterpolated3DmeshReferenceListUsingPointMap(imageWidth, imageHeight, pointMap, pointMapInterpolated, rgbMap, firstNewReferenceInList);

	}
	else if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		createInterpolated2DmeshReferenceListUsingRGBmap2DOD(imageWidth, imageHeight, rgbMap, firstNewReferenceInList);
	}
	else
	{
		cout << "Error: illegal number of dimensions" << endl;
		exit(0);
	}

	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t\t\t end: 1b. object data generation - create interpolated mesh" << endl;
		long time1bODgenerationCreateInterpolatedMeshEnd;
		time1bODgenerationCreateInterpolatedMeshEnd = getTimeAsLong();
		if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
		{
			time1bODgenerationCreateInterpolatedMeshTotalTrain = time1bODgenerationCreateInterpolatedMeshTotalTrain+ time1bODgenerationCreateInterpolatedMeshEnd-time1bODgenerationCreateInterpolatedMeshStart;
			time1bODgenerationCreateInterpolatedMeshIndexTrain++;
		}
		else if(trainOrTest == TEST)
		{
			time1bODgenerationCreateInterpolatedMeshTotalTest = time1bODgenerationCreateInterpolatedMeshTotalTest+ time1bODgenerationCreateInterpolatedMeshEnd-time1bODgenerationCreateInterpolatedMeshStart;
			time1bODgenerationCreateInterpolatedMeshIndexTest++;
		}
		cout << "\t\t\t time1bODgenerationCreateInterpolatedMesh = " << time1bODgenerationCreateInterpolatedMeshEnd-time1bODgenerationCreateInterpolatedMeshStart << endl;

	}


	#ifndef OR_OPTIMISATION_DO_NOT_SAVE_REFERENCE_LIST_TO_FILE
		//not used;
		//DepthMap(imageWidth, imageHeight, depthMap, depthMapInterpolated, rgbMap, firstReferenceInInterpolatedMesh, &vi);

	string interpolatedMeshFileName = objectName + "interpolatedMesh";
	string interpolatedMeshFileNameLDR = interpolatedMeshFileName + trainOrTestString + SCENE_FILE_NAME_EXTENSION;
	char* charstarinterpolatedMeshFileNameLDR = const_cast<char*>(interpolatedMeshFileNameLDR.c_str());


	//add map to reference list
	write2DreferenceListCollapsedTo1DtoFile(charstarinterpolatedMeshFileNameLDR, firstReferenceInInterpolatedMesh);
	#endif


#ifdef OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW
	printInterpolatedMeshReferenceList(firstReferenceInInterpolatedMesh, vi, objectName, trainOrTest, dimension, viewIndex);
#endif

	delete pointMapInterpolated;

	return result;
}


static int interpolatedMeshPointSelection[4][4];
static int interpolatedMeshPointUsageSelection[4][2];


#define MIN_NUMBER_OF_POINTS_TO_CREATE_INTERPOLATED_POINT (2)
LDreference* convertMeshPointToReferences3DOD(ORmeshPoint* currentMeshPointInMeshList, LDreference* firstNewReferenceInInterpolatedMesh)
{
	LDreference* currentReferenceInInterpolatedMesh = firstNewReferenceInInterpolatedMesh;

	vec nullPointVector;
	nullPointVector.x = 0.0;
	nullPointVector.y = 0.0;
	nullPointVector.z = 0.0;

	if(!compareVectors(&(currentMeshPointInMeshList->point), &nullPointVector))
	{
		vec interpolatedMeshPointArray[4];
		bool interpolatedMeshPointArrayFilled[4];
		for(int i=0; i<4; i++)
		{
			interpolatedMeshPointArray[i].x = 0.0;
			interpolatedMeshPointArray[i].y = 0.0;
			interpolatedMeshPointArray[i].z = 0.0;
			interpolatedMeshPointArrayFilled[i] = false;
		}

		bool ok = true;
		//calculate interpolated mesh points
		for(int i=0; i<4; i++)
		{
			int jCounts = 0;
			for(int j=0; j<4; j++)
			{
				int q = interpolatedMeshPointSelection[i][j];
				if(currentMeshPointInMeshList->adjacentMeshPointFilled[q])
				{
					if(!compareVectors(&(currentMeshPointInMeshList->adjacentMeshPoint[q]->point), &nullPointVector))
					{//do not interpolate over null point vectors
						interpolatedMeshPointArray[i].x = interpolatedMeshPointArray[i].x + currentMeshPointInMeshList->adjacentMeshPoint[q]->point.x;
						interpolatedMeshPointArray[i].y = interpolatedMeshPointArray[i].y + currentMeshPointInMeshList->adjacentMeshPoint[q]->point.y;
						interpolatedMeshPointArray[i].z = interpolatedMeshPointArray[i].z + currentMeshPointInMeshList->adjacentMeshPoint[q]->point.z;
						jCounts++;
					}
				}
			}
			if(jCounts >= MIN_NUMBER_OF_POINTS_TO_CREATE_INTERPOLATED_POINT)
			{
				interpolatedMeshPointArray[i].x = (interpolatedMeshPointArray[i].x)/jCounts;
				interpolatedMeshPointArray[i].y = (interpolatedMeshPointArray[i].y)/jCounts;
				interpolatedMeshPointArray[i].z = (interpolatedMeshPointArray[i].z)/jCounts;
				interpolatedMeshPointArrayFilled[i] = true;
			}
		}

		for(int i=0; i<4; i++)
		{
			if(interpolatedMeshPointArrayFilled[interpolatedMeshPointUsageSelection[i][0]] && interpolatedMeshPointArrayFilled[interpolatedMeshPointUsageSelection[i][1]])
			{
				ORpolygon poly;
				multiplyVectorByScalarRT(&(currentMeshPointInMeshList->point), OR_SNAPSHOT_SCALE_FACTOR, &(poly.point1));
				multiplyVectorByScalarRT(&(interpolatedMeshPointArray[interpolatedMeshPointUsageSelection[i][0]]), OR_SNAPSHOT_SCALE_FACTOR, &(poly.point2));
				multiplyVectorByScalarRT(&(interpolatedMeshPointArray[interpolatedMeshPointUsageSelection[i][1]]), OR_SNAPSHOT_SCALE_FACTOR, &(poly.point3));

				currentReferenceInInterpolatedMesh->type = REFERENCE_TYPE_TRI;
				copyVectors(&(currentReferenceInInterpolatedMesh->vertex1relativePosition), &(poly.point1));
				copyVectors(&(currentReferenceInInterpolatedMesh->vertex2relativePosition), &(poly.point2));
				copyVectors(&(currentReferenceInInterpolatedMesh->vertex3relativePosition), &(poly.point3));
				copyVectors(&(currentReferenceInInterpolatedMesh->vertex1absolutePosition), &(poly.point1));
				copyVectors(&(currentReferenceInInterpolatedMesh->vertex2absolutePosition), &(poly.point2));
				copyVectors(&(currentReferenceInInterpolatedMesh->vertex3absolutePosition), &(poly.point3));
				colour col;
				col.r = (currentMeshPointInMeshList->col.r);
				col.g = (currentMeshPointInMeshList->col.g);
				col.b = (currentMeshPointInMeshList->col.b);

				unsigned int colByte1 = (unsigned int)DAT_FILE_FIRST_RGB_COLOUR << (unsigned int)24;
				unsigned int colByte2 = (unsigned int)col.r << (unsigned int)16;
				unsigned int colByte3 = (unsigned int)col.g << (unsigned int)8;
				unsigned int colByte4 = (unsigned int)col.b;

				currentReferenceInInterpolatedMesh->colour = colByte1 | colByte2 | colByte3 | colByte4;
				currentReferenceInInterpolatedMesh->absoluteColour = colByte1 | colByte2 | colByte3 | colByte4;

				#ifdef OR_DEBUG
				/*
				cout << "col.r  = " << col.r  << endl;
				cout << "col.g  = " << col.g  << endl;
				cout << "col.b  = " << col.b  << endl;
				cout << "colByte1 = " << colByte1 << endl;
				cout << "colByte2 = " << colByte2 << endl;
				cout << "colByte3 = " << colByte3 << endl;
				cout << "colByte4 = " << colByte4 << endl;
				cout << "colByte1 | colByte2 | colByte3 | colByte4 = " << (colByte1 | colByte2 | colByte3 | colByte4) << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex1relativePosition.x = " << currentReferenceInInterpolatedMesh->vertex1relativePosition.x << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex1relativePosition.y = " << currentReferenceInInterpolatedMesh->vertex1relativePosition.y << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex1relativePosition.z = " << currentReferenceInInterpolatedMesh->vertex1relativePosition.z << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex2relativePosition.x = " << currentReferenceInInterpolatedMesh->vertex2relativePosition.x << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex2relativePosition.y = " << currentReferenceInInterpolatedMesh->vertex2relativePosition.y << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex2relativePosition.z = " << currentReferenceInInterpolatedMesh->vertex2relativePosition.z << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex3relativePosition.x = " << currentReferenceInInterpolatedMesh->vertex3relativePosition.x << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex3relativePosition.y = " << currentReferenceInInterpolatedMesh->vertex3relativePosition.y << endl;
				cout << "currentReferenceInInterpolatedMesh->vertex3relativePosition.z = " << currentReferenceInInterpolatedMesh->vertex3relativePosition.z << endl;
				*/
				#endif

				LDreference* newReference = new LDreference();
				currentReferenceInInterpolatedMesh->next = newReference;
				currentReferenceInInterpolatedMesh = currentReferenceInInterpolatedMesh->next;
			}
		}
	}

	return currentReferenceInInterpolatedMesh;
}


LDreference* convertMeshPointToReferences2DOD(ORmeshPoint* currentMeshPointInMeshList, LDreference* firstNewReferenceInInterpolatedMesh)
{
	LDreference* currentReferenceInInterpolatedMesh = firstNewReferenceInInterpolatedMesh;

	vec v1;
	vec v2;
	vec v3;
	vec v4;

	int x = currentMeshPointInMeshList->xInt;
	int y = currentMeshPointInMeshList->yInt;

	/*future - with combined 2Dmesh's;
	int x = currentMeshPointInMeshList->point.x;
	int y = currentMeshPointInMeshList->point.y;
	*/

	v1.x = x+0.0;
	v1.y = y+0.0;
	v1.z = 0.0;
	v2.x = x+1.0;
	v2.y = y+0.0;
	v2.z = 0.0;
	v3.x = x+1.0;
	v3.y = y+1.0;
	v3.z = 0.0;
	v4.x = x+0.0;
	v4.y = y+1.0;
	v4.z = 0.0;

	currentReferenceInInterpolatedMesh->type = REFERENCE_TYPE_QUAD;
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex1relativePosition), &v1);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex2relativePosition), &v2);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex3relativePosition), &v3);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex4relativePosition), &v4);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex1absolutePosition), &v1);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex2absolutePosition), &v2);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex3absolutePosition), &v3);
	copyVectors(&(currentReferenceInInterpolatedMesh->vertex4absolutePosition), &v4);
	colour col;
	col.r = (currentMeshPointInMeshList->col.r);
	col.g = (currentMeshPointInMeshList->col.g);
	col.b = (currentMeshPointInMeshList->col.b);

	unsigned int colByte1 = (unsigned int)DAT_FILE_FIRST_RGB_COLOUR << (unsigned int)24;
	unsigned int colByte2 = (unsigned int)col.r << (unsigned int)16;
	unsigned int colByte3 = (unsigned int)col.g << (unsigned int)8;
	unsigned int colByte4 = (unsigned int)col.b;

	currentReferenceInInterpolatedMesh->colour = colByte1 | colByte2 | colByte3 | colByte4;
	currentReferenceInInterpolatedMesh->absoluteColour = colByte1 | colByte2 | colByte3 | colByte4;

	#ifdef OR_DEBUG
	/*
	cout << "col.r  = " << col.r  << endl;
	cout << "col.g  = " << col.g  << endl;
	cout << "col.b  = " << col.b  << endl;
	cout << "colByte1 = " << colByte1 << endl;
	cout << "colByte2 = " << colByte2 << endl;
	cout << "colByte3 = " << colByte3 << endl;
	cout << "colByte4 = " << colByte4 << endl;
	cout << "colByte1 | colByte2 | colByte3 | colByte4 = " << (colByte1 | colByte2 | colByte3 | colByte4) << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex1relativePosition.x = " << currentReferenceInInterpolatedMesh->vertex1relativePosition.x << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex1relativePosition.y = " << currentReferenceInInterpolatedMesh->vertex1relativePosition.y << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex1relativePosition.z = " << currentReferenceInInterpolatedMesh->vertex1relativePosition.z << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex2relativePosition.x = " << currentReferenceInInterpolatedMesh->vertex2relativePosition.x << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex2relativePosition.y = " << currentReferenceInInterpolatedMesh->vertex2relativePosition.y << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex2relativePosition.z = " << currentReferenceInInterpolatedMesh->vertex2relativePosition.z << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex3relativePosition.x = " << currentReferenceInInterpolatedMesh->vertex3relativePosition.x << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex3relativePosition.y = " << currentReferenceInInterpolatedMesh->vertex3relativePosition.y << endl;
	cout << "currentReferenceInInterpolatedMesh->vertex3relativePosition.z = " << currentReferenceInInterpolatedMesh->vertex3relativePosition.z << endl;
	*/
	#endif

	LDreference* newReference = new LDreference();
	currentReferenceInInterpolatedMesh->next = newReference;
	currentReferenceInInterpolatedMesh = currentReferenceInInterpolatedMesh->next;

	return currentReferenceInInterpolatedMesh;
}





bool createInterpolatedMeshReferenceListUsingMeshList(ORmeshPoint* firstMeshPointInMeshList, LDreference* firstReferenceInInterpolatedMesh, RTviewInfo* vi, string objectName, int trainOrTest, int dimension)
{
	bool result = true;

	interpolatedMeshPointSelection[0][0] = 0;
	interpolatedMeshPointSelection[0][1] = 1;
	interpolatedMeshPointSelection[0][2] = 3;
	interpolatedMeshPointSelection[0][3] = 4;
	interpolatedMeshPointSelection[1][0] = 1;
	interpolatedMeshPointSelection[1][1] = 2;
	interpolatedMeshPointSelection[1][2] = 4;
	interpolatedMeshPointSelection[1][3] = 5;
	interpolatedMeshPointSelection[2][0] = 3;
	interpolatedMeshPointSelection[2][1] = 4;
	interpolatedMeshPointSelection[2][2] = 6;
	interpolatedMeshPointSelection[2][3] = 7;
	interpolatedMeshPointSelection[3][0] = 4;
	interpolatedMeshPointSelection[3][1] = 5;
	interpolatedMeshPointSelection[3][2] = 7;
	interpolatedMeshPointSelection[3][3] = 8;

	interpolatedMeshPointUsageSelection[0][0] = 0;
	interpolatedMeshPointUsageSelection[0][1] = 1;
	interpolatedMeshPointUsageSelection[1][0] = 0;
	interpolatedMeshPointUsageSelection[1][1] = 2;
	interpolatedMeshPointUsageSelection[2][0] = 1;
	interpolatedMeshPointUsageSelection[2][1] = 3;
	interpolatedMeshPointUsageSelection[3][0] = 2;
	interpolatedMeshPointUsageSelection[3][1] = 3;

	long time1bODgenerationCreateInterpolatedMeshStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t\t\t start: 1b. object data generation - create interpolated mesh" << endl;
		time1bODgenerationCreateInterpolatedMeshStart = getTimeAsLong();
	}

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	int imageWidth = vi->imageWidth;
	int imageHeight = vi->imageHeight;

	ORmeshPoint* currentMeshPointInMeshList = firstMeshPointInMeshList;
	LDreference* currentReferenceInInterpolatedMesh = firstReferenceInInterpolatedMesh;

	while(currentMeshPointInMeshList->next != NULL)
	{
		if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{
			currentReferenceInInterpolatedMesh = convertMeshPointToReferences3DOD(currentMeshPointInMeshList, currentReferenceInInterpolatedMesh);
		}
		else
		{
			currentReferenceInInterpolatedMesh = convertMeshPointToReferences2DOD(currentMeshPointInMeshList, currentReferenceInInterpolatedMesh);
		}
		currentMeshPointInMeshList = currentMeshPointInMeshList->next;
	}

	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t\t\t end: 1b. object data generation - create interpolated mesh" << endl;
		long time1bODgenerationCreateInterpolatedMeshEnd;
		time1bODgenerationCreateInterpolatedMeshEnd = getTimeAsLong();
		if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
		{
			time1bODgenerationCreateInterpolatedMeshTotalTrain = time1bODgenerationCreateInterpolatedMeshTotalTrain+ time1bODgenerationCreateInterpolatedMeshEnd-time1bODgenerationCreateInterpolatedMeshStart;
			time1bODgenerationCreateInterpolatedMeshIndexTrain++;
		}
		else if(trainOrTest == TEST)
		{
			time1bODgenerationCreateInterpolatedMeshTotalTest = time1bODgenerationCreateInterpolatedMeshTotalTest+ time1bODgenerationCreateInterpolatedMeshEnd-time1bODgenerationCreateInterpolatedMeshStart;
			time1bODgenerationCreateInterpolatedMeshIndexTest++;
		}
		cout << "\t\t\t time1bODgenerationCreateInterpolatedMesh = " << time1bODgenerationCreateInterpolatedMeshEnd-time1bODgenerationCreateInterpolatedMeshStart << endl;
	}


	#ifndef OR_OPTIMISATION_DO_NOT_SAVE_REFERENCE_LIST_TO_FILE
		//not used;
		//DepthMap(imageWidth, imageHeight, depthMap, depthMapInterpolated, rgbMap, firstReferenceInInterpolatedMesh, &vi);

	string interpolatedMeshFileName = objectName + "interpolatedMesh";
	string interpolatedMeshFileNameLDR = interpolatedMeshFileName + trainOrTestString + SCENE_FILE_NAME_EXTENSION;
	char* charstarinterpolatedMeshFileNameLDR = const_cast<char*>(interpolatedMeshFileNameLDR.c_str());


	//add map to reference list
	write2DreferenceListCollapsedTo1DtoFile(charstarinterpolatedMeshFileNameLDR, firstReferenceInInterpolatedMesh);
	#endif


#ifdef OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW
	printInterpolatedMeshReferenceList(firstReferenceInInterpolatedMesh, vi, objectName, trainOrTest, dimension, 0);
#endif

	return result;
}



void printInterpolatedMeshReferenceList(LDreference* firstReferenceInInterpolatedMesh, RTviewInfo* vi, string objectName, int trainOrTest, int dimension, int viewIndex)
{
	//ray trace interpolated map at new eye position;

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	char viewIndexString[10];
	sprintf(viewIndexString, "%d",  viewIndex);

	string mapFileName = objectName + "initialViewMap" + "ViewIndex" + viewIndexString + "ZoomIndex" + "0";


	string TEMPinterpolatedRGBMapFileNameForRayTracing = mapFileName + "Mesh";

	string TEMPinterpolatedMapFileNameForRayTracingTAL = TEMPinterpolatedRGBMapFileNameForRayTracing + trainOrTestString + TAL_EXTENSION;
	string TEMPinterpolatedRGBMapFileNameForRayTracingPPMRGB = TEMPinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
	string TEMPinterpolatedRGBMapFileNameForRayTracingPPMLuminosity = TEMPinterpolatedRGBMapFileNameForRayTracing + LUMINOSITY_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
	string TEMPinterpolatedRGBMapFileNameForRayTracingPPMContrast = TEMPinterpolatedRGBMapFileNameForRayTracing + LUMINOSITY_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;


	char* TEMPcharstarinterpolatedMapFileNameForRayTracingTAL = const_cast<char*>(TEMPinterpolatedMapFileNameForRayTracingTAL.c_str());
	char* TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB = const_cast<char*>(TEMPinterpolatedRGBMapFileNameForRayTracingPPMRGB.c_str());
	char* TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMLuminosity = const_cast<char*>(TEMPinterpolatedRGBMapFileNameForRayTracingPPMLuminosity.c_str());
	char* TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMContrast = const_cast<char*>(TEMPinterpolatedRGBMapFileNameForRayTracingPPMContrast.c_str());



		//create Tal file;
	if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		int widthRecord = vi->imageWidth;
		int heightRecord = vi->imageHeight;
		vi->imageWidth = OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_WIDTH;
		vi->imageHeight = OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_HEIGHT;
		vi->focalLength = vi->focalLength/OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_SCALE_FACTOR;

		write2DReferenceListCollapsedTo1DToFileRayTraceFormat(TEMPcharstarinterpolatedMapFileNameForRayTracingTAL, firstReferenceInInterpolatedMesh, true, vi, false, NULL, NULL);

		vi->focalLength = vi->focalLength*OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_SCALE_FACTOR;
		vi->imageWidth = widthRecord;
		vi->imageHeight = heightRecord;

	}
	else if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		RTviewInfo viFacingImage;
		viFacingImage.imageWidth = TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_WIDTH;
		viFacingImage.imageHeight = TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_HEIGHT;
		viFacingImage.viewWidth = TH_OR_FACING_2D_MAP_DEFAULT_VIEWSIZE_WIDTH;
		viFacingImage.viewHeight = TH_OR_FACING_2D_MAP_DEFAULT_VIEWSIZE_HEIGHT;
		viFacingImage.focalLength = TH_OR_FACING_2D_MAP_DEFAULT_FOCAL;
		viFacingImage.eye.x = TH_OR_FACING_2D_MAP_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		viFacingImage.eye.y = TH_OR_FACING_2D_MAP_DEFAULT_EYE_Y;
		viFacingImage.eye.z = TH_OR_FACING_2D_MAP_DEFAULT_EYE_Z;
		viFacingImage.viewAt.x = TH_OR_FACING_2D_MAP_DEFAULT_VIEWAT_X;
		viFacingImage.viewAt.y = TH_OR_FACING_2D_MAP_DEFAULT_VIEWAT_Y;
		viFacingImage.viewAt.z = TH_OR_FACING_2D_MAP_DEFAULT_VIEWAT_Z;
		viFacingImage.viewUp.x = TH_OR_FACING_2D_MAP_DEFAULT_VIEWUP_X;
		viFacingImage.viewUp.y = TH_OR_FACING_2D_MAP_DEFAULT_VIEWUP_Y;
		viFacingImage.viewUp.z = TH_OR_FACING_2D_MAP_DEFAULT_VIEWUP_Z;

		write2DReferenceListCollapsedTo1DToFileRayTraceFormat(TEMPcharstarinterpolatedMapFileNameForRayTracingTAL, firstReferenceInInterpolatedMesh, true, &viFacingImage, false, NULL, NULL);
	}
	else
	{
		cout << "Error: illegal number of dimensions" << endl;
		exit(0);
	}

	//raytrace Tal file;
	setLightingMode(LIGHTING_MODE_BASIC);
	#ifdef OR_DEBUG
	//cout << "TEMPinterpolatedRGBMapFileNameForRayTracingPPMRGB = " << TEMPinterpolatedRGBMapFileNameForRayTracingPPMRGB << endl;
	//cout << "TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB = " << TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB << endl;
	#endif

#ifndef DO_NOT_DEMONSTRATE_CPP_STRING_TO_CSTRING_CAST_CORRUPTION
	rayTraceScene(TEMPcharstarinterpolatedMapFileNameForRayTracingTAL, TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB, true, false, NULL, NULL, NULL, NULL);
	cout << "TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB = " << TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB << endl;

#else
	rayTraceScene(TEMPcharstarinterpolatedMapFileNameForRayTracingTAL, "TEMPinterpolatedRGBMap.rgb.ppm", true, false, NULL, NULL, NULL, NULL);
	cout << "TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGB = " << "TEMPinterpolatedRGBMap.rgb.ppm" << endl;

#endif

	#ifdef OR_DEBUG
	/*
	//generate additional pixmaps
	createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMap, luminosityMap);
	createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);

	generatePixmapFromRGBmap(TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMRGBRGB, imageWidth, imageHeight, rgbMap);
	generatePixmapFromLuminosityMap(TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMLuminosity, imageWidth, imageHeight, luminosityMap);
	generatePixmapFromLuminosityContrastMap(TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMContrast, imageWidth, imageHeight, luminosityContrastMap);

	cout << "TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMLuminosity = " << TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMLuminosity << endl;
	cout << "TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMContrast = " << TEMPcharstarinterpolatedRGBMapFileNameForRayTracingPPMContrast << endl;
	*/
	#endif
}


bool createOrAddPointsToFeaturesList(double* pointMap, unsigned char* rgbMap, double* depthMap, ORfeature firstFeatureInList[], RTviewInfo* vi, int trainOrTest, int viewIndex, string objectName, int dimension, int numberOfZoomIndicies, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], bool useEdgeZeroCrossingMap)
{
	bool result = true;

	long time1cODgenerationFeatureListCreationStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t\t\t start: 1c. object data generation - feature list creation" << endl;
		time1cODgenerationFeatureListCreationStart = getTimeAsLong();
	}

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	int imageWidth = vi->imageWidth;
	int imageHeight = vi->imageHeight;

	char viewIndexString[10];
	sprintf(viewIndexString, "%d",  viewIndex);

	for(int zoomIndex=0; zoomIndex < numberOfZoomIndicies; zoomIndex++)
	{
		char zoomIndexString[10];
		sprintf(zoomIndexString, "%d", zoomIndex);

		#ifdef OR_DEBUG
		/*
		cout << "objectName = " << objectName << endl;
		cout << "viewIndexString = " << viewIndexString << endl;
		cout << "zoomIndexString = " << zoomIndexString << endl;
		cout << "viewIndex = " << viewIndex << endl;
		cout << "zoomIndex = " << zoomIndex << endl;
		*/
		#endif

		string mapFileNameWithZoom = "" + objectName + "initialViewMap" + "ViewIndex" + viewIndexString + "ZoomIndex" + zoomIndexString;

		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "mapFileNameWithZoom = " << mapFileNameWithZoom << endl;
		}

		int zoom = int(pow(2.0, double(zoomIndex)));		//1, 2, 4, 8, 16 etc

		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "zoomIndex = " << zoomIndex << endl;
			cout << "zoom = " << zoom << endl;
		}

		int resampledWidth = (imageWidth/zoom);
		int resampledHeight = (imageHeight/zoom);

		if(dimension == OR_METHOD3DOD_DIMENSIONS)
		{

	#ifndef OR_METHOD_ASSUME_TRAIN_AND_TEST_WITH_MULTI_VIEWPOINT_ADVANCED_VERSION
	//#ifndef OR_DEBUG_METHOD_ASSUME_TRAIN_AND_TEST_WITH_SINGLE_VIEWPOINT

		//add to cornerlist
		#ifdef OR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION	//not used anymore
			if(!generateFeatureList3DOD(vi, depthMap, pointMap, depthContrastBooleanMap, luminosityContrastBooleanMap, luminosityContrastMapMinusDepthContrastMap, firstFeatureInList[zoomIndex], trainOrTest))
			{
				result = false;
			}
		#else

		//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
			//#ifdef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
			double* pointNormalMap = new double[imageWidth*imageHeight*VECTOR_NUM_VALUES];
			double* pointNormalContrastMap = new double[imageWidth*imageHeight];
			//must use contrast maps with heitger here - check these work - ie points are detected by heitger on lines of contrast
			createPointNormalMapFromPointMap(imageWidth, imageHeight, pointMap, pointNormalMap);
			createPointNormalContrastMapFromPointNormalMap(imageWidth, imageHeight, pointNormalMap, pointNormalContrastMap);
			unsigned char* pointNormalContrastMapConvertedToRgbMap = new unsigned char[imageWidth*imageHeight*RGB_NUM];
			generateRGBmapFromPointNormalContrastMap(imageWidth, imageHeight, pointNormalContrastMap, pointNormalContrastMapConvertedToRgbMap);
			//#else
			double* depthGradientMap = new double[imageWidth*imageHeight*DEPTH_GRADIENT_MAP_VEC_NUM_DIMENSIONS];	//NOT USED FOR 2D!
			createDepthGradientMapFromDepthMap(imageWidth, imageHeight, depthMap, depthGradientMap);
			unsigned char* depthMapConvertedToRgbMap = new unsigned char[imageWidth*imageHeight*RGB_NUM];
			unsigned char* depthGradientMapConvertedToRgbMap = new unsigned char[imageWidth*imageHeight*RGB_NUM];
			generateRGBMapFromDepthMapOrDepthContrastMap(imageWidth, imageHeight, depthMap, depthMapConvertedToRgbMap);
			generateRGBmapFromDepthGradientMap(imageWidth, imageHeight, depthGradientMap, depthGradientMapConvertedToRgbMap);		//will not work properly in a single special case; ie where there is an equal and opposite depth change in both x and y [because this will result in an equal and opposite r and b change, and the heitger feature detection only works with luminosity, r + g]
			//#endif
		//#endif



		//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
			//#ifndef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
			double* depthContrastMap = new double[imageWidth* imageHeight];
			double* depthGradientContrastMap = new double[imageWidth*imageHeight];
			#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
			bool* depthContrastBooleanMap = new bool[imageWidth*imageHeight];
			bool* foregroundDepthCheckDepthContrastBooleanMap = new bool[imageWidth*imageHeight];
			#endif
			//#endif
		//#else

			double* luminosityMap = new double[imageWidth* imageHeight];
			double* luminosityContrastMap = new double[imageWidth* imageHeight];
			#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
			bool* luminosityContrastBooleanMap = new bool[imageWidth*imageHeight];
			bool* foregroundDepthCheckLuminosityhContrastBooleanMap = new bool[imageWidth*imageHeight];
			#endif
		//#endif

			if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
			{
				if(!OR_METHOD_USE_MESH_LISTS)
				{
					if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
					{
						if(!OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST)
						{
							#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
								createContrastMapFromMapWithForegroundDepthCheck(imageWidth, imageHeight, depthMap, depthMap, depthContrastMap, depthContrastBooleanMap, foregroundDepthCheckDepthContrastBooleanMap, EDGE_DEPTH_CONTRAST_THRESHOLD);
							#else
								createContrastMapFromMap(imageWidth, imageHeight, depthMap, depthContrastMap);
							#endif
							createDepthGradientContrastMapFromDepthGradientMap(imageWidth, imageHeight, depthGradientMap, depthGradientContrastMap);
						}
					}
					else
					{
						createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMap, luminosityMap);
						#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
							createContrastMapFromMapWithForegroundDepthCheck(imageWidth, imageHeight, luminosityMap, depthMap, luminosityContrastMap, luminosityContrastBooleanMap, foregroundDepthCheckLuminosityhContrastBooleanMap, EDGE_LUMINOSITY_CONTRAST_THRESHOLD);
						#else
							createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);
							#ifdef OR_DEBUG
							/*
							for(int y = 0; y < imageHeight; y++)
							{
  								for(int x = 0; x < imageWidth; x++)
								{
									cout << "luminosityMap[y*imageWidth + x] = " << luminosityMap[y*imageWidth + x] << endl;
									cout << "luminosityContrastMap[y*imageWidth + x] = " << luminosityContrastMap[y*imageWidth + x] << endl;
								}
							}
							*/
							#endif
						#endif
					}
				}
			}

			if(zoom == 1)
			{
				//use non contrast maps with heitger
				double sensitivity = 1.0;	//0.5
				if(OR_USE_FIND_CORNER_FEATURES)
				{
					if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
					{
						if(OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST)
						{
							if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, pointNormalContrastMapConvertedToRgbMap, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST))
							{
								result = false;
							}
						}
						else
						{
							if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, depthMapConvertedToRgbMap, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT))
							{
								result = false;
							}
							if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, depthGradientMapConvertedToRgbMap, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST))
							{
								result = false;
							}
						}
					}
					else
					{
						sensitivity = 1.0;
						if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, rgbMap, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT))
						{
							result = false;
						}
					}
				}

				if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
				{
					if(OR_METHOD_USE_MESH_LISTS)
					{
						if(!OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED)
						{
							if(OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
							{
								if(!addCentredFeaturesToFeatureListUsingMeshList(firstFeatureInList, IRRELEVANT, dimension, firstMeshPointInMeshList, IRRELEVANT, useEdgeZeroCrossingMap))
								{
									result = false;
								}
							}
							else
							{
								if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
								{
									if(!addCentredFeaturesToFeatureListUsingMeshList(&(firstFeatureInList[zoomIndex]), EDGE_NORMAL_CONTRAST_THRESHOLD, dimension, firstMeshPointInMeshList, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST, useEdgeZeroCrossingMap))
									{
										result = false;
									}
								}
								else
								{
									if(!addCentredFeaturesToFeatureListUsingMeshList(&(firstFeatureInList[zoomIndex]), EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, firstMeshPointInMeshList, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST, useEdgeZeroCrossingMap))
									{
										result = false;
									}
								}
							}
						}
					}
					else
					{

						if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
						{
							if(OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST)
							{
								if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, pointNormalContrastMap, EDGE_NORMAL_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST))
								{
									result = false;
								}
							}
							else
							{
								/*
								if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, depthContrastMap, EDGE_DEPTH_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST))
								{
									result = false;
								}
								*/
								if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, depthGradientContrastMap, EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST))
								{
									result = false;
								}
							}
						}
						else
						{
							if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, luminosityContrastMap, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST))
							{
								result = false;
							}
						}
					}
				}

			}
			else
			{//now scale the maps before performing feature detection

				unsigned char* rgbMapResampled = new unsigned char[resampledWidth*resampledHeight*RGB_NUM];
				resampleRGBmap(rgbMap, imageWidth, imageHeight, rgbMapResampled, zoom, OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF);

			//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
				//#ifdef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
				unsigned char* pointNormalContrastMapConvertedToRgbMapResampled = new unsigned char[resampledWidth*resampledHeight*RGB_NUM];
				resampleRGBmap(pointNormalContrastMapConvertedToRgbMap, imageWidth, imageHeight, pointNormalContrastMapConvertedToRgbMapResampled, zoom, OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF);
				//#else
				unsigned char* depthMapConvertedToRgbMapResampled = new unsigned char[resampledWidth*resampledHeight*RGB_NUM];
				unsigned char* depthGradientMapConvertedToRgbMapResampled = new unsigned char[resampledWidth*resampledHeight*RGB_NUM];
				resampleRGBmap(depthMapConvertedToRgbMap, imageWidth, imageHeight, depthMapConvertedToRgbMapResampled, zoom, OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF);
				resampleRGBmap(depthGradientMapConvertedToRgbMap, imageWidth, imageHeight, depthGradientMapConvertedToRgbMapResampled, zoom, OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF);
				//#endif
			//#endif


	//NEWLY ENABLED;
	#ifdef OR_METHOD3DOD_IF_NOT_USING_MESH_LISTS_USE_CENTRED_FEATURE_DETECTION_DURING_ZOOM
	//#ifdef OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES
		//#ifndef OR_METHOD_USE_MESH_LISTS
			//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
				//#ifdef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
				double* pointNormalMapResampled = new double[resampledWidth*resampledHeight*VECTOR_NUM_VALUES];
				double* pointNormalContrastMapResampled = new double[resampledWidth*resampledHeight];
				//#else
				double* depthContrastMapResampled = new double[resampledWidth* resampledHeight];
				double* depthGradientContrastMapResampled = new double[resampledWidth*resampledHeight];
				//#endif
			//#else

				double* luminosityMapResampled = new double[resampledWidth* resampledHeight];
				double* luminosityContrastMapResampled = new double[resampledWidth* resampledHeight];
			//#endif
		//#endif
	//#endif
	#endif

				//NEWLY ENABLED;
				#ifdef OR_METHOD3DOD_IF_NOT_USING_MESH_LISTS_USE_CENTRED_FEATURE_DETECTION_DURING_ZOOM
				if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
				{
					if(!OR_METHOD_USE_MESH_LISTS)
					{
						if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
						{
							if(OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST)
							{

								createPointNormalMapFromPointMap(resampledWidth, resampledHeight, pointMapResampled, pointNormalMapResampled);
								createPointNormalContrastMapFromPointNormalMap(resampledWidth, resampledHeight, pointNormalMap, pointNormalContrastMapResampled);
							}
							else
							{
								#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
									createContrastMapFromMapWithForegroundDepthCheck(resampledWidth, resampledHeight, depthMapResampled, depthMapResampled, depthContrastMapResampled);
								#else
									createContrastMapFromMap(imageWidth, imageHeight, depthMapResampled, depthContrastMapResampled);
								#endif
								createDepthGradientContrastMapFromDepthGradientMap(resampledWidth, resampledHeight, depthGradientMapResampled, depthGradientContrastMapResampled);
							}

						}
						else
						{
							createLuminosityMapFromRGBMap(resampledWidth, resampledHeight, rgbMapResampled, luminosityMapResampled);

							#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
								createContrastMapFromMapWithForegroundDepthCheck(resampledWidth, resampledHeight, luminosityMapResampled, depthMapResampled, luminosityContrastMapResampled);
							#else
								createContrastMapFromMap(imageWidth, imageHeight, luminosityMapResampled, luminosityContrastMapResampled);
							#endif
						}
					}
				}
				#endif


				string resampledRGBMapFileName = mapFileNameWithZoom + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//FEATUREMAP_PPM_EXTENSION_PART
				char* resampledRGBAtDesiredzoomCharFileName = const_cast<char*>(resampledRGBMapFileName.c_str());
				generatePixmapFromRGBmap(resampledRGBAtDesiredzoomCharFileName, resampledWidth, resampledHeight, rgbMapResampled);
				if(OR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "generating " << resampledRGBAtDesiredzoomCharFileName << endl;
				}

				double sensitivity = 1.0;	//0.5
				if(OR_USE_FIND_CORNER_FEATURES)
				{
					if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
					{
						if(OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST)
						{
							if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, pointNormalContrastMapConvertedToRgbMapResampled, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST))
							{
								result = false;
							}
						}
						else
						{
							/*
							if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, depthMapConvertedToRgbMapResampled, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT))
							{
								result = false;
							}
							*/
							if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, depthGradientMapConvertedToRgbMapResampled, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST))
							{
								result = false;
							}
						}
					}
					else
					{
						sensitivity = 1.0;
						if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, rgbMapResampled, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT))
						{
							result = false;
						}
					}
				}


				//NEWLY ENABLED;
				#ifdef OR_METHOD3DOD_IF_NOT_USING_MESH_LISTS_USE_CENTRED_FEATURE_DETECTION_DURING_ZOOM
				if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
				{
					if(OR_METHOD_USE_MESH_LISTS)
					{
						//cannot use mesh lists for zoomed centre feature detection
					}
					else
					{
						if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
						{

							if(OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST)
							{
								if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, pointNormalContrastMapResampled, EDGE_NORMAL_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
								{
									result = false;
								}
							}
							else
							{
								/*
								if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, depthContrastMapResampled, EDGE_DEPTH_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
								{
								      result = false;
								}
								*/
								if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, depthGradientContrastMapResampled, EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
								{
									result = false;
								}
							}
						}
						else
						{
							if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, luminosityContrastMapResampled, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
							{
								result = false;
							}
						}
					}
				}
				#endif


				delete rgbMapResampled;
			//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
				//#ifdef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
				delete pointNormalContrastMapConvertedToRgbMapResampled;
				//#else
				delete depthMapConvertedToRgbMapResampled;
				delete depthGradientMapConvertedToRgbMapResampled;
				//#endif
			//#endif


	//NEWLY ENABLED;
	#ifdef OR_METHOD3DOD_IF_NOT_USING_MESH_LISTS_USE_CENTRED_FEATURE_DETECTION_DURING_ZOOM
	//#ifdef OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES
		//#ifndef OR_METHOD_USE_MESH_LISTS
			//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
				//#ifdef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
				delete pointNormalMapResampled;
				delete pointNormalContrastMapResampled;
				//#else
				delete depthContrastMapResampled;
				delete depthGradientContrastMapResampled;
				//#endif
			//#else

				delete luminosityMapResampled;
				delete luminosityContrastMapResampled;
			//#endif
		//#endif
	//#endif
	#endif

			}


		//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
			//#ifdef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
			delete pointNormalMap;
			delete pointNormalContrastMap;
			delete pointNormalContrastMapConvertedToRgbMap;
			//#else
			delete depthGradientMap;
			delete depthMapConvertedToRgbMap;
			delete depthGradientMapConvertedToRgbMap;
			//#endif
		//#endif


	//#ifdef OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION
		//#ifndef OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST
			delete depthContrastMap;
			delete depthGradientContrastMap;
			#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
			delete depthContrastBooleanMap;
			delete foregroundDepthCheckDepthContrastBooleanMap;
			#endif
		//#endif
	//#else
			delete luminosityMap;
			delete luminosityContrastMap;
			#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
			delete luminosityContrastBooleanMap;
			delete foregroundDepthCheckLuminosityhContrastBooleanMap;
			#endif
	//#endif

		#endif
	//#endif
	#endif
		}
		else if(dimension == OR_METHOD2DOD_DIMENSIONS)
		{
			if(zoom == 1)
			{
				//add to cornerlist
				double sensitivity = 1.0;

				if(OR_USE_FIND_CORNER_FEATURES)
				{
					if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, rgbMap, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT))
					{
						result = false;
					}
				}

				if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
				{
					if(OR_METHOD_USE_MESH_LISTS)
					{
						if(!OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED)
						{
							if(!addCentredFeaturesToFeatureListUsingMeshList(&(firstFeatureInList[zoomIndex]), EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, firstMeshPointInMeshList, CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST, useEdgeZeroCrossingMap))
							{
								result = false;
							}
						}
					}
					else
					{
						double* luminosityMap = new double[imageWidth* imageHeight];
						double* luminosityContrastMap = new double[imageWidth* imageHeight];
						createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMap, luminosityMap);
						createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);
						if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), imageWidth, imageHeight, luminosityContrastMap, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST))
						{
							result = false;
						}
						delete luminosityMap;
						delete luminosityContrastMap;

					}
				}
			}
			else
			{

				unsigned char* rgbMapResampled = new unsigned char[resampledWidth*resampledHeight*RGB_NUM];
				resampleRGBmap(rgbMap, imageWidth, imageHeight, rgbMapResampled, zoom, OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF);

				string resampledRGBMapFileName = mapFileNameWithZoom + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
				char* resampledRGBAtDesiredzoomCharFileName = const_cast<char*>(resampledRGBMapFileName.c_str());
				generatePixmapFromRGBmap(resampledRGBAtDesiredzoomCharFileName, resampledWidth, resampledHeight, rgbMapResampled);
				if(OR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "generating " << resampledRGBAtDesiredzoomCharFileName << endl;
				}

				//add to cornerlist
				double sensitivity = 1.0;
				if(OR_USE_FIND_CORNER_FEATURES)
				{
					if(!addCornerFeaturesToFeatureListUsingRGBmap(vi, rgbMapResampled, &(firstFeatureInList[zoomIndex]), trainOrTest, mapFileNameWithZoom, sensitivity, dimension, pointMap, depthMap, zoom, INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT))
					{
						result = false;
					}
				}

				#ifdef OR_METHOD2DOD_IF_NOT_USING_MESH_LISTS_USE_CENTRED_FEATURE_DETECTION_DURING_ZOOM
				if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
				{
					if(!OR_METHOD_USE_MESH_LISTS)
					{
						double* luminosityMapResampled = new double[resampledWidth* resampledHeight];
						double* luminosityContrastMapResampled = new double[resampledWidth* resampledHeight];
						createLuminosityMapFromRGBMap(resampledWidth, resampledHeight, rgbMapResampled, luminosityMapResampled);
						createContrastMapFromMap(resampledWidth, resampledHeight, luminosityMapResampled, luminosityContrastMapResampled);
						if(!addCentredFeaturesToFeatureListUsingContrastMap(&(firstFeatureInList[zoomIndex]), resampledWidth, resampledHeight, luminosityContrastMapResampled, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, dimension, pointMap, depthMap, zoom, vi, useEdgeZeroCrossingMap, INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST))
						{
							result = false;
						}

						delete luminosityMapResampled;
						delete luminosityContrastMapResampled;
					}
				}
				#endif



				delete rgbMapResampled;
			}
		}
		else
		{
			cout << "Error: illegal number of dimensions" << endl;
			exit(0);
		}

		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			ORfeature* currentFeatureInList = &(firstFeatureInList[zoomIndex]);
			int numberOfFeatures = 0;
			while(currentFeatureInList->next != NULL)
			{
				numberOfFeatures++;
				//cout << "currentFeatureInList++" << endl;
				currentFeatureInList=currentFeatureInList->next;
			}
			cout << "numberOfFeatures at zoom " << zoom << " = " << numberOfFeatures << endl;
		}

		bool* featuresMapCompleteOfficial = new bool[resampledWidth*resampledHeight];
		generateBooleanMapFromFeatureList(resampledWidth, resampledHeight, &(firstFeatureInList[zoomIndex]), featuresMapCompleteOfficial, vi, zoom);
		string featuresMapCompleteOfficialFileName = mapFileNameWithZoom + FEATURESMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
		generatePixmapFromBooleanMap(featuresMapCompleteOfficialFileName, resampledWidth, resampledHeight, featuresMapCompleteOfficial);
		delete featuresMapCompleteOfficial;
	}

	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		cout << "\t\t\t end: 1c. object data generation - feature list creation" << endl;
		long time1cODgenerationFeatureListCreationEnd;
		time1cODgenerationFeatureListCreationEnd = getTimeAsLong();
		if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
		{
			time1cODgenerationFeatureListCreationTotalTrain = time1cODgenerationFeatureListCreationTotalTrain+ time1cODgenerationFeatureListCreationEnd-time1cODgenerationFeatureListCreationStart;
			time1cODgenerationFeatureListCreationIndexTrain++;
		}
		else if(trainOrTest == TEST)
		{
			time1cODgenerationFeatureListCreationTotalTest = time1cODgenerationFeatureListCreationTotalTest+ time1cODgenerationFeatureListCreationEnd-time1cODgenerationFeatureListCreationStart;
			time1cODgenerationFeatureListCreationIndexTest++;
		}
		cout << "\t\t\t time1cODgenerationFeatureListCreation = " << time1cODgenerationFeatureListCreationEnd-time1cODgenerationFeatureListCreationStart << endl;
	}

	return result;

}


#ifdef OR_METHOD_GEOMETRIC_COMPARISON

bool checkIfFeatureContainerWithSameFeatureIndiciesExists(ORfeatureContainer* firstFeatureContainerInBestMatchList, ORfeatureContainer* currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList)
{
	bool exists = false;

	ORfeatureContainer* currentFeatureContainerInBestMatchList = firstFeatureContainerInBestMatchList;

	while(currentFeatureContainerInBestMatchList->next != NULL)
	{
		if((currentFeatureContainerInBestMatchList->firstFeatureInFeatureList->objectName == currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList->firstFeatureInFeatureList->objectName) &&
		(currentFeatureContainerInBestMatchList->firstFeatureInFeatureList->viewIndex == currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList->firstFeatureInFeatureList->viewIndex) &&
		(currentFeatureContainerInBestMatchList->firstFeatureInFeatureList->polyIndex == currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList->firstFeatureInFeatureList->polyIndex) &&
		(currentFeatureContainerInBestMatchList->firstFeatureInFeatureList->sideIndex == currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList->firstFeatureInFeatureList->sideIndex))
		{
			exists = true;
		}

		currentFeatureContainerInBestMatchList = currentFeatureContainerInBestMatchList->next;
	}

	return exists;

}

#endif


bool addCornerFeaturesToFeatureListUsingRGBmap(RTviewInfo* vi, unsigned char* rgbMap, ORfeature* firstFeatureInList, int trainOrTest, string mapFileName, double sensitivity, int dimension, double* pointMap, double* depthMap, int zoom, bool interpixelRGBmapType)
{
	char currentTempFolder[EXE_FOLDER_PATH_MAX_LENGTH];
	getCurrentDirectory(currentTempFolder);

	bool result = true;

	//added by RBB 3 Oct 09
	ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}
	ORfeature* firstNewFeatureInList = currentFeatureInList;


	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	int imageWidth = (vi->imageWidth) / zoom;
	int imageHeight = (vi->imageHeight) / zoom;

	string rgbMapFileNameRas = "test-rgbinput.ras";
	string featureMapFileNameRas = "test-kptmap-newendian.ras";
	string featureMapFileNameRasOldEndian = "test-kptmap.ras";
	string featureMapFileNameAscii = "test-kpts.ascii";
	string baseName = "test";

#ifdef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "Using Heitger feature detection code: preconditions" << endl;
		cout << "\t FD.exe is installed (\"heitger feature detection with RBB Endian Mod v0.5b\")" << endl;
		cout << "\t\t [Olof Henricsson 96-06-07 release of Friedrich Heitger's SE with a) ras format endian swap, b) ras format relaxed, c) multiple endian outputs for Keypoints exe, d) Windows VS compatibility, e) change of binary name to FD.exe]" << endl;
		cout << "\t imagemagik is installed" << endl;
	}

	string rgbMapFileName = mapFileName + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
	string featureMapFileName = mapFileName + FEATUREMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
	unsigned char* featureRgbMap = new unsigned char[imageWidth*imageHeight*RGB_NUM];


	#ifdef TEMP_TEST_HEITGER_FEATURE_THRESHOLD_IMAGE_SIZE_DEPENDENCE

		string featureMapFileNameTEMP = mapFileName + FEATUREMAP_PPM_EXTENSION_PART + trainOrTestString + "TEMP" + PPM_EXTENSION;
		char* charstarfeatureMapTEMPFileName = const_cast<char*>(featureMapFileNameTEMP.c_str());
		unsigned char* featureRgbMapTEMP = new unsigned char[1600*800*RGB_NUM];

		string rgbMapFileNameTEMP = mapFileName + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + "TEMP" + PPM_EXTENSION;
		char* charstarRgbMapTEMPFileName = const_cast<char*>(rgbMapFileNameTEMP.c_str());
		unsigned char* rgbMapTEMP = new unsigned char[1600*800*RGB_NUM];

		colour black;
		black.r = 0;
		black.g = 0;
		black.b = 0;
		for(int x=0;x<1600;x++)
		{
			for(int y=0;y<800;y++)
			{
				setRGBMapValues(x, y, 1600, &black, rgbMapTEMP);
			}
		}

		for(int x=0;x<imageWidth;x++)
		{
			for(int y=0;y<imageHeight;y++)
			{
				colour temp;
				getRGBMapValues(x, y, imageWidth, rgbMap, &temp);
				setRGBMapValues(x, y, 1600, &temp, rgbMapTEMP);
			}
		}

		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "generatePixmapFromRGBmap(" << rgbMapFileNameTEMP << ", imageWidth, imageHeight, rgbMapTEMP);" << endl;
		}
		generatePixmapFromRGBmap(charstarRgbMapTEMPFileName, 1600, 800, rgbMapTEMP);
		string convertRGBPPMtoRGBRASCommand = "convert " + rgbMapFileNameTEMP + " " + rgbMapFileNameRas;
	#else
	string convertRGBPPMtoRGBRASCommand = "convert " + rgbMapFileName + " " + rgbMapFileNameRas;
	#endif

	string convertRGBRAStoFeatureRASCommand = "";
#ifdef LINUX
	convertRGBRAStoFeatureRASCommand = convertRGBRAStoFeatureRASCommand + exeFolderCharStar + "/FD.exe" + " -workingfolder " + currentTempFolder + " -i " + rgbMapFileNameRas + " -o " + baseName + " -keypoints -quality -sigma 2.25 -raster";
#else
	convertRGBRAStoFeatureRASCommand = convertRGBRAStoFeatureRASCommand + exeFolderCharStar + "/FD.exe" + " -workingfolder " + currentTempFolder + " -i " + rgbMapFileNameRas + " -o " + baseName + " -keypoints -quality -sigma 2.25 -raster";
#endif
	string convertFeatureRAStoFeaturePPMCommand = "convert -depth 8 " + featureMapFileNameRas + " " + featureMapFileName;

	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "system(" << convertRGBPPMtoRGBRASCommand << ");" << endl;
	}
	system(convertRGBPPMtoRGBRASCommand.c_str());

	setCurrentDirectory(exeFolderCharStar);
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "system(" << convertRGBRAStoFeatureRASCommand << ");" << endl;
	}
	system(convertRGBRAStoFeatureRASCommand.c_str());
	setCurrentDirectory(currentTempFolder);

	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "system(" << convertFeatureRAStoFeaturePPMCommand << ");" << endl;
	}
	system(convertFeatureRAStoFeaturePPMCommand.c_str());
	pixmap* featurePixMap;
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "featurePixMapTrain = loadPPM(" << featureMapFileName << ");" << endl;
	}
	featurePixMap = loadPPM(featureMapFileName);

	#ifdef TEMP_TEST_HEITGER_FEATURE_THRESHOLD_IMAGE_SIZE_DEPENDENCE
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "createRGBMapFromPixmapImage(featurePixMap, featureRgbMapTEMP);" << endl;
		}
		createRGBMapFromPixmapImage(featurePixMap, featureRgbMapTEMP);
		freePixmap(featurePixMap);

		for(int x=0;x<imageWidth;x++)
		{
			for(int y=0;y<imageHeight;y++)
			{
				colour temp;
				getRGBMapValues(x, y, 1600, featureRgbMapTEMP, &temp);
				setRGBMapValues(x, y, imageWidth, &temp, featureRgbMap);
			}
		}

	#else
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "createRGBMapFromPixmapImage(featurePixMap, featureRgbMap);" << endl;
		cout << "imageWidth = " << imageWidth << endl;
		cout << "imageHeight = " << imageHeight << endl;
	}
	createRGBMapFromPixmapImage(featurePixMap, featureRgbMap);
	freePixmap(featurePixMap);
	#endif



	#ifdef USE_HEITGER_ASCII_KEYPOINT_GENERATION
		string convertFeatureRAStoFeatureAsciiCommand = "./Keypoints -i " + baseName + " -ascii -thresh 40.0";	//40.0
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "system(" << convertFeatureRAStoFeatureAsciiCommand << ");" << endl;
		}
		system(convertFeatureRAStoFeatureAsciiCommand.c_str());
		generateFeatureListFromHeitgerFeatureAsciiMap(firstNewFeatureInList, featureMapFileNameAscii, zoom, RTviewInfo* vi);

	#else
		#ifdef USE_RBB_QUADRATIC_FIT_KEYPOINT_GENERATION
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit(firstFeatureInList, featureRgbMap, imageWidth, imageHeight, sensitivity, dimension, pointMap, depthMap);" << endl;
			}
			generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit(firstNewFeatureInList, featureRgbMap, imageWidth, imageHeight, sensitivity, dimension, pointMap, depthMap, zoom, vi, interpixelRGBmapType);
		#elif defined USE_RBB_BASIC_KEYPOINT_GENERATION
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "generateFeatureListFromHeitgerFeatureRGBMap(firstFeatureInList, featureRgbMap, imageWidth, imageHeight, rgbMap,  sensitivity, dimension, pointMap, depthMap);" << endl;
			}
			generateFeatureListFromHeitgerFeatureRGBMap(firstNewFeatureInList, featureRgbMap, imageWidth, imageHeight, rgbMap, sensitivity, dimension, pointMap, depthMap, zoom, vi,);
		#else
			cout << "error: some form of keypoint generation must be defined" << end;
			exit(0);
		#endif
	#endif
#else
	//unsigned char* featureRgbMap = new unsigned char[imageWidth*imageHeight*RGB_NUM];
	generateFeatureListFromRGBMap(firstFeatureInList, rgbMap, imageWidth, imageHeight, zoom, trainOrTestString, mapFileName, dimension, pointMap, depthMap, vi);

#endif

	delete featureRgbMap;


	return result;
}




	//this code assumes Test Plan 1; a) no parallax processing and b) all virtual [no real life data]
#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
bool generateNormalisedSnapshotsExperienceListUsingPolyList(LDreference* firstReferenceInInterpolatedMesh, ORpolygon* firstPolygonInList, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrainOrTest, ANNexperience* firstExperienceInList, int* numberOfTrainOrTestPolys, int trainOrTest, int viewIndex, string objectName, int dimension, ORfeature* firstFeatureInList)
#else
bool generateNormalisedSnapshotsUsingPolyList(LDreference* firstReferenceInInterpolatedMesh, ORpolygon firstPolygonInList[], int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrainOrTest, int numberOfTrainOrTestPolys[], int trainOrTest, int viewIndex, string objectName, int dimension, ORfeature* firstFeatureInList, int numberOfZoomIndicies)
#endif
{
	bool generatePixmapFiles = false;
	if(!OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
	{
		generatePixmapFiles = true;
	}
	else
	{
		if(!OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
		{
			if(trainOrTest == TEST)
			{
				generatePixmapFiles = true;
			}
		}
	}


	bool doNotCheckContrast;
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		#ifdef OR_METHOD_2DOD_DO_NOT_REQUIRE_SNAPSHOT_CONTRAST
		doNotCheckContrast = true;
		#else
		doNotCheckContrast = false;
		#endif
	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		#ifdef OR_METHOD_3DOD_DO_NOT_REQUIRE_SNAPSHOT_CONTRAST
		doNotCheckContrast = true;
		#else
		doNotCheckContrast = false;
		#endif
	}
	else
	{
		cout << "error: illegal dimension" << endl;
		exit(0);
	}


	bool result = true;

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	int numSidesPerPolygon = OR_METHOD_POLYGON_NUMBER_OF_SIDES;

	char viewIndexString[10];
	char zoomIndexString[10];
	char polygonIndexString[10];
	char sideIndexString[10];
	#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
	char featurePosNoise1CountString[10];
	char featurePosNoise2CountString[10];
	char featurePosNoise3CountString[10];
	#endif

	int cropWidth;
	int cropHeight;
	int smallImageRatio;
	bool performCrop = false;
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		cropWidth = OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X;
		cropHeight = OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y;
		smallImageRatio = OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO;
		if(OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP)
		{
			performCrop = true;
		}
	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		cropWidth = OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X;
		cropHeight = OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y;
		smallImageRatio = OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO;
		if(OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP)
		{
			performCrop = true;
		}
	}



	int uncroppedWidth = imageWidthFacingPoly+(2*cropWidth);
 	int uncroppedHeight = imageHeightFacingPoly+(2*cropHeight);

	/*
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "cropWidth = " << cropWidth << endl;
		cout << "cropHeight = " << cropHeight << endl;
		cout << "performCrop = " << performCrop << endl;
		cout << "uncroppedWidth = " << uncroppedWidth << endl;
		cout << "uncroppedHeight = " << uncroppedHeight << endl;
	}
	*/


	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
	int imageWidthFacingPolyWithGaussianApplied = imageWidthFacingPoly+(OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
 	int imageHeightFacingPolyWithGaussianApplied = imageHeightFacingPoly+(OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
	int cropWidthWithGaussianApplied = cropWidth - (OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
	int cropHeightWithGaussianApplied = cropHeight - (OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
	#endif

	if(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE < OR_METHOD_2DOD_NORM_SNAPSHOT_X)
	{
		cout << "error: WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE < OR_METHOD_2DOD_NORM_S.._X, please redefine WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE" << endl;
		exit(0);
	}
	if(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE < OR_METHOD_3DOD_NORM_SNAPSHOT_X)
	{
		cout << "error: WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE < OR_METHOD_3DOD_NORM_S.._X, please redefine WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE" << endl;
		exit(0);
	}

	#ifdef USE_OPENGL
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		double normSnapshotZoom = 0.0;
		#ifdef LINUX
		//Note linux supports windows of arbitrary size
		normSnapshotZoom = 1.0;
		#else
		//Note windows enforces a minimum horizontal windows size of 104 pixels
		normSnapshotZoom = OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH;
		#endif

		setViewPort2Dortho(0.0-(OR_METHOD_2DOD_PREDEFINED_T_WIDTH*normSnapshotZoom/2.0), 0.0+(OR_METHOD_2DOD_PREDEFINED_T_WIDTH*normSnapshotZoom/2.0), 0.0-(OR_METHOD_2DOD_PREDEFINED_T_WIDTH*normSnapshotZoom/2.0), 0.0+(OR_METHOD_2DOD_PREDEFINED_T_WIDTH*normSnapshotZoom/2.0));
	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		double normSnapshotZoom = 0.0;
		#ifdef LINUX
		//Note linux supports windows of arbitrary size
		normSnapshotZoom = 1.0;
		#else
		//Note windows enforces a minimum horizontal windows size of 104 pixels
		normSnapshotZoom = OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH;
		#endif
		setViewPort3Dortho(-(OR_METHOD3DOD_SNAPSHOT_DISTANCE)*normSnapshotZoom, (OR_METHOD3DOD_SNAPSHOT_DISTANCE)*normSnapshotZoom, -(OR_METHOD3DOD_SNAPSHOT_DISTANCE)*normSnapshotZoom, (OR_METHOD3DOD_SNAPSHOT_DISTANCE)*normSnapshotZoom, -(OR_METHOD3DOD_SNAPSHOT_DISTANCE*2.0), (OR_METHOD3DOD_SNAPSHOT_DISTANCE*2.0));
	}
	#endif

	unsigned char* rgbMapUncroppedFacingPoly = new unsigned char[uncroppedWidth*uncroppedHeight*RGB_NUM];
 	unsigned char* rgbMapFacingPoly = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
	unsigned char* rgbMapFacingPolyWithGaussianApplied = new unsigned char[(imageWidthFacingPoly+(OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2))*(imageHeightFacingPoly+(OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2))*RGB_NUM];
	unsigned char* rgbMapFacingPolyWithGaussianAppliedCropped = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	#endif

	double* rgbDevIEnormalisedHueContrastMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
//#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
	int smallImageWidth = imageWidthFacingPoly/smallImageRatio;
	int smallImageHeight = imageHeightFacingPoly/smallImageRatio;
 	unsigned char* rgbMapSmallFacingPoly = new unsigned char[smallImageWidth*smallImageHeight*RGB_NUM];
	//#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS
	#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS_ADVANCED_RESAMPLING
	double* depthMapSmallFacingPoly = new double[smallImageWidth*smallImageHeight];
	#else
	unsigned char* tempDepthRGBMapFacingPoly = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
 	unsigned char* depthRGBMapSmallFacingPoly = new unsigned char[smallImageWidth*smallImageHeight];
	#endif
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly = new double[smallImageWidth*smallImageHeight*VEC_MAP_VEC_NUM_DIMENSIONS];
	unsigned char* rgbDev8BitSmallMapFacingPoly = new unsigned char[smallImageWidth*smallImageHeight*RGB_NUM];
	//#endif
//#endif

//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	signed char* dctCoeffArrayY = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* dctCoeffArrayYcr = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* dctCoeffArrayYcb = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* dctCoeffArrayYDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* dctCoeffArrayYCrDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* dctCoeffArrayYCbDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];

//#endif


	double* luminosityMapFacingPoly = new double[imageWidthFacingPoly* imageHeightFacingPoly];
	bool* luminosityBooleanMapFacingPoly = new bool[imageWidthFacingPoly*imageHeightFacingPoly];
	double* luminosityContrastMapFacingPoly = new double[imageWidthFacingPoly* imageHeightFacingPoly];
	bool* luminosityContrastBooleanMapFacingPoly = new bool[imageWidthFacingPoly* imageHeightFacingPoly];
	bool* foregroundDepthCheckLuminosityContrastBooleanMap = new bool[imageWidthFacingPoly* imageHeightFacingPoly];
	double* normalMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];

	double* depthMapUncroppedFacingPoly = new double[uncroppedWidth*uncroppedHeight];
	double* depthMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly];					//NOT USED for 2D!
	double* depthContrastMapFacingPoly = new double[imageWidthFacingPoly* imageHeightFacingPoly];					//NOT USED for 2D!
	bool* depthContrastBooleanMapFacingPoly = new bool[imageWidthFacingPoly*imageHeightFacingPoly];				//NOT USED for 2D!
	bool* foregroundDepthCheckDepthContrastBooleanMap = new bool[imageWidthFacingPoly*imageHeightFacingPoly];				//NOT USED for 2D!
	double* depthGradientMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly*XY_VECTOR_MAP_NUM_DIMENSIONS];	//NOT USED for 2D!
	double* depthGradientContrastMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly];			//NOT USED for 2D!
	bool* depthGradientContrastBooleanMapFacingPoly = new bool[imageWidthFacingPoly*imageHeightFacingPoly];		//NOT USED for 2D!



	double* pointMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];	//NOT USED for 2D!





#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
	ANNexperience* currentExperience = firstExperienceInList;
#endif

	#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
	int numFeaturePos1Values;
	int numFeaturePos2Values;
	int numFeaturePos3Values;
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		setNoiseArraysMethod2DOD();
		numFeaturePos1Values = OR_METHOD3DOD_NUM_ORI_NOISE_VALUES;
		numFeaturePos2Values = OR_METHOD3DOD_NUM_POS_NOISE_VALUES;
		numFeaturePos3Values = 0;
	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		setNoiseArraysMethod3DOD();
		numFeaturePos1Values = OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES;
		numFeaturePos2Values = OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES;
		numFeaturePos3Values = OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES;
	}
	#endif

	for(int zoomIndex=0; zoomIndex < numberOfZoomIndicies; zoomIndex++)
	{
		/*
		char snapshotMapsText[imageWidthFacingPoly*imageHeightFacingPoly*3 + imageWidthFacingPoly*imageHeightFacingPoly*3 + imageWidthFacingPoly*imageHeightFacingPoly + smallImageWidth*smallImageHeight*3 + smallImageWidth*smallImageHeight*3 + smallImageWidth*smallImageHeight];
		*/
			/* for
 			unsigned char* rgbMapFacingPoly = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
			double* rgbDevIEnormalisedHueContrastMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
			double* depthMapFacingPoly = new double[imageWidthFacingPoly*imageHeightFacingPoly];					//NOT USED for 2D!
 			unsigned char* rgbMapSmallFacingPoly = new unsigned char[smallImageWidth*smallImageHeight*RGB_NUM];
 			unsigned char* depthRGBMapSmallFacingPoly = new unsigned char[smallImageWidth*smallImageHeight];
			double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly = new double[smallImageWidth*smallImageHeight*VEC_MAP_VEC_NUM_DIMENSIONS];
			*/

		numberOfTrainOrTestPolys[zoomIndex] = 0;

		int polygonIndex = 0;
		ORpolygon* currentPolygonInList = &(firstPolygonInList[zoomIndex]);
		//for every polygon
			//rotate the camera about the interpolated 3D Map such that the polygon is facing the camera, and at a distance U
		while(currentPolygonInList->next != NULL)
		{
			if(polygonIndex < maxNumberOfPolygonsTrainOrTest)
			{//only add experience to list if less than number of polygons supported by NN

				if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
				{
					cout << "\t\t\t  pIndex = " << polygonIndex << endl;
					cout << "\t\t\t  numberOfTrainOrTestPolys[zoomIndex] = " << numberOfTrainOrTestPolys[zoomIndex] << endl;
				}
				#ifdef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
				int numPolySideSnapshotsPassedContrastChecks = 0;
				#endif
				//for every orientation of the polygon;
				for(int side=0; side < numSidesPerPolygon; side++)
				{
					#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
					cout << "p = " << p << ", s = " << s << endl;
					for(int featurePosNoise1=0; featurePosNoise1 < numFeaturePos1Values; featurePosNoise1++)
					{
						for(int featurePosNoise2=0; featurePosNoise2 < numFeaturePos2Values; featurePosNoise2++)
						{
							for(int featurePosNoise3=0; featurePosNoise3 < numFeaturePos3Values; featurePosNoise3++)
							{
					#endif
								string interpolatedMeshFileName = objectName + "interpolatedMesh";
								string interpolatedMapFileNameForRayTracing;

								sprintf(polygonIndexString, "%d", numberOfTrainOrTestPolys[zoomIndex]);
								sprintf(sideIndexString, "%d", side);
								sprintf(viewIndexString, "%d", viewIndex);
								sprintf(zoomIndexString, "%d", zoomIndex);

								#ifdef OR_DEBUG
								//cout << "zoomIndex = " << zoomIndex << ", numberOfTrainOrTestPolys[zoomIndex] = " << numberOfTrainOrTestPolys[zoomIndex] << ", side = " << side << endl;
								#endif

								#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
								//cout << "\t featurePosNoise1 = " << featurePosNoise1 << ", featurePosNoise2 = " << featurePosNoise2 << ", featurePosNoise3 = " << featurePosNoise3 << endl;

								sprintf(featurePosNoise1CountString, "%d", featurePosNoise1);
								sprintf(featurePosNoise2CountString, "%d", featurePosNoise2);
								sprintf(featurePosNoise3CountString, "%d", featurePosNoise3);

								if((numFeaturePos1Values > 1) || (numFeaturePos2Values > 1) || (numFeaturePos3Values > 1))
								{
									interpolatedMapFileNameForRayTracing = interpolatedMeshFileName + "ViewIndex" + viewIndexString + "ZoomIndex" + zoomIndexString + "FacingPoly" + polygonIndexString + "side" + sideIndexString + "featurePosNoise1" + featurePosNoise1CountString + "featurePosNoise2" + featurePosNoise2CountString + "featurePosNoise3" + featurePosNoise3CountString;
								}
								else
								{
									interpolatedMapFileNameForRayTracing = interpolatedMeshFileName + "ViewIndex" + viewIndexString + "ZoomIndex" + zoomIndexString + "FacingPoly" + polygonIndexString + "side" + sideIndexString;
								}
								#else
								interpolatedMapFileNameForRayTracing = interpolatedMeshFileName + "ViewIndex" + viewIndexString + "ZoomIndex" + zoomIndexString + "FacingPoly" + polygonIndexString + "side" + sideIndexString;
								#endif

								string interpolatedMapFileNameForRayTracingLDR = interpolatedMapFileNameForRayTracing + SCENE_FILE_NAME_EXTENSION;
								string interpolatedMapFileNameForRayTracingTAL = interpolatedMapFileNameForRayTracing + TAL_EXTENSION;
								string rgbMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								#ifdef OR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
								string rgbDevMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + RGB_DEV_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								#endif

								#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
 								string rgbMapFacingPolySmallFileName = interpolatedMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
 								string booleanContrastRgbMapFacingPolySmallFileName = interpolatedMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_BOOLEAN_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								#endif
								#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS
 								string depthMapFacingPolySmallFileName = interpolatedMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + DEPTHMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								#endif
								#ifdef OR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
								string rgbDevMapFacingPolySmallFileName = interpolatedMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_DEV_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								#endif
								#endif

								//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
 								string rgbMapFacingPolySmallFileNameJPEG = interpolatedMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + trainOrTestString + JPEG_EXTENSION;
								//#endif

								string luminosityMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + LUMINOSITY_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								string luminosityBooleanMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + LUMINOSITY_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								string luminosityContrastMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + LUMINOSITY_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								string luminosityContrastBooleanMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + LUMINOSITY_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;
								string transformedFeaturesNearestFileName = interpolatedMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + trainOrTestString + TFD_EXTENSION;
								string transformedFeaturesAllFileName = interpolatedMapFileNameForRayTracing + TRANSFORMED_FEATURES_ALL_EXTENSION_PART + trainOrTestString + TFD_EXTENSION;
								string transformedFeaturesOTFileName = interpolatedMapFileNameForRayTracing + TRANSFORMED_FEATURES_OT_EXTENSION_PART + trainOrTestString + TFD_EXTENSION;

								string normalMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + NORMALMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;		//NOT USED for 2D!
								string depthMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + DEPTHMAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;		//NOT USED for 2D!
								string depthContrastMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + DEPTH_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;		//NOT USED for 2D!
								string depthContrastBooleanMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + DEPTH_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED for 2D!
								string depthGradientMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + DEPTH_GRADIENT_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED for 2D!
								string depthGradientContrastMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + DEPTH_GRADIENT_CONTRAST_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED for 2D!
								string depthGradientContrastBooleanMapFacingPolyFileName = interpolatedMapFileNameForRayTracing + DEPTH_GRADIENT_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION_PART + trainOrTestString + PPM_EXTENSION;	//NOT USED for 2D!



								RTviewInfo viFacingPoly;		//used by raytracer, and 3DOD with OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING



													#ifndef OR_METHOD_3DOD_USE_FORMAL_TRANSFORMATION_METHOD
														if(dimension == OR_METHOD3DOD_DIMENSIONS)
														{


															double normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeStart;
															if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
															{
																if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
																{
																	cout << "\t\t\t start: 3a. 3DOD normalised snapshot generation - calculate snapshot view wrt polygon" << endl;
																}

																normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeStart = getTimeAsLong();
															}


															vec eyeFacingPoly;
															vec viewAtFacingPoly;
															vec viewUpFacingPoly;

															calculateEyePositionAndOrientation3DOD(&eyeFacingPoly, &viewAtFacingPoly, &viewUpFacingPoly, &viewPortWidthHeightDepth, currentPolygonInList, side);


															#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING

															double featurePosNoise1X = oriNoiseArray[featurePosNoise1][0];
															double featurePosNoise1Y = oriNoiseArray[featurePosNoise1][1];
															double featurePosNoise1Z = oriNoiseArray[featurePosNoise1][2];
															double featurePosNoise2X = posNoiseArray[featurePosNoise2][0];
															double featurePosNoise2Y = posNoiseArray[featurePosNoise2][1];
															double featurePosNoise2Z = posNoiseArray[featurePosNoise2][2];

															viFacingPoly.eye.x = eyeFacingPoly.x + featurePosNoise2X;
															viFacingPoly.eye.y = eyeFacingPoly.y + featurePosNoise2Y;
															viFacingPoly.eye.z = eyeFacingPoly.z + featurePosNoise2Z;
															viFacingPoly.viewUp.x = viewUpFacingPoly.x + featurePosNoise1X + featurePosNoise2X;
															viFacingPoly.viewUp.y = viewUpFacingPoly.y + featurePosNoise1Y + featurePosNoise2Y;
															viFacingPoly.viewUp.z = viewUpFacingPoly.z + featurePosNoise1Z + featurePosNoise2Z;
															viFacingPoly.viewAt.x = viewAtFacingPoly.x + featurePosNoise2X;
															viFacingPoly.viewAt.y = viewAtFacingPoly.y + featurePosNoise2Y;
															viFacingPoly.viewAt.z = viewAtFacingPoly.z + featurePosNoise2Z;
															#else
															viFacingPoly.eye.x = eyeFacingPoly.x;
															viFacingPoly.eye.y = eyeFacingPoly.y;
															viFacingPoly.eye.z = eyeFacingPoly.z;
															viFacingPoly.viewUp.x = viewUpFacingPoly.x;
															viFacingPoly.viewUp.y = viewUpFacingPoly.y;
															viFacingPoly.viewUp.z = viewUpFacingPoly.z;
															viFacingPoly.viewAt.x = viewAtFacingPoly.x;
															viFacingPoly.viewAt.y = viewAtFacingPoly.y;
															viFacingPoly.viewAt.z = viewAtFacingPoly.z;
															#endif

															#ifdef USE_OPENGL
															vec viewPortWidthHeightDepth;
															viewPortWidthHeightDepth.x = OR_METHOD3DOD_SNAPSHOT_DISTANCE;
															viewPortWidthHeightDepth.y = OR_METHOD3DOD_SNAPSHOT_DISTANCE;
															viewPortWidthHeightDepth.z = OR_METHOD3DOD_SNAPSHOT_DISTANCE*2.0;
															setViewPort3D(&(viFacingPoly.eye), &(viFacingPoly.viewAt), &(viFacingPoly.viewUp), &viewPortWidthHeightDepth);
															//setViewPort3Dbasic();
															/*if scale object data instead of normalising distance viewport is away from t;
															transformObjectData3DOD(firstReferenceInInterpolatedMesh, currentPolygonInList, side, first);
															*/
															#else
															//use raytracer;
															viFacingPoly.imageWidth = imageWidthFacingPoly;
															viFacingPoly.imageHeight = imageHeightFacingPoly;
															#ifdef PRDATOR_OVERRULE_KNOWN_BAD_EYEPOS_AND_ORIENTATION_AUTO_GEN
															viFacingPoly.focalLength = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL;
															#else
															viFacingPoly.focalLength = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL;
															#endif
															viFacingPoly.viewWidth = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH;
															viFacingPoly.viewHeight = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT;
															#endif

															cout << "NB there is a known problem here with generation of eye and viewUp coordinates" << endl;

															if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
															{
																if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
																{
																	cout << "\t\t\t end: 3a. 3DOD normalised snapshot generation - calculate snapshot view wrt polygon" << endl;
																}
																double normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeEnd;
																normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeEnd = getTimeAsLong();
																if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
																{
																	time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTrain = time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTrain+ normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeEnd-normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeStart;
																	time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTrain++;
																}
																else if(trainOrTest == TEST)
																{
																	time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTest = time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonTotalTest+ normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeEnd-normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeStart;
																	time3aNormalisedSnapshotGeneration3DODCalculateSnapshotViewWRTPolygonIndexTest++;
																}
																if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
																{
																	cout << "normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTime = " << normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeEnd-normalisedSnapshotGeneration3DOD3aCalculateSnapshotViewWRTPolygonTimeStart << endl;
																}
															}

														}
														else if(dimension == OR_METHOD2DOD_DIMENSIONS)
														{
													#endif
								long time3aNormalisedSnapshotGenerationTransformDataWRTPolygonStart;
								if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
								{
									if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
									{
										cout << "\t\t\t start: 3a. normalised snapshot generation - transform data wrt polygon" << endl;
									}

									time3aNormalisedSnapshotGenerationTransformDataWRTPolygonStart = getTimeAsLong();
								}
								bool first = false;

								#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
								if((polygonIndex == 0) && (side == 0) && (featurePosNoise1 == 0) && (featurePosNoise2 == 0)  && (featurePosNoise3 == 0))
								{
									first = true;
								}
								#else
								if((polygonIndex == 0) && (side == 0))
								{
									first = true;
								}
								#endif

								if(dimension == OR_METHOD3DOD_DIMENSIONS)
								{
									transformObjectData3DOD(firstReferenceInInterpolatedMesh, currentPolygonInList, side, first, &(firstFeatureInList[zoomIndex]));

									//use raytracer;

									#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
									double featurePosNoise1X = oriNoiseArray[featurePosNoise1][0];
									double featurePosNoise1Y = oriNoiseArray[featurePosNoise1][1];
									double featurePosNoise1Z = oriNoiseArray[featurePosNoise1][2];
									double featurePosNoise2X = posNoiseArray[featurePosNoise2][0];
									double featurePosNoise2Y = posNoiseArray[featurePosNoise2][1];
									double featurePosNoise2Z = posNoiseArray[featurePosNoise2][2];
									viFacingPoly.eye.x = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_X + featurePosNoise2X;
									viFacingPoly.eye.y = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_Y + featurePosNoise2Y;
									viFacingPoly.eye.z = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_Z + featurePosNoise2Z;
									viFacingPoly.viewUp.x = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_X + featurePosNoise1X + featurePosNoise2X;
									viFacingPoly.viewUp.y = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_Y + featurePosNoise1Y + featurePosNoise2Y;
									viFacingPoly.viewUp.z = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_Z + featurePosNoise1Z + featurePosNoise2Z;
									viFacingPoly.viewAt.x = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_X + featurePosNoise2X;
									viFacingPoly.viewAt.y = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_Y + featurePosNoise2Y;
									viFacingPoly.viewAt.z = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_Z + featurePosNoise2Z;
									viFacingPoly.focalLength = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL;
									viFacingPoly.viewWidth = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH;
									viFacingPoly.viewHeight = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT;
									#endif

									#ifdef USE_OPENGL
									#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
									vec viewPortWidthHeightDepth;
									viewPortWidthHeightDepth.x = OR_METHOD3DOD_SNAPSHOT_DISTANCE;
									viewPortWidthHeightDepth.y = OR_METHOD3DOD_SNAPSHOT_DISTANCE;
									viewPortWidthHeightDepth.z = OR_METHOD3DOD_SNAPSHOT_DISTANCE*2.0;
									setViewPort3D(&(viFacingPoly.eye), &(viFacingPoly.viewAt), &(viFacingPoly.viewUp), &viewPortWidthHeightDepth);
									#endif
									#else
									viFacingPoly.focalLength = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL;
									viFacingPoly.viewWidth = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH;
									viFacingPoly.viewHeight = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT;
									#endif
								}
								else if(dimension == OR_METHOD2DOD_DIMENSIONS)
								{
									#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
									double featurePosNoise1X = oriNoiseArray[featurePosNoise1][0];
									double featurePosNoise1Y = oriNoiseArray[featurePosNoise1][1];
									double featurePosNoise2X = posNoiseArray[featurePosNoise2][0];
									double featurePosNoise2Y = posNoiseArray[featurePosNoise2][1];
									double featurePosNoise3X = posNoiseArray[featurePosNoise3][0];
									double featurePosNoise3Y = posNoiseArray[featurePosNoise3][1];
									currentPolygonInList->point1.x = currentPolygonInList->point1.x + featurePosNoise1X;
									currentPolygonInList->point1.y = currentPolygonInList->point1.y + featurePosNoise1Y;
									currentPolygonInList->point2.x = currentPolygonInList->point2.x + featurePosNoise2X;
									currentPolygonInList->point2.y = currentPolygonInList->point2.y + featurePosNoise2Y;
									currentPolygonInList->point3.x = currentPolygonInList->point3.x + featurePosNoise3X;
									currentPolygonInList->point3.y = currentPolygonInList->point3.y + featurePosNoise3Y;
									#endif

									transformObjectData2DOD(firstReferenceInInterpolatedMesh, currentPolygonInList, side, first, &(firstFeatureInList[zoomIndex]));

									#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
									currentPolygonInList->point1.x = currentPolygonInList->point1.x - featurePosNoise1X;	//restore orig feat pos
									currentPolygonInList->point1.y = currentPolygonInList->point1.y - featurePosNoise1Y;	//restore orig feat pos
									currentPolygonInList->point2.x = currentPolygonInList->point2.x - featurePosNoise2X;	//restore orig feat pos
									currentPolygonInList->point2.y = currentPolygonInList->point2.y - featurePosNoise2Y;	//restore orig feat pos
									currentPolygonInList->point3.x = currentPolygonInList->point3.x - featurePosNoise3X;	//restore orig feat pos
									currentPolygonInList->point3.y = currentPolygonInList->point3.y - featurePosNoise3Y;	//restore orig feat pos
									#endif

									#ifndef USE_OPENGL
									//use raytracer;
									viFacingPoly.eye.x = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_X;
									viFacingPoly.eye.y = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_Y;
									viFacingPoly.eye.z = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_Z;
									viFacingPoly.viewUp.x = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_X;
									viFacingPoly.viewUp.y = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_Y;
									viFacingPoly.viewUp.z = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_Z;
									viFacingPoly.viewAt.x = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_X;
									viFacingPoly.viewAt.y = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_Y;
									viFacingPoly.viewAt.z = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_Z;
									viFacingPoly.focalLength = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_FOCAL;
									viFacingPoly.viewWidth = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH;
									viFacingPoly.viewHeight = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT;
									#endif
								}
								else
								{
									cout << "Error: Invalid Dimension" << endl;
									exit(0);
								}


								if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
								{
									if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
									{
										cout << "\t\t\t end: 3a. normalised snapshot generation - transform data wrt polygon" << endl;
									}
									long time3aNormalisedSnapshotGenerationTransformDataWRTPolygonEnd;
									time3aNormalisedSnapshotGenerationTransformDataWRTPolygonEnd = getTimeAsLong();
									if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
									{
										time3aNormalisedSnapshotGenerationTransformDataWRTPolygonTotalTrain = time3aNormalisedSnapshotGenerationTransformDataWRTPolygonTotalTrain+ time3aNormalisedSnapshotGenerationTransformDataWRTPolygonEnd-time3aNormalisedSnapshotGenerationTransformDataWRTPolygonStart;
										time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTrain++;

									}
									else if(trainOrTest == TEST)
									{
										time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonTotalTest = time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonTotalTest+ time3aNormalisedSnapshotGenerationTransformDataWRTPolygonEnd-time3aNormalisedSnapshotGenerationTransformDataWRTPolygonStart;
										time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygonIndexTest++;
									}
									if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
									{
										cout << "\t\t\t time3aNormalisedSnapshotGeneration2DODTransformDataWRTPolygon = " << time3aNormalisedSnapshotGenerationTransformDataWRTPolygonEnd-time3aNormalisedSnapshotGenerationTransformDataWRTPolygonStart << endl;
									}
								}

														#ifndef OR_METHOD_3DOD_USE_FORMAL_TRANSFORMATION_METHOD
															}
															else
															{
																cout << "Error: Invalid Dimension" << endl;
																exit(0);
															}
														#endif


								long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotStart;
								if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
								{
									if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
									{
										cout << "\t\t\t start: 3b. normalised snapshot generation - raytrace or opengl snapshot" << endl;
									}

									time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotStart = getTimeAsLong();
								}

								#ifdef USE_OPENGL

								bool usePredefinedODmatrixOperations;

						#ifdef OR_METHOD_3DOD
							#ifdef OR_METHOD_3DOD_USE_FORMAL_TRANSFORMATION_METHOD
								#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
								usePredefinedODmatrixOperations = true;
								#else
								usePredefinedODmatrixOperations = false;
								#endif
							#else
								usePredefinedODmatrixOperations = false;
							#endif
						#else
								#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
								usePredefinedODmatrixOperations = true;
								#else
								usePredefinedODmatrixOperations = false;
								#endif
						#endif

								if(dimension == OR_METHOD3DOD_DIMENSIONS)
								{
									#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS
									drawPrimitivesReferenceListToOpenGL(firstReferenceInInterpolatedMesh, dimension, usePredefinedODmatrixOperations);
									updateScreen();
									writeScreenToRGBMap(uncroppedWidth, uncroppedHeight, rgbMapUncroppedFacingPoly);
									updateScreen();
									writeScreenToDepthMap(uncroppedWidth, uncroppedHeight, depthMapUncroppedFacingPoly);
									updateScreen();
									#else
									drawPrimitivesReferenceListToOpenGLandCreateRGBmap(firstReferenceInInterpolatedMesh, uncroppedWidth, uncroppedHeight, rgbMapUncroppedFacingPoly, dimension, usePredefinedODmatrixOperations);
								#endif
									}
								else if(dimension == OR_METHOD2DOD_DIMENSIONS)
								{
									drawPrimitivesReferenceListToOpenGLandCreateRGBmap(firstReferenceInInterpolatedMesh, uncroppedWidth, uncroppedHeight, rgbMapUncroppedFacingPoly, dimension, usePredefinedODmatrixOperations);
								}
								else
								{
									cout << "Error: Invalid Dimension" << endl;
									exit(0);
								}

								#else

									//create Tal file;
								write2DReferenceListCollapsedTo1DToFileRayTraceFormat(charstarinterpolatedMapFileNameForRayTracingTAL, firstReferenceInInterpolatedMesh, true, &viFacingPoly, false, NULL, NULL);		//original

								//add map to reference list - TEMP
								write2DreferenceListCollapsedTo1DtoFile(charstarinterpolatedMapFileNameForRayTracingLDR, firstReferenceInInterpolatedMesh);

									//raytrace Tal file;
								//setSceneLightingConditions(1.0, 1.0, 1.0, 0.0, 0.0);	//turn off diffuse and specular highlighting; only use ambient highlighting
								setLightingMode(LIGHTING_MODE_BASIC);
								cout << "charstarinterpolatedMapFileNameForRayTracingTAL = " << charstarinterpolatedMapFileNameForRayTracingTAL << endl;
								rayTraceScene(charstarinterpolatedMapFileNameForRayTracingTAL, NULL, false, true, rgbMapUncroppedFacingPoly, depthMapFacingPoly, normalMapFacingPoly, pointMapFacingPoly);

								#endif

								if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
								{
									if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
									{
										cout << "\t\t\t end: 3b. normalised snapshot generation - raytrace or opengl snapshot" << endl;
									}
									long time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotEnd;
									time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotEnd = getTimeAsLong();
									if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
									{
										time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTrain = time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTrain+ time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotEnd-time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotStart;
										time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTrain++;
									}
									else if(trainOrTest == TEST)
									{
										time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTest = time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotTotalTest+ time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotEnd-time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotStart;
										time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotIndexTest++;
									}
									if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
									{
										cout << "\t\t\t time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshot = " << time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotEnd-time3bNormalisedSnapshotGenerationRaytraceOrOpenGLSnapshotStart << endl;
									}
								}


							#ifndef OR_USE_OR_NEURAL_NETWORK_COMPARITOR

								if(performCrop)
								{
									cropRGBmap(uncroppedWidth, uncroppedHeight, cropWidth, cropHeight, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapUncroppedFacingPoly, rgbMapFacingPoly);
								}
								else
								{
									rgbMapFacingPoly = rgbMapUncroppedFacingPoly;
								}


								double normalisedAverageHueContrastR = 0;
								double normalisedAverageHueContrastG = 0;
								double normalisedAverageHueContrastB = 0;
								double averageContrastWithRespectToAverageColour;
								double averageStarkContrastWithRespectToAverageColour;
								double averageLocalContrast;
								double averageLocalStarkContrast;

								createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly, rgbDevIEnormalisedHueContrastMapFacingPoly, &normalisedAverageHueContrastR, &normalisedAverageHueContrastG, &normalisedAverageHueContrastB, &averageContrastWithRespectToAverageColour, &averageStarkContrastWithRespectToAverageColour, &averageLocalContrast, &averageLocalStarkContrast);

								bool passedContrastCheck = false;
								#ifdef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
								if(doNotCheckContrast)
								{
									passedContrastCheck = true;
								}
								else
								{
									if(side > 0)
									{
										if(numPolySideSnapshotsPassedContrastChecks == 0)
										{
											passedContrastCheck = false;
										}
										else
										{
											passedContrastCheck = true;
										}
									}
									else
									{//side == 0
										bool currentImagePassedContrastCheck = false;
										if(checkImageHasContrastValuesOnly(averageContrastWithRespectToAverageColour, averageStarkContrastWithRespectToAverageColour, averageLocalContrast, averageLocalStarkContrast))
										{
											currentImagePassedContrastCheck = true;
										}

										if(currentImagePassedContrastCheck)
										{
											passedContrastCheck = true;
										}
										else
										{
											passedContrastCheck = false;
										}
									}
								}
								#else
								passedContrastCheck = true;
								#endif
								if(passedContrastCheck)
								{
									#ifdef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
									numPolySideSnapshotsPassedContrastChecks++;
									#endif

									//a. generate snapshot maps (eg 28x28 pixels);
									if(generatePixmapFiles)
									{
										generatePixmapFromRGBmap(rgbMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
									}

									if(OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
									{
										//this is only used for test, as cannot properly draw a vector map info in a 2D pixmap
										if(generatePixmapFiles)
										{
											generatePixmapFromNormalMap(rgbMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, rgbDevIEnormalisedHueContrastMapFacingPoly);
										}
									}
									#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS
									if(dimension == OR_METHOD3DOD_DIMENSIONS)
									{
										//temp;	//generatePixmapFromDepthMapOrDepthContrastMapAdvanced(depthMapFacingPolyFileName, uncroppedWidth, uncroppedHeight, depthMapUncroppedFacingPoly, UNKNOWN_OPENGL_NOHIT_DEPTH, UNKNOWN_OPENGL_ESTIMATE_MAX_DEPTH);
										if(OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP)
										{
											cropDepthMap(uncroppedWidth, uncroppedHeight, OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X, OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y, imageWidthFacingPoly, imageHeightFacingPoly, depthMapUncroppedFacingPoly, depthMapFacingPoly);
										}
										else
										{
											depthMapFacingPoly = depthMapUncroppedFacingPoly;
										}
										if(generatePixmapFiles)
										{
											generatePixmapFromDepthMapOrDepthContrastMapAdvanced(depthMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthMapFacingPoly, UNKNOWN_OPENGL_NOHIT_DEPTH, UNKNOWN_OPENGL_ESTIMATE_MAX_DEPTH);
										}
									}
									#endif

									//b. generate small snapshot maps (eg 7x7 pixels);
								#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON

									#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD2_IMAGEMAGICK_CONVERT_INTERPOLATION

										string convertImageToSmallImageCommand = "";
										int resizePercentage = 100/smallImageRatio;
										unsigned char resizePercentageString[10];
										sprintf(resizePercentageString, "%d", resizePercentage);
										cout << "resizePercentageString = " << resizePercentageString << endl;

										convertImageToSmallImageCommand = convertImageToSmallImageCommand + "convert " + "-depth 8 -resize '" + resizePercentageString + "%' " + rgbMapFacingPolyFileName + " " + rgbMapFacingPolySmallFileName;
										system(convertImageToSmallImageCommand.c_str());
										pixmap* rgbPixMapSmall;
										rgbPixMapSmall = loadPPM(rgbMapFacingPolySmallFileName);
										createRGBMapFromPixmapImage(rgbPixMapSmall, rgbMapSmallFacingPoly);
										freePixmap(rgbPixMapSmall);
										remove(rgbMapFacingPolySmallFileName.c_str());

									#else

										int ignoreBackgroundComparisonMethod;
										if(dimension == OR_METHOD3DOD_DIMENSIONS)
										{
											ignoreBackgroundComparisonMethod = OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_OFF;
										}
										else if(dimension == OR_METHOD2DOD_DIMENSIONS)
										{
											ignoreBackgroundComparisonMethod = OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE_COMPLETELY;
										}
										#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
										cropRGBmap(uncroppedWidth, uncroppedHeight, cropWidthWithGaussianApplied, cropHeightWithGaussianApplied, imageWidthFacingPolyWithGaussianApplied, imageHeightFacingPolyWithGaussianApplied, rgbMapUncroppedFacingPoly, rgbMapFacingPolyWithGaussianApplied);
										applyGaussianKernelToRGBmap(imageWidthFacingPolyWithGaussianApplied, imageHeightFacingPolyWithGaussianApplied, OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIGMA, rgbMapFacingPolyWithGaussianApplied, rgbMapFacingPolyWithGaussianApplied);
										cropRGBmap(imageWidthFacingPolyWithGaussianApplied, imageHeightFacingPolyWithGaussianApplied, OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2, OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPolyWithGaussianApplied, rgbMapFacingPolyWithGaussianAppliedCropped);
										if(dimension == OR_METHOD2DOD_DIMENSIONS)
										{
											disablePixelsThatAreNotContainedInTheObjectTriangle2DOD(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPolyWithGaussianAppliedCropped);
										}
										resampleRGBmap(rgbMapFacingPolyWithGaussianAppliedCropped, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapSmallFacingPoly, smallImageRatio, ignoreBackgroundComparisonMethod);
										#else
										resampleRGBmap(rgbMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapSmallFacingPoly, smallImageRatio, ignoreBackgroundComparisonMethod);
										#endif
									#endif

										if(generatePixmapFiles)
										{
											generatePixmapFromRGBmap(rgbMapFacingPolySmallFileName, smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly);
											#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
											generateBooleanContrastPixmapFromRGBMap(booleanContrastRgbMapFacingPolySmallFileName, smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly);
											#endif
										}

										double smallTempnormalisedAverageRHueContrast;
										double smallTempnormalisedAverageGHueContrast;
										double smallTempnormalisedAverageBHueContrast;
										double smallTempaverageContrastWithRespectToAverageColour;
										double smallTempaverageStarkContrastWithRespectToAverageColour;
										double smallTempaverageLocalContrast;
										double smallTempaverageLocalStarkContrast;
										if(OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
										{
											createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly, rgbDevIEnormalisedHueContrastMapSmallFacingPoly, &smallTempnormalisedAverageRHueContrast, &smallTempnormalisedAverageGHueContrast, &smallTempnormalisedAverageBHueContrast, &smallTempaverageContrastWithRespectToAverageColour, &smallTempaverageStarkContrastWithRespectToAverageColour, &smallTempaverageLocalContrast, &smallTempaverageLocalStarkContrast);
										}

										#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS
										#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS_ADVANCED_RESAMPLING
										resampleLumOrContrastOrDepthMap(depthMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, depthMapSmallFacingPoly, smallImageRatio, UNKNOWN_OPENGL_NOHIT_DEPTH);
										if(generatePixmapFiles)
										{
											generatePixmapFromDepthMapOrDepthContrastMapAdvanced(depthMapFacingPolySmallFileName, smallImageWidth, smallImageHeight, depthMapSmallFacingPoly, UNKNOWN_OPENGL_NOHIT_DEPTH, UNKNOWN_OPENGL_ESTIMATE_MAX_DEPTH);
										}
										#else
										generateRGBMapFromDepthMapOrDepthContrastMapAdvanced(imageWidthFacingPoly, imageHeightFacingPoly, depthMapFacingPoly, tempDepthRGBMapFacingPoly, UNKNOWN_OPENGL_NOHIT_DEPTH, UNKNOWN_OPENGL_ESTIMATE_MAX_DEPTH);
										resampleRGBmap(tempDepthRGBMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, depthRGBMapSmallFacingPoly, smallImageRatio);
										if(generatePixmapFiles)
										{
											generatePixmapFromRGBmap(depthMapFacingPolySmallFileName, smallImageWidth, smallImageHeight, depthRGBMapSmallFacingPoly);
										}
										#endif
										#endif
								#endif

									//d. prepare feature lists with additional snapshot characterisation data (DCT, average hue etc) used for SQL database snapshot binning
									if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
									{
										vec normalisedAverageHueContrast;
										normalisedAverageHueContrast.x = normalisedAverageHueContrastR;
										normalisedAverageHueContrast.y = normalisedAverageHueContrastG;
										normalisedAverageHueContrast.z = normalisedAverageHueContrastB;
										colour cullednormalisedAverageHueContrast;
										cullAndBinNormalisedHueContrast(&normalisedAverageHueContrast, &cullednormalisedAverageHueContrast);

										currentPolygonInList->firstFeatureInNearestFeatureList->avgCol.r = cullednormalisedAverageHueContrast.r;
										currentPolygonInList->firstFeatureInNearestFeatureList->avgCol.g = cullednormalisedAverageHueContrast.g;
										currentPolygonInList->firstFeatureInNearestFeatureList->avgCol.b = cullednormalisedAverageHueContrast.b;

										#ifdef OR_DEBUG
										//cout << "normalisedAverageHueContrast.x = " << normalisedAverageHueContrast.x << endl;
										//cout << "normalisedAverageHueContrast.y = " << normalisedAverageHueContrast.y << endl;
										//cout << "normalisedAverageHueContrast.z = " << normalisedAverageHueContrast.z << endl;
										//cout << "cullednormalisedAverageHueContrast.r = " << int(cullednormalisedAverageHueContrast.r) << endl;
										//cout << "cullednormalisedAverageHueContrast.g = " << int(cullednormalisedAverageHueContrast.g) << endl;
										//cout << "cullednormalisedAverageHueContrast.b = " << int(cullednormalisedAverageHueContrast.b) << endl;
										#endif

										/*
										//non rgb dev;
										colour avgCol;
										calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly, &avgCol);

										int rBin = (int)(((double)(avgCol.r)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
										int gBin = (int)(((double)(avgCol.g)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
										int bBin = (int)(((double)(avgCol.b)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);

										currentPolygonInList->firstFeatureInNearestFeatureList->avgCol.r = avgCol.r;
										currentPolygonInList->firstFeatureInNearestFeatureList->avgCol.g = avgCol.g;
										currentPolygonInList->firstFeatureInNearestFeatureList->avgCol.b = avgCol.b;
										*/
									}

									if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
									{
										if(!generatePixmapFiles)
										{
											generatePixmapFromRGBmap(rgbMapFacingPolySmallFileName, smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly);
										}

										readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(&rgbMapFacingPolySmallFileName, &rgbMapFacingPolySmallFileNameJPEG, currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeff, false);

									#ifndef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
										#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
										int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
										currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeffArrayBinned = convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeff, concatonatedDctCoeffArrayBiasIntNOTUSED);
										#else
										currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeffArrayBinned = convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeff);
										#endif
									#endif
										#ifdef OR_DEBUG
										//cout << "currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeffArrayBinned = " << currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeffArrayBinned << endl;
										#endif

										if(!generatePixmapFiles)
										{
											remove(rgbMapFacingPolySmallFileName.c_str());
											remove(rgbMapFacingPolySmallFileNameJPEG.c_str());
										}
									}

									if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
									{

									}

									//c. save feature lists to file - will not be used in the future when SQL database takes over entirely
									bool createNearbyFeaturesFile = false;
									if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST)
									{
										if(trainOrTest == TEST)
										{
											createNearbyFeaturesFile = true;
										}
									}
									if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
									{
										createNearbyFeaturesFile = true;
									}
									if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE)
									{
										if(createNearbyFeaturesFile)
										{
											createTransformedFeaturesFile(currentPolygonInList->firstFeatureInNearestFeatureList, transformedFeaturesNearestFileName, objectName, viewIndex, zoomIndex, numberOfTrainOrTestPolys[zoomIndex], side, trainOrTest);		//create ascii file containing nearby transformed features
										}
									}
									if(OR_METHOD_CREATE_ALL_FEATURES_FILE)
									{
										createTransformedFeaturesFile(&(firstFeatureInList[zoomIndex]), transformedFeaturesAllFileName, objectName, viewIndex, zoomIndex, numberOfTrainOrTestPolys[zoomIndex], side, trainOrTest);							//create ascii file containing all transformed features
									}
									if(OR_METHOD_CREATE_OT_FEATURES_FILE)
									{
										ORfeature* firstFeatureInTransformedObjectTriangleList = new ORfeature();
										ORfeature* secondFeatureInTransformedObjectTriangleList = new ORfeature();
										ORfeature* thirdFeatureInTransformedObjectTriangleList = new ORfeature();
										ORfeature* lastFeatureInTransformedObjectTriangleList = new ORfeature();
										firstFeatureInTransformedObjectTriangleList->pointTransformed.x = currentPolygonInList->point1Transformed.x;
										firstFeatureInTransformedObjectTriangleList->pointTransformed.y = currentPolygonInList->point1Transformed.y;
										firstFeatureInTransformedObjectTriangleList->pointTransformed.z = currentPolygonInList->point1Transformed.z;
										secondFeatureInTransformedObjectTriangleList->pointTransformed.x = currentPolygonInList->point2Transformed.x;
										secondFeatureInTransformedObjectTriangleList->pointTransformed.y = currentPolygonInList->point2Transformed.y;
										secondFeatureInTransformedObjectTriangleList->pointTransformed.z = currentPolygonInList->point2Transformed.z;
										thirdFeatureInTransformedObjectTriangleList->pointTransformed.x = currentPolygonInList->point3Transformed.x;
										thirdFeatureInTransformedObjectTriangleList->pointTransformed.y = currentPolygonInList->point3Transformed.y;
										thirdFeatureInTransformedObjectTriangleList->pointTransformed.z = currentPolygonInList->point3Transformed.z;
										firstFeatureInTransformedObjectTriangleList->next = secondFeatureInTransformedObjectTriangleList;
										secondFeatureInTransformedObjectTriangleList->next = thirdFeatureInTransformedObjectTriangleList;
										thirdFeatureInTransformedObjectTriangleList->next = lastFeatureInTransformedObjectTriangleList;
										createTransformedFeaturesFile(firstFeatureInTransformedObjectTriangleList, transformedFeaturesOTFileName, objectName, viewIndex, zoomIndex, numberOfTrainOrTestPolys[zoomIndex], side, trainOrTest);							//create ascii file containing all transformed features
										delete firstFeatureInTransformedObjectTriangleList;
									}


									//e. input snapshot data into sql
									#ifdef OR_IMAGE_COMPARISON_SQL

									unsigned char* rgb8bitSmallMapForInstantDBqueryAccess;
									if(OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
									{
										#ifdef OR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V2_OR_V3
										convertNormalisedHueDeviationMapTo3x8bitMap(smallImageWidth, smallImageHeight, rgbDevIEnormalisedHueContrastMapSmallFacingPoly, rgbDev8BitSmallMapFacingPoly);
										rgb8bitSmallMapForInstantDBqueryAccess = rgbDev8BitSmallMapFacingPoly;
										#else
										rgb8bitSmallMapForInstantDBqueryAccess = rgbMapSmallFacingPoly;
										#endif
									}
									else
									{
										rgb8bitSmallMapForInstantDBqueryAccess = NULL;
									}

									if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL || (trainOrTest != TEST))
									{
										if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
										{
											convertSnapshotMapsToStringForSQLdatabaseEntry(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly, rgbDevIEnormalisedHueContrastMapFacingPoly, depthMapFacingPoly, smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly, depthMapSmallFacingPoly, rgbDevIEnormalisedHueContrastMapSmallFacingPoly, dimension, currentPolygonInList->firstFeatureInNearestFeatureList->snapshotMapsText, OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS, &(currentPolygonInList->firstFeatureInNearestFeatureList->snapshotMapsTextLength));
											#ifdef OR_DEBUG
											//cout << "(currentPolygonInList->firstFeatureInNearestFeatureList->snapshotMapsTextLength) = " << (currentPolygonInList->firstFeatureInNearestFeatureList->snapshotMapsTextLength) << endl;
											#endif
										}

									}
									if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
									{
										bool ignoreOTfeatures;
										if(dimension == OR_METHOD2DOD_DIMENSIONS)
										{
											ignoreOTfeatures = true;
										}
										else if(dimension == OR_METHOD3DOD_DIMENSIONS)
										{
											#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
											ignoreOTfeatures = true;
											#else
											ignoreOTfeatures = false;
											#endif
										}

										bool addPermutationsOfTrainFeaturesForGeoBinning;
										int maxNumFeaturePermutations;
										string tableName;
										long databaseTableSize;

										if(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING)
										{
											maxNumFeaturePermutations = 100;
											addPermutationsOfTrainFeaturesForGeoBinning = true;
										}
										else
										{
											maxNumFeaturePermutations = 1;
											addPermutationsOfTrainFeaturesForGeoBinning = false;
										}
										databaseTableSize = databaseTableSizeTrain;
										tableName = OR_MYSQL_TABLE_NAME_TRAIN;

										insertTransformedFeatureListIntoDatabase(currentPolygonInList->firstFeatureInNearestFeatureList, objectName, viewIndex, zoomIndex, numberOfTrainOrTestPolys[zoomIndex], side, TRAIN, ignoreOTfeatures, rgb8bitSmallMapForInstantDBqueryAccess, smallImageWidth, smallImageHeight,  addPermutationsOfTrainFeaturesForGeoBinning, maxNumFeaturePermutations, tableName, &databaseTableSize);

										databaseTableSizeTrain = databaseTableSize;
									}

									if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
									{
										if((trainOrTest == TEST) || (trainOrTest == TRAIN_AND_TEST))
										{
											bool ignoreOTfeatures;
											if(dimension == OR_METHOD2DOD_DIMENSIONS)
											{
												ignoreOTfeatures = true;
											}
											else if(dimension == OR_METHOD3DOD_DIMENSIONS)
											{
												#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
												ignoreOTfeatures = true;
												#else
												ignoreOTfeatures = false;
												#endif
											}

											bool addPermutationsOfTrainFeaturesForGeoBinning;
											int maxNumFeaturePermutations;
											string tableName;
											long databaseTableSize;

											maxNumFeaturePermutations = 1;
											addPermutationsOfTrainFeaturesForGeoBinning = false;
											databaseTableSize = databaseTableSizeTest;
											tableName = OR_MYSQL_TABLE_NAME_TEST;

											insertTransformedFeatureListIntoDatabase(currentPolygonInList->firstFeatureInNearestFeatureList, objectName, viewIndex, zoomIndex, numberOfTrainOrTestPolys[zoomIndex], side, TEST, ignoreOTfeatures, rgb8bitSmallMapForInstantDBqueryAccess, smallImageWidth, smallImageHeight,  addPermutationsOfTrainFeaturesForGeoBinning, maxNumFeaturePermutations, tableName, &databaseTableSize);

											databaseTableSizeTest = databaseTableSize;
										}
									}

									#endif

									if(OR_IMAGE_COMPARISON_DECISION_TREE)
									{
										if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
										{

											bool ignoreOTfeatures;
											if(dimension == OR_METHOD2DOD_DIMENSIONS)
											{
												ignoreOTfeatures = true;
											}
											else if(dimension == OR_METHOD3DOD_DIMENSIONS)
											{
												#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
												ignoreOTfeatures = true;
												#else
												ignoreOTfeatures = false;
												#endif
											}

											#ifdef OR_DEBUG
											/*
											for(int i=0; i < OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
											{
												cout << "i = " << i << ", coeff=" << int(currentPolygonInList->firstFeatureInNearestFeatureList->dctCoeff[i]) << endl;
											}
											*/
											#endif

											//f. input snapshot data into image comparison decision tree
											//addSnapshotIDReferenceToImageComparisonDecisionTree(smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly);
											long snapshotReferenceID = (databaseTableSizeTrain-1);
											addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly, currentPolygonInList->firstFeatureInNearestFeatureList, snapshotReferenceID, ignoreOTfeatures);		//-1 is required because databaseTableSizeTrain has already been incremented
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											createAndOrParseIntoDirectory(&imageComparisonTreeBaseDirectory, NULL, false, false);	//reset current directory
											#endif
										}
									}
								#ifdef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
								}
								#endif
							#else
								#ifndef OR_MANY_NUMBER_OF_OUTPUT_FILES
								generatePixmapFromRGBmap(rgbMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
								remove(charstarinterpolatedMapFileNameForRayTracingTAL.c_str());
								#else
								if((featurePosNoise1 ==0) && (featurePosNoise2 == 0))
								{
									//generate pixmaps for snapshot
									createLuminosityMapFromRGBMap(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly, luminosityMapFacingPoly);
									createLuminosityBooleanMap(imageWidthFacingPoly, imageHeightFacingPoly, luminosityMapFacingPoly, luminosityBooleanMapFacingPoly);
								#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
									createContrastMapFromMapWithForegroundDepthCheck(imageWidthFacingPoly, imageHeightFacingPoly, luminosityMapFacingPoly, depthMapFacingPoly, luminosityContrastMapFacingPoly, luminosityContrastBooleanMapFacingPoly, foregroundDepthCheckLuminosityContrastBooleanMap, EDGE_LUMINOSITY_CONTRAST_THRESHOLD);
								#else
									createContrastMapFromMap(imageWidthFacingPoly, imageHeightFacingPoly, luminosityMapFacingPoly, luminosityContrastMapFacingPoly);
									createLuminosityContrastBooleanMap(imageWidthFacingPoly, imageHeightFacingPoly, luminosityContrastMapFacingPoly, luminosityContrastBooleanMapFacingPoly);
								#endif

									if(dimension == OR_METHOD3DOD_DIMENSIONS)
									{
									#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
										createContrastMapFromMapWithForegroundDepthCheck(imageWidthFacingPoly, imageHeightFacingPoly, depthMapFacingPoly, depthMapFacingPoly, depthContrastMapFacingPoly, depthContrastBooleanMapFacingPoly, foregroundDepthCheckDepthContrastBooleanMap, EDGE_LUMINOSITY_CONTRAST_THRESHOLD);
									#else
										createContrastMapFromMap(imageWidthFacingPoly, imageHeightFacingPoly, depthMapFacingPoly, depthContrastMapFacingPoly);
										createDepthContrastBooleanMap(imageWidthFacingPoly, imageHeightFacingPoly, depthContrastMapFacingPoly, depthContrastBooleanMapFacingPoly);
									#endif
										createDepthGradientMapFromDepthMap(imageWidthFacingPoly, imageHeightFacingPoly, depthMapFacingPoly, depthGradientMapFacingPoly);
										createDepthGradientContrastMapFromDepthGradientMap(imageWidthFacingPoly, imageHeightFacingPoly, depthGradientMapFacingPoly, depthGradientContrastMapFacingPoly);
										createDepthGradientContrastBooleanMap(imageWidthFacingPoly, imageHeightFacingPoly, depthGradientContrastMapFacingPoly, depthGradientContrastBooleanMapFacingPoly);
										//subtractBooleanMaps(imageWidthFacingPoly, imageHeightFacingPoly, depthGradientContrastBooleanMapFacingPoly, depthContrastBooleanMapFacingPoly, depthGradientContrastMapMinusDepthContrastMapFacingPoly);
										//addBooleanMaps(imageWidthFacingPoly, imageHeightFacingPoly, depthGradientContrastBooleanMapFacingPoly, depthContrastBooleanMapFacingPoly, depthGradientContrastMapPlusDepthContrastMapFacingPoly);
									}

									generatePixmapFromRGBmap(rgbMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
									generatePixmapFromNormalMap(normalMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, normalMapFacingPoly);
									generatePixmapFromLuminosityMap(luminosityMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, luminosityMapFacingPoly);
									generatePixmapFromBooleanMap(luminosityBooleanMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, luminosityBooleanMapFacingPoly);
									generatePixmapFromLuminosityContrastMap(luminosityContrastMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, luminosityContrastMapFacingPoly);
									generatePixmapFromBooleanMap(luminosityContrastBooleanMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, luminosityContrastBooleanMapFacingPoly);

									if(dimension == OR_METHOD3DOD_DIMENSIONS)
									{
										generatePixmapFromNormalMap(normalMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, normalMapFacingPoly);
										generatePixmapFromDepthMapOrDepthContrastMap(depthMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthMapFacingPoly);
										generatePixmapFromDepthMapOrDepthContrastMap(depthContrastMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthContrastMapFacingPoly);
										generatePixmapFromBooleanMap(depthContrastBooleanMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthContrastBooleanMapFacingPoly);
										generatePixmapFromDepthGradientMap(depthGradientMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthGradientMapFacingPoly);
										generatePixmapFromDepthGradientContrastMap(depthGradientContrastMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthGradientContrastMapFacingPoly);
										generatePixmapFromBooleanMap(depthGradientContrastBooleanMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, depthGradientContrastBooleanMapFacingPoly);
									}
								}
								else
								{
									generatePixmapFromRGBmap(rgbMapFacingPolyFileName, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
									remove(charstarinterpolatedMapFileNameForRayTracingTAL.c_str());
								}
								#endif
							#endif

								#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
									//generate nn experience for snapshot
								long objectDecision = polygonIndex*numSidesPerPolygon + side;

							#ifdef OR_FEED_RGB_MAP
								generateExperienceWith2DRGBMap(rgbMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, currentExperience, objectDecision);
							#elif defined OR_FEED_LUMINOSITY_MAP
								generateExperienceWith2DMap(luminosityMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, LUMINOSITY_MAP_MAX_LUMINOSITY_VALUE, currentExperience, objectDecision);
							#elif defined OR_FEED_LUMINOSITY_BOOLEAN_MAP
								generateExperienceWith2DBooleanMap(luminosityBooleanMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, currentExperience, objectDecision);
							#elif defined OR_FEED_LUMINOSITY_CONTRAST_MAP
								generateExperienceWith2DMap(luminosityContrastMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, CONTRAST_MAP_MAX_CONTRAST_VALUE, currentExperience, objectDecision);
							#elif defined OR_FEED_LUMINOSITY_CONTRAST_BOOLEAN_MAP
								generateExperienceWith2DBooleanMap(luminosityContrastBooleanMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, currentExperience, objectDecision);
							#elif defined OR_FEED_DEPTH_MAP_3DONLY
								generateExperienceWith2DMap(depthMapFacingPoly, imageWidthFacingPoly, imageHeightFacingPoly, ESTIMATE_MAX_DEPTH_T_REAL, currentExperience, objectDecision);
							#elif defined OR_FEED_DEPTH_CONTRAST_MAP_3DONLY
								generateExperienceWith2DMap(depthContrastMap, imageWidthFacingPoly, imageHeightFacingPoly, (ESTIMATE_MAX_DEPTH_T_REAL*DEPTH_CONTRAST_FRACTION_THRESHOLD), currentExperience, objectDecision);
							#elif defined OR_FEED_DEPTH_CONTRAST_BOOLEAN_MAP_3DONLY
								generateExperienceWith2DBooleanMap(depthContrastBooleanMap, imageWidthFacingPoly, imageHeightFacingPoly, currentExperience, objectDecision);
							#elif defined OR_FEED_DEPTH_GRADIENT_CONTRAST_MAP_3DONLY
								generateExperienceWith2DMap(depthGradientContrastMap, imageWidthFacingPoly, imageHeightFacingPoly, (ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST*DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD), currentExperience, objectDecision);
							#elif defined OR_FEED_DEPTH_GRADIENT_CONTRAST_BOOLEAN_MAP_3DONLY
								generateExperienceWith2DBooleanMap(depthGradientContrastBooleanMap, imageWidthFacingPoly, imageHeightFacingPoly, currentExperience, objectDecision);
							#else
								cout << "Error: no feed defined" << endl;
								exit(0);
							#endif

								//add experience to experience list
								ANNexperience* newExperience = new ANNexperience();
								currentExperience->next = newExperience;
								currentExperience = currentExperience->next;

								#endif

					#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
							}
						}
					}
					#endif
				}

				#ifdef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
				if((numPolySideSnapshotsPassedContrastChecks == 3) || doNotCheckContrast)
				{
				#endif
					numberOfTrainOrTestPolys[zoomIndex] = numberOfTrainOrTestPolys[zoomIndex] + 1;
					polygonIndex++;
				#ifdef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
				}
				#endif
			}

			currentPolygonInList=currentPolygonInList->next;
		}

		#ifdef OR_DEBUG
		/*
		currentExperience = firstExperienceInList;
		while(currentExperience->next != NULL)
		{
			cout << "currentExperience->classTargetValue = " << currentExperience->classTargetValue << endl;
			ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
			while(currentExperienceInput->next != NULL)
			{
				cout << "currentExperienceInput->inputValue = " << currentExperienceInput->inputValue << endl;
				currentExperienceInput = currentExperienceInput->next;
			}
			currentExperience = currentExperience->next;
		}
		*/
		#endif
	}


 	delete rgbMapFacingPoly;
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
	delete rgbMapFacingPolyWithGaussianApplied;
	delete rgbMapFacingPolyWithGaussianAppliedCropped;
	#endif

	delete rgbDevIEnormalisedHueContrastMapFacingPoly;
	delete depthMapFacingPoly;
	delete luminosityMapFacingPoly;
	delete luminosityBooleanMapFacingPoly;
	delete luminosityContrastMapFacingPoly;
	delete luminosityContrastBooleanMapFacingPoly;
	delete pointMapFacingPoly;
	delete normalMapFacingPoly;

	delete depthContrastMapFacingPoly;	//NOT USED for 2D!
	delete depthContrastBooleanMapFacingPoly;	//NOT USED for 2D!
	delete depthGradientMapFacingPoly;		//NOT USED for 2D!
	delete depthGradientContrastMapFacingPoly;	//NOT USED for 2D!
	delete depthGradientContrastBooleanMapFacingPoly;	//NOT USED for 2D!

//#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
 	delete rgbMapSmallFacingPoly;
	//#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS
	#ifdef OR_METHOD_3DOD_USE_SNAPSHOT_DEPTH_MAPS_ADVANCED_RESAMPLING
 	delete depthMapSmallFacingPoly;
	#else
	delete tempDepthRGBMapFacingPoly;
	delete depthRGBMapSmallFacingPoly;
	#endif
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	delete rgbDevIEnormalisedHueContrastMapSmallFacingPoly;
	delete rgbDev8BitSmallMapFacingPoly;
	//#endif
//#endif

	return result;


}



#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR

ANNneuronContainer* initialiseNormalisedSnapshotNeuralNetwork(ANNneuronContainer* firstInputNeuronInNetwork, int* numberOfInputNeurons, int numberOfOutputNeurons, int imageWidth, int imageHeight)
{
	ANNneuronContainer* firstOutputNeuronInNetwork;

	long numberOfLayers = OR_NUMBER_OF_NN_LAYERS;
	int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D;
	double meanLayerDivergenceFactor = 1.0;
	double probabilityANNneuronConnectionWithPreviousLayerNeuron = 1.0;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0.0;

	ANNexperience* tempExperience = new ANNexperience;
#ifdef OR_FEED_RGB_MAP
	*numberOfInputNeurons = imageWidth* imageHeight* RGB_NUM;
#elif defined OR_FEED_LUMINOSITY_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_LUMINOSITY_BOOLEAN_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_LUMINOSITY_CONTRAST_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_LUMINOSITY_CONTRAST_BOOLEAN_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_DEPTH_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_DEPTH_CONTRAST_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_DEPTH_CONTRAST_BOOLEAN_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_DEPTH_GRADIENT_CONTRAST_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#elif defined OR_FEED_DEPTH_GRADIENT_CONTRAST_BOOLEAN_MAP
	*numberOfInputNeurons = imageWidth* imageHeight;
#else
	cout << "Error: no feed defined" << endl;
	exit(0);
#endif

	firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork,* numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);

	return firstOutputNeuronInNetwork;
}


	//NB this function only works properly if the the neural network was trained on the object at all angles.
double compareNormalisedSnapshotExperienceListWithNeuralNetwork(ANNexperience* firstExperienceInTestList, ANNneuronContainer* firstInputNeuronInNetwork, ANNneuronContainer* firstOutputNeuronInNetwork, int numberOfInputNeurons, int numberOfOutputNeurons, int numberOfTrainPolySides)
{
	double averageMinNNErrorFound = 0.0;
	double minMinNNErrorFound = MAX_ANN_BACK_PROPAGATION_ERROR;

	ANNexperience* currentExperience = firstExperienceInTestList;
	int testSnapshotIndex = 0;
	while(currentExperience->next != NULL)
	{
		double minExperienceBackPropagationPassErrorForAllTrainedOutcomes = MAX_ANN_BACK_PROPAGATION_ERROR;
		int minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex = -1;

		for(int trainedSnapshotIndex=0; trainedSnapshotIndex < numberOfTrainPolySides;trainedSnapshotIndex++)
		{
			currentExperience->classTargetValue = trainedSnapshotIndex;		//replace test experience class target with a given training outcome - test experience classtargets are not used.
			double experienceBackPropagationPassError = calculateExperienceErrorForHypotheticalDecision(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);
			//cout << "\t\t experienceBackPropagationPassError = " << experienceBackPropagationPassError << endl;

			if(experienceBackPropagationPassError < minExperienceBackPropagationPassErrorForAllTrainedOutcomes)
			{
				minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex = trainedSnapshotIndex;
				minExperienceBackPropagationPassErrorForAllTrainedOutcomes = experienceBackPropagationPassError;
			}
		}
		cout << "testSnapshotIndex (Test ANNexperience Index) = " << testSnapshotIndex << endl;
	#ifndef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
		cout << "\t InferredPolygonIndex = " << (testSnapshotIndex/OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
		cout << "\t InferredSideIndex = " << (testSnapshotIndex%OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
	#else
		cout << "\t InferredPolygonIndex = " << ((testSnapshotIndex/(NUM_ORI_NOISE_VALUES*NUM_POS_NOISE_VALUES))/OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
		cout << "\t InferredSideIndex = " << ((testSnapshotIndex/(NUM_ORI_NOISE_VALUES*NUM_POS_NOISE_VALUES))%OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
	#endif
		cout << "\t minExperienceBackPropagationPassErrorForAllTrainedOutcomes = " << minExperienceBackPropagationPassErrorForAllTrainedOutcomes << endl;
		cout << "\t minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex = " << minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex << endl;
	#ifndef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
		cout << "\t minExperienceBackPropagationPassErrorForAllTrainedOutcomesInferredPolygonIndex = " << (minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex/OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
		cout << "\t minExperienceBackPropagationPassErrorForAllTrainedOutcomesInferredSideIndex = " << (minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex%OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
	#else
		cout << "\t minExperienceBackPropagationPassErrorForAllTrainedOutcomesInferredPolygonIndex = " << (minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex/OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
		cout << "\t minExperienceBackPropagationPassErrorForAllTrainedOutcomesInferredSideIndex = " << (minExperienceBackPropagationPassErrorForAllTrainedOutcomesIndex%OR_METHOD_POLYGON_NUMBER_OF_SIDES) << endl;
	#endif

	#ifndef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
		if(testSnapshotIndex < numberOfTrainPolySides)
		{
			averageMinNNErrorFound = averageMinNNErrorFound + minExperienceBackPropagationPassErrorForAllTrainedOutcomes;
		}
	#else
		if(testSnapshotIndex < numberOfTrainPolySides*NUM_ORI_NOISE_VALUES*NUM_POS_NOISE_VALUES)
		{
			averageMinNNErrorFound = averageMinNNErrorFound + minExperienceBackPropagationPassErrorForAllTrainedOutcomes;
		}
	#endif
		/*
		if(minExperienceBackPropagationPassErrorForAllTrainedOutcomes < minMinNNErrorFound)
		{
			minMinNNErrorFound = minExperienceBackPropagationPassErrorForAllTrainedOutcomes;
		}
		*/

		currentExperience = currentExperience->next;
		testSnapshotIndex++;
	}

#ifndef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
	averageMinNNErrorFound = averageMinNNErrorFound/numberOfTrainPolySides;
#else
	averageMinNNErrorFound = averageMinNNErrorFound/(numberOfTrainPolySides*NUM_ORI_NOISE_VALUES*NUM_POS_NOISE_VALUES);
#endif
	return averageMinNNErrorFound;
}

#endif




#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
void setNoiseArraysMethod3DOD()
{
	int arrayLoc = 0;
	for(int uvxIndex = -OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS/2; uvxIndex <= OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS/2; uvxIndex++)
	{
		for(int uvyIndex = -OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS/2; uvyIndex <= OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS/2; uvyIndex++)
		{
			for(int uvzIndex = -OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS/2; uvzIndex <= OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS/2; uvzIndex++)
			{
				oriNoiseArray[arrayLoc][0] = (double)uvxIndex/(OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS-1)*OR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS;
				oriNoiseArray[arrayLoc][1] = (double)uvyIndex/(OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS-1)*OR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS;
				oriNoiseArray[arrayLoc][2] = (double)uvzIndex/(OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS-1)*OR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS;
				arrayLoc++;
			}
		}
	}

	arrayLoc = 0;
	for(int uvxIndex = -OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS/2; uvxIndex <= OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS/2; uvxIndex++)
	{
		for(int uvyIndex = -OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS/2; uvyIndex <= OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS/2; uvyIndex++)
		{
			for(int uvzIndex = -OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS/2; uvzIndex <= OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS/2; uvzIndex++)
			{
				posNoiseArray[arrayLoc][0] = (double)uvxIndex/(OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS;
				posNoiseArray[arrayLoc][1] = (double)uvyIndex/(OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS;
				posNoiseArray[arrayLoc][2] = (double)uvzIndex/(OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS;
				arrayLoc++;
			}
		}
	}
}


void setNoiseArraysMethod2DOD()
{
	int arrayLoc = 0;
	for(int uvxIndex = -OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS/2; uvxIndex <= OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS/2; uvxIndex++)
	{
		for(int uvyIndex = -OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS/2; uvyIndex <= OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS/2; uvyIndex++)
		{
			featurePosNoise1Array[arrayLoc][0] = (double)uvxIndex/(OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS;
			featurePosNoise1Array[arrayLoc][1] = (double)uvyIndex/(OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS;
			featurePosNoise2Array[arrayLoc][0] = (double)uvxIndex/(OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS;
			featurePosNoise2Array[arrayLoc][1] = (double)uvyIndex/(OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS;
			featurePosNoise3Array[arrayLoc][0] = (double)uvxIndex/(OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS;
			featurePosNoise3Array[arrayLoc][1] = (double)uvyIndex/(OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS-1)*OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS;
			arrayLoc++;
		}
	}

}
#endif



int createViFromMultiViewList(RTviewInfo* vi, string fileName, int multiViewViewIndex, int dimension)
{
	cout << "createViFromMultiViewList{}: multiViewViewIndex = " << multiViewViewIndex << endl;

	int lineCount = 0;

	ifstream parseFileObject(fileName.c_str());

	if(parseFileObject.rdbuf()->is_open())
	{
		char c;
		int charCount = 0;
		bool waitingForNewLine = false;

		bool readingObjectName = true;
		bool readingimageext = false;
		bool readingimageWidth = false;
		bool readingimageHeight = false;

		bool readingdepthext = false;
		bool readingvieweyex = false;
		bool readingvieweyey = false;
		bool readingvieweyez = false;
		bool readingviewatx = false;
		bool readingviewaty = false;
		bool readingviewatz = false;
		bool readingviewupx = false;
		bool readingviewupy = false;
		bool readingviewupz = false;
		bool readingviewfocal = false;
		bool readingviewsizew = false;
		bool readingviewsizeh = false;
		bool readingscale = false;

		bool readingxoffset = false;
		bool readingyoffset = false;

		string objectNameString = "";
		string imageextString = "";
		string imageWidthString = "";
		string imageHeightString = "";

		string depthextString = "";
		string vieweyexString = "";
		string vieweyeyString = "";
		string vieweyezString = "";
		string viewatxString = "";
		string viewatyString = "";
		string viewatzString = "";
		string viewupxString = "";
		string viewupyString = "";
		string viewupzString = "";
		string viewfocalString = "";
		string viewsizewString = "";
		string viewsizehString = "";
		string scaleString = "";

		string xoffsetString = "";
		string yoffsetString = "";

		while(parseFileObject.get(c))
		{
			charCount++;
			bool finalise = false;

			if((waitingForNewLine) && (c == CHAR_NEWLINE))
			{
				lineCount++;
				waitingForNewLine = false;
				readingObjectName = true;
			}
			else if(waitingForNewLine)
			{
				//do nothing, still waiting for new line
			}

			else if((readingObjectName) && (c == CHAR_SPACE))
			{
				readingObjectName = false;
				readingimageext = true;
			}
			else if(readingObjectName)
			{
				objectNameString = objectNameString + c;
			}

			else if((readingimageext) && (c == CHAR_SPACE))
			{
				readingimageext = false;
				readingimageWidth= true;
			}
			else if(readingimageext)
			{
				imageextString = imageextString + c;
			}

			else if((readingimageWidth) && (c == CHAR_SPACE))
			{
				readingimageWidth = false;
				readingimageHeight = true;
			}
			else if(readingimageWidth)
			{
				imageWidthString = imageWidthString + c;
			}

			else if((readingimageHeight) && (c == CHAR_SPACE))
			{
				readingimageHeight = false;
				if(dimension == OR_METHOD2DOD_DIMENSIONS)
				{
					readingxoffset = true;
				}
				else if(dimension == OR_METHOD3DOD_DIMENSIONS)
				{
					readingdepthext = true;
				}
				else
				{
					cout << "illegal dimension" << endl;
					exit(0);
				}
			}
			else if(readingimageHeight)
			{
				imageHeightString = imageHeightString + c;
			}

			else if((readingdepthext) && (c == CHAR_SPACE))
			{
				readingdepthext = false;
				readingvieweyex = true;
			}
			else if(readingdepthext)
			{
				depthextString = depthextString + c;
			}

			else if((readingvieweyex) && (c == CHAR_SPACE))
			{
				readingvieweyex = false;
				readingvieweyey = true;
			}
			else if(readingvieweyex)
			{
				vieweyexString = vieweyexString + c;
			}
			else if((readingvieweyey) && (c == CHAR_SPACE))
			{
				readingvieweyey = false;
				readingvieweyez = true;
			}
			else if(readingvieweyey)
			{
				vieweyeyString = vieweyeyString + c;
			}
			else if((readingvieweyez) && (c == CHAR_SPACE))
			{
				readingvieweyez = false;
				readingviewatx = true;
			}
			else if(readingvieweyez)
			{
				vieweyezString = vieweyezString + c;
			}

			else if((readingviewatx) && (c == CHAR_SPACE))
			{
				readingviewatx = false;
				readingviewaty = true;
			}
			else if(readingviewatx)
			{
				viewatxString = viewatxString + c;
			}
			else if((readingviewaty) && (c == CHAR_SPACE))
			{
				readingviewaty = false;
				readingviewatz = true;
			}
			else if(readingviewaty)
			{
				viewatyString = viewatyString + c;
			}
			else if((readingviewatz) && (c == CHAR_SPACE))
			{
				readingviewatz = false;
				readingviewupx = true;
			}
			else if(readingviewatz)
			{
				viewatzString = viewatzString + c;
			}

			else if((readingviewupx) && (c == CHAR_SPACE))
			{
				readingviewupx = false;
				readingviewupy = true;
			}
			else if(readingviewupx)
			{
				viewupxString = viewupxString + c;
			}
			else if((readingviewupy) && (c == CHAR_SPACE))
			{
				readingviewupy = false;
				readingviewupz = true;
			}
			else if(readingviewupy)
			{
				viewupyString = viewupyString + c;
			}
			else if((readingviewupz) && (c == CHAR_SPACE))
			{
				readingviewupz = false;
				readingviewfocal = true;
			}
			else if(readingviewupz)
			{
				viewupzString = viewupzString + c;
			}

			else if((readingviewfocal) && (c == CHAR_SPACE))
			{
				readingviewfocal = false;
				readingviewsizew = true;
			}
			else if(readingviewfocal)
			{
				viewfocalString = viewfocalString + c;
			}

			else if((readingviewsizew) && (c == CHAR_SPACE))
			{
				readingviewsizew = false;
				readingviewsizeh = true;
			}
			else if(readingviewsizew)
			{
				viewsizewString = viewsizewString + c;
			}
			else if((readingviewsizeh) && (c == CHAR_SPACE))
			{
				readingviewsizeh = false;
				readingscale = true;
			}
			else if(readingviewsizeh)
			{
				viewsizehString = viewsizehString + c;
			}

			else if((readingscale) && (c == CHAR_NEWLINE))
			{
				readingscale = false;
				finalise = true;
			}
			else if(readingscale)
			{
				scaleString = scaleString + c;
			}


			else if((readingxoffset) && (c == CHAR_SPACE))
			{
				readingxoffset = false;
				readingyoffset = true;
			}
			else if(readingxoffset)
			{
				xoffsetString = xoffsetString + c;
			}
			else if((readingyoffset) && (c == CHAR_NEWLINE))
			{
				readingyoffset = false;
				finalise = true;
			}
			else if(readingyoffset)
			{
				yoffsetString = yoffsetString + c;
			}

			else
			{
				//cout << "file i/o error" << endl;
				//exit(0);
			}

			if(finalise)
			{
				if(lineCount == multiViewViewIndex)
				{
					vi->objectName = objectNameString;
					vi->imageExtensionName = imageextString;
					vi->imageWidth = atof(imageWidthString.c_str());
					vi->imageHeight = atof(imageHeightString.c_str());

					if(dimension == OR_METHOD2DOD_DIMENSIONS)
					{
						vi->xOffset = atof(xoffsetString.c_str());
						vi->yOffset = atof(yoffsetString.c_str());

					}
					else if(dimension == OR_METHOD3DOD_DIMENSIONS)
					{
						vi->depthExtensionName = depthextString;
						vi->eye.x = atof(vieweyexString.c_str());
						vi->eye.y = atof(vieweyeyString.c_str());
						vi->eye.z = atof(vieweyezString.c_str());
						vi->viewAt.x = atof(viewatxString.c_str());
						vi->viewAt.y = atof(viewatyString.c_str());
						vi->viewAt.z = atof(viewatzString.c_str());
						vi->viewUp.x = atof(viewupxString.c_str());
						vi->viewUp.y = atof(viewupyString.c_str());
						vi->viewUp.z = atof(viewupzString.c_str());
						vi->focalLength = atof(viewfocalString.c_str());
						vi->viewWidth = atof(viewsizewString.c_str());
						vi->viewHeight = atof(viewsizehString.c_str());
						vi->depthScale = atof(scaleString.c_str());
					}
					else
					{
						cout << "illegal dimension" << endl;
						exit(0);
					}

					cout << "finalise:" << endl;
					cout << "vi->objectName = " << vi->objectName << endl;
					cout << "vi->imageExtensionName = " << vi->imageExtensionName << endl;
					cout << "vi->imageWidth = " << vi->imageWidth << endl;
					cout << "vi->imageHeight = " << vi->imageHeight << endl;
					cout << "vi->xOffset = " << vi->xOffset << endl;
					cout << "vi->yOffset = " << vi->yOffset << endl;
					cout << "vi->depthExtensionName = " << vi->depthExtensionName << endl;
					cout << "vi->eye.x = " << vi->eye.x << endl;
					cout << "vi->eye.y = " << vi->eye.y << endl;
					cout << "vi->eye.z = " << vi->eye.z << endl;
					cout << "vi->viewAt.x = " << vi->viewAt.x << endl;
					cout << "vi->viewAt.y = " << vi->viewAt.y << endl;
					cout << "vi->viewAt.z = " << vi->viewAt.z << endl;
					cout << "vi->viewUp.x = " << vi->viewUp.x << endl;
					cout << "vi->viewUp.y = " << vi->viewUp.y << endl;
					cout << "vi->viewUp.z = " << vi->viewUp.z << endl;
					cout << "vi->focalLength = " << vi->focalLength << endl;
					cout << "vi->viewWidth = " << vi->viewWidth << endl;
					cout << "vi->viewHeight = " << vi->viewHeight << endl;
					cout << "vi->depthScale = " << vi->depthScale << endl;
				}

				objectNameString = "";
				imageextString = "";
				imageWidthString = "";
				imageHeightString = "";

				depthextString = "";
				vieweyexString = "";
				vieweyeyString = "";
				vieweyezString = "";
				viewatxString = "";
				viewatyString = "";
				viewatzString = "";
				viewupxString = "";
				viewupyString = "";
				viewupzString = "";
				viewfocalString = "";
				viewsizewString = "";
				viewsizehString = "";
				scaleString = "";

				xoffsetString = "";
				yoffsetString = "";

				lineCount++;
				readingObjectName = true;
				finalise = false;
			}
		}

		parseFileObject.close();
	}
	else
	{
		cout << "error opening file, " << fileName << endl;
	}

	return lineCount;
}







