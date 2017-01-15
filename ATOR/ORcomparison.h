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
 * File Name: ORcomparison.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3j1a 14-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_COMPARISON
#define HEADER_OR_COMPARISON

#include "ORglobalDefs.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"

//#ifdef OR_PRINT_ALGORITHM_AND_TIME_DETAILS
extern long time5aNormalisedSnapshotComparisonLoadComparisonDataTotal;
extern long time5aNormalisedSnapshotComparisonLoadComparisonDataIndex;

extern long time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal;
extern long time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex;
//#endif

//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void fillDCTcoeffSelectionArrays();
//#endif

#ifdef OR_IMAGE_COMPARISON_SQL
double compareNormalisedSnapshots(const int numberOfTestPolys[], const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTestZoomIndicies, const int trainOrTest, const int testViewNumber);
#else
//double compareNormalisedSnapshots(const int numberOfTrainPolys[], const int numberOfTestPolys[], const int numberOfTrainViewIndicies, const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string trainObjectNameArray[], const int numberOfTrainObjects, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTrainZoomIndicies, const int numberOfTestZoomIndicies, const int testViewNumber);
#endif

//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
void convertNormalisedHueDeviationMapTo3x8bitMap(int imageWidth, const int imageHeight, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, unsigned char* rgbDev8BitSmallMapFacingPoly);
	void cullAndBinNormalisedHueContrast(vec* normalisedHueContrast, colour* culledNormalisedHueContrast);
//#endif
//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(signed char dctCoeffArrayY[], signed char dctCoeffArrayYcr[], signed char dctCoeffArrayYcb[], signed char concatonatedSignedDctCoeffArray[]);
//#endif

void convertImageFileType(const string* imageBaseFileName, const string* imageBaseFileNameConverted, const string imageExtension, const string imageExtensionConverted);
#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
void createGeoTableHTMLfromFeatureList(const ORfeature* firstFeatureInNearestFeatureList, const bool applyBinning, string* geoTableHTMLoutputString);
#endif
/*
#ifdef DEBUG_OR_OUTPUT_DT_BIN
bool determineIfGeoBinningIdenticalMatchFound(ORfeature* firstFeatureInNearestFeatureList, int pBinxRequirement[], int pBinyRequirement[]);
#endif
*/
void readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(const string* rgbMapSmallFacingPolyFileNamePPM, const string* rgbMapSmallFacingPolyFileNameJPEG, signed char* concatonatedSignedDctCoeffArrayRequirement, const bool printOutput);


#endif




