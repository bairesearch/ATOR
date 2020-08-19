/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORTHimageCategorisationNN.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3n7e 17-August-2020
 * Description: Test Harness for OR Image Categorisation NN method (not yet implemented)
 * /
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


#ifndef HEADER_OR_TH_IMAGE_CATEGORISATION_NN
#define HEADER_OR_TH_IMAGE_CATEGORISATION_NN

#include "ANNexperienceClass.hpp"
#include "ANNneuronClass.hpp"

//#define OR_IMAGE_CATEGORISTION_NN_USE_SMALL_NUMBER_LAYERS

#ifdef OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES
	#define OR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_PIXMAP_EXPERIENCE (3)
	#define OR_IMAGE_COMPARISON_NN_DIVERGENCE_FACTOR_PIXMAP_EXPERIENCE (0.6)
#else
	#ifdef OR_IMAGE_CATEGORISTION_NN_USE_SMALL_NUMBER_LAYERS
		#define OR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_PIXMAP_EXPERIENCE (2)	//5	//7
		#define OR_IMAGE_COMPARISON_NN_DIVERGENCE_FACTOR_PIXMAP_EXPERIENCE (0.3)
	#else
		#define OR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_PIXMAP_EXPERIENCE (5)	//5	//7
		#define OR_IMAGE_COMPARISON_NN_DIVERGENCE_FACTOR_PIXMAP_EXPERIENCE (0.8)
	#endif
#endif

#define OR_IMAGE_COMPARISON_NN_NUMBER_OF_NN_LAYERS_DCT_EXPERIENCE (7)

			//0.8	//0.85; <0 = converging
#define OR_IMAGE_COMPARISON_NN_PROBABILITY_CONNECTION_WITH_PREVIOUS_LAYER_PIXMAP_EXPERIENCE (1.0)	//1.0;
#define OR_IMAGE_COMPARISON_NN_PROBABILITY_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_PIXMAP_EXPERIENCE (0.0)	//0.0;
#ifdef OR_IMAGE_CATEGORISTION_NN_USE_SMALL_NUMBER_LAYERS
	#define OR_IMAGE_COMPARISON_NN_DIVERGENCE_TYPE (LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D)
#else
	#define OR_IMAGE_COMPARISON_NN_DIVERGENCE_TYPE (LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS)
#endif

//#define OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_RGB_MAP
#define OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_LUMINOSITY_MAP
//#define OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_FEED_LUMINOSITY_BOOLEAN_MAP
//#define OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_LUMINOSITY_CONTRAST_MAP
//#define OR_IMAGE_COMPARISON_NN_PIXMAP_EXPERIENCE_LUMINOSITY_CONTRAST_BOOLEAN_MAP	//DEFAULT

#define OR_IMAGE_CATEGORISTION_NN_USE_GEO_EXPERIENCE (1)	//not yet coded [will have to emulate close geo values, or extract them from ORcomparison.cpp into results.html]
#define OR_IMAGE_CATEGORISTION_NN_USE_DCT_EXPERIENCE (2)
#define OR_IMAGE_CATEGORISTION_NN_USE_PIXMAP_EXPERIENCE (3)

#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN
	#define OR_IMAGE_CATEGORISTION_NN_NUMBER_NEURAL_NETWORKS (64)
#endif

#define OR_IMAGE_CATEGORISTION_NN_NUMBER_OUTPUT_NEURONS (COMPILE_TH_OR_IMAGE_CATEGORISTION_NN_USE_MULTI_BIT_OUTPUT_PER_NET_OUTPUTS)
#define OR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION (1)
#define OR_IMAGE_CATEGORISTION_NN_OBJECT_DECISION_ALTERNATE (0)


#define OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES_RATIO (4)	//28/4 = 7



class ORfileNameMatch{
private:
	/*There are currently no private attributes of this class*/
public:

	ORfileNameMatch(void); // constructor declaration
	~ORfileNameMatch();	//  and destructor.

	string fileName1;
	string fileName2;

	ORfileNameMatch* next;
};


int main();
void deriveDCTcoefficients(string exampleImageFileName, signed char dctCoeff[]);
ANNneuronContainer* initialiseImageNeuralNetwork(int NNtypeBeingTested, ANNneuronContainer* firstInputNeuronInNetwork, int64_t* numberOfInputNeurons, int64_t numberOfOutputNeurons, string exampleImageFileName);
bool generateDCTarrayExperienceFromImage(string imageFileName, ANNexperience* currentExperience, int objectDecision);
bool generatePixelMapExperienceFromImage(string imageFileName, ANNexperience* currentExperience, int objectDecision);
void generateExperienceFromOneArray(double* array, int arrayLength, double maxInputValueForNormalisation, ANNexperience* tempExperience, int objectDecision);
int ORTHimageCategorisationNN();
void createImageFileNameMatchListFromMatchFile(string fileName, ORfileNameMatch* firstMatchInList);


#endif
