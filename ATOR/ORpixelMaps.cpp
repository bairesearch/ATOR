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
 * File Name: ORpixelMaps.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3f5c 21-July-2015
 *
 *******************************************************************************/

#include "ORglobalDefs.h"
#include "ORpixelMaps.h"
#include "RTpixelMaps.h"
#include "SHAREDvector.h"

//double* pointNormalMap = new double[(imageWidth-2)*(imageHeight-2)*VECTOR_MAP_NUM_DIMENSIONS];

//requires;
//vec meshPointNormals[4];	//from 4 tris creating using mesh point and two out of four surrounding meshpoints, x-, x+, y-, y+
//vec meshPointNormal; 		//average of four calculated meshpoint normals
void calculateMeshPointNormalsUsingPointMap(int x, int y, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, double* pointMap, vec* meshPointNormal, vec meshPointNormals[])
{
	vec averageNormalVector;
	averageNormalVector.x = 0.0;
	averageNormalVector.y = 0.0;
	averageNormalVector.z = 0.0;

	if((kernelWidth >= 3) && (kernelHeight >= 3))
	{//point normal map is defined for pixels, with the first and last row and first and last column of the normal map being null defined

		if((y >= 1) && (y <imageHeight-1))
		{//check kernel pix is within image

			if((x >= 1) && (x <imageWidth-1))
			{//check kernel pix is within image

				vec point1;
				getPointMapValue(x, y, imageWidth, pointMap, &point1);

				for(int q=1; q<9; q=q+2)
				{
					vec point2;
					vec point3;

					int point2x = x + q%3 - 1;
					int point2y = y + q/3 - 1;

					getPointMapValue(point2x, point2y, imageWidth, pointMap, &point2);

					int r=(q+2)%8;

					int point3x = x + r%3 - 1;
					int point3y = y + r/3 - 1;

					getPointMapValue(point3x, point3y, imageWidth, pointMap, &point3);

					//calculate normal of vector using 3 points
					vec currentNormalVec;
					calculateNormalOfTri(&point1, &point2, &point3, &currentNormalVec);

					copyVectors(&(meshPointNormals[(q-1)/2]),  &currentNormalVec);

					averageNormalVector.x = averageNormalVector.x + currentNormalVec.x;
					averageNormalVector.y = averageNormalVector.y + currentNormalVec.y;
					averageNormalVector.z = averageNormalVector.z + currentNormalVec.z;
				}

				averageNormalVector.x = averageNormalVector.x/4.0;
				averageNormalVector.y = averageNormalVector.y/4.0;
				averageNormalVector.z = averageNormalVector.z/4.0;

				#ifndef DO_NOT_NORMALISE_POINT_NORMAL_MAP
				normaliseVector(&averageNormalVector);
				#endif

			}
		}
	}
	else if((kernelWidth == 2) && (kernelHeight == 2))
	{//NEW: point normal map is defined for interpixels, with the last row and last column of the normal map being null defined

		/*
		2 4
		1 3
		*/

		if(y < (imageHeight-1))
		{//check kernel pix is within image

			if(x < (imageWidth-1))
			{//check kernel pix is within image

				vec point1;
				vec point2;
				vec point3;
				vec point4;

				getPointMapValue(x, y, imageWidth, pointMap, &point1);
				getPointMapValue(x, y+1, imageWidth, pointMap, &point2);
				getPointMapValue(x+1, y, imageWidth, pointMap, &point3);
				getPointMapValue(x+1, y+1, imageWidth, pointMap, &point4);

				vec currentNormalVec;
				calculateNormalOfTri(&point1, &point2, &point3, &currentNormalVec);
				copyVectors(&(meshPointNormals[0]),  &currentNormalVec);

				averageNormalVector.x = averageNormalVector.x + currentNormalVec.x;
				averageNormalVector.y = averageNormalVector.y + currentNormalVec.y;
				averageNormalVector.z = averageNormalVector.z + currentNormalVec.z;

				#ifndef DO_NOT_NORMALISE_POINT_NORMAL_MAP
				normaliseVector(&averageNormalVector);
				#endif

				calculateNormalOfTri(&point3, &point2, &point4, &currentNormalVec);
				copyVectors(&(meshPointNormals[1]),  &currentNormalVec);

				averageNormalVector.x = averageNormalVector.x + currentNormalVec.x;
				averageNormalVector.y = averageNormalVector.y + currentNormalVec.y;
				averageNormalVector.z = averageNormalVector.z + currentNormalVec.z;


				#ifndef DO_NOT_NORMALISE_POINT_NORMAL_MAP
				normaliseVector(&averageNormalVector);
				#endif
			}
			else
			{
				averageNormalVector.x = MAP_VALUE_OUT_OF_RANGE;
				averageNormalVector.y = MAP_VALUE_OUT_OF_RANGE;
				averageNormalVector.z = MAP_VALUE_OUT_OF_RANGE;
			}
		}
		else
		{
			averageNormalVector.x = MAP_VALUE_OUT_OF_RANGE;
			averageNormalVector.y = MAP_VALUE_OUT_OF_RANGE;
			averageNormalVector.z = MAP_VALUE_OUT_OF_RANGE;
		}
	}
	else
	{
		cout << "error: invalid contrast map generation kernel width and/or height" << endl;
		exit(0);
	}

	copyVectors(meshPointNormal,  &averageNormalVector);
}

void createPointNormalMapFromPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointNormalMap)
{
	int kernelWidth = DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH;
	int kernelHeight = DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			vec meshPointNormals[4];	//from 4 tris creating using mesh point and two out of four surrounding meshpoints, x-, x+, y-, y+
			vec meshPointNormal; 		//average of four calculated meshpoint normals
			calculateMeshPointNormalsUsingPointMap(x, y, kernelWidth, kernelHeight, imageWidth, imageHeight, pointMap, &meshPointNormal, meshPointNormals);

			setVectorMapValue(x, y, imageWidth, &meshPointNormal, pointNormalMap);

			#ifdef OR_DEBUG
			/*
			cout << "meshPointNormal.x = " << meshPointNormal.x << endl;
			cout << "meshPointNormal.y = " << meshPointNormal.y << endl;
			cout << "meshPointNormal.z = " << meshPointNormal.z << endl;
			*/
			#endif
		}
	}
}

void createPointNormalContrastMapFromPointNormalMap(int imageWidth, int imageHeight, double* pointNormalMap, double* pointNormalContrastMap)
{
	for(int y = 1; y < imageHeight-1; y++)
	{
		for(int x = 1; x < imageWidth-1; x++)
		{
			double contrastVal = calculatePointNormalContrastLevelWithinKernel(x, y, pointNormalMap, DEFAULT_NORMAL_MAP_GENERATION_KERNEL_WIDTH, DEFAULT_NORMAL_MAP_GENERATION_KERNEL_HEIGHT, imageWidth, imageHeight);
			setLumOrContrastOrDepthMapValue(x, y, imageWidth, contrastVal, pointNormalContrastMap);
		}
	}
}

double calculatePointNormalContrastLevelWithinKernel(int pixelX, int pixelY, double* pointNormalMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	double contrastLevel = 0.0;

	if((kernelWidth >= 3) && (kernelHeight >= 3))
	{//point normal contrast map is defined for pixels
		vec centrePixelNormal;
		getVectorMapValue(pixelX, pixelY, imageWidth, pointNormalMap, &centrePixelNormal);

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
							vec currentPixelNormal;
							getVectorMapValue(x, y, imageWidth, pointNormalMap, &currentPixelNormal);

							//calc point normal vector diff between centre pixel and current surrounding kernel pixel
							double currentContrastLevelX = absDouble(centrePixelNormal.x - currentPixelNormal.x);
							double currentContrastLevelY = absDouble(centrePixelNormal.y - currentPixelNormal.y);
							double currentContrastLevelZ = absDouble(centrePixelNormal.z - currentPixelNormal.z);

							if(OR_USE_CONTRAST_CALC_METHOD_B)
							{
								contrastLevel = contrastLevel + currentContrastLevelX+currentContrastLevelY+currentContrastLevelZ;
							}
							else
							{
								contrastLevel = maxDouble(contrastLevel, (currentContrastLevelX+currentContrastLevelY+currentContrastLevelZ));
							}
						}
					}
				}
			}
		}
	}
	else if((kernelWidth == 2) && (kernelHeight == 2))
	{//NEW: point normal contrast map is defined for pixels, with the first and last row and first and last column of the point normal map being null defined

		if((pixelY > 0) && (pixelY < (imageHeight-1)))
		{//check kernel pix is within image

			if((pixelX > 0) && (pixelX < (imageWidth-1)))
			{//check kernel pix is within image

				for(int y = (pixelY-1); y<= (pixelY+1)-1; y++)
				{
					vec interpixelNormalValue;
					getXYvectorMapValue((pixelX-1), y, imageWidth, pointNormalMap, &interpixelNormalValue);
					vec adjacentInterpixelNormalValue;
					getXYvectorMapValue((pixelX-1)+1, y, imageWidth, pointNormalMap, &adjacentInterpixelNormalValue);

					double currentContrastLevelX = absDouble(interpixelNormalValue.x - adjacentInterpixelNormalValue.x);
					double currentContrastLevelY = absDouble(interpixelNormalValue.y - adjacentInterpixelNormalValue.y);
					double currentContrastLevelZ = absDouble(interpixelNormalValue.z - adjacentInterpixelNormalValue.z);

					contrastLevel = contrastLevel + currentContrastLevelX+currentContrastLevelY+currentContrastLevelZ;
				}

				for(int x = (pixelX-1); x<= (pixelX+1)-1; x++)
				{
					vec interpixelNormalValue;
					getXYvectorMapValue(x, (pixelY-1), imageWidth, pointNormalMap, &interpixelNormalValue);
					vec adjacentInterpixelNormalValue;
					getXYvectorMapValue(x, (pixelY-1)+1, imageWidth, pointNormalMap, &adjacentInterpixelNormalValue);

					double currentContrastLevelX = absDouble(interpixelNormalValue.x - adjacentInterpixelNormalValue.x);
					double currentContrastLevelY = absDouble(interpixelNormalValue.y - adjacentInterpixelNormalValue.y);
					double currentContrastLevelZ = absDouble(interpixelNormalValue.z - adjacentInterpixelNormalValue.z);

					contrastLevel = contrastLevel + currentContrastLevelX+currentContrastLevelY+currentContrastLevelZ;
				}
			}
			else
			{
				contrastLevel = MAP_VALUE_OUT_OF_RANGE;
			}
		}
		else
		{
			contrastLevel = MAP_VALUE_OUT_OF_RANGE;
		}
	}
	else
	{
		cout << "error: invalid contrast map generation kernel width and/or height" << endl;
		exit(0);
	}

	return contrastLevel;
}




