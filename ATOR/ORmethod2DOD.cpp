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
 * File Name: ORmethod2DOD.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3f3a 10-July-2015
 * NB pointmap is a new addition for test streamlining; NB in test scenarios 2 and 3, there will be no pointmap available; the pointmap will have to be generated after depth map is obtained by using calculatePointUsingTInWorld()
 *******************************************************************************/


#include "ORmethod2DOD.h"
#include "SHAREDvector.h"
#include "ORpixelMaps.h"
#include "ORoperations.h"
#ifdef USE_OPENGL
	#include "LDopengl.h"
#endif

double calculateXYorientationOfSide(ORpolygon* currentPolygonInList, int side)
{
	double orientationOfSide;

	if(side == 0)
	{
		orientationOfSide = calculateAngleBetweenVectors2D(&(currentPolygonInList->point1), &(currentPolygonInList->point2));
	}
	else if(side == 1)
	{
		orientationOfSide = calculateAngleBetweenVectors2D(&(currentPolygonInList->point2), &(currentPolygonInList->point3));
	}
	else if(side == 2)
	{
		orientationOfSide = calculateAngleBetweenVectors2D(&(currentPolygonInList->point3), &(currentPolygonInList->point1));
	}
	else
	{
		cout << "error misdefined side" << endl;
		exit(0);
	}
	return orientationOfSide;
}

double calculateXYlengthOfSide(ORpolygon* currentPolygonInList, int side)
{
	double lengthOfSide;

	if(side == 0)
	{
		lengthOfSide = calculateTheDistanceBetweenTwoPointsXYOnly(&(currentPolygonInList->point1), &(currentPolygonInList->point2));
	}
	else if(side == 1)
	{
		lengthOfSide = calculateTheDistanceBetweenTwoPointsXYOnly(&(currentPolygonInList->point2), &(currentPolygonInList->point3));
	}
	else if(side == 2)
	{
		lengthOfSide = calculateTheDistanceBetweenTwoPointsXYOnly(&(currentPolygonInList->point3), &(currentPolygonInList->point1));
	}
	else
	{
		cout << "error misdefined side" << endl;
		exit(0);
	}
	return lengthOfSide;
}

double calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(ORpolygon* transformedObjectTriangle, int side)
{
	double perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide;

	if(side == 0)
	{
		perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide = -(transformedObjectTriangle->point3.y - transformedObjectTriangle->point1.y);	//or transformedObjectTriangle->point2.y - transformedObjectTriangle->point1.y);
	}
	else if(side == 1)
	{
		perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide = -(transformedObjectTriangle->point1.y - transformedObjectTriangle->point2.y);
	}
	else if(side == 2)
	{
		perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide = -(transformedObjectTriangle->point2.y - transformedObjectTriangle->point3.y);
	}
	else
	{
		cout << "error misdefined side" << endl;
		exit(0);
	}



	return perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide;
}

double calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(ORpolygon* transformedObjectTriangle, int side)
{
	double xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex;

	if(side == 0)
	{
		//OLD; 			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point3.x - transformedObjectTriangle->point1.x);	//or transformedObjectTriangle->point2.x - transformedObjectTriangle->point3.x);
		if(transformedObjectTriangle->point1.x < transformedObjectTriangle->point2.x)
		{
			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point3.x - transformedObjectTriangle->point1.x);	//or transformedObjectTriangle->point2.x - transformedObjectTriangle->point3.x);
		}
		else
		{
			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point3.x - transformedObjectTriangle->point2.x);	//or transformedObjectTriangle->point2.x - transformedObjectTriangle->point3.x);
		}
	}
	else if(side == 1)
	{
		//OLD; 			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point1.x - transformedObjectTriangle->point2.x);
		if(transformedObjectTriangle->point2.x < transformedObjectTriangle->point3.x)
		{
			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point1.x - transformedObjectTriangle->point2.x);
		}
		else
		{
			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point1.x - transformedObjectTriangle->point3.x);
		}
	}
	else if(side == 2)
	{
		//OLD; xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point2.x - transformedObjectTriangle->point3.x);	//OLD
		//OLD2; xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point2.x - transformedObjectTriangle->point1.x);

		if(transformedObjectTriangle->point3.x < transformedObjectTriangle->point1.x)
		{
			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex =  (transformedObjectTriangle->point2.x - transformedObjectTriangle->point3.x);
		}
		else
		{
			xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = (transformedObjectTriangle->point2.x - transformedObjectTriangle->point1.x);
		}

	}
	else
	{
		cout << "error misdefined side" << endl;
		exit(0);
	}

	return xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex;
}













void TEMPprintReferenceListVertexPositions2DOD(LDreference* firstReferenceInInterpolated2DrgbMap)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		cout << "TESTX currentReference->vertex1absolutePosition.x  = " << currentReference->vertex1absolutePosition.x  << endl;
		cout << "TESTX currentReference->vertex1absolutePosition.y  = " << currentReference->vertex1absolutePosition.y  << endl;
		cout << "TESTX currentReference->vertex2absolutePosition.x  = " << currentReference->vertex2absolutePosition.x  << endl;
		cout << "TESTX currentReference->vertex2absolutePosition.y  = " << currentReference->vertex2absolutePosition.y  << endl;

		currentReference = currentReference->next;
	}



}



