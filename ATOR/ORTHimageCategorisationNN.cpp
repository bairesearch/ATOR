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
 * File Name: ORTHimageCategorisationNN.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3d4a 8-June-2014
 * Test Harness for OR Image Categorisation NN method (not yet implemented)
 *******************************************************************************/

/*Description;

This was considered a possible solution to the information theory problem (image /snapshot categorisation) which looked like it could possibly work (seemed like it was on the right track).

All algorithms I had come up with to categorise snapshot data resemble neural networks, and a subset of the latest one pretty much resembled a NN exactly, which is convenient because my existing NN software can be used to test it rather than having to write something else.

This algorithm is a hybrid binary string / network categorisation method, and have outlined it in case it needs to be written up.

This algorithm utilises an array/series of single (/dual) output neural networks. This would therefore also make sense of the low axon (output) to dendrite (input/connection) ratio observed in brain networks - a phenomenon which I could never make sense of - at least not in the context of how artificial nets are usually used (for selecting between a reasonably large number of self-learned categories [output neurons]).

---

Requires ~ x64 dumb neural networks (NN)
(ie, static/pre-generated - not dynamic/trained)

Each NN gives boolean output
(1 or 0 - true or false)

Either feed in image data ( Eg 8x8 pixels) or ~ 64 DCT coefficients
(values chosen are independent of number of NN used)

(NN are either generated with random weights/biases,
or are optimised to be sensitive
to unique regions of image area / variations in image)

It is required that slight variations to input image
will not change the output value. Require 99.9% repeatability
such that probability of mis-categorisation is < 10% (1 - 0.999^64).

produces 64 bit string (2^64)
(18446744073709551616
unique combinations)

*/


#include "ORTHimageCategorisationNN.h"

#include "ORglobalDefs.h"
#include "XMLrulesClass.h"

#include "ORrules.h"
#include "ORimagecomparison.h"
#include "ORcomparison.h"

#include "RTppm.h"
#include "RTpixelMaps.h"

#include "LDjpeg.h"

#include "ANNneuronClass.h"
#include "ANNFormation.h"
#include "ANNTraining.h"
#include "ANNXMLconversion.h"
#include "ANNsprite.h"
#include "ANNUpdateAlgorithm.h"
#include "ANNdisplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;



FileNameMatch::FileNameMatch(void)
{
	fileName1 = "";
	fileName2 = "";

	next = NULL;

}

FileNameMatch::~FileNameMatch()
{
	if(next != NULL)
	{
		delete next;
	}
}


int main()
{
	bool result = true;

	//srand(static_cast<unsigned>(time(0)));
	srand( (unsigned)time(NULL) );	//seeds randomness

	result = ORTHimageCategorisationNN();

	return result;
}