void cropRGBmap(int originalImageWidth, int originalImageHeight, int cropXPos, int cropYPos, int croppedWidth, int croppedHeight, unsigned char* rgbMapUncropped, unsigned char* rgbMap)
{
	int croppedYPos = 0;
	for(int y = cropYPos; y < cropYPos+croppedHeight; y++)
	{
		int croppedXPos = 0;
		for(int x = cropXPos; x < cropXPos+croppedWidth; x++)
		{
			colour col;
			getRGBMapValues(x, y, originalImageWidth, rgbMapUncropped, &col);
			setRGBMapValues(croppedXPos, croppedYPos, croppedWidth, &col, rgbMap);

			croppedXPos++;
		}
		croppedYPos++;
	}
}

void cropDepthMap(int originalImageWidth, int originalImageHeight, int cropXPos, int cropYPos, int croppedWidth, int croppedHeight, double* depthMapUncropped, double* depthMap)
{
	int croppedYPos = 0;
	for(int y = cropYPos; y < cropYPos+croppedHeight; y++)
	{
		int croppedXPos = 0;
		for(int x = cropXPos; x < cropXPos+croppedWidth; x++)
		{
			double depthVal;
			depthVal = getLumOrContrastOrDepthMapValue(x, y, originalImageWidth, depthMapUncropped);
			setLumOrContrastOrDepthMapValue(croppedXPos, croppedYPos, croppedWidth, depthVal, depthMap);

			croppedXPos++;
		}
		croppedYPos++;
	}
}








void setXYvectorMapValue(int x, int y, int imageWidth, vec* XYvectorVal, double* XYvectorMap)
{
	XYvectorMap[y*imageWidth*XY_VECTOR_MAP_NUM_DIMENSIONS + x*XY_VECTOR_MAP_NUM_DIMENSIONS + XY_VECTOR_MAP_VEC_X] = XYvectorVal->x;
	XYvectorMap[y*imageWidth*XY_VECTOR_MAP_NUM_DIMENSIONS + x*XY_VECTOR_MAP_NUM_DIMENSIONS + XY_VECTOR_MAP_VEC_Y] = XYvectorVal->y;
}

void getXYvectorMapValue(int x, int y, int imageWidth, double* XYvectorMap, vec* XYvectorVal)
{
	XYvectorVal->x = XYvectorMap[y*imageWidth*XY_VECTOR_MAP_NUM_DIMENSIONS + x*XY_VECTOR_MAP_NUM_DIMENSIONS + XY_VECTOR_MAP_VEC_X];
	XYvectorVal->y = XYvectorMap[y*imageWidth*XY_VECTOR_MAP_NUM_DIMENSIONS + x*XY_VECTOR_MAP_NUM_DIMENSIONS + XY_VECTOR_MAP_VEC_Y];
}





void generateRGBmapFromDepthGradientMap(int imageWidth, int imageHeight, double* depthGradientMap, unsigned char* rgbMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			vec depthGradient;
			getXYvectorMapValue(x, y, imageWidth,depthGradientMap,&depthGradient);


			int r;
			int g;
			int b;

			if((absDouble(depthGradient.x) > ESTIMATE_MAX_DEPTH_GRADIENT) || (absDouble(depthGradient.y) >ESTIMATE_MAX_DEPTH_GRADIENT))
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;

				r = 127 + (int)(depthGradient.x/ESTIMATE_MAX_DEPTH_GRADIENT*127);
				b = 127 + (int)(depthGradient.y/ESTIMATE_MAX_DEPTH_GRADIENT*127);
			}

			colour col;
			col.r = r;
			col.g = g;
			col.b = b;
			setRGBMapValues(x, y, imageWidth, &col, rgbMap);

		}
	}
}







void generateRGBmapFromDepthGradientContrastMap(int imageWidth, int imageHeight, double* depthGradientContrastMap, unsigned char* rgbMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			double depthGradientContrastVal;
			depthGradientContrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthGradientContrastMap);
			int depthGradientContrastValNormalised = minInt((int)(depthGradientContrastVal/ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST*255.0), 255);

			colour col;
			col.r = depthGradientContrastValNormalised;
			col.g = depthGradientContrastValNormalised;
			col.b = depthGradientContrastValNormalised;
			setRGBMapValues(x, y, imageWidth, &col, rgbMap);

		}
	}
}

void generatePixmapFromDepthGradientContrastMap(string imageFileName, int imageWidth, int imageHeight, double* depthGradientContrastMap)
{
	int x,y;
	pixmap* pm;

	pm = newPixmap(imageWidth, imageHeight);

	for(y = 0; y < imageHeight; y++)
	{
  		for(x = 0; x < imageWidth; x++)
		{
			double depthGradientContrastVal;
			depthGradientContrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthGradientContrastMap);
			int depthGradientContrastValNormalised = minInt((int)(depthGradientContrastVal/ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST*255.0), 255);
			placepointPPM(pm, x, y, depthGradientContrastValNormalised, depthGradientContrastValNormalised, depthGradientContrastValNormalised);
		}
	}

	writeImage(imageFileName, pm);
}


void generatePixmapFromDepthGradientMap(string imageFileName, int imageWidth, int imageHeight, double* depthGradientMap)
{
	int x,y;
	pixmap* pm;

	pm = newPixmap(imageWidth, imageHeight);

	for(y = 0; y < imageHeight; y++)
	{
  		for(x = 0; x < imageWidth; x++)
		{
			vec depthGradient;
			getXYvectorMapValue(x, y, imageWidth,depthGradientMap,&depthGradient);


			int r;
			int g;
			int b;

			if((absDouble(depthGradient.x) > ESTIMATE_MAX_DEPTH_GRADIENT) || (absDouble(depthGradient.y) >ESTIMATE_MAX_DEPTH_GRADIENT))
			{
				r = 255;
				g = 255;
				b = 255;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;

				/*
				if(depthGradient.x > 0)
				{
					r = (int)(depthGradient.x/ESTIMATE_MAX_DEPTH_GRADIENT*255);
				}
				else
				{
					b = -(int)(depthGradient.x/ESTIMATE_MAX_DEPTH_GRADIENT*255);
				}
				if(depthGradient.y > 0)
				{
					r = (int)(depthGradient.y/ESTIMATE_MAX_DEPTH_GRADIENT*255);
				}
				else
				{
					b = - (int)(depthGradient.y/ESTIMATE_MAX_DEPTH_GRADIENT*255);
				}
				*/

				r = 127 + (int)(depthGradient.x/ESTIMATE_MAX_DEPTH_GRADIENT*127);
				b = 127 + (int)(depthGradient.y/ESTIMATE_MAX_DEPTH_GRADIENT*127);

			}


			placepointPPM(pm, x, y, r, g, b);
			//placepointPPM(nm, x, y, (int)((depthGradient.x+1.0)*128.0), (int)((depthGradient.y+1.0)*128.0), 0);

		}
	}

	writeImage(imageFileName, pm);
}


































//methods taken from ORoperations.cpp;





