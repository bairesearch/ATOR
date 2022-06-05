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
 * File Name: ATORdatabaseSQL.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3p4c 07-August-2021
 * /
 *******************************************************************************/


#include "ATORdatabaseSQL.hpp"

//move this function elsewhere;
int ATORdatabaseSQLClass::countIncrements(const int maxIncrement)
{
	int total = 0;
	for(int i=0; i<=maxIncrement; i++)
	{
		total = total+i;
	}
	return total;
}

//#ifdef ATOR_IMAGE_COMPARISON_SQL
int64_t databaseTableSizeTrainInitial;
int64_t databaseTableSizeTrain;
int64_t databaseTableSizeTest;
int64_t databaseTableSizeDecisionTreeInitial;
int64_t databaseTableSizeDecisionTree;



void ATORdatabaseSQLClass::convertSnapshotMapsToStringForSQLdatabaseEntry(int imageWidthFacingPoly, const int imageHeightFacingPoly, uchar* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, const double* depthMapFacingPoly, int smallImageWidth, const int smallImageHeight, uchar* rgbMapSmallFacingPoly, const double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, const int dimension, char* snapshotMapsText, const double depthScale, const bool compareRgbDevMapsNotRgbMaps, int* stringSize)
{
	int index=0;

	if(!compareRgbDevMapsNotRgbMaps)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour col;
				RTpixelMaps.getRGBMapValues(x, y, imageWidthFacingPoly, rgbMapFacingPoly, &col);
				snapshotMapsText[index++] = col.r;
				snapshotMapsText[index++] = col.g;
				snapshotMapsText[index++] = col.b;
			}
		}
	}
	else
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				vec* rgbDev;
				RTpixelMaps.getVectorMapValue(x, y, imageWidthFacingPoly, rgbDevIEnormalisedHueContrastMapFacingPoly, rgbDev);
				snapshotMapsText[index++] = rgbDev->x* ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->y* ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->z* ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
			}
		}
	}
	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour depthMap24BitPixelValue;
				double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidthFacingPoly, depthMapFacingPoly);
				RTpixelMaps.calculate24BitDepthPixmapPixelFromDepth(depth, depthScale, 0.0, &depthMap24BitPixelValue);
				snapshotMapsText[index++] = depthMap24BitPixelValue.r;
				snapshotMapsText[index++] = depthMap24BitPixelValue.g;
				snapshotMapsText[index++] = depthMap24BitPixelValue.b;
			}
		}
	}


	if(!compareRgbDevMapsNotRgbMaps)
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				colour col;
				RTpixelMaps.getRGBMapValues(x, y, smallImageWidth, rgbMapSmallFacingPoly, &col);
				snapshotMapsText[index++] = col.r;
				snapshotMapsText[index++] = col.g;
				snapshotMapsText[index++] = col.b;
			}
		}
	}
	else
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				vec* rgbDev;
				RTpixelMaps.getVectorMapValue(x, y, smallImageWidth, depthMapSmallFacingPoly, rgbDev);
				snapshotMapsText[index++] = rgbDev->x* ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->y* ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->z* ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
			}
		}
	}
	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				colour depthMap24BitPixelValue;
				double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, smallImageWidth, rgbDevIEnormalisedHueContrastMapSmallFacingPoly);
				RTpixelMaps.calculate24BitDepthPixmapPixelFromDepth(depth, depthScale, 0.0, &depthMap24BitPixelValue);
				snapshotMapsText[index++] = depthMap24BitPixelValue.r;
				snapshotMapsText[index++] = depthMap24BitPixelValue.g;
				snapshotMapsText[index++] = depthMap24BitPixelValue.b;
			}
		}
	}

	*stringSize = index;
}


void ATORdatabaseSQLClass::convertSQLdatabaseStringToSnapshotMaps(const int imageWidthFacingPoly, const int imageHeightFacingPoly, uchar* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, double* depthMapFacingPoly, const int smallImageWidth, const int smallImageHeight, uchar* rgbMapSmallFacingPoly, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, const int dimension, const char* snapshotMapsText, const double depthScale, const bool compareRgbDevMapsNotRgbMaps)
{
	int index=0;

	if(!compareRgbDevMapsNotRgbMaps)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour col;
				col.r = snapshotMapsText[index++];
				col.g = snapshotMapsText[index++];
				col.b = snapshotMapsText[index++];
				RTpixelMaps.setRGBMapValues(x, y, imageWidthFacingPoly, &col, rgbMapFacingPoly);
			}
		}
	}
	else
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				vec rgbDev;
				rgbDev.x = snapshotMapsText[index++]/ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.y = snapshotMapsText[index++]/ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.z = snapshotMapsText[index++]/ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				RTpixelMaps.setVectorMapValue(x, y, imageWidthFacingPoly, &rgbDev, rgbDevIEnormalisedHueContrastMapFacingPoly);

			}
		}
	}
	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour depthMap24BitPixelValue;
				depthMap24BitPixelValue.r = snapshotMapsText[index++];
				depthMap24BitPixelValue.g = snapshotMapsText[index++];
				depthMap24BitPixelValue.b = snapshotMapsText[index++];
				double depth = RTpixelMaps.calculateDepthFrom24BitDepthValue(&depthMap24BitPixelValue,  depthScale, 0.0);
				RTpixelMaps.setLumOrContrastOrDepthMapValue(x, y, imageWidthFacingPoly, depth, depthMapFacingPoly);
			}
		}
	}

	if(!compareRgbDevMapsNotRgbMaps)
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				colour col;
				col.r = snapshotMapsText[index++];
				col.g = snapshotMapsText[index++];
				col.b = snapshotMapsText[index++];
				RTpixelMaps.setRGBMapValues(x, y, smallImageWidth, &col, rgbMapSmallFacingPoly);
			}
		}
	}
	else
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				vec rgbDev;
				rgbDev.x = snapshotMapsText[index++]/ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.y = snapshotMapsText[index++]/ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.z = snapshotMapsText[index++]/ATOR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				RTpixelMaps.setVectorMapValue(x, y, smallImageWidth, &rgbDev, rgbDevIEnormalisedHueContrastMapSmallFacingPoly);

			}
		}
	}
	if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				colour depthMap24BitPixelValue;
				depthMap24BitPixelValue.r = snapshotMapsText[index++];
				depthMap24BitPixelValue.g = snapshotMapsText[index++];
				depthMap24BitPixelValue.b = snapshotMapsText[index++];
				double depth = RTpixelMaps.calculateDepthFrom24BitDepthValue(&depthMap24BitPixelValue, depthScale, 0.0);
				RTpixelMaps.setLumOrContrastOrDepthMapValue(x, y, smallImageWidth, depth, depthMapSmallFacingPoly);
			}
		}
	}
}



void ATORdatabaseSQLClass::createFeatureListUsingDatabaseQuery(ATORfeature* firstFeatureInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures, const string sqlDatabaseTestTableName, const int64_t testID, const string testObjectName, const int testViewIndex, const int testZoomIndex, const int testPolyIndex, const int testSideIndex, const bool useTestID)
{
	int query_state;
	MYSQL_ROW row;

	string sqlSelectCommand = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandP1 = "SELECT ";
	string sqlSelectCommandP2 = "";
	string sqlSelectCommandP3 = " FROM ";
	string sqlSelectCommandP4 = "";	//ATOR_MYSQL_DATABASE_NAME;
	string sqlSelectCommandP5 = "";		//".";
	string sqlSelectCommandP6 = sqlDatabaseTestTableName;
	string sqlSelectCommandP7 = "";
	string sqlSelectCommandP8 = "";
	string sqlSelectCommandP9 = "";
	string sqlSelectCommandP10 = "";
	string sqlSelectCommandP11 = "";
	string sqlSelectCommandP12 = "";

	string testIDString = SHAREDvars.convertLongToString(testID);
	string testviewIndexString = SHAREDvars.convertIntToString(testViewIndex);
	string testzoomIndexString = SHAREDvars.convertIntToString(testZoomIndex);
	string testpolygonIndexString = SHAREDvars.convertIntToString(testPolyIndex);
	string testsideIndexString = SHAREDvars.convertIntToString(testSideIndex);

	if(useTestID)
	{
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + ATOR_MYSQL_FIELD_NAME_ID + " = " + testIDString + ");";	//only query train objects
	}
	else
	{
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + ATOR_MYSQL_FIELD_NAME_OBJECTNAME + " = '" + testObjectName + "'";	//only query train objects
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM + " = " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TEST;
		sqlSelectCommandP9 = sqlSelectCommandP9 + " AND " + ATOR_MYSQL_FIELD_NAME_VIEWNUM + " = " + testviewIndexString;
		sqlSelectCommandP10 = sqlSelectCommandP10 + " AND " + ATOR_MYSQL_FIELD_NAME_ZOOMNUM + " = " + testzoomIndexString;
		sqlSelectCommandP11 = sqlSelectCommandP11 + " AND " + ATOR_MYSQL_FIELD_NAME_POLYNUM + " = " + testpolygonIndexString;
		sqlSelectCommandP12 = sqlSelectCommandP12 + " AND " + ATOR_MYSQL_FIELD_NAME_SIDENUM + " = " + testsideIndexString + ");";
	}

	int numFeatures = ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;	//this value is already dependent upon whether ignoreOTfeatures is true of false
	sqlSelectCommandP2 = createSQLSelectRowCommand(numFeatures);

	sqlSelectCommand = sqlSelectCommandP1 + sqlSelectCommandP2 + sqlSelectCommandP3 + sqlSelectCommandP4 + sqlSelectCommandP5 + sqlSelectCommandP6 + sqlSelectCommandP7 + sqlSelectCommandP8 + sqlSelectCommandP9 + sqlSelectCommandP10 + sqlSelectCommandP11 + sqlSelectCommandP12;
	char* sqlSelectCommandCharStar = const_cast<char*>(sqlSelectCommand.c_str());

	query_state = mysql_query(connection, sqlSelectCommandCharStar);
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);

		while((row = mysql_fetch_row(result)) != NULL)
		{//only use first matching feature; nb that when train table is separated from test table every feature will have a unique objectname, viewnum, zoomnum, polynum and testside

			if(tableIndex > 1)
			{
				cerr << "error: multiple identical test snapshots detected" << endl;
				exit(EXIT_ERROR);

			}
			addSQLRowDataToFeatureList(row, firstFeatureInList, createFeatureObjects, ignoreOTfeatures, numFeatures);
			tableIndex++;
		}

	}
	mysql_free_result(result);
}


