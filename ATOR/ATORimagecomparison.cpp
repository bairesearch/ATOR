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
 * File Name: ATORimagecomparison.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3m11a 10-January-2018
 *
 *******************************************************************************/


#include "ATORimagecomparison.hpp"
//#include "ATORmethod.hpp"

void ORimagecomparisonClass::disablePixelsThatAreNotContainedInTheObjectTriangle2DOD(const int imageWidth, const int imageHeight, unsigned char* rgbMap)
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
				RTpixelMaps.setRGBMapValues(x, y, imageWidth, &disabledColour, rgbMap);
			}
		}
	}


}


void ORimagecomparisonClass::applyGaussianKernelToRGBmap(int imageWidth, const int imageHeight, const double sigma, const unsigned char* rgbMap, unsigned char* rgbMapWithGaussianApplied)
{
	applyGaussianKernelToLuminosityMapOrRGBmapComponent(imageWidth, imageHeight, sigma, NULL, NULL, rgbMap, rgbMapWithGaussianApplied, true, RGB_RED);
	applyGaussianKernelToLuminosityMapOrRGBmapComponent(imageWidth, imageHeight, sigma, NULL, NULL, rgbMap, rgbMapWithGaussianApplied, true, RGB_GREEN);
	applyGaussianKernelToLuminosityMapOrRGBmapComponent(imageWidth, imageHeight, sigma, NULL, NULL, rgbMap, rgbMapWithGaussianApplied, true, RGB_BLUE);
}

void ORimagecomparisonClass::applyGaussianKernelToLuminosityMapOrRGBmapComponent(int imageWidth, const int imageHeight, const double sigma, const double* luminosityMap, double* luminosityMapWithGaussianApplied, const unsigned char* rgbMap, unsigned char* rgbMapWithGaussianApplied, const bool useRGBmapComponent, const int rgbMapComponentNumber)
{
	double s;
	int support,i, j, k, kk, x, y;

	support = (int) (3.0*sigma);              /* set support size for gaussian*/

	double* mask = new double[support +1];
	double* buf2 = new double[imageWidth*imageHeight];
	double* buf = new double[imageWidth*imageHeight];

	for(y = 0; y< imageHeight; y++)
	{
		for(x = 0; x< imageHeight; x++)
		{
			if(useRGBmapComponent)
			{
				buf[y*imageWidth + x] = double(RTpixelMaps.getRGBMapValue(x, y, imageWidth, rgbMapComponentNumber, rgbMap));
			}
			else
			{
				buf[y*imageWidth + x] = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityMap);
			}
		}
	}

	for(i=0; i <= support; i++)								/* one-sided gaussian mask*/
	{
		mask[i]=1.0F/((double)sqrt(2.0F*(double)PI)*(sigma)*(sigma))*(double)exp((-0.5F*((double)i / sigma)*((double)i / sigma)));
	}

	for(j = 0; j < imageWidth*imageHeight; j+=imageWidth)
	{           /* filter all lines*/
		s = mask[0];
		for(i = j+support; i < j+imageWidth-support; i++)		/* initialize vector with*/
		buf2[i] = buf[i]* s;									/* central weight*/

		for(k=1; k<=support; k++)
		{/* sum up remain. weighted*/
			s = mask[k];										/* vectors*/
			for(i=j+support; i < j+imageWidth-support; i++)
			buf2[i] += (buf[i+k] + buf[i-k])* s;
		}
	}

	for(j = support*imageWidth; j < (imageHeight-support)*imageWidth; j+=imageWidth)
	{ /* filter all columns*/
		s = mask[0];
		for(i = j+support; i < j+imageWidth-support; i++)		/* initialize vector*/
		buf[i] = buf2[i]* s;                       			/* central weight*/

		for(k=1, kk=imageWidth; kk<=support*imageWidth; kk+=imageWidth, k++)
		{ /* sum up remaining */
			s = mask[k];                               			/* weighted vectors*/

			for(i=j+support; i < j+imageWidth-support; i++)
			buf[i] += (buf2[i+kk] + buf2[i-kk])* s;
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
				RTpixelMaps.setRGBMapValue(x, y, imageWidth, rgbMapComponentNumber, rgbMapWithGaussianApplied, (unsigned char)(buf[y*imageWidth + x]));
			}
			else
			{
				RTpixelMaps.setLumOrContrastOrDepthMapValue(x, y, imageWidth, buf[y*imageWidth + x], luminosityMapWithGaussianApplied);
			}
		}
	}

	delete mask;
	delete buf2;
	delete buf;
}


