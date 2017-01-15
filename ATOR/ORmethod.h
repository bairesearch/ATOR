/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ORmethod.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3c2a 19-October-2012
 *
 *******************************************************************************/

#ifndef HEADER_OR_METHOD
#define HEADER_OR_METHOD

#include "LDreferenceClass.h"
#include "ORpolygonList.h"
#include "RTviewinfo.h"
#include "ORglobalDefs.h"



#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
	#include "ANNexperienceClass.h"
	#include "ANNneuronClass.h"
#endif




#define OR_OBJECT_DATA_SOURCE_GENERATE_DATA (1)
#define OR_OBJECT_DATA_SOURCE_PREEXISTING_DATA (2)
#define OR_OBJECT_DATA_SOURCE_USER_FILE (3)
#define OR_OBJECT_DATA_SOURCE_USER_LIST (4)



#define OR_METHOD3DOD_SNAPSHOT_DISTANCE (20.0)	//20 default




#define PREEXISTING_IMAGE_DATA_VIEW_INDEX_DEGREES (5)

#ifdef OR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA

	#ifdef OR_METHOD2DOD_USE_SINGLE_OBJECT
		#define OR_METHOD2DOD_NUM_OF_OBJECTS (1)
		#ifdef OR_USE_GOOGLE_MAPS
			#define OR_METHOD2DOD_OBJECT_0_NAME "google"
		#elif defined OR_USE_STAR_MAPS
			#define OR_METHOD2DOD_OBJECT_0_NAME "starmap"
		#else
			#define OR_METHOD2DOD_OBJECT_0_NAME "868"
		#endif
		//#define OR_METHOD2DOD_OBJECT_0_NAME "525"
		//#define OR_METHOD2DOD_OBJECT_0_NAME "868"

	#else
		#define OR_METHOD2DOD_NUM_OF_OBJECTS (3)
		#define OR_METHOD2DOD_OBJECT_0_NAME "868"	//200
	#endif
	#define OR_METHOD2DOD_OBJECT_1_NAME "525"	//251
	#define OR_METHOD2DOD_OBJECT_2_NAME "315"	//315
	#define OR_METHOD2DOD_OBJECT_3_NAME "525"
	#define OR_METHOD2DOD_OBJECT_4_NAME "693"
	#define OR_METHOD2DOD_OBJECT_5_NAME "868"
	#define OR_METHOD2DOD_OBJECT_6_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_7_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_8_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_9_NAME "undefined"

#elif defined OR_METHOD2DOD_GENERATE_IMAGE_DATA
	#ifdef OR_METHOD2DOD_USE_SINGLE_OBJECT
		#define OR_METHOD2DOD_NUM_OF_OBJECTS (1)
	#else
		#define OR_METHOD2DOD_NUM_OF_OBJECTS (2)

	#endif
	#define TH_OR_USE_SHIELD_LDR_FILE
	#define OR_METHOD2DOD_OBJECT_0_NAME "shield"
	#define OR_METHOD2DOD_OBJECT_1_NAME "cross"
	#define OR_METHOD2DOD_OBJECT_2_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_3_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_4_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_5_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_6_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_7_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_8_NAME "undefined"
	#define OR_METHOD2DOD_OBJECT_9_NAME "undefined"
#endif

#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
	#ifdef OR_METHOD3DOD_USE_SINGLE_OBJECT
		#define OR_METHOD3DOD_NUM_OF_OBJECTS (1)
	#else
		//#define OR_METHOD3DOD_NUM_OF_OBJECTS (1)

	#endif
	#define OR_METHOD3DOD_OBJECT_0_NAME "cube"
	#define OR_METHOD3DOD_OBJECT_1_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_2_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_3_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_4_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_5_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_6_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_7_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_8_NAME "undefined"
	#define OR_METHOD3DOD_OBJECT_9_NAME "undefined"
#endif



#ifdef OR_METHOD2DOD_TEST_ACROSS_MULTIPLE_VIEWS

	#ifdef OR_USE_GOOGLE_MAPS
		#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (5) //(16)
		#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
	#elif defined OR_USE_STAR_MAPS
		#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (1)
		#ifdef OR_USE_SINGLE_TRAIN_STAR_MAP
			#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
		#else
			#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (32)
		#endif
	#else
		#ifdef TEST_GEO
			#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (2)	//2
			#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
		#else
			#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (16)
			#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
		#endif
	#endif