double calculateForegroundMinimumDepthWithinKernel(int pixelX, int pixelY, int imageWidth, int imageHeight, int kernelWidth, int kernelHeight, double* depthMap, vec* nearbyPointOfMinimumDepth, int zoom)
{
	double minDepthForNearbyPoints = REALLY_FAR_AWAY;

	for(int ky = pixelY-((kernelHeight-1)/2); ky <= pixelY+(kernelHeight/2); ky++)
	{
		for(int kx = pixelX-((kernelWidth-1)/2); kx <= pixelX+(kernelWidth/2); kx++)
		{
			if(((kernelWidth%2 != 0) || (kernelHeight%2 != 0)) && (kx == 0) && (ky == 0))
			{
				//do not modify minDepthForNearbyPoints for the depth value of the central pixel x,y
			}
			else
			{
				double depthVal = getLumOrContrastOrDepthMapValue(kx*zoom, ky*zoom, imageWidth*zoom, depthMap);

				#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
				if(!compareDoubles(depthVal, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [2 june 09] {ensures the pixel is not a background pixel}
				{//off object

					if(depthVal < minDepthForNearbyPoints)
					{
						minDepthForNearbyPoints = depthVal;

						#ifndef OR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
						nearbyPointOfMinimumDepth->x = kx;
						nearbyPointOfMinimumDepth->y = ky;
						nearbyPointOfMinimumDepth->z = depthVal;	//not used
						#endif
					}
				}
				#else
				if(depthVal < minDepthForNearbyPoints)
				{
					minDepthForNearbyPoints = depthVal;

					#ifndef OR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
					nearbyPointOfMinimumDepth->x = kx;
					nearbyPointOfMinimumDepth->y = ky;
					nearbyPointOfMinimumDepth->z = depthVal;	//not used
					#endif
				}
				#endif
			}
		}
	}
	return minDepthForNearbyPoints;
}





void createContrastMapFromMapWithForegroundDepthCheckOLD(int imageWidth, int imageHeight, double* luminosityOrDepthMap, double* depthMap, double* contrastMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold)
{
	int kernelWidth = DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_WIDTH;
	int kernelHeight = DEFAULT_CONTRAST_MAP_GENERATION_KERNEL_HEIGHT;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, contrastBooleanMap);
			setBooleanMapValue(x, y, imageWidth, false, foregroundDepthCheckContrastBooleanMap);
		}
	}


	//now lead a 3x3 kernel across image and calculate areas of high contrast
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double contrastVal = calculateContrastLevelWithinKernelWithForegroundDepthCheckOLD(x, y, luminosityOrDepthMap, depthMap, kernelWidth, kernelHeight, imageWidth, imageHeight, foregroundDepthCheckContrastBooleanMap, mapThreshold);
			//double contrastVal = calculateContrastLevelWithinKernel(x, y, luminosityOrDepthMap, kernelWidth, kernelHeight, imageWidth, imageHeight);

			setLumOrContrastOrDepthMapValue(x, y, imageWidth, contrastVal, contrastMap);

			if(contrastVal > mapThreshold)
			{
				bool contrastValPassedThreshold = false;

				if(OR_USE_CONTRAST_CALC_METHOD_C)
				{
					if(contrastVal != MAP_VALUE_OUT_OF_RANGE)
					{
						contrastValPassedThreshold = true;
					}
					else
					{
						contrastValPassedThreshold = false;
					}
				}
				else
				{
					contrastValPassedThreshold = true;
				}

				setBooleanMapValue(x, y, imageWidth, contrastValPassedThreshold, contrastBooleanMap);
			}
		}
	}
}

double calculateContrastLevelWithinKernelWithForegroundDepthCheckOLD(int pixelX, int pixelY, double* luminosityOrDepthMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold)
{
	double contrastLevel = 0.0;

	if((kernelWidth >= 3) && (kernelHeight >= 3))
	{//contrast map is defined for pixels
		double centrePixelDepth = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, depthMap);
		double centrePixelVal = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, luminosityOrDepthMap);

		#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
		bool centrePixelNoHit = false;
		if(compareDoubles(centrePixelDepth, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [4 Nov 08] {ensures the pixel is not a background pixel}
		{
			centrePixelNoHit = true;
		}
		#endif
		#ifdef OR_DEBUG
		/*
		cout <<"..." <<endl;
		cout << "pixelX = " << pixelX << endl;
		cout << "pixelY = " << pixelY << endl;
		cout << "pixelX-(kernelWidth/2) = " << pixelX-(kernelWidth/2) << endl;
		cout << "pixelX+(kernelWidth/2) = " << pixelX+(kernelWidth/2) << endl;
		cout << "pixelY+(kernelHeight/2) = " << pixelY+(kernelHeight/2) << endl;
		cout << "pixelY-(kernelHeight/2) = " << pixelY-(kernelHeight/2) << endl;
		*/
		#endif

		for(int y = pixelY-(kernelHeight/2); y <= pixelY+(kernelHeight/2); y++)
		{
			for(int x = pixelX-(kernelWidth/2); x <= pixelX+(kernelWidth/2); x++)
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
							double kernelPixelVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityOrDepthMap);
							double currentContrastLevel = absDouble(centrePixelVal - kernelPixelVal);

							//calc diff lum diff between centre pixel and current surrounding kernel pixel

							double kernelPixelDepth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);
							#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
							if(compareDoubles(kernelPixelDepth, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [4 Nov 08] {ensures the pixel is not a background pixel}
							{
								kernelPixelDepth = ESTIMATE_MAX_DEPTH_T_REAL;
							}
							#endif

							if(centrePixelDepth < (kernelPixelDepth+OR_MAX_DEPTH_NOISE))	//NEW official [4 Nov 08] {ensures the pixel is more foreground than other pixel}
							{
								#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
								if(!centrePixelNoHit)
								{
								#endif

									if(OR_USE_CONTRAST_CALC_METHOD_B)
									{
										contrastLevel = contrastLevel + currentContrastLevel;
									}
									else
									{
										contrastLevel = maxDouble(contrastLevel, currentContrastLevel);
									}

									if(currentContrastLevel > mapThreshold)	//mapThreshold/((kernelWidth*kernelHeight) - 1
									{
										setBooleanMapValue(x, y, imageWidth, true, foregroundDepthCheckContrastBooleanMap);
									}
								#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
								}
								#endif
							}

							#ifdef OR_DEBUG
							//cout << "kx = " << x-pixelX << endl;
							//cout << "ky = " << y-pixelY << endl;
							//cout << "contrastLevel = " << contrastLevel << endl;
							#endif
						}
					}
				}
			}
		}
	}
	else if((kernelWidth == 2) && (kernelHeight == 2))
	{//NEW: contrast map is defined for interpixels, with the last row and last column of the contrast map being null defined

		if(pixelY < (imageHeight-1))
		{//check kernel pix is within image

			if(pixelX < (imageWidth-1))
			{//check kernel pix is within image

				for(int y = pixelY; y<= pixelY+1; y++)
				{
					double pixelDepthMeasurement = getLumOrContrastOrDepthMapValue(pixelX, y, imageWidth, depthMap);
					double pixelValMeasurement = getLumOrContrastOrDepthMapValue(pixelX, y, imageWidth, luminosityOrDepthMap);

					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(pixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						pixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif

					double adjacentPixelDepthMeasurement = getLumOrContrastOrDepthMapValue(pixelX+1, y, imageWidth, depthMap);
					double adjacentPixelValMeasurement = getLumOrContrastOrDepthMapValue(pixelX+1, y, imageWidth, luminosityOrDepthMap);

					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(adjacentPixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						adjacentPixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif

					double currentContrastLevel = absDouble(pixelValMeasurement - adjacentPixelValMeasurement);

					if(pixelDepthMeasurement < adjacentPixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(pixelX, y, imageWidth, true, foregroundDepthCheckContrastBooleanMap);

						}
						contrastLevel = contrastLevel + currentContrastLevel;
					}
					else if(adjacentPixelDepthMeasurement < pixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(pixelX+1, y, imageWidth, true, foregroundDepthCheckContrastBooleanMap);

						}
						contrastLevel = contrastLevel + currentContrastLevel;
					}
				}

				for(int x = pixelX; x<= pixelX+1; x++)
				{
					double pixelDepthMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY, imageWidth, depthMap);
					double pixelValMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY, imageWidth, luminosityOrDepthMap);

					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(pixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						pixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif

					double adjacentPixelDepthMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY+1, imageWidth, depthMap);
					double adjacentPixelValMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY+1, imageWidth, luminosityOrDepthMap);

					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(adjacentPixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						adjacentPixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif

					double currentContrastLevel = absDouble(pixelValMeasurement - adjacentPixelValMeasurement);

					if(pixelDepthMeasurement < adjacentPixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(x, pixelY, imageWidth, true, foregroundDepthCheckContrastBooleanMap);
						}
						contrastLevel = contrastLevel + currentContrastLevel;
					}
					else if(adjacentPixelDepthMeasurement < pixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(x, pixelY+1, imageWidth, true, foregroundDepthCheckContrastBooleanMap);
						}
						contrastLevel = contrastLevel + currentContrastLevel;
					}
				}
			}
			else
			{
				contrastLevel = MAP_VALUE_OUT_OF_RANGE;
			}
		}
		else
		{
			contrastLevel = MAP_VALUE_OUT_OF_RANGE;
		}
	}
	else
	{
		cout << "error: invalid contrast map generation kernel width and/or height" << endl;
		exit(0);
	}

	return contrastLevel;
}


