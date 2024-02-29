 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORmethod.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_METHOD
#define HEADER_ATOR_METHOD

#include "LDreferenceClass.hpp"
#include "ATORpolygonList.hpp"
#include "RTviewinfo.hpp"
#include "ATORglobalDefs.hpp"
#include "ATORmethod3DOD.hpp"
#include "ATORmethod2DOD.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORoperations.hpp"
#include "ATORfeatureGeneration.hpp"
#include "ATORquadraticFit.hpp"
#include "RTscene.hpp"
#include "LDparser.hpp"
#include "LDreferenceManipulation.hpp"
#include "RTreferenceManipulation.hpp"
#include "SHAREDvector.hpp"
#include "ATORcomparison.hpp"
#include "ATORimagecomparison.hpp"
#include "ATORdatabaseFileIO.hpp"
#include "LDjpeg.hpp";
#ifdef ATOR_IMAGE_COMPARISON_SQL
#include "LDmysql.hpp";
#include "ATORdatabaseSQL.hpp"
#endif
#include "ATORdatabaseDecisionTree.hpp"
#include "ATORdatabaseDecisionTreeOperations.hpp"
	#include "LDopengl.hpp"


/*
#ifdef ATOR_USE_ATOR_NEURAL_NETWORK_COMPARITOR
	#include "ANNexperienceClass.hpp"
	#include "ANNneuronClass.hpp"
#endif
*/



#define ATOR_OBJECT_DATA_SOURCE_GENERATE_DATA (1)
#define ATOR_OBJECT_DATA_SOURCE_PREEXISTING_DATA (2)
#define ATOR_OBJECT_DATA_SOURCE_USER_FILE (3)
#define ATOR_OBJECT_DATA_SOURCE_USER_LIST (4)



#define ATOR_METHOD3DOD_SNAPSHOT_DISTANCE (20.0)	//20 default




#define PREEXISTING_IMAGE_DATA_VIEW_INDEX_DEGREES (5)

#ifdef ATOR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA

	#ifdef ATOR_METHOD2DOD_USE_SINGLE_OBJECT
		#define ATOR_METHOD2DOD_NUM_OF_OBJECTS (1)
		#ifdef ATOR_USE_GOOGLE_MAPS
			#define ATOR_METHOD2DOD_OBJECT_0_NAME "google"
		#elif defined ATOR_USE_STAR_MAPS
			#define ATOR_METHOD2DOD_OBJECT_0_NAME "starmap"
		#else
			#define ATOR_METHOD2DOD_OBJECT_0_NAME "868"
		#endif
		//#define ATOR_METHOD2DOD_OBJECT_0_NAME "525"
		//#define ATOR_METHOD2DOD_OBJECT_0_NAME "868"

	#else
		#define ATOR_METHOD2DOD_NUM_OF_OBJECTS (3)
		#define ATOR_METHOD2DOD_OBJECT_0_NAME "868"	//200
	#endif
	#define ATOR_METHOD2DOD_OBJECT_1_NAME "525"	//251
	#define ATOR_METHOD2DOD_OBJECT_2_NAME "315"	//315
	#define ATOR_METHOD2DOD_OBJECT_3_NAME "525"
	#define ATOR_METHOD2DOD_OBJECT_4_NAME "693"
	#define ATOR_METHOD2DOD_OBJECT_5_NAME "868"
	#define ATOR_METHOD2DOD_OBJECT_6_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_7_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_8_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_9_NAME "undefined"

#elif defined ATOR_METHOD2DOD_GENERATE_IMAGE_DATA
	#ifdef ATOR_METHOD2DOD_USE_SINGLE_OBJECT
		#define ATOR_METHOD2DOD_NUM_OF_OBJECTS (1)
	#else
		#define ATOR_METHOD2DOD_NUM_OF_OBJECTS (2)

	#endif
	#define TH_ATOR_USE_SHIELD_LDR_FILE
	#define ATOR_METHOD2DOD_OBJECT_0_NAME "shield"
	#define ATOR_METHOD2DOD_OBJECT_1_NAME "cross"
	#define ATOR_METHOD2DOD_OBJECT_2_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_3_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_4_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_5_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_6_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_7_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_8_NAME "undefined"
	#define ATOR_METHOD2DOD_OBJECT_9_NAME "undefined"