#else
	#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
	#define OR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (1)
#endif

/*
#ifdef OR_METHOD2DOD_DEBUG_ONLY_USE_SMALL_NUM_POLYGONS
	#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN (6)	//default: 3
	#ifdef OR_DEBUG_ONLY_WANT_TRAIN_VS_TEST_RGB_FILE_KNOWLEDGE
		#ifdef OR_DEBUG_USE_ONLY_ONE_POLY
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (1)
		#elif defined OR_DEBUG_USE_ONLY_TWO_POLY
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (2)
		#else
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (3)
		#endif
	#else
		#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (6)		//default: 6
	#endif
#else
	#ifdef OR_USE_SINGLE_TRAIN_STAR_MAP
		#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN 1000 //(250) (6)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
		#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST 1000 //(250) (6)
	#else
		#ifdef TEST_GEO
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN 500 //(250) (6)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST 500 //(250) (6)
		#else
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN 500 //(250) (6)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
			#define OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST 500 //(250) (6)
		#endif

	#endif

#endif
*/



#ifdef OR_METHOD3DOD_TEST_ACROSS_MULTIPLE_VIEWS
	#define OR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TRAIN (6)
	#define OR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST (6)
#else
	#define OR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
	#define OR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST (1)
#endif

/*
#ifdef OR_METHOD3DOD_DEBUG_ONLY_USE_SMALL_NUM_POLYGONS
	#define OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN (6)
	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON
	#define OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST (6)
	#else
	#define OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST (18)
	#endif
#else
	#define OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN (42)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
	#define OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST (42)
#endif
*/



#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
	#define OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS (3)
	#define OR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS (0.25)		//+/- 15 = +/- ~1deg - depends upon other viewup coordinates; really need to make this dynamic by inserting noise value calculation into calculateEyePositionAndOrientation	[NO: because test and train will be from different view ports, it should be dependent upon world coordinates as is currently the case]
	#define OR_METHOD3DOD_NUM_ORI_NOISE_VALUES (27) 		//(NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS)
	#define OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS (3)
	#define OR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS (0.5)		//+/- 0.2 units - will depend upon size of viewport; really need to make this dynamic by inserting noise value calculation into calculateEyePositionAndOrientation [NO: because test and train will be from different view ports, it should be dependent upon world coordinates as is currently the case]
	#define OR_METHOD3DOD_NUM_POS_NOISE_VALUES (27) 		//(NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS)

	#define OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS (3)
	#define OR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS (1.0)		//1.0 pixels in original image (not snapshot image)
	#define OR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES (9) 		//(NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS)


#else
	#define OR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS (1)
	#define OR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS (1)
	#define OR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS (0.0)		//not used
	#define OR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS (0.0)		//not used
	#define OR_METHOD3DOD_NUM_ORI_NOISE_VALUES (1) 		//(NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS)
	#define OR_METHOD3DOD_NUM_POS_NOISE_VALUES (1) 		//(NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS)
	#define OR_METHOD3DOD_NUM_SCALE_NOISE_VALUES (3) 		//2DOD ONLY

#endif



#define OR_FEED_RGB_MAP			//IDEAL BUT NOT YET TESTED
//#define OR_FEED_LUMINOSITY_MAP
//#define OR_FEED_LUMINOSITY_BOOLEAN_MAP
//#define OR_FEED_LUMINOSITY_CONTRAST_MAP
//#define OR_FEED_LUMINOSITY_CONTRAST_BOOLEAN_MAP	//DEFAULT1
//#define OR_FEED_DEPTH_MAP_3DONLY
//#define OR_FEED_DEPTH_CONTRAST_MAP_3DONLY
//#define OR_FEED_DEPTH_CONTRAST_BOOLEAN_MAP_3DONLY
//#define OR_FEED_DEPTH_GRADIENT_CONTRAST_MAP_3DONLY
//#define OR_FEED_DEPTH_GRADIENT_CONTRAST_BOOLEAN_MAP_3DONLY