/*
void createContrastBooleanMapFromContrastMapWithForegroundDepthCheck(int imageWidth, int imageHeight, double* contrastMap, double* depthMap, bool* contrastBooleanMap, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold)
{
	int kernelWidth;
	int kernelHeight;
	bool generateInterpixelDepthContrastMap = DEFAULT_CONTRAST_MAP_GENERATION_INTERPIXEL;
	if(generateInterpixelDepthContrastMap)
	{
		kernelWidth = CONTRAST_MAP_GENERATION_KERNEL_WIDTH_INTERPIXEL;
		kernelHeight = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT_INTERPIXEL;
	}
	else
	{
		kernelWidth = CONTRAST_MAP_GENERATION_KERNEL_WIDTH;
		kernelHeight = CONTRAST_MAP_GENERATION_KERNEL_HEIGHT;
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, foregroundDepthCheckContrastBooleanMap);
		}
	}


	//now lead a 3x3 kernel across image and calculate areas of high contrast
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double contrastVal = getContrastLevelWithinKernelWithForegroundDepthCheck(x, y, contrastMap, depthMap, kernelWidth, kernelHeight, imageWidth, imageHeight, foregroundDepthCheckContrastBooleanMap, mapThreshold);

			if(contrastVal > mapThreshold)
			{
				bool contrastValPassedThreshold = false;

				if(OR_USE_CONTRAST_CALC_METHOD_C)
				{
					if(contrastVal != MAP_VALUE_OUT_OF_RANGE)
					{
						contrastValPassedThreshold = true;
					}
					else
					{
						contrastValPassedThreshold = false;
					}
				{
				else
				{
					contrastValPassedThreshold = true;
				}


				setBooleanMapValue(x, y, imageWidth, contrastValPassedThreshold, contrastBooleanMap);
			}

		}
	}
}

double getContrastLevelWithinKernelWithForegroundDepthCheck(int pixelX, int pixelY, double* contrastMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, bool* foregroundDepthCheckContrastBooleanMap, double mapThreshold)
{
	double contrastLevel = 0.0;

	if((kernelWidth >= 3) && (kernelHeight >= 3))
	{//contrast map is defined for pixels
		double centrePixelDepth = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, depthMap);
		double centrePixelVal = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, luminosityOrDepthMap);

		#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
		bool centrePixelNoHit = false;
		if(compareDoubles(centrePixelDepth, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [4 Nov 08] {ensures the pixel is not a background pixel}
		{
			centrePixelNoHit = true;
		}
		#endif


		for(int y = pixelY-(kernelHeight/2); y <= pixelY+(kernelHeight/2); y++)
		{
			for(int x = pixelX-(kernelWidth/2); x <= pixelX+(kernelWidth/2); x++)
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
							double currentContrastLevel = getLumOrContrastOrDepthMapValue(x, y, imageWidth, contrastMap);

							//calc diff lum diff between centre pixel and current surrounding kernel pixel

							double kernelPixelDepth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);
							#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
							if(compareDoubles(kernelPixelDepth, RT_RAYTRACE_NO_HIT_DEPTH_T))	//NEW official [4 Nov 08] {ensures the pixel is not a background pixel}
							{
								kernelPixelDepth = ESTIMATE_MAX_DEPTH_T_REAL;
							}
							#endif

							if(centrePixelDepth < (kernelPixelDepth+OR_MAX_DEPTH_NOISE))	//NEW official [4 Nov 08] {ensures the pixel is more foreground than other pixel}
							{
								#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
								if(!centrePixelNoHit)
								{
								#endif

									if(currentContrastLevel > mapThreshold)	//mapThreshold/((kernelWidth*kernelHeight) - 1
									{
										setBooleanMapValue(pixelX, pixelY, imageWidth, true, foregroundDepthCheckContrastBooleanMap);
									}
								#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
								}
								#endif
							}

							#ifdef OR_DEBUG
							//cout << "x = " << x << endl;
							//cout << "y = " << y << endl;
							//cout << "contrastLevel = " << contrastLevel << endl;
							#endif
						}
					}
				}
			}
		}
	}
	else if((kernelWidth == 2) && (kernelHeight == 2))
	{//NEW: contrast map is defined for interpixels, with the last row and last column of the contrast map being null defined

		if(pixelY < (imageHeight-1))
		{//check kernel pix is within image

			if(pixelX < (imageWidth-1))
			{//check kernel pix is within image

				double currentContrastLevel = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, contrastMap);
				contrastLevel = currentContrastLevel;

				for(int y = pixelY; y<= pixelY+1; y++)
				{
					//cout << "y = " << y << endl;

					double pixelDepthMeasurement = getLumOrContrastOrDepthMapValue(pixelX, y, imageWidth, depthMap);

					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(pixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						pixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif


					double adjacentPixelDepthMeasurement = getLumOrContrastOrDepthMapValue(pixelX+1, y, imageWidth, depthMap);


					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(adjacentPixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						adjacentPixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif

					if(pixelDepthMeasurement < adjacentPixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(pixelX, y, imageWidth, true, foregroundDepthCheckContrastBooleanMap);

						}
					}
					else if(adjacentPixelDepthMeasurement < pixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(pixelX+1, y, imageWidth, true, foregroundDepthCheckContrastBooleanMap);

						}
					}

				}

				for(int x = pixelX; x<= pixelX+1; x++)
				{
					//cout << "x = " << x << endl;

					double pixelDepthMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY, imageWidth, depthMap);


					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(pixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						pixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif


					double adjacentPixelDepthMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY+1, imageWidth, depthMap);

					#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
					if(compareDoubles(adjacentPixelDepthMeasurement, RT_RAYTRACE_NO_HIT_DEPTH_T))
					{
						adjacentPixelDepthMeasurement = ESTIMATE_MAX_DEPTH_T_REAL;
					}
					#endif

					if(pixelDepthMeasurement < adjacentPixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(x, pixelY, imageWidth, true, foregroundDepthCheckContrastBooleanMap);
						}
					}
					else if(adjacentPixelDepthMeasurement < pixelDepthMeasurement)
					{
						if(currentContrastLevel > mapThreshold)		// mapThreshold/4.0
						{
							setBooleanMapValue(x, pixelY+1, imageWidth, true, foregroundDepthCheckContrastBooleanMap);
						}
					}
				}
			}
			else
			{
				contrastLevel = MAP_VALUE_OUT_OF_RANGE;
			}
		}
		else
		{
			contrastLevel = MAP_VALUE_OUT_OF_RANGE;
		}
	}
	else
	{
		cout << "error: invalid contrast map generation kernel width and/or height" << endl;
		exit(0);
	}

	return contrastLevel;
}
*/







/*
	//untested counters aliasing;
double calculateContrastLevelWithinKernelWithForegroundDepthCheck(int pixelX, int pixelY, double* luminosityMap, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	double contrastLevel = 0.0;
	double centrePixelDepth = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, depthMap);


	for(int y = pixelY-(kernelHeight/2); y<= pixelY+(kernelHeight/2); y++)
	{
		int ForegroundDepthCheckKernelY = ((y-pixelY)*2)+pixelY;

		for(int x = pixelX-(kernelWidth/2); x<= pixelX+(kernelWidth/2); x++)
		{
			int ForegroundDepthCheckKernelX = ((x-pixelX)*2)+pixelX;

			if((ForegroundDepthCheckKernelY >= 0) && (ForegroundDepthCheckKernelY <imageHeight))
			{//check kernel pix is within image

				if((ForegroundDepthCheckKernelX >= 0) && (ForegroundDepthCheckKernelX <imageWidth))
				{//check kernel pix is within image

					if((x == pixelX) && (y == pixelY))
					{

					}
					else
					{
						double kernelPixelDepth = getLumOrContrastOrDepthMapValue(ForegroundDepthCheckKernelX, ForegroundDepthCheckKernelY, imageWidth, depthMap);

						//calc diff lum diff between centre pixel and current surrounding kernel pixel

						double centrePixelLuminosity = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, luminosityMap);
						double kernelPixelLuminosity = getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityMap);
						double currentContrastLevel = absDouble(centrePixelLuminosity - kernelPixelLuminosity);

						if(centrePixelDepth > (kernelPixelDepth-OR_MAX_DEPTH_NOISE))
						{
							contrastLevel = maxDouble(contrastLevel, currentContrastLevel);
						}
						#ifdef OR_DEBUG
						//cout << "x = " << x << endl;
						//cout << "y = " << y << endl;
						//cout << "contrastLevel = " << contrastLevel << endl;
						#endif
					}
				}
			}
		}
	}

	return contrastLevel;
}
*/


