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
 * File Name: ORrules.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3e2c 29-August-2014
 *
 *******************************************************************************/

#include "ORrules.h"
#include "ORglobalDefs.h"
#include "XMLrulesClass.h"
#include "RTglobalDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;



bool convertStringToBool(string stringValue)
{
	bool result = false;

	for(int i=0; i<stringValue.length(); i++)
	{
		stringValue[i] = tolower(stringValue[i]);
	}

	if(stringValue == "true")
	{
		result = true;
	}
	else if(stringValue == "false")
	{
		result = false;
	}
	else
	{
		cout << "invalid string value in OR rules XML - \"true\" or \"false\" expected" << endl;
		exit(0);
	}
	return result;
}


void fillInORrulesExternVariables()
{


	/************** 0. PREPARE STANDARD XML RULES (SET DEFAULTS) *********************/


	//OR_IMAGE_COMPARISON_DECISION_TREE...

	OR_IMAGE_COMPARISON_DECISION_TREE = true;

	OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING = true;
	OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING = false;
	OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
	OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON = false;

	OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING = true;
	OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING = false;
	OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
	OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON = false;

	OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL = true;		//tested - default ON [this should be able to be turned off for optimisation purposes, it needs to fix up memory leak however] - this will not be changable in xml file, it should never need to be changed in release versions
	OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE = true;	//tested - default ON [turn this off to display html results file properly] - this will not be changable in xml file, it should never need to be changed in release versions


	//OR_FEATURES...

	//xml determined by default;
	OR_USE_FIND_CORNER_FEATURES = true;
	OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES = true;
	OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES = false;
	OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY = false;

	OR_METHOD_USE_MESH_LISTS = false;   //[has not been tested with centred feature detection]
	OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED = false;
	OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE = 0.1;
	OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED = false;		//never use this - not yet implemented

	MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE = (20);					 //this needs to be made dynamic based upon vi
	MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY = (2);
	OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE = false;		 //tested - default OFF
	OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE = (0.1);					  //this needs to be made dynamic based upon vi
	OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = (OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE);  //this needs to be made dynamic based upon vi 	  //this needs to be checked, this value might need to be increased slightly
	OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT = (0.3);				  //this needs to be made dynamic based upon vi
	OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE = (1.0);					  //this needs to be made dynamic based upon zoom??
	OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = (OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE);  //this needs to be made dynamic based upon zoom??	  //this needs to be checked, this value might need to be increased slightly



	//OR_METHOD3DOD FEATURE...

	OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST = true;
	OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION = false;

	OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE = (0.001);



	//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION = false;



	//OR_SHARED_VARS...	//general contrast threshold constraints

	OR_USE_CONTRAST_CALC_METHOD_C = true;				  //tested - default ON		//OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with OR_USE_CONTRAST_CALC_METHOD_B
	OR_USE_CONTRAST_CALC_METHOD_B = false;				  //tested - default OFF	//OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with OR_USE_CONTRAST_CALC_METHOD_B

	LUMINOSITY_CONTRAST_FRACTION_THRESHOLD = (0.05); 	 //default 0.05 for method C, default 0.02 for method B
	DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD = (0.05);	 //default 0.05 for method C, default 0.02 for method B  //this needs to be dynamic - not static - and should be dependant upon focal length
	DEPTH_CONTRAST_FRACTION_THRESHOLD = (0.05);		 //default 0.05 for method C, default 0.02 for method B  //this needs to be dynamic - not static - and should be dependant upon focal length
	POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD = (0.1);	 //default 0.1 for method C, default 0.02 for method B

	LUMINOSITY_FRACTION_THRESHOLD = (0.01);
	OR_MAX_DEPTH_NOISE = (0.1);
	ESTIMATE_MAX_DEPTH_T_REAL = (20.0);



	//OR_METHOD2DOD...

	OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS = true;



	//OR_METHOD...

	OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING = true;

	OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = 10000;
	OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST = 10000;
	OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = 10000;
	OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST = 10000;



	//LD_OPENGL...

	OR_SNAPSHOT_WINDOW_POSITION_X = (0);	     	//dependent upon resolution of monitor - should be moved to command line parameter
	OR_SNAPSHOT_WINDOW_POSITION_Y = (800);	     //dependent upon resolution of monitor - should be moved to command line parameter

	//OR_IMAGE_COMPARISON...

	OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = (0.0);			  //not tested or used yet
	OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = (0.0); 		  //not tested or used yet
	OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = (30.0); 	  //5
	OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = (20.0);
	IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE = (68.2656);
	OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF = (10.0);
	OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS = (100.0/(28*28/2));			  //OLD: (100.0/(28*28))
	OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF = (10.0);			  //needs to be tested

	//OR_OPERATIONS...

	XYCOORDINATES_CONVERSION_INTO_PIXELS = (0.05);    //default 0.0002
	MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD = (5);  //in pixel units

	//OR_PIXEL_MAPS...

	ESTIMATE_MAX_DEPTH_GRADIENT = (0.1);
	DEPTH_GRADIENT_CONTRAST_THRESHOLD = (0.1);

	//OR_QUADRATIC_FIT...

	MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE = (10.0);
	MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO = (0.7);
	USE_EDGISE_3X3_KERNEL = true;
	USE_EDGISE_5X5_KERNEL = false;
	A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL = (-20.0);				  //(-EDGE_LUMINOSITY_CONTRAST_THRESHOLD/5)
	A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL = (-30.0);				  //(-EDGE_LUMINOSITY_CONTRAST_THRESHOLD/5)
	HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD = (256*1.5);							  //NB this represents the optimised values for 2DOD, consider/test using (256*1.0) for 3DOD...!!



	//OR OPTIMISATION IMAGE CROPPING VARIABLES...

	WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE = (104);
	OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO = (4);
	OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO = (5);  //has to result in a small image <= 8x8 pixels (single DCT block)


	//OR ACCURACY/TUNING VARIABLES...

	OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD = (0.2);


	//OR DISPLAY VARIABLES...

	OR_PRINT_ALGORITHM_PROGRESS = false;		    //true in xml/testing
	OR_PRINT_ALGORITHM_AND_TIME_DETAILS = false;	    //true in xml/testing
	OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL = false;

	OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = false; 			 //true in xml/testing
	OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL = false;   	 //true in xml/testing
	OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS = false;     //true in xml/testing
	DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = false;	    //for 2DOD, this will stop data outside of the normalised triangle object data from being culled
	strcpy(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME, "ORResults.html");


	//OR TRANSFORMATION ACCURACY VARIABLES...

	OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = true;
	OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = true;
	POLYGON_MIN_ANGLE_DEGREES = (10.0);

	OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = (0.1);	//OLD; 20  //this needs to be made dynamic in the future! [based upon distance object is away from pov]
	OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = (20.0);	  //100 //20



	//OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...

	OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL = (20);
	OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER = (5);



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY = false;		    //untested - default OFF
	OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY = false;	    //untested - default OFF

	OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR = (0.3);
	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE = (6.0);



	//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

	OR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD = 0.2;


	//OR SPARE PARAMETERS VARIABLES...

	OR_RULES_XML_SPARE_PARAMETER_1 = false;
	OR_RULES_XML_SPARE_PARAMETER_2 = (0.0);


	/************** 1. PARSE STANDARD XML RULES *********************/


	//Standard OR Rules Entries;
	RulesClass * currentReferenceRulesClass = ORrulesObjectRecognition;
	while(currentReferenceRulesClass->next != NULL)
	{
		//OR_IMAGE_COMPARISON_DECISION_TREE...

		if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_DECISION_TREE_NAME)
		{
			OR_IMAGE_COMPARISON_DECISION_TREE = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_NAME)
		{
			OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING_NAME)
		{
			OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NAME)
		{
			OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_NAME)
		{
			OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NAME)
		{
			OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING_NAME)
		{
			OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NAME)
		{
			OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON_NAME)
		{
			OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL_NAME)
		{
			OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_NAME)
		{
			OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}


		//OR_FEATURES...

		else if(currentReferenceRulesClass->name == OR_USE_FIND_CORNER_FEATURES_NAME)
		{
			OR_USE_FIND_CORNER_FEATURES = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_NAME)
		{
			OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_NAME)
		{
			OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_NAME)
		{
			OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == OR_METHOD_USE_MESH_LISTS_NAME)
		{
			OR_METHOD_USE_MESH_LISTS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED_NAME)
		{
			OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE_NAME)
		{
			OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED_NAME)
		{
			OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE_NAME)
		{
			MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY_NAME)
		{
			MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE_NAME)
		{
			OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_NAME)
		{
			OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2_NAME)
		{
			OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT_NAME)
		{
			OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_NAME)
		{
			OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2_NAME)
		{
			OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = currentReferenceRulesClass->fractionalValue;
		}



		//OR_METHOD3DOD FEATURE...

		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST_NAME)
		{
			OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION_NAME)
		{
			OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION = convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE_NAME)
		{
			OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE = currentReferenceRulesClass->fractionalValue;
		}


		//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION = convertStringToBool(currentReferenceRulesClass->stringValue);
		}



		//OR_SHARED_VARS...	//general contrast threshold constraints


		else if(currentReferenceRulesClass->name == OR_USE_CONTRAST_CALC_METHOD_C_NAME)
		{
			OR_USE_CONTRAST_CALC_METHOD_C =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_USE_CONTRAST_CALC_METHOD_B_NAME)
		{
			OR_USE_CONTRAST_CALC_METHOD_B =  convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == LUMINOSITY_CONTRAST_FRACTION_THRESHOLD_NAME)
		{
			LUMINOSITY_CONTRAST_FRACTION_THRESHOLD =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD_NAME)
		{
			DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == DEPTH_CONTRAST_FRACTION_THRESHOLD_NAME)
		{
			DEPTH_CONTRAST_FRACTION_THRESHOLD =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD_NAME)
		{
			POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == LUMINOSITY_FRACTION_THRESHOLD_NAME)
		{
			LUMINOSITY_FRACTION_THRESHOLD =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_MAX_DEPTH_NOISE_NAME)
		{
			OR_MAX_DEPTH_NOISE =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ESTIMATE_MAX_DEPTH_T_REAL_NAME)
		{
			ESTIMATE_MAX_DEPTH_T_REAL =  currentReferenceRulesClass->fractionalValue;
		}

		//OR_METHOD2DOD...

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		//OR_METHOD...

		else if(currentReferenceRulesClass->name == OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING_NAME)
		{
			OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN_NAME)
		{
			OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST_NAME)
		{
			OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN_NAME)
		{
			OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST_NAME)
		{
			OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST = int(currentReferenceRulesClass->fractionalValue);
		}



		//LD_OPENGL...

		else if(currentReferenceRulesClass->name == OR_SNAPSHOT_WINDOW_POSITION_X_NAME)
		{
			OR_SNAPSHOT_WINDOW_POSITION_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_SNAPSHOT_WINDOW_POSITION_Y_NAME)
		{
			OR_SNAPSHOT_WINDOW_POSITION_Y = int(currentReferenceRulesClass->fractionalValue);
		}


		//OR_IMAGE_COMPARISON...

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE_NAME)
		{
			IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF_NAME)
		{
			OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS_NAME)
		{
			OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF_NAME)
		{
			OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF = currentReferenceRulesClass->fractionalValue;
		}

		//OR_OPERATIONS...

		else if(currentReferenceRulesClass->name == XYCOORDINATES_CONVERSION_INTO_PIXELS_NAME)
		{
			XYCOORDINATES_CONVERSION_INTO_PIXELS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD_NAME)
		{
			MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD = currentReferenceRulesClass->fractionalValue;
		}

		//OR_PIXEL_MAPS...

		else if(currentReferenceRulesClass->name == ESTIMATE_MAX_DEPTH_GRADIENT_NAME)
		{
			ESTIMATE_MAX_DEPTH_GRADIENT = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == DEPTH_GRADIENT_CONTRAST_THRESHOLD_NAME)
		{
			DEPTH_GRADIENT_CONTRAST_THRESHOLD = currentReferenceRulesClass->fractionalValue;
		}

		//OR_QUADRATIC_FIT...

		else if(currentReferenceRulesClass->name == MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE_NAME)
		{
			MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO_NAME)
		{
			MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == USE_EDGISE_3X3_KERNEL_NAME)
		{
			USE_EDGISE_3X3_KERNEL = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == USE_EDGISE_5X5_KERNEL_NAME)
		{
			USE_EDGISE_5X5_KERNEL = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL_NAME)
		{
			A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL_NAME)
		{
			A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD_NAME)
		{
			HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD = currentReferenceRulesClass->fractionalValue;
		}



		//OR OPTIMISATION IMAGE CROPPING VARIABLES...

		else if(currentReferenceRulesClass->name == WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE_NAME)
		{
			WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO_NAME)
		{
			OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO_NAME)
		{
			OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO = int(currentReferenceRulesClass->fractionalValue);
		}


		//OR DISPLAY VARIABLES...

		else if(currentReferenceRulesClass->name == OR_PRINT_ALGORITHM_PROGRESS_NAME)
		{
			OR_PRINT_ALGORITHM_PROGRESS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_PRINT_ALGORITHM_AND_TIME_DETAILS_NAME)
		{
			OR_PRINT_ALGORITHM_AND_TIME_DETAILS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL_NAME)
		{
			OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_NAME)
		{
			OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL_NAME)
		{
			OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS_NAME)
		{
			OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW_NAME)
		{
			DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME_NAME)
		{
			strcpy(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME, (currentReferenceRulesClass->stringValue).c_str());
		}

		//OR ACCURACY/TUNING VARIABLES...

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD_NAME)
		{
			OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD = currentReferenceRulesClass->fractionalValue;
		}

		//OR TRANSFORMATION ACCURACY VARIABLES...

		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST_NAME)
		{
			OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST_NAME)
		{
			OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == POLYGON_MIN_ANGLE_DEGREES_NAME)
		{
			POLYGON_MIN_ANGLE_DEGREES = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON_NAME)
		{
			OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON_NAME)
		{
			OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = currentReferenceRulesClass->fractionalValue;
		}


		//OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...


		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL_NAME)
		{
			OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER_NAME)
		{
			OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR OPTIMISATION GEOMETRIC COMPARISON...

		else if(currentReferenceRulesClass->name == OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY_NAME)
		{
			OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY_NAME)
		{
			OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR_NAME)
		{
			OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE_NAME)
		{
			OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE = currentReferenceRulesClass->fractionalValue;
		}


		//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD_NAME)
		{
			OR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD = currentReferenceRulesClass->fractionalValue;
		}


		//OR SPARE PARAMETERS VARIABLES...

		else if(currentReferenceRulesClass->name == OR_RULES_XML_SPARE_PARAMETER_1_NAME)
		{
			OR_RULES_XML_SPARE_PARAMETER_1 = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_RULES_XML_SPARE_PARAMETER_2_NAME)
		{
			OR_RULES_XML_SPARE_PARAMETER_2 = currentReferenceRulesClass->fractionalValue;
		}




		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;

	}

	/************** 2. POST PROCESSES STANDARD XML RULES *********************/

	//OR_IMAGE_COMPARISON_DECISION_TREE...

	OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = false;
	OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = false;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = false;
	OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = false;

	if(OR_IMAGE_COMPARISON_DECISION_TREE)
	{
		OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING = false;
		OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING = false;
		OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = false;
		OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON = false;

		if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = true;
		}

		if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
		{
			OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = true;
		}

		if(OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
		}

		if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
		{
			OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = true;
		}
	}
	else
	{
		OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING = false;
		OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING = false;
		OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = false;
		OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON = false;

		if(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING)
		{
			OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = true;
		}

		if(OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING)
		{
			OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = true;
		}

		if(OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
		}

		if(OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON)
		{
			OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = true;
		}
	}


	OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY = true;
	#ifdef OR_IMAGE_COMPARISON_SQL
	if(OR_IMAGE_COMPARISON_DECISION_TREE)
	{
		OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY = false;		//this cannot be used with decision tree activated
	}
	#endif



	//OR_METHOD_GEOMETRIC_COMPARISON...

	OR_METHOD_TRANSFORM_NEARBY_FEATURES = false;			//NEW this has been made dependent upon whether geo comparison is used.

	OR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES = false;	//this can be used with OR_IMAGE_COMPARISON_DECISION_TREE as nearby feature data is not taken and used from snapshot tables [only used in the definition of decision tree tables]

	OR_METHOD_TRANSFORM_KEY_OT_FEATURES = false;
	OR_METHOD_TRANSFORM_ALL_FEATURES = false;
	OR_METHOD_CREATE_OT_FEATURES_FILE = false;
	OR_METHOD_CREATE_ALL_FEATURES_FILE = false;

	OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = false;
	OR_METHOD_CREATE_NEARBY_FEATURES_FILE = false;
	OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = false;
	OR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST = false;