void ORimagecomparisonClass::createNormalisedHueContrastMapUsingRGBmapAndCalculateAllContrastValues(int imageWidth, int imageHeight, unsigned char* rgbMap, double* normalisedHueContrastMap, double* normalisedAverageHueContrastR, double* normalisedAverageHueContrastG, double* normalisedAverageHueContrastB, double* averageContrastWithRespectToAverageColour, double* averageStarkContrastWithRespectToAverageColour, double* averageLocalContrast, double* averageLocalStarkContrast)
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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);

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


				totalR = totalR + testImagePixelColour.r;
				totalG = totalG + testImagePixelColour.g;
				totalB = totalB + testImagePixelColour.b;
				index++;

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			}
		#endif
		}
	}

	double averageRminusG = double(totalRminusG)/double(index);
	double averageRminusB = double(totalRminusB)/double(index);
	double averageGminusB = double(totalGminusB)/double(index);


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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif
				#ifdef OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_BINNING
				double rContrastWithRespectToAverage = SHAREDvars.absDouble(averager - int(testImagePixelColour.r));
				double gContrastWithRespectToAverage = SHAREDvars.absDouble(averageg - int(testImagePixelColour.g));
				double bContrastWithRespectToAverage = SHAREDvars.absDouble(averageb - int(testImagePixelColour.b));
				#else	//use hue RGB dev binning
				int RminusG = (int(testImagePixelColour.r) - int(testImagePixelColour.g));
				int RminusB = (int(testImagePixelColour.r) - int(testImagePixelColour.b));
				int GminusB = (int(testImagePixelColour.g) - int(testImagePixelColour.b));

				double rContrastWithRespectToAverage = SHAREDvars.absDouble(averageRminusG - RminusG);
				double gContrastWithRespectToAverage = SHAREDvars.absDouble(averageRminusB - RminusB);
				double bContrastWithRespectToAverage = SHAREDvars.absDouble(averageGminusB - GminusB);
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
									RTpixelMaps.getRGBMapValues(kx, ky, imageWidth, rgbMap, &testImagePixelColourKernel);

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

				/*OLD Bad;
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

				RTpixelMaps.setVectorMapValue(x, y, imageWidth, &normalisedAverageHueContrastAcrossKernel, normalisedHueContrastMap);

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

	double averageRHueContrast = double(totalRHueContrast)/(double(index*IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT));
	double averageGHueContrast = double(totalGHueContrast)/(double(index*IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT));
	double averageBHueContrast = double(totalBHueContrast)/(double(index*IMAGE_COMPARISON_KERNEL_WIDTH*IMAGE_COMPARISON_KERNEL_HEIGHT));



	/*OLD; normalised wrt to average luminosity not saturation
	*normalisedAverageHueContrastR = (averageRHueContrast/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
	*normalisedAverageHueContrastG = (averageGHueContrast/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
	*normalisedAverageHueContrastB = (averageBHueContrast/(double(totalR+totalG+totalB)/3.0))*double(index)/double(MAX_RGB_VAL);
	*/

	*normalisedAverageHueContrastR = (averageRHueContrast/(double(averageRHueContrast+averageGHueContrast+averageBHueContrast)/3.0));	///double(MAX_RGB_VAL)
	*normalisedAverageHueContrastG = (averageGHueContrast/(double(averageRHueContrast+averageGHueContrast+averageBHueContrast)/3.0));
	*normalisedAverageHueContrastB = (averageBHueContrast/(double(averageRHueContrast+averageGHueContrast+averageBHueContrast)/3.0));

	/*OLD Bad;
	if(averageRminusG != 0)
	{
		*normalisedAverageHueContrastR = (averageRHueContrast/(double(averageRminusG)))/double(MAX_RGB_VAL);
	}
	if(averageRminusB != 0)
	{
		*normalisedAverageHueContrastG = (averageGHueContrast/(double(averageRminusB)))/double(MAX_RGB_VAL);
	}
	if(averageGminusB != 0)
	{
		*normalisedAverageHueContrastB = (averageBHueContrast/(double(averageGminusB)))/double(MAX_RGB_VAL);
	}
	*/

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

}

