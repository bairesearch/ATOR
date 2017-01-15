/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ORdatabaseFileIO.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3c5a 15-December-2012
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

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;

#ifdef OR_USE_DATABASE

static string databaseFolderName;

void initialiseDatabase(string newDatabaseFolderName)
{
	databaseFolderName = newDatabaseFolderName;
}

bool directoryExists(string * folderName)
{
	bool folderExists = false;

	#ifdef LINUX
	struct stat st;
	if(stat(folderName->c_str(), &st) == 0)
	{
		#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "\tdirectoryExists: folderName = " << *folderName << endl;
		#endif
		folderExists = true;
	}
	#else
	DWORD ftyp = GetFileAttributes(folderName->c_str());
	if(ftyp != INVALID_FILE_ATTRIBUTES)
	{
		if(ftyp & FILE_ATTRIBUTE_DIRECTORY)
		{
			folderExists = true;
		}
	}
	/*
	if((GetFileAttributes(folderName->c_str())) != INVALID_FILE_ATTRIBUTES)
	{
		folderExists = true;
	}
	*/
	#endif

	return folderExists;
}

bool makeDirectory(string * folderName)
{
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "\tmakeDirectory: folderName = " << *folderName << endl;
	#endif
	bool result = true;

	#ifdef LINUX
	mkdir(folderName->c_str(), 0755);
	#else
	if(CreateDirectory(folderName->c_str(), 0) == 0)	//if( _mkdir(folderName->c_str()) != 0)	//
	{
		result = false;
	}
	#endif

	return result;
}

bool setCurrentDirectory(string * folderName)
{
	bool result = true;
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "\tsetCurrentDirectory: folderName = " << *folderName << endl;
	#endif
	#ifdef LINUX
	chdir(folderName->c_str());
	#else
	if(SetCurrentDirectory(folderName->c_str()) == 0)
	{
		result = false;
	}
	#endif
	return result;
}

bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string * folderName)
{
	bool result = true;
	#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "checkIfFolderExistsAndIfNotMakeAndSetAsCurrent: folderName = " << *folderName << endl;
	#endif
	if(!directoryExists(folderName))
	{
		makeDirectory(folderName);
	}
	setCurrentDirectory(folderName);

	return result;
}

