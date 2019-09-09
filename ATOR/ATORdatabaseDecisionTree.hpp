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
 * File Name: ATORdatabaseDecisionTree.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3n1a 10-August-2019
 * /
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
	public: void addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures);
		#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		private: void addSnapshotIDReferenceToList(const string* parseFileName, const int64_t snapshotID);
		#endif

	public: void getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const int trainOrTest, string* DTbin);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
		private: void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  const char* currentDirectoryCharStar, const int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);

	public: void createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], const int trainOrTest);
		public: void createSnapshotIDreferenceImageComparisonDecisionTreeString(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], char* currentDirectoryCharStar, int* currentDirectoryLength, string* currentDirectory);
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