#define OR_NUMBER_OF_NN_LAYERS (7)		//NB I am using the THANNwithOpenLRRC.exe object experience number of layers ...
//#define OR_MAX_NUMBER_OF_EPOCHS 1000		//not used - NB 2D NNs do not use folds during training as each (image) experience has a different class target
#ifdef OR_DEBUG_TRAIN_USING_FAR_INSUFFICIENT_NUMBER_EPOCHS
	#define	OR_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 10
#else
	#ifdef OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
		#define	OR_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 10
	#else
		#define	OR_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 1000	//default: 1000
	#endif
#endif



#define OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_SCALE_FACTOR (5.0)
#define OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_WIDTH (120)
#define OR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_HEIGHT (80)

#ifdef OR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA

	#ifdef OR_USE_GOOGLE_MAPS
		#ifdef OR_USE_CROPPED_GOOGLE_MAPS_FOR_BEST_CASE_SCENARIO_TEST
 			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (320)
			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (100)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (OR_METHOD_2DOD_NORM_SNAPSHOT_X)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (OR_METHOD_2DOD_NORM_SNAPSHOT_Y)
		#else
 			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (540)	//1080 	//540	//270
			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (330)	//660	//330	//165
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (OR_METHOD_2DOD_NORM_SNAPSHOT_X)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (OR_METHOD_2DOD_NORM_SNAPSHOT_Y)
		#endif
	#elif defined OR_USE_STAR_MAPS
		#ifdef OR_USE_SINGLE_TRAIN_STAR_MAP
 			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (1600)
			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (800)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (200)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (200)
		#else
 			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (200)
			#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (200)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (100)
			#define OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (100)
		#endif
	#elif defined OR_USE_AMSTERDAM_TEST_IMAGE_DATA
 		#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (768)
		#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (576)
		#define OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (OR_METHOD_2DOD_NORM_SNAPSHOT_X)
		#define OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (OR_METHOD_2DOD_NORM_SNAPSHOT_Y)
	#endif

	#define OR_METHOD2DOD_2D_NEURAL_NETWORK_WIDTH (OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH)
	#define OR_METHOD2DOD_2D_NEURAL_NETWORK_HEIGHT (OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT)

#elif defined OR_METHOD2DOD_GENERATE_IMAGE_DATA
 	#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (768)	//320	//120
	#define TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (576)	//200	//90
	#define OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (OR_METHOD_2DOD_NORM_SNAPSHOT_X)
	#define OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (OR_METHOD_2DOD_NORM_SNAPSHOT_Y)
	#define OR_METHOD2DOD_2D_NEURAL_NETWORK_WIDTH (OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH)
	#define OR_METHOD2DOD_2D_NEURAL_NETWORK_HEIGHT (OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT)
#endif


#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
	#define TH_OR_METHOD3DOD_DEFAULT_IMAGE_WIDTH (400)	//40
	#define TH_OR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT (400)	//30
	#define OR_METHOD3DOD_POLYGON_SNAPSHOT_WIDTH (OR_METHOD_3DOD_NORM_SNAPSHOT_X)
	#define OR_METHOD3DOD_POLYGON_SNAPSHOT_HEIGHT (OR_METHOD_3DOD_NORM_SNAPSHOT_Y)
	#define OR_METHOD3DOD_2D_NEURAL_NETWORK_WIDTH (OR_METHOD3DOD_POLYGON_SNAPSHOT_WIDTH)
	#define OR_METHOD3DOD_2D_NEURAL_NETWORK_HEIGHT (OR_METHOD3DOD_POLYGON_SNAPSHOT_HEIGHT)
#endif




