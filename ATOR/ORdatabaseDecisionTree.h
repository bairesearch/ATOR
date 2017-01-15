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
 * File Name: ORdatabaseDecisionTree.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3d2a 14-April-2014
 *
 *******************************************************************************/



#ifndef HEADER_OR_DATABASE_DECISIONTREE
#define HEADER_OR_DATABASE_DECISIONTREE



#include "ORglobalDefs.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <time.h>
	#include <math.h>
	using namespace std;

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE

#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
extern string imageComparisonTreeBaseDirectory;
extern string imageComparisonTreeName;
#endif




void addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures);
	#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	void addSnapshotIDReferenceToList(string * parseFileName, long snapshotID);
	#endif

void getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], SnapshotIDreferenceList * firstReferenceInSnapshotIDreferenceList, int trainOrTest, string * DTbin);
	void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
	void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
	void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
	void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
	void addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);

void createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin, bool ignoreOTfeatures, int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], int trainOrTest);
	void createSnapshotIDreferenceImageComparisonDecisionTreeString(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], char * currentDirectoryCharStar, int * currentDirectoryLength, string * currentDirectory);
	#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	void getSnapshotIDreferencesInImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, SnapshotIDreferenceList * firstReferenceInSnapshotIDreferenceList, string * currentDirectory);
		void parseSnapshotIDreferenceList(string * parseFileName, SnapshotIDreferenceList * firstReferenceInSnapshotIDreferenceList);
		#endif

		void createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, bool createOrParse, string * currentDirectory, char * currentDirectoryCharStar, int * currDirIndex);	//IMPORTANT: this method only writes to char * currentDirectoryCharStar (not string currentDirectory) [therefore it requires OR_IMAGE_COMPARISON_DECISION_TREE_SQL - and cannot be used with a file system based decision tree]

			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			void createAndOrParseIntoDirectory(string * folderNameFullPath, string * folderName, bool create, bool relativeOrAbsolute);
			#endif

//#endif

#endif




