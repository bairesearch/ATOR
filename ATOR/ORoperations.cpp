/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ORoperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3d2a 14-April-2014
 *
 *******************************************************************************/

#include "ORglobalDefs.h"
#include "ORoperations.h"
#include "RToperations.h"
#include "SHAREDvector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

/*
#define MIN_OBJECT_DEPTH_AS_DETERMINED_BY_CURRENT_FOCUS (5)
double minObjectDepthAsDeterminedByCurrentFocus = MIN_OBJECT_DEPTH_AS_DETERMINED_BY_CURRENT_FOCUS;
	//this needs to be modified dynamically instead of statically!
*/



void createPointMapFromDepthMap(int imageWidth, int imageHeight, double * depthMap, double * pointMap, ViewInfo * vi)
{
	//fill luminosityMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double depth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			vec xyzWorld;

			if(depth != RT_RAYTRACE_NO_HIT_DEPTH_T)
			{
				calculatePointMapValue(x, y, depth, &xyzWorld, vi);
			}
			else
			{
				xyzWorld.x = 0.0;
				xyzWorld.y = 0.0;
				xyzWorld.z = 0.0;

			}

			setPointMapValue(x, y, imageWidth, &xyzWorld, pointMap);
		}
	}
}

void printvi(ViewInfo * vi)
{
	cout << "printvi()" << endl;

	cout << "vi->viewAt.x = " << vi->viewAt.x << endl;
	cout << "vi->viewAt.y = " << vi->viewAt.y << endl;
	cout << "vi->viewAt.z = " << vi->viewAt.z << endl;
	cout << "vi->viewUp.x = " << vi->viewUp.x << endl;
	cout << "vi->viewUp.y = " << vi->viewUp.y << endl;
	cout << "vi->viewUp.z = " << vi->viewUp.z << endl;
	cout << "vi->eye.x = " << vi->eye.x << endl;
	cout << "vi->eye.y = " << vi->eye.y << endl;
	cout << "vi->eye.z = " << vi->eye.z << endl;
	cout << "vi->imageWidth = " << vi->imageWidth << endl;
	cout << "vi->imageHeight = " << vi->imageHeight << endl;
	cout << "vi->viewWidth = " << vi->viewWidth << endl;
	cout << "vi->viewHeight = " << vi->viewHeight << endl;
	cout << "vi->focalLength = " << vi->focalLength << endl;
}


void printPointMap(int imageWidth, int imageHeight, double * pointMap)
{
	cout << "printPointMap()" << endl;

	//fill luminosityMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			vec xyzWorld;
			getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);

			if(!((xyzWorld.x == RT_RAYTRACE_NO_HIT_POINT_X) && (xyzWorld.x == RT_RAYTRACE_NO_HIT_POINT_Y) && (xyzWorld.x == RT_RAYTRACE_NO_HIT_POINT_Z)))
			{
				cout << xyzWorld.x << " " << xyzWorld.y << " " << xyzWorld.z << endl;
			}
		}
	}
}

void printDepthMap(int imageWidth, int imageHeight, double * depthMap)
{
	cout << "printDepthMap()" << endl;

	//fill luminosityMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			vec xyzWorld;
			double depth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			if(depth != RT_RAYTRACE_NO_HIT_DEPTH_T)
			{
				cout << depth << endl;
			}
		}
	}
}





bool addFeatureToListAndIfCommonFeatureExistsTakeAverage(vec * proposedFeature, Feature * firstFeatureInList, double maxFeatureDistanceError, bool checkAlsoZ)
{
	bool foundCommonFeature = false;

	Feature * currentFeatureInList = firstFeatureInList;

	if(currentFeatureInList->next != NULL)
	{
		while(currentFeatureInList->next != NULL)
		{
			 //METHOD1;
			double distanceBetweenTwoPoints;
			if(checkAlsoZ)
			{
				distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList->point), proposedFeature);
			}
			else
			{
				distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPointsXYOnly(&(currentFeatureInList->point), proposedFeature);
			}

			if(distanceBetweenTwoPoints < maxFeatureDistanceError)
			{
				#ifdef OR_DEBUG
				//cout << "found a fail" << endl;			
				#endif
				int currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature = currentFeatureInList->numberOfFeaturePixelsUsedToGenerateFeature;
				double newx = (currentFeatureInList->point.x + proposedFeature->x/currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature)/(currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature+1)*currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature;
				double newy = (currentFeatureInList->point.y + proposedFeature->y/currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature)/(currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature+1)*currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature;
				double newz = (currentFeatureInList->point.z + proposedFeature->z/currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature)/(currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature+1)*currentNumberOfFeaturePixelsUsedToGenerateConflictingFeature;
				currentFeatureInList->xViewport = newx;
				currentFeatureInList->yViewport = newy;
				currentFeatureInList->point.x = newx;
				currentFeatureInList->point.y = newy;
				currentFeatureInList->point.z = newz;
				currentFeatureInList->numberOfFeaturePixelsUsedToGenerateFeature = currentFeatureInList->numberOfFeaturePixelsUsedToGenerateFeature + 1;

				foundCommonFeature = true;
			}

			currentFeatureInList=currentFeatureInList->next;

		}

		if(!foundCommonFeature)
		{
			currentFeatureInList->numberOfFeaturePixelsUsedToGenerateFeature = 1;
			currentFeatureInList->xViewport = proposedFeature->x;
			currentFeatureInList->yViewport = proposedFeature->y;
			currentFeatureInList->point.x = proposedFeature->x;
			currentFeatureInList->point.y = proposedFeature->y;
			currentFeatureInList->point.z = proposedFeature->z;
			Feature * newFeature = new Feature();
			currentFeatureInList->next = newFeature;
			currentFeatureInList = currentFeatureInList->next;
		}
	}
	else
	{
		currentFeatureInList->numberOfFeaturePixelsUsedToGenerateFeature = 1;
		currentFeatureInList->xViewport = proposedFeature->x;
		currentFeatureInList->yViewport = proposedFeature->y;
		currentFeatureInList->point.x = proposedFeature->x;
		currentFeatureInList->point.y = proposedFeature->y;
		currentFeatureInList->point.z = proposedFeature->z;
		Feature * newFeature = new Feature();
		currentFeatureInList->next = newFeature;
		currentFeatureInList = currentFeatureInList->next;

		//allow first corner to be added without precondition
	}

	return foundCommonFeature;
}



