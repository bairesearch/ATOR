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
 * File Name: ORcomparison.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3f7d 16-August-2015
 *
 *******************************************************************************/


#include "ORcomparison.h"
#include "ORimagecomparison.h"
#ifdef OR_IMAGE_COMPARISON_SQL
#include "ORdatabaseSQL.h"
#include "ORdatabaseDecisionTree.h"
#endif
#include "ORdatabaseFileIO.h"
#include "ORpixelMaps.h"
#include "ORoperations.h"
#include "LDreferenceManipulation.h"
#include "SHAREDvars.h"	//required for writeStringToFileObject

#include "LDjpeg.h"

#ifndef LINUX
	#define OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
#endif



int passes;


#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
static ORfeatureContainer trainfeatureGeoCompBinArray[OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS][2];	//trainfeatureGeoCompBinArray now stores 2 feature combinations (instead of 1 feature)
#endif
#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
static ORfeatureContainer trainfeatureImageAverageColourCompBinArray[OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL][OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL][OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL];
#endif

//#ifdef OR_PRINT_ALGORITHM_AND_TIME_DETAILS
long time5aNormalisedSnapshotComparisonLoadComparisonDataTotal = 0.0;
long time5aNormalisedSnapshotComparisonLoadComparisonDataIndex = 0.0;

long time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal = 0.0;
long time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex = 0.0;
//#endif