#ifdef OR_METHOD2DOD_GENERATE_IMAGE_DATA
	#ifdef OR_METHOD2DOD_TEST_ACROSS_MULTIPLE_VIEWS
		#define TH_OR_METHOD2DOD_DEFAULT_EYE_X 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT_EYE_Y 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT_EYE_Z 20	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_X 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Y 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_X 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Y -100
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT_FOCAL 4.0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH 16
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT 12
		#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH)
		#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT)
		#define TH_OR_METHOD2DOD_DEFAULT2_EYE_X 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT2_EYE_Y 0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT2_EYE_Z 20	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_X 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_Y 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_X 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_Y -100
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT2_FOCAL 4.0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH 16
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT 12
	#else

		#define TH_OR_METHOD2DOD_DEFAULT_EYE_X 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT_EYE_Y 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT_EYE_Z 20	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_X 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Y 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_X 0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Y -100
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT_FOCAL 4.0
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH 16
		#define TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT 12
		#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH)
		#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT)
		#define TH_OR_METHOD2DOD_DEFAULT2_EYE_X 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT2_EYE_Y 0.0	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT2_EYE_Z 20	//NB eye position 1 during OR train
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_X 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_Y 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_X 30
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_Y -100
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_Z 0
		#define TH_OR_METHOD2DOD_DEFAULT2_FOCAL 4.0
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH 16
		#define TH_OR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT 12
	#endif
#elif defined OR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA
		//vis are not used with preexisting image data, only image size;
	#define TH_OR_METHOD2DOD_DEFAULT_EYE_X 0.0	//NB eye position 1 during OR train
	#define TH_OR_METHOD2DOD_DEFAULT_EYE_Y 0.0	//NB eye position 1 during OR train
	#define TH_OR_METHOD2DOD_DEFAULT_EYE_Z 20	//NB eye position 1 during OR train
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_X 0
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Y 0
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Z 0
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_X 0
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Y -100
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Z 0
	#define TH_OR_METHOD2DOD_DEFAULT_FOCAL 4.0
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH 16
	#define TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT 12
#ifdef OR_USE_SINGLE_TRAIN_STAR_MAP
	#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (200)
	#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (200)
#else
	#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH)
	#define TH_OR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT)
#endif
	#define TH_OR_METHOD2DOD_DEFAULT2_EYE_X 0.0	//NB eye position 1 during OR train
	#define TH_OR_METHOD2DOD_DEFAULT2_EYE_Y 0	//NB eye position 1 during OR train
	#define TH_OR_METHOD2DOD_DEFAULT2_EYE_Z 20	//NB eye position 1 during OR train
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_X 0
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_Y 0
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWAT_Z 0
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_X 0
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_Y -100
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWUP_Z 0
	#define TH_OR_METHOD2DOD_DEFAULT2_FOCAL 4.0
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH 16
	#define TH_OR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT 12

#endif


#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
	#define TH_OR_METHOD3DOD_DEFAULT_EYE_X 15	//NB eye position 1 during OR train
	#define TH_OR_METHOD3DOD_DEFAULT_EYE_Y 20	//NB eye position 1 during OR train
	#define TH_OR_METHOD3DOD_DEFAULT_EYE_Z 10	//NB eye position 1 during OR train
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWAT_X 0
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWAT_Y 0
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWAT_Z 0
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWUP_X 0
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWUP_Y 0
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWUP_Z 100
	#define TH_OR_METHOD3DOD_DEFAULT_FOCAL 20.0
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWSIZE_WIDTH 4
	#define TH_OR_METHOD3DOD_DEFAULT_VIEWSIZE_HEIGHT 4


	#define TH_OR_METHOD3DOD_DEFAULT2_IMAGE_WIDTH (TH_OR_METHOD3DOD_DEFAULT_IMAGE_WIDTH)
	#define TH_OR_METHOD3DOD_DEFAULT2_IMAGE_HEIGHT (TH_OR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT)
	#define TH_OR_METHOD3DOD_DEFAULT2_EYE_X 15	//NB eye position 2 during OR test
	#define TH_OR_METHOD3DOD_DEFAULT2_EYE_Y 20	//NB eye position 2 during OR test
	#define TH_OR_METHOD3DOD_DEFAULT2_EYE_Z 10	//NB eye position 2 during OR test
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWAT_X 0
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWAT_Y 0
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWAT_Z 0
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWUP_X 0
	#ifndef OR_METHOD3DOD_TEST
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWUP_Y 0
	#else
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWUP_Y 50
	#endif
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWUP_Z 100
	#define TH_OR_METHOD3DOD_DEFAULT2_FOCAL 20.0
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWSIZE_WIDTH 4
	#define TH_OR_METHOD3DOD_DEFAULT2_VIEWSIZE_HEIGHT 4
#endif







