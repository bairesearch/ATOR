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
 * f
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ORdatabaseFileIO.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3f7a 16-August-2015
 *
 *******************************************************************************/


#include "ORdatabaseFileIO.h"
#include "ORdatabaseSQL.h"	//required for determineGeoBinX()/determineGeoBinY() only ... - these functions and/or compareFeaturesListForMatch() should probably be moved elsewhere instead, say to "ORdatabaseOperations.cpp"
#include "SHAREDvector.h"
#include "LDreferenceManipulation.h"

#ifdef LINUX
#include <sys/stat.h>
#else
//#include <dirent.h>
#include <windows.h>
#endif

#ifdef OR_USE_DATABASE

static string databaseFolderName;

void initialiseDatabase(string newDatabaseFolderName)
{
	databaseFolderName = newDatabaseFolderName;
}

bool DBdirectoryExists(string* folderName)
{
	bool folderExists = directoryExists(folderName);
	if(folderExists)
	{
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "\tdirectoryExists: folderName = " <<* folderName << endl;
		#endif
	}

	return folderExists;
}

bool DBcreateDirectory(string* folderName)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "\tDBcreateDirectory: folderName = " <<* folderName << endl;
	#endif
	bool result = true;

	createDirectory(folderName);
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

bool DBsetCurrentDirectory(string* folderName)
{
	bool result = true;
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "\tDBsetCurrentDirectory: folderName = " <<* folderName << endl;
	#endif
	setCurrentDirectory(folderName);
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

bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string* folderName)
{
	bool result = true;
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "checkIfFolderExistsAndIfNotMakeAndSetAsCurrent: folderName = " <<* folderName << endl;
	#endif
	if(!DBdirectoryExists(folderName))
	{
		DBcreateDirectory(folderName);
	}
	DBsetCurrentDirectory(folderName);

	return result;
}

string DBgenerateServerDatabaseName(string* objectName, bool trainOrTest)
{
	string databaseName;
	if(!trainOrTest)
	{
		databaseName = databaseFolderName;
	}
	else
	{
		#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "entityName = " <<* entityName << endl;
		#endif

		#ifdef GIA_DATABASE_FILESYSTEM_USE_MULTIPLE_SERVERS
		char entityFirstCharacter = entityName->at(0);
		if((entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z))
		{
			cout << "determineServerName error: (entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z)" << endl;
			cout << "entityName = " <<* entityName << endl;
			exit(0);
		}

		int entityFirstCharacterIndex = entityFirstCharacter - ASCII_TABLE_INDEX_OF_a;
		#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "entityFirstCharacterIndex = " << entityFirstCharacterIndex << endl;
		#endif
		string serverName = serverNameArray[entityFirstCharacterIndex]; 	//this could be a more complex algorithm; eg serverName = (string)"/mnt/" + serverNameArray[entityFirstCharacterIndex]
		databaseName =  serverName + OR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
		#else
		databaseName = databaseFolderName;
		#endif
	}
	#ifdef OR_DATABASE_DEBUG
	cout << "databaseName = " << databaseName << endl;
	#endif

	return databaseName;
}


string DBgenerateFolderName(string* objectName, bool trainOrTest)
{
	//eg network/server/ORdatabase/e/x/a/example/...

	string databaseName = DBgenerateServerDatabaseName(objectName, trainOrTest);
	string fileName = databaseName;

	#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "1fileName = " << fileName << endl;
	#endif
	DBsetCurrentDirectory(&fileName);

	if(!trainOrTest)
	{
		fileName = fileName + OR_DATABASE_TEST_FOLDER_NAME + "/";
		string testFolderName = OR_DATABASE_TEST_FOLDER_NAME;
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&testFolderName);
	}
	else
	{
		#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "*objectName = " <<* objectName << endl;
		#endif

		fileName = fileName + OR_DATABASE_TRAIN_FOLDER_NAME + "/";
		string trainFolderName = OR_DATABASE_TRAIN_FOLDER_NAME;
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&trainFolderName);

		int numberOfEntityNameLevels;
		if(objectName->length() < OR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS)
		{
			numberOfEntityNameLevels = objectName->length();
		}
		else
		{
			numberOfEntityNameLevels = OR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS;
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

		#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "2fileName = " << fileName << endl;
		#endif
	}

	#ifdef OR_DATABASE_DEBUG
	cout << "fileName = " << fileName << endl;
	#endif

	return fileName;
}