#endif

#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
	#ifdef ATOR_METHOD3DOD_USE_SINGLE_OBJECT
		#define ATOR_METHOD3DOD_NUM_OF_OBJECTS (1)
	#else
		//#define ATOR_METHOD3DOD_NUM_OF_OBJECTS (1)

	#endif
	#define ATOR_METHOD3DOD_OBJECT_0_NAME "cube"
	#define ATOR_METHOD3DOD_OBJECT_1_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_2_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_3_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_4_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_5_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_6_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_7_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_8_NAME "undefined"
	#define ATOR_METHOD3DOD_OBJECT_9_NAME "undefined"
#endif



#ifdef ATOR_METHOD2DOD_TEST_ACROSS_MULTIPLE_VIEWS

	#ifdef ATOR_USE_GOOGLE_MAPS
		#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (5) //(16)
		#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
	#elif defined ATOR_USE_STAR_MAPS
		#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (1)
		#ifdef ATOR_USE_SINGLE_TRAIN_STAR_MAP
			#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
		#else
			#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (32)
		#endif
	#else
		#ifdef TEST_GEO
			#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (2)	//2
			#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
		#else
			#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (16)
			#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
		#endif
	#endif
#else
	#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
	#define ATOR_METHOD2DOD_NUMBER_OF_VIEWINDICIES_TEST (1)
#endif

/*
#ifdef ATOR_METHOD2DOD_DEBUG_ONLY_USE_SMALL_NUM_POLYGONS
	#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN (6)	//default: 3
	#ifdef ATOR_DEBUG_ONLY_WANT_TRAIN_VS_TEST_RGB_FILE_KNOWLEDGE
		#ifdef ATOR_DEBUG_USE_ONLY_ONE_POLY
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (1)
		#elif defined ATOR_DEBUG_USE_ONLY_TWO_POLY
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (2)
		#else
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (3)
		#endif
	#else
		#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST (6)		//default: 6
	#endif
#else
	#ifdef ATOR_USE_SINGLE_TRAIN_STAR_MAP
		#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN 1000 //(250) (6)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
		#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST 1000 //(250) (6)
	#else
		#ifdef TEST_GEO
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN 500 //(250) (6)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST 500 //(250) (6)
		#else
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN 500 //(250) (6)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
			#define ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TEST 500 //(250) (6)
		#endif

	#endif

#endif
*/



#ifdef ATOR_METHOD3DOD_TEST_ACROSS_MULTIPLE_VIEWS
	#define ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TRAIN (6)
	#define ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST (6)
#else
	#define ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TRAIN (1)
	#define ATOR_METHOD3DOD_NUMBER_OF_VIEWINDICIES_TEST (1)
#endif

/*
#ifdef ATOR_METHOD3DOD_DEBUG_ONLY_USE_SMALL_NUM_POLYGONS
	#define ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN (6)
	#ifdef DEBUG_ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON
	#define ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST (6)
	#else
	#define ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST (18)
	#endif
#else
	#define ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN (42)	//36 required for cross //for Cube only; 10	//Default >= 10 //NB this should be always greater than number of polygons used
	#define ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TEST (42)
#endif
*/



#ifdef ATOR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
	#define ATOR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS (3)
	#define ATOR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS (0.25)		//+/- 15 = +/- ~1deg - depends upon other viewUp coordinates; really need to make this dynamic by inserting noise value calculation into calculateEyePositionAndOrientation	[NO: because test and train will be from different view ports, it should be dependent upon world coordinates as is currently the case]
	#define ATOR_METHOD3DOD_NUM_ORI_NOISE_VALUES (27) 		//(NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS)
	#define ATOR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS (3)
	#define ATOR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS (0.5)		//+/- 0.2 units - will depend upon size of viewport; really need to make this dynamic by inserting noise value calculation into calculateEyePositionAndOrientation [NO: because test and train will be from different view ports, it should be dependent upon world coordinates as is currently the case]
	#define ATOR_METHOD3DOD_NUM_POS_NOISE_VALUES (27) 		//(NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS)

	#define ATOR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES_PER_AXIS (3)
	#define ATOR_METHOD2DOD_FEAT_POS_MAX_NOISE_PER_AXIS (1.0)		//1.0 pixels in original image (not snapshot image)
	#define ATOR_METHOD2DOD_NUM_FEAT_POS_NOISE_VALUES (9) 		//(NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS)


