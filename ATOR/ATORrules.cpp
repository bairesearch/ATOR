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
 * File Name: ATORrules.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3o4a 17-November-2020
 * /
 *******************************************************************************/


#include "ATORrules.hpp"




void ATORrulesClass::fillInATORrulesExternVariables()
{


	/************** 0. PREPARE STANDARD XML RULES (SET DEFAULTS)* ********************/


	//ATOR_IMAGE_COMPARISON_DECISION_TREE...

	ATOR_IMAGE_COMPARISON_DECISION_TREE = true;

	ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING = true;
	ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING = false;
	ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
	ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON = false;

	ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING = true;
	ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING = false;
	ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
	ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON = false;

	ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL = true;		//tested - default ON [this should be able to be turned off for optimisation purposes, it needs to fix up memory leak however] - this will not be changable in xml file, it should never need to be changed in release versions
	ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE = true;	//tested - default ON [turn this off to display html results file properly] - this will not be changable in xml file, it should never need to be changed in release versions


	//OR_FEATURES...

	//xml determined by default;
	ATOR_USE_FIND_CORNER_FEATURES = true;
	ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES = true;
	ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES = false;
	ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY = false;

	ATOR_METHOD_USE_MESH_LISTS = false;   //[has not been tested with centred feature detection]
	ATOR_METHOD_3DOD_USE_MESH_LISTS_COMBINED = false;
	ATOR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE = 0.1;
	ATOR_METHOD_2DOD_USE_MESH_LISTS_COMBINED = false;		//never use this - not yet implemented

	MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE = (20);					 //this needs to be made dynamic based upon vi
	MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY = (2);
	ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE = false;		 //tested - default OFF
	ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE = (0.1);					  //this needs to be made dynamic based upon vi
	ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = (ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE);  //this needs to be made dynamic based upon vi 	  //this needs to be checked, this value might need to be increased slightly
	ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT = (0.3);				  //this needs to be made dynamic based upon vi
	ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE = (1.0);					  //this needs to be made dynamic based upon zoom??
	ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = (ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE);  //this needs to be made dynamic based upon zoom??	  //this needs to be checked, this value might need to be increased slightly



	//ATOR_METHOD3DOD FEATURE...

	ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST = true;
	ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION = false;

	ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE = (0.001);



	//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION = false;



	//OR_SHARED_VARS...	//general contrast threshold constraints

	ATOR_USE_CONTRAST_CALC_METHOD_C = true;				  //tested - default ON		//ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with ATOR_USE_CONTRAST_CALC_METHOD_B
	ATOR_USE_CONTRAST_CALC_METHOD_B = false;				  //tested - default OFF	//ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with ATOR_USE_CONTRAST_CALC_METHOD_B

	LUMINOSITY_CONTRAST_FRACTION_THRESHOLD = (0.05); 	 //default 0.05 for method C, default 0.02 for method B
	DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD = (0.05);	 //default 0.05 for method C, default 0.02 for method B  //this needs to be dynamic - not static - and should be dependant upon focal length
	DEPTH_CONTRAST_FRACTION_THRESHOLD = (0.05);		 //default 0.05 for method C, default 0.02 for method B  //this needs to be dynamic - not static - and should be dependant upon focal length
	POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD = (0.1);	 //default 0.1 for method C, default 0.02 for method B

	LUMINOSITY_FRACTION_THRESHOLD = (0.01);
	ATOR_MAX_DEPTH_NOISE = (0.1);
	ESTIMATE_MAX_DEPTH_T_REAL = (20.0);



	//ATOR_METHOD2DOD...

	ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS = true;



	//OR_METHOD...

	ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING = true;

	ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = 10000;
	ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST = 10000;
	ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = 10000;
	ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST = 10000;



	//LD_OPENGL...

	ATOR_SNAPSHOT_WINDOW_POSITION_X = (0);	     	//dependent upon resolution of monitor - should be moved to command line parameter
	ATOR_SNAPSHOT_WINDOW_POSITION_Y = (800);	     //dependent upon resolution of monitor - should be moved to command line parameter

	//OR_IMAGE_COMPARISON...

	ATOR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = (0.0);			  //not tested or used yet
	ATOR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = (0.0); 		  //not tested or used yet
	ATOR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = (30.0); 	  //5
	ATOR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = (20.0);
	IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE = (68.2656);
	ATOR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF = (10.0);
	ATOR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS = (100.0/(28*28/2));			  //OLD: (100.0/(28*28))
	ATOR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF = (10.0);			  //needs to be tested

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
	ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO = (4);
	ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO = (5);  //has to result in a small image <= 8x8 pixels (single DCT block)


	//OR ACCURACY/TUNING VARIABLES...

	ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD = (0.2);


	//OR DISPLAY VARIABLES...

	ATOR_PRINT_ALGORITHM_PROGRESS = false;		    //true in xml/testing
	ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS = false;	    //true in xml/testing
	ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL = false;

	ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = false; 			 //true in xml/testing
	ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL = false;   	 //true in xml/testing
	ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS = false;     //true in xml/testing
	DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = false;	    //for 2DOD, this will stop data outside of the normalised triangle object data from being culled
	strcpy(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME, "ORResults.html");


	//OR TRANSFORMATION ACCURACY VARIABLES...

	ATOR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = true;
	ATOR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = true;
	POLYGON_MIN_ANGLE_DEGREES = (10.0);

	ATOR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = (0.1);	//OLD; 20  //this needs to be made dynamic in the future! [based upon distance object is away from pov]
	ATOR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = (20.0);	  //100 //20



	//ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...

	ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL = (20);
	ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER = (5);



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY = false;		    //untested - default OFF
	ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY = false;	    //untested - default OFF

	ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR = (0.3);
	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE = (6.0);



	//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

	ATOR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD = 0.2;


	//OR SPARE PARAMETERS VARIABLES...

	ATOR_RULES_XML_SPARE_PARAMETER_1 = false;
	ATOR_RULES_XML_SPARE_PARAMETER_2 = (0.0);


	/************** 1. PARSE STANDARD XML RULES* ********************/


	//Standard OR Rules Entries;
	XMLrulesClass* currentReferenceRulesClass = ATORrulesObjectRecognition;
	while(currentReferenceRulesClass->next != NULL)
	{
		//ATOR_IMAGE_COMPARISON_DECISION_TREE...

		if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_DECISION_TREE_NAME)
		{
			ATOR_IMAGE_COMPARISON_DECISION_TREE = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_NAME)
		{
			ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING_NAME)
		{
			ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NAME)
		{
			ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_NAME)
		{
			ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NAME)
		{
			ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING_NAME)
		{
			ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NAME)
		{
			ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON_NAME)
		{
			ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL_NAME)
		{
			ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_NAME)
		{
			ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}


		//OR_FEATURES...

		else if(currentReferenceRulesClass->name == ATOR_USE_FIND_CORNER_FEATURES_NAME)
		{
			ATOR_USE_FIND_CORNER_FEATURES = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_NAME)
		{
			ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_NAME)
		{
			ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY_NAME)
		{
			ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == ATOR_METHOD_USE_MESH_LISTS_NAME)
		{
			ATOR_METHOD_USE_MESH_LISTS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_USE_MESH_LISTS_COMBINED_NAME)
		{
			ATOR_METHOD_3DOD_USE_MESH_LISTS_COMBINED =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE_NAME)
		{
			ATOR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_USE_MESH_LISTS_COMBINED_NAME)
		{
			ATOR_METHOD_2DOD_USE_MESH_LISTS_COMBINED =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE_NAME)
		{
			MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY_NAME)
		{
			MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE_NAME)
		{
			ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_NAME)
		{
			ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2_NAME)
		{
			ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT_NAME)
		{
			ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_NAME)
		{
			ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2_NAME)
		{
			ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = currentReferenceRulesClass->fractionalValue;
		}



		//ATOR_METHOD3DOD FEATURE...

		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST_NAME)
		{
			ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION_NAME)
		{
			ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE_NAME)
		{
			ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE = currentReferenceRulesClass->fractionalValue;
		}


		//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}



		//OR_SHARED_VARS...	//general contrast threshold constraints


		else if(currentReferenceRulesClass->name == ATOR_USE_CONTRAST_CALC_METHOD_C_NAME)
		{
			ATOR_USE_CONTRAST_CALC_METHOD_C =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_USE_CONTRAST_CALC_METHOD_B_NAME)
		{
			ATOR_USE_CONTRAST_CALC_METHOD_B =  SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
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
		else if(currentReferenceRulesClass->name == ATOR_MAX_DEPTH_NOISE_NAME)
		{
			ATOR_MAX_DEPTH_NOISE =  currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ESTIMATE_MAX_DEPTH_T_REAL_NAME)
		{
			ESTIMATE_MAX_DEPTH_T_REAL =  currentReferenceRulesClass->fractionalValue;
		}

		//ATOR_METHOD2DOD...

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		//OR_METHOD...

		else if(currentReferenceRulesClass->name == ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING_NAME)
		{
			ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN_NAME)
		{
			ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST_NAME)
		{
			ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN_NAME)
		{
			ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST_NAME)
		{
			ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST = int(currentReferenceRulesClass->fractionalValue);
		}



		//LD_OPENGL...

		else if(currentReferenceRulesClass->name == ATOR_SNAPSHOT_WINDOW_POSITION_X_NAME)
		{
			ATOR_SNAPSHOT_WINDOW_POSITION_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_SNAPSHOT_WINDOW_POSITION_Y_NAME)
		{
			ATOR_SNAPSHOT_WINDOW_POSITION_Y = int(currentReferenceRulesClass->fractionalValue);
		}


		//OR_IMAGE_COMPARISON...

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			ATOR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			ATOR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			ATOR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION_NAME)
		{
			ATOR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE_NAME)
		{
			IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF_NAME)
		{
			ATOR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS_NAME)
		{
			ATOR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF_NAME)
		{
			ATOR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF = currentReferenceRulesClass->fractionalValue;
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
			USE_EDGISE_3X3_KERNEL = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == USE_EDGISE_5X5_KERNEL_NAME)
		{
			USE_EDGISE_5X5_KERNEL = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
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
		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO_NAME)
		{
			ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO_NAME)
		{
			ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO = int(currentReferenceRulesClass->fractionalValue);
		}


		//OR DISPLAY VARIABLES...

		else if(currentReferenceRulesClass->name == ATOR_PRINT_ALGORITHM_PROGRESS_NAME)
		{
			ATOR_PRINT_ALGORITHM_PROGRESS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_NAME)
		{
			ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL_NAME)
		{
			ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML_NAME)
		{
			ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL_NAME)
		{
			ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS_NAME)
		{
			ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW_NAME)
		{
			DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME_NAME)
		{
			strcpy(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME, (currentReferenceRulesClass->stringValue).c_str());
		}

		//OR ACCURACY/TUNING VARIABLES...

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD_NAME)
		{
			ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD = currentReferenceRulesClass->fractionalValue;
		}

		//OR TRANSFORMATION ACCURACY VARIABLES...

		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST_NAME)
		{
			ATOR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST_NAME)
		{
			ATOR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == POLYGON_MIN_ANGLE_DEGREES_NAME)
		{
			POLYGON_MIN_ANGLE_DEGREES = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON_NAME)
		{
			ATOR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON_NAME)
		{
			ATOR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = currentReferenceRulesClass->fractionalValue;
		}


		//ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...


		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL_NAME)
		{
			ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER_NAME)
		{
			ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR OPTIMISATION GEOMETRIC COMPARISON...

		else if(currentReferenceRulesClass->name == ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY_NAME)
		{
			ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY_NAME)
		{
			ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR_NAME)
		{
			ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE_NAME)
		{
			ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE = currentReferenceRulesClass->fractionalValue;
		}


		//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD_NAME)
		{
			ATOR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD = currentReferenceRulesClass->fractionalValue;
		}


		//OR SPARE PARAMETERS VARIABLES...

		else if(currentReferenceRulesClass->name == ATOR_RULES_XML_SPARE_PARAMETER_1_NAME)
		{
			ATOR_RULES_XML_SPARE_PARAMETER_1 = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_RULES_XML_SPARE_PARAMETER_2_NAME)
		{
			ATOR_RULES_XML_SPARE_PARAMETER_2 = currentReferenceRulesClass->fractionalValue;
		}




		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;

	}

	/************** 2. POST PROCESSES STANDARD XML RULES* ********************/

	//ATOR_IMAGE_COMPARISON_DECISION_TREE...

	ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = false;
	ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = false;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = false;
	ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = false;

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE)
	{
		ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING = false;
		ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING = false;
		ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = false;
		ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON = false;

		if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = true;
		}

		if(ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
		{
			ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = true;
		}

		if(ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
		}

		if(ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
		{
			ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = true;
		}
	}
	else
	{
		ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING = false;
		ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING = false;
		ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = false;
		ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON = false;

		if(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING)
		{
			ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = true;
		}

		if(ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING)
		{
			ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = true;
		}

		if(ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = true;
		}

		if(ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON)
		{
			ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = true;
		}
	}


	ATOR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY = true;
	#ifdef ATOR_IMAGE_COMPARISON_SQL
	if(ATOR_IMAGE_COMPARISON_DECISION_TREE)
	{
		ATOR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY = false;		//this cannot be used with decision tree activated
	}
	#endif



	//ATOR_METHOD_GEOMETRIC_COMPARISON...

	ATOR_METHOD_TRANSFORM_NEARBY_FEATURES = false;			//NEW this has been made dependent upon whether geo comparison is used.

	ATOR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES = false;	//this can be used with ATOR_IMAGE_COMPARISON_DECISION_TREE as nearby feature data is not taken and used from snapshot tables [only used in the definition of decision tree tables]

	ATOR_METHOD_TRANSFORM_KEY_OT_FEATURES = false;
	ATOR_METHOD_TRANSFORM_ALL_FEATURES = false;
	ATOR_METHOD_CREATE_OT_FEATURES_FILE = false;
	ATOR_METHOD_CREATE_ALL_FEATURES_FILE = false;

	ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = false;
	ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE = false;
	ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = false;
	ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST = false;

#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON

	if(ATOR_METHOD_TRANSFORM_KEY_OT_FEATURES)
	{
		ATOR_METHOD_CREATE_OT_FEATURES_FILE = true;
	}

	if(ATOR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ATOR_METHOD_CREATE_ALL_FEATURES_FILE = true;
	}

	#ifdef ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO

		ATOR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

		ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = true;

		ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
		if(ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
		{
			ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
		}

	#elif defined ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

		ATOR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

		ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = true;

		ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
		if(ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
		{
			ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
		}

	#elif defined ATOR_METHOD_GEOMETRIC_COMPARISON_NON_OPTIMISED_FILE_IO

		ATOR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

	#elif defined TEMPTEST3GEOACCURACY

		ATOR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

		if(ATOR_METHOD_TRANSFORM_NEARBY_FEATURES)
		{
			ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
		}

	#else
		if(ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING)
		{
			ATOR_METHOD_TRANSFORM_NEARBY_FEATURES = true;

			if(!ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL)
			{
				ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO = true;
				ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST = true;
				if(ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST)
				{
					ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
				}
				//ATOR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES = true
			}

			#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES

			ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS = true;
			if(ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS)
			{
				ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE = true;
			}

			#endif
		}

	#endif
#endif



	//OR_SHARED_VARS...	//general contrast threshold constraints

	if(ATOR_USE_CONTRAST_CALC_METHOD_B)
	{
		if(ATOR_USE_CONTRAST_CALC_METHOD_C)
		{
			cerr << "error - cannot define two contrast calculation methods in OR rules xml file" << endl;
			exit(EXIT_ERROR);
		}
	}
	MAX_LUMINOSITY_CONTRAST = 0;
	MAX_NORMAL_CONTRAST = 0;
	ESTIMATE_MAX_DEPTH_CONTRAST = 0;
	ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = 0;

	if(ATOR_USE_CONTRAST_CALC_METHOD_C)
	{
		MAX_LUMINOSITY_CONTRAST = (MAX_LUMINOSITY*4);
		MAX_NORMAL_CONTRAST = (MAX_NORMAL*3.0*4);
		ESTIMATE_MAX_DEPTH_CONTRAST = (ESTIMATE_MAX_DEPTH_T_REAL);
		ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST = (ESTIMATE_MAX_DEPTH_CONTRAST*4.0);	//OLD: 0.1 instead

	}
	else if(ATOR_USE_CONTRAST_CALC_METHOD_B)
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
	if(ATOR_USE_CONTRAST_CALC_METHOD_C)
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

	if(ATOR_USE_CONTRAST_CALC_METHOD_C)
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

	if(ATOR_USE_CONTRAST_CALC_METHOD_C)
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

	if(ATOR_USE_CONTRAST_CALC_METHOD_C)
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

	IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR = (IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE* 3);
	//#define ATOR_IMAGE_COMPARISON_MIN_TOTAL_NUM_STARK_LOCAL_CONTRASTS (100.0/(28*28/2))



	//OR_OPERATIONS...
	MAX_FEATURE_DISTANCE_ERROR_USING_POINT_MAP_METHOD = (XYCOORDINATES_CONVERSION_INTO_PIXELS* MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD);	//in world units		//OLD;(0.001)



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

	if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML)
	{
		if(!ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
		{
			DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = true;
		}
	}

	#ifdef TEST_VIEWTIME_SPEED_Z_FOR_WEBSITE
	ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = false;
	#endif



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE = (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE*2.0);
	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE = (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE*2.0);

	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION = (ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR);
	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION = (ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR);

	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS = ((int64_t)(((double)ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE)/ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION)); //(40)
	ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS = ((int64_t)(((double)ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE)/ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION)); //(40)



	//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...
	ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON = false;
	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON = true;
	}



	/************** 3. PREPARE OPTIONAL XML RULES (SET DEFAULTS) * ********************/


	//OR_FEATURES...

	//dynamically determined by default (these do not need to be defined in the xml file);
	ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
	ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
	ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = false;
	ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
	ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
	ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = false;
	ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = false;
	ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = false;
	ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = false;
	ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = false;

	if(ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES)
	{

		//mesh list quadratic fit for accurate edge calculation and centre feature calculations
		if(ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY)
		{
			ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = true;		//THIS IS OPTIONAL [has been tested with centred feature detection]
			ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = true;		//THIS IS OPTIONAL [has been tested with centred feature detection]
		}
		else
		{
			ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
			ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = false;
		}

		if(ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
		{
			ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = true;
			ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = true;
		}
		else
		{
			ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
		}

		if(ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
		{
			ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = true;
			ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = true;
		}
		else
		{
			ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = false;
		}

		if(ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES)
		{
			ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = true;

			//#define ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OLD
			ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = true;

			if(ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
			{
				ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = true;	//this will not be used in the future as edge points calculated will have depth filtering applied based upon contiguous boundary edge trace
			}

			ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = true;	//TESTHING THIS now!

		}

		//mesh lists (only currently used for centred feature calculations in combined mesh lists)
			//the following is only valid when ATOR_DEBUG_METHOD_ASSUME_TRAIN_AND_TEST_WITH_SINGLE_VIEWPOINT is not defined
		if(ATOR_METHOD_USE_MESH_LISTS)
		{
			/*consider these in the future...
			#ifdef TRY_UNTESTED_SOFTWARE2
				#define ATOR_METHOD_3DOD_USE_MESH_LISTS_COMBINED	//probably need to verify quadratic fit edges are calculated properly upon mesh advanced join
				#define ATOR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE (0.1) //this needs to be made dynamic based upon vi
				//#define ATOR_METHOD_2DOD_USE_MESH_LISTS_COMBINED		//dont use	//not yet implemented (low priority) - there is no way of combining 2D mesh lists without eg x/y offset information...
			#endif
			*/
			ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = true;
		}
	}



	//ATOR_METHOD3DOD FEATURE...

	ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = false;
	if(ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION)
	{
		ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = true;
	}




	//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...
	//dynamically determined by default (these do not need to be defined in the xml file);

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = false;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = false;

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X = 2;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X = 2;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y = 2;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y = 2;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y = -1;

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y = -1;

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X = 0;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y = 1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X = -1;
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y = -1;

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE)
	{
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = (2);	//must be even! [not odd]
	#else
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = (1);
	#endif
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = (32);		//6bit signed; -16->+15 	//num DCT bins per dimension
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = (6);

		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = 10;				//deterministic dct coeff comparison requirement
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = 50; 	//deterministic dct coeff comparison requirement
	}
	else
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = (1);		//+/- 8 requires highly compressed JPEG data

	#ifdef ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_REQUIREMENT_V4
		//ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL is used, therefore ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT_BIN_ALL must be max 64bit
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = (16);		//5bit signed; -8->+7 	//num DCT bins per dimension
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = (5);
	#else
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = (32);		//6bit signed; -16->+15 	//num DCT bins per dimension
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = (6);
	#endif

		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = 10;				//deterministic dct coeff comparison requirement
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = 50; 	//deterministic dct coeff comparison requirement

	}


	//#define ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ABS_LUM_INFORMATION	//this cannot be used as lighting intensity and/or colour varies between scenes

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = true;
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = true;	//even with matched Lum:chroma subsampling, chroma data is still too compressed, so need to artificially raise the quality of the jpeg compression to determine the colour DCT coefficients
	}
	else
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = false;
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = false;	//even with matched Lum:chroma subsampling, chroma data is still too compressed, so need to artificially raise the quality of the jpeg compression to determine the colour DCT coefficients
	}

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION)
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = (7);
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = (3);
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = (3);
	}
	else
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = (7);
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = (0);
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = (0);
	}



	//ATOR_METHOD2DOD...

	if(ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS)
	{
		//either no cull or square cull is required for fourier transform
		ATOR_METHOD_DO_NOT_CULL_SNAPSHOT = true;			//square cull
		ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = false;		//no cull
	}
	else
	{
		ATOR_METHOD_DO_NOT_CULL_SNAPSHOT = false;			//square cull
		ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = false;		//no cull
	}



	//OR_METHOD...

	ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = false;
	ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;
	ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = false;
	ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;

	if(ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING)
	{
		ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = true;
		//ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = true;

		if(ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS)
		{
			ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 3;
		}
		else
		{
			ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;
		}

		if(ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS)
		{
			ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 3;
		}
		else
		{
			ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = 1;
		}

	}


	//OR OPTIMISATION IMAGE CROPPING VARIABLES...

	ATOR_METHOD_2DOD_NORM_SNAPSHOT_X = (ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO*(ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE-1));	//Eg 28	[must be divisible by ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO]
	ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y = (ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO*(ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE-1)); 	//Eg 28	[must be divisible by ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO]

	ATOR_METHOD_3DOD_NORM_SNAPSHOT_X = (ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO*ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE);		//eg 40
	ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y = (ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO*ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE);		//eg 40



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	if(ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY)
	{
		NUMBER_OF_POLYGONS_PER_FEATURE = (1); 		//default; 1
	}
	else
	{
		NUMBER_OF_POLYGONS_PER_FEATURE = (3); 		//default; 3
	}

	if(ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY)
	{
		ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = (2);	//default; 4, 10 will reduce speed but increase redundancy [due to bug?]
	}
	else
	{
		ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = (4);	//default; 4, 10 will reduce speed but increase redundancy [due to bug?]
	}




	/************** 4. SET OPTIONAL XML RULES* ********************/

	//Extra OR Rules Entries;
		//now allow for override of default dependencies;
	currentReferenceRulesClass = ATORrulesObjectRecognition;
	while(currentReferenceRulesClass->next != NULL)
	{

		//OR_FEATURES...
		if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_NAME)
		{
			ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_NAME)
		{
			ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED_NAME)
		{
			ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL_NAME)
		{
			ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL_NAME)
		{
			ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS_NAME)
		{
			ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT_NAME)
		{
			ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE_NAME)
		{
			ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET_NAME)
		{
			ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST_NAME)
		{
			ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}



		//ATOR_METHOD3DOD FEATURE...
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST_NAME)
		{
			ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}



		//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y_NAME)
		{
			ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y = int(currentReferenceRulesClass->fractionalValue);
		}

		//ATOR_METHOD2DOD...

		else if(currentReferenceRulesClass->name == ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_NAME)
		{
			ATOR_METHOD_DO_NOT_CULL_SNAPSHOT = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME_NAME)
		{
			ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}

		//OR_METHOD...

		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS_NAME)
		{
			ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS_NAME)
		{
			ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS_NAME)
		{
			ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = SHAREDvars.convertStringToBool(currentReferenceRulesClass->stringValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS_NAME)
		{
			ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR OPTIMISATION IMAGE CROPPING VARIABLES...

		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_NORM_SNAPSHOT_X_NAME)
		{
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y_NAME)
		{
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_NORM_SNAPSHOT_X_NAME)
		{
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_X = int(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y_NAME)
		{
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y = int(currentReferenceRulesClass->fractionalValue);
		}

		//OR OPTIMISATION GEOMETRIC COMPARISON...

		else if(currentReferenceRulesClass->name == NUMBER_OF_POLYGONS_PER_FEATURE_NAME)
		{
			NUMBER_OF_POLYGONS_PER_FEATURE = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE_NAME)
		{
			ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = currentReferenceRulesClass->fractionalValue;
		}


		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}





	/************** 5.  POST PROCESSES OPTIONAL XML RULES* ********************/


	//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET = (ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS/2);	//value to add to signed value before binning
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS = (ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb);	 	//only compare a selection of 15 of the 64x3 DCT coefficients (of the Y, Cr and Cb blocks) {64 bit - Y DCT; 5bit[signed]*3 + YCr DCT; 5bit[signed]*3 + YCb DCT; 5bit[signed]*7}   - ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT0 d0, ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT1 d1, ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT2 d2, ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT3 d3 ... ATOR_MYSQL_FIELD_NAME_DCT_COEFFICIENT15 d15

	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING = string("-quality ") + SHAREDvars.convertIntToString(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY) + "%";
	ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING = string("-quality ") + SHAREDvars.convertIntToString(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED) + "%";

	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING)
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_ON;
	}
	else
	{
		ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_OFF;
	}



	//OR OPTIMISATION IMAGE CROPPING VARIABLES...

	if(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X%ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO != 0)
	{
		cout << "warning; OR rules XML: ATOR_METHOD_2DOD_NORM_SNAPSHOT_X is not divisible by ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO" << endl;
	}
	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		if(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X/ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO > ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE)
		{
			cerr << "error; OR rules XML: ATOR_METHOD_2DOD_NORM_SNAPSHOT_X/ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO > ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE" << endl;
			exit(EXIT_ERROR);
		}
	}

	if(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X%ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO != 0)
	{
		cout << "warning; OR rules XML: ATOR_METHOD_3DOD_NORM_SNAPSHOT_X is not divisible by ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO" << endl;
	}
	if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		if(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X/ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO > ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE)
		{
			cerr << "error; OR rules XML: ATOR_METHOD_3DOD_NORM_SNAPSHOT_X/ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO > ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_BLOCK_SIZE" << endl;
			exit(EXIT_ERROR);
		}
	}



	ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = false;
	ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = false;
	ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = 0;
	ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = 0;
	ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = 1.0;
	ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = 0;
	ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = 0;
	ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = 1.0;


	int ATOR_METHOD_XDOD_NORM_SNAPSHOT_X = 0;
	int ATOR_METHOD_XDOD_NORM_SNAPSHOT_Y = 0;
	int ATOR_METHOD_XDOD_SNAPSHOT_SIZE = 0;
	int ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE = 0;
	if(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X > ATOR_METHOD_2DOD_NORM_SNAPSHOT_X)
	{
		ATOR_METHOD_XDOD_NORM_SNAPSHOT_X = ATOR_METHOD_3DOD_NORM_SNAPSHOT_X;
		ATOR_METHOD_XDOD_NORM_SNAPSHOT_Y = ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y;
		ATOR_METHOD_XDOD_SNAPSHOT_SIZE = ATOR_METHOD_XDOD_NORM_SNAPSHOT_X*ATOR_METHOD_XDOD_NORM_SNAPSHOT_Y;
		ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE = ((ATOR_METHOD_3DOD_NORM_SNAPSHOT_X/ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO)*(ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y/ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO));	//must choose largest snapshot out of 2DOD and 3DOD

	}
	else
	{
		ATOR_METHOD_XDOD_NORM_SNAPSHOT_X = ATOR_METHOD_2DOD_NORM_SNAPSHOT_X;
		ATOR_METHOD_XDOD_NORM_SNAPSHOT_Y = ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y;
		ATOR_METHOD_XDOD_SNAPSHOT_SIZE = ATOR_METHOD_XDOD_NORM_SNAPSHOT_X*ATOR_METHOD_XDOD_NORM_SNAPSHOT_Y;
		ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE = ((ATOR_METHOD_2DOD_NORM_SNAPSHOT_X/ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO)*(ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y/ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO));	//must choose largest snapshot out of 2DOD and 3DOD
	}


	bool minimumHorizontalWindowsSizeIsLessThanSnapshotWidth = false;
	#ifdef LINUX
		//Note linux supports windows of arbitrary size
		minimumHorizontalWindowsSizeIsLessThanSnapshotWidth = true;
	#else

		//Note Windows (XP classic theme) enforces a minimum horizontal windows size of 104 pixels
		if(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE < ATOR_METHOD_XDOD_NORM_SNAPSHOT_X)
		{
			minimumHorizontalWindowsSizeIsLessThanSnapshotWidth = true;
		}
	#endif


	if(minimumHorizontalWindowsSizeIsLessThanSnapshotWidth)
	{

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
			//#define ATOR_METHOD_DO_NOT_CULL_SNAPSHOT	- needs to be made dynamic
			ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
			ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = (ATOR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = (ATOR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X+(ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X*2))/double(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X));
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = (ATOR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = (ATOR_IMAGE_COMPARISON_DECISION_TREE_GAUSSIAN_KERNEL_SIZE/2);
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double((ATOR_METHOD_3DOD_NORM_SNAPSHOT_X+(ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X*2))/double(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X));
		#else
			ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = false;
			ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = false;
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = (0);
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = (0);
			ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (1.0);
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = (0);
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = (0);
			ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (1.0);
		#endif

	}
	else
	{
		ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
		ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = true;	//CURRENTLY REQUIRES OPENGL(?)
		ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-ATOR_METHOD_2DOD_NORM_SNAPSHOT_X)/2);
		ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y)/2);
		ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X+(ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X*2))/double(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X));	//#define ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH (double(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE)/double(ATOR_METHOD_2DOD_NORM_SNAPSHOT_X))
		ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-ATOR_METHOD_3DOD_NORM_SNAPSHOT_X)/2);
		ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = ((WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE-ATOR_METHOD_3DOD_NORM_SNAPSHOT_X)/2);
		ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = (double(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X+(ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X*2))/double(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X));	//#define ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH (double(WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE)/double(ATOR_METHOD_3DOD_NORM_SNAPSHOT_X))

	}


	//OR SQL PIXMAPS VARIABLES...

	ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH = (ATOR_METHOD_XDOD_SNAPSHOT_SIZE*3 + ATOR_METHOD_XDOD_SNAPSHOT_SIZE*3 + ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE*3 + ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE*3);



	//OR OPTIMISATION GEOMETRIC COMPARISON...

	ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM = (ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE);


}


