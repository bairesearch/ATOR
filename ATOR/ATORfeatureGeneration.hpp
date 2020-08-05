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
 * File Name: ATORfeatureGeneration.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3n6a 01-August-2020
 * /
 *******************************************************************************/


#ifndef HEADER_OR_FEATURE
#define HEADER_OR_FEATURE

#include "ATORglobalDefs.hpp"
#include "ATORpolygonList.hpp"
#include "ATORoperations.hpp"
#include "ATORquadraticFit.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORimagecomparison.hpp"
#include "SHAREDvector.hpp"

#define CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST (1)
#define CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST (2)

#define OR_FEATURE_DETECTION_ZOOM (1)

#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	//not used anymore
	#define OR_FEATURE_DETECTION_NUM_KERNEL_ORIENTATIONS (4)
	#define OR_FEATURE_DETECTION_MIN_ORIENTATIONS_WITH_HIGH_CONTRAST (OR_FEATURE_DETECTION_NUM_KERNEL_ORIENTATIONS)
	#define OR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH (5)	//2 either side of 1 central pixel, where the 2 either side must each be similar in colour
	#define OR_FEATURE_DETECTION_SCAN_LINE_KERNEL_CONTRAST_STACK_SIZE (OR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH-1)
	#define OR_FEATURE_DETECTION_SCAN_LINE_KERNEL_NO_CONTRAST_MAX_THRESHOLD (0.15)
	#define OR_FEATURE_DETECTION_SCAN_LINE_KERNEL_HIGH_CONTRAST_MIN_THRESHOLD (0.2)
#endif

#define HEITGER_FEATURE_INTERPRETATION_KERNEL_HEIGHT (3)
#define HEITGER_FEATURE_INTERPRETATION_KERNEL_WIDTH (3)


/*high level methods*/
class ORfeatureGenerationClass
{
	private: RTsceneClass RTscene;
	private: RTpixelMapsClass RTpixelMaps;
	private: ORquadraticFitClass ORquadraticFit;
	private: ORpixelMapsClass ORpixelMaps;
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	private: ORimagecomparisonClass ORimagecomparison;
	public: void generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit(ORfeature* firstFeatureInFeatureList, unsigned char* featureRgbMap, int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const bool interpixelRGBmapType);
	public: void generateEdgeListFromRGBmapWithQuadraticFit(unsigned char* rgbMap, bool* edgeBoolMap, ORQFzeroCrossing* edgeZeroCrossingMap[], int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const int interpixelContrastMapType);
		public: void generateEdgeListFromContrastMapWithQuadraticFit(const double* contrastMap, bool* edgeBoolMap, ORQFzeroCrossing* edgeZeroCrossingMap[], int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const int interpixelContrastMapType);

	/*Edge ORfeature methods*/

	public: bool addCentredFeaturesToFeatureListUsingContrastMap(ORfeature* firstFeatureInFeatureList, int imageWidth, int imageHeight, const double contrastMap[], const double sensitivity, int dimension, double pointMap[], double depthMap[], int zoom, RTviewInfo* vi, const bool useEdgeZeroCrossingMap, const int interpixelContrastMapType);
		private: bool defineRegionCheckNextPixelNonRecursive(ORpixelContiguous* firstInPixelContiguousStack, int xInitial, int yInitial, const bool edgeBoolMap[], constEffective ORQFzeroCrossing* edgeZeroCrossingMap[], int alreadyProcessed[], int imageWidth, const int imageHeight, double* regionSize, int* sumX, int* sumY, vec* sumPos, const int dimension, double pointMap[], const double depthMap[], int zoom, const bool useEdgeZeroCrossingMap, RTviewInfo* vi, int* maxXx, int* maxXy, const int interpixelContrastMapType);
		private: bool traceEdgeCheckNextPixelNonRecursive(int xInitialOnBoundary, int yInitialOnBoundary, int alreadyProcessed[], constEffective ORQFzeroCrossing* edgeZeroCrossingMap[], const bool useEdgeZeroCrossingMap, int imageWidth, const int imageHeight, double* sumX, double* sumY, vec* sumPos, int* numberOfCounts, const int minRegionCircumferenceForRegion, const int dimension, double pointMap[], const double depthMap[], int zoom, ORpixelContiguous* firstInPixelContiguousBoundaryStack, RTviewInfo* vi, const int interpixelContrastMapType);
		private: double getDepthValueWithOrWithoutForegroundCheck(const double pointNonWorldCoordx, const double pointNonWorldCoordy, const int imageWidth, const int imageHeight, const int kernelWidthForegroundCheck, const int kernelHeightForegroundCheck, const double depthMap[], const int zoom);

	public: bool addCentredFeaturesToFeatureListUsingMeshList(ORfeature* firstFeatureInFeatureList, const double sensitivity, int dimension, ORmeshPoint* firstMeshPointInMeshList, const int contrastValChosen, const bool useEdgeZeroCrossingMap);
		private: bool defineRegionCheckNextPixelUsingMeshPointNonRecursive(ORpixelContiguous* firstInPixelContiguousStack, ORmeshPoint* firstMeshPoint, double* regionSize, const int contrastValChosen, int* sumX, int* sumY, vec* sumPos, const bool useEdgeZeroCrossingMap, ORmeshPoint* aMeshPointOnTheBoundary, const double sensitivity);
		private: bool traceEdgeCheckNextPixelUsingMeshPointNonRecursive(ORmeshPoint* aMeshPointOnTheBoundary, double* sumX, double* sumY, vec* sumPos, int* numberOfCounts, const int minRegionCircumferenceForRegion, ORpixelContiguous* firstInPixelContiguousBoundaryStack, const bool useEdgeZeroCrossingMap);

		private: ORfeature* traceEdgeAndAddMinimaAndMaximaFeatures(ORfeature* firstNewFeatureInList, const double centroidX, const double centroidY, const vec* centroidPos, ORpixelContiguous* firstInPixelContiguousBoundaryStack, const int dimension, int zoom, const int circumferenceForRegion);

	/*Old high level methods - point ORfeature methods*/
	public: void generateFeatureListFromHeitgerFeatureRGBMap(ORfeature* firstFeatureInFeatureList, unsigned char* featureRgbMap, int imageWidth, const int imageHeight, const unsigned char* rgbMap, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi);
	#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	public: void generateFeatureListFromRGBMap(ORfeature* firstFeatureInFeatureList, unsigned char* rgbMap, int imageWidth, const int imageHeight, int zoom, const string trainOrTestString, const string mapFileName, const RTviewInfo* vi);
	#endif
	public: void generateFeatureListFromHeitgerFeatureAsciiMap(ORfeature* firstFeatureInFeatureList, const string featureMapFileNameAscii, int zoom, const RTviewInfo* vi);

	/*
	void clearContiguousStackRecursive(ORpixelContiguous* currentInPixelContiguousStack, int alreadyProcessed[], int imageWidth, int imageHeight);	//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
	void clearContiguousStackRecursive(ORpixelContiguous* currentInPixelContiguousStack, int imageWidth, int imageHeight);	//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
	*/
	private: void deleteContiguousStackAndResetEdgesNonRecursive(ORpixelContiguous* firstInPixelContiguousStack, int alreadyProcessed[], const int imageWidth, const int imageHeight);
	private: void deleteContiguousStackAndResetEdgesNonRecursive(ORpixelContiguous* firstInPixelContiguousStack);
};

#endif