#else
	#define ATOR_METHOD3DOD_NUM_ORI_NOISE_VALUES_PER_AXIS (1)
	#define ATOR_METHOD3DOD_NUM_POS_NOISE_VALUES_PER_AXIS (1)
	#define ATOR_METHOD3DOD_ORI_MAX_NOISE_PER_AXIS (0.0)		//not used
	#define ATOR_METHOD3DOD_POS_MAX_NOISE_PER_AXIS (0.0)		//not used
	#define ATOR_METHOD3DOD_NUM_ORI_NOISE_VALUES (1) 		//(NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS*NUM_ORI_NOISE_VALUES_PER_AXIS)
	#define ATOR_METHOD3DOD_NUM_POS_NOISE_VALUES (1) 		//(NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS*NUM_POS_NOISE_VALUES_PER_AXIS)
	#define ATOR_METHOD3DOD_NUM_SCALE_NOISE_VALUES (3) 		//2DOD ONLY

#endif



#define ATOR_FEED_RGB_MAP			//IDEAL BUT NOT YET TESTED
//#define ATOR_FEED_LUMINOSITY_MAP
//#define ATOR_FEED_LUMINOSITY_BOOLEAN_MAP
//#define ATOR_FEED_LUMINOSITY_CONTRAST_MAP
//#define ATOR_FEED_LUMINOSITY_CONTRAST_BOOLEAN_MAP	//DEFAULT1
//#define ATOR_FEED_DEPTH_MAP_3DONLY
//#define ATOR_FEED_DEPTH_CONTRAST_MAP_3DONLY
//#define ATOR_FEED_DEPTH_CONTRAST_BOOLEAN_MAP_3DONLY
//#define ATOR_FEED_DEPTH_GRADIENT_CONTRAST_MAP_3DONLY
//#define ATOR_FEED_DEPTH_GRADIENT_CONTRAST_BOOLEAN_MAP_3DONLY



#define ATOR_NUMBER_OF_NN_LAYERS (7)		//NB I am using the THANNwithOpenLRRC.exe object experience number of layers ...
//#define ATOR_MAX_NUMBER_OF_EPOCHS 1000		//not used - NB 2D NNs do not use folds during training as each (image) experience has a different class target
#ifndef ATOR_DEBUG_TRAIN_USING_FAR_INSUFFICIENT_NUMBER_EPOCHS
	#ifdef ATOR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
		#define	ATOR_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 10
	#else
		#define	ATOR_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 1000	//default: 1000
	#endif
#endif



#define ATOR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_SCALE_FACTOR (5.0)
#define ATOR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_WIDTH (120)
#define ATOR_DEBUG_RAYTRACE_INTERPOLATED_3D_MAP_WITH_ORIGINAL_VIEW_HEIGHT (80)

#ifdef ATOR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA

	#ifdef ATOR_USE_GOOGLE_MAPS
		#ifdef ATOR_USE_CROPPED_GOOGLE_MAPS_FOR_BEST_CASE_SCENARIO_TEST
 			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (320)
			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (100)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (ATOR_METHOD_2DOD_NORM_SNAPSHOT_X)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y)
		#else
 			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (540)	//1080 	//540	//270
			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (330)	//660	//330	//165
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (ATOR_METHOD_2DOD_NORM_SNAPSHOT_X)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y)
		#endif
	#elif defined ATOR_USE_STAR_MAPS
		#ifdef ATOR_USE_SINGLE_TRAIN_STAR_MAP
 			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (1600)
			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (800)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (200)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (200)
		#else
 			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (200)
			#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (200)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (100)
			#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (100)
		#endif
	#elif defined ATOR_USE_AMSTERDAM_TEST_IMAGE_DATA
 		#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (768)
		#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (576)
		#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (ATOR_METHOD_2DOD_NORM_SNAPSHOT_X)
		#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y)
	#endif

	#define ATOR_METHOD2DOD_2D_NEURAL_NETWORK_WIDTH (ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH)
	#define ATOR_METHOD2DOD_2D_NEURAL_NETWORK_HEIGHT (ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT)