bool ORimagecomparisonClass::checkImageHasContrastValuesOnly(const double averageContrastWithRespectToAverageColour, const double averageStarkContrastWithRespectToAverageColour, const double averageLocalContrast, const double averageLocalStarkContrast)
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








//compareImagesRGBwithPosDevAndLocalStarkContAndHueDev or compareImagesRGBWithPosDevAndLocalStarkContAndLumDev [assumes light source is always white]

void ORimagecomparisonClass::calculateAverageColour(int imageWidth, int imageHeight, unsigned char* rgbMap, colour* avgCol)
{
	double totalColr = 0.0;
	double totalColg = 0.0;
	double totalColb = 0.0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);
			totalColr = totalColr + testImagePixelColour.r;
			totalColg = totalColg + testImagePixelColour.g;
			totalColb = totalColb + testImagePixelColour.b;
		}
	}

	avgCol->r = totalColr/(imageHeight*imageWidth);
	avgCol->g = totalColg/(imageHeight*imageWidth);
	avgCol->b = totalColb/(imageHeight*imageWidth);
}


double ORimagecomparisonClass::compareImagesRGBwithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain, double* averageHueDeviationR, double* averageHueDeviationG, double* averageHueDeviationB, double* averageXkernelRelativePositionForLowestErrorMatch, double* averageYkernelRelativePositionForLowestErrorMatch)
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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTest, &testImagePixelColour);

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTrain, &trainImagePixelColourCentre);
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
								RTpixelMaps.getRGBMapValues(kx, ky, imageWidth, rgbMapTrain, &trainImagePixelColour);

							#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBpixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
							#else
								//double pixelError = compareRGBpixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);

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
				RTpixelMaps.getRGBMapValues(XKernelRelativePositionForLowestErrorMatch+x, YKernelRelativePositionForLowestErrorMatch+y, imageWidth, rgbMapTrain, &trainImagePixelColourOptimum);

				double rdeviationOptimum = SHAREDvars.absDouble(redErrorlowestMatchErrorAcrossKernel);
				double gdeviationOptimum = SHAREDvars.absDouble(greenErrorlowestMatchErrorAcrossKernel);
				double bdeviationOptimum = SHAREDvars.absDouble(blueErrorlowestMatchErrorAcrossKernel);

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

	*averageHueDeviationR = totalRHueDeviation/(matchErrorIndex);
	*averageHueDeviationG = totalGHueDeviation/(matchErrorIndex);
	*averageHueDeviationB = totalBHueDeviation/(matchErrorIndex);

	*averageXkernelRelativePositionForLowestErrorMatch = totalXKernelRelativePositionForLowestErrorMatch/(matchErrorIndex);
	*averageYkernelRelativePositionForLowestErrorMatch = totalYKernelRelativePositionForLowestErrorMatch/(matchErrorIndex);

	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);
	double error = averageMatchErrorNormalised;


	if(starkLocalDeviationIndex > OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS*(imageWidth*imageHeight))
	{
		error = 8888.0;
	}

	return error;
}




