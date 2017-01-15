/*******************************************************************************
 *
 * File Name: ORcomparison.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a6b 30-Apr-2012
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

#include "LDjpeg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

//#include <mysql.h>
using namespace std;


#ifndef LINUX
	#define OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
#endif



int passes;


#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
static FeatureContainer trainfeatureGeoCompBinArray[OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS][2];	//trainfeatureGeoCompBinArray now stores 2 feature combinations (instead of 1 feature)
#endif
#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
static FeatureContainer trainfeatureImageAverageColourCompBinArray[OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL][OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL][OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL];
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
void filldctCoeffSelectionArrays()
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
//FeatureContainer testfeatureGeoCompArray[numberOfTestObjects][numberOfTestViewIndicies][numberOfTestZoomIndicies][maxNumberOfTestPolysAcrossAllObjects][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
#ifndef LINUX
#ifdef OR_METHOD_2DOD
	FeatureContainer testfeatureGeoCompArray[OR_METHOD2DOD_NUM_OF_OBJECTS][OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST][OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
#elif defined OR_METHOD_3DOD
	FeatureContainer testfeatureGeoCompArray[OR_METHOD3DOD_NUM_OF_OBJECTS][OR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST][OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
#endif
#endif
*/

#ifdef OR_IMAGE_COMPARISON_SQL
double compareNormalisedSnapshots(int numberOfTestPolys[], int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, string testObjectNameArray[], int numberOfTestObjects, int dimension, int numberOfTestZoomIndicies, int trainOrTest, int testViewNumber)
#else
double compareNormalisedSnapshots(int numberOfTrainPolys[], int numberOfTestPolys[], int numberOfTrainViewIndicies, int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, string trainObjectNameArray[], int numberOfTrainObjects, string testObjectNameArray[], int numberOfTestObjects, int dimension, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies, int testViewNumber)
#endif
{

	ObjectReferenceList * firstReferenceInSnapshotMatchObjectReferenceList = new ObjectReferenceList();
	
	bool useGeneratedTestPixmapFiles = false;
	bool useGeneratedTrainPixmapFiles = false;
	bool OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = false;

	#ifdef OR_METHOD_GEOMETRIC_COMPARISON
		if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
		{
			if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
			{
				//cout << "\n\n\n\n\n\n\n OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE \n\n\n\n\n\n\n" << endl;
			}
			else
			{
				//cout << "\n\n\n\n\n\n\n !OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE \n\n\n\n\n\n\n" << endl;
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
	if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{

		string ICRheader = "";
		if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS)
		{
			ICRheader = ICRheader + "<HTML><HEAD><TITLE>Results </TITLE></HEAD><BODY>Results<p>Source Version 2d2g<p>";
		}
		else
		{
			ICRheader = ICRheader + "<HTML><HEAD><TITLE>Image Comparitor Results For Normalised Snapshots (CONFIDENTIAL - PROVIDED UNDER CONTRACT)</TITLE></HEAD><BODY><h1>Image Comparitor Results For Normalised Snapshots (CONFIDENTIAL - PROVIDED UNDER CONTRACT)</h1><p>Source Version 2d2g<p>Richard Bruce Baxter Object Recognition Method - Australian Provisional Patent Filed";
		}


		writeStringToFileObject2(ICRheader, &writeFileObject);

		string ICRtableStart = "";
		if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
		{
			ICRtableStart = ICRtableStart + "<TABLE width=\"100%\" cellpadding=\"10\" cellspacing=\"0\" border=\"1\"><TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>Test</TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>Train</TD></TR><TR><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>PolygonBAI</TD><TD>Side</TD><TD>Image</TD><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>PolygonBAI</TD><TD>Side</TD><TD>Image</TD></TR>";
		}
		else
		{
			ICRtableStart = ICRtableStart + "<TABLE width=\"100%\" cellpadding=\"10\" cellspacing=\"0\" border=\"1\"><TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>Test</TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>Train</TD></TR><TR><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>X</TD><TD>Y</TD><TD>Image</TD><TD>Object</TD><TD>View</TD><TD>Zoom</TD><TD>X</TD><TD>Y</TD><TD>Image</TD></TR>";
		}
		writeStringToFileObject2(ICRtableStart, &writeFileObject);

	}


	//optimisation;
	unsigned char * trainrgbMap = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	unsigned char * testrgbMap = new unsigned char[imageWidthFacingPoly*imageHeightFacingPoly*RGB_NUM];
	//#ifdef OR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	double * traindepthMap = new double[imageWidthFacingPoly*imageHeightFacingPoly];
	double * testdepthMap = new double[imageWidthFacingPoly*imageHeightFacingPoly];
	//#endif

	int smallImageWidth = imageWidthFacingPoly/smallImageRatio;
	int smallImageHeight = imageHeightFacingPoly/smallImageRatio;
	
#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
	unsigned char * trainrgbMapSmall = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	unsigned char * testrgbMapSmall = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	//#ifdef OR_METHOD3DOD_IMAGE_COMPARISON_COMPARE_DEPTH_MAPS
	double * traindepthMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)];
	double * testdepthMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)];
	//#endif
#endif
//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	signed char * testdctCoeffArrayY = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * testdctCoeffArrayYCr = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * testdctCoeffArrayYCb = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * testdctCoeffArrayYDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * testdctCoeffArrayYCrDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * testdctCoeffArrayYCbDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];

	signed char * testconcatonatedSignedDctCoeffArrayRequirement = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
//#endif

//#ifdef OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL
	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	double * trainrgbDevMap = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];	
	double * testrgbDevMap = new double[imageWidthFacingPoly*imageHeightFacingPoly*VEC_MAP_VEC_NUM_DIMENSIONS];
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON
	double * trainrgbDevMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*VEC_MAP_VEC_NUM_DIMENSIONS];
	double * testrgbDevMapSmall = new double[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*VEC_MAP_VEC_NUM_DIMENSIONS];
	unsigned char * trainrgbDev8BitSmallMap = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	unsigned char * testrgbDev8BitSmallMap = new unsigned char[(imageWidthFacingPoly/smallImageRatio)*(imageHeightFacingPoly/smallImageRatio)*RGB_NUM];
	//#endif
//#endif
/*
#ifdef OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE
char testsnapshotMapsText[OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_MAX_DATA_LENGTH];
char trainsnapshotMapsText[OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_MAX_DATA_LENGTH];
#else
char * testsnapshotMapsText;
char * trainsnapshotMapsText;
#endif
*/

