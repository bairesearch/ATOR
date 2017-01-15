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
 * File Name: ORmethod3DOD.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3f3a 10-July-2015
 *
 *******************************************************************************/


#ifndef HEADER_OR_METHOD_3DOD
#define HEADER_OR_METHOD_3DOD

#include "ORpolygonList.h"
#include "RTviewinfo.h"
#include "ORglobalDefs.h"
#include "LDreferenceClass.h"
#include "ORfeature.h"

#define TRAIN_STRING ".train"
#define TEST_STRING ".test"

#define OR_SNAPSHOT_SCALE_FACTOR (10)	//10 looks good -  CHECK THIS - why is this not 1.0?

//not currently used;
#ifndef USE_OPENGL
	//#define OR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U_OPENGL (1.0);	//not used as orthogonal 3d viewport used
	#define OR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U (25.0)
#endif


void transformObjectData3DOD(LDreference* firstReferenceInInterpolated3DRGBMap, ORpolygon* currentPolygonInList, int side, bool first, ORfeature* firstFeatureInList);

void calculateEyePositionAndOrientation3DOD(vec* eyeFacingPoly, vec* viewAtFacingPoly, vec* viewUpFacingPoly, vec* viewPortWidthHeightDepth, ORpolygon* pol, int side);


void create3DmeshUsingPointMap3DOD(int imageWidth, int imageHeight, double* pointMap, double* depthMap, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], bool useEdgeZeroCrossingMap, int contrastValChosen, RTviewInfo* vi);

void createInterpolated3DmeshReferenceListUsingPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointMapInterpolated, unsigned char* rgbMap, LDreference* firstReferenceInInterpolated3Dmap);


#ifdef OR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION
bool generateFeatureList3DOD(RTviewInfo* vi, double* depthMap, double* pointMap, bool* depthContrastBooleanMap, bool* luminosityContrastBooleanMap, bool* luminosityContrastMapMinusDepthContrastMap, ORfeature* firstFeatureInList, int trainOrTest);

void reconcileFeaturesMap(int imageWidth, int imageHeight, bool* featuresBooleanMap);
void createFeaturesUsingBooleanMap(int imageWidth, int imageHeight, bool* booleanMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], RTviewInfo* vi);
void createFeaturesUsingBooleanMapUsingDepthMap(int imageWidth, int imageHeight, bool* booleanMap, double* depthMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], RTviewInfo* vi, double* pointMap);
void createFeaturesUsingBooleanMapUsingPointMap(int imageWidth, int imageHeight, bool* booleanMap, double* pointMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3]);
	//void createFeaturesUsingBooleanMap(int imageWidth, int imageHeight, bool* booleanMap, double* depthMap, bool* featuresUsingDepthContrastMap, bool* featuresUsingDepthContrastMapComplete);

void generateFeatureListUsingFeatureArrays(int imageWidth, int imageHeight, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList, RTviewInfo* vi);
void generateFeatureListUsingFeatureArraysUsingDepthMap(int imageWidth, int imageHeight, double* depthMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList, RTviewInfo* vi);
void generateFeatureListUsingFeatureArraysUsingPointMap(int imageWidth, int imageHeight, double* pointMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList);

#endif


#endif
