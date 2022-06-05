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
 * File Name: ATORdatabaseFileIO.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3p5a 14-March-2022
 * /
 *******************************************************************************/


#include "ATORdatabaseFileIO.hpp"

#ifdef LINUX
#include <sys/stat.h>
#else
//#include <dirent.h>
#include <windows.h>
#endif

#ifdef ATOR_USE_DATABASE

static string databaseFolderName;

void ATORdatabaseFileIOClass::initialiseDatabase(const string newDatabaseFolderName)
{
	databaseFolderName = newDatabaseFolderName;
}

bool ATORdatabaseFileIOClass::DBdirectoryExists(string* folderName)
{
	bool folderExists = SHAREDvars.directoryExists(folderName);
	if(folderExists)
	{
	}

	return folderExists;
}

bool ATORdatabaseFileIOClass::DBcreateDirectory(string* folderName)
{
	bool result = true;

	SHAREDvars.createDirectory(folderName);
	/*removed debug support for Windows;
	#ifndef LINUX
	if(CreateDirectory(folderName->c_str(), 0) == 0)	//if( _mkdir(folderName->c_str()) != 0)	//
	{
		result = false;
	}
	#endif
	*/

	return result;
}

bool ATORdatabaseFileIOClass::DBsetCurrentDirectory(string* folderName)
{
	bool result = true;
	SHAREDvars.setCurrentDirectory(folderName);
	/*removed debug support for Windows;
	#ifndef LINUX
	if(SetCurrentDirectory(folderName) == 0)
	{
		result = false;
	}
	#endif
	*/
	return result;
}

bool ATORdatabaseFileIOClass::checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string* folderName)
{
	bool result = true;
	if(!DBdirectoryExists(folderName))
	{
		DBcreateDirectory(folderName);
	}
	DBsetCurrentDirectory(folderName);

	return result;
}

string ATORdatabaseFileIOClass::DBgenerateServerDatabaseName(const string* objectName, const bool trainOrTest)
{
	string databaseName;
	if(!trainOrTest)
	{
		databaseName = databaseFolderName;
	}
	else
	{
		#ifdef ATOR_DATABASE_VERBOSE_FILESYSTEM_IO
		//cout << "entityName = " <<* entityName << endl;
		#endif

		#ifdef GIA_DATABASE_FILESYSTEM_USE_MULTIPLE_SERVERS
		char entityFirstCharacter = entityName->at(0);
		if((entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z))
		{
			cerr << "determineServerName error: (entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z)" << endl;
			cerr << "entityName = " <<* entityName << endl;
			exit(EXIT_ERROR);
		}

		int entityFirstCharacterIndex = entityFirstCharacter - ASCII_TABLE_INDEX_OF_a;
		#ifdef ATOR_DATABASE_VERBOSE_FILESYSTEM_IO
		//cout << "entityFirstCharacterIndex = " << entityFirstCharacterIndex << endl;
		#endif
		string serverName = serverNameArray[entityFirstCharacterIndex]; 	//this could be a more complex algorithm; eg serverName = (string)"/mnt/" + serverNameArray[entityFirstCharacterIndex]
		databaseName =  serverName + ATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
		#else
		databaseName = databaseFolderName;
		#endif
	}
	#ifdef ATOR_DATABASE_VERBOSE
	cout << "databaseName = " << databaseName << endl;
	#endif

	return databaseName;
}