bool checkFeatureListForCommonFeature(vec * corner, Feature * firstFeatureInList, double maxFeatureDistanceError, bool checkAlsoZ)
{
	bool foundCommonFeature = false;

	Feature * currentFeatureInList = firstFeatureInList;

	if(currentFeatureInList->next != NULL)
	{
		while(currentFeatureInList->next != NULL)
		{
			/*
			//METHOD2
			if(compareVectors(&(currentFeatureInList->point), corner))
			{
				foundCommonFeature = true;
			}
			*/

			 //METHOD1;
			double distanceBetweenTwoPoints;
			if(checkAlsoZ)
			{
				distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList->point), corner);
			}
			else
			{
				distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPointsXYOnly(&(currentFeatureInList->point), corner);
			}

			if(distanceBetweenTwoPoints < maxFeatureDistanceError)
			{
				#ifdef OR_DEBUG
				//cout << "found a fail" << endl;			
				#endif
				foundCommonFeature = true;
			}

			currentFeatureInList=currentFeatureInList->next;
		}
	}
	else
	{
		//allow first corner to be added without precondition
	}

	return foundCommonFeature;

}


void generateBooleanMapFromFeatureList(int imageWidth, int imageHeight, Feature * firstFeatureInList,  bool * featuresMap, ViewInfo * vi, int zoom)
{
	//initialise featuresMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, featuresMap);
		}
	}

	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		#ifdef OR_DEBUG
		/*
		cout << "currentFeatureInList->point.x = " << currentFeatureInList->point.x << endl;
		cout << "currentFeatureInList->point.y = " << currentFeatureInList->point.y << endl;
		cout << "x = " << currentFeatureInList->xViewport << endl;
		cout << "y = " << currentFeatureInList->yViewport << endl;
		*/
		#endif

		/*
		double xWorld = currentFeatureInList->point.x;
		double yWorld = currentFeatureInList->point.y;
		int x = (xWorld / XYCOORDINATES_CONVERSION_INTO_PIXELS)+(imageWidth/2);
		int y = (yWorld / XYCOORDINATES_CONVERSION_INTO_PIXELS)+(imageHeight/2);
		now use calculatePointMapValue() instead..
		*/
		int x = currentFeatureInList->xViewport / zoom;
		int y = currentFeatureInList->yViewport / zoom;

		setBooleanMapValue(x, y, imageWidth, true, featuresMap);
		currentFeatureInList=currentFeatureInList->next;
	}
}


void generateBooleanMapFromFeatureListOLD(int imageWidth, int imageHeight, Feature * firstFeatureInList,  bool * featuresMap)
{
	//initialise featuresMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, featuresMap);
		}
	}

	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		int x = currentFeatureInList->point.x;
		int y = currentFeatureInList->point.y;
		setBooleanMapValue(x, y, imageWidth, true, featuresMap);
		currentFeatureInList=currentFeatureInList->next;
	}
}


bool checkPolygonListForCommonPolygon(PolygonBAI * polygon, PolygonBAI * firstPolygonInList)
{
	bool foundCommonPolygon = false;

	PolygonBAI * currentPolygonInList = firstPolygonInList;

	if(currentPolygonInList->next != NULL)
	{
		while(currentPolygonInList->next != NULL)
		{
			bool point1Common = false;
			bool point2Common = false;
			bool point3Common = false;

			if(compareVectors(&(currentPolygonInList->point1), &(polygon->point1)))
			{
				point1Common = true;
				if(compareVectors(&(currentPolygonInList->point2), &(polygon->point2)))
				{
					point2Common = true;
					if(compareVectors(&(currentPolygonInList->point3), &(polygon->point3)))
					{
						point3Common = true;
					}
				}
				else if(compareVectors(&(currentPolygonInList->point2), &(polygon->point3)))
				{
					point2Common = true;
					if(compareVectors(&(currentPolygonInList->point3), &(polygon->point2)))
					{
						point3Common = true;
					}
				}

			}
			else if(compareVectors(&(currentPolygonInList->point1), &(polygon->point2)))
			{
				point1Common = true;
				if(compareVectors(&(currentPolygonInList->point2), &(polygon->point1)))
				{
					point2Common = true;
					if(compareVectors(&(currentPolygonInList->point3), &(polygon->point3)))
					{
						point3Common = true;
					}
				}
				else if(compareVectors(&(currentPolygonInList->point2), &(polygon->point3)))
				{
					point2Common = true;
					if(compareVectors(&(currentPolygonInList->point3), &(polygon->point1)))
					{
						point3Common = true;
					}
				}
			}
			else if(compareVectors(&(currentPolygonInList->point1), &(polygon->point3)))
			{
				point1Common = true;
				if(compareVectors(&(currentPolygonInList->point2), &(polygon->point1)))
				{
					point2Common = true;
					if(compareVectors(&(currentPolygonInList->point3), &(polygon->point2)))
					{
						point3Common = true;
					}
				}
				else if(compareVectors(&(currentPolygonInList->point2), &(polygon->point2)))
				{
					point2Common = true;
					if(compareVectors(&(currentPolygonInList->point3), &(polygon->point1)))
					{
						point3Common = true;
					}
				}

			}

			if(point1Common && point2Common && point3Common)
			{
				foundCommonPolygon = true;
			}

			currentPolygonInList=currentPolygonInList->next;
		}
	}
	else
	{
		//allow first polygon to be added without precondition	
		#ifdef OR_DEBUG
		//cout << "(currentPolygonInList->next == NULL)" << endl;
		#endif
	}

	return foundCommonPolygon;

}

bool checkFeatureListForCommonFeatureBasic(Feature * corner, Feature * firstFeatureInList)
{
	bool foundCommonFeature = false;

	Feature * currentFeatureInList = firstFeatureInList;

	while(currentFeatureInList->next != NULL)
	{
		if(compareVectors(&(currentFeatureInList->point), &(corner->point)))
		{
			foundCommonFeature = true;
		}

		currentFeatureInList=currentFeatureInList->next;
	}

	return foundCommonFeature;
}



