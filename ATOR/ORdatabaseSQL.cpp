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
 * File Name: ORdatabaseSQL.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3h15b 29-February-2016
 *
 *******************************************************************************/


#include "ORdatabaseSQL.h"
#include "LDreferenceManipulation.h"
#include "RTpixelMaps.h"

//move this function elsewhere;
int countIncrements(int maxIncrement)
{
	int total = 0;
	for(int i=0; i<=maxIncrement; i++)
	{
		total = total+i;
	}
	return total;
}

//#ifdef OR_IMAGE_COMPARISON_SQL
long databaseTableSizeTrainInitial;
long databaseTableSizeTrain;
long databaseTableSizeTest;
long databaseTableSizeDecisionTreeInitial;
long databaseTableSizeDecisionTree;



void convertSnapshotMapsToStringForSQLdatabaseEntry(int imageWidthFacingPoly, int imageHeightFacingPoly, unsigned char* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, double* depthMapFacingPoly, int smallImageWidth, int smallImageHeight, unsigned char* rgbMapSmallFacingPoly, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, int dimension, char* snapshotMapsText, double depthScale, bool compareRgbDevMapsNotRgbMaps, int* stringSize)
{
	int index=0;

	if(!compareRgbDevMapsNotRgbMaps)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour col;
				getRGBMapValues(x, y, imageWidthFacingPoly, rgbMapFacingPoly, &col);
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
				getVectorMapValue(x, y, imageWidthFacingPoly, rgbDevIEnormalisedHueContrastMapFacingPoly, rgbDev);
				snapshotMapsText[index++] = rgbDev->x* OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->y* OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->z* OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
			}
		}
	}
	if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour depthMap24BitPixelValue;
				double depth = getLumOrContrastOrDepthMapValue(x, y, imageWidthFacingPoly, depthMapFacingPoly);
				calculate24BitDepthPixmapPixelFromDepth(depth, depthScale, 0.0, &depthMap24BitPixelValue);
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
				getRGBMapValues(x, y, smallImageWidth, rgbMapSmallFacingPoly, &col);
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
				getVectorMapValue(x, y, smallImageWidth, depthMapSmallFacingPoly, rgbDev);
				snapshotMapsText[index++] = rgbDev->x* OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->y* OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				snapshotMapsText[index++] = rgbDev->z* OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
			}
		}
	}
	if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				colour depthMap24BitPixelValue;
				double depth = getLumOrContrastOrDepthMapValue(x, y, smallImageWidth, rgbDevIEnormalisedHueContrastMapSmallFacingPoly);
				calculate24BitDepthPixmapPixelFromDepth(depth, depthScale, 0.0, &depthMap24BitPixelValue);
				snapshotMapsText[index++] = depthMap24BitPixelValue.r;
				snapshotMapsText[index++] = depthMap24BitPixelValue.g;
				snapshotMapsText[index++] = depthMap24BitPixelValue.b;
			}
		}
	}

	*stringSize = index;
}


void convertSQLdatabaseStringToSnapshotMaps(int imageWidthFacingPoly, int imageHeightFacingPoly, unsigned char* rgbMapFacingPoly, double* rgbDevIEnormalisedHueContrastMapFacingPoly, double* depthMapFacingPoly, int smallImageWidth, int smallImageHeight, unsigned char* rgbMapSmallFacingPoly, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, double* depthMapSmallFacingPoly, int dimension, char* snapshotMapsText, double depthScale, bool compareRgbDevMapsNotRgbMaps)
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
				setRGBMapValues(x, y, imageWidthFacingPoly, &col, rgbMapFacingPoly);
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
				rgbDev.x = snapshotMapsText[index++]/OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.y = snapshotMapsText[index++]/OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.z = snapshotMapsText[index++]/OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				setVectorMapValue(x, y, imageWidthFacingPoly, &rgbDev, rgbDevIEnormalisedHueContrastMapFacingPoly);

			}
		}
	}
	if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < imageHeightFacingPoly; y++)
		{
  			for(int x = 0; x < imageWidthFacingPoly; x++)
			{
				colour depthMap24BitPixelValue;
				depthMap24BitPixelValue.r = snapshotMapsText[index++];
				depthMap24BitPixelValue.g = snapshotMapsText[index++];
				depthMap24BitPixelValue.b = snapshotMapsText[index++];
				double depth = calculateDepthFrom24BitDepthValue(&depthMap24BitPixelValue,  depthScale, 0.0);
				setLumOrContrastOrDepthMapValue(x, y, imageWidthFacingPoly, depth, depthMapFacingPoly);
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
				setRGBMapValues(x, y, smallImageWidth, &col, rgbMapSmallFacingPoly);
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
				rgbDev.x = snapshotMapsText[index++]/OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.y = snapshotMapsText[index++]/OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				rgbDev.z = snapshotMapsText[index++]/OR_IMAGE_COMPARISON_SQL_HUE_DEV_VAL_TO_8BIT_CONVERSION_MULTIPLIER;
				setVectorMapValue(x, y, smallImageWidth, &rgbDev, rgbDevIEnormalisedHueContrastMapSmallFacingPoly);

			}
		}
	}
	if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		for(int y = 0; y < smallImageHeight; y++)
		{
  			for(int x = 0; x < smallImageWidth; x++)
			{
				colour depthMap24BitPixelValue;
				depthMap24BitPixelValue.r = snapshotMapsText[index++];
				depthMap24BitPixelValue.g = snapshotMapsText[index++];
				depthMap24BitPixelValue.b = snapshotMapsText[index++];
				double depth = calculateDepthFrom24BitDepthValue(&depthMap24BitPixelValue, depthScale, 0.0);
				setLumOrContrastOrDepthMapValue(x, y, smallImageWidth, depth, depthMapSmallFacingPoly);
			}
		}
	}
}



void createFeatureListUsingDatabaseQuery(ORfeature* firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures, string sqlDatabaseTestTableName, long testID, string testObjectName, int testViewIndex, int testZoomIndex, int testPolyIndex, int testSideIndex, bool useTestID)
{
	int query_state;
	MYSQL_ROW row;

	string sqlSelectCommand = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandP1 = "SELECT ";
	string sqlSelectCommandP2 = "";
	string sqlSelectCommandP3 = " FROM ";
	string sqlSelectCommandP4 = "";	//OR_MYSQL_DATABASE_NAME;
	string sqlSelectCommandP5 = "";		//".";
	string sqlSelectCommandP6 = sqlDatabaseTestTableName;
	string sqlSelectCommandP7 = "";
	string sqlSelectCommandP8 = "";
	string sqlSelectCommandP9 = "";
	string sqlSelectCommandP10 = "";
	string sqlSelectCommandP11 = "";
	string sqlSelectCommandP12 = "";

	string testIDString = convertLongToString(testID);
	string testviewIndexString = convertIntToString(testViewIndex);
	string testzoomIndexString = convertIntToString(testZoomIndex);
	string testpolygonIndexString = convertIntToString(testPolyIndex);
	string testsideIndexString = convertIntToString(testSideIndex);

	if(useTestID)
	{
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + OR_MYSQL_FIELD_NAME_ID + " = " + testIDString + ");";	//only query train objects
	}
	else
	{
		#ifdef OR_DEBUG
		//cout << "sqlDatabaseTestTableName = " << sqlDatabaseTestTableName << endl;
		#endif
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + OR_MYSQL_FIELD_NAME_OBJECTNAME + " = '" + testObjectName + "'";	//only query train objects
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM + " = " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TEST;
		sqlSelectCommandP9 = sqlSelectCommandP9 + " AND " + OR_MYSQL_FIELD_NAME_VIEWNUM + " = " + testviewIndexString;
		sqlSelectCommandP10 = sqlSelectCommandP10 + " AND " + OR_MYSQL_FIELD_NAME_ZOOMNUM + " = " + testzoomIndexString;
		sqlSelectCommandP11 = sqlSelectCommandP11 + " AND " + OR_MYSQL_FIELD_NAME_POLYNUM + " = " + testpolygonIndexString;
		sqlSelectCommandP12 = sqlSelectCommandP12 + " AND " + OR_MYSQL_FIELD_NAME_SIDENUM + " = " + testsideIndexString + ");";
	}

	int numFeatures = OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;	//this value is already dependent upon whether ignoreOTfeatures is true of false
	sqlSelectCommandP2 = createSQLSelectRowCommand(numFeatures);

	sqlSelectCommand = sqlSelectCommandP1 + sqlSelectCommandP2 + sqlSelectCommandP3 + sqlSelectCommandP4 + sqlSelectCommandP5 + sqlSelectCommandP6 + sqlSelectCommandP7 + sqlSelectCommandP8 + sqlSelectCommandP9 + sqlSelectCommandP10 + sqlSelectCommandP11 + sqlSelectCommandP12;
	#ifdef OR_SQL_DATABASE_DEBUG
	cout << "sqlSelectCommand = " << sqlSelectCommand << endl;
	#endif
	char* sqlSelectCommandCharStar = const_cast<char*>(sqlSelectCommand.c_str());

	query_state = mysql_query(connection, sqlSelectCommandCharStar);
	#ifdef OR_DEBUG
	//cout << "select qeuery performed" << endl;
	#endif
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
				cout << "error: multiple identical test snapshots detected" << endl;
				#ifdef OR_DEBUG
				/*
				cout << "testObjectName = " << testObjectName << endl;
				cout << "OR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TEST = " << OR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TEST << endl;
				cout << "testviewIndexString = " << testviewIndexString << endl;
				cout << "testzoomIndexString = " << testzoomIndexString << endl;
				cout << "testpolygonIndexString = " << testpolygonIndexString << endl;
				cout << "testsideIndexString = " << testsideIndexString << endl;
				*/
				#endif
				exit(0);

			}
			addSQLRowDataToFeatureList(row, firstFeatureInList, createFeatureObjects, ignoreOTfeatures, numFeatures);
			tableIndex++;
		}

	}
	mysql_free_result(result);
}


