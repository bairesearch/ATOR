 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORdatabaseDecisionTreeOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#include "ATORdatabaseDecisionTreeOperations.hpp"

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE

#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength, int concatonatedDctCoeffArrayBiasInt[])
#else
//void convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], char* DCTcoeff64bitValueString, int* DCTcoeff64bitValueStringLength)
#endif
#else
#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
//uint64_t convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[], int concatonatedDctCoeffArrayBiasInt[])
#else
uint64_t ATORdatabaseDecisionTreeOperationsClass::convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(const schar concatonatedSignedDctCoeffArray[])
#endif
#endif
{
	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	*DCTcoeff64bitValueStringLength = 0;
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#else
	uint64_t dctCoeffArrayBinned = 0;
	#endif

	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		int arrayValueSigned = concatonatedSignedDctCoeffArray[i];

		/*
		//OLD; perform interpolation before signed to unsigned conversion

		//perform binning only if necessary/requested
		int numDistintValsPerColumn = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
		if(numDistintValsPerColumn != 1)
		{
			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			double arrayValueSignedDouble = double(arrayValueSigned)/double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL);
			arrayValueSigned = arrayValueSigned/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;

			//cout << "arrayValueSignedDouble = " << arrayValueSignedDouble << endl;
			//cout << "arrayValueSigned = " << arrayValueSigned << endl;

			if(arrayValueSigned >= 0)
			{
				if(arrayValueSignedDouble >= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

				}
				else if(arrayValueSignedDouble <= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;

				}
				else
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
				}
			}
			else
			{//arrayValueSigned < 0

				if(arrayValueSignedDouble >= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

				}
				else if(arrayValueSignedDouble <= (double(arrayValueSigned)+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;

				}
				else
				{
					concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
				}
			}
			#else
			arrayValueSigned = arrayValueSigned/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
			#endif
		}

		if(arrayValueSigned > ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
		{
			arrayValueSigned = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
		}
		else if(arrayValueSigned < -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
		{
			arrayValueSigned = -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
		}
		int arrayValueUnsigned = arrayValueSigned + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;	//convert to unsigned
		*/


		double arrayValueUnsignedDouble;
		uint32_t arrayValueUnsigned = determineDCTBinUnsigned(arrayValueSigned, &arrayValueUnsignedDouble);	//used to be int

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int numDistintValsPerColumn = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
		if(numDistintValsPerColumn != 1)
		{

			if(arrayValueUnsignedDouble >= (double(arrayValueUnsigned)+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
			{
				concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;

			}
			else if(arrayValueUnsignedDouble <= (double(arrayValueUnsigned)+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD))
			{
				concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;
			}
			else
			{
				concatonatedDctCoeffArrayBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
			}
		}
		#endif


	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		//cout << "arrayValueUnsigned = " << arrayValueUnsigned << endl;
		for(int q=0; q<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_IN_BITS; q++)
		{
			if(index == 8)
			{
				index = 0;
				DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = binaryConvertedToChar;
				*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
				binaryConvertedToChar = 0x00;
			}

			bool bitValue = false;
			int arrayValueUnsignedAfterBitShift = int(arrayValueUnsigned) >> q;
			//cout << "arrayValueUnsignedAfterBitShift = " << arrayValueUnsignedAfterBitShift << endl;
			if(arrayValueUnsignedAfterBitShift%2 == 0)
			{//LSb == 0 ; therefore an even number
				bitValue = false;
			}
			else
			{//LSb == 1 ; therefore an odd number
				bitValue =  true;
			}

			if(bitValue)
			{
				binaryConvertedToChar = binaryConvertedToChar | (0x01 << index);
			}
			else
			{
				binaryConvertedToChar = binaryConvertedToChar | (0x00 << index);
			}

			index++;
		}

	#else
		int power = ((ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS-i-1)*2)+1; 	//13,11,9,7,5,3,1
		dctCoeffArrayBinned = dctCoeffArrayBinned + int64_t(arrayValueUnsigned)*powLong(10, power);
	#endif
	}

	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
	//add final character to currentDirectoryCharStar string if necessary
	if(index > 0)
	{
		DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = binaryConvertedToChar;
		*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
	}

	//add null character onto end of string
	DCTcoeff64bitValueString[*DCTcoeff64bitValueStringLength] = '\0';
	*DCTcoeff64bitValueStringLength = *DCTcoeff64bitValueStringLength + 1;
	#else
	return dctCoeffArrayBinned;
	#endif
}

int ATORdatabaseDecisionTreeOperationsClass::determineGeoBinX(const double featurePointTransformedXpos)
{
	int geoBinX = (featurePointTransformedXpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
	return geoBinX;
}

int ATORdatabaseDecisionTreeOperationsClass::determineGeoBinY(const double featurePointTransformedYpos)
{
	int geoBinY = (featurePointTransformedYpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
	return geoBinY;
}

double ATORdatabaseDecisionTreeOperationsClass::determineGeoBinDoubleX(const double featurePointTransformedXpos)
{
	double geoBinX = (featurePointTransformedXpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
	return geoBinX;
}

double ATORdatabaseDecisionTreeOperationsClass::determineGeoBinDoubleY(const double featurePointTransformedYpos)
{
	double geoBinY = (featurePointTransformedYpos / ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);
	return geoBinY;
}


uint32_t ATORdatabaseDecisionTreeOperationsClass::determineDCTBinUnsigned(int arrayValueSigned, double* arrayValueUnsignedDouble)
{
	if(arrayValueSigned > ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		arrayValueSigned = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}
	else if(arrayValueSigned < -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL)
	{
		arrayValueSigned = -ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET/ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	}

	uint32_t arrayValueUnsigned;
	int numDistintValsPerColumn = ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
	if(numDistintValsPerColumn != 1)
	{
		*arrayValueUnsignedDouble = determineDCTBinUnsignedDouble(arrayValueSigned);
		arrayValueUnsigned = (uint32_t)(*arrayValueUnsignedDouble);
	}
	else
	{
		arrayValueUnsigned = arrayValueSigned + ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET;
	}

	return arrayValueUnsigned;
}

double ATORdatabaseDecisionTreeOperationsClass::determineDCTBinUnsignedDouble(const int arrayValueSigned)
{
	double arrayValueUnsignedDouble = double(arrayValueSigned)/double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL) + double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET);
	return arrayValueUnsignedDouble;
}

int64_t ATORdatabaseDecisionTreeOperationsClass::powLong(const int64_t val, const int degree)
{
	int64_t result = 1;
	for(int i=1; i<degree; i++)
	{
		result = result*val;
	}
	return result;
}


//#endif