PolygonBAI * addPolysToListForGivenFeatureAndNearestFeatureList(PolygonBAI * firstCurrentPolygonInList, PolygonBAI * firstPolygonInList, Feature * firstFeatureInNearestFeatureList, int numberOfPolygonsPerFeature, int dimension)
{
	PolygonBAI * currentPolygonInList = firstCurrentPolygonInList;
	int count = 0;

	Feature * currentFeature = firstFeatureInNearestFeatureList;
	Feature * currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList->next;
	while(currentFeatureInNearestFeatureList->next != NULL)
	{
		Feature * currentFeatureInNearestFeatureList2 = firstFeatureInNearestFeatureList;
		while(currentFeatureInNearestFeatureList2->next != NULL)
		{
			//make sure poly uses different points;
			if(compareDoubles(calculateTheDistanceBetweenTwoPoints(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point)),0.0)
			|| compareDoubles(calculateTheDistanceBetweenTwoPoints(&(currentFeature->point), &(currentFeatureInNearestFeatureList2->point)),0.0)
			|| compareDoubles(calculateTheDistanceBetweenTwoPoints(&(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point)),0.0))
			{
				//polys must use unique features
			}
			else
			{
				bool passedAreaTest = false;

				if(dimension == OR_METHOD3DOD_DIMENSIONS)
				{
					//CHECK THIS - need to invent code for this scenario
					//if(calculateAreaOfTriangle3D(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point)) > OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON)

					if(OR_METHOD_3DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST)
					{
						double areaOfT = calculateAreaOfTriangle(&(currentFeature->pointNonWorldCoord), &(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeatureInNearestFeatureList2->pointNonWorldCoord));

						double internalAngle1 = calculateInteriorAngleOfAPolygonVertex(&(currentFeature->pointNonWorldCoord), &(currentFeatureInNearestFeatureList2->pointNonWorldCoord), &(currentFeatureInNearestFeatureList->pointNonWorldCoord));
						double internalAngle2 = calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeature->pointNonWorldCoord), &(currentFeatureInNearestFeatureList2->pointNonWorldCoord));
						double internalAngle3 = calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList2->pointNonWorldCoord), &(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeature->pointNonWorldCoord));

						bool minAngleTest = true;
						
						#ifdef OR_DEBUG
						/*
						cout << "areaOfT = " << areaOfT << endl;
						cout << "internalAngle1 = " << internalAngle1 << endl;
						cout << "internalAngle2 = " << internalAngle2 << endl;
						cout << "internalAngle3 = " << internalAngle3 << endl;
						*/
						#endif

						double minAngleAllowedRadians = (POLYGON_MIN_ANGLE_DEGREES / 180.0) * PI;
						if((absDouble(internalAngle1) < minAngleAllowedRadians) || (absDouble(internalAngle2) < minAngleAllowedRadians) || (absDouble(internalAngle3) < minAngleAllowedRadians))
						{
							minAngleTest = false;
						}
						if((areaOfT > OR_METHOD3DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON) && (minAngleTest ==  true))
						{
							passedAreaTest = true;
						}
					}
					else
					{
						//basic:
						passedAreaTest = true;
					}
				}
				else if(dimension == OR_METHOD2DOD_DIMENSIONS)
				{
					/*
					double minXOfT = minDouble(minDouble(currentFeature->point.x, currentFeatureInNearestFeatureList->point.x), currentFeatureInNearestFeatureList2->point.x);
					double minYOfT = minDouble(minDouble(currentFeature->point.y, currentFeatureInNearestFeatureList->point.y), currentFeatureInNearestFeatureList2->point.y);
					double maxXOfT = maxDouble(maxDouble(currentFeature->point.x, currentFeatureInNearestFeatureList->point.x), currentFeatureInNearestFeatureList2->point.x);
					double maxYOfT =  maxDouble(maxDouble(currentFeature->point.y, currentFeatureInNearestFeatureList->point.y), currentFeatureInNearestFeatureList2->point.y);
					double widthOfOriginalT = absDouble(minXOfT - maxXOfT);
					double heightOfOriginalT = absDouble(minYOfT - maxYOfT);
					double maxLengthOfT = maxDouble(widthOfOriginalT, heightOfOriginalT);
					double minAcceptableAreaOfT = maxLengthOfT*(maxLengthOfT/OR_METHOD_MINIMUM_SIDE_LENGTH_RATIO_OF_EQUILATERAL_T) / 2.0; //area of t = w*h/2
					double minAcceptableAreaOfTNormalised = minAcceptableAreaOfT*(maxLengthOfT/OR_METHOD_SIDE_LENGTH_FOR_MINIMUM_SIDE_LENGTH_RATIO_OF_EQUILATERAL_T);	//large t's have wider spread features,
					if(minAcceptableAreaOfTNormalised < OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON)
					{
						minAcceptableAreaOfTNormalised = OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON;
					}

					//double maxLengthOfT = maxDouble(widthOfOriginalT, heightOfOriginalT);
					//double averageSideLengthOfMinimumSizedTEquilateral = sqrt(OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON);
					//double averageSideLengthOfMinimumSizedTThin = averageSideLengthOfMinimumSizedTEquilateral*2.0;
					//double areaRequirementNormalisationFactor = maxLengthOfT/averageSideLengthOfMinimumSizedTThin;
					//f(areaOfT/areaRequirementNormalisationFactor > OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON)

					double areaOfT = calculateAreaOfTriangle(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point));
					if(areaOfT > minAcceptableAreaOfTNormalised)
					*/

					if(OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST)
					{
						double areaOfT = calculateAreaOfTriangle(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point));

						double internalAngle1 = calculateInteriorAngleOfAPolygonVertex(&(currentFeature->point), &(currentFeatureInNearestFeatureList2->point), &(currentFeatureInNearestFeatureList->point));
						double internalAngle2 = calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList->point), &(currentFeature->point), &(currentFeatureInNearestFeatureList2->point));
						double internalAngle3 = calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList2->point), &(currentFeatureInNearestFeatureList->point), &(currentFeature->point));

						bool minAngleTest = true;
						double minAngleAllowedRadians = (POLYGON_MIN_ANGLE_DEGREES / 180.0) * PI;
						if((absDouble(internalAngle1) < minAngleAllowedRadians) || (absDouble(internalAngle2) < minAngleAllowedRadians) || (absDouble(internalAngle3) < minAngleAllowedRadians))
						{
							minAngleTest = false;
						}
						if((areaOfT > OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON) && (minAngleTest ==  true))
						{
							passedAreaTest = true;
						}
					}
					else
					{

						//basic:
						double areaOfT = calculateAreaOfTriangle(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point));
						if(areaOfT > OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON)
						{
							passedAreaTest = true;
						}
					}
				}

				if(passedAreaTest)
				{//minimum area of triangle enforcement - eliminates small polys subject to noise, and also eliminates colinear polys

					PolygonBAI tempPolygon;
					copyVectorRT(&(tempPolygon.point1), &(currentFeature->point));
					copyVectorRT(&(tempPolygon.point2), &(currentFeatureInNearestFeatureList->point));
					copyVectorRT(&(tempPolygon.point3), &(currentFeatureInNearestFeatureList2->point));
					if(!checkPolygonListForCommonPolygon(&tempPolygon, firstPolygonInList))
					{
						if(count < numberOfPolygonsPerFeature)
						{
							#ifdef OR_DEBUG
							/*
							cout << "\nnew poly added: " << endl;
							cout << "currentFeatureInNearestFeatureListz->point.x = " << currentFeature->point.x << endl;
							cout << "currentFeatureInNearestFeatureListz->point.y = " << currentFeature->point.y << endl;
							cout << "currentFeatureInNearestFeatureListz->point.z = " << currentFeature->point.z << endl;
							cout << "currentFeatureInNearestFeatureList1->point.x = " << currentFeatureInNearestFeatureList->point.x << endl;
							cout << "currentFeatureInNearestFeatureList1->point.y = " << currentFeatureInNearestFeatureList->point.y << endl;
							cout << "currentFeatureInNearestFeatureList1->point.z = " << currentFeatureInNearestFeatureList->point.z << endl;
							cout << "currentFeatureInNearestFeatureList2->point.x = " << currentFeatureInNearestFeatureList2->point.x << endl;
							cout << "currentFeatureInNearestFeatureList2->point.y = " << currentFeatureInNearestFeatureList2->point.y << endl;
							cout << "currentFeatureInNearestFeatureList2->point.z = " << currentFeatureInNearestFeatureList2->point.z << endl;
							*/
							#endif

							copyVectorRT(&(currentPolygonInList->point1), &(currentFeature->point));
							copyVectorRT(&(currentPolygonInList->point2), &(currentFeatureInNearestFeatureList->point));
							copyVectorRT(&(currentPolygonInList->point3), &(currentFeatureInNearestFeatureList2->point));

							PolygonBAI * newPolygon = new PolygonBAI();
							currentPolygonInList->next = newPolygon;
							currentPolygonInList = currentPolygonInList->next;

							count++;
						}
					}
				}
			}

			currentFeatureInNearestFeatureList2=currentFeatureInNearestFeatureList2->next;
		}

		currentFeatureInNearestFeatureList=currentFeatureInNearestFeatureList->next;
	}

	return currentPolygonInList;
}

