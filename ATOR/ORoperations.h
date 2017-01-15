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
 * File Name: ORoperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3i18a 21-September-2016
 *
 *******************************************************************************/


#ifndef HEADER_OR_OPERATIONS
#define HEADER_OR_OPERATIONS

#include "ORglobalDefs.h"
#include "RTppm.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"
#include "RTscene.h"	//required for view info construct
#include "ORpixelMaps.h"
#include "LDreferenceClass.h"


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



void printPointMap(int imageWidth, int imageHeight, double* pointMap);
void printDepthMap(int imageWidth, int imageHeight, double* depthMap);
void printvi(RTviewInfo* vi);


void createPointMapFromDepthMap(int imageWidth, int imageHeight, double* depthMap, double* pointMap, RTviewInfo* vi);


	bool checkFeatureListForCommonFeature(vec* corner, ORfeature* firstFeatureInList, double maxFeatureDistanceError, bool checkAlsoZ);
void generateBooleanMapFromFeatureList(int imageWidth, int imageHeight, ORfeature* firstFeatureInList,  bool* featuresMap, RTviewInfo* vi, int zoom);
void generateBooleanMapFromFeatureListOLD(int imageWidth, int imageHeight, ORfeature* firstFeatureInList,  bool* featuresMap);
bool generatePolygonListUsingFeatureList(int imageWidth, int imageHeight, ORfeature firstFeatureInList[], ORpolygon firstPolygonInList[], int numberOfZoomIndicies);
bool generatePolygonListUsingFeatureListLocalised(int imageWidth, int imageHeight, ORfeature firstFeatureInList[], ORpolygon firstPolygonInList[], int numberOfZoomIndicies, int dimension);
	int calculateNumberOfNearestFeatures(int numberOfPolygonsPerFeature, int numberOfNearbyFeaturesToTransform);
	void generateNearestFeaturesList(ORfeature* firstFeatureInNearestFeatureList, int numberOfNearestFeatures);
	ORpolygon* addPolysToListForGivenFeatureAndNearestFeatureList(ORpolygon* firstCurrentPolygonInList, ORpolygon* firstPolygonInList, ORfeature* firstFeatureInNearestFeatureList, int numberOfPolygonsPerFeature, int dimension);
	bool checkFeatureListForCommonFeatureBasic(ORfeature* corner, ORfeature* firstFeatureInList);

	bool checkPolygonListForCommonPolygon(ORpolygon* polygon, ORpolygon* firstPolygonInList);

bool addFeatureToListAndIfCommonFeatureExistsTakeAverage(vec* proposedFeature, ORfeature* firstFeatureInList, double maxFeatureDistanceError, bool checkAlsoZ);


void createInterpolatedPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointMapInterpolated);


void storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap);
void restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap);
void applyTransformationMatrixToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, mat* transformationMatrix);
void applyTranslationToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, vec* translationVector);



#endif




