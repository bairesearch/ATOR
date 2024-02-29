 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORoperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_OPERATIONS
#define HEADER_ATOR_OPERATIONS

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
#define ATOR_ZERO_ANOMALY_FEATURE_X_POS (0)
#define ATOR_ZERO_ANOMALY_FEATURE_Y_POS (0)
#define ATOR_BACKGROUND_ANOMALY_Z_POS (0.0)

//#define ZCOORDINATES_DEPTH_CONVERSION_INTO_PIXELS ()	//image size/view size?	//this needs to be made dynamic - it is dependant upon POV
//#define XYCOORDINATES_CONVERSION_INTO_PIXELS (20)	//image size/view size?	//this needs to be made dynamic - it is dependant upon POV

#define MAX_DISTANCE (9999999999.9)

//not used anymore;
#ifdef ATOR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION
	#define FEATURES_BOOLEAN_MAP_RECONCILIATION_WIDTH (5)
	#define FEATURES_BOOLEAN_MAP_RECONCILIATION_HEIGHT (5)
#endif



class ATORoperationsClass
{
	private: RTpixelMapsClass RTpixelMaps;
	private: RTsceneClass RTscene;
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	public: void printPointMap(int imageWidth, const int imageHeight, double* pointMap);
	public: void printDepthMap(const int imageWidth, const int imageHeight, const double* depthMap);
	public: void printvi(const RTviewInfo* vi);


	public: void createPointMapFromDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, double* pointMap, RTviewInfo* vi);


		public: bool checkFeatureListForCommonFeature(const vec* corner, const ATORfeature* firstFeatureInList, const double maxFeatureDistanceError, const bool checkAlsoZ);
	public: void generateBooleanMapFromFeatureList(const int imageWidth, const int imageHeight, const ATORfeature* firstFeatureInList,  bool* featuresMap, const RTviewInfo* vi, const int zoom);
	private: void generateBooleanMapFromFeatureListOLD(const int imageWidth, const int imageHeight, const ATORfeature* firstFeatureInList,  bool* featuresMap);
	private: bool generatePolygonListUsingFeatureList(const int imageWidth, const int imageHeight, const ATORfeature firstFeatureInList[], ATORpolygon firstPolygonInList[], const int numberOfZoomIndicies);
	public: bool generatePolygonListUsingFeatureListLocalised(const int imageWidth, const int imageHeight, const ATORfeature firstFeatureInList[], ATORpolygon firstPolygonInList[], const int numberOfZoomIndicies, const int dimension);
		public: int calculateNumberOfNearestFeatures(const int numberOfPolygonsPerFeature, const int numberOfNearbyFeaturesToTransform);
		public: void generateNearestFeaturesList(ATORfeature* firstFeatureInNearestFeatureList, const int numberOfNearestFeatures);
		private: ATORpolygon* addPolysToListForGivenFeatureAndNearestFeatureList(ATORpolygon* firstCurrentPolygonInList, const ATORpolygon* firstPolygonInList, ATORfeature* firstFeatureInNearestFeatureList, const int numberOfPolygonsPerFeature, const int dimension);
		private: bool checkFeatureListForCommonFeatureBasic(const ATORfeature* corner, const ATORfeature* firstFeatureInList);

		private: bool checkPolygonListForCommonPolygon(const ATORpolygon* polygon, const ATORpolygon* firstPolygonInList);

	private: bool addFeatureToListAndIfCommonFeatureExistsTakeAverage(vec* proposedFeature, ATORfeature* firstFeatureInList, const double maxFeatureDistanceError, const bool checkAlsoZ);


	public: void createInterpolatedPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointMapInterpolated);


	public: void storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap);
	public: void restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap);
	public: void applyTransformationMatrixToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, mat* transformationMatrix);
	public: void applyTranslationToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, vec* translationVector);
};



#endif




