/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3d2d 14-April-2014
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
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <time.h>
	#include <math.h>
	using namespace std;



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

void createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(SnapshotIDreferenceList * firstReferenceInSnapshotIDreferenceList, string sqlDatabaseDecisionTreeTableName, char * decisionTreeBinText, int decisionTreeBinTextLength, int trainOrTest);
void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(string sqlDatabaseDecisionTreeTableName, char * decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long * databaseTableSize);

#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(string sqlDatabaseDecisionTreeTableName, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(char * decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long * databaseTableSize, char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(char * decisionTreeMultipleRowInsertQueryTextCharStar, long * decisionTreeSQLmultipleRowInsertQueryLength);
#endif

//#endif

void convertSnapshotMapsToStringForSQLdatabaseEntry(int imageWidthFacingPoly, int imageHeightFacingPoly, unsigned char * rgbMapFacingPoly, double * rgbDevIEnormalisedHueContrastMapFacingPoly, double * depthMapFacingPoly, int smallImageWidth, int smallImageHeight, unsigned char * rgbMapSmallFacingPoly, double * rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double * depthMapSmallFacingPoly, int dimension, char * snapshotMapsText, double depthScale, bool compareRgbDevMapsNotRgbMaps, int * stringSize);
void convertSQLdatabaseStringToSnapshotMaps(int imageWidthFacingPoly, int imageHeightFacingPoly, unsigned char * rgbMapFacingPoly, double * rgbDevIEnormalisedHueContrastMapFacingPoly, double * depthMapFacingPoly, int smallImageWidth, int smallImageHeight, unsigned char * rgbMapSmallFacingPoly, double * rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double * depthMapSmallFacingPoly, int dimension, char * snapshotMapsText, double depthScale, bool compareRgbDevMapsNotRgbMaps);


//#ifdef OR_IMAGE_COMPARISON_SQL
void createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(FeatureContainer * firstFeatureContainerInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], unsigned char * rgb8bitSmallMapForInstantDBqueryAccessRequirement, int smallImageWidth, int smallImageHeight, string trainTableName, int trainOrTest);
void insertTransformedFeatureListIntoDatabase(Feature * firstFeatureInList, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest, bool ignoreOTfeatures, unsigned char * rgb8bitSmallMapForInstantDBqueryAccess, int smallImageWidth, int smallImageHeight, bool addPermutationsOfTrainFeaturesForGeoBinning, int maxNumFeaturePermutations, string tableName, long * databaseTableSize);
void addSQLRowDataToFeatureList(MYSQL_ROW row, Feature * firstFeatureInList, bool createFeatureObjects, bool ignoreOTfeatures, int numFeatures);
void createSQLSelectRowCommand(char sqlSelectRowCommand[], int numFeatures);
void createFeatureListUsingDatabaseQuery(Feature * firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures, string sqlDatabaseTestTableName, long testID, string testObjectName, int testViewIndex, int testZoomIndex, int testPolyIndex, int testSideIndex, bool useTestID);
//#endif


long powLong(long val, int degree);
	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[], char * DCTcoeff64bitValueString, int * DCTcoeff64bitValueStringLength, int concatonatedDctCoeffArrayBiasInt[]);
	#else
	void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[], char * DCTcoeff64bitValueString, int * DCTcoeff64bitValueStringLength);
	#endif
	#else
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	unsigned long convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[], int concatonatedDctCoeffArrayBiasInt[]);
	#else
	unsigned long convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[]);
	#endif
	#endif


long calculateGeoxyBinMultiDimensional(int geoxBin[], int geoyBin[]);

void convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(signed char concatonatedSignedDctCoeffArray[], int geoxBin[], int geoyBin[], unsigned long * linearCombination);

int determineGeoBinX(double featurePointTransformedXpos);
int determineGeoBinY(double featurePointTransformedYpos);
double determineGeoBinDoubleX(double featurePointTransformedXpos);
double determineGeoBinDoubleY(double featurePointTransformedYpos);

unsigned int determineDCTBinUnsigned(int arrayValueSigned, double * arrayValueUnsignedDouble);
double determineDCTBinUnsignedDouble(int arrayValueSigned);


#endif

#endif




