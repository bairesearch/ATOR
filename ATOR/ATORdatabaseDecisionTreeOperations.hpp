 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORdatabaseDecisionTreeOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_DATABASE_DECISIONTREE_OPERATIONS
#define HEADER_ATOR_DATABASE_DECISIONTREE_OPERATIONS

#include "ATORglobalDefs.hpp"

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE

//move this function elsewhere;
class ATORdatabaseDecisionTreeOperationsClass
{
	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength, int concatonatedDctCoeffArrayBiasInt[]);
	#else
	//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength);
	#endif
	#else
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
	//uint64_t convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], int concatonatedDctCoeffArrayBiasInt[]);
	#else
	public: uint64_t convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[]);
	#endif
	#endif

	public: int determineGeoBinX(const double featurePointTransformedXpos);
	public: int determineGeoBinY(const double featurePointTransformedYpos);
	public: double determineGeoBinDoubleX(const double featurePointTransformedXpos);
	public: double determineGeoBinDoubleY(const double featurePointTransformedYpos);
	
	public: uint32_t determineDCTBinUnsigned(int arrayValueSigned, double* arrayValueUnsignedDouble);
	private: double determineDCTBinUnsignedDouble(const int arrayValueSigned);
	
	private: int64_t powLong(const int64_t val, const int degree);
};

//#endif

#endif