//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	//see dct coeff selection criteria
static int dctCoeffSelectionArrayY[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y_MAX][2];	//2 is for x and y
static int dctCoeffSelectionArrayYCr[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr_MAX][2];	//2 is for x and y
static int dctCoeffSelectionArrayYCb[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb_MAX][2];	//2 is for x and y
void fillDCTcoeffSelectionArrays()
{
	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ABS_LUM_INFORMATION
	cout << "error: OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ABS_LUM_INFORMATION has not been programmed" << endl;
	exit(0);
	#endif

	dctCoeffSelectionArrayY[0][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X;
	dctCoeffSelectionArrayY[0][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y;
	dctCoeffSelectionArrayY[1][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X;
	dctCoeffSelectionArrayY[1][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y;
	dctCoeffSelectionArrayY[2][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X;
	dctCoeffSelectionArrayY[2][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y;
	dctCoeffSelectionArrayY[3][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X;
	dctCoeffSelectionArrayY[3][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y;
	dctCoeffSelectionArrayY[4][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X;
	dctCoeffSelectionArrayY[4][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y;
	dctCoeffSelectionArrayY[5][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X;
	dctCoeffSelectionArrayY[5][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y;
	dctCoeffSelectionArrayY[6][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X;
	dctCoeffSelectionArrayY[6][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y;
	dctCoeffSelectionArrayY[7][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X;
	dctCoeffSelectionArrayY[7][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y;
	dctCoeffSelectionArrayY[8][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X;
	dctCoeffSelectionArrayY[8][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y;

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		dctCoeffSelectionArrayYCr[0][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X;
		dctCoeffSelectionArrayYCr[0][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y;
		dctCoeffSelectionArrayYCr[1][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X;
		dctCoeffSelectionArrayYCr[1][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y;
		dctCoeffSelectionArrayYCr[2][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X;
		dctCoeffSelectionArrayYCr[2][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y;
		dctCoeffSelectionArrayYCr[3][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X;
		dctCoeffSelectionArrayYCr[3][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y;
		dctCoeffSelectionArrayYCr[4][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X;
		dctCoeffSelectionArrayYCr[4][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y;
		dctCoeffSelectionArrayYCr[5][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X;
		dctCoeffSelectionArrayYCr[5][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y;
		dctCoeffSelectionArrayYCr[6][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X;
		dctCoeffSelectionArrayYCr[6][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y;
		dctCoeffSelectionArrayYCr[7][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X;
		dctCoeffSelectionArrayYCr[7][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y;
		dctCoeffSelectionArrayYCr[8][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X;
		dctCoeffSelectionArrayYCr[8][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y;

		dctCoeffSelectionArrayYCb[0][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X;
		dctCoeffSelectionArrayYCb[0][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y;
		dctCoeffSelectionArrayYCb[1][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X;
		dctCoeffSelectionArrayYCb[1][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y;
		dctCoeffSelectionArrayYCb[2][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X;
		dctCoeffSelectionArrayYCb[2][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y;
		dctCoeffSelectionArrayYCb[3][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X;
		dctCoeffSelectionArrayYCb[3][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y;
		dctCoeffSelectionArrayYCb[4][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X;
		dctCoeffSelectionArrayYCb[4][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y;
		dctCoeffSelectionArrayYCb[5][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X;
		dctCoeffSelectionArrayYCb[5][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y;
		dctCoeffSelectionArrayYCb[6][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X;
		dctCoeffSelectionArrayYCb[6][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y;
		dctCoeffSelectionArrayYCb[7][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X;
		dctCoeffSelectionArrayYCb[7][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y;
		dctCoeffSelectionArrayYCb[8][0] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X;
		dctCoeffSelectionArrayYCb[8][1] = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y;
	}


	/*not possible; exceeeds 64bit;*/
	/*
	dctCoeffSelectionArrayY[0][0] = 0;
	dctCoeffSelectionArrayY[0][1] = 0;
	dctCoeffSelectionArrayY[1][0] = 1;
	dctCoeffSelectionArrayY[1][1] = 0;
	dctCoeffSelectionArrayY[2][0] = 2;
	dctCoeffSelectionArrayY[2][1] = 0;
	dctCoeffSelectionArrayY[3][0] = 1;
	dctCoeffSelectionArrayY[3][1] = 0;
	dctCoeffSelectionArrayY[4][0] = 1;
	dctCoeffSelectionArrayY[4][1] = 1;
	dctCoeffSelectionArrayY[5][0] = 1;
	dctCoeffSelectionArrayY[5][1] = 2;
	dctCoeffSelectionArrayY[6][0] = 2;
	dctCoeffSelectionArrayY[6][1] = 0;
	dctCoeffSelectionArrayY[7][0] = 2;
	dctCoeffSelectionArrayY[7][1] = 1;
	dctCoeffSelectionArrayY[8][0] = 2;
	dctCoeffSelectionArrayY[8][1] = 2;

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		dctCoeffSelectionArrayYCr[0][0] = 0;
		dctCoeffSelectionArrayYCr[0][1] = 0;
		dctCoeffSelectionArrayYCr[1][0] = 1;
		dctCoeffSelectionArrayYCr[1][1] = 0;
		dctCoeffSelectionArrayYCr[2][0] = 2;
		dctCoeffSelectionArrayYCr[2][1] = 0;
		dctCoeffSelectionArrayYCr[3][0] = 1;
		dctCoeffSelectionArrayYCr[3][1] = 0;
		dctCoeffSelectionArrayYCr[4][0] = 1;
		dctCoeffSelectionArrayYCr[4][1] = 1;
		dctCoeffSelectionArrayYCr[5][0] = 1;
		dctCoeffSelectionArrayYCr[5][1] = 2;
		dctCoeffSelectionArrayYCr[6][0] = 2;
		dctCoeffSelectionArrayYCr[6][1] = 0;
		dctCoeffSelectionArrayYCr[7][0] = 2;
		dctCoeffSelectionArrayYCr[7][1] = 1;
		dctCoeffSelectionArrayYCr[8][0] = 2;
		dctCoeffSelectionArrayYCr[8][1] = 2;

		dctCoeffSelectionArrayYCb[0][0] = 0;
		dctCoeffSelectionArrayYCb[0][1] = 0;
		dctCoeffSelectionArrayYCb[1][0] = 1;
		dctCoeffSelectionArrayYCb[1][1] = 0;
		dctCoeffSelectionArrayYCb[2][0] = 2;
		dctCoeffSelectionArrayYCb[2][1] = 0;
		dctCoeffSelectionArrayYCb[3][0] = 1;
		dctCoeffSelectionArrayYCb[3][1] = 0;
		dctCoeffSelectionArrayYCb[4][0] = 1;
		dctCoeffSelectionArrayYCb[4][1] = 1;
		dctCoeffSelectionArrayYCb[5][0] = 1;
		dctCoeffSelectionArrayYCb[5][1] = 2;
		dctCoeffSelectionArrayYCb[6][0] = 2;
		dctCoeffSelectionArrayYCb[6][1] = 0;
		dctCoeffSelectionArrayYCb[7][0] = 2;
		dctCoeffSelectionArrayYCb[7][1] = 1;
		dctCoeffSelectionArrayYCb[8][0] = 2;
		dctCoeffSelectionArrayYCb[8][1] = 2;
	}
	*/


}
//#endif


/*
//ORfeatureContainer testfeatureGeoCompArray[numberOfTestObjects][numberOfTestViewIndicies][numberOfTestZoomIndicies][maxNumberOfTestPolysAcrossAllObjects][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
#ifndef LINUX
#ifdef OR_METHOD_2DOD
	ORfeatureContainer testfeatureGeoCompArray[OR_METHOD2DOD_NUM_OF_OBJECTS][OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST][OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
#elif defined OR_METHOD_3DOD
	ORfeatureContainer testfeatureGeoCompArray[OR_METHOD3DOD_NUM_OF_OBJECTS][OR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST][OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
#endif
#endif
*/

#ifdef OR_IMAGE_COMPARISON_SQL
double compareNormalisedSnapshots(int numberOfTestPolys[], int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, string testObjectNameArray[], int numberOfTestObjects, int dimension, int numberOfTestZoomIndicies, int trainOrTest, int testViewNumber)
#else
double compareNormalisedSnapshots(int numberOfTrainPolys[], int numberOfTestPolys[], int numberOfTrainViewIndicies, int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, string trainObjectNameArray[], int numberOfTrainObjects, string testObjectNameArray[], int numberOfTestObjects, int dimension, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies, int testViewNumber)
#endif
{
	string currentTempFolder = getCurrentDirectory();

	ORobjectReferenceList* firstReferenceInSnapshotMatchObjectReferenceList = new ORobjectReferenceList();

	bool useGeneratedTestPixmapFiles = false;
	bool useGeneratedTrainPixmapFiles = false;
	bool OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = false;

	#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
	{
		if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
		{
		}
		else
		{
			useGeneratedTestPixmapFiles = true;
			useGeneratedTrainPixmapFiles = true;
			if(!OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
			{
				OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = true;
			}
		}
	}
	else
	{
		#ifdef OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
		cout << "Linux must be used or OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL" << endl;
		exit(0);
		#endif
		if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
		{
			useGeneratedTestPixmapFiles = true;
		}
		else
		{
			useGeneratedTestPixmapFiles = true;
			useGeneratedTrainPixmapFiles = true;
		}

		if(!OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
		{
			if(!OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
			{
				OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = true;
			}
		}
	}
	#else
	useGeneratedTestPixmapFiles = true;
	useGeneratedTrainPixmapFiles = true;
	OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = true;
	#endif


	bool doNotCheckContrast;
	int smallImageRatio;
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{
		smallImageRatio = OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO;
		#ifdef OR_METHOD_2DOD_DO_NOT_REQUIRE_SNAPSHOT_CONTRAST
		doNotCheckContrast = true;
		#else
		doNotCheckContrast = false;
		#endif
	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		smallImageRatio = OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO;
		#ifdef OR_METHOD_3DOD_DO_NOT_REQUIRE_SNAPSHOT_CONTRAST
		doNotCheckContrast = true;
		#else
		doNotCheckContrast = false;
		#endif
	}
	else
	{
		cout << "error: illegal dimension" << endl;
		exit(0);
	}


	ofstream writeFileObject(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME);
	#ifdef DEBUG_OR_OUTPUT_DT_BIN_WRITE_TO_FILE
	ofstream writeFileObjectDTbinDebug("DTbinDebug.txt");
	#endif
	#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
	ifstream parseFileObjectDTbinDebug("DTbinDebug.txt");
	string DTbinDebugArray[MAX_NUMBER_DT_BIN_RECORDS];
	int numDTbinRecords = 0;
	char DTbinRecordTemp[DT_BIN_RECORD_MAX_LENGTH];
	while(parseFileObjectDTbinDebug.getline(DTbinRecordTemp, DT_BIN_RECORD_MAX_LENGTH))
	{
		DTbinDebugArray[numDTbinRecords] = DTbinRecordTemp;
		numDTbinRecords++;
	}
	#endif


	if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{

		string ICRheader = "";
		if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS)
		{
			ICRheader = ICRheader + "<HTML><HEAD><TITLE>Results </TITLE><style type=\"text/css\">TD { font-size:50%; } </style></HEAD><BODY>Results<p>Project Version: 3f7d 16-August-2015<p>";
		}
		else
		{
			ICRheader = ICRheader + "<HTML><HEAD><TITLE>Image Comparitor Results For Normalised Snapshots (CONFIDENTIAL - PROVIDED UNDER CONTRACT)</TITLE><style type=\"text/css\">body{font-size:50%}</style></HEAD><BODY><h1>Image Comparitor Results For Normalised Snapshots (CONFIDENTIAL - PROVIDED UNDER CONTRACT)</h1><p>Source Version 2d2g<p>Richard Bruce Baxter Object Recognition Method - Australian Provisional Patent Filed";
		}


		writeStringToFileObject(ICRheader, &writeFileObject);

		string ICRtableStart = "";
		if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
		{
			ICRtableStart = ICRtableStart + "<TABLE width=\"100%\" cellpadding=\"3\" cellspacing=\"0\" border=\"1\"><TR><TD colspan=\"6\">Test</TD><TD colspan=\"6\">Train</TD><TD></TD></TR><TR><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>ORpolygon</TD><TD>Side</TD><TD>Image</TD><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>ORpolygon</TD><TD>Side</TD><TD>Image</TD><TD></TD></TR>";
		}
		else
		{
			ICRtableStart = ICRtableStart + "<TABLE width=\"100%\" cellpadding=\"3\" cellspacing=\"0\" border=\"1\"><TR><TD colspan=\"6\">Test</TD><TD colspan=\"6\">Train</TD><TD></TD></TR><TR><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>X</TD><TD>Y</TD><TD>Image</TD><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>X</TD><TD>Y</TD><TD>Image</TD><TD></TD></TR>";
		}
		writeStringToFileObject(ICRtableStart, &writeFileObject);

	}


	//optimisation;
	unsigned char* rgbMapTrain = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	unsigned char* rgbMapTest = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	//#ifdef OR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	double* traindepthMap = new double[imageWidthFacingPoly*imageHeightFacingPoly];
	double* testdepthMap = new double[imageWidthFacingPoly*imageHeightFacingPoly];
	//#endif

	int smallImageWidth = imageWidthFacingPoly/smallImageRatio;
	int smallImageHeight = imageHeightFacingPoly/smallImageRatio;

	#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
	unsigned char* trainrgbMapSmall = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	unsigned char* testrgbMapSmall = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	//#ifdef OR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	double* traindepthMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)];
	double* testdepthMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)];
	//#endif
	#endif

	//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	signed char* testdctCoeffArrayY = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* testdctCoeffArrayYCr = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* testdctCoeffArrayYCb = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* testdctCoeffArrayYDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* testdctCoeffArrayYCrDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* testdctCoeffArrayYCbDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char* testconcatonatedSignedDctCoeffArrayRequirement = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];

	//#endif

	//#ifdef OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL
	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	double* trainrgbDevMap = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
	double* testrgbDevMap = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	double* trainrgbDevMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*VEC_MAP_VEC_NUM_DIMENSIONS];
	double* testrgbDevMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*VEC_MAP_VEC_NUM_DIMENSIONS];
	unsigned char* trainrgbDev8BitSmallMap = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	unsigned char* testrgbDev8BitSmallMap = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	//#endif
	//#endif


#ifdef TEMPTEST3GEOACCURACY
	int numberOfNearestFeatures = calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "numberOfPolygonsPerFeature = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
		cout << "numberOfAdditionalFeatures = " << OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
		cout << "numberOfNearestFeatures = " << numberOfNearestFeatures << endl;
	}

	ORfeature* testFirstFeatureInNearestFeatureList = new ORfeature();
	ORfeature* trainFirstFeatureInNearestFeatureList = new ORfeature();

	generateNearestFeaturesList(testFirstFeatureInNearestFeatureList, numberOfNearestFeatures);
	generateNearestFeaturesList(trainFirstFeatureInNearestFeatureList, numberOfNearestFeatures);

	int maxNumberOfTrainPolysAcrossAllObjects = 0;
	int maxNumberOfTestPolysAcrossAllObjects = 0;

	for(int o=0; o<numberOfTrainObjects; o++)
	{
		for(int v=0; v<numberOfTrainViewIndicies; v++)
		{
			for(int z=0; z<numberOfTrainZoomIndicies; z++)
			{
				int temp = numberOfTrainPolys[(o*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(v*numberOfTrainZoomIndicies) + z];
				if(temp > maxNumberOfTrainPolysAcrossAllObjects)
				{
					maxNumberOfTrainPolysAcrossAllObjects = temp;
				}
			}
		}
	}
	for(int o=0; o<numberOfTestObjects; o++)
	{
		for(int v=0; v<numberOfTestViewIndicies; v++)
		{
			for(int z=0; z<numberOfTestZoomIndicies; z++)
			{
				int temp = numberOfTestPolys[(o*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(v*numberOfTestZoomIndicies) + z];
				if(temp > maxNumberOfTestPolysAcrossAllObjects)
				{
					maxNumberOfTestPolysAcrossAllObjects = temp;
				}
			}
		}
	}
#endif


	//generature featurelists now; this is an optimisation.

#ifdef OR_METHOD_GEOMETRIC_COMPARISON

	int numberOfNearestFeatures = calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);

	ORfeature* testFirstFeatureInNearestFeatureList = new ORfeature();
	ORfeature* trainFirstFeatureInNearestFeatureList = new ORfeature();

	generateNearestFeaturesList(testFirstFeatureInNearestFeatureList, numberOfNearestFeatures);
	generateNearestFeaturesList(trainFirstFeatureInNearestFeatureList, numberOfNearestFeatures);

	int maxNumberOfTestPolysAcrossAllObjects = 0;
	for(int o=0; o<numberOfTestObjects; o++)
	{
		for(int v=0; v<numberOfTestViewIndicies; v++)
		{
			for(int z=0; z<numberOfTestZoomIndicies; z++)
			{
				int temp = numberOfTestPolys[(o*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(v*numberOfTestZoomIndicies) + z];
				if(temp > maxNumberOfTestPolysAcrossAllObjects)
				{
					maxNumberOfTestPolysAcrossAllObjects = temp;
				}
			}
		}
	}

	#ifdef LINUX
	ORfeatureContainer testfeatureGeoCompArray[numberOfTestObjects][numberOfTestViewIndicies][numberOfTestZoomIndicies][maxNumberOfTestPolysAcrossAllObjects][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
	#else
	/*
	array<ORfeatureContainer ^> ^ testfeatureGeoCompArray = new array<ORfeatureContainer ^>(numberOfTestObjects*numberOfTestViewIndicies*numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*OR_METHOD_POLYGON_NUMBER_OF_SIDES);
	int tempdim1 = numberOfTestViewIndicies*numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*OR_METHOD_POLYGON_NUMBER_OF_SIDES
	int tempdim2 = numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*OR_METHOD_POLYGON_NUMBER_OF_SIDES
	int tempdim3 = maxNumberOfTestPolysAcrossAllObjects*OR_METHOD_POLYGON_NUMBER_OF_SIDES
	int tempdim4 = OR_METHOD_POLYGON_NUMBER_OF_SIDES
	*/
	#endif

	if(OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO)
	{
		for(int testObjectIndex=0; testObjectIndex<numberOfTestObjects; testObjectIndex++)
		{
			for(int testViewIndex=0; testViewIndex<numberOfTestViewIndicies; testViewIndex++)
			{//there will not be multiple testview indicies for 2D data (image) algorithm

				for(int testZoomIndex=0; testZoomIndex < numberOfTestZoomIndicies; testZoomIndex++)
				{
					for(int testPolyIndex=0; testPolyIndex<numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]; testPolyIndex++)
					{
						for(int testSideIndex=0; testSideIndex < OR_METHOD_POLYGON_NUMBER_OF_SIDES; testSideIndex++)
						{
							string testpolygonIndexString = convertIntToString(testPolyIndex);
							string testsideIndexString = convertIntToString(testSideIndex);
							string testviewIndexString = convertIntToString(testViewIndex+testViewNumber);
							string testzoomIndexString = convertIntToString(testZoomIndex);

							string testinterpolatedRGBMapFileNameForRayTracing;
							testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

							string testinterpolatedNearestFeaturesMapFileName;
							testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;

							ORfeature* firstFeatureInList = new ORfeature();

							bool ignoreOTfeatures;
							if(dimension == OR_METHOD2DOD_DIMENSIONS)
							{
								ignoreOTfeatures = true;
							}
							else if(dimension == OR_METHOD3DOD_DIMENSIONS)
							{
								#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
								ignoreOTfeatures = true;
								#else
								ignoreOTfeatures = false;
								#endif
							}

							createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, firstFeatureInList, true, false, ignoreOTfeatures);
							#ifndef OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
							(testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList = firstFeatureInList;
							#endif
						}
					}
				}
			}
		}
	}

	#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO

	int maxNumberOfTrainPolysAcrossAllObjects = 0;
	for(int o=0; o<numberOfTrainObjects; o++)
	{
		for(int v=0; v<numberOfTrainViewIndicies; v++)
		{
			for(int z=0; z<numberOfTrainZoomIndicies; z++)
			{
				int temp = numberOfTrainPolys[(o*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(v*numberOfTrainZoomIndicies) + z];
				if(temp > maxNumberOfTrainPolysAcrossAllObjects)
				{
					maxNumberOfTrainPolysAcrossAllObjects = temp;
				}
			}
		}
	}

	#ifndef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
	ORfeatureContainer trainfeatureGeoCompArray[numberOfTrainObjects][numberOfTrainViewIndicies][numberOfTrainZoomIndicies][maxNumberOfTrainPolysAcrossAllObjects][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
	#endif
	
	for(int trainObjectIndex=0; trainObjectIndex<numberOfTrainObjects; trainObjectIndex++)
	{
		for(int trainViewIndex=0; trainViewIndex<numberOfTrainViewIndicies; trainViewIndex++)
		{//there will not be multiple trainview indicies for 2D data (image) algorithm

			for(int trainZoomIndex=0; trainZoomIndex < numberOfTrainZoomIndicies; trainZoomIndex++)
			{
				for(int trainPolyIndex=0; trainPolyIndex<numberOfTrainPolys[(trainObjectIndex*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(trainViewIndex*numberOfTrainZoomIndicies) + trainZoomIndex]; trainPolyIndex++)
				{
					for(int trainSideIndex=0; trainSideIndex < OR_METHOD_POLYGON_NUMBER_OF_SIDES; trainSideIndex++)
					{
						string trainpolygonIndexString = convertIntToString(trainPolyIndex);
						string trainsideIndexString = convertIntToString(trainSideIndex);
						string trainviewIndexString = convertIntToString(trainViewIndex);
						string trainzoomIndexString = convertIntToString(trainZoomIndex);

						string traininterpolatedRGBMapFileNameForRayTracing;
						traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainObjectIndex] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

						string traininterpolatedNearestFeaturesMapFileName;
						traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
						//generateNearestFeaturesList(&(trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainPolyIndex][trainSideIndex]), numberOfNearestFeatures);

						ORfeature* firstFeatureInList = new ORfeature();

						bool ignoreOTfeatures;
						if(dimension == OR_METHOD2DOD_DIMENSIONS)
						{
							ignoreOTfeatures = true;

						}
						else if(dimension == OR_METHOD3DOD_DIMENSIONS)
						{
							#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
							ignoreOTfeatures = true;
							#else
							ignoreOTfeatures = false;
							#endif

						}
						createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, firstFeatureInList, true, false, ignoreOTfeatures);

						#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
						string trainrgbMapFacingPolyFileName = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;

						readImage(trainrgbMapFacingPolyFileName, rgbMapTrain);

						colour avgCol;
						calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTrain, &avgCol);

						int rBin = (int)(((double)(avgCol.r)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int gBin = (int)(((double)(avgCol.g)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int bBin = (int)(((double)(avgCol.b)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);

						ORfeatureContainer* currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[rBin][gBin][bBin]);
						// go to last feature container in list (ie append to list if list already has items)
						while(currentFeatureContainerInList->next != NULL)
						{
							currentFeatureContainerInList = currentFeatureContainerInList->next;
						}
						currentFeatureContainerInList->firstFeatureInFeatureList = firstFeatureInList;
						ORfeatureContainer* newFeatureContainer = new ORfeatureContainer();
						currentFeatureContainerInList->next = newFeatureContainer;
						#else
						(trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainZoomIndex][trainPolyIndex][trainSideIndex]).firstFeatureInFeatureList = firstFeatureInList;
						#endif
					}
				}
			}
		}
	}

	#elif defined OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

	int maxNumberOfTrainPolysAcrossAllObjects = 0;
	for(int o=0; o<numberOfTrainObjects; o++)
	{
		for(int v=0; v<numberOfTrainViewIndicies; v++)
		{
			for(int z=0; z<numberOfTrainZoomIndicies; z++)
			{
				int temp = numberOfTrainPolys[(o*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(v*numberOfTrainZoomIndicies) + z];
				if(temp > maxNumberOfTrainPolysAcrossAllObjects)
				{
					maxNumberOfTrainPolysAcrossAllObjects = temp;
				}
			}
		}
	}

	for(int trainObjectIndex=0; trainObjectIndex<numberOfTrainObjects; trainObjectIndex++)
	{
		for(int trainViewIndex=0; trainViewIndex<numberOfTrainViewIndicies; trainViewIndex++)
		{//there will not be multiple trainview indicies for 2D data (image) algorithm

			for(int trainZoomIndex=0; trainZoomIndex < numberOfTrainZoomIndicies; trainZoomIndex++)
			{
				for(int trainPolyIndex=0; trainPolyIndex<numberOfTrainPolys[(trainObjectIndex*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(trainViewIndex*numberOfTrainZoomIndicies) + trainZoomIndex]; trainPolyIndex++)
				{
					for(int trainSideIndex=0; trainSideIndex < OR_METHOD_POLYGON_NUMBER_OF_SIDES; trainSideIndex++)
					{
						string trainpolygonIndexString = convertIntToString(trainPolyIndex);
						string trainsideIndexString = convertIntToString(trainSideIndex);
						string trainviewIndexString = convertIntToString(trainViewIndex);
						string trainzoomIndexString = convertIntToString(trainZoomIndex);

						string traininterpolatedRGBMapFileNameForRayTracing;
						traininterpolatedRGBMapFileNameForRayTracing = trainObjectNameArray[trainObjectIndex] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

						string traininterpolatedNearestFeaturesMapFileName;
						traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
						ORfeature* firstFeatureInTempList = new ORfeature();

						bool ignoreOTfeatures;
						if(dimension == OR_METHOD2DOD_DIMENSIONS)
						{
							ignoreOTfeatures = true;
						}
						else if(dimension == OR_METHOD3DOD_DIMENSIONS)
						{
							#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
							ignoreOTfeatures = true;
							#else
							ignoreOTfeatures = false;
							#endif
						}
						createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, firstFeatureInTempList, true, false, ignoreOTfeatures);

						ORfeature* currentFeatureInTempList = firstFeatureInTempList;

						//now bin the features;
						int findex1 = 0;

						while(currentFeatureInTempList->next != NULL)
						{
							ORfeature* currentFeatureInTempList2 = firstFeatureInTempList;

							int findex2 = 0;
							while(currentFeatureInTempList2->next != NULL)
							{
								if(findex1 != findex2)
								{
									int x1Bin = determineGeoBinX(currentFeatureInTempList->pointTransformed.x);
									int y1Bin = determineGeoBinY(currentFeatureInTempList->pointTransformed.y);
									int x2Bin = determineGeoBinX(currentFeatureInTempList2->pointTransformed.x);
									int y2Bin = determineGeoBinY(currentFeatureInTempList2->pointTransformed.y);

									bool insideBin = false;

									if(x1Bin >= 0 )
									{
										if(x1Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
										{
											if(y1Bin >= 0 )
											{
												if(y1Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
												{
													if(x2Bin >= 0 )
													{
														if(x2Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
														{
															if(y2Bin >= 0 )
															{
																if(y2Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
																{
																	insideBin = true;
																	//this passes
																}
															}
														}
													}
												}
											}
										}
									}

									if(insideBin)
									{
										ORfeatureContainer* currentFeatureContainerInList1 = &(trainfeatureGeoCompBinArray[x1Bin][y1Bin][x2Bin][y2Bin][0]);
										ORfeatureContainer* currentFeatureContainerInList2 = &(trainfeatureGeoCompBinArray[x1Bin][y1Bin][x2Bin][y2Bin][1]);

										// go to last feature container in list (ie append to list if list already has items)
										while(currentFeatureContainerInList1->next != NULL)
										{
											currentFeatureContainerInList1 = currentFeatureContainerInList1->next;
										}
										while(currentFeatureContainerInList2->next != NULL)
										{
											currentFeatureContainerInList2 = currentFeatureContainerInList2->next;
										}
										currentFeatureContainerInList1->firstFeatureInFeatureList = currentFeatureInTempList;
										ORfeatureContainer* newFeatureContainer1 = new ORfeatureContainer();
										currentFeatureContainerInList1->next = newFeatureContainer1;
										currentFeatureContainerInList2->firstFeatureInFeatureList = currentFeatureInTempList2;
										ORfeatureContainer* newFeatureContainer2 = new ORfeatureContainer();
										currentFeatureContainerInList2->next = newFeatureContainer2;
									}

								}
								currentFeatureInTempList2 = currentFeatureInTempList2->next;
								findex2++;
							}

							currentFeatureInTempList = currentFeatureInTempList->next;
							findex1++;
						}
					}
				}
			}
		}
	}

	#ifdef OR_DEBUG_COMPARISON
	/*
	for(int x=0; x<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS; x++)
	{
		for(int y=0; y<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS; y++)
		{
			cout << "bin x = " << x << ", bin y = " << y << endl;
			ORfeatureContainer* currentFeatureContainerInList = &(trainfeatureGeoCompBinArray[x][y]);

			// go to last feature container in list (ie append to list if list already has items)
			while(currentFeatureContainerInList->next != NULL)
			{
				cout << "\t feature transformed point, x = " << currentFeatureContainerInList->firstFeatureInFeatureList->pointTransformed.x << ", y = " << currentFeatureContainerInList->firstFeatureInFeatureList->pointTransformed.y << ", z = " << currentFeatureContainerInList->firstFeatureInFeatureList->pointTransformed.z << endl;
				currentFeatureContainerInList = currentFeatureContainerInList->next;
			}
		}
	}
	*/
	#endif

	#endif


#endif

	passes = 0;

	double averageMatchErrorAcrossAllObjects = 0.0;

	for(int testObjectIndex=0; testObjectIndex<numberOfTestObjects; testObjectIndex++)
	{
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "\t\t  testObjectIndex = " << testObjectIndex << endl;
		}

		#ifndef OR_IMAGE_COMPARISON_SQL
		if(OR_IMAGE_COMPARISON_DECISION_TREE)
		{
			#ifndef TEMP_REMOVE_FLASH
			if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
			{
				string trainviewIndexStringTemp = convertIntToString(0);

				string mapFileName = trainObjectNameArray[testObjectIndex] + "initialViewMap" + "ViewIndex" + trainviewIndexStringTemp + "ZoomIndex" + "0";
				string blank = "";

				string convertPPMtoPNGCommand = "convert " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION + " " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION;
				if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
				{
					cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
				}
				system(convertPPMtoPNGCommand.c_str());

				if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
				{
					string convertPPMtoPNGCommand2 = "convert " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION + " " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION;
					if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
					{
						cout << "system(" << convertPPMtoPNGCommand2 << ");" << endl;
					}
					system(convertPPMtoPNGCommand2.c_str());
				}
			}
			#endif
		}
		#endif

		double averageMatchErrorAcrossAllViewIndicies = 0.0;

		for(int testViewIndex=0; testViewIndex<numberOfTestViewIndicies; testViewIndex++)
		{
			if(OR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "\t\t\t  testViewIndex = " << testViewIndex+testViewNumber << endl;
			}

			for(int testZoomIndex=0; testZoomIndex < numberOfTestZoomIndicies; testZoomIndex++)
			{
				if(OR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "\t\t\t  testZoomIndex = " << testZoomIndex << endl;
				}

				long time5zNormalisedSnapshotComparisonSingleViewTestStart;
				if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
				{
					#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					cout << "\t\t\t start: 5z. normalised snapshot comparison - single view speed test (for website)" << endl;
					#endif
					time5zNormalisedSnapshotComparisonSingleViewTestStart = getTimeAsLong();
				}
				int numPassesForTestViewIndex = 0;

				#ifndef TEMP_REMOVE_FLASH
				if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
				{
					string testviewIndexStringTemp = convertIntToString(testViewIndex+testViewNumber);
					string testzoomIndexStringTemp = convertIntToString(testZoomIndex);

					string mapFileName = testObjectNameArray[testObjectIndex] + "initialViewMap" + "ViewIndex" + testviewIndexStringTemp + "ZoomIndex" + testzoomIndexStringTemp;
					string blank = "";

					string convertPPMtoPNGCommand = "convert " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION + " " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION;
					if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
					{
						cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
					}
					system(convertPPMtoPNGCommand.c_str());

					if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
					{
						string convertPPMtoPNGCommand2 = "convert " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION + " " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION;
						if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
						{
							cout << "system(" << convertPPMtoPNGCommand2 << ");" << endl;
						}
						system(convertPPMtoPNGCommand2.c_str());
					}

					string ICRNewTestViewIndexRow3 = "<TR><TD colspan=\"6\"><img src=\"" + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION + "\" border=0> </TD><TD colspan=\"6\"><img src=\"" + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION + "\" border=0> </TD><TD></TD></TR>";
					writeStringToFileObject(ICRNewTestViewIndexRow3, &writeFileObject);
					if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
					{
						string ICRNewTestViewIndexRow4 = "<TR><TD colspan=\"6\"><img src=\"" + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION + "\" border=0> </TD><TD colspan=\"6\"><img src=\"" + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION + "\" border=0> </TD><TD></TD></TR>";
						writeStringToFileObject(ICRNewTestViewIndexRow4, &writeFileObject);
					}
				}
				#endif

				double averageMatchError = 0.0;

				for(int testPolyIndex=0; testPolyIndex<numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]; testPolyIndex++)
				{
					if(OR_PRINT_ALGORITHM_PROGRESS)
					{
						if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
						{
							cout << "\t\t\t\t  testPIndex = " << testPolyIndex << endl;
						}
					}

					int trainPolyIndexWithLowestErrorRecord[3][5];	//for each test side x3, 0=trainObjectIndex, 1=trainViewIndex, 2=trainViewIndex, 3=trainpolyindex, 4=trainsideIndex

					#ifndef OR_ASSERT_MATCHES_FOR_ALL_SIDES
					bool foundMatchRecord[3];	//for each test side
					double currentLowestErrorRecord[3];
					string trainObjectNameWithLowestErrorRecord[3];
					for(int i=0; i<3; i++)
					{
						foundMatchRecord[i] = false;
						currentLowestErrorRecord[i] = -1.0;
						trainObjectNameWithLowestErrorRecord[i] = "";
					}
					#endif

					#ifdef DEBUG_OR_OUTPUT_DT_BIN
					string DTbinWithLowestErrorRecord[3];	//for each test side
					int geoxBinWithLowestErrorRecord[3][2];				//for each test side
					int geoyBinWithLowestErrorRecord[3][2];				//for each test side
					long geoxybinWithLowestErrorRecord[3];				//for each test side
					#endif

					#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
					string trainObjectNameWithLowestErrorRecordAcrossAllSides;
					#endif

					double averageMatchErrorAcrossSides = 0.0;

					for(int testSideIndex=0; testSideIndex < OR_METHOD_POLYGON_NUMBER_OF_SIDES; testSideIndex++)
					{

						bool testMapLoaded;
						if(OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
						{
							testMapLoaded = false;
						}

						string testviewIndexString = convertIntToString(testViewIndex+testViewNumber);
						string testzoomIndexString = convertIntToString(testZoomIndex);
						string testpolygonIndexString = convertIntToString(testPolyIndex);
						string testsideIndexString = convertIntToString(testSideIndex);

						string testinterpolatedRGBMapFileNameForRayTracing;
						testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

						string testrgbMapSmallFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

						if(useGeneratedTestPixmapFiles)
						{
							if(!OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
							{

								string testrgbMapFacingPolyFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

								string testrgbMapFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

								readImage(testrgbMapFacingPolyFileName, rgbMapTest);

								#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
								readImage(testrgbMapSmallFacingPolyFileName, testrgbMapSmall);
								#endif

							}


							if(!OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								if(!OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO)
								{
									/*
									if(OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
									{
										string testviewIndexString = convertIntToString(testViewIndex+testViewNumber);
										string testzoomIndexString = convertIntToString(testZoomIndex);
										string testpolygonIndexString = convertIntToString(testPolyIndex);
										string testsideIndexString = convertIntToString(testSideIndex);
										string testinterpolatedRGBMapFileNameForRayTracing;
										testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
									}
									*/
									string testinterpolatedNearestFeaturesMapFileName;
									testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
									createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);
								}
							}

							#ifdef TEMPTEST3GEOACCURACY
							string testinterpolatedNearestFeaturesMapFileName;
							testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
							createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);
							//cout << "TEMPTEST3GEOACCURACY Part 2" << endl;
							//cout << "testinterpolatedNearestFeaturesMapFileName = " << testinterpolatedNearestFeaturesMapFileName << endl;
							//cout << "end" << endl;
							#endif
						}

						double currentLowestError = VERY_HIGH_ERROR;

					#ifdef OR_METHOD_GEOMETRIC_COMPARISON_BINNING

						ORfeature* currentTestFeature;

						if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
						{
							currentTestFeature = new ORfeature();
							long testID = 0;							//this will be used in the future to shortcut identify test snapshots for a particular object at a particular viewindex and zoom that have been recently added to database and require comparision against the entire database
							string sqlDatabaseTestTableName = OR_MYSQL_TABLE_NAME_TEST;			//this will be used in the future to shortcut identify test snapshots for a particular object at a particular viewindex and zoom that have been recently added to database and require comparision against the entire database
							bool ignoreOTfeaturestest;
							if(dimension == OR_METHOD2DOD_DIMENSIONS)
							{
								ignoreOTfeaturestest = true;

							}
							else if(dimension == OR_METHOD3DOD_DIMENSIONS)
							{
								#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
								ignoreOTfeaturestest = true;
								#else
								ignoreOTfeaturestest = false;
								#endif

							}
							createFeatureListUsingDatabaseQuery(currentTestFeature, true, false, ignoreOTfeaturestest, sqlDatabaseTestTableName, testID, testObjectNameArray[testObjectIndex], testViewIndex+testViewNumber, testZoomIndex, testPolyIndex, testSideIndex, false);

							if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
							{
								convertSQLdatabaseStringToSnapshotMaps(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, testrgbDevMap, testdepthMap, smallImageWidth, smallImageHeight, testrgbMapSmall, testdepthMapSmall, testrgbDevMapSmall, dimension, currentTestFeature->snapshotMapsText, OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS);
							}
						}
						else
						{
							if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
							{
								#ifndef OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
								currentTestFeature = (testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList;
								#endif
							}
							else
							{
								currentTestFeature = new ORfeature(); 	//create dummy feature - not used.
							}
						}

						colour normalisedAverageHueDeviationRequirement;
						if(OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
						{
							if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								normalisedAverageHueDeviationRequirement.r = currentTestFeature->avgCol.r;
								normalisedAverageHueDeviationRequirement.g = currentTestFeature->avgCol.g;
								normalisedAverageHueDeviationRequirement.b = currentTestFeature->avgCol.b;
								#ifdef OR_DEBUG_COMPARISON
								cout << "normalisedAverageHueDeviationRequirement.r = " << int(normalisedAverageHueDeviationRequirement.r) << endl;
								cout << "normalisedAverageHueDeviationRequirement.g = " << int(normalisedAverageHueDeviationRequirement.g) << endl;
								cout << "normalisedAverageHueDeviationRequirement.b = " << int(normalisedAverageHueDeviationRequirement.b) << endl;
								#endif
							}
							else
							{
								double normalisedAverageHueContrastR;
								double normalisedAverageHueContrastG;
								double normalisedAverageHueContrastB;
								double averageContrastWithRespectToAverageColour;
								double averageStarkContrastWithRespectToAverageColour;
								double averageLocalContrast;
								double averageLocalStarkContrast;
								createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(imageWidthFacingPoly, imageWidthFacingPoly, rgbMapTest, testrgbDevMap, &normalisedAverageHueContrastR, &normalisedAverageHueContrastG, &normalisedAverageHueContrastB, &averageContrastWithRespectToAverageColour, &averageStarkContrastWithRespectToAverageColour, &averageLocalContrast, &averageLocalStarkContrast);
								vec normalisedAverageHueContrast;
								normalisedAverageHueContrast.x = normalisedAverageHueContrastR;
								normalisedAverageHueContrast.y = normalisedAverageHueContrastG;
								normalisedAverageHueContrast.z = normalisedAverageHueContrastB;
								colour cullednormalisedAverageHueContrast;

								cullAndBinNormalisedHueContrast(&normalisedAverageHueContrast, &cullednormalisedAverageHueContrast);
								#ifdef OR_DEBUG_COMPARISON
								cout << "normalisedAverageHueContrast.x = " << normalisedAverageHueContrast.x << endl;
								cout << "normalisedAverageHueContrast.y = " << normalisedAverageHueContrast.y << endl;
								cout << "normalisedAverageHueContrast.z = " << normalisedAverageHueContrast.z << endl;
								cout << "cullednormalisedAverageHueContrast->r = " << int(cullednormalisedAverageHueContrast.r) << endl;
								cout << "cullednormalisedAverageHueContrast->g = " << int(cullednormalisedAverageHueContrast.g) << endl;
								cout << "cullednormalisedAverageHueContrast->b = " << int(cullednormalisedAverageHueContrast.b) << endl;
								#endif

								normalisedAverageHueDeviationRequirement.r = cullednormalisedAverageHueContrast.r;
								normalisedAverageHueDeviationRequirement.g = cullednormalisedAverageHueContrast.g;
								normalisedAverageHueDeviationRequirement.b = cullednormalisedAverageHueContrast.b;
							}
						}

						unsigned char* rgb8bitSmallMapForInstantDBqueryAccessRequirement;
						if(OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
						{
							/*
							if(OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS)
							{//optimised, rgb dev maps have already been created

							}
							else
							{
							*/

							double smallTempnormalisedAverageRHueContrast;
							double smallTempnormalisedAverageGHueContrast;
							double smallTempnormalisedAverageBHueContrast;
							double smallTempaverageContrastWithRespectToAverageColour;
							double smallTempaverageStarkContrastWithRespectToAverageColour;
							double smallTempaverageLocalContrast;
							double smallTempaverageLocalStarkContrast;
							createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(smallImageWidth, smallImageHeight, testrgbMapSmall, testrgbDevMapSmall, &smallTempnormalisedAverageRHueContrast, &smallTempnormalisedAverageGHueContrast, &smallTempnormalisedAverageBHueContrast, &smallTempaverageContrastWithRespectToAverageColour, &smallTempaverageStarkContrastWithRespectToAverageColour, &smallTempaverageLocalContrast, &smallTempaverageLocalStarkContrast);
							convertNormalisedHueDeviationMapTo3x8bitMap(smallImageWidth, smallImageHeight, testrgbDevMapSmall, testrgbDev8BitSmallMap);
							rgb8bitSmallMapForInstantDBqueryAccessRequirement = testrgbDev8BitSmallMap;
						}
						else
						{
							rgb8bitSmallMapForInstantDBqueryAccessRequirement = NULL;
						}

						signed char* testconcatonatedSignedDctCoeffArrayRequirement = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
						{
							if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								testconcatonatedSignedDctCoeffArrayRequirement = currentTestFeature->dctCoeff;

								#ifdef OR_DEBUG_COMPARISON
								for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
								{
									cout << "testconcatonatedSignedDctCoeffArrayRequirement[i] = " << int(testconcatonatedSignedDctCoeffArrayRequirement[i]) << endl;
								}
								cout << "stored currentTestFeature->dctCoeffArrayBinned = " << currentTestFeature->dctCoeffArrayBinned << endl;
								#endif
							}
							else
							{
								string testrgbMapSmallFacingPolyFileNameJPEG = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + JPG_EXTENSION;

								readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(&testrgbMapSmallFacingPolyFileName, &testrgbMapSmallFacingPolyFileNameJPEG, testconcatonatedSignedDctCoeffArrayRequirement, false);
							}
						}
						else
						{
							testconcatonatedSignedDctCoeffArrayRequirement = NULL;
						}

						//#ifdef OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
						bool fLoopOn = true;
						bool fLoopSingle = false;
						if(!OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							fLoopSingle = true;
						}

						ORfeature* testcurrentFeatureInNearestFeatureList = currentTestFeature;
						int findex1 = 0;
						while((testcurrentFeatureInNearestFeatureList->next != NULL) && fLoopOn)
						{
							ORfeature* testcurrentFeatureInNearestFeatureList2 = currentTestFeature;
							int findex2 = 0;
							while((testcurrentFeatureInNearestFeatureList2->next != NULL) && fLoopOn)
							{
								if((findex1 != findex2) || fLoopSingle)
								{
									if(fLoopSingle)
									{
										fLoopOn = false;
									}

									int xBin;
									int yBin;
									int x2Bin;
									int y2Bin;

									bool geoBinWithinRange = false;
									if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
									{
										xBin = determineGeoBinX(testcurrentFeatureInNearestFeatureList->pointTransformed.x);
										yBin = determineGeoBinY(testcurrentFeatureInNearestFeatureList->pointTransformed.y);
										x2Bin = determineGeoBinX(testcurrentFeatureInNearestFeatureList2->pointTransformed.x);
										y2Bin = determineGeoBinY(testcurrentFeatureInNearestFeatureList2->pointTransformed.y);


										#ifndef OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NO_EXPLICIT_FOR_LOOPS
										if((xBin > 0)
										&& (xBin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (yBin > 0)
										&& (yBin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
										&& (x2Bin > 0)
										&& (x2Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (y2Bin > 0)
										&& (y2Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1))
										#else
										if((xBin >= 0)
										&& (xBin <= OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (yBin >= 0)
										&& (yBin <= OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
										&& (x2Bin >= 0)
										&& (x2Bin <= OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (y2Bin >= 0)
										&& (y2Bin <= OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1))
										#endif
										{
											geoBinWithinRange = true;
										}
									}
									else
									{
										geoBinWithinRange = true;
									}

									if(geoBinWithinRange)
									{

										//check current bin and nearest bins
										#ifndef OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NO_EXPLICIT_FOR_LOOPS
										for(int x = xBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=xBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
										{
											for(int y = yBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=yBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
											{
												//check current bin and nearest bins
												for(int x2 = x2Bin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2<=x2Bin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2++)
												{
													for(int y2 = y2Bin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2<=y2Bin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2++)
													{
										#else

										int x;
										int y;
										int x2;
										int y2;

										if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
										{
											x = xBin;
											y = yBin;
											x2 = x2Bin;
											y2 = y2Bin;
										}
										else
										{
											x = 0;	//not used
											y = 0;	//not used
											x2 = 0;	//not used
											y2 = 0;	//not used
										}
										#endif

									#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

										ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin =  &(trainfeatureGeoCompBinArray[x][y][x2][y2][0]);
										ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin2 =  &(trainfeatureGeoCompBinArray[x][y][x2][y2][1]);
										ORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
										ORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin2 = firstFeatureContainerInTestFeatureMatchingTrainBin2;

									#else

										int geoxBin[2];
										int geoyBin[2];
										long geoxybin = 0.0;

										geoxBin[0] = x;
										geoyBin[0] = y;
										geoxBin[1] = x2;
										geoyBin[1] = y2;

										if(!OR_IMAGE_COMPARISON_DECISION_TREE)
										{
											#ifdef OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V1
											geoxybin = calculateGeoxyBinMultiDimensional(geoxBin, geoyBin);
											#ifdef OR_DEBUG_COMPARISON
											cout << "geoxybin = " << geoxybin << endl;
											#endif
											#endif
										}

										bool ignoreOTfeaturestrain;
										if(dimension == OR_METHOD2DOD_DIMENSIONS)
										{
											ignoreOTfeaturestrain = true;
										}
										else if(dimension == OR_METHOD3DOD_DIMENSIONS)
										{
											#ifdef OR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
											ignoreOTfeaturestrain = true;
											#else
											ignoreOTfeaturestrain = false;
											#endif
										}


										#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
										bool DTbinDebugMatchFound = false;
										#endif

										ORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin;
										ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin = new ORfeatureContainer();

										string DTbin = "";
										#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
										//assert OR_IMAGE_COMPARISON_DECISION_TREE
										int exceptionModifier = 1;	//positive
										for(int exceptionIndex=0; exceptionIndex<(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)+1; exceptionIndex++)
										{
											if(OR_IMAGE_COMPARISON_DECISION_TREE)
											{
												if(exceptionIndex != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)
												{
													testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2] = testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2]+exceptionModifier;
												}
										#else
										if(OR_IMAGE_COMPARISON_DECISION_TREE)
										{
										#endif
											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL_DIRECT_ACCESS
											createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(firstFeatureContainerInTestFeatureMatchingTrainBin, ignoreOTfeaturestrain, smallImageWidth, smallImageHeight, testrgbMapSmall, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, trainOrTest);
											#else

											#ifdef OR_DEBUG_COMPARISON
											for(int z=0; z<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; z++)
											{
												cout << "concatonatedSignedDctCoeffArrayRequirement[z] = " << int(testconcatonatedSignedDctCoeffArrayRequirement[z]) << endl;
											}
											#endif

											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;

											ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList = new ORsnapshotIDreferenceList();
											ORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDreferenceList;

											getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(smallImageWidth, smallImageHeight, testrgbMapSmall, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, firstReferenceInSnapshotIDreferenceList, trainOrTest, &DTbin);
											#ifdef OR_DEBUG_COMPARISON
											cout << "DTbin = " << DTbin << endl;
											#endif

											#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
											for(int i=0; i<numDTbinRecords; i++)
											{
												if(DTbinDebugArray[i] == DTbin)
												{
													DTbinDebugMatchFound = true;
													cout << "match found: DTbin = " << DTbin << endl;
												}
											}
											#endif

											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											createAndOrParseIntoDirectory(&imageComparisonTreeBaseDirectory, NULL, false, false);	//reset current directory
											#endif

											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;

											int refcount = 0;
											while(currentReferenceInSnapshotIDReferenceList->next != NULL)
											{
												#ifdef OR_DEBUG_COMPARISON
												cout << "found c: refcount" << refcount << endl;
												cout << "currentReferenceInSnapshotIDReferenceList->referenceID = " << currentReferenceInSnapshotIDReferenceList->referenceID << endl;
												#endif

												int irrelevant = 0;
												ORfeature* firstNewFeature = new ORfeature();
												currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList = firstNewFeature;
												createFeatureListUsingDatabaseQuery(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList, true, false, ignoreOTfeaturestrain, OR_MYSQL_TABLE_NAME_TRAIN, currentReferenceInSnapshotIDReferenceList->referenceID, "", irrelevant, irrelevant, irrelevant, irrelevant, true);
												ORfeatureContainer* newFeatureContainer = new ORfeatureContainer();
												currentFeatureContainerInTestFeatureMatchingTrainBin->next = newFeatureContainer;
												currentFeatureContainerInTestFeatureMatchingTrainBin = currentFeatureContainerInTestFeatureMatchingTrainBin->next;

												currentReferenceInSnapshotIDReferenceList = currentReferenceInSnapshotIDReferenceList->next;

												refcount++;
											}

											delete firstReferenceInSnapshotIDreferenceList;
											#endif
											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
										}
										else
										{
											createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(firstFeatureContainerInTestFeatureMatchingTrainBin, true, false, ignoreOTfeaturestrain, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, rgb8bitSmallMapForInstantDBqueryAccessRequirement, smallImageWidth, smallImageHeight, OR_MYSQL_TABLE_NAME_TRAIN, trainOrTest);
											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
											if(exceptionIndex != 0)
											{
												currentFeatureContainerInTestFeatureMatchingTrainBin = NULL;
											}
											#endif
										}
									#endif

										#ifdef OR_DEBUG_COMPARISON
										cout << "xBin = " << xBin << endl;
										cout << "yBin = " << yBin << endl;
										cout << "x2Bin = " << x2Bin << endl;
										cout << "y2Bin = " << y2Bin << endl;
										cout << "x = " << x << endl;
										cout << "y = " << y << endl;
										cout << "x2 = " << x2 << endl;
										cout << "y2 = " << y2 << endl;
										cout << "firstFeatureContainerInTestFeatureMatchingTrainBin->next = " << firstFeatureContainerInTestFeatureMatchingTrainBin->next << endl;
										#endif

										int indx = 0;

										while(currentFeatureContainerInTestFeatureMatchingTrainBin->next != NULL)
										{
											passes++;

											#ifdef OR_DEBUG_COMPARISON
											cout << "indx = " << indx << endl;

											cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x << endl;
											cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y << endl;
											cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.z = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.z << endl;
											cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x << endl;
											cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y << endl;
											cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.z = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.z << endl;

											cout << "testcurrentFeatureInNearestFeatureList->pointTransformed.x = " << testcurrentFeatureInNearestFeatureList->pointTransformed.x << endl;
											cout << "testcurrentFeatureInNearestFeatureList->pointTransformed.y = " << testcurrentFeatureInNearestFeatureList->pointTransformed.y << endl;
											cout << "testcurrentFeatureInNearestFeatureList->pointTransformed.z = " << testcurrentFeatureInNearestFeatureList->pointTransformed.z << endl;
											#endif

											indx++;

											bool passedGeometricCheck = false;

											if(OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY)
											{
												if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
												{
													#ifdef USE_GEO_TEST_GRID_SIZE_LARGE
													passedGeometricCheck = true;
													#else
													double currentDistanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed));
													double xDev1 = testcurrentFeatureInNearestFeatureList->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x;
													double yDev1 = testcurrentFeatureInNearestFeatureList->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y;
													if(currentDistanceBetweenTwoPoints < OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
													{
														#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
														double currentDistanceBetweenTwoPoints2 = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList2->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed));
														double xDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.x;
														double yDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.y;
														#else
														double currentDistanceBetweenTwoPoints2 = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList2->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed));
														double xDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x;
														double yDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y;
														#endif

														if(currentDistanceBetweenTwoPoints2 < OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
														{
															#ifdef OR_GEOMETRIC_CHECK_COMPARISON_ERROR_DEV_CHECK
															if(sqrt(pow(absDouble(xDev1 - xDev2),2.0), pow(absDouble(yDev1 - yDev2), 2.0)) < OR_GEOMETRIC_CHECK_COMPARISON_ERROR_DEV)
															{
															#endif
																passedGeometricCheck = true;
															#ifdef OR_GEOMETRIC_CHECK_COMPARISON_ERROR_DEV_CHECK
															}
															#endif
														}
													}
													#endif
												}
												else
												{
													passedGeometricCheck = true;
												}
											}
											else
											{
												passedGeometricCheck = true;
											}

											if(passedGeometricCheck)
											{
												string trainObjectString = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->objectName;
												int trainViewIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->viewIndex;
												int trainZoomIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->zoomIndex;
												int trainPolyIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->polyIndex;
												int trainSideIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->sideIndex;

												#ifdef OR_DEBUG_COMPARISON
												cout << "\n\t\tPASSED GeometricCheck" << endl;
												cout << "\t\t\t testPolyIndex = " << testPolyIndex << endl;
												cout << "\t\t passedGeometricCheck" << endl;

												cout << "testObjectIndex = " << testObjectIndex << endl;
												cout << "testViewIndex = " << testViewIndex+testViewNumber << endl;
												cout << "testZoomIndex = " << testZoomIndex << endl;
												cout << "testPolyIndex = " << testPolyIndex << endl;
												cout << "testSideIndex = " << testSideIndex << endl;
												cout << testcurrentFeatureInNearestFeatureList->OTpointIndex << "\t" << testcurrentFeatureInNearestFeatureList->pointTransformed.x <<  "\t" << testcurrentFeatureInNearestFeatureList->pointTransformed.y <<  "\t" << testcurrentFeatureInNearestFeatureList->pointTransformed.z << "\t" << testcurrentFeatureInNearestFeatureList->point.x <<  "\t" << testcurrentFeatureInNearestFeatureList->point.y <<  "\t" << testcurrentFeatureInNearestFeatureList->point.z <<endl;
												cout << testcurrentFeatureInNearestFeatureList2->OTpointIndex << "\t" << testcurrentFeatureInNearestFeatureList2->pointTransformed.x <<  "\t" << testcurrentFeatureInNearestFeatureList2->pointTransformed.y <<  "\t" << testcurrentFeatureInNearestFeatureList2->pointTransformed.z << "\t" << testcurrentFeatureInNearestFeatureList2->point.x <<  "\t" << testcurrentFeatureInNearestFeatureList2->point.y <<  "\t" << testcurrentFeatureInNearestFeatureList2->point.z <<endl;

												cout << "trainObjectString = " << trainObjectString << endl;
												cout << "trainViewIndex = " << trainViewIndex << endl;
												cout << "trainZoomIndex = " << trainZoomIndex << endl;
												cout << "trainPolyIndex = " << trainPolyIndex << endl;
												cout << "trainSideIndex = " << trainSideIndex << endl;
												cout << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->OTpointIndex << "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.z << "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->point.x <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->point.y <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->point.z <<endl;
												//cout << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->OTpointIndex << "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.x <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.y <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.z << "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->point.x <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->point.y <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->point.z <<endl;
												#endif

												string trainpolygonIndexString = convertIntToString(trainPolyIndex);
												string trainsideIndexString = convertIntToString(trainSideIndex);
												string trainviewIndexString = convertIntToString(trainViewIndex);
												string trainzoomIndexString = convertIntToString(trainZoomIndex);

												string traininterpolatedRGBMapFileNameForRayTracing;
												traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

					#else



						#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL

						colour avgColTestMap;
						calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, &avgColTestMap);

						int rBinMid = (int)(((double)avgColTestMap.r/MAX_RGB_VAL)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int gBinMid = (int)(((double)avgColTestMap.g/MAX_RGB_VAL)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int bBinMid = (int)(((double)avgColTestMap.b/MAX_RGB_VAL)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);

						#ifdef OR_DEBUG_COMPARISON
						cout << "avgColTestMap.r = " << (int)avgColTestMap.r << endl;
						cout << "avgColTestMap.g = " << (int)avgColTestMap.g << endl;
						cout << "avgColTestMap.b = " << (int)avgColTestMap.b << endl;
						cout << "rBinMid = " << rBinMid << endl;
						cout << "gBinMid = " << gBinMid << endl;
						cout << "bBinMid = " << bBinMid << endl;
						#endif

						int rBinMin = rBinMid-(OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int rBinMax = rBinMid+(OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int gBinMin = gBinMid-(OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int gBinMax = gBinMid+(OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int bBinMin = bBinMid-(OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int bBinMax = bBinMid+(OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);

						#ifndef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_ALLOW_LIGHTING_COLOUR_SATURATION_VARIATION
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
						#endif

						for(int r = rBinMin; r < rBinMax; r++)
						{
							for(int g = gBinMin; g < gBinMax; g++)
							{
								for(int b = bBinMin; b < bBinMax; b++)
								{
									#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_ALLOW_LIGHTING_COLOUR_SATURATION_VARIATION
									for(int lightColourSaturationVar = 0-OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_LIGHTING_COLOUR_SATURATION_VARIATION_VAL_PER_COL; lightColourSaturationVar < 0+OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_LIGHTING_COLOUR_SATURATION_VARIATION_VAL_PER_COL; lightColourSaturationVar++)
									{
										int rColourSaturationVaried = r + lightColourSaturationVar;
										int gColourSaturationVaried = g + lightColourSaturationVar;
										int bColourSaturationVaried = b + lightColourSaturationVar;

										bool performImgCheck = true;
										if(rColourSaturationVaried < 0)
										{
											performImgCheck = false;
										}
										if(rColourSaturationVaried > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
										{
											performImgCheck = false;
										}
										if(gColourSaturationVaried < 0)
										{
											performImgCheck = false;
										}
										if(gColourSaturationVaried > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
										{
											performImgCheck = false;
										}
										if(bColourSaturationVaried < 0)
										{
											performImgCheck = false;
										}
										if(bColourSaturationVaried > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
										{
											performImgCheck = false;
										}

										if(performImgCheck)
										{
											ORfeatureContainer* currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[rColourSaturationVaried][gColourSaturationVaried][bColourSaturationVaried]);
									#else
										ORfeatureContainer* currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[r][g][b]);
									#endif

										while(currentFeatureContainerInList->next != NULL)
										{
											string trainObjectString = currentFeatureContainerInList->firstFeatureInFeatureList->objectName;
											int trainViewIndex = currentFeatureContainerInList->firstFeatureInFeatureList->viewIndex;
											int trainZoomIndex = currentFeatureContainerInList->firstFeatureInFeatureList->zoomIndex;
											int trainPolyIndex = currentFeatureContainerInList->firstFeatureInFeatureList->polyIndex;
											int trainSideIndex = currentFeatureContainerInList->firstFeatureInFeatureList->sideIndex;

						#else

						for(int trainObjectIndex=0; trainObjectIndex<numberOfTrainObjects; trainObjectIndex++)
						{
							for(int trainViewIndex=0; trainViewIndex<numberOfTrainViewIndicies; trainViewIndex++)
							{//there will not be multiple trainview indicies for 2D data (image) algorithm

								for(int trainZoomIndex=0; trainZoomIndex<numberOfTrainZoomIndicies; trainZoomIndex++)
								{
									for(int trainPolyIndex=0; trainPolyIndex<numberOfTrainPolys[(trainObjectIndex*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(trainViewIndex*numberOfTrainZoomIndicies) + trainZoomIndex]; trainPolyIndex++)
									{
										for(int trainSideIndex=0; trainSideIndex < OR_METHOD_POLYGON_NUMBER_OF_SIDES; trainSideIndex++)
										{

											string trainObjectString = objectNameArray[trainObjectIndex];

						#endif

										string trainpolygonIndexString = convertIntToString(trainPolyIndex);
										string trainsideIndexString = convertIntToString(trainSideIndex);
										string trainviewIndexString = convertIntToString(trainViewIndex);
										string trainzoomIndexString = convertIntToString(trainZoomIndex);

										string traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

										#ifdef OR_METHOD_GEOMETRIC_COMPARISON
										bool exactMatchFoundIrrelevant;
										#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO
											#ifndef OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
											#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
											bool passedGeometricCheck = compareFeaturesListForMatch((testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList, currentFeatureContainerInList->firstFeatureInFeatureList, dimension, &exactMatchFoundIrrelevant);
											#else
											bool passedGeometricCheck = compareFeaturesListForMatch((testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList, (trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainZoomIndex][trainPolyIndex][trainSideIndex]).firstFeatureInFeatureList, dimension, &exactMatchFoundIrrelevant);
											#endif
											#endif
										#else
											string traininterpolatedNearestFeaturesMapFileName;
											traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
											createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainFirstFeatureInNearestFeatureList, false, false, false);
											bool passedGeometricCheck = compareFeaturesListForMatch(testFirstFeatureInNearestFeatureList, trainFirstFeatureInNearestFeatureList, dimension, &exactMatchFoundIrrelevant);
										#endif
										#else
										bool passedGeometricCheck = true;
										#endif

										if(passedGeometricCheck)
										{
											#ifdef OR_DEBUG_COMPARISON
											cout << "PASSED GeometricCheck" << endl;
											#endif

					#endif
										#ifndef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE_OLD_ADDITIONAL

											long time5aNormalisedSnapshotComparisonLoadComparisonDataStart;
											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "passedGeometricCheck" << endl;
													cout << "\t\t\t start: 5a. normalised snapshot comparison - load snapshot data for comparison" << endl;
												}
												time5aNormalisedSnapshotComparisonLoadComparisonDataStart = getTimeAsLong();
											}

											if(useGeneratedTestPixmapFiles)
											{
												if(OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
												{
													if(!testMapLoaded)
													{//optimisation;

														testMapLoaded = true;

														string testviewIndexString = convertIntToString(testViewIndex+testViewNumber);
														string testzoomIndexString = convertIntToString(testZoomIndex);
														string testpolygonIndexString = convertIntToString(testPolyIndex);
														string testsideIndexString = convertIntToString(testSideIndex);

														string testinterpolatedRGBMapFileNameForRayTracing;
														testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

														string testrgbMapFacingPolyFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

														string testrgbMapFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

														readImage(testrgbMapFacingPolyFileName, rgbMapTest);

														#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
														string testrgbMapSmallFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

														readImage(testrgbMapSmallFacingPolyFileName, testrgbMapSmall);
														#endif
													}
												}
											}

											if(useGeneratedTrainPixmapFiles)
											{
												#ifdef OR_USE_DATABASE
												#ifdef OR_DATABASE_DEBUG
												cout << "DBgenerateFolderName: trainObjectString = " << trainObjectString << endl;
												#endif
												DBgenerateFolderName(&trainObjectString, true);
												#endif

												string trainrgbMapFacingPolyFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;

												string trainrgbMapFacingPolyFileName = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
												#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
												string trainrgbMapSmallFacingPolyFileName= traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
												#endif

												readImage(trainrgbMapFacingPolyFileName, rgbMapTrain);

												#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
												readImage(trainrgbMapSmallFacingPolyFileName, trainrgbMapSmall);
												#endif

												#ifdef OR_USE_DATABASE
												setCurrentDirectory(currentTempFolder);
												#endif
											}
											else
											{
												convertSQLdatabaseStringToSnapshotMaps(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTrain, trainrgbDevMap, traindepthMap, smallImageWidth, smallImageHeight, trainrgbMapSmall, traindepthMapSmall, trainrgbDevMapSmall, dimension, currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->snapshotMapsText, OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS);
											}

											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t end: 5a. normalised snapshot comparison - load snapshot data for comparison" << endl;
												}
												long time5aNormalisedSnapshotComparisonLoadComparisonDataEnd;
												time5aNormalisedSnapshotComparisonLoadComparisonDataEnd = getTimeAsLong();
												time5aNormalisedSnapshotComparisonLoadComparisonDataTotal = time5aNormalisedSnapshotComparisonLoadComparisonDataTotal + time5aNormalisedSnapshotComparisonLoadComparisonDataEnd-time5aNormalisedSnapshotComparisonLoadComparisonDataStart;
												time5aNormalisedSnapshotComparisonLoadComparisonDataIndex ++;
												if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t time5aNormalisedSnapshotComparisonLoadComparisonData = " << time5aNormalisedSnapshotComparisonLoadComparisonDataEnd-time5aNormalisedSnapshotComparisonLoadComparisonDataStart << endl;
												}
											}

											long time5bNormalisedSnapshotComparisonCompareSnapshotDataStart;
											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t start: 5b. normalised snapshot comparison - compare snapshot data" << endl;
												}
												time5bNormalisedSnapshotComparisonCompareSnapshotDataStart = getTimeAsLong();
											}


											/*
											if(OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS)
											{//optimised, rgb dev maps have already been created - use these to perform the comparsion

											}
											else
											{
											*/

											#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
											bool localMatchFound = false;
											#endif


											#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON

											int resampledWidth = (imageWidthFacingPoly/smallImageRatio);
											int resampledHeight = (imageHeightFacingPoly/smallImageRatio);

											#ifdef OR_DEBUG_COMPARISON
											cout << "testrgbMapSmall = " << endl;
											printRGBMap(resampledWidth, resampledHeight, testrgbMapSmall);
											cout << "trainrgbMapSmall = " << endl;
											printRGBMap(resampledWidth, resampledHeight, trainrgbMapSmall);
											#endif

											#ifdef OR_METHOD_SMALL_IMAGE_FIRST_COMPARISON_NO_KERNEL
												double errorSmall = compareImagesRGBsmallNoKernel(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);			//cannot use this function until it has been tested
											#else
												#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
													double errorSmall = compareImagesRGBwithPosDev(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);
												#else
													double errorSmall = compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);			//cannot use this function until it has been tested
												#endif
											#endif

											#ifdef OR_DEBUG_COMPARISON
											cout << "errorSmall = " << errorSmall << endl;
											cout << "OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = " << OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION << endl;
											#endif
											if(errorSmall < OR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD)
											{
											#endif

												#ifdef OR_DEBUG_COMPARISON
												cout << "\t\t\tpassed SMALL IMAGE" << endl;
												#endif

												#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
													double error = compareImagesRGBwithPosDev(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, rgbMapTrain);
												#else
													double error = compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, rgbMapTrain);			//cannot use this function until it has been tested
													//double error = compareImagesRGBwithPosDev(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, rgbMapTrain);
												#endif

												#ifdef OR_DEBUG_COMPARISON
												if(error < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
												{
													cout << "\t\t\t\t\tpassed LARGE IMAGE" << endl;
													cout << "PASSED im" << endl;
												}
												else
												{
													cout << "FAILED im" << endl;
												}
												#endif

												if(error < currentLowestError)
												{
													#ifdef OR_DEBUG_COMPARISON
													cout << "transformed RGB check passed" << endl;
													#endif

													#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
													bool passedContrastCheck = false;
													if(doNotCheckContrast)
													{
														passedContrastCheck = true;
													}
													else if(checkImageHasContrast(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTrain))
													{
														passedContrastCheck = true;
													}
													if(passedContrastCheck)
													{
													#endif
														#ifdef OR_DEBUG_COMPARISON
														cout << "contrast check passed" << endl;
														#endif

														currentLowestError = error;

														#ifdef OR_DEBUG_COMPARISON
														if(error < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
														{
															cout << "transformed RGB check passed" << endl;
														}
														#endif

														#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
														trainObjectNameWithLowestErrorRecordAcrossAllSides = trainObjectString;
														#ifndef OR_ASSERT_MATCHES_FOR_ALL_SIDES
														trainObjectNameWithLowestErrorRecord[testSideIndex] = trainObjectString;
														#endif
														#else
														trainPolyIndexWithLowestErrorRecord[testSideIndex][0] = trainObjectIndex;
														#endif

														trainPolyIndexWithLowestErrorRecord[testSideIndex][1] = trainViewIndex;
														trainPolyIndexWithLowestErrorRecord[testSideIndex][2] = trainZoomIndex;
														trainPolyIndexWithLowestErrorRecord[testSideIndex][3] = trainPolyIndex;
														trainPolyIndexWithLowestErrorRecord[testSideIndex][4] = trainSideIndex;
														//trainPolyIndexWithLowestErrorRecordStringSide[testSideIndex] = trainrgbMapFacingPolyFileNameWithoutExt;

														#ifdef DEBUG_OR_OUTPUT_DT_BIN
														DTbinWithLowestErrorRecord[testSideIndex] = DTbin;
														geoxBinWithLowestErrorRecord[testSideIndex][0] = geoxBin[0];
														geoxBinWithLowestErrorRecord[testSideIndex][1] = geoxBin[1];
														geoyBinWithLowestErrorRecord[testSideIndex][0] = geoyBin[0];
														geoyBinWithLowestErrorRecord[testSideIndex][1] = geoyBin[1];
														geoxybinWithLowestErrorRecord[testSideIndex] = geoxybin;
														#endif

														#ifndef OR_ASSERT_MATCHES_FOR_ALL_SIDES
														foundMatchRecord[testSideIndex] = true;
														currentLowestErrorRecord[testSideIndex] = currentLowestError;
														#endif

														#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
														localMatchFound = true;
														#endif
														#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
														cout << "localMatchFound" << endl;
														#endif

													#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
													}
													#endif
												}
												else
												{
													#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
													if(DTbinDebugMatchFound)
													{
														cout << "error: (2DTbinDebugMatchFound && !localMatchFound): DTbin = " << DTbin << endl;
													}
													#endif
												}

											#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
											}
											else
											{
												#ifdef DEBUG_OR_OUTPUT_DT_BIN_READ_FROM_FILE_AND_WORK_OUT_WHY_BIN_IS_NOT_BEING_DETECTED
												if(DTbinDebugMatchFound)
												{
													cout << "error: (1DTbinDebugMatchFound && !localMatchFound): DTbin = " << DTbin << endl;
												}
												#endif
											}
											#endif

											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t end: 5b. normalised snapshot comparison - compare snapshot data" << endl;
												}
												long time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd;
												time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd = getTimeAsLong();
												time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal = time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal + time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd-time5bNormalisedSnapshotComparisonCompareSnapshotDataStart;
												time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex++;
												if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t time5bNormalisedSnapshotComparisonCompareSnapshotData = " << time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd-time5bNormalisedSnapshotComparisonCompareSnapshotDataStart << endl;
												}
											}

										#endif
					#ifdef OR_METHOD_GEOMETRIC_COMPARISON_BINNING

											}

											currentFeatureContainerInTestFeatureMatchingTrainBin=currentFeatureContainerInTestFeatureMatchingTrainBin->next;
											#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
											currentFeatureContainerInTestFeatureMatchingTrainBin2=currentFeatureContainerInTestFeatureMatchingTrainBin2->next;
											#endif
										}

										#ifndef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
										delete firstFeatureContainerInTestFeatureMatchingTrainBin;
										#endif
										#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
											if(exceptionIndex != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)
											{
												testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2] = testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2]-exceptionModifier;
											}
											exceptionModifier = exceptionModifier* -1;
										}
										#endif
										#ifndef OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NO_EXPLICIT_FOR_LOOPS
													}
												}
											}
										}
										#endif
									}
								}
								testcurrentFeatureInNearestFeatureList2 = testcurrentFeatureInNearestFeatureList2->next;
								findex2++;
							}
							testcurrentFeatureInNearestFeatureList = testcurrentFeatureInNearestFeatureList->next;
							findex1++;
						}
						
						if(!OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL && !OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							delete currentTestFeature;
						}

					#else
						#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL

											}

										currentFeatureContainerInList = currentFeatureContainerInList->next;

										#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_ALLOW_LIGHTING_COLOUR_SATURATION_VARIATION
										}
										}
										#endif
									}
								}
							}
						}
						#else

										}
										else
										{
											//cout << "FAILED GeometricCheck test" << endl;
										}
									}
								}
							}
						}
					}
						#endif
					#endif

						#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
						if(!doNotCheckContrast)
						{
							if(!checkImageHasContrast(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest))
							{
								currentLowestError = OR_IMAGE_COMPARISON_NO_CONTRAST_ERROR_KEY;
							}
						}
						#endif

						#ifdef OR_DEBUG_COMPARISON
						if(currentLowestError == OR_IMAGE_COMPARISON_NO_CONTRAST_ERROR_KEY)
						{
							cout << "no best match was determined for test data with train data:" << endl;
						}
						else
						{
							cout << "the following best match was determined for test data with train data:" << endl;
							cout << "testObjectIndex = " << testObjectIndex << endl;

							cout << "testViewIndex = " << testViewIndex+testViewNumber << endl;
							cout << "testZoomIndex = " << testZoomIndex << endl;
							cout << "testPolyIndex = " << testPolyIndex << endl;
							cout << "testSideIndex = " << testSideIndex << endl;
							#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
								cout << "trainObjectNameWithLowestError = " << trainObjectNameWithLowestErrorRecordAcrossAllSides << endl;
							#else
								cout << "trainObjectIndexWithLowestError = " << trainPolyIndexWithLowestErrorRecord[0][0] << endl;
							#endif
							cout << "trainViewIndexWithHighestAccuracy = " <<  trainPolyIndexWithLowestErrorRecord[0][1] << endl;
							cout << "trainZoomIndexWithHighestAccuracy = " <<  trainPolyIndexWithLowestErrorRecord[0][2] << endl;
							cout << "trainPolyIndexWithHighestAccuracy = " << trainPolyIndexWithLowestErrorRecord[0][3] << endl;
							cout << "trainSideIndexWithHighestAccuracy = " << trainPolyIndexWithLowestErrorRecord[0][4] << endl;
							cout << "error of match = " << currentLowestError << endl;
						}
						#endif

						averageMatchError = averageMatchError + currentLowestError;
						averageMatchErrorAcrossSides = averageMatchErrorAcrossSides + currentLowestError;
					}

					averageMatchErrorAcrossSides = (averageMatchErrorAcrossSides/OR_METHOD_POLYGON_NUMBER_OF_SIDES);
					#ifdef OR_DEBUG_COMPARISON
					cout << "averageMatchErrorAcrossSides = " << averageMatchErrorAcrossSides << endl;
					#endif

				#ifndef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					#ifdef OR_ASSERT_MATCHES_FOR_ALL_SIDES
					if(averageMatchErrorAcrossSides < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
					{
						string averageMatchErrorAcrossSidesString = convertDoubleToString(averageMatchErrorAcrossSides, "%0.3f");

						#ifdef OR_DEBUG_COMPARISON
						cout << "transformed RGB check passed 2: (averageMatchErrorAcrossSides < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)" << endl;
						#endif

						if(((trainPolyIndexWithLowestErrorRecord[0][2] == trainPolyIndexWithLowestErrorRecord[1][2]) && (trainPolyIndexWithLowestErrorRecord[0][2] == trainPolyIndexWithLowestErrorRecord[2][2])) && ((trainPolyIndexWithLowestErrorRecord[0][3] == trainPolyIndexWithLowestErrorRecord[1][3]) && (trainPolyIndexWithLowestErrorRecord[0][3] == trainPolyIndexWithLowestErrorRecord[2][3])))
						{//all sides are referring to the same poly, and all sides are referring to the same zoom

					#endif
							#ifdef OR_DEBUG_COMPARISON_ALLOW_SAME_OBJECT_AND_SAME_VIEW_TO_BE_COMPARED
							if(1)
							#elif defined OR_DEBUG_COMPARISON_ALLOW_SAME_OBJECT_BUT_DIFFERENT_VIEWS_TO_BE_COMPARED
							if(!((trainObjectNameWithLowestErrorRecordAcrossAllSides == testObjectNameArray[testObjectIndex]) && (trainPolyIndexWithLowestErrorRecord[0][1] == testViewIndex+testViewNumber)))
							#else
							if(trainObjectNameWithLowestErrorRecordAcrossAllSides != testObjectNameArray[testObjectIndex])
							#endif
							{//object name and object view are not both the same

								//a real image comparitor match has been declared found

								#ifdef OR_DEBUG_COMPARISON
								cout << "transformed RGB check passed 3: (a real image comparitor match has been declared found)" << endl;
								#endif

								numPassesForTestViewIndex++;

								if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
								{
									cout << "the following best match was determined for test data with train data:" << endl;
									cout << "testObjectIndex = " << testObjectIndex << endl;
									cout << "testViewIndex = " << testViewIndex+testViewNumber << endl;
									cout << "testZoomIndex = " << testZoomIndex << endl;
									cout << "testPolyIndex = " << testPolyIndex << endl;
									cout << "testSideIndex = 0, 1, 2" << endl;
									#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
										cout << "trainObjectNameWithLowestError = " << trainObjectNameWithLowestErrorRecordAcrossAllSides << endl;
									#else
										cout << "trainObjectIndexWithLowestError = " << trainPolyIndexWithLowestErrorRecord[0][0] << endl;
									#endif
									cout << "trainViewIndexWithHighestAccuracy = " <<  trainPolyIndexWithLowestErrorRecord[0][1] << endl;
									cout << "trainZoomIndexWithHighestAccuracy = " <<  trainPolyIndexWithLowestErrorRecord[0][2] << endl;
									cout << "trainPolyIndexWithHighestAccuracy = " << trainPolyIndexWithLowestErrorRecord[0][3] << endl;
									cout << "trainSideIndexWithHighestAccuracy = " << trainPolyIndexWithLowestErrorRecord[0][4] << endl;
									cout << "error of match = " << averageMatchErrorAcrossSides << endl;
								}

								#ifdef OR_ASSERT_MATCHES_FOR_ALL_SIDES
								//see if object exists, and if it does, increment number of successful snapshots
								ORobjectReferenceList* currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
								bool foundObjectNameInObjectReferenceList = false;
								while(currentReferenceInSnapshotMatchObjectReferenceList->next != NULL)
								{
									if(currentReferenceInSnapshotMatchObjectReferenceList->objectName == trainObjectNameWithLowestErrorRecordAcrossAllSides)
									{
										currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots + 1;
										foundObjectNameInObjectReferenceList = true;
									}

									currentReferenceInSnapshotMatchObjectReferenceList = currentReferenceInSnapshotMatchObjectReferenceList->next;
								}
								if(!foundObjectNameInObjectReferenceList)
								{
									#ifdef OR_DEBUG_COMPARISON
									cout << "!foundObjectNameInObjectReferenceList:" << trainObjectNameWithLowestErrorRecordAcrossAllSides << endl;
									#endif
									currentReferenceInSnapshotMatchObjectReferenceList->objectName = trainObjectNameWithLowestErrorRecordAcrossAllSides;
									currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = 1;
									ORobjectReferenceList* newObjectReference = new ORobjectReferenceList();
									currentReferenceInSnapshotMatchObjectReferenceList->next = newObjectReference;
								}
								#endif

								for(int s=0; s<OR_METHOD_POLYGON_NUMBER_OF_SIDES; s++)
								{
									#ifndef OR_ASSERT_MATCHES_FOR_ALL_SIDES
									if(foundMatchRecord[s])
									{
									if(currentLowestErrorRecord[s] < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
									{
									#endif

									#ifndef OR_ASSERT_MATCHES_FOR_ALL_SIDES
									//see if object exists, and if it does, increment number of successful snapshots
									ORobjectReferenceList* currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
									bool foundObjectNameInObjectReferenceList = false;
									while(currentReferenceInSnapshotMatchObjectReferenceList->next != NULL)
									{
										if(currentReferenceInSnapshotMatchObjectReferenceList->objectName == trainObjectNameWithLowestErrorRecord[s])
										{
											currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots + 1;
											foundObjectNameInObjectReferenceList = true;
										}

										currentReferenceInSnapshotMatchObjectReferenceList = currentReferenceInSnapshotMatchObjectReferenceList->next;
									}
									if(!foundObjectNameInObjectReferenceList)
									{
										#ifdef OR_DEBUG_COMPARISON
										cout << "!foundObjectNameInObjectReferenceList:" << trainObjectNameWithLowestErrorRecord[s] << endl;
										#endif
										currentReferenceInSnapshotMatchObjectReferenceList->objectName = trainObjectNameWithLowestErrorRecord[s];
										currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = 1;
										ORobjectReferenceList* newObjectReference = new ORobjectReferenceList();
										currentReferenceInSnapshotMatchObjectReferenceList->next = newObjectReference;
									}
									#endif

									if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
									{
										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											string trainobjectIndexString = trainObjectNameWithLowestErrorRecordAcrossAllSides;
										#else
											string trainobjectIndexString = convertIntToString(trainPolyIndexWithLowestErrorRecord[s][0]);
										#endif

										string trainviewIndexString = convertIntToString(trainPolyIndexWithLowestErrorRecord[s][1]);
										string trainzoomIndexString = convertIntToString(trainPolyIndexWithLowestErrorRecord[s][2]);
										string trainpolygonIndexString = convertIntToString(trainPolyIndexWithLowestErrorRecord[s][3]);
										string trainsideIndexString = convertIntToString(trainPolyIndexWithLowestErrorRecord[s][4]);

										string testviewIndexString = convertIntToString(testViewIndex+testViewNumber);
										string testzoomIndexString = convertIntToString(testZoomIndex);
										string testpolygonIndexString = convertIntToString(testPolyIndex);
										string testsideIndexString = convertIntToString(s);

										string blank = "";

										string traininterpolatedRGBMapFileNameForRayTracing;
										string testinterpolatedRGBMapFileNameForRayTracing;

									#ifdef TEMPTEST3GEOACCURACY	//doesn't work with OR_USE_DATABASE
										cout << "TEMPTEST3GEOACCURACY Part 3" << endl;

										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES

											traininterpolatedRGBMapFileNameForRayTracing = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#else

											traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#endif
										string traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
										createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainFirstFeatureInNearestFeatureList, false, false, false);

										testinterpolatedRGBMapFileNameForRayTracing = objectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
										string testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
										createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);

										cout << "\n new match found;" << endl;
										cout << "traininterpolatedNearestFeaturesMapFileName = " << traininterpolatedNearestFeaturesMapFileName << endl;
										ORfeature* currentFeatureInList = trainFirstFeatureInNearestFeatureList;
										while(currentFeatureInList->next != NULL)
										{
											cout << currentFeatureInList->OTpointIndex << "\t" << currentFeatureInList->pointTransformed.x <<  "\t" << currentFeatureInList->pointTransformed.y <<  "\t" << currentFeatureInList->pointTransformed.z << "\t" << currentFeatureInList->point.x <<  "\t" << currentFeatureInList->point.y <<  "\t" << currentFeatureInList->point.z <<endl;
											currentFeatureInList = currentFeatureInList->next;
										}
										cout << "testinterpolatedNearestFeaturesMapFileName = " << testinterpolatedNearestFeaturesMapFileName << endl;
										currentFeatureInList = testFirstFeatureInNearestFeatureList;
										while(currentFeatureInList->next != NULL)
										{
											cout << currentFeatureInList->OTpointIndex << "\t" << currentFeatureInList->pointTransformed.x <<  "\t" << currentFeatureInList->pointTransformed.y <<  "\t" << currentFeatureInList->pointTransformed.z << "\t" << currentFeatureInList->point.x <<  "\t" << currentFeatureInList->point.y <<  "\t" << currentFeatureInList->point.z <<endl;
											currentFeatureInList = currentFeatureInList->next;
										}
										bool exactMatchFoundIrrelevant;
										bool passedGeometricCheck = compareFeaturesListForMatch(testFirstFeatureInNearestFeatureList, trainFirstFeatureInNearestFeatureList, dimension, &exactMatchFoundIrrelevant);
										cout << "passedGeometricCheck = " << passedGeometricCheck << endl;
									#endif

										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											traininterpolatedRGBMapFileNameForRayTracing = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#else
											traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#endif
										string trainrgbMapFacingPolyFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;

										string traininterpolatedRGBMapFileNameForRayTracingSanitised;
										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											traininterpolatedRGBMapFileNameForRayTracingSanitised = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "X" + trainpolygonIndexString + "Y" + trainsideIndexString;
										#else
											traininterpolatedRGBMapFileNameForRayTracingSanitised = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "X" + trainpolygonIndexString + "Y" + trainsideIndexString;
										#endif
										string trainrgbMapFacingPolyFileNameWithoutExtSanitised = traininterpolatedRGBMapFileNameForRayTracingSanitised + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;


										testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
										string testrgbMapFacingPolyFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

										string testinterpolatedRGBMapFileNameForRayTracingSanitised;
										testinterpolatedRGBMapFileNameForRayTracingSanitised = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "X" + testpolygonIndexString + "Y" + testsideIndexString;
										string testrgbMapFacingPolyFileNameWithoutExtSanitised = testinterpolatedRGBMapFileNameForRayTracingSanitised + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

										bool generateTrainPixmapFiles = false;
										bool generateTestPixmapFiles = false;
										if(!OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
										{
											generateTrainPixmapFiles = true;
											generateTestPixmapFiles = true;
										}
										else
										{
											if(!OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
											{
												generateTestPixmapFiles = true;
											}
										}

										#ifdef OR_DEBUG_COMPARISON
										/*
										if(generateTrainPixmapFiles)
										{
											string trainrgbMapFacingPolyFileNameTemp = trainrgbMapFacingPolyFileNameWithoutExt + PPM_EXTENSION;
											generatePixmapFromRGBmap(trainrgbMapFacingPolyFileNameTemp, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
										}
										if(generateTestPixmapFiles)
										{
											string testrgbMapFacingPolyFileNameTemp = testrgbMapFacingPolyFileNameWithoutExt + PPM_EXTENSION;
											generatePixmapFromRGBmap(testrgbMapFacingPolyFileNameTemp, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
										}
										*/
										#endif

										#ifdef DEBUG_OR_OUTPUT_DT_BIN
										string DTbin = DTbinWithLowestErrorRecord[s];
										#endif

										string trainImgSrcHtmlTags = "";
										string testImgSrcHtmlTags = "";
										string imageWidthFacingPolyString = convertIntToString(imageWidthFacingPoly);	//increase image size for small images - for better visualisation [rather than zoom in browser]
										string imageHeightFacingPolyString = convertIntToString(imageHeightFacingPoly);	//increase image size for small images - for better visualisation [rather than zoom in browser]
										if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
										{
											convertImageFileType(&trainrgbMapFacingPolyFileNameWithoutExt, &trainrgbMapFacingPolyFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											convertImageFileType(&testrgbMapFacingPolyFileNameWithoutExt, &testrgbMapFacingPolyFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + trainrgbMapFacingPolyFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + testrgbMapFacingPolyFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";

											#ifdef DEBUG_OR_OUTPUT_SMALL_IMAGES
											string trainrgbMapSmallFacingPolyFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;
											string testrgbMapSmallFacingPolyFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;
											convertImageFileType(&trainrgbMapSmallFacingPolyFileNameWithoutExt, &trainrgbMapSmallFacingPolyFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											convertImageFileType(&testrgbMapSmallFacingPolyFileNameWithoutExt, &testrgbMapSmallFacingPolyFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + trainrgbMapSmallFacingPolyFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + testrgbMapSmallFacingPolyFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											#endif

											#ifdef DEBUG_OR_OUTPUT_DECISION_TREE_IMAGES
											string trainbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_BOOLEAN_CONTRAST_MAP_PPM_EXTENSION_PART + TRAIN_STRING;
											string testbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_BOOLEAN_CONTRAST_MAP_PPM_EXTENSION_PART + TEST_STRING;
											convertImageFileType(&trainbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, &trainbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											convertImageFileType(&testbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, &testbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + trainbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + testbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											#ifdef DEBUG_OR_OUTPUT_DECISION_TREE_IMAGES_DIFF
											string diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_BOOLEAN_CONTRAST_MAP_PPM_EXTENSION_PART;
											string diffbooleanContrastRgbMapFacingPolySmallFileName = diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PPM_EXTENSION;
											string trainbooleanContrastRgbMapFacingPolySmallFileName = trainbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PPM_EXTENSION;
											string testbooleanContrastRgbMapFacingPolySmallFileName = testbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PPM_EXTENSION;
											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_1_ALL_RGB_COMPONENTS_WITH_DIRECTION
											int booleanContrastrgbMapSmallWidth = (2*imageWidthFacingPoly)/smallImageRatio;
											int booleanContrastrgbMapSmallHeight = (2*imageHeightFacingPoly)/smallImageRatio;
											#else
											int booleanContrastrgbMapSmallWidth = imageWidthFacingPoly/smallImageRatio;
											int booleanContrastrgbMapSmallHeight = imageHeightFacingPoly/smallImageRatio;
											#endif
											unsigned char* trainbooleanContrastrgbMapSmall = new unsigned char[booleanContrastrgbMapSmallWidth*booleanContrastrgbMapSmallHeight*RGB_NUM];
											unsigned char* testbooleanContrastrgbMapSmall = new unsigned char[booleanContrastrgbMapSmallWidth*booleanContrastrgbMapSmallHeight*RGB_NUM];
											readImage(&trainbooleanContrastRgbMapFacingPolySmallFileName, trainbooleanContrastrgbMapSmall);
											readImage(&testbooleanContrastRgbMapFacingPolySmallFileName, testbooleanContrastrgbMapSmall);
											generateBooleanDiffMapFromRGBMaps(diffbooleanContrastRgbMapFacingPolySmallFileName, booleanContrastrgbMapSmallWidth, booleanContrastrgbMapSmallHeight, trainbooleanContrastrgbMapSmall, testbooleanContrastrgbMapSmall);
											/*
											bool* diffbooleanContrastrgbMapSmall = new bool[booleanContrastrgbMapSmallWidth*booleanContrastrgbMapSmallHeight];
											createBooleanDiffMapFromRGBMaps(diffbooleanContrastrgbMapSmall, booleanContrastrgbMapSmallWidth, booleanContrastrgbMapSmallHeight, trainbooleanContrastrgbMapSmall, testbooleanContrastrgbMapSmall);
											generatePixmapFromBooleanMap(diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, booleanContrastrgbMapSmallWidth, booleanContrastrgbMapSmallHeight, bool* booleanMap);
											*/
											convertImageFileType(&diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, &diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + diffbooleanContrastRgbMapFacingPolySmallFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											#endif
											#endif

											#ifdef DEBUG_OR_OUTPUT_DCT_TABLES
											string testrgbMapSmallFacingPolyFileNameJPEGTemp = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + ".temp" + JPG_EXTENSION;
											string trainrgbMapSmallFacingPolyFileNameJPEGTemp = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + ".temp" + JPG_EXTENSION;
											string testrgbMapSmallFacingPolyFileNamePPMTemp = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;
											string trainrgbMapSmallFacingPolyFileNamePPMTemp = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;

											string testDCTTableHTMLOutputString = "<P>DCT coefficients:<BR />";
											string trainDCTTableHTMLOutputString = "<P>DCT coefficients:<BR />";
											string testDCTBinnedPrelimTableHTMLOutputString = "<TABLE><TR>";
											string trainDCTBinnedPrelimTableHTMLOutputString = "<TABLE><TR>";
											string testDCTBinnedTableHTMLOutputString = "<TABLE><TR>";
											string trainDCTBinnedTableHTMLOutputString = "<TABLE><TR>";
											signed char testconcatonatedSignedDctCoeffArrayRequirementTemp[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
											signed char trainconcatonatedSignedDctCoeffArrayRequirementTemp[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
											setPointerToDCTtableHTMLoutputString(&testDCTTableHTMLOutputString);
											readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(&testrgbMapSmallFacingPolyFileNamePPMTemp, &testrgbMapSmallFacingPolyFileNameJPEGTemp, testconcatonatedSignedDctCoeffArrayRequirementTemp, true);
											setPointerToDCTtableHTMLoutputString(&trainDCTTableHTMLOutputString);
											readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(&trainrgbMapSmallFacingPolyFileNamePPMTemp, &trainrgbMapSmallFacingPolyFileNameJPEGTemp, trainconcatonatedSignedDctCoeffArrayRequirementTemp, true);
											//next code extracted from convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue (determinations of arrayValueUnsigned)
											for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
											{
												int arrayValueSigned = testconcatonatedSignedDctCoeffArrayRequirementTemp[i];
												testDCTBinnedPrelimTableHTMLOutputString = testDCTBinnedPrelimTableHTMLOutputString + "<TD>" + convertIntToString(arrayValueSigned) + "</TD>";

												double arrayValueUnsignedDouble;
												int arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);
												testDCTBinnedTableHTMLOutputString = testDCTBinnedTableHTMLOutputString + "<TD>" + convertUnsignedIntToString(arrayValueUnsigned) + "</TD>";
											}
											for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
											{
												int arrayValueSigned = trainconcatonatedSignedDctCoeffArrayRequirementTemp[i];
												trainDCTBinnedPrelimTableHTMLOutputString = trainDCTBinnedPrelimTableHTMLOutputString + "<TD>" + convertIntToString(arrayValueSigned) + "</TD>";

												double arrayValueUnsignedDouble;
												int arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);
												trainDCTBinnedTableHTMLOutputString = trainDCTBinnedTableHTMLOutputString + "<TD>" + convertUnsignedIntToString(arrayValueUnsigned) + "</TD>";
											}
											testDCTBinnedPrelimTableHTMLOutputString = testDCTBinnedPrelimTableHTMLOutputString + "</TR></TABLE>";
											trainDCTBinnedPrelimTableHTMLOutputString = trainDCTBinnedPrelimTableHTMLOutputString + "</TR></TABLE>";
											testDCTBinnedTableHTMLOutputString = testDCTBinnedTableHTMLOutputString + "</TR></TABLE>";
											trainDCTBinnedTableHTMLOutputString = trainDCTBinnedTableHTMLOutputString + "</TR></TABLE>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + testDCTTableHTMLOutputString;
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + trainDCTTableHTMLOutputString;
											testImgSrcHtmlTags = testImgSrcHtmlTags + testDCTBinnedPrelimTableHTMLOutputString;
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + trainDCTBinnedPrelimTableHTMLOutputString;
											testImgSrcHtmlTags = testImgSrcHtmlTags + testDCTBinnedTableHTMLOutputString + "</P>";
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + trainDCTBinnedTableHTMLOutputString + "</P>";
											#endif


											#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
											string traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
											createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainFirstFeatureInNearestFeatureList, false, false, false);
											string testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
											createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);
											bool exactMatchFound;
											bool passedGeometricCheck = compareFeaturesListForMatch(testFirstFeatureInNearestFeatureList, trainFirstFeatureInNearestFeatureList, dimension, &exactMatchFound);	//required to set matchFound
											string trainGeoTableHTMLOutputString = "";
											string testGeoTableHTMLOutputString = "";
											string trainGeoBinnedTableHTMLOutputString = "";
											string testGeoBinnedTableHTMLOutputString = "";
											createGeoTableHTMLfromFeatureList(trainFirstFeatureInNearestFeatureList, false, &trainGeoTableHTMLOutputString);
											createGeoTableHTMLfromFeatureList(testFirstFeatureInNearestFeatureList, false, &testGeoTableHTMLOutputString);
											createGeoTableHTMLfromFeatureList(trainFirstFeatureInNearestFeatureList, true, &trainGeoBinnedTableHTMLOutputString);
											createGeoTableHTMLfromFeatureList(testFirstFeatureInNearestFeatureList, true, &testGeoBinnedTableHTMLOutputString);
											//cout << "endDEBUG_OR_OUTPUT_GEO_COORDINATES" << endl;
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<P>Nearest Features (transformed):<BR />" + trainGeoTableHTMLOutputString;
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<P>Nearest Features (transformed):<BR />" + testGeoTableHTMLOutputString;
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + trainGeoBinnedTableHTMLOutputString + "</P>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + testGeoBinnedTableHTMLOutputString + "</P>";
											#endif

											#ifdef DEBUG_OR_OUTPUT_DT_BIN
											int testgeoxBinTemp[2];
											int testgeoyBinTemp[2];
											long testgeoxybinTemp;
											testgeoxBinTemp[0] = geoxBinWithLowestErrorRecord[s][0];
											testgeoxBinTemp[1] = geoxBinWithLowestErrorRecord[s][1];
											testgeoyBinTemp[0] = geoyBinWithLowestErrorRecord[s][0];
											testgeoyBinTemp[1] = geoyBinWithLowestErrorRecord[s][1];
											testgeoxybinTemp = geoxybinWithLowestErrorRecord[s];
											/*
											bool geoBinningIsSameAsOriginal = passedGeometricCheck; 	//no this does not identify exact matches only
											*/
											/*
											bool geoBinningIsSameAsOriginal	= determineIfGeoBinningIdenticalMatchFound(trainFirstFeatureInNearestFeatureList, testgeoxBinTemp, testgeoyBinTemp);
												//NB this calculation gives apparent false positives: it will throw false if 2 matches is found with 2 nearest feature sets, and if these 2 matches do not correspond to the features binned in the test set decision tree [yet this is OK, as these features should be detected by another DTbinning)
												//this is because when intelligent binning is enabled, there may be more than one matched trainset for every test set. [because there are multiple permutations of feature matches], yet only one of these will be recorded.
											*/
											bool geoBinningIsSameAsOriginal	= exactMatchFound;		//in general, at least one set of 2 matches is found (even if the DTbinned features from the test set do not have an exact match in the train set)

											string origTrainDTBin;
											if(geoBinningIsSameAsOriginal)
											{
												colour trainnormalisedAverageHueDeviationRequirementTemp;	//not currently set

												//extract from getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck
												string currentDirectory = "";
												char currentDirectory = "";
												int currentDirectoryLength;
												createSnapshotIDreferenceImageComparisonDecisionTreeString(smallImageWidth, smallImageHeight, trainrgbMapSmall, testgeoxybinTemp, testgeoxBinTemp, testgeoyBinTemp, &trainnormalisedAverageHueDeviationRequirementTemp, trainconcatonatedSignedDctCoeffArrayRequirementTemp, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);
												currentDirectoryCharStar[currentDirectoryLength] = '\0';
												origTrainDTBin = currentDirectoryCharStar;
											}
											else
											{
												origTrainDTBin = "unknowable (no exact geo matches)";	//(core geo bins differ from test)
											}
											bool DTbinEqualsOrigTrainDTbin;
											string DTbinEqualsOrigTrainDTbinString = "";
											if(origTrainDTBin == DTbin)
											{
												DTbinEqualsOrigTrainDTbin = true;
												DTbinEqualsOrigTrainDTbinString = "<font color='green'>[same]</font>";
											}
											else
											{
												DTbinEqualsOrigTrainDTbin = false;
												if(geoBinningIsSameAsOriginal)
												{
													DTbinEqualsOrigTrainDTbinString = "<font color='red'>[diff]</font>";
												}
												else
												{
													DTbinEqualsOrigTrainDTbinString = "<font color='orange'>[diff]</font>";
												}
											}
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<P>DTBin = " + DTbin + "<BR />origTrainDTBin = " + origTrainDTBin + " " + DTbinEqualsOrigTrainDTbinString + "</P>";	//(rederivation of original train bin [ie, without intelligent pseudo randomised binning])
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<P>DTBin = " + DTbin + "</P>";

											#ifdef DEBUG_OR_OUTPUT_DT_BIN_WRITE_TO_FILE
											#ifdef DEBUG_OR_OUTPUT_DT_BIN_WRITE_TO_FILE_ONLY_WRITE_MATCHES_NOT_REQUIRING_INTELLIGENT_BINNING
											if(DTbinEqualsOrigTrainDTbin)
											{
											#endif
												writeStringToFileObject(DTbin + "\n", &writeFileObjectDTbinDebug);
											#ifdef DEBUG_OR_OUTPUT_DT_BIN_WRITE_TO_FILE_ONLY_WRITE_MATCHES_NOT_REQUIRING_INTELLIGENT_BINNING
											}
											#endif
											#endif
											#endif
										}
										else
										{
											convertImageFileType(&trainrgbMapFacingPolyFileNameWithoutExt, &trainrgbMapFacingPolyFileNameWithoutExtSanitised, PPM_EXTENSION, PNG_EXTENSION);
											convertImageFileType(&testrgbMapFacingPolyFileNameWithoutExt, &testrgbMapFacingPolyFileNameWithoutExtSanitised, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + trainrgbMapFacingPolyFileNameWithoutExtSanitised + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + testrgbMapFacingPolyFileNameWithoutExtSanitised + PNG_EXTENSION + "\" border=0>";
										}

										#define DEBUG_OR_IMAGE_COMPARISON_LIST_MATCHED_FILE_NAMES
										#ifdef DEBUG_OR_IMAGE_COMPARISON_LIST_MATCHED_FILE_NAMES
										string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testrgbMapFacingPolyFileNameWithoutExt + PPM_EXTENSION + "</TD><TD>" + testImgSrcHtmlTags + "</TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainrgbMapFacingPolyFileNameWithoutExt + PPM_EXTENSION + "</TD><TD>" + trainImgSrcHtmlTags + "</TD>";
										#else
										string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testsideIndexString + "</TD><TD>" + testImgSrcHtmlTags + "</TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainsideIndexString + "</TD><TD>" + trainImgSrcHtmlTags + "</TD>";
										#endif

										#ifdef OR_ASSERT_MATCHES_FOR_ALL_SIDES
										ICRmatchRow = ICRmatchRow + "<TD>" + averageMatchErrorAcrossSidesString + "</TD></TR>";
										#else
										string currentLowestErrorString = convertDoubleToString(currentLowestErrorRecord[s], "%0.3f");
										ICRmatchRow = ICRmatchRow + "<TD>" + currentLowestErrorString + "</TD></TR>";
										#endif

										writeStringToFileObject(ICRmatchRow, &writeFileObject);
									}
									#ifndef OR_ASSERT_MATCHES_FOR_ALL_SIDES
									}
									}
									#endif
								}
							}
						#ifdef OR_ASSERT_MATCHES_FOR_ALL_SIDES
						}

					}
					#endif
				#endif

				}
				averageMatchError = averageMatchError/(numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]* OR_METHOD_POLYGON_NUMBER_OF_SIDES);
				if(OR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "average error = " << averageMatchError << endl;
				}

				averageMatchErrorAcrossAllViewIndicies = averageMatchErrorAcrossAllViewIndicies + averageMatchError;

				if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
				{
					#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					cout << "\t\t\t end: 5z. normalised snapshot comparison - single view speed test (for website)" << endl;
					#endif
					long time5zNormalisedSnapshotComparisonSingleViewTestEnd;
					time5zNormalisedSnapshotComparisonSingleViewTestEnd = getTimeAsLong();
					long time5zNormalisedSnapshotComparisonSingleViewTestTotal = time5zNormalisedSnapshotComparisonSingleViewTestEnd-time5zNormalisedSnapshotComparisonSingleViewTestStart;
					//time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex++;
					#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					cout << "\t\t\t time5zNormalisedSnapshotComparisonSingleViewTest = " << (double)(time5zNormalisedSnapshotComparisonSingleViewTestEnd-time5zNormalisedSnapshotComparisonSingleViewTestStart)/CLOCKS_PER_SEC << endl;
					#endif
				}

				if(OR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "testObject = " << testObjectNameArray[testObjectIndex] << endl;
					cout << "numPassesForTestViewIndex " << testViewIndex+testViewNumber << " = " << numPassesForTestViewIndex << endl;
				}
			}
		}

		averageMatchErrorAcrossAllViewIndicies = averageMatchErrorAcrossAllViewIndicies/numberOfTestViewIndicies;
		if(OR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "average error across all view indicies = " << averageMatchErrorAcrossAllViewIndicies << endl;
		}

		averageMatchErrorAcrossAllObjects = averageMatchErrorAcrossAllObjects + averageMatchErrorAcrossAllViewIndicies;
	}

	averageMatchErrorAcrossAllObjects = averageMatchErrorAcrossAllObjects/numberOfTestObjects;

	if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{
		string ICRtableEnd = "</TABLE>";
		writeStringToFileObject(ICRtableEnd, &writeFileObject);

		string ICRfooter  = "</BODY></HTML>";
	}
	writeFileObject.close();
	#ifdef DEBUG_OR_OUTPUT_DT_BIN_WRITE_TO_FILE
	writeFileObjectDTbinDebug.close();
	#endif

	delete rgbMapTrain;
	delete rgbMapTest;
	//#ifdef OR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	delete traindepthMap;
	delete testdepthMap;
	//#endif
	#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
	delete trainrgbMapSmall;
	delete testrgbMapSmall;
	//#ifdef OR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	delete traindepthMapSmall;
	delete testdepthMapSmall;
	//#endif
	#endif

	/*
	//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	delete testdctCoeffArrayY;
	delete testdctCoeffArrayYCr;
	delete testdctCoeffArrayYCb;
	delete testconcatonatedSignedDctCoeffArrayRequirement;
	//#endif
	*/

	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	delete trainrgbDevMap;
	delete testrgbDevMap;
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	delete trainrgbDevMapSmall;
	delete testrgbDevMapSmall;
	delete trainrgbDev8BitSmallMap;
	delete testrgbDev8BitSmallMap;
	//#endif

	ORobjectReferenceList* currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
	string highestNumMatchingSnapshotsObjectName = "";
	int highestNumMatchingSnapshotsNumObjects = 0;
	string secondHighestNumMatchingSnapshotsObjectName = "";
	int secondHighestNumMatchingSnapshotsNumObjects = 0;
	while(currentReferenceInSnapshotMatchObjectReferenceList->next != NULL)
	{
		if(currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots > highestNumMatchingSnapshotsNumObjects)
		{
			secondHighestNumMatchingSnapshotsObjectName = highestNumMatchingSnapshotsObjectName;
			secondHighestNumMatchingSnapshotsNumObjects = highestNumMatchingSnapshotsNumObjects;
			highestNumMatchingSnapshotsObjectName = currentReferenceInSnapshotMatchObjectReferenceList->objectName;
			highestNumMatchingSnapshotsNumObjects = currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots;
		}
		currentReferenceInSnapshotMatchObjectReferenceList = currentReferenceInSnapshotMatchObjectReferenceList->next;
	}

	delete firstReferenceInSnapshotMatchObjectReferenceList;

	cout << "******************************** OR Results* ******************************* " << endl;
	cout << "Best Match Object From Train List - Name = " << highestNumMatchingSnapshotsObjectName << endl;
	cout << "Best Match Object From Train List - Redundancy = " << highestNumMatchingSnapshotsNumObjects << endl;
	cout << "Second Best Match Object From Train List - Name = " << secondHighestNumMatchingSnapshotsObjectName << endl;
	cout << "Second Best Match Object From Train List - Redundancy = " << secondHighestNumMatchingSnapshotsNumObjects << endl;
	cout << "**************************************************************************** " << endl;

	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "\n\npasses = \n\n" << passes << endl;
	}

	return averageMatchErrorAcrossAllObjects;
}



void convertImageFileType(string* imageBaseFileName, string* imageBaseFileNameConverted, string imageExtension, string imageExtensionConverted)
{
	string convertPPMtoPNGCommand = "convert " +* imageBaseFileName + imageExtension + " " +* imageBaseFileNameConverted + imageExtensionConverted;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
	{
		cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
	}
	system(convertPPMtoPNGCommand.c_str());
}





#define VERY_HIGH_ERROR (99999999999999.0)









//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
void convertNormalisedHueDeviationMapTo3x8bitMap(int imageWidth, int imageHeight, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, unsigned char* rgbDev8BitSmallMapFacingPoly)
{
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			vec normalisedHueContrast;
			getVectorMapValue(x, y, imageWidth, rgbDevIEnormalisedHueContrastMapSmallFacingPoly, &normalisedHueContrast);

			colour culledNormalisedHueContrast;
			cullAndBinNormalisedHueContrast(&normalisedHueContrast, &culledNormalisedHueContrast);

			setRGBMapValues(x, y, imageWidth, &culledNormalisedHueContrast, rgbDev8BitSmallMapFacingPoly);
		}
	}
}



void cullAndBinNormalisedHueContrast(vec* normalisedHueContrast, colour* culledNormalisedHueContrast)
{
	if(normalisedHueContrast->x*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		culledNormalisedHueContrast->r = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		culledNormalisedHueContrast->r = normalisedHueContrast->x*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
	if(normalisedHueContrast->y*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		culledNormalisedHueContrast->g = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		culledNormalisedHueContrast->g = normalisedHueContrast->y*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
	if(normalisedHueContrast->z*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		culledNormalisedHueContrast->b = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		culledNormalisedHueContrast->b = (normalisedHueContrast->z)*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
}
//#endif







//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING

void convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(signed char dctCoeffArrayY[], signed char dctCoeffArrayYcr[], signed char dctCoeffArrayYcb[], signed char concatonatedSignedDctCoeffArray[])
{
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y; i++)
	{
		int xIndex = dctCoeffSelectionArrayY[i][0];
		int yIndex = dctCoeffSelectionArrayY[i][1];

		concatonatedSignedDctCoeffArray[i] = dctCoeffArrayY[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
		#ifdef OR_DEBUG_COMPARISON
		cout << "x = " << int( dctCoeffArrayY[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex]) << endl;
		#endif
	}
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr; i++)
	{
		int xIndex = dctCoeffSelectionArrayYCr[i][0];
		int yIndex = dctCoeffSelectionArrayYCr[i][1];
		concatonatedSignedDctCoeffArray[i+OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y] = dctCoeffArrayYcr[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
		#ifdef OR_DEBUG_COMPARISON
		cout << "y = " << int( dctCoeffArrayYcr[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex]) << endl;
		#endif
	}
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb; i++)
	{
		int xIndex = dctCoeffSelectionArrayYCb[i][0];
		int yIndex = dctCoeffSelectionArrayYCb[i][1];

		concatonatedSignedDctCoeffArray[i+OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y+OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr] = dctCoeffArrayYcb[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
		#ifdef OR_DEBUG_COMPARISON
		cout << "z = " << int( dctCoeffArrayYcb[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex]) << endl;
		#endif
	}

	#ifdef OR_DEBUG_COMPARISON
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
	      cout << "concatonatedSignedDctCoeffArray[i] = " << int(concatonatedSignedDctCoeffArray[i]) << endl;
	}
	#endif
}

#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
static char* stringFormatDouble = "%0.6f";
static char* stringFormatDec = "%d";

void createGeoTableHTMLfromFeatureList(ORfeature* firstFeatureInNearestFeatureList, bool applyBinning, string* geoTableHTMLoutputString)
{
	*geoTableHTMLoutputString = *geoTableHTMLoutputString + "<TABLE>";

	ORfeature* currentFeatureInList = firstFeatureInNearestFeatureList;
	while((currentFeatureInList->next != NULL) && !(currentFeatureInList->lastFilledFeatureInList))
	{
		#ifdef OR_DEBUG_COMPARISON
		cout << currentFeatureInList->OTpointIndex << "\t" << currentFeatureInList->pointTransformed.x <<  "\t" << currentFeatureInList->pointTransformed.y <<  "\t" << currentFeatureInList->pointTransformed.z << "\t" << currentFeatureInList->point.x <<  "\t" << currentFeatureInList->point.y <<  "\t" << currentFeatureInList->point.z <<endl;
		#endif
		if(currentFeatureInList->matchFound)
		{
			string coordinateXString = "";
			string coordinateYString = "";
			string coordinateZString = "";
			if(applyBinning)
			{
				int xBin = determineGeoBinX(currentFeatureInList->pointTransformed.x);
				int yBin = determineGeoBinY(currentFeatureInList->pointTransformed.y);
				int zBin = 0;
				coordinateXString = convertIntToString(xBin);
				coordinateYString = convertIntToString(yBin);
				coordinateZString = convertIntToString(zBin);
			}
			else
			{
				coordinateXString = convertDoubleToString(currentFeatureInList->pointTransformed.x, "%0.6f");
				coordinateYString = convertDoubleToString(currentFeatureInList->pointTransformed.y, "%0.6f");
				coordinateZString = convertDoubleToString(currentFeatureInList->pointTransformed.z, "%0.6f");
			}

			*geoTableHTMLoutputString = *geoTableHTMLoutputString + "<TR><TD>" + coordinateXString + "</TD><TD>" + coordinateYString + "</TD><TD>" + coordinateZString + "</TD></TR>";
		}

		currentFeatureInList = currentFeatureInList->next;
	}

	*geoTableHTMLoutputString = *geoTableHTMLoutputString + "</TABLE>";
}
#endif

/*
#ifdef DEBUG_OR_OUTPUT_DT_BIN
bool determineIfGeoBinningIdenticalMatchFound(ORfeature* firstFeatureInNearestFeatureList, int pBinxRequirement[], int pBinyRequirement[])
{
	bool geoBinningIsSameAsOriginal = false;

	ORfeature* currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList;
	while((currentFeatureInNearestFeatureList->next != NULL) && !(currentFeatureInNearestFeatureList->lastFilledFeatureInList))
	{
		ORfeature* currentFeatureInNearestFeatureList2 = firstFeatureInNearestFeatureList;
		while((currentFeatureInNearestFeatureList2->next != NULL) && !(currentFeatureInNearestFeatureList2->lastFilledFeatureInList))
		{
			bool passedAll = true;
			int geoxBin[2];
			int geoyBin[2];
			int xBin = determineGeoBinX(currentFeatureInNearestFeatureList->pointTransformed.x);
			int yBin = determineGeoBinY(currentFeatureInNearestFeatureList->pointTransformed.y);
			int x2Bin = determineGeoBinX(currentFeatureInNearestFeatureList2->pointTransformed.x);
			int y2Bin = determineGeoBinY(currentFeatureInNearestFeatureList2->pointTransformed.y);
			geoxBin[0] = xBin;
			geoyBin[0] = yBin;
			geoxBin[1] = x2Bin;
			geoyBin[1] = y2Bin;
			for(int x=0; x<2; x++)
			{
				if(geoxBin[x] != pBinxRequirement[x])
				{
					passedAll = false;
				}
			}
			for(int y=0; y<2; y++)
			{
				if(geoyBin[y] != pBinyRequirement[y])
				{
					passedAll = false;
				}
			}
			if(passedAll)
			{
				//cout << "geoBinningIsSameAsOriginal" << endl;
				geoBinningIsSameAsOriginal = true;
			}
			currentFeatureInNearestFeatureList2 = currentFeatureInNearestFeatureList2->next;
		}
		currentFeatureInNearestFeatureList = currentFeatureInNearestFeatureList->next;
	}
	return geoBinningIsSameAsOriginal;
}
#endif
*/

void readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(string* rgbMapSmallFacingPolyFileNamePPM, string* rgbMapSmallFacingPolyFileNameJPEG, signed char* concatonatedSignedDctCoeffArrayRequirement, bool printOutput)
{
	#ifdef OR_DEBUG_COMPARISON
	cout << "*rgbMapSmallFacingPolyFileNamePPM = " <<* rgbMapSmallFacingPolyFileNamePPM << endl;
	cout << "*rgbMapSmallFacingPolyFileNameJPEG = " <<* rgbMapSmallFacingPolyFileNameJPEG << endl;
	#endif

	int dctCoeffArrayHeight = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
	int dctCoeffArrayWidth = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;

	signed char dctCoeffArrayY[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char dctCoeffArrayYcr[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char dctCoeffArrayYcb[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char dctCoeffArrayYDummy[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char dctCoeffArrayYCrDummy[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char dctCoeffArrayYCbDummy[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];

	string convertSmallImageRGBtoJPEGCommand = "";

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING)
	{
		convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " +* rgbMapSmallFacingPolyFileNamePPM + " " +* rgbMapSmallFacingPolyFileNameJPEG;
		system(convertSmallImageRGBtoJPEGCommand.c_str());
		readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(*rgbMapSmallFacingPolyFileNameJPEG, dctCoeffArrayY, dctCoeffArrayYCrDummy, dctCoeffArrayYCbDummy, dctCoeffArrayHeight, dctCoeffArrayWidth, printOutput);
		convertSmallImageRGBtoJPEGCommand = "";
		convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " +* rgbMapSmallFacingPolyFileNamePPM + " " +* rgbMapSmallFacingPolyFileNameJPEG;
		system(convertSmallImageRGBtoJPEGCommand.c_str());
		readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(*rgbMapSmallFacingPolyFileNameJPEG, dctCoeffArrayYDummy, dctCoeffArrayYcr, dctCoeffArrayYcb, dctCoeffArrayHeight, dctCoeffArrayWidth, printOutput);
	}
	else
	{
		convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " +* rgbMapSmallFacingPolyFileNamePPM + " " +* rgbMapSmallFacingPolyFileNameJPEG;
		system(convertSmallImageRGBtoJPEGCommand.c_str());
		readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(*rgbMapSmallFacingPolyFileNameJPEG, dctCoeffArrayY, dctCoeffArrayYcr, dctCoeffArrayYcb, dctCoeffArrayHeight, dctCoeffArrayWidth, printOutput);
	}

	//long binnedAllDCTCoeff64BitValue = convertDCTCoeffIndividualArraysToBinnedAllDCTCoeff64BitValue(dctCoeffArrayY, dctCoeffArrayYcr, dctCoeffArrayYcb, concatonatedSignedDctCoeffArrayRequirement);	//may not be used here
	convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(dctCoeffArrayY, dctCoeffArrayYcr, dctCoeffArrayYcb, concatonatedSignedDctCoeffArrayRequirement);

	#ifdef OR_DEBUG_COMPARISON
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX; i++)
	{
		cout << "concatonatedSignedDctCoeffArrayRequirement[i] = " << int(concatonatedSignedDctCoeffArrayRequirement[i]) << endl;
	}
	#endif
}

//#endif
