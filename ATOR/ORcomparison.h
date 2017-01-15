/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3c1a 11-October-2012
 *
 *******************************************************************************/



#ifndef HEADER_OR_COMPARISON
#define HEADER_OR_COMPARISON


#include "ORglobalDefs.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <time.h>
	#include <math.h>
	using namespace std;


//#ifdef OR_PRINT_ALGORITHM_AND_TIME_DETAILS
extern long time5aNormalisedSnapshotComparisonLoadComparisonDataTotal;
extern long time5aNormalisedSnapshotComparisonLoadComparisonDataIndex;

extern long time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal;
extern long time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex;
//#endif

//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void filldctCoeffSelectionArrays();
//#endif

#ifdef OR_IMAGE_COMPARISON_SQL
double compareNormalisedSnapshots(int numberOfTestPolys[], int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, string testObjectNameArray[], int numberOfTestObjects, int dimension, int numberOfTestZoomIndicies, int trainOrTest, int testViewNumber);
#else
double compareNormalisedSnapshots(int numberOfTrainPolys[], int numberOfTestPolys[], int numberOfTrainViewIndicies, int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, string trainObjectNameArray[], int numberOfTrainObjects, string testObjectNameArray[], int numberOfTestObjects, int dimension, int numberOfTrainZoomIndicies, int numberOfTestZoomIndicies, int testViewNumber);
#endif

//#ifdef OR_GENERATE_IMAGE_COMPARITOR_RESULTS_HTML
void writeStringToFileObject2(string s, ofstream * writeFileObject);
//#endif

//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
void convertNormalisedHueDeviationMapTo3x8BitMap(int imageWidth, int imageHeight, double * rgbDevIEnormalisedHueContrastMapSmallFacingPoly, unsigned char * rgbDev8BitSmallMapFacingPoly);
	void cullAndBinNormalisedHueContrast(vec * normalisedHueContrast, colour * cullednormalisedHueContrast);
//#endif
//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void convertDCTCoeffIndividualArraysToConcatonatedSignedDctCoeffArray(signed char dctCoeffArrayY[], signed char dctCoeffArrayYCr[], signed char dctCoeffArrayYCb[], signed char concatonatedSignedDctCoeffArray[]);
//#endif

void convertImageFileType(string * imageBaseFileName, string * imageBaseFileNameConverted, string imageExtension, string imageExtensionConverted);
#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
void createGeoTableHTMLfromFeatureList(Feature * firstFeatureInNearestFeatureList, bool applyBinning, string * geoTableHTMLOutputString);
#endif
/*
#ifdef DEBUG_OR_OUTPUT_DT_BIN
bool determineIfGeoBinningIdenticalMatchFound(Feature * firstFeatureInNearestFeatureList, int pBinxRequirement[], int pBinyRequirement[]);
#endif
*/
void readDCTCoeffIndividualArraysAndConvertToConcatonatedSignedDctCoeffArray(string * rgbMapSmallFacingPolyFileNamePPMCPlus, string * rgbMapSmallFacingPolyFileNameJPEGCPlus, signed char * concatonatedSignedDctCoeffArrayRequirement, bool printOutput);


#endif




