 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORmethod3DOD.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_METHOD_3DOD
#define HEADER_ATOR_METHOD_3DOD

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

#define ATOR_SNAPSHOT_SCALE_FACTOR (10)	//10 looks good -  CHECK THIS - why is this not 1.0?

//not currently used;
#ifndef USE_OPENGL
	//#define ATOR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U_OPENGL (1.0);	//not used as orthogonal 3d viewport used
	#define ATOR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U (25.0)
#endif


class ATORmethod3DODClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: ATORoperationsClass ATORoperations;
	private: ATORpolygonListClass ATORpolygonList;
	private: ATORfeatureGenerationClass ATORfeatureGeneration;
	private: RTsceneClass RTscene;
	private: RTpixelMapsClass RTpixelMaps;
	private: ATORpixelMapsClass ATORpixelMaps;
	public: void transformObjectData3DOD(LDreference* firstReferenceInInterpolated3DRGBMap, ATORpolygon* currentPolygonInList, const int side, const bool first, ATORfeature* firstFeatureInList);

	public: void calculateEyePositionAndOrientation3DOD(vec* eyeFacingPoly, vec* viewAtFacingPoly, vec* viewUpFacingPoly, vec* viewPortWidthHeightDepth, ATORpolygon* pol, const int side);


	public: void create3DmeshUsingPointMap3DOD(int imageWidth, const int imageHeight, double* pointMap, const double* depthMap, uchar* rgbMap, ATORmeshPoint* firstMeshPointInMeshList, ATORmeshPoint* meshPointArray[], const bool useEdgeZeroCrossingMap, const int contrastValChosen, RTviewInfo* vi);

	public: void createInterpolated3DmeshReferenceListUsingPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointMapInterpolated, uchar* rgbMap, LDreference* firstReferenceInInterpolated3Dmap);

	public: void create3DMeshReferenceListUsingPointMap(int imageWidth, int imageHeight, double* pointMap, uchar* rgbMap, ATORmeshPoint* firstMeshPointInMeshList);

	#ifdef ATOR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION
	public: bool generateFeatureList3DOD(RTviewInfo* vi, const double* depthMap, double* pointMap, const bool* depthContrastBooleanMap, const bool* luminosityContrastBooleanMap, const bool* luminosityContrastMapMinusDepthContrastMap, ATORfeature* firstFeatureInList, const int trainOrTest);

	private: void reconcileFeaturesMap(const int imageWidth, const int imageHeight, bool* featuresBooleanMap);
	private: void createFeaturesUsingBooleanMap(const int imageWidth, const int imageHeight, const bool* booleanMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], const RTviewInfo* vi);
	private: void createFeaturesUsingBooleanMapUsingDepthMap(const int imageWidth, const int imageHeight, const bool* booleanMap, const double* depthMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], RTviewInfo* vi, const double* pointMap);
	private: void createFeaturesUsingBooleanMapUsingPointMap(int imageWidth, const int imageHeight, const bool* booleanMap, double* pointMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3]);
	//void createFeaturesUsingBooleanMap(int imageWidth, int imageHeight, bool* booleanMap, double* depthMap, bool* featuresUsingDepthContrastMap, bool* featuresUsingDepthContrastMapComplete);

	private: void generateFeatureListUsingFeatureArrays(const int imageWidth, const int imageHeight, const int maxDotProductResultXposArrayComplete[3][3][3], const int maxDotProductResultYposArrayComplete[3][3][3], ATORfeature* firstFeatureInList, const RTviewInfo* vi);
	private: void generateFeatureListUsingFeatureArraysUsingDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, const int maxDotProductResultXposArrayComplete[3][3][3], const int maxDotProductResultYposArrayComplete[3][3][3], ATORfeature* firstFeatureInList, RTviewInfo* vi);
	private: void generateFeatureListUsingFeatureArraysUsingPointMap(int imageWidth, const int imageHeight, double* pointMap, const int maxDotProductResultXposArrayComplete[3][3][3], const int maxDotProductResultYposArrayComplete[3][3][3], ATORfeature* firstFeatureInList);
	#endif

};



#endif