void createDepthGradientMapFromDepthMap(int imageWidth, int imageHeight, double* depthMap, double* depthGradientMap)
{
	int kernelWidth = DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH;
	int kernelHeight = DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			vec depthGradientVal;
			calculateDepthGradientValueWithinKernel(x, y, depthMap, kernelWidth, kernelHeight, imageWidth, imageHeight, &depthGradientVal);
			setXYvectorMapValue(x, y, imageWidth, &depthGradientVal, depthGradientMap);
		}
	}
}



double calculateDepthGradientValueWithinKernel(int pixelX, int pixelY, double* depthMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight, vec* depthGradientVal)
{
	double xGradient = 0;
	double yGradient = 0;

	if((kernelWidth >= 3) && (kernelHeight >= 3))
	{//depth gradient map is defined for pixels
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
							//calc diff lum diff between centre pixel and current surrounding kernel pixel

							double centrePixelPositionInDepthMapDepth = getLumOrContrastOrDepthMapValue(pixelX, pixelY, imageWidth, depthMap);
							double kernelCurrentPixelPositionInDepthMapDepth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

							if(x < pixelX)
							{
								xGradient = xGradient + (centrePixelPositionInDepthMapDepth-kernelCurrentPixelPositionInDepthMapDepth);
							}
							else if(x > pixelX)
							{
								xGradient = xGradient - (centrePixelPositionInDepthMapDepth-kernelCurrentPixelPositionInDepthMapDepth);
							}

							if(y < pixelY)
							{
								yGradient = yGradient + (centrePixelPositionInDepthMapDepth-kernelCurrentPixelPositionInDepthMapDepth);
							}
							else if(y > pixelY)
							{
								yGradient = yGradient - (centrePixelPositionInDepthMapDepth-kernelCurrentPixelPositionInDepthMapDepth);
							}
						}
					}
				}
			}
		}
	}
	else if((kernelWidth == 2) && (kernelHeight == 2))
	{//NEW: depth gradient map is defined for interpixels, with the last row and last column of the gradient map being null defined

		if(pixelY < (imageHeight-1))
		{//check kernel pix is within image

			if(pixelX < (imageWidth-1))
			{//check kernel pix is within image

				for(int y = pixelY; y<= pixelY+1; y++)
				{
					double pixelDepthMeasurement = getLumOrContrastOrDepthMapValue(pixelX, y, imageWidth, depthMap);
					double adjacentPixelDepthMeasurement = getLumOrContrastOrDepthMapValue(pixelX+1, y, imageWidth, depthMap);

					xGradient = xGradient + (adjacentPixelDepthMeasurement - pixelDepthMeasurement);
				}

				for(int x = pixelX; x<= pixelX+1; x++)
				{
					double pixelDepthMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY, imageWidth, depthMap);
					double adjacentPixelDepthMeasurement = getLumOrContrastOrDepthMapValue(x, pixelY+1, imageWidth, depthMap);

					yGradient = yGradient + (adjacentPixelDepthMeasurement - pixelDepthMeasurement);
				}
			}
			else
			{
				xGradient = MAP_VALUE_OUT_OF_RANGE;
				yGradient = MAP_VALUE_OUT_OF_RANGE;
			}
		}
		else
		{
			xGradient = MAP_VALUE_OUT_OF_RANGE;
			yGradient = MAP_VALUE_OUT_OF_RANGE;
		}
	}
	else
	{
		cout << "error: invalid contrast map generation kernel width and/or height" << endl;
		exit(0);
	}

	depthGradientVal->x = xGradient;
	depthGradientVal->y = yGradient;

	return 0.0;
}

void subtractBooleanMaps(int imageWidth, int imageHeight, bool* booleanMap1, bool* booleanMap2, bool* booleanMap1MinusBooleanMap2)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			bool booleanMap1MinusBooleanMap2Val;

			bool booleanMap1Val = getBooleanMapValue(x, y, imageWidth, booleanMap1);
			bool booleanMap2Val = getBooleanMapValue(x, y, imageWidth, booleanMap2);

			if(booleanMap1Val && !booleanMap2Val)
			{
				booleanMap1MinusBooleanMap2Val = true;
			}
			else
			{
				booleanMap1MinusBooleanMap2Val = false;
			}

			setBooleanMapValue(x, y, imageWidth, booleanMap1MinusBooleanMap2Val, booleanMap1MinusBooleanMap2);

		}
	}
}


void addBooleanMaps(int imageWidth, int imageHeight, bool* booleanMap1, bool* booleanMap2, bool* booleanMap1PlusBooleanMap2)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			bool booleanMap1PlusBooleanMap2Val;

			bool booleanMap1Val = getBooleanMapValue(x, y, imageWidth, booleanMap1);
			bool booleanMap2Val = getBooleanMapValue(x, y, imageWidth, booleanMap2);

			if(booleanMap1Val || booleanMap2Val)
			{
				booleanMap1PlusBooleanMap2Val = true;
			}
			else
			{
				booleanMap1PlusBooleanMap2Val = false;
			}

			setBooleanMapValue(x, y, imageWidth, booleanMap1PlusBooleanMap2Val, booleanMap1PlusBooleanMap2);

		}
	}
}




	//NB this function uses createContrastMapFromMap... (however it really should
void createDepthGradientContrastMapFromDepthGradientMap(int imageWidth, int imageHeight, double* depthGradientMap, double* depthGradientContrastMap)
{
	createDepthGradientContrastMapFromMap(imageWidth, imageHeight, depthGradientMap, depthGradientContrastMap);
}

void createDepthGradientContrastMapFromMap(int imageWidth, int imageHeight, double* depthGradientMap, double* depthGradientContrastMap)
{
	int kernelWidth = DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_WIDTH;
	int kernelHeight = DEFAULT_DEPTH_GRADIENT_MAP_GENERATION_KERNEL_HEIGHT;

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double contrastVal = calculateDepthGradientContrastValueWithinKernel(x, y, depthGradientMap, kernelWidth, kernelHeight, imageWidth, imageHeight);
			setLumOrContrastOrDepthMapValue(x, y, imageWidth, contrastVal, depthGradientContrastMap);
		}
	}

}