int calculateNumberOfNearestFeatures(int numberOfPolygonsPerFeature, int numberOfNearbyFeaturesToTransform)
{
	int numberOfNearestFeatures;
	if(numberOfPolygonsPerFeature == 1)
	{
		numberOfNearestFeatures = 3 + numberOfNearbyFeaturesToTransform;
	}
	else if(numberOfPolygonsPerFeature == 3)
	{
		numberOfNearestFeatures = 3 + maxInt(1, numberOfNearbyFeaturesToTransform);
	}
	else if(numberOfPolygonsPerFeature == 4)
	{
		numberOfNearestFeatures = 3 + maxInt(3, numberOfNearbyFeaturesToTransform);
	}
	else
	{
		cout << "error numberOfPolygonsPerFeature must be <= 4" << endl;
		exit(0);
	}

	return numberOfNearestFeatures;
}




void generateNearestFeaturesList(Feature * firstFeatureInNearestFeatureList, int numberOfNearestFeatures)
{
	Feature * currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList;
	for(int i=0; i<numberOfNearestFeatures; i++)
	{
		Feature * newFeature = new Feature();
		currentFeatureInNearestFeatureList->point.x = REALLY_FAR_AWAY;
		currentFeatureInNearestFeatureList->point.y = REALLY_FAR_AWAY;
		currentFeatureInNearestFeatureList->point.z = REALLY_FAR_AWAY;
		currentFeatureInNearestFeatureList->next = newFeature;
		currentFeatureInNearestFeatureList = currentFeatureInNearestFeatureList->next;
	}
}


