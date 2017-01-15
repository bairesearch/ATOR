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
 * File Name: SHAREDglobalDefs.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Description: Generic Construct Functions: shared global definitions (configure to compile different BAI projects)
 * Project Version: 3g1a 14-October-2015
 *
 *******************************************************************************/


#include "ORglobalDefs.h"

//OR_IMAGE_COMPARISON_DECISION_TREE...

bool OR_IMAGE_COMPARISON_DECISION_TREE;

bool OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING;
bool OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING;
bool OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING;
bool OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON;

bool OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING;
bool OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING;
bool OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING;
bool OR_IMAGE_COMPARISON_SQL_SMALL_HUE_DEV_MAP_COMPARISON;

bool OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING;
bool OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING;
bool OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING;
bool OR_IMAGE_COMPARISON_SMALL_HUE_DEV_MAP_COMPARISON;

bool OR_IMAGE_COMPARISON_TEST_GEO_BIN_ACCURACY;

bool OR_IMAGE_COMPARISON_SQL_GET_TEST_DATA_FROM_SQL;		 //tested - default ON [this should be able to be turned off for optimisation purposes, it needs to fix up memory leak however] - this will not be changable in xml file, it should never need to be changed in release versions
bool OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE;  	 //tested - default ON [turn this off to display html results file properly] - this will not be changable in xml file, it should never need to be changed in release versions


//OR_METHOD_GEOMETRIC_COMPARISON...

bool OR_METHOD_TRANSFORM_NEARBY_FEATURES;

bool OR_SQL_DATABASE_STORE_ALL_NEARBY_AND_OT_FEATURES;

bool OR_METHOD_TRANSFORM_KEY_OT_FEATURES;
bool OR_METHOD_TRANSFORM_ALL_FEATURES;
bool OR_METHOD_CREATE_OT_FEATURES_FILE;
bool OR_METHOD_CREATE_ALL_FEATURES_FILE;

bool OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TEST_FILE_IO;
bool OR_METHOD_CREATE_NEARBY_FEATURES_FILE;
bool OR_METHOD_CREATE_NEARBY_FEATURES_FILE_ALWAYS;
bool OR_METHOD_CREATE_NEARBY_FEATURES_FILE_DURING_TEST;


//OR_FEATURES...

//xml determined by default;
bool OR_USE_FIND_CORNER_FEATURES;
bool OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES;
bool OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES;
bool OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY;

bool OR_METHOD_USE_MESH_LISTS;   //[has not been tested with centred feature detection]
bool OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED;
double OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE;
bool OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED;	//never use this - not yet implemented

//dynamically determined by default;
bool OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP;
bool OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP;
bool OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED;
bool OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
bool OR_METHOD2DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP_BOOL;
bool OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_DIAGONALS;
bool OR_TRACE_CONTIGUOUS_REGION_BOUNDARY_RESET_EDGES_DYNAMICALLY_OPT;
bool OR_METHOD3DOD_USE_3D_EDGE_POINTS_CALCULATED_DURING_QUADRATIC_FIT_INSTEAD_OF_DURING_EDGE_TRACE;
bool OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_TRACE_MIN_MAX_NOT_TESTED_YET;
bool OR_CREATE_INTERPOLATED_MESH_REFERENCE_LIST_USING_MESH_LIST;


double MIN_REGION_SIZE_TO_CALCULATE_CENTRED_FEATURE;					 //this needs to be made dynamic based upon vi
int MINIMUM_NUMBER_OF_PIXELS_IN_A_BOUNDARY;
bool OR_USE_FIND_CONTIGUOUS_REGION_CENTRED_FEATURES_BOUNDARY_FEATURES_DO_NOT_USE_UNCHANGING_CASE;		 //tested - default OFF
double OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE;					 //this needs to be made dynamic based upon vi
double OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_NOISE_2;  //this needs to be made dynamic based upon vi  	 //this needs to be checked, this value might need to be increased slightly
double OR_METHOD_3DOD_CONTINUOUS_EDGE_MAX_POINT_MOVEMENT;				 //this needs to be made dynamic based upon vi
double OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE;					 //this needs to be made dynamic based upon zoom??
double OR_METHOD_2DOD_CONTINUOUS_EDGE_MAX_NOISE_2;  //this needs to be made dynamic based upon zoom??	 //this needs to be checked, this value might need to be increased slightly