void transformObjectData2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, int side, bool first, ORfeature* firstFeatureInList)
{
	long time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t start: 3ai. 2DOD normalised snapshot generation - transform data wrt polygon - matrix calc" << endl;
		}
		time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart = getTimeAsLong();
	}


	ORpolygon* transformedObjectTriangle = new ORpolygon();	//equilateral triangle
	transformedObjectTriangle->point1.x = currentPolygonInList->point1.x;
	transformedObjectTriangle->point1.y = currentPolygonInList->point1.y;
	transformedObjectTriangle->point2.x = currentPolygonInList->point2.x;
	transformedObjectTriangle->point2.y = currentPolygonInList->point2.y;
	transformedObjectTriangle->point3.x = currentPolygonInList->point3.x;
	transformedObjectTriangle->point3.y = currentPolygonInList->point3.y;


	ORpolygon* predefinedTriangle = new ORpolygon();	//equilateral triangle
	predefinedTriangle->point1.x = 0;
	predefinedTriangle->point1.y = 0;
	predefinedTriangle->point2.x = 1;
	predefinedTriangle->point2.y = 0;
	predefinedTriangle->point3.x = 0.5;
	predefinedTriangle->point3.y = sqrt(pow(1.0,2) - pow(0.5, 2));

	//these additional points go through the same transformation, as they are used to cull all non rendered transformed data (where a square cull instead of a triangular cull is used);
	/*
	vec point4;
	vec point5;
	vec ptp4;
	vec ptp5;
	ptp5.x = 0.0;
	ptp5.y = sqrt(pow(1.0,2) - pow(0.5, 2));
	ptp5.x = 1.0;
	ptp5.y = sqrt(pow(1.0,2) - pow(0.5, 2));
	*/

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "\n \t starting to tranform object triangle, predefinedTriangle->point3.y = " << predefinedTriangle->point3.y << endl;
	cout << "0. currentPolygonInList without transformation " << endl;
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	#endif

	double orientationOfObjectTriangleSide = calculateXYorientationOfSide(currentPolygonInList, side);
	double lengthOfObjectTriangleSide = calculateXYlengthOfSide(currentPolygonInList, side);
	double lengthOfPredefinedTriangleSide = calculateTheDistanceBetweenTwoPointsXYOnly(&(predefinedTriangle->point1), &(predefinedTriangle->point2));	//=1 for equilateral triangle


	//1a. Scale object data such that the object triangle side is of same length as a predefined side of a predefined triangle
	double scaleFactor1a = lengthOfPredefinedTriangleSide/lengthOfObjectTriangleSide;


	mat scaleMatrix1a;
	createScaleMatrix2D(&scaleMatrix1a, scaleFactor1a);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	//openglUseMatrix = true;
	opengl2DmatrixTransformation1ScaleFactor = scaleFactor1a;

	#else
	if(first)
	{
		storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap);
	}
	else
	{
		restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap);
	}
	//applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &scaleMatrix1a);
	#endif

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "1. Scale object data such that the object triangle side is of same length as a predefined side of a predefined triangle" << endl;
	cout << "scaleMatrix1a.a.x = " << scaleMatrix1a.a.x << endl;
	cout << "scaleMatrix1a.a.y = " << scaleMatrix1a.a.y << endl;
	cout << "scaleMatrix1a.a.z = " << scaleMatrix1a.a.z << endl;
	cout << "scaleMatrix1a.b.x = " << scaleMatrix1a.b.x << endl;
	cout << "scaleMatrix1a.b.y = " << scaleMatrix1a.b.y << endl;
	cout << "scaleMatrix1a.b.z = " << scaleMatrix1a.b.z << endl;
	cout << "scaleMatrix1a.c.x = " << scaleMatrix1a.c.x << endl;
	cout << "scaleMatrix1a.c.y = " << scaleMatrix1a.c.y << endl;
	cout << "scaleMatrix1a.c.z = " << scaleMatrix1a.c.z << endl;
	#endif

	//1b. tranform object triangle; Scale object triangle such that the object triangle side is of same length as a predefined side of a predefined triangle
	vec vecNew;
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &scaleMatrix1a);
	copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &scaleMatrix1a);
	copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &scaleMatrix1a);
	copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	multiplyVectorByMatrix(&vecNew, &(point4), &scaleMatrix1a);
	copyVectors(&(point4), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(point5), &scaleMatrix1a);
	copyVectors(&(point5), &vecNew);
	*/

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	#endif

	//1c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature;	//startup
		currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			if(compareVectors(&(currentFeature->point), &(currentPolygonInList->point1)))
			{
				currentFeature->OTpointIndex = 1;
			}
			else if(compareVectors(&(currentFeature->point), &(currentPolygonInList->point2)))
			{
				currentFeature->OTpointIndex = 2;
			}
			else if(compareVectors(&(currentFeature->point), &(currentPolygonInList->point3)))
			{
				currentFeature->OTpointIndex = 3;
			}
			else
			{
				currentFeature->OTpointIndex = 0;
			}
			#endif

			copyVectors(&(currentFeature->pointTransformed), &(currentFeature->point));		//startup
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix1a);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			#ifdef OR_DEBUG_METHOD_2DOD
			cout << "nearest currentFeature->pointTransformed: x = " << currentFeature->pointTransformed.x << ", y = " << currentFeature->pointTransformed.y << ", z = " << currentFeature->pointTransformed.z << endl;
			#endif
			currentFeature = currentFeature->next;
		}
	}

	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature;
		currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			if(compareVectors(&(currentFeature->point), &(currentPolygonInList->point1)))
			{
				currentFeature->OTpointIndex = 1;
			}
			else if(compareVectors(&(currentFeature->point), &(currentPolygonInList->point2)))
			{
				currentFeature->OTpointIndex = 2;
			}
			else if(compareVectors(&(currentFeature->point), &(currentPolygonInList->point3)))
			{
				currentFeature->OTpointIndex = 3;
			}
			else
			{
				currentFeature->OTpointIndex = 0;
			}
			#endif

			copyVectors(&(currentFeature->pointTransformed), &(currentFeature->point));		//startup
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix1a);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}


	//2ia. rotate object data such that the object triangle side is parallel with X axis [and 2ii. third apex is above the lowest 2 apexes]
	double rotationFactor2i = -orientationOfObjectTriangleSide;

	mat rotateMatrix2i;
	createRotationMatrix2D(&rotateMatrix2i, rotationFactor2i);
	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation2iRotationFactor = rotationFactor2i;
	#else
	//applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &rotateMatrix2i);
	#endif
	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "2. rotate object data such that the object triangle side is parallel with X axis" << endl;
	cout << "rotateMatrix2i.a.x = " << rotateMatrix2i.a.x << endl;
	cout << "rotateMatrix2i.a.y = " << rotateMatrix2i.a.y << endl;
	cout << "rotateMatrix2i.a.z = " << rotateMatrix2i.a.z << endl;
	cout << "rotateMatrix2i.b.x = " << rotateMatrix2i.b.x << endl;
	cout << "rotateMatrix2i.b.y = " << rotateMatrix2i.b.y << endl;
	cout << "rotateMatrix2i.b.z = " << rotateMatrix2i.b.z << endl;
	cout << "rotateMatrix2i.c.x = " << rotateMatrix2i.c.x << endl;
	cout << "rotateMatrix2i.c.y = " << rotateMatrix2i.c.y << endl;
	cout << "rotateMatrix2i.c.z = " << rotateMatrix2i.c.z << endl;
	#endif

	//2ib. tranform object triangle;  rotate object triangle such that the object triangle side is parallel with X axis
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &rotateMatrix2i);
	copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &rotateMatrix2i);
	copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &rotateMatrix2i);
	copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	multiplyVectorByMatrix(&vecNew, &(point4), &rotateMatrix2i);
	copyVectors(&(point4), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(point5), &rotateMatrix2i);
	copyVectors(&(point5), &vecNew);
	*/

	//2ic. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2i);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			#ifdef OR_DEBUG_METHOD_2DOD
			cout << "nearest currentFeature->pointTransformed: x = " << currentFeature->pointTransformed.x << ", y = " << currentFeature->pointTransformed.y << ", z = " << currentFeature->pointTransformed.z << endl;
			#endif
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2i);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}



	//2ii. now if necessary, rotate by 180 degrees...
	bool rotationBy180DegreesNecessary = false;

	if(side == 0)
	{
		if(transformedObjectTriangle->point3.y > transformedObjectTriangle->point1.y)
		{
			rotationBy180DegreesNecessary = true;
		}
	}
	else if(side == 1)
	{
		if(transformedObjectTriangle->point1.y > transformedObjectTriangle->point2.y)
		{
			rotationBy180DegreesNecessary = true;
		}
	}
	else if(side == 2)
	{
		if(transformedObjectTriangle->point2.y > transformedObjectTriangle->point3.y)
		{
			rotationBy180DegreesNecessary = true;
		}
	}
	else
	{
		cout << "error: illegal side" << endl;
		exit(0);
	}


	double rotationFactor2ii;

	if(rotationBy180DegreesNecessary)
	{
		rotationFactor2ii = PI;
	}
	else
	{
		rotationFactor2ii = 0.0;
	}

	//2iia. now if necessary, rotate by 180 degrees...
	mat rotateMatrix2ii;
	createRotationMatrix2D(&rotateMatrix2ii, rotationFactor2ii);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation2iiRotationFactor = rotationFactor2ii;
	#else
	//applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &rotateMatrix2ii);
	#endif

	//2iib. tranform object triangle; rotate object triangle by 180 degrees if necessary
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &rotateMatrix2ii);
	copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &rotateMatrix2ii);
	copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &rotateMatrix2ii);
	copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	multiplyVectorByMatrix(&vecNew, &(point4), &rotateMatrix2ii);
	copyVectors(&(point4), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(point5), &rotateMatrix2ii);
	copyVectors(&(point5), &vecNew);
	*/

	//2iic. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2ii);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			#ifdef OR_DEBUG_METHOD_2DOD
			cout << "nearest currentFeature->pointTransformed: x = " << currentFeature->pointTransformed.x << ", y = " << currentFeature->pointTransformed.y << ", z = " << currentFeature->pointTransformed.z << endl;
			#endif
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2ii);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}




	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	#endif


	double perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide = predefinedTriangle->point3.y;
