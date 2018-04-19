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
 * File Name: ATORmethod3DOD.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3m12b 15-January-2018
 *
 *******************************************************************************/


#ifndef HEADER_OR_METHOD_3DOD
#define HEADER_OR_METHOD_3DOD

#include "ATORpolygonList.hpp"
#include "RTviewinfo.hpp"
#include "ATORglobalDefs.hpp"
#include "LDreferenceClass.hpp"
#include "ATORfeatureGeneration.hpp"
#include "SHAREDvector.hpp"
#include "ATORoperations.hpp"
#include "LDopengl.hpp"
#include "ATORoperations.hpp"

#define TRAIN_STRING ".train"
#define TEST_STRING ".test"

#define OR_SNAPSHOT_SCALE_FACTOR (10)	//10 looks good -  CHECK THIS - why is this not 1.0?

//not currently used;
#ifndef USE_OPENGL
	//#define OR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U_OPENGL (1.0);	//not used as orthogonal 3d viewport used
	#define OR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U (25.0)
#endif


class ORmethod3DODClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: ORoperationsClass ORoperations;
	private: ORpolygonListClass ORpolygonList;
	private: ORfeatureGenerationClass ORfeatureGeneration;
	private: RTsceneClass RTscene;
	private: RTpixelMapsClass RTpixelMaps;
	private: ORpixelMapsClass ORpixelMaps;
	public: void transformObjectData3DOD(LDreference* firstReferenceInInterpolated3DRGBMap, ORpolygon* currentPolygonInList, const int side, const bool first, ORfeature* firstFeatureInList);

	public: void calculateEyePositionAndOrientation3DOD(vec* eyeFacingPoly, vec* viewAtFacingPoly, vec* viewUpFacingPoly, vec* viewPortWidthHeightDepth, ORpolygon* pol, const int side);


	public: void create3DmeshUsingPointMap3DOD(int imageWidth, const int imageHeight, double* pointMap, const double* depthMap, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], const bool useEdgeZeroCrossingMap, const int contrastValChosen, RTviewInfo* vi);

	public: void createInterpolated3DmeshReferenceListUsingPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointMapInterpolated, unsigned char* rgbMap, LDreference* firstReferenceInInterpolated3Dmap);

	public: void create3DMeshReferenceListUsingPointMap(int imageWidth, int imageHeight, double* pointMap, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList);

	#ifdef OR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION
	public: bool generateFeatureList3DOD(RTviewInfo* vi, const double* depthMap, double* pointMap, const bool* depthContrastBooleanMap, const bool* luminosityContrastBooleanMap, const bool* luminosityContrastMapMinusDepthContrastMap, ORfeature* firstFeatureInList, const int trainOrTest);

	private: void reconcileFeaturesMap(const int imageWidth, const int imageHeight, bool* featuresBooleanMap);
	private: void createFeaturesUsingBooleanMap(const int imageWidth, const int imageHeight, const bool* booleanMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], const RTviewInfo* vi);
	private: void createFeaturesUsingBooleanMapUsingDepthMap(const int imageWidth, const int imageHeight, const bool* booleanMap, const double* depthMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], RTviewInfo* vi, const double* pointMap);
	private: void createFeaturesUsingBooleanMapUsingPointMap(int imageWidth, const int imageHeight, const bool* booleanMap, double* pointMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3]);
	//void createFeaturesUsingBooleanMap(int imageWidth, int imageHeight, bool* booleanMap, double* depthMap, bool* featuresUsingDepthContrastMap, bool* featuresUsingDepthContrastMapComplete);

	private: void generateFeatureListUsingFeatureArrays(const int imageWidth, const int imageHeight, const int maxDotProductResultXposArrayComplete[3][3][3], const int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList, const RTviewInfo* vi);
	private: void generateFeatureListUsingFeatureArraysUsingDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, const int maxDotProductResultXposArrayComplete[3][3][3], const int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList, RTviewInfo* vi);
	private: void generateFeatureListUsingFeatureArraysUsingPointMap(int imageWidth, const int imageHeight, double* pointMap, const int maxDotProductResultXposArrayComplete[3][3][3], const int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList);
	#endif

};



#endif