bool generatePolygonListUsingFeatureListLocalised(int imageWidth, int imageHeight, Feature firstFeatureInList[], PolygonBAI firstPolygonInList[], int numberOfZoomIndicies, int dimension)
{
	bool result = true;

	for(int trainZoomIndex=0; trainZoomIndex < numberOfZoomIndicies; trainZoomIndex++)
	{
		PolygonBAI * currentPolygonInList = &(firstPolygonInList[trainZoomIndex]);
		Feature * currentFeatureInList1 = &(firstFeatureInList[trainZoomIndex]);

		int cornerIndexList1 = 0;
		while(currentFeatureInList1->next != NULL)
		{
			int numberOfNearestFeatures = calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);

			#ifdef OR_DEBUG
			/*
			cout << "numberOfPolygonsPerFeature = " << NUMBER_OF_POLYGONS_PER_FEATURE << endl;
			cout << "numberOfAdditionalFeatures = " << OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM << endl;
			cout << "numberOfNearestFeatures = " << numberOfNearestFeatures << endl;
			*/
			#endif

			Feature *  firstFeatureInNearestFeatureList = new Feature();
			//generateNearestFeaturesList(firstFeatureInNearestFeatureList, numberOfNearestFeatures);
			firstFeatureInNearestFeatureList->point.x = currentFeatureInList1->point.x;
			firstFeatureInNearestFeatureList->point.y = currentFeatureInList1->point.y;
			firstFeatureInNearestFeatureList->point.z = currentFeatureInList1->point.z;
			firstFeatureInNearestFeatureList->pointNonWorldCoord.x = currentFeatureInList1->pointNonWorldCoord.x;
			firstFeatureInNearestFeatureList->pointNonWorldCoord.y = currentFeatureInList1->pointNonWorldCoord.y;
			firstFeatureInNearestFeatureList->pointNonWorldCoord.z = currentFeatureInList1->pointNonWorldCoord.z;
			Feature * secondFeatureInNearestFeatureList = new Feature();
			firstFeatureInNearestFeatureList->next = secondFeatureInNearestFeatureList;

			double previousDistanceToNearestFeatureFromFeatureList2 = 0.0;

			Feature * currentFeatureInNearestFeatureList = secondFeatureInNearestFeatureList;	//do not use first point
			int currentNearestFeatureIndex = 1;
			bool stillFindingNearestFeatures = true;
			while((currentNearestFeatureIndex < numberOfNearestFeatures) && stillFindingNearestFeatures)
			{
				double distanceToNearestFeatureFromFeatureList2 = REALLY_FAR_AWAY;

				//add corner 2 to nearest corner list
				Feature * currentFeatureInList2 = &(firstFeatureInList[trainZoomIndex]);
				int cornerIndexList2 = 0;
				bool foundASpot = false;

				while(currentFeatureInList2->next != NULL)
				{
					if((calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList2->point), &(currentFeatureInList1->point)) < distanceToNearestFeatureFromFeatureList2)
					&& (calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList2->point), &(currentFeatureInList1->point)) > previousDistanceToNearestFeatureFromFeatureList2))
					{
						if(!checkFeatureListForCommonFeatureBasic(currentFeatureInList2, firstFeatureInNearestFeatureList))
						{//should not be necessary
							if(!compareVectors(&(currentFeatureInList2->point), &(currentFeatureInList1->point)))
							{
								distanceToNearestFeatureFromFeatureList2 = calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList2->point), &(currentFeatureInList1->point));
								copyVectors(&(currentFeatureInNearestFeatureList->point), &(currentFeatureInList2->point));
								copyVectors(&(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeatureInList2->pointNonWorldCoord));
								foundASpot = true;
								
								#ifdef OR_DEBUG
								/*
								cout << "ADDING NEAREST FEATURE:" << endl;
								cout << "currentFeatureInNearestFeatureList->point.x = " << currentFeatureInNearestFeatureList->point.x << endl;
								cout << "currentFeatureInNearestFeatureList->point.y = " << currentFeatureInNearestFeatureList->point.y << endl;
								cout << "currentFeatureInNearestFeatureList->point.z = " << currentFeatureInNearestFeatureList->point.z << endl;
								cout << "currentFeatureInNearestFeatureList->pointNonWorldCoord.x = " << currentFeatureInNearestFeatureList->pointNonWorldCoord.x << endl;
								cout << "currentFeatureInNearestFeatureList->pointNonWorldCoord.y = " << currentFeatureInNearestFeatureList->pointNonWorldCoord.y << endl;
								cout << "currentFeatureInNearestFeatureList->pointNonWorldCoord.z = " << currentFeatureInNearestFeatureList->pointNonWorldCoord.z << endl;
								*/
								#endif
							}
						}
					}

					currentFeatureInList2 = currentFeatureInList2->next;
					cornerIndexList2++;
				}

				previousDistanceToNearestFeatureFromFeatureList2 = distanceToNearestFeatureFromFeatureList2;

				if(!foundASpot)
				{
					stillFindingNearestFeatures = false;
					#ifdef OR_DEBUG
					//cout << "!foundASpot" << endl;
					#endif
				}
				else
				{
					Feature * newFeature = new Feature();
					currentFeatureInNearestFeatureList->next = newFeature;
					currentFeatureInNearestFeatureList = currentFeatureInNearestFeatureList->next;
					currentNearestFeatureIndex++;
				}
			}

			#ifdef OR_DEBUG
			/*
			cout << "DEBUG_NEAREST_FEATURE_FIND:" << endl;
			cout << "currentFeatureInList1->point.x = " << currentFeatureInList1->point.x << endl;
			cout << "currentFeatureInList1->point.y = " << currentFeatureInList1->point.y << endl;
			cout << "currentFeatureInList1->point.z = " << currentFeatureInList1->point.z << endl;
			cout << "currentFeatureInList1->pointNonWorldCoord.x = " << currentFeatureInList1->pointNonWorldCoord.x << endl;
			cout << "currentFeatureInList1->pointNonWorldCoord.y = " << currentFeatureInList1->pointNonWorldCoord.y << endl;
			cout << "currentFeatureInList1->pointNonWorldCoord.z = " << currentFeatureInList1->pointNonWorldCoord.z << endl;
			currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList;
			while(currentFeatureInNearestFeatureList->next != NULL)
			{
				cout << "a nearest corner;" << endl;
				cout << "currentFeatureInNearestFeatureList->point.x = " << currentFeatureInNearestFeatureList->point.x << endl;
				cout << "currentFeatureInNearestFeatureList->point.y = " << currentFeatureInNearestFeatureList->point.y << endl;
				cout << "currentFeatureInNearestFeatureList->point.z = " << currentFeatureInNearestFeatureList->point.z << endl;
				cout << "currentFeatureInNearestFeatureList->pointNonWorldCoord.x = " << currentFeatureInNearestFeatureList->pointNonWorldCoord.x << endl;
				cout << "currentFeatureInNearestFeatureList->pointNonWorldCoord.y = " << currentFeatureInNearestFeatureList->pointNonWorldCoord.y << endl;
				cout << "currentFeatureInNearestFeatureList->pointNonWorldCoord.z = " << currentFeatureInNearestFeatureList->pointNonWorldCoord.z << endl;

				currentFeatureInNearestFeatureList=currentFeatureInNearestFeatureList->next;
			}
			*/
			#endif

			//#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES
			PolygonBAI * backupOfOldCurrentPolygon = currentPolygonInList;
			//#endif

			currentPolygonInList = addPolysToListForGivenFeatureAndNearestFeatureList(currentPolygonInList, firstPolygonInList, firstFeatureInNearestFeatureList, NUMBER_OF_POLYGONS_PER_FEATURE, dimension);

			if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
			{
				PolygonBAI * tempCurrentPolygonInList = backupOfOldCurrentPolygon;
				while(tempCurrentPolygonInList->next != NULL)
				{
					tempCurrentPolygonInList->firstFeatureInNearestFeatureList = firstFeatureInNearestFeatureList;
					tempCurrentPolygonInList = tempCurrentPolygonInList->next;
				}
			}

			if(!OR_METHOD_TRANSFORM_NEARBY_FEATURES)
			{
				delete firstFeatureInNearestFeatureList;
			}

			currentFeatureInList1 = currentFeatureInList1->next;
			cornerIndexList1++;
		}

		#ifdef OR_DEBUG
		/*
		cout << "DEBUG_POLYGON_GENERATION:" << endl;
		int numSidesPerPolygon = 3;
		int numberOfPolygons = 0;
		currentPolygonInList = &(firstPolygonInList[trainZoomIndex]);
		while(currentPolygonInList->next != NULL)
		{
			numberOfPolygons++;
			cout << "polygon = " << numberOfPolygons << endl;
			cout << "currentPolygonInList->point1.x = " << currentPolygonInList->point1.x << endl;
			cout << "currentPolygonInList->point1.y = " << currentPolygonInList->point1.y << endl;
			cout << "currentPolygonInList->point1.z = " << currentPolygonInList->point1.z << endl;
			cout << "currentPolygonInList->point2.x = " << currentPolygonInList->point2.x << endl;
			cout << "currentPolygonInList->point2.y = " << currentPolygonInList->point2.y << endl;
			cout << "currentPolygonInList->point2.z = " << currentPolygonInList->point2.z << endl;
			cout << "currentPolygonInList->point3.x = " << currentPolygonInList->point3.x << endl;
			cout << "currentPolygonInList->point3.y = " << currentPolygonInList->point3.y << endl;
			cout << "currentPolygonInList->point3.z = " << currentPolygonInList->point3.z << endl;

			currentPolygonInList=currentPolygonInList->next;
		}
		cout << "numberOfPolygons = " << numberOfPolygons << endl;
		*/
		#endif
	}

	return result;

}
bool generatePolygonListUsingFeatureList(int imageWidth, int imageHeight, Feature firstFeatureInList[], PolygonBAI firstPolygonInList[], int numberOfZoomIndicies)
{
	bool result = true;

	for(int trainZoomIndex=0; trainZoomIndex < numberOfZoomIndicies; trainZoomIndex++)
	{
		PolygonBAI * currentPolygonInList = &(firstPolygonInList[trainZoomIndex]);
		Feature * currentFeatureInList1 = &(firstFeatureInList[trainZoomIndex]);

		int cornerIndexList1 = 0;
		while(currentFeatureInList1->next != NULL)
		{
			//add corner 2 to polygon
			Feature * currentFeatureInList2 = &(firstFeatureInList[trainZoomIndex]);
			int cornerIndexList2 = 0;
			while(currentFeatureInList2->next != NULL)
			{
				//add corner 3 to polygon
				Feature * currentFeatureInList3 = &(firstFeatureInList[trainZoomIndex]);
				int cornerIndexList3 = 0;
				while(currentFeatureInList3->next != NULL)
				{
					//make sure poly uses different points;
					if((cornerIndexList1 == cornerIndexList2) || (cornerIndexList2 == cornerIndexList3) || (cornerIndexList1 == cornerIndexList3))
					{
						//polys must use unique features
					}
					else
					{
						PolygonBAI tempPolygon;
						copyVectorRT(&(tempPolygon.point1), &(currentFeatureInList1->point));
						copyVectorRT(&(tempPolygon.point2), &(currentFeatureInList2->point));
						copyVectorRT(&(tempPolygon.point3), &(currentFeatureInList3->point));
						if(!checkPolygonListForCommonPolygon(&tempPolygon, firstPolygonInList))
						{
							#ifdef OR_DEBUG
							/*
							cout << "new poly added: " << endl;
							cout << "currentFeatureInList1->point.x = " <<  currentFeatureInList1->point.x << endl;
							cout << "currentFeatureInList1->point.y = " <<  currentFeatureInList1->point.y << endl;
							cout << "currentFeatureInList1->point.z = " <<  currentFeatureInList1->point.z << endl;
							cout << "currentFeatureInList2->point.x = " <<  currentFeatureInList2->point.x << endl;
							cout << "currentFeatureInList2->point.y = " <<  currentFeatureInList2->point.y << endl;
							cout << "currentFeatureInList2->point.z = " <<  currentFeatureInList2->point.z << endl;
							cout << "currentFeatureInList3->point.x = " <<  currentFeatureInList3->point.x << endl;
							cout << "currentFeatureInList3->point.y = " <<  currentFeatureInList3->point.y << endl;
							cout << "currentFeatureInList3->point.z = " <<  currentFeatureInList3->point.z << endl;
							*/
							#endif

							copyVectorRT(&(currentPolygonInList->point1), &(currentFeatureInList1->point));
							copyVectorRT(&(currentPolygonInList->point2), &(currentFeatureInList2->point));
							copyVectorRT(&(currentPolygonInList->point3), &(currentFeatureInList3->point));
							PolygonBAI * newPolygon = new PolygonBAI();
							currentPolygonInList->next = newPolygon;
							currentPolygonInList = currentPolygonInList->next;
						}
					}

					currentFeatureInList3 = currentFeatureInList3->next;
					cornerIndexList3++;
				}

				currentFeatureInList2 = currentFeatureInList2->next;
				cornerIndexList2++;
			}

			currentFeatureInList1 = currentFeatureInList1->next;
			cornerIndexList1++;
		}

		#ifdef OR_DEBUG
		/*
		int numSidesPerPolygon = 3;
		int numberOfPolygons = 0;
		currentPolygonInList = &(firstPolygonInList[trainZoomIndex]);
		while(currentPolygonInList->next != NULL)
		{
			numberOfPolygons++;
			currentPolygonInList=currentPolygonInList->next;
		}
		cout << "numberOfPolygons = " << numberOfPolygons << endl;
		*/
		#endif		
	}

	return result;

}