double calculateDepthGradientContrastValueWithinKernel(int pixelX, int pixelY, double* depthGradientMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	//METHOD1/2
	double contrastLevel = 0;

	if((kernelWidth >= 3) && (kernelHeight >= 3))
	{//depth gradient contrast map is defined for pixels
		for(int y = pixelY-(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)); y<= pixelY+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)); y=y+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)))
		{
			for(int x = pixelX-(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)); x<= pixelX+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)); x=x+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)))
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
							//METHOD2OLD - works but does not detect features;

							vec kernelCurrentPixelPositionInDepthGradientMapGradient;
							getXYvectorMapValue(x, y, imageWidth, depthGradientMap, &kernelCurrentPixelPositionInDepthGradientMapGradient);
							vec centrePixelPositionInDepthGradientMapGradient;
							getXYvectorMapValue(pixelX, pixelY, imageWidth, depthGradientMap, &centrePixelPositionInDepthGradientMapGradient);

							double currentContrastLevel = 0.0;
							double currentContrastLevelX = absDouble(centrePixelPositionInDepthGradientMapGradient.x - kernelCurrentPixelPositionInDepthGradientMapGradient.x);
							double currentContrastLevelY = absDouble(centrePixelPositionInDepthGradientMapGradient.y - kernelCurrentPixelPositionInDepthGradientMapGradient.y);

							currentContrastLevel = currentContrastLevelX + currentContrastLevelY;
							if(OR_USE_CONTRAST_CALC_METHOD_B)
							{
								contrastLevel = contrastLevel + absDouble(contrastLevel - currentContrastLevel);
							}
							else
							{
								contrastLevel = maxDouble(contrastLevel, currentContrastLevel);
							}

							/*
							//METHOD1OLD;
							vec kernelCurrentPixelPositionInDepthGradientMapGradient;
							getXYvectorMapValue(x, y, imageWidth, depthGradientMap, &kernelCurrentPixelPositionInDepthGradientMapGradient);
							vec centrePixelPositionInDepthGradientMapGradient;
							getXYvectorMapValue(pixelX, pixelY, imageWidth, depthGradientMap, &centrePixelPositionInDepthGradientMapGradient);

							double currentContrastLevel = 0.0;
							if((centrePixelPositionInDepthGradientMapGradient.x > 0) && (kernelCurrentPixelPositionInDepthGradientMapGradient.x < 0))
							{
								currentContrastLevel = currentContrastLevel + absDouble(centrePixelPositionInDepthGradientMapGradient.x - kernelCurrentPixelPositionInDepthGradientMapGradient.x);
							}
							else if((centrePixelPositionInDepthGradientMapGradient.x < 0) && (kernelCurrentPixelPositionInDepthGradientMapGradient.x > 0))
							{
								currentContrastLevel = currentContrastLevel + absDouble(centrePixelPositionInDepthGradientMapGradient.x - kernelCurrentPixelPositionInDepthGradientMapGradient.x);
							}

							if((centrePixelPositionInDepthGradientMapGradient.y > 0) && (kernelCurrentPixelPositionInDepthGradientMapGradient.y < 0))
							{
								currentContrastLevel = currentContrastLevel + absDouble(centrePixelPositionInDepthGradientMapGradient.y - kernelCurrentPixelPositionInDepthGradientMapGradient.y);
							}
							else if((centrePixelPositionInDepthGradientMapGradient.y < 0) && (kernelCurrentPixelPositionInDepthGradientMapGradient.y > 0))
							{
								currentContrastLevel = currentContrastLevel + absDouble(centrePixelPositionInDepthGradientMapGradient.y - kernelCurrentPixelPositionInDepthGradientMapGradient.y);
							}
							contrastLevel = maxDouble(contrastLevel, currentContrastLevel);
							*/

						}
					}
				}
			}
		}
	}
	else if((kernelWidth == 2) && (kernelHeight == 2))
	{//NEW: depth gradient contrast map is defined for pixels, with the first and last row and first and last column of the depth gradient contrast map being null defined

		if((pixelY > 0) && (pixelY < (imageHeight-1)))
		{//check kernel pix is within image

			if((pixelX > 0) && (pixelX < (imageWidth-1)))
			{//check kernel pix is within image

				for(int y = (pixelY-1); y<= (pixelY+1)-1; y++)
				{
					vec interpixelGradientValue;
					getXYvectorMapValue((pixelX-1), y, imageWidth, depthGradientMap, &interpixelGradientValue);
					vec adjacentInterpixelGradientValue;
					getXYvectorMapValue((pixelX-1)+1, y, imageWidth, depthGradientMap, &adjacentInterpixelGradientValue);

					double currentContrastLevelX = absDouble(interpixelGradientValue.x - adjacentInterpixelGradientValue.x);
					double currentContrastLevelY = absDouble(interpixelGradientValue.y - adjacentInterpixelGradientValue.y);
					double currentContrastLevel = currentContrastLevelX + currentContrastLevelY;

					contrastLevel = contrastLevel + currentContrastLevel;
				}

				for(int x = (pixelX-1); x<= (pixelX+1)-1; x++)
				{
					vec interpixelGradientValue;
					getXYvectorMapValue(x, (pixelY-1), imageWidth, depthGradientMap, &interpixelGradientValue);
					vec adjacentInterpixelGradientValue;
					getXYvectorMapValue(x, (pixelY-1)+1, imageWidth, depthGradientMap, &adjacentInterpixelGradientValue);

					double currentContrastLevelX = absDouble(interpixelGradientValue.x - adjacentInterpixelGradientValue.x);
					double currentContrastLevelY = absDouble(interpixelGradientValue.y - adjacentInterpixelGradientValue.y);
					double currentContrastLevel = currentContrastLevelX + currentContrastLevelY;

					contrastLevel = contrastLevel + currentContrastLevel;
				}
			}
			else
			{
				contrastLevel = MAP_VALUE_OUT_OF_RANGE;
			}
		}
		else
		{
			contrastLevel = MAP_VALUE_OUT_OF_RANGE;
		}
	}
	else
	{
		cout << "error: invalid contrast map generation kernel width and/or height" << endl;
		exit(0);
	}

	return contrastLevel;
}


double calculateDepthGradientContrastValueWithinKernelWRONG(int pixelX, int pixelY, double* depthGradientMap, int kernelWidth, int kernelHeight, int imageWidth, int imageHeight)
{
	//METHOD3;
	#define DEPTH_GRADIENT_SIMILARITY_INDICATOR_UNDEFINED (0)
	//double* depthGradientList = new double[DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES* DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES* XY_VECTOR_MAP_NUM_DIMENSIONS];
	int depthGradientSimilarityArray[DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES* DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES];
	for(int y = 0; y<DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES; y++)
	{
		for(int x = 0; x<DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES; x++)
		{
			depthGradientSimilarityArray[y*DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES+x] = DEPTH_GRADIENT_SIMILARITY_INDICATOR_UNDEFINED;
		}
	}

	int currentDepthGradientSimilarityIndicator = 1;

	for(int y = pixelY-(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)); y<= pixelY+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)); y=y+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)))
	{
		for(int x = pixelX-(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)); x<= pixelX+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)); x=x+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)))
		{
			if((y >= 0) && (y <imageHeight))
			{//check kernel pix is within image

				if((x >= 0) && (x <imageWidth))
				{//check kernel pix is within image



					int yDiff = y-pixelY+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1));
					int xDiff = x-pixelX+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1));

					bool foundAMatch = false;

					#ifdef OR_DEBUG
					//cout << "yDiff  = " << yDiff << endl;
					//cout << "xDiff  = " << xDiff << endl;
					#endif

					if(depthGradientSimilarityArray[yDiff*DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES+xDiff] == DEPTH_GRADIENT_SIMILARITY_INDICATOR_UNDEFINED)
					{
						vec kernelCurrentPixelPositionInDepthGradientMapGradient;
						getXYvectorMapValue(x, y, imageWidth, depthGradientMap, &kernelCurrentPixelPositionInDepthGradientMapGradient);


						for(int y2 = pixelY-(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)); y2<= pixelY+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)); y2=y2+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1)))
						{
							for(int x2 = pixelX-(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)); x2<= pixelX+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)); x2=x2+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1)))
							{
								if((y2 >= 0) && (y2 <imageHeight))
								{//check kernel pix is within image

									if((x2 >= 0) && (x2 <imageWidth))
									{//check kernel pix is within image



										int yDiff2 = y2-pixelY+(kernelHeight/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_HEIGHT_NUMBER_SAMPLES-1));
										int xDiff2 = x2-pixelX+(kernelWidth/(DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES-1));

										#ifdef OR_DEBUG
										//cout << "xDiff2  = " << xDiff2 << endl;
										//cout << "yDiff2  = " << yDiff2 << endl;
										#endif

										if(depthGradientSimilarityArray[yDiff2*DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES+xDiff2] == DEPTH_GRADIENT_SIMILARITY_INDICATOR_UNDEFINED)
										{
											vec kernelCurrentPixelPositionInDepthGradientMapGradient2;
											getXYvectorMapValue(x2, y2, imageWidth, depthGradientMap, &kernelCurrentPixelPositionInDepthGradientMapGradient2);

											double currentContrastLevelX = kernelCurrentPixelPositionInDepthGradientMapGradient2.x - kernelCurrentPixelPositionInDepthGradientMapGradient.x;
											double currentContrastLevelY = kernelCurrentPixelPositionInDepthGradientMapGradient2.y - kernelCurrentPixelPositionInDepthGradientMapGradient.y;
											if(!checkIfMeetDepthGradientContrastThreshold(absDouble(currentContrastLevelX)) && !checkIfMeetDepthGradientContrastThreshold(absDouble(currentContrastLevelY)))
											{//depth gradients are the same for pixel and pixel2
												depthGradientSimilarityArray[yDiff*DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES+xDiff] = currentDepthGradientSimilarityIndicator;
												depthGradientSimilarityArray[yDiff2*DEPTH_GRADIENT_CONTRAST_MAP_KERNEL_WIDTH_NUMBER_SAMPLES+xDiff2] = currentDepthGradientSimilarityIndicator;
												foundAMatch = true;
											}
											else
											{//depth gradients are different for pixel1 and pixel2; do nothing at this time

											}
										}
										else
										{
											//similarity matches have already been determined for this pixel2
										}
									}
								}
							}
						}

					}
					else
					{
						//similarity matches have already been determined for this pixel
					}

					if(foundAMatch)
					{
						currentDepthGradientSimilarityIndicator++;
					}
				}
			}
		}
	}


	/*
	check for FEATURE Scenarios;

	corner scenario A;

	* 1	* 1	* 1
	_________
	* 2 	* 3	* 1
		/
	* 5	* 4	* 1

	corner scenario B;

  ---__	* 1	* 2	* 1 ___----
	  ---- _____------  // / //
	* 3	* 1	* 5 // //  //
		| // / // / // /
	* 3	* 4	* 5 / /// /
		|  // // // // //
		|  ///  // // /

	So either A) there are 3->5 corned pixels of same distinct depth gradient, or B) 5 or more distinct depth gradients within 3x3 kernel, then a corner has been found.

	*/

	double contrastLevel = 0.0;

	int maxDepthGradientSimilarityIndicator = currentDepthGradientSimilarityIndicator;

	if(maxDepthGradientSimilarityIndicator >=3)	//default; >= 5
	{
		//corner scenario B met
		contrastLevel = ESTIMATE_MAX_DEPTH_GRADIENT_CONTRAST;
	}
	else
	{

	}

	return contrastLevel;
}










bool checkIfMeetDepthGradientContrastThreshold(double depthGradientContrastValue)
{
	if(depthGradientContrastValue > DEPTH_GRADIENT_CONTRAST_THRESHOLD)
	{
		return true;
	}
	else
	{
		return false;
	}

}