#elif defined ATOR_METHOD2DOD_GENERATE_IMAGE_DATA
 	#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH (768)	//320	//120
	#define TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT (576)	//200	//90
	#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH (ATOR_METHOD_2DOD_NORM_SNAPSHOT_X)
	#define ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT (ATOR_METHOD_2DOD_NORM_SNAPSHOT_Y)
	#define ATOR_METHOD2DOD_2D_NEURAL_NETWORK_WIDTH (ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH)
	#define ATOR_METHOD2DOD_2D_NEURAL_NETWORK_HEIGHT (ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT)
#endif


#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
	#define TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_WIDTH (400)	//40
	#define TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT (400)	//30
	#define ATOR_METHOD3DOD_POLYGON_SNAPSHOT_WIDTH (ATOR_METHOD_3DOD_NORM_SNAPSHOT_X)
	#define ATOR_METHOD3DOD_POLYGON_SNAPSHOT_HEIGHT (ATOR_METHOD_3DOD_NORM_SNAPSHOT_Y)
	#define ATOR_METHOD3DOD_2D_NEURAL_NETWORK_WIDTH (ATOR_METHOD3DOD_POLYGON_SNAPSHOT_WIDTH)
	#define ATOR_METHOD3DOD_2D_NEURAL_NETWORK_HEIGHT (ATOR_METHOD3DOD_POLYGON_SNAPSHOT_HEIGHT)
#endif




#ifdef ATOR_METHOD2DOD_GENERATE_IMAGE_DATA
	#ifdef ATOR_METHOD2DOD_TEST_ACROSS_MULTIPLE_VIEWS
		#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_X 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_Y 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_Z 20	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Y 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Y -100
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_FOCAL 4.0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH 16
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT 12
		#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH)
		#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT)
		#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_X 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Y 0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Z 20	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Y 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Y -100
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_FOCAL 4.0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH 16
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT 12
	#else

		#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_X 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_Y 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_Z 20	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Y 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Y -100
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT_FOCAL 4.0
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH 16
		#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT 12
		#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH)
		#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT)
		#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_X 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Y 0.0	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Z 20	//NB eye position 1 during ATOR train
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_X 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Y 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_X 30
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Y -100
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Z 0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_FOCAL 4.0
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH 16
		#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT 12
	#endif
#elif defined ATOR_METHOD2DOD_USE_PREEXISTING_IMAGE_DATA
		//vis are not used with preexisting image data, only image size;
	#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_X 0.0	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_Y 0.0	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD2DOD_DEFAULT_EYE_Z 20	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_X 0
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Y 0
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Z 0
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_X 0
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Y -100
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Z 0
	#define TH_ATOR_METHOD2DOD_DEFAULT_FOCAL 4.0
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH 16
	#define TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT 12
#ifdef ATOR_USE_SINGLE_TRAIN_STAR_MAP
	#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (200)
	#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (200)
#else
	#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_WIDTH (TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH)
	#define TH_ATOR_METHOD2DOD_DEFAULT2_IMAGE_HEIGHT (TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT)
#endif
	#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_X 0.0	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Y 0	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD2DOD_DEFAULT2_EYE_Z 20	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_X 0
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Y 0
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWAT_Z 0
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_X 0
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Y -100
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWUP_Z 0
	#define TH_ATOR_METHOD2DOD_DEFAULT2_FOCAL 4.0
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_WIDTH 16
	#define TH_ATOR_METHOD2DOD_DEFAULT2_VIEWSIZE_HEIGHT 12

#endif


