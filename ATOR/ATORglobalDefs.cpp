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
 * File Name: SHAREDglobalDefs.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Description: Generic Construct Functions: shared global definitions (configure to compile different BAI projects)
 * Project Version: 3p4c 07-August-2021
 * /
 *******************************************************************************/


#include "ATORglobalDefs.hpp"

//ATOR_IMAGE_COMPARISON_DECISION_TREE...

bool ATOR_IMAGE_COMPARISON_DECISION_TREE;

bool ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING;
bool ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING;
bool ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING;
bool ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON;

bool ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING;
bool ATOR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING;
bool ATOR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING;
bool ATOR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON;

bool ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING;
bool ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING;
bool ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING;
bool ATOR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON;

bool ATOR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY;

bool ATOR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL;		 //tested - default ON [this should be able to be turned off for optimisation purposes, it needs to fix up memory leak however] - this will not be changable in xml file, it should never need to be changed in release versions
bool ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE;  	 //tested - default ON [turn this off to display html results file properly] - this will not be changable in xml file, it should never need to be changed in release versions


//ATOR_METHOD_GEOMETRIC_COMPARISON...

bool ATOR_METHOD_TRANSFORM_NEARBY_FEATURES;

bool ATOR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES;

bool ATOR_METHOD_TRANSFORM_KEY_OT_FEATURES;
bool ATOR_METHOD_TRANSFORM_ALL_FEATURES;
bool ATOR_METHOD_CREATE_OT_FEATURES_FILE;
bool ATOR_METHOD_CREATE_ALL_FEATURES_FILE;

bool ATOR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO;
bool ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE;
bool ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS;
bool ATOR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST;


//OR_FEATURES...

//xml determined by default;
bool ATOR_USE_FIND_CORNER_FEATURES;
bool ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES;
bool ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES;
bool ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY;

bool ATOR_METHOD_USE_MESH_LISTS;   //[has not been tested with centred feature detection]
bool ATOR_METHOD_3DOD_USE_MESH_LISTS_COMBINED;
double ATOR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE;
bool ATOR_METHOD_2DOD_USE_MESH_LISTS_COMBINED;	//never use this - not yet implemented

//dynamically determined by default;
bool ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP;
bool ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP;
bool ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED;
bool ATOR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
bool ATOR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
bool ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS;
bool ATOR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT;
bool ATOR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE;
bool ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET;
bool ATOR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST;


double MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE;					 //this needs to be made dynamic based upon vi
int MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY;
bool ATOR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE;		 //tested - default OFF
double ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE;					 //this needs to be made dynamic based upon vi
double ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2;  //this needs to be made dynamic based upon vi  	 //this needs to be checked, this value might need to be increased slightly
double ATOR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT;				 //this needs to be made dynamic based upon vi
double ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE;					 //this needs to be made dynamic based upon zoom??
double ATOR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2;  //this needs to be made dynamic based upon zoom??	 //this needs to be checked, this value might need to be increased slightly


//ATOR_METHOD3DOD FEATURE...

bool ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST;
bool ATOR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FATOR_FEATURE_DETECTION;
bool ATOR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST;

double ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE;

//ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY;			//deterministic dct coeff comparison requirement
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED; 	//deterministic dct coeff comparison requirement

string ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING;			//deterministic dct coeff comparison requirement
string ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING; 	//deterministic dct coeff comparison requirement

bool ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION;
bool ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING;
bool ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING;

string ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING;

int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb;

int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y;

int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y;

int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y;

int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
int ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS;

//OR_SHARED_VARS...	//general contrast threshold constraints

bool ATOR_USE_CONTRAST_CALC_METHOD_C;			//RTglobalDefs.h	//ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with ATOR_USE_CONTRAST_CALC_METHOD_B
bool ATOR_USE_CONTRAST_CALC_METHOD_B;			//RTglobalDefs.h	//ATOR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with ATOR_USE_CONTRAST_CALC_METHOD_B

double LUMINOSITY_CONTRAST_FRACTION_THRESHOLD;		//RTglobalDefs.h
double DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD;	//this needs to be dynamic - not static - and should be dependant upon focal length
double DEPTH_CONTRAST_FRACTION_THRESHOLD;		//this needs to be dynamic - not static - and should be dependant upon focal length
double POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD;

double LUMINOSITY_FRACTION_THRESHOLD;			//RTglobalDefs.h
double ATOR_MAX_DEPTH_NOISE;
double ESTIMATE_MAX_DEPTH_T_REAL;			//RTglobalDefs.h

double MAX_LUMINOSITY_CONTRAST;				//RTglobalDefs.h
double MAX_NORMAL_CONTRAST;				//RTglobalDefs.h
double ESTIMATE_MAX_DEPTH_CONTRAST;
double ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST;

double EDGE_LUMINOSITY_CONTRAST_THRESHOLD;		//RTglobalDefs.h
double EDGE_LUMINOSITY_THRESHOLD;			//RTglobalDefs.h
double EDGE_NORMAL_CONTRAST_THRESHOLD;
double EDGE_DEPTH_CONTRAST_THRESHOLD;
double EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD;

int INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT;
int INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST;
int INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST;



//ATOR_METHOD2DOD...

bool ATOR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS;
bool ATOR_METHOD_DO_NOT_CULL_SNAPSHOT;
bool ATOR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME;



//OR_METHOD...

bool ATOR_METHOD_SUPPORT_HIGH_LEVEL_SCALING;