double ORimagecomparisonClass::compareImagesRGBwithPosDevAndLocalStarkContAndHueDev(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain)
{
	double totalStarkLocalDeviation = 0.0;
	int starkLocalDeviationIndex = 0;

	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	double averageXkernelRelativePositionForLowestErrorMatch = 0.0;
	double averageYkernelRelativePositionForLowestErrorMatch = 0.0;

	double averageHueDeviationR = 0.0;
	double averageHueDeviationG = 0.0;
	double averageHueDeviationB = 0.0;
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


	compareImagesRGBwithPosDevAndLocalStarkContAndHueDevCalculateAveragesOnly(imageWidth, imageHeight, rgbMapTest, rgbMapTrain, &averageHueDeviationR, &averageHueDeviationG, &averageHueDeviationB, &averageXkernelRelativePositionForLowestErrorMatch, &averageYkernelRelativePositionForLowestErrorMatch);

	averageLumDeviation = averageHueDeviationR + averageHueDeviationG + averageHueDeviationB;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTest, &testImagePixelColour);

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTrain, &trainImagePixelColourCentre);
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
								RTpixelMaps.getRGBMapValues(kx, ky, imageWidth, rgbMapTrain, &trainImagePixelColour);

							#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBpixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
							#else
								//double pixelError = compareRGBpixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);

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

				totalDifferenceFromAverageRHueDeviation = totalDifferenceFromAverageRHueDeviation + SHAREDvars.absDouble(averageHueDeviationR - redErrorlowestMatchErrorAcrossKernel);
				totalDifferenceFromAverageGHueDeviation = totalDifferenceFromAverageGHueDeviation + SHAREDvars.absDouble(averageHueDeviationG - greenErrorlowestMatchErrorAcrossKernel);
				totalDifferenceFromAverageBHueDeviation = totalDifferenceFromAverageBHueDeviation + SHAREDvars.absDouble(averageHueDeviationB - blueErrorlowestMatchErrorAcrossKernel);
				totalDifferenceFromAverageLumDeviation = totalDifferenceFromAverageLumDeviation + SHAREDvars.absDouble(averageLumDeviation - (redErrorlowestMatchErrorAcrossKernel+blueErrorlowestMatchErrorAcrossKernel+greenErrorlowestMatchErrorAcrossKernel));
				hueDeviationIndex++;

					//NEW; additional; check for stark differences

				colour trainImagePixelColourOptimum;
				RTpixelMaps.getRGBMapValues(XKernelRelativePositionForLowestErrorMatch+x, YKernelRelativePositionForLowestErrorMatch+y, imageWidth, rgbMapTrain, &trainImagePixelColourOptimum);

				double rdeviationOptimum = SHAREDvars.absDouble(redErrorlowestMatchErrorAcrossKernel);
				double gdeviationOptimum = SHAREDvars.absDouble(greenErrorlowestMatchErrorAcrossKernel);
				double bdeviationOptimum = SHAREDvars.absDouble(blueErrorlowestMatchErrorAcrossKernel);

				double deviationOptimum = rdeviationOptimum + gdeviationOptimum + bdeviationOptimum;
				if(deviationOptimum > OR_IMAGE_COMPARISON_STARK_LOCAL_DEVIATION_MIN_MIN_COL_DIFF)
				//if((averagerdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagegdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION) && (averagebdeviation < OR_IMAGE_COMPARISON_HAS_CONTRAST_WRT_AVERAGE_COLOUR_MIN_AVERAGE_COL_DEVIATION))
				{
					totalStarkLocalDeviation = totalStarkLocalDeviation + deviationOptimum;
					starkLocalDeviationIndex++;
				}


				if(averageXkernelRelativePositionForLowestErrorMatch-XKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch + SHAREDvars.absDouble(averageXkernelRelativePositionForLowestErrorMatch-XKernelRelativePositionForLowestErrorMatch);
				}
				else
				{
					totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch + SHAREDvars.absDouble(averageXkernelRelativePositionForLowestErrorMatch-XKernelRelativePositionForLowestErrorMatch);
				}

				if(averageYkernelRelativePositionForLowestErrorMatch-YKernelRelativePositionForLowestErrorMatch > 0)
				{
					totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch + SHAREDvars.absDouble(averageYkernelRelativePositionForLowestErrorMatch-YKernelRelativePositionForLowestErrorMatch);
				}
				else
				{
					totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch + SHAREDvars.absDouble(averageYkernelRelativePositionForLowestErrorMatch-YKernelRelativePositionForLowestErrorMatch);
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

	double averagetotalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averagetotalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	double averagetotalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch/(matchErrorIndex);
	double averagetotalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch = totalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch/(matchErrorIndex);
	double averageXPositionalDeviation = (averagetotalDifferenceFromAverageXKernelRelativePositionPositiveForLowestErrorMatch + -averagetotalDifferenceFromAverageXKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averageYPositionalDeviation = (averagetotalDifferenceFromAverageYKernelRelativePositionPositiveForLowestErrorMatch + -averagetotalDifferenceFromAverageYKernelRelativePositionNegativeForLowestErrorMatch)/2.0;

	double averagePositionalDeviationNormalised = (averageXPositionalDeviation+averageYPositionalDeviation)/2.0;		//normalised to max=1.0 		//IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING;
	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);
	double averageHueDeviationNormalised = ((averagetotalDifferenceFromAverageRHueDeviation + averagetotalDifferenceFromAverageGHueDeviation + averagetotalDifferenceFromAverageBHueDeviation)/(3.0*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR));	//normalise to ~1

#ifdef OR_IMAGE_COMPARISON_VERBOSE_NEW_COMPARITOR
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


	return error;
}