int ORTHimageCategorisationNN()
{
	//************************
	//PART 0: Standard Initialisations
	//************************

	bool result = true;


	if(!parseORrulesXMLfile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInORrulesExternVariables();

	#define IRRELEVANT (0)

	//************************
	//PART 1: Standard Initialisations; create neural network 1 (for unit properties and unit combat decisions)
	//************************

	int numberOfNeuralNetworks = OR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS;
	int categorisationDatasetBinaryStringUniqueIdentifierNumberOfBits = numberOfNeuralNetworks;		// > 1 trillion categories


	NeuronContainer * firstInputNeuronInNetwork[numberOfNeuralNetworks];
	NeuronContainer * firstOutputNeuronInNetwork[numberOfNeuralNetworks];
	long numberOfInputNeurons = 0;	//dynamically generated based upon experience creation
	long numberOfOutputNeurons = OR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS;

	/*
	OR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE	//not yet coded [will have to emulate close geo values, or extract them from ORcomparison.cpp into results.html]
	OR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE
	OR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE
	*/

	string exampleImageFileName = "315interpolatedMeshViewIndex0ZoomIndex0FacingPoly214side0.rgb.train.ppm";

	#ifdef OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES
	string exampleImageFileNameSmall = exampleImageFileName + ".small.ppm";

	int smallImageRatio = OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES_RATIO;
	string convertImageToSmallImageCommand = "";
	int resizePercentage = 100/smallImageRatio;
	char resizePercentageString[10];
	sprintf(resizePercentageString, "%d", resizePercentage);

	convertImageToSmallImageCommand = convertImageToSmallImageCommand + "convert " + "-depth 8 -resize '" + resizePercentageString + "%' " + exampleImageFileName + " " + exampleImageFileNameSmall;
	system(convertImageToSmallImageCommand.c_str());

	exampleImageFileName = exampleImageFileNameSmall;
	#endif


 	int NNtypeBeingTested = OR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE;


	for(int nn=0; nn< numberOfNeuralNetworks; nn++)
	{
		firstInputNeuronInNetwork[nn] = new NeuronContainer();

		firstOutputNeuronInNetwork[nn] = initialiseImageNeuralNetwork(NNtypeBeingTested, firstInputNeuronInNetwork[nn], &numberOfInputNeurons, numberOfOutputNeurons, exampleImageFileName);

		//method A: dumb NN use random weights/biases
		resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuronInNetwork[nn]);
			//resetInputsAndClassTargets(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, newExperience);

		//method B: dumb NN optimised to be sensitive to unique regions of image area / variations in image [NOT YET CODED]
	}

	string imageCategorisationNNMethodCheckImageMatchList = "imageCategorisationNNMethodCheckImageMatchList-868.txt";

	//now open a predetermined image match list file
	FileNameMatch * firstMatchInList =  new FileNameMatch();
	createImageFileNameMatchListFromMatchFile(imageCategorisationNNMethodCheckImageMatchList, firstMatchInList);

	FileNameMatch * currentMatchInList = firstMatchInList;
	int l = 0;
	while(currentMatchInList->next != NULL)
	{
		//for every image match, calculate the categorisationDatasetBinaryString value for each component, and verify that they are equal (exactly)

		long objectDecision = OR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION;

		long previousCategorisationValue = 0;

		for(int m=0; m<2; m++)
		{
			string imageFileName = "";
			if(m == 0)
			{
				imageFileName = currentMatchInList->fileName1;
			}
			else
			{
				imageFileName = currentMatchInList->fileName2;
			}

			#ifdef OR_DEBUG
			//cout << "\timageFileName = " << imageFileName << endl;
			#endif

			#ifdef OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES
			string imageFileNameSmall = imageFileName + ".small.ppm";

			int smallImageRatio = OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES_RATIO;
			string convertImageToSmallImageCommand = "";
			int resizePercentage = 100/smallImageRatio;
			char resizePercentageString[10];
			sprintf(resizePercentageString, "%d", resizePercentage);
			//cout << "resizePercentageString = " << resizePercentageString << endl;

			convertImageToSmallImageCommand = convertImageToSmallImageCommand + "convert " + "-depth 8 -resize '" + resizePercentageString + "%' " + imageFileName + " " + imageFileNameSmall;
			system(convertImageToSmallImageCommand.c_str());

			imageFileName = imageFileNameSmall;
			#endif

			//used for numerical representation of categorisationValue ("binary string")
			long categorisationValue = 0; 	//64bit int	[type must be of length >= OR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS]
			int index = 0;

			//used for ascii representation of categorisationValue ("binary string")
			char categorisationValueAsString[100];
			int categorisationValueAsStringIndex = 0;
			int indexChar = 0;		//used to convert binary to char
			char binaryConvertedToChar = 0x00;

			Experience * currentExperience = new Experience;


			/*
			if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE)
			{
				numberOfOutputNeurons = PROPERTIES_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
				generateExperienceFromUnitPropertiesDecision(tempUnitReference, tempUnitRefenceOpponent, objectDecision, currentExperience, currentPhase);
			}
			else */if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE)
			{
				generateDCTarrayExperienceFromImage(imageFileName, currentExperience, objectDecision);

			}
			else if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE)
			{
				generatePixelMapExperienceFromImage(imageFileName, currentExperience, objectDecision);
			}
			else
			{
				cout << "error: initialiseImageNeuralNetwork(): illegal NNtypeBeingTested" << endl;
				exit(0);
			}

			for(int nn=0; nn<OR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS; nn++)
			{
				#ifdef OR_DEBUG
				/*
				if(nn == 0)
				{
					cout << "\t\tnn == 0 experience is as follows;" << endl;
					ExperienceInput * currentExperienceInputInExperience = currentExperience->firstExperienceInput;
					for(long i = 0; i < numberOfInputNeurons; i++)
					{
						cout << "currentExperienceInputInExperience->inputValue = " << currentExperienceInputInExperience->inputValue << endl;
						currentExperienceInputInExperience = currentExperienceInputInExperience->next;
					}
				}
				*/
				#endif

				#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
				if(nn == 0)
				{
					cout << "\t\tnn == 0 experience is as follows;" << endl;
					debugPrintNNOutputs = true;
				}
				else
				{
					debugPrintNNOutputs = false;
				}
				#endif

				currentExperience->classTargetValue = OR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION;

			#ifdef COMPILE_TH_OR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET

				double experienceBackPropagationPassError;

				storeNeuralNetworkBiasAndWeights(firstInputNeuronInNetwork[nn]);

				resetInputsAndClassTargets(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

				experienceBackPropagationPassError = ANNBackPropogationPass(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn]);

				double outputValueArray[OR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS];
				NeuronContainer * currentOutputNeuron = firstOutputNeuronInNetwork[nn];
				for(int o=0; o<OR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS; o++)
				{
					outputValueArray[o] = currentOutputNeuron->neuron->output;
					currentOutputNeuron = currentOutputNeuron->nextNeuronContainer;
				}

				restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuronInNetwork[nn]);

				bool bitArray[COMPILE_TH_OR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_NUM_BITS];
				int bitArrayIndex = 0;
				for(int q=0; q<COMPILE_TH_OR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_OUTPUTS-1; q++)
				{
					for(int r=q+1; r<COMPILE_TH_OR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_OUTPUTS; r++)
					{
						if(outputValueArray[q] < outputValueArray[r])
						{
							bitArray[bitArrayIndex] = true;
						}
						else
						{
							bitArray[bitArrayIndex] = false;

						}
						bitArrayIndex++;
					}
				}
				#ifdef OR_DEBUG
				//cout << "bitArrayIndex = " << bitArrayIndex << endl;
				#endif

				for(int i=0; i<COMPILE_TH_OR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_NUM_BITS; i++)
				{
					bool bitValue = bitArray[i];
					if(indexChar == 8)
					{
						//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
						indexChar = 0;
						categorisationValueAsString[categorisationValueAsStringIndex] = binaryConvertedToChar;
						categorisationValueAsStringIndex++;
						binaryConvertedToChar = 0x00;
					}

					if(bitValue)
					{
						binaryConvertedToChar = binaryConvertedToChar | (0x01 << indexChar);
						categorisationValue = categorisationValue | (0x01 << index);
					}
					else
					{
						binaryConvertedToChar = binaryConvertedToChar | (0x00 << indexChar);
						categorisationValue = categorisationValue | (0x00 << index);
					}
					index++;
					indexChar++;
				}

			#else

							// orig version;
							double experienceBackPropagationPassError = calculateExperienceErrorForHypotheticalDecision(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

							#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
							if(nn == 0)
							{
								debugPrintNNOutputs = false;
							}
							#endif
							#ifdef OR_DEBUG
							//cout << "experienceBackPropagationPassError = " << experienceBackPropagationPassError << endl;
							#endif
							currentExperience->classTargetValue = OR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION_ALTERNATE;


							double experienceBackPropagationPassErrorAlternateDecision = calculateExperienceErrorForHypotheticalDecision(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

							#ifdef OR_DEBUG
							//cout << "experienceBackPropagationPassErrorAlternateDecision = " << experienceBackPropagationPassErrorAlternateDecision << endl;
							/*
							if(nn == 0)
							{
								cout << "\t\tnn == 0 errors are as follows;" << endl;
								ExperienceInput * currentExperienceInputInExperience = currentExperience->firstExperienceInput;
								cout << "experienceBackPropagationPassError1 = " << experienceBackPropagationPassError << endl;
								cout << "experienceBackPropagationPassError2 = " << experienceBackPropagationPassErrorAlternateDecision << endl;
							}
							*/
							#endif

							bool bitValue;
							if(experienceBackPropagationPassError < experienceBackPropagationPassErrorAlternateDecision)
							{
								//cout << "bit = 1, nn = " << nn << endl;
								bitValue = 1;
							}
							else
							{
								//cout << "bit = 0, nn = " << nn << endl;
								bitValue = 0;
							}

							if(indexChar == 8)
							{
								//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
								indexChar = 0;
								categorisationValueAsString[categorisationValueAsStringIndex] = binaryConvertedToChar;
								categorisationValueAsStringIndex++;
								binaryConvertedToChar = 0x00;
							}

							if(bitValue)
							{
								binaryConvertedToChar = binaryConvertedToChar | (0x01 << indexChar);
								categorisationValue = categorisationValue | (0x01 << index);
							}
							else
							{
								binaryConvertedToChar = binaryConvertedToChar | (0x00 << indexChar);
								categorisationValue = categorisationValue | (0x00 << index);
							}
							index++;
							indexChar++;

			#endif


			}

			#ifdef OR_DEBUG
			//cout << "categorisationValue = " << categorisationValue << endl;
			#endif

			if(m == 0)
			{
				previousCategorisationValue = categorisationValue;
			}
			else
			{
				cout << "categorisationValue image 1 = " << previousCategorisationValue << endl;
				cout << "categorisationValue image 2 = " << categorisationValue << endl;

				if(categorisationValue == previousCategorisationValue)
				{
					cout << "\t SUCCESS: categorisationValue == previousCategorisationValue" << endl;
				}
				if(categorisationValue != previousCategorisationValue)
				{
					cout << "\t FAIL: categorisationValue != previousCategorisationValue" << endl;
				}

			}



		}

		currentMatchInList = currentMatchInList->next;
		l++;
	}

	return result;
}