void generatePolygonsUsingFeatureArraysEfficientNOTCOMPLETE(int imageWidth, int imageHeight, double * depthMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3])
{
	//Added by RBB - for polygon calculations [ONLY USE COMPLETE DATA FOR THESE CALCULATIONS];

	double maxDotProductResultArrayCompleteNearestPoint[3][3][3][3];
	int maxDotProductResultXposArrayCompleteNearestPoint[3][3][3][3];
	int maxDotProductResultYposArrayCompleteNearestPoint[3][3][3][3];

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				for(int commonUnitVector = 0; commonUnitVector < 3; commonUnitVector++)
				{
					maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = MAX_DISTANCE;
					maxDotProductResultXposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = 0;
					maxDotProductResultYposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = 0;
				}
			}
		}
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				vec corner;
				corner.x = maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				corner.y = maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				corner.z = getLumOrContrastOrDepthMapValue(corner.x, corner.y, imageWidth, depthMap);

				//find closest point using with common unitX;
				int commonUnitVector;
				commonUnitVector = VECTOR_VAL_X;

				int uvxIndex2 = uvxIndex;

				for(int uvyIndex2 = 0; uvyIndex2 < 3; uvyIndex2++)
				{
					for(int uvzIndex2 = 0; uvzIndex2 < 3; uvzIndex2++)
					{
						if((uvxIndex == uvxIndex2) && (uvyIndex == uvyIndex2) && (uvzIndex == uvzIndex2))
						{

						}
						else
						{
							vec ambientPoint;
							ambientPoint.x = maxDotProductResultXposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
							ambientPoint.y = maxDotProductResultYposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
							ambientPoint.z = getLumOrContrastOrDepthMapValue(ambientPoint.x, ambientPoint.y, imageWidth, depthMap);

							double distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&corner, &ambientPoint);


							if(distanceBetweenTwoPoints> MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD)
							{
								if(distanceBetweenTwoPoints < maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector])
								{
									maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = distanceBetweenTwoPoints;
									maxDotProductResultXposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = ambientPoint.x;
									maxDotProductResultYposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = ambientPoint.y;

								}
							}
							else
							{
								//identical features
							}
						}
					}
				}

				//find closest point using with common unitY;
				commonUnitVector = VECTOR_VAL_Y;

				for(int uvxIndex2 = 0; uvxIndex2 < 3; uvxIndex2++)
				{
					int uvyIndex2 = uvyIndex;

					for(int uvzIndex2 = 0; uvzIndex2 < 3; uvzIndex2++)
					{
						if((uvxIndex == uvxIndex2) && (uvyIndex == uvyIndex2) && (uvzIndex == uvzIndex2))
						{
							//identical features
						}
						else
						{
							vec ambientPoint;
							ambientPoint.x = maxDotProductResultXposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
							ambientPoint.y = maxDotProductResultYposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
							ambientPoint.z = getLumOrContrastOrDepthMapValue(ambientPoint.x, ambientPoint.y, imageWidth, depthMap);

							double distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&corner, &ambientPoint);

							if(distanceBetweenTwoPoints> MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD)
							{
								if(distanceBetweenTwoPoints < maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector])
								{
									maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = distanceBetweenTwoPoints;
									maxDotProductResultXposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = ambientPoint.x;
									maxDotProductResultYposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = ambientPoint.y;

								}
							}
							else
							{
								//identical features
							}
						}
					}

				}

				//find closest point using with common unitZ;
				commonUnitVector = VECTOR_VAL_Z;

				for(int uvxIndex2 = 0; uvxIndex2 < 3; uvxIndex2++)
				{
					for(int uvyIndex2 = 0; uvyIndex2 < 3; uvyIndex2++)
					{
						int uvzIndex2 = uvzIndex;

						if((uvxIndex == uvxIndex2) && (uvyIndex == uvyIndex2) && (uvzIndex == uvzIndex2))
						{
							//identical features
						}
						else
						{
							vec ambientPoint;
							ambientPoint.x = maxDotProductResultXposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
							ambientPoint.y = maxDotProductResultYposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
							ambientPoint.z = getLumOrContrastOrDepthMapValue(ambientPoint.x, ambientPoint.y, imageWidth, depthMap);

							double distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&corner, &ambientPoint);

							if(distanceBetweenTwoPoints> MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD)
							{
								if(distanceBetweenTwoPoints < maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector])
								{
									maxDotProductResultArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = distanceBetweenTwoPoints;
									maxDotProductResultXposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = ambientPoint.x;
									maxDotProductResultYposArrayCompleteNearestPoint[uvxIndex][uvyIndex][uvzIndex][commonUnitVector] = ambientPoint.y;

								}
							}
							else
							{
								//identical features
							}
						}
					}
				}

			}
		}
	}



	/*

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				vec corner;
				corner.x = maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				corner.y = maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				corner.z = getLumOrContrastOrDepthMapValue(corner.x, corner.y, imageWidth, depthMap);

				//find closest point using with common unitX;
				int commonUnitVector;
				commonUnitVector = VECTOR_VAL_X;

				for(int uvxIndex2 = 0; uvxIndex2 < 3; uvxIndex2++)
				{
					for(int uvyIndex2 = 0; uvyIndex2 < 3; uvyIndex2++)
					{
						for(int uvzIndex2 = 0; uvzIndex2 < 3; uvzIndex2++)
						{
							if((uvxIndex == uvxIndex2) && (uvyIndex == uvyIndex2) && (uvzIndex == uvzIndex2))
							{

							}
							else
							{
								vec ambientPoint;
								ambientPoint.x = maxDotProductResultXposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
								ambientPoint.y = maxDotProductResultYposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
								ambientPoint.z = getLumOrContrastOrDepthMapValue(ambientPoint.x, ambientPoint.y, imageWidth, depthMap);

								double distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&corner, &ambientPoint);

								if(distanceBetweenTwoPoints> MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD)
								{

								}
							}
						}
					}
				}
			}
		}
	}
	*/
}