#ifdef ATOR_METHOD3DOD_GENERATE_IMAGE_DATA
	#define TH_ATOR_METHOD3DOD_DEFAULT_EYE_X 15	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD3DOD_DEFAULT_EYE_Y 20	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD3DOD_DEFAULT_EYE_Z 10	//NB eye position 1 during ATOR train
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_X 0
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_Y 0
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_Z 0
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_X 0
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_Y 0
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_Z 100
	#define TH_ATOR_METHOD3DOD_DEFAULT_FOCAL 20.0
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWSIZE_WIDTH 4
	#define TH_ATOR_METHOD3DOD_DEFAULT_VIEWSIZE_HEIGHT 4


	#define TH_ATOR_METHOD3DOD_DEFAULT2_IMAGE_WIDTH (TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_WIDTH)
	#define TH_ATOR_METHOD3DOD_DEFAULT2_IMAGE_HEIGHT (TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT)
	#define TH_ATOR_METHOD3DOD_DEFAULT2_EYE_X 15	//NB eye position 2 during ATOR test
	#define TH_ATOR_METHOD3DOD_DEFAULT2_EYE_Y 20	//NB eye position 2 during ATOR test
	#define TH_ATOR_METHOD3DOD_DEFAULT2_EYE_Z 10	//NB eye position 2 during ATOR test
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWAT_X 0
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWAT_Y 0
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWAT_Z 0
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_X 0
	#ifndef ATOR_METHOD3DOD_TEST
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_Y 0
	#else
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_Y 50
	#endif
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWUP_Z 100
	#define TH_ATOR_METHOD3DOD_DEFAULT2_FOCAL 20.0
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWSIZE_WIDTH 4
	#define TH_ATOR_METHOD3DOD_DEFAULT2_VIEWSIZE_HEIGHT 4
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

#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH (ATOR_METHOD2DOD_POLYGON_SNAPSHOT_WIDTH)
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT (ATOR_METHOD2DOD_POLYGON_SNAPSHOT_HEIGHT)
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_X 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_Y 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_EYE_Z -5
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_X 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_Y 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWAT_Z 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_X 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_Y -100
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWUP_Z 0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_FOCAL 10.0
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH 4
#define TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT 4



#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH (ATOR_METHOD3DOD_POLYGON_SNAPSHOT_WIDTH)
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT (ATOR_METHOD3DOD_POLYGON_SNAPSHOT_HEIGHT)
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_X 0
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_Y 0
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_EYE_Z -5.0		//default -5
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_X 0
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_Y 0
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWAT_Z 0		//default 0
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_X 0
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_Y -100
#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWUP_Z 0
#ifdef USE_OPENGL
	#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL 10.0
	#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH 4
	#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT 4

#else
	#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_FOCAL (4.0)
	#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_WIDTH 2
	#define TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_VIEWSIZE_HEIGHT 2

#endif






#define DISTANCE_BETWEEN_EYES (2.0)

#define MAX_DOUBLE (99999999.9)
#define IRRELEVANT (1)