string ATORdatabaseFileIOClass::DBgenerateFolderName(string* objectName, const bool trainOrTest)
{
	//eg network/server/ORdatabase/e/x/a/example/...

	string databaseName = DBgenerateServerDatabaseName(objectName, trainOrTest);
	string fileName = databaseName;

	#ifdef ATOR_DATABASE_VERBOSE_FILESYSTEM_IO
	cout << "1fileName = " << fileName << endl;
	#endif
	DBsetCurrentDirectory(&fileName);

	if(!trainOrTest)
	{
		fileName = fileName + ATOR_DATABASE_TEST_FOLDER_NAME + "/";
		string testFolderName = ATOR_DATABASE_TEST_FOLDER_NAME;
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&testFolderName);
	}
	else
	{
		#ifdef ATOR_DATABASE_VERBOSE_FILESYSTEM_IO
		cout << "*objectName = " <<* objectName << endl;
		#endif

		fileName = fileName + ATOR_DATABASE_TRAIN_FOLDER_NAME + "/";
		string trainFolderName = ATOR_DATABASE_TRAIN_FOLDER_NAME;
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&trainFolderName);

		int numberOfEntityNameLevels;
		if(objectName->length() < ATOR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)
		{
			numberOfEntityNameLevels = objectName->length();
		}
		else
		{
			numberOfEntityNameLevels = ATOR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS;
		}
		for(int level=0; level<numberOfEntityNameLevels; level++)
		{
			string folderName = "";
			folderName = folderName + objectName->at(level);
			fileName = fileName + folderName + "/";
			checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&folderName);
		}
		fileName = fileName +* objectName + "/";
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(objectName);

		#ifdef ATOR_DATABASE_VERBOSE_FILESYSTEM_IO
		cout << "2fileName = " << fileName << endl;
		#endif
	}

	#ifdef ATOR_DATABASE_VERBOSE
	cout << "fileName = " << fileName << endl;
	#endif

	return fileName;
}

#endif


#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
bool ATORdatabaseFileIOClass::compareFeaturesListForMatch(ATORfeature* testFirstFeatureInNearestFeatureList, ATORfeature* trainFirstFeatureInNearestFeatureList, const int dimension, bool* exactMatchFound)
{
	int numberOfFeatureGeoMatches = 0;
	int numberOfFeatureGeoBinnedExactMatches = 0;
	bool passedGeometricCheck = false;
	ATORfeature* testcurrentFeatureInNearestFeatureList = testFirstFeatureInNearestFeatureList;
	while((testcurrentFeatureInNearestFeatureList->next != NULL) && !(testcurrentFeatureInNearestFeatureList->lastFilledFeatureInList))
	{
		bool testpassedDimensionCheck = true;
		if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
		{//NB do not compare transformed object triangle features if ATOR_METHOD2DOD_DIMENSIONS, as these will always be set to a predefined object triangle
			if(testcurrentFeatureInNearestFeatureList->OTpointIndex != 0)
			{
				testpassedDimensionCheck = false;
			}
		}

		if(testpassedDimensionCheck)
		{
			ATORfeature* traincurrentFeatureInNearestFeatureList = trainFirstFeatureInNearestFeatureList;
			while((traincurrentFeatureInNearestFeatureList->next != NULL) && !(traincurrentFeatureInNearestFeatureList->lastFilledFeatureInList))
			{
				bool trainpassedDimensionCheck = true;
				if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
				{//NB do not compare transformed object triangle features if ATOR_METHOD2DOD_DIMENSIONS, as these will always be set to a predefined object triangle
					if(traincurrentFeatureInNearestFeatureList->OTpointIndex != 0)
					{
						trainpassedDimensionCheck = false;
					}
				}
				if(trainpassedDimensionCheck)
				{
					double requiredMaxError;
					#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
						double minwidthheightOfOriginalTriangleTestAndTrain = SHAREDvars.minDouble(testcurrentFeatureInNearestFeatureList->minWidthAndHeightOfOrigOT, traincurrentFeatureInNearestFeatureList->minWidthAndHeightOfOrigOT);
						requiredMaxError = (1.0/(SHAREDvars.maxDouble((minwidthheightOfOriginalTriangleTestAndTrain - ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT), ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT)/(double)ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT))* ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
					#else
						requiredMaxError = ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
					#endif

					if(SHAREDvector.compareVectorsArbitraryError(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(traincurrentFeatureInNearestFeatureList->pointTransformed), requiredMaxError))
					{
						numberOfFeatureGeoMatches++;
						#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES
						testcurrentFeatureInNearestFeatureList->matchFound = true;
						traincurrentFeatureInNearestFeatureList->matchFound = true;
						#endif
					}

					int trainxBin = ATORdatabaseSQL.determineGeoBinX(traincurrentFeatureInNearestFeatureList->pointTransformed.x);
					int trainyBin = ATORdatabaseSQL.determineGeoBinY(traincurrentFeatureInNearestFeatureList->pointTransformed.y);
					int testxBin = ATORdatabaseSQL.determineGeoBinX(testcurrentFeatureInNearestFeatureList->pointTransformed.x);
					int testyBin = ATORdatabaseSQL.determineGeoBinY(testcurrentFeatureInNearestFeatureList->pointTransformed.y);
					if((trainxBin == testxBin) && (trainyBin == testyBin))
					{
						numberOfFeatureGeoBinnedExactMatches++;
					}
				}

				traincurrentFeatureInNearestFeatureList = traincurrentFeatureInNearestFeatureList->next;
			}
		}

		testcurrentFeatureInNearestFeatureList = testcurrentFeatureInNearestFeatureList->next;
	}

	if(numberOfFeatureGeoMatches >= ATOR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES)
	{
		passedGeometricCheck = true;
	}
	if(numberOfFeatureGeoBinnedExactMatches >= ATOR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES)
	{
		*exactMatchFound = true;
	}
	return passedGeometricCheck;

}