void createDepthGradientContrastBooleanMap(int imageWidth, int imageHeight, double* depthGradientContrastMap, bool* depthGradientContrastBooleanMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double depthGradientContrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthGradientContrastMap);
			bool depthGradientContrastValPassedThreshold = false;
			if(depthGradientContrastVal > EDGE_DEPTH_GRADIENT_CONTRAST_THRESHOLD)
			{
				depthGradientContrastValPassedThreshold = true;
			}
			else
			{
				depthGradientContrastValPassedThreshold = false;
			}
			setBooleanMapValue(x, y, imageWidth, depthGradientContrastValPassedThreshold, depthGradientContrastBooleanMap);
		}
	}
}


void createDepthContrastBooleanMap(int imageWidth, int imageHeight, double* depthContrastMap, bool* depthContrastBooleanMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double depthContrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthContrastMap);
			bool depthContrastValPassedThreshold = false;

			if(depthContrastVal > EDGE_DEPTH_CONTRAST_THRESHOLD)
			{
				if(OR_USE_CONTRAST_CALC_METHOD_C)
				{
					if(depthContrastVal != MAP_VALUE_OUT_OF_RANGE)
					{
						depthContrastValPassedThreshold = true;
					}
					else
					{
						depthContrastValPassedThreshold = false;
					}
				}
				else
				{
					depthContrastValPassedThreshold = true;
				}
			}
			else
			{
				depthContrastValPassedThreshold = false;
			}
			setBooleanMapValue(x, y, imageWidth, depthContrastValPassedThreshold, depthContrastBooleanMap);
		}
	}
}



void createArbitraryContrastBooleanMap(int imageWidth, int imageHeight, double* contrastMap, bool* contrastBooleanMap, double sensitivity)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double contrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, contrastMap);
			bool contrastValPassedThreshold = false;
			if(contrastVal > sensitivity)
			{
				if(OR_USE_CONTRAST_CALC_METHOD_C)
				{
					if(contrastVal != MAP_VALUE_OUT_OF_RANGE)
					{
						contrastValPassedThreshold = true;
					}
					else
					{
						contrastValPassedThreshold = false;
					}
				}
				else
				{
					contrastValPassedThreshold = true;
				}
			}
			else
			{
				contrastValPassedThreshold = false;
			}
			setBooleanMapValue(x, y, imageWidth, contrastValPassedThreshold, contrastBooleanMap);
		}
	}
}






void createPointNormalContrastBooleanMap(int imageWidth, int imageHeight, double* pointNormalContrastMap, bool* pointNormalContrastBooleanMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double pointNormalContrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, pointNormalContrastMap);
			bool pointNormalContrastValPassedThreshold = false;

			#ifdef OR_DEBUG
			/*
			cout << "pointNormalContrastVal = " << pointNormalContrastVal << endl;
			cout << "EDGE_NORMAL_CONTRAST_THRESHOLD = " << EDGE_NORMAL_CONTRAST_THRESHOLD << endl;
			*/
			#endif

			if(pointNormalContrastVal > EDGE_NORMAL_CONTRAST_THRESHOLD)
			{
				pointNormalContrastValPassedThreshold = true;

			}
			else
			{
				pointNormalContrastValPassedThreshold = false;
			}
			setBooleanMapValue(x, y, imageWidth, pointNormalContrastValPassedThreshold, pointNormalContrastBooleanMap);
		}
	}
}

void generateRGBmapFromPointNormalContrastMap(int imageWidth, int imageHeight, double* pointNormalContrastMap, unsigned char* rgbMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
  		for(int x = 0; x < imageWidth; x++)
		{
			double pointNormalContrastVal;
			pointNormalContrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, pointNormalContrastMap);

			int pointNormalContrastValNormalised = minInt((int)(pointNormalContrastVal/MAX_NORMAL_CONTRAST*MAX_RGB_VAL), MAX_RGB_VAL);
			#ifdef USE_CONTRAST_PIXMAP_INVERSE
			pointNormalContrastValNormalised = -(pointNormalContrastValNormalised-MAX_RGB_VAL);
			#endif

			colour col;
			col.r = pointNormalContrastValNormalised;
			col.g = pointNormalContrastValNormalised;
			col.b = pointNormalContrastValNormalised;
			#ifdef OR_DEBUG
			//cout << "pointNormalContrastVal = " << pointNormalContrastVal << endl;
			//cout << "pointNormalContrastVal col.r = " << (int)(col.r) << endl;
			#endif

			setRGBMapValues(x, y, imageWidth, &col, rgbMap);
		}
	}
}


void generatePixmapFromPointNormalContrastMap(string imageFileName, int imageWidth, int imageHeight, double* pointNormalContrastMap)
{
	int x,y;
	pixmap* pm;

	pm = newPixmap(imageWidth, imageHeight);

	for(y = 0; y < imageHeight; y++)
	{
  		for(x = 0; x < imageWidth; x++)
		{
			double contrast;
			contrast = getLumOrContrastOrDepthMapValue(x, y, imageWidth, pointNormalContrastMap);
			int contrastNormalised = minInt((int)(contrast/MAX_NORMAL_CONTRAST*MAX_RGB_VAL), MAX_RGB_VAL);
		#ifdef USE_CONTRAST_PIXMAP_INVERSE
			contrastNormalised = -(contrastNormalised-MAX_RGB_VAL);
		#endif
			placepointPPM(pm, x, y, contrastNormalised, contrastNormalised, contrastNormalised);
		}
	}

	writeImage(imageFileName, pm);
}










//NOT YET FINISHED
void createNormalMap(int imageWidth, int imageHeight, double* luminosityContrastMapEye1, double* depthMap, double* depthContrastMap, double* depthGradientContrastMap, double* normalMap)
{

}
//NOT YET FINISHED
double findDepthOfGivenPixel(int xEye1, int yEye1, int imageWidth, int imageHeight, double* luminosityContrastMapEye1, double* luminosityContrastMapEye2, unsigned char* rgbMapEye1, unsigned char* rgbMapEye2, double* calculatedxEye2, double* calculatedyEye2)
{
	double calculatedDepthOfEye1Pixel;

	colour col;
	double contrastVal;
	getRGBMapValues(xEye1, yEye1, imageWidth, rgbMapEye1, &col);
	contrastVal = getLumOrContrastOrDepthMapValue(xEye1, yEye1, imageWidth, luminosityContrastMapEye1);

	//now find this corresponding contrast val in map 2 [and ensure the RGB ratio is the same - NB cannot compare RGB values of same pixel from both eyes due to specular highlighting]
	return 0.0;
}