class ATORmethodClass
{
	private: LDopenglClass LDopengl;
	private: ATORcomparisonClass ATORcomparison;
	private: SHAREDvarsClass SHAREDvars;
	private: ATORfeatureGenerationClass ATORfeatureGeneration;
	private: ATORoperationsClass ATORoperations;
	private: LDparserClass LDparser;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: ATORdatabaseFileIOClass ATORdatabaseFileIO;
	private: ATORmethod3DODClass ATORmethod3DOD;
	private: ATORmethod2DODClass ATORmethod2DOD;
	private: RTppmClass RTppm;
	private: RTsceneClass RTscene;
	private: RTpixelMapsClass RTpixelMaps;
	private: ATORpixelMapsClass ATORpixelMaps;
	private: RTreferenceManipulationClass RTreferenceManipulation;
	private: SHAREDvectorClass SHAREDvector;
	private: ATORimagecomparisonClass ATORimagecomparison;
	#ifdef ATOR_IMAGE_COMPARISON_SQL
	private: LDmysqlClass LDmysql;
	private: ATORdatabaseSQLClass ATORdatabaseSQL;
	#endif
	private: ATORdatabaseDecisionTreeClass ATORdatabaseDecisionTree;
	private: ATORdatabaseDecisionTreeOperationsClass ATORdatabaseDecisionTreeOperations;
	private: bool ORTHmethod(int dimension, const int numberOfTrainObjects, string trainObjectNameArray[], const int numberOfTestObjects, string testObjectNameArray[], int* numberOfTrainPolys, int* numberOfTestPolys, const int objectDataSource, RTviewInfo* viTrain, RTviewInfo* viTest, int imageWidthFacingPoly, int imageHeightFacingPoly, const int maxNumberOfPolygonsTrain, const int maxNumberOfPolygonsTest, const int numberOfTrainViewIndiciesPerObject, const int numberOfTestViewIndiciesPerObject, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies);
		private: bool ATORmethodInitialise(const int imageWidthFacingPoly, const int imageHeightFacingPoly, const bool initialiseTrain, const bool initialiseTest, const bool clearTrainTable, const int dimension, const string sqlIPaddress, const string sqlUsername, const string sqlPassword);
		private: bool ATORmethodTrainOrTest(int dimension, const int numberOfObjects, string objectNameArray[], const int objectDataSource, RTviewInfo* vi, int imageWidthFacingPoly, int imageHeightFacingPoly, const int maxNumberOfPolygons, const int numberOfViewIndiciesPerObject, int numberOfViewIndiciesPerObjectWithUniquePolygons, int* numberOfPolys, const int trainOrTest, int numberOfZoomIndicies, const int viewNumber, const string multViewListFileName);
			private: bool createOrAddToInterpolatedMeshAndFeaturesList(LDreference* initialReferenceInSceneFile, RTviewInfo* vi, LDreference* firstReferenceInInterpolatedMesh, ATORmeshPoint* firstMeshPointInMeshList, ATORfeature firstFeatureInList[], const int trainOrTest, const int viewIndex, const string objectName, int dimension, const int objectDataSource, const int numberOfZoomIndicies, const bool useEdgeZeroCrossingMap);
				private: bool createRGBandPointMap(LDreference* initialReferenceInSceneFile, double* pointMap, uchar* rgbMap, double* depthMap, RTviewInfo* vi, const int trainOrTest, const int viewIndex, const string objectName, const int dimension, const int objectDataSource);
				private: bool createOrAddToInterpolatedMeshReferenceListUsingPointAndRGBMap(double* pointMap, uchar* rgbMap, LDreference* firstReferenceInInterpolatedMesh, RTviewInfo* vi, const string objectName, const int trainOrTest, const int dimension, const int viewIndex);
					private: void printInterpolatedMeshReferenceList(LDreference* firstReferenceInInterpolatedMesh, RTviewInfo* vi, const string objectName, const int trainOrTest, const int dimension, const int viewIndex);
				private: bool createOrAddPointsToFeaturesList(double* pointMap, uchar* rgbMap, double* depthMap, ATORfeature firstFeatureInList[], RTviewInfo* vi, const int trainOrTest, const int viewIndex, const string objectName, int dimension, const int numberOfZoomIndicies, ATORmeshPoint* firstMeshPointInMeshList, constEffective ATORmeshPoint* meshPointArray[], const bool useEdgeZeroCrossingMap);
			private: bool createInterpolatedMeshReferenceListUsingMeshList(ATORmeshPoint* firstMeshPointInMeshList, LDreference* firstReferenceInInterpolatedMesh, RTviewInfo* vi, const string objectName, const int trainOrTest, const int dimension);
				private: LDreference* convertMeshPointToReferences3DOD(ATORmeshPoint* currentMeshPointInMeshList, LDreference* firstNewReferenceInInterpolatedMesh);
				private: LDreference* convertMeshPointToReferences2DOD(const ATORmeshPoint* currentMeshPointInMeshList, LDreference* firstNewReferenceInInterpolatedMesh);