#ifdef TEMPTEST3GEOACCURACY
	int numberOfNearestFeatures = calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);
	if(OR_PRINT_ALGORITHM_PROGRESS)
	{
		cout << "numberOfPolygonsPerFeature = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
		cout << "numberOfAdditionalFeatures = " << OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
		cout << "numberOfNearestFeatures = " << numberOfNearestFeatures << endl;
	}
	
	Feature * testfirstFeatureInNearestFeatureList = new Feature();
	Feature * trainfirstFeatureInNearestFeatureList = new Feature();

	generateNearestFeaturesList(testfirstFeatureInNearestFeatureList, numberOfNearestFeatures);
	generateNearestFeaturesList(trainfirstFeatureInNearestFeatureList, numberOfNearestFeatures);

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
	/*
	cout << "numberOfPolygonsPerFeature = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
	cout << "numberOfAdditionalFeatures = " << OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
	cout << "numberOfNearestFeatures = " << numberOfNearestFeatures << endl;
	*/
	
	Feature * testfirstFeatureInNearestFeatureList = new Feature();
	Feature * trainfirstFeatureInNearestFeatureList = new Feature();

	generateNearestFeaturesList(testfirstFeatureInNearestFeatureList, numberOfNearestFeatures);
	generateNearestFeaturesList(trainfirstFeatureInNearestFeatureList, numberOfNearestFeatures);

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
		FeatureContainer testfeatureGeoCompArray[numberOfTestObjects][numberOfTestViewIndicies][numberOfTestZoomIndicies][maxNumberOfTestPolysAcrossAllObjects][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
	#else
		/*
		array<FeatureContainer ^> ^ testfeatureGeoCompArray = new array<FeatureContainer ^>(numberOfTestObjects*numberOfTestViewIndicies*numberOfTestZoomIndicies*maxNumberOfTestPolysAcrossAllObjects*OR_METHOD_POLYGON_NUMBER_OF_SIDES);
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
							
							char testpolygonIndexString[10];
							char testsideIndexString[10];
							char testviewIndexString[10];
							char testzoomIndexString[10];

							sprintf(testpolygonIndexString, "%d", testPolyIndex);
							sprintf(testsideIndexString, "%d", testSideIndex);
							sprintf(testviewIndexString, "%d", testViewIndex+testViewNumber);
							sprintf(testzoomIndexString, "%d", testZoomIndex);

							string testinterpolatedRGBMapFileNameForRayTracing;
							testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

							string testinterpolatedNearestFeaturesMapFileName;
							testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;

							//generateNearestFeaturesList(&(testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]), numberOfNearestFeatures);

							Feature * firstFeatureInList = new Feature();

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
		FeatureContainer trainfeatureGeoCompArray[numberOfTrainObjects][numberOfTrainViewIndicies][numberOfTrainZoomIndicies][maxNumberOfTrainPolysAcrossAllObjects][OR_METHOD_POLYGON_NUMBER_OF_SIDES];
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

							char trainzoomIndexString[10];
							char trainviewIndexString[10];
							char trainpolygonIndexString[10];
							char trainsideIndexString[10];


							sprintf(trainpolygonIndexString, "%d", trainPolyIndex);
							sprintf(trainsideIndexString, "%d", trainSideIndex);
							sprintf(trainviewIndexString, "%d", trainViewIndex);
							sprintf(trainzoomIndexString, "%d", trainZoomIndex);

							string traininterpolatedRGBMapFileNameForRayTracing;
							traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainObjectIndex] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

							string traininterpolatedNearestFeaturesMapFileName;
							traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
							//generateNearestFeaturesList(&(trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainPolyIndex][trainSideIndex]), numberOfNearestFeatures);


							Feature * firstFeatureInList = new Feature();

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
								string trainrgbMapFacingPolyFileNameCPlus = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
								char * trainrgbMapFacingPolyFileName = const_cast<char*>(trainrgbMapFacingPolyFileNameCPlus.c_str());

								pixmap * trainrgbPixMap;

								//cout << "\t trainrgbPixMap = load_ppm(" << trainrgbMapFacingPolyFileName << ");" << endl;
								trainrgbPixMap = load_ppm(trainrgbMapFacingPolyFileName);

								//cout << "\t createRGBMapFromPixmapImage(trainrgbPixMap, trainrgbMap);" << endl;
								createRGBMapFromPixmapImage(trainrgbPixMap, trainrgbMap);
								free_pixmap(trainrgbPixMap);

								colour avgCol;
								calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, trainrgbMap, &avgCol);

								int rBin = (int)(((double)(avgCol.r)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
								int gBin = (int)(((double)(avgCol.g)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
								int bBin = (int)(((double)(avgCol.b)/(MAX_RGB_VAL))*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);

								FeatureContainer * currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[rBin][gBin][bBin]);
								// go to last feature container in list (ie append to list if list already has items)
								while(currentFeatureContainerInList->next != NULL)
								{
									currentFeatureContainerInList = currentFeatureContainerInList->next;
								}
								currentFeatureContainerInList->firstFeatureInFeatureList = firstFeatureInList;
								FeatureContainer * newFeatureContainer = new FeatureContainer();
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

		/*
		for(int xBin = 0; xBin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS; xBin++)
		{
			for(int yBin = 0; yBin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS; yBin++)
			{
				for(int x2Bin = 0; x2Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS; x2Bin++)
				{
					for(int y2Bin = 0; y2Bin < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS; y2Bin++)
					{
						FeatureContainer * currentFeatureContainerInList1 = &(trainfeatureGeoCompBinArray[xBin][yBin][x2Bin][y2Bin][0]);
						FeatureContainer * currentFeatureContainerInList2 = &(trainfeatureGeoCompBinArray[xBin][yBin][x2Bin][y2Bin][1]);
						currentFeatureContainerInList1->next = NULL;
						currentFeatureContainerInList2->next = NULL;
					}
				}
			}
		}
		*/


		//cout << "0" << endl;

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

							char trainpolygonIndexString[10];
							char trainsideIndexString[10];
							char trainviewIndexString[10];
							char trainzoomIndexString[10];

							sprintf(trainpolygonIndexString, "%d", trainPolyIndex);
							sprintf(trainsideIndexString, "%d", trainSideIndex);
							sprintf(trainviewIndexString, "%d", trainViewIndex);
							sprintf(trainzoomIndexString, "%d", trainZoomIndex);

							string traininterpolatedRGBMapFileNameForRayTracing;
							traininterpolatedRGBMapFileNameForRayTracing = trainObjectNameArray[trainObjectIndex] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;





							string traininterpolatedNearestFeaturesMapFileName;
							traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
							Feature * firstFeatureInTempList = new Feature();

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

							Feature * currentFeatureInTempList = firstFeatureInTempList;
								//now bin the features;
							int findex1 = 0;

							//cout << "1" << endl;

							while(currentFeatureInTempList->next != NULL)
							{
								Feature * currentFeatureInTempList2 = firstFeatureInTempList;
									//now bin the features;
								int findex2 = 0;
								while(currentFeatureInTempList2->next != NULL)
								{
									if(findex1 != findex2)
									{
										//cout << "2" << endl;

										int x1Bin = (currentFeatureInTempList->pointTransformed.x / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
										int y1Bin = (currentFeatureInTempList->pointTransformed.y / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
										int x2Bin = (currentFeatureInTempList2->pointTransformed.x / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
										int y2Bin = (currentFeatureInTempList2->pointTransformed.y / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);


										//cout << "3" << endl;
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
																		//this passes; cout << "inside bin during array formation - good" << endl;
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
											//cout << "4" << endl;

											FeatureContainer * currentFeatureContainerInList1 = &(trainfeatureGeoCompBinArray[x1Bin][y1Bin][x2Bin][y2Bin][0]);
											FeatureContainer * currentFeatureContainerInList2 = &(trainfeatureGeoCompBinArray[x1Bin][y1Bin][x2Bin][y2Bin][1]);

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
											FeatureContainer * newFeatureContainer1 = new FeatureContainer();
											currentFeatureContainerInList1->next = newFeatureContainer1;
											currentFeatureContainerInList2->firstFeatureInFeatureList = currentFeatureInTempList2;
											FeatureContainer * newFeatureContainer2 = new FeatureContainer();
											currentFeatureContainerInList2->next = newFeatureContainer2;

											//cout << "5" << endl;

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
		//cout << "here" << endl;


		//OLD temp debug
		/*
		for(int x=0; x<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS; x++)
		{
			for(int y=0; y<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS; y++)
			{
				cout << "bin x = " << x << ", bin y = " << y << endl;
				FeatureContainer * currentFeatureContainerInList = &(trainfeatureGeoCompBinArray[x][y]);

				// go to last feature container in list (ie append to list if list already has items)
				while(currentFeatureContainerInList->next != NULL)
				{
					cout << "\t feature transformed point, x = " << currentFeatureContainerInList->firstFeatureInFeatureList->pointTransformed.x << ", y = " << currentFeatureContainerInList->firstFeatureInFeatureList->pointTransformed.y << ", z = " << currentFeatureContainerInList->firstFeatureInFeatureList->pointTransformed.z << endl;
					currentFeatureContainerInList = currentFeatureContainerInList->next;
				}
			}
		}
		exit(0);
		*/

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
				char trainviewIndexStringTemp[10];
				sprintf(trainviewIndexStringTemp, "%d", 0);

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

				string ICRNewTestViewIndexRow1 = "<TR><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + trainviewIndexStringTemp + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD><img src=\"" + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION + "\" border=0> </TD></TR>";
				writeStringToFileObject2(ICRNewTestViewIndexRow1, &writeFileObject);

				if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
				{
					string ICRNewTestViewIndexRow2 = "<TR><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + trainviewIndexStringTemp + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD><img src=\"" + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TRAIN_STRING + PNG_EXTENSION + "\" border=0> </TD></TR>";
					writeStringToFileObject2(ICRNewTestViewIndexRow2, &writeFileObject);
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
					char testviewIndexStringTemp[10];
					sprintf(testviewIndexStringTemp, "%d", testViewIndex+testViewNumber);
					char testzoomIndexStringTemp[10];
					sprintf(testzoomIndexStringTemp, "%d", testZoomIndex);

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

					string ICRNewTestViewIndexRow3 = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexStringTemp + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD><img src=\"" + mapFileName + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION + "\" border=0> </TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD></TR>";
					writeStringToFileObject2(ICRNewTestViewIndexRow3, &writeFileObject);
					if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
					{
						string ICRNewTestViewIndexRow4 = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexStringTemp + "</TD><TD>" + blank + "</TD><TD>" + "</TD><TD>" + blank + blank + "</TD><TD><img src=\"" + mapFileName + FEATURESMAP_PPM_EXTENSION_PART + TEST_STRING + PNG_EXTENSION + "\" border=0> </TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD><TD>" + blank + "</TD></TR>";
						writeStringToFileObject2(ICRNewTestViewIndexRow4, &writeFileObject);
					}
				}
			#endif

				
				double averageMatchError = 0.0;
				
				for(int testPolyIndex=0; testPolyIndex<numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex]; testPolyIndex++)
				{
					//cout << "h-1b" << endl;
					
					if(OR_PRINT_ALGORITHM_PROGRESS)
					{
						if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
						{
							cout << "\t\t\t\t  testPIndex = " << testPolyIndex << endl;
						}
					}


					int trainPolyIndexWithLowestErrorRecord[3][5];	//for each test side x3, 0=trainObjectIndex, 1=trainViewIndex, 2=trainViewIndex, 3=trainpolyindex, 4=trainsideIndex
					#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
					string trainObjectNameWithLowestErrorRecord;
					#endif

					double averageMatchErrorAcrossSides = 0.0;

					for(int testSideIndex=0; testSideIndex < OR_METHOD_POLYGON_NUMBER_OF_SIDES; testSideIndex++)
					{
					
						bool testMapLoaded;
						if(OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
						{
							testMapLoaded = false;
						}

						char testviewIndexString[10];
						char testzoomIndexString[10];
						char testpolygonIndexString[10];
						char testsideIndexString[10];

						sprintf(testviewIndexString, "%d", testViewIndex+testViewNumber);
						sprintf(testzoomIndexString, "%d", testZoomIndex);
						sprintf(testpolygonIndexString, "%d", testPolyIndex);
						sprintf(testsideIndexString, "%d", testSideIndex);

						string testinterpolatedRGBMapFileNameForRayTracing;
						testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

						string testrgbMapSmallFacingPolyFileNameCPlus = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;
						char * testrgbMapSmallFacingPolyFileName = const_cast<char*>(testrgbMapSmallFacingPolyFileNameCPlus.c_str());
							
						if(useGeneratedTestPixmapFiles)
						{
							if(!OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
							{

								string testrgbMapFacingPolyFileNameCPlusWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

								string testrgbMapFacingPolyFileNameCPlus = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;
								char * testrgbMapFacingPolyFileName = const_cast<char*>(testrgbMapFacingPolyFileNameCPlus.c_str());

								pixmap * testrgbPixMap;
								//cout << "testrgbPixMap = load_ppm(" << testrgbMapFacingPolyFileName << ");" << endl;
								testrgbPixMap = load_ppm(testrgbMapFacingPolyFileName);

								//cout << "createRGBMapFromPixmapImage(testrgbPixMap, testrgbMap);" << endl;
								createRGBMapFromPixmapImage(testrgbPixMap, testrgbMap);

								free_pixmap(testrgbPixMap);



								#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
								pixmap * testrgbPixMapSmall;
								cout << "testrgbPixMapSmall = load_ppm(" << testrgbMapSmallFacingPolyFileName << ");" << endl;
								testrgbPixMapSmall = load_ppm(testrgbMapSmallFacingPolyFileName);
								cout << "createRGBMapFromPixmapImage(testrgbPixMapSmall, testrgbMapSmall);" << endl;
								createRGBMapFromPixmapImage(testrgbPixMapSmall, testrgbMapSmall);
								free_pixmap(testrgbPixMapSmall);
								#endif

							}


							if(!OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								if(!OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO)
								{
									/*
									if(OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION)
									{
										char testviewIndexString[10];
										char testzoomIndexString[10];
										char testpolygonIndexString[10];
										char testsideIndexString[10];
										sprintf(testviewIndexString, "%d", testViewIndex+testViewNumber);
										sprintf(testzoomIndexString, "%d", testZoomIndex);
										sprintf(testpolygonIndexString, "%d", testPolyIndex);
										sprintf(testsideIndexString, "%d", testSideIndex);
										string testinterpolatedRGBMapFileNameForRayTracing;
										testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
									}
									*/
									string testinterpolatedNearestFeaturesMapFileName;
									testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
									createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testfirstFeatureInNearestFeatureList, false, false, false);
								}
							}

							#ifdef TEMPTEST3GEOACCURACY
							string testinterpolatedNearestFeaturesMapFileName;
							testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
							createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testfirstFeatureInNearestFeatureList, false, false, false);
							#endif
						}



						double currentLowestError = VERY_HIGH_ERROR;

						/*
						//int trainObjectIndexWithLowestError;
						//int trainViewIndexWithLowestError;
						//int trainPolyIndexWithLowestError;
						//int trainSideIndexWithLowestError;
						*/



					#ifdef OR_METHOD_GEOMETRIC_COMPARISON_BINNING

						Feature * currentTestFeature;
						
						
						if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
						{
							currentTestFeature = new Feature();
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
								convertSQLdatabaseStringToSnapshotMaps(imageWidthFacingPoly, imageHeightFacingPoly, testrgbMap, testrgbDevMap, testdepthMap, smallImageWidth, smallImageHeight, testrgbMapSmall, testdepthMapSmall, testrgbDevMapSmall, dimension, currentTestFeature->snapshotMapsText, OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS);
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
								currentTestFeature = new Feature(); 	//create dummy feature - not used.
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

								//cout << "normalisedAverageHueDeviationRequirement.r = " << int(normalisedAverageHueDeviationRequirement.r) << endl;
								//cout << "normalisedAverageHueDeviationRequirement.g = " << int(normalisedAverageHueDeviationRequirement.g) << endl;
								//cout << "normalisedAverageHueDeviationRequirement.b = " << int(normalisedAverageHueDeviationRequirement.b) << endl;
							}
							else
							{
								double normalisedAverageRHueContrast;
								double normalisedAverageGHueContrast;
								double normalisedAverageBHueContrast;
								double averageContrastWithRespectToAverageColour;
								double averageStarkContrastWithRespectToAverageColour;
								double averageLocalContrast;
								double averageLocalStarkContrast;
								createNormalisedHueContrastMapUsingRGBMapAndCalculateAllContrastValues(imageWidthFacingPoly, imageWidthFacingPoly, testrgbMap, testrgbDevMap, &normalisedAverageRHueContrast, &normalisedAverageGHueContrast, &normalisedAverageBHueContrast, &averageContrastWithRespectToAverageColour, &averageStarkContrastWithRespectToAverageColour, &averageLocalContrast, &averageLocalStarkContrast);
								vec normalisedAverageHueContrast;
								normalisedAverageHueContrast.x = normalisedAverageRHueContrast;
								normalisedAverageHueContrast.y = normalisedAverageGHueContrast;
								normalisedAverageHueContrast.z = normalisedAverageBHueContrast;
								colour cullednormalisedAverageHueContrast;
								/*
								cout << "normalisedAverageHueContrast.x = " << normalisedAverageHueContrast.x << endl;
								cout << "normalisedAverageHueContrast.y = " << normalisedAverageHueContrast.y << endl;
								cout << "normalisedAverageHueContrast.z = " << normalisedAverageHueContrast.z << endl;
								*/
								cullAndBinNormalisedHueContrast(&normalisedAverageHueContrast, &cullednormalisedAverageHueContrast);
								/*
								cout << "cullednormalisedAverageHueContrast->r = " << int(cullednormalisedAverageHueContrast.r) << endl;
								cout << "cullednormalisedAverageHueContrast->g = " << int(cullednormalisedAverageHueContrast.g) << endl;
								cout << "cullednormalisedAverageHueContrast->b = " << int(cullednormalisedAverageHueContrast.b) << endl;
								*/

								normalisedAverageHueDeviationRequirement.r = cullednormalisedAverageHueContrast.r;
								normalisedAverageHueDeviationRequirement.g = cullednormalisedAverageHueContrast.g;
								normalisedAverageHueDeviationRequirement.b = cullednormalisedAverageHueContrast.b;
							}
						}


						unsigned char * rgb8BitSmallMapForInstantDBQueryAccessRequirement;
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
							createNormalisedHueContrastMapUsingRGBMapAndCalculateAllContrastValues(smallImageWidth, smallImageHeight, testrgbMapSmall, testrgbDevMapSmall, &smallTempnormalisedAverageRHueContrast, &smallTempnormalisedAverageGHueContrast, &smallTempnormalisedAverageBHueContrast, &smallTempaverageContrastWithRespectToAverageColour, &smallTempaverageStarkContrastWithRespectToAverageColour, &smallTempaverageLocalContrast, &smallTempaverageLocalStarkContrast);
							convertNormalisedHueDeviationMapTo3x8BitMap(smallImageWidth, smallImageHeight, testrgbDevMapSmall, testrgbDev8BitSmallMap);
							rgb8BitSmallMapForInstantDBQueryAccessRequirement = testrgbDev8BitSmallMap;
						
						}
						else
						{
							rgb8BitSmallMapForInstantDBQueryAccessRequirement = NULL;
						}

						signed char * testconcatonatedSignedDctCoeffArrayRequirement;
						if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
						{
							if(OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
							{
								testconcatonatedSignedDctCoeffArrayRequirement = currentTestFeature->dctCoeff;

								/*
								for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
								{
									cout << "testconcatonatedSignedDctCoeffArrayRequirement[i] = " << int(testconcatonatedSignedDctCoeffArrayRequirement[i]) << endl;
								}
								*/

								//cout << "stored currentTestFeature->dctCoeffArrayBinned = " << currentTestFeature->dctCoeffArrayBinned << endl;
							}
							else
							{
								testconcatonatedSignedDctCoeffArrayRequirement = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
								
								/*RECENTLY REMOVED;
								if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
								{
									generatePixmapFromRGBMap(testrgbMapSmallFacingPolyFileName, smallImageWidth, smallImageHeight, rgbMapSmallFacingPoly);							
								}
								*/

								string testrgbMapSmallFacingPolyFileNameJPEGCPlus = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + JPG_EXTENSION;
								char * testrgbMapSmallFacingPolyFileNameJPEG = const_cast<char*>(testrgbMapSmallFacingPolyFileNameJPEGCPlus.c_str());						
								string convertSmallImageRGBtoJPEGCommand = "";

								/*									
								convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " +	testrgbMapSmallFacingPolyFileName + " " + testrgbMapSmallFacingPolyFileNameJPEG;
								system(convertSmallImageRGBtoJPEGCommand.c_str());
								int dctCoeffArrayHeight = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
								int dctCoeffArrayWidth = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
								readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(testrgbMapSmallFacingPolyFileNameJPEG, testdctCoeffArrayY, testdctCoeffArrayYCr, testdctCoeffArrayYCb, dctCoeffArrayHeight, dctCoeffArrayWidth, true);
								*/

								if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING)
								{
									int dctCoeffArrayHeight = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
									int dctCoeffArrayWidth = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
									for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D; i++)
									{
										testdctCoeffArrayY[i] = 0;
									}
									for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D; i++)
									{
										testdctCoeffArrayYCr[i] = 0;
									}
									for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D; i++)
									{
										testdctCoeffArrayYCb[i] = 0;
									}							

									//cout << "testrgbMapSmallFacingPolyFileName = " << testrgbMapSmallFacingPolyFileName << endl;
									//cout << "testrgbMapSmallFacingPolyFileNameJPEG = " << testrgbMapSmallFacingPolyFileNameJPEG << endl;
									
									convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " + testrgbMapSmallFacingPolyFileName + " " + testrgbMapSmallFacingPolyFileNameJPEG;
									system(convertSmallImageRGBtoJPEGCommand.c_str());
									readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(testrgbMapSmallFacingPolyFileNameJPEG, testdctCoeffArrayY, testdctCoeffArrayYCrDummy, testdctCoeffArrayYCbDummy, dctCoeffArrayHeight, dctCoeffArrayWidth, true);
									convertSmallImageRGBtoJPEGCommand = "";
									convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " + testrgbMapSmallFacingPolyFileName + " " + testrgbMapSmallFacingPolyFileNameJPEG;
									system(convertSmallImageRGBtoJPEGCommand.c_str());
									readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(testrgbMapSmallFacingPolyFileNameJPEG, testdctCoeffArrayYDummy, testdctCoeffArrayYCr, testdctCoeffArrayYCb, dctCoeffArrayHeight, dctCoeffArrayWidth, true);
									//cout << "here14b" << endl;
									//exit(0);
								}
								else
								{
								
									//cout << "testrgbMapSmallFacingPolyFileName = " << testrgbMapSmallFacingPolyFileName << endl;
									//cout << "testrgbMapSmallFacingPolyFileNameJPEG = " << testrgbMapSmallFacingPolyFileNameJPEG << endl;
																	
									convertSmallImageRGBtoJPEGCommand = convertSmallImageRGBtoJPEGCommand + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING + " " + testrgbMapSmallFacingPolyFileName + " " + testrgbMapSmallFacingPolyFileNameJPEG;
									system(convertSmallImageRGBtoJPEGCommand.c_str());
									int dctCoeffArrayHeight = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
									int dctCoeffArrayWidth = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
									for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D; i++)
									{
										testdctCoeffArrayY[i] = 0;
									}
									for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D; i++)
									{
										testdctCoeffArrayYCr[i] = 0;
									}
									for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D; i++)
									{
										testdctCoeffArrayYCb[i] = 0;
									}
									readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(testrgbMapSmallFacingPolyFileNameJPEG, testdctCoeffArrayY, testdctCoeffArrayYCr, testdctCoeffArrayYCb, dctCoeffArrayHeight, dctCoeffArrayWidth, true);
									//cout << "here14" << endl;
									//exit(0);
								}

								
								
								//long binnedAllDCTCoeff64BitValue = convertDCTCoeffIndividualArraysToBinnedAllDCTCoeff64BitValue(testdctCoeffArrayY, testdctCoeffArrayYCr, testdctCoeffArrayYCb, testconcatonatedSignedDctCoeffArrayRequirement);	//may not be used here
								convertDCTCoeffIndividualArraysToConcatonatedSignedDctCoeffArray(testdctCoeffArrayY, testdctCoeffArrayYCr, testdctCoeffArrayYCb, testconcatonatedSignedDctCoeffArrayRequirement);

								/*RECENTLY REMOVED;
								if(OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE)
								{
									remove(testrgbMapSmallFacingPolyFileName);
								}
								*/				
							}
						}
						else
						{
							testconcatonatedSignedDctCoeffArrayRequirement = NULL;
						}
						
						
						//cout << "here0" << endl;
						//#ifdef OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
						bool fLoopOn = true;
						bool fLoopSingle = false;
						if(!OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
						{
							fLoopSingle = true;
						}
												
						//cout << "h0" << endl;
						Feature * testcurrentFeatureInNearestFeatureList = currentTestFeature;
						int findex1 = 0;
						while((testcurrentFeatureInNearestFeatureList->next != NULL) && fLoopOn)
						{
							Feature * testcurrentFeatureInNearestFeatureList2 = currentTestFeature;
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

									//cout << "here1b" << endl;
									
									bool geoBinWithinRange = false;
									if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
									{
										xBin = (testcurrentFeatureInNearestFeatureList->pointTransformed.x / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
										yBin = (testcurrentFeatureInNearestFeatureList->pointTransformed.y / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
										x2Bin = (testcurrentFeatureInNearestFeatureList2->pointTransformed.x / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
										y2Bin = (testcurrentFeatureInNearestFeatureList2->pointTransformed.y / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);

									
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
										//cout << "sad" << endl;
										
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
						
														//cout << "here4b" << endl;


									#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

										FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin =  &(trainfeatureGeoCompBinArray[x][y][x2][y2][0]);
										FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin2 =  &(trainfeatureGeoCompBinArray[x][y][x2][y2][1]);
										FeatureContainer * currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
										FeatureContainer * currentFeatureContainerInTestFeatureMatchingTrainBin2 = firstFeatureContainerInTestFeatureMatchingTrainBin2;

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
											//cout << "geoxybin = " << geoxybin << endl;
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



										//cout << "here1c" << endl;
										
										FeatureContainer * currentFeatureContainerInTestFeatureMatchingTrainBin;
										FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin = new FeatureContainer();		

										if(OR_IMAGE_COMPARISON_DECISION_TREE)
										{
											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_SLOW_BUT_USE_LESS_HD
											int exceptionModifier = 1;	//positive
											for(int exceptionIndex=0; exceptionIndex<(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)+1; exceptionIndex++)
											{ 
												if(exceptionIndex != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS*2)
												{
													testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2] = testconcatonatedSignedDctCoeffArrayRequirement[exceptionIndex/2]+exceptionModifier;
												} 
											#endif

											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL_DIRECT_ACCESS
												createFeatureContainerListUsingUsingGetSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCheck(firstFeatureContainerInTestFeatureMatchingTrainBin, ignoreOTfeaturestrain, smallImageWidth, smallImageHeight, testrgbMapSmall, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, trainOrTest);
											#else
												/*
												for(int z=0; z<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; z++)
												{
													cout << "3 concatonatedSignedDctCoeffArrayRequirement[z] = " << int(testconcatonatedSignedDctCoeffArrayRequirement[z]) << endl;
												}
												*/

												currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;


												SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList = new SnapshotIDReferenceList();
												SnapshotIDReferenceList * currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDReferenceList;

												getSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCheck(smallImageWidth, smallImageHeight, testrgbMapSmall, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, firstReferenceInSnapshotIDReferenceList, trainOrTest);
												//cout << "hasdf" << endl;
												#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
												createAndOrParseIntoDirectory(&imageComparisonTreeBaseDirectory, NULL, false, false);	//reset current directory
												#endif

												currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
												//cout << "asd" << endl;

												/*
												bool chicken = false;
												int chickencount = 0;
												*/

												while(currentReferenceInSnapshotIDReferenceList->next != NULL)
												{
													//chickencount++;

													//cout << "found c" << endl;

													int irrelevant = 0;
													Feature * firstNewFeature = new Feature();
													currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList = firstNewFeature;
													createFeatureListUsingDatabaseQuery(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList, true, false, ignoreOTfeaturestrain, OR_MYSQL_TABLE_NAME_TRAIN, currentReferenceInSnapshotIDReferenceList->referenceID, "", irrelevant, irrelevant, irrelevant, irrelevant, true);
													FeatureContainer * newFeatureContainer = new FeatureContainer();
													currentFeatureContainerInTestFeatureMatchingTrainBin->next = newFeatureContainer;
													currentFeatureContainerInTestFeatureMatchingTrainBin = currentFeatureContainerInTestFeatureMatchingTrainBin->next;

													currentReferenceInSnapshotIDReferenceList = currentReferenceInSnapshotIDReferenceList->next;
												}


												/*
												if(chickencount > 3)
												{
												      chicken = true;
												}
												if(chicken)
												{
												      cout << "start create list" << endl;
												      sleep(3);
												}
												*/

												delete firstReferenceInSnapshotIDReferenceList;

												//if(chicken)
												//{
												//	cout << "end create list" << endl;
												//	sleep(3);
												//}


											#endif

										}
										else
										{
											//cout << "sa1" << endl;
											createFeaturesListUsingDatabaseQueryGeoXYbinRequirement(firstFeatureContainerInTestFeatureMatchingTrainBin, true, false, ignoreOTfeaturestrain, geoxybin, geoxBin, geoyBin, &normalisedAverageHueDeviationRequirement, testconcatonatedSignedDctCoeffArrayRequirement, rgb8BitSmallMapForInstantDBQueryAccessRequirement, smallImageWidth, smallImageHeight, OR_MYSQL_TABLE_NAME_TRAIN, trainOrTest);
											//cout << "sa2" << endl;														
											//exit(0);
										}
										currentFeatureContainerInTestFeatureMatchingTrainBin = firstFeatureContainerInTestFeatureMatchingTrainBin;
									#endif



									/*
									cout << "xBin = " << xBin << endl;
									cout << "yBin = " << yBin << endl;
									cout << "x2Bin = " << x2Bin << endl;
									cout << "y2Bin = " << y2Bin << endl;
									cout << "x = " << x << endl;
									cout << "y = " << y << endl;
									cout << "x2 = " << x2 << endl;
									cout << "y2 = " << y2 << endl;


									cout << "here5b" << endl;
									cout << "firstFeatureContainerInTestFeatureMatchingTrainBin->next = " << firstFeatureContainerInTestFeatureMatchingTrainBin->next << endl;
									*/

									int indx = 0;

									//cout << "before ->" << endl;
									
									while(currentFeatureContainerInTestFeatureMatchingTrainBin->next != NULL)
									{
										passes++;

										/*
										cout << "here4; indx = " << indx << endl;

										cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x << endl;
										cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y << endl;
										cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.z = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.z << endl;
										cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x << endl;
										cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y << endl;
										cout << "currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.z = " << currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.z << endl;

										cout << "testcurrentFeatureInNearestFeatureList->pointTransformed.x = " << testcurrentFeatureInNearestFeatureList->pointTransformed.x << endl;
										cout << "testcurrentFeatureInNearestFeatureList->pointTransformed.y = " << testcurrentFeatureInNearestFeatureList->pointTransformed.y << endl;
										cout << "testcurrentFeatureInNearestFeatureList->pointTransformed.z = " << testcurrentFeatureInNearestFeatureList->pointTransformed.z << endl;
										*/

										indx++;


										//cout << "here4" << endl;

										bool passedGeometricCheck = false;


										if(OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY)
										{

											if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
											{

												#ifdef USE_GEO_TEST_GRID_SIZE_LARGE

													passedGeometricCheck = true;

												#else

													double currentDistanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed));
													//cout << "c2" << endl;
													double xDev1 = testcurrentFeatureInNearestFeatureList->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.x;
													double yDev1 = testcurrentFeatureInNearestFeatureList->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->pointTransformed.y;
													if(currentDistanceBetweenTwoPoints < OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
													{
														//cout << "c4" << endl;

														#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2
														double currentDistanceBetweenTwoPoints2 = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList2->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed));
														double xDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.x;
														double yDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.y;
														#else
														double currentDistanceBetweenTwoPoints2 = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList2->pointTransformed), &(currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed));
														double xDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.x - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.x;
														double yDev2 = testcurrentFeatureInNearestFeatureList2->pointTransformed.y - currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->next->pointTransformed.y;
														#endif

														//cout << "c3" << endl;
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

												//cout << "c5" << endl;

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


										//temp;
										/*
										if(passedGeometricCheck)
										{
											string trainObjectString = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->objectName;
											int trainViewIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->viewIndex;
											int trainPolyIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->polyIndex;
											int trainSideIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->sideIndex;
											int trainZoomIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->zoomIndex;


											//cout << "\t\t\t testPolyIndex = " << testPolyIndex << endl;
											//cout << "\t\t passedGeometricCheck" << endl;


											cout << "\n\t\t PASSED GeometricCheck" << endl;
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

										}
										*/

										/*
										if(!passedGeometricCheck)
										{
											cout << "FAILED geo" << endl;
										}
										*/

										//cout << "before geo" << endl;
										
										if(passedGeometricCheck)
										{



											string trainObjectString = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->objectName;
											int trainViewIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->viewIndex;
											int trainZoomIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->zoomIndex;
											int trainPolyIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->polyIndex;
											int trainSideIndex = currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->sideIndex;



											//cout << "\t\tPASSED geo" << endl;
											//cout << "\t\t\t testPolyIndex = " << testPolyIndex << endl;
											//cout << "\t\t passedGeometricCheck" << endl;

											/*
											cout << "\n\t\t PASSED GeometricCheck" << endl;
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
											cout << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->OTpointIndex << "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.x <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.y <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->pointTransformed.z << "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->point.x <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->point.y <<  "\t" << currentFeatureContainerInTestFeatureMatchingTrainBin2->firstFeatureInFeatureList->point.z <<endl;
											*/

											char trainpolygonIndexString[10];
											char trainsideIndexString[10];
											char trainviewIndexString[10];
											char trainzoomIndexString[10];

											sprintf(trainpolygonIndexString, "%d", trainPolyIndex);
											sprintf(trainsideIndexString, "%d", trainSideIndex);
											sprintf(trainviewIndexString, "%d", trainViewIndex);
											sprintf(trainzoomIndexString, "%d", trainZoomIndex);

											string traininterpolatedRGBMapFileNameForRayTracing;
											traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

											//cout << "here5" << endl;



					#else



					#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL

						colour avgColTestMap;
						calculateAverageColour(imageWidthFacingPoly, imageHeightFacingPoly, testrgbMap, &avgColTestMap);

						int rBinMid = (int)(((double)avgColTestMap.r/MAX_RGB_VAL)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int gBinMid = (int)(((double)avgColTestMap.g/MAX_RGB_VAL)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);
						int bBinMid = (int)(((double)avgColTestMap.b/MAX_RGB_VAL)*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL);

						/*
						cout << "avgColTestMap.r = " << (int)avgColTestMap.r << endl;
						cout << "avgColTestMap.g = " << (int)avgColTestMap.g << endl;
						cout << "avgColTestMap.b = " << (int)avgColTestMap.b << endl;
						cout << "rBinMid = " << rBinMid << endl;
						cout << "gBinMid = " << gBinMid << endl;
						cout << "bBinMid = " << bBinMid << endl;
						*/

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
											FeatureContainer * currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[rColourSaturationVaried][gColourSaturationVaried][bColourSaturationVaried]);
								#else
										FeatureContainer * currentFeatureContainerInList = &(trainfeatureImageAverageColourCompBinArray[r][g][b]);
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

										char trainpolygonIndexString[10];
										char trainsideIndexString[10];
										char trainviewIndexString[10];
										char trainzoomIndexString[10];

										sprintf(trainpolygonIndexString, "%d", trainPolyIndex);
										sprintf(trainsideIndexString, "%d", trainSideIndex);
										sprintf(trainviewIndexString, "%d", trainViewIndex);
										sprintf(trainzoomIndexString, "%d", trainZoomIndex);

										string traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;



									#ifdef OR_METHOD_GEOMETRIC_COMPARISON
											#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO
												#ifndef OR_WINDOWS_COMPILER_LIMITATION_MUST_GET_TEST_DATA_FROM_SQL
												#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
												bool passedGeometricCheck = compareFeaturesListForMatch((testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList, currentFeatureContainerInList->firstFeatureInFeatureList, dimension);
												#else
												bool passedGeometricCheck = compareFeaturesListForMatch((testfeatureGeoCompArray[testObjectIndex][testViewIndex][testZoomIndex][testPolyIndex][testSideIndex]).firstFeatureInFeatureList, (trainfeatureGeoCompArray[trainObjectIndex][trainViewIndex][trainZoomIndex][trainPolyIndex][trainSideIndex]).firstFeatureInFeatureList, dimension);
												#endif
												#endif
											#else
												string traininterpolatedNearestFeaturesMapFileName;
												traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
												createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainfirstFeatureInNearestFeatureList, false, false, false);
												bool passedGeometricCheck = compareFeaturesListForMatch(testfirstFeatureInNearestFeatureList, trainfirstFeatureInNearestFeatureList, dimension);
											#endif
									#else
										bool passedGeometricCheck = true;
									#endif

										/*
										if(!passedGeometricCheck)
										{
											cout << "FAILED geo" << endl;
										}
										*/


										if(passedGeometricCheck)
										{
											//cout << "PASSED geo" << endl;


					#endif
										//cout << "c45" << endl;

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

														//cout << "c46" << endl;

														testMapLoaded = true;

														char testviewIndexString[10];
														char testzoomIndexString[10];
														char testpolygonIndexString[10];
														char testsideIndexString[10];

														sprintf(testviewIndexString, "%d", testViewIndex+testViewNumber);
														sprintf(testzoomIndexString, "%d", testZoomIndex);
														sprintf(testpolygonIndexString, "%d", testPolyIndex);
														sprintf(testsideIndexString, "%d", testSideIndex);

														string testinterpolatedRGBMapFileNameForRayTracing;
														testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;

														string testrgbMapFacingPolyFileNameCPlusWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

														string testrgbMapFacingPolyFileNameCPlus = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;
														char * testrgbMapFacingPolyFileName = const_cast<char*>(testrgbMapFacingPolyFileNameCPlus.c_str());

														pixmap * testrgbPixMap;
														//cout << "testrgbPixMap = load_ppm(" << testrgbMapFacingPolyFileName << ");" << endl;
														testrgbPixMap = load_ppm(testrgbMapFacingPolyFileName);

														//cout << "createRGBMapFromPixmapImage(testrgbPixMap, testrgbMap);" << endl;
														createRGBMapFromPixmapImage(testrgbPixMap, testrgbMap);

														free_pixmap(testrgbPixMap);

													#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
														//cout << "h3" << endl;
														string testrgbMapSmallFacingPolyFileNameCPlus = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;
														char * testrgbMapSmallFacingPolyFileName = const_cast<char*>(testrgbMapSmallFacingPolyFileNameCPlus.c_str());
														pixmap * testrgbPixMapSmall;
														testrgbPixMapSmall = load_ppm(testrgbMapSmallFacingPolyFileName);
														createRGBMapFromPixmapImage(testrgbPixMapSmall, testrgbMapSmall);
														free_pixmap(testrgbPixMapSmall);
														//cout << "h4" << endl;
													#endif
													}
												}
											}

											if(useGeneratedTrainPixmapFiles)
											{	

												string trainrgbMapFacingPolyFileNameCPlusWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;

												string trainrgbMapFacingPolyFileNameCPlus = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
												char * trainrgbMapFacingPolyFileName = const_cast<char*>(trainrgbMapFacingPolyFileNameCPlus.c_str());
											#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
												string trainrgbMapSmallFacingPolyFileNameCPlus = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
												char * trainrgbMapSmallFacingPolyFileName = const_cast<char*>(trainrgbMapSmallFacingPolyFileNameCPlus.c_str());
											#endif

												pixmap * trainrgbPixMap;

												//cout << "\t trainrgbPixMap = load_ppm(" << trainrgbMapFacingPolyFileName << ");" << endl;
												trainrgbPixMap = load_ppm(trainrgbMapFacingPolyFileName);


												//cout << "\t createRGBMapFromPixmapImage(trainrgbPixMap, trainrgbMap);" << endl;
												createRGBMapFromPixmapImage(trainrgbPixMap, trainrgbMap);
												free_pixmap(trainrgbPixMap);


											#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
												//cout << "h5" << endl;
												pixmap * trainrgbPixMapSmall;
												//cout << "trainrgbMapSmallFacingPolyFileName = " << trainrgbMapSmallFacingPolyFileName << endl;
												trainrgbPixMapSmall = load_ppm(trainrgbMapSmallFacingPolyFileName);
												createRGBMapFromPixmapImage(trainrgbPixMapSmall, trainrgbMapSmall);
												free_pixmap(trainrgbPixMapSmall);
												//cout << "h6" << endl;
											#endif	
											}
											else
											{
												//cout << "n2" << endl;
												convertSQLdatabaseStringToSnapshotMaps(imageWidthFacingPoly, imageHeightFacingPoly, trainrgbMap, trainrgbDevMap, traindepthMap, smallImageWidth, smallImageHeight, trainrgbMapSmall, traindepthMapSmall, trainrgbDevMapSmall, dimension, currentFeatureContainerInTestFeatureMatchingTrainBin->firstFeatureInFeatureList->snapshotMapsText, OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE, OR_IMAGE_COMPARISON_COMPARE_RGB_DEV_MAPS_NOT_RGB_MAPS);
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
							
							
										#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
											//cout << "h7" << endl;
											int resampledWidth = (imageWidthFacingPoly/smallImageRatio);
											int resampledHeight = (imageHeightFacingPoly/smallImageRatio);
											
											/*
											cout << "testrgbMapSmall = " << endl;
											printRGBMap(resampledWidth, resampledHeight, testrgbMapSmall);
											cout << "trainrgbMapSmall = " << endl;
											printRGBMap(resampledWidth, resampledHeight, trainrgbMapSmall);
											*/

											#ifdef OR_METHOD_SMALL_IMAGE_FIRST_COMPARISON_NO_KERNEL
												double errorSmall = compareImagesRGBSmallNoKernel(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);			//cannot use this function until it has been tested
											#else
												#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
													double errorSmall = compareImagesRGBWithPosDev(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);
												#else
													double errorSmall = compareImagesRGBWithPosDevAndLocalStarkContAndHueDev(resampledWidth, resampledHeight, testrgbMapSmall, trainrgbMapSmall);			//cannot use this function until it has been tested
												#endif
											#endif
											//cout << "errorSmall = " << errorSmall << endl;
											//cout << "OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION = " << OR_METHOD_GEOMETRIC_COMPARISON_USE_TEST_RGB_MAP_OPTIMISATION << endl;
											if(errorSmall < OR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD)
											{

										#endif
												//cout << "\t\t\tpassed SMALL IMAGE" << endl;
												
												#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
													double error = compareImagesRGBWithPosDev(imageWidthFacingPoly, imageHeightFacingPoly, testrgbMap, trainrgbMap);
												#else
													double error = compareImagesRGBWithPosDevAndLocalStarkContAndHueDev(imageWidthFacingPoly, imageHeightFacingPoly, testrgbMap, trainrgbMap);			//cannot use this function until it has been tested
													//double error = compareImagesRGBWithPosDev(imageWidthFacingPoly, imageHeightFacingPoly, testrgbMap, trainrgbMap);
												#endif

												//temp

												
												if(error < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
												{
													//cout << "\t\t\t\t\tpassed LARGE IMAGE" << endl;
													//cout << "PASSED im" << endl;
												}
												else
												{
													//cout << "FAILED im" << endl;
												}
												



												if(error < currentLowestError)
												{
													
														
													//cout << "transformed RGB check passed" << endl;

													#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
													bool passedContrastCheck = false;
													if(doNotCheckContrast)
													{
														passedContrastCheck = true;
													}
													else if(checkImageHasContrast(imageWidthFacingPoly, imageHeightFacingPoly, trainrgbMap))
													{
														passedContrastCheck = true;
													}
													if(passedContrastCheck)
													{
													#endif
														//cout << "contrast check passed" << endl;

														currentLowestError = error;

														/*
														if(error < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
														{
															//cout << "transformed RGB check passed" << endl;
														}
														*/


														#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
															trainObjectNameWithLowestErrorRecord = trainObjectString;
														#else
															trainPolyIndexWithLowestErrorRecord[testSideIndex][0] = trainObjectIndex;
														#endif


														trainPolyIndexWithLowestErrorRecord[testSideIndex][1] = trainViewIndex;
														trainPolyIndexWithLowestErrorRecord[testSideIndex][2] = trainZoomIndex;
														trainPolyIndexWithLowestErrorRecord[testSideIndex][3] = trainPolyIndex;
														trainPolyIndexWithLowestErrorRecord[testSideIndex][4] = trainSideIndex;
														//trainPolyIndexWithLowestErrorRecordStringSide[testSideIndex] = trainrgbMapFacingPolyFileNameCPlusWithoutExt;

														

													#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
													}
													#endif
												}

										#ifdef OR_METHOD_USE_SMALL_IMAGE_FIRST_COMPARISON
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
											exceptionModifier = exceptionModifier * -1;													
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
						delete currentTestFeature;


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
											//cout << "fail geo test" << endl;
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
							if(!checkImageHasContrast(imageWidthFacingPoly, imageHeightFacingPoly, testrgbMap))
							{
								currentLowestError = OR_IMAGE_COMPARISON_NO_CONTRAST_ERROR_KEY;
							}
						}
						#endif

						/*
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
								cout << "trainObjectNameWithLowestError = " << trainObjectNameWithLowestErrorRecord << endl;
							#else
								cout << "trainObjectIndexWithLowestError = " << trainPolyIndexWithLowestErrorRecord[0][0] << endl;
							#endif
							cout << "trainViewIndexWithHighestAccuracy = " << trainViewIndexWithLowestError << endl;
							cout << "trainZoomIndexWithHighestAccuracy = " << trainZoomIndexWithLowestError << endl;
							cout << "trainPolyIndexWithHighestAccuracy = " << trainPolyIndexWithLowestError << endl;
							cout << "trainSideIndexWithHighestAccuracy = " << trainSideIndexWithLowestError << endl;
							cout << "error of match = " << currentLowestError << endl;

						}
						*/

						averageMatchError = averageMatchError + currentLowestError;
						averageMatchErrorAcrossSides = averageMatchErrorAcrossSides + currentLowestError;


					}

					averageMatchErrorAcrossSides = (averageMatchErrorAcrossSides/OR_METHOD_POLYGON_NUMBER_OF_SIDES);
					//cout << "averageMatchErrorAcrossSides = " << averageMatchErrorAcrossSides << endl;


				#ifndef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
					if(averageMatchErrorAcrossSides < OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD)
					{
						char averageMatchErrorAcrossSidesString[10];
						sprintf(averageMatchErrorAcrossSidesString, "%0.3f", averageMatchErrorAcrossSides);

						//cout << "transformed RGB check passed 2" << endl;

						if(((trainPolyIndexWithLowestErrorRecord[0][2] == trainPolyIndexWithLowestErrorRecord[1][2]) && (trainPolyIndexWithLowestErrorRecord[0][2] == trainPolyIndexWithLowestErrorRecord[2][2])) && ((trainPolyIndexWithLowestErrorRecord[0][3] == trainPolyIndexWithLowestErrorRecord[1][3]) && (trainPolyIndexWithLowestErrorRecord[0][3] == trainPolyIndexWithLowestErrorRecord[2][3])))
						{//all sides are referring to the same poly, and all sides are referring to the same zoom
							
							#ifdef OR_DEBUG_COMPARISON_ALLOW_SAME_OBJECT_AND_SAME_VIEW_TO_BE_COMPARED
							if(1)
							#elif defined OR_DEBUG_COMPARISON_ALLOW_SAME_OBJECT_BUT_DIFFERENT_VIEWS_TO_BE_COMPARED
							if(!((trainObjectNameWithLowestErrorRecord == testObjectNameArray[testObjectIndex]) && (trainPolyIndexWithLowestErrorRecord[0][1] == testViewIndex+testViewNumber)))
							#else
							if(trainObjectNameWithLowestErrorRecord != testObjectNameArray[testObjectIndex])
							#endif
							{//object name and object view are not both the same
								
								//a real image comparitor match has been declared found
								
								//cout << "transformed RGB check passed 3" << endl;

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
										cout << "trainObjectNameWithLowestError = " << trainObjectNameWithLowestErrorRecord << endl;
									#else
										cout << "trainObjectIndexWithLowestError = " << trainPolyIndexWithLowestErrorRecord[0][0] << endl;
									#endif
									cout << "trainViewIndexWithHighestAccuracy = " <<  trainPolyIndexWithLowestErrorRecord[0][1] << endl;
									cout << "trainZoomIndexWithHighestAccuracy = " <<  trainPolyIndexWithLowestErrorRecord[0][2] << endl;
									cout << "trainPolyIndexWithHighestAccuracy = " << trainPolyIndexWithLowestErrorRecord[0][3] << endl;
									cout << "trainSideIndexWithHighestAccuracy = " << trainPolyIndexWithLowestErrorRecord[0][4] << endl;
									cout << "error of match = " << averageMatchErrorAcrossSides << endl;
								}


								//see if object exists, and if it does, increment number of successful snapshots
								ObjectReferenceList * currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
								bool foundObjectNameInObjectReferenceList = false;
								while(currentReferenceInSnapshotMatchObjectReferenceList->next != NULL)
								{
									if(currentReferenceInSnapshotMatchObjectReferenceList->objectName == trainObjectNameWithLowestErrorRecord)
									{
										currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots + 1;
										foundObjectNameInObjectReferenceList = true;
									}	


									currentReferenceInSnapshotMatchObjectReferenceList = currentReferenceInSnapshotMatchObjectReferenceList->next;
								}
								if(!foundObjectNameInObjectReferenceList)
								{
									currentReferenceInSnapshotMatchObjectReferenceList->objectName = trainObjectNameWithLowestErrorRecord;
									currentReferenceInSnapshotMatchObjectReferenceList->numMatchingSnapshots = 1;
									ObjectReferenceList * newObjectReference = new ObjectReferenceList();
									currentReferenceInSnapshotMatchObjectReferenceList->next = newObjectReference;
								}

								//cout << "h0" << endl;
								for(int s=0; s<OR_METHOD_POLYGON_NUMBER_OF_SIDES; s++)
								{
									if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
									{

										//cout << "h02" << endl;


										char trainviewIndexString[10];
										char trainzoomIndexString[10];
										char trainpolygonIndexString[10];
										char trainsideIndexString[10];


										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											//cout << "h03" << endl;
											string trainobjectIndexString = trainObjectNameWithLowestErrorRecord;
											//cout << "h04" << endl;
										#else
											char trainobjectIndexString[10];
											sprintf(trainobjectIndexString, "%d", trainPolyIndexWithLowestErrorRecord[s][0]);
										#endif

										sprintf(trainviewIndexString, "%d", trainPolyIndexWithLowestErrorRecord[s][1]);
										sprintf(trainzoomIndexString, "%d", trainPolyIndexWithLowestErrorRecord[s][2]);
										sprintf(trainpolygonIndexString, "%d", trainPolyIndexWithLowestErrorRecord[s][3]);
										sprintf(trainsideIndexString, "%d", trainPolyIndexWithLowestErrorRecord[s][4]);

										//cout << "h05" << endl;
										char testviewIndexString[10];
										char testzoomIndexString[10];
										char testpolygonIndexString[10];
										char testsideIndexString[10];
										sprintf(testviewIndexString, "%d", testViewIndex+testViewNumber);
										sprintf(testzoomIndexString, "%d", testZoomIndex);
										sprintf(testpolygonIndexString, "%d", testPolyIndex);
										sprintf(testsideIndexString, "%d", s);
										//cout << "h1" << endl;

										string blank = "";

										string traininterpolatedRGBMapFileNameForRayTracing;
										string testinterpolatedRGBMapFileNameForRayTracing;

									#ifdef TEMPTEST3GEOACCURACY
										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES

											traininterpolatedRGBMapFileNameForRayTracing = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#else

											traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#endif
										string traininterpolatedNearestFeaturesMapFileName = traininterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TRAIN_STRING + TFD_EXTENSION;
										createFeaturesListUsingFeaturesFile(traininterpolatedNearestFeaturesMapFileName, trainfirstFeatureInNearestFeatureList, false, false, false);


										testinterpolatedRGBMapFileNameForRayTracing = objectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
										string testinterpolatedNearestFeaturesMapFileName = testinterpolatedRGBMapFileNameForRayTracing + TRANSFORMED_FEATURES_NEARBY_EXTENSION_PART + TEST_STRING + TFD_EXTENSION;
										createFeaturesListUsingFeaturesFile(testinterpolatedNearestFeaturesMapFileName, testfirstFeatureInNearestFeatureList, false, false, false);



										cout << "\n new match found;" << endl;
										cout << "traininterpolatedNearestFeaturesMapFileName = " << traininterpolatedNearestFeaturesMapFileName << endl;
										Feature * currentFeatureInList = trainfirstFeatureInNearestFeatureList;
										while(currentFeatureInList->next != NULL)
										{

											cout << currentFeatureInList->OTpointIndex << "\t" << currentFeatureInList->pointTransformed.x <<  "\t" << currentFeatureInList->pointTransformed.y <<  "\t" << currentFeatureInList->pointTransformed.z << "\t" << currentFeatureInList->point.x <<  "\t" << currentFeatureInList->point.y <<  "\t" << currentFeatureInList->point.z <<endl;

											currentFeatureInList = currentFeatureInList->next;
										}
										cout << "testinterpolatedNearestFeaturesMapFileName = " << testinterpolatedNearestFeaturesMapFileName << endl;
										currentFeatureInList = testfirstFeatureInNearestFeatureList;
										while(currentFeatureInList->next != NULL)
										{
											cout << currentFeatureInList->OTpointIndex << "\t" << currentFeatureInList->pointTransformed.x <<  "\t" << currentFeatureInList->pointTransformed.y <<  "\t" << currentFeatureInList->pointTransformed.z << "\t" << currentFeatureInList->point.x <<  "\t" << currentFeatureInList->point.y <<  "\t" << currentFeatureInList->point.z <<endl;

											currentFeatureInList = currentFeatureInList->next;
										}
										bool passedGeometricCheck = compareFeaturesListForMatch(testfirstFeatureInNearestFeatureList, trainfirstFeatureInNearestFeatureList, dimension);
										cout << "passedGeometricCheck = " << passedGeometricCheck << endl;
									#endif




										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											traininterpolatedRGBMapFileNameForRayTracing = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#else
											traininterpolatedRGBMapFileNameForRayTracing = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;
										#endif
										string trainrgbMapFacingPolyFileNameCPlusWithoutExt = traininterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;

										string traininterpolatedRGBMapFileNameForRayTracingSanitised;
										#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_PRINT_OBJECT_NAMES
											traininterpolatedRGBMapFileNameForRayTracingSanitised = trainobjectIndexString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "X" + trainpolygonIndexString + "Y" + trainsideIndexString;
										#else
											traininterpolatedRGBMapFileNameForRayTracingSanitised = objectNameArray[trainPolyIndexWithLowestErrorRecord[0][0]] + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "ZoomIndex" + trainzoomIndexString + "X" + trainpolygonIndexString + "Y" + trainsideIndexString;
										#endif
										string trainrgbMapFacingPolyFileNameCPlusWithoutExtSanitised = traininterpolatedRGBMapFileNameForRayTracingSanitised + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING;



										testinterpolatedRGBMapFileNameForRayTracing = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "FacingPoly" + testpolygonIndexString + "side" + testsideIndexString;
										string testrgbMapFacingPolyFileNameCPlusWithoutExt = testinterpolatedRGBMapFileNameForRayTracing + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

										string testinterpolatedRGBMapFileNameForRayTracingSanitised;
										testinterpolatedRGBMapFileNameForRayTracingSanitised = testObjectNameArray[testObjectIndex] + "interpolatedMesh" + "ViewIndex" + testviewIndexString + "ZoomIndex" + testzoomIndexString + "X" + testpolygonIndexString + "Y" + testsideIndexString;
										string testrgbMapFacingPolyFileNameCPlusWithoutExtSanitised = testinterpolatedRGBMapFileNameForRayTracingSanitised + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING;

										//cout << "h2" << endl;

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

										/*
										if(generateTrainPixmapFiles)
										{
											string trainrgbMapFacingPolyFileNameTemp = trainrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION;
											char * trainrgbMapFacingPolyFileNameTempCharStar = const_cast<char*>(trainrgbMapFacingPolyFileNameTemp.c_str());
											generatePixmapFromRGBMap(trainrgbMapFacingPolyFileNameTempCharStar, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
										}
										if(generateTestPixmapFiles)
										{
											string testrgbMapFacingPolyFileNameTemp = testrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION;
											char * testrgbMapFacingPolyFileNameCharStar = const_cast<char*>(testrgbMapFacingPolyFileNameTemp.c_str());
											generatePixmapFromRGBMap(testrgbMapFacingPolyFileNameCharStar, imageWidthFacingPoly, imageHeightFacingPoly, rgbMapFacingPoly);
										}
										*/

										if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
										{

											string convertPPMtoPNGCommand = "convert " + trainrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION + " " + trainrgbMapFacingPolyFileNameCPlusWithoutExt + PNG_EXTENSION;
											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
											{
												cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
											}
											system(convertPPMtoPNGCommand.c_str());
											string convertPPMtoPNGCommand2 = "convert " + testrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION + " " + testrgbMapFacingPolyFileNameCPlusWithoutExt + PNG_EXTENSION;
											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
											{
												cout << "system(" << convertPPMtoPNGCommand2 << ");" << endl;
											}

											system(convertPPMtoPNGCommand2.c_str());
											
											#define DEBUG_OR_IMAGE_COMPARISON_LIST_MATCHED_FILE_NAMES
											#ifdef DEBUG_OR_IMAGE_COMPARISON_LIST_MATCHED_FILE_NAMES
											string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION + "</TD><TD><img src=\"" + testrgbMapFacingPolyFileNameCPlusWithoutExt + PNG_EXTENSION + "\" border=0> </TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION + "</TD><TD><img src=\"" + trainrgbMapFacingPolyFileNameCPlusWithoutExt + PNG_EXTENSION + "\" border=0> </TD>";											
											#else
											string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testsideIndexString + "</TD><TD><img src=\"" + testrgbMapFacingPolyFileNameCPlusWithoutExt + PNG_EXTENSION + "\" border=0> </TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainsideIndexString + "</TD><TD><img src=\"" + trainrgbMapFacingPolyFileNameCPlusWithoutExt + PNG_EXTENSION + "\" border=0> </TD>";
											#endif
											
											#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_COMPARISON_DCT_TABLES_TO_HTML
											string DCTTableHTMLOutputString;
											setpointerToDCTTableHTMLOutputString(&DCTTableHTMLOutputString);
											string testrgbMapSmallFacingPolyFileNameJPEGCPlusTemp = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + ".temp" + JPG_EXTENSION;
											string trainrgbMapSmallFacingPolyFileNameJPEGCPlusTemp = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + ".temp" + JPG_EXTENSION;
											string testrgbMapSmallFacingPolyFileNamePPMCPlusTemp = testinterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TEST_STRING + PPM_EXTENSION;
											string trainrgbMapSmallFacingPolyFileNamePPMCPlusTemp = traininterpolatedRGBMapFileNameForRayTracing + SMALL_MAP_EXTENSION_PART + RGB_MAP_PPM_EXTENSION_PART + TRAIN_STRING + PPM_EXTENSION;
											int dctCoeffArrayHeight = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;
											int dctCoeffArrayWidth = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D;	
											string testconvertSmallImageRGBtoJPEGCommandTemp = ""; 
											string trainconvertSmallImageRGBtoJPEGCommandTemp = "";
											testconvertSmallImageRGBtoJPEGCommandTemp = testconvertSmallImageRGBtoJPEGCommandTemp + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_TEMP_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING + " " + testrgbMapSmallFacingPolyFileNamePPMCPlusTemp + " " + testrgbMapSmallFacingPolyFileNameJPEGCPlusTemp;
											trainconvertSmallImageRGBtoJPEGCommandTemp = trainconvertSmallImageRGBtoJPEGCommandTemp + "convert " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_TEMP_STRING + " " + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING + " " + trainrgbMapSmallFacingPolyFileNamePPMCPlusTemp + " " + trainrgbMapSmallFacingPolyFileNameJPEGCPlusTemp;
											system(testconvertSmallImageRGBtoJPEGCommandTemp.c_str());
											system(trainconvertSmallImageRGBtoJPEGCommandTemp.c_str());												
											char * testrgbMapSmallFacingPolyFileNameJPEGCPlusTempCharStar = const_cast<char*>(testrgbMapSmallFacingPolyFileNameJPEGCPlusTemp.c_str());
											char * trainrgbMapSmallFacingPolyFileNameJPEGCPlusTempCharStar = const_cast<char*>(trainrgbMapSmallFacingPolyFileNameJPEGCPlusTemp.c_str());
											signed char * tempNullSignedCharArray = NULL;
											readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(testrgbMapSmallFacingPolyFileNameJPEGCPlusTempCharStar, tempNullSignedCharArray, tempNullSignedCharArray, tempNullSignedCharArray, dctCoeffArrayHeight, dctCoeffArrayWidth, false);
											readVerySmallHighlyCompressedJPEGfileAndStoreDCTcoefficients(trainrgbMapSmallFacingPolyFileNameJPEGCPlusTempCharStar, tempNullSignedCharArray, tempNullSignedCharArray, tempNullSignedCharArray, dctCoeffArrayHeight, dctCoeffArrayWidth, false);
											ICRmatchRow = ICRmatchRow +  "<TD>" + DCTTableHTMLOutputString + "</TD></TR>";
											#else
											ICRmatchRow = ICRmatchRow + "<TD>" + averageMatchErrorAcrossSidesString + "</TD></TR>";
											#endif
											writeStringToFileObject2(ICRmatchRow, &writeFileObject);
										}
										else
										{


											string convertPPMtoPNGCommand = "convert " + trainrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION + " " + trainrgbMapFacingPolyFileNameCPlusWithoutExtSanitised + PNG_EXTENSION;
											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
											{
												cout << "system(" << convertPPMtoPNGCommand << ");" << endl;
											}
											system(convertPPMtoPNGCommand.c_str());
											string convertPPMtoPNGCommand2 = "convert " + testrgbMapFacingPolyFileNameCPlusWithoutExt + PPM_EXTENSION + " " + testrgbMapFacingPolyFileNameCPlusWithoutExtSanitised + PNG_EXTENSION;
											if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
											{
												cout << "system(" << convertPPMtoPNGCommand2 << ");" << endl;
											}
											system(convertPPMtoPNGCommand2.c_str());
											string ICRmatchRow = "<TR><TD>" + testObjectNameArray[testObjectIndex] + "</TD><TD>" + testviewIndexString + "</TD><TD>" + testzoomIndexString + "</TD><TD>" + testpolygonIndexString + "</TD><TD>" + testsideIndexString + "</TD><TD><img src=\"" + testrgbMapFacingPolyFileNameCPlusWithoutExtSanitised + PNG_EXTENSION + "\" border=0> </TD><TD>" + trainobjectIndexString + "</TD><TD>" + trainviewIndexString + "</TD><TD>" + trainzoomIndexString + "</TD><TD>" + trainpolygonIndexString + "</TD><TD>" + trainsideIndexString + "</TD><TD><img src=\"" + trainrgbMapFacingPolyFileNameCPlusWithoutExtSanitised + PNG_EXTENSION + "\" border=0> </TD> <TD>" + averageMatchErrorAcrossSidesString + "</TD></TR>";
											writeStringToFileObject2(ICRmatchRow, &writeFileObject);
										}

									}
								}
							}
							//cout << "h3" << endl;

						}
					}
				#endif


				}
				averageMatchError = averageMatchError/(numberOfTestPolys[(testObjectIndex*numberOfTestViewIndicies*numberOfTestZoomIndicies)+(testViewIndex*numberOfTestZoomIndicies) + testZoomIndex] * OR_METHOD_POLYGON_NUMBER_OF_SIDES);
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
		writeStringToFileObject2(ICRtableEnd, &writeFileObject);

		string ICRfooter  = "</BODY></HTML>";
	}
	writeFileObject.close();

	delete trainrgbMap;
	delete testrgbMap;
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

//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	delete testdctCoeffArrayY;
	delete testdctCoeffArrayYCr;
	delete testdctCoeffArrayYCb;
	delete testconcatonatedSignedDctCoeffArrayRequirement;
//#endif

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


	ObjectReferenceList * currentReferenceInSnapshotMatchObjectReferenceList = firstReferenceInSnapshotMatchObjectReferenceList;
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
	
	cout << "******************************** OR Results ******************************** " << endl;						
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





#define VERY_HIGH_ERROR (99999999999999.0)



//#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML
void writeStringToFileObject2(string s, ofstream * writeFileObject)
{
	for(int i=0; i < s.size(); i++)
	{
		writeFileObject->put(s[i]);
	}
}
//#endif





//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
void convertNormalisedHueDeviationMapTo3x8BitMap(int imageWidth, int imageHeight, double * rgbDevIEnormalisedHueContrastMapSmallFacingPoly, unsigned char * rgbDev8BitSmallMapFacingPoly)
{
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			vec normalisedHueContrast;
			getVectorMapValue(x, y, imageWidth, rgbDevIEnormalisedHueContrastMapSmallFacingPoly, &normalisedHueContrast);

			colour cullednormalisedHueContrast;
			cullAndBinNormalisedHueContrast(&normalisedHueContrast, &cullednormalisedHueContrast);

			setRGBMapValues(x, y, imageWidth, &cullednormalisedHueContrast, rgbDev8BitSmallMapFacingPoly);

		}
	}
}