#ifndef OR_METHOD_2DOD_ASSUME_NO_3D_ROTATION

	//3a. Scale object data on Y axis such that the third apex is the same perpendicular distance away from the side as is the case for the predefined triangle.
	double perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide = calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(transformedObjectTriangle, side);

	mat scaleMatrix3a;
	double scaleFactor3a = perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide;

	//OLD: createYAxisScaleMatrix2D(&scaleMatrix3a, lengthOfPredefinedTriangleSide/perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide);
	createYAxisScaleMatrix2D(&scaleMatrix3a, scaleFactor3a);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation3aScaleFactor = scaleFactor3a;
	#else
	//applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &scaleMatrix3a);
	#endif

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "3. Scale object data on Y axis such that the third apex is the same perpendicular distance away from the side as is the case for the predefined triangle." << endl;
	cout << "scaleMatrix3a.a.x = " << scaleMatrix3a.a.x << endl;
	cout << "scaleMatrix3a.a.y = " << scaleMatrix3a.a.y << endl;
	cout << "scaleMatrix3a.a.z = " << scaleMatrix3a.a.z << endl;
	cout << "scaleMatrix3a.b.x = " << scaleMatrix3a.b.x << endl;
	cout << "scaleMatrix3a.b.y = " << scaleMatrix3a.b.y << endl;
	cout << "scaleMatrix3a.b.z = " << scaleMatrix3a.b.z << endl;
	cout << "scaleMatrix3a.c.x = " << scaleMatrix3a.c.x << endl;
	cout << "scaleMatrix3a.c.y = " << scaleMatrix3a.c.y << endl;
	cout << "scaleMatrix3a.c.z = " << scaleMatrix3a.c.z << endl;
	#endif

	//3b. tranform object triangle; Scale object triangle on Y axis such that the third apex is the same perpendicular distance away from the side as is the case for the predefined triangle.
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &scaleMatrix3a);
	copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &scaleMatrix3a);
	copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &scaleMatrix3a);
	copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	multiplyVectorByMatrix(&vecNew, &(point4), &scaleMatrix3a);
	copyVectors(&(point4), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(point5), &scaleMatrix3a);
	copyVectors(&(point5), &vecNew);
	*/

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	#endif

	//3c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix3a);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			#ifdef OR_DEBUG_METHOD_2DOD
			cout << "nearest currentFeature->pointTransformed: x = " << currentFeature->pointTransformed.x << ", y = " << currentFeature->pointTransformed.y << ", z = " << currentFeature->pointTransformed.z << endl;
			#endif
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix3a);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}


	//4a. shear object data along X axis such that object triangle apexes are coincident with predefined triangle apexes
	double xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(transformedObjectTriangle, side);
		//double shearRequired = (xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex - (lengthOfPredefinedTriangleSide/2))/lengthOfPredefinedTriangleSide;	OLD
	double shearRequired4a = (xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex - (lengthOfPredefinedTriangleSide/2))/perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide;


	mat shearMatrix4a;
	createXAxisShearMatrix2D(&shearMatrix4a, shearRequired4a);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation4aShearFactor = shearRequired4a;
	#else
	//applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &shearMatrix4a);
	#endif

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "4. shear object data along X axis such that object triangle apexes are coincident with predefined triangle apexes." << endl;
	cout << "shearMatrix4a.a.x = " << shearMatrix4a.a.x << endl;
	cout << "shearMatrix4a.a.y = " << shearMatrix4a.a.y << endl;
	cout << "shearMatrix4a.a.z = " << shearMatrix4a.a.z << endl;
	cout << "shearMatrix4a.b.x = " << shearMatrix4a.b.x << endl;
	cout << "shearMatrix4a.b.y = " << shearMatrix4a.b.y << endl;
	cout << "shearMatrix4a.b.z = " << shearMatrix4a.b.z << endl;
	cout << "shearMatrix4a.c.x = " << shearMatrix4a.c.x << endl;
	cout << "shearMatrix4a.c.y = " << shearMatrix4a.c.y << endl;
	cout << "shearMatrix4a.c.z = " << shearMatrix4a.c.z << endl;
	#endif

	//4b. tranform object triangle; shear object trianlge along X axis such that object triangle apexes are coincident with predefined triangle apexes
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &shearMatrix4a);
	copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &shearMatrix4a);
	copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &shearMatrix4a);
	copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	multiplyVectorByMatrix(&vecNew, &(point4), &shearMatrix4a);
	copyVectors(&(point4), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(point5), &shearMatrix4a);
	copyVectors(&(point5), &vecNew);
	*/

	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	#endif

	//4c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &shearMatrix4a);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			#ifdef OR_DEBUG_METHOD_2DOD
			cout << "nearest currentFeature->pointTransformed: x = " << currentFeature->pointTransformed.x << ", y = " << currentFeature->pointTransformed.y << ", z = " << currentFeature->pointTransformed.z << endl;
			#endif
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &shearMatrix4a);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}


#endif


	//5a. translate object data on X and Y axis such that the object triangle is centred at 0,0
	vec translationVector;
	if(side == 0)
	{
		if(transformedObjectTriangle->point1.x < transformedObjectTriangle->point2.x)
		{
			translationVector.x = -(transformedObjectTriangle->point1.x) - (lengthOfPredefinedTriangleSide/2);
			translationVector.y = -(transformedObjectTriangle->point1.y) + (perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/2);
		}
		else
		{
			translationVector.x = -(transformedObjectTriangle->point2.x) - (lengthOfPredefinedTriangleSide/2);
			translationVector.y = -(transformedObjectTriangle->point1.y) + (perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/2);
		}

	}
	else if(side == 1)
	{
		if(transformedObjectTriangle->point2.x < transformedObjectTriangle->point3.x)
		{
			translationVector.x = -(transformedObjectTriangle->point2.x) - (lengthOfPredefinedTriangleSide/2);
			translationVector.y = -(transformedObjectTriangle->point2.y) + (perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/2);
		}
		else
		{
			translationVector.x = -(transformedObjectTriangle->point3.x) - (lengthOfPredefinedTriangleSide/2);
			translationVector.y = -(transformedObjectTriangle->point2.y) + (perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/2);
		}


	}
	else if(side == 2)
	{
		if(transformedObjectTriangle->point3.x < transformedObjectTriangle->point1.x)
		{
			translationVector.x = -(transformedObjectTriangle->point3.x) - (lengthOfPredefinedTriangleSide/2);
			translationVector.y = -(transformedObjectTriangle->point3.y) + (perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/2);
		}
		else
		{
			translationVector.x = -(transformedObjectTriangle->point1.x) - (lengthOfPredefinedTriangleSide/2);
			translationVector.y = -(transformedObjectTriangle->point3.y) + (perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/2);
		}
	}





#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation5aTranslationFactorX = translationVector.x;
	opengl2DmatrixTransformation5aTranslationFactorY = translationVector.y;

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS_ADV

	mat multipliedMatrix;
	mat matTemp;
	createIdentityMatrixRT(&multipliedMatrix);

	#ifndef OR_METHOD_2DOD_ASSUME_NO_3D_ROTATION
	multiplyMatricies(&matTemp, &multipliedMatrix, &shearMatrix4a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix3a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	#endif
	multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2ii);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2i);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix1a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);

	copyMatrixTwoIntoMatrixOne(&opengl2DmultiplicationMatrix, &multipliedMatrix);

	/*
	//now create inverted matrix;
	mat multipliedMatrixInverted;
	createIdentityMatrixRT(&multipliedMatrixInverted);

	multiplyMatricies(&matTemp, &multipliedMatrixInverted, &scaleMatrix1a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrixInverted, &rotateMatrix2i);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrixInverted, &rotateMatrix2ii);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrixInverted, &scaleMatrix3a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrixInverted, &shearMatrix4a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);

	ptp4.x = ptp4.x + translationVector.x;
	ptp4.y = ptp4.y + translationVector.y;
	ptp5.x = ptp5.x + translationVector.x;
	ptp5.y = ptp5.y + translationVector.y;

	multMatrixByVector(&multipliedMatrixInverted, &ptp4, &point4);
	multMatrixByVector(&multipliedMatrixInverted, &ptp5, &point5);
	*/
	/*
	point4.x = point4.x + translationVector.x;
	point4.y = point4.y + translationVector.y;
	point5.x = point5.x + translationVector.x;
	point5.y = point5.y + translationVector.y;
		*/
	#endif
#else
	/*
	scaleMatrix1a
	rotateMatrix2i
	rotateMatrix2ii
	scaleMatrix3a
	shearMatrix4a
	translationVector
	*/

	mat multipliedMatrix;
	mat matTemp;
	createIdentityMatrixRT(&multipliedMatrix);

	#ifndef OR_METHOD_2DOD_ASSUME_NO_3D_ROTATION
	multiplyMatricies(&matTemp, &multipliedMatrix, &shearMatrix4a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix3a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	#endif
	multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2ii);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2i);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix1a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);

	applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &multipliedMatrix);

	applyTranslationToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &translationVector);
#endif

	#ifdef OR_DEBUG_METHOD_2DOD
	//cout << "perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide = " << perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide << endl;
	cout << "5. translate object data on X and Y axis such that the object triangle is centred at 0,0" << endl;
	cout << "translationVector.x = " << translationVector.x << endl;
	cout << "translationVector.y = " << translationVector.y << endl;
	cout << "translationVector.z = " << translationVector.z << endl;
	#endif

	//5b. tranform object triangle; translate the object triangle on X and Y axis such that the object triangle is centred at 0,0
	transformedObjectTriangle->point1.x = transformedObjectTriangle->point1.x + translationVector.x;
	transformedObjectTriangle->point1.y = transformedObjectTriangle->point1.y + translationVector.y;
	transformedObjectTriangle->point2.x = transformedObjectTriangle->point2.x + translationVector.x;
	transformedObjectTriangle->point2.y = transformedObjectTriangle->point2.y + translationVector.y;
	transformedObjectTriangle->point3.x = transformedObjectTriangle->point3.x + translationVector.x;
	transformedObjectTriangle->point3.y = transformedObjectTriangle->point3.y + translationVector.y;


	#ifdef OR_DEBUG_METHOD_2DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	#endif

	//5c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			currentFeature->pointTransformed.x = currentFeature->pointTransformed.x + translationVector.x;
			currentFeature->pointTransformed.y = currentFeature->pointTransformed.y + translationVector.y;
			#ifdef OR_DEBUG_METHOD_2DOD
			cout << "nearest currentFeature->pointTransformed: x = " << currentFeature->pointTransformed.x << ", y = " << currentFeature->pointTransformed.y << ", z = " << currentFeature->pointTransformed.z << endl;
			#endif
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			currentFeature->pointTransformed.x = currentFeature->pointTransformed.x + translationVector.x;
			currentFeature->pointTransformed.y = currentFeature->pointTransformed.y + translationVector.y;
			currentFeature = currentFeature->next;
		}
	}


	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t end: 3ai. 2DOD normalised snapshot generation - transform data wrt polygon - matrix calc" << endl;
		}
		long time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd;
		time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd = getTimeAsLong();
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygon = " << time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd-time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart << endl;
		}
	}

	long time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t start: 3aii. 2DOD normalised snapshot generation - transform data wrt polygon - cull" << endl;
		}

		time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart = getTimeAsLong();
	}