//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE
void createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, bool ignoreOTfeatures, char* decisionTreeBinText, int decisionTreeBinTextLength, int trainOrTest)
{

	int query_state;
	MYSQL_ROW row;

	string sqlSelectCommandOpen = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandClose = "";
	string sqlSelectCommandSelect = "SELECT ";
	string sqlSelectCommandSelectContents = "";

#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL_DIRECT_ACCESS_USE_JOIN_OPT
	cout << "SQL join optimisation not yet coded" << endl;
	exit(0);
#else
	string sqlSelectCommandFrom = "";
	string sqlSelectCommandWhereOpen = "";
	string sqlSelectCommandWhereClose = "));";

	sqlSelectCommandFrom = sqlSelectCommandFrom + " FROM " + OR_MYSQL_TABLE_NAME_TRAIN;
	sqlSelectCommandWhereOpen = sqlSelectCommandWhereOpen + " WHERE (" + OR_MYSQL_FIELD_NAME_ID + " IN (";	//only query train objects

	int numFeatures = OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;	//this value is already dependent upon whether ignoreOTfeatures is true of false
	sqlSelectCommandSelectContents = createSQLSelectRowCommand(numFeatures);

	string sqlSelectCommandWhereContentsSelect = "SELECT ";
	string sqlSelectCommandWhereContentsSelectContents = OR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID;
	string sqlSelectCommandWhereContentsFrom = "";
	string sqlSelectCommandWhereContentsWhereOpen = "";
	char sqlSelectCommandWhereContentsWhereContents[OR_IMAGE_COMPARISON_DECISION_TREE_STRING_MAX_LENGTH];
	string sqlSelectCommandWhereContentsWhereClose = ")";

	sqlSelectCommandWhereContentsFrom = sqlSelectCommandWhereContentsFrom + " FROM " + OR_MYSQL_TABLE_NAME_DECISIONTREE;
	if(trainOrTest == TEST)
	{
		sqlSelectCommandWhereContentsWhereOpen = sqlSelectCommandWhereContentsWhereOpen + " WHERE (" + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}
	else if(trainOrTest == TRAIN_AND_TEST)
	{
		string maxDecisionTreeTableRowString = convertLongToString(databaseTableSizeDecisionTreeInitial);
		sqlSelectCommandWhereContentsWhereOpen = sqlSelectCommandWhereContentsWhereOpen + " WHERE (" + OR_MYSQL_FIELD_NAME_ID + " <= " + maxDecisionTreeTableRowString + " AND " + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
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

	char sqlSelectCommandCharStar[OR_IMAGE_COMPARISON_SQL_DATABASE_TEST_AND_TRAIN_TABLES_GET_QUERY_MAX_LENGTH + OR_IMAGE_COMPARISON_DECISION_TREE_STRING_MAX_LENGTH];
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

	#ifdef OR_DEBUG
	/*
	for(int q=0;q<sqlSelectCommandCharStarIndex;q++)
	{
		cout << sqlSelectCommandCharStar[q] << endl;
	}
	*/
	#endif

	#ifdef OR_SQL_DATABASE_DEBUG
	cout << "sqlSelectCommandCharStar = " << sqlSelectCommandCharStar << endl;
	#else
	#ifdef OR_SQL_DATABASE_DEBUG_COMPARISON_ONLY
	cout << "sqlSelectCommandCharStar = " << sqlSelectCommandCharStar << endl;
	#endif
	#endif

	query_state = mysql_real_query(connection, sqlSelectCommandCharStar, sqlSelectCommandCharStarIndex);
	//cout << "select qeuery performed" << endl;
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);

		ORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;

		while((row = mysql_fetch_row(result)) != NULL)
		{//only use first matching feature; nb that when train table is separated from test table every feature will have a unique objectname, viewnum, zoomnum, polynum and testside

			ORfeature* firstNewFeature = new ORfeature();
			currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList = firstNewFeature;
			addSQLRowDataToFeatureList(row, firstNewFeature, true, ignoreOTfeatures, numFeatures);
			ORfeatureContainer* newFeatureContainer = new ORfeatureContainer();
			currentFeatureContainerInTestFeatureMatchingTrainBin->next = newFeatureContainer;
			currentFeatureContainerInTestFeatureMatchingTrainBin = currentFeatureContainerInTestFeatureMatchingTrainBin->next;

			tableIndex++;
		}

	}
	mysql_free_result(result);
}




void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(string sqlDatabaseDecisionTreeTableName, char* decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long* databaseTableSize)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP1 = "INSERT INTO ";
	string sqlInsertCommandP2 = sqlDatabaseDecisionTreeTableName;
	string sqlInsertCommandP3 = " (";
	string sqlInsertCommandP4 = "";
	string sqlInsertCommandP5 = ") VALUES (";
	string sqlInsertCommandP6 = "";
	string sqlInsertCommandP7 = ");";

	string tableIDString = convertLongToString(*databaseTableSize);
	string snapshotReferenceIDString = convertLongToString(snapshotReferenceID);

	sqlInsertCommandP4 = sqlInsertCommandP4 + OR_MYSQL_FIELD_NAME_ID + ", " + OR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID + ", " + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN;
	sqlInsertCommandP6 = sqlInsertCommandP6 + tableIDString + ", " + snapshotReferenceIDString + ", ";

	sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6;
	char sqlInsertCommandCharStar[1000];
	int i;

	for(i=0; i <sqlInsertCommand.length(); i++)
	{
	 	sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
	}

	//must be done this way as decisionTreeBinText may contain null characters

	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
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
	sqlInsertCommandCharStar[i] = ')';
	i++;
	sqlInsertCommandCharStar[i] = ';';
	i++;

	#ifdef OR_DEBUG
	/*
	for(int q=0;q<i;q++)
	{
		cout << "c = " << sqlInsertCommandCharStar[q] << endl;
	}
	*/
	#endif

	#ifdef OR_SQL_DATABASE_DEBUG
	cout << "sqlInsertCommandCharStar = " << sqlInsertCommandCharStar << endl;
	//exit(0);
	#endif
	performSQLrealInsertQuery(sqlInsertCommandCharStar, i);
	*databaseTableSize = *databaseTableSize + 1;
}




#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION

void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(string sqlDatabaseDecisionTreeTableName, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP1 = "INSERT INTO ";
	string sqlInsertCommandP2 = sqlDatabaseDecisionTreeTableName;
	string sqlInsertCommandP3 = " (";
	string sqlInsertCommandP4 = "";
	string sqlInsertCommandP5 = ") VALUES ";

	sqlInsertCommandP4 = sqlInsertCommandP4 + OR_MYSQL_FIELD_NAME_ID + ", " + OR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID + ", " + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN;

	sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5;

	for(int j=0; j<sqlInsertCommand.length(); j++)
	{
		decisionTreeMultipleRowInsertQueryTextCharStar[*decisionTreeSQLmultipleRowInsertQueryLength] = sqlInsertCommand[j];
		*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength + 1;
	}

	#ifdef OR_SQL_DATABASE_DEBUG
	//cout << "insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart{}" << endl;
	//cout << "sqlInsertCommand = " << sqlInsertCommand << endl;
	#endif

}