#endif


#ifdef OR_METHOD_GEOMETRIC_COMPARISON
bool compareFeaturesListForMatch(ORfeature* testFirstFeatureInNearestFeatureList, ORfeature* trainFirstFeatureInNearestFeatureList, int dimension, bool* exactMatchFound)
{
	int numberOfFeatureGeoMatches = 0;
	int numberOfFeatureGeoBinnedExactMatches = 0;
	bool passedGeometricCheck = false;
	ORfeature* testcurrentFeatureInNearestFeatureList = testFirstFeatureInNearestFeatureList;
	while((testcurrentFeatureInNearestFeatureList->next != NULL) && !(testcurrentFeatureInNearestFeatureList->lastFilledFeatureInList))
	{
		bool testpassedDimensionCheck = true;
		if(dimension == OR_METHOD2DOD_DIMENSIONS)
		{//NB do not compare transformed object triangle features if OR_METHOD2DOD_DIMENSIONS, as these will always be set to a predefined object triangle
			if(testcurrentFeatureInNearestFeatureList->OTpointIndex != 0)
			{
				testpassedDimensionCheck = false;
			}
		}

		if(testpassedDimensionCheck)
		{
			ORfeature* traincurrentFeatureInNearestFeatureList = trainFirstFeatureInNearestFeatureList;
			while((traincurrentFeatureInNearestFeatureList->next != NULL) && !(traincurrentFeatureInNearestFeatureList->lastFilledFeatureInList))
			{
				bool trainpassedDimensionCheck = true;
				if(dimension == OR_METHOD2DOD_DIMENSIONS)
				{//NB do not compare transformed object triangle features if OR_METHOD2DOD_DIMENSIONS, as these will always be set to a predefined object triangle
					if(traincurrentFeatureInNearestFeatureList->OTpointIndex != 0)
					{
						trainpassedDimensionCheck = false;
					}
				}
				if(trainpassedDimensionCheck)
				{
					double requiredMaxError;
					#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
						double minwidthheightOfOriginalTriangleTestAndTrain = minDouble(testcurrentFeatureInNearestFeatureList->minWidthAndHeightOfOrigOT, traincurrentFeatureInNearestFeatureList->minWidthAndHeightOfOrigOT);
						requiredMaxError = (1.0/(maxDouble((minwidthheightOfOriginalTriangleTestAndTrain - OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT), OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT)/(double)OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT))* OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
					#else
						requiredMaxError = OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
					#endif

					if(compareVectorsArbitraryError(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(traincurrentFeatureInNearestFeatureList->pointTransformed), requiredMaxError))
					{
						numberOfFeatureGeoMatches++;
						#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
						testcurrentFeatureInNearestFeatureList->matchFound = true;
						traincurrentFeatureInNearestFeatureList->matchFound = true;
						#endif
					}

					int trainxBin = determineGeoBinX(traincurrentFeatureInNearestFeatureList->pointTransformed.x);
					int trainyBin = determineGeoBinY(traincurrentFeatureInNearestFeatureList->pointTransformed.y);
					int testxBin = determineGeoBinX(testcurrentFeatureInNearestFeatureList->pointTransformed.x);
					int testyBin = determineGeoBinY(testcurrentFeatureInNearestFeatureList->pointTransformed.y);
					#ifdef OR_DEBUG
					//cout << "\ntrainxBin = " << trainxBin << endl;
					//cout << "trainyBin = " << trainyBin << endl;
					//cout << "testxBin = " << testxBin << endl;
					//cout << "testyBin = " << testyBin << endl;
					//cout << testxBin << "\t" << testyBin << "\t" << trainxBin << "\t" << trainyBin << endl;
					#endif
					if((trainxBin == testxBin) && (trainyBin == testyBin))
					{
						numberOfFeatureGeoBinnedExactMatches++;
						#ifdef OR_DEBUG
						//cout << "\tnumberOfFeatureGeoBinnedExactMatches = " << numberOfFeatureGeoBinnedExactMatches << endl;
						#endif
					}
				}

				traincurrentFeatureInNearestFeatureList = traincurrentFeatureInNearestFeatureList->next;
			}
		}

		testcurrentFeatureInNearestFeatureList = testcurrentFeatureInNearestFeatureList->next;
	}

	if(numberOfFeatureGeoMatches >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES)
	{
		passedGeometricCheck = true;
	}
	if(numberOfFeatureGeoBinnedExactMatches >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES)
	{
		*exactMatchFound = true;
	}
	return passedGeometricCheck;

}


