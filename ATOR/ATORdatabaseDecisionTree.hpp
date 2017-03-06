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
 * File Name: ATORdatabaseDecisionTree.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3j3e 17-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_DATABASE_DECISIONTREE
#define HEADER_OR_DATABASE_DECISIONTREE

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "ATORdatabaseSQL.hpp"
#include "LDreferenceManipulation.hpp"
#include "RTpixelMaps.hpp"

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE

#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
extern string imageComparisonTreeBaseDirectory;
extern string imageComparisonTreeName;
#endif

class ORdatabaseDecisionTreeClass
{
	private: ORdatabaseSQLClass ORdatabaseSQL;
	private: SHAREDvarsClass SHAREDvars;
	private: RTpixelMapsClass RTpixelMaps;
	public: void addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures);
	#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		private: void addSnapshotIDReferenceToList(const string* parseFileName, const long snapshotID);
	#endif

	public: void getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const int trainOrTest, string* DTbin);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  const char* currentDirectoryCharStar, const int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);

	public: void createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], const int trainOrTest);
		public: void createSnapshotIDreferenceImageComparisonDecisionTreeString(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], char* currentDirectoryCharStar, int* currentDirectoryLength, string* currentDirectory);
	#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		private: void getSnapshotIDreferencesInImageComparisonDecisionTree(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, string* currentDirectory);
			private: void parseSnapshotIDreferenceList(const string* parseFileName, ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList);
		#endif

			private: void createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex);	//IMPORTANT: this method only writes to char* currentDirectoryCharStar (not string currentDirectory) [therefore it requires OR_IMAGE_COMPARISON_DECISION_TREE_SQL - and cannot be used with a file system based decision tree]

			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				public: void createAndOrParseIntoDirectory(string* folderNameFullPath, string* folderName, const bool create, const bool relativeOrAbsolute);
			#endif

};

//#endif

#endif