bool ATOR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS;
int ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
bool ATOR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS;
int ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;

int ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
int ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST;
int ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
int ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST;




//LD_OPENGL...

int ATOR_SNAPSHOT_WINDOW_POSITION_X;		 //dependent upon resolution of monitor - should be moved to command line parameter
int ATOR_SNAPSHOT_WINDOW_POSITION_Y;	 //dependent upon resolution of monitor - should be moved to command line parameter



//OR_IMAGE_COMPARISON...

double ATOR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION;			 //not tested or used yet
double ATOR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION;  		 //not tested or used yet
double ATOR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION;		 //5
double ATOR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION;
double IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE;
double IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR;
double ATOR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF;
double ATOR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS;		 //need to change to (100.0/(28*28/2)) after testing
double ATOR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF;			 //needs to be tested



//OR_OPERATIONS...

double XYCOORDINATES_CONVERSION_INTO_PIXELS;	 //default 0.0002
double MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD;	 //in pixel units
double MAX_FEATURE_DISTANCE_ERROR_USING_POINT_MAP_METHOD;	 //in world units		 //OLD;(0.001)



//OR_PIXEL_MAPS...

double ESTIMATE_MAX_DEPTH_GRADIENT;
double DEPTH_GRADIENT_CONTRAST_THRESHOLD;

bool DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_INTERPIXEL;
int DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH;
int DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT;

bool DEFAULT_NORMAL_MAP_GENERATION_INTERPIXEL;
int DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH;
int DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT;

bool DEFAULT_CONTRAST_MAP_GENERATION_INTERPIXEL;
int DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_WIDTH;
int DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_HEIGHT;


//OR_QUADRATIC_FIT...

double MAXIMUM_GRADIENT_OF_CURVATURE_WITH_RESPECT_TO_ORIENTATION_OF_CURVE;
double MAXIMUM_ALPHA_TO_BETA_GRADIENT_RATIO;
bool USE_EDGISE_3X3_KERNEL;
bool USE_EDGISE_5X5_KERNEL;
double A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_5X5_KERNEL;  			 //(-EDGE_LUMINOSITY_CONTRAST_THRESHOLD/5)
double A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD_3X3_KERNEL;  			 //(-EDGE_LUMINOSITY_CONTRAST_THRESHOLD/5)
double HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD;						 //NB this represents the optimised values for 2DOD, consider/test using (256*1.0) for 3DOD...!!
double HEITGER_FEATURE_RGB_MAP_KERNEL_THRESHOLD;	 //NB this represents the optimised values for 2DOD, consider/test using (HEITGER_FEATURE_RGB_MAP_CENTRE_THRESHOLD/4.0) for 3DOD...!!
double HEITGER_FEATURE_RGB_MAP_TOTAL_KERNEL_THRESHOLD;



int QUADRATIC_FIT_KERNEL_SIZE;
double ZERO_CROSSING_POSITION_THRESHOLD;
double A3A4_COEFFICIENT_NEGATIVE_CURVATURE_POINT_THRESHOLD;



//OR OPTIMISATION IMAGE CROPPING VARIABLES...

int WINDOWS_MINIMUM_HORIZONTAL_WINDOW_SIZE;
int ATOR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO;
int ATOR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO;	//has to result in a small image <= 8x8 pixels (single DCT block)

int ATOR_METHOD_2DOD_NORM_SNAPSHOT_X;	//secondary
int ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y;	//secondary
int ATOR_METHOD_3DOD_NORM_SNAPSHOT_X;	//secondary
int ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y;	//secondary

/*
int ATOR_METHOD_XDOD_SNAPSHOT_SIZE;			//must choose largest snapshot out of 2DOD and 3DOD
int ATOR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE;		//must choose largest snapshot out of 2DOD and 3DOD
*/
bool ATOR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP;
bool ATOR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP;
int ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X;
int ATOR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y;
double ATOR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH;
int ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X;
int ATOR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y;
double ATOR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH;



//OR DISPLAY VARIABLES...

bool ATOR_PRINT_ALGORITHM_PROGRESS;		//true in xml/testing
bool ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS;	//true in xml/testing
bool ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL;

bool ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML;	//true in xml/testing
bool ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL;	//true in xml/testing
bool ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS;	//true in xml/testing
bool DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW;		//for 2DOD, this will stop data outside of the normalised triangle object data from being culled
char ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME[100];



//OR ACCURACY/TUNING VARIABLES...

double ATOR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD;



//OR TRANSFORMATION ACCURACY VARIABLES...

bool ATOR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST;
bool ATOR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST;
double POLYGON_MIN_ANGLE_DEGREES;

double ATOR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON;	//this needs to be made dynamic in the future! [based upon distance object is away from pov]
double ATOR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON;	//100 //20



//ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...

int ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
int ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;



//OR OPTIMISATION GEOMETRIC COMPARISON...

bool ATOR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY;			 //untested - default OFF
bool ATOR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY;	 //untested - default OFF

int NUMBER_OF_POLYGONS_PER_FEATURE;
int ATOR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;

int ATOR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM;



double ATOR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE;

double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE;
double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE;

double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION;
double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION;

double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS;	//(40)
double ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS;	//(40)



//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

bool ATOR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON;

double ATOR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD;



//OR SQL PIXMAPS VARIABLES...

int ATOR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH;



//OR SPARE PARAMETERS VARIABLES...
bool ATOR_RULES_XML_SPARE_PARAMETER_1;
double ATOR_RULES_XML_SPARE_PARAMETER_2;