//OR_METHOD3DOD FEATURE...

bool OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST;
bool OR_METHOD3DOD_USE_SHAPE_CONTRAST_INSTEAD_OF_LUMINOSITY_CONTRAST_FOR_FEATURE_DETECTION;
bool OR_METHOD3DOD_USE_NORMAL_CONTRAST_INSTEAD_OF_DEPTH_GRADIENT_AND_DEPTH_GRADIENT_CONTRAST_FOR_SHAPE_CONTRAST;

double OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE;

//OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING...

int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY;			//deterministic dct coeff comparison requirement
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED; 	//deterministic dct coeff comparison requirement

string OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_STRING;			//deterministic dct coeff comparison requirement
string OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_DCT_QUALITY_CHROMA_ENHANCED_STRING; 	//deterministic dct coeff comparison requirement

bool OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_COLOUR_INFORMATION;
bool OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_MATCHED_LUM_CHROMA_SUBSAMPLING;
bool OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_USE_ENHANCED_CHROMA_SUBSAMPLING;

string OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_JPG_CHROMA_SUBSAMPLING_STRING;

int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCr;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_YCb;

int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_0_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_1_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_2_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_3_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_4_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_5_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_6_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_7_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_Y_CELL_8_Y;

int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_0_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_1_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_2_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_3_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_4_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_5_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_6_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_7_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCR_CELL_8_Y;

int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_0_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_1_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_2_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_3_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_4_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_5_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_6_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_7_Y;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_X;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DCT_COEFF_SELECTION_ARRAY_YCB_CELL_8_Y;

int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
int OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS;

//OR_SHARED_VARS...	//general contrast threshold constraints

bool OR_USE_CONTRAST_CALC_METHOD_C;			//RTglobalDefs.h	//OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with OR_USE_CONTRAST_CALC_METHOD_B
bool OR_USE_CONTRAST_CALC_METHOD_B;			//RTglobalDefs.h	//OR_USE_CONTIGUOUS_REGION_FIND_CENTRED_FEATURES_SUBPIXEL_ACCURACY may be more recommended with OR_USE_CONTRAST_CALC_METHOD_B

double LUMINOSITY_CONTRAST_FRACTION_THRESHOLD;		//RTglobalDefs.h
double DEPTH_GRADIENT_CONTRAST_FRACTION_THRESHOLD;	//this needs to be dynamic - not static - and should be dependant upon focal length
double DEPTH_CONTRAST_FRACTION_THRESHOLD;		//this needs to be dynamic - not static - and should be dependant upon focal length
double POINT_NORMAL_CONTRAST_FRACTION_THRESHOLD;

double LUMINOSITY_FRACTION_THRESHOLD;			//RTglobalDefs.h
double OR_MAX_DEPTH_NOISE;
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



//OR_METHOD2DOD...

bool OR_IMAGE_COMPARISON_PATTERN_NEVER_CULL_SNAPSHOTS;
bool OR_METHOD_DO_NOT_CULL_SNAPSHOT;
bool OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME;



//OR_METHOD...

bool OR_METHOD_SUPPORT_HIGH_LEVEL_SCALING;

bool OR_METHOD2DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS;
int OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
bool OR_METHOD3DOD_SUPPORT_SNAPSHOTS_AT_MULTIPLE_ZOOMS;
int OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;

int OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
int OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST;
int OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
int OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST;




//LD_OPENGL...

int OR_SNAPSHOT_WINDOW_POSITION_X;		 //dependent upon resolution of monitor - should be moved to command line parameter
int OR_SNAPSHOT_WINDOW_POSITION_Y;	 //dependent upon resolution of monitor - should be moved to command line parameter



//OR_IMAGE_COMPARISON...

double OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION;			 //not tested or used yet
double OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION;  		 //not tested or used yet
double OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION;		 //5
double OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION;
double IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR_BASE;
double IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR;
double OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF;
double OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS;		 //need to change to (100.0/(28*28/2)) after testing
double OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF;			 //needs to be tested



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
int OR_METHOD_2DOD_USE_SMALL_IMAGE_RATIO;
int OR_METHOD_3DOD_USE_SMALL_IMAGE_RATIO;	//has to result in a small image <= 8x8 pixels (single DCT block)

int OR_METHOD_2DOD_NORM_SNAPSHOT_X;	//secondary
int OR_METHOD_2DOD_NORM_SNAPSHOT_Y;	//secondary
int OR_METHOD_3DOD_NORM_SNAPSHOT_X;	//secondary
int OR_METHOD_3DOD_NORM_SNAPSHOT_Y;	//secondary

/*
int OR_METHOD_XDOD_SNAPSHOT_SIZE;			//must choose largest snapshot out of 2DOD and 3DOD
int OR_METHOD_XDOD_SNAPSHOT_SMALL_IMAGE_SIZE;		//must choose largest snapshot out of 2DOD and 3DOD
*/
bool OR_METHOD_2DOD_USE_NORM_SNAPSHOT_CROP;
bool OR_METHOD_3DOD_USE_NORM_SNAPSHOT_CROP;
int OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_X;
int OR_METHOD_2DOD_NORM_SNAPSHOT_CROP_Y;
double OR_METHOD_2DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH;
int OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_X;
int OR_METHOD_3DOD_NORM_SNAPSHOT_CROP_Y;
double OR_METHOD_3DOD_NORM_SNAPSHOT_UNCROPPED_WIDTH_TO_CROPPED_WIDTH;



//OR DISPLAY VARIABLES...

bool OR_PRINT_ALGORITHM_PROGRESS;		//true in xml/testing
bool OR_PRINT_ALGORITHM_AND_TIME_DETAILS;	//true in xml/testing
bool OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL;

bool OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML;	//true in xml/testing
bool OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL;	//true in xml/testing
bool OR_GENERATE_IMAGE_COMPARITOR_RESULTS_NO_EXPLICIT_CONFIDENTIAL_WARNINGS;	//true in xml/testing
bool DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW;		//for 2DOD, this will stop data outside of the normalised triangle object data from being culled
char OR_GENERATE_IMAGE_COMPARITOR_RESULTS_FNAME[100];



//OR ACCURACY/TUNING VARIABLES...

double OR_IMAGE_COMPARITOR_MATCH_ERROR_THRESHOLD;



//OR TRANSFORMATION ACCURACY VARIABLES...

bool OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST;
bool OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST;
double POLYGON_MIN_ANGLE_DEGREES;

double OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON;	//this needs to be made dynamic in the future! [based upon distance object is away from pov]
double OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON;	//100 //20



//OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING...

int OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL;
int OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING_MULTIPLIER;



//OR OPTIMISATION GEOMETRIC COMPARISON...

bool OR_METHOD_ONLY_USE_ONE_POLY_PER_FEATURE_FAST_BUT_LOW_REDUNDANCY;			 //untested - default OFF
bool OR_METHOD_ONLY_USE_TWO_NEAREST_FEATURES_TO_COMPARE_FAST_BUT_LOW_REDUNDANCY;	 //untested - default OFF

int NUMBER_OF_POLYGONS_PER_FEATURE;
int OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;

int OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM;



double OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR;
double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NEAREST_FEATURE_MAX_DISTANCE;

double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_RANGE;
double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_RANGE;

double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION;
double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION;

double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS;	//(40)
double OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS;	//(40)



//OR OPTIMISATION SMALL IMAGE COMPARISON VARIABLES...

bool OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON;

double OR_IMAGE_COMPARITOR_SMALL_MATCH_ERROR_THRESHOLD;



//OR SQL PIXMAPS VARIABLES...

int OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_DATA_LENGTH;



//OR SPARE PARAMETERS VARIABLES...
bool OR_RULES_XML_SPARE_PARAMETER_1;
double OR_RULES_XML_SPARE_PARAMETER_2;




