/*******************************************************************************
 *
 * File Name: ORdatabaseDecisionTree.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a7b 09-June-2012
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




void addSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCombinations(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures);
	#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	void addSnapshotIDReferenceToList(string * parseFileName, long snapshotID);
	#endif
	
void getSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCheck(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList, int trainOrTest);
	void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
	void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
	void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopGeo(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
	void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopAvgHueDev(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
	void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopFinal(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);

void createFeatureContainerListUsingUsingGetSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCheck(FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin, bool ignoreOTfeatures, int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], int trainOrTest);
	void createSnapshotIDReferenceImageComparisonDecisionTreeString(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], char * currentDirectoryCharStar, int * currentDirectoryLength, string * currentDirectory);	
	#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	void getSnapshotIDReferencesInImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList, string * currentDirectory);
		void parseSnapshotIDReferenceList(string * parseFileName, SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList);
		#endif
		
		void createOrParseSnapshotIDReferenceImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, bool createOrParse, string * currentDirectory, char * currentDirectoryCharStar, int * currDirIndex);

			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			void createAndOrParseIntoDirectory(string * folderNameFullPath, string * folderName, bool create, bool relativeOrAbsolute);
			#endif
			
//#endif

#endif




