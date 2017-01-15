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
 * File Name: ORdatabaseSQL.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3j1a 14-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_DATABASE_SQL
#define HEADER_OR_DATABASE_SQL

#include "ORglobalDefs.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"
#ifdef OR_IMAGE_COMPARISON_SQL
#include "LDmysql.h"
#endif

#ifdef OR_IMAGE_COMPARISON_SQL
extern long databaseTableSizeTrainInitial;
extern long databaseTableSizeTrain;
extern long databaseTableSizeTest;
extern long databaseTableSizeDecisionTreeInitial;
extern long databaseTableSizeDecisionTree;

//move this function elsewhere;
int countIncrements(const int maxIncrement);


//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE
void createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int trainOrTest);

void createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const string sqlDatabaseDecisionTreeTableName, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int trainOrTest);
void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(const string sqlDatabaseDecisionTreeTableName, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const long snapshotReferenceID, long* databaseTableSize);

#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(const string sqlDatabaseDecisionTreeTableName, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(const char* decisionTreeBinText, const int decisionTreeBinTextLength, const long snapshotReferenceID, long* databaseTableSize, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength);
#endif

//#endif

void convertSnapshotMapsToStringForSQLdatabaseEntry(int imageWidthFacingPoly, const int imageHeightFacingPoly, unsigned char* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, const double* depthMapFacingPoly, int smallImageWidth, const int smallImageHeight, unsigned char* rgbMapSmallFacingPoly, const double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, const int dimension, char* snapshotMapsText, const double depthScale, const bool compareRgbDevMapsNotRgbMaps, int* stringSize);
void convertSQLdatabaseStringToSnapshotMaps(const int imageWidthFacingPoly, const int imageHeightFacingPoly, unsigned char* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, double* depthMapFacingPoly, const int smallImageWidth, const int smallImageHeight, unsigned char* rgbMapSmallFacingPoly, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, const int dimension, const char* snapshotMapsText, const double depthScale, const bool compareRgbDevMapsNotRgbMaps);


//#ifdef OR_IMAGE_COMPARISON_SQL
void createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(ORfeatureContainer* firstFeatureContainerInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], unsigned char* rgb8bitSmallMapForInstantDBqueryAccessRequirement, int smallImageWidth, const int smallImageHeight, const string trainTableName, const int trainOrTest);
void insertTransformedFeatureListIntoDatabase(ORfeature* firstFeatureInList, const string objectName, const int viewIndex, const int zoomIndex, const int polyIndex, const int sideIndex, const int trainOrTest, const bool ignoreOTfeatures, unsigned char* rgb8bitSmallMapForInstantDBqueryAccess, int smallImageWidth, const int smallImageHeight, const bool addPermutationsOfTrainFeaturesForGeoBinning, const int maxNumFeaturePermutations, const string tableName, long* databaseTableSize);
void addSQLRowDataToFeatureList(const MYSQL_ROW row, ORfeature* firstFeatureInList, const bool createFeatureObjects, const bool ignoreOTfeatures, const int numFeatures);
string createSQLSelectRowCommand(const int numFeatures);
void createFeatureListUsingDatabaseQuery(ORfeature* firstFeatureInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures, const string sqlDatabaseTestTableName, const long testID, const string testObjectName, const int testViewIndex, const int testZoomIndex, const int testPolyIndex, const int testSideIndex, const bool useTestID);
//#endif


long powLong(const long val, const int degree);
	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const signed char concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength, int concatonatedDctCoeffArrayBiasInt[]);
	#else
	//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const signed char concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength);
	#endif
	#else
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	//unsigned long convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const signed char concatonatedSignedDctCoeffArray[], int concatonatedDctCoeffArrayBiasInt[]);
	#else
	unsigned long convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const signed char concatonatedSignedDctCoeffArray[]);
	#endif
	#endif


long calculateGeoxyBinMultiDimensional(const int geoxBin[], const int geoyBin[]);

void convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(const signed char concatonatedSignedDctCoeffArray[], const int geoxBin[], const int geoyBin[], unsigned long* linearCombination);

int determineGeoBinX(const double featurePointTransformedXpos);
int determineGeoBinY(const double featurePointTransformedYpos);
double determineGeoBinDoubleX(const double featurePointTransformedXpos);
double determineGeoBinDoubleY(const double featurePointTransformedYpos);

unsigned int determineDCTBinUnsigned(int arrayValueSigned, double* arrayValueUnsignedDouble);
double determineDCTBinUnsignedDouble(const int arrayValueSigned);


#endif

#endif