void ATORdatabaseFileIOClass::addFeatureToEndOfFeatureList(ATORfeature* firstFeatureInList, ATORfeature* featureToAdd)
{
	ATORfeature* currentFeatureInList = firstFeatureInList;

	// go to last feature in list (ie append to list if list already has items)
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}
	currentFeatureInList->objectName = featureToAdd->objectName;
	currentFeatureInList->trainOrTest = featureToAdd->trainOrTest;
	currentFeatureInList->viewIndex = featureToAdd->viewIndex;
	currentFeatureInList->zoomIndex = featureToAdd->zoomIndex;
	currentFeatureInList->polyIndex = featureToAdd->polyIndex;
	currentFeatureInList->sideIndex = featureToAdd->sideIndex;
	#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	currentFeatureInList->OTpointIndex = featureToAdd->OTpointIndex;
	#endif
	SHAREDvector.copyVectors(&(currentFeatureInList->pointTransformed), &(featureToAdd->pointTransformed));

	ATORfeature* newFeature = new ATORfeature();
	currentFeatureInList->next = newFeature;

}




void ATORdatabaseFileIOClass::createFeaturesListUsingFeaturesFile(const string fileName, ATORfeature* firstFeatureInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures)
{
	ATORfeature* currentFeatureInList = firstFeatureInList;

	if(appendToList)
	{//if creatingFeatureObjects, go to last feature in list (ie append to list if list already has items)
		while(currentFeatureInList->next != NULL)
		{
			currentFeatureInList = currentFeatureInList->next;
		}
	}

	ifstream parseFileObject(fileName.c_str());

	if(parseFileObject.rdbuf()->is_open( ))
	{
		char c;
		int charCount = 0;
		int lineCount = 0;
		bool waitingForNewLine = false;
		bool readingObjectName = true;
		bool readingTestOrTrainIndex = false;
		bool readingViewIndex = false;
		bool readingZoomIndex = false;
		bool readingPolyIndex = false;
		bool readingSideIndex = false;
		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		bool readingOTIndex = false;
		#endif
		#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		bool readingminWidthAndHeightOfOrigOT = false;
		#endif
		bool readingtransformedPointX = false;
		bool readingtransformedPointY = false;
		bool readingtransformedPointZ = false;
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
		bool readingPointX = false;
		bool readingPointY = false;
		bool readingPointZ = false;
		#endif

		string objectNameString = "";
		string trainOrTestString = "";
		string viewIndexString = "";
		string zoomIndexString = "";
		string polyIndexString = "";
		string sideIndexString = "";
		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		string OTpointIndexString = "";
		#endif
		#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		string minWidthAndHeightOfOrigOTString = "";
		#endif
		string transformedPointXString = "";
		string transformedPointYString = "";
		string transformedPointZString = "";
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
		string PointXString = "";
		string PointYString = "";
		string PointZString = "";
		#endif

		while(parseFileObject.get(c))
		{
			charCount++;

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
				readingTestOrTrainIndex = true;
			}
			else if(readingObjectName)
			{
				objectNameString = objectNameString + c;
			}
			else if((readingTestOrTrainIndex) && (c == CHAR_SPACE))
			{
				readingTestOrTrainIndex = false;
				readingViewIndex = true;
			}
			else if(readingTestOrTrainIndex)
			{
				trainOrTestString = trainOrTestString + c;
			}
			else if((readingViewIndex) && (c == CHAR_SPACE))
			{
				readingViewIndex = false;
				readingZoomIndex = true;
			}
			else if(readingViewIndex)
			{
				viewIndexString = viewIndexString + c;
			}
			else if((readingZoomIndex) && (c == CHAR_SPACE))
			{
				readingZoomIndex = false;
				readingPolyIndex = true;
			}
			else if(readingZoomIndex)
			{
				zoomIndexString = zoomIndexString + c;
			}
			else if((readingPolyIndex) && (c == CHAR_SPACE))
			{
				readingPolyIndex = false;
				readingSideIndex = true;
			}
			else if(readingPolyIndex)
			{
				polyIndexString = polyIndexString + c;
			}
			else if((readingSideIndex) && (c == CHAR_SPACE))
			{
				readingSideIndex = false;
			#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
				readingOTIndex = true;
			#else
				readingtransformedPointX = true;
			#endif
			}
			else if(readingSideIndex)
			{
				sideIndexString = sideIndexString + c;
			}
		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			else if((readingOTIndex) && (c == CHAR_SPACE))
			{
				readingOTIndex = false;
				#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
				readingminWidthAndHeightOfOrigOT = true;
				#else
				readingtransformedPointX = true;
				#endif
			}
			else if(readingOTIndex)
			{
				OTpointIndexString = OTpointIndexString + c;
			}
		#endif
		#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
			else if((readingminWidthAndHeightOfOrigOT) && (c == CHAR_SPACE))
			{
				readingminWidthAndHeightOfOrigOT = false;
				readingtransformedPointX = true;
			}
			else if(readingminWidthAndHeightOfOrigOT)
			{
				minWidthAndHeightOfOrigOTString = minWidthAndHeightOfOrigOTString + c;
			}
		#endif
			else if((readingtransformedPointX) && (c == CHAR_SPACE))
			{
				readingtransformedPointX = false;
				readingtransformedPointY = true;
			}
			else if(readingtransformedPointX)
			{
				transformedPointXString = transformedPointXString + c;
			}
			else if((readingtransformedPointY) && (c == CHAR_SPACE))
			{
				readingtransformedPointY = false;
				readingtransformedPointZ = true;
			}
			else if(readingtransformedPointY)
			{
				transformedPointYString = transformedPointYString + c;
			}
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
			else if((readingtransformedPointZ) && (c == CHAR_SPACE))
			{
				readingtransformedPointZ = false;
				readingPointX = true;
			}
			else if((readingPointZ) && (c == CHAR_NEWLINE))
		#else
			else if((readingtransformedPointZ) && (c == CHAR_NEWLINE))
		#endif
			{

				if(!ignoreOTfeatures || ((int)(SHAREDvars.convertStringToDouble(OTpointIndexString)) == 0))
				{
					currentFeatureInList->objectName = objectNameString;
					currentFeatureInList->trainOrTest = (bool)(SHAREDvars.convertStringToDouble(trainOrTestString));
					currentFeatureInList->viewIndex = (int)(SHAREDvars.convertStringToDouble(viewIndexString));
					currentFeatureInList->zoomIndex = (int)(SHAREDvars.convertStringToDouble(zoomIndexString));
					currentFeatureInList->polyIndex = (int)(SHAREDvars.convertStringToDouble(polyIndexString));
					currentFeatureInList->sideIndex = (int)(SHAREDvars.convertStringToDouble(sideIndexString));
					#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
					currentFeatureInList->OTpointIndex = (int)(SHAREDvars.convertStringToDouble(OTpointIndexString));
					#endif
					#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
					currentFeatureInList->minWidthAndHeightOfOrigOT = (int)(SHAREDvars.convertStringToDouble(minWidthAndHeightOfOrigOTString));
					#endif
					currentFeatureInList->pointTransformed.x = (SHAREDvars.convertStringToDouble(transformedPointXString));
					currentFeatureInList->pointTransformed.y = (SHAREDvars.convertStringToDouble(transformedPointYString));
					currentFeatureInList->pointTransformed.z = (SHAREDvars.convertStringToDouble(transformedPointZString));
					#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
					currentFeatureInList->point.x = (SHAREDvars.convertStringToDouble(PointXString));
					currentFeatureInList->point.y = (SHAREDvars.convertStringToDouble(PointYString));
					currentFeatureInList->point.z = (SHAREDvars.convertStringToDouble(PointZString));
					#endif



					#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES
					currentFeatureInList->matchFound = false;
					#endif

					if(createFeatureObjects)
					{
						ATORfeature* newFeature = new ATORfeature();
						currentFeatureInList->next = newFeature;
					}

					currentFeatureInList->lastFilledFeatureInList = false;		//added 8 June 2012 to combat overrun bug
					currentFeatureInList->next->lastFilledFeatureInList = true;		//added 8 June 2012 to combat overrun bug

					currentFeatureInList=currentFeatureInList->next;
				}
				objectNameString = "";
				trainOrTestString = "";
				viewIndexString = "";
				zoomIndexString = "";
				polyIndexString = "";
				sideIndexString = "";
				#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
				OTpointIndexString = "";
				#endif
				#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
				minWidthAndHeightOfOrigOTString = "";
				#endif
				transformedPointXString = "";
				transformedPointYString = "";
				transformedPointZString = "";
				#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
				PointXString = "";
				PointYString = "";
				PointZString = "";
				#endif

				lineCount++;
				#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
				readingPointZ = false;
				#else
				readingtransformedPointZ = false;
				#endif
				readingObjectName = true;
			}
			else if(readingtransformedPointZ)
			{
				transformedPointZString = transformedPointZString + c;
			}
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
			else if((readingPointX) && (c == CHAR_SPACE))
			{
				readingPointX = false;
				readingPointY = true;
			}
			else if(readingPointX)
			{
				PointXString = PointXString + c;
			}
			else if((readingPointY) && (c == CHAR_SPACE))
			{
				readingPointY = false;
				readingPointZ = true;
			}
			else if(readingPointY)
			{
				PointYString = PointYString + c;
			}
			else if(readingPointZ)
			{
				PointZString = PointZString + c;
			}
		#endif
			else
			{

			}
		}

		parseFileObject.close();
	}
	else
	{
		cout << "error opening file, " << fileName << endl;
	}
}