void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(char* decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long* databaseTableSize, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string sqlInsertCommand = "";	//eg INSERT INTO snapshot (pBinxy) VALUES (430);
	string sqlInsertCommandP6 = "";

	string tableIDString = convertLongToString(*databaseTableSize);
	string snapshotReferenceIDString = convertLongToString(snapshotReferenceID);

	sqlInsertCommandP6 = sqlInsertCommandP6 + "(" + tableIDString + ", " + snapshotReferenceIDString + ", ";
	sqlInsertCommand = sqlInsertCommandP6;
	char sqlInsertCommandCharStar[1000];
	int i;

	for(i=0; i <sqlInsertCommand.length(); i++)
	{
	 	sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
	}

	//must be done this way as decisionTreeBinText may contain null characters

	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
	//cout << "decisionTreeBinTextLength = " << decisionTreeBinTextLength << endl;
	for(int q=0; q <decisionTreeBinTextLength; q++)
	{
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
	sqlInsertCommandCharStar[i] = ')';
	i++;
	sqlInsertCommandCharStar[i] = ',';
	i++;

	for(int j=0; j<i; j++)
	{
		decisionTreeMultipleRowInsertQueryTextCharStar[*decisionTreeSQLmultipleRowInsertQueryLength] = sqlInsertCommandCharStar[j];
		*decisionTreeSQLmultipleRowInsertQueryLength = *decisionTreeSQLmultipleRowInsertQueryLength + 1;
	}
	#ifdef OR_SQL_DATABASE_DEBUG
	//cout << "insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration{}" << endl;
	//cout << "decisionTreeMultipleRowInsertQueryTextCharStar = " << decisionTreeMultipleRowInsertQueryTextCharStar << endl;
	#endif

	*databaseTableSize = *databaseTableSize + 1;
}


void insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
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

		performSQLrealInsertQuery(decisionTreeMultipleRowInsertQueryTextCharStar,* decisionTreeSQLmultipleRowInsertQueryLength);
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
void insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(string sqlDatabaseDecisionTreeTableName, char* decisionTreeBinText, int decisionTreeBinTextLength, long snapshotReferenceID, long* databaseTableSize, char* decisionTreeMultipleRowInsertQueryTextCharStar, int decisionTreeSQLmultipleRowInsertQueryLength)
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

	sqlInsertCommandP4 = sqlInsertCommandP4 + OR_MYSQL_FIELD_NAME_ID + ", " + OR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID + ", " + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN;
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

	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
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


void createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, string sqlDatabaseDecisionTreeTableName, char* decisionTreeBinText, int decisionTreeBinTextLength, int trainOrTest)
{
	int query_state;
	MYSQL_ROW row;

	string sqlSelectCommand = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandP1 = "SELECT ";
	string sqlSelectCommandP2 = "";
	string sqlSelectCommandP3 = " FROM ";
	string sqlSelectCommandP4 = "";	//OR_MYSQL_DATABASE_NAME;
	string sqlSelectCommandP5 = "";		//".";
	string sqlSelectCommandP6 = sqlDatabaseDecisionTreeTableName;
	string sqlSelectCommandP7 = "";
	string sqlSelectCommandP8 = "";
	string sqlSelectCommandP9 = "";
	string sqlSelectCommandP10 = "";
	string sqlSelectCommandP11 = "";
	string sqlSelectCommandP12 = "";

	sqlSelectCommandP2 = OR_MYSQL_FIELD_NAME_DECISIONTREE_SNAPSHOT_REF_ID;

	if(trainOrTest == TEST)
	{
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}
	else if(trainOrTest == TRAIN_AND_TEST)
	{
		string maxDecisionTreeTableRowString = convertLongToString(databaseTableSizeDecisionTreeInitial);
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + OR_MYSQL_FIELD_NAME_ID + " <= " + maxDecisionTreeTableRowString + " AND " + OR_MYSQL_FIELD_NAME_DECISIONTREE_BIN + " = ";
	}

	sqlSelectCommand = sqlSelectCommandP1 + sqlSelectCommandP2 + sqlSelectCommandP3 + sqlSelectCommandP4 + sqlSelectCommandP5 + sqlSelectCommandP6 + sqlSelectCommandP7;

	char sqlSelectCommandCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_STRING_MAX_LENGTH];
	int i;
	for(i=0; i <sqlSelectCommand.length(); i++)
	{
	 	sqlSelectCommandCharStar[i] = sqlSelectCommand[i];
	}

	//must be done this way as decisionTreeBinText may contain null characters

	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DO_NOT_USE_REAL_SQL_QUERY
	//cout << "decisionTreeBinTextLength = " << decisionTreeBinTextLength << endl;
	for(int q=0; q<decisionTreeBinTextLength; q++)
	{
	 	sqlSelectCommandCharStar[i] = decisionTreeBinText[q];
		i++;
	}
	#else
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

	#ifdef OR_DEBUG
	/*
	for(int q=0;q<i;q++)
	{
		cout << sqlSelectCommandCharStar[q];
	}
	cout << "" << endl;
	*/
	#endif

	#ifdef OR_SQL_DATABASE_DEBUG
	cout << "sqlSelectCommandCharStar = " << sqlSelectCommandCharStar << endl;
	//exit(0);
	#else
		#ifdef OR_SQL_DATABASE_DEBUG_COMPARISON_ONLY
		cout << "sqlSelectCommandCharStar = " << sqlSelectCommandCharStar << endl;
		#endif
	#endif

	query_state = mysql_real_query(connection, sqlSelectCommandCharStar, i);
	#ifdef OR_DEBUG
	//cout << "select qeuery performed" << endl;
	#endif
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);

		ORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDreferenceList;

		while((row = mysql_fetch_row(result)) != NULL)
		{//only use first matching feature; nb that when train table is separated from test table every feature will have a unique objectname, viewnum, zoomnum, polynum and testside

			char* tableIDCharStar;
			tableIDCharStar = row[0];
			long tableID = (long)(atof(tableIDCharStar));

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_LOOKUP_DO_NOT_ALLOW_REPEATS
			//test for multiple references to same object ID added by RBB 23Mar10
			bool snapshotIDReferenceAlreadyAddedToList = false;
			if(tableIndex > 0)
			{
				ORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceListTemp = firstReferenceInSnapshotIDreferenceList;
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
				#ifdef OR_DEBUG
				//cout << "tableID = " << tableID << endl;
				#endif

				ORsnapshotIDreferenceList* newReferenceInSnapshotIDReferenceList = new ORsnapshotIDreferenceList();
				currentReferenceInSnapshotIDReferenceList->next = newReferenceInSnapshotIDReferenceList;
				currentReferenceInSnapshotIDReferenceList = currentReferenceInSnapshotIDReferenceList->next;
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_LOOKUP_DO_NOT_ALLOW_REPEATS
			}
			#endif

			tableIndex++;

		}

	}
	mysql_free_result(result);
}


//#endif












void createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(ORfeatureContainer* firstFeatureContainerInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour* normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], unsigned char* rgb8bitSmallMapForInstantDBqueryAccessRequirement, int smallImageWidth, int smallImageHeight, string trainTableName, int trainOrTest)
{
	if(!createFeatureObjects)
	{
		cout << "error; createFeaturesListUsingDatabaseQueryGeoXYbinRequirement requires createFeatureObjects" << endl;
		exit(0);
	}

	MYSQL_ROW row;
	int query_state;

	string sqlSelectCommand = "";	//eg SELECT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
	string sqlSelectCommandP1 = "SELECT ";
	string sqlSelectCommandP2 = "";
	string sqlSelectCommandP3 = " FROM ";
	string sqlSelectCommandP4 = "";	//OR_MYSQL_DATABASE_NAME;
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
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM + " = " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TRAIN;	//only query train objects
	}
	else if(trainOrTest == TRAIN_AND_TEST)
	{
		string maxTrainTableRowString = convertLongToString(databaseTableSizeTrainInitial);
		sqlSelectCommandP7 = sqlSelectCommandP7 + " WHERE (" + OR_MYSQL_FIELD_NAME_ID + " <= " + maxTrainTableRowString + " AND " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM + " = " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM_TRAIN;	//only query train objects
	}
	else
	{
		cout << "illegal trainOrTest value" << endl;
		exit(0);
	}


#ifdef OR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK
	unsigned long linearCombination = 0;
	convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(concatonatedSignedDctCoeffArrayRequirement, pBinxRequirement, pBinyRequirement, &linearCombination);
	string linearCombinationString = convertLongToString(linearCombination);
	#ifdef OR_DEBUG
	//cout << "linearCombination = " << linearCombination << endl;
	//cout << "linearCombinationString = " << linearCombinationString << endl;
	#endif
	sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL + " >= (" + linearCombinationString + " - " + OR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK_MAX_DIFF + ")" " AND " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL + " <= (" + linearCombinationString + " + " + OR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK_MAX_DIFF + ")";
	cout << "sqlSelectCommandP8 = " << sqlSelectCommandP8 << endl;
