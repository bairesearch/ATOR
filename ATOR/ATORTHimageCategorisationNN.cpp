 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORTHimageCategorisationNN.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * Description: Test Harness for OR Image Categorisation NN method (not yet implemented)
 * /
 *******************************************************************************/


/*Description (OLD);

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


#include "ATORTHimageCategorisationNN.hpp"

#include "ATORglobalDefs.hpp"
#include "XMLrulesClass.hpp"

#include "ATORrules.hpp"
#include "ATORimagecomparison.hpp"
#include "ATORcomparison.hpp"

#include "RTppm.hpp"
#include "RTpixelMaps.hpp"

#include "LDjpeg.hpp"

#include "ANNneuronClass.hpp"
#include "ANNFormation.hpp"
#include "ANNTraining.hpp"
#include "ANNXMLconversion.hpp"
#include "ANNsprite.hpp"
#include "ANNUpdateAlgorithm.hpp"
#include "ANNdisplay.hpp"

ATORfileNameMatch::ATORfileNameMatch(void)
{
	fileName1 = "";
	fileName2 = "";

	next = NULL;

}

ATORfileNameMatch::~ATORfileNameMatch()
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


	if(!parseATORrulesXMLfile())
	{
		cerr << "error: no rules file detected" << endl;
		exit(EXIT_ERROR);
	}
	fillInATORrulesExternVariables();

	#define IRRELEVANT (0)

	//************************
	//PART 1: Standard Initialisations; create neural network 1 (for unit properties and unit combat decisions)
	//************************

	int numberOfNeuralNetworks = ATOR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS;
	int categorisationDatasetBinaryStringUniqueIdentifierNumberOfBits = numberOfNeuralNetworks;		// > 1 trillion categories


	ANNneuronContainer* firstInputNeuronInNetwork[numberOfNeuralNetworks];
	ANNneuronContainer* firstOutputNeuronInNetwork[numberOfNeuralNetworks];
	int64_t numberOfInputNeurons = 0;	//dynamically generated based upon experience creation
	int64_t numberOfOutputNeurons = ATOR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS;

	/*
	ATOR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE	//not yet coded [will have to emulate close geo values, or extract them from ATORcomparison.cpp into results.html]
	ATOR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE
	ATOR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE
	*/

	string exampleImageFileName = "315interpolatedMeshViewIndex0ZoomIndex0FacingPoly214side0.rgb.train.ppm";

	#ifdef ATOR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES
	string exampleImageFileNameSmall = exampleImageFileName + ".small.ppm";

	int smallImageRatio = ATOR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES_RATIO;
	string convertImageToSmallImageCommand = "";
	int resizePercentage = 100/smallImageRatio;
	char resizePercentageString[10];
	resizePercentageString = convertIntToString(resizePercentage);

	convertImageToSmallImageCommand = convertImageToSmallImageCommand + "convert " + "-depth 8 -resize '" + resizePercentageString + "%' " + exampleImageFileName + " " + exampleImageFileNameSmall;
	system(convertImageToSmallImageCommand.c_str());

	exampleImageFileName = exampleImageFileNameSmall;
	#endif


 	int NNtypeBeingTested = ATOR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE;


	for(int nn=0; nn< numberOfNeuralNetworks; nn++)
	{
		firstInputNeuronInNetwork[nn] = new ANNneuronContainer();

		firstOutputNeuronInNetwork[nn] = initialiseImageNeuralNetwork(NNtypeBeingTested, firstInputNeuronInNetwork[nn], &numberOfInputNeurons, numberOfOutputNeurons, exampleImageFileName);

		//method A: dumb NN use random weights/biases
		resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuronInNetwork[nn]);
			//resetInputsAndClassTargets(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, newExperience);

		//method B: dumb NN optimised to be sensitive to unique regions of image area / variations in image [NOT YET CODED]
	}

	string imageCategorisationNNMethodCheckImageMatchList = "imageCategorisationNNMethodCheckImageMatchList-868.txt";

	//now open a predetermined image match list file
	ATORfileNameMatch* firstMatchInList =  new ATORfileNameMatch();
	createImageFileNameMatchListFromMatchFile(imageCategorisationNNMethodCheckImageMatchList, firstMatchInList);

	ATORfileNameMatch* currentMatchInList = firstMatchInList;
	int l = 0;
	while(currentMatchInList->next != NULL)
	{
		//for every image match, calculate the categorisationDatasetBinaryString value for each component, and verify that they are equal (exactly)

		int64_t objectDecision = ATOR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION;

		int64_t previousCategorisationValue = 0;

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


			#ifdef ATOR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES
			string imageFileNameSmall = imageFileName + ".small.ppm";

			int smallImageRatio = ATOR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES_RATIO;
			string convertImageToSmallImageCommand = "";
			int resizePercentage = 100/smallImageRatio;
			char resizePercentageString[10];
			resizePercentageString = convertIntToString(resizePercentage);

			convertImageToSmallImageCommand = convertImageToSmallImageCommand + "convert " + "-depth 8 -resize '" + resizePercentageString + "%' " + imageFileName + " " + imageFileNameSmall;
			system(convertImageToSmallImageCommand.c_str());

			imageFileName = imageFileNameSmall;
			#endif

			//used for numerical representation of categorisationValue ("binary string")
			int64_t categorisationValue = 0; 	//64bit int	[type must be of length >= ATOR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS]
			int index = 0;

			//used for ascii representation of categorisationValue ("binary string")
			char categorisationValueAsString[100];
			int categorisationValueAsStringIndex = 0;
			int indexChar = 0;		//used to convert binary to char
			char binaryConvertedToChar = 0x00;

			ANNexperience* currentExperience = new ANNexperience;


			/*
			if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE)
			{
				numberOfOutputNeurons = PROPERTIES_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
				generateExperienceFromUnitPropertiesDecision(tempUnitReference, tempUnitRefenceOpponent, objectDecision, currentExperience, currentPhase);
			}
			else*/if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE)
			{
				generateDCTarrayExperienceFromImage(imageFileName, currentExperience, objectDecision);

			}
			else if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE)
			{
				generatePixelMapExperienceFromImage(imageFileName, currentExperience, objectDecision);
			}
			else
			{
				cerr << "error: initialiseImageNeuralNetwork{}: illegal NNtypeBeingTested" << endl;
				exit(EXIT_ERROR);
			}

			for(int nn=0; nn<ATOR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS; nn++)
			{


				currentExperience->classTargetValue = ATOR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION;

			#ifdef COMPILE_TH_ATOR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET

				double experienceBackPropagationPassError;

				storeNeuralNetworkBiasAndWeights(firstInputNeuronInNetwork[nn]);

				resetInputsAndClassTargets(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

				experienceBackPropagationPassError = ANNBackPropogationPass(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn]);

				double outputValueArray[ATOR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS];
				ANNneuronContainer* currentOutputNeuron = firstOutputNeuronInNetwork[nn];
				for(int o=0; o<ATOR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS; o++)
				{
					outputValueArray[o] = currentOutputNeuron->neuron->output;
					currentOutputNeuron = currentOutputNeuron->nextNeuronContainer;
				}

				restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuronInNetwork[nn]);

				bool bitArray[COMPILE_TH_ATOR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_NUM_BITS];
				int bitArrayIndex = 0;
				for(int q=0; q<COMPILE_TH_ATOR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_OUTPUTS-1; q++)
				{
					for(int r=q+1; r<COMPILE_TH_ATOR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_OUTPUTS; r++)
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

				for(int i=0; i<COMPILE_TH_ATOR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_NUM_BITS; i++)
				{
					bool bitValue = bitArray[i];
					if(indexChar == 8)
					{
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

							currentExperience->classTargetValue = ATOR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION_ALTERNATE;


							double experienceBackPropagationPassErrorAlternateDecision = calculateExperienceErrorForHypotheticalDecision(firstInputNeuronInNetwork[nn], firstOutputNeuronInNetwork[nn], numberOfInputNeurons, numberOfOutputNeurons, currentExperience);


							bool bitValue;
							if(experienceBackPropagationPassError < experienceBackPropagationPassErrorAlternateDecision)
							{
								bitValue = 1;
							}
							else
							{
								bitValue = 0;
							}

							if(indexChar == 8)
							{
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




void deriveDCTcoefficients(string exampleImageFileName, schar dctCoeff[])
{
	schar* dctCoeffArrayY = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* dctCoeffArrayYcr = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* dctCoeffArrayYcb = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* dctCoeffArrayYDummy = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* dctCoeffArrayYCrDummy = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];
	schar* dctCoeffArrayYCbDummy = new schar[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_MAX_NUM_DCT_COEFFICIENTS_1D];

	string rgbMapFacingPolySmallFileNameJPEG = "temp.jpg";

	readDCTcoeffIndividualArraysAndConvertToConcatonatedSignedDCTcoeffArray(exampleImageFileName, rgbMapFacingPolySmallFileNameJPEG, dctCoeff, false);
}


ANNneuronContainer* initialiseImageNeuralNetwork(int NNtypeBeingTested, ANNneuronContainer* firstInputNeuronInNetwork, int64_t* numberOfInputNeurons, int64_t numberOfOutputNeurons, string exampleImageFileName)
{
	ANNneuronContainer* firstOutputNeuronInNetwork;

	int64_t numberOfLayers;

	int64_t tempObjectDecision = IRRELEVANT;
	ANNexperience* tempExperience = new ANNexperience;

	/*
	if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE)
	{
		numberOfOutputNeurons = PROPERTIES_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
		generateExperienceFromUnitPropertiesDecision(tempUnitReference, tempUnitRefenceOpponent, tempObjectDecision, tempExperience, currentPhase);
	}
	else*/if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE)
	{
		generateDCTarrayExperienceFromImage(exampleImageFileName, tempExperience, tempObjectDecision);

	}
	else if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE)
	{
		generatePixelMapExperienceFromImage(exampleImageFileName, tempExperience, tempObjectDecision);
	}
	else
	{
		cerr << "error: initialiseImageNeuralNetwork{}: illegal NNtypeBeingTested" << endl;
		exit(EXIT_ERROR);
	}

	*numberOfInputNeurons = countNumberOfExperienceInputs(tempExperience);

	cout << "\n\n numberOfInputNeurons calculated = " <<* numberOfInputNeurons << endl;

	delete tempExperience;
		//arbitrary NN number of layers heuristic - need to optimise this


	/*if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE)
	{
		numberOfLayers = GAME_NUMBER_OF_NN_LAYERS;
		firstOutputNeuronInNetwork = formNeuralNetWithOptimisedProperties(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers);
	}
	else*/if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE)
	{
		numberOfLayers = ATOR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_DCT_EXPERIENCE;
		int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
		double meanLayerDivergenceFactor = 1.5;
		double probabilityANNneuronConnectionWithPreviousLayerNeuron = 1.0;
		double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0.5;
		firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork,* numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
	}
	else if(NNtypeBeingTested == ATOR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE)
	{
		numberOfLayers = ATOR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_PIXMAP_EXPERIENCE;
		int layerDivergenceType = ATOR_IMAGE_COMPARISON_NN_DIVERGENCE_TYPE; 	//LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D	//DEFAULT = LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D;
		double meanLayerDivergenceFactor = ATOR_IMAGE_COMPARISON_NN_DIVERGENCE_FACTOR_PIXMAP_EXPERIENCE;
		double probabilityANNneuronConnectionWithPreviousLayerNeuron = ATOR_IMAGE_COMPARISON_NN_PROBABILITY_CONNECTION_WITH_PREVIOUS_LAYER_PIXMAP_EXPERIENCE;
		double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = ATOR_IMAGE_COMPARISON_NN_PROBABILITY_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_PIXMAP_EXPERIENCE;		//default 0.0
		firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork,* numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
	}
	else
	{

	}

	cout << "Neural Network Properties:" << endl;
	cout << "numberOfInputNeurons = " <<* numberOfInputNeurons << endl;
	cout << "numberOfOutputNeurons = " << numberOfOutputNeurons << endl;
	cout << "numberOfLayers = " << numberOfLayers << endl;

	return firstOutputNeuronInNetwork;
}