#ifdef USE_OPENGL_OPTIMISED_TRIANGLE_RENDERING
	//setViewPort(100, 100, transformedObjectTriangle->point1.x, transformedObjectTriangle->point1.y);
	setViewPort2Dortho(-1.0, 1.0, -1.0, 1.0);
#else
	//6. Disable References That Are Not Contained In The Triangle [these won't be raytraced / generated / fed into neural network]
	if(!OR_METHOD_DO_NOT_CULL_SNAPSHOT_EXTREME)
	{
		if(!DEMO_TO_CUSTOMER_HIDE_T_FROM_VIEW)
		{

			if(OR_METHOD_DO_NOT_CULL_SNAPSHOT)
			{
				bool padBoundary = false;
				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_GAUSSIAN_PREPROCESSOR_METHOD1
				padBoundary = true;
				#endif
				//disableReferencesThatAreNotContainedInTheObjectSquare2DOD(firstReferenceInInterpolated2DrgbMap, currentPolygonInList, padBoundary);
				disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(firstReferenceInInterpolated2DrgbMap, currentPolygonInList, padBoundary, side, shearRequired4a);
				//disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced2(firstReferenceInInterpolated2DrgbMap, currentPolygonInList, &point4, &point5, padBoundary);

			}
			else
			{
				disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(firstReferenceInInterpolated2DrgbMap, currentPolygonInList);
			}
		}
	}
#endif

#ifdef OR_METHOD_TRANSFORM_KEY_OT_FEATURES
	currentPolygonInList->point1Transformed.x = transformedObjectTriangle->point1.x;
	currentPolygonInList->point1Transformed.y = transformedObjectTriangle->point1.y;
	currentPolygonInList->point2Transformed.x = transformedObjectTriangle->point2.x;
	currentPolygonInList->point2Transformed.y = transformedObjectTriangle->point2.y;
	currentPolygonInList->point3Transformed.x = transformedObjectTriangle->point3.x;
	currentPolygonInList->point3Transformed.y = transformedObjectTriangle->point3.y;
#endif


#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
	//now add minWidthAndHeightOfOrigOT information

	double minXOfTriangle = minDouble(minDouble(currentPolygonInList->point1.x, currentPolygonInList->point2.x), currentPolygonInList->point3.x);
	double minYOfTriangle = minDouble(minDouble(currentPolygonInList->point1.y, currentPolygonInList->point2.y), currentPolygonInList->point3.y);
	double maxXOfTriangle = maxDouble(maxDouble(currentPolygonInList->point1.x, currentPolygonInList->point2.x), currentPolygonInList->point3.x);
	double maxYOfTriangle =  maxDouble(maxDouble(currentPolygonInList->point1.y, currentPolygonInList->point2.y), currentPolygonInList->point3.y);
	double widthOfOriginalTriangle = absDouble(minXOfTriangle - maxXOfTriangle);
	double heightOfOriginalTriangle = absDouble(minYOfTriangle - maxYOfTriangle);
	double minwidthheightOfOriginalTriangle = minDouble(widthOfOriginalTriangle, heightOfOriginalTriangle);


	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			currentFeature->minWidthAndHeightOfOrigOT = minwidthheightOfOriginalTriangle;
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			currentFeature->minWidthAndHeightOfOrigOT = minwidthheightOfOriginalTriangle;
			currentFeature = currentFeature->next;
		}
	}
#endif


	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t end: 3aii. 2DOD normalised snapshot generation - transform data wrt polygon - cull" << endl;
		}
		long time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd;
		time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd = getTimeAsLong();
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygon = " << time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd-time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart << endl;
		}
	}
}




 /*NEWEST, Barycentric Technique;*/
