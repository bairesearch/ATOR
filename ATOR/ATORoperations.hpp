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
 * File Name: ATORoperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3m12a 15-January-2018
 *
 *******************************************************************************/


#ifndef HEADER_OR_OPERATIONS
#define HEADER_OR_OPERATIONS

#include "ATORglobalDefs.hpp"
#include "RTppm.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "RTscene.hpp"	//required for view info construct
#include "ATORpixelMaps.hpp"
#include "LDreferenceClass.hpp"
#include "RToperations.hpp"
#include "SHAREDvector.hpp"


//#defien USE_ORIGINAL_WRONG_FEATURE_GEN



#define SCENE_FILE_NAME_EXTENSION ".ldr"


	//issues;
#define OR_ZERO_ANOMALY_FEATURE_X_POS (0)
#define OR_ZERO_ANOMALY_FEATURE_Y_POS (0)
#define OR_BACKGROUND_ANOMALY_Z_POS (0.0)

//#define ZCOORDINATES_DEPTH_CONVERSION_INTO_PIXELS ()	//image size/view size?	//this needs to be made dynamic - it is dependant upon POV
//#define XYCOORDINATES_CONVERSION_INTO_PIXELS (20)	//image size/view size?	//this needs to be made dynamic - it is dependant upon POV

#define MAX_DISTANCE (9999999999.9)

//not used anymore;
#ifdef OR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION
	#define FEATURES_BOOLEAN_MAP_RECONCILIATION_WIDTH (5)
	#define FEATURES_BOOLEAN_MAP_RECONCILIATION_HEIGHT (5)
#endif



class ORoperationsClass
{
	private: RTpixelMapsClass RTpixelMaps;
	private: RTsceneClass RTscene;
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	public: void printPointMap(int imageWidth, const int imageHeight, double* pointMap);
	public: void printDepthMap(const int imageWidth, const int imageHeight, const double* depthMap);
	public: void printvi(const RTviewInfo* vi);


	public: void createPointMapFromDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, double* pointMap, RTviewInfo* vi);


		public: bool checkFeatureListForCommonFeature(const vec* corner, const ORfeature* firstFeatureInList, const double maxFeatureDistanceError, const bool checkAlsoZ);
	public: void generateBooleanMapFromFeatureList(const int imageWidth, const int imageHeight, const ORfeature* firstFeatureInList,  bool* featuresMap, const RTviewInfo* vi, const int zoom);
	private: void generateBooleanMapFromFeatureListOLD(const int imageWidth, const int imageHeight, const ORfeature* firstFeatureInList,  bool* featuresMap);
	private: bool generatePolygonListUsingFeatureList(const int imageWidth, const int imageHeight, const ORfeature firstFeatureInList[], ORpolygon firstPolygonInList[], const int numberOfZoomIndicies);
	public: bool generatePolygonListUsingFeatureListLocalised(const int imageWidth, const int imageHeight, const ORfeature firstFeatureInList[], ORpolygon firstPolygonInList[], const int numberOfZoomIndicies, const int dimension);
		public: int calculateNumberOfNearestFeatures(const int numberOfPolygonsPerFeature, const int numberOfNearbyFeaturesToTransform);
		public: void generateNearestFeaturesList(ORfeature* firstFeatureInNearestFeatureList, const int numberOfNearestFeatures);
		private: ORpolygon* addPolysToListForGivenFeatureAndNearestFeatureList(ORpolygon* firstCurrentPolygonInList, const ORpolygon* firstPolygonInList, ORfeature* firstFeatureInNearestFeatureList, const int numberOfPolygonsPerFeature, const int dimension);
		private: bool checkFeatureListForCommonFeatureBasic(const ORfeature* corner, const ORfeature* firstFeatureInList);

		private: bool checkPolygonListForCommonPolygon(const ORpolygon* polygon, const ORpolygon* firstPolygonInList);

	private: bool addFeatureToListAndIfCommonFeatureExistsTakeAverage(vec* proposedFeature, ORfeature* firstFeatureInList, const double maxFeatureDistanceError, const bool checkAlsoZ);


	public: void createInterpolatedPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointMapInterpolated);


	public: void storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap);
	public: void restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap);
	public: void applyTransformationMatrixToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, mat* transformationMatrix);
	public: void applyTranslationToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, vec* translationVector);
};



#endif