#define TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_WIDTH (400)
#define TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_HEIGHT (400)
#define TH_OR_FACING_2D_MAP_DEFAULT_EYE_X (TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_WIDTH/2)
#define TH_OR_FACING_2D_MAP_DEFAULT_EYE_Y (TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_HEIGHT/2)
#define TH_OR_FACING_2D_MAP_DEFAULT_EYE_Z -30
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWAT_X (TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_WIDTH/2)
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWAT_Y (TH_OR_FACING_2D_MAP_DEFAULT_IMAGE_HEIGHT/2)
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWAT_Z 0
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWUP_X 0
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWUP_Y -100
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWUP_Z 0
#define TH_OR_FACING_2D_MAP_DEFAULT_FOCAL 2.0
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWSIZE_WIDTH 4
#define TH_OR_FACING_2D_MAP_DEFAULT_VIEWSIZE_HEIGHT 4

#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH (OR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH)
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT (OR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT)
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_X 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_Y 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_Z -5
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_X 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_Y 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_Z 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_X 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_Y -100
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_Z 0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_FOCAL 10.0
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH 4
#define TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT 4



#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH (OR_METHOD3DOD_POLYGON_SNAPSHOT_WIDTH)
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT (OR_METHOD3DOD_POLYGON_SNAPSHOT_HEIGHT)
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_X 0
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_Y 0
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_Z -5.0		//default -5
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_X 0
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_Y 0
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_Z 0		//default 0
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_X 0
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_Y -100
#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_Z 0
#ifdef USE_OPENGL
	#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL 10.0
	#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH 4
	#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT 4

#else
	#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL (4.0)
	#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH 2
	#define TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT 2

#endif






#define DISTANCE_BETWEEN_EYES (2.0)

#define MAX_DOUBLE (99999999.9)
#define IRRELEVANT (1)

bool ORTHMethod(int dimension, int numberOfTrainObjects, string trainObjectNameArray[], int numberOfTestObjects, string testObjectNameArray[], int * numberOfTrainPolys, int * numberOfTestPolys, int objectDataSource, view_info * viTrain, view_info * viTest, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrain, int maxNumberOfPolygonsTest, int numberOfTrainViewIndiciesPerObject, int numberOfTestViewIndiciesPerObject, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies);
	bool ORMethodInitialise(int imageWidthFacingPoly, int imageHeightFacingPoly, bool initialiseTrain, bool initialiseTest, bool clearTrainTable, int dimension, string sqlipaddress, string sqlusername, string sqlpassword);
	bool ORMethodTrainOrTest(int dimension, int numberOfObjects, string objectNameArray[], int objectDataSource, view_info * vi, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygons, int numberOfViewIndiciesPerObject, int numberOfViewIndiciesPerObjectWithUniquePolygons, int * numberOfPolys, int trainOrTest, int numberOfZoomIndicies, int viewNumber, string multViewListFileName);
		bool createOrAddToInterpolatedMeshAndFeaturesList(Reference * initialReferenceInSceneFile, view_info * vi, Reference * firstReferenceInInterpolatedMesh, MeshPoint * firstMeshPointInMeshList, Feature firstFeatureInList[], int trainOrTest, int viewIndex, string objectName, int dimension, int objectDataSource, int numberOfZoomIndicies, bool useEdgeZeroCrossingMap);
			bool createRGBAndPointMap(Reference * initialReferenceInSceneFile, double * pointMap, unsigned char * rgbMap, double * depthMap, view_info * vi, int trainOrTest, int viewIndex, string objectName, int dimension, int objectDataSource);
			bool createOrAddToInterpolatedMeshReferenceListUsingPointAndRGBMap(double * pointMap, unsigned char * rgbMap, Reference * firstReferenceInInterpolatedMesh, view_info * vi, string objectName, int trainOrTest, int dimension, int viewIndex);
				void printInterpolatedMeshReferenceList(Reference * firstReferenceInInterpolatedMesh, view_info * vi, string objectName, int trainOrTest, int dimension, int viewIndex);
			bool createOrAddPointsToFeaturesList(double * pointMap, unsigned char * rgbMap, double * depthMap, Feature firstFeatureInList[], view_info * vi, int trainOrTest, int viewIndex, string objectName, int dimension, int numberOfZoomIndicies, MeshPoint * firstMeshPointInMeshList, MeshPoint * meshPointArray[], bool useEdgeZeroCrossingMap);
		bool createInterpolatedMeshReferenceListUsingMeshList(MeshPoint * firstMeshPointInMeshList, Reference * firstReferenceInInterpolatedMesh, view_info * vi, string objectName, int trainOrTest, int dimension);
			Reference * convertMeshPointToReferences3DOD(MeshPoint * currentMeshPointInMeshList, Reference * firstNewReferenceInInterpolatedMesh);
			Reference * convertMeshPointToReferences2DOD(MeshPoint * currentMeshPointInMeshList, Reference * firstNewReferenceInInterpolatedMesh);

		bool addCornerFeaturesToFeatureListUsingRGBMap(view_info * vi, unsigned char * rgbMap, Feature * firstFeatureInList, int trainOrTest, string mapFileName, double sensitivity, int dimension, double * pointMap, double * depthMap, int zoom, bool interpixelRGBMapType);
	#ifdef OR_IMAGE_COMPARISON_SQL
	bool ORMethodCompareTestWithTrain(int dimension, int numberOfTestObjects, string testObjectNameArray[], int imageWidthFacingPoly, int imageHeightFacingPoly, int * numberOfTestPolys, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTestZoomIndicies, int trainOrTest, int testViewNumber);
	bool ORMethodTrain(int dimension, int numberOfTrainObjects, string trainObjectNameArray[], int * numberOfTrainPolys, int objectDataSource, view_info * viTrain, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrain, int numberOfTrainViewIndiciesPerObject, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int trainOrTest, string sqlipaddress, string sqlusername, string sqlpassword, bool clearTrainTable, int viewNumber, string multViewListFileName);
	bool ORMethodTest(int dimension, int numberOfTestObjects, string testObjectNameArray[], int * numberOfTestPolys, int objectDataSource, view_info * viTest, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTest, int numberOfTestViewIndiciesPerObject, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTestZoomIndicies, int trainOrTest, string sqlipaddress, string sqlusername, string sqlpassword, bool clearTrainTable, int viewNumber, string multViewListFileName);
	#else
	bool ORMethodCompareTestWithTrain(int dimension, int numberOfTrainObjects, string trainObjectNameArray[], int numberOfTestObjects, string testObjectNameArray[], int imageWidthFacingPoly, int imageHeightFacingPoly, int * numberOfTrainPolys, int * numberOfTestPolys, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies, int testViewNumber);
	#endif
	bool ORMethodExit();