string DBgenerateServerDatabaseName(string * objectName, bool trainOrTest)
{
	string databaseName;
	if(!trainOrTest)
	{
		databaseName = databaseFolderName;
	}
	else
	{
		#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
		//cout << "entityName = " << *entityName << endl;
		#endif
		
		#ifdef GIA_DATABASE_FILESYSTEM_USE_MULTIPLE_SERVERS
		char entityFirstCharacter = entityName->at(0);
		if((entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z))
		{
			cout << "determineServerName error: (entityFirstCharacter < ASCII_TABLE_INDEX_OF_a) || (entityFirstCharacter > ASCII_TABLE_INDEX_OF_z)" << endl;
			cout << "entityName = " << *entityName << endl;
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
	cout << "serverName = " << serverName << endl;
	#endif

	return databaseName;
}


string DBgenerateFolderName(string * objectName, bool trainOrTest)
{
	string tempFolder = tempFolderCharStar;
	
	//eg network/server/ORdatabase/e/x/a/example/...
	
	string databaseName = DBgenerateServerDatabaseName(objectName, trainOrTest);
	string fileName = databaseName;

	#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
	cout << "1fileName = " << fileName << endl;
	#endif
	setCurrentDirectory(&fileName);

	if(!trainOrTest)
	{
		fileName = fileName + OR_DATABASE_TEST_FOLDER_NAME + "/";
		string testFolderName = OR_DATABASE_TEST_FOLDER_NAME;
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&testFolderName);	
	}
	else
	{
		#ifdef OR_DATABASE_DEBUG_FILESYSTEM_IO
		cout << "*objectName = " << *objectName << endl;
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
		fileName = fileName + *objectName + "/";
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
bool compareFeaturesListForMatch(Feature * testFirstFeatureInNearestFeatureList, Feature * trainFirstFeatureInNearestFeatureList, int dimension, bool * exactMatchFound)
{
	int numberOfFeatureGeoMatches = 0;
	int numberOfFeatureGeoBinnedExactMatches = 0;
	bool passedGeometricCheck = false;
	Feature * testcurrentFeatureInNearestFeatureList = testFirstFeatureInNearestFeatureList;
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
			Feature * traincurrentFeatureInNearestFeatureList = trainFirstFeatureInNearestFeatureList;
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
						requiredMaxError = (1.0/(maxDouble((minwidthheightOfOriginalTriangleTestAndTrain - OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT), OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT)/(double)OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD_MIN_EXPECTED_ORIG_TRI_WIDTH_OR_HEIGHT)) * OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
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


void addFeatureToEndOfFeatureList(Feature * firstFeatureInList, Feature * featureToAdd)
{
	Feature * currentFeatureInList = firstFeatureInList;

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

	Feature * newFeature = new Feature();
	currentFeatureInList->next = newFeature;

}




void createFeaturesListUsingFeaturesFile(string fileName, Feature * firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures)
{
	Feature * currentFeatureInList = firstFeatureInList;

	if(appendToList)
	{//if creatingFeatureObjects, go to last feature in list (ie append to list if list already has items)
		while(currentFeatureInList->next != NULL)
		{
			currentFeatureInList = currentFeatureInList->next;
		}
	}

	char * fileNamecharstar = const_cast<char*>(fileName.c_str());
	ifstream * parseFileObject = new ifstream(fileNamecharstar);

	if(parseFileObject->rdbuf( )->is_open( ))
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

		char objectNameString[100] = "";
		char trainOrTestString[10] = "";
		char viewIndexString[10] = "";
		char zoomIndexString[10] = "";
		char polyIndexString[10] = "";
		char sideIndexString[10] = "";
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		char OTpointIndexString[10] = "";
		#endif
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		char minWidthAndHeightOfOrigOTString[100] = "";
		#endif
		char transformedPointXString[100] = "";
		char transformedPointYString[100] = "";
		char transformedPointZString[100] = "";
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
		char PointXString[100] = "";
		char PointYString[100] = "";
		char PointZString[100] = "";
		#endif

		while (parseFileObject->get(c))
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
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(objectNameString, typeString);
			}
			else if((readingTestOrTrainIndex) && (c == CHAR_SPACE))
			{
				readingTestOrTrainIndex = false;
				readingViewIndex = true;
			}
			else if(readingTestOrTrainIndex)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(trainOrTestString, typeString);
			}
			else if((readingViewIndex) && (c == CHAR_SPACE))
			{
				readingViewIndex = false;
				readingZoomIndex = true;
			}
			else if(readingViewIndex)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(viewIndexString, typeString);
			}
			else if((readingZoomIndex) && (c == CHAR_SPACE))
			{
				readingZoomIndex = false;
				readingPolyIndex = true;
			}
			else if(readingZoomIndex)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(zoomIndexString, typeString);
			}
			else if((readingPolyIndex) && (c == CHAR_SPACE))
			{
				readingPolyIndex = false;
				readingSideIndex = true;
			}
			else if(readingPolyIndex)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(polyIndexString, typeString);
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
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(sideIndexString, typeString);
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
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(OTpointIndexString, typeString);
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
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(minWidthAndHeightOfOrigOTString, typeString);
			}
		#endif
			else if((readingtransformedPointX) && (c == CHAR_SPACE))
			{
				readingtransformedPointX = false;
				readingtransformedPointY = true;
			}
			else if(readingtransformedPointX)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(transformedPointXString, typeString);
			}
			else if((readingtransformedPointY) && (c == CHAR_SPACE))
			{
				readingtransformedPointY = false;
				readingtransformedPointZ = true;
			}
			else if(readingtransformedPointY)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(transformedPointYString, typeString);
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

				if(!ignoreOTfeatures || ((int)(atof(OTpointIndexString)) == 0))
				{
					currentFeatureInList->objectName = objectNameString;
					currentFeatureInList->trainOrTest = (bool)(atof(trainOrTestString));
					currentFeatureInList->viewIndex = (int)(atof(viewIndexString));
					currentFeatureInList->zoomIndex = (int)(atof(zoomIndexString));
					currentFeatureInList->polyIndex = (int)(atof(polyIndexString));
					currentFeatureInList->sideIndex = (int)(atof(sideIndexString));
					#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
					currentFeatureInList->OTpointIndex = (int)(atof(OTpointIndexString));
					#endif
					#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
					currentFeatureInList->minWidthAndHeightOfOrigOT = (int)(atof(minWidthAndHeightOfOrigOTString));
					#endif
					currentFeatureInList->pointTransformed.x = (atof(transformedPointXString));
					currentFeatureInList->pointTransformed.y = (atof(transformedPointYString));
					currentFeatureInList->pointTransformed.z = (atof(transformedPointZString));
					#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
					currentFeatureInList->point.x = (atof(PointXString));
					currentFeatureInList->point.y = (atof(PointYString));
					currentFeatureInList->point.z = (atof(PointZString));
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
						Feature * newFeature = new Feature();
						currentFeatureInList->next = newFeature;
					}

					currentFeatureInList->lastFilledFeatureInList = false;		//added 8 June 2012 to combat overrun bug
					currentFeatureInList->next->lastFilledFeatureInList = true;		//added 8 June 2012 to combat overrun bug

					currentFeatureInList=currentFeatureInList->next;
				}
				objectNameString[0] = '\0';
				trainOrTestString[0] = '\0';
				viewIndexString[0] = '\0';
				zoomIndexString[0] = '\0';
				polyIndexString[0] = '\0';
				sideIndexString[0] = '\0';
				#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
				OTpointIndexString[0] = '\0';
				#endif
				#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
				minWidthAndHeightOfOrigOTString[0] = '\0';
				#endif
				transformedPointXString[0] = '\0';
				transformedPointYString[0] = '\0';
				transformedPointZString[0] = '\0';
				#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
				PointXString[0] = '\0';
				PointYString[0] = '\0';
				PointZString[0] = '\0';
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
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(transformedPointZString, typeString);
			}
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
			else if((readingPointX) && (c == CHAR_SPACE))
			{
				readingPointX = false;
				readingPointY = true;
			}
			else if(readingPointX)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(PointXString, typeString);
			}
			else if((readingPointY) && (c == CHAR_SPACE))
			{
				readingPointY = false;
				readingPointZ = true;
			}
			else if(readingPointY)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(PointYString, typeString);
			}
			else if(readingPointZ)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(PointZString, typeString);
			}
		#endif
			else
			{

			}
		}

		parseFileObject->close();
		delete parseFileObject;
	}
	else
	{
		cout << "error opening file, " << fileName << endl;
	}
}




