 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORdatabaseFileIO.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_DATABASE_FILEIO
#define HEADER_ATOR_DATABASE_FILEIO

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "ATORdatabaseDecisionTreeOperations.hpp"
#include "SHAREDvector.hpp"
#include "LDreferenceManipulation.hpp"

#define ATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"ATORfsdatabase/")
#define ATOR_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME "/home/systemusername/source/"
#define ATOR_DATABASE_CONCEPT_NAME_SUBDIRECTORY_INDEX_NUMBER_OF_LEVELS (3) 	//eg e/x/a/example

#define ASCII_TABLE_INDEX_OF_a (97)
#define ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET (26)
#define ASCII_TABLE_INDEX_OF_z (ASCII_TABLE_INDEX_OF_a + ASCII_TABLE_NUMBER_OF_LETTERS_IN_ALPHABET)

#define ATOR_DATABASE_TEST_FOLDER_NAME "test"
#define ATOR_DATABASE_TRAIN_FOLDER_NAME "train"

class ATORdatabaseFileIOClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: ATORdatabaseDecisionTreeOperationsClass ATORdatabaseDecisionTreeOperations;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	#ifdef ATOR_USE_DATABASE
	public: void initialiseDatabase(const string newDatabaseFolderName);
	private: bool DBdirectoryExists(const string* folderName);
	private: bool DBcreateDirectory(const string* folderName);
	private: bool DBsetCurrentDirectory(const string* folderName);
	private: bool checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(const string* folderName);
	private: string DBgenerateServerDatabaseName(const string* objectName, const bool trainOrTest);
	public: string DBgenerateFolderName(const string* objectName, const bool trainOrTest);
	#endif

	#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
		public: bool compareFeaturesListForMatch(ATORfeature* testFirstFeatureInNearestFeatureList, ATORfeature* trainFirstFeatureInNearestFeatureList, const int dimension, bool* exactMatchFound);
		private: void addFeatureToEndOfFeatureList(ATORfeature* firstFeatureInList, ATORfeature* featureToAdd);
		public: void createTransformedFeaturesFile(const ATORfeature* firstFeatureInList, const string fileName, const string objectName, const int viewIndex, const int zoomIndex, const int polyIndex, const int sideIndex, const int trainOrTest);
		public: void createFeaturesListUsingFeaturesFile(const string fileName, ATORfeature* firstFeatureInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures);
	#endif

};

#endif




