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
 * File Name: ATORoperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3n10a 31-October-2020
 * /
 *******************************************************************************/


#include "ATORoperations.hpp"

/*
#define MIN_OBJECT_DEPTH_AS_DETERMINED_BY_CURRENT_FOCUS (5)
double minObjectDepthAsDeterminedByCurrentFocus = MIN_OBJECT_DEPTH_AS_DETERMINED_BY_CURRENT_FOCUS;
	//this needs to be modified dynamically instead of statically!
*/



void ORoperationsClass::createPointMapFromDepthMap(const int imageWidth, const int imageHeight, const double* depthMap, double* pointMap, RTviewInfo* vi)
{
	//fill luminosityMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			vec xyzWorld;

			if(depth != RT_RAYTRACE_NO_HIT_DEPTH_T)
			{
				RTscene.calculatePointMapValue(x, y, depth, &xyzWorld, vi);
			}
			else
			{
				xyzWorld.x = 0.0;
				xyzWorld.y = 0.0;
				xyzWorld.z = 0.0;

			}

			RTpixelMaps.setPointMapValue(x, y, imageWidth, &xyzWorld, pointMap);
		}
	}
}

void ORoperationsClass::printvi(const RTviewInfo* vi)
{
	cout << "printvi{}" << endl;

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


void ORoperationsClass::printPointMap(int imageWidth, const int imageHeight, double* pointMap)
{
	cout << "printPointMap{}" << endl;

	//fill luminosityMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			vec xyzWorld;
			RTpixelMaps.getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);

			if(!((xyzWorld.x == RT_RAYTRACE_NO_HIT_POINT_X) && (xyzWorld.x == RT_RAYTRACE_NO_HIT_POINT_Y) && (xyzWorld.x == RT_RAYTRACE_NO_HIT_POINT_Z)))
			{
				cout << xyzWorld.x << " " << xyzWorld.y << " " << xyzWorld.z << endl;
			}
		}
	}
}

void ORoperationsClass::printDepthMap(const int imageWidth, const int imageHeight, const double* depthMap)
{
	cout << "printDepthMap{}" << endl;

	//fill luminosityMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			vec xyzWorld;
			double depth = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			if(depth != RT_RAYTRACE_NO_HIT_DEPTH_T)
			{
				cout << depth << endl;
			}
		}
	}
}





bool ORoperationsClass::addFeatureToListAndIfCommonFeatureExistsTakeAverage(vec* proposedFeature, ORfeature* firstFeatureInList, const double maxFeatureDistanceError, const bool checkAlsoZ)
{
	bool foundCommonFeature = false;

	ORfeature* currentFeatureInList = firstFeatureInList;

	if(currentFeatureInList->next != NULL)
	{
		while(currentFeatureInList->next != NULL)
		{
			 //METHOD1;
			double distanceBetweenTwoPoints;
			if(checkAlsoZ)
			{
				distanceBetweenTwoPoints = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList->point), proposedFeature);
			}
			else
			{
				distanceBetweenTwoPoints = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&(currentFeatureInList->point), proposedFeature);
			}

			if(distanceBetweenTwoPoints < maxFeatureDistanceError)
			{
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
			ORfeature* newFeature = new ORfeature();
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
		ORfeature* newFeature = new ORfeature();
		currentFeatureInList->next = newFeature;
		currentFeatureInList = currentFeatureInList->next;

		//allow first corner to be added without precondition
	}

	return foundCommonFeature;
}



bool ORoperationsClass::checkFeatureListForCommonFeature(const vec* corner, const ORfeature* firstFeatureInList, const double maxFeatureDistanceError, const bool checkAlsoZ)
{
	bool foundCommonFeature = false;

	const ORfeature* currentFeatureInList = firstFeatureInList;

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
				distanceBetweenTwoPoints = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList->point), corner);
			}
			else
			{
				distanceBetweenTwoPoints = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&(currentFeatureInList->point), corner);
			}

			if(distanceBetweenTwoPoints < maxFeatureDistanceError)
			{
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


void ORoperationsClass::generateBooleanMapFromFeatureList(const int imageWidth, const int imageHeight, const ORfeature* firstFeatureInList,  bool* featuresMap, const RTviewInfo* vi, const int zoom)
{
	//initialise featuresMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			RTpixelMaps.setBooleanMapValue(x, y, imageWidth, false, featuresMap);
		}
	}

	const ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{

		/*
		double xWorld = currentFeatureInList->point.x;
		double yWorld = currentFeatureInList->point.y;
		int x = (xWorld / XYCOORDINATES_CONVERSION_INTO_PIXELS)+(imageWidth/2);
		int y = (yWorld / XYCOORDINATES_CONVERSION_INTO_PIXELS)+(imageHeight/2);
		now use calculatePointMapValue() instead..
		*/
		int x = currentFeatureInList->xViewport / zoom;
		int y = currentFeatureInList->yViewport / zoom;

		RTpixelMaps.setBooleanMapValue(x, y, imageWidth, true, featuresMap);
		currentFeatureInList=currentFeatureInList->next;
	}
}


