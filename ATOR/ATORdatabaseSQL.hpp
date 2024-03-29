 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORdatabaseSQL.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_DATABASE_SQL
#define HEADER_ATOR_DATABASE_SQL

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORdatabaseDecisionTreeOperations.hpp"
#include "ATORpolygonList.hpp"
#ifdef ATOR_IMAGE_COMPARISON_SQL
#include "LDmysql.hpp"
#include "LDreferenceManipulation.hpp"
#include "RTpixelMaps.hpp"
#endif

#ifdef ATOR_IMAGE_COMPARISON_SQL
extern int64_t databaseTableSizeTrainInitial;
extern int64_t databaseTableSizeTrain;
extern int64_t databaseTableSizeTest;
extern int64_t databaseTableSizeDecisionTreeInitial;
extern int64_t databaseTableSizeDecisionTree;

//move this function elsewhere;
class ATORdatabaseSQLClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: ATORdatabaseDecisionTreeOperationsClass ATORdatabaseDecisionTreeOperations;
	private: LDmysqlClass LDmysql;
	private: RTpixelMapsClass RTpixelMaps;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	
	private: int countIncrements(const int maxIncrement);


	//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE
	public: void createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int trainOrTest);

	public: void createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const string sqlDatabaseDecisionTreeTableName, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int trainOrTest);
	public: void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(const string sqlDatabaseDecisionTreeTableName, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int64_t snapshotReferenceID, int64_t* databaseTableSize);

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
	public: void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(const string sqlDatabaseDecisionTreeTableName, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
	public: void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int64_t snapshotReferenceID, int64_t* databaseTableSize, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
	public: void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength);
	#endif

	//#endif

	public: void convertSnapshotMapsToStringForSQLdatabaseEntry(int imageWidthFacingPoly, const int imageHeightFacingPoly, uchar* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, const double* depthMapFacingPoly, int smallImageWidth, const int smallImageHeight, uchar* rgbMapSmallFacingPoly, const double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, const int dimension, char* snapshotMapsText, const double depthScale, const bool compareRgbDevMapsNotRgbMaps, int* stringSize);
	public: void convertSQLdatabaseStringToSnapshotMaps(const int imageWidthFacingPoly, const int imageHeightFacingPoly, uchar* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, double* depthMapFacingPoly, const int smallImageWidth, const int smallImageHeight, uchar* rgbMapSmallFacingPoly, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, const int dimension, const char* snapshotMapsText, const double depthScale, const bool compareRgbDevMapsNotRgbMaps);


	//#ifdef ATOR_IMAGE_COMPARISON_SQL
	public: void createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(ATORfeatureContainer* firstFeatureContainerInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], uchar* rgb8bitSmallMapForInstantDBqueryAccessRequirement, int smallImageWidth, const int smallImageHeight, const string trainTableName, const int trainOrTest);
	public: void insertTransformedFeatureListIntoDatabase(ATORfeature* firstFeatureInList, const string objectName, const int viewIndex, const int zoomIndex, const int polyIndex, const int sideIndex, const int trainOrTest, const bool ignoreOTfeatures, uchar* rgb8bitSmallMapForInstantDBqueryAccess, int smallImageWidth, const int smallImageHeight, const bool addPermutationsOfTrainFeaturesForGeoBinning, const int maxNumFeaturePermutations, const string tableName, int64_t* databaseTableSize);
	private: void addSQLRowDataToFeatureList(const MYSQL_ROW row, ATORfeature* firstFeatureInList, const bool createFeatureObjects, const bool ignoreOTfeatures, const int numFeatures);
	private: string createSQLSelectRowCommand(const int numFeatures);
	public: void createFeatureListUsingDatabaseQuery(ATORfeature* firstFeatureInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures, const string sqlDatabaseTestTableName, const int64_t testID, const string testObjectName, const int testViewIndex, const int testZoomIndex, const int testPolyIndex, const int testSideIndex, const bool useTestID);
	//#endif


	public: int64_t calculateGeoxyBinMultiDimensional(const int geoxBin[], const int geoyBin[]);

	private: void convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(const schar concatonatedSignedDctCoeffArray[], const int geoxBin[], const int geoyBin[], uint64_t* linearCombination);
};


#endif

#endif




