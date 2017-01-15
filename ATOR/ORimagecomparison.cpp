/*******************************************************************************
 *
 * File Name: ORimagecomparison.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a7e 12-June-2012
 *
 *******************************************************************************/

#include "ORimagecomparison.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

//#include <mysql.h>
using namespace std;




//#include "ORmethod.h"
#include "ORpixelMaps.h"
#include "ORoperations.h"

void disablePixelsThatAreNotContainedInTheObjectTriangle2DOD(int imageWidth, int imageHeight, unsigned char * rgbMap)
{
	colour disabledColour;
	disabledColour.r = OR_SNAPSHOT_BACKGROUND_COLOUR_R;
	disabledColour.g = OR_SNAPSHOT_BACKGROUND_COLOUR_G;
	disabledColour.b = OR_SNAPSHOT_BACKGROUND_COLOUR_B;
	
	for(int x=0; x<imageWidth; x++)
	{
		for(int y=0; y<imageHeight; y++)
		{
			bool disablePixel = false;
			int xDevFromCentre;
			if(x < (imageWidth/2))
			{
				xDevFromCentre = (imageWidth/2) - x;
			}
			else
			{
				xDevFromCentre = x - (imageWidth/2);
			}
			
			if(xDevFromCentre > (y/2))
			{
				disablePixel = true;
			}
			
			if(disablePixel)
			{
				setRGBMapValues(x, y, imageWidth, &disabledColour, rgbMap);
			}
		}	
	}


}


void applyGaussianKernelToRGBMap(int imageWidth, int imageHeight, double sigma, unsigned char * rgbMap, unsigned char * rgbMapWithGaussianApplied)
{
	applyGaussianKernelToLuminosityMapOrRGBMapComponent(imageWidth, imageHeight, sigma, NULL, NULL, rgbMap, rgbMapWithGaussianApplied, true, RGB_RED);
	applyGaussianKernelToLuminosityMapOrRGBMapComponent(imageWidth, imageHeight, sigma, NULL, NULL, rgbMap, rgbMapWithGaussianApplied, true, RGB_GREEN);
	applyGaussianKernelToLuminosityMapOrRGBMapComponent(imageWidth, imageHeight, sigma, NULL, NULL, rgbMap, rgbMapWithGaussianApplied, true, RGB_BLUE);
}

void applyGaussianKernelToLuminosityMapOrRGBMapComponent(int imageWidth, int imageHeight, double sigma, double * luminosityMap, double * luminosityMapWithGaussianApplied, unsigned char * rgbMap, unsigned char * rgbMapWithGaussianApplied, bool useRGBmapComponent, int rgbMapComponentNumber)
{
	double s;
	int support,i, j, k, kk, x, y;

	support = (int) (3.0*sigma);              /* set support size for gaussian */

	double * mask = new double[support +1];
	double * buf2 = new double[imageWidth*imageHeight];
	double * buf = new double[imageWidth*imageHeight];

	for(y = 0; y< imageHeight; y++)
	{
		for(x = 0; x< imageHeight; x++)
		{
			if(useRGBmapComponent)
			{
				buf[y*imageWidth + x] = double(getRGBMapValue(x, y, imageWidth, rgbMapComponentNumber, rgbMap));
			}
			else
			{
				buf[y*imageWidth + x] = getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityMap);
			}
		}
	}

	for(i=0; i <= support; i++)								/* one-sided gaussian mask */
	{
		mask[i]=1.0F/((double)sqrt(2.0F*(double)PI)*(sigma)*(sigma))*(double)exp((-0.5F*((double)i / sigma)*((double)i / sigma)));
	}

	for(j = 0; j < imageWidth*imageHeight; j+=imageWidth)
	{           /* filter all lines */
		s = mask[0];
		for(i = j+support; i < j+imageWidth-support; i++)		/* initialize vector with */
		buf2[i] = buf[i] * s;									/* central weight */

		for(k=1; k<=support; k++)
		{/* sum up remain. weighted */
			s = mask[k];										/* vectors */
			for(i=j+support; i < j+imageWidth-support; i++)
			buf2[i] += (buf[i+k] + buf[i-k]) * s;
		}
	}

	for(j = support*imageWidth; j < (imageHeight-support)*imageWidth; j+=imageWidth)
	{ /* filter all columns */
		s = mask[0];
		for(i = j+support; i < j+imageWidth-support; i++)		/* initialize vector */
		buf[i] = buf2[i] * s;                       			/* central weight */

		for(k=1, kk=imageWidth; kk<=support*imageWidth; kk+=imageWidth, k++)
		{ /* sum up remaining  */
			s = mask[k];                               			/* weighted vectors */

			for(i=j+support; i < j+imageWidth-support; i++)
			buf[i] += (buf2[i+kk] + buf2[i-kk]) * s;
		}
	}

	for (i=0;i<support;i++)
	{
		for (y=0;y<imageHeight;y++)
		{
			buf[i+y*imageWidth]=0.0F;
			buf[imageWidth-i-1+y*imageWidth]=0.0F;
		}
		for (x=0;x<imageWidth;x++)
		{
			buf[x+i*imageWidth]=0.0F;
			buf[x+(imageHeight-i-1)*imageWidth]=0.0F;
		}
	}


	for(y = 0; y< imageHeight; y++)
	{
		for(x = 0; x< imageHeight; x++)
		{
			if(useRGBmapComponent)
			{
				setRGBMapValue(x, y, imageWidth, rgbMapComponentNumber, rgbMapWithGaussianApplied, (unsigned char)(buf[y*imageWidth + x]));
			}
			else
			{		
				setLumOrContrastOrDepthMapValue(x, y, imageWidth, buf[y*imageWidth + x], luminosityMapWithGaussianApplied);
			}
		}
	}
	
	delete mask;
	delete buf2;
	delete buf;	
}