void createInterpolatedPointMap(int imageWidth, int imageHeight, double * pointMap, double * pointMapInterpolated)
{
	//initialise featuresMap
	for(int y = -1; y < imageHeight; y++)
	{
		for(int x = -1; x < imageWidth; x++)
		{
			vec totalPosition;
			totalPosition.x = 0.0; 	//average position of 4 surrounding point values
			totalPosition.y = 0.0;
			totalPosition.z = 0.0;

			int kx;
			int ky;

			int numberOfPoints = 0;

			for(int ky = y; ky <= y+1; ky++)
			{
				for(int kx = x; kx <= x+1; kx++)
				{
					if((kx == -1) || (ky == -1) || (kx == imageWidth) || (ky == imageHeight))
					{

					}
					else
					{
						vec pointMapValue;
						getPointMapValue(kx, ky, imageWidth, pointMap, &pointMapValue);

						vec nullPointVector;
						nullPointVector.x = 0.0;
						nullPointVector.y = 0.0;
						nullPointVector.z = 0.0;

						if(!compareVectors(&pointMapValue, &nullPointVector))
						{
							addVectorsRT(&totalPosition, &pointMapValue, &totalPosition);
							numberOfPoints++;
						}
					}


				}
			}

			vec interpolatedPosition;
			divideVectorByScalarRT(&totalPosition, numberOfPoints, &interpolatedPosition);
			setPointMapValue((x+1), (y+1), (imageWidth+1), &interpolatedPosition, pointMapInterpolated);
		}
	}
}


