 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORcomparison.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#include "ATORcomparison.hpp"
#ifdef ATOR_IMAGE_COMPARISON_SQL
#endif


#ifndef LINUX
	#define ATOR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
#endif



int passes;


#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
static ATORfeatureContainer trainfeatureGeoCompBinArray[ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS_STATIC][ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS_STATIC][ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS_STATIC][ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS_STATIC][2];	//trainfeatureGeoCompBinArray now stores 2 feature combinations (instead of 1 feature)
#endif
#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
static ATORfeatureContainer trainfeatureImageAverageColourCompBinArray[ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL_STATIC][ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL_STATIC][ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL_STATIC];
#endif

//#ifdef ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS
int64_t time5aNormalisedSnapshotComparisonLoadComparisonDataTotal = 0.0;
int64_t time5aNormalisedSnapshotComparisonLoadComparisonDataIndex = 0.0;

int64_t time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal = 0.0;
int64_t time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex = 0.0;
//#endif


//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	//see dct coeff selection criteria
static int dctCoeffSelectionArrayY[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y_MAX][2];	//2 is for x and y
static int dctCoeffSelectionArrayYCr[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr_MAX][2];	//2 is for x and y
static int dctCoeffSelectionArrayYCb[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb_MAX][2];	//2 is for x and y
void ATORcomparisonClass::fillDCTcoeffSelectionArrays()
{
	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ABS_LUM_INFORMATION
	cerr << "error: ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ABS_LUM_INFORMATION has not been programmed" << endl;
	exit(EXIT_ERROR);
	#endif

	dctCoeffSelectionArrayY[0][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X;
	dctCoeffSelectionArrayY[0][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y;
	dctCoeffSelectionArrayY[1][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X;
	dctCoeffSelectionArrayY[1][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y;
	dctCoeffSelectionArrayY[2][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X;
	dctCoeffSelectionArrayY[2][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y;
	dctCoeffSelectionArrayY[3][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X;
	dctCoeffSelectionArrayY[3][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y;
	dctCoeffSelectionArrayY[4][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X;
	dctCoeffSelectionArrayY[4][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y;
	dctCoeffSelectionArrayY[5][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X;
	dctCoeffSelectionArrayY[5][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y;
	dctCoeffSelectionArrayY[6][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X;
	dctCoeffSelectionArrayY[6][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y;
	dctCoeffSelectionArrayY[7][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X;
	dctCoeffSelectionArrayY[7][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y;
	dctCoeffSelectionArrayY[8][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X;
	dctCoeffSelectionArrayY[8][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y;

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		dctCoeffSelectionArrayYCr[0][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X;
		dctCoeffSelectionArrayYCr[0][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y;
		dctCoeffSelectionArrayYCr[1][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X;
		dctCoeffSelectionArrayYCr[1][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y;
		dctCoeffSelectionArrayYCr[2][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X;
		dctCoeffSelectionArrayYCr[2][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y;
		dctCoeffSelectionArrayYCr[3][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X;
		dctCoeffSelectionArrayYCr[3][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y;
		dctCoeffSelectionArrayYCr[4][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X;
		dctCoeffSelectionArrayYCr[4][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y;
		dctCoeffSelectionArrayYCr[5][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X;
		dctCoeffSelectionArrayYCr[5][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y;
		dctCoeffSelectionArrayYCr[6][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X;
		dctCoeffSelectionArrayYCr[6][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y;
		dctCoeffSelectionArrayYCr[7][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X;
		dctCoeffSelectionArrayYCr[7][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y;
		dctCoeffSelectionArrayYCr[8][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X;
		dctCoeffSelectionArrayYCr[8][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y;

		dctCoeffSelectionArrayYCb[0][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X;
		dctCoeffSelectionArrayYCb[0][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y;
		dctCoeffSelectionArrayYCb[1][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X;
		dctCoeffSelectionArrayYCb[1][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y;
		dctCoeffSelectionArrayYCb[2][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X;
		dctCoeffSelectionArrayYCb[2][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y;
		dctCoeffSelectionArrayYCb[3][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X;
		dctCoeffSelectionArrayYCb[3][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y;
		dctCoeffSelectionArrayYCb[4][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X;
		dctCoeffSelectionArrayYCb[4][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y;
		dctCoeffSelectionArrayYCb[5][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X;
		dctCoeffSelectionArrayYCb[5][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y;
		dctCoeffSelectionArrayYCb[6][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X;
		dctCoeffSelectionArrayYCb[6][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y;
		dctCoeffSelectionArrayYCb[7][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X;
		dctCoeffSelectionArrayYCb[7][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y;
		dctCoeffSelectionArrayYCb[8][0] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X;
		dctCoeffSelectionArrayYCb[8][1] = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y;
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

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
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
//ATORfeatureContainer testfeatureGeoCompArray[numberOfTestObjects][numberOfTestViewIndicies][numberOfTestZoomIndicies][maxNumberOfTestPolysAcrossAllObjects][ATOR_METHOD_POLYGON_NUMBER_OF_SIDES];
#ifndef LINUX
#ifdef ATOR_METHOD_2DOD
	ATORfeatureContainer testfeatureGeoCompArray[ATOR_METHOD2DOD_NUM_OF_OBJECTS][ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST][ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST][ATOR_METHOD_POLYGON_NUMBER_OF_SIDES];
#elif defined ATOR_METHOD_3DOD
	ATORfeatureContainer testfeatureGeoCompArray[ATOR_METHOD3DOD_NUM_OF_OBJECTS][ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST][ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST][ATOR_METHOD_POLYGON_NUMBER_OF_SIDES];
#endif
#endif
*/

#ifdef ATOR_IMAGE_COMPARISON_SQL
double ATORcomparisonClass::compareNormalisedSnapshots(const int numberOfTestPolys[], const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTestZoomIndicies, const int trainOrTest, const int testViewNumber)
#else
double ATORcomparisonClass::compareNormalisedSnapshots(const int numberOfTrainPolys[], const int numberOfTestPolys[], const int numberOfTrainViewIndicies, const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string trainObjectNameArray[], const int numberOfTrainObjects, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTrainZoomIndicies, const int numberOfTestZoomIndicies, const int testViewNumber)
#endif
{
	string currentTempFolder = SHAREDvars.getCurrentDirectory();

	ATORobjectReferenceList* firstReferenceInSnapshotMatchObjectReferenceList = new ATORobjectReferenceList();

	bool useGeneratedTestPixmapFiles = false;
	bool useGeneratedTrainPixmapFiles = false;
	bool ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = false;

	#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
	if(ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
	{
		if(ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
		{
		}
		else
		{
			useGeneratedTestPixmapFiles = true;
			useGeneratedTrainPixmapFiles = true;
			if(!ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
			{
				ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = true;
			}
		}
	}
	else
	{
		#ifdef ATOR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
		cerr << "Linux must be used or ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL" << endl;
		exit(EXIT_ERROR);
		#endif
		if(ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
		{
			useGeneratedTestPixmapFiles = true;
		}
		else
		{
			useGeneratedTestPixmapFiles = true;
			useGeneratedTrainPixmapFiles = true;
		}

		if(!ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
		{
			if(!ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
			{
				ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = true;
			}
		}
	}
	#else
	useGeneratedTestPixmapFiles = true;
	useGeneratedTrainPixmapFiles = true;
	ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = true;
	#endif


	bool doNotCheckContrast;
	int smallImageRatio;
	if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
	{
		smallImageRatio = ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO;
		#ifdef ATOR_METHOD_2DOD_DO_NOT_REQUIRE_SNAPSHOT_CONTRAST
		doNotCheckContrast = true;
		#else
		doNotCheckContrast = false;
		#endif
	}
	else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		smallImageRatio = ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO;
		#ifdef ATOR_METHOD_3DOD_DO_NOT_REQUIRE_SNAPSHOT_CONTRAST
		doNotCheckContrast = true;
		#else
		doNotCheckContrast = false;
		#endif
	}
	else
	{
		cerr << "error: illegal dimension" << endl;
		exit(EXIT_ERROR);
	}


	ofstream writeFileObject(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME);


	if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{

		string ICRheader = "";
		if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS)
		{
			ICRheader = ICRheader + "<HTML><HEAD><TITLE>Results </TITLE><style type=\"text/css\">TD { font-size:50%; } </style></HEAD><BODY>Results<p>Project Version: 3r1a 29-February-2024<p>";
		}
		else
		{
			ICRheader = ICRheader + "<HTML><HEAD><TITLE>Image Comparitor Results For Normalised Snapshots (CONFIDENTIAL - PROVIDED UNDER CONTRACT)</TITLE><style type=\"text/css\">body{font-size:50%}</style></HEAD><BODY><h1>Image Comparitor Results For Normalised Snapshots (CONFIDENTIAL - PROVIDED UNDER CONTRACT)</h1><p>Source Version 2d2g<p>Richard Bruce Baxter Object Recognition Method - Australian Provisional Patent Filed";
		}


		SHAREDvars.writeStringToFileObject(ICRheader, &writeFileObject);

		string ICRtableStart = "";
		if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
		{
			ICRtableStart = ICRtableStart + "<TABLE width=\"100%\" cellpadding=\"3\" cellspacing=\"0\" border=\"1\"><TR><TD colspan=\"6\">Test</TD><TD colspan=\"6\">Train</TD><TD></TD></TR><TR><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>ATORpolygon</TD><TD>Side</TD><TD>Image</TD><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>ATORpolygon</TD><TD>Side</TD><TD>Image</TD><TD></TD></TR>";
		}
		else
		{
			ICRtableStart = ICRtableStart + "<TABLE width=\"100%\" cellpadding=\"3\" cellspacing=\"0\" border=\"1\"><TR><TD colspan=\"6\">Test</TD><TD colspan=\"6\">Train</TD><TD></TD></TR><TR><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>X</TD><TD>Y</TD><TD>Image</TD><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>X</TD><TD>Y</TD><TD>Image</TD><TD></TD></TR>";
		}
		SHAREDvars.writeStringToFileObject(ICRtableStart, &writeFileObject);

	}


	//optimisation;
	uchar* rgbMapTrain = new uchar[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	uchar* rgbMapTest = new uchar[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	//#ifdef ATOR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	double* traindepthMap = new double[imageWidthFacingPoly*imageHeightFacingPoly];
	double* testdepthMap = new double[imageWidthFacingPoly*imageHeightFacingPoly];
	//#endif

	int smallImageWidth = imageWidthFacingPoly/smallImageRatio;
	int smallImageHeight = imageHeightFacingPoly/smallImageRatio;

	#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
	uchar* trainrgbMapSmall = new uchar[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	uchar* testrgbMapSmall = new uchar[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	//#ifdef ATOR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	double* traindepthMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)];
	double* testdepthMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)];
	//#endif
	#endif

	//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	schar* testdctCoeffArrayY = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* testdctCoeffArrayYCr = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* testdctCoeffArrayYCb = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* testdctCoeffArrayYDummy = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* testdctCoeffArrayYCrDummy = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* testdctCoeffArrayYCbDummy = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* testconcatonatedSignedDctCoeffArrayRequirement = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];

	//#endif

	//#ifdef ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL
	//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	double* trainrgbDevMap = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
	double* testrgbDevMap = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
	//#endif
	//#ifdef ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	double* trainrgbDevMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*VEC_MAP_VEC_NUM_DIMENSIONS];
	double* testrgbDevMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*VEC_MAP_VEC_NUM_DIMENSIONS];
	uchar* trainrgbDev8BitSmallMap = new uchar[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	uchar* testrgbDev8BitSmallMap = new uchar[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	//#endif
	//#endif


#ifdef TEMPTEST3GEOACCURACY
	int numberOfNearestFeatures = ATORoperations.calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);
	if(ATOR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "numberOfPolygonsPerFeature = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
		cout << "numberOfAdditionalFeatures = " << ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
		cout << "numberOfNearestFeatures = " << numberOfNearestFeatures << endl;
	}

	ATORfeature* testFirstFeatureInNearestFeatureList = new ATORfeature();
	ATORfeature* trainFirstFeatureInNearestFeatureList = new ATORfeature();

	ATORoperations.generateNearestFeaturesList(testFirstFeatureInNearestFeatureList, numberOfNearestFeatures);
	ATORoperations.generateNearestFeaturesList(trainFirstFeatureInNearestFeatureList, numberOfNearestFeatures);

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

#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON

	int numberOfNearestFeatures = ATORoperations.calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);

	ATORfeature* testFirstFeatureInNearestFeatureList = new ATORfeature();
	ATORfeature* trainFirstFeatureInNearestFeatureList = new ATORfeature();

	ATORoperations.generateNearestFeaturesList(testFirstFeatureInNearestFeatureList, numberOfNearestFeatures);
	ATORoperations.generateNearestFeaturesList(trainFirstFeatureInNearestFeatureList, numberOfNearestFeatures);

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
	ATORfeatureContainer testfeatureGeoCompArray[numberOfTestObjects][numberOfTestViewIndicies][numberOfTestZoomIndicies][maxNumberOfTestPolysAcrossAllObjects][ATOR_METHOD_POLYGON_NUMBER_OF_SIDES];
	#else
	/*
	array<ATORfeatureContainer ^> ^ testfeatureGeoCompArray = new array<ATORfeatureContainer ^>(numberOfTestObjects*numberOfTestViewIndicies*numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*ATOR_METHOD_POLYGON_NUMBER_OF_SIDES);
	int tempdim1 = numberOfTestViewIndicies*numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*ATOR_METHOD_POLYGON_NUMBER_OF_SIDES
	int tempdim2 = numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*ATOR_METHOD_POLYGON_NUMBER_OF_SIDES
	int tempdim3 = maxNumberOfTestPolysAcrossAllObjects*ATOR_METHOD_POLYGON_NUMBER_OF_SIDES
	int tempdim4 = ATOR_METHOD_POLYGON_NUMBER_OF_SIDES
	*/
	#endif

	if(ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO)
	{
		for(int testObjectIndex=0; testObjectIndex<numberOfTestObjects; testObjectIndex++)
		{
			for(int testViewIndex=0; testViewIndex<numberOfTestViewIndicies; testViewIndex++)
			{//there will not be multiple testview indicies for 2D data (image) algorithm

				for(int testZoomIndex=0; testZoomIndex < numberOfTestZoomIndicies; testZoomIndex++)
				{
					for(int testPolyIndex=0; testPolyIndex<numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]; testPolyIndex++)
					{
						for(int testSideIndex=0; testSideIndex < ATOR_METHOD_POLYGON_NUMBER_OF_SIDES; testSideIndex++)
						{
							string testpolygonIndexString = SHAREDvars.convertIntToString(testPolyIndex);
							string testsideIndexString = SHAREDvars.convertIntToString(testSideIndex);
							string testviewIndexString = SHAREDvars.convertIntToString(testViewIndex+testViewNumber);
							string testzoomIndexString = SHAREDvars.convertIntToString(testZoomIndex);

							string testinterpolatedRGBMapFileNameForRayTracing;
							testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

							string testinterpolatedNearestFeaturesMapFileName;
							testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;

							ATORfeature* firstFeatureInList = new ATORfeature();

							bool ignoreOTfeatures;
							if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
							{
								ignoreOTfeatures = true;
							}
							else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
							{
								#ifdef ATOR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
								ignoreOTfeatures = true;
								#else
								ignoreOTfeatures = false;
								#endif
							}

							ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, firstFeatureInList, true, false, ignoreOTfeatures);
							#ifndef ATOR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
							(testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList = firstFeatureInList;
							#endif
						}
					}
				}
			}
		}
	}

	#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO

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

	#ifndef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
	ATORfeatureContainer trainfeatureGeoCompArray[numberOfTrainObjects][numberOfTrainViewIndicies][numberOfTrainZoomIndicies][maxNumberOfTrainPolysAcrossAllObjects][ATOR_METHOD_POLYGON_NUMBER_OF_SIDES];
	#endif

	for(int trainObjectIndex=0; trainObjectIndex<numberOfTrainObjects; trainObjectIndex++)
	{
		for(int trainViewIndex=0; trainViewIndex<numberOfTrainViewIndicies; trainViewIndex++)
		{//there will not be multiple trainview indicies for 2D data (image) algorithm

			for(int trainZoomIndex=0; trainZoomIndex < numberOfTrainZoomIndicies; trainZoomIndex++)
			{
				for(int trainPolyIndex=0; trainPolyIndex<numberOfTrainPolys[(trainObjectIndex*numberOfTrainViewIndicies*numberOfTrainZoomIndicies)+(trainViewIndex*numberOfTrainZoomIndicies) + trainZoomIndex]; trainPolyIndex++)
				{
					for(int trainSideIndex=0; trainSideIndex < ATOR_METHOD_POLYGON_NUMBER_OF_SIDES; trainSideIndex++)
					{
						string trainpolygonIndexString = SHAREDvars.convertIntToString(trainPolyIndex);
						string trainsideIndexString = SHAREDvars.convertIntToString(trainSideIndex);
						string trainviewIndexString = SHAREDvars.convertIntToString(trainViewIndex);
						string trainzoomIndexString = SHAREDvars.convertIntToString(trainZoomIndex);

						string traininterpolatedRGBMapFileNameForRayTracing;
						traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainObjectIndex] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

						string traininterpolatedNearestFeaturesMapFileName;
						traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
						//ATORoperations.generateNearestFeaturesList(&(trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainPolyIndex][trainSideIndex]), numberOfNearestFeatures);

						ATORfeature* firstFeatureInList = new ATORfeature();

						bool ignoreOTfeatures;
						if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
						{
							ignoreOTfeatures = true;

						}
						else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
							#ifdef ATOR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
							ignoreOTfeatures = true;
							#else
							ignoreOTfeatures = false;
							#endif

						}
						ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, firstFeatureInList, true, false, ignoreOTfeatures);

						#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
						string trainrgbMapFacingPolyFileName = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;

						RTpixelMaps.readImage(trainrgbMapFacingPolyFileName, rgbMapTrain);

						colour avgCol;
						ATORimagecomparison.calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTrain, &avgCol);

						int rBin = (int)(((double)(avgCol.r)/(MAX_RGB_VAL))*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int gBin = (int)(((double)(avgCol.g)/(MAX_RGB_VAL))*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int bBin = (int)(((double)(avgCol.b)/(MAX_RGB_VAL))*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);

						ATORfeatureContainer* currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[rBin][gBin][bBin]);
						// go to last feature container in list (ie append to list if list already has items)
						while(currentFeatureContainerInList->next != NULL)
						{
							currentFeatureContainerInList = currentFeatureContainerInList->next;
						}
						currentFeatureContainerInList->firstFeatureInFeatureList = firstFeatureInList;
						ATORfeatureContainer* newFeatureContainer = new ATORfeatureContainer();
						currentFeatureContainerInList->next = newFeatureContainer;
						#else
						(trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainZoomIndex][trainPolyIndex][trainSideIndex]).firstFeatureInFeatureList = firstFeatureInList;
						#endif
					}
				}
			}
		}
	}

	#elif defined ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

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
					for(int trainSideIndex=0; trainSideIndex < ATOR_METHOD_POLYGON_NUMBER_OF_SIDES; trainSideIndex++)
					{
						string trainpolygonIndexString = SHAREDvars.convertIntToString(trainPolyIndex);
						string trainsideIndexString = SHAREDvars.convertIntToString(trainSideIndex);
						string trainviewIndexString = SHAREDvars.convertIntToString(trainViewIndex);
						string trainzoomIndexString = SHAREDvars.convertIntToString(trainZoomIndex);

						string traininterpolatedRGBMapFileNameForRayTracing;
						traininterpolatedRGBMapFileNameForRayTracing = trainObjectNameArray[trainObjectIndex] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

						string traininterpolatedNearestFeaturesMapFileName;
						traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
						ATORfeature* firstFeatureInTempList = new ATORfeature();

						bool ignoreOTfeatures;
						if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
						{
							ignoreOTfeatures = true;
						}
						else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
						{
							#ifdef ATOR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
							ignoreOTfeatures = true;
							#else
							ignoreOTfeatures = false;
							#endif
						}
						ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, firstFeatureInTempList, true, false, ignoreOTfeatures);

						ATORfeature* currentFeatureInTempList = firstFeatureInTempList;

						//now bin the features;
						int findex1 = 0;

						while(currentFeatureInTempList->next != NULL)
						{
							ATORfeature* currentFeatureInTempList2 = firstFeatureInTempList;

							int findex2 = 0;
							while(currentFeatureInTempList2->next != NULL)
							{
								if(findex1 != findex2)
								{
									int x1Bin = ATORdatabaseDecisionTreeOperations.determineGeoBinX(currentFeatureInTempList->pointTransformed.x);
									int y1Bin = ATORdatabaseDecisionTreeOperations.determineGeoBinY(currentFeatureInTempList->pointTransformed.y);
									int x2Bin = ATORdatabaseDecisionTreeOperations.determineGeoBinX(currentFeatureInTempList2->pointTransformed.x);
									int y2Bin = ATORdatabaseDecisionTreeOperations.determineGeoBinY(currentFeatureInTempList2->pointTransformed.y);

									bool insideBin = false;

									if(x1Bin >= 0 )
									{
										if(x1Bin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
										{
											if(y1Bin >= 0 )
											{
												if(y1Bin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
												{
													if(x2Bin >= 0 )
													{
														if(x2Bin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
														{
															if(y2Bin >= 0 )
															{
																if(y2Bin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
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
										ATORfeatureContainer* currentFeatureContainerInList1 = &(trainfeatureGeoCompBinArray[x1Bin][y1Bin][x2Bin][y2Bin][0]);
										ATORfeatureContainer* currentFeatureContainerInList2 = &(trainfeatureGeoCompBinArray[x1Bin][y1Bin][x2Bin][y2Bin][1]);

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
										ATORfeatureContainer* newFeatureContainer1 = new ATORfeatureContainer();
										currentFeatureContainerInList1->next = newFeatureContainer1;
										currentFeatureContainerInList2->firstFeatureInFeatureList = currentFeatureInTempList2;
										ATORfeatureContainer* newFeatureContainer2 = new ATORfeatureContainer();
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


	#endif


#endif

	passes = 0;

	double averageMatchErrorAcrossAllObjects = 0.0;

	for(int testObjectIndex=0; testObjectIndex<numberOfTestObjects; testObjectIndex++)
	{
		if(ATOR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "\t\t  testObjectIndex = " << testObjectIndex << endl;
		}

		#ifndef ATOR_IMAGE_COMPARISON_SQL
		if(ATOR_IMAGE_COMPARISON_DECISION_TREE)
		{
			#ifndef TEMP_REMOVE_FLASH
			if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
			{
				string trainviewIndexStringTemp = SHAREDvars.convertIntToString(0);

				string mapFileName = trainObjectNameArray[testObjectIndex] + "initialViewMap" + "ViewIndex" + trainviewIndexStringTemp + "ZoomIndex" + "0";
				string blank = "";

				string convertPPMtoPNGCommand = "convert " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION + " " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION;
				if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
				{
					cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
				}
				system(convertPPMtoPNGCommand.c_str());

				if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
				{
					string convertPPMtoPNGCommand2 = "convert " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION + " " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION;
					if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
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
			if(ATOR_PRINT_ALGORITHM_PROGRESS)
			{
				cout << "\t\t\t  testViewIndex = " << testViewIndex+testViewNumber << endl;
			}

			for(int testZoomIndex=0; testZoomIndex < numberOfTestZoomIndicies; testZoomIndex++)
			{
				if(ATOR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "\t\t\t  testZoomIndex = " << testZoomIndex << endl;
				}

				int64_t time5zNormalisedSnapshotComparisonSingleViewTestStart;
				if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS)
				{
					#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					cout << "\t\t\t start: 5z. normalised snapshot comparison - single view speed test (for website)" << endl;
					#endif
					time5zNormalisedSnapshotComparisonSingleViewTestStart = SHAREDvars.getTimeAsLong();
				}
				int numPassesForTestViewIndex = 0;

				#ifndef TEMP_REMOVE_FLASH
				if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
				{
					string testviewIndexStringTemp = SHAREDvars.convertIntToString(testViewIndex+testViewNumber);
					string testzoomIndexStringTemp = SHAREDvars.convertIntToString(testZoomIndex);

					string mapFileName = testObjectNameArray[testObjectIndex] + "initialViewMap" + "ViewIndex" + testviewIndexStringTemp + "ZoomIndex" + testzoomIndexStringTemp;
					string blank = "";

					string convertPPMtoPNGCommand = "convert " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION + " " + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION;
					if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
					{
						cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
					}
					system(convertPPMtoPNGCommand.c_str());

					if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
					{
						string convertPPMtoPNGCommand2 = "convert " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION + " " + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION;
						if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
						{
							cout << "system(" << convertPPMtoPNGCommand2 << ");" << endl;
						}
						system(convertPPMtoPNGCommand2.c_str());
					}

					string ICRNewTestViewIndexRow3 = "<TR><TD colspan=\"6\"><img src=\"" + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION + "\" border=0> </TD><TD colspan=\"6\"><img src=\"" + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION + "\" border=0> </TD><TD></TD></TR>";
					SHAREDvars.writeStringToFileObject(ICRNewTestViewIndexRow3, &writeFileObject);
					if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
					{
						string ICRNewTestViewIndexRow4 = "<TR><TD colspan=\"6\"><img src=\"" + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION + "\" border=0> </TD><TD colspan=\"6\"><img src=\"" + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION + "\" border=0> </TD><TD></TD></TR>";
						SHAREDvars.writeStringToFileObject(ICRNewTestViewIndexRow4, &writeFileObject);
					}
				}
				#endif

				double averageMatchError = 0.0;

				for(int testPolyIndex=0; testPolyIndex<numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]; testPolyIndex++)
				{
					if(ATOR_PRINT_ALGORITHM_PROGRESS)
					{
						if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
						{
							cout << "\t\t\t\t  testPIndex = " << testPolyIndex << endl;
						}
					}

					int trainPolyIndexWithLowestErrorRecord[3][5];	//for each test side x3, 0=trainObjectIndex, 1=trainViewIndex, 2=trainViewIndex, 3=trainpolyindex, 4=trainsideIndex

					#ifndef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
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


					#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
					string trainObjectNameWithLowestErrorRecordAcrossAllSides;
					#endif

					double averageMatchErrorAcrossSides = 0.0;

					for(int testSideIndex=0; testSideIndex < ATOR_METHOD_POLYGON_NUMBER_OF_SIDES; testSideIndex++)
					{

						bool testMapLoaded;
						if(ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
						{
							testMapLoaded = false;
						}

						string testviewIndexString = SHAREDvars.convertIntToString(testViewIndex+testViewNumber);
						string testzoomIndexString = SHAREDvars.convertIntToString(testZoomIndex);
						string testpolygonIndexString = SHAREDvars.convertIntToString(testPolyIndex);
						string testsideIndexString = SHAREDvars.convertIntToString(testSideIndex);

						string testinterpolatedRGBMapFileNameForRayTracing;
						testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

						string testrgbMapSmallFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

						if(useGeneratedTestPixmapFiles)
						{
							if(!ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
							{

								string testrgbMapFacingPolyFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

								string testrgbMapFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

								RTpixelMaps.readImage(testrgbMapFacingPolyFileName, rgbMapTest);

								#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
								RTpixelMaps.readImage(testrgbMapSmallFacingPolyFileName, testrgbMapSmall);
								#endif

							}


							if(!ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								if(!ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO)
								{
									/*
									if(ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
									{
										string testviewIndexString = SHAREDvars.convertIntToString(testViewIndex+testViewNumber);
										string testzoomIndexString = SHAREDvars.convertIntToString(testZoomIndex);
										string testpolygonIndexString = SHAREDvars.convertIntToString(testPolyIndex);
										string testsideIndexString = SHAREDvars.convertIntToString(testSideIndex);
										string testinterpolatedRGBMapFileNameForRayTracing;
										testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
									}
									*/
									string testinterpolatedNearestFeaturesMapFileName;
									testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
									ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);
								}
							}

							#ifdef TEMPTEST3GEOACCURACY
							string testinterpolatedNearestFeaturesMapFileName;
							testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
							ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);
							//cout << "TEMPTEST3GEOACCURACY Part 2" << endl;
							//cout << "testinterpolatedNearestFeaturesMapFileName = " << testinterpolatedNearestFeaturesMapFileName << endl;
							//cout << "end" << endl;
							#endif
						}

						double currentLowestError = VERY_HIGH_ERROR;

					#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON_BINNING

						ATORfeature* currentTestFeature;

						#ifdef ATOR_USE_SQL
						if(ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
						{
							currentTestFeature = new ATORfeature();
							int64_t testID = 0;							//this will be used in the future to shortcut identify test snapshots for a particular object at a particular viewindex and zoom that have been recently added to database and require comparision against the entire database
							string sqlDatabaseTestTableName = ATOR_MYSQL_TABLE_NAME_TEST;			//this will be used in the future to shortcut identify test snapshots for a particular object at a particular viewindex and zoom that have been recently added to database and require comparision against the entire database
							bool ignoreOTfeaturestest;
							if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
							{
								ignoreOTfeaturestest = true;

							}
							else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
							{
								#ifdef ATOR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
								ignoreOTfeaturestest = true;
								#else
								ignoreOTfeaturestest = false;
								#endif

							}
							ATORdatabaseSQL.createFeatureListUsingDatabaseQuery(currentTestFeature, true, false, ignoreOTfeaturestest, sqlDatabaseTestTableName, testID, testObjectNameArray[testObjectIndex], testViewIndex+testViewNumber, testZoomIndex, testPolyIndex, testSideIndex, false);

							if(ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
							{
								ATORdatabaseSQL.convertSQLdatabaseStringToSnapshotMaps(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, testrgbDevMap, testdepthMap, smallImageWidth, smallImageHeight, testrgbMapSmall, testdepthMapSmall, testrgbDevMapSmall, dimension, currentTestFeature->snapshotMapsText, ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, ATOR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS);
							}
						}
						else
						{
						#endif
							if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
							{
								#ifndef ATOR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
								currentTestFeature = (testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList;
								#endif
							}
							else
							{
								currentTestFeature = new ATORfeature(); 	//create dummy feature - not used.
							}
						#ifdef ATOR_USE_SQL
						}
						#endif

						colour normalisedAverageHueDeviationRequirement;
						if(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING)
						{
							if(ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								normalisedAverageHueDeviationRequirement.r = currentTestFeature->avgCol.r;
								normalisedAverageHueDeviationRequirement.g = currentTestFeature->avgCol.g;
								normalisedAverageHueDeviationRequirement.b = currentTestFeature->avgCol.b;
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
								ATORimagecomparison.createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(imageWidthFacingPoly, imageWidthFacingPoly, rgbMapTest, testrgbDevMap, &normalisedAverageHueContrastR, &normalisedAverageHueContrastG, &normalisedAverageHueContrastB, &averageContrastWithRespectToAverageColour, &averageStarkContrastWithRespectToAverageColour, &averageLocalContrast, &averageLocalStarkContrast);
								vec normalisedAverageHueContrast;
								normalisedAverageHueContrast.x = normalisedAverageHueContrastR;
								normalisedAverageHueContrast.y = normalisedAverageHueContrastG;
								normalisedAverageHueContrast.z = normalisedAverageHueContrastB;
								colour cullednormalisedAverageHueContrast;

								cullAndBinNormalisedHueContrast(&normalisedAverageHueContrast, &cullednormalisedAverageHueContrast);

								normalisedAverageHueDeviationRequirement.r = cullednormalisedAverageHueContrast.r;
								normalisedAverageHueDeviationRequirement.g = cullednormalisedAverageHueContrast.g;
								normalisedAverageHueDeviationRequirement.b = cullednormalisedAverageHueContrast.b;
							}
						}

						uchar* rgb8bitSmallMapForInstantDBqueryAccessRequirement;
						if(ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON)
						{
							/*
							if(ATOR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS)
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
							ATORimagecomparison.createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(smallImageWidth, smallImageHeight, testrgbMapSmall, testrgbDevMapSmall, &smallTempnormalisedAverageRHueContrast, &smallTempnormalisedAverageGHueContrast, &smallTempnormalisedAverageBHueContrast, &smallTempaverageContrastWithRespectToAverageColour, &smallTempaverageStarkContrastWithRespectToAverageColour, &smallTempaverageLocalContrast, &smallTempaverageLocalStarkContrast);
							convertNormalisedHueDeviationMapTo3x8bitMap(smallImageWidth, smallImageHeight, testrgbDevMapSmall, testrgbDev8BitSmallMap);
							rgb8bitSmallMapForInstantDBqueryAccessRequirement = testrgbDev8BitSmallMap;
						}
						else
						{
							rgb8bitSmallMapForInstantDBqueryAccessRequirement = NULL;
						}

						schar* testconcatonatedSignedDctCoeffArrayRequirement = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
						{
							if(ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								testconcatonatedSignedDctCoeffArrayRequirement = currentTestFeature->dctCoeff;

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

						//#ifdef ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
						bool fLoopOn = true;
						bool fLoopSingle = false;
						if(!ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							fLoopSingle = true;
						}

						ATORfeature* testcurrentFeatureInNearestFeatureList = currentTestFeature;
						int findex1 = 0;
						while((testcurrentFeatureInNearestFeatureList->next != NULL) && fLoopOn)
						{
							ATORfeature* testcurrentFeatureInNearestFeatureList2 = currentTestFeature;
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
									if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
									{
										xBin = ATORdatabaseDecisionTreeOperations.determineGeoBinX(testcurrentFeatureInNearestFeatureList->pointTransformed.x);
										yBin = ATORdatabaseDecisionTreeOperations.determineGeoBinY(testcurrentFeatureInNearestFeatureList->pointTransformed.y);
										x2Bin = ATORdatabaseDecisionTreeOperations.determineGeoBinX(testcurrentFeatureInNearestFeatureList2->pointTransformed.x);
										y2Bin = ATORdatabaseDecisionTreeOperations.determineGeoBinY(testcurrentFeatureInNearestFeatureList2->pointTransformed.y);


										#ifndef ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NO_EXPLICIT_FOR_LOOPS
										if((xBin > 0)
										&& (xBin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (yBin > 0)
										&& (yBin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
										&& (x2Bin > 0)
										&& (x2Bin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (y2Bin > 0)
										&& (y2Bin < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1))
										#else
										if((xBin >= 0)
										&& (xBin <= ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (yBin >= 0)
										&& (yBin <= ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
										&& (x2Bin >= 0)
										&& (x2Bin <= ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
										&& (y2Bin >= 0)
										&& (y2Bin <= ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1))
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
										#ifndef ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NO_EXPLICIT_FOR_LOOPS
										for(int x = xBin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=xBin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
										{
											for(int y = yBin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=yBin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
											{
												//check current bin and nearest bins
												for(int x2 = x2Bin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2<=x2Bin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2++)
												{
													for(int y2 = y2Bin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2<=y2Bin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2++)
													{
										#else

										int x;
										int y;
										int x2;
										int y2;

										if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
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

									#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

										ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin =  &(trainfeatureGeoCompBinArray[x][y][x2][y2][0]);
										ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin2 =  &(trainfeatureGeoCompBinArray[x][y][x2][y2][1]);
										ATORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
										ATORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin2 = firstFeatureContainerInTestFeatureMatchingTrainBin2;

									#else

										int geoxBin[2];
										int geoyBin[2];
										int64_t geoxybin = 0.0;

										geoxBin[0] = x;
										geoyBin[0] = y;
										geoxBin[1] = x2;
										geoyBin[1] = y2;

										if(!ATOR_IMAGE_COMPARISON_DECISION_TREE)
										{
											#ifdef ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_REQUIREMENT_V1
											geoxybin = ATORdatabaseSQL.calculateGeoxyBinMultiDimensional(geoxBin, geoyBin);
											#endif
										}

										bool ignoreOTfeaturestrain;
										if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
										{
											ignoreOTfeaturestrain = true;
										}
										else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
										{
											#ifdef ATOR_METHOD_3DOD_IGNORE_OT_FEATURES_DURING_GEO_COMPARISON
											ignoreOTfeaturestrain = true;
											#else
											ignoreOTfeaturestrain = false;
											#endif
										}



										ATORfeatureContainer* currentFeatureContainerInTestFeatureMatchingTrainBin;
										ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin = new ATORfeatureContainer();

										string DTbin = "";
										#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
										//assert ATOR_IMAGE_COMPARISON_DECISION_TREE
										int exceptionModifier = 1;	//positive
										for(int exceptionIndex=0; exceptionIndex<(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)+1; exceptionIndex++)
										{
											if(ATOR_IMAGE_COMPARISON_DECISION_TREE)
											{
												if(exceptionIndex != ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)
												{
													testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2] = testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2]+exceptionModifier;
												}
										#else
										if(ATOR_IMAGE_COMPARISON_DECISION_TREE)
										{
										#endif
											#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_DIRECT_ACCESS
											ATORdatabaseDecisionTree.createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(firstFeatureContainerInTestFeatureMatchingTrainBin, ignoreOTfeaturestrain, smallImageWidth, smallImageHeight, testrgbMapSmall, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, trainOrTest);
											#else


											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;

											ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList = new ATORsnapshotIDreferenceList();
											ATORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDreferenceList;

											ATORdatabaseDecisionTree.getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(smallImageWidth, smallImageHeight, testrgbMapSmall, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, firstReferenceInSnapshotIDreferenceList, trainOrTest, &DTbin);


											#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											ATORdatabaseDecisionTree.createAndOrParseIntoDirectory(&imageComparisonTreeBaseDirectory, NULL, false, false);	//reset current directory
											#endif

											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;

											int refcount = 0;
											while(currentReferenceInSnapshotIDReferenceList->next != NULL)
											{

												int irrelevant = 0;
												ATORfeature* firstNewFeature = new ATORfeature();
												currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList = firstNewFeature;
												ATORdatabaseSQL.createFeatureListUsingDatabaseQuery(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList, true, false, ignoreOTfeaturestrain, ATOR_MYSQL_TABLE_NAME_TRAIN, currentReferenceInSnapshotIDReferenceList->referenceID, "", irrelevant, irrelevant, irrelevant, irrelevant, true);
												ATORfeatureContainer* newFeatureContainer = new ATORfeatureContainer();
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
											ATORdatabaseSQL.createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(firstFeatureContainerInTestFeatureMatchingTrainBin, true, false, ignoreOTfeaturestrain, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, rgb8bitSmallMapForInstantDBqueryAccessRequirement, smallImageWidth, smallImageHeight, ATOR_MYSQL_TABLE_NAME_TRAIN, trainOrTest);
											currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
											#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
											if(exceptionIndex != 0)
											{
												currentFeatureContainerInTestFeatureMatchingTrainBin = NULL;
											}
											#endif
										}
									#endif


										int indx = 0;

										while(currentFeatureContainerInTestFeatureMatchingTrainBin->next != NULL)
										{
											passes++;


											indx++;

											bool passedGeometricCheck = false;

											if(ATOR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY)
											{
												if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
												{
													#ifdef USE_GEO_TEST_GRID_SIZE_LARGE
													passedGeometricCheck = true;
													#else
													double currentDistanceBetweenTwoPoints = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed));
													double xDev1 = testcurrentFeatureInNearestFeatureList->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x;
													double yDev1 = testcurrentFeatureInNearestFeatureList->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y;
													if(currentDistanceBetweenTwoPoints < ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
													{
														#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
														double currentDistanceBetweenTwoPoints2 = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList2->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed));
														double xDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.x;
														double yDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.y;
														#else
														double currentDistanceBetweenTwoPoints2 = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList2->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed));
														double xDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x;
														double yDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y;
														#endif

														if(currentDistanceBetweenTwoPoints2 < ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
														{
															#ifdef ATOR_GEOMETRIC_CHECK_COMPARISON_ERROR_DEV_CHECK
															if(sqrt(pow(SHAREDvars.absDouble(xDev1 - xDev2),2.0), pow(SHAREDvars.absDouble(yDev1 - yDev2), 2.0)) < ATOR_GEOMETRIC_CHECK_COMPARISON_ERROR_DEV)
															{
															#endif
																passedGeometricCheck = true;
															#ifdef ATOR_GEOMETRIC_CHECK_COMPARISON_ERROR_DEV_CHECK
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


												string trainpolygonIndexString = SHAREDvars.convertIntToString(trainPolyIndex);
												string trainsideIndexString = SHAREDvars.convertIntToString(trainSideIndex);
												string trainviewIndexString = SHAREDvars.convertIntToString(trainViewIndex);
												string trainzoomIndexString = SHAREDvars.convertIntToString(trainZoomIndex);

												string traininterpolatedRGBMapFileNameForRayTracing;
												traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

					#else



						#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL

						colour avgColTestMap;
						ATORimagecomparison.calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, &avgColTestMap);

						int rBinMid = (int)(((double)avgColTestMap.r/MAX_RGB_VAL)*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int gBinMid = (int)(((double)avgColTestMap.g/MAX_RGB_VAL)*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int bBinMid = (int)(((double)avgColTestMap.b/MAX_RGB_VAL)*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);


						int rBinMin = rBinMid-(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int rBinMax = rBinMid+(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int gBinMin = gBinMid-(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int gBinMax = gBinMid+(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int bBinMin = bBinMid-(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);
						int bBinMax = bBinMid+(ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NO_BINS_TO_COMPARE/2);

						#ifndef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_ALLOW_LIGHTING_COLOUR_SATURATION_VARIATION
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
						#endif

						for(int r = rBinMin; r < rBinMax; r++)
						{
							for(int g = gBinMin; g < gBinMax; g++)
							{
								for(int b = bBinMin; b < bBinMax; b++)
								{
									#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_ALLOW_LIGHTING_COLOUR_SATURATION_VARIATION
									for(int lightColourSaturationVar = 0-ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_LIGHTING_COLOUR_SATURATION_VARIATION_VAL_PER_COL; lightColourSaturationVar < 0+ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_LIGHTING_COLOUR_SATURATION_VARIATION_VAL_PER_COL; lightColourSaturationVar++)
									{
										int rColourSaturationVaried = r + lightColourSaturationVar;
										int gColourSaturationVaried = g + lightColourSaturationVar;
										int bColourSaturationVaried = b + lightColourSaturationVar;

										bool performImgCheck = true;
										if(rColourSaturationVaried < 0)
										{
											performImgCheck = false;
										}
										if(rColourSaturationVaried > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
										{
											performImgCheck = false;
										}
										if(gColourSaturationVaried < 0)
										{
											performImgCheck = false;
										}
										if(gColourSaturationVaried > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
										{
											performImgCheck = false;
										}
										if(bColourSaturationVaried < 0)
										{
											performImgCheck = false;
										}
										if(bColourSaturationVaried > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
										{
											performImgCheck = false;
										}

										if(performImgCheck)
										{
											ATORfeatureContainer* currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[rColourSaturationVaried][gColourSaturationVaried][bColourSaturationVaried]);
									#else
										ATORfeatureContainer* currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[r][g][b]);
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
										for(int trainSideIndex=0; trainSideIndex < ATOR_METHOD_POLYGON_NUMBER_OF_SIDES; trainSideIndex++)
										{

											string trainObjectString = objectNameArray[trainObjectIndex];

						#endif

										string trainpolygonIndexString = SHAREDvars.convertIntToString(trainPolyIndex);
										string trainsideIndexString = SHAREDvars.convertIntToString(trainSideIndex);
										string trainviewIndexString = SHAREDvars.convertIntToString(trainViewIndex);
										string trainzoomIndexString = SHAREDvars.convertIntToString(trainZoomIndex);

										string traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

										#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
										bool exactMatchFoundIrrelevant;
										#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO
											#ifndef ATOR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
											#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
											bool passedGeometricCheck = ATORdatabaseFileIO.compareFeaturesListForMatch((testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList, currentFeatureContainerInList->firstFeatureInFeatureList, dimension, &exactMatchFoundIrrelevant);
											#else
											bool passedGeometricCheck = ATORdatabaseFileIO.compareFeaturesListForMatch((testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList, (trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainZoomIndex][trainPolyIndex][trainSideIndex]).firstFeatureInFeatureList, dimension, &exactMatchFoundIrrelevant);
											#endif
											#endif
										#else
											string traininterpolatedNearestFeaturesMapFileName;
											traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
											ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainFirstFeatureInNearestFeatureList, false, false, false);
											bool passedGeometricCheck = ATORdatabaseFileIO.compareFeaturesListForMatch(testFirstFeatureInNearestFeatureList, trainFirstFeatureInNearestFeatureList, dimension, &exactMatchFoundIrrelevant);
										#endif
										#else
										bool passedGeometricCheck = true;
										#endif

										if(passedGeometricCheck)
										{

					#endif
										#ifndef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE_OLD_ADDITIONAL

											int64_t time5aNormalisedSnapshotComparisonLoadComparisonDataStart;
											if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "passedGeometricCheck" << endl;
													cout << "\t\t\t start: 5a. normalised snapshot comparison - load snapshot data for comparison" << endl;
												}
												time5aNormalisedSnapshotComparisonLoadComparisonDataStart = SHAREDvars.getTimeAsLong();
											}

											if(useGeneratedTestPixmapFiles)
											{
												if(ATOR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
												{
													if(!testMapLoaded)
													{//optimisation;

														testMapLoaded = true;

														string testviewIndexString = SHAREDvars.convertIntToString(testViewIndex+testViewNumber);
														string testzoomIndexString = SHAREDvars.convertIntToString(testZoomIndex);
														string testpolygonIndexString = SHAREDvars.convertIntToString(testPolyIndex);
														string testsideIndexString = SHAREDvars.convertIntToString(testSideIndex);

														string testinterpolatedRGBMapFileNameForRayTracing;
														testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

														string testrgbMapFacingPolyFileNameWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

														string testrgbMapFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

														RTpixelMaps.readImage(testrgbMapFacingPolyFileName, rgbMapTest);

														#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
														string testrgbMapSmallFacingPolyFileName = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;

														RTpixelMaps.readImage(testrgbMapSmallFacingPolyFileName, testrgbMapSmall);
														#endif
													}
												}
											}

											if(useGeneratedTrainPixmapFiles)
											{
												#ifdef ATOR_USE_DATABASE
												#ifdef ATOR_DATABASE_VERBOSE
												cout << "DBgenerateFolderName: trainObjectString = " << trainObjectString << endl;
												#endif
												ATORdatabaseFileIO.DBgenerateFolderName(&trainObjectString, true);
												#endif

												string trainrgbMapFacingPolyFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;

												string trainrgbMapFacingPolyFileName = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
												#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
												string trainrgbMapSmallFacingPolyFileName= traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
												#endif

												RTpixelMaps.readImage(trainrgbMapFacingPolyFileName, rgbMapTrain);

												#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
												RTpixelMaps.readImage(trainrgbMapSmallFacingPolyFileName, trainrgbMapSmall);
												#endif

												#ifdef ATOR_USE_DATABASE
												SHAREDvars.setCurrentDirectory(currentTempFolder);
												#endif
											}
											else
											{
												#ifdef ATOR_IMAGE_COMPARISON_SQL
												ATORdatabaseSQL.convertSQLdatabaseStringToSnapshotMaps(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTrain, trainrgbDevMap, traindepthMap, smallImageWidth, smallImageHeight, trainrgbMapSmall, traindepthMapSmall, trainrgbDevMapSmall, dimension, currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->snapshotMapsText, ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, ATOR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS);
												#else
												cerr << "ATORcomparisonClass::compareNormalisedSnapshots error: !useGeneratedTrainPixmapFiles requires ATOR_IMAGE_COMPARISON_SQL" << endl;
												exit(EXIT_ERROR);
												#endif
											}

											if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t end: 5a. normalised snapshot comparison - load snapshot data for comparison" << endl;
												}
												int64_t time5aNormalisedSnapshotComparisonLoadComparisonDataEnd;
												time5aNormalisedSnapshotComparisonLoadComparisonDataEnd = SHAREDvars.getTimeAsLong();
												time5aNormalisedSnapshotComparisonLoadComparisonDataTotal = time5aNormalisedSnapshotComparisonLoadComparisonDataTotal + time5aNormalisedSnapshotComparisonLoadComparisonDataEnd-time5aNormalisedSnapshotComparisonLoadComparisonDataStart;
												time5aNormalisedSnapshotComparisonLoadComparisonDataIndex ++;
												if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t time5aNormalisedSnapshotComparisonLoadComparisonData = " << time5aNormalisedSnapshotComparisonLoadComparisonDataEnd-time5aNormalisedSnapshotComparisonLoadComparisonDataStart << endl;
												}
											}

											int64_t time5bNormalisedSnapshotComparisonCompareSnapshotDataStart;
											if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t start: 5b. normalised snapshot comparison - compare snapshot data" << endl;
												}
												time5bNormalisedSnapshotComparisonCompareSnapshotDataStart = SHAREDvars.getTimeAsLong();
											}


											/*
											if(ATOR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS)
											{//optimised, rgb dev maps have already been created - use these to perform the comparsion

											}
											else
											{
											*/



											#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON

											int resampledWidth = (imageWidthFacingPoly/smallImageRatio);
											int resampledHeight = (imageHeightFacingPoly/smallImageRatio);


											#ifdef ATOR_METHOD_SMALL_IMAGE_FIRST_COMPARISON_NO_KERNEL
												double errorSmall = ATORimagecomparison.compareImagesRGBsmallNoKernel(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);			//cannot use this function until it has been tested
											#else
												#ifndef ATOR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
													double errorSmall = ATORimagecomparison.compareImagesRGBwithPosDev(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);
												#else
													double errorSmall = ATORimagecomparison.compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);			//cannot use this function until it has been tested
												#endif
											#endif

											if(errorSmall < ATOR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD)
											{
											#endif


												#ifndef ATOR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
													double error = ATORimagecomparison.compareImagesRGBwithPosDev(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, rgbMapTrain);
												#else
													double error = ATORimagecomparison.compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, rgbMapTrain);			//cannot use this function until it has been tested
													//double error = ATORimagecomparison.compareImagesRGBwithPosDev(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest, rgbMapTrain);
												#endif


												if(error < currentLowestError)
												{

													#ifndef ATOR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
													bool passedContrastCheck = false;
													if(doNotCheckContrast)
													{
														passedContrastCheck = true;
													}
													else if(ATORimagecomparison.checkImageHasContrast(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTrain))
													{
														passedContrastCheck = true;
													}
													if(passedContrastCheck)
													{
													#endif

														currentLowestError = error;


														#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
														trainObjectNameWithLowestErrorRecordAcrossAllSides = trainObjectString;
														#ifndef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
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


														#ifndef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
														foundMatchRecord[testSideIndex] = true;
														currentLowestErrorRecord[testSideIndex] = currentLowestError;
														#endif


													#ifndef ATOR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
													}
													#endif
												}
												else
												{
												}

											#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
											}
											else
											{
											}
											#endif

											if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS)
											{
												if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t end: 5b. normalised snapshot comparison - compare snapshot data" << endl;
												}
												int64_t time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd;
												time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd = SHAREDvars.getTimeAsLong();
												time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal = time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal + time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd-time5bNormalisedSnapshotComparisonCompareSnapshotDataStart;
												time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex++;
												if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
												{
													cout << "\t\t\t time5bNormalisedSnapshotComparisonCompareSnapshotData = " << time5bNormalisedSnapshotComparisonCompareSnapshotDataEnd-time5bNormalisedSnapshotComparisonCompareSnapshotDataStart << endl;
												}
											}

										#endif
					#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON_BINNING

											}

											currentFeatureContainerInTestFeatureMatchingTrainBin=currentFeatureContainerInTestFeatureMatchingTrainBin->next;
											#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
											currentFeatureContainerInTestFeatureMatchingTrainBin2=currentFeatureContainerInTestFeatureMatchingTrainBin2->next;
											#endif
										}

										#ifndef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
										delete firstFeatureContainerInTestFeatureMatchingTrainBin;
										#endif
										#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
											if(exceptionIndex != ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)
											{
												testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2] = testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2]-exceptionModifier;
											}
											exceptionModifier = exceptionModifier* -1;
										}
										#endif
										#ifndef ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NO_EXPLICIT_FOR_LOOPS
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

						if(!ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL && !ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							delete currentTestFeature;
						}

					#else
						#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL

											}

										currentFeatureContainerInList = currentFeatureContainerInList->next;

										#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_ALLOW_LIGHTING_COLOUR_SATURATION_VARIATION
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
										}
									}
								}
							}
						}
					}
						#endif
					#endif

						#ifndef ATOR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
						if(!doNotCheckContrast)
						{
							if(!ATORimagecomparison.checkImageHasContrast(imageWidthFacingPoly, imageHeightFacingPoly, rgbMapTest))
							{
								currentLowestError = ATOR_IMAGE_COMPARISON_NO_CONTRAST_ERROR_KEY;
							}
						}
						#endif


						averageMatchError = averageMatchError + currentLowestError;
						averageMatchErrorAcrossSides = averageMatchErrorAcrossSides + currentLowestError;
					}

					averageMatchErrorAcrossSides = (averageMatchErrorAcrossSides/ATOR_METHOD_POLYGON_NUMBER_OF_SIDES);

				#ifndef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					#ifdef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
					if(averageMatchErrorAcrossSides < ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
					{
						string averageMatchErrorAcrossSidesString = SHAREDvars.convertDoubleToString(averageMatchErrorAcrossSides, "%0.3f");


						if(((trainPolyIndexWithLowestErrorRecord[0][2] == trainPolyIndexWithLowestErrorRecord[1][2]) && (trainPolyIndexWithLowestErrorRecord[0][2] == trainPolyIndexWithLowestErrorRecord[2][2])) && ((trainPolyIndexWithLowestErrorRecord[0][3] == trainPolyIndexWithLowestErrorRecord[1][3]) && (trainPolyIndexWithLowestErrorRecord[0][3] == trainPolyIndexWithLowestErrorRecord[2][3])))
						{//all sides are referring to the same poly, and all sides are referring to the same zoom

					#endif
							#ifdef ATOR_VERBOSE_COMPARISON_ALLOW_SAME_OBJECT_AND_SAME_VIEW_TO_BE_COMPARED
							if(1)
							#elif defined ATOR_VERBOSE_COMPARISON_ALLOW_SAME_OBJECT_BUT_DIFFERENT_VIEWS_TO_BE_COMPARED
							if(!((trainObjectNameWithLowestErrorRecordAcrossAllSides == testObjectNameArray[testObjectIndex]) && (trainPolyIndexWithLowestErrorRecord[0][1] == testViewIndex+testViewNumber)))
							#else
							if(trainObjectNameWithLowestErrorRecordAcrossAllSides != testObjectNameArray[testObjectIndex])
							#endif
							{//object name and object view are not both the same

								//a real image comparitor match has been declared found


								numPassesForTestViewIndex++;

								if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
								{
									cout << "the following best match was determined for test data with train data:" << endl;
									cout << "testObjectIndex = " << testObjectIndex << endl;
									cout << "testViewIndex = " << testViewIndex+testViewNumber << endl;
									cout << "testZoomIndex = " << testZoomIndex << endl;
									cout << "testPolyIndex = " << testPolyIndex << endl;
									cout << "testSideIndex = 0, 1, 2" << endl;
									#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
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

								#ifdef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
								//see if object exists, and if it does, increment number of successful snapshots
								ATORobjectReferenceList* currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
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
									currentReferenceInSnapshotMatchObjectReferenceList->objectName = trainObjectNameWithLowestErrorRecordAcrossAllSides;
									currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = 1;
									ATORobjectReferenceList* newObjectReference = new ATORobjectReferenceList();
									currentReferenceInSnapshotMatchObjectReferenceList->next = newObjectReference;
								}
								#endif

								for(int s=0; s<ATOR_METHOD_POLYGON_NUMBER_OF_SIDES; s++)
								{
									#ifndef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
									if(foundMatchRecord[s])
									{
									if(currentLowestErrorRecord[s] < ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
									{
									#endif

									#ifndef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
									//see if object exists, and if it does, increment number of successful snapshots
									ATORobjectReferenceList* currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
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
										currentReferenceInSnapshotMatchObjectReferenceList->objectName = trainObjectNameWithLowestErrorRecord[s];
										currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = 1;
										ATORobjectReferenceList* newObjectReference = new ATORobjectReferenceList();
										currentReferenceInSnapshotMatchObjectReferenceList->next = newObjectReference;
									}
									#endif

									if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
									{
										#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											string trainobjectIndexString = trainObjectNameWithLowestErrorRecordAcrossAllSides;
										#else
											string trainobjectIndexString = SHAREDvars.convertIntToString(trainPolyIndexWithLowestErrorRecord[s][0]);
										#endif

										string trainviewIndexString = SHAREDvars.convertIntToString(trainPolyIndexWithLowestErrorRecord[s][1]);
										string trainzoomIndexString = SHAREDvars.convertIntToString(trainPolyIndexWithLowestErrorRecord[s][2]);
										string trainpolygonIndexString = SHAREDvars.convertIntToString(trainPolyIndexWithLowestErrorRecord[s][3]);
										string trainsideIndexString = SHAREDvars.convertIntToString(trainPolyIndexWithLowestErrorRecord[s][4]);

										string testviewIndexString = SHAREDvars.convertIntToString(testViewIndex+testViewNumber);
										string testzoomIndexString = SHAREDvars.convertIntToString(testZoomIndex);
										string testpolygonIndexString = SHAREDvars.convertIntToString(testPolyIndex);
										string testsideIndexString = SHAREDvars.convertIntToString(s);

										string blank = "";

										string traininterpolatedRGBMapFileNameForRayTracing;
										string testinterpolatedRGBMapFileNameForRayTracing;

									#ifdef TEMPTEST3GEOACCURACY	//doesn't work with ATOR_USE_DATABASE
										cout << "TEMPTEST3GEOACCURACY Part 3" << endl;

										#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES

											traininterpolatedRGBMapFileNameForRayTracing = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#else

											traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#endif
										string traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
										ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainFirstFeatureInNearestFeatureList, false, false, false);

										testinterpolatedRGBMapFileNameForRayTracing = objectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
										string testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
										ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);

										cout << "\n new match found;" << endl;
										cout << "traininterpolatedNearestFeaturesMapFileName = " << traininterpolatedNearestFeaturesMapFileName << endl;
										ATORfeature* currentFeatureInList = trainFirstFeatureInNearestFeatureList;
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
										bool passedGeometricCheck = ATORdatabaseFileIO.compareFeaturesListForMatch(testFirstFeatureInNearestFeatureList, trainFirstFeatureInNearestFeatureList, dimension, &exactMatchFoundIrrelevant);
										cout << "passedGeometricCheck = " << passedGeometricCheck << endl;
									#endif

										#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											traininterpolatedRGBMapFileNameForRayTracing = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#else
											traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#endif
										string trainrgbMapFacingPolyFileNameWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;

										string traininterpolatedRGBMapFileNameForRayTracingSanitised;
										#ifdef ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
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
										if(!ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
										{
											generateTrainPixmapFiles = true;
											generateTestPixmapFiles = true;
										}
										else
										{
											if(!ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
											{
												generateTestPixmapFiles = true;
											}
										}



										string trainImgSrcHtmlTags = "";
										string testImgSrcHtmlTags = "";
										string imageWidthFacingPolyString = SHAREDvars.convertIntToString(imageWidthFacingPoly);	//increase image size for small images - for better visualisation [rather than zoom in browser]
										string imageHeightFacingPolyString = SHAREDvars.convertIntToString(imageHeightFacingPoly);	//increase image size for small images - for better visualisation [rather than zoom in browser]
										if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
										{
											convertImageFileType(&trainrgbMapFacingPolyFileNameWithoutExt, &trainrgbMapFacingPolyFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											convertImageFileType(&testrgbMapFacingPolyFileNameWithoutExt, &testrgbMapFacingPolyFileNameWithoutExt, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + trainrgbMapFacingPolyFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + testrgbMapFacingPolyFileNameWithoutExt + PNG_EXTENSION + "\" border=0>";



											#ifdef ATOR_VERBOSE_OUTPUT_DCT_TABLES
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
											schar testconcatonatedSignedDctCoeffArrayRequirementTemp[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
											schar trainconcatonatedSignedDctCoeffArrayRequirementTemp[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
											LDjpeg.setPointerToDCTtableHTMLoutputString(&testDCTTableHTMLOutputString);
											readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(&testrgbMapSmallFacingPolyFileNamePPMTemp, &testrgbMapSmallFacingPolyFileNameJPEGTemp, testconcatonatedSignedDctCoeffArrayRequirementTemp, true);
											LDjpeg.setPointerToDCTtableHTMLoutputString(&trainDCTTableHTMLOutputString);
											readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(&trainrgbMapSmallFacingPolyFileNamePPMTemp, &trainrgbMapSmallFacingPolyFileNameJPEGTemp, trainconcatonatedSignedDctCoeffArrayRequirementTemp, true);
											//next code extracted from convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue (determinations of arrayValueUnsigned)
											for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
											{
												int arrayValueSigned = testconcatonatedSignedDctCoeffArrayRequirementTemp[i];
												testDCTBinnedPrelimTableHTMLOutputString = testDCTBinnedPrelimTableHTMLOutputString + "<TD>" + SHAREDvars.convertIntToString(arrayValueSigned) + "</TD>";

												double arrayValueUnsignedDouble;
												int arrayValueUnsigned = ATORdatabaseDecisionTreeOperations.determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);
												testDCTBinnedTableHTMLOutputString = testDCTBinnedTableHTMLOutputString + "<TD>" + SHAREDvars.convertUnsignedIntToString(arrayValueUnsigned) + "</TD>";
											}
											for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
											{
												int arrayValueSigned = trainconcatonatedSignedDctCoeffArrayRequirementTemp[i];
												trainDCTBinnedPrelimTableHTMLOutputString = trainDCTBinnedPrelimTableHTMLOutputString + "<TD>" + SHAREDvars.convertIntToString(arrayValueSigned) + "</TD>";

												double arrayValueUnsignedDouble;
												int arrayValueUnsigned = ATORdatabaseDecisionTreeOperations.determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);
												trainDCTBinnedTableHTMLOutputString = trainDCTBinnedTableHTMLOutputString + "<TD>" + SHAREDvars.convertUnsignedIntToString(arrayValueUnsigned) + "</TD>";
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


											#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES
											string traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
											ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainFirstFeatureInNearestFeatureList, false, false, false);
											string testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
											ATORdatabaseFileIO.createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testFirstFeatureInNearestFeatureList, false, false, false);
											bool exactMatchFound;
											bool passedGeometricCheck = ATORdatabaseFileIO.compareFeaturesListForMatch(testFirstFeatureInNearestFeatureList, trainFirstFeatureInNearestFeatureList, dimension, &exactMatchFound);	//required to set matchFound
											string trainGeoTableHTMLOutputString = "";
											string testGeoTableHTMLOutputString = "";
											string trainGeoBinnedTableHTMLOutputString = "";
											string testGeoBinnedTableHTMLOutputString = "";
											createGeoTableHTMLfromFeatureList(trainFirstFeatureInNearestFeatureList, false, &trainGeoTableHTMLOutputString);
											createGeoTableHTMLfromFeatureList(testFirstFeatureInNearestFeatureList, false, &testGeoTableHTMLOutputString);
											createGeoTableHTMLfromFeatureList(trainFirstFeatureInNearestFeatureList, true, &trainGeoBinnedTableHTMLOutputString);
											createGeoTableHTMLfromFeatureList(testFirstFeatureInNearestFeatureList, true, &testGeoBinnedTableHTMLOutputString);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<P>Nearest Features (transformed):<BR />" + trainGeoTableHTMLOutputString;
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<P>Nearest Features (transformed):<BR />" + testGeoTableHTMLOutputString;
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + trainGeoBinnedTableHTMLOutputString + "</P>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + testGeoBinnedTableHTMLOutputString + "</P>";
											#endif

										}
										else
										{
											convertImageFileType(&trainrgbMapFacingPolyFileNameWithoutExt, &trainrgbMapFacingPolyFileNameWithoutExtSanitised, PPM_EXTENSION, PNG_EXTENSION);
											convertImageFileType(&testrgbMapFacingPolyFileNameWithoutExt, &testrgbMapFacingPolyFileNameWithoutExtSanitised, PPM_EXTENSION, PNG_EXTENSION);
											trainImgSrcHtmlTags = trainImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + trainrgbMapFacingPolyFileNameWithoutExtSanitised + PNG_EXTENSION + "\" border=0>";
											testImgSrcHtmlTags = testImgSrcHtmlTags + "<img height=" + imageWidthFacingPolyString + " src=\"" + testrgbMapFacingPolyFileNameWithoutExtSanitised + PNG_EXTENSION + "\" border=0>";
										}

										
										#ifdef VERBOSE_ATOR_IMAGE_COMPARISON_LIST_MATCHED_FILE_NAMES
										string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testrgbMapFacingPolyFileNameWithoutExt + PPM_EXTENSION + "</TD><TD>" + testImgSrcHtmlTags + "</TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainrgbMapFacingPolyFileNameWithoutExt + PPM_EXTENSION + "</TD><TD>" + trainImgSrcHtmlTags + "</TD>";
										#else
										string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testsideIndexString + "</TD><TD>" + testImgSrcHtmlTags + "</TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainsideIndexString + "</TD><TD>" + trainImgSrcHtmlTags + "</TD>";
										#endif

										#ifdef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
										ICRmatchRow = ICRmatchRow + "<TD>" + averageMatchErrorAcrossSidesString + "</TD></TR>";
										#else
										string currentLowestErrorString = SHAREDvars.convertDoubleToString(currentLowestErrorRecord[s], "%0.3f");
										ICRmatchRow = ICRmatchRow + "<TD>" + currentLowestErrorString + "</TD></TR>";
										#endif

										SHAREDvars.writeStringToFileObject(ICRmatchRow, &writeFileObject);
									}
									#ifndef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
									}
									}
									#endif
								}
							}
						#ifdef ATOR_ASSERT_MATCHES_FOR_ALL_SIDES
						}

					}
					#endif
				#endif

				}
				averageMatchError = averageMatchError/(numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]* ATOR_METHOD_POLYGON_NUMBER_OF_SIDES);
				if(ATOR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "average error = " << averageMatchError << endl;
				}

				averageMatchErrorAcrossAllViewIndicies = averageMatchErrorAcrossAllViewIndicies + averageMatchError;

				if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS)
				{
					#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					cout << "\t\t\t end: 5z. normalised snapshot comparison - single view speed test (for website)" << endl;
					#endif
					int64_t time5zNormalisedSnapshotComparisonSingleViewTestEnd;
					time5zNormalisedSnapshotComparisonSingleViewTestEnd = SHAREDvars.getTimeAsLong();
					int64_t time5zNormalisedSnapshotComparisonSingleViewTestTotal = time5zNormalisedSnapshotComparisonSingleViewTestEnd-time5zNormalisedSnapshotComparisonSingleViewTestStart;
					//time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex++;
					#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					cout << "\t\t\t time5zNormalisedSnapshotComparisonSingleViewTest = " << (double)(time5zNormalisedSnapshotComparisonSingleViewTestEnd-time5zNormalisedSnapshotComparisonSingleViewTestStart)/CLOCKS_PER_SEC << endl;
					#endif
				}

				if(ATOR_PRINT_ALGORITHM_PROGRESS)
				{
					cout << "testObject = " << testObjectNameArray[testObjectIndex] << endl;
					cout << "numPassesForTestViewIndex " << testViewIndex+testViewNumber << " = " << numPassesForTestViewIndex << endl;
				}
			}
		}

		averageMatchErrorAcrossAllViewIndicies = averageMatchErrorAcrossAllViewIndicies/numberOfTestViewIndicies;
		if(ATOR_PRINT_ALGORITHM_PROGRESS)
		{
			cout << "average error across all view indicies = " << averageMatchErrorAcrossAllViewIndicies << endl;
		}

		averageMatchErrorAcrossAllObjects = averageMatchErrorAcrossAllObjects + averageMatchErrorAcrossAllViewIndicies;
	}

	averageMatchErrorAcrossAllObjects = averageMatchErrorAcrossAllObjects/numberOfTestObjects;

	if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{
		string ICRtableEnd = "</TABLE>";
		SHAREDvars.writeStringToFileObject(ICRtableEnd, &writeFileObject);

		string ICRfooter  = "</BODY></HTML>";
	}
	writeFileObject.close();

	delete rgbMapTrain;
	delete rgbMapTest;
	//#ifdef ATOR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	delete traindepthMap;
	delete testdepthMap;
	//#endif
	#ifdef ATOR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
	delete trainrgbMapSmall;
	delete testrgbMapSmall;
	//#ifdef ATOR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	delete traindepthMapSmall;
	delete testdepthMapSmall;
	//#endif
	#endif

	/*
	//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	delete testdctCoeffArrayY;
	delete testdctCoeffArrayYCr;
	delete testdctCoeffArrayYCb;
	delete testconcatonatedSignedDctCoeffArrayRequirement;
	//#endif
	*/

	//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	delete trainrgbDevMap;
	delete testrgbDevMap;
	//#endif
	//#ifdef ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	delete trainrgbDevMapSmall;
	delete testrgbDevMapSmall;
	delete trainrgbDev8BitSmallMap;
	delete testrgbDev8BitSmallMap;
	//#endif

	ATORobjectReferenceList* currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
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

	if(ATOR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "\n\npasses = \n\n" << passes << endl;
	}

	return averageMatchErrorAcrossAllObjects;
}



void ATORcomparisonClass::convertImageFileType(const string* imageBaseFileName, const string* imageBaseFileNameConverted, const string imageExtension, const string imageExtensionConverted)
{
	string convertPPMtoPNGCommand = "convert " +* imageBaseFileName + imageExtension + " " +* imageBaseFileNameConverted + imageExtensionConverted;
	if(ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
	{
		cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
	}
	system(convertPPMtoPNGCommand.c_str());
}





#define VERY_HIGH_ERROR (99999999999999.0)









//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
void ATORcomparisonClass::convertNormalisedHueDeviationMapTo3x8bitMap(int imageWidth, const int imageHeight, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, uchar* rgbDev8BitSmallMapFacingPoly)
{
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			vec normalisedHueContrast;
			RTpixelMaps.getVectorMapValue(x, y, imageWidth, rgbDevIEnormalisedHueContrastMapSmallFacingPoly, &normalisedHueContrast);

			colour culledNormalisedHueContrast;
			cullAndBinNormalisedHueContrast(&normalisedHueContrast, &culledNormalisedHueContrast);

			RTpixelMaps.setRGBMapValues(x, y, imageWidth, &culledNormalisedHueContrast, rgbDev8BitSmallMapFacingPoly);
		}
	}
}



void ATORcomparisonClass::cullAndBinNormalisedHueContrast(vec* normalisedHueContrast, colour* culledNormalisedHueContrast)
{
	if(normalisedHueContrast->x*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		culledNormalisedHueContrast->r = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		culledNormalisedHueContrast->r = normalisedHueContrast->x*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
	if(normalisedHueContrast->y*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		culledNormalisedHueContrast->g = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		culledNormalisedHueContrast->g = normalisedHueContrast->y*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
	if(normalisedHueContrast->z*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		culledNormalisedHueContrast->b = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		culledNormalisedHueContrast->b = (normalisedHueContrast->z)*ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
}
//#endif







//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING

void ATORcomparisonClass::convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(schar dctCoeffArrayY[], schar dctCoeffArrayYcr[], schar dctCoeffArrayYcb[], schar concatonatedSignedDctCoeffArray[])
{
	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y; i++)
	{
		int xIndex = dctCoeffSelectionArrayY[i][0];
		int yIndex = dctCoeffSelectionArrayY[i][1];

		concatonatedSignedDctCoeffArray[i] = dctCoeffArrayY[yIndex*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
	}
	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr; i++)
	{
		int xIndex = dctCoeffSelectionArrayYCr[i][0];
		int yIndex = dctCoeffSelectionArrayYCr[i][1];
		concatonatedSignedDctCoeffArray[i+ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y] = dctCoeffArrayYcr[yIndex*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
	}
	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb; i++)
	{
		int xIndex = dctCoeffSelectionArrayYCb[i][0];
		int yIndex = dctCoeffSelectionArrayYCb[i][1];

		concatonatedSignedDctCoeffArray[i+ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y+ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr] = dctCoeffArrayYcb[yIndex*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
	}

}

#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES
static char* stringFormatDouble = "%0.6f";
static char* stringFormatDec = "%d";

void ATORcomparisonClass::createGeoTableHTMLfromFeatureList(const ATORfeature* firstFeatureInNearestFeatureList, const bool applyBinning, string* geoTableHTMLoutputString)
{
	*geoTableHTMLoutputString = *geoTableHTMLoutputString + "<TABLE>";

	const ATORfeature* currentFeatureInList = firstFeatureInNearestFeatureList;
	while((currentFeatureInList->next != NULL) && !(currentFeatureInList->lastFilledFeatureInList))
	{
		if(currentFeatureInList->matchFound)
		{
			string coordinateXString = "";
			string coordinateYString = "";
			string coordinateZString = "";
			if(applyBinning)
			{
				int xBin = ATORdatabaseDecisionTreeOperations.determineGeoBinX(currentFeatureInList->pointTransformed.x);
				int yBin = ATORdatabaseDecisionTreeOperations.determineGeoBinY(currentFeatureInList->pointTransformed.y);
				int zBin = 0;
				coordinateXString = SHAREDvars.convertIntToString(xBin);
				coordinateYString = SHAREDvars.convertIntToString(yBin);
				coordinateZString = SHAREDvars.convertIntToString(zBin);
			}
			else
			{
				coordinateXString = SHAREDvars.convertDoubleToString(currentFeatureInList->pointTransformed.x, "%0.6f");
				coordinateYString = SHAREDvars.convertDoubleToString(currentFeatureInList->pointTransformed.y, "%0.6f");
				coordinateZString = SHAREDvars.convertDoubleToString(currentFeatureInList->pointTransformed.z, "%0.6f");
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
bool determineIfGeoBinningIdenticalMatchFound(ATORfeature* firstFeatureInNearestFeatureList, int pBinxRequirement[], int pBinyRequirement[])
{
	bool geoBinningIsSameAsOriginal = false;

	ATORfeature* currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList;
	while((currentFeatureInNearestFeatureList->next != NULL) && !(currentFeatureInNearestFeatureList->lastFilledFeatureInList))
	{
		ATORfeature* currentFeatureInNearestFeatureList2 = firstFeatureInNearestFeatureList;
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

void ATORcomparisonClass::readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(const string* rgbMapSmallFacingPolyFileNamePPM, const string* rgbMapSmallFacingPolyFileNameJPEG, schar* concatonatedSignedDctCoeffArrayRequirement, const bool printOutput)
{

	int dctCoeffArrayHeight = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
	int dctCoeffArrayWidth = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;

	schar dctCoeffArrayY[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar dctCoeffArrayYcr[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar dctCoeffArrayYcb[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar dctCoeffArrayYDummy[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar dctCoeffArrayYCrDummy[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar dctCoeffArrayYCbDummy[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];

	string convertSmallImageRGBtoJPEGCommand = "";

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING)
	{
		convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING + " " + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " +* rgbMapSmallFacingPolyFileNamePPM + " " +* rgbMapSmallFacingPolyFileNameJPEG;
		system(convertSmallImageRGBtoJPEGCommand.c_str());
		LDjpeg.readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(*rgbMapSmallFacingPolyFileNameJPEG, dctCoeffArrayY, dctCoeffArrayYCrDummy, dctCoeffArrayYCbDummy, dctCoeffArrayHeight, dctCoeffArrayWidth, printOutput);
		convertSmallImageRGBtoJPEGCommand = "";
		convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING + " " + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " +* rgbMapSmallFacingPolyFileNamePPM + " " +* rgbMapSmallFacingPolyFileNameJPEG;
		system(convertSmallImageRGBtoJPEGCommand.c_str());
		LDjpeg.readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(*rgbMapSmallFacingPolyFileNameJPEG, dctCoeffArrayYDummy, dctCoeffArrayYcr, dctCoeffArrayYcb, dctCoeffArrayHeight, dctCoeffArrayWidth, printOutput);
	}
	else
	{
		convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING + " " + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " +* rgbMapSmallFacingPolyFileNamePPM + " " +* rgbMapSmallFacingPolyFileNameJPEG;
		system(convertSmallImageRGBtoJPEGCommand.c_str());
		LDjpeg.readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(*rgbMapSmallFacingPolyFileNameJPEG, dctCoeffArrayY, dctCoeffArrayYcr, dctCoeffArrayYcb, dctCoeffArrayHeight, dctCoeffArrayWidth, printOutput);
	}

	//int64_t binnedAllDCTCoeff64BitValue = convertDCTCoeffIndividualArraysToBinnedAllDCTCoeff64BitValue(dctCoeffArrayY, dctCoeffArrayYcr, dctCoeffArrayYcb, concatonatedSignedDctCoeffArrayRequirement);	//may not be used here
	convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(dctCoeffArrayY, dctCoeffArrayYcr, dctCoeffArrayYcb, concatonatedSignedDctCoeffArrayRequirement);

}

//#endif