//this function should probably be moved elsewhere
void resampleRGBmap(unsigned char* rgbMap, int imageWidth, int imageHeight, unsigned char* resampledRGBmapAtDesiredZoomChar, int zoom, int ignoreBackgroundComparisonMethod)
{
	int resampledWidth = (imageWidth/zoom);
	int resampledHeight = (imageHeight/zoom);

	int* resampledRGBMapAtDesiredzoomInt = new int[resampledWidth*resampledHeight*RGB_NUM];
	bool* resampledRGBMapAtDesiredzoomBool = new bool[resampledWidth*resampledHeight];

	if(ignoreBackgroundComparisonMethod == OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE_COMPLETELY)
	{
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledRGBMapAtDesiredzoomBool[y*resampledWidth + x] = true;
			}
		}
	}

	int* resampledRGBMapAtDesiredzoomCount = new int[resampledWidth*resampledHeight];
	if(ignoreBackgroundComparisonMethod == OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE)
	{
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledRGBMapAtDesiredzoomCount[y*resampledWidth + x] = 0;
			}
		}
	}


	if(zoom > 1)
	{
		//create resampledRGBMapAtDesiredzoomInt

		//initialise resampledRGBMapAtDesiredzoomInt
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = 0.0;
				resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = 0.0;
				resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = 0.0;
			}
		}

		//add to resampledRGBMapAtDesiredzoomInt

		//Method 2; take max value; this will create aliasing but this is better than gray values on corner edges
		/*
		for(int y = 0; y < imageHeight; y++)
		{
  			for(int x = 0; x < imageWidth; x++)
			{

				//Method 2; take max value; this will create aliasing but this is better than gray values on corner edges
				int lumKernel = rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_RED] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE];
				int lumCurrent = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_RED] + resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_GREEN] + resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_BLUE];
				if(lumKernel > lumCurrent)
				{
					resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_RED] = rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_RED];
					resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_GREEN] = rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN];
					resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_BLUE] = rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE];
				}
			}
		}
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED];
				resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN];
				resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE];
			}
		}
		*/

		//Method 1; take average - may not be appropriate for this application
		for(int y = 0; y < imageHeight; y++)
		{
  			for(int x = 0; x < imageWidth; x++)
			{
				if(ignoreBackgroundComparisonMethod == OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE_COMPLETELY)
				{
					if((rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_RED] == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] == OR_SNAPSHOT_BACKGROUND_COLOUR_B))
					{
						resampledRGBMapAtDesiredzoomBool[(y/zoom)*resampledWidth + (x/zoom)] = false;
					}
				}

				if(ignoreBackgroundComparisonMethod == OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE)
				{
					if(!((rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_RED] == OR_SNAPSHOT_BACKGROUND_COLOUR_R) && (rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] == OR_SNAPSHOT_BACKGROUND_COLOUR_G) && (rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] == OR_SNAPSHOT_BACKGROUND_COLOUR_B)))
					{
						resampledRGBMapAtDesiredzoomCount[(y/zoom)*resampledWidth + (x/zoom)] = resampledRGBMapAtDesiredzoomCount[(y/zoom)*resampledWidth + (x/zoom)] + 1;
						resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_RED] = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_RED] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_RED];
						resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_GREEN] = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_GREEN] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN];
						resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_BLUE] = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_BLUE] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE];
					}
				}
				else
				{
					resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_RED] = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_RED] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_RED];
					resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_GREEN] = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_GREEN] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN];
					resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_BLUE] = resampledRGBMapAtDesiredzoomInt[(y/zoom)*resampledWidth*RGB_NUM + (x/zoom)*RGB_NUM + RGB_BLUE] + rgbMap[y*imageWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE];
				}

			}
		}

		//normalise resampledRGBMapAtDesiredzoomInt
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				if(ignoreBackgroundComparisonMethod == OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE)
				{
					if(resampledRGBMapAtDesiredzoomCount[y*resampledWidth + x] != 0)
					{
						resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED]/(resampledRGBMapAtDesiredzoomCount[y*resampledWidth + x]);
						resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN]/(resampledRGBMapAtDesiredzoomCount[y*resampledWidth + x]);
						resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE]/(resampledRGBMapAtDesiredzoomCount[y*resampledWidth + x]);
					}
				}
				else
				{
					resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED]/(zoom*zoom);
					resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN]/(zoom*zoom);
					resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE]/(zoom*zoom);
				}

				if(ignoreBackgroundComparisonMethod == OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_TYPE_IGNORE_COMPLETELY)
				{
					if(resampledRGBMapAtDesiredzoomBool[y*resampledWidth + x] == false)
					{
						resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = (unsigned char)(OR_SNAPSHOT_BACKGROUND_COLOUR_R);
						resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = (unsigned char)(OR_SNAPSHOT_BACKGROUND_COLOUR_G);
						resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = (unsigned char)(OR_SNAPSHOT_BACKGROUND_COLOUR_B);
					}
					else
					{
						resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = (unsigned char)(resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED]);
						resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = (unsigned char)(resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN]);
						resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = (unsigned char)(resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE]);
					}
				}
				else
				{
					resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = (unsigned char)(resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED]);
					resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = (unsigned char)(resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN]);
					resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = (unsigned char)(resampledRGBMapAtDesiredzoomInt[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE]);
				}
			}
		}

	}
	else
	{
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED] = rgbMap[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_RED];
				resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN] = rgbMap[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_GREEN];
				resampledRGBmapAtDesiredZoomChar[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE] = rgbMap[y*resampledWidth*RGB_NUM + x*RGB_NUM + RGB_BLUE];
			}
		}
	}

	delete resampledRGBMapAtDesiredzoomBool;
	delete resampledRGBMapAtDesiredzoomInt;

}




//this function should probably be moved elsewhere
void resampleLumOrContrastOrDepthMap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double* resampledLumOrContrastOrDepthMapAtDesiredZoomChar, int zoom, double offMapValue)
{
	int resampledWidth = (imageWidth/zoom);
	int resampledHeight = (imageHeight/zoom);

	int* resampledMapAtDesiredzoomInt = new int[resampledWidth*resampledHeight];

	bool* resampledMapAtDesiredzoomBool = new bool[resampledWidth*resampledHeight];
	if(OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON)
	{
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledMapAtDesiredzoomBool[y*resampledWidth + x] = true;
			}
		}
	}
#ifdef OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_V2
	int* resampledMapAtDesiredzoomCount = new int[resampledWidth*resampledHeight];
	for(int y = 0; y < resampledHeight; y++)
	{
  		for(int x = 0; x < resampledWidth; x++)
		{
			resampledMapAtDesiredzoomCount[y*resampledWidth + x] = 0;
		}
	}
#endif

	if(zoom > 1)
	{
		//create resampledMapAtDesiredzoomInt
			//initialise resampledMapAtDesiredzoomInt
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledMapAtDesiredzoomInt[y*resampledWidth + x] = 0.0;
			}
		}

		//add to resampledMapAtDesiredzoomInt

		//Method 2; take max value; this will create aliasing but this is better than gray values on corner edges
		/*
		for(int y = 0; y < imageHeight; y++)
		{
  			for(int x = 0; x < imageWidth; x++)
			{

				//Method 2; take max value; this will create aliasing but this is better than gray values on corner edges
				int lumKernel = lumOrContrastOrDepthMap[y*imageWidth + x];
				int lumCurrent = resampledMapAtDesiredzoomInt[(y/zoom)*resampledWidth + (x/zoom)];
				if(lumKernel > lumCurrent)
				{
					resampledMapAtDesiredzoomInt[(y/zoom)*resampledWidth + (x/zoom)] = lumOrContrastOrDepthMap[y*imageWidth + x];
				}
			}
		}

		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledMapAtDesiredzoomChar[y*resampledWidth + x] = resampledMapAtDesiredzoomInt[y*resampledWidth + x];
			}
		}
		*/

		//Method 1; take average - may not be appropriate for this application
		for(int y = 0; y < imageHeight; y++)
		{
  			for(int x = 0; x < imageWidth; x++)
			{
				if(OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON)
				{
					if(compareDoubles(lumOrContrastOrDepthMap[y*imageWidth + x], offMapValue))
					{
						resampledMapAtDesiredzoomBool[(y/zoom)*resampledWidth + (x/zoom)] = false;
					}
				}

			#ifdef OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_V2
				if(!compareDoubles(lumOrContrastOrDepthMap[y*imageWidth + x], offMapValue))
				{
					resampledMapAtDesiredzoomCount[(y/zoom)*resampledWidth + (x/zoom)] = resampledMapAtDesiredzoomCount[(y/zoom)*resampledWidth + (x/zoom)] + 1;
					resampledMapAtDesiredzoomInt[(y/zoom)*resampledWidth + (x/zoom)] = resampledMapAtDesiredzoomInt[(y/zoom)*resampledWidth + (x/zoom)] + lumOrContrastOrDepthMap[y*imageWidth + x];
				}
			#else
				resampledMapAtDesiredzoomInt[(y/zoom)*resampledWidth + (x/zoom)] = resampledMapAtDesiredzoomInt[(y/zoom)*resampledWidth + (x/zoom)] + lumOrContrastOrDepthMap[y*imageWidth + x];
			#endif


			}
		}

		//normalise resampledMapAtDesiredzoomInt
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				#ifdef OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON_V2
				if(!compareDoubles(resampledMapAtDesiredzoomCount[y*resampledWidth + x], offMapValue))
				{
					resampledMapAtDesiredzoomInt[y*resampledWidth + x] = resampledMapAtDesiredzoomInt[y*resampledWidth + x]/(resampledMapAtDesiredzoomCount[y*resampledWidth + x]);
				}
				#else
				resampledMapAtDesiredzoomInt[y*resampledWidth + x] = resampledMapAtDesiredzoomInt[y*resampledWidth + x]/(zoom*zoom);
				#endif

				if(OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON)
				{
					if(resampledMapAtDesiredzoomBool[y*resampledWidth + x] == false)
					{
						resampledLumOrContrastOrDepthMapAtDesiredZoomChar[y*resampledWidth + x] = (unsigned char)(OR_SNAPSHOT_BACKGROUND_COLOUR_R);
					}
					else
					{
						resampledLumOrContrastOrDepthMapAtDesiredZoomChar[y*resampledWidth + x] = (unsigned char)(resampledMapAtDesiredzoomInt[y*resampledWidth + x]);

					}
				}
				else
				{
					resampledLumOrContrastOrDepthMapAtDesiredZoomChar[y*resampledWidth + x] = (unsigned char)(resampledMapAtDesiredzoomInt[y*resampledWidth + x]);

				}
			}
		}


	}
	else
	{
		for(int y = 0; y < resampledHeight; y++)
		{
  			for(int x = 0; x < resampledWidth; x++)
			{
				resampledLumOrContrastOrDepthMapAtDesiredZoomChar[y*resampledWidth + x] = lumOrContrastOrDepthMap[y*resampledWidth + x];
			}
		}
	}

	//#ifdef OR_METHOD_USE_SMALL_IMAGE_RATIO_IGNORE_BG_COMPARISON
	delete resampledMapAtDesiredzoomBool;
	//#endif

	delete resampledMapAtDesiredzoomInt;

}









