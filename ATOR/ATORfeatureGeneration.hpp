 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORfeatureGeneration.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_FEATURE
#define HEADER_ATOR_FEATURE

#include "ATORglobalDefs.hpp"
#include "ATORpolygonList.hpp"
#include "ATORoperations.hpp"
#include "ATORquadraticFit.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORimagecomparison.hpp"
#include "SHAREDvector.hpp"

#define CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST (1)
#define CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST (2)

#define ATOR_FEATURE_DETECTION_ZOOM (1)

#ifndef ATOR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	//not used anymore
	#define ATOR_FEATURE_DETECTION_NUM_KERNEL_ORIENTATIONS (4)
	#define ATOR_FEATURE_DETECTION_MIN_ORIENTATIONS_WITH_HIGH_CONTRAST (ATOR_FEATURE_DETECTION_NUM_KERNEL_ORIENTATIONS)
	#define ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH (5)	//2 either side of 1 central pixel, where the 2 either side must each be similar in colour
	#define ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_CONTRAST_STACK_SIZE (ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_LENGTH-1)
	#define ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_NO_CONTRAST_MAX_THRESHOLD (0.15)
	#define ATOR_FEATURE_DETECTION_SCAN_LINE_KERNEL_HIGH_CONTRAST_MIN_THRESHOLD (0.2)
#endif

#define HEITGER_FEATURE_INTERPRETATION_KERNEL_HEIGHT (3)
#define HEITGER_FEATURE_INTERPRETATION_KERNEL_WIDTH (3)


/*high level methods*/
class ATORfeatureGenerationClass
{
	private: RTsceneClass RTscene;
	private: RTpixelMapsClass RTpixelMaps;
	private: ATORquadraticFitClass ATORquadraticFit;
	private: ATORpixelMapsClass ATORpixelMaps;
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	private: ATORimagecomparisonClass ATORimagecomparison;
	public: void generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit(ATORfeature* firstFeatureInFeatureList, uchar* featureRgbMap, int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const bool interpixelRGBmapType);
	public: void generateEdgeListFromRGBmapWithQuadraticFit(uchar* rgbMap, bool* edgeBoolMap, ATORQFzeroCrossing* edgeZeroCrossingMap[], int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const int interpixelContrastMapType);
		public: void generateEdgeListFromContrastMapWithQuadraticFit(const double* contrastMap, bool* edgeBoolMap, ATORQFzeroCrossing* edgeZeroCrossingMap[], int imageWidth, const int imageHeight, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi, const int interpixelContrastMapType);

	/*Edge ATORfeature methods*/

	public: bool addCentredFeaturesToFeatureListUsingContrastMap(ATORfeature* firstFeatureInFeatureList, int imageWidth, int imageHeight, const double contrastMap[], const double sensitivity, int dimension, double pointMap[], double depthMap[], int zoom, RTviewInfo* vi, const bool useEdgeZeroCrossingMap, const int interpixelContrastMapType);
		private: bool defineRegionCheckNextPixelNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack, int xInitial, int yInitial, const bool edgeBoolMap[], constEffective ATORQFzeroCrossing* edgeZeroCrossingMap[], int alreadyProcessed[], int imageWidth, const int imageHeight, double* regionSize, int* sumX, int* sumY, vec* sumPos, const int dimension, double pointMap[], const double depthMap[], int zoom, const bool useEdgeZeroCrossingMap, RTviewInfo* vi, int* maxXx, int* maxXy, const int interpixelContrastMapType);
		private: bool traceEdgeCheckNextPixelNonRecursive(int xInitialOnBoundary, int yInitialOnBoundary, int alreadyProcessed[], constEffective ATORQFzeroCrossing* edgeZeroCrossingMap[], const bool useEdgeZeroCrossingMap, int imageWidth, const int imageHeight, double* sumX, double* sumY, vec* sumPos, int* numberOfCounts, const int minRegionCircumferenceForRegion, const int dimension, double pointMap[], const double depthMap[], int zoom, ATORpixelContiguous* firstInPixelContiguousBoundaryStack, RTviewInfo* vi, const int interpixelContrastMapType);
		private: double getDepthValueWithOrWithoutForegroundCheck(const double pointNonWorldCoordx, const double pointNonWorldCoordy, const int imageWidth, const int imageHeight, const int kernelWidthForegroundCheck, const int kernelHeightForegroundCheck, const double depthMap[], const int zoom);

	public: bool addCentredFeaturesToFeatureListUsingMeshList(ATORfeature* firstFeatureInFeatureList, const double sensitivity, int dimension, ATORmeshPoint* firstMeshPointInMeshList, const int contrastValChosen, const bool useEdgeZeroCrossingMap);
		private: bool defineRegionCheckNextPixelUsingMeshPointNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack, ATORmeshPoint* firstMeshPoint, double* regionSize, const int contrastValChosen, int* sumX, int* sumY, vec* sumPos, const bool useEdgeZeroCrossingMap, ATORmeshPoint* aMeshPointOnTheBoundary, const double sensitivity);
		private: bool traceEdgeCheckNextPixelUsingMeshPointNonRecursive(ATORmeshPoint* aMeshPointOnTheBoundary, double* sumX, double* sumY, vec* sumPos, int* numberOfCounts, const int minRegionCircumferenceForRegion, ATORpixelContiguous* firstInPixelContiguousBoundaryStack, const bool useEdgeZeroCrossingMap);

		private: ATORfeature* traceEdgeAndAddMinimaAndMaximaFeatures(ATORfeature* firstNewFeatureInList, const double centroidX, const double centroidY, const vec* centroidPos, ATORpixelContiguous* firstInPixelContiguousBoundaryStack, const int dimension, int zoom, const int circumferenceForRegion);

	/*Old high level methods - point ATORfeature methods*/
	public: void generateFeatureListFromHeitgerFeatureRGBMap(ATORfeature* firstFeatureInFeatureList, uchar* featureRgbMap, int imageWidth, const int imageHeight, const uchar* rgbMap, const double sensitivity, const int dimension, double* pointMap, const double* depthMap, int zoom, RTviewInfo* vi);
	#ifndef ATOR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	public: void generateFeatureListFromRGBMap(ATORfeature* firstFeatureInFeatureList, uchar* rgbMap, int imageWidth, const int imageHeight, int zoom, const string trainOrTestString, const string mapFileName, const RTviewInfo* vi);
	#endif
	public: void generateFeatureListFromHeitgerFeatureAsciiMap(ATORfeature* firstFeatureInFeatureList, const string featureMapFileNameAscii, int zoom, const RTviewInfo* vi);

	/*
	void clearContiguousStackRecursive(ATORpixelContiguous* currentInPixelContiguousStack, int alreadyProcessed[], int imageWidth, int imageHeight);	//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
	void clearContiguousStackRecursive(ATORpixelContiguous* currentInPixelContiguousStack, int imageWidth, int imageHeight);	//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
	*/
	private: void deleteContiguousStackAndResetEdgesNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack, int alreadyProcessed[], const int imageWidth, const int imageHeight);
	private: void deleteContiguousStackAndResetEdgesNonRecursive(ATORpixelContiguous* firstInPixelContiguousStack);
};

#endif