void deriveDCTcoefficients(string exampleImageFileName, signed char dctCoeff[])
{
	signed char * dctCoeffArrayY = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * dctCoeffArrayYcr = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * dctCoeffArrayYcb = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * dctCoeffArrayYDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * dctCoeffArrayYCrDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	signed char * dctCoeffArrayYCbDummy = new signed char[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];

	string rgbMapFacingPolySmallFileNameJPEG = "temp.jpg";

	readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(exampleImageFileName, rgbMapFacingPolySmallFileNameJPEG, dctCoeff, false);
}


NeuronContainer * initialiseImageNeuralNetwork(int NNtypeBeingTested, NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long numberOfOutputNeurons, string exampleImageFileName)
{
	NeuronContainer * firstOutputNeuronInNetwork;

	long numberOfLayers;

	long tempObjectDecision = IRRELEVANT;
	Experience * tempExperience = new Experience;

	/*
	if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE)
	{
		numberOfOutputNeurons = PROPERTIES_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
		generateExperienceFromUnitPropertiesDecision(tempUnitReference, tempUnitRefenceOpponent, tempObjectDecision, tempExperience, currentPhase);
	}
	else */if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE)
	{
		generateDCTarrayExperienceFromImage(exampleImageFileName, tempExperience, tempObjectDecision);

	}
	else if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE)
	{
		generatePixelMapExperienceFromImage(exampleImageFileName, tempExperience, tempObjectDecision);
	}
	else
	{
		cout << "error: initialiseImageNeuralNetwork(): illegal NNtypeBeingTested" << endl;
		exit(0);
	}

	*numberOfInputNeurons = countNumberOfExperienceInputs(tempExperience);

	cout << "\n\n numberOfInputNeurons calculated = " << *numberOfInputNeurons << endl;

	delete tempExperience;
		//arbitrary NN number of layers heuristic - need to optimise this


	/*if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE)
	{
		numberOfLayers = GAME_NUMBER_OF_NN_LAYERS;
		firstOutputNeuronInNetwork = formNeuralNetWithOptimisedProperties(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers);
	}
	else */if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE)
	{
		numberOfLayers = OR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_DCT_EXPERIENCE;
		int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
		double meanLayerDivergenceFactor = 1.5;
		double probabilityNeuronConnectionWithPreviousLayerNeuron = 1.0;
		double probabilityNeuronConnectionWithAllPreviousLayersNeurons = 0.5;
		firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, *numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);
	}
	else if(NNtypeBeingTested == OR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE)
	{
		numberOfLayers = OR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_PIXMAP_EXPERIENCE;
		int layerDivergenceType = OR_IMAGE_COMPARISON_NN_DIVERGENCE_TYPE; 	//LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D	//DEFAULT = LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D;
		double meanLayerDivergenceFactor = OR_IMAGE_COMPARISON_NN_DIVERGENCE_FACTOR_PIXMAP_EXPERIENCE;
		double probabilityNeuronConnectionWithPreviousLayerNeuron = OR_IMAGE_COMPARISON_NN_PROBABILITY_CONNECTION_WITH_PREVIOUS_LAYER_PIXMAP_EXPERIENCE;
		double probabilityNeuronConnectionWithAllPreviousLayersNeurons = OR_IMAGE_COMPARISON_NN_PROBABILITY_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_PIXMAP_EXPERIENCE;		//default 0.0
		firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, *numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);
	}
	else
	{

	}

	cout << "Neural Network Properties:" << endl;
	cout << "numberOfInputNeurons = " << *numberOfInputNeurons << endl;
	cout << "numberOfOutputNeurons = " << numberOfOutputNeurons << endl;
	cout << "numberOfLayers = " << numberOfLayers << endl;

	return firstOutputNeuronInNetwork;
}