double maxDoubles(double a, double b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

double minDoubles(double a, double b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}




void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, bool padBoundary, int side, double shearFactor)
{

	vec A;
	vec B;
	vec C;
	vec D;

	vec pointApexOfTri;
	initialiseVector(&pointApexOfTri);

	if(side == 0)
	{
		A.x = currentPolygonInList->point1.x;
		A.y = currentPolygonInList->point1.y;
		A.z = 0.0;
		B.x = currentPolygonInList->point2.x;
		B.y = currentPolygonInList->point2.y;
		B.z = 0.0;
		pointApexOfTri.x = currentPolygonInList->point3.x;
		pointApexOfTri.y = currentPolygonInList->point3.y;
		pointApexOfTri.z = 0.0;
	}
	else if(side == 1)
	{
		A.x = currentPolygonInList->point2.x;
		A.y = currentPolygonInList->point2.y;
		A.z = 0.0;
		B.x = currentPolygonInList->point3.x;
		B.y = currentPolygonInList->point3.y;
		B.z = 0.0;
		pointApexOfTri.x = currentPolygonInList->point1.x;
		pointApexOfTri.y = currentPolygonInList->point1.y;
		pointApexOfTri.z = 0.0;
	}
	else if(side == 2)
	{
		A.x = currentPolygonInList->point3.x;
		A.y = currentPolygonInList->point3.y;
		A.z = 0.0;
		B.x = currentPolygonInList->point1.x;
		B.y = currentPolygonInList->point1.y;
		B.z = 0.0;
		pointApexOfTri.x = currentPolygonInList->point2.x;
		pointApexOfTri.y = currentPolygonInList->point2.y;
		pointApexOfTri.z = 0.0;
	}
	else
	{
		cout << "error misdefined side" << endl;
		exit(0);
	}

	initialiseVector(&C);
	initialiseVector(&D);

	vec parallelVectorToTriangleBase;
	initialiseVector(&parallelVectorToTriangleBase);
	subtractVectorsRT(&(B), &(A), &parallelVectorToTriangleBase);		//vect = vect1 - vect2

	multiplyVectorByScalar(&parallelVectorToTriangleBase, (1.0/shearFactor));		//NB 1.2 is to compensate for aliasing errors

	vec p3plusparallelVectorToTriangleBase;
	vec p3minusparallelVectorToTriangleBase;

	initialiseVector(&p3plusparallelVectorToTriangleBase);
	initialiseVector(&p3minusparallelVectorToTriangleBase);

	addVectorsRT(&(pointApexOfTri), &parallelVectorToTriangleBase, &p3plusparallelVectorToTriangleBase);
	subtractVectorsRT(&(pointApexOfTri), &parallelVectorToTriangleBase, &p3minusparallelVectorToTriangleBase);

	C.x = p3plusparallelVectorToTriangleBase.x;
	C.y = p3plusparallelVectorToTriangleBase.y;
	C.z = 0.0;
	D.x = p3minusparallelVectorToTriangleBase.x;
	D.y = p3minusparallelVectorToTriangleBase.y;
	D.z = 0.0;

	#ifdef OR_DEBUG
	//cout << "shearFactor = " << shearFactor << endl;
	/*
	cout << "currentPolygonInList->point1.x = " << currentPolygonInList->point1.x << endl;
	cout << "currentPolygonInList->point1.y = " << currentPolygonInList->point1.y << endl;
	cout << "currentPolygonInList->point2.x = " << currentPolygonInList->point2.x << endl;
	cout << "currentPolygonInList->point2.y = " << currentPolygonInList->point2.y << endl;
	cout << "currentPolygonInList->point3.x = " << currentPolygonInList->point3.x << endl;
	cout << "currentPolygonInList->point3.y = " << currentPolygonInList->point3.y << endl;

	cout << "p3plusparallelVectorToTriangleBase.x = " << p3plusparallelVectorToTriangleBase.x << endl;
	cout << "p3plusparallelVectorToTriangleBase.y = " << p3plusparallelVectorToTriangleBase.y << endl;
	cout << "p3minusparallelVectorToTriangleBase.x = " << p3minusparallelVectorToTriangleBase.x << endl;
	cout << "p3minusparallelVectorToTriangleBase.y = " << p3minusparallelVectorToTriangleBase.y << endl;

	cout << "A.x = " << A.x << endl;
	cout << "A.y = " << A.y << endl;
	cout << "B.x = " << B.x << endl;
	cout << "B.y = " << B.y << endl;
	cout << "C.x = " << C.x << endl;
	cout << "C.y = " << C.y << endl;
	cout << "D.x = " << D.x << endl;
	cout << "D.y = " << D.y << endl;
	cout << " " << endl;
	cout << currentPolygonInList->point1.x << "\t" << currentPolygonInList->point1.y << endl;
	cout << currentPolygonInList->point2.x << "\t" << currentPolygonInList->point2.y << endl;
	cout << currentPolygonInList->point3.x << "\t" << currentPolygonInList->point3.y << endl;
	cout << " " << endl;
	cout << p3minusparallelVectorToTriangleBase.x << "\t" << p3minusparallelVectorToTriangleBase.y << endl;
	cout << p3plusparallelVectorToTriangleBase.x << "\t" << p3plusparallelVectorToTriangleBase.y << endl;

	cout << " " << endl;
	cout << A.x << "\t" << A.y << endl;
	cout << B.x << "\t" << B.y << endl;
	cout << C.x << "\t" << C.y << endl;
	cout << D.x << "\t" << D.y << endl;
	cout << " " << endl;
	cout << " " << endl;
	cout << " " << endl;
	*/
	#endif

		//calculate size of bounding box (for triangle cull optimisation)
	double maxxOrig = maxDoubles(maxDoubles(maxDoubles(A.x, B.x), C.x), D.x);
	double minxOrig = minDoubles(minDoubles(minDoubles(A.x, B.x), C.x), D.x);
	double maxyOrig = maxDoubles(maxDoubles(maxDoubles(A.y, B.y), C.y), D.y);
	double minyOrig = minDoubles(minDoubles(minDoubles(A.y, B.y), C.y), D.y);
	double maxx;
	double minx;
	double maxy;
	double miny;
	double xdiff = maxxOrig - minxOrig;
	double ydiff = maxyOrig - minyOrig;
	if(padBoundary)
	{
		double xPadding = (maxx-minx)*OR_IMAGE_COMPARISON_PREPROCESS_BOUNDARY_RATIO;
		double yPadding = (maxy-miny)*OR_IMAGE_COMPARISON_PREPROCESS_BOUNDARY_RATIO;
		maxx = (maxxOrig + xPadding) + (xdiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
		minx = (minxOrig - xPadding) - (xdiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
		maxy = (maxyOrig + yPadding) + (ydiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
		miny = (minyOrig - yPadding) - (ydiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
	}
	else
	{
		maxx = maxxOrig + (xdiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
		minx = minxOrig - (xdiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
		maxy = maxyOrig + (ydiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
		miny = minyOrig - (ydiff* OR_METHOD2DOD_IMAGE_COMPARISON_CULL_ERROR);
	}

	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		bool currentReferenceLiesInsideObjectSquare;

		double testPointx;
		double testPointy;

		vec P;

		#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
		testPointx = (currentReference->vertex1absolutePosition.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		testPointy = (currentReference->vertex1absolutePosition.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		#else
		testPointx = currentReference->vertex1absolutePositionBackup.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		testPointy = currentReference->vertex1absolutePositionBackup.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		#endif

			//bounding box optimisation;
		if((testPointx < maxx) && (testPointx > minx) && (testPointy < maxy) && (testPointy > miny))
		{

			currentReferenceLiesInsideObjectSquare = true;
		}
		else
		{
			currentReferenceLiesInsideObjectSquare = false;
		}

		if(currentReferenceLiesInsideObjectSquare)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			#ifdef OR_DEBUG
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
			#endif
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}


/*
void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced2(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, vec* point4, vec* point5, bool padBoundary)
{
	vec A;
	vec B;
	vec C;
	vec D;

	A.x = currentPolygonInList->point1.x;
	A.y = currentPolygonInList->point1.y;
	A.z = 0.0;
	B.x = currentPolygonInList->point2.x;
	B.y = currentPolygonInList->point2.y;
	B.z = 0.0;
	C.x = point4->x;
	C.y = point4->y;
	C.z = 0.0;
	D.x = point5->x;
	D.y = point5->y;
	D.z = 0.0;

	cout << "currentPolygonInList->point1.x = " << currentPolygonInList->point1.x << endl;
	cout << "currentPolygonInList->point1.y = " << currentPolygonInList->point1.y << endl;
	cout << "currentPolygonInList->point2.x = " << currentPolygonInList->point2.x << endl;
	cout << "currentPolygonInList->point2.y = " << currentPolygonInList->point2.y << endl;
	cout << "point4->x = " << point4->x << endl;
	cout << "point4->y = " << point4->y << endl;
	cout << "point5->x = " << point5->x << endl;
	cout << "point5->y = " << point5->y << endl;

		//calculate size of bounding box (for triangle cull optimisation)
	double maxxOrig = maxDoubles(maxDoubles(maxDoubles(A.x, B.x), C.x), D.x);
	double minxOrig = minDoubles(minDoubles(minDoubles(A.x, B.x), C.x), D.x);
	double maxyOrig = maxDoubles(maxDoubles(maxDoubles(A.y, B.y), C.y), D.y);
	double minyOrig = minDoubles(minDoubles(minDoubles(A.y, B.y), C.y), D.y);
	double maxx;
	double minx;
	double maxy;
	double miny;
	if(padBoundary)
	{
		double xPadding = (maxx-minx)*OR_IMAGE_COMPARISON_PREPROCESS_BOUNDARY_RATIO;
		double yPadding = (maxy-miny)*OR_IMAGE_COMPARISON_PREPROCESS_BOUNDARY_RATIO;
		maxx = maxxOrig + xPadding;
		minx = minxOrig - xPadding;
		maxy = maxyOrig + yPadding;
		miny = minyOrig - yPadding;
	}
	else
	{
		maxx = maxxOrig;
		minx = minxOrig;
		maxy = maxyOrig;
		miny = minyOrig;
	}

	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		bool currentReferenceLiesInsideObjectSquare;

		double testPointx;
		double testPointy;

		vec P;

		#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
		testPointx = (currentReference->vertex1absolutePosition.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		testPointy = (currentReference->vertex1absolutePosition.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		#else
		testPointx = currentReference->vertex1absolutePositionBackup.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		testPointy = currentReference->vertex1absolutePositionBackup.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		#endif

			//bounding box optimisation;
		if((testPointx < maxx) && (testPointx > minx) && (testPointy < maxy) && (testPointy > miny))
		{

			currentReferenceLiesInsideObjectSquare = true;
		}
		else
		{
			currentReferenceLiesInsideObjectSquare = false;
		}

		if(currentReferenceLiesInsideObjectSquare)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}



void disableReferencesThatAreNotContainedInTheObjectSquare2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, bool padBoundary)
{
	vec A;
	vec B;
	vec C;

	A.x = currentPolygonInList->point1.x;
	A.y = currentPolygonInList->point1.y;
	A.z = 0.0;
	B.x = currentPolygonInList->point2.x;
	B.y = currentPolygonInList->point2.y;
	B.z = 0.0;
	C.x = currentPolygonInList->point3.x;
	C.y = currentPolygonInList->point3.y;
	C.z = 0.0;

		//calculate size of bounding box (for triangle cull optimisation)
	double maxxOrig = maxDoubles(maxDoubles(A.x, B.x), C.x);
	double minxOrig = minDoubles(minDoubles(A.x, B.x), C.x);
	double maxyOrig = maxDoubles(maxDoubles(A.y, B.y), C.y);
	double minyOrig = minDoubles(minDoubles(A.y, B.y), C.y);
	double maxx;
	double minx;
	double maxy;
	double miny;
	if(padBoundary)
	{
		double xPadding = (maxx-minx)*OR_IMAGE_COMPARISON_PREPROCESS_BOUNDARY_RATIO;
		double yPadding = (maxy-miny)*OR_IMAGE_COMPARISON_PREPROCESS_BOUNDARY_RATIO;
		maxx = maxxOrig + xPadding;
		minx = minxOrig - xPadding;
		maxy = maxyOrig + yPadding;;
		miny = minyOrig - yPadding;
	}
	else
	{
		maxx = maxxOrig;
		minx = minxOrig;
		maxy = maxyOrig;
		miny = minyOrig;
	}

	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{


		bool currentReferenceLiesInsideObjectSquare;

		double testPointx;
		double testPointy;

		vec P;

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
		testPointx = (currentReference->vertex1absolutePosition.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		testPointy = (currentReference->vertex1absolutePosition.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));

	#else

		testPointx = currentReference->vertex1absolutePositionBackup.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		testPointy = currentReference->vertex1absolutePositionBackup.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
	#endif

			//bounding box optimisation;
		if((testPointx < maxx) && (testPointx > minx) && (testPointy < maxy) && (testPointy > miny))
		{

			currentReferenceLiesInsideObjectSquare = true;
		}
		else
		{
			currentReferenceLiesInsideObjectSquare = false;
		}

		if(currentReferenceLiesInsideObjectSquare)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}
*/


void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList)
{
	vec A;
	vec B;
	vec C;

	A.x = currentPolygonInList->point1.x;
	A.y = currentPolygonInList->point1.y;
	A.z = 0.0;
	B.x = currentPolygonInList->point2.x;
	B.y = currentPolygonInList->point2.y;
	B.z = 0.0;
	C.x = currentPolygonInList->point3.x;
	C.y = currentPolygonInList->point3.y;
	C.z = 0.0;

		//calculate size of bounding box (for triangle cull optimisation)
	double maxx = maxDoubles(maxDoubles(A.x, B.x), C.x);
	double minx = minDoubles(minDoubles(A.x, B.x), C.x);
	double maxy = maxDoubles(maxDoubles(A.y, B.y), C.y);
	double miny = minDoubles(minDoubles(A.y, B.y), C.y);


	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		bool currentReferenceLiesInsideObjectTriangle;

		double testPointx;
		double testPointy;

		vec P;

		#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
		testPointx = (currentReference->vertex1absolutePosition.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		testPointy = (currentReference->vertex1absolutePosition.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		#else
		testPointx = currentReference->vertex1absolutePositionBackup.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		testPointy = currentReference->vertex1absolutePositionBackup.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		#endif

			//bounding box optimisation;
		if((testPointx < maxx) && (testPointx > minx) && (testPointy < maxy) && (testPointy > miny))
		{
			P.x = testPointx;
			P.y = testPointy;
			P.z = 0.0;

			// Compute vectors

			vec v0;
			vec v1;
			vec v2;

			//v0 = C - A
			subtractVectorsRT(&C, &A, &v0);	//vect = vect1 - vect2
			//v1 = B - A
			subtractVectorsRT(&B, &A, &v1);	//vect = vect1 - vect2
			//v2 = P - A
			subtractVectorsRT(&P, &A, &v2);	//vect = vect1 - vect2

			// Compute dot products
			double dot00 = dotProduct(&v0, &v0);
			double dot01 = dotProduct(&v0, &v1);
			double dot02 = dotProduct(&v0, &v2);
			double dot11 = dotProduct(&v1, &v1);
			double dot12 = dotProduct(&v1, &v2);

			// Compute barycentric coordinates
			double invDenom = 1.0 / ((dot00* dot11) - (dot01* dot01));
			double u = ((dot11* dot02) - (dot01* dot12))* invDenom;
			double v = ((dot00* dot12) - (dot01* dot02))* invDenom;

			// Check if point is in triangle
			if((u > 0) && (v > 0) && (u + v < 1))
			{
				currentReferenceLiesInsideObjectTriangle = true;
			}
			else
			{
				currentReferenceLiesInsideObjectTriangle = false;
			}
		}
		else
		{
			currentReferenceLiesInsideObjectTriangle = false;
		}

		if(currentReferenceLiesInsideObjectTriangle)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}



/*NEWER, Barycentric Technique - descriptive but less efficient;

void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		bool currentReferenceLiesInsideObjectTriangle;

		double testPointx;
		double testPointy;
		vec A;
		vec B;
		vec C;
		vec P;

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
		testPointx = (currentReference->vertex1absolutePosition.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		testPointy = (currentReference->vertex1absolutePosition.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
	#else
		testPointx = currentReference->vertex1absolutePositionBackup.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		testPointy = currentReference->vertex1absolutePositionBackup.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
	#endif

		A.x = currentPolygonInList->point1.x;
		A.y = currentPolygonInList->point1.y;
		A.z = 0.0;
		B.x = currentPolygonInList->point2.x;
		B.y = currentPolygonInList->point2.y;
		B.z = 0.0;
		C.x = currentPolygonInList->point3.x;
		C.y = currentPolygonInList->point3.y;
		C.z = 0.0;
		P.x = testPointx;
		P.y = testPointy;
		P.z = 0.0;

		// Compute vectors

		vec v0;
		vec v1;
		vec v2;

		//v0 = C - A
		subtractVectorsRT(&C, &A, &v0);		//vect = vect1 - vect2
		//v1 = B - A
		subtractVectorsRT(&B, &A, &v1);
		//v2 = P - A
		subtractVectorsRT(&P, &A, &v2);

		// Compute dot products
		double dot00 = dotProduct(&v0, &v0);
		double dot01 = dotProduct(&v0, &v1);
		double dot02 = dotProduct(&v0, &v2);
		double dot11 = dotProduct(&v1, &v1);
		double dot12 = dotProduct(&v1, &v2);

		// Compute barycentric coordinates
		double invDenom = 1.0 / ((dot00* dot11) - (dot01* dot01));
		double u = ((dot11* dot02) - (dot01* dot12))* invDenom;
		double v = ((dot00* dot12) - (dot01* dot02))* invDenom;

		// Check if point is in triangle
		if((u > 0) && (v > 0) && (u + v < 1))
		{
			currentReferenceLiesInsideObjectTriangle = true;
		}
		else
		{
			currentReferenceLiesInsideObjectTriangle = false;
		}

		if(currentReferenceLiesInsideObjectTriangle)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}


}
*/


/*NEWER, Same Side Technique, but still inefficient method;

function SameSide(p1,p2, a,b)
    cp1 = CrossProduct(b-a, p1-a)
    cp2 = CrossProduct(b-a, p2-a)
    if DotProduct(cp1, cp2) >= 0 then return true
    else return false

function PointInTriangle(p, a,b,c)
    if SameSide(p,a, b,c) and SameSide(p,b, a,c)
        and SameSide(p,c, a,b) then return true
    else return false

 */



//NEWER, use triangle area test, but still inefficient method;
/*
void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		bool currentReferenceLiesInsideObjectTriangle;

		double testPointx;
		double testPointy;
		vec A;
		vec B;
		vec C;
		vec P;

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
		testPointx = (currentReference->vertex1absolutePosition.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));
		testPointy = (currentReference->vertex1absolutePosition.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0));

	#else

		testPointx = currentReference->vertex1absolutePositionBackup.x + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
		testPointy = currentReference->vertex1absolutePositionBackup.y + (OD2D_INTERPOLATED_MESH_PIXEL_SIZE/2.0);
	#endif

		A.x = currentPolygonInList->point1.x;
		A.y = currentPolygonInList->point1.y;
		A.z = 0.0;
		B.x = currentPolygonInList->point2.x;
		B.y = currentPolygonInList->point2.y;
		B.z = 0.0;
		C.x = currentPolygonInList->point3.x;
		C.y = currentPolygonInList->point3.y;
		C.z = 0.0;
		P.x = testPointx;
		P.y = testPointy;
		P.z = 0.0;

		//cout << "currentPolygonInList->point1.x = " << currentPolygonInList->point1.x << endl;
		//cout << "currentPolygonInList->point1.y = " << currentPolygonInList->point1.y << endl;
		//cout << "currentPolygonInList->point2.x = " << currentPolygonInList->point2.x << endl;
		//cout << "currentPolygonInList->point2.y = " << currentPolygonInList->point2.y << endl;
		//cout << "currentPolygonInList->point3.x = " << currentPolygonInList->point3.x << endl;
		//cout << "currentPolygonInList->point3.y = " << currentPolygonInList->point3.y << endl;
		//cout << "testPointx = " << testPointx << endl;
		//cout << "testPointy = " << testPointy << endl;

		double AreaPP1P2 = calculateAreaOfTriangle(&P, &A, &B);
		double AreaPP2P3 = calculateAreaOfTriangle(&P, &B, &C);
		double AreaPP3P1 = calculateAreaOfTriangle(&P, &C, &A);
		double AreaP1P2P3 = calculateAreaOfTriangle(&A, &B, &C);


		if(compareDoubles((AreaPP1P2 + AreaPP2P3 + AreaPP3P1),  AreaP1P2P3))
		{
			currentReferenceLiesInsideObjectTriangle = true;
			//cout << "numberOfApexesOfReferenceQuadThatLieInsideTriangle++" << endl;
		}
		else
		{
			currentReferenceLiesInsideObjectTriangle = false;
			//cout << "fails++" << endl;
		}

		if(currentReferenceLiesInsideObjectTriangle)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}
*/



/*OLD thorough but inefficient method;

void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList)
{
	LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{

		int numberOfApexesOfReferenceQuadThatLieInsideTriangle = 0;
		int numberOfSidesOfPolygonsInInterpolated2DRGBMap = 4;
		for(int i=0; i<numberOfSidesOfPolygonsInInterpolated2DRGBMap; i++)
		{
			double testPointx;
			double testPointy;
			vec A;
			vec B;
			vec C;
			vec P;

		#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
			if(i == 0)
			{
				testPointx = currentReference->vertex1absolutePosition.x;
				testPointy = currentReference->vertex1absolutePosition.y;
			}
			else if(i == 1)
			{
				testPointx = currentReference->vertex2absolutePosition.x;
				testPointy = currentReference->vertex2absolutePosition.y;
			}
			else if(i == 2)
			{
				testPointx = currentReference->vertex3absolutePosition.x;
				testPointy = currentReference->vertex3absolutePosition.y;
			}
			else
			{//i == 3
				testPointx = currentReference->vertex4absolutePosition.x;
				testPointy = currentReference->vertex4absolutePosition.y;
			}
		#else
			if(i == 0)
			{
				testPointx = currentReference->vertex1absolutePositionBackup.x;
				testPointy = currentReference->vertex1absolutePositionBackup.y;
			}
			else if(i == 1)
			{
				testPointx = currentReference->vertex2absolutePositionBackup.x;
				testPointy = currentReference->vertex2absolutePositionBackup.y;
			}
			else if(i == 2)
			{
				testPointx = currentReference->vertex3absolutePositionBackup.x;
				testPointy = currentReference->vertex3absolutePositionBackup.y;
			}
			else
			{//i == 3
				testPointx = currentReference->vertex4absolutePositionBackup.x;
				testPointy = currentReference->vertex4absolutePositionBackup.y;
			}
		#endif

			A.x = currentPolygonInList->point1.x;
			A.y = currentPolygonInList->point1.y;
			A.z = 0.0;
			B.x = currentPolygonInList->point2.x;
			B.y = currentPolygonInList->point2.y;
			B.z = 0.0;
			C.x = currentPolygonInList->point3.x;
			C.y = currentPolygonInList->point3.y;
			C.z = 0.0;
			P.x = testPointx;
			P.y = testPointy;
			P.z = 0.0;


			//cout << "currentPolygonInList->point1.x = " << currentPolygonInList->point1.x << endl;
			//cout << "currentPolygonInList->point1.y = " << currentPolygonInList->point1.y << endl;
			//cout << "currentPolygonInList->point2.x = " << currentPolygonInList->point2.x << endl;
			//cout << "currentPolygonInList->point2.y = " << currentPolygonInList->point2.y << endl;
			//cout << "currentPolygonInList->point3.x = " << currentPolygonInList->point3.x << endl;
			//cout << "currentPolygonInList->point3.y = " << currentPolygonInList->point3.y << endl;
			//cout << "testPointx = " << testPointx << endl;
			//cout << "testPointy = " << testPointy << endl;



			double AreaPP1P2 = calculateAreaOfTriangle(&P, &A, &B);
			double AreaPP2P3 = calculateAreaOfTriangle(&P, &B, &C);
			double AreaPP3P1 = calculateAreaOfTriangle(&P, &C, &A);
			double AreaP1P2P3 = calculateAreaOfTriangle(&A, &B, &C);


			if(compareDoubles((AreaPP1P2 + AreaPP2P3 + AreaPP3P1),  AreaP1P2P3))
			{
				numberOfApexesOfReferenceQuadThatLieInsideTriangle++;
				//cout << "numberOfApexesOfReferenceQuadThatLieInsideTriangle++" << endl;
			}
			else
			{
				//cout << "fails++" << endl;
			}


			//if(compareDoubles(APxABnormalised.x, BPxBCnormalised.x) && compareDoubles(APxABnormalised.y, BPxBCnormalised.y) && compareDoubles(APxABnormalised.z, BPxBCnormalised.z)
			//&& compareDoubles(APxABnormalised.x, CPxCAnormalised.x) && compareDoubles(APxABnormalised.y, CPxCAnormalised.y) && compareDoubles(APxABnormalised.z, CPxCAnormalised.z)
			//&& compareDoubles(BPxBCnormalised.x, BPxBCnormalised.x) && compareDoubles(BPxBCnormalised.y, BPxBCnormalised.y) && compareDoubles(BPxBCnormalised.z, BPxBCnormalised.z))
			//{
			//	numberOfApexesOfReferenceQuadThatLieInsideTriangle++;
			//	cout << "numberOfApexesOfReferenceQuadThatLieInsideTriangle++" << endl;
			//}

		}

		bool currentReferenceLiesInsideObjectTriangle;
		if(numberOfApexesOfReferenceQuadThatLieInsideTriangle == 0)
		{
			currentReferenceLiesInsideObjectTriangle = false;
		}
		else
		{
			currentReferenceLiesInsideObjectTriangle = true;
		}

		if(currentReferenceLiesInsideObjectTriangle)
		{
			currentReference->referenceEnabledMethod2DOD = true;
			//cout << "currentReference->referenceEnabledMethod2DOD = true" << endl;
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}



}
*/



void createInterpolated2DmeshReferenceListUsingRGBmap2DOD(int imageWidth, int imageHeight, unsigned char* rgbMap, LDreference* firstReferenceInInterpolatedMap)
{
	LDreference* currentReferenceInInterpolated2DMap = firstReferenceInInterpolatedMap;

	for(int y = 0; y < (imageHeight); y++)
	{
		for(int x = 0; x < (imageWidth); x++)
		{
			vec v1;
			vec v2;
			vec v3;
			vec v4;

			v1.x = x+0.0;
			v1.y = y+0.0;
			v1.z = 0.0;
			v2.x = x+1.0;
			v2.y = y+0.0;
			v2.z = 0.0;
			v3.x = x+1.0;
			v3.y = y+1.0;
			v3.z = 0.0;
			v4.x = x+0.0;
			v4.y = y+1.0;
			v4.z = 0.0;

			currentReferenceInInterpolated2DMap->type = REFERENCE_TYPE_QUAD;
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex1relativePosition), &v1);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex2relativePosition), &v2);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex3relativePosition), &v3);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex4relativePosition), &v4);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex1absolutePosition), &v1);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex2absolutePosition), &v2);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex3absolutePosition), &v3);
			copyVectors(&(currentReferenceInInterpolated2DMap->vertex4absolutePosition), &v4);

			colour col;
			getRGBMapValues(x, y, imageWidth, rgbMap, &col);

			unsigned int colByte1 = (unsigned int)DAT_FILE_FIRST_RGB_COLOUR << (unsigned int)24;
			unsigned int colByte2 = (unsigned int)col.r << (unsigned int)16;
			unsigned int colByte3 = (unsigned int)col.g << (unsigned int)8;
			unsigned int colByte4 = (unsigned int)col.b;


			currentReferenceInInterpolated2DMap->colour = colByte1 | colByte2 | colByte3 | colByte4;
			currentReferenceInInterpolated2DMap->absoluteColour = colByte1 | colByte2 | colByte3 | colByte4;

			#ifdef OR_DEBUG
			/*
			cout << "colByte1 = " << colByte1 << endl;
			cout << "colByte2 = " << colByte2 << endl;
			cout << "colByte3 = " << colByte3 << endl;
			cout << "colByte4 = " << colByte4 << endl;
			cout << "colByte1 | colByte2 | colByte3 | colByte4 = " << (colByte1 | colByte2 | colByte3 | colByte4) << endl;
			cout << "currentReferenceInInterpolated2DMap->vertex1relativePosition.x = " << currentReferenceInInterpolated2DMap->vertex1relativePosition.x << endl;
			cout << "currentReferenceInInterpolated2DMap->vertex1relativePosition.y = " << currentReferenceInInterpolated2DMap->vertex1relativePosition.y << endl;
			cout << "currentReferenceInInterpolated2DMap->vertex1relativePosition.z = " << currentReferenceInInterpolated2DMap->vertex1relativePosition.z << endl;
			*/
			#endif

			LDreference* newReference = new LDreference();
			currentReferenceInInterpolated2DMap->next = newReference;
			currentReferenceInInterpolated2DMap = currentReferenceInInterpolated2DMap->next;
		}
	}
}