void createTransformedFeaturesFile(Feature * firstFeatureInList, string fileName, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest)
{
	char * fileNamecharstar = const_cast<char*>(fileName.c_str());
	ofstream writeFileObject(fileNamecharstar);

	Feature * currentFeature = firstFeatureInList;
	while(currentFeature->next != NULL)
	{
		char polygonIndexString[10];
		char sideIndexString[10];
		char viewIndexString[10];
		char zoomIndexString[10];
		char trainOrTestString[10];
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		char OTpointIndexString[10];
		#endif
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		char minWidthAndHeightOfOrigOTString[10];
		#endif
		char transformedpositionCoordinatesString[100];
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
		char positionCoordinatesString[100];
		#endif

		sprintf(polygonIndexString, "%d", polyIndex);
		sprintf(sideIndexString, "%d", sideIndex);
		sprintf(viewIndexString, "%d", viewIndex);
		sprintf(zoomIndexString, "%d", zoomIndex);
		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		sprintf(OTpointIndexString, "%d", currentFeature->OTpointIndex);
		#endif
		#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
		sprintf(minWidthAndHeightOfOrigOTString, "%d", currentFeature->minWidthAndHeightOfOrigOT);
		#endif
		sprintf(trainOrTestString, "%d", ((int)trainOrTest));
		convertPositionCoordinatesToString(&(currentFeature->pointTransformed), transformedpositionCoordinatesString);
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
		convertPositionCoordinatesToString(&(currentFeature->point), positionCoordinatesString);
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

