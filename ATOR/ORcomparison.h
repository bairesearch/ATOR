/*******************************************************************************
 *
 * File Name: ORcomparison.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a6a 20-Mar-2012
 *
 *******************************************************************************/



#ifndef HEADER_OR_COMPARISON
#define HEADER_OR_COMPARISON


#include "SHAREDglobalDefs.h"
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


#endif