void create2DmeshUsingRGBmap2DOD(int imageWidth, int imageHeight, double imageXOffset, double imageYOffset, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], bool useEdgeZeroCrossingMap)
{
	//#ifdef OR_USE_CONTRAST_CALC_METHOD_C
#ifndef LINUX
	ORmeshPoint** meshPointInterpixelArray = new ORmeshPoint* [imageWidth*imageHeight];
	ORQFzeroCrossing** edgeZeroCrossingMap = new ORQFzeroCrossing* [imageWidth*imageHeight];
#else
	ORmeshPoint* meshPointInterpixelArray[imageWidth*imageHeight];
	ORQFzeroCrossing* edgeZeroCrossingMap[imageWidth*imageHeight];
#endif

	int interpixelContrastMapType = INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST;
	bool* edgeBoolMap = new bool[imageWidth*imageHeight];
	if(OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
	{
		if(useEdgeZeroCrossingMap)
		{
			//1. edge process image using ORquadraticFit
			for(int i=0; i<imageWidth*imageHeight; i++)
			{
				edgeZeroCrossingMap[i] = NULL;
			}
			generateEdgeListFromRGBmapWithQuadraticFit(rgbMap, edgeBoolMap, edgeZeroCrossingMap, imageWidth, imageHeight, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, OR_METHOD2DOD_DIMENSIONS, NULL, NULL, 1.0, NULL, interpixelContrastMapType);
		}
	}

	ORmeshPoint* currentMeshPointInMesh = firstMeshPointInMeshList;
	int numMeshPointsInExistingMesh = 0;
	while(currentMeshPointInMesh->next != NULL)
	{
		numMeshPointsInExistingMesh++;
		currentMeshPointInMesh = currentMeshPointInMesh->next;
	}

	ORmeshPoint* firstNewMeshPointInMesh = currentMeshPointInMesh;
	currentMeshPointInMesh = firstNewMeshPointInMesh;

	ORmeshPoint* firstMeshPointInInterpixelMesh = new ORmeshPoint();
	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		firstNewMeshPointInMesh->interpixelMeshPointFilled = true;	// x+0.5, y+0.5
		firstNewMeshPointInMesh->interpixelMeshPoint = firstMeshPointInInterpixelMesh;
	}

	//create mesh points

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			ORmeshPoint* currentMeshPointInInterpixelMesh;
			if(OR_USE_CONTRAST_CALC_METHOD_C)
			{
				currentMeshPointInInterpixelMesh = currentMeshPointInMesh->interpixelMeshPoint;
				meshPointInterpixelArray[y*imageWidth + x] = currentMeshPointInInterpixelMesh;	//required for speed
			}
			else
			{
				currentMeshPointInInterpixelMesh = currentMeshPointInMesh;
			}

			colour col;
			getRGBMapValues(x, y, imageWidth, rgbMap, &col);

			currentMeshPointInMesh->col.r = col.r;
			currentMeshPointInMesh->col.g = col.g;
			currentMeshPointInMesh->col.b = col.b;
			currentMeshPointInMesh->point.x = x+imageXOffset;
			currentMeshPointInMesh->point.y = y+imageYOffset;
			currentMeshPointInMesh->point.z = 0.0;
			currentMeshPointInMesh->xInt = x;
			currentMeshPointInMesh->yInt = y;
			currentMeshPointInMesh->luminosity = calculateLuminosityLevelFromRGBVal(&col);

			if(OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
			{
				if(useEdgeZeroCrossingMap)
				{
					if(edgeBoolMap[y*imageWidth + x] ==  true)
					{
						currentMeshPointInInterpixelMesh->edge = true;
						currentMeshPointInInterpixelMesh->zeroCrossingValueX = edgeZeroCrossingMap[y*imageWidth + x]->zeroCrossingValueX;
						currentMeshPointInInterpixelMesh->zeroCrossingValueY = edgeZeroCrossingMap[y*imageWidth + x]->zeroCrossingValueY;
						currentMeshPointInInterpixelMesh->alpha = edgeZeroCrossingMap[y*imageWidth + x]->alpha;
					}
				}
			}

			meshPointArray[y*imageWidth + x] = currentMeshPointInMesh;	//required for speed

			ORmeshPoint* newMeshPoint = new ORmeshPoint();
			currentMeshPointInMesh->next = newMeshPoint;
			currentMeshPointInMesh = currentMeshPointInMesh->next;

			if(OR_USE_CONTRAST_CALC_METHOD_C)
			{
				ORmeshPoint* newMeshPointInInterpixelMesh = new ORmeshPoint();
				currentMeshPointInInterpixelMesh->next = newMeshPointInInterpixelMesh;
				currentMeshPointInMesh->interpixelMeshPointFilled = true;	// x+0.5, y+0.5
				currentMeshPointInMesh->interpixelMeshPoint = newMeshPointInInterpixelMesh;
			}
		}
	}

	//calculate mesh point adjacent mesh points

	currentMeshPointInMesh = firstNewMeshPointInMesh;

	ORmeshPoint* currentMeshPointInInterpixelMesh;
	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		currentMeshPointInInterpixelMesh = firstMeshPointInInterpixelMesh;
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			//currentMeshPointInMesh = meshPointArray[y*imageWidth + x];

			for(int q = 0; q < 9; q++)
			{
				int kx;
				int ky;

				kx = x + q%3 - 1;
				ky = y + q/3 - 1;

				if((kx >= 0) && (kx <= imageWidth-1) && (ky >= 0) && (ky <= imageHeight-1))
				{

					currentMeshPointInMesh->adjacentMeshPoint[q] = meshPointArray[ky*imageWidth + kx];
					currentMeshPointInMesh->adjacentMeshPointFilled[q] = true;
					if(OR_USE_CONTRAST_CALC_METHOD_C)
					{
						currentMeshPointInInterpixelMesh->adjacentMeshPoint[q] = meshPointInterpixelArray[ky*imageWidth + kx];
						currentMeshPointInInterpixelMesh->adjacentMeshPointFilled[q] = true;
					}
				}
				else
				{
					currentMeshPointInMesh->adjacentMeshPoint[q] = NULL;
					currentMeshPointInMesh->adjacentMeshPointFilled[q] = false;
					if(OR_USE_CONTRAST_CALC_METHOD_C)
					{
						currentMeshPointInInterpixelMesh->adjacentMeshPoint[q] = NULL;
						currentMeshPointInInterpixelMesh->adjacentMeshPointFilled[q] = false;
					}
				}
			}

			currentMeshPointInMesh = currentMeshPointInMesh->next;
			if(OR_USE_CONTRAST_CALC_METHOD_C)
			{
				currentMeshPointInInterpixelMesh = currentMeshPointInInterpixelMesh->next;
			}
		}
	}

	//calculate mesh point properties
	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		for(int y = 1; y < (imageHeight-1); y++)
		{
			for(int x = 1; x < (imageWidth-1); x++)
			{
				calculateMeshPointInterpixelLuminosityContrast(meshPointArray[y*imageWidth + x]);
			}
		}
	}
	else
	{
		for(int y = 1; y < (imageHeight-1); y++)
		{
			for(int x = 1; x < (imageWidth-1); x++)
			{
				calculateMeshPointLuminosityContrast(meshPointArray[y*imageWidth + x]);
			}
		}
	}

	//UNTESTED (copied from OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED)
	if(OR_METHOD_2DOD_USE_MESH_LISTS_COMBINED)
	{
		//A) join border mesh points of new mesh with existing mesh mesh points
		if(numMeshPointsInExistingMesh > 0)
		{
			for(int a=0; a<2; a++)
			{//a=axis
				int qSize;
				int rSize;

				if(a == 0)
				{
					qSize = imageWidth;
					rSize = imageHeight;
				}
				else if(a==1)
				{
					qSize = imageHeight;
					rSize = imageWidth;
				}

				for(int q=0; q<qSize; q++)
				{
					for(int r=0; r <=rSize; r=r+rSize)
					{
						int x;
						int y;

						if(a == 0)
						{
							x = q;
							y = r;
						}
						else if(a==1)
						{
							x = r;
							y = q;
						}

						vec borderPoint;
						borderPoint.x = x+imageXOffset;
						borderPoint.y = y+imageYOffset;
						borderPoint.z = 0.0;

						ORmeshPoint* nearestMeshpointInExistingMesh;
						bool hasFoundMeshPoint = false;
						nearestMeshpointInExistingMesh = findMeshPointIntInMesh(firstMeshPointInMeshList, &borderPoint, &hasFoundMeshPoint, numMeshPointsInExistingMesh);
						if(hasFoundMeshPoint == false)
						{
							cout << "error: cannot find adjacent meshpoint" << endl;
							exit(0);
						}

						ORmeshPoint* meshpointInAdditionalMesh;
						meshpointInAdditionalMesh = meshPointArray[y*imageWidth + x];

						/*old inefficient;
						hasFoundMeshPoint = false;
						meshpointInAdditionalMesh = findMeshPointIntInMesh(firstNewMeshPointInMesh, x, y, &hasFoundMeshPoint);
						if(hasFoundMeshPoint == false)
						{
							cout << "error: cannot find adjacent meshpoint" << endl;
							exit(0);
						}
						*/

						if(calculateTheDistanceBetweenTwoPoints(&(nearestMeshpointInExistingMesh->point), &(meshpointInAdditionalMesh->point)) < OR_METHOD_2DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE)
						{
							//now redirect all mesh points surrounding the mesh point of the existing mesh to the corresponding mesh point of the additional mesh
							for(int q = 0; q< 4; q++)
							{
								if((q+1)%2 == 0)
								{//then q corresponds to a +ve x or y direction
									nearestMeshpointInExistingMesh->adjacentMeshPoint[q]->adjacentMeshPoint[q-1] = meshpointInAdditionalMesh;
									meshpointInAdditionalMesh->adjacentMeshPoint[q] = nearestMeshpointInExistingMesh->adjacentMeshPoint[q];
								}
								else
								{//then q corresponds to a -ve x or y direction

									nearestMeshpointInExistingMesh->adjacentMeshPoint[q]->adjacentMeshPoint[q+1] = meshpointInAdditionalMesh;
									meshpointInAdditionalMesh->adjacentMeshPoint[q] = nearestMeshpointInExistingMesh->adjacentMeshPoint[q];
								}
							}
						}
					}
				}
			}


		}
	}

	#ifndef LINUX
	delete meshPointInterpixelArray;
	delete edgeZeroCrossingMap;
	#endif
}