#ifdef OR_METHOD_GEOMETRIC_COMPARISON

	if(OR_METHOD_TRANSFORM_KEY_OT_FEATURES)
	{
		OR_METHOD_CREATE_OT_FEATURES_FILE = true;
	}

	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		OR_METHOD_CREATE_ALL_FEATURES_FILE = true;
	}

	#ifdef OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO

		OR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

		OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = true;

		OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
		if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
		{
			OR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
		}

	#elif defined OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

		OR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

		OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = true;

		OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
		if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
		{
			OR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
		}

	#elif defined OR_METHOD_GEOMETRIC_COMPARISON_NON_OPTIMISED_FILE_IO

		OR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

	#elif defined TEMPTEST3GEOACCURACY

		OR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

		if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
		{
			OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
		}

	#else
		if(OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
		{
			OR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

			if(!OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
			{
				OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = true;
				OR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST = true;
				if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST)
				{
					OR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
				}
				//OR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES = true
			}

			#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES

			OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
			if(OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
			{
				OR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
			}

			#endif
		}

	#endif
#endif



	//OR_SHARED_VARS...	//general contrast threshold constraints

	if(OR_USE_CONTRAST_CALC_METHOD_B)
	{
		if(OR_USE_CONTRAST_CALC_METHOD_C)
		{
			cout << "error - cannot define two contrast calculation methods in OR rules xml file" << endl;
			exit(0);
		}
	}
	MAX_LUMINOSITY_CONTRAST = 0;
	MAX_NORMAL_CONTRAST = 0;
	ESTIMATE_MAX_DEPTH_CONTRAST = 0;
	ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = 0;

	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		MAX_LUMINOSITY_CONTRAST = (MAX_LUMINOSITY*4);
		MAX_NORMAL_CONTRAST = (MAX_NORMAL*3.0*4);
		ESTIMATE_MAX_DEPTH_CONTRAST = (ESTIMATE_MAX_DEPTH_T_REAL);
		ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = (ESTIMATE_MAX_DEPTH_CONTRAST*4.0);	//OLD: 0.1 instead

	}
	else if(OR_USE_CONTRAST_CALC_METHOD_B)
	{
		MAX_LUMINOSITY_CONTRAST = (MAX_LUMINOSITY*8);
		MAX_NORMAL_CONTRAST = (MAX_NORMAL*3.0*8);
		ESTIMATE_MAX_DEPTH_CONTRAST = (ESTIMATE_MAX_DEPTH_T_REAL*8.0);
		ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = (ESTIMATE_MAX_DEPTH_CONTRAST*8.0*2.0);
	}
	else
	{
		MAX_LUMINOSITY_CONTRAST = (MAX_LUMINOSITY);
		MAX_NORMAL_CONTRAST = (MAX_NORMAL*3.0);
		ESTIMATE_MAX_DEPTH_CONTRAST = (ESTIMATE_MAX_DEPTH_T_REAL);
		ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = (ESTIMATE_MAX_DEPTH_CONTRAST*2.0);
	}

	#ifdef USE_OLD_ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST
	ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = 0.1;
	#endif

	EDGE_LUMINOSITY_CONTRAST_THRESHOLD = (MAX_LUMINOSITY_CONTRAST*LUMINOSITY_CONTRAST_FRACTION_THRESHOLD);   //OLD = ~0.07*MAX_LUMINOSITY_CONTRAST = ~ ((20.0/254.0)*MAX_LUMINOSITY_CONTRAST)
	EDGE_LUMINOSITY_THRESHOLD = (MAX_LUMINOSITY*LUMINOSITY_FRACTION_THRESHOLD);
	EDGE_NORMAL_CONTRAST_THRESHOLD = (MAX_NORMAL_CONTRAST*POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD);
	EDGE_DEPTH_CONTRAST_THRESHOLD = (ESTIMATE_MAX_DEPTH_CONTRAST*DEPTH_CONTRAST_FRACTION_THRESHOLD);
#ifdef USE_OLD_ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST
	EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD = (ESTIMATE_MAX_DEPTH_CONTRAST*DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD);
#else
	EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD = (ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST*DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD);
#endif
	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT = (0);
		INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST = (1);
		INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST = (2);
	}
	else
	{
		INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT = (0);
		INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST = (0);
		INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST = (0);
	}



	//OR_PIXEL_MAPS...

	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_INTERPIXEL = (true);
		DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH = (DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL);
		DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT = (DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL);
	}
	else
	{
		DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_INTERPIXEL = (false);
		DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH = (DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL);
		DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT = (DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT_NO_INTERPIXEL);
	}

	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		DEFAULT_NORMAL_MAP_GENERATION_INTERPIXEL = (true);
		DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH = (NORMAL_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL);
		DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT = (NORMAL_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL);
	}
	else
	{
		DEFAULT_NORMAL_MAP_GENERATION_INTERPIXEL = (false);
		DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH = (NORMAL_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL);
		DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT = (NORMAL_MAP_GENERATION_KERNEL_HEIGHT_NO_INTERPIXEL);
	}


	//RT_PIXEL_MAPS...

	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		DEFAULT_CONTRAST_MAP_GENERATION_INTERPIXEL = (true);
		DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_WIDTH = (CONTRAST_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL);
		DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_HEIGHT = (CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL);
	}
	else
	{
		DEFAULT_CONTRAST_MAP_GENERATION_INTERPIXEL = (false);
		DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_WIDTH = (CONTRAST_MAP_GENERATION_KERNEL_WIDTH_NO_INTERPIXEL);
		DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_HEIGHT = (CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_NO_INTERPIXEL);
	}




	//OR_IMAGE_COMPARISON...

	IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR = (IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE * 3);
	//#define OR_IMAGE_COMPARISON_MIN_TOTAL_NUM_STARK_LOCAL_CONTRASTS (100.0/(28*28/2))



	//OR_OPERATIONS...
	MAX_FEATURE_DISTANCE_ERROR_USING_POINT_MAP_METHOD = (XYCOORDINATES_CONVERSION_INTO_PIXELS * MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD);	//in world units		//OLD;(0.001)



	//OR_QUADRATIC_FIT...
	HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD = (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/4.0);		//NB this represents the optimised values for 2DOD, consider/test using (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/4.0) for 3DOD...!!
	HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD = (HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD*10.0);

	if(USE_EDGISE_5X5_KERNEL)
	{
		QUADRATIC_FIT_KERNEL_SIZE = (QUADRATIC_FIT_KERNEL_SIZE_5X5);
		ZERO_CROSSING_POSITION_THRESHOLD = (HALF_PIXEL_WIDTH);
		A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD = (A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL);
	}
	else if(USE_EDGISE_3X3_KERNEL)
	{
		QUADRATIC_FIT_KERNEL_SIZE = (QUADRATIC_FIT_KERNEL_SIZE_3X3);
		ZERO_CROSSING_POSITION_THRESHOLD = (HALF_PIXEL_WIDTH);
		A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD = (A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL);
	}
	else
	{
		cout << "OR rules XML error: a quadratic fit kernel must be defined" << endl;
	}



	//OR DISPLAY VARIABLES...

	if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{
		if(!OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
		{
			DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = true;
		}
	}

	#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
	OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = false;
	#endif



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE = (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE*2.0);
	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE = (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE*2.0);

	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION = (OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR);
	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION = (OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR);

	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS = ((long)(((double)OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE)/OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION)); //(40)
	OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS = ((long)(((double)OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE)/OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION)); //(40)



	//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...
	OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON = false;
	if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON = true;
	}



	/************** 3. PREPARE OPTIONAL XML RULES (SET DEFAULTS)  *********************/


	//OR_FEATURES...

	//dynamically determined by default (these do not need to be defined in the xml file);
	OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
	OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
	OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = false;
	OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
	OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
	OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = false;
	OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = false;
	OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = false;
	OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = false;
	OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = false;

	if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
	{

		//mesh list quadratic fit for accurate edge calculation and centre feature calculations
		if(OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY)
		{
			OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = true;		//THIS IS OPTIONAL [has been tested with centred feature detection]
			OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = true;		//THIS IS OPTIONAL [has been tested with centred feature detection]
		}
		else
		{
			OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
			OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
		}

		if(OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
		{
			OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = true;
			OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = true;
		}
		else
		{
			OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
		}

		if(OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
		{
			OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = true;
			OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = true;
		}
		else
		{
			OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
		}

		if(OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES)
		{
			OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = true;

			//#define OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OLD
			OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = true;

			if(OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
			{
				OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = true;	//this will not be used in the future as edge points calculated will have depth filtering applied based upon contiguous boundary edge trace
			}

			OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = true;	//TESTHING THIS now!

		}

		//mesh lists (only currently used for centred feature calculations in combined mesh lists)
			//the following is only valid when OR_DEBUG_METHOD_ASSUME_TRAIN_AND_TEST_WITH_SINGLE_VIEWPOINT is not defined
		if(OR_METHOD_USE_MESH_LISTS)
		{
			/*consider these in the future...
			#ifdef TRY_UNTESTED_SOFTWARE2
				#define OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED	//probably need to verify quadratic fit edges are calculated properly upon mesh advanced join
				#define OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE (0.1) //this needs to be made dynamic based upon vi
				//#define OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED		//dont use	//not yet implemented (low priority) - there is no way of combining 2D mesh lists without eg x/y offset information...
			#endif
			*/
			OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = true;
		}
	}



	//OR_METHOD3DOD FEATURE...

	OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = false;
	if(OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION)
	{
		OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = true;
	}




	//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...
	//dynamically determined by default (these do not need to be defined in the xml file);

	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = false;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = false;

	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X = 2;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X = 2;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y = 2;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y = 2;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y = -1;

	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y = -1;

	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X = 0;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y = 1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X = -1;
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y = -1;

	if(OR_IMAGE_COMPARISON_DECISION_TREE)
	{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = (2);	//must be even! [not odd]
	#else
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = (1);
	#endif
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = (32);		//6bit signed; -16->+15 	//num DCT bins per dimension
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = (6);

		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = 10;				//deterministic dct coeff comparison requirement
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = 50; 	//deterministic dct coeff comparison requirement
	}
	else
	{
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = (1);		//+/- 8 requires highly compressed JPEG data

	#ifdef OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V4
		//OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL is used, therefore OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL must be max 64bit
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = (16);		//5bit signed; -8->+7 	//num DCT bins per dimension
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = (5);
	#else
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = (32);		//6bit signed; -16->+15 	//num DCT bins per dimension
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = (6);
	#endif

		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = 10;				//deterministic dct coeff comparison requirement
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = 50; 	//deterministic dct coeff comparison requirement

	}


	//#define OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ABS_LUM_INFORMATION	//this cannot be used as lighting intensity and/or colour varies between scenes

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = true;
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = true;	//even with matched Lum:chroma subsampling, chroma data is still too compressed, so need to artificially raise the quality of the jpeg compression to determine the colour DCT coefficients
	}
	else
	{
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = false;
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = false;	//even with matched Lum:chroma subsampling, chroma data is still too compressed, so need to artificially raise the quality of the jpeg compression to determine the colour DCT coefficients
	}

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = (7);
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = (3);
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = (3);
	}
	else
	{
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = (7);
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = (0);
		OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = (0);
	}



	//OR_METHOD2DOD...

	if(OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS)
	{
		//either no cull or square cull is required for fourier transform
		OR_METHOD_DO_NOT_CULL_SNAPSHOT = true;			//square cull
		OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = false;		//no cull
	}
	else
	{
		OR_METHOD_DO_NOT_CULL_SNAPSHOT = false;			//square cull
		OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = false;		//no cull
	}



	//OR_METHOD...

	OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = false;
	OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;
	OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = false;
	OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;

	if(OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING)
	{
		OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = true;
		//OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = true;

		if(OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS)
		{
			OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 3;
		}
		else
		{
			OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;
		}

		if(OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS)
		{
			OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 3;
		}
		else
		{
			OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;
		}

	}


	//OR OPTIMISATION IMAGE CROPPING VARIABLES...

	OR_METHOD_2DOD_NORM_SNAPSHOT_X = (OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO*(OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE-1));	//Eg 28	[must be divisible by OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO]
	OR_METHOD_2DOD_NORM_SNAPSHOT_Y = (OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO*(OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE-1)); 	//Eg 28	[must be divisible by OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO]

	OR_METHOD_3DOD_NORM_SNAPSHOT_X = (OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO*OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE);		//eg 40
	OR_METHOD_3DOD_NORM_SNAPSHOT_Y = (OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO*OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE);		//eg 40



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	if(OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY)
	{
		NUMBER_OF_POLYGONS_PER_FEATURE = (1); 		//default; 1
	}
	else
	{
		NUMBER_OF_POLYGONS_PER_FEATURE = (3); 		//default; 3
	}

	if(OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY)
	{
		OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = (2);	//default; 4, 10 will reduce speed but increase redundancy [due to bug?]
	}
	else
	{
		OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = (4);	//default; 4, 10 will reduce speed but increase redundancy [due to bug?]
	}




	/************** 4. SET OPTIONAL XML RULES *********************/

	//Extra OR Rules Entries;
		//now allow for override of default dependencies;
	currentReferenceRulesClass = ORrulesObjectRecognition;
	while(currentReferenceRulesClass->next != NULL)
	{

		//OR_FEATURES...
		if(currentReferenceRulesClass->name == OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_NAME)
		{
			OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_NAME)
		{
			OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED_NAME)
		{
			OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL_NAME)
		{
			OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL_NAME)
		{
			OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS_NAME)
		{
			OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT_NAME)
		{
			OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE_NAME)
		{
			OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET_NAME)
		{
			OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST_NAME)
		{
			OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = convertStringToBool(currentReferenceRulesClass->stringValue);
		}



		//OR_METHOD3DOD FEATURE...
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST_NAME)
		{
			OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = convertStringToBool(currentReferenceRulesClass->stringValue);
		}



		//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y_NAME)
		{
			OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR_METHOD2DOD...

		else if(currentReferenceRulesClass->name == OR_METHOD_DO_NOT_CULL_SNAPSHOT_NAME)
		{
			OR_METHOD_DO_NOT_CULL_SNAPSHOT = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME_NAME)
		{
			OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		//OR_METHOD...

		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS_NAME)
		{
			OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS_NAME)
		{
			OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS_NAME)
		{
			OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS_NAME)
		{
			OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR OPTIMISATION IMAGE CROPPING VARIABLES...

		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_NORM_SNAPSHOT_X_NAME)
		{
			OR_METHOD_2DOD_NORM_SNAPSHOT_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_2DOD_NORM_SNAPSHOT_Y_NAME)
		{
			OR_METHOD_2DOD_NORM_SNAPSHOT_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_NORM_SNAPSHOT_X_NAME)
		{
			OR_METHOD_3DOD_NORM_SNAPSHOT_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_3DOD_NORM_SNAPSHOT_Y_NAME)
		{
			OR_METHOD_3DOD_NORM_SNAPSHOT_Y = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR OPTIMISATION GEOMETRIC COMPARISON...

		else if(currentReferenceRulesClass->name == NUMBER_OF_POLYGONS_PER_FEATURE_NAME)
		{
			NUMBER_OF_POLYGONS_PER_FEATURE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_NAME)
		{
			OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = currentReferenceRulesClass->fractionalValue;
		}


		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}





	/************** 5.  POST PROCESSES OPTIONAL XML RULES *********************/


	//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET = (OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS/2);	//value to add to signed value before binning
	OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS = (OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr + OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb);	 	//only compare a selection of 15 of the 64x3 DCT coefficients (of the Y, Cr and Cb blocks) {64 bit - Y DCT; 5bit[signed]*3 + YCr DCT; 5bit[signed]*3 + YCb DCT; 5bit[signed]*7}   - OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT0 d0, OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT1 d1, OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT2 d2, OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT3 d3 ... OR_MYSQL_FIELD_NAME_DCT_COEFFICIENT15 d15

	string tempString = "";
	char tempStringCharStar[100];

	tempString = "";
	tempStringCharStar[0] = '\0';
	sprintf(tempStringCharStar, "%d", OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY);
	tempString = tempString + "-quality " + tempStringCharStar + "%";
	strcpy(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING, tempString.c_str());

	tempString = "";
	tempStringCharStar[0] = '\0';
	sprintf(tempStringCharStar, "%d", OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED);
	tempString = tempString + "-quality " + tempStringCharStar + "%";
	strcpy(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING, tempString.c_str());

	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING)
	{
		strcpy(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING, OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_ON);
	}
	else
	{

		strcpy(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING, OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_OFF);
	}

	#ifdef OR_DEBUG
	//cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING << endl;
	//cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING << endl;
	#endif


	//OR OPTIMISATION IMAGE CROPPING VARIABLES...

	if(OR_METHOD_2DOD_NORM_SNAPSHOT_X%OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO != 0)
	{
		cout << "warning; OR rules XML: OR_METHOD_2DOD_NORM_SNAPSHOT_X is not divisible by OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO" << endl;
	}
	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		if(OR_METHOD_2DOD_NORM_SNAPSHOT_X/OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO > OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE)
		{
			cout << "error; OR rules XML: OR_METHOD_2DOD_NORM_SNAPSHOT_X/OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO > OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE" << endl;
			exit(0);
		}
	}

	if(OR_METHOD_3DOD_NORM_SNAPSHOT_X%OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO != 0)
	{
		cout << "warning; OR rules XML: OR_METHOD_3DOD_NORM_SNAPSHOT_X is not divisible by OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO" << endl;
	}
	if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		if(OR_METHOD_3DOD_NORM_SNAPSHOT_X/OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO > OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE)
		{
			cout << "error; OR rules XML: OR_METHOD_3DOD_NORM_SNAPSHOT_X/OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO > OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE" << endl;
			exit(0);
		}
	}



	OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = false;
	OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = false;
	OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = 0;
	OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = 0;
	OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = 1.0;
	OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = 0;
	OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = 0;
	OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = 1.0;


	int OR_METHOD_XDOD_NORM_SNAPSHOT_X = 0;
	int OR_METHOD_XDOD_NORM_SNAPSHOT_Y = 0;
	int OR_METHOD_XDOD_SNAPSHOT_SIZE = 0;
	int OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE = 0;
	if(OR_METHOD_3DOD_NORM_SNAPSHOT_X > OR_METHOD_2DOD_NORM_SNAPSHOT_X)
	{
		OR_METHOD_XDOD_NORM_SNAPSHOT_X = OR_METHOD_3DOD_NORM_SNAPSHOT_X;
		OR_METHOD_XDOD_NORM_SNAPSHOT_Y = OR_METHOD_3DOD_NORM_SNAPSHOT_Y;
		OR_METHOD_XDOD_SNAPSHOT_SIZE = OR_METHOD_XDOD_NORM_SNAPSHOT_X*OR_METHOD_XDOD_NORM_SNAPSHOT_Y;
		OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE = ((OR_METHOD_3DOD_NORM_SNAPSHOT_X/OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO)*(OR_METHOD_3DOD_NORM_SNAPSHOT_Y/OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO));	//must choose largest snapshot out of 2DOD and 3DOD

	}
	else
	{
		OR_METHOD_XDOD_NORM_SNAPSHOT_X = OR_METHOD_2DOD_NORM_SNAPSHOT_X;
		OR_METHOD_XDOD_NORM_SNAPSHOT_Y = OR_METHOD_2DOD_NORM_SNAPSHOT_Y;
		OR_METHOD_XDOD_SNAPSHOT_SIZE = OR_METHOD_XDOD_NORM_SNAPSHOT_X*OR_METHOD_XDOD_NORM_SNAPSHOT_Y;
		OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE = ((OR_METHOD_2DOD_NORM_SNAPSHOT_X/OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO)*(OR_METHOD_2DOD_NORM_SNAPSHOT_Y/OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO));	//must choose largest snapshot out of 2DOD and 3DOD
	}


	bool minimumHorizontalWindowsSizeIsLessThanSnapshotWidth = false;
	#ifdef LINUX
		//Note linux supports windows of arbitrary size
		minimumHorizontalWindowsSizeIsLessThanSnapshotWidth = true;
	#else

		//Note Windows (XP classic theme) enforces a minimum horizontal windows size of 104 pixels
		if(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE < OR_METHOD_XDOD_NORM_SNAPSHOT_X)
		{
			minimumHorizontalWindowsSizeIsLessThanSnapshotWidth = true;
		}
	#endif


	if(minimumHorizontalWindowsSizeIsLessThanSnapshotWidth)
	{

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
			//#define OR_METHOD_DO_NOT_CULL_SNAPSHOT	- needs to be made dynamic
			OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
			OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
			OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = (OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = (OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double(OR_METHOD_2DOD_NORM_SNAPSHOT_X+(OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X*2))/double(OR_METHOD_2DOD_NORM_SNAPSHOT_X));
			OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = (OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = (OR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double((OR_METHOD_3DOD_NORM_SNAPSHOT_X+(OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X*2))/double(OR_METHOD_3DOD_NORM_SNAPSHOT_X));
		#else
			OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = false;
			OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = false;
			OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = (0);
			OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = (0);
			OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (1.0);
			OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = (0);
			OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = (0);
			OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (1.0);
		#endif

	}
	else
	{
		OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
		OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
		OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-OR_METHOD_2DOD_NORM_SNAPSHOT_X)/2);
		OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-OR_METHOD_2DOD_NORM_SNAPSHOT_Y)/2);
		OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double(OR_METHOD_2DOD_NORM_SNAPSHOT_X+(OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X*2))/double(OR_METHOD_2DOD_NORM_SNAPSHOT_X));	//#define OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH (double(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE)/double(OR_METHOD_2DOD_NORM_SNAPSHOT_X))
		OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-OR_METHOD_3DOD_NORM_SNAPSHOT_X)/2);
		OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-OR_METHOD_3DOD_NORM_SNAPSHOT_X)/2);
		OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double(OR_METHOD_3DOD_NORM_SNAPSHOT_X+(OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X*2))/double(OR_METHOD_3DOD_NORM_SNAPSHOT_X));	//#define OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH (double(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE)/double(OR_METHOD_3DOD_NORM_SNAPSHOT_X))

	}


	//OR SQL PIXMAPS VARIABLES...

	OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH = (OR_METHOD_XDOD_SNAPSHOT_SIZE*3 + OR_METHOD_XDOD_SNAPSHOT_SIZE*3 + OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE*3 + OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE*3);



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM = (OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE);

	#ifdef DEBUG_OR_PRINT_OR_RULES_EXTERN_VARS
	printORrulesExternVariables();
	#endif

}