double ORimagecomparisonClass::compareImagesRGBsmallNoKernel(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain)
{
	double totalMatchError = 0.0;
	int matchErrorIndex = 0;

	int hueDeviationIndex = 0;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			colour testImagePixelColour;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTest, &testImagePixelColour);

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR

			colour trainImagePixelColourCentre;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTrain, &trainImagePixelColourCentre);
			if(((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)) || ((trainImagePixelColourCentre.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (trainImagePixelColourCentre.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (trainImagePixelColourCentre.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
			{
				//pitch black - assume outside of t
			}
			else
			{
		#endif
			#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
				double pixelError = compareRGBpixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColourCentre);
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


	return error;
}




#ifndef OR_IMAGE_COMPARISON_USE_NEW_COMPARITOR
double ORimagecomparisonClass::compareImagesRGBwithPosDevAndLocalStarkCont(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain)
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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTest, &testImagePixelColour);

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTrain, &trainImagePixelColourCentre);
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
								RTpixelMaps.getRGBMapValues(kx, ky, imageWidth, rgbMapTrain, &trainImagePixelColour);

								#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBpixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
								#else
								double pixelError = compareRGBpixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);
								#endif

								if(pixelError < lowestMatchErrorAcrossKernel)
								{
									if(!SHAREDvars.compareDoubles(pixelError, 0.0))
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
				RTpixelMaps.getRGBMapValues(XKernelRelativePositionForLowestErrorMatch+x, YKernelRelativePositionForLowestErrorMatch+y, imageWidth, rgbMapTrain, &trainImagePixelColourOptimum);

				double rdeviationOptimum = SHAREDvars.absDouble(trainImagePixelColourOptimum.r - testImagePixelColour.r);
				double gdeviationOptimum = SHAREDvars.absDouble(trainImagePixelColourOptimum.g - testImagePixelColour.g);
				double bdeviationOptimum = SHAREDvars.absDouble(trainImagePixelColourOptimum.b - testImagePixelColour.b);

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

	double averageXPositionalDeviation = (averageXKernelRelativePositionPositiveForLowestErrorMatch + -averageXKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averageYPositionalDeviation = (averageYKernelRelativePositionPositiveForLowestErrorMatch + -averageYKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averagePositionalDeviationNormalised = ((averageXPositionalDeviation+averageYPositionalDeviation)/IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING;
	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);

	double error = averageMatchErrorNormalised + averagePositionalDeviationNormalised;

	if(starkLocalDeviationIndex > OR_IMAGE_COMPARISON_MAX_TOTAL_NUM_STARK_LOCAL_DEVIATIONS*(imageWidth*imageHeight))
	{
		error = 8888.0;
	}


	return error;
}



double ORimagecomparisonClass::compareImagesRGBwithPosDev(int imageWidth, const int imageHeight, unsigned char* rgbMapTest, unsigned char* rgbMapTrain)
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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTest, &testImagePixelColour);

		#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
			#ifdef OR_USE_STAR_MAPS
			if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
			#else
			colour trainImagePixelColourCentre;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapTrain, &trainImagePixelColourCentre);
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
								RTpixelMaps.getRGBMapValues(kx, ky, imageWidth, rgbMapTrain, &trainImagePixelColour);

								#ifdef OR_IMAGE_COMPARITOR_USE_NORMALISED_HUE_ERROR_COMPARISON
								double pixelError = compareRGBpixelsForMatchBAD(&testImagePixelColour, &trainImagePixelColour);
								#else
								double pixelError = compareRGBpixelsForMatchHueOnly(&testImagePixelColour, &trainImagePixelColour);
								#endif

								if(pixelError < lowestMatchErrorAcrossKernel)
								{
									if(!SHAREDvars.compareDoubles(pixelError, 0.0))
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

	double averageXPositionalDeviation = (averageXKernelRelativePositionPositiveForLowestErrorMatch + -averageXKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averageYPositionalDeviation = (averageYKernelRelativePositionPositiveForLowestErrorMatch + -averageYKernelRelativePositionNegativeForLowestErrorMatch)/2.0;
	double averagePositionalDeviationNormalised = ((averageXPositionalDeviation+averageYPositionalDeviation)/IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_KERNEL_POSITIONAL_DEVIATION_WEIGHTING;
	double averageMatchErrorNormalised = (totalMatchError/matchErrorIndex);
	#ifdef OR_USE_OLD_BUGGY_BUT_WORKS_CODE
	double error = averageMatchErrorNormalised + averagePositionalDeviationNormalised;
	#else
	double error = averageMatchErrorNormalised;
	#endif


	return error;
}
#endif




#ifndef OR_METHOD_CHECK_SNAPSHOT_CONTRAST_BEFORE_SAVING_SNAPSHOT
		//NEW; 1sXy; do not accept images with no contrast - ie all same colour
bool ORimagecomparisonClass::checkImageHasContrast(int imageWidth, const int imageHeight, unsigned char* rgbMap)
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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);
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
				RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &testImagePixelColour);

			#ifndef OR_DO_NOT_IGNORE_BACKGROUND_COLOUR
				if((testImagePixelColour.r == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (testImagePixelColour.g == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (testImagePixelColour.b == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
				{
					//pitch black - assume outside of t
				}
				else
				{
			#endif
					double rdeviation = SHAREDvars.absDouble(averager - testImagePixelColour.r);
					double gdeviation = SHAREDvars.absDouble(averageg - testImagePixelColour.g);
					double bdeviation = SHAREDvars.absDouble(averageb - testImagePixelColour.b);

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
void ORimagecomparisonClass::calculateHueError(colour* testImagePixelColour, colour* trainImagePixelColour, double* hueError, double* nonHueError)
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

	*nonHueError = abs(testImagePixelColour->r - trainImagePixelColour->r) + abs(testImagePixelColour->g - trainImagePixelColour->g) + abs(testImagePixelColour->b - trainImagePixelColour->b);


}
#endif

#ifdef OR_IMAGE_COMPARISON_USE_BAD_CODE
	//this function is designed to be independant of lighting conditions
	//hue error is normalised based upon colour satuation, if low colour satuation then calculate error more on luminosity contrast rather than hue/colour contrast	[not yet tested]
double ORimagecomparisonClass::calculateHueErrorNormalisedBAD(const colour* testImagePixelColour, const colour* trainImagePixelColour, double* hueError, double* nonHueErrorNormalised)
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


	return 0.0;
}


	//this function is designed to be independant of lighting conditions
	//hue error is normalised based upon colour satuation, if low colour satuation then calculate error more on luminosity contrast rather than hue/colour contrast	[not yet tested]
double ORimagecomparisonClass::compareRGBpixelsForMatchBAD(const colour* testImagePixelColour, const colour* trainImagePixelColour)
{
	/*OLD;
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	double pixelError =  (nonHueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR) + (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING;
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
double ORimagecomparisonClass::compareRGBpixelsForMatchHueOnly(colour* testImagePixelColour, colour* trainImagePixelColour)
{
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	double pixelError =  (hueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_ERROR)*IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_HUE_WEIGHTING;


	return pixelError;
}

double ORimagecomparisonClass::compareRGBpixelsForMatchLumContrastOnly(colour* testImagePixelColour, colour* trainImagePixelColour)
{
	double hueError;
	double nonHueError;

	calculateHueError(testImagePixelColour, trainImagePixelColour, &hueError, &nonHueError);

	double pixelError =  (nonHueError/IMAGE_COMPARISON_MISFIT_AVG_PIXEL_COMPARISON_NON_HUE_ERROR);


	return 0.0;
}
#endif

/*
double compareRGBPixelsForMatchDirectional(colour* testImagePixelColour, colour* trainImagePixelColour)
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
void ORimagecomparisonClass::createColourHueContrastVecMapFromRGBMapNOTUSED(int imageWidth, const int imageHeight, unsigned char* rgbMap, double* colourHueContrastVectorMap, int* colourHueContrastCategoryIntMap)
{
	//now lead a 3x3 kernel across image and calculate areas of high contrast
	for(int y = 0; y < (imageHeight-1); y++)
	{
		for(int x = 0; x < (imageWidth-1); x++)
		{
			vec colourHueContrastVecVal;
			int colourHueContrastCategoryIntVal;
			calculateColourHueContrastVecLevelWithinKernelNOTUSED(x, y, rgbMap, HUE_CONTRAST_VECTOR_MAP_GENERATION_KERNEL_WIDTH, HUE_CONTRAST_VECTOR_MAP_GENERATION_KERNEL_HEIGHT, imageWidth, imageHeight, &colourHueContrastVecVal, &colourHueContrastCategoryIntVal);
			ORpixelMaps.setXYvectorMapValue(x, y, (imageWidth-1), &colourHueContrastVecVal, colourHueContrastVectorMap);
			RTpixelMaps.setIntMapValue(x, y, (imageWidth-1), colourHueContrastCategoryIntVal, colourHueContrastCategoryIntMap);
		}
	}
}


void ORimagecomparisonClass::calculateColourHueContrastVecLevelWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight, vec* colourHueContrastVecVal, int* colourHueContrastCategoryIntVal)
{
	colourHueContrastVecVal->x = 0.0;
	colourHueContrastVecVal->y = 0.0;

	/*
	colour pixelRGBColour[4];
	colour pixelRGBColourTemp[4];

	RTpixelMaps.getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &(pixelRGBColour[0]));
	RTpixelMaps.getRGBMapValues(pixelX+1, pixelY, imageWidth, rgbMap, &(pixelRGBColour[1]));
	RTpixelMaps.getRGBMapValues(pixelX, pixelY+1, imageWidth, rgbMap, &(pixelRGBColour[2]));
	RTpixelMaps.getRGBMapValues(pixelX+1, pixelY+1, imageWidth, rgbMap, &(pixelRGBColour[3]));

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
	RTpixelMaps.getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &topLeftPixelPositionInRGBColour);

	colour topRightPixelPositionInRGBColour;
	RTpixelMaps.getRGBMapValues(pixelX+1, pixelY, imageWidth, rgbMap, &topRightPixelPositionInRGBColour);

	colour bottomLeftPixelPositionInRGBColour;
	RTpixelMaps.getRGBMapValues(pixelX, pixelY+1, imageWidth, rgbMap, &bottomLeftPixelPositionInRGBColour);

	colour bottomRightPixelPositionInRGBColour;
	RTpixelMaps.getRGBMapValues(pixelX+1, pixelY+1, imageWidth, rgbMap, &bottomRightPixelPositionInRGBColour);


	//x
	double xTop = compareRGBpixelsForMatchHueOnly(&topLeftPixelPositionInRGBColour, &topRightPixelPositionInRGBColour);
	bool xTopThreshold = false;
	if(xTop > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		xTopThreshold = true;
	}
	double xBottom = compareRGBpixelsForMatchHueOnly(&bottomLeftPixelPositionInRGBColour, &bottomRightPixelPositionInRGBColour);
	bool xBottomThreshold = false;
	if(xBottom > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		xBottomThreshold = true;
	}

	//y
	double yLeft = compareRGBpixelsForMatchHueOnly(&topLeftPixelPositionInRGBColour, &bottomLeftPixelPositionInRGBColour);
	bool yLeftThreshold = false;
	if(yLeft > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		yLeftThreshold = true;
	}

	double yRight = compareRGBpixelsForMatchHueOnly(&topRightPixelPositionInRGBColour, &bottomRightPixelPositionInRGBColour);
	bool yRightThreshold = false;
	if(yRight > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		yRightThreshold = true;
	}

	//diagonal
	double diagonalTopLeftToBottomRight = compareRGBpixelsForMatchHueOnly(&topLeftPixelPositionInRGBColour, &bottomRightPixelPositionInRGBColour);
	bool diagonalTopLeftToBottomRightThreshold = false;
	if(diagonalTopLeftToBottomRight > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		diagonalTopLeftToBottomRightThreshold = true;
	}

	double diagonalTopRightToBottomLeft = compareRGBpixelsForMatchHueOnly(&topRightPixelPositionInRGBColour, &bottomLeftPixelPositionInRGBColour);
	bool diagonalTopRightToBottomLeftThreshold = false;
	if(diagonalTopRightToBottomLeft > COLOUR_HUE_CONTRAST_THRESHOLD)
	{
		diagonalTopRightToBottomLeftThreshold = true;
	}


	if(!xTopThreshold && !xBottomThreshold && yLeftThreshold && yRightThreshold && diagonalTopLeftToBottomRightThreshold && diagonalTopRightToBottomLeftThreshold)
	{
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
void createColourHueContrastMapFromRGBMap(int imageWidth, int imageHeight, unsigned char* rgbMap, double* colourHueContrastMap)
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



double calculateColourHueContrastLevelWithinKernel(int pixelX, int pixelY, unsigned char* rgbMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
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


double ORimagecomparisonClass::calculateColourHueSumContrastWithinKernelNOTUSED(int pixelX, int pixelY, unsigned char* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight)
{
	double sumContrast = 0.0;

	colour centrePixelPositionInRGBColour;
	RTpixelMaps.getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &centrePixelPositionInRGBColour);

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
						RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &kernelCurrentPixelPositionInRGBColour);

						sumContrast = sumContrast + compareRGBpixelsForMatchHueOnly(&centrePixelPositionInRGBColour, &kernelCurrentPixelPositionInRGBColour);
					}
			/*
				}
			}
			*/
		}
	}

	return sumContrast;
}


double ORimagecomparisonClass::calculateColourHueRadialSumContrastWithinKernelNOTUSED(const int pixelX, const int pixelY, unsigned char* rgbMap, const int kernelWidth, const int kernelHeight, int imageWidth, const int imageHeight)
{
	double sumContrast = 0.0;

	//colour centrePixelPositionInRGBColour;
	//RTpixelMaps.getRGBMapValues(pixelX, pixelY, imageWidth, rgbMap, &centrePixelPositionInRGBColour);

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
						RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &kernelCurrentPixelPositionInRGBColour);


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
											colour subKernelCurrentPixelPositionInRGBColour;
											RTpixelMaps.getRGBMapValues(sx, sy, imageWidth, rgbMap, &subKernelCurrentPixelPositionInRGBColour);

											sumContrast = sumContrast + compareRGBpixelsForMatchHueOnly(&subKernelCurrentPixelPositionInRGBColour, &kernelCurrentPixelPositionInRGBColour);
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



