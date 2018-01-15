/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: ATORcomparison.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3m11b 10-January-2018
 *
 *******************************************************************************/


#ifndef HEADER_OR_COMPARISON
#define HEADER_OR_COMPARISON

#include "ATORglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "ATORpolygonList.hpp"
#include "ATORimagecomparison.hpp"
#include "ATORdatabaseSQL.hpp"
#include "ATORdatabaseDecisionTree.hpp"
#include "ATORdatabaseFileIO.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORoperations.hpp"
#include "LDreferenceManipulation.hpp"
#include "LDjpeg.hpp"

//#ifdef OR_PRINT_ALGORITHM_AND_TIME_DETAILS
extern long time5aNormalisedSnapshotComparisonLoadComparisonDataTotal;
extern long time5aNormalisedSnapshotComparisonLoadComparisonDataIndex;

extern long time5bNormalisedSnapshotComparisonCompareSnapshotDataTotal;
extern long time5bNormalisedSnapshotComparisonCompareSnapshotDataIndex;
//#endif

//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
class ORcomparisonClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: ORimagecomparisonClass ORimagecomparison;
	private: RTpixelMapsClass RTpixelMaps;
	private: ORoperationsClass ORoperations;
	private: ORdatabaseSQLClass ORdatabaseSQL;
	private: ORdatabaseDecisionTreeClass ORdatabaseDecisionTree;
	private: ORdatabaseFileIOClass ORdatabaseFileIO;
	private: LDjpegClass LDjpeg;
	public: void fillDCTcoeffSelectionArrays();
	//#endif

	#ifdef OR_IMAGE_COMPARISON_SQL
	public: double compareNormalisedSnapshots(const int numberOfTestPolys[], const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTestZoomIndicies, const int trainOrTest, const int testViewNumber);
	#else
	//double compareNormalisedSnapshots(const int numberOfTrainPolys[], const int numberOfTestPolys[], const int numberOfTrainViewIndicies, const int numberOfTestViewIndicies, int imageWidthFacingPoly, int imageHeightFacingPoly, const string trainObjectNameArray[], const int numberOfTrainObjects, const string testObjectNameArray[], const int numberOfTestObjects, const int dimension, const int numberOfTrainZoomIndicies, const int numberOfTestZoomIndicies, const int testViewNumber);
	#endif

	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	public: void convertNormalisedHueDeviationMapTo3x8bitMap(int imageWidth, const int imageHeight, double* rgbDevIEnormalisedHueContrastMapSmallFacingPoly, unsigned char* rgbDev8BitSmallMapFacingPoly);
		public: void cullAndBinNormalisedHueContrast(vec* normalisedHueContrast, colour* culledNormalisedHueContrast);
	//#endif
	//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	private: void convertDCTcoeffIndividualArraysToConcatonatedSignedDCTcoeffArray(signed char dctCoeffArrayY[], signed char dctCoeffArrayYcr[], signed char dctCoeffArrayYcb[], signed char concatonatedSignedDctCoeffArray[]);
	//#endif

	private: void convertImageFileType(const string* imageBaseFileName, const string* imageBaseFileNameConverted, const string imageExtension, const string imageExtensionConverted);
	#ifdef VERBOSE_OR_OUTPUT_GEO_COORDINATES
	private: void createGeoTableHTMLfromFeatureList(const ORfeature* firstFeatureInNearestFeatureList, const bool applyBinning, string* geoTableHTMLoutputString);
	#endif
	/*
	#ifdef DEBUG_OR_OUTPUT_DT_BIN
	bool determineIfGeoBinningIdenticalMatchFound(ORfeature* firstFeatureInNearestFeatureList, int pBinxRequirement[], int pBinyRequirement[]);
	#endif
	*/
	public: void readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(const string* rgbMapSmallFacingPolyFileNamePPM, const string* rgbMapSmallFacingPolyFileNameJPEG, signed char* concatonatedSignedDctCoeffArrayRequirement, const bool printOutput);
};


#endif