void printORrulesExternVariables()
{

	//OR_IMAGE_COMPARISON_DECISION_TREE...

	cout << "OR_IMAGE_COMPARISON_DECISION_TREE = " << OR_IMAGE_COMPARISON_DECISION_TREE << endl;

	cout << "OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING = " << OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING = " << OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = " << OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON = " << OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING = " << OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING = " << OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = " << OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON = " << OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = " << OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = " << OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING << endl;
	cout << "OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = " << OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY = " << OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL = " << OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL << endl;
	cout << "OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE = " << OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//OR_FEATURES...

	//xml determined by default;
	cout << "OR_USE_FIND_CORNER_FEATURES = " << OR_USE_FIND_CORNER_FEATURES << endl;
	cout << "OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES = " << OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES << endl;
	cout << "OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES = " << OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES << endl;
	cout << "OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY = " << OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY << endl;
	cout << "OR_METHOD_USE_MESH_LISTS = " << OR_METHOD_USE_MESH_LISTS << endl;   //[has not been tested with centred feature detection]
	cout << "" << endl;
	//dynamically determined by default;
	cout << "OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = " << OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP << endl;
	cout << "OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = " << OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP << endl;
	cout << "OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = " << OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED << endl;
	cout << "OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = " << OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL << endl;
	cout << "OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = " << OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL << endl;
	cout << "OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = " << OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS << endl;
	cout << "OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = " << OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT << endl;
	cout << "OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = " << OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE << endl;
	cout << "OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = " << OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET << endl;
	cout << "OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = " << OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST << endl;
	cout << "" << endl;
	cout << "MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE = " << MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE << endl;
	cout << "MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY = " << MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY << endl;
	cout << "OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE = " << OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE << endl;
	cout << "OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE = " << OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE << endl;
	cout << "OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = " << OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 << endl;
	cout << "OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT = " << OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT << endl;
	cout << "OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE = " << OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE << endl;
	cout << "OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = " << OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//OR_METHOD3DOD FEATURE...

	cout << "OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST = " << OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST << endl;
	cout << "OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION = " << OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION << endl;
	cout << "OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = " << OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST << endl;
	cout << "" << endl;
	cout << "OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE = " << OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY << endl;			//deterministic dct coeff comparison requirement
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED << endl; 	//deterministic dct coeff comparison requirement
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING << endl;			//deterministic dct coeff comparison requirement
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING << endl; 	//deterministic dct coeff comparison requirement
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y << endl;
	cout << "" << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET << endl;
	cout << "OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS = " << OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//OR_SHARED_VARS...	//general contrast threshold constraints

	cout << "OR_USE_CONTRAST_CALC_METHOD_C = " << OR_USE_CONTRAST_CALC_METHOD_C << endl;		//OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with OR_USE_CONTRAST_CALC_METHOD_B
	cout << "OR_USE_CONTRAST_CALC_METHOD_B = " << OR_USE_CONTRAST_CALC_METHOD_B << endl;		//OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with OR_USE_CONTRAST_CALC_METHOD_B
	cout << "" << endl;
	cout << "LUMINOSITY_CONTRAST_FRACTION_THRESHOLD = " << LUMINOSITY_CONTRAST_FRACTION_THRESHOLD << endl;
	cout << "DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD = " << DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD << endl;	//this needs to be dynamic - not static - and should be dependant upon focal length
	cout << "DEPTH_CONTRAST_FRACTION_THRESHOLD = " << DEPTH_CONTRAST_FRACTION_THRESHOLD << endl;		//this needs to be dynamic - not static - and should be dependant upon focal length
	cout << "POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD = " << POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD << endl;
	cout << "" << endl;
	cout << "LUMINOSITY_FRACTION_THRESHOLD = " << LUMINOSITY_FRACTION_THRESHOLD << endl;
	cout << "OR_MAX_DEPTH_NOISE = " << OR_MAX_DEPTH_NOISE << endl;
	cout << "ESTIMATE_MAX_DEPTH_T_REAL = " << ESTIMATE_MAX_DEPTH_T_REAL << endl;
	cout << "" << endl;
	cout << "MAX_LUMINOSITY_CONTRAST = " << MAX_LUMINOSITY_CONTRAST << endl;
	cout << "MAX_NORMAL_CONTRAST = " << MAX_NORMAL_CONTRAST << endl;
	cout << "ESTIMATE_MAX_DEPTH_CONTRAST = " << ESTIMATE_MAX_DEPTH_CONTRAST << endl;
	cout << "ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = " << ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST << endl;
	cout << "" << endl;
	cout << "EDGE_LUMINOSITY_CONTRAST_THRESHOLD = " << EDGE_LUMINOSITY_CONTRAST_THRESHOLD << endl;
	cout << "EDGE_LUMINOSITY_THRESHOLD = " << EDGE_LUMINOSITY_THRESHOLD << endl;
	cout << "EDGE_NORMAL_CONTRAST_THRESHOLD = " << EDGE_NORMAL_CONTRAST_THRESHOLD << endl;
	cout << "EDGE_DEPTH_CONTRAST_THRESHOLD = " << EDGE_DEPTH_CONTRAST_THRESHOLD << endl;
	cout << "EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD = " << EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD << endl;
	cout << "" << endl;
	cout << "INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT = " << INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT << endl;
	cout << "INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST = " << INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST << endl;
	cout << "INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST = " << INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//OR_METHOD2DOD...

	cout << "OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS = " << OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS << endl;
	cout << "OR_METHOD_DO_NOT_CULL_SNAPSHOT = " << OR_METHOD_DO_NOT_CULL_SNAPSHOT << endl;
	cout << "OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = " << OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_METHOD...

	cout << "OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING = " << OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING << endl;
	cout << "" << endl;
	cout << "OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = " << OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS << endl;
	cout << "OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = " << OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS << endl;
	cout << "OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = " << OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS << endl;
	cout << "OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = " << OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS << endl;
	cout << "" << endl;
	cout << "OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = " << OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN << endl;
	cout << "OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST = " << OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST << endl;
	cout << "OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = " << OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN << endl;
	cout << "OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST = " << OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;



	//LD_OPENGL...

	cout << "OR_SNAPSHOT_WINDOW_POSITION_X = " << OR_SNAPSHOT_WINDOW_POSITION_X << endl;		 //dependent upon resolution of monitor - should be moved to command line parameter
	cout << "OR_SNAPSHOT_WINDOW_POSITION_Y = " << OR_SNAPSHOT_WINDOW_POSITION_Y << endl;	 //dependent upon resolution of monitor - should be moved to command line parameter
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_IMAGE_COMPARISON...

	cout << "OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = " << OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION << endl;			 //not tested or used yet
	cout << "OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = " << OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION << endl;  		 //not tested or used yet
	cout << "OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = " << OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION << endl;		 //5
	cout << "OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = " << OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION << endl;
	cout << "IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE = " << IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE << endl;
	cout << "IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR = " << IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR << endl;
	cout << "OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF = " << OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF << endl;
	cout << "OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS = " << OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS << endl;		 //need to change to (100.0/(28*28/2)) after testing
	cout << "OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF = " << OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF << endl;			 //needs to be tested
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_OPERATIONS...

	cout << "XYCOORDINATES_CONVERSION_INTO_PIXELS = " << XYCOORDINATES_CONVERSION_INTO_PIXELS << endl;	 //default 0.0002
	cout << "MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD = " << MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD << endl;	 //in pixel units
	cout << "MAX_FEATURE_DISTANCE_ERROR_USING_POINT_MAP_METHOD = " << MAX_FEATURE_DISTANCE_ERROR_USING_POINT_MAP_METHOD << endl;	 //in world units		 //OLD;(0.001)
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_PIXEL_MAPS...

	cout << "ESTIMATE_MAX_DEPTH_GRADIENT = " << ESTIMATE_MAX_DEPTH_GRADIENT << endl;
	cout << "DEPTH_GRADIENT_CONTRAST_THRESHOLD = " << DEPTH_GRADIENT_CONTRAST_THRESHOLD << endl;
	cout << "" << endl;
	cout << "DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_INTERPIXEL = " << DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_INTERPIXEL << endl;
	cout << "DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH = " << DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH << endl;
	cout << "DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT = " << DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT << endl;
	cout << "" << endl;
	cout << "DEFAULT_NORMAL_MAP_GENERATION_INTERPIXEL = " << DEFAULT_NORMAL_MAP_GENERATION_INTERPIXEL << endl;
	cout << "DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH = " << DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH << endl;
	cout << "DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT = " << DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//RT_PIXEL_MAPS...

	cout << "DEFAULT_CONTRAST_MAP_GENERATION_INTERPIXEL = " << DEFAULT_CONTRAST_MAP_GENERATION_INTERPIXEL << endl;
	cout << "DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_WIDTH = " << DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_WIDTH << endl;
	cout << "DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_HEIGHT = " << DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_HEIGHT << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_QUADRATIC_FIT...

	cout << "MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE = " << MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE << endl;
	cout << "MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO = " << MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO << endl;
	cout << "USE_EDGISE_3X3_KERNEL = " << USE_EDGISE_3X3_KERNEL << endl;
	cout << "USE_EDGISE_5X5_KERNEL = " << USE_EDGISE_5X5_KERNEL << endl;
	cout << "A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL = " << A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL << endl;  			 //(-EDGE_LUMINOSITY_CONTRAST_THRESHOLD/5)
	cout << "A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL = " << A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL << endl;  			 //(-EDGE_LUMINOSITY_CONTRAST_THRESHOLD/5)
	cout << "HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD = " << HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD << endl;						 //NB this represents the optimised values for 2DOD, consider/test using (256*1.0) for 3DOD...!!
	cout << "HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD = " << HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD << endl;	 //NB this represents the optimised values for 2DOD, consider/test using (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/4.0) for 3DOD...!!
	cout << "HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD = " << HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	cout << "QUADRATIC_FIT_KERNEL_SIZE = " << QUADRATIC_FIT_KERNEL_SIZE << endl;
	cout << "ZERO_CROSSING_POSITION_THRESHOLD = " << ZERO_CROSSING_POSITION_THRESHOLD << endl;
	cout << "A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD = " << A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;



	//OR OPTIMISATION IMAGE CROPPING VARIABLES...

	cout << "WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE = " << WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE << endl;
	cout << "OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO = " << OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO << endl;
	cout << "OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO = " << OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO << endl;	//has to result in a small image <= 8x8 pixels (single DCT block)
	cout << "" << endl;
	cout << "OR_METHOD_2DOD_NORM_SNAPSHOT_X = " << OR_METHOD_2DOD_NORM_SNAPSHOT_X << endl;	//secondary
	cout << "OR_METHOD_2DOD_NORM_SNAPSHOT_Y = " << OR_METHOD_2DOD_NORM_SNAPSHOT_Y << endl;	//secondary
	cout << "OR_METHOD_3DOD_NORM_SNAPSHOT_X = " << OR_METHOD_3DOD_NORM_SNAPSHOT_X << endl;	//secondary
	cout << "OR_METHOD_3DOD_NORM_SNAPSHOT_Y = " << OR_METHOD_3DOD_NORM_SNAPSHOT_Y << endl;	//secondary
	cout << "" << endl;
	/*
	int OR_METHOD_XDOD_SNAPSHOT_SIZE;			//must choose largest snapshot out of 2DOD and 3DOD
	int OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE;		//must choose largest snapshot out of 2DOD and 3DOD
	*/
	cout << "OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = " << OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP << endl;
	cout << "OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = " << OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP << endl;
	cout << "OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = " << OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X << endl;
	cout << "OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = " << OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y << endl;
	cout << "OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = " << OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH << endl;
	cout << "OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = " << OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X << endl;
	cout << "OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = " << OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y << endl;
	cout << "OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = " << OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR DISPLAY VARIABLES...

	cout << "OR_PRINT_ALGORITHM_PROGRESS = " << OR_PRINT_ALGORITHM_PROGRESS << endl;		//true in xml/testing
	cout << "OR_PRINT_ALGORITHM_AND_TIME_DETAILS = " << OR_PRINT_ALGORITHM_AND_TIME_DETAILS << endl;	//true in xml/testing
	cout << "OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL = " << OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL << endl;
	cout << "" << endl;
	cout << "OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = " << OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML << endl;	//true in xml/testing
	cout << "OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL = " << OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL << endl;	//true in xml/testing
	cout << "OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS = " << OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS << endl;	//true in xml/testing
	cout << "DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = " << DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW << endl;		//for 2DOD, this will stop data outside of the normalised triangle object data from being culled
	cout << "OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME = " << OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR ACCURACY/TUNING VARIABLES...

	cout << "OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD = " << OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD << endl;
	cout << "" << endl;


	//OR TRANSFORMATION ACCURACY VARIABLES...

	cout << "OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = " << OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST << endl;
	cout << "OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = " << OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST << endl;
	cout << "POLYGON_MIN_ANGLE_DEGREES = " << POLYGON_MIN_ANGLE_DEGREES << endl;
	cout << "" << endl;
	cout << "OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = " << OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON << endl;	//this needs to be made dynamic in the future! [based upon distance object is away from pov]
	cout << "OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = " << OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON << endl;	//100 //20
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...

	cout << "OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL = " << OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL << endl;
	cout << "OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER = " << OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR OPTIMISATION GEOMETRIC COMPARISON...

	cout << "OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY = " << OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY << endl;			 //untested - default OFF
	cout << "OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY = " << OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY << endl;	 //untested - default OFF
	cout << "" << endl;
	cout << "NUMBER_OF_POLYGONS_PER_FEATURE = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
	cout << "OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = " << OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE << endl;
	cout << "" << endl;
	cout << "OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM = " << OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR = " << OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR << endl;
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE << endl;
	cout << "" << endl;
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE << endl;
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE << endl;
	cout << "" << endl;
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION << endl;
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION << endl;
	cout << "" << endl;
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS << endl;	//(40)
	cout << "OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS = " << OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS << endl;	//(40)
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

	cout << "OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON = " << OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

}