void generateExperienceFromOneArray(double* array, int arrayLength, double maxInputValueForNormalisation, ANNexperience* currentExperience, int objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	for(int i=0; i<arrayLength; i++)
	{
		currentExperienceInput->inputValue = normaliseExperienceInput(array[i], maxInputValueForNormalisation);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();

		currentExperienceInput->next = newExperienceInput;

		currentExperienceInput = currentExperienceInput->next;
	}
}



bool generateDCTarrayExperienceFromImage(string imageFileName, ANNexperience* currentExperience, int objectDecision)
{
	//now derive DCT coefficients
	schar dctCoeff[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	double dctCoeffDouble[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	deriveDCTcoefficients(imageFileName, dctCoeff);

	double maxValue = 0.0;
	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX; i++)
	{
		dctCoeffDouble[i] = (double)dctCoeff[i] + 128;
		if(dctCoeffDouble[i] > maxValue)
		{
			maxValue = dctCoeffDouble[i];
		}
	}
	double maxInputValueForNormalisation = maxValue;
	generateExperienceFromOneArray(dctCoeffDouble, ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX, maxInputValueForNormalisation, currentExperience, objectDecision);

}

//generates an experience from an image
bool generatePixelMapExperienceFromImage(string imageFileName, ANNexperience* currentExperience, int objectDecision)
{
	bool result = true;

	//1. generate random camera + lighting position conditions;
		//CHECK THIS - need to randomly vary lighting conditions

	//3. load pixmap into RAM objectImage from generated pixmap
	pixmap* objectImage;
	objectImage = loadPPM(imageFileName);

	//4. produce contrast map from pixmap image
	uchar* rgbMap = new uchar [objectImage->wide* objectImage->high* RGB_NUM];
	double* luminosityMap = new double[objectImage->wide* objectImage->high];
	bool* luminosityBooleanMap = new bool[objectImage->wide* objectImage->high];
	double* luminosityContrastMap = new double[objectImage->wide* objectImage->high];
	bool* luminosityContrastBooleanMap = new bool[objectImage->wide* objectImage->high];

	createRGBMapFromPixmapImage(objectImage, rgbMap);

	createLuminosityMapFromRGBMap(objectImage->wide, objectImage->high, rgbMap, luminosityMap);
		//createLuminosityMapFromPixmapImage(objectImage, luminosityMap);
	createLuminosityBooleanMap(objectImage->wide, objectImage->high, luminosityMap, luminosityBooleanMap);
	createContrastMapFromMap(objectImage->wide, objectImage->high, luminosityMap, luminosityContrastMap);
	createLuminosityContrastBooleanMap(objectImage->wide, objectImage->high, luminosityContrastMap, luminosityContrastBooleanMap);

	//5. apply quadratic fit to image [from Java code];
		//CHECK THIS - accuracy of quadratic fit is not currently required

	//fill experience input tree

	#ifdef ATOR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_RGB_MAP
	generateExperienceWith2DrgbMap(rgbMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined ATOR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_LUMINOSITY_BOOLEAN_MAP
	generateExperienceWith2DbooleanMap(luminosityBooleanMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined ATOR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_LUMINOSITY_CONTRAST_BOOLEAN_MAP
	generateExperienceWith2DbooleanMap(luminosityContrastBooleanMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined ATOR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_LUMINOSITY_MAP
	generateExperienceWith2Dmap(luminosityMap, objectImage->wide, objectImage->high, LUMINOSITY_MAP_MAX_LUMINOSITY_VALUE, currentExperience, objectDecision);
	#elif defined ATOR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_LUMINOSITY_CONTRAST_MAP
	generateExperienceWith2Dmap(luminosityContrastMap, objectImage->wide, objectImage->high, CONTRAST_MAP_MAX_CONTRAST_VALUE, currentExperience, objectDecision);
	#else
	cerr << "Error: no feed defined" << endl;
	exit(EXIT_ERROR);
	#endif

	//#define DEBUG_ANN_OBJECT_RECOGNITION_EXPERIENCES_OUTPUT_ALL_MAPS

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





void createImageFileNameMatchListFromMatchFile(string fileName, ATORfileNameMatch* firstMatchInList)
{
	ATORfileNameMatch* currentMatchInList = firstMatchInList;

	ifstream parseFileObject(fileNamecharstar.c_str());

	if(parseFileObject.rdbuf()->is_open())
	{
		char c;
		int charCount = 0;
		int lineCount = 0;
		bool waitingForNewLine = false;

		bool readingObjectName1 = true;
		bool readingObjectName2 = false;

		string objectName1String = "";
		string objectName2String = "";

		while(parseFileObject.get(c))
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
				objectName1String = objectName1String + c;
			}
			else if((readingObjectName2) && (c == CHAR_SPACE))
			{
				//ignore preceeding spaces
			}
			else if((readingObjectName2) && (c == CHAR_NEWLINE))
			{
				currentMatchInList->fileName1 = objectName1String;
				currentMatchInList->fileName2 = objectName2String;


				ATORfileNameMatch* newMatch = new ATORfileNameMatch();
				currentMatchInList->next = newMatch;

				currentMatchInList=currentMatchInList->next;

				objectName1String = "";
				objectName2String = "";

				readingObjectName2 = false;
				readingObjectName1 = true;

			}
			else if(readingObjectName2)
			{
				objectName2String = objectName2String + c;
			}
		}

		parseFileObject.close();
	}
	else
	{
		cout << "error opening file, " << fileName << endl;
	}
}











