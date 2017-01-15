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
 * File Name: ORdatabaseFileIO.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3i16a 27-August-2016
 *
 *******************************************************************************/


#ifndef HEADER_OR_DATABASE_FILEIO
#define HEADER_OR_DATABASE_FILEIO

#include "ORglobalDefs.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"

#define OR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"ORdatabase/")
#define OR_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME "/home/systemusername/source/"
#define OR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg e/x/a/example

#define ASCII_TABLE_INDEX_OF_a (97)
#define ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET (26)
#define ASCII_TABLE_INDEX_OF_z (ASCII_TABLE_INDEX_OF_a + ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET)

#define OR_DATABASE_TEST_FOLDER_NAME "test"
#define OR_DATABASE_TRAIN_FOLDER_NAME "train"

#ifdef OR_USE_DATABASE
void initialiseDatabase(string newDatabaseFolderName);
bool DBdirectoryExists(string* folderName);
bool DBcreateDirectory(string* folderName);
bool DBsetCurrentDirectory(string* folderName);
bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string* folderName);
string DBgenerateServerDatabaseName(string* objectName, bool trainOrTest);
string DBgenerateFolderName(string* objectName, bool trainOrTest);
#endif

#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	bool compareFeaturesListForMatch(ORfeature* testFirstFeatureInNearestFeatureList, ORfeature* trainFirstFeatureInNearestFeatureList, int dimension, bool* exactMatchFound);
	void addFeatureToEndOfFeatureList(ORfeature* firstFeatureInList, ORfeature* featureToAdd);
	void createTransformedFeaturesFile(ORfeature* firstFeatureInList, string fileName, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest);
	void createFeaturesListUsingFeaturesFile(string fileName, ORfeature* firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures);
#endif

#endif




