/*******************************************************************************
 *
 * File Name: ORdatabaseSQL.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a6c 07-May-2012
 *
 *******************************************************************************/



#ifndef HEADER_OR_DATABASE_SQL
#define HEADER_OR_DATABASE_SQL



#include "SHAREDglobalDefs.h"
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

#ifdef OR_IMAGE_COMPARISON_SQL
#include "LDmysql.h";
#endif

#ifdef OR_IMAGE_COMPARISON_SQL
extern long databaseTableSizeTrainInitial;
extern long databaseTableSizeTrain;
extern long databaseTableSizeTest;
extern long databaseTableSizeDecisionTreeInitial;
extern long databaseTableSizeDecisionTree;

//move this function elsewhere;
int countIncrements(int maxIncrement);


//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE
void createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin, bool ignoreOTfeatures, char * decisionTreeBinText, int decisionTreeBinTextLength, int trainOrTest);

void createSnapshotIDReferenceListUsingSQLDatabaseDecisionTreeTableQuery(SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList, string sqlDatabaseDecisionTreeTableName, char * decisionTreeBinText, int decisionTreeBinTextLength, int trainOrTest);
void insertSnapshotIDReferenceIntoSQLDatabaseDecisionTree(string sqlDatabaseDecisionTreeTableName, char * decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long * databaseTableSize);

#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
void insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeStart(string sqlDatabaseDecisionTreeTableName, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
void insertSnapshotIDReferenceIntoSQLDatabaseDecisionTreeIteration(char * decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long * databaseTableSize, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
void insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeEnd(char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength);
#endif

//#endif

void convertSnapshotMapsToStringForSQLdatabaseEntry(int imageWidthFacingPoly, int imageHeightFacingPoly, unsigned char * rgbMapFacingPoly, double * rgbDevIEnormalisedHueContrastMapFacingPoly, double * depthMapFacingPoly, int smallImageWidth, int smallImageHeight, unsigned char * rgbMapSmallFacingPoly, double * rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double * depthMapSmallFacingPoly, int dimension, char * snapshotMapsText, double depthScale, bool compareRgbDevMapsNotRgbMaps, int * stringSize);
void convertSQLdatabaseStringToSnapshotMaps(int imageWidthFacingPoly, int imageHeightFacingPoly, unsigned char * rgbMapFacingPoly, double * rgbDevIEnormalisedHueContrastMapFacingPoly, double * depthMapFacingPoly, int smallImageWidth, int smallImageHeight, unsigned char * rgbMapSmallFacingPoly, double * rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double * depthMapSmallFacingPoly, int dimension, char * snapshotMapsText, double depthScale, bool compareRgbDevMapsNotRgbMaps);


//#ifdef OR_IMAGE_COMPARISON_SQL
void createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(FeatureContainer * firstFeatureContainerInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], unsigned char * rgb8BitSmallMapForInstantDBQueryAccessRequirement, int smallImageWidth, int smallImageHeight, string trainTableName, int trainOrTest);
void insertTransformedFeatureListIntoDatabase(Feature * firstFeatureInList, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest, bool ignoreOTfeatures, unsigned char * rgb8BitSmallMapForInstantDBQueryAccess, int smallImageWidth, int smallImageHeight, bool addPermutationsOfTrainFeaturesForGeoBinning, int maxNumFeaturePermutations, string tableName, long * databaseTableSize);
void addSQLRowDataToFeatureList(MYSQL_ROW row, Feature * firstFeatureInList, bool createFeatureObjects, bool ignoreOTfeatures, int numFeatures);
void createSQLSelectRowCommand(char sqlSelectRowCommand[], int numFeatures);
void createFeatureListUsingDatabaseQuery(Feature * firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures, string sqlDatabaseTestTableName, long testID, string testObjectName, int testViewIndex, int testZoomIndex, int testPolyIndex, int testSideIndex, bool useTestID);
//#endif


long powInt(long val, int degree);
	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	void convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(signed char concatonatedSignedDctCoeffArray[], char * DCTCoeff64BitValueString, int * DCTCoeff64BitValueStringLength, int concatonatedDctCoeffArrayBiasInt[]);
	#else
	void convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(signed char concatonatedSignedDctCoeffArray[], char * DCTCoeff64BitValueString, int * DCTCoeff64BitValueStringLength);
	#endif
	#else
	unsigned long convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(signed char concatonatedSignedDctCoeffArray[]);
	#endif


long calculateGeoxyBinMultiDimensional(int geoxBin[], int geoyBin[]);

void convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(signed char concatonatedSignedDctCoeffArray[], int geoxBin[], int geoyBin[], unsigned long * linearCombination);



#endif

#endif