void generateExperienceFromOneArray(double * array, int arrayLength, double maxInputValueForNormalisation, Experience * currentExperience, int objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	for(int i=0; i<arrayLength; i++)
	{
		currentExperienceInput->inputValue = normaliseExperienceInput(array[i], maxInputValueForNormalisation);

		ExperienceInput * newExperienceInput = new ExperienceInput();

		currentExperienceInput->next = newExperienceInput;

		currentExperienceInput = currentExperienceInput->next;
	}
}



bool generateDCTarrayExperienceFromImage(string imageFileName, Experience * currentExperience, int objectDecision)
{
	//now derive DCT coefficients
	signed char dctCoeff[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	double dctCoeffDouble[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	deriveDCTcoefficients(imageFileName, dctCoeff);

	double maxValue = 0.0;
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX; i++)
	{
		dctCoeffDouble[i] = (double)dctCoeff[i] + 128;
		if(dctCoeffDouble[i] > maxValue)
		{
			maxValue = dctCoeffDouble[i];
		}
	}
	double maxInputValueForNormalisation = maxValue;
	generateExperienceFromOneArray(dctCoeffDouble, OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX, maxInputValueForNormalisation, currentExperience, objectDecision);

}

//generates an experience from an image
bool generatePixelMapExperienceFromImage(string imageFileName, Experience * currentExperience, int objectDecision)
{
	bool result = true;

	//1. generate random camera + lighting position conditions;
		//CHECK THIS - need to randomly vary lighting conditions

	char * charstarrayImageFileName = const_cast<char*>(imageFileName.c_str()); //NOT USED for 2D!

	//3. load pixmap into RAM objectImage from generated pixmap
	pixmap * objectImage;
	#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_3
	cout << "image file being loaded = " << charstarrayImageFileName << endl;
	#endif
	objectImage = loadPPM(charstarrayImageFileName);

	//4. produce contrast map from pixmap image
	unsigned char * rgbMap = new unsigned char [objectImage->wide * objectImage->high * RGB_NUM];
	double * luminosityMap = new double[objectImage->wide * objectImage->high];
	bool * luminosityBooleanMap = new bool[objectImage->wide * objectImage->high];
	double * luminosityContrastMap = new double[objectImage->wide * objectImage->high];
	bool * luminosityContrastBooleanMap = new bool[objectImage->wide * objectImage->high];

	createRGBMapFromPixmapImage(objectImage, rgbMap);

	createLuminosityMapFromRGBMap(objectImage->wide, objectImage->high, rgbMap, luminosityMap);
		//createLuminosityMapFromPixmapImage(objectImage, luminosityMap);
	createLuminosityBooleanMap(objectImage->wide, objectImage->high, luminosityMap, luminosityBooleanMap);
	createContrastMapFromMap(objectImage->wide, objectImage->high, luminosityMap, luminosityContrastMap);
	createLuminosityContrastBooleanMap(objectImage->wide, objectImage->high, luminosityContrastMap, luminosityContrastBooleanMap);

	//5. apply quadratic fit to image [from Java code];
		//CHECK THIS - accuracy of quadratic fit is not currently required

	//fill experience input tree

	#ifdef OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_RGB_MAP
	generateExperienceWith2DRGBMap(rgbMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_LUMINOSITY_BOOLEAN_MAP
	generateExperienceWith2DBooleanMap(luminosityBooleanMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_LUMINOSITY_CONTRAST_BOOLEAN_MAP
	generateExperienceWith2DBooleanMap(luminosityContrastBooleanMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_LUMINOSITY_MAP
	generateExperienceWith2DMap(luminosityMap, objectImage->wide, objectImage->high, LUMINOSITY_MAP_MAX_LUMINOSITY_VALUE, currentExperience, objectDecision);
	#elif defined OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_LUMINOSITY_CONTRAST_MAP
	generateExperienceWith2DMap(luminosityContrastMap, objectImage->wide, objectImage->high, CONTRAST_MAP_MAX_CONTRAST_VALUE, currentExperience, objectDecision);
	#else
	cout << "Error: no feed defined" << endl;
	exit(0);
	#endif

	//#define DEBUG_ANN_OBJECT_RECOGNITION_EXPERIENCES_OUTPUT_ALL_MAPS
	#ifdef DEBUG_ANN_OBJECT_RECOGNITION_EXPERIENCES_OUTPUT_ALL_MAPS
	string imageFileNameStart = "";
	imageFileNameStart = charstarrayImageFileName;
	string PPMFileNameLuminosity = (imageFileNameStart) + LUMINOSITY_MAP_PPM_EXTENSION;
	char * PPMFileNameLuminosityCharArray = const_cast<char*>(PPMFileNameLuminosity.c_str());
	cout << "PPMFileNameLuminosityCharArray = " << PPMFileNameLuminosityCharArray << endl;
	generatePixmapFromLuminosityMap(PPMFileNameLuminosityCharArray, objectImage->wide, objectImage->high, luminosityMap);

	string PPMFileNameContrast = (imageFileNameStart) + LUMINOSITY_CONTRAST_MAP_PPM_EXTENSION;
	char * PPMFileNameContrastCharArray = const_cast<char*>(PPMFileNameContrast.c_str());
	cout << "PPMFileNameContrastCharArray = " << PPMFileNameContrastCharArray << endl;
	generatePixmapFromLuminosityContrastMap(PPMFileNameContrastCharArray, objectImage->wide, objectImage->high, luminosityContrastMap);

	string PPMFileNameLuminosityBoolean = (imageFileNameStart) + LUMINOSITY_BOOLEAN_MAP_PPM_EXTENSION;
	char * PPMFileNameLuminosityBooleanCharArray = const_cast<char*>(PPMFileNameLuminosityBoolean.c_str());
	cout << "PPMFileNameLuminosityBooleanCharArray = " << PPMFileNameLuminosityBooleanCharArray << endl;
	generatePixmapFromBooleanMap(PPMFileNameLuminosityBooleanCharArray, objectImage->wide, objectImage->high, luminosityBooleanMap);

	string PPMFileNameContrastBoolean = (imageFileNameStart) + LUMINOSITY_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION;
	char * PPMFileNameContrastBooleanCharArray = const_cast<char*>(PPMFileNameContrastBoolean.c_str());
	cout << "PPMFileNameContrastBooleanCharArray = " << PPMFileNameContrastBooleanCharArray << endl;
	generatePixmapFromBooleanMap(PPMFileNameContrastBooleanCharArray, objectImage->wide, objectImage->high, luminosityContrastBooleanMap);
	#endif

	//memory clean up

	delete luminosityContrastMap;
	delete luminosityMap;

	/*DOESNT WORK FOR SOME REASON;
	delete objectImage;
	delete objectImageContrast;
	delete objectImageLuminosity;
	//freePixmap(objectImage);
	//freePixmap(objectImageContrast);
	//freePixmap(objectImageLuminosity);
	*/
	freePixmap(objectImage);

	return result;
}





void createImageFileNameMatchListFromMatchFile(string fileName, FileNameMatch * firstMatchInList)
{
	FileNameMatch * currentMatchInList = firstMatchInList;

	char * fileNamecharstar = const_cast<char*>(fileName.c_str());
	ifstream * parseFileObject = new ifstream(fileNamecharstar);

	if(parseFileObject->rdbuf( )->is_open( ))
	{
		char c;
		int charCount = 0;
		int lineCount = 0;
		bool waitingForNewLine = false;

		bool readingObjectName1 = true;
		bool readingObjectName2 = false;

		char objectName1String[100] = "";
		char objectName2String[100] = "";

		while (parseFileObject->get(c))
		{
			charCount++;

			if((waitingForNewLine) && (c == CHAR_NEWLINE))
			{
				lineCount++;
				waitingForNewLine = false;
				readingObjectName1 = true;
			}
			else if(waitingForNewLine)
			{
				//do nothing, still waiting for new line
			}
			else if((readingObjectName1) && (c == CHAR_SPACE))
			{
				readingObjectName1 = false;
				readingObjectName2 = true;
			}
			else if(readingObjectName1)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(objectName1String, typeString);
			}
			else if((readingObjectName2) && (c == CHAR_SPACE))
			{
				//ignore preceeding spaces
			}
			else if((readingObjectName2) && (c == CHAR_NEWLINE))
			{
				currentMatchInList->fileName1 = objectName1String;
				currentMatchInList->fileName2 = objectName2String;

				#ifdef OR_DEBUG
				//cout << "objectName1String = " << objectName1String << endl;
				//cout << "objectName2String = " << objectName2String << endl;
				#endif

				FileNameMatch * newMatch = new FileNameMatch();
				currentMatchInList->next = newMatch;

				currentMatchInList=currentMatchInList->next;

				objectName1String[0] = '\0';
				objectName2String[0] = '\0';

				readingObjectName2 = false;
				readingObjectName1 = true;

			}
			else if(readingObjectName2)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(objectName2String, typeString);
			}
		}

		parseFileObject->close();
		delete parseFileObject;
	}
	else
	{
		cout << "error opening file, " << fileName << endl;
	}
}