void ATORdatabaseFileIOClass::createTransformedFeaturesFile(const ATORfeature* firstFeatureInList, const string fileName, const string objectName, const int viewIndex, const int zoomIndex, const int polyIndex, const int sideIndex, const int trainOrTest)
{
	ofstream writeFileObject(fileName.c_str());

	const ATORfeature* currentFeature = firstFeatureInList;
	while(currentFeature->next != NULL)
	{
		string polygonIndexString = SHAREDvars.convertIntToString(polyIndex);
		string sideIndexString = SHAREDvars.convertIntToString(sideIndex);
		string viewIndexString = SHAREDvars.convertIntToString(viewIndex);
		string zoomIndexString = SHAREDvars.convertIntToString(zoomIndex);
		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		string OTpointIndexString = SHAREDvars.convertIntToString(currentFeature->OTpointIndex);
		#endif
		#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		string minWidthAndHeightOfOrigOTString = SHAREDvars.convertIntToString(currentFeature->minWidthAndHeightOfOrigOT);
		#endif
		string trainOrTestString = SHAREDvars.convertIntToString(((int)trainOrTest));
		string transformedpositionCoordinatesString = LDreferenceManipulation.convertPositionCoordinatesToString(&(currentFeature->pointTransformed));
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
		string positionCoordinatesString = LDreferenceManipulation.convertPositionCoordinatesToString(&(currentFeature->point));
		#endif

		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		string lineText = objectName + " " + trainOrTestString + " " + viewIndexString + " " + zoomIndexString + " " + polygonIndexString + " " + sideIndexString + " " + OTpointIndexString + " " + minWidthAndHeightOfOrigOTString + " " + transformedpositionCoordinatesString;
		#else
		string lineText = objectName + " " + trainOrTestString + " " + viewIndexString + " " + zoomIndexString + " " + polygonIndexString + " " + sideIndexString + " " + OTpointIndexString + " " + transformedpositionCoordinatesString;
		#endif
		#else
		string lineText = objectName + " " + trainOrTestString + " " + viewIndexString + " " + zoomIndexString + " " + polygonIndexString + " " + sideIndexString + " " + transformedpositionCoordinatesString;
		#endif

		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
		lineText = lineText + positionCoordinatesString;
		#endif

		for(int i = 0; i<lineText.length(); i++)
		{
			writeFileObject.put(lineText[i]);
		}
		writeFileObject.put(CHAR_NEWLINE);

		currentFeature = currentFeature->next;
	}

	writeFileObject.close();
}
#endif

