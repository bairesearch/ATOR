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
 * File Name: ATORdatabaseFileIO.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3n9a 11-September-2020
 * /
 *******************************************************************************/


#ifndef HEADER_OR_DATABASE_FILEIO
#define HEADER_OR_DATABASE_FILEIO

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "ATORdatabaseSQL.hpp"	//required for determineGeoBinX()/determineGeoBinY() only ... - these functions and/or compareFeaturesListForMatch() should probably be moved elsewhere instead, say to "ORdatabaseOperations.cpp"
#include "SHAREDvector.hpp"
#include "LDreferenceManipulation.hpp"

#define OR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"ORdatabase/")
#define OR_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME "/home/systemusername/source/"
#define OR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg e/x/a/example

#define ASCII_TABLE_INDEX_OF_a (97)
#define ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET (26)
#define ASCII_TABLE_INDEX_OF_z (ASCII_TABLE_INDEX_OF_a + ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET)

#define OR_DATABASE_TEST_FOLDER_NAME "test"
#define OR_DATABASE_TRAIN_FOLDER_NAME "train"

class ORdatabaseFileIOClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: ORdatabaseSQLClass ORdatabaseSQL;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	#ifdef OR_USE_DATABASE
	public: void initialiseDatabase(const string newDatabaseFolderName);
	private: bool DBdirectoryExists(string* folderName);
	private: bool DBcreateDirectory(string* folderName);
	private: bool DBsetCurrentDirectory(string* folderName);
	private: bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(string* folderName);
	private: string DBgenerateServerDatabaseName(const string* objectName, const bool trainOrTest);
	public: string DBgenerateFolderName(string* objectName, const bool trainOrTest);
	#endif

	#ifdef OR_METHOD_GEOMETRIC_COMPARISON
		public: bool compareFeaturesListForMatch(ORfeature* testFirstFeatureInNearestFeatureList, ORfeature* trainFirstFeatureInNearestFeatureList, const int dimension, bool* exactMatchFound);
		private: void addFeatureToEndOfFeatureList(ORfeature* firstFeatureInList, ORfeature* featureToAdd);
		public: void createTransformedFeaturesFile(const ORfeature* firstFeatureInList, const string fileName, const string objectName, const int viewIndex, const int zoomIndex, const int polyIndex, const int sideIndex, const int trainOrTest);
		public: void createFeaturesListUsingFeaturesFile(const string fileName, ORfeature* firstFeatureInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures);
	#endif

};

#endif




