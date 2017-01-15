/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction.
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
 * File Name: ORfeature.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3d2d 14-April-2014
 *
 *******************************************************************************/



#ifndef HEADER_OR_FEATURE
#define HEADER_OR_FEATURE

#include "ORglobalDefs.h"
#include "ORpolygonList.h"
#include "ORoperations.h"
#include "ORquadraticFit.h"

	#include <string>
	#include <iostream>
	#include <fstream>
	#include <time.h>
	#include <math.h>
	using namespace std;




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
void generateFeatureListFromHeitgerFeatureRGBmapWithQuadraticFit(Feature * firstFeatureInFeatureList, unsigned char * featureRgbMap, int imageWidth, int imageHeight, double sensitivity, int dimension, double * pointMap, double * depthMap, int zoom, ViewInfo * vi, bool interpixelRGBmapType);
void generateEdgeListFromRGBmapWithQuadraticFit(unsigned char * rgbMap, bool * edgeBoolMap, QFzeroCrossing * edgeZeroCrossingMap[], int imageWidth, int imageHeight, double sensitivity, int dimension, double * pointMap, double * depthMap, int zoom, ViewInfo * vi, int interpixelContrastMapType);
	void generateEdgeListFromContrastMapWithQuadraticFit(double * contrastMap, bool * edgeBoolMap, QFzeroCrossing * edgeZeroCrossingMap[], int imageWidth, int imageHeight, double sensitivity, int dimension, double * pointMap, double * depthMap, int zoom, ViewInfo * vi, int interpixelContrastMapType);

/*Edge Feature methods*/

bool addCentredFeaturesToFeatureListUsingContrastMap(Feature * firstFeatureInFeatureList, int imageWidth, int imageHeight, double contrastMap[], double sensitivity, int dimension, double pointMap[], double depthMap[], int zoom, ViewInfo * vi, bool useEdgeZeroCrossingMap, int interpixelContrastMapType);
	bool defineRegionCheckNextPixelNonRecursive(PixelContiguous * firstInPixelContiguousStack, int xInitial, int yInitial, bool edgeBoolMap[], QFzeroCrossing * edgeZeroCrossingMap[], int alreadyProcessed[], int imageWidth, int imageHeight, double * regionSize, int * sumX, int * sumY, vec * sumPos, int dimension, double pointMap[], double depthMap[], int zoom, bool useEdgeZeroCrossingMap, ViewInfo * vi, int * maxXx, int * maxXy, int interpixelContrastMapType);
	bool traceEdgeCheckNextPixelNonRecursive(int xInitialOnBoundary, int yInitialOnBoundary, int alreadyProcessed[], QFzeroCrossing * edgeZeroCrossingMap[], bool useEdgeZeroCrossingMap, int imageWidth, int imageHeight, double * sumX, double * sumY, vec * sumPos, int * numberOfCounts, int minRegionCircumferenceForRegion, int dimension, double pointMap[], double depthMap[], int zoom, PixelContiguous * firstInPixelContiguousBoundaryStack, ViewInfo * vi, int interpixelContrastMapType);
	double getDepthValueWithOrWithoutForegroundCheck(double pointNonWorldCoordx, double pointNonWorldCoordy, int imageWidth, int imageHeight, int kernelWidthForegroundCheck, int kernelHeightForegroundCheck, double depthMap[], int zoom);

bool addCentredFeaturesToFeatureListUsingMeshList(Feature * firstFeatureInFeatureList, double sensitivity, int dimension, MeshPoint * firstMeshPointInMeshList, int contrastValChosen, bool useEdgeZeroCrossingMap);
	bool defineRegionCheckNextPixelUsingMeshPointNonRecursive(PixelContiguous * firstInPixelContiguousStack, MeshPoint * firstMeshPoint, double * regionSize, int contrastValChosen, int * sumX, int * sumY, vec * sumPos, bool useEdgeZeroCrossingMap, MeshPoint * aMeshPointOnTheBoundary, double sensitivity);
	bool traceEdgeCheckNextPixelUsingMeshPointNonRecursive(MeshPoint * aMeshPointOnTheBoundary, double * sumX, double * sumY, vec * sumPos, int * numberOfCounts, int minRegionCircumferenceForRegion, PixelContiguous * firstInPixelContiguousBoundaryStack, bool useEdgeZeroCrossingMap);

	Feature * traceEdgeAndAddMinimaAndMaximaFeatures(Feature * firstNewFeatureInList, double centroidX, double centroidY, vec * centroidPos, PixelContiguous * firstInPixelContiguousBoundaryStack, int dimension, int zoom, int circumferenceForRegion);

/*Old high level methods - point Feature methods*/
void generateFeatureListFromHeitgerFeatureRGBMap(Feature * firstFeatureInFeatureList, unsigned char * featureRgbMap, int imageWidth, int imageHeight, unsigned char * rgbMap, double sensitivity, int dimension, double * pointMap, double * depthMap, int zoom, ViewInfo * vi);
#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
//void generateFeatureListFromRGBMap(Feature * firstFeatureInFeatureList, unsigned char * rgbMap, int imageWidth, int imageHeight);
void generateFeatureListFromRGBMap(Feature * firstFeatureInFeatureList, unsigned char * rgbMap, int imageWidth, int imageHeight, int zoom, string trainOrTestString, string mapFileName, ViewInfo * vi);
#endif
void generateFeatureListFromHeitgerFeatureAsciiMap(Feature * firstFeatureInFeatureList, string featureMapFileNameAscii, int zoom, ViewInfo * vi);

/*
void clearContiguousStackRecursive(PixelContiguous * currentInPixelContiguousStack, int alreadyProcessed[], int imageWidth, int imageHeight);	//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
void clearContiguousStackRecursive(PixelContiguous * currentInPixelContiguousStack, int imageWidth, int imageHeight);	//Dont use this - dangerous might reach recursion limit when contiguos regions are large but undefined
*/
void deleteContiguousStackAndResetEdgesNonRecursive(PixelContiguous * firstInPixelContiguousStack, int alreadyProcessed[], int imageWidth, int imageHeight);
void deleteContiguousStackAndResetEdgesNonRecursive(PixelContiguous * firstInPixelContiguousStack);

#endif