void ORoperationsClass::generateBooleanMapFromFeatureListOLD(const int imageWidth, const int imageHeight, const ORfeature* firstFeatureInList,  bool* featuresMap)
{
	//initialise featuresMap
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			RTpixelMaps.setBooleanMapValue(x, y, imageWidth, false, featuresMap);
		}
	}

	const ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		int x = currentFeatureInList->point.x;
		int y = currentFeatureInList->point.y;
		RTpixelMaps.setBooleanMapValue(x, y, imageWidth, true, featuresMap);
		currentFeatureInList=currentFeatureInList->next;
	}
}


bool ORoperationsClass::checkPolygonListForCommonPolygon(const ORpolygon* polygon, const ORpolygon* firstPolygonInList)
{
	bool foundCommonPolygon = false;

	const ORpolygon* currentPolygonInList = firstPolygonInList;

	if(currentPolygonInList->next != NULL)
	{
		while(currentPolygonInList->next != NULL)
		{
			bool point1Common = false;
			bool point2Common = false;
			bool point3Common = false;

			if(SHAREDvector.compareVectors(&(currentPolygonInList->point1), &(polygon->point1)))
			{
				point1Common = true;
				if(SHAREDvector.compareVectors(&(currentPolygonInList->point2), &(polygon->point2)))
				{
					point2Common = true;
					if(SHAREDvector.compareVectors(&(currentPolygonInList->point3), &(polygon->point3)))
					{
						point3Common = true;
					}
				}
				else if(SHAREDvector.compareVectors(&(currentPolygonInList->point2), &(polygon->point3)))
				{
					point2Common = true;
					if(SHAREDvector.compareVectors(&(currentPolygonInList->point3), &(polygon->point2)))
					{
						point3Common = true;
					}
				}

			}
			else if(SHAREDvector.compareVectors(&(currentPolygonInList->point1), &(polygon->point2)))
			{
				point1Common = true;
				if(SHAREDvector.compareVectors(&(currentPolygonInList->point2), &(polygon->point1)))
				{
					point2Common = true;
					if(SHAREDvector.compareVectors(&(currentPolygonInList->point3), &(polygon->point3)))
					{
						point3Common = true;
					}
				}
				else if(SHAREDvector.compareVectors(&(currentPolygonInList->point2), &(polygon->point3)))
				{
					point2Common = true;
					if(SHAREDvector.compareVectors(&(currentPolygonInList->point3), &(polygon->point1)))
					{
						point3Common = true;
					}
				}
			}
			else if(SHAREDvector.compareVectors(&(currentPolygonInList->point1), &(polygon->point3)))
			{
				point1Common = true;
				if(SHAREDvector.compareVectors(&(currentPolygonInList->point2), &(polygon->point1)))
				{
					point2Common = true;
					if(SHAREDvector.compareVectors(&(currentPolygonInList->point3), &(polygon->point2)))
					{
						point3Common = true;
					}
				}
				else if(SHAREDvector.compareVectors(&(currentPolygonInList->point2), &(polygon->point2)))
				{
					point2Common = true;
					if(SHAREDvector.compareVectors(&(currentPolygonInList->point3), &(polygon->point1)))
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
	}

	return foundCommonPolygon;

}

bool ORoperationsClass::checkFeatureListForCommonFeatureBasic(const ORfeature* corner, const ORfeature* firstFeatureInList)
{
	bool foundCommonFeature = false;

	const ORfeature* currentFeatureInList = firstFeatureInList;

	while(currentFeatureInList->next != NULL)
	{
		if(SHAREDvector.compareVectors(&(currentFeatureInList->point), &(corner->point)))
		{
			foundCommonFeature = true;
		}

		currentFeatureInList=currentFeatureInList->next;
	}

	return foundCommonFeature;
}



ORpolygon* ORoperationsClass::addPolysToListForGivenFeatureAndNearestFeatureList(ORpolygon* firstCurrentPolygonInList, const ORpolygon* firstPolygonInList, ORfeature* firstFeatureInNearestFeatureList, const int numberOfPolygonsPerFeature, const int dimension)
{
	ORpolygon* currentPolygonInList = firstCurrentPolygonInList;
	int count = 0;

	ORfeature* currentFeature = firstFeatureInNearestFeatureList;
	ORfeature* currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList->next;
	while(currentFeatureInNearestFeatureList->next != NULL)
	{
		ORfeature* currentFeatureInNearestFeatureList2 = firstFeatureInNearestFeatureList;
		while(currentFeatureInNearestFeatureList2->next != NULL)
		{
			//make sure poly uses different points;
			if(SHAREDvars.compareDoubles(SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point)),0.0)
			|| SHAREDvars.compareDoubles(SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeature->point), &(currentFeatureInNearestFeatureList2->point)),0.0)
			|| SHAREDvars.compareDoubles(SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point)),0.0))
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
						double areaOfT = SHAREDvector.calculateAreaOfTriangle(&(currentFeature->pointNonWorldCoord), &(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeatureInNearestFeatureList2->pointNonWorldCoord));

						double internalAngle1 = SHAREDvector.calculateInteriorAngleOfAPolygonVertex(&(currentFeature->pointNonWorldCoord), &(currentFeatureInNearestFeatureList2->pointNonWorldCoord), &(currentFeatureInNearestFeatureList->pointNonWorldCoord));
						double internalAngle2 = SHAREDvector.calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeature->pointNonWorldCoord), &(currentFeatureInNearestFeatureList2->pointNonWorldCoord));
						double internalAngle3 = SHAREDvector.calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList2->pointNonWorldCoord), &(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeature->pointNonWorldCoord));

						bool minAngleTest = true;


						double minAngleAllowedRadians = (POLYGON_MIN_ANGLE_DEGREES / 180.0)* PI;
						if((SHAREDvars.absDouble(internalAngle1) < minAngleAllowedRadians) || (SHAREDvars.absDouble(internalAngle2) < minAngleAllowedRadians) || (SHAREDvars.absDouble(internalAngle3) < minAngleAllowedRadians))
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
					double widthOfOriginalT = SHAREDvars.absDouble(minXOfT - maxXOfT);
					double heightOfOriginalT = SHAREDvars.absDouble(minYOfT - maxYOfT);
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

					double areaOfT = SHAREDvector.calculateAreaOfTriangle(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point));
					if(areaOfT > minAcceptableAreaOfTNormalised)
					*/

					if(OR_METHOD_2DOD_USE_POLYGON_MIN_MAX_INTERNAL_ANGLE_TEST)
					{
						double areaOfT = SHAREDvector.calculateAreaOfTriangle(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point));

						double internalAngle1 = SHAREDvector.calculateInteriorAngleOfAPolygonVertex(&(currentFeature->point), &(currentFeatureInNearestFeatureList2->point), &(currentFeatureInNearestFeatureList->point));
						double internalAngle2 = SHAREDvector.calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList->point), &(currentFeature->point), &(currentFeatureInNearestFeatureList2->point));
						double internalAngle3 = SHAREDvector.calculateInteriorAngleOfAPolygonVertex(&(currentFeatureInNearestFeatureList2->point), &(currentFeatureInNearestFeatureList->point), &(currentFeature->point));

						bool minAngleTest = true;
						double minAngleAllowedRadians = (POLYGON_MIN_ANGLE_DEGREES / 180.0)* PI;
						if((SHAREDvars.absDouble(internalAngle1) < minAngleAllowedRadians) || (SHAREDvars.absDouble(internalAngle2) < minAngleAllowedRadians) || (SHAREDvars.absDouble(internalAngle3) < minAngleAllowedRadians))
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
						double areaOfT = SHAREDvector.calculateAreaOfTriangle(&(currentFeature->point), &(currentFeatureInNearestFeatureList->point), &(currentFeatureInNearestFeatureList2->point));
						if(areaOfT > OR_METHOD2DOD_MINIMUM_AREA_OF_NORMALISATION_POLYGON)
						{
							passedAreaTest = true;
						}
					}
				}

				if(passedAreaTest)
				{//minimum area of triangle enforcement - eliminates small polys subject to noise, and also eliminates colinear polys

					ORpolygon tempPolygon;
					SHAREDvector.copyVectorRT(&(tempPolygon.point1), &(currentFeature->point));
					SHAREDvector.copyVectorRT(&(tempPolygon.point2), &(currentFeatureInNearestFeatureList->point));
					SHAREDvector.copyVectorRT(&(tempPolygon.point3), &(currentFeatureInNearestFeatureList2->point));
					if(!checkPolygonListForCommonPolygon(&tempPolygon, firstPolygonInList))
					{
						if(count < numberOfPolygonsPerFeature)
						{

							SHAREDvector.copyVectorRT(&(currentPolygonInList->point1), &(currentFeature->point));
							SHAREDvector.copyVectorRT(&(currentPolygonInList->point2), &(currentFeatureInNearestFeatureList->point));
							SHAREDvector.copyVectorRT(&(currentPolygonInList->point3), &(currentFeatureInNearestFeatureList2->point));

							ORpolygon* newPolygon = new ORpolygon();
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

int ORoperationsClass::calculateNumberOfNearestFeatures(const int numberOfPolygonsPerFeature, const int numberOfNearbyFeaturesToTransform)
{
	int numberOfNearestFeatures;
	if(numberOfPolygonsPerFeature == 1)
	{
		numberOfNearestFeatures = 3 + numberOfNearbyFeaturesToTransform;
	}
	else if(numberOfPolygonsPerFeature == 3)
	{
		numberOfNearestFeatures = 3 + SHAREDvars.maxInt(1, numberOfNearbyFeaturesToTransform);
	}
	else if(numberOfPolygonsPerFeature == 4)
	{
		numberOfNearestFeatures = 3 + SHAREDvars.maxInt(3, numberOfNearbyFeaturesToTransform);
	}
	else
	{
		cerr << "error numberOfPolygonsPerFeature must be <= 4" << endl;
		exit(EXIT_ERROR);
	}

	return numberOfNearestFeatures;
}




void ORoperationsClass::generateNearestFeaturesList(ORfeature* firstFeatureInNearestFeatureList, const int numberOfNearestFeatures)
{
	ORfeature* currentFeatureInNearestFeatureList = firstFeatureInNearestFeatureList;
	for(int i=0; i<numberOfNearestFeatures; i++)
	{
		ORfeature* newFeature = new ORfeature();
		currentFeatureInNearestFeatureList->point.x = REALLY_FAR_AWAY;
		currentFeatureInNearestFeatureList->point.y = REALLY_FAR_AWAY;
		currentFeatureInNearestFeatureList->point.z = REALLY_FAR_AWAY;
		currentFeatureInNearestFeatureList->next = newFeature;
		currentFeatureInNearestFeatureList = currentFeatureInNearestFeatureList->next;
	}
}


bool ORoperationsClass::generatePolygonListUsingFeatureListLocalised(const int imageWidth, const int imageHeight, const ORfeature firstFeatureInList[], ORpolygon firstPolygonInList[], const int numberOfZoomIndicies, const int dimension)
{
	bool result = true;

	for(int trainZoomIndex=0; trainZoomIndex < numberOfZoomIndicies; trainZoomIndex++)
	{
		ORpolygon* currentPolygonInList = &(firstPolygonInList[trainZoomIndex]);
		const ORfeature* currentFeatureInList1 = &(firstFeatureInList[trainZoomIndex]);

		int cornerIndexList1 = 0;
		while(currentFeatureInList1->next != NULL)
		{
			int numberOfNearestFeatures = calculateNumberOfNearestFeatures(NUMBER_OF_POLYGONS_PER_FEATURE, OR_METHOD_NUM_NEARBY_FEATURES_TO_TRANSFORM);


			ORfeature* firstFeatureInNearestFeatureList = new ORfeature();
			//generateNearestFeaturesList(firstFeatureInNearestFeatureList, numberOfNearestFeatures);
			firstFeatureInNearestFeatureList->point.x = currentFeatureInList1->point.x;
			firstFeatureInNearestFeatureList->point.y = currentFeatureInList1->point.y;
			firstFeatureInNearestFeatureList->point.z = currentFeatureInList1->point.z;
			firstFeatureInNearestFeatureList->pointNonWorldCoord.x = currentFeatureInList1->pointNonWorldCoord.x;
			firstFeatureInNearestFeatureList->pointNonWorldCoord.y = currentFeatureInList1->pointNonWorldCoord.y;
			firstFeatureInNearestFeatureList->pointNonWorldCoord.z = currentFeatureInList1->pointNonWorldCoord.z;
			ORfeature* secondFeatureInNearestFeatureList = new ORfeature();
			firstFeatureInNearestFeatureList->next = secondFeatureInNearestFeatureList;

			double previousDistanceToNearestFeatureFromFeatureList2 = 0.0;

			ORfeature* currentFeatureInNearestFeatureList = secondFeatureInNearestFeatureList;	//do not use first point
			int currentNearestFeatureIndex = 1;
			bool stillFindingNearestFeatures = true;
			while((currentNearestFeatureIndex < numberOfNearestFeatures) && stillFindingNearestFeatures)
			{
				double distanceToNearestFeatureFromFeatureList2 = REALLY_FAR_AWAY;

				//add corner 2 to nearest corner list
				const ORfeature* currentFeatureInList2 = &(firstFeatureInList[trainZoomIndex]);
				int cornerIndexList2 = 0;
				bool foundASpot = false;

				while(currentFeatureInList2->next != NULL)
				{
					if((SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList2->point), &(currentFeatureInList1->point)) < distanceToNearestFeatureFromFeatureList2)
					&& (SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList2->point), &(currentFeatureInList1->point)) > previousDistanceToNearestFeatureFromFeatureList2))
					{
						if(!checkFeatureListForCommonFeatureBasic(currentFeatureInList2, firstFeatureInNearestFeatureList))
						{//should not be necessary
							if(!SHAREDvector.compareVectors(&(currentFeatureInList2->point), &(currentFeatureInList1->point)))
							{
								distanceToNearestFeatureFromFeatureList2 = SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(currentFeatureInList2->point), &(currentFeatureInList1->point));
								SHAREDvector.copyVectors(&(currentFeatureInNearestFeatureList->point), &(currentFeatureInList2->point));
								SHAREDvector.copyVectors(&(currentFeatureInNearestFeatureList->pointNonWorldCoord), &(currentFeatureInList2->pointNonWorldCoord));
								foundASpot = true;

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
				}
				else
				{
					ORfeature* newFeature = new ORfeature();
					currentFeatureInNearestFeatureList->next = newFeature;
					currentFeatureInNearestFeatureList = currentFeatureInNearestFeatureList->next;
					currentNearestFeatureIndex++;
				}
			}


			//#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES
			ORpolygon* backupOfOldCurrentPolygon = currentPolygonInList;
			//#endif

			currentPolygonInList = addPolysToListForGivenFeatureAndNearestFeatureList(currentPolygonInList, firstPolygonInList, firstFeatureInNearestFeatureList, NUMBER_OF_POLYGONS_PER_FEATURE, dimension);

			if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
			{
				ORpolygon* tempCurrentPolygonInList = backupOfOldCurrentPolygon;
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

	}

	return result;

}
bool ORoperationsClass::generatePolygonListUsingFeatureList(const int imageWidth, const int imageHeight, const ORfeature firstFeatureInList[], ORpolygon firstPolygonInList[], const int numberOfZoomIndicies)
{
	bool result = true;

	for(int trainZoomIndex=0; trainZoomIndex < numberOfZoomIndicies; trainZoomIndex++)
	{
		ORpolygon* currentPolygonInList = &(firstPolygonInList[trainZoomIndex]);
		const ORfeature* currentFeatureInList1 = &(firstFeatureInList[trainZoomIndex]);

		int cornerIndexList1 = 0;
		while(currentFeatureInList1->next != NULL)
		{
			//add corner 2 to polygon
			const ORfeature* currentFeatureInList2 = &(firstFeatureInList[trainZoomIndex]);
			int cornerIndexList2 = 0;
			while(currentFeatureInList2->next != NULL)
			{
				//add corner 3 to polygon
				const ORfeature* currentFeatureInList3 = &(firstFeatureInList[trainZoomIndex]);
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
						ORpolygon tempPolygon;
						SHAREDvector.copyVectorRT(&(tempPolygon.point1), &(currentFeatureInList1->point));
						SHAREDvector.copyVectorRT(&(tempPolygon.point2), &(currentFeatureInList2->point));
						SHAREDvector.copyVectorRT(&(tempPolygon.point3), &(currentFeatureInList3->point));
						if(!checkPolygonListForCommonPolygon(&tempPolygon, firstPolygonInList))
						{

							SHAREDvector.copyVectorRT(&(currentPolygonInList->point1), &(currentFeatureInList1->point));
							SHAREDvector.copyVectorRT(&(currentPolygonInList->point2), &(currentFeatureInList2->point));
							SHAREDvector.copyVectorRT(&(currentPolygonInList->point3), &(currentFeatureInList3->point));
							ORpolygon* newPolygon = new ORpolygon();
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

	}

	return result;

}

/*
void generatePolygonsUsingFeatureArraysEfficientNOTCOMPLETE(int imageWidth, int imageHeight, double* depthMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3])
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





//	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
//	{
//		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
//		{
//			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
//			{
//				vec corner;
//				corner.x = maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
//				corner.y = maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
//				corner.z = getLumOrContrastOrDepthMapValue(corner.x, corner.y, imageWidth, depthMap);
//
//				//find closest point using with common unitX;
//				int commonUnitVector;
//				commonUnitVector = VECTOR_VAL_X;
//
//				for(int uvxIndex2 = 0; uvxIndex2 < 3; uvxIndex2++)
//				{
//					for(int uvyIndex2 = 0; uvyIndex2 < 3; uvyIndex2++)
//					{
//						for(int uvzIndex2 = 0; uvzIndex2 < 3; uvzIndex2++)
//						{
//							if((uvxIndex == uvxIndex2) && (uvyIndex == uvyIndex2) && (uvzIndex == uvzIndex2))
//							{
//
//							}
//							else
//							{
//								vec ambientPoint;
//								ambientPoint.x = maxDotProductResultXposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
//								ambientPoint.y = maxDotProductResultYposArrayComplete[uvxIndex2][uvyIndex2][uvzIndex2];
//								ambientPoint.z = getLumOrContrastOrDepthMapValue(ambientPoint.x, ambientPoint.y, imageWidth, depthMap);
//
//								double distanceBetweenTwoPoints = calculateTheDistanceBetweenTwoPoints(&corner, &ambientPoint);
//
//								if(distanceBetweenTwoPoints> MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD)
//								{
//
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
	
}
*/


void ORoperationsClass::createInterpolatedPointMap(int imageWidth, const int imageHeight, double* pointMap, double* pointMapInterpolated)
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
						RTpixelMaps.getPointMapValue(kx, ky, imageWidth, pointMap, &pointMapValue);

						vec nullPointVector;
						nullPointVector.x = 0.0;
						nullPointVector.y = 0.0;
						nullPointVector.z = 0.0;

						if(!SHAREDvector.compareVectors(&pointMapValue, &nullPointVector))
						{
							SHAREDvector.addVectorsRT(&totalPosition, &pointMapValue, &totalPosition);
							numberOfPoints++;
						}
					}


				}
			}

			vec interpolatedPosition;
			SHAREDvector.divideVectorByScalarRT(&totalPosition, numberOfPoints, &interpolatedPosition);
			RTpixelMaps.setPointMapValue((x+1), (y+1), (imageWidth+1), &interpolatedPosition, pointMapInterpolated);
		}
	}
}


/*
void createInterpolatedDepthMap(int imageWidth, int imageHeight, double* depthMap, double* depthMapInterpolated)
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





void ORoperationsClass::applyTransformationMatrixToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, mat* transformationMatrix)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		vec vecNew;
		SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentReference->vertex1absolutePosition), transformationMatrix);
		SHAREDvector.copyVectors(&(currentReference->vertex1absolutePosition), &vecNew);
		SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentReference->vertex2absolutePosition), transformationMatrix);
		SHAREDvector.copyVectors(&(currentReference->vertex2absolutePosition), &vecNew);
		SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentReference->vertex3absolutePosition), transformationMatrix);
		SHAREDvector.copyVectors(&(currentReference->vertex3absolutePosition), &vecNew);
		SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentReference->vertex4absolutePosition), transformationMatrix);
		SHAREDvector.copyVectors(&(currentReference->vertex4absolutePosition), &vecNew);

		currentReference = currentReference->next;
	}
}

void ORoperationsClass::applyTranslationToAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap, vec* translationVector)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
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


void ORoperationsClass::restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
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

void ORoperationsClass::storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(LDreference* firstReferenceInInterpolated2DrgbMap)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
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