/*
void createInterpolatedDepthMap(int imageWidth, int imageHeight, double * depthMap, double * depthMapInterpolated)
{
	//initialise featuresMap
	for(int y = -1; y < imageHeight; y++)
	{
		for(int x = -1; x < imageWidth; x++)
		{
			double totalDepth = 0.0; 	//average depth about 4 surrounding depth values

			int kx;
			int ky;

			int numberOfDepthPoints = 0;

			for(int ky = y; ky <= y+1; ky++)
			{
				for(int kx = x; kx <= x+1; kx++)
				{
					if((kx == -1) || (ky == -1) || (kx == imageWidth) || (ky == imageHeight))
					{

					}
					else
					{
						totalDepth = totalDepth + getLumOrContrastOrDepthMapValue(kx, ky, imageWidth, depthMap);
						numberOfDepthPoints++;
					}
				}
			}

			double interpolatedDepth = totalDepth/(double)numberOfDepthPoints;
			setLumOrContrastOrDepthMapValue((x+1), (y+1), (imageWidth+1), interpolatedDepth, depthMapInterpolated);
		}
	}
}
*/





void applyTransformationMatrixToAllReferencesIn2Dlist(Reference * firstReferenceInInterpolated2DrgbMap, mat * transformationMatrix)
{
	Reference * currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		vec vecNew;
		multiplyVectorByMatrix(&vecNew, &(currentReference->vertex1absolutePosition), transformationMatrix);
		copyVectors(&(currentReference->vertex1absolutePosition), &vecNew);
		multiplyVectorByMatrix(&vecNew, &(currentReference->vertex2absolutePosition), transformationMatrix);
		copyVectors(&(currentReference->vertex2absolutePosition), &vecNew);
		multiplyVectorByMatrix(&vecNew, &(currentReference->vertex3absolutePosition), transformationMatrix);
		copyVectors(&(currentReference->vertex3absolutePosition), &vecNew);
		multiplyVectorByMatrix(&vecNew, &(currentReference->vertex4absolutePosition), transformationMatrix);
		copyVectors(&(currentReference->vertex4absolutePosition), &vecNew);

		currentReference = currentReference->next;
	}
}

void applyTranslationToAllReferencesIn2Dlist(Reference * firstReferenceInInterpolated2DrgbMap, vec * translationVector)
{
	Reference * currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		currentReference->vertex1absolutePosition.x = currentReference->vertex1absolutePosition.x + translationVector->x;
		currentReference->vertex1absolutePosition.y = currentReference->vertex1absolutePosition.y + translationVector->y;
		currentReference->vertex1absolutePosition.z = currentReference->vertex1absolutePosition.z + translationVector->z;
		currentReference->vertex2absolutePosition.x = currentReference->vertex2absolutePosition.x + translationVector->x;
		currentReference->vertex2absolutePosition.y = currentReference->vertex2absolutePosition.y + translationVector->y;
		currentReference->vertex2absolutePosition.z = currentReference->vertex2absolutePosition.z + translationVector->z;
		currentReference->vertex3absolutePosition.x = currentReference->vertex3absolutePosition.x + translationVector->x;
		currentReference->vertex3absolutePosition.y = currentReference->vertex3absolutePosition.y + translationVector->y;
		currentReference->vertex3absolutePosition.z = currentReference->vertex3absolutePosition.z + translationVector->z;
		currentReference->vertex4absolutePosition.x = currentReference->vertex4absolutePosition.x + translationVector->x;
		currentReference->vertex4absolutePosition.y = currentReference->vertex4absolutePosition.y + translationVector->y;
		currentReference->vertex4absolutePosition.z = currentReference->vertex4absolutePosition.z + translationVector->z;

		currentReference = currentReference->next;
	}
}


void restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(Reference * firstReferenceInInterpolated2DrgbMap)
{
	Reference * currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		currentReference->vertex1absolutePosition.x = currentReference->vertex1absolutePositionBackup.x;
		currentReference->vertex1absolutePosition.y = currentReference->vertex1absolutePositionBackup.y;
		currentReference->vertex1absolutePosition.z = currentReference->vertex1absolutePositionBackup.z;
		currentReference->vertex2absolutePosition.x = currentReference->vertex2absolutePositionBackup.x;
		currentReference->vertex2absolutePosition.y = currentReference->vertex2absolutePositionBackup.y;
		currentReference->vertex2absolutePosition.z = currentReference->vertex2absolutePositionBackup.z;
		currentReference->vertex3absolutePosition.x = currentReference->vertex3absolutePositionBackup.x;
		currentReference->vertex3absolutePosition.y = currentReference->vertex3absolutePositionBackup.y;
		currentReference->vertex3absolutePosition.z = currentReference->vertex3absolutePositionBackup.z;
		currentReference->vertex4absolutePosition.x = currentReference->vertex4absolutePositionBackup.x;
		currentReference->vertex4absolutePosition.y = currentReference->vertex4absolutePositionBackup.y;
		currentReference->vertex4absolutePosition.z = currentReference->vertex4absolutePositionBackup.z;

		currentReference = currentReference->next;
	}
}

void storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(Reference * firstReferenceInInterpolated2DrgbMap)
{
	Reference * currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		currentReference->vertex1absolutePositionBackup.x = currentReference->vertex1absolutePosition.x;
		currentReference->vertex1absolutePositionBackup.y = currentReference->vertex1absolutePosition.y;
		currentReference->vertex1absolutePositionBackup.z = currentReference->vertex1absolutePosition.z;
		currentReference->vertex2absolutePositionBackup.x = currentReference->vertex2absolutePosition.x;
		currentReference->vertex2absolutePositionBackup.y = currentReference->vertex2absolutePosition.y;
		currentReference->vertex2absolutePositionBackup.z = currentReference->vertex2absolutePosition.z;
		currentReference->vertex3absolutePositionBackup.x = currentReference->vertex3absolutePosition.x;
		currentReference->vertex3absolutePositionBackup.y = currentReference->vertex3absolutePosition.y;
		currentReference->vertex3absolutePositionBackup.z = currentReference->vertex3absolutePosition.z;
		currentReference->vertex4absolutePositionBackup.x = currentReference->vertex4absolutePosition.x;
		currentReference->vertex4absolutePositionBackup.y = currentReference->vertex4absolutePosition.y;
		currentReference->vertex4absolutePositionBackup.z = currentReference->vertex4absolutePosition.z;

		currentReference = currentReference->next;
	}
}