void cullAndBinNormalisedHueContrast(vec * normalisedHueContrast, colour * cullednormalisedHueContrast)
{
	if(normalisedHueContrast->x*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		cullednormalisedHueContrast->r = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		cullednormalisedHueContrast->r = normalisedHueContrast->x*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
	if(normalisedHueContrast->y*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		cullednormalisedHueContrast->g = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		cullednormalisedHueContrast->g = normalisedHueContrast->y*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
	if(normalisedHueContrast->z*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER > OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		cullednormalisedHueContrast->b = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else
	{
		cullednormalisedHueContrast->b = (normalisedHueContrast->z)*OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;
	}
}
//#endif







//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING

	//check this routine;
void convertDCTCoeffIndividualArraysToConcatonatedSignedDctCoeffArray(signed char dctCoeffArrayY[], signed char dctCoeffArrayYCr[], signed char dctCoeffArrayYCb[], signed char concatonatedSignedDctCoeffArray[])
{
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y; i++)
	{
		int xIndex = dctCoeffSelectionArrayY[i][0];
		int yIndex = dctCoeffSelectionArrayY[i][1];

		concatonatedSignedDctCoeffArray[i] = dctCoeffArrayY[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
		//cout << "x = " << int( dctCoeffArrayY[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex]) << endl;

	}
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr; i++)
	{
		int xIndex = dctCoeffSelectionArrayYCr[i][0];
		int yIndex = dctCoeffSelectionArrayYCr[i][1];
		concatonatedSignedDctCoeffArray[i+OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y] = dctCoeffArrayYCr[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
		//cout << "y = " << int( dctCoeffArrayYCr[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex]) << endl;
	}
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb; i++)
	{
		int xIndex = dctCoeffSelectionArrayYCb[i][0];
		int yIndex = dctCoeffSelectionArrayYCb[i][1];

		concatonatedSignedDctCoeffArray[i+OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y+OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr] = dctCoeffArrayYCb[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex];
		//cout << "z = " << int( dctCoeffArrayYCb[yIndex*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D + xIndex]) << endl;
	}

	/*
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
	      cout << "concatonatedSignedDctCoeffArray[i] = " << int(concatonatedSignedDctCoeffArray[i]) << endl;
	}
	*/
	

}




//#endif