void createNormalisedHueContrastMapUsingRGBMapAndCalculateAllContrastValues(int imageWidth, int imageHeight, unsigned char * rgbMap, double * normalisedHueContrastMap, double * normalisedAverageRHueContrast, double * normalisedAverageGHueContrast, double * normalisedAverageBHueContrast, double * averageContrastWithRespectToAverageColour, double * averageStarkContrastWithRespectToAverageColour, double * averageLocalContrast, double * averageLocalStarkContrast)
{
	int totalR = 0;
	int totalG = 0;
	int totalB = 0;
	int index = 0;

	int totalRminusG = 0;
	int totalRminusB = 0;
	int totalGminusB = 0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);




		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

				int RminusG = (int(testImagePixelColour.r) - int(testImagePixelColour.g));
				int RminusB = (int(testImagePixelColour.r) - int(testImagePixelColour.b));
				int GminusB = (int(testImagePixelColour.g) - int(testImagePixelColour.b));
				totalRminusG = totalRminusG + abs(RminusG);
				totalRminusB = totalRminusB + abs(RminusB);
				totalGminusB = totalGminusB + abs(GminusB);

				/*
				cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;
				cout << "(int)testImagePixelColour.g = " << (int)testImagePixelColour.g << endl;
				cout << "(int)testImagePixelColour.b = " << (int)testImagePixelColour.b << endl;

				cout << "(int)RminusG = " << (int)RminusG << endl;
				cout << "(int)RminusB = " << (int)RminusB << endl;
				cout << "(int)GminusB = " << (int)GminusB << endl;
				*/

				totalR = totalR + testImagePixelColour.r;
				totalG = totalG + testImagePixelColour.g;
				totalB = totalB + testImagePixelColour.b;
				index++;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	//exit(0);

	double averageRminusG = double(totalRminusG)/double(index);
	double averageRminusB = double(totalRminusB)/double(index);
	double averageGminusB = double(totalGminusB)/double(index);

	/*
	cout << "totalRminusG = " << totalRminusG << endl;
	cout << "totalRminusB = " << totalRminusB << endl;
	cout << "totalGminusB = " << totalGminusB << endl;

	cout << "averageRminusG = " << averageRminusG << endl;
	cout << "averageRminusB = " << averageRminusB << endl;
	cout << "averageGminusB = " << averageGminusB << endl;
	*/

	double averager = double(totalR)/(index);
	double averageg = double(totalG)/(index);
	double averageb = double(totalB)/(index);

	double totalrContrastWithRespectToAverageGreaterThanThreshold = 0.0;
	double totalgContrastWithRespectToAverageGreaterThanThreshold = 0.0;
	double totalbContrastWithRespectToAverageGreaterThanThreshold = 0.0;
	double totalrContrastWithRespectToAverage = 0.0;
	double totalgContrastWithRespectToAverage = 0.0;
	double totalbContrastWithRespectToAverage = 0.0;

	double totalStarkLocalContrast = 0.0;
	int starkLocalContrastIndex = 0;

	int totalRHueContrast = 0;
	int totalGHueContrast = 0;
	int totalBHueContrast = 0;

	int hueContrastIndex = 0;



	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);
			//cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

				#ifdef OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_BINNING
				double rContrastWithRespectToAverage = absDouble(averager - int(testImagePixelColour.r));
				double gContrastWithRespectToAverage = absDouble(averageg - int(testImagePixelColour.g));
				double bContrastWithRespectToAverage = absDouble(averageb - int(testImagePixelColour.b));
				#else	//use hue RGB dev binning
				int RminusG = (int(testImagePixelColour.r) - int(testImagePixelColour.g));
				int RminusB = (int(testImagePixelColour.r) - int(testImagePixelColour.b));
				int GminusB = (int(testImagePixelColour.g) - int(testImagePixelColour.b));

				double rContrastWithRespectToAverage = absDouble(averageRminusG - RminusG);
				double gContrastWithRespectToAverage = absDouble(averageRminusB - RminusB);
				double bContrastWithRespectToAverage = absDouble(averageGminusB - GminusB);
				#endif

				totalrContrastWithRespectToAverage = totalrContrastWithRespectToAverage + rContrastWithRespectToAverage;
				totalgContrastWithRespectToAverage = totalgContrastWithRespectToAverage + gContrastWithRespectToAverage;
				totalbContrastWithRespectToAverage = totalbContrastWithRespectToAverage + bContrastWithRespectToAverage;

				if(rContrastWithRespectToAverage > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
				{
					totalrContrastWithRespectToAverageGreaterThanThreshold = totalrContrastWithRespectToAverageGreaterThanThreshold + rContrastWithRespectToAverage;
				}
				if(gContrastWithRespectToAverage > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
				{
					totalgContrastWithRespectToAverageGreaterThanThreshold = totalgContrastWithRespectToAverageGreaterThanThreshold + gContrastWithRespectToAverage;
				}
				if(bContrastWithRespectToAverage > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
				{
					totalbContrastWithRespectToAverageGreaterThanThreshold = totalbContrastWithRespectToAverageGreaterThanThreshold + bContrastWithRespectToAverage;
				}




				int redContrastTotalAcrossKernel = 0;
				int greenContrastTotalAcrossKernel = 0;
				int blueContrastTotalAcrossKernel = 0;

				for(int ky = y-(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky <= y+(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky++)
				{
					for(int kx = x-(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx <= x+(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								if(!((kx == x) && (ky == y)))
								{

									colour testImagePixelColourKernel;
									getRGBMapValues(kx, ky, imageWidth, rgbMap, &testImagePixelColourKernel);
									//cout << "\t(testImagePixelColourKernel.r = " << (testImagePixelColourKernel.r << endl;

									#ifdef OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_BINNING
									int rContrastWRTkernel = testImagePixelColour.r - testImagePixelColourKernel.r;
									int gContrastWRTkernel = testImagePixelColour.g - testImagePixelColourKernel.g;
									int bContrastWRTkernel = testImagePixelColour.b - testImagePixelColourKernel.b;

									int hueContrast = (abs(abs(redContrast) - abs(greenContrast)) + abs(abs(redContrast) - abs(blueContrast)) + abs(abs(greenContrast) - abs(blueContrast)));
									#else	//use hue RGB dev binning
									int kernelRminusG = (int(testImagePixelColourKernel.r) - int(testImagePixelColourKernel.g));
									int kernelRminusB = (int(testImagePixelColourKernel.r) - int(testImagePixelColourKernel.b));
									int kernelGminusB = (int(testImagePixelColourKernel.g) - int(testImagePixelColourKernel.b));

									int rContrastWRTkernel = abs(kernelRminusG - RminusG);
									int gContrastWRTkernel = abs(kernelRminusB - RminusB);
									int bContrastWRTkernel = abs(kernelGminusB - GminusB);

									#endif

									redContrastTotalAcrossKernel = redContrastTotalAcrossKernel + abs(rContrastWRTkernel);
									greenContrastTotalAcrossKernel = greenContrastTotalAcrossKernel + abs(gContrastWRTkernel);
									blueContrastTotalAcrossKernel = blueContrastTotalAcrossKernel + abs(bContrastWRTkernel);


								}
							}
						}
					}
				}

					//NEW; additional; check for global hue dev [due to lighting conditions]

				totalRHueContrast = totalRHueContrast + redContrastTotalAcrossKernel;
				totalGHueContrast = totalGHueContrast + greenContrastTotalAcrossKernel;
				totalBHueContrast = totalBHueContrast + blueContrastTotalAcrossKernel;

				//cout << "redContrastTotalAcrossKernel = " << redContrastTotalAcrossKernel << endl;
				//cout << "greenContrastTotalAcrossKernel = " << greenContrastTotalAcrossKernel << endl;
				//cout << "blueContrastTotalAcrossKernel = " << blueContrastTotalAcrossKernel << endl;

				double averagerContrastTotalAcrossKernel = redContrastTotalAcrossKernel/(IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT);
				double averagegContrastTotalAcrossKernel = greenContrastTotalAcrossKernel/(IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT);
				double averagebContrastTotalAcrossKernel = blueContrastTotalAcrossKernel/(IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT);
				double averageHueContrastAcrossKernel = (redContrastTotalAcrossKernel + greenContrastTotalAcrossKernel + blueContrastTotalAcrossKernel)/(IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT);
				vec normalisedAverageHueContrastAcrossKernel;

				/*OLD; normalised wrt to average luminosity not saturation
				normalisedAverageHueContrastAcrossKernel.x = (averagerContrastTotalAcrossKernel/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
				normalisedAverageHueContrastAcrossKernel.y = (averagegContrastTotalAcrossKernel/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
				normalisedAverageHueContrastAcrossKernel.z = (averagebContrastTotalAcrossKernel/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
				*/

				normalisedAverageHueContrastAcrossKernel.x = (averagerContrastTotalAcrossKernel/(double(redContrastTotalAcrossKernel+greenContrastTotalAcrossKernel+blueContrastTotalAcrossKernel)/3.0))/double(MAX_RGB_VAL);
				normalisedAverageHueContrastAcrossKernel.y = (averagegContrastTotalAcrossKernel/(double(redContrastTotalAcrossKernel+greenContrastTotalAcrossKernel+blueContrastTotalAcrossKernel)/3.0))/double(MAX_RGB_VAL);
				normalisedAverageHueContrastAcrossKernel.z = (averagebContrastTotalAcrossKernel/(double(redContrastTotalAcrossKernel+greenContrastTotalAcrossKernel+blueContrastTotalAcrossKernel)/3.0))/double(MAX_RGB_VAL);

				/*bad;
				if(averageRminusG != 0)
				{
					normalisedAverageHueContrastAcrossKernel.x = (averagerContrastTotalAcrossKernel/(double(averageRminusG)))/double(MAX_RGB_VAL);
				}
				if(averageRminusB != 0)
				{
					normalisedAverageHueContrastAcrossKernel.y = (averagegContrastTotalAcrossKernel/(double(averageRminusB)))/double(MAX_RGB_VAL);
				}
				if(averageGminusB != 0)
				{
					normalisedAverageHueContrastAcrossKernel.z = (averagebContrastTotalAcrossKernel/(double(averageGminusB)))/double(MAX_RGB_VAL);
				}
				*/

				setVectorMapValue(x, y, imageWidth, &normalisedAverageHueContrastAcrossKernel, normalisedHueContrastMap);

					//NEW; additional; check for stark differences


				if(averageHueContrastAcrossKernel > OR_IMAGE_COMPARISON_STARK_LOCAL_CONTRAST_MIN_AVG_COL_DIFF)
				{
					totalStarkLocalContrast = totalStarkLocalContrast + averageHueContrastAcrossKernel;
					starkLocalContrastIndex++;
				}
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	//cout << "totalRHueContrast = " << totalRHueContrast << endl;
	//cout << "totalGHueContrast = " << totalGHueContrast << endl;
	//cout << "totalBHueContrast = " << totalBHueContrast << endl;

	double averageRHueContrast = double(totalRHueContrast)/(double(index*IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT));
	double averageGHueContrast = double(totalGHueContrast)/(double(index*IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT));
	double averageBHueContrast = double(totalBHueContrast)/(double(index*IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT));

	//cout << "averageRHueContrast = " << averageRHueContrast << endl;
	//cout << "averageGHueContrast = " << averageGHueContrast << endl;
	//cout << "averageBHueContrast = " << averageBHueContrast << endl;

	/*OLD; normalised wrt to average luminosity not saturation
	*normalisedAverageRHueContrast = (averageRHueContrast/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
	*normalisedAverageGHueContrast = (averageGHueContrast/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
	*normalisedAverageBHueContrast = (averageBHueContrast/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
	*/

	*normalisedAverageRHueContrast = (averageRHueContrast/(double(averageRHueContrast+averageGHueContrast+averageBHueContrast)/3.0));	///double(MAX_RGB_VAL)
	*normalisedAverageGHueContrast = (averageGHueContrast/(double(averageRHueContrast+averageGHueContrast+averageBHueContrast)/3.0));
	*normalisedAverageBHueContrast = (averageBHueContrast/(double(averageRHueContrast+averageGHueContrast+averageBHueContrast)/3.0));

	/*OLD Bad;
	if(averageRminusG != 0)
	{
		*normalisedAverageRHueContrast = (averageRHueContrast/(double(averageRminusG)))/double(MAX_RGB_VAL);
	}
	if(averageRminusB != 0)
	{
		*normalisedAverageGHueContrast = (averageGHueContrast/(double(averageRminusB)))/double(MAX_RGB_VAL);
	}
	if(averageGminusB != 0)
	{
		*normalisedAverageBHueContrast = (averageBHueContrast/(double(averageGminusB)))/double(MAX_RGB_VAL);
	}
	*/



	//cout << "normalisedAverageRHueContrast = " << *normalisedAverageRHueContrast << endl;
	//cout << "normalisedAverageGHueContrast = " << *normalisedAverageGHueContrast << endl;
	//cout << "normalisedAverageBHueContrast = " << *normalisedAverageBHueContrast << endl;


	double averagerContrastWithRespectToAverage = totalrContrastWithRespectToAverage/index;
	double averagegContrastWithRespectToAverage = totalgContrastWithRespectToAverage/index;
	double averagebContrastWithRespectToAverage = totalbContrastWithRespectToAverage/index;

	double totalContrastWithRespectToAverage = totalrContrastWithRespectToAverage + totalgContrastWithRespectToAverage + totalbContrastWithRespectToAverage;
	double totalStarkContrastWithRespectToAverage = totalrContrastWithRespectToAverageGreaterThanThreshold + totalgContrastWithRespectToAverageGreaterThanThreshold + totalbContrastWithRespectToAverageGreaterThanThreshold;
	double totalLocalContrast = totalRHueContrast + totalGHueContrast + totalBHueContrast;

	*averageContrastWithRespectToAverageColour = (totalContrastWithRespectToAverage/index);						//compare with OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION		//should be /3 after future change	//or; averagerContrastWithRespectToAverage+averagegContrastWithRespectToAverage+averagebContrastWithRespectToAverage
	#ifdef OR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
	*averageStarkContrastWithRespectToAverageColour = totalStarkContrastWithRespectToAverage/(imageWidth*imageHeight);		//compare with OR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION	//should be /index after future change without OR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
	#else
	*averageStarkContrastWithRespectToAverageColour = totalStarkContrastWithRespectToAverage/index;
	#endif
	*averageLocalContrast = totalLocalContrast/index;					//not currently used
	*averageLocalStarkContrast = totalStarkLocalContrast/index;				//not currently used

	/*
	cout << "*averageLocalContrast  = " << *averageLocalContrast << endl;
	cout << "*averageContrastWithRespectToAverageColour = " << *averageContrastWithRespectToAverageColour << endl;
	cout << "OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = " << OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION << endl;
	cout << "OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION = " << OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION << endl;
	*/
}

bool checkImageHasContrastValuesOnly(double averageContrastWithRespectToAverageColour, double averageStarkContrastWithRespectToAverageColour, double averageLocalContrast, double averageLocalStarkContrast)
{
	#ifdef OR_IMAGE_COMPARISON_REQUIRE_SNAPSHOT_STARK_LOCAL_CONTRAST
	if((averageContrastWithRespectToAverageColour > OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averageStarkContrastWithRespectToAverageColour > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
	&& (averageLocalContrast > OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION) && (averageLocalStarkContrast > OR_IMAGE_COMPARISON_HAS_STARK_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION))
	#else
	if((averageContrastWithRespectToAverageColour > OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averageLocalContrast > OR_IMAGE_COMPARISON_HAS_LOCAL_CONTRAST_MIN_AVERAGE_COL_DEVIATION))
	#endif
	{
		return true;
	}
	else
	{
		return false;
	}
}









	//compareImagesRGBWithPosDevAndLocalStarkContAndHueDev or compareImagesRGBWithPosDevAndLocalStarkContAndLumDev [assumes light source is always white]


void calculateAverageColour(int imageWidth, int imageHeight, unsigned char * rgbMap, colour * avgCol)
{
	double totalColr = 0.0;
	double totalColg = 0.0;
	double totalColb = 0.0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);
			totalColr = totalColr + testImagePixelColour.r;
			totalColg = totalColg + testImagePixelColour.g;
			totalColb = totalColb + testImagePixelColour.b;
			//cout
		}
	}

	avgCol->r = totalColr/(imageHeight*imageWidth);
	avgCol->g = totalColg/(imageHeight*imageWidth);
	avgCol->b = totalColb/(imageHeight*imageWidth);
}


double compareImagesRGBWithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(int imageWidth, int imageHeight, unsigned char * testrgbMap, unsigned char * trainrgbMap, double * averageRHueDeviation, double * averageGHueDeviation, double * averageBHueDeviation, double * averageXKernelRelativePositionForLowestErrorMatch, double * averageYKernelRelativePositionForLowestErrorMatch)
{
	double totalStarkLocalDeviation = 0.0;
	int starkLocalDeviationIndex = 0;

	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	double totalXKernelRelativePositionForLowestErrorMatch = 0.0;
	double totalYKernelRelativePositionForLowestErrorMatch = 0.0;

	double totalRHueDeviation = 0.0;
	double totalGHueDeviation = 0.0;
	double totalBHueDeviation = 0.0;
	int hueDeviationIndex = 0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, testrgbMap, &testImagePixelColour);
			//cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			getRGBMapValues(x, y, imageWidth, trainrgbMap, &trainImagePixelColourCentre);
			if(((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)) || ((trainImagePixelColourCentre.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (trainImagePixelColourCentre.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (trainImagePixelColourCentre.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
			#endif
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

				double lowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				int redErrorlowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				int blueErrorlowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				int greenErrorlowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				double XKernelRelativePositionForLowestErrorMatch;
				double YKernelRelativePositionForLowestErrorMatch;

				for(int ky = y-(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky <= y+(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky++)
				{
					for(int kx = x-(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx <= x+(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								colour trainImagePixelColour;
								getRGBMapValues(kx, ky, imageWidth, trainrgbMap, &trainImagePixelColour);
								//cout << "\t(inttrainImagePixelColour.r = " << (inttrainImagePixelColour.r << endl;

							#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBPixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
							#else
								//double pixelError = compareRGBPixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);

								int redError = testImagePixelColour.r - trainImagePixelColour.r;
								int greenError = testImagePixelColour.g - trainImagePixelColour.g;
								int blueError = testImagePixelColour.b - trainImagePixelColour.b;

								double hueError = (abs(abs(redError) - abs(greenError)) + abs(abs(redError) - abs(blueError)) + abs(abs(greenError) - abs(blueError)));

								double pixelError =  (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR);

							#endif

								if(pixelError < lowestMatchErrorAcrossKernel)
								{

									lowestMatchErrorAcrossKernel = pixelError;
									redErrorlowestMatchErrorAcrossKernel = redError;
									blueErrorlowestMatchErrorAcrossKernel = greenError;
									greenErrorlowestMatchErrorAcrossKernel = blueError;
									XKernelRelativePositionForLowestErrorMatch = kx-x;
									YKernelRelativePositionForLowestErrorMatch = ky-y;

									/*What was this for???
									if(!compareDoubles(pixelError, 0.0))
									{
										XKernelRelativePositionForLowestErrorMatch = kx-x;
										YKernelRelativePositionForLowestErrorMatch = ky-y;
									}
									else
									{//if error is zero, just take the centre kernel pixel as the match
										XKernelRelativePositionForLowestErrorMatch = 0.0;
										YKernelRelativePositionForLowestErrorMatch = 0.0;
									}
									*/
								}

							}
						}
					}
				}

					//NEW; additional; check for global hue dev [due to lighting conditions]

				totalRHueDeviation = totalRHueDeviation + redErrorlowestMatchErrorAcrossKernel;
				totalGHueDeviation = totalGHueDeviation + greenErrorlowestMatchErrorAcrossKernel;
				totalBHueDeviation = totalBHueDeviation + blueErrorlowestMatchErrorAcrossKernel;
				hueDeviationIndex++;

					//NEW; additional; check for stark differences

				colour trainImagePixelColourOptimum;
				getRGBMapValues(XKernelRelativePositionForLowestErrorMatch+x, YKernelRelativePositionForLowestErrorMatch+y, imageWidth, trainrgbMap, &trainImagePixelColourOptimum);

				double rdeviationOptimum = absDouble(redErrorlowestMatchErrorAcrossKernel);
				double gdeviationOptimum = absDouble(greenErrorlowestMatchErrorAcrossKernel);
				double bdeviationOptimum = absDouble(blueErrorlowestMatchErrorAcrossKernel);

				double deviationOptimum = rdeviationOptimum + gdeviationOptimum + bdeviationOptimum;
				if(deviationOptimum > OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF)
				//if((averagerdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagegdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagebdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION))
				{
					totalStarkLocalDeviation = totalStarkLocalDeviation + deviationOptimum;
					starkLocalDeviationIndex++;
				}


				totalXKernelRelativePositionForLowestErrorMatch = totalXKernelRelativePositionForLowestErrorMatch+XKernelRelativePositionForLowestErrorMatch;
				totalYKernelRelativePositionForLowestErrorMatch = totalYKernelRelativePositionForLowestErrorMatch+YKernelRelativePositionForLowestErrorMatch;

				totalMatchError = totalMatchError+lowestMatchErrorAcrossKernel;
				matchErrorIndex++;
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	*averageRHueDeviation = totalRHueDeviation/(matchErrorIndex);
	*averageGHueDeviation = totalGHueDeviation/(matchErrorIndex);
	*averageBHueDeviation = totalBHueDeviation/(matchErrorIndex);

	*averageXKernelRelativePositionForLowestErrorMatch = totalXKernelRelativePositionForLowestErrorMatch/(matchErrorIndex);
	*averageYKernelRelativePositionForLowestErrorMatch = totalYKernelRelativePositionForLowestErrorMatch/(matchErrorIndex);
	//cout << "averageXKernelRelativePositionForLowestErrorMatch = " << averageXKernelRelativePositionForLowestErrorMatch << endl;
	//cout << "averageYKernelRelativePositionForLowestErrorMatch = " << averageYKernelRelativePositionForLowestErrorMatch << endl;

	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);
	double error = averageMatchErrorNormalised;

	//cout << "totalMatchErrorNormalised = " << totalMatchErrorNormalised << endl;
	//cout << "error = " << error << endl;

	if(starkLocalDeviationIndex > OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS*(imageWidth*imageHeight))
	{
		error = 8888.0;
	}

	return error;
}




double compareImagesRGBWithPosDevAndLocalStarkContAndHueDev(int imageWidth, int imageHeight, unsigned char * testrgbMap, unsigned char * trainrgbMap)
{
	double totalStarkLocalDeviation = 0.0;
	int starkLocalDeviationIndex = 0;

	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	double averageXKernelRelativePositionForLowestErrorMatch = 0.0;
	double averageYKernelRelativePositionForLowestErrorMatch = 0.0;

	double averageRHueDeviation = 0.0;
	double averageGHueDeviation = 0.0;
	double averageBHueDeviation = 0.0;
	double averageLumDeviation = 0.0;

	double totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch = 0.0;
	double totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch = 0.0;
	double totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch = 0.0;
	double totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch = 0.0;

	double totalDifferenceFromAverageRHueDeviation = 0.0;
	double totalDifferenceFromAverageGHueDeviation = 0.0;
	double totalDifferenceFromAverageBHueDeviation = 0.0;
	double totalDifferenceFromAverageLumDeviation = 0.0;

	int hueDeviationIndex = 0;


	compareImagesRGBWithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(imageWidth, imageHeight, testrgbMap, trainrgbMap, &averageRHueDeviation, &averageGHueDeviation, &averageBHueDeviation, &averageXKernelRelativePositionForLowestErrorMatch, &averageYKernelRelativePositionForLowestErrorMatch);

	averageLumDeviation = averageRHueDeviation + averageGHueDeviation + averageBHueDeviation;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, testrgbMap, &testImagePixelColour);
			//cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			getRGBMapValues(x, y, imageWidth, trainrgbMap, &trainImagePixelColourCentre);
			if(((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)) || ((trainImagePixelColourCentre.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (trainImagePixelColourCentre.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (trainImagePixelColourCentre.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
			#endif
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

				double lowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				int redErrorlowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				int blueErrorlowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				int greenErrorlowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				double XKernelRelativePositionForLowestErrorMatch;
				double YKernelRelativePositionForLowestErrorMatch;

				for(int ky = y-(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky <= y+(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky++)
				{
					for(int kx = x-(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx <= x+(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								colour trainImagePixelColour;
								getRGBMapValues(kx, ky, imageWidth, trainrgbMap, &trainImagePixelColour);
								//cout << "\t(inttrainImagePixelColour.r = " << (inttrainImagePixelColour.r << endl;

							#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBPixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
							#else
								//double pixelError = compareRGBPixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);

								int redError = testImagePixelColour.r - trainImagePixelColour.r;
								int greenError = testImagePixelColour.g - trainImagePixelColour.g;
								int blueError = testImagePixelColour.b - trainImagePixelColour.b;

								double hueError = (abs(abs(redError) - abs(greenError)) + abs(abs(redError) - abs(blueError)) + abs(abs(greenError) - abs(blueError)));

								double pixelError = (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR);

							#endif

								if(pixelError < lowestMatchErrorAcrossKernel)
								{

									lowestMatchErrorAcrossKernel = pixelError;
									redErrorlowestMatchErrorAcrossKernel = redError;
									blueErrorlowestMatchErrorAcrossKernel = greenError;
									greenErrorlowestMatchErrorAcrossKernel = blueError;
									XKernelRelativePositionForLowestErrorMatch = kx-x;
									YKernelRelativePositionForLowestErrorMatch = ky-y;

									/*What was this for???
									if(!compareDoubles(pixelError, 0.0))
									{
										XKernelRelativePositionForLowestErrorMatch = kx-x;
										YKernelRelativePositionForLowestErrorMatch = ky-y;
									}
									else
									{//if error is zero, just take the centre kernel pixel as the match
										XKernelRelativePositionForLowestErrorMatch = 0.0;
										YKernelRelativePositionForLowestErrorMatch = 0.0;
									}
									*/
								}

							}
						}
					}
				}

					//NEW; additional; check for global hue dev [due to lighting conditions]

				totalDifferenceFromAverageRHueDeviation = totalDifferenceFromAverageRHueDeviation + absDouble(averageRHueDeviation - redErrorlowestMatchErrorAcrossKernel);
				totalDifferenceFromAverageGHueDeviation = totalDifferenceFromAverageGHueDeviation + absDouble(averageGHueDeviation - greenErrorlowestMatchErrorAcrossKernel);
				totalDifferenceFromAverageBHueDeviation = totalDifferenceFromAverageBHueDeviation + absDouble(averageBHueDeviation - blueErrorlowestMatchErrorAcrossKernel);
				totalDifferenceFromAverageLumDeviation = totalDifferenceFromAverageLumDeviation + absDouble(averageLumDeviation - (redErrorlowestMatchErrorAcrossKernel+blueErrorlowestMatchErrorAcrossKernel+greenErrorlowestMatchErrorAcrossKernel));
				hueDeviationIndex++;

					//NEW; additional; check for stark differences

				colour trainImagePixelColourOptimum;
				getRGBMapValues(XKernelRelativePositionForLowestErrorMatch+x, YKernelRelativePositionForLowestErrorMatch+y, imageWidth, trainrgbMap, &trainImagePixelColourOptimum);

				double rdeviationOptimum = absDouble(redErrorlowestMatchErrorAcrossKernel);
				double gdeviationOptimum = absDouble(greenErrorlowestMatchErrorAcrossKernel);
				double bdeviationOptimum = absDouble(blueErrorlowestMatchErrorAcrossKernel);

				double deviationOptimum = rdeviationOptimum + gdeviationOptimum + bdeviationOptimum;
				if(deviationOptimum > OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF)
				//if((averagerdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagegdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagebdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION))
				{
					totalStarkLocalDeviation = totalStarkLocalDeviation + deviationOptimum;
					starkLocalDeviationIndex++;
				}


				if(averageXKernelRelativePositionForLowestErrorMatch-XKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch + absDouble(averageXKernelRelativePositionForLowestErrorMatch-XKernelRelativePositionForLowestErrorMatch);
				}
				else
				{
					totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch + absDouble(averageXKernelRelativePositionForLowestErrorMatch-XKernelRelativePositionForLowestErrorMatch);
				}

				if(averageYKernelRelativePositionForLowestErrorMatch-YKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch + absDouble(averageYKernelRelativePositionForLowestErrorMatch-YKernelRelativePositionForLowestErrorMatch);
				}
				else
				{
					totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch + absDouble(averageYKernelRelativePositionForLowestErrorMatch-YKernelRelativePositionForLowestErrorMatch);
				}

				totalMatchError = totalMatchError+lowestMatchErrorAcrossKernel;
				matchErrorIndex++;
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	double averagetotalDifferenceFromAverageRHueDeviation = totalDifferenceFromAverageRHueDeviation/(matchErrorIndex);
	double averagetotalDifferenceFromAverageGHueDeviation = totalDifferenceFromAverageGHueDeviation/(matchErrorIndex);
	double averagetotalDifferenceFromAverageBHueDeviation = totalDifferenceFromAverageBHueDeviation/(matchErrorIndex);
	double averagetotalDifferenceFromAverageLumDeviation = totalDifferenceFromAverageLumDeviation/(matchErrorIndex);
	//cout << "averagetotalDifferenceFromAverageRHueDeviation = " << averagetotalDifferenceFromAverageRHueDeviation << endl;
	//cout << "averagetotalDifferenceFromAverageGHueDeviation = " << averagetotalDifferenceFromAverageGHueDeviation << endl;
	//cout << "averagetotalDifferenceFromAverageBHueDeviation = " << averagetotalDifferenceFromAverageBHueDeviation << endl;

	double averagetotalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averagetotalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	double averagetotalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averagetotalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	double averageXPositionalDeviation = (averagetotalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch + -averagetotalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averageYPositionalDeviation = (averagetotalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch + -averagetotalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	//cout << "averagetotalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch = " << averageXKernelRelativePositionPositiveForLowestErrorMatch << endl;
	//cout << "averagetotalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch = " << averageXKernelRelativePositionNegativeForLowestErrorMatch << endl;
	//cout << "averagetotalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch = " << averageYKernelRelativePositionPositiveForLowestErrorMatch << endl;
	//cout << "averagetotalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch = " << averageYKernelRelativePositionNegativeForLowestErrorMatch << endl;
	//cout << "averageXPositionalDeviation = " << averageXPositionalDeviation << endl;
	//cout << "averageYPositionalDeviation = " << averageYPositionalDeviation << endl;

	double averagePositionalDeviationNormalised = (averageXPositionalDeviation+averageYPositionalDeviation)/2.0;		//normalised to max=1.0 		//IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING;
	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);
	double averageHueDeviationNormalised = ((averagetotalDifferenceFromAverageRHueDeviation + averagetotalDifferenceFromAverageGHueDeviation + averagetotalDifferenceFromAverageBHueDeviation)/(3.0*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR));	//normalise to ~1

#ifdef OR_IMAGE_COMPARISON_DEBUG_NEW_COMPARITOR
	double error = averageMatchErrorNormalised + averageHueDeviationNormalised;
	/*
	double error = averageMatchErrorNormalised + averagePositionalDeviationNormalised + averageHueDeviationNormalised;
	if(starkLocalDeviationIndex > OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS*(imageWidth*imageHeight))
	{
		error = 8888.0;
	}
	*/
#else
	double error = averageMatchErrorNormalised + averagePositionalDeviationNormalised + averageHueDeviationNormalised;
	if(starkLocalDeviationIndex > OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS*(imageWidth*imageHeight))
	{
		error = 8888.0;
	}
#endif



	//cout << "averagePositionalDeviationNormalised = " << averagePositionalDeviationNormalised << endl;
	//cout << "totalMatchErrorNormalised = " << totalMatchErrorNormalised << endl;
	//cout << "averageHueDeviationNormalised = " << averageHueDeviationNormalised << endl;
	//cout << "error = " << error << endl;



	return error;
}




double compareImagesRGBSmallNoKernel(int imageWidth, int imageHeight, unsigned char * testrgbMap, unsigned char * trainrgbMap)
{
	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	int hueDeviationIndex = 0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, testrgbMap, &testImagePixelColour);
			//cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR

			colour trainImagePixelColourCentre;
			getRGBMapValues(x, y, imageWidth, trainrgbMap, &trainImagePixelColourCentre);
			if(((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)) || ((trainImagePixelColourCentre.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (trainImagePixelColourCentre.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (trainImagePixelColourCentre.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

			#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
				double pixelError = compareRGBPixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColourCentre);
			#else
				int redError = testImagePixelColour.r - trainImagePixelColourCentre.r;
				int greenError = testImagePixelColour.g - trainImagePixelColourCentre.g;
				int blueError = testImagePixelColour.b - trainImagePixelColourCentre.b;

				double hueError = (abs(abs(redError) - abs(greenError)) + abs(abs(redError) - abs(blueError)) + abs(abs(greenError) - abs(blueError)));

				double pixelError = (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR);

			#endif

				totalMatchError = totalMatchError+pixelError;
				matchErrorIndex++;
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	double error = (totalMatchError/matchErrorIndex);

	//cout << "error = " << error << endl;

	return error;
}




#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
double compareImagesRGBWithPosDevAndLocalStarkCont(int imageWidth, int imageHeight, unsigned char * testrgbMap, unsigned char * trainrgbMap)
{
	double totalStarkLocalDeviation = 0.0;
	int starkLocalDeviationIndex = 0;

	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	double totalXKernelRelativePositionPositiveForLowestErrorMatch = 0.0;
	double totalYKernelRelativePositionPositiveForLowestErrorMatch = 0.0;
	double totalXKernelRelativePositionNegativeForLowestErrorMatch = 0.0;
	double totalYKernelRelativePositionNegativeForLowestErrorMatch = 0.0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, testrgbMap, &testImagePixelColour);
			//cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			getRGBMapValues(x, y, imageWidth, trainrgbMap, &trainImagePixelColourCentre);
			if(((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)) || ((trainImagePixelColourCentre.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (trainImagePixelColourCentre.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (trainImagePixelColourCentre.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
			#endif
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

				double lowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				double XKernelRelativePositionForLowestErrorMatch;
				double YKernelRelativePositionForLowestErrorMatch;

				for(int ky = y-(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky <= y+(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky++)
				{
					for(int kx = x-(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx <= x+(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								colour trainImagePixelColour;
								getRGBMapValues(kx, ky, imageWidth, trainrgbMap, &trainImagePixelColour);
								//cout << "\t(inttrainImagePixelColour.r = " << (inttrainImagePixelColour.r << endl;

								#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBPixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
								#else
								double pixelError = compareRGBPixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);
								#endif

								if(pixelError < lowestMatchErrorAcrossKernel)
								{
									if(!compareDoubles(pixelError, 0.0))
									{
										lowestMatchErrorAcrossKernel = pixelError;
										XKernelRelativePositionForLowestErrorMatch = kx-x;
										YKernelRelativePositionForLowestErrorMatch = ky-y;
									}
									else
									{//if error is zero, just take the centre kernel pixel as the match
										lowestMatchErrorAcrossKernel = pixelError;
										XKernelRelativePositionForLowestErrorMatch = 0.0;
										YKernelRelativePositionForLowestErrorMatch = 0.0;
									}
								}

							}
						}
					}
				}

					//NEW; additional; check for stark differences

				colour trainImagePixelColourOptimum;
				getRGBMapValues(XKernelRelativePositionForLowestErrorMatch+x, YKernelRelativePositionForLowestErrorMatch+y, imageWidth, trainrgbMap, &trainImagePixelColourOptimum);

				double rdeviationOptimum = absDouble(trainImagePixelColourOptimum.r - testImagePixelColour.r);
				double gdeviationOptimum = absDouble(trainImagePixelColourOptimum.g - testImagePixelColour.g);
				double bdeviationOptimum = absDouble(trainImagePixelColourOptimum.b - testImagePixelColour.b);

				double deviationOptimum = rdeviationOptimum + gdeviationOptimum + bdeviationOptimum;
				if(deviationOptimum > OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF)
				//if((averagerdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagegdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagebdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION))
				{
					totalStarkLocalDeviation = totalStarkLocalDeviation + deviationOptimum;
					starkLocalDeviationIndex++;
				}


				if(XKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalXKernelRelativePositionPositiveForLowestErrorMatch = totalXKernelRelativePositionPositiveForLowestErrorMatch+XKernelRelativePositionForLowestErrorMatch;
				}
				else
				{
					totalXKernelRelativePositionNegativeForLowestErrorMatch = totalXKernelRelativePositionNegativeForLowestErrorMatch+XKernelRelativePositionForLowestErrorMatch;
				}

				if(YKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalYKernelRelativePositionPositiveForLowestErrorMatch = totalYKernelRelativePositionPositiveForLowestErrorMatch+YKernelRelativePositionForLowestErrorMatch;
				}
				else
				{
					totalYKernelRelativePositionNegativeForLowestErrorMatch = totalYKernelRelativePositionNegativeForLowestErrorMatch+YKernelRelativePositionForLowestErrorMatch;
				}

				totalMatchError = totalMatchError+lowestMatchErrorAcrossKernel;
				matchErrorIndex++;
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}


	double averageXKernelRelativePositionPositiveForLowestErrorMatch = totalXKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averageXKernelRelativePositionNegativeForLowestErrorMatch = totalXKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	double averageYKernelRelativePositionPositiveForLowestErrorMatch = totalYKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averageYKernelRelativePositionNegativeForLowestErrorMatch = totalYKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	//cout << "averageXKernelRelativePositionPositiveForLowestErrorMatch = " << averageXKernelRelativePositionPositiveForLowestErrorMatch << endl;
	//cout << "averageXKernelRelativePositionNegativeForLowestErrorMatch = " << averageXKernelRelativePositionNegativeForLowestErrorMatch << endl;
	//cout << "averageYKernelRelativePositionPositiveForLowestErrorMatch = " << averageYKernelRelativePositionPositiveForLowestErrorMatch << endl;
	//cout << "averageYKernelRelativePositionNegativeForLowestErrorMatch = " << averageYKernelRelativePositionNegativeForLowestErrorMatch << endl;

	double averageXPositionalDeviation = (averageXKernelRelativePositionPositiveForLowestErrorMatch + -averageXKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averageYPositionalDeviation = (averageYKernelRelativePositionPositiveForLowestErrorMatch + -averageYKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averagePositionalDeviationNormalised = ((averageXPositionalDeviation+averageYPositionalDeviation)/IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING;
	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);

	double error = averageMatchErrorNormalised + averagePositionalDeviationNormalised;


	//cout << "averageXPositionalDeviation = " << averageXPositionalDeviation << endl;
	//cout << "averageYPositionalDeviation = " << averageYPositionalDeviation << endl;
	//cout << "averagePositionalDeviationNormalised = " << averagePositionalDeviationNormalised << endl;
	//cout << "totalMatchErrorNormalised = " << totalMatchErrorNormalised << endl;
	//cout << "error = " << error << endl;

	if(starkLocalDeviationIndex > OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS*(imageWidth*imageHeight))
	{
		error = 8888.0;
	}

	return error;
}



double compareImagesRGBWithPosDev(int imageWidth, int imageHeight, unsigned char * testrgbMap, unsigned char * trainrgbMap)
{
	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	double totalXKernelRelativePositionPositiveForLowestErrorMatch = 0.0;
	double totalYKernelRelativePositionPositiveForLowestErrorMatch = 0.0;
	double totalXKernelRelativePositionNegativeForLowestErrorMatch = 0.0;
	double totalYKernelRelativePositionNegativeForLowestErrorMatch = 0.0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, testrgbMap, &testImagePixelColour);
			//cout << "(int)testImagePixelColour.r = " << (int)testImagePixelColour.r << endl;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			getRGBMapValues(x, y, imageWidth, trainrgbMap, &trainImagePixelColourCentre);
			if(((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)) || ((trainImagePixelColourCentre.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (trainImagePixelColourCentre.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (trainImagePixelColourCentre.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
			#endif
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif

				double lowestMatchErrorAcrossKernel = VERY_HIGH_ERROR;
				double XKernelRelativePositionForLowestErrorMatch;
				double YKernelRelativePositionForLowestErrorMatch;

				for(int ky = y-(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky <= y+(IMAGE_COMPARISON_KERNEL_HEIGHT/2); ky++)
				{
					for(int kx = x-(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx <= x+(IMAGE_COMPARISON_KERNEL_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								colour trainImagePixelColour;
								getRGBMapValues(kx, ky, imageWidth, trainrgbMap, &trainImagePixelColour);
								//cout << "\t(inttrainImagePixelColour.r = " << (inttrainImagePixelColour.r << endl;

								#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBPixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
								#else
								double pixelError = compareRGBPixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);
								#endif

								if(pixelError < lowestMatchErrorAcrossKernel)
								{
									if(!compareDoubles(pixelError, 0.0))
									{
										lowestMatchErrorAcrossKernel = pixelError;
										XKernelRelativePositionForLowestErrorMatch = kx-x;
										YKernelRelativePositionForLowestErrorMatch = ky-y;
									}
									else
									{//if error is zero, just take the centre kernel pixel as the match
										lowestMatchErrorAcrossKernel = pixelError;
										XKernelRelativePositionForLowestErrorMatch = 0.0;
										YKernelRelativePositionForLowestErrorMatch = 0.0;
									}
								}

							}
						}
					}
				}
				if(XKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalXKernelRelativePositionPositiveForLowestErrorMatch = totalXKernelRelativePositionPositiveForLowestErrorMatch+XKernelRelativePositionForLowestErrorMatch;
				}
				else
				{
					totalXKernelRelativePositionNegativeForLowestErrorMatch = totalXKernelRelativePositionNegativeForLowestErrorMatch+XKernelRelativePositionForLowestErrorMatch;
				}

				if(YKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalYKernelRelativePositionPositiveForLowestErrorMatch = totalYKernelRelativePositionPositiveForLowestErrorMatch+YKernelRelativePositionForLowestErrorMatch;
				}
				else
				{
					totalYKernelRelativePositionNegativeForLowestErrorMatch = totalYKernelRelativePositionNegativeForLowestErrorMatch+YKernelRelativePositionForLowestErrorMatch;
				}

				totalMatchError = totalMatchError+lowestMatchErrorAcrossKernel;
				matchErrorIndex++;
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif

		}
	}

	#ifdef OR_USE_OLD_BUGGY_BUT_WORKS_CODE
	double averageXKernelRelativePositionPositiveForLowestErrorMatch = totalXKernelRelativePositionPositiveForLowestErrorMatch/(imageWidth*imageHeight);
	double averageXKernelRelativePositionNegativeForLowestErrorMatch = totalXKernelRelativePositionNegativeForLowestErrorMatch/(imageWidth*imageHeight);
	double averageYKernelRelativePositionPositiveForLowestErrorMatch = totalYKernelRelativePositionPositiveForLowestErrorMatch/(imageWidth*imageHeight);
	double averageYKernelRelativePositionNegativeForLowestErrorMatch = totalYKernelRelativePositionNegativeForLowestErrorMatch/(imageWidth*imageHeight);
	#else
	double averageXKernelRelativePositionPositiveForLowestErrorMatch = totalXKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averageXKernelRelativePositionNegativeForLowestErrorMatch = totalXKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	double averageYKernelRelativePositionPositiveForLowestErrorMatch = totalYKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averageYKernelRelativePositionNegativeForLowestErrorMatch = totalYKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	#endif

	//cout << "averageXKernelRelativePositionPositiveForLowestErrorMatch = " << averageXKernelRelativePositionPositiveForLowestErrorMatch << endl;
	//cout << "averageXKernelRelativePositionNegativeForLowestErrorMatch = " << averageXKernelRelativePositionNegativeForLowestErrorMatch << endl;
	//cout << "averageYKernelRelativePositionPositiveForLowestErrorMatch = " << averageYKernelRelativePositionPositiveForLowestErrorMatch << endl;
	//cout << "averageYKernelRelativePositionNegativeForLowestErrorMatch = " << averageYKernelRelativePositionNegativeForLowestErrorMatch << endl;
	double averageXPositionalDeviation = (averageXKernelRelativePositionPositiveForLowestErrorMatch + -averageXKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	//cout << "averageXPositionalDeviation = " << averageXPositionalDeviation << endl;
	double averageYPositionalDeviation = (averageYKernelRelativePositionPositiveForLowestErrorMatch + -averageYKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	//cout << "averageYPositionalDeviation = " << averageYPositionalDeviation << endl;
	double averagePositionalDeviationNormalised = ((averageXPositionalDeviation+averageYPositionalDeviation)/IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING;
	//cout << "averagePositionalDeviationNormalised = " << averagePositionalDeviationNormalised << endl;
	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);
	//cout << "totalMatchErrorNormalised = " << totalMatchErrorNormalised << endl;
	#ifdef OR_USE_OLD_BUGGY_BUT_WORKS_CODE
	double error = averageMatchErrorNormalised + averagePositionalDeviationNormalised;
	#else
	double error = averageMatchErrorNormalised;
	#endif

	//cout << "totalMatchError = " << totalMatchError << endl;

	return error;
}
#endif


	/*
	double averageXDeviationNormalised = averageXPositionalDeviation/(totalXKernelRelativePositionPositiveForLowestErrorMatch-totalXKernelRelativePositionNegativeForLowestErrorMatch);
	cout << "averageXDeviationNormalised = " << averageXDeviationNormalised << endl;
	double averageYDeviationNormalised = averageYPositionalDeviation/(totalYKernelRelativePositionPositiveForLowestErrorMatch-totalYKernelRelativePositionNegativeForLowestErrorMatch);
	cout << "averageYDeviationNormalised = " << averageYDeviationNormalised << endl;
	double averagePositionalDeviationNormalised = ((averageXDeviationNormalised+averageYDeviationNormalised)/IMAGE_COMPARISON_MISFIT_AVG_KERNEL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_DEVIATION_WEIGHTING;

	*/


#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
		//NEW; 1sXy; do not accept images with no contrast - ie all same colour
bool checkImageHasContrast(int imageWidth, int imageHeight, unsigned char * rgbMap)
{
	bool result;

	double averager = 0.0;
	double averageg = 0.0;
	double averageb = 0.0;
	int index = 0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif
				averager = averager + testImagePixelColour.r;
				averageg = averageg + testImagePixelColour.g;
				averageb = averageb + testImagePixelColour.b;
				index++;
		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	if(index == 0)
	{
		//error; entire image is black
		result = false;
	}
	else
	{
		averager = averager/(index);
		averageg = averageg/(index);
		averageb = averageb/(index);

		double totalrdeviationGreaterThanThreshold = 0.0;
		double totalgdeviationGreaterThanThreshold = 0.0;
		double totalbdeviationGreaterThanThreshold = 0.0;
		double averagerdeviation = 0.0;
		double averagegdeviation = 0.0;
		double averagebdeviation = 0.0;

		for(int y = 0; y < imageHeight; y++)
		{
			for(int x = 0; x < imageWidth; x++)
			{
				colour testImagePixelColour;
				getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);

			#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
				if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
				{
					//pitch black - assume outside of t
				}
				else
				{
			#endif
					double rdeviation = absDouble(averager - testImagePixelColour.r);
					double gdeviation = absDouble(averageg - testImagePixelColour.g);
					double bdeviation = absDouble(averageb - testImagePixelColour.b);

					averagerdeviation = averagerdeviation + rdeviation;
					averagegdeviation = averagegdeviation + gdeviation;
					averagebdeviation = averagebdeviation + bdeviation;

					if(rdeviation > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
					{
						totalrdeviationGreaterThanThreshold = totalrdeviationGreaterThanThreshold + rdeviation;
					}
					if(gdeviation > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
					{
						totalgdeviationGreaterThanThreshold = totalgdeviationGreaterThanThreshold + gdeviation;
					}
					if(bdeviation > OR_IMAGE_COMPARISON_HAS_STARK_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
					{
						totalbdeviationGreaterThanThreshold = totalbdeviationGreaterThanThreshold + bdeviation;
					}
			#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
				}
			#endif
			}
		}
		averagerdeviation = averagerdeviation/(index);
		averagegdeviation = averagegdeviation/(index);
		averagebdeviation = averagebdeviation/(index);

		/*
		cout << "averagerdeviation = " << averagerdeviation << endl;
		cout << "averagegdeviation = " << averagegdeviation << endl;
		cout << "averagebdeviation = " << averagebdeviation << endl;
		cout << "totalrdeviationGreaterThanThreshold = " << totalrdeviationGreaterThanThreshold << endl;
		cout << "totalgdeviationGreaterThanThreshold = " << totalgdeviationGreaterThanThreshold << endl;
		cout << "totalbdeviationGreaterThanThreshold = " << totalbdeviationGreaterThanThreshold << endl;
		*/

		double averagedeviation = averagerdeviation + averagegdeviation + averagebdeviation;

		if(averagedeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION)
		//if((averagerdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagegdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagebdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION))
		{
			double totaldeviationGreaterThanThreshold = totalrdeviationGreaterThanThreshold + totalgdeviationGreaterThanThreshold + totalbdeviationGreaterThanThreshold;

			#ifdef OR_IMAGE_COMPARISON_USE_OLD_AVERAGE_CONTRAST_THRESHOLDING
			if(totaldeviationGreaterThanThreshold < OR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION*(imageWidth*imageHeight))
			#else
			if(totaldeviationGreaterThanThreshold/index < OR_IMAGE_COMPARISON_HAS_CONTRAST_MIN_TOTAL_THRESHOLDED_COL_DEVIATION)
			#endif
			{
				result = false;
			}
			else
			{
				result = true;
			}
		}
		else
		{
			result = true;
		}

	}

	return result;

}
#endif















#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	//this function is designed to be independant of lighting conditions
void calculateHueError(colour * testImagePixelColour, colour * trainImagePixelColour, double * hueError, double * nonHueError)
{
	int redError = testImagePixelColour->r - trainImagePixelColour->r;
	int greenError = testImagePixelColour->g - trainImagePixelColour->g;
	int blueError = testImagePixelColour->b - trainImagePixelColour->b;

	/*
	if((redError > 0) && (greenError > 0) && (blueError > 0))
	{
		//colour possible maintained, therefore possible lighting variation
	}
	else if((redError < 0) && (greenError < 0) && (blueError < 0))
	{
		//colour possible maintained, therefore possible lighting variation
	}
	else
	{
		//colour definately not maintained
	}
	*/

	#ifdef OR_USE_OLD_BUGGY_BUT_WORKS_CODE
	*hueError = (abs(redError - greenError) + abs(redError - blueError) + abs(greenError - blueError));
	#else
	*hueError = (abs(abs(redError) - abs(greenError)) + abs(abs(redError) - abs(blueError)) + abs(abs(greenError) - abs(blueError)));
	#endif

	//cout << "hueError = " << hueError << endl;

	*nonHueError = abs(testImagePixelColour->r - trainImagePixelColour->r) + abs(testImagePixelColour->g - trainImagePixelColour->g) + abs(testImagePixelColour->b - trainImagePixelColour->b);

	//cout << "nonHueError = " << nonHueError << endl;

}
#endif

#ifdef OR_IMAGE_COMPARISON_USE_BAD_CODE
	//this function is designed to be independant of lighting conditions
	//hue error is normalised based upon colour satuation, if low colour satuation then calculate error more on luminosity contrast rather than hue/colour contrast	[not yet tested]
double calculateHueErrorNormalisedBAD(colour * testImagePixelColour, colour * trainImagePixelColour, double * hueError, double * nonHueErrorNormalised)
{
	int redError = testImagePixelColour->r - trainImagePixelColour->r;
	int greenError = testImagePixelColour->g - trainImagePixelColour->g;
	int blueError = testImagePixelColour->b - trainImagePixelColour->b;

	/*
	if((redError > 0) && (greenError > 0) && (blueError > 0))
	{
		//colour possible maintained, therefore possible lighting variation
	}
	else if((redError < 0) && (greenError < 0) && (blueError < 0))
	{
		//colour possible maintained, therefore possible lighting variation
	}
	else
	{
		//colour definately not maintained
	}
	*/

	#ifdef OR_USE_OLD_BUGGY_BUT_WORKS_CODE
	*hueError = (abs(redError - greenError) + abs(redError - blueError) + abs(greenError - blueError));
	#else
	*hueError = (abs(abs(redError) - abs(greenError)) + abs(abs(redError) - abs(blueError)) + abs(abs(greenError) - abs(blueError)));
	#endif

	//cout << "hueError = " << hueError << endl;

	double colourSaturation = abs(testImagePixelColour->r - trainImagePixelColour->g) + abs(testImagePixelColour->r - trainImagePixelColour->b) + abs(testImagePixelColour->b - trainImagePixelColour->g);

	double nonHueError = abs(testImagePixelColour->r - trainImagePixelColour->r) + abs(testImagePixelColour->g - trainImagePixelColour->g) + abs(testImagePixelColour->b - trainImagePixelColour->b);

	if(colourSaturation < nonHueError)
	{
		*nonHueErrorNormalised = nonHueError/colourSaturation;
	}
	else
	{
		*nonHueErrorNormalised = 0.0;
	}

	//cout << "nonHueError = " << nonHueError << endl;
	return 0.0;
}


	//this function is designed to be independant of lighting conditions
	//hue error is normalised based upon colour satuation, if low colour satuation then calculate error more on luminosity contrast rather than hue/colour contrast	[not yet tested]
double compareRGBPixelsForMatchBAD(colour * testImagePixelColour, colour * trainImagePixelColour)
{
	/*OLD;
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	//cout << "nonHueError = " << nonHueError << endl;
	//cout << "hueError = " << hueError << endl;

	double pixelError =  (nonHueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR) + (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING;
	//cout << "pixelError = " << pixelError << endl;
	*/

	//new
	double hueError;
	double nonHueErrorNormalised;

	calculateHueErrorNormalisedBAD(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueErrorNormalised);

	double pixelError =  (nonHueErrorNormalised) + (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR);

	return pixelError;
}
#endif

#ifndef OR_USE_HEITGER_OBJECT_FEATURE_CALCULATION_CODE
	//this function is designed to be independant of lighting conditions
double compareRGBPixelsForMatchHueOnly(colour * testImagePixelColour, colour * trainImagePixelColour)
{
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	//cout << "nonHueError = " << nonHueError << endl;
	//cout << "hueError = " << hueError << endl;

	double pixelError =  (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING;
	//cout << "pixelError = " << pixelError << endl;



	return pixelError;
}

double compareRGBPixelsForMatchLumContrastOnly(colour * testImagePixelColour, colour * trainImagePixelColour)
{
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	//cout << "nonHueError = " << nonHueError << endl;
	//cout << "hueError = " << hueError << endl;

	double pixelError =  (nonHueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR);
	//cout << "pixelError = " << pixelError << endl;
	return 0.0;
}
#endif

/*
double compareRGBPixelsForMatchDirectional(colour * testImagePixelColour, colour * trainImagePixelColour)
{
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	//cout << "nonHueError = " << nonHueError << endl;
	//cout << "hueError = " << hueError << endl;

	double pixelError =  (nonHueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR) + (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING;
	//cout << "pixelError = " << pixelError << endl;

	double lumTest = calculateLuminosityLevelFromColour(testImagePixelColour);
	double lumTrain = calculateLuminosityLevelFromColour(trainImagePixelColour);

	return pixelError;
}
*/



#ifdef OR_IMAGE_COMPARISON_USE_BAD_CODE
void createColourHueContrastVecMapFromRGBMapNOTUSED(int imageWidth, int imageHeight, unsigned char * rgbMap, double * colourHueContrastVectorMap, int * colourHueContrastCategoryIntMap)
{
	//now lead a 3x3 kernel across image and calculate areas of high contrast
	for(int y = 0; y < (imageHeight-1); y++)
	{
		for(int x = 0; x < (imageWidth-1); x++)
		{
			vec colourHueContrastVecVal;
			int colourHueContrastCategoryIntVal;
			calculateColourHueContrastVecLevelWithinKernelNOTUSED(x, y, rgbMap, HUE_CONTRAST_VECTOR_MAP_GENERATION_KERNEL_WIDTH, HUE_CONTRAST_VECTOR_MAP_GENERATION_KERNEL_HEIGHT, imageWidth, imageHeight, &colourHueContrastVecVal, &colourHueContrastCategoryIntVal);
			setXYVectorMapValue(x, y, (imageWidth-1), &colourHueContrastVecVal, colourHueContrastVectorMap);
			setIntMapValue(x, y, (imageWidth-1), colourHueContrastCategoryIntVal, colourHueContrastCategoryIntMap);


		}
	}
}


void calculateColourHueContrastVecLevelWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, vec * colourHueContrastVecVal, int * colourHueContrastCategoryIntVal)
{
	colourHueContrastVecVal->x = 0.0;
	colourHueContrastVecVal->y = 0.0;

	/*
	colour pixelRGBColour[4];
	colour pixelRGBColourTemp[4];

	getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &(pixelRGBColour[0]));
	getRGBMapValues(pixelX+1, pixelY, imageWidth, rgbMap, &(pixelRGBColour[1]));
	getRGBMapValues(pixelX, pixelY+1, imageWidth, rgbMap, &(pixelRGBColour[2]));
	getRGBMapValues(pixelX+1, pixelY+1, imageWidth, rgbMap, &(pixelRGBColour[3]));

	for(int r=0; r<4; r++)
	{

		//now rotate for new mask
		copyColours(&(pixelRGBColourTemp[0]), &(pixelRGBColour[0]));
		copyColours(&(pixelRGBColourTemp[1]), &(pixelRGBColour[1]));
		copyColours(&(pixelRGBColourTemp[2]), &(pixelRGBColour[2]));
		copyColours(&(pixelRGBColourTemp[3]), &(pixelRGBColour[3]));

		copyColours(&(pixelRGBColour[0]), &(pixelRGBColourTemp[2]));
		copyColours(&(pixelRGBColour[1]), &(pixelRGBColourTemp[0]));
		copyColours(&(pixelRGBColour[2]), &(pixelRGBColourTemp[3]));
		copyColours(&(pixelRGBColour[3]), &(pixelRGBColourTemp[1]));

	}
	*/

	colour topLeftPixelPositionInRGBColour;		// pixelX, pixelY
	getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &topLeftPixelPositionInRGBColour);

	colour topRightPixelPositionInRGBColour;
	getRGBMapValues(pixelX+1, pixelY, imageWidth, rgbMap, &topRightPixelPositionInRGBColour);

	colour bottomLeftPixelPositionInRGBColour;
	getRGBMapValues(pixelX, pixelY+1, imageWidth, rgbMap, &bottomLeftPixelPositionInRGBColour);

	colour bottomRightPixelPositionInRGBColour;
	getRGBMapValues(pixelX+1, pixelY+1, imageWidth, rgbMap, &bottomRightPixelPositionInRGBColour);

	/*
	cout << "topLeftPixelPositionInRGBColour.r = " << (int)topLeftPixelPositionInRGBColour.r << endl;
	cout << "topLeftPixelPositionInRGBColour.g = " << (int)topLeftPixelPositionInRGBColour.g << endl;
	cout << "topLeftPixelPositionInRGBColour.b = " << (int)topLeftPixelPositionInRGBColour.b << endl;
	cout << "topRightPixelPositionInRGBColour.r = " << (int)topRightPixelPositionInRGBColour.r << endl;
	cout << "topRightPixelPositionInRGBColour.g = " << (int)topRightPixelPositionInRGBColour.g << endl;
	cout << "topRightPixelPositionInRGBColour.b = " << (int)topRightPixelPositionInRGBColour.b << endl;
	cout << "bottomLeftPixelPositionInRGBColour.r = " << (int)bottomLeftPixelPositionInRGBColour.r << endl;
	cout << "bottomLeftPixelPositionInRGBColour.g = " << (int)bottomLeftPixelPositionInRGBColour.g << endl;
	cout << "bottomLeftPixelPositionInRGBColour.b = " << (int)bottomLeftPixelPositionInRGBColour.b << endl;
	cout << "bottomRightPixelPositionInRGBColour.r = " << (int)bottomRightPixelPositionInRGBColour.r << endl;
	cout << "bottomRightPixelPositionInRGBColour.g = " << (int)bottomRightPixelPositionInRGBColour.g << endl;
	cout << "bottomRightPixelPositionInRGBColour.b = " << (int)bottomRightPixelPositionInRGBColour.b << endl;
	*/

	//x
	double xTop = compareRGBPixelsForMatchHueOnly(&topLeftPixelPositionInRGBColour, &topRightPixelPositionInRGBColour);
	bool xTopThreshold = false;
	if(xTop > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		xTopThreshold = true;
	}
	//cout << "xTop = " << xTop << endl;
	double xBottom = compareRGBPixelsForMatchHueOnly(&bottomLeftPixelPositionInRGBColour, &bottomRightPixelPositionInRGBColour);
	bool xBottomThreshold = false;
	if(xBottom > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		xBottomThreshold = true;
	}
	//cout << "xBottom = " << xBottom << endl;

	//y
	double yLeft = compareRGBPixelsForMatchHueOnly(&topLeftPixelPositionInRGBColour, &bottomLeftPixelPositionInRGBColour);
	bool yLeftThreshold = false;
	if(yLeft > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		yLeftThreshold = true;
	}
	//cout << "yLeft = " << yLeft << endl;

	double yRight = compareRGBPixelsForMatchHueOnly(&topRightPixelPositionInRGBColour, &bottomRightPixelPositionInRGBColour);
	bool yRightThreshold = false;
	if(yRight > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		yRightThreshold = true;
	}
	//cout << "yRight = " << yRight << endl;

	//diagonal
	double diagonalTopLeftToBottomRight = compareRGBPixelsForMatchHueOnly(&topLeftPixelPositionInRGBColour, &bottomRightPixelPositionInRGBColour);
	bool diagonalTopLeftToBottomRightThreshold = false;
	if(diagonalTopLeftToBottomRight > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		diagonalTopLeftToBottomRightThreshold = true;
	}
	//cout << "diagonalTopLeftToBottomRight = " << diagonalTopLeftToBottomRight << endl;

	double diagonalTopRightToBottomLeft = compareRGBPixelsForMatchHueOnly(&topRightPixelPositionInRGBColour, &bottomLeftPixelPositionInRGBColour);
	bool diagonalTopRightToBottomLeftThreshold = false;
	if(diagonalTopRightToBottomLeft > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		diagonalTopRightToBottomLeftThreshold = true;
	}
	//cout << "diagonalTopRightToBottomLeft = " << diagonalTopRightToBottomLeft << endl;

	if(!xTopThreshold && !xBottomThreshold && yLeftThreshold && yRightThreshold && diagonalTopLeftToBottomRightThreshold && diagonalTopRightToBottomLeftThreshold)
	{
		//cout << "x" << endl;
		// x x
		// - -
		colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_1;

		//colourHueContrastVecVal->x = 0.0;
		//colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;

	}
	else if(xTopThreshold && xBottomThreshold && !yLeftThreshold && !yRightThreshold && diagonalTopLeftToBottomRightThreshold && diagonalTopRightToBottomLeftThreshold)
	{
		//cout << "x" << endl;
		// x -
		// x -
		colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_2;

		//colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		//colourHueContrastVecVal->y = 0.0;

	}
	else if(!xTopThreshold && xBottomThreshold && yLeftThreshold && !yRightThreshold && !diagonalTopLeftToBottomRightThreshold && diagonalTopRightToBottomLeftThreshold)
	{
		//cout << "x" << endl;
		// - -
		// x -
		//negative
		colourHueContrastVecVal->x = -(xTop + xBottom - diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = -(yLeft + yRight - diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_3;

		//colourHueContrastVecVal->x = -(0.0 + xBottom + 0.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		//colourHueContrastVecVal->y = -(yLeft + 0.0 + 0.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
	}
	else if(!xTopThreshold && xBottomThreshold && !yLeftThreshold && yRightThreshold && diagonalTopLeftToBottomRightThreshold && !diagonalTopRightToBottomLeftThreshold)
	{
		//cout << "x" << endl;
		// - -
		// - x
		//positive
		colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 - diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 - diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_4;

		//colourHueContrastVecVal->x = (0.0 + xBottom + diagonalTopLeftToBottomRight/2.0 + 0.0)/3.0;
		//colourHueContrastVecVal->y = (0.0 + yRight + diagonalTopLeftToBottomRight/2.0 + 0.0)/3.0;
	}
	else if(xTopThreshold && !xBottomThreshold && !yLeftThreshold && yRightThreshold && !diagonalTopLeftToBottomRightThreshold && diagonalTopRightToBottomLeftThreshold)
	{
		//cout << "x" << endl;
		// - x
		// - -
		//negative
		colourHueContrastVecVal->x = -(xTop + xBottom - diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = -(yLeft + yRight - diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_5;

		//colourHueContrastVecVal->x = -(xTop + 0.0 + 0.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		//colourHueContrastVecVal->y = -(0.0 + yRight + 0.0 + diagonalTopRightToBottomLeft/2.0)/3.0;


	}
	else if(xTopThreshold && !xBottomThreshold && yLeftThreshold && !yRightThreshold && diagonalTopLeftToBottomRightThreshold && !diagonalTopRightToBottomLeftThreshold)
	{
		//cout << "x" << endl;
		// x -
		// - -
		//positive
		colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 - diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 - diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_6;

		//colourHueContrastVecVal->x = (xTop + 0.0 + diagonalTopLeftToBottomRight/2.0 + 0.0)/3.0;
		//colourHueContrastVecVal->y = (yLeft + 0.0 + diagonalTopLeftToBottomRight/2.0 + 0.0)/3.0;


	}
	else if(xTopThreshold && xBottomThreshold && yLeftThreshold && yRightThreshold)
	{
		//cout << "x" << endl;
		// x x
		// x x
		colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_7;

		//colourHueContrastVecVal->x = 0;
		//colourHueContrastVecVal->y = 0;


	}
	else if(!xTopThreshold && !xBottomThreshold && !yLeftThreshold && !yRightThreshold && !diagonalTopLeftToBottomRightThreshold && !diagonalTopRightToBottomLeftThreshold)
	{//no obvious corner
		//cout << "h1" << endl;
		// - -
		// - -
		colourHueContrastVecVal->x = (xTop + xBottom + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		colourHueContrastVecVal->y = (yLeft + yRight + diagonalTopLeftToBottomRight/2.0 + diagonalTopRightToBottomLeft/2.0)/3.0;
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_8;
		//colourHueContrastVecVal->x = 0;
		//colourHueContrastVecVal->y = 0;

	}
	else
	{
		*colourHueContrastCategoryIntVal = COLOUR_HUE_CONTRAST_CATEGORY_0;
	}




}




/*
void createColourHueContrastMapFromRGBMap(int imageWidth, int imageHeight, unsigned char * rgbMap, double * colourHueContrastMap)
{
	//now lead a 3x3 kernel across image and calculate areas of high contrast
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			double contrastVal = calculateColourHueContrastLevelWithinKernel(x, y, rgbMap, CONTRAST_MAP_GENERATION_KERNEL_WIDTH, CONTRAST_MAP_GENERATION_KERNEL_HEIGHT, width, height);
			setLumOrContrastOrDepthMapValue(x, y, width, contrastVal, colourHueContrastMap);
		}
	}
}



double calculateColourHueContrastLevelWithinKernel(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	double contrastLevel = 0.0;

	colour centrePixelPositionInRGBColour;
	getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &centrePixelPositionInRGBColour);

	for(int y = pixelY-(kernelHeight/2); y<= pixelY+(kernelHeight/2); y++)
	{
		for(int x = pixelX-(kernelWidth/2); x<= pixelX+(kernelWidth/2); x++)
		{
			if((y >= 0) && (y <imageHeight))
			{//check kernel pix is within image

				if((x >= 0) && (x <imageWidth))
				{//check kernel pix is within image

					if((x == pixelX) && (y == pixelY))
					{

					}
					else
					{
						//cout << "x = " << x << endl;
						//cout << "y = " << y << endl;

						//calc diff lum diff between centre pixel and current surrounding kernel pixel

						colour kernelCurrentPixelPositionInRGBColour;
						getRGBMapValues(x, y, imageWidth, rgbMap, &kernelCurrentPixelPositionInRGBColour);


						//double hueError;
						//double nonHueError;
						//calculateHueError(centrePixelPositionInRGBColour, kernelCurrentPixelPositionInRGBColour, &hueError, &nonHueError);
						//double currentContrastLevel = absDouble(hueError+nonHueError);

						double currentContrastLevel = compareRGBPixelsForMatch(centrePixelPositionInRGBColour, kernelCurrentPixelPositionInRGBColour);

						contrastLevel = maxDouble(contrastLevel, currentContrastLevel);


					}
				}
			}
		}
	}

	return contrastLevel;
}

*/


double calculateColourHueSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	double sumContrast = 0.0;

	colour centrePixelPositionInRGBColour;
	getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &centrePixelPositionInRGBColour);

	for(int y = pixelY-(kernelHeight/2); y<= pixelY+(kernelHeight/2); y++)
	{
		for(int x = pixelX-(kernelWidth/2); x<= pixelX+(kernelWidth/2); x++)
		{
			/*assumes pixelX, and pixelY are not on edge of map
			if((y >= 0) && (y <imageHeight))
			{//check kernel pix is within image

				if((x >= 0) && (x <imageWidth))
				{//check kernel pix is within image
			*/
					if((x == pixelX) && (y == pixelY))
					{

					}
					else
					{
						colour kernelCurrentPixelPositionInRGBColour;
						getRGBMapValues(x, y, imageWidth, rgbMap, &kernelCurrentPixelPositionInRGBColour);

						sumContrast = sumContrast + compareRGBPixelsForMatchHueOnly(&centrePixelPositionInRGBColour, &kernelCurrentPixelPositionInRGBColour);

					}
			/*
				}
			}
			*/
		}
	}

	return sumContrast;
}


double calculateColourHueRadialSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char * rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	double sumContrast = 0.0;

	//colour centrePixelPositionInRGBColour;
	//getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &centrePixelPositionInRGBColour);

	for(int y = pixelY-(kernelHeight/2); y<= pixelY+(kernelHeight/2); y++)
	{
		for(int x = pixelX-(kernelWidth/2); x<= pixelX+(kernelWidth/2); x++)
		{
			/*assumes pixelX, and pixelY are not on edge of map
			if((y >= 0) && (y <imageHeight))
			{//check kernel pix is within image

				if((x >= 0) && (x <imageWidth))
				{//check kernel pix is within image
			*/
					if((x == pixelX) && (y == pixelY))
					{

					}
					else
					{
						colour kernelCurrentPixelPositionInRGBColour;
						getRGBMapValues(x, y, imageWidth, rgbMap, &kernelCurrentPixelPositionInRGBColour);


						for(int sy = y-(RADIAL_CONTRAST_SUB_KERNEL_HEIGHT/2); sy<= y+(RADIAL_CONTRAST_SUB_KERNEL_HEIGHT/2); sy++)
						{
							for(int sx = x-(RADIAL_CONTRAST_SUB_KERNEL_WIDTH/2); sx<= x+(RADIAL_CONTRAST_SUB_KERNEL_WIDTH/2); sx++)
							{
								//ensures sx and xy are within kernel
								if((sy >= pixelY-(kernelHeight/2)) && (sy <pixelY+(kernelHeight/2)))
								{//check kernel pix is within image

									if((sx >= pixelX-(kernelWidth/2)) && (sx < pixelX+(kernelWidth/2)))
									{//check kernel pix is within image



										if((x == pixelX) && (y == pixelY))
										{

										}
										else
										{
											//cout <<"he1" << endl;

											colour subKernelCurrentPixelPositionInRGBColour;
											getRGBMapValues(sx, sy, imageWidth, rgbMap, &subKernelCurrentPixelPositionInRGBColour);

											sumContrast = sumContrast + compareRGBPixelsForMatchHueOnly(&subKernelCurrentPixelPositionInRGBColour, &kernelCurrentPixelPositionInRGBColour);

										}

									}
								}

							}
						}

					}
			/*assumes pixelX, and pixelY are not on edge of map
				}
			}
			*/
		}
	}

	return sumContrast;
}
#endif




/*highmembutfast;

				#ifdef  OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

					//cout << "as" << endl;

					#define OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN (1000)	//this is a major limitation for large object databases

					string trainObjectNameMostPopular;
					int trainObjectIndexMostPopular;
					int trainViewIndexMostPopular;
					int trainPolyIndexMostPopular;
					int trainSideIndexMostPopular;

					int trainObjectIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					int trainViewIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					int trainPolyIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					int trainSideIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					for(int ftest=0;ftest<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;ftest++)
					{
						for(int x=0;x<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X;x++)
						{
							for(int y=0;y<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y;y++)
							{
								for(int ftrain=0;ftrain<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN;ftrain++)
								{
									trainObjectIndexVote[ftest][x][y][ftrain] = 0;
									trainViewIndexVote[ftest][x][y][ftrain] = 0;
									trainPolyIndexVote[ftest][x][y][ftrain] = 0;
									trainSideIndexVote[ftest][x][y][ftrain] = 0;
								}
							}
						}
					}

					//cout << "as2" << endl;


					//Feature recordOfClosestMatchedFeature[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					string trainObjectNameClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					int trainViewIndexClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					int trainPolyIndexClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					int trainSideIndexClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					bool foundAFeatureMatch[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y][OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN];
					for(int ftest=0;ftest<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;ftest++)
					{
						for(int x=0;x<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X;x++)
						{
							for(int y=0;y<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y;y++)
							{
								for(int ftrain=0;ftrain<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN;ftrain++)
								{
									foundAFeatureMatch[ftest][x][y][ftrain] = false;
								}
							}
						}
					}

					//cout << "as3" << endl;

					int ftest = 0;
					int numberOfFeatureGeoMatches = 0;
					bool passedGeometricCheck = false;
					Feature * testcurrentFeatureInNearestFeatureList = &(testfeatureArray[testObjectIndex][testViewIndex][testPolyIndex][testSideIndex]);
					while(testcurrentFeatureInNearestFeatureList->next != NULL)
					{
						bool testpassedDimensionCheck = true;
						if(dimension == OR_METHOD2DOD_DIMENSIONS)
						{//NB do not compare transformed object triangle features if OR_METHOD2DOD_DIMENSIONS, as these will always be set to a predefined object triangle
							if(testcurrentFeatureInNearestFeatureList->OTpointIndex != 0)
							{
								testpassedDimensionCheck = false;
							}
						}

						if(testpassedDimensionCheck)
						{
							//cout << "h1" << endl;

							int xBin = determineGeoBinX(testcurrentFeatureInNearestFeatureList->pointTransformed.x);
							int yBin = determineGeoBinY(testcurrentFeatureInNearestFeatureList->pointTransformed.y);



							//check current bin and nearest bins
							int xBinIndex = 0;
							for(int x = xBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=xBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
							{

								int yBinIndex =0;
								for(int y = yBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=yBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
								{

									bool insideBin = false;


									if(x >= 0 )
									{
										if(x < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
										{
											if(y >= 0 )
											{
												if(y < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
												{
													insideBin = true;
												}
											}
										}
									}

									if(insideBin)
									{
										//cout << "h2" << endl;
										//cout << "x = " << x << endl;
										//cout << "y = " << y << endl;

										Feature * firstFeatureInTestFeatureMatchingTrainBin =  &(trainfeatureBinArray[x][y]);
										Feature * currentFeatureInTestFeatureMatchingTrainBin = firstFeatureInTestFeatureMatchingTrainBin;
										int ftrain = 0;

										while(currentFeatureInTestFeatureMatchingTrainBin->next != NULL)
										{
											//cout << "h3" << endl;

											double currentDistanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(currentFeatureInTestFeatureMatchingTrainBin->pointTransformed));
											if(currentDistanceBetweenTwoPoints < OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
											{
												if(ftrain < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN)
												{
													//cout << "ftest = " << ftest << endl;
													//cout << "xBinIndex = " << xBinIndex << endl;
													//cout << "yBinIndex = " << yBinIndex << endl;
													//cout << "ftrain = " << ftrain << endl;

													foundAFeatureMatch[ftest][xBinIndex][yBinIndex][ftrain] = true;
													trainObjectNameClosest[ftest][xBinIndex][yBinIndex][ftrain] = currentFeatureInTestFeatureMatchingTrainBin->objectName;
													trainViewIndexClosest[ftest][xBinIndex][yBinIndex][ftrain] = currentFeatureInTestFeatureMatchingTrainBin->viewIndex;
													trainPolyIndexClosest[ftest][xBinIndex][yBinIndex][ftrain] = currentFeatureInTestFeatureMatchingTrainBin->polyIndex;
													trainSideIndexClosest[ftest][xBinIndex][yBinIndex][ftrain] = currentFeatureInTestFeatureMatchingTrainBin->sideIndex;
													//recordOfClosestMatchedFeature[ftest][xBinIndex][yBinIndex][ftrain] =  currentFeatureInTestFeatureMatchingTrainBin;	//not needed
													//cout << "found an item - good" << endl;
												}
											}

											ftrain++;

											currentFeatureInTestFeatureMatchingTrainBin = currentFeatureInTestFeatureMatchingTrainBin->next;

										}

									}
									yBinIndex++;

								}
								xBinIndex++;
							}

							ftest++;

						}

						testcurrentFeatureInNearestFeatureList = testcurrentFeatureInNearestFeatureList->next;
					}

					//cout << "as4" << endl;

					for(int ftest=0;ftest<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;ftest++)
					{
						for(int x=0;x<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X;x++)
						{
							for(int y=0;y<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y;y++)
							{
								for(int ftrain=0;ftrain<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN;ftrain++)
								{
									if(foundAFeatureMatch[ftest][x][y][ftrain])
									{
										for(int ftest2=0;ftest2<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;ftest2++)
										{
											for(int x2=0;x2<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X;x2++)
											{
												for(int y2=0;y2<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y;y2++)
												{
													for(int ftrain2=0;ftrain2<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN;ftrain2++)
													{
														if(foundAFeatureMatch[ftest2][x2][y2][ftrain2])
														{
															if(trainObjectNameClosest[ftest2][x2][y2][ftrain2] == trainObjectNameClosest[ftest][x][y][ftrain])
															{
																trainObjectIndexVote[ftest][x][y][ftrain] = trainObjectIndexVote[ftest][x][y][ftrain] + 1;
															}
															if(trainViewIndexClosest[ftest2][x2][y2][ftrain2] == trainViewIndexClosest[ftest][x][y][ftrain])
															{
																trainViewIndexVote[ftest][x][y][ftrain] = trainViewIndexVote[ftest][x][y][ftrain] + 1;
															}
															if(trainPolyIndexClosest[ftest2][x2][y2][ftrain2] == trainPolyIndexClosest[ftest][x][y][ftrain])
															{
																trainPolyIndexVote[ftest][x][y][ftrain] = trainPolyIndexVote[ftest][x][y][ftrain] + 1;
															}
															if(trainSideIndexClosest[ftest2][x2][y2][ftrain2] == trainSideIndexClosest[ftest][x][y][ftrain])
															{
																trainSideIndexVote[ftest][x][y][ftrain] = trainSideIndexVote[ftest][x][y][ftrain] + 1;
															}
														}

													}
												}
											}
										}
									}

								}
							}
						}
					}



					//cout << "h5" << endl;

					int trainObjectIndexMaxNumVotes = 0;
					int trainViewIndexMaxNumVotes = 0;
					int trainPolyIndexMaxNumVotes = 0;
					int trainSideIndexMaxNumVotes = 0;

					for(int ftest=0;ftest<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;ftest++)
					{
						for(int x=0;x<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X;x++)
						{
							for(int y=0;y<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y;y++)
							{
								for(int ftrain=0;ftrain<OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_MAXIMUM_NUMBER_OF_F_PER_BIN;ftrain++)
								{
									if(trainObjectIndexVote[ftest][x][y][ftrain] > trainObjectIndexMaxNumVotes)
									{
										trainObjectIndexMaxNumVotes = trainObjectIndexVote[ftest][x][y][ftrain];
										trainObjectNameMostPopular = trainObjectNameClosest[ftest][x][y][ftrain];
									}
									if(trainViewIndexVote[ftest][x][y][ftrain] > trainViewIndexMaxNumVotes)
									{
										trainViewIndexMaxNumVotes = trainViewIndexVote[ftest][x][y][ftrain];
										trainViewIndexMostPopular = trainViewIndexClosest[ftest][x][y][ftrain];
									}
									if(trainPolyIndexVote[ftest][x][y][ftrain] > trainPolyIndexMaxNumVotes)
									{
										trainPolyIndexMaxNumVotes = trainPolyIndexVote[ftest][x][y][ftrain];
										trainPolyIndexMostPopular = trainPolyIndexClosest[ftest][x][y][ftrain];
									}
									if(trainSideIndexVote[ftest][x][y][ftrain] > trainSideIndexMaxNumVotes)
									{
										trainSideIndexMaxNumVotes = trainSideIndexVote[ftest][x][y][ftrain];
										trainSideIndexMostPopular = trainSideIndexClosest[ftest][x][y][ftrain];
									}
								}
							}
						}
					}


					//cout << "h6" << endl;




					if((trainObjectIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES) && (trainViewIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES) && (trainPolyIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES) && (trainSideIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES))
					{
						passedGeometricCheck = true;
					}

					string trainObjectString = trainObjectNameMostPopular;
					int trainViewIndex = trainViewIndexMostPopular;
					int trainPolyIndex = trainPolyIndexMostPopular;
					int trainSideIndex = trainSideIndexMostPopular;


					cout << "trainObjectIndexMaxNumVotes = " << trainObjectIndexMaxNumVotes << endl;
					cout << "trainViewIndexMaxNumVotes = " << trainViewIndexMaxNumVotes << endl;
					cout << "trainPolyIndexMaxNumVotes = " << trainPolyIndexMaxNumVotes << endl;
					cout << "trainSideIndexMaxNumVotes = " << trainSideIndexMaxNumVotes << endl;
					cout << "trainObjectString = " << trainObjectString << endl;
					cout << "trainViewIndex = " << trainViewIndex << endl;
					cout << "trainPolyIndex = " << trainPolyIndex << endl;
					cout << "trainSideIndex = " << trainSideIndex << endl;


					char trainpolygonIndexString[10];
					char trainsideIndexString[10];
					char trainviewIndexString[10];

					sprintf(trainpolygonIndexString, "%d", trainPolyIndex);
					sprintf(trainsideIndexString, "%d", trainSideIndex);
					sprintf(trainviewIndexString, "%d", trainViewIndex);

					string traininterpolatedRGBMapFileNameForRayTracing;
					traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

					//cout << "h7" << endl;

				#else



*/

/*OLD;
				#ifdef  OR_IMAGE_COMPARISON_NONSQL_GEOMETRIC_COMPARISON_OPTIMISED_TRAIN_FILE_IO_V2

					string trainObjectNameMostPopular;
					int trainObjectIndexMostPopular;
					int trainViewIndexMostPopular;
					int trainPolyIndexMostPopular;
					int trainSideIndexMostPopular;

					int trainObjectIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					int trainViewIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					int trainPolyIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					int trainSideIndexVote[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					for(int i=0;i<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;i++)
					{
						trainObjectIndexVote[i] = 0;
						trainViewIndexVote[i] = 0;
						trainPolyIndexVote[i] = 0;
						trainSideIndexVote[i] = 0;
					}

					string trainObjectNameClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					int trainViewIndexClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					int trainPolyIndexClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					int trainSideIndexClosest[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					bool foundAFeatureMatch[OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE];
					for(int i=0;i<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;i++)
					{
						foundAFeatureMatch[i] = false;
					}

					int featureIndexForComparison = 0;
					int numberOfFeatureGeoMatches = 0;
					bool passedGeometricCheck = false;
					Feature * testcurrentFeatureInNearestFeatureList = &(testfeatureArray[testObjectIndex][testViewIndex][testPolyIndex][testSideIndex]);
					while(testcurrentFeatureInNearestFeatureList->next != NULL)
					{
						bool testpassedDimensionCheck = true;
						if(dimension == OR_METHOD2DOD_DIMENSIONS)
						{//NB do not compare transformed object triangle features if OR_METHOD2DOD_DIMENSIONS, as these will always be set to a predefined object triangle
							if(testcurrentFeatureInNearestFeatureList->OTpointIndex != 0)
							{
								testpassedDimensionCheck = false;
							}
						}

						if(testpassedDimensionCheck)
						{
							//cout << "h1" << endl;

							int xBin = (testcurrentFeatureInNearestFeatureList->pointTransformed.x / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_X_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS/2);
							int yBin = (testcurrentFeatureInNearestFeatureList->pointTransformed.y / OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_Y_BIN_SEPARATION) + (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS/2);

							double closestDistanceBetweenTwoPoints = REALLY_FAR_AWAY;
							Feature * recordOfClosestMatchedFeature;

							//check current bin and nearest bins
							for(int x = xBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=xBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
							{
								for(int y = yBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=yBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
								{
									bool insideBin = false;


									if(x >= 0 )
									{
										if(x < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS)
										{
											if(y >= 0 )
											{
												if(y < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS)
												{
													insideBin = true;
												}
											}
										}
									}

									if(insideBin)
									{
										//cout << "h2" << endl;
										//cout << "x = " << x << endl;
										//cout << "y = " << y << endl;

										Feature * firstFeatureInTestFeatureMatchingTrainBin =  &(trainfeatureBinArray[x][y]);
										Feature * currentFeatureInTestFeatureMatchingTrainBin = firstFeatureInTestFeatureMatchingTrainBin;

										while(currentFeatureInTestFeatureMatchingTrainBin->next != NULL)
										{
											//cout << "h3" << endl;

											double currentDistanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&(testcurrentFeatureInNearestFeatureList->pointTransformed), &(currentFeatureInTestFeatureMatchingTrainBin->pointTransformed));
											if(currentDistanceBetweenTwoPoints < closestDistanceBetweenTwoPoints)
											{
												closestDistanceBetweenTwoPoints = currentDistanceBetweenTwoPoints;
												recordOfClosestMatchedFeature =  currentFeatureInTestFeatureMatchingTrainBin;
												//cout << "found an item - good" << endl;
											}

											currentFeatureInTestFeatureMatchingTrainBin = currentFeatureInTestFeatureMatchingTrainBin->next;

										}

									}

								}
							}

							if(closestDistanceBetweenTwoPoints < OR_GEOMETRIC_CHECK_COMPARISON_MAX_ERROR)
							{
								//cout << "found a matched item - good" << endl;

								foundAFeatureMatch[featureIndexForComparison] = true;
								trainObjectNameClosest[featureIndexForComparison] = testcurrentFeatureInNearestFeatureList->objectName;
								trainViewIndexClosest[featureIndexForComparison] = testcurrentFeatureInNearestFeatureList->viewIndex;
								trainPolyIndexClosest[featureIndexForComparison] = testcurrentFeatureInNearestFeatureList->polyIndex;
								trainSideIndexClosest[featureIndexForComparison] = testcurrentFeatureInNearestFeatureList->sideIndex;
							}

							featureIndexForComparison++;

						}

						testcurrentFeatureInNearestFeatureList = testcurrentFeatureInNearestFeatureList->next;
					}

					for(int i=0;i<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;i++)
					{
						if(foundAFeatureMatch[i])
						{
							for(int j=0;j<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;j++)
							{
								if(foundAFeatureMatch[j])
								{
									if(trainObjectNameClosest[j] == trainObjectNameClosest[i])
									{
										trainObjectIndexVote[i] = trainObjectIndexVote[i] + 1;
									}
									if(trainViewIndexClosest[j] == trainViewIndexClosest[i])
									{
										trainViewIndexVote[i] = trainViewIndexVote[i] + 1;
									}
									if(trainPolyIndexClosest[j] == trainPolyIndexClosest[i])
									{
										trainPolyIndexVote[i] = trainPolyIndexVote[i] + 1;
									}
									if(trainSideIndexClosest[j] == trainSideIndexClosest[i])
									{
										trainSideIndexVote[i] = trainSideIndexVote[i] + 1;
									}
								}
							}
						}
					}

					//cout << "h5" << endl;

					int trainObjectIndexMaxNumVotes = 0;
					int trainViewIndexMaxNumVotes = 0;
					int trainPolyIndexMaxNumVotes = 0;
					int trainSideIndexMaxNumVotes = 0;

					for(int i=0;i<OR_METHOD_NUM_NEARBY_FEATURES_TO_COMPARE;i++)
					{
						if(trainObjectIndexVote[i] > trainObjectIndexMaxNumVotes)
						{
							trainObjectIndexMaxNumVotes = trainObjectIndexVote[i];
							trainObjectNameMostPopular = trainObjectNameClosest[i];
						}
						if(trainViewIndexVote[i] > trainViewIndexMaxNumVotes)
						{
							trainViewIndexMaxNumVotes = trainViewIndexVote[i];
							trainViewIndexMostPopular = trainViewIndexClosest[i];
						}
						if(trainPolyIndexVote[i] > trainPolyIndexMaxNumVotes)
						{
							trainPolyIndexMaxNumVotes = trainPolyIndexVote[i];
							trainPolyIndexMostPopular = trainPolyIndexClosest[i];
						}
						if(trainSideIndexVote[i] > trainSideIndexMaxNumVotes)
						{
							trainSideIndexMaxNumVotes = trainSideIndexVote[i];
							trainSideIndexMostPopular = trainSideIndexClosest[i];
						}
					}

					//cout << "h6" << endl;




					if((trainObjectIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES) && (trainViewIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES) && (trainPolyIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES) && (trainSideIndexMaxNumVotes >= OR_GEOMETRIC_CHECK_MIN_NUMBER_PASSES))
					{
						passedGeometricCheck = true;
					}

					string trainObjectString = trainObjectNameMostPopular;
					int trainViewIndex = trainViewIndexMostPopular;
					int trainPolyIndex = trainPolyIndexMostPopular;
					int trainSideIndex = trainSideIndexMostPopular;


					cout << "trainObjectIndexMaxNumVotes = " << trainObjectIndexMaxNumVotes << endl;
					cout << "trainViewIndexMaxNumVotes = " << trainViewIndexMaxNumVotes << endl;
					cout << "trainPolyIndexMaxNumVotes = " << trainPolyIndexMaxNumVotes << endl;
					cout << "trainSideIndexMaxNumVotes = " << trainSideIndexMaxNumVotes << endl;
					cout << "trainObjectString = " << trainObjectString << endl;
					cout << "trainViewIndex = " << trainViewIndex << endl;
					cout << "trainPolyIndex = " << trainPolyIndex << endl;
					cout << "trainSideIndex = " << trainSideIndex << endl;


					char trainpolygonIndexString[10];
					char trainsideIndexString[10];
					char trainviewIndexString[10];

					sprintf(trainpolygonIndexString, "%d", trainPolyIndex);
					sprintf(trainsideIndexString, "%d", trainSideIndex);
					sprintf(trainviewIndexString, "%d", trainViewIndex);

					string traininterpolatedRGBMapFileNameForRayTracing;
					traininterpolatedRGBMapFileNameForRayTracing = trainObjectString + "interpolatedMesh" + "ViewIndex" + trainviewIndexString + "FacingPoly" + trainpolygonIndexString + "side" + trainsideIndexString;

					//cout << "h7" << endl;

				#else
*/






