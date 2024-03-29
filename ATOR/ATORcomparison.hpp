 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORcomparison.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_COMPARISON
#define HEADER_ATOR_COMPARISON

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "ATORimagecomparison.hpp"
#include "ATORdatabaseSQL.hpp"
#include "ATORdatabaseDecisionTree.hpp"
#include "ATORdatabaseDecisionTreeOperations.hpp"
#include "ATORdatabaseFileIO.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORoperations.hpp"
#include "LDreferenceManipulation.hpp"
#include "LDjpeg.hpp"

//#ifdef ATOR_PRINT_ALGORITHM_AND_TIME_DETAILS
extern int64_t time5aNormalisedSnapshotComparisonLoadComparisonDataTotal;
extern int64_t time5aNormalisedSnapshotComparisonLoadComparisonDataIndex;

extern int64_t time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal;
extern int64_t time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex;
//#endif

//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
class ATORcomparisonClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: RTpixelMapsClass RTpixelMaps;
	private: ATORoperationsClass ATORoperations;
	private: ATORimagecomparisonClass ATORimagecomparison;
	#ifdef ATOR_IMAGE_COMPARISON_SQL
	private: ATORdatabaseSQLClass ATORdatabaseSQL;
	#endif
	private: ATORdatabaseDecisionTreeClass ATORdatabaseDecisionTree;
	private: ATORdatabaseDecisionTreeOperationsClass ATORdatabaseDecisionTreeOperations;
	private: ATORdatabaseFileIOClass ATORdatabaseFileIO;
	private: LDjpegClass LDjpeg;
	public: void fillDCTcoeffSelectionArrays();
	//#endif

	#ifdef ATOR_IMAGE_COMPARISON_SQL
	public: double compareNormalisedSnapshots(const int numberOfTestPolys[], const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTestZoomIndicies, const int trainOrTest, const int testViewNumber);
	#else
	public: double compareNormalisedSnapshots(const int numberOfTrainPolys[], const int numberOfTestPolys[], const int numberOfTrainViewIndicies, const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string trainObjectNameArray[], const int numberOfTrainObjects, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTrainZoomIndicies, const int numberOfTestZoomIndicies, const int testViewNumber);
	#endif

	//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	public: void convertNormalisedHueDeviationMapTo3x8bitMap(int imageWidth, const int imageHeight, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, uchar* rgbDev8BitSmallMapFacingPoly);
		public: void cullAndBinNormalisedHueContrast(vec* normalisedHueContrast, colour* culledNormalisedHueContrast);
	//#endif
	//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	private: void convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(schar dctCoeffArrayY[], schar dctCoeffArrayYcr[], schar dctCoeffArrayYcb[], schar concatonatedSignedDctCoeffArray[]);
	//#endif

	private: void convertImageFileType(const string* imageBaseFileName, const string* imageBaseFileNameConverted, const string imageExtension, const string imageExtensionConverted);
	#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES
	private: void createGeoTableHTMLfromFeatureList(const ATORfeature* firstFeatureInNearestFeatureList, const bool applyBinning, string* geoTableHTMLoutputString);
	#endif
	/*
	#ifdef DEBUG_OR_OUTPUT_DT_BIN
	bool determineIfGeoBinningIdenticalMatchFound(ATORfeature* firstFeatureInNearestFeatureList, int pBinxRequirement[], int pBinyRequirement[]);
	#endif
	*/
	public: void readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(const string* rgbMapSmallFacingPolyFileNamePPM, const string* rgbMapSmallFacingPolyFileNameJPEG, schar* concatonatedSignedDctCoeffArrayRequirement, const bool printOutput);
};


#endif