#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
	bool generateNormalisedSnapshotsExperienceListUsingPolyList(Reference * firstReferenceInInterpolatedMesh, PolygonBAI * firstPolygonInList, int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrainOrTest, Experience * firstExperienceInList, int * numberOfTrainOrTestPolys, int trainOrTest, int viewIndex, string objectName, int dimension, Feature * firstFeatureInList);
	NeuronContainer * initialiseNormalisedSnapshotNeuralNetwork(NeuronContainer * firstInputNeuronInNetwork, int * numberOfInputNeurons, int numberOfOutputNeurons, int imageWidth, int imageHeight);
	double compareNormalisedSnapshotExperienceListWithNeuralNetwork(Experience * firstExperienceInTestList, NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork, int numberOfInputNeurons, int numberOfOutputNeurons, int numberOfTrainPolySides);
#else
	bool generateNormalisedSnapshotsUsingPolyList(Reference * firstReferenceInInterpolatedMesh, PolygonBAI firstPolygonInList[], int imageWidthFacingPoly, int imageHeightFacingPoly, int maxNumberOfPolygonsTrainOrTest, int numberOfTrainOrTestPolys[], int trainOrTest, int viewIndex, string objectName, int dimension, Feature * firstFeatureInList, int numberOfZoomIndicies);
#endif

#ifdef OR_METHOD_GEOMETRIC_COMPARISON
bool checkIfFeatureContainerWithSameFeatureIndiciesExists(FeatureContainer * firstFeatureContainerInBestMatchList, FeatureContainer * currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList);
#endif


void setNoiseArraysMethod2DOD();
void setNoiseArraysMethod3DOD();

int createViFromMultiViewList(view_info * vi, string fileName, int multiViewViewIndex, int dimension);


#endif