//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE
void ATORdatabaseSQLClass::createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int trainOrTest)
{

	int query_state;
	MYSQL_ROW row;

	string sqlSelectCommandOpen = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandClose = "";
	string sqlSelectCommandSelect = "SELECT ";
	string sqlSelectCommandSelectContents = "";

#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_DIRECT_ACCESS_USE_JOIN_OPT
	cerr << "SQL join optimisation not yet coded" << endl;
	exit(EXIT_ERROR);
#else
	string sqlSelectCommandFrom = "";
	string sqlSelectCommandWhereOpen = "";
	string sqlSelectCommandWhereClose = "));";

	sqlSelectCommandFrom = sqlSelectCommandFrom + " FROM " + ATOR_MYSQL_TABLE_NAME_TRAIN;
	sqlSelectCommandWhereOpen = sqlSelectCommandWhereOpen + " WHERE (" + ATOR_MYSQL_FIELD_NAME_ID + " IN (";	//only query train objects

	int numFeatures = ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;	//this value is already dependent upon whether ignoreOTfeatures is true of false
	sqlSelectCommandSelectContents = createSQLSelectRowCommand(numFeatures);

	string sqlSelectCommandWhereContentsSelect = "SELECT ";
	string sqlSelectCommandWhereContentsSelectContents = ATOR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID;
	string sqlSelectCommandWhereContentsFrom = "";
	string sqlSelectCommandWhereContentsWhereOpen = "";
	char sqlSelectCommandWhereContentsWhereContents[ATOR_IMAGE_COMPARISON_DECISION_TREE_STRING_MAX_LENGTH];
	string sqlSelectCommandWhereContentsWhereClose = ")";

	sqlSelectCommandWhereContentsFrom = sqlSelectCommandWhereContentsFrom + " FROM " + ATOR_MYSQL_TABLE_NAME_DECISIONTREE;
	if(trainOrTest == TEST)
	{
		sqlSelectCommandWhereContentsWhereOpen = sqlSelectCommandWhereContentsWhereOpen + " WHERE (" + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}
	else if(trainOrTest == TRAIN_AND_TEST)
	{
		string maxDecisionTreeTableRowString = SHAREDvars.convertLongToString(databaseTableSizeDecisionTreeInitial);
		sqlSelectCommandWhereContentsWhereOpen = sqlSelectCommandWhereContentsWhereOpen + " WHERE (" + ATOR_MYSQL_FIELD_NAME_ID + " <= " + maxDecisionTreeTableRowString + " AND " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}

	//must be done this way as decisionTreeBinText may contain null characters
	int sqlSelectCommandWhereContentsWhereContentsLength = 0;
	sqlSelectCommandWhereContentsWhereContents[sqlSelectCommandWhereContentsWhereContentsLength] = '\'';
	sqlSelectCommandWhereContentsWhereContentsLength++;
	sqlSelectCommandWhereContentsWhereContentsLength = sqlSelectCommandWhereContentsWhereContentsLength + mysql_real_escape_string(&mysql, &(sqlSelectCommandWhereContentsWhereContents[sqlSelectCommandWhereContentsWhereContentsLength]), decisionTreeBinText, decisionTreeBinTextLength);
	sqlSelectCommandWhereContentsWhereContents[sqlSelectCommandWhereContentsWhereContentsLength] = '\'';
	sqlSelectCommandWhereContentsWhereContentsLength++;

	sqlSelectCommandOpen = sqlSelectCommandOpen + sqlSelectCommandSelect + sqlSelectCommandSelectContents + sqlSelectCommandFrom + sqlSelectCommandWhereOpen + sqlSelectCommandWhereContentsSelect + sqlSelectCommandWhereContentsSelectContents + sqlSelectCommandWhereContentsFrom + sqlSelectCommandWhereContentsWhereOpen;
	sqlSelectCommandClose = sqlSelectCommandWhereContentsWhereClose + sqlSelectCommandWhereClose;

	char sqlSelectCommandCharStar[ATOR_IMAGE_COMPARISON_SQL_DATABASE_TEST_AND_TRAIN_TABLES_GET_QUERY_MAX_LENGTH + ATOR_IMAGE_COMPARISON_DECISION_TREE_STRING_MAX_LENGTH];
	int sqlSelectCommandCharStarIndex = 0;
	for(int i=0; i <sqlSelectCommandOpen.length(); i++)
	{
	 	sqlSelectCommandCharStar[sqlSelectCommandCharStarIndex] = sqlSelectCommandOpen[i];
		sqlSelectCommandCharStarIndex++;
	}
	for(int i=0; i <sqlSelectCommandWhereContentsWhereContentsLength; i++)
	{
	 	sqlSelectCommandCharStar[sqlSelectCommandCharStarIndex] = sqlSelectCommandWhereContentsWhereContents[i];
		sqlSelectCommandCharStarIndex++;
	}
	for(int i=0; i <sqlSelectCommandClose.length(); i++)
	{
	 	sqlSelectCommandCharStar[sqlSelectCommandCharStarIndex] = sqlSelectCommandClose[i];
		sqlSelectCommandCharStarIndex++;
	}
#endif


	#ifndef ATOR_SQL_DATABASE_DEBUG
	#endif

	query_state = mysql_real_query(connection, sqlSelectCommandCharStar, sqlSelectCommandCharStarIndex);
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);

		ATORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;

		while((row = mysql_fetch_row(result)) != NULL)
		{//only use first matching feature; nb that when train table is separated from test table every feature will have a unique objectname, viewnum, zoomnum, polynum and testside

			ATORfeature* firstNewFeature = new ATORfeature();
			currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList = firstNewFeature;
			addSQLRowDataToFeatureList(row, firstNewFeature, true, ignoreOTfeatures, numFeatures);
			ATORfeatureContainer* newFeatureContainer = new ATORfeatureContainer();
			currentFeatureContainerInTestFeatureMatchingTrainBin->next = newFeatureContainer;
			currentFeatureContainerInTestFeatureMatchingTrainBin = currentFeatureContainerInTestFeatureMatchingTrainBin->next;

			tableIndex++;
		}

	}
	mysql_free_result(result);
}




void ATORdatabaseSQLClass::insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(const string sqlDatabaseDecisionTreeTableName, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int64_t snapshotReferenceID, int64_t* databaseTableSize)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP1 = "INSERT INTO ";
	string sqlInsertCommandP2 = sqlDatabaseDecisionTreeTableName;
	string sqlInsertCommandP3 = " (";
	string sqlInsertCommandP4 = "";
	string sqlInsertCommandP5 = ") VALUES (";
	string sqlInsertCommandP6 = "";
	string sqlInsertCommandP7 = ");";

	string tableIDString = SHAREDvars.convertLongToString(*databaseTableSize);
	string snapshotReferenceIDString = SHAREDvars.convertLongToString(snapshotReferenceID);

	sqlInsertCommandP4 = sqlInsertCommandP4 + ATOR_MYSQL_FIELD_NAME_ID + ", " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID + ", " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN;
	sqlInsertCommandP6 = sqlInsertCommandP6 + tableIDString + ", " + snapshotReferenceIDString + ", ";

	sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6;
	char sqlInsertCommandCharStar[1000];
	int i;

	for(i=0; i <sqlInsertCommand.length(); i++)
	{
	 	sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
	}

	//must be done this way as decisionTreeBinText may contain null characters

	#ifndef DEBUG_ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
	sqlInsertCommandCharStar[i] = '\'';
	i++;
	i = i + mysql_real_escape_string(&mysql, &(sqlInsertCommandCharStar[i]), decisionTreeBinText, decisionTreeBinTextLength);
	sqlInsertCommandCharStar[i] = '\'';
	i++;
	#endif
	sqlInsertCommandCharStar[i] = ')';
	i++;
	sqlInsertCommandCharStar[i] = ';';
	i++;


	LDmysql.performSQLrealInsertQuery(sqlInsertCommandCharStar, i);
	*databaseTableSize = *databaseTableSize + 1;
}




#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION

void ATORdatabaseSQLClass::insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(const string sqlDatabaseDecisionTreeTableName, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP1 = "INSERT INTO ";
	string sqlInsertCommandP2 = sqlDatabaseDecisionTreeTableName;
	string sqlInsertCommandP3 = " (";
	string sqlInsertCommandP4 = "";
	string sqlInsertCommandP5 = ") VALUES ";

	sqlInsertCommandP4 = sqlInsertCommandP4 + ATOR_MYSQL_FIELD_NAME_ID + ", " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID + ", " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN;

	sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5;

	for(int j=0; j<sqlInsertCommand.length(); j++)
	{
		decisionTreeMultipleRowInsertQueryTextCharStar[*decisionTreeSQLmultipleRowInsertQueryLength] = sqlInsertCommand[j];
		*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength + 1;
	}


}


void ATORdatabaseSQLClass::insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int64_t snapshotReferenceID, int64_t* databaseTableSize, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP6 = "";

	string tableIDString = SHAREDvars.convertLongToString(*databaseTableSize);
	string snapshotReferenceIDString = SHAREDvars.convertLongToString(snapshotReferenceID);

	sqlInsertCommandP6 = sqlInsertCommandP6 + "(" + tableIDString + ", " + snapshotReferenceIDString + ", ";
	sqlInsertCommand = sqlInsertCommandP6;
	char sqlInsertCommandCharStar[1000];
	int i;

	for(i=0; i <sqlInsertCommand.length(); i++)
	{
	 	sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
	}

	//must be done this way as decisionTreeBinText may contain null characters

	#ifndef DEBUG_ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
	sqlInsertCommandCharStar[i] = '\'';
	i++;
	i = i + mysql_real_escape_string(&mysql, &(sqlInsertCommandCharStar[i]), decisionTreeBinText, decisionTreeBinTextLength);
	sqlInsertCommandCharStar[i] = '\'';
	i++;
	#endif
	sqlInsertCommandCharStar[i] = ')';
	i++;
	sqlInsertCommandCharStar[i] = ',';
	i++;

	for(int j=0; j<i; j++)
	{
		decisionTreeMultipleRowInsertQueryTextCharStar[*decisionTreeSQLmultipleRowInsertQueryLength] = sqlInsertCommandCharStar[j];
		*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength + 1;
	}

	*databaseTableSize = *databaseTableSize + 1;
}