			private: bool addCornerFeaturesToFeatureListUsingRGBmap(RTviewInfo* vi, uchar* rgbMap, ATORfeature* firstFeatureInList, const int trainOrTest, const string mapFileName, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, const bool interpixelRGBmapType);
		#ifdef ATOR_USE_COMPARISON
		#ifdef ATOR_IMAGE_COMPARISON_SQL
		private: bool ATORmethodCompareTestWithTrain(const int dimension, const int numberOfTestObjects, const string testObjectNameArray[], int imageWidthFacingPoly, int imageHeightFacingPoly, const int* numberOfTestPolys, const int numberOfTestViewIndiciesPerObjectWithUniquePolygons, const int numberOfTestZoomIndicies, const int trainOrTest, const int testViewNumber);
		#else
		//bool ATORmethodCompareTestWithTrain(const int dimension, const int numberOfTrainObjects, const string trainObjectNameArray[], const int numberOfTestObjects, const string testObjectNameArray[], int imageWidthFacingPoly, int imageHeightFacingPoly, const int* numberOfTrainPolys, const int* numberOfTestPolys, const int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, const int numberOfTestViewIndiciesPerObjectWithUniquePolygons, const int numberOfTrainZoomIndicies, const int numberOfTestZoomIndicies, const int testViewNumber);
		#endif
		#endif
		public: bool ATORmethodTrain(int dimension, const int numberOfTrainObjects, string trainObjectNameArray[], int* numberOfTrainPolys, const int objectDataSource, RTviewInfo* viTrain, int imageWidthFacingPoly, int imageHeightFacingPoly, const int maxNumberOfPolygonsTrain, const int numberOfTrainViewIndiciesPerObject, int numberOfTrainViewIndiciesPerObjectWithUniquePolygons, int numberOfTrainZoomIndicies, const int trainOrTest, const string sqlIPaddress, const string sqlUsername, const string sqlPassword, const bool clearTrainTable, const int viewNumber, const string multViewListFileName);
		public: bool ATORmethodTest(int dimension, const int numberOfTestObjects, string testObjectNameArray[], int* numberOfTestPolys, const int objectDataSource, RTviewInfo* viTest, int imageWidthFacingPoly, int imageHeightFacingPoly, const int maxNumberOfPolygonsTest, const int numberOfTestViewIndiciesPerObject, int numberOfTestViewIndiciesPerObjectWithUniquePolygons, int numberOfTestZoomIndicies, const int trainOrTest, const string sqlIPaddress, const string sqlUsername, const string sqlPassword, const bool clearTrainTable, const int viewNumber, const string multViewListFileName);

		private: bool ATORmethodExit();


	#ifdef ATOR_USE_ATOR_NEURAL_NETWORK_COMPARITOR
	//bool generateNormalisedSnapshotsExperienceListUsingPolyList(LDreference* firstReferenceInInterpolatedMesh, ATORpolygon* firstPolygonInList, int imageWidthFacingPoly, int imageHeightFacingPoly, const int maxNumberOfPolygonsTrainOrTest, ANNexperience* firstExperienceInList, int* numberOfTrainOrTestPolys, const int trainOrTest, const int viewIndex, const string objectName, const int dimension, ATORfeature* firstFeatureInList);
		private: ANNneuronContainer* initialiseNormalisedSnapshotNeuralNetwork(const ANNneuronContainer* firstInputNeuronInNetwork, int* numberOfInputNeurons, const int numberOfOutputNeurons, int imageWidth, int imageHeight);
		private: double compareNormalisedSnapshotExperienceListWithNeuralNetwork(ANNexperience* firstExperienceInTestList, const ANNneuronContainer* firstInputNeuronInNetwork, const ANNneuronContainer* firstOutputNeuronInNetwork, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numberOfTrainPolySides);
	#else
		private: bool generateNormalisedSnapshotsUsingPolyList(LDreference* firstReferenceInInterpolatedMesh, ATORpolygon firstPolygonInList[], int imageWidthFacingPoly, int imageHeightFacingPoly, const int maxNumberOfPolygonsTrainOrTest, int numberOfTrainOrTestPolys[], const int trainOrTest, const int viewIndex, const string objectName, const int dimension, ATORfeature* firstFeatureInList, const int numberOfZoomIndicies);
	#endif

	#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
	private: bool checkIfFeatureContainerWithSameFeatureIndiciesExists(const ATORfeatureContainer* firstFeatureContainerInBestMatchList, const ATORfeatureContainer* currentFeatureContainerInMatchListcurrentFeatureContainerInMatchList);
	#endif


	private: void setNoiseArraysMethod2DOD();
	private: void setNoiseArraysMethod3DOD();

	public: int createViFromMultiViewList(RTviewInfo* vi, const string fileName, const int multiViewViewIndex, const int dimension);
};


#endif