#else

	if(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING)
	{
	#ifdef OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V1
		string pBinxyValueRequirementString = convertIntToString(pBinxyValueRequirement);
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " + OR_MYSQL_FIELD_NAME_GEO_BINS_XY + " = " + pBinxyValueRequirementString;
	#elif defined OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V2
		//V2 MIGHT BE BAD DEPENDING UPON HOW GOOD MYSQL IS; if this is used px1->px2 and py1 -> py2 need to be indexed. note there are ~100^4 combinations of binned variables.
		for(int i=0; i<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; i++)
		{
			string pBinxValueRequirementMinString = convertIntToString(pBinxRequirement[i]-1);
			string pBinyValueRequirementMinString = convertIntToString(pBinyRequirement[i]-1);

			string pBinxNameRequirementString = convertIntToString(i+1);
			string pBinyNameRequirementString = convertIntToString(i+1);

			string pBinxValueRequirementMaxString = convertIntToString(pBinxRequirement[i]+1);
			string pBinyValueRequirementMaxString = convertIntToString(pBinyRequirement[i]+1);

			sqlSelectCommandP8 = sqlSelectCommandP8 + " AND " +
			OR_MYSQL_FIELD_NAME_GEO_BINS_X + pBinxNameRequirementString + " >= " + pBinxValueRequirementMinString + " AND " +
			OR_MYSQL_FIELD_NAME_GEO_BINS_X + pBinxNameRequirementString + " <= " + pBinxValueRequirementMaxString + " AND " +
			OR_MYSQL_FIELD_NAME_GEO_BINS_Y + pBinyNameRequirementString + " >= " + pBinyValueRequirementMinString + " AND " +
			OR_MYSQL_FIELD_NAME_GEO_BINS_Y + pBinyNameRequirementString + " <= " + pBinyValueRequirementMaxString;
		}
	#elif defined OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V3
		int xBinIndex = 0;
		sqlSelectCommandP8 = sqlSelectCommandP8 + " AND (";
		int count = 0;
		#ifdef OR_DEBUG
		/*
		cout << "pBinxRequirement[0] = " << pBinxRequirement[0] << endl;
		cout << "pBinyRequirement[0] = " << pBinyRequirement[0] << endl;
		cout << "pBinxRequirement[1] = " << pBinxRequirement[1] << endl;
		cout << "pBinyRequirement[1] = " << pBinyRequirement[1] << endl;
		cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X << endl;
		*/
		#endif

		for(int x = pBinxRequirement[0]-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=pBinxRequirement[0]+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
		{
			int yBinIndex =0;
			for(int y = pBinyRequirement[0]-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=pBinyRequirement[0]+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
			{
				//check current bin and nearest bins
				int xBinIndex2 = 0;
				for(int x2 = pBinxRequirement[1]-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2<=pBinxRequirement[1]+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2++)
				{
					int y2BinIndex =0;
					for(int y2 = pBinyRequirement[1]-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2<=pBinyRequirement[1]+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2++)
					{
						int geoxBin[2];
						int geoyBin[2];
						long geoxybin;

						geoxBin[0] = x;
						geoyBin[0] = y;
						geoxBin[1] = x2;
						geoyBin[1] = y2;

						geoxybin = calculateGeoxyBinMultiDimensional(geoxBin, geoyBin);

						char pBinxyValueRequirementString[25];
						pBinxyValueRequirementString = convertIntToString(geoxybin);
						sqlSelectCommandP8 = sqlSelectCommandP8 + OR_MYSQL_FIELD_NAME_GEO_BINS_XY + " = " + pBinxyValueRequirementString;

						count++;

						if(count == OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X*OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y*OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X*OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y)
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
		for(int x=0; x<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; x++)
		{
			for(int y=0; y<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; y++)
			{
				char pBinxValueRequirementString[10];
				pBinxValueRequirementString = convertIntToString(pBinxRequirement[x]);
				char pBinyValueRequirementString[10];
				pBinyValueRequirementString = convertIntToString(pBinyRequirement[y]);

				char pBinxNameRequirementString[10];
				pBinxNameRequirementString = convertIntToString(x+1);
				char pBinyNameRequirementString[10];
				pBinyNameRequirementString = convertIntToString(y+1);


				sqlSelectCommandP8 = sqlSelectCommandP8 + "(" +
				OR_MYSQL_FIELD_NAME_GEO_BINS_X + pBinxNameRequirementString + " = " + pBinxValueRequirementString + " AND " +
				OR_MYSQL_FIELD_NAME_GEO_BINS_Y + pBinyNameRequirementString + " = " + pBinyValueRequirementString + ")";

				if((x == OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS-1) && (y == OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS-1))
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

	if(OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING)
	{
	#ifdef OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING_REQUIREMENT_V2
		for(int colour=0; colour < 3; colour++)
		{
			char normalisedAverageHueDevValueMaxString[10];
			char normalisedAverageHueDevValueMinString[10];
			unsigned char maxVal;
			unsigned char minVal;
			unsigned char avgVal;


			string normalisedAverageHueDevName;
			if(colour == 0)
			{
				avgVal = normalisedAverageHueDeviationRequirement->r;
				normalisedAverageHueDevName = OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_R_BINS;
			}
			else if(colour == 1)
			{
				avgVal = normalisedAverageHueDeviationRequirement->g;
				normalisedAverageHueDevName = OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_G_BINS;
			}
			else
			{
				avgVal = normalisedAverageHueDeviationRequirement->b;
				normalisedAverageHueDevName = OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_B_BINS;

			}
			maxVal = minInt(int(avgVal) + 1, RGB_NUM);
			minVal = maxInt(int(avgVal) - 1, 0);

			normalisedAverageHueDevValueMaxString = convertIntToString(maxVal);
			normalisedAverageHueDevValueMinString = convertIntToString(minVal);

			sqlSelectCommandP9 = sqlSelectCommandP9 + " AND " + normalisedAverageHueDevName + " >= " + normalisedAverageHueDevValueMinString + " AND " + normalisedAverageHueDevName + " <= " + normalisedAverageHueDevValueMaxString;
				//requires optimisation
		}
	#elif defined OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING_REQUIREMENT_V3

		int rBinMid = (int)normalisedAverageHueDeviationRequirement->r;
		int gBinMid = (int)normalisedAverageHueDeviationRequirement->g;
		int bBinMid = (int)normalisedAverageHueDeviationRequirement->b;

		#ifdef OR_DEBUG
		/*
		cout << "normalisedAverageHueDeviationRequirement->r = " << (int)normalisedAverageHueDeviationRequirement->r << endl;
		cout << "normalisedAverageHueDeviationRequirement->g = " << (int)normalisedAverageHueDeviationRequirement->g << endl;
		cout << "normalisedAverageHueDeviationRequirement->b = " << (int)normalisedAverageHueDeviationRequirement->b << endl;
		cout << "rBinMid = " << rBinMid << endl;
		cout << "gBinMid = " << gBinMid << endl;
		cout << "bBinMid = " << bBinMid << endl;
		*/
		#endif

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
		if(rBinMax > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
		{
			rBinMax = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
		}
		if(gBinMin < 0)
		{
			gBinMin = 0;
		}
		if(gBinMax > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
		{
			gBinMax = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
		}
		if(bBinMin < 0)
		{
			bBinMin = 0;
		}
		if(bBinMax > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
		{
			bBinMax = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
		}

		sqlSelectCommandP9 = sqlSelectCommandP9 + " AND (";

		for(int r = rBinMin; r <= rBinMax; r++)
		{
			for(int g = gBinMin; g <= gBinMax; g++)
			{
				for(int b = bBinMin; b <= bBinMax; b++)
				{
					string normalisedAverageHueDevName = OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
					int currentCombinergbBin = r*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + g*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + b;
					string currentCombinergbBinString = convertIntToString(currentCombinergbBin);

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



	if(OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
	#ifdef OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V2
		//V2 MIGHT BE BAD DEPENDING UPON HOW GOOD MYSQL IS; if this is used d0->dx need to be indexed. note there are 16^16 combinations of dct variables.
		for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{
			string dctCoeffNumNameString = convertIntToString(dctCoeffNum);

			//int maxVal = minInt(int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) + 1, OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET-1);	//nb allowed bin values are between -8 and +7
			//int minVal = maxInt(int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) - 1, -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET);
			int maxVal = int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) + 1;	//nb allowed bin values are between -8 and +7
			int minVal = int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]) - 1;

			string dctCoeffNumValMaxString = convertIntToString(maxVal);
			string dctCoeffNumValMinString = convertIntToString(minVal);

			sqlSelectCommandP10 = sqlSelectCommandP10 + " AND " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + " >= " + dctCoeffNumValMinString + " AND " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + " <= " + dctCoeffNumValMaxString;		//only query train objects
		}
	#elif defined OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V3
		int numberOfSuccessfulDCTcomparisons = 0;
		for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{

			char dctCoeffNumNameString[10];
			dctCoeffNumNameString = convertIntToString(dctCoeffNum);

			int dctCoeffNumVal = int(concatonatedSignedDctCoeffArrayRequirement[dctCoeffNum]);
			#ifdef OR_DEBUG
			//cout << "dctCoeffNumVal = " << dctCoeffNumVal << endl;
			#endif
			char dctCoeffNumValString[10];
			dctCoeffNumValString = convertIntToString(dctCoeffNumVal);

				//Seg SELECT columnname, COUNT(*) FROM tablename
			//eg COUNT `pBinxy` FROM `objectRecog`.`snapshot` WHERE ( ( `pBinxy` = 823 ) );
			string sqlSelectCommandDCTqueryV3 = "";
			sqlSelectCommandDCTqueryV3 = sqlSelectCommandDCTqueryV3 + "SELECT " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + ", COUNT(*) FROM " + trainTableName + " WHERE " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumNameString + " = " + dctCoeffNumValString;


			#ifdef OR_SQL_DATABASE_DEBUG
			cout << "sqlSelectCommandDCTqueryV3 = " << sqlSelectCommandDCTqueryV3 << endl;
			//exit(0);
			#endif
			char* sqlSelectCommandDCTqueryV3CharStar = const_cast<char*>(sqlSelectCommandDCTqueryV3.c_str());
			long numberOfDCTcomparisons;

			query_state = mysql_query(connection, sqlSelectCommandDCTqueryV3CharStar);
			#ifdef OR_DEBUG
			//cout << "select query performed" << endl;
			#endif
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
					numberOfDCTcomparisons = long(atof(row[1]));
				}
			}
			mysql_free_result(result);

			if(numberOfDCTcomparisons >= 1)
			{
				numberOfSuccessfulDCTcomparisons++;
			}
		}
		if(numberOfSuccessfulDCTcomparisons > OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V3_MIN_NUM_SUCC_COMPARISONS_REUQIRED)
		{

	#elif defined OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V4
		int numberOfSuccessfulDCTcomparisons = 0;

		#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			char dctCoeffArrayBinnedString[1000];
			int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
			int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS];
			convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffNumValString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedDctCoeffArrayBiasIntNOTUSED);
			#else
			char dctCoeffNumValString[1000];
			int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
			convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffNumValString, &DCTCoeff64BitValueStringLengthNOTUSED);
			#endif
		#else
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS];
			unsigned long dctCoeffArrayBinned = convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, concatonatedDctCoeffArrayBiasIntNOTUSED);
			#else
			unsigned long dctCoeffArrayBinned = convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement);
			#endif
			char dctCoeffNumValString[25];
			sprintf(dctCoeffNumValString, "%ld", dctCoeffArrayBinned);
		#endif

		sqlSelectCommandP10 = sqlSelectCommandP10 + " AND " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL + " = " + dctCoeffNumValString;		//only query train objects
	#endif
	}

	if(OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON)
	{
	#ifdef OR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V2
		for(int x=0; x<smallImageWidth; x++)
		{
			for(int y=0; y<smallImageHeight; y++)
			{
				colour currentSmallImageValue;
				getRGBMapValues(x, y, smallImageWidth, rgb8bitSmallMapForInstantDBqueryAccessRequirement, &currentSmallImageValue);

				for(int colour=0; colour < 3; colour++)
				{
					char normalisedHueDevValueMaxString[10];
					char normalisedHueDevValueMinString[10];
					unsigned char maxVal;
					unsigned char minVal;
					unsigned char avgVal;

					string normalisedHueDevName;
					if(colour == 0)
					{
						avgVal = currentSmallImageValue.r;
						normalisedHueDevName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_R
					}
					else if(colour == 1)
					{
						avgVal = currentSmallImageValue.g;
						normalisedHueDevName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_G;
					}
					else
					{
						avgVal = currentSmallImageValue.b;
						normalisedHueDevName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_B;
					}

					char rgbValueRequirementMinString[10];
					rgbValueRequirementMinString = convertIntToString(avgVal-1);

					char rgbxNameRequirementString[10];
					rgbxNameRequirementString = convertIntToString(x+1);
					char rgbyNameRequirementString[10];
					rgbyNameRequirementString = convertIntToString(y+1);

					char rgbValueRequirementMaxString[10];
					rgbValueRequirementMaxString = convertIntToString(avgVal+1);

					sqlSelectCommandP11 = sqlSelectCommandP11 + " AND "
					normalisedHueDevName + "y" + rgbyNameRequirementString + "x" + rgbxNameRequirementString " >= " + rgbValueRequirementMinString +
					normalisedHueDevName + "y" + rgbyNameRequirementString + "x" + rgbxNameRequirementString " <= " + rgbValueRequirementMaxString;
				}
			}
		}
	#elif defined OR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V3
		int numberOfSuccessfulSmallImagecomparisons = 0;
		for(int x=0; x<smallImageWidth; x++)
		{
			for(int y=0; y<smallImageHeight; y++)
			{
				colour currentSmallImageValue;
				getRGBMapValues(x, y, smallImageWidth, rgb8bitSmallMapForInstantDBqueryAccessRequirement, &currentSmallImageValue);

				for(int colour=0; colour < 3; colour++)
				{
					char normalisedHueDevValueMaxString[10];
					char normalisedHueDevValueMinString[10];
					unsigned char maxVal;
					unsigned char minVal;
					unsigned char avgVal;

					string normalisedHueDevName;
					if(colour == 0)
					{
						avgVal = currentSmallImageValue.r;
						normalisedHueDevName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_R
					}
					else if(colour == 1)
					{
						avgVal = currentSmallImageValue.g;
						normalisedHueDevName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_G;
					}
					else
					{
						avgVal = currentSmallImageValue.b;
						normalisedHueDevName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_B;
					}

					char rgbValueRequirementString[10];
					rgbValueRequirementString = convertIntToString(avgVal);

					char rgbxNameRequirementString[10];
					rgbxNameRequirementString = convertIntToString(x+1);
					char rgbyNameRequirementString[10];
					rgbyNameRequirementString = convertIntToString(y+1);


					//eg SELECT columnname, COUNT(*) FROM tablename
					string sqlSelectCommandSmallImageComparisonqueryV3 = "";
					string cellName = normalisedHueDevName + "y" + rgbyNameRequirementString + "x" + rgbxNameRequirementString;
					string sqlSelectCommandSmallImageComparisonqueryV3 = sqlSelectCommandSmallImageComparisonqueryV3 + "SELECT " cellName + ", COUNT(*) FROM " + OR_MYSQL_TABLE_NAME + " WHERE " + cellName + " = " + rgbValueRequirementString;

					#ifdef OR_SQL_DATABASE_DEBUG
					cout << "sqlSelectCommandDCTqueryV3 = " << sqlSelectCommandDCTqueryV3 << endl;
					#endif
					char* sqlSelectCommandSmallImageComparisonqueryV3CharStar = const_cast<char*>(sqlSelectCommandSmallImageComparisonqueryV3.c_str());
					long numberOfSmallImagecomparisons;

					query_state = mysql_query(connection, sqlSelectCommandSmallImageComparisonqueryV3CharStar);
					#ifdef OR_DEBUG
					//cout << "select qeuery performed" << endl;
					#endif
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
							numberOfSmallImagecomparisons = long(atof(row[1]));
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

		if(numberOfSuccessfulSmallImagecomparisons > OR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V3_MIN_NUM_SUCC_COMPARISONS_REUQIRED_NORMALISED*smallImageWidth*smallImageHeight*3)
		{
	#endif
	}
#endif

	int numFeatures;
	if(OR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES)
	{
		numFeatures = OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;
	}
	else
	{
		numFeatures = OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS;
	}

	sqlSelectCommandP2 = createSQLSelectRowCommand(numFeatures);

	//do not extract snapshot images (small or large) from DB at this stage of development

	sqlSelectCommand = sqlSelectCommandP1 + sqlSelectCommandP2 + sqlSelectCommandP3 + sqlSelectCommandP4 + sqlSelectCommandP5 + sqlSelectCommandP6 + sqlSelectCommandP7 + sqlSelectCommandP8 + sqlSelectCommandP9 + sqlSelectCommandP10 + sqlSelectCommandP11 + sqlSelectCommandP12;
	#ifdef OR_SQL_DATABASE_DEBUG_COMPARISON_ONLY
	cout << "sqlSelectCommand = " << sqlSelectCommand << endl;
	#endif
	char* sqlSelectCommandCharStar = const_cast<char*>(sqlSelectCommand.c_str());


	query_state = mysql_query(connection, sqlSelectCommandCharStar);
	#ifdef OR_DEBUG
	//cout << "select qeuery performed" << endl;
	#endif
	if (query_state !=0)
	{
		cout << mysql_error(connection) << endl;
	}
	else
	{
		int tableIndex = 0;
		result = mysql_store_result(connection);
		ORfeatureContainer* currentFeatureContainerInList = firstFeatureContainerInList;

		while((row = mysql_fetch_row(result)) != NULL)
		{
			ORfeature* firstNewFeature = new ORfeature();
			currentFeatureContainerInList->firstFeatureInFeatureList = firstNewFeature;
			ORfeature* currentFeatureInList = currentFeatureContainerInList->firstFeatureInFeatureList;

			addSQLRowDataToFeatureList(row, firstNewFeature, createFeatureObjects, ignoreOTfeatures, numFeatures);

			tableIndex++;

			ORfeatureContainer* newFeatureContainer = new ORfeatureContainer();
			currentFeatureContainerInList->next = newFeatureContainer;
			currentFeatureContainerInList = currentFeatureContainerInList->next;
		}

	}
	mysql_free_result(result);

		#ifdef OR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V3
		}
		#endif
	#ifdef OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V3
	}
	#endif
}

void addSQLRowDataToFeatureList(MYSQL_ROW row, ORfeature* firstFeatureInList, bool createFeatureObjects, bool ignoreOTfeatures, int numFeatures)
{
	if(!createFeatureObjects)
	{
		cout << "error; createFeaturesListUsingDatabaseQueryGeoXYbinRequirement requires createFeatureObjects" << endl;
		exit(0);
	}

	ORfeature* currentFeatureInList = firstFeatureInList;

	int fieldIndex = 0;

	char* tableIDCharStar;
	char* trainOrTestStringCharStar;
	char* objectNameCharStar;
	char* viewIndexStringCharStar;
	char* zoomIndexStringCharStar;
	char* polyIndexStringCharStar;
	char* sideIndexStringCharStar;
	#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	char* OTpointIndexStringCharStar;
	#endif
	char* pointTransformedxCharStar;
	char* pointTransformedyCharStar;
	char* pointTransformedzCharStar;
	#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
	char* pointxCharStar;
	char* pointyCharStar;
	char* pointzCharStar;
	#endif
	char* pBinxCharStar;
	char* pBinyCharStar;
	char* pBinxyCharStar;
	//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	char* dctCoeffNumStringCharStar;
	char* dctCoeffArrayBinnedCharStar;
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	char* colAvgRBinStringCharStar;
	char* colAvgGBinStringCharStar;
	char* colAvgBBinStringCharStar;
	char* colAvgBinStringCharStar;
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE
	char* allTextCharStar;
	//#endif


	long tableID;
	int trainOrTest;
	string objectName;
	int viewIndex;
	int zoomIndex;
	int polyIndex;
	int sideIndex;
	#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	int OTpointIndex[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_MAX];
	#endif
	//#ifdef OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
	int pBinx[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
	int pBiny[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
	long pBinxy;
	//#endif
	vec pointTransformed[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_MAX];
	#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
	vec point[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_MAX];
	#endif
	//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	signed char dctCoefficients[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	unsigned long dctCoeffArrayBinned;
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	colour colAvg;
	//#endif

	tableIDCharStar = row[fieldIndex++];
	objectNameCharStar = row[fieldIndex++];
	trainOrTestStringCharStar = row[fieldIndex++];
	viewIndexStringCharStar = row[fieldIndex++];
	zoomIndexStringCharStar = row[fieldIndex++];
	polyIndexStringCharStar = row[fieldIndex++];
	sideIndexStringCharStar = row[fieldIndex++];

	#ifdef OR_DEBUG
	/*
	cout << "tableIDCharStar = " << tableIDCharStar << endl;
	cout << "trainOrTestStringCharStar = " << trainOrTestStringCharStar << endl;
	cout << "objectNameCharStar = " << objectNameCharStar << endl;
	cout << "viewIndexStringCharStar = " << viewIndexStringCharStar << endl;
	cout << "zoomIndexStringCharStar = " << zoomIndexStringCharStar << endl;
	cout << "polyIndexStringCharStar = " << polyIndexStringCharStar << endl;
	cout << "sideIndexStringCharStar = " << sideIndexStringCharStar << endl;
	*/
	#endif

	tableID = (long)(atof(tableIDCharStar));
	objectName = objectNameCharStar;
	trainOrTest = (bool)(atof(trainOrTestStringCharStar));
	viewIndex = (int)(atof(viewIndexStringCharStar));
	zoomIndex = (int)(atof(zoomIndexStringCharStar));
	polyIndex = (int)(atof(polyIndexStringCharStar));
	sideIndex = (int)(atof(sideIndexStringCharStar));

	int numFoundFeatures = 0;	//added 14 June 2012 (for the case where there is less features recorded in sql than expected / eg 4]
	bool stillFindingFeatures = true;
	unsigned long* lengths;
	lengths = mysql_fetch_lengths(result);
	for(int featureNum=0; featureNum<numFeatures; featureNum++)
	{
		#ifdef OR_DEBUG
		//cout << "lengths[fieldIndex]  = " << lengths[fieldIndex+1]  << endl;
		#endif
		if(lengths[fieldIndex+1] == 0)	//index of pointTransformedxCharStar [as if this is empty, then the entire feature entry is empty]
		{
			stillFindingFeatures = false;
		}

		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		OTpointIndexStringCharStar = row[fieldIndex++];
		#endif
		pointTransformedxCharStar = row[fieldIndex++];
		pointTransformedyCharStar = row[fieldIndex++];
		pointTransformedzCharStar = row[fieldIndex++];;
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
		pointxCharStar = row[fieldIndex++];
		pointyCharStar = row[fieldIndex++];
		pointzCharStar = row[fieldIndex++];
		#endif

		if(stillFindingFeatures)
		{
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			OTpointIndex[featureNum] = (int)(atof(OTpointIndexStringCharStar));
			#endif
			pointTransformed[featureNum].x = atof(pointTransformedxCharStar);
			pointTransformed[featureNum].y = atof(pointTransformedyCharStar);
			pointTransformed[featureNum].z = atof(pointTransformedzCharStar);
			#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
			point[featureNum].x = atof(pointxCharStar);
			point[featureNum].y = atof(pointyCharStar);
			point[featureNum].z = atof(pointzCharStar);
			#endif
			numFoundFeatures++;
		}
	}

	if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
	{
		//this information is only used to bin the feature list data, and is not transfered to the feature list
		for(int geobinDimension = 0; geobinDimension < OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; geobinDimension++)
		{
			pBinxCharStar = row[fieldIndex++];
			pBinyCharStar = row[fieldIndex++];
			pBinx[geobinDimension] = (unsigned char)(atof(pBinxCharStar));
			pBiny[geobinDimension] = (unsigned char)(atof(pBinyCharStar));
		}
		pBinxyCharStar = row[fieldIndex++];
		pBinxy = (unsigned char)(atof(pBinxyCharStar));
	}

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{

			dctCoeffNumStringCharStar = row[fieldIndex++];
			dctCoefficients[dctCoeffNum] = (signed char)(atof(dctCoeffNumStringCharStar));
			#ifdef OR_DEBUG
			//cout << "dctCoefficients[dctCoeffNum]  = " << int(dctCoefficients[dctCoeffNum]) << endl;
			#endif
		}
		dctCoeffArrayBinnedCharStar = row[fieldIndex++];
		dctCoeffArrayBinned = (unsigned long)(atol(dctCoeffArrayBinnedCharStar));
	}

	if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
	{
		colAvgRBinStringCharStar = row[fieldIndex++];
		colAvgGBinStringCharStar = row[fieldIndex++];
		colAvgBBinStringCharStar = row[fieldIndex++];
		colAvgBinStringCharStar = row[fieldIndex++];
		colAvg.r = (unsigned char)(atof(colAvgRBinStringCharStar));
		colAvg.g = (unsigned char)(atof(colAvgGBinStringCharStar));
		colAvg.b = (unsigned char)(atof(colAvgBBinStringCharStar));
		//dont use colAvgBinStringCharStar yet...
	}

	if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
	{
		allTextCharStar = row[fieldIndex++];
				//NO: rows can contain binary data including null characters; int allTextCharStarLength = strlen(allTextCharStar);
		for(int i=0; i< OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH; i++)
		{
			firstFeatureInList->snapshotMapsText[i] = allTextCharStar[i];
		}
		#ifdef OR_DEBUG
		//cout << "firstFeatureInList->snapshotMapsText = " << endl;
		//cout << firstFeatureInList->snapshotMapsText << endl;
		#endif
	}

	#ifdef OR_DEBUG
	//cout << "\n\nadding features to list\n\n" << endl;
	#endif

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
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			currentFeatureInList->OTpointIndex = OTpointIndex[featureNum];
			#endif
			currentFeatureInList->pointTransformed.x = pointTransformed[featureNum].x;
			currentFeatureInList->pointTransformed.y = pointTransformed[featureNum].y;
			currentFeatureInList->pointTransformed.z = pointTransformed[featureNum].z;
			#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
			currentFeatureInList->point.x = point[featureNum].x;
			currentFeatureInList->point.y = point[featureNum].y;
			currentFeatureInList->point.z = point[featureNum].z;
			#endif

			if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
			{
				currentFeatureInList->avgCol.r = colAvg.r;
				currentFeatureInList->avgCol.g = colAvg.g;
				currentFeatureInList->avgCol.b = colAvg.b;
			}

			if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
			{
				for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
				{
					currentFeatureInList->dctCoeff[dctCoeffNum] = dctCoefficients[dctCoeffNum];
				}
				currentFeatureInList->dctCoeffArrayBinned = dctCoeffArrayBinned;
			}

			#ifdef OR_DEBUG
			/*
			cout << "\n\tfeature added to list;" << endl;
			cout << "objectName = " << objectName << endl;
			cout << "trainOrTest = " << trainOrTest << endl;
			cout << "viewIndex = " << viewIndex << endl;
			cout << "zoomIndex = " << zoomIndex << endl;
			cout << "polyIndex = " << polyIndex << endl;
			cout << "sideIndex = " << sideIndex << endl;
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			cout << "OTpointIndex[featureNum] = " << OTpointIndex[featureNum] << endl;
			#endif
			cout << "pointTransformed[featureNum].x = " << pointTransformed[featureNum].x << endl;
			cout << "pointTransformed[featureNum].y = " << pointTransformed[featureNum].y << endl;
			cout << "pointTransformed[featureNum].z = " << pointTransformed[featureNum].z << endl;
			#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
			cout << "point[featureNum].x = " << point[featureNum].x << endl;
			cout << "point[featureNum].y = " << point[featureNum].y << endl;
			cout << "point[featureNum].z = " << point[featureNum].z << endl;
			#endif
			if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
			{
				cout << "currentFeatureInList->avgCol.r = " << int(currentFeatureInList->avgCol.r) << endl;
				cout << "currentFeatureInList->avgCol.g = " << int(currentFeatureInList->avgCol.g) << endl;
				cout << "currentFeatureInList->avgCol.b = " << int(currentFeatureInList->avgCol.b) << endl;
			}
			if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
			{
				for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
				{
					cout << "currentFeatureInList->dctCoeff[dctCoeffNum] = " << int(currentFeatureInList->dctCoeff[dctCoeffNum]) << endl;
				}
				cout << "currentFeatureInList->dctCoeffArrayBinned = " << currentFeatureInList->dctCoeffArrayBinned << endl;
			}
			cout << "\n" << endl;
			*/
			#endif

			if(createFeatureObjects)
			{
				ORfeature* newFeature = new ORfeature();
				currentFeatureInList->next = newFeature;
			}
			currentFeatureInList=currentFeatureInList->next;
		}
	}
}



string createSQLSelectRowCommand(int numFeatures)
{
	string sqlSelectCommand = "";
	sqlSelectCommand = sqlSelectCommand + OR_MYSQL_FIELD_NAME_ID + ", " + OR_MYSQL_FIELD_NAME_OBJECTNAME + ", " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM + ", " + OR_MYSQL_FIELD_NAME_VIEWNUM + ", " + OR_MYSQL_FIELD_NAME_ZOOMNUM + ", " + OR_MYSQL_FIELD_NAME_POLYNUM + ", " + OR_MYSQL_FIELD_NAME_SIDENUM;

	for(int featureNum=0; featureNum<numFeatures; featureNum++)
	{
		string featureNumString = convertIntToString(featureNum+1);

		#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_OTNUM + featureNumString;
		#endif
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDX + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDY + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDZ + featureNumString;
		#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_POINTX + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTY + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTZ + featureNumString;
		#endif
	}

	if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
	{
		//this information is only used to bin the feature list data, and is not transfered to the feature list
		for(int geobinDimension = 0; geobinDimension < OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; geobinDimension++)
		{
			string geobinDimensionString = convertIntToString(geobinDimension+1);
			sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_X + geobinDimensionString + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_Y + geobinDimensionString;
		}
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_XY;
	}

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
		{
			string dctCoeffNumString = convertIntToString(dctCoeffNum);

			sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumString;
		}
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL;
	}

	if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
	{
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_R_BINS + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_G_BINS + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_B_BINS + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
	}

	if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
	{
		sqlSelectCommand = sqlSelectCommand + ", " + OR_MYSQL_FIELD_NAME_ALL_IMAGE_TEXT;
	}

	return sqlSelectCommand;
}






void insertTransformedFeatureListIntoDatabase(ORfeature* firstFeatureInList, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest, bool ignoreOTfeatures, unsigned char* rgb8bitSmallMapForInstantDBqueryAccess, int smallImageWidth, int smallImageHeight, bool addPermutationsOfTrainFeaturesForGeoBinning, int maxNumFeaturePermutations, string tableName, long* databaseTableSize)
{
	ORfeature* currentFeatureInTempList = firstFeatureInList;

	//now bin the features;

	int findex1 = 0;

	while((currentFeatureInTempList->next != NULL) && (findex1 < maxNumFeaturePermutations))
	{
		ORfeature* currentFeatureInTempList2 = firstFeatureInList;

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

						string tableIDString = convertLongToString(*databaseTableSize);
						string trainOrTestString = convertIntToString(((int)trainOrTest));
						string viewIndexString = convertIntToString(viewIndex);
						string zoomIndexString = convertIntToString(zoomIndex);
						string polygonIndexString = convertIntToString(polyIndex);
						string sideIndexString = convertIntToString(sideIndex);

						sqlInsertCommandP4 = sqlInsertCommandP4 + OR_MYSQL_FIELD_NAME_ID + ", " + OR_MYSQL_FIELD_NAME_OBJECTNAME + ", " + OR_MYSQL_FIELD_NAME_TRAINORTESTNUM + ", " + OR_MYSQL_FIELD_NAME_VIEWNUM + ", " + OR_MYSQL_FIELD_NAME_ZOOMNUM + ", " + OR_MYSQL_FIELD_NAME_POLYNUM + ", " + OR_MYSQL_FIELD_NAME_SIDENUM;
						sqlInsertCommandP6 = sqlInsertCommandP6 + tableIDString + ", " + "'" + objectName + "'" + ", " + trainOrTestString + ", " + viewIndexString + ", " + zoomIndexString + ", " + polygonIndexString + ", " + sideIndexString;

						bool insideBin = false;

						int geoxBin[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						int geoyBin[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						geoxBin[0] = determineGeoBinX(currentFeatureInTempList->pointTransformed.x);
						geoyBin[0] = determineGeoBinY(currentFeatureInTempList->pointTransformed.y);
						geoxBin[1] = determineGeoBinX(currentFeatureInTempList2->pointTransformed.x);
						geoyBin[1] = determineGeoBinY(currentFeatureInTempList2->pointTransformed.y);

						if(addPermutationsOfTrainFeaturesForGeoBinning)
						{
							if((geoxBin[0] >= 0)
							&& (geoxBin[0] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
							&& (geoyBin[0] >= 0)
							&& (geoyBin[0] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
							&& (geoxBin[1] >= 0)
							&& (geoxBin[1] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
							&& (geoyBin[1] >= 0)
							&& (geoyBin[1] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS))
							{
								insideBin = true;
								//this passes; cout << "inside bin during array formation - good" << endl;
							}

							if(insideBin)
							{
								for(int featureNum=0; featureNum<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; featureNum++)
								{
									ORfeature* currentFeature;

									if(featureNum == 0)
									{
										currentFeature = currentFeatureInTempList;
									}
									else
									{
										currentFeature = currentFeatureInTempList2;
									}

									string featureNumString = convertIntToString(featureNum+1);

									#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									string OTpointIndexString = convertIntToString(currentFeature->OTpointIndex);
									#endif

									string transformedpositionCoordinatesString = convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->pointTransformed));
									#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
									string positionCoordinatesString = convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->point));
									#endif

									#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_OTNUM + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + OTpointIndexString;
									#endif
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDX + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDY + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + transformedpositionCoordinatesString;
									#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_POINTX + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTY + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + positionCoordinatesString;
									#endif

									currentFeature = currentFeature->next;
								}

								if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
								{
									for(int geobinDimension=0; geobinDimension<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; geobinDimension++)
									{
										string geobinDimensionString = convertIntToString(geobinDimension+1);

										#ifdef OR_DEBUG
										//cout << "geoxBin[" << geobinDimension << "] = " << geoxBin[geobinDimension] << endl;
										//cout << "geoyBin[" << geobinDimension << "] = " << geoyBin[geobinDimension] << endl;
										#endif

										string geobinxString = convertIntToString(geoxBin[geobinDimension]);
										string geobinyString = convertIntToString(geoyBin[geobinDimension]);

										sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_X + geobinDimensionString + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_Y + geobinDimensionString;
										sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + geobinxString + ", " + geobinyString;

									}
								}
							}
						}

						ORfeature* currentFeature = firstFeatureInList;

						bool storeAllNearbyAndOTFeaturesDuringTrain;
						if(OR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES)
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
								featurePositionModifier = OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS;
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

									string featureNumString = convertIntToString(featureNum+1+featurePositionModifier);		//if OR_METHOD_GEOMETRIC_COMPARISON_BINNING, add all OT and nearby feature points After the binned nearby feature points

									#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									string OTpointIndexString = convertIntToString(currentFeature->OTpointIndex);
									#endif

									string transformedpositionCoordinatesString = convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->pointTransformed));
									#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
									string positionCoordinatesString = convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(&(currentFeature->point));
									#endif

									#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_OTNUM + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + OTpointIndexString;
									#endif
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDX + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDY + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTTRANSFORMEDZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + transformedpositionCoordinatesString;
									#ifdef OR_METHOD_GEO_COMPARISON_RECORD_ORIGINAL_T_FOR_DEBUG
									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_POINTX + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTY + featureNumString + ", " + OR_MYSQL_FIELD_NAME_POINTZ + featureNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + positionCoordinatesString;
									#endif

									featureNum++;
								}

								currentFeature = currentFeature->next;
							}
						}

						//what is the following used for??? (OR_MYSQL_FIELD_NAME_GEO_BINS_X/OR_MYSQL_FIELD_NAME_GEO_BINS_Y data from test table in SQL database should not be used - I guess it is for a succssful SQL select query only - may as well place dummy data into here)
						if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							if(!addPermutationsOfTrainFeaturesForGeoBinning)
							{//during creation of test objects

								currentFeature = firstFeatureInList;

								int geobinDimension = 0;
								int geoxBin[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
								int geoyBin[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
								while((currentFeature->next != NULL) && (geobinDimension < OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS))
								{
									if((!ignoreOTfeatures) || (currentFeature->OTpointIndex == 0))
									{//perform binning of nearby features only (not OT features)

										string geobinDimensionString = convertIntToString(geobinDimension+1);

										geoxBin[geobinDimension] = determineGeoBinX(currentFeature->pointTransformed.x);
										geoyBin[geobinDimension] = determineGeoBinY(currentFeature->pointTransformed.y);

										#ifdef OR_DEBUG
										//cout << "geoxBin[" << geobinDimension << "] = " << geoxBin[geobinDimension] << endl;
										//cout << "geoyBin[" << geobinDimension << "] = " << geoyBin[geobinDimension] << endl;
										#endif

										string geobinxString = convertIntToString(geoxBin[geobinDimension]);
										string geobinyString = convertIntToString(geoyBin[geobinDimension]);

										sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_X + geobinDimensionString + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_Y + geobinDimensionString;
										sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + geobinxString + ", " + geobinyString;

										geobinDimension++;
									}

									currentFeature = currentFeature->next;
								}
							}
						}



						if(!addPermutationsOfTrainFeaturesForGeoBinning || insideBin)
						{
							if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
							{
								long geoxyBin = calculateGeoxyBinMultiDimensional(geoxBin, geoyBin);

								string geobinxyString = convertIntToString(geoxyBin);

								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_GEO_BINS_XY;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + geobinxyString;
							}

							#ifdef OR_DEBUG
							//cout << "geoxyBin = " << geoxyBin << endl;
							#endif

							if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
							{
								currentFeature = firstFeatureInList;
								for(int dctCoeffNum = 0; dctCoeffNum <OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; dctCoeffNum++)
								{
									string dctCoeffNumString = convertIntToString(dctCoeffNum);

									string dctCoeffValString = convertIntToString(currentFeature->dctCoeff[dctCoeffNum]);

									sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BINS + dctCoeffNumString;
									sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + dctCoeffValString;
								}

								#ifdef OR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK
								unsigned long linearCombination = 0;
								convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(currentFeature->dctCoeff, geoxBin, geoyBin, &linearCombination);
								string dctCoeffArrayBinnedString = convertLongToString(linearCombination);
								#ifdef OR_DEBUG
								//cout << "linearCombination = " << linearCombination << endl;
								#endif
								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + dctCoeffArrayBinnedString;
								#else
								#ifdef OR_DEBUG
								//cout << "currentFeature->dctCoeffArrayBinned = " << currentFeature->dctCoeffArrayBinned << endl;
								#endif
								string dctCoeffArrayBinnedString = convertLongToString(currentFeature->dctCoeffArrayBinned);
								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + dctCoeffArrayBinnedString;
								#endif
							}

							if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
							{
								currentFeature = firstFeatureInList;
								string colAvgRBinString = convertIntToString(currentFeature->avgCol.r);
								string colAvgGBinString = convertIntToString(currentFeature->avgCol.g);
								string colAvgBBinString = convertIntToString(currentFeature->avgCol.b);
								int currentCombinergbBin = int(currentFeature->avgCol.r)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + int(currentFeature->avgCol.g*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL) + int(currentFeature->avgCol.b);
								string colAvgBinString = convertIntToString(currentCombinergbBin);

								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_R_BINS + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_G_BINS + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_B_BINS + ", " + OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + colAvgRBinString + ", " + colAvgGBinString + ", " + colAvgBBinString + ", " + colAvgBinString;
							}

							if(OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON)
							{
								#ifdef OR_IMAGE_COMPARISON_SQL_DB_USE_RGB_8BIT_SMALL_MAP_QUERY_REQUIREMENT_V2_OR_V3
								for(int y=0; y<smallImageHeight;y++)
								{
									string simystring = convertIntToString(y);

									for(int x=0; x<smallImageWidth;x++)
									{
										string simxstring = convertIntToString(x);

										colour tempCol;
										getRGBMapValues(x, y, smallImageWidth, rgb8bitSmallMapForInstantDBqueryAccess, &tempCol);
										for(int c = 0; c< RGB_NUM; c++)
										{
											string colourName;
											unsigned char colourVal;
											if(c == 0)
											{
												colourName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_R;
												colourVal = tempCol.r;
											}
											else if(c == 1)
											{
												colourName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_G;
												colourVal = tempCol.g;
											}
											else
											{
												colourName = OR_MYSQL_FIELD_NAME_SMALL_IMAGE_B;
												colourVal = tempCol.b;
											}

											string colourValString = convertIntToString(colourVal);

											sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + colourName + "y" + simystring + "x" + simxstring;
											sqlInsertCommandP6 = sqlInsertCommandP6 + ", " + colourValString;
										}
									}
								}
								#endif
							}

							if(!OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
							{

								//do not extract snapshot images (small or large) from DB at this stage of development


								sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6 + sqlInsertCommandP7;
								#ifdef OR_SQL_DATABASE_DEBUG
								cout << "sqlInsertCommand = " << sqlInsertCommand << endl;
								//exit(0);
								#endif
								char* sqlInsertCommandCharStar = const_cast<char*>(sqlInsertCommand.c_str());
								performSQLinsertQuery(sqlInsertCommandCharStar);
								*databaseTableSize = *databaseTableSize + 1;

							}
							else
							{
								sqlInsertCommandP4 = sqlInsertCommandP4 + ", " + OR_MYSQL_FIELD_NAME_ALL_IMAGE_TEXT;
								sqlInsertCommandP6 = sqlInsertCommandP6 + ", ";
								sqlInsertCommand = sqlInsertCommandP1 + sqlInsertCommandP2 + sqlInsertCommandP3 + sqlInsertCommandP4 + sqlInsertCommandP5 + sqlInsertCommandP6;
								char sqlInsertCommandCharStar[OR_IMAGE_COMPARISON_SQL_DATABASE_TEST_AND_TRAIN_TABLES_INSERT_QUERY_MAX_LENGTH];
								int i;

								for(i=0; i <sqlInsertCommand.length(); i++)
								{
	 								sqlInsertCommandCharStar[i] = sqlInsertCommand[i];
								}

								//must be done this way as decisionTreeBinText may contain null characters

								#ifdef OR_DEBUG
								//cout << "OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_MAX_DATA_LENGTH = " << OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_MAX_DATA_LENGTH << endl;
								//cout << "OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH = " << OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH << endl;
								#endif

								sqlInsertCommandCharStar[i] = '\'';
								i++;
								currentFeature = firstFeatureInList;
								i = i + mysql_real_escape_string(&mysql, &(sqlInsertCommandCharStar[i]), currentFeature->snapshotMapsText, OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH);
								sqlInsertCommandCharStar[i] = '\'';
								i++;

								sqlInsertCommandCharStar[i] = ')';
								i++;
								sqlInsertCommandCharStar[i] = ';';
								i++;

								#ifdef OR_SQL_DATABASE_DEBUG
								cout << "sqlInsertCommand = " << sqlInsertCommandCharStar << endl;
								#endif
								performSQLrealInsertQuery(sqlInsertCommandCharStar, i);

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







long powLong(long val, int degree)
{
	long result = 1;
	for(int i=1; i<degree; i++)
	{
		result = result*val;
	}
	return result;
}


#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength, int concatonatedDctCoeffArrayBiasInt[])
#else
void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength)
#endif
#else
#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
unsigned long convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[], int concatonatedDctCoeffArrayBiasInt[])
#else
unsigned long convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(signed char concatonatedSignedDctCoeffArray[])
#endif
#endif
{
	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	*DCTcoeff64bitValueStringLength = 0;
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#else
	unsigned long dctCoeffArrayBinned = 0;
	#endif

	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		int arrayValueSigned = concatonatedSignedDctCoeffArray[i];
		#ifdef OR_DEBUG
		//cout << " concatonatedSignedDctCoeffArray[i] = " << int(concatonatedSignedDctCoeffArray[i]) << endl;
		#endif

		/*
		//OLD; perform interpolation before signed to unsigned conversion

		//perform binning only if necessary/requested
		int numDistintValsPerColumn = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
		if(numDistintValsPerColumn != 1)
		{
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			double arrayValueSignedDouble = double(arrayValueSigned)/double(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL);
			arrayValueSigned = arrayValueSigned/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;

			//cout << "arrayValueSignedDouble = " << arrayValueSignedDouble << endl;
			//cout << "arrayValueSigned = " << arrayValueSigned << endl;

			if(arrayValueSigned >= 0)
			{
				if(arrayValueSignedDouble >= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION+OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

				}
				else if(arrayValueSignedDouble <= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION-OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;

				}
				else
				{
					concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
				}
			}
			else
			{//arrayValueSigned < 0

				if(arrayValueSignedDouble >= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION+OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

				}
				else if(arrayValueSignedDouble <= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION-OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;

				}
				else
				{
					concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
				}
			}
			#else
			arrayValueSigned = arrayValueSigned/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
			#endif
		}

		if(arrayValueSigned > OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
		{
			arrayValueSigned = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
		}
		else if(arrayValueSigned < -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
		{
			arrayValueSigned = -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
		}
		int arrayValueUnsigned = arrayValueSigned + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;	//convert to unsigned
		*/

		#ifdef OR_DEBUG
		//cout << "arrayValueSigned = " << arrayValueSigned << endl;
		#endif

		double arrayValueUnsignedDouble;
		unsigned int arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);	//used to be int

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int numDistintValsPerColumn = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
		if(numDistintValsPerColumn != 1)
		{
			#ifdef OR_DEBUG
			//cout << "arrayValueUnsignedDouble = " << arrayValueUnsignedDouble << endl;
			//cout << "arrayValueUnsigned = " << arrayValueUnsigned << endl;
			#endif

			if(arrayValueUnsignedDouble >= (double(arrayValueUnsigned)+0.5-DOUBLE_MIN_PRECISION+OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
			{
				concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

			}
			else if(arrayValueUnsignedDouble <= (double(arrayValueUnsigned)+0.5-DOUBLE_MIN_PRECISION-OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
			{
				concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;
			}
			else
			{
				concatonatedDctCoeffArrayBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
			}
		}
		#endif

		#ifdef OR_DEBUG
		//cout << "arrayValueUnsigned = " << arrayValueUnsigned << endl;
		#endif

	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		//cout << "arrayValueUnsigned = " << arrayValueUnsigned << endl;
		for(int q=0; q<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS; q++)
		{
			if(index == 8)
			{
				#ifdef OR_DEBUG
				//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
				//cout << "int(binaryConvertedToChar) = " << int(binaryConvertedToChar) << endl;
				#endif
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
				#ifdef OR_DEBUG
				//cout << "false" << endl;
				#endif
			}
			else
			{//LSb == 1 ; therefore an odd number
				bitValue =  true;
				#ifdef OR_DEBUG
				//cout << "true" << endl;
				#endif
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
		int power = ((OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS-i-1)*2)+1; 	//13,11,9,7,5,3,1
		dctCoeffArrayBinned = dctCoeffArrayBinned + long(arrayValueUnsigned)*powLong(10, power);
		#ifdef OR_DEBUG
		//cout << "power = " << power << endl;
		//cout << i << ": arrayValueUnsigned = " << arrayValueUnsigned << ", dctCoeffArrayBinned = " << dctCoeffArrayBinned << endl;
		#endif
	#endif
	}

	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	//add final character to currentDirectoryCharStar string if necessary
	if(index > 0)
	{
		#ifdef OR_DEBUG
		//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
		//cout << "int(binaryConvertedToChar) = " << int(binaryConvertedToChar) << endl;
		#endif
		DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = binaryConvertedToChar;
		*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
	}

	//add null character onto end of string
	DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = '\0';
	*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
	#else
	#ifdef OR_DEBUG
	//cout << "finished: dctCoeffArrayBinned = " << dctCoeffArrayBinned << endl;
	#endif
	return dctCoeffArrayBinned;
	#endif
}





long calculateGeoxyBinMultiDimensional(int geoxBin[], int geoyBin[])
{
	long geoxyBin;
	if(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS == 2)
	{
		#ifdef OR_DEBUG
		//cout << "geoxBin[0] = " << geoxBin[0] << endl;
		//cout << "geoxBin[1] = " << geoxBin[1] << endl;
		//cout << "geoyBin[0] = " << geoyBin[0] << endl;
		//cout << "geoyBin[1] = " << geoyBin[1] << endl;
		#endif

		geoxyBin =
			 ((long(geoyBin[0])* OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS* OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS* OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
			+ (long(geoxBin[0])* OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS* OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
			+ (long(geoyBin[1])* OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
			+ (long(geoxBin[1])));

		#ifdef OR_DEBUG
		//cout << "geoxyBin = " << geoxyBin << endl;
		#endif
	}
	else
	{
		cout << "OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS != 2; this scenario has not been programmed" << endl;
	}
	return geoxyBin;
}



void convertConcatonatedSignedDctCoeffArrayAndGeoToLinearCombination(signed char concatonatedSignedDctCoeffArray[], int geoxBin[], int geoyBin[], unsigned long* linearCombination)
{
	//int linearCombinationArray[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS + OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
	int linearCombinationArray[OR_LINEAR_COMBINATION_ARRAY_MAX_SIZE];

	int index = 0;
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		int arrayValueSigned = concatonatedSignedDctCoeffArray[i];

		double arrayValueUnsignedDouble;
		unsigned int arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);		//used to be int

		linearCombinationArray[index] = arrayValueUnsigned;
		#ifdef OR_DEBUG
		//cout << "concatonatedSignedDctCoeffArray[i] = " << concatonatedSignedDctCoeffArray[i] << endl;
		//cout << "linearCombinationArray[index] = " << linearCombinationArray[index] << endl;
		#endif
		index++;
	}
	for(int i=0; i<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; i++)
	{
		linearCombinationArray[index] =  int(geoxBin[i]);
		index++;
	}
	for(int i=0; i<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; i++)
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

		#ifdef OR_DEBUG
		//cout << "linearCombinationArray[i] = " << int(linearCombinationArray[i]) << endl;
		//cout << "\tlinearCombination = " <<* linearCombination << endl;
		//cout << "\tlinearCombinationDouble = " << linearCombinationDouble << endl;
		#endif
	}
	*linearCombination = (unsigned long)(linearCombinationDouble/OR_IMAGE_COMPARISON_SQL_LINEAR_COMBINATION_NETWORK_DOUBLE_TO_U_LONG_CONVERSION);
	#ifdef OR_DEBUG
	//cout << "linearCombination = " <<* linearCombination << endl;
	#endif
}


int determineGeoBinX(double featurePointTransformedXpos)
{
	int geoBinX = (featurePointTransformedXpos / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
	return geoBinX;
}

int determineGeoBinY(double featurePointTransformedYpos)
{
	int geoBinY = (featurePointTransformedYpos / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
	return geoBinY;
}

double determineGeoBinDoubleX(double featurePointTransformedXpos)
{
	double geoBinX = (featurePointTransformedXpos / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
	return geoBinX;
}

double determineGeoBinDoubleY(double featurePointTransformedYpos)
{
	double geoBinY = (featurePointTransformedYpos / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
	return geoBinY;
}


unsigned int determineDCTBinUnsigned(int arrayValueSigned, double* arrayValueUnsignedDouble)
{
	if(arrayValueSigned > OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		arrayValueSigned = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else if(arrayValueSigned < -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		arrayValueSigned = -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}

	unsigned int arrayValueUnsigned;
	int numDistintValsPerColumn = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	if(numDistintValsPerColumn != 1)
	{
		*arrayValueUnsignedDouble = determineDCTBinUnsignedDouble(arrayValueSigned);
		arrayValueUnsigned = (unsigned int)(*arrayValueUnsignedDouble);
	}
	else
	{
		arrayValueUnsigned = arrayValueSigned + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
	}

	return arrayValueUnsigned;
}

double determineDCTBinUnsignedDouble(int arrayValueSigned)
{
	double arrayValueUnsignedDouble = double(arrayValueSigned)/double(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL) + double(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET);
	return arrayValueUnsignedDouble;
}



//#endif