void ATORdatabaseSQLClass::insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	if(decisionTreeMultipleRowInsertQueryTextCharStar[(*decisionTreeSQLmultipleRowInsertQueryLength)-1] != ' ')
	{
		string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
		string sqlInsertCommandP1 = ";";

		sqlInsertCommand = sqlInsertCommandP1;

		for(int i=0; i<sqlInsertCommand.length(); i++)
		{
	 		decisionTreeMultipleRowInsertQueryTextCharStar[(*decisionTreeSQLmultipleRowInsertQueryLength)-1] = sqlInsertCommand[i];	//-1 is to overwrite previous ','
			*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength+1;
		}
		*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength-1;

		LDmysql.performSQLrealInsertQuery(decisionTreeMultipleRowInsertQueryTextCharStar,* decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else
	{
		//no acceptable decision tree data for this bin
	}
}

/*old; this code is to insert multiple rows into multiple tables
void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(char* decisionTreeMultipleRowInsertQueryTextCharStar, int decisionTreeSQLmultipleRowInsertQueryLength)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP1 = "INSERT ALL ";

	sqlInsertCommand = sqlInsertCommandP1;

	for(int i=0; i<sqlInsertCommand.length(); i++)
	{
	 	decisionTreeMultipleRowInsertQueryTextCharStar[decisionTreeSQLmultipleRowInsertQueryLength+i] = sqlInsertCommand[i];
	}
}
*/

/*old; this code is to insert multiple rows into multiple tables
void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(string sqlDatabaseDecisionTreeTableName, char* decisionTreeBinText, int decisionTreeBinTextLength, int64_t snapshotReferenceID, int64_t* databaseTableSize, char* decisionTreeMultipleRowInsertQueryTextCharStar, int decisionTreeSQLmultipleRowInsertQueryLength)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP1 = "\n\tINTO ";
	string sqlInsertCommandP2 = sqlDatabaseDecisionTreeTableName;
	string sqlInsertCommandP3 = " (";
	string sqlInsertCommandP4 = "";
	string sqlInsertCommandP5 = ") VALUES (";
	string sqlInsertCommandP6 = "";

	string tableIDString = convertLongToString(*databaseTableSize);
	string snapshotReferenceIDString = convertLongToString(snapshotReferenceID);

	sqlInsertCommandP4 = sqlInsertCommandP4 + ATOR_MYSQL_FIELD_NAME_ID + ", " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID + ", " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN;
	sqlInsertCommandP6 = sqlInsertCommandP6 + tableIDString + ", " + snapshotReferenceIDString + ", ";

	sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6;
	char sqlInsertCommandCharStar[1000];
	int i;


	//cout << "sqlInsertCommand.length() = " << sqlInsertCommand.length() << endl;
	for(i=0; i <sqlInsertCommand.length(); i++)
	{
	 	sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
		//cout << "c2 = " << sqlInsertCommand[i] << endl;
	}
	//cout << "i1 = " << i << endl;
		//must be done this way as decisionTreeBinText may contain null characters

	#ifdef DEBUG_ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
	//cout << "decisionTreeBinTextLength = " << decisionTreeBinTextLength << endl;
	for(int q=0; q <decisionTreeBinTextLength; q++)
	{
		//cout << "i = " << i << endl;
	 	//cout << "sqlInsertCommandCharStar[i] = " << sqlInsertCommandCharStar[i] << endl;
		sqlInsertCommandCharStar[i] = decisionTreeBinText[q];
		i++;
	}
	#else
	sqlInsertCommandCharStar[i] = '\'';
	i++;
	i = i + mysql_real_escape_string(&mysql, &(sqlInsertCommandCharStar[i]), decisionTreeBinText, decisionTreeBinTextLength);
	sqlInsertCommandCharStar[i] = '\'';
	i++;
	#endif

	for(int j=0; j<i; j++)
	{
		decisionTreeMultipleRowInsertQueryTextCharStar[decisionTreeSQLmultipleRowInsertQueryLength] = sqlInsertCommandCharStar[j];
		*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength + 1;
	}

	*databaseTableSize = *databaseTableSize + 1;
}
*/

#endif


void ATORdatabaseSQLClass::createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const string sqlDatabaseDecisionTreeTableName, const char* decisionTreeBinText, const int decisionTreeBinTextLength, const int trainOrTest)
{
	int query_state;
	MYSQL_ROW row;

	string sqlSelectCommand = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandP1 = "SELECT ";
	string sqlSelectCommandP2 = "";
	string sqlSelectCommandP3 = " FROM ";
	string sqlSelectCommandP4 = "";	//ATOR_MYSQL_DATABASE_NAME;
	string sqlSelectCommandP5 = "";		//".";
	string sqlSelectCommandP6 = sqlDatabaseDecisionTreeTableName;
	string sqlSelectCommandP7 = "";
	string sqlSelectCommandP8 = "";
	string sqlSelectCommandP9 = "";
	string sqlSelectCommandP10 = "";
	string sqlSelectCommandP11 = "";
	string sqlSelectCommandP12 = "";

	sqlSelectCommandP2 = ATOR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID;

	if(trainOrTest == TEST)
	{
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}
	else if(trainOrTest == TRAIN_AND_TEST)
	{
		string maxDecisionTreeTableRowString = SHAREDvars.convertLongToString(databaseTableSizeDecisionTreeInitial);
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + ATOR_MYSQL_FIELD_NAME_ID + " <= " + maxDecisionTreeTableRowString + " AND " + ATOR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}

	sqlSelectCommand = sqlSelectCommandP1 + sqlSelectCommandP2 + sqlSelectCommandP3 + sqlSelectCommandP4 + sqlSelectCommandP5 + sqlSelectCommandP6 + sqlSelectCommandP7;

	char sqlSelectCommandCharStar[ATOR_IMAGE_COMPARISON_DECISION_TREE_STRING_MAX_LENGTH];
	int i;
	for(i=0; i <sqlSelectCommand.length(); i++)
	{
	 	sqlSelectCommandCharStar[i] = sqlSelectCommand[i];
	}

	//must be done this way as decisionTreeBinText may contain null characters

	#ifndef DEBUG_ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
	sqlSelectCommandCharStar[i] = '\'';
	i++;
	i = i + mysql_real_escape_string(&mysql, &(sqlSelectCommandCharStar[i]), decisionTreeBinText, decisionTreeBinTextLength);
	sqlSelectCommandCharStar[i] = '\'';
	i++;
	#endif
	sqlSelectCommandCharStar[i] = ')';
	i++;
	sqlSelectCommandCharStar[i] = ';';
	i++;


	#ifndef ATOR_SQL_DATABASE_DEBUG
	#endif

	query_state = mysql_real_query(connection, sqlSelectCommandCharStar, i);
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);

		ATORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDreferenceList;

		while((row = mysql_fetch_row(result)) != NULL)
		{//only use first matching feature; nb that when train table is separated from test table every feature will have a unique objectname, viewnum, zoomnum, polynum and testside

			char* tableIDCharStar;
			tableIDCharStar = row[0];
			int64_t tableID = (int64_t)(atof(tableIDCharStar));

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_LOOKUP_DO_NOT_ALLOW_REPEATS
			//test for multiple references to same object ID added by RBB 23Mar10
			bool snapshotIDReferenceAlreadyAddedToList = false;
			if(tableIndex > 0)
			{
				ATORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceListTemp = firstReferenceInSnapshotIDreferenceList;
				while(currentReferenceInSnapshotIDReferenceListTemp->next != NULL)
				{
					if(tableID == currentReferenceInSnapshotIDReferenceListTemp->referenceID)
					{
						snapshotIDReferenceAlreadyAddedToList = true;
					}
					currentReferenceInSnapshotIDReferenceListTemp = currentReferenceInSnapshotIDReferenceListTemp->next;
				}
			}
			if(!snapshotIDReferenceAlreadyAddedToList)
			{
			#endif
				currentReferenceInSnapshotIDReferenceList->referenceID = tableID;

				ATORsnapshotIDreferenceList* newReferenceInSnapshotIDReferenceList = new ATORsnapshotIDreferenceList();
				currentReferenceInSnapshotIDReferenceList->next = newReferenceInSnapshotIDReferenceList;
				currentReferenceInSnapshotIDReferenceList = currentReferenceInSnapshotIDReferenceList->next;
			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_LOOKUP_DO_NOT_ALLOW_REPEATS
			}
			#endif

			tableIndex++;

		}

	}
	mysql_free_result(result);
}


//#endif