void ATORrulesClass::printATORrulesExternVariables()
{

	//ATOR_IMAGE_COMPARISON_DECISION_TREE...

	cout << "ATOR_IMAGE_COMPARISON_DECISION_TREE = " << ATOR_IMAGE_COMPARISON_DECISION_TREE << endl;

	cout << "ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING = " << ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING = " << ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = " << ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON = " << ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING = " << ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING = " << ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = " << ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON = " << ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING = " << ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING = " << ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING << endl;
	cout << "ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON = " << ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY = " << ATOR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL = " << ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL << endl;
	cout << "ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE = " << ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//OR_FEATURES...

	//xml determined by default;
	cout << "ATOR_USE_FIND_CORNER_FEATURES = " << ATOR_USE_FIND_CORNER_FEATURES << endl;
	cout << "ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES = " << ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES << endl;
	cout << "ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES = " << ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES << endl;
	cout << "ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY = " << ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY << endl;
	cout << "ATOR_METHOD_USE_MESH_LISTS = " << ATOR_METHOD_USE_MESH_LISTS << endl;   //[has not been tested with centred feature detection]
	cout << "" << endl;
	//dynamically determined by default;
	cout << "ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = " << ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP << endl;
	cout << "ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP = " << ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP << endl;
	cout << "ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED = " << ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED << endl;
	cout << "ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = " << ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL << endl;
	cout << "ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL = " << ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL << endl;
	cout << "ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS = " << ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS << endl;
	cout << "ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT = " << ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT << endl;
	cout << "ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE = " << ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE << endl;
	cout << "ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET = " << ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET << endl;
	cout << "ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST = " << ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST << endl;
	cout << "" << endl;
	cout << "MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE = " << MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE << endl;
	cout << "MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY = " << MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY << endl;
	cout << "ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE = " << ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE << endl;
	cout << "ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE = " << ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE << endl;
	cout << "ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = " << ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2 << endl;
	cout << "ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT = " << ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT << endl;
	cout << "ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE = " << ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE << endl;
	cout << "ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 = " << ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2 << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//ATOR_METHOD3DOD FEATURE...

	cout << "ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST = " << ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST << endl;
	cout << "ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION = " << ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION << endl;
	cout << "ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST = " << ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE = " << ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY << endl;			//deterministic dct coeff comparison requirement
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED << endl; 	//deterministic dct coeff comparison requirement
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING << endl;			//deterministic dct coeff comparison requirement
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING << endl; 	//deterministic dct coeff comparison requirement
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y << endl;
	cout << "" << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET << endl;
	cout << "ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS = " << ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

	//OR_SHARED_VARS...	//general contrast threshold constraints

	cout << "ATOR_USE_CONTRAST_CALC_METHOD_C = " << ATOR_USE_CONTRAST_CALC_METHOD_C << endl;		//ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with ATOR_USE_CONTRAST_CALC_METHOD_B
	cout << "ATOR_USE_CONTRAST_CALC_METHOD_B = " << ATOR_USE_CONTRAST_CALC_METHOD_B << endl;		//ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with ATOR_USE_CONTRAST_CALC_METHOD_B
	cout << "" << endl;
	cout << "LUMINOSITY_CONTRAST_FRACTION_THRESHOLD = " << LUMINOSITY_CONTRAST_FRACTION_THRESHOLD << endl;
	cout << "DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD = " << DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD << endl;	//this needs to be dynamic - not static - and should be dependant upon focal length
	cout << "DEPTH_CONTRAST_FRACTION_THRESHOLD = " << DEPTH_CONTRAST_FRACTION_THRESHOLD << endl;		//this needs to be dynamic - not static - and should be dependant upon focal length
	cout << "POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD = " << POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD << endl;
	cout << "" << endl;
	cout << "LUMINOSITY_FRACTION_THRESHOLD = " << LUMINOSITY_FRACTION_THRESHOLD << endl;
	cout << "ATOR_MAX_DEPTH_NOISE = " << ATOR_MAX_DEPTH_NOISE << endl;
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

	//ATOR_METHOD2DOD...

	cout << "ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS = " << ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS << endl;
	cout << "ATOR_METHOD_DO_NOT_CULL_SNAPSHOT = " << ATOR_METHOD_DO_NOT_CULL_SNAPSHOT << endl;
	cout << "ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME = " << ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_METHOD...

	cout << "ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING = " << ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = " << ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS << endl;
	cout << "ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = " << ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS << endl;
	cout << "ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS = " << ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS << endl;
	cout << "ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS = " << ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = " << ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN << endl;
	cout << "ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST = " << ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST << endl;
	cout << "ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN = " << ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN << endl;
	cout << "ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST = " << ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;



	//LD_OPENGL...

	cout << "ATOR_SNAPSHOT_WINDOW_POSITION_X = " << ATOR_SNAPSHOT_WINDOW_POSITION_X << endl;		 //dependent upon resolution of monitor - should be moved to command line parameter
	cout << "ATOR_SNAPSHOT_WINDOW_POSITION_Y = " << ATOR_SNAPSHOT_WINDOW_POSITION_Y << endl;	 //dependent upon resolution of monitor - should be moved to command line parameter
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR_IMAGE_COMPARISON...

	cout << "ATOR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = " << ATOR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION << endl;			 //not tested or used yet
	cout << "ATOR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION = " << ATOR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION << endl;  		 //not tested or used yet
	cout << "ATOR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = " << ATOR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION << endl;		 //5
	cout << "ATOR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = " << ATOR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION << endl;
	cout << "IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE = " << IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE << endl;
	cout << "IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR = " << IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR << endl;
	cout << "ATOR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF = " << ATOR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF << endl;
	cout << "ATOR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS = " << ATOR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS << endl;		 //need to change to (100.0/(28*28/2)) after testing
	cout << "ATOR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF = " << ATOR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF << endl;			 //needs to be tested
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
	cout << "ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO = " << ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO << endl;
	cout << "ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO = " << ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO << endl;	//has to result in a small image <= 8x8 pixels (single DCT block)
	cout << "" << endl;
	cout << "ATOR_METHOD_2DOD_NORM_SNAPSHOT_X = " << ATOR_METHOD_2DOD_NORM_SNAPSHOT_X << endl;	//secondary
	cout << "ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y = " << ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y << endl;	//secondary
	cout << "ATOR_METHOD_3DOD_NORM_SNAPSHOT_X = " << ATOR_METHOD_3DOD_NORM_SNAPSHOT_X << endl;	//secondary
	cout << "ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y = " << ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y << endl;	//secondary
	cout << "" << endl;
	/*
	int ATOR_METHOD_XDOD_SNAPSHOT_SIZE;			//must choose largest snapshot out of 2DOD and 3DOD
	int ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE;		//must choose largest snapshot out of 2DOD and 3DOD
	*/
	cout << "ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP = " << ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP << endl;
	cout << "ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP = " << ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP << endl;
	cout << "ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X = " << ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X << endl;
	cout << "ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y = " << ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y << endl;
	cout << "ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = " << ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH << endl;
	cout << "ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X = " << ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X << endl;
	cout << "ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y = " << ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y << endl;
	cout << "ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH = " << ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR DISPLAY VARIABLES...

	cout << "ATOR_PRINT_ALGORITHM_PROGRESS = " << ATOR_PRINT_ALGORITHM_PROGRESS << endl;		//true in xml/testing
	cout << "ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS = " << ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS << endl;	//true in xml/testing
	cout << "ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL = " << ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL << endl;
	cout << "" << endl;
	cout << "ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML = " << ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML << endl;	//true in xml/testing
	cout << "ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL = " << ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL << endl;	//true in xml/testing
	cout << "ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS = " << ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS << endl;	//true in xml/testing
	cout << "DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW = " << DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW << endl;		//for 2DOD, this will stop data outside of the normalised triangle object data from being culled
	cout << "ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME = " << ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR ACCURACY/TUNING VARIABLES...

	cout << "ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD = " << ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD << endl;
	cout << "" << endl;


	//OR TRANSFORMATION ACCURACY VARIABLES...

	cout << "ATOR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = " << ATOR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST << endl;
	cout << "ATOR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST = " << ATOR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST << endl;
	cout << "POLYGON_MIN_ANGLE_DEGREES = " << POLYGON_MIN_ANGLE_DEGREES << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = " << ATOR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON << endl;	//this needs to be made dynamic in the future! [based upon distance object is away from pov]
	cout << "ATOR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON = " << ATOR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON << endl;	//100 //20
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...

	cout << "ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL = " << ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL << endl;
	cout << "ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER = " << ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR OPTIMISATION GEOMETRIC COMPARISON...

	cout << "ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY = " << ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY << endl;			 //untested - default OFF
	cout << "ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY = " << ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY << endl;	 //untested - default OFF
	cout << "" << endl;
	cout << "NUMBER_OF_POLYGONS_PER_FEATURE = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
	cout << "ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE = " << ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM = " << ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR = " << ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR << endl;
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE << endl;
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION << endl;
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION << endl;
	cout << "" << endl;
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS << endl;	//(40)
	cout << "ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS = " << ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS << endl;	//(40)
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;


	//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

	cout << "ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON = " << ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "" << endl;

}





