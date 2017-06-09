/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: ATORdatabaseFileIO.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3l1b 02-June-2017
 *
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