void ATORdatabaseSQLClass::createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(ATORfeatureContainer* firstFeatureContainerInList, const bool createFeatureObjects, const bool appendToList, const bool ignoreOTfeatures, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], uchar* rgb8bitSmallMapForInstantDBqueryAccessRequirement, int smallImageWidth, const int smallImageHeight, const string trainTableName, const int trainOrTest)
{
	if(!createFeatureObjects)
	{
		cerr << "error; createFeaturesListUsingDatabaseQueryGeoXYbinRequirement requires createFeatureObjects" << endl;
		exit(EXIT_ERROR);
	}

	MYSQL_ROW row;
	int query_state;

	string sqlSelectCommand = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandP1 = "SELECT ";
	string sqlSelectCommandP2 = "";
	string sqlSelectCommandP3 = " FROM ";
	string sqlSelectCommandP4 = "";	//ATOR_MYSQL_DATABASE_NAME;
	string sqlSelectCommandP5 = "";		//".";
	string sqlSelectCommandP6 = trainTableName;
	string sqlSelectCommandP7 = "";
	string sqlSelectCommandP8 = "";
	string sqlSelectCommandP9 = "";
	string sqlSelectCommandP10 = "";
	string sqlSelectCommandP11 = "";
	string sqlSelectCommandP12 = ");";

	if(trainOrTest == TEST)
	{
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM + " = " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TRAIN;	//only query train objects
	}
	else if(trainOrTest == TRAIN_AND_TEST)
	{
		string maxTrainTableRowString = SHAREDvars.convertLongToString(databaseTableSizeTrainInitial);
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + ATOR_MYSQL_FIELD_NAME_ID + " <= " + maxTrainTableRowString + " AND " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM + " = " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TRAIN;	//only query train objects
	}
	else
	{
		cerr << "illegal trainOrTest value" << endl;
		exit(EXIT_ERROR);
	}


#ifdef ATOR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK
	uint64_t linearCombination = 0;
	convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(concatonatedSignedDctCoeffArrayRequirement, pBinxRequirement, pBinyRequirement, &linearCombination);
	string linearCombinationString = SHAREDvars.convertLongToString(linearCombination);
	sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL + " >= (" + linearCombinationString + " - " + ATOR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK_MAX_DIFF + ")" " AND " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL + " <= (" + linearCombinationString + " + " + ATOR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK_MAX_DIFF + ")";
	cout << "sqlSelectCommandP8 = " << sqlSelectCommandP8 << endl;
#else

	if(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING)
	{
	#ifdef ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V1
		string pBinxyValueRequirementString = SHAREDvars.convertIntToString(pBinxyValueRequirement);
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_XY + " = " + pBinxyValueRequirementString;
	#elif defined ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V2
		//V2 MIGHT BE BAD DEPENDING UPON HOW GOOD MYSQL IS; if this is used px1->px2 and py1 -> py2 need to be indexed. note there are ~100^4 combinations of binned variables.
		for(int i=0; i<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; i++)
		{
			string pBinxValueRequirementMinString = SHAREDvars.convertIntToString(pBinxRequirement[i]-1);
			string pBinyValueRequirementMinString = SHAREDvars.convertIntToString(pBinyRequirement[i]-1);

			string pBinxNameRequirementString = SHAREDvars.convertIntToString(i+1);
			string pBinyNameRequirementString = SHAREDvars.convertIntToString(i+1);

			string pBinxValueRequirementMaxString = SHAREDvars.convertIntToString(pBinxRequirement[i]+1);
			string pBinyValueRequirementMaxString = SHAREDvars.convertIntToString(pBinyRequirement[i]+1);

			sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " +
			ATOR_MYSQL_FIELD_NAME_GEO_BINS_X + pBinxNameRequirementString + " >= " + pBinxValueRequirementMinString + " AND " +
			ATOR_MYSQL_FIELD_NAME_GEO_BINS_X + pBinxNameRequirementString + " <= " + pBinxValueRequirementMaxString + " AND " +
			ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y + pBinyNameRequirementString + " >= " + pBinyValueRequirementMinString + " AND " +
			ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y + pBinyNameRequirementString + " <= " + pBinyValueRequirementMaxString;
		}
	#elif defined ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V3
		int xBinIndex = 0;
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND (";
		int count = 0;

		for(int x = pBinxRequirement[0]-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=pBinxRequirement[0]+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
		{
			int yBinIndex =0;
			for(int y = pBinyRequirement[0]-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=pBinyRequirement[0]+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
			{
				//check current bin and nearest bins
				int xBinIndex2 = 0;
				for(int x2 = pBinxRequirement[1]-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2<=pBinxRequirement[1]+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2++)
				{
					int y2BinIndex =0;
					for(int y2 = pBinyRequirement[1]-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2<=pBinyRequirement[1]+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2++)
					{
						int geoxBin[2];
						int geoyBin[2];
						int64_t geoxybin;

						geoxBin[0] = x;
						geoyBin[0] = y;
						geoxBin[1] = x2;
						geoyBin[1] = y2;

						geoxybin = calculateGeoxyBinMultiDimensional(geoxBin, geoyBin);

						char pBinxyValueRequirementString[25];
						pBinxyValueRequirementString = SHAREDvars.convertIntToString(geoxybin);
						sqlSelectCommandP8 = sqlSelectCommandP8 + ATOR_MYSQL_FIELD_NAME_GEO_BINS_XY + " = " + pBinxyValueRequirementString;

						count++;

						if(count == ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X*ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y*ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X*ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y)
						{
							sqlSelectCommandP8 = sqlSelectCommandP8 + ")";
						}
						else
						{
							sqlSelectCommandP8 = sqlSelectCommandP8 + " OR ";
						}
					}
				}
			}
		}
		/*
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND (" +
		for(int x=0; x<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; x++)
		{
			for(int y=0; y<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; y++)
			{
				char pBinxValueRequirementString[10];
				pBinxValueRequirementString = SHAREDvars.convertIntToString(pBinxRequirement[x]);
				char pBinyValueRequirementString[10];
				pBinyValueRequirementString = SHAREDvars.convertIntToString(pBinyRequirement[y]);

				char pBinxNameRequirementString[10];
				pBinxNameRequirementString = SHAREDvars.convertIntToString(x+1);
				char pBinyNameRequirementString[10];
				pBinyNameRequirementString = SHAREDvars.convertIntToString(y+1);


				sqlSelectCommandP8 = sqlSelectCommandP8 + "(" +
				ATOR_MYSQL_FIELD_NAME_GEO_BINS_X + pBinxNameRequirementString + " = " + pBinxValueRequirementString + " AND " +
				ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y + pBinyNameRequirementString + " = " + pBinyValueRequirementString + ")";

				if((x == ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS-1) && (y == ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS-1))
				{
					sqlSelectCommandP8 = sqlSelectCommandP8 + ")";
				}
				else
				{
					sqlSelectCommandP8 = sqlSelectCommandP8 + " OR ";
				}

			}
		}
		*/
	#endif
	}

	if(ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING)
	{
	#ifdef ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING_REQUIREMENT_V2
		for(int colour=0; colour < 3; colour++)
		{
			char normalisedAverageHueDevValueMaxString[10];
			char normalisedAverageHueDevValueMinString[10];
			uchar maxVal;
			uchar minVal;
			uchar avgVal;


			string normalisedAverageHueDevName;
			if(colour == 0)
			{
				avgVal = normalisedAverageHueDeviationRequirement->r;
				normalisedAverageHueDevName = ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_R_BINS;
			}
			else if(colour == 1)
			{
				avgVal = normalisedAverageHueDeviationRequirement->g;
				normalisedAverageHueDevName = ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_G_BINS;
			}
			else
			{
				avgVal = normalisedAverageHueDeviationRequirement->b;
				normalisedAverageHueDevName = ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_B_BINS;

			}
			maxVal = SHAREDvars.minInt(int(avgVal) + 1, RGB_NUM);
			minVal = SHAREDvars.maxInt(int(avgVal) - 1, 0);

			normalisedAverageHueDevValueMaxString = SHAREDvars.convertIntToString(maxVal);
			normalisedAverageHueDevValueMinString = SHAREDvars.convertIntToString(minVal);

			sqlSelectCommandP9 = sqlSelectCommandP9 + " AND " + normalisedAverageHueDevName + " >= " + normalisedAverageHueDevValueMinString + " AND " + normalisedAverageHueDevName + " <= " + normalisedAverageHueDevValueMaxString;
				//requires optimisation
		}
	#elif defined ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING_REQUIREMENT_V3

		int rBinMid = (int)normalisedAverageHueDeviationRequirement->r;
		int gBinMid = (int)normalisedAverageHueDeviationRequirement->g;
		int bBinMid = (int)normalisedAverageHueDeviationRequirement->b;


		int rBinMin = rBinMid-1;
		int rBinMax = rBinMid+1;
		int gBinMin = gBinMid-1;
		int gBinMax = gBinMid+1;
		int bBinMin = bBinMid-1;
		int bBinMax = bBinMid+1;

		if(rBinMin < 0)
		{
			rBinMin = 0;
		}
		if(rBinMax > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
		{
			rBinMax = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
		}
		if(gBinMin < 0)
		{
			gBinMin = 0;
		}
		if(gBinMax > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
		{
			gBinMax = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
		}
		if(bBinMin < 0)
		{
			bBinMin = 0;
		}
		if(bBinMax > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
		{
			bBinMax = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
		}

		sqlSelectCommandP9 = sqlSelectCommandP9 + " AND (";

		for(int r = rBinMin; r <= rBinMax; r++)
		{
			for(int g = gBinMin; g <= gBinMax; g++)
			{
				for(int b = bBinMin; b <= bBinMax; b++)
				{
					string normalisedAverageHueDevName = ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
					int currentCombinergbBin = r*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + g*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + b;
					string currentCombinergbBinString = SHAREDvars.convertIntToString(currentCombinergbBin);

					sqlSelectCommandP9 = sqlSelectCommandP9 + normalisedAverageHueDevName + " = " + currentCombinergbBinString;

					if((r == rBinMax) && (g == gBinMax) && (b == bBinMax))
					{
						sqlSelectCommandP9 = sqlSelectCommandP9 + ")";
					}
					else
					{
						sqlSelectCommandP9 = sqlSelectCommandP9 + " OR ";
					}
				}
			}
		}
	#endif
	}



	if(ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
	#ifdef ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V2
		//V2 MIGHT BE BAD DEPENDING UPON HOW GOOD MYSQL IS; if this is used d0->dx need to be indexed. note there are 16^16 combinations of dct variables.
		for(int dctCoeffNum = 0; dctCoeffNum <ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{
			string dctCoeffNumNameString = SHAREDvars.convertIntToString(dctCoeffNum);

			//int maxVal = SHAREDvars.minInt(int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) + 1, ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET-1);	//nb allowed bin values are between -8 and +7
			//int minVal = SHAREDvars.maxInt(int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) - 1, -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET);
			int maxVal = int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) + 1;	//nb allowed bin values are between -8 and +7
			int minVal = int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) - 1;

			string dctCoeffNumValMaxString = SHAREDvars.convertIntToString(maxVal);
			string dctCoeffNumValMinString = SHAREDvars.convertIntToString(minVal);

			sqlSelectCommandP10 = sqlSelectCommandP10 + " AND " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + " >= " + dctCoeffNumValMinString + " AND " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + " <= " + dctCoeffNumValMaxString;		//only query train objects
		}
	#elif defined ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V3
		int numberOfSuccessfulDCTcomparisons = 0;
		for(int dctCoeffNum = 0; dctCoeffNum <ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{

			char dctCoeffNumNameString[10];
			dctCoeffNumNameString = SHAREDvars.convertIntToString(dctCoeffNum);

			int dctCoeffNumVal = int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]);
			char dctCoeffNumValString[10];
			dctCoeffNumValString = SHAREDvars.convertIntToString(dctCoeffNumVal);

				//Seg SELECT columnname, COUNT(*) FROM tablename
			//eg COUNT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
			string sqlSelectCommandDCTqueryV3 = "";
			sqlSelectCommandDCTqueryV3 = sqlSelectCommandDCTqueryV3 + "SELECT " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + ", COUNT(*) FROM " + trainTableName + " WHERE " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + " = " + dctCoeffNumValString;


			char* sqlSelectCommandDCTqueryV3CharStar = const_cast<char*>(sqlSelectCommandDCTqueryV3.c_str());
			int64_t numberOfDCTcomparisons;

			query_state = mysql_query(connection, sqlSelectCommandDCTqueryV3CharStar);
			if (query_state !=0)
			{
				cout << mysql_error(connection) << endl;
			}
			else
			{
				int tableIndex = 0;
				result = mysql_store_result(connection);

				if((row = mysql_fetch_row(result)) != NULL)
				{
					numberOfDCTcomparisons = int64_t(atof(row[1]));
				}
			}
			mysql_free_result(result);

			if(numberOfDCTcomparisons >= 1)
			{
				numberOfSuccessfulDCTcomparisons++;
			}
		}
		if(numberOfSuccessfulDCTcomparisons > ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V3_MIN_NUM_SUCC_COMPARISONS_REUQIRED)
		{

	#elif defined ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V4
		int numberOfSuccessfulDCTcomparisons = 0;

		#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			char dctCoeffArrayBinnedString[1000];
			int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
			int concatonatedDctCoeffArrayBiasIntNOTUSED[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS];
			//convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffNumValString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedDctCoeffArrayBiasIntNOTUSED);
			#else
			char dctCoeffNumValString[1000];
			int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
			//convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffNumValString, &DCTCoeff64BitValueStringLengthNOTUSED);
			#endif
		#else
			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			int concatonatedDctCoeffArrayBiasIntNOTUSED[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS];
			//uint64_t dctCoeffArrayBinned = convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, concatonatedDctCoeffArrayBiasIntNOTUSED);
			#else
			uint64_t dctCoeffArrayBinned = convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement);
			#endif
			char dctCoeffNumValString[25];
			sprintf(dctCoeffNumValString, "%ld", dctCoeffArrayBinned);
		#endif

		sqlSelectCommandP10 = sqlSelectCommandP10 + " AND " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL + " = " + dctCoeffNumValString;		//only query train objects
	#endif
	}

	if(ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON)
	{
	#ifdef ATOR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V2
		for(int x=0; x<smallImageWidth; x++)
		{
			for(int y=0; y<smallImageHeight; y++)
			{
				colour currentSmallImageValue;
				RTpixelMaps.getRGBMapValues(x, y, smallImageWidth, rgb8bitSmallMapForInstantDBqueryAccessRequirement, &currentSmallImageValue);

				for(int colour=0; colour < 3; colour++)
				{
					char normalisedHueDevValueMaxString[10];
					char normalisedHueDevValueMinString[10];
					uchar maxVal;
					uchar minVal;
					uchar avgVal;

					string normalisedHueDevName;
					if(colour == 0)
					{
						avgVal = currentSmallImageValue.r;
						normalisedHueDevName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_R
					}
					else if(colour == 1)
					{
						avgVal = currentSmallImageValue.g;
						normalisedHueDevName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_G;
					}
					else
					{
						avgVal = currentSmallImageValue.b;
						normalisedHueDevName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_B;
					}

					char rgbValueRequirementMinString[10];
					rgbValueRequirementMinString = SHAREDvars.convertIntToString(avgVal-1);

					char rgbxNameRequirementString[10];
					rgbxNameRequirementString = SHAREDvars.convertIntToString(x+1);
					char rgbyNameRequirementString[10];
					rgbyNameRequirementString = SHAREDvars.convertIntToString(y+1);

					char rgbValueRequirementMaxString[10];
					rgbValueRequirementMaxString = SHAREDvars.convertIntToString(avgVal+1);

					sqlSelectCommandP11 = sqlSelectCommandP11 + " AND "
					normalisedHueDevName + "y" + rgbyNameRequirementString + "x" + rgbxNameRequirementString " >= " + rgbValueRequirementMinString +
					normalisedHueDevName + "y" + rgbyNameRequirementString + "x" + rgbxNameRequirementString " <= " + rgbValueRequirementMaxString;
				}
			}
		}
	#elif defined ATOR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V3
		int numberOfSuccessfulSmallImagecomparisons = 0;
		for(int x=0; x<smallImageWidth; x++)
		{
			for(int y=0; y<smallImageHeight; y++)
			{
				colour currentSmallImageValue;
				RTpixelMaps.getRGBMapValues(x, y, smallImageWidth, rgb8bitSmallMapForInstantDBqueryAccessRequirement, &currentSmallImageValue);

				for(int colour=0; colour < 3; colour++)
				{
					char normalisedHueDevValueMaxString[10];
					char normalisedHueDevValueMinString[10];
					uchar maxVal;
					uchar minVal;
					uchar avgVal;

					string normalisedHueDevName;
					if(colour == 0)
					{
						avgVal = currentSmallImageValue.r;
						normalisedHueDevName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_R
					}
					else if(colour == 1)
					{
						avgVal = currentSmallImageValue.g;
						normalisedHueDevName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_G;
					}
					else
					{
						avgVal = currentSmallImageValue.b;
						normalisedHueDevName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_B;
					}

					char rgbValueRequirementString[10];
					rgbValueRequirementString = SHAREDvars.convertIntToString(avgVal);

					char rgbxNameRequirementString[10];
					rgbxNameRequirementString = SHAREDvars.convertIntToString(x+1);
					char rgbyNameRequirementString[10];
					rgbyNameRequirementString = SHAREDvars.convertIntToString(y+1);


					//eg SELECT columnname, COUNT(*) FROM tablename
					string sqlSelectCommandSmallImageComparisonqueryV3 = "";
					string cellName = normalisedHueDevName + "y" + rgbyNameRequirementString + "x" + rgbxNameRequirementString;
					string sqlSelectCommandSmallImageComparisonqueryV3 = sqlSelectCommandSmallImageComparisonqueryV3 + "SELECT " cellName + ", COUNT(*) FROM " + ATOR_MYSQL_TABLE_NAME + " WHERE " + cellName + " = " + rgbValueRequirementString;

					char* sqlSelectCommandSmallImageComparisonqueryV3CharStar = const_cast<char*>(sqlSelectCommandSmallImageComparisonqueryV3.c_str());
					int64_t numberOfSmallImagecomparisons;

					query_state = mysql_query(connection, sqlSelectCommandSmallImageComparisonqueryV3CharStar);
					if (query_state !=0)
					{
						cout << mysql_error(connection) << endl;
					}
					else
					{
						int tableIndex = 0;
						result = mysql_store_result(connection);

						if((row = mysql_fetch_row(result)) != NULL)
						{
							cout << "row = " << row << endl;
							row = mysql_fetch_row(result);
							numberOfSmallImagecomparisons = int64_t(atof(row[1]));
						}

					}
					mysql_free_result(result);

					if(numberOfSmallImagecomparisons >= 1)
					{
						numberOfSuccessfulSmallImagecomparisons++;
					}
				}
			}
		}

		if(numberOfSuccessfulSmallImagecomparisons > ATOR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V3_MIN_NUM_SUCC_COMPARISONS_REUQIRED_NORMALISED*smallImageWidth*smallImageHeight*3)
		{
	#endif
	}
#endif

	int numFeatures;
	if(ATOR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES)
	{
		numFeatures = ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;
	}
	else
	{
		numFeatures = ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS;
	}

	sqlSelectCommandP2 = createSQLSelectRowCommand(numFeatures);

	//do not extract snapshot images (small or large) from DB at this stage of development

	sqlSelectCommand = sqlSelectCommandP1 + sqlSelectCommandP2 + sqlSelectCommandP3 + sqlSelectCommandP4 + sqlSelectCommandP5 + sqlSelectCommandP6 + sqlSelectCommandP7 + sqlSelectCommandP8 + sqlSelectCommandP9 + sqlSelectCommandP10 + sqlSelectCommandP11 + sqlSelectCommandP12;
	char* sqlSelectCommandCharStar = const_cast<char*>(sqlSelectCommand.c_str());


	query_state = mysql_query(connection, sqlSelectCommandCharStar);
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);
		ATORfeatureContainer* currentFeatureContainerInList = firstFeatureContainerInList;

		while((row = mysql_fetch_row(result)) != NULL)
		{
			ATORfeature* firstNewFeature = new ATORfeature();
			currentFeatureContainerInList->firstFeatureInFeatureList = firstNewFeature;
			ATORfeature* currentFeatureInList = currentFeatureContainerInList->firstFeatureInFeatureList;

			addSQLRowDataToFeatureList(row, firstNewFeature, createFeatureObjects, ignoreOTfeatures, numFeatures);

			tableIndex++;

			ATORfeatureContainer* newFeatureContainer = new ATORfeatureContainer();
			currentFeatureContainerInList->next = newFeatureContainer;
			currentFeatureContainerInList = currentFeatureContainerInList->next;
		}

	}
	mysql_free_result(result);

		#ifdef ATOR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V3
		}
		#endif
	#ifdef ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V3
	}
	#endif
}

void ATORdatabaseSQLClass::addSQLRowDataToFeatureList(const MYSQL_ROW row, ATORfeature* firstFeatureInList, const bool createFeatureObjects, const bool ignoreOTfeatures, const int numFeatures)
{
	if(!createFeatureObjects)
	{
		cerr << "error; createFeaturesListUsingDatabaseQueryGeoXYbinRequirement requires createFeatureObjects" << endl;
		exit(EXIT_ERROR);
	}

	ATORfeature* currentFeatureInList = firstFeatureInList;

	int fieldIndex = 0;

	char* tableIDCharStar;
	char* trainOrTestStringCharStar;
	char* objectNameCharStar;
	char* viewIndexStringCharStar;
	char* zoomIndexStringCharStar;
	char* polyIndexStringCharStar;
	char* sideIndexStringCharStar;
	#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	char* OTpointIndexStringCharStar;
	#endif
	char* pointTransformedxCharStar;
	char* pointTransformedyCharStar;
	char* pointTransformedzCharStar;
	#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
	char* pointxCharStar;
	char* pointyCharStar;
	char* pointzCharStar;
	#endif
	char* pBinxCharStar;
	char* pBinyCharStar;
	char* pBinxyCharStar;
	//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	char* dctCoeffNumStringCharStar;
	char* dctCoeffArrayBinnedCharStar;
	//#endif
	//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	char* colAvgRBinStringCharStar;
	char* colAvgGBinStringCharStar;
	char* colAvgBBinStringCharStar;
	char* colAvgBinStringCharStar;
	//#endif
	//#ifdef ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE
	char* allTextCharStar;
	//#endif


	int64_t tableID;
	int trainOrTest;
	string objectName;
	int viewIndex;
	int zoomIndex;
	int polyIndex;
	int sideIndex;
	#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	int OTpointIndex[ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_MAX];
	#endif
	//#ifdef ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
	int pBinx[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
	int pBiny[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
	int64_t pBinxy;
	//#endif
	vec pointTransformed[ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_MAX];
	#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
	vec point[ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_MAX];
	#endif
	//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	schar dctCoefficients[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	uint64_t dctCoeffArrayBinned;
	//#endif
	//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	colour colAvg;
	//#endif

	tableIDCharStar = row[fieldIndex++];
	objectNameCharStar = row[fieldIndex++];
	trainOrTestStringCharStar = row[fieldIndex++];
	viewIndexStringCharStar = row[fieldIndex++];
	zoomIndexStringCharStar = row[fieldIndex++];
	polyIndexStringCharStar = row[fieldIndex++];
	sideIndexStringCharStar = row[fieldIndex++];


	tableID = (int64_t)(atof(tableIDCharStar));
	objectName = objectNameCharStar;
	trainOrTest = (bool)(atof(trainOrTestStringCharStar));
	viewIndex = (int)(atof(viewIndexStringCharStar));
	zoomIndex = (int)(atof(zoomIndexStringCharStar));
	polyIndex = (int)(atof(polyIndexStringCharStar));
	sideIndex = (int)(atof(sideIndexStringCharStar));

	int numFoundFeatures = 0;	//added 14 June 2012 (for the case where there is less features recorded in sql than expected / eg 4]
	bool stillFindingFeatures = true;
	uint64_t* lengths;
	lengths = mysql_fetch_lengths(result);
	for(int featureNum=0; featureNum<numFeatures; featureNum++)
	{
		if(lengths[fieldIndex+1] == 0)	//index of pointTransformedxCharStar [as if this is empty, then the entire feature entry is empty]
		{
			stillFindingFeatures = false;
		}

		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		OTpointIndexStringCharStar = row[fieldIndex++];
		#endif
		pointTransformedxCharStar = row[fieldIndex++];
		pointTransformedyCharStar = row[fieldIndex++];
		pointTransformedzCharStar = row[fieldIndex++];;
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
		pointxCharStar = row[fieldIndex++];
		pointyCharStar = row[fieldIndex++];
		pointzCharStar = row[fieldIndex++];
		#endif

		if(stillFindingFeatures)
		{
			#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			OTpointIndex[featureNum] = (int)(atof(OTpointIndexStringCharStar));
			#endif
			pointTransformed[featureNum].x = atof(pointTransformedxCharStar);
			pointTransformed[featureNum].y = atof(pointTransformedyCharStar);
			pointTransformed[featureNum].z = atof(pointTransformedzCharStar);
			#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
			point[featureNum].x = atof(pointxCharStar);
			point[featureNum].y = atof(pointyCharStar);
			point[featureNum].z = atof(pointzCharStar);
			#endif
			numFoundFeatures++;
		}
	}

	if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
	{
		//this information is only used to bin the feature list data, and is not transfered to the feature list
		for(int geobinDimension = 0; geobinDimension < ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; geobinDimension++)
		{
			pBinxCharStar = row[fieldIndex++];
			pBinyCharStar = row[fieldIndex++];
			pBinx[geobinDimension] = (uchar)(atof(pBinxCharStar));
			pBiny[geobinDimension] = (uchar)(atof(pBinyCharStar));
		}
		pBinxyCharStar = row[fieldIndex++];
		pBinxy = (uchar)(atof(pBinxyCharStar));
	}

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		for(int dctCoeffNum = 0; dctCoeffNum <ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{

			dctCoeffNumStringCharStar = row[fieldIndex++];
			dctCoefficients[dctCoeffNum] = (schar)(atof(dctCoeffNumStringCharStar));
		}
		dctCoeffArrayBinnedCharStar = row[fieldIndex++];
		dctCoeffArrayBinned = (uint64_t)(atol(dctCoeffArrayBinnedCharStar));
	}

	if(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
	{
		colAvgRBinStringCharStar = row[fieldIndex++];
		colAvgGBinStringCharStar = row[fieldIndex++];
		colAvgBBinStringCharStar = row[fieldIndex++];
		colAvgBinStringCharStar = row[fieldIndex++];
		colAvg.r = (uchar)(atof(colAvgRBinStringCharStar));
		colAvg.g = (uchar)(atof(colAvgGBinStringCharStar));
		colAvg.b = (uchar)(atof(colAvgBBinStringCharStar));
		//dont use colAvgBinStringCharStar yet...
	}

	if(ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
	{
		allTextCharStar = row[fieldIndex++];
				//NO: rows can contain binary data including null characters; int allTextCharStarLength = strlen(allTextCharStar);
		for(int i=0; i< ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH; i++)
		{
			firstFeatureInList->snapshotMapsText[i] = allTextCharStar[i];
		}
	}


	for(int featureNum=0; featureNum<numFoundFeatures; featureNum++)
	{
		if((!ignoreOTfeatures) || (OTpointIndex[featureNum] == 0))
		{

			currentFeatureInList->objectName = objectName;
			currentFeatureInList->trainOrTest = trainOrTest;
			currentFeatureInList->viewIndex = viewIndex;
			currentFeatureInList->zoomIndex = zoomIndex;
			currentFeatureInList->polyIndex = polyIndex;
			currentFeatureInList->sideIndex = sideIndex;
			#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			currentFeatureInList->OTpointIndex = OTpointIndex[featureNum];
			#endif
			currentFeatureInList->pointTransformed.x = pointTransformed[featureNum].x;
			currentFeatureInList->pointTransformed.y = pointTransformed[featureNum].y;
			currentFeatureInList->pointTransformed.z = pointTransformed[featureNum].z;
			#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
			currentFeatureInList->point.x = point[featureNum].x;
			currentFeatureInList->point.y = point[featureNum].y;
			currentFeatureInList->point.z = point[featureNum].z;
			#endif

			if(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
			{
				currentFeatureInList->avgCol.r = colAvg.r;
				currentFeatureInList->avgCol.g = colAvg.g;
				currentFeatureInList->avgCol.b = colAvg.b;
			}

			if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
			{
				for(int dctCoeffNum = 0; dctCoeffNum <ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
				{
					currentFeatureInList->dctCoeff[dctCoeffNum] = dctCoefficients[dctCoeffNum];
				}
				currentFeatureInList->dctCoeffArrayBinned = dctCoeffArrayBinned;
			}


			if(createFeatureObjects)
			{
				ATORfeature* newFeature = new ATORfeature();
				currentFeatureInList->next = newFeature;
			}
			currentFeatureInList=currentFeatureInList->next;
		}
	}
}



string ATORdatabaseSQLClass::createSQLSelectRowCommand(const int numFeatures)
{
	string sqlSelectCommand = "";
	sqlSelectCommand = sqlSelectCommand + ATOR_MYSQL_FIELD_NAME_ID + ", " + ATOR_MYSQL_FIELD_NAME_OBJECTNAME + ", " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM + ", " + ATOR_MYSQL_FIELD_NAME_VIEWNUM + ", " + ATOR_MYSQL_FIELD_NAME_ZOOMNUM + ", " + ATOR_MYSQL_FIELD_NAME_POLYNUM + ", " + ATOR_MYSQL_FIELD_NAME_SIDENUM;

	for(int featureNum=0; featureNum<numFeatures; featureNum++)
	{
		string featureNumString = SHAREDvars.convertIntToString(featureNum+1);

		#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_OTNUM + featureNumString;
		#endif
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDX + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDY + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDZ + featureNumString;
		#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_POINTX + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTY + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTZ + featureNumString;
		#endif
	}

	if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
	{
		//this information is only used to bin the feature list data, and is not transfered to the feature list
		for(int geobinDimension = 0; geobinDimension < ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; geobinDimension++)
		{
			string geobinDimensionString = SHAREDvars.convertIntToString(geobinDimension+1);
			sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_X + geobinDimensionString + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y + geobinDimensionString;
		}
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_XY;
	}

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		for(int dctCoeffNum = 0; dctCoeffNum <ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{
			string dctCoeffNumString = SHAREDvars.convertIntToString(dctCoeffNum);

			sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumString;
		}
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL;
	}

	if(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
	{
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_R_BINS + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_G_BINS + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_B_BINS + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
	}

	if(ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
	{
		sqlSelectCommand = sqlSelectCommand + ", " + ATOR_MYSQL_FIELD_NAME_ALL_IMAGE_TEXT;
	}

	return sqlSelectCommand;
}






void ATORdatabaseSQLClass::insertTransformedFeatureListIntoDatabase(ATORfeature* firstFeatureInList, const string objectName, const int viewIndex, const int zoomIndex, const int polyIndex, const int sideIndex, const int trainOrTest, const bool ignoreOTfeatures, uchar* rgb8bitSmallMapForInstantDBqueryAccess, int smallImageWidth, const int smallImageHeight, const bool addPermutationsOfTrainFeaturesForGeoBinning, const int maxNumFeaturePermutations, const string tableName, int64_t* databaseTableSize)
{
	ATORfeature* currentFeatureInTempList = firstFeatureInList;

	//now bin the features;

	int findex1 = 0;

	while((currentFeatureInTempList->next != NULL) && (findex1 < maxNumFeaturePermutations))
	{
		ATORfeature* currentFeatureInTempList2 = firstFeatureInList;

		if((!ignoreOTfeatures) || (currentFeatureInTempList->OTpointIndex == 0) || !addPermutationsOfTrainFeaturesForGeoBinning)
		{//perform binning of nearby features only (not OT features)

			int findex2 = 0;
			while((currentFeatureInTempList2->next != NULL) && (findex2 < maxNumFeaturePermutations))
			{
				if((!ignoreOTfeatures) || (currentFeatureInTempList2->OTpointIndex == 0) || !addPermutationsOfTrainFeaturesForGeoBinning)
				{//perform binning of nearby features only (not OT features)

					if((findex1 != findex2) || !addPermutationsOfTrainFeaturesForGeoBinning)
					{
						string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
						string sqlInsertCommandP1 = "INSERT INTO ";
						string sqlInsertCommandP2 = tableName;
						string sqlInsertCommandP3 = " (";
						string sqlInsertCommandP4 = "";
						string sqlInsertCommandP5 = ") VALUES (";
						string sqlInsertCommandP6 = "";
						string sqlInsertCommandP7 = ");";

						string tableIDString = SHAREDvars.convertLongToString(*databaseTableSize);
						string trainOrTestString = SHAREDvars.convertIntToString(((int)trainOrTest));
						string viewIndexString = SHAREDvars.convertIntToString(viewIndex);
						string zoomIndexString = SHAREDvars.convertIntToString(zoomIndex);
						string polygonIndexString = SHAREDvars.convertIntToString(polyIndex);
						string sideIndexString = SHAREDvars.convertIntToString(sideIndex);

						sqlInsertCommandP4 = sqlInsertCommandP4 + ATOR_MYSQL_FIELD_NAME_ID + ", " + ATOR_MYSQL_FIELD_NAME_OBJECTNAME + ", " + ATOR_MYSQL_FIELD_NAME_TRAINORTESTNUM + ", " + ATOR_MYSQL_FIELD_NAME_VIEWNUM + ", " + ATOR_MYSQL_FIELD_NAME_ZOOMNUM + ", " + ATOR_MYSQL_FIELD_NAME_POLYNUM + ", " + ATOR_MYSQL_FIELD_NAME_SIDENUM;
						sqlInsertCommandP6 = sqlInsertCommandP6 + tableIDString + ", " + "'" + objectName + "'" + ", " + trainOrTestString + ", " + viewIndexString + ", " + zoomIndexString + ", " + polygonIndexString + ", " + sideIndexString;

						bool insideBin = false;

						int geoxBin[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						int geoyBin[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						geoxBin[0] = determineGeoBinX(currentFeatureInTempList->pointTransformed.x);
						geoyBin[0] = determineGeoBinY(currentFeatureInTempList->pointTransformed.y);
						geoxBin[1] = determineGeoBinX(currentFeatureInTempList2->pointTransformed.x);
						geoyBin[1] = determineGeoBinY(currentFeatureInTempList2->pointTransformed.y);

						if(addPermutationsOfTrainFeaturesForGeoBinning)
						{
							if((geoxBin[0] >= 0)
							&& (geoxBin[0] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
							&& (geoyBin[0] >= 0)
							&& (geoyBin[0] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
							&& (geoxBin[1] >= 0)
							&& (geoxBin[1] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
							&& (geoyBin[1] >= 0)
							&& (geoyBin[1] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS))
							{
								insideBin = true;
								//this passes; cout << "inside bin during array formation - good" << endl;
							}

							if(insideBin)
							{
								for(int featureNum=0; featureNum<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; featureNum++)
								{
									ATORfeature* currentFeature;

									if(featureNum == 0)
									{
										currentFeature = currentFeatureInTempList;
									}
									else
									{
										currentFeature = currentFeatureInTempList2;
									}

									string featureNumString = SHAREDvars.convertIntToString(featureNum+1);

									#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									string OTpointIndexString = SHAREDvars.convertIntToString(currentFeature->OTpointIndex);
									#endif

									string transformedpositionCoordinatesString = LDreferenceManipulation.convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->pointTransformed));
									#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
									string positionCoordinatesString = LDreferenceManipulation.convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->point));
									#endif

									#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_OTNUM + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + OTpointIndexString;
									#endif
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDX + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDY + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + transformedpositionCoordinatesString;
									#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_POINTX + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTY + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + positionCoordinatesString;
									#endif

									currentFeature = currentFeature->next;
								}

								if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
								{
									for(int geobinDimension=0; geobinDimension<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; geobinDimension++)
									{
										string geobinDimensionString = SHAREDvars.convertIntToString(geobinDimension+1);


										string geobinxString = SHAREDvars.convertIntToString(geoxBin[geobinDimension]);
										string geobinyString = SHAREDvars.convertIntToString(geoyBin[geobinDimension]);

										sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_X + geobinDimensionString + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y + geobinDimensionString;
										sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + geobinxString + ", " + geobinyString;

									}
								}
							}
						}

						ATORfeature* currentFeature = firstFeatureInList;

						bool storeAllNearbyAndOTFeaturesDuringTrain;
						if(ATOR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES)
						{
							storeAllNearbyAndOTFeaturesDuringTrain = insideBin;
						}
						else
						{
							storeAllNearbyAndOTFeaturesDuringTrain = false;
						}
						if(!addPermutationsOfTrainFeaturesForGeoBinning || storeAllNearbyAndOTFeaturesDuringTrain)
						{

							int featurePositionModifier;
							if(addPermutationsOfTrainFeaturesForGeoBinning)
							{
								featurePositionModifier = ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS;
							}
							else
							{
								featurePositionModifier = 0;
							}

							currentFeature = firstFeatureInList;
							int featureNum = 0;		//nearby feature / OT feature
							while(currentFeature->next != NULL)
							{

								if(addPermutationsOfTrainFeaturesForGeoBinning || ((!ignoreOTfeatures) || (currentFeature->OTpointIndex == 0)))
								{//perform binning of nearby features only (not OT features)

									string featureNumString = SHAREDvars.convertIntToString(featureNum+1+featurePositionModifier);		//if ATOR_METHOD_GEOMETRIC_COMPARISON_BINNING, add all OT and nearby feature points After the binned nearby feature points

									#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									string OTpointIndexString = SHAREDvars.convertIntToString(currentFeature->OTpointIndex);
									#endif

									string transformedpositionCoordinatesString = LDreferenceManipulation.convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->pointTransformed));
									#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
									string positionCoordinatesString = LDreferenceManipulation.convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->point));
									#endif

									#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_OTNUM + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + OTpointIndexString;
									#endif
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDX + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDY + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTTRANSFORMEDZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + transformedpositionCoordinatesString;
									#ifdef ATOR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_VERBOSE
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_POINTX + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTY + featureNumString + ", " + ATOR_MYSQL_FIELD_NAME_POINTZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + positionCoordinatesString;
									#endif

									featureNum++;
								}

								currentFeature = currentFeature->next;
							}
						}

						//what is the following used for??? (ATOR_MYSQL_FIELD_NAME_GEO_BINS_X/ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y data from test table in SQL database should not be used - I guess it is for a succssful SQL select query only - may as well place dummy data into here)
						if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							if(!addPermutationsOfTrainFeaturesForGeoBinning)
							{//during creation of test objects

								currentFeature = firstFeatureInList;

								int geobinDimension = 0;
								int geoxBin[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
								int geoyBin[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
								while((currentFeature->next != NULL) && (geobinDimension < ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS))
								{
									if((!ignoreOTfeatures) || (currentFeature->OTpointIndex == 0))
									{//perform binning of nearby features only (not OT features)

										string geobinDimensionString = SHAREDvars.convertIntToString(geobinDimension+1);

										geoxBin[geobinDimension] = determineGeoBinX(currentFeature->pointTransformed.x);
										geoyBin[geobinDimension] = determineGeoBinY(currentFeature->pointTransformed.y);


										string geobinxString = SHAREDvars.convertIntToString(geoxBin[geobinDimension]);
										string geobinyString = SHAREDvars.convertIntToString(geoyBin[geobinDimension]);

										sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_X + geobinDimensionString + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_Y + geobinDimensionString;
										sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + geobinxString + ", " + geobinyString;

										geobinDimension++;
									}

									currentFeature = currentFeature->next;
								}
							}
						}



						if(!addPermutationsOfTrainFeaturesForGeoBinning || insideBin)
						{
							if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
							{
								int64_t geoxyBin = calculateGeoxyBinMultiDimensional(geoxBin, geoyBin);

								string geobinxyString = SHAREDvars.convertIntToString(geoxyBin);

								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_GEO_BINS_XY;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + geobinxyString;
							}


							if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
							{
								currentFeature = firstFeatureInList;
								for(int dctCoeffNum = 0; dctCoeffNum <ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
								{
									string dctCoeffNumString = SHAREDvars.convertIntToString(dctCoeffNum);

									string dctCoeffValString = SHAREDvars.convertIntToString(currentFeature->dctCoeff[dctCoeffNum]);

									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + dctCoeffValString;
								}

								#ifdef ATOR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK
								uint64_t linearCombination = 0;
								convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(currentFeature->dctCoeff, geoxBin, geoyBin, &linearCombination);
								string dctCoeffArrayBinnedString = SHAREDvars.convertLongToString(linearCombination);
								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + dctCoeffArrayBinnedString;
								#else
								string dctCoeffArrayBinnedString = SHAREDvars.convertLongToString(currentFeature->dctCoeffArrayBinned);
								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + dctCoeffArrayBinnedString;
								#endif
							}

							if(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
							{
								currentFeature = firstFeatureInList;
								string colAvgRBinString = SHAREDvars.convertIntToString(currentFeature->avgCol.r);
								string colAvgGBinString = SHAREDvars.convertIntToString(currentFeature->avgCol.g);
								string colAvgBBinString = SHAREDvars.convertIntToString(currentFeature->avgCol.b);
								int currentCombinergbBin = int(currentFeature->avgCol.r)*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + int(currentFeature->avgCol.g*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL) + int(currentFeature->avgCol.b);
								string colAvgBinString = SHAREDvars.convertIntToString(currentCombinergbBin);

								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_R_BINS + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_G_BINS + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_B_BINS + ", " + ATOR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + colAvgRBinString + ", " + colAvgGBinString + ", " + colAvgBBinString + ", " + colAvgBinString;
							}

							if(ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON)
							{
								#ifdef ATOR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V2_OR_V3
								for(int y=0; y<smallImageHeight;y++)
								{
									string simystring = SHAREDvars.convertIntToString(y);

									for(int x=0; x<smallImageWidth;x++)
									{
										string simxstring = SHAREDvars.convertIntToString(x);

										colour tempCol;
										RTpixelMaps.getRGBMapValues(x, y, smallImageWidth, rgb8bitSmallMapForInstantDBqueryAccess, &tempCol);
										for(int c = 0; c< RGB_NUM; c++)
										{
											string colourName;
											uchar colourVal;
											if(c == 0)
											{
												colourName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_R;
												colourVal = tempCol.r;
											}
											else if(c == 1)
											{
												colourName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_G;
												colourVal = tempCol.g;
											}
											else
											{
												colourName = ATOR_MYSQL_FIELD_NAME_SMALL_IMAGE_B;
												colourVal = tempCol.b;
											}

											string colourValString = SHAREDvars.convertIntToString(colourVal);

											sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + colourName + "y" + simystring + "x" + simxstring;
											sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + colourValString;
										}
									}
								}
								#endif
							}

							if(!ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
							{

								//do not extract snapshot images (small or large) from DB at this stage of development


								sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6 + sqlInsertCommandP7;
								char* sqlInsertCommandCharStar = const_cast<char*>(sqlInsertCommand.c_str());
								LDmysql.performSQLinsertQuery(sqlInsertCommandCharStar);
								*databaseTableSize = *databaseTableSize + 1;

							}
							else
							{
								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + ATOR_MYSQL_FIELD_NAME_ALL_IMAGE_TEXT;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", ";
								sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6;
								char sqlInsertCommandCharStar[ATOR_IMAGE_COMPARISON_SQL_DATABASE_TEST_AND_TRAIN_TABLES_INSERT_QUERY_MAX_LENGTH];
								int i;

								for(i=0; i <sqlInsertCommand.length(); i++)
								{
	 								sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
								}

								//must be done this way as decisionTreeBinText may contain null characters


								sqlInsertCommandCharStar[i] = '\'';
								i++;
								currentFeature = firstFeatureInList;
								i = i + mysql_real_escape_string(&mysql, &(sqlInsertCommandCharStar[i]), currentFeature->snapshotMapsText, ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH);
								sqlInsertCommandCharStar[i] = '\'';
								i++;

								sqlInsertCommandCharStar[i] = ')';
								i++;
								sqlInsertCommandCharStar[i] = ';';
								i++;

								LDmysql.performSQLrealInsertQuery(sqlInsertCommandCharStar, i);

								*databaseTableSize = *databaseTableSize + 1;
							}
						}
					}
				}
				findex2++;
				currentFeatureInTempList2 = currentFeatureInTempList2->next;
			}
		}
		findex1++;
		currentFeatureInTempList = currentFeatureInTempList->next;
	}
}







int64_t ATORdatabaseSQLClass::powLong(const int64_t val, const int degree)
{
	int64_t result = 1;
	for(int i=1; i<degree; i++)
	{
		result = result*val;
	}
	return result;
}


#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength, int concatonatedDctCoeffArrayBiasInt[])
#else
//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength)
#endif
#else
#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
//uint64_t convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], int concatonatedDctCoeffArrayBiasInt[])
#else
uint64_t ATORdatabaseSQLClass::convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[])
#endif
#endif
{
	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	*DCTcoeff64bitValueStringLength = 0;
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#else
	uint64_t dctCoeffArrayBinned = 0;
	#endif

	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		int arrayValueSigned = concatonatedSignedDctCoeffArray[i];

		/*
		//OLD; perform interpolation before signed to unsigned conversion

		//perform binning only if necessary/requested
		int numDistintValsPerColumn = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
		if(numDistintValsPerColumn != 1)
		{
			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			double arrayValueSignedDouble = double(arrayValueSigned)/double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL);
			arrayValueSigned = arrayValueSigned/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;

			//cout << "arrayValueSignedDouble = " << arrayValueSignedDouble << endl;
			//cout << "arrayValueSigned = " << arrayValueSigned << endl;

			if(arrayValueSigned >= 0)
			{
				if(arrayValueSignedDouble >= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

				}
				else if(arrayValueSignedDouble <= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;

				}
				else
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
				}
			}
			else
			{//arrayValueSigned < 0

				if(arrayValueSignedDouble >= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

				}
				else if(arrayValueSignedDouble <= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;

				}
				else
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
				}
			}
			#else
			arrayValueSigned = arrayValueSigned/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
			#endif
		}

		if(arrayValueSigned > ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
		{
			arrayValueSigned = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
		}
		else if(arrayValueSigned < -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
		{
			arrayValueSigned = -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
		}
		int arrayValueUnsigned = arrayValueSigned + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;	//convert to unsigned
		*/


		double arrayValueUnsignedDouble;
		uint32_t arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);	//used to be int

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int numDistintValsPerColumn = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
		if(numDistintValsPerColumn != 1)
		{

			if(arrayValueUnsignedDouble >= (double(arrayValueUnsigned)+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
			{
				concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

			}
			else if(arrayValueUnsignedDouble <= (double(arrayValueUnsigned)+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
			{
				concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;
			}
			else
			{
				concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
			}
		}
		#endif


	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		//cout << "arrayValueUnsigned = " << arrayValueUnsigned << endl;
		for(int q=0; q<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS; q++)
		{
			if(index == 8)
			{
				index = 0;
				DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = binaryConvertedToChar;
				*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
				binaryConvertedToChar = 0x00;
			}

			bool bitValue = false;
			int arrayValueUnsignedAfterBitShift = int(arrayValueUnsigned) >> q;
			//cout << "arrayValueUnsignedAfterBitShift = " << arrayValueUnsignedAfterBitShift << endl;
			if(arrayValueUnsignedAfterBitShift%2 == 0)
			{//LSb == 0 ; therefore an even number
				bitValue = false;
			}
			else
			{//LSb == 1 ; therefore an odd number
				bitValue =  true;
			}

			if(bitValue)
			{
				binaryConvertedToChar = binaryConvertedToChar | (0x01 << index);
			}
			else
			{
				binaryConvertedToChar = binaryConvertedToChar | (0x00 << index);
			}

			index++;
		}

	#else
		int power = ((ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS-i-1)*2)+1; 	//13,11,9,7,5,3,1
		dctCoeffArrayBinned = dctCoeffArrayBinned + int64_t(arrayValueUnsigned)*powLong(10, power);
	#endif
	}

	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	//add final character to currentDirectoryCharStar string if necessary
	if(index > 0)
	{
		DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = binaryConvertedToChar;
		*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
	}

	//add null character onto end of string
	DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = '\0';
	*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
	#else
	return dctCoeffArrayBinned;
	#endif
}





int64_t ATORdatabaseSQLClass::calculateGeoxyBinMultiDimensional(const int geoxBin[], const int geoyBin[])
{
	int64_t geoxyBin;
	if(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS == 2)
	{

		geoxyBin =
			 ((int64_t(geoyBin[0])* ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS* ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS* ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
			+ (int64_t(geoxBin[0])* ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS* ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
			+ (int64_t(geoyBin[1])* ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
			+ (int64_t(geoxBin[1])));

	}
	else
	{
		cout << "ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS != 2; this scenario has not been programmed" << endl;
	}
	return geoxyBin;
}



void ATORdatabaseSQLClass::convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(const schar concatonatedSignedDctCoeffArray[], const int geoxBin[], const int geoyBin[], uint64_t* linearCombination)
{
	//int linearCombinationArray[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS + ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
	int linearCombinationArray[ATOR_LINEAR_COMBINATION_ARRAY_MAX_SIZE];

	int index = 0;
	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		int arrayValueSigned = concatonatedSignedDctCoeffArray[i];

		double arrayValueUnsignedDouble;
		uint32_t arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);		//used to be int

		linearCombinationArray[index] = arrayValueUnsigned;
		index++;
	}
	for(int i=0; i<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; i++)
	{
		linearCombinationArray[index] =  int(geoxBin[i]);
		index++;
	}
	for(int i=0; i<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; i++)
	{
		linearCombinationArray[index] = int(geoyBin[i]);
		index++;
	}
	double linearCombinationDouble = 1;
	for(int i=0; i<index; i++)
	{
		double subsetDouble = 0;
		for(int j=0; j<=i; j++)
		{
			subsetDouble = subsetDouble + linearCombinationArray[j];
		}
		linearCombinationDouble = linearCombinationDouble* subsetDouble;

	}
	*linearCombination = (uint64_t)(linearCombinationDouble/ATOR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK_DOUBLE_TO_U_LONG_CONVERSION);
}


int ATORdatabaseSQLClass::determineGeoBinX(const double featurePointTransformedXpos)
{
	int geoBinX = (featurePointTransformedXpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
	return geoBinX;
}

int ATORdatabaseSQLClass::determineGeoBinY(const double featurePointTransformedYpos)
{
	int geoBinY = (featurePointTransformedYpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
	return geoBinY;
}

double ATORdatabaseSQLClass::determineGeoBinDoubleX(const double featurePointTransformedXpos)
{
	double geoBinX = (featurePointTransformedXpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
	return geoBinX;
}

double ATORdatabaseSQLClass::determineGeoBinDoubleY(const double featurePointTransformedYpos)
{
	double geoBinY = (featurePointTransformedYpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
	return geoBinY;
}


uint32_t ATORdatabaseSQLClass::determineDCTBinUnsigned(int arrayValueSigned, double* arrayValueUnsignedDouble)
{
	if(arrayValueSigned > ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		arrayValueSigned = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else if(arrayValueSigned < -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		arrayValueSigned = -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}

	uint32_t arrayValueUnsigned;
	int numDistintValsPerColumn = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	if(numDistintValsPerColumn != 1)
	{
		*arrayValueUnsignedDouble = determineDCTBinUnsignedDouble(arrayValueSigned);
		arrayValueUnsigned = (uint32_t)(*arrayValueUnsignedDouble);
	}
	else
	{
		arrayValueUnsigned = arrayValueSigned + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
	}

	return arrayValueUnsigned;
}

double ATORdatabaseSQLClass::determineDCTBinUnsignedDouble(const int arrayValueSigned)
{
	double arrayValueUnsignedDouble = double(arrayValueSigned)/double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL) + double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET);
	return arrayValueUnsignedDouble;
}



//#endif



