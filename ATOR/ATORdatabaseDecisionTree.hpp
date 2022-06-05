 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORdatabaseDecisionTree.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_DATABASE_DECISIONTREE
#define HEADER_ATOR_DATABASE_DECISIONTREE

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "ATORdatabaseSQL.hpp"
#include "LDreferenceManipulation.hpp"
#include "RTpixelMaps.hpp"

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE

#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
extern string imageComparisonTreeBaseDirectory;
extern string imageComparisonTreeName;
#endif

class ATORdatabaseDecisionTreeClass
{
	private: ATORdatabaseSQLClass ATORdatabaseSQL;
	private: SHAREDvarsClass SHAREDvars;
	private: RTpixelMapsClass RTpixelMaps;
	public: void addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(int imageWidth, const int imageHeight, uchar* rgbMapSmall, ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures);
		#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		private: void addSnapshotIDReferenceToList(const string* parseFileName, const int64_t snapshotID);
		#endif

	public: void getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const int trainOrTest, string* DTbin);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, const int imageHeight, uchar* rgbMapSmall, ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(const int imageWidth, const int imageHeight, const uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(const int imageWidth, const int imageHeight, const uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(const int imageWidth, const int imageHeight, const uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  const char* currentDirectoryCharStar, const int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);

	public: void createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, int imageWidth, const int imageHeight, uchar* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], const int trainOrTest);
		public: void createSnapshotIDreferenceImageComparisonDecisionTreeString(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], char* currentDirectoryCharStar, int* currentDirectoryLength, string* currentDirectory);
		#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		private: void getSnapshotIDreferencesInImageComparisonDecisionTree(int imageWidth, const int imageHeight, uchar* rgbMapSmall, ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, string* currentDirectory);
			private: void parseSnapshotIDreferenceList(const string* parseFileName, ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList);
		#endif

			private: void createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex);	//IMPORTANT: this method only writes to char* currentDirectoryCharStar (not string currentDirectory) [therefore it requires ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL - and cannot be used with a file system based decision tree]

				#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
				public: void createAndOrParseIntoDirectory(string* folderNameFullPath, string* folderName, const bool create, const bool relativeOrAbsolute);
				#endif

};

//#endif

#endif