void addFeatureToEndOfFeatureList(ORfeature* firstFeatureInList, ORfeature* featureToAdd)
{
	ORfeature* currentFeatureInList = firstFeatureInList;

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
	#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	currentFeatureInList->OTpointIndex = featureToAdd->OTpointIndex;
	#endif
	copyVectors(&(currentFeatureInList->pointTransformed), &(featureToAdd->pointTransformed));

	ORfeature* newFeature = new ORfeature();
	currentFeatureInList->next = newFeature;

}




void createFeaturesListUsingFeaturesFile(string fileName, ORfeature* firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures)
{
	ORfeature* currentFeatureInList = firstFeatureInList;

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
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		bool readingOTIndex = false;
		#endif
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		bool readingminWidthAndHeightOfOrigOT = false;
		#endif
		bool readingtransformedPointX = false;
		bool readingtransformedPointY = false;
		bool readingtransformedPointZ = false;
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
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
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		string OTpointIndexString = "";
		#endif
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		string minWidthAndHeightOfOrigOTString = "";
		#endif
		string transformedPointXString = "";
		string transformedPointYString = "";
		string transformedPointZString = "";
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
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
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
				readingOTIndex = true;
			#else
				readingtransformedPointX = true;
			#endif
			}
			else if(readingSideIndex)
			{
				sideIndexString = sideIndexString + c;
			}
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			else if((readingOTIndex) && (c == CHAR_SPACE))
			{
				readingOTIndex = false;
				#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
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
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
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
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
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

				if(!ignoreOTfeatures || ((int)(convertStringToDouble(OTpointIndexString)) == 0))
				{
					currentFeatureInList->objectName = objectNameString;
					currentFeatureInList->trainOrTest = (bool)(convertStringToDouble(trainOrTestString));
					currentFeatureInList->viewIndex = (int)(convertStringToDouble(viewIndexString));
					currentFeatureInList->zoomIndex = (int)(convertStringToDouble(zoomIndexString));
					currentFeatureInList->polyIndex = (int)(convertStringToDouble(polyIndexString));
					currentFeatureInList->sideIndex = (int)(convertStringToDouble(sideIndexString));
					#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
					currentFeatureInList->OTpointIndex = (int)(convertStringToDouble(OTpointIndexString));
					#endif
					#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
					currentFeatureInList->minWidthAndHeightOfOrigOT = (int)(convertStringToDouble(minWidthAndHeightOfOrigOTString));
					#endif
					currentFeatureInList->pointTransformed.x = (convertStringToDouble(transformedPointXString));
					currentFeatureInList->pointTransformed.y = (convertStringToDouble(transformedPointYString));
					currentFeatureInList->pointTransformed.z = (convertStringToDouble(transformedPointZString));
					#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
					currentFeatureInList->point.x = (convertStringToDouble(PointXString));
					currentFeatureInList->point.y = (convertStringToDouble(PointYString));
					currentFeatureInList->point.z = (convertStringToDouble(PointZString));
					#endif


					#ifdef OR_DEBUG
					/*
					cout << "\tfeature added to list;" << endl;
					cout << "objectNameString = " << objectNameString << endl;
					cout << "trainOrTestString = " << trainOrTestString << endl;
					cout << "viewIndexString = " << viewIndexString << endl;
					cout << "zoomIndexString = " << zoomIndexString << endl;
					cout << "polyIndexString = " << polyIndexString << endl;
					cout << "sideIndexString = " << sideIndexString << endl;
					cout << "OTpointIndexString = " << OTpointIndexString << endl;
					cout << "transformedPointXString = " << transformedPointXString << endl;
					cout << "transformedPointYString = " << transformedPointYString << endl;
					cout << "transformedPointZString = " << transformedPointZString << endl;
					cout << "currentFeatureInList->objectName = " << currentFeatureInList->objectName << endl;
					cout << "currentFeatureInList->trainOrTest = " << currentFeatureInList->trainOrTest << endl;
					cout << "currentFeatureInList->viewIndex = " << currentFeatureInList->viewIndex << endl;
					cout << "currentFeatureInList->polyIndex = " << currentFeatureInList->polyIndex << endl;
					cout << "currentFeatureInList->sideIndex = " << currentFeatureInList->sideIndex << endl;
					cout << "currentFeatureInList->pointTransformed.x = " << currentFeatureInList->pointTransformed.x << endl;
					cout << "currentFeatureInList->pointTransformed.y = " << currentFeatureInList->pointTransformed.y << endl;
					cout << "currentFeatureInList->pointTransformed.z = " << currentFeatureInList->pointTransformed.z << endl;
					#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
					cout << "currentFeatureInList->OTpointIndex = " << currentFeatureInList->OTpointIndex << endl;
					#endif
					*/
					#endif

					#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
					currentFeatureInList->matchFound = false;
					#endif

					if(createFeatureObjects)
					{
						ORfeature* newFeature = new ORfeature();
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
				#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
				OTpointIndexString = "";
				#endif
				#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
				minWidthAndHeightOfOrigOTString = "";
				#endif
				transformedPointXString = "";
				transformedPointYString = "";
				transformedPointZString = "";
				#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
				PointXString = "";
				PointYString = "";
				PointZString = "";
				#endif

				lineCount++;
				#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
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
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
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




void createTransformedFeaturesFile(ORfeature* firstFeatureInList, string fileName, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest)
{
	ofstream writeFileObject(fileName.c_str());

	ORfeature* currentFeature = firstFeatureInList;
	while(currentFeature->next != NULL)
	{
		string polygonIndexString = convertIntToString(polyIndex);
		string sideIndexString = convertIntToString(sideIndex);
		string viewIndexString = convertIntToString(viewIndex);
		string zoomIndexString = convertIntToString(zoomIndex);
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		string OTpointIndexString = convertIntToString(currentFeature->OTpointIndex);
		#endif
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		string minWidthAndHeightOfOrigOTString = convertIntToString(currentFeature->minWidthAndHeightOfOrigOT);
		#endif
		string trainOrTestString = convertIntToString(((int)trainOrTest));
		string transformedpositionCoordinatesString = convertPositionCoordinatesToString(&(currentFeature->pointTransformed));
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
		string positionCoordinatesString = convertPositionCoordinatesToString(&(currentFeature->point));
		#endif

		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		string lineText = objectName + " " + trainOrTestString + " " + viewIndexString + " " + zoomIndexString + " " + polygonIndexString + " " + sideIndexString + " " + OTpointIndexString + " " + minWidthAndHeightOfOrigOTString + " " + transformedpositionCoordinatesString;
		#else
		string lineText = objectName + " " + trainOrTestString + " " + viewIndexString + " " + zoomIndexString + " " + polygonIndexString + " " + sideIndexString + " " + OTpointIndexString + " " + transformedpositionCoordinatesString;
		#endif
		#else
		string lineText = objectName + " " + trainOrTestString + " " + viewIndexString + " " + zoomIndexString + " " + polygonIndexString + " " + sideIndexString + " " + transformedpositionCoordinatesString;
		#endif

		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
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

