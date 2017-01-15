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
 * File Name: ORmethod3DOD.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3h9a 09-December-2015
 * NB pointmap is a new addition for test streamlining; NB in test scenarios 2 and 3, there will be no pointmap available; the pointmap will have to be generated after depth map is obtained by using calculatePointUsingTInWorld()
 *******************************************************************************/


#include "ORmethod3DOD.h"
#include "SHAREDvector.h"
#include "ORoperations.h"
#include "LDopengl.h"
#include "ORoperations.h"

void transformObjectData3DOD(LDreference* firstReferenceInInterpolated3DRGBMap, ORpolygon* currentPolygonInList, int side, bool first, ORfeature* firstFeatureInList)
{
	long time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygonStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t start: 3ai. 3DOD normalised snapshot generation - transform data wrt polygon - matrix calc" << endl;
		}
		time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygonStart = getTimeAsLong();
	}

	ORpolygon* transformedObjectTriangle = new ORpolygon();	//equilateral triangle
	transformedObjectTriangle->point1.x = currentPolygonInList->point1.x;
	transformedObjectTriangle->point1.y = currentPolygonInList->point1.y;
	transformedObjectTriangle->point1.z = currentPolygonInList->point1.z;
	transformedObjectTriangle->point2.x = currentPolygonInList->point2.x;
	transformedObjectTriangle->point2.y = currentPolygonInList->point2.y;
	transformedObjectTriangle->point2.z = currentPolygonInList->point2.z;
	transformedObjectTriangle->point3.x = currentPolygonInList->point3.x;
	transformedObjectTriangle->point3.y = currentPolygonInList->point3.y;
	transformedObjectTriangle->point3.z = currentPolygonInList->point3.z;

	#ifdef OR_DEBUG_METHOD_3DOD
	cout << "\n \t starting to tranform object triangle " << endl;
	cout << "0. currentPolygonInList without transformation " << endl;
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point1.z = " << transformedObjectTriangle->point1.z << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point2.z = " << transformedObjectTriangle->point2.z << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	cout << "transformedObjectTriangle->point3.z = " << transformedObjectTriangle->point3.z << endl;
	cout << transformedObjectTriangle->point1.x << " " << transformedObjectTriangle->point1.y << " " << transformedObjectTriangle->point1.z << endl;
	cout << transformedObjectTriangle->point2.x << " " << transformedObjectTriangle->point2.y << " " << transformedObjectTriangle->point2.z << endl;
	cout << transformedObjectTriangle->point3.x << " " << transformedObjectTriangle->point3.y << " " << transformedObjectTriangle->point3.z << endl;
	#endif

	//1. Perform All Rotations (X, Y, Z, such that object triangle side is parallel with x axis, and object triangle normal is parallel with z axis)

	vec normalBeforeRotation;
	calculateNormalOfTri(&(currentPolygonInList->point1), &(currentPolygonInList->point2), &(currentPolygonInList->point3), &normalBeforeRotation);
	vec normalBeforeRotationNormalised;	//not required
	normaliseVectorRT(&normalBeforeRotation, &normalBeforeRotationNormalised);	//not required

	mat xyzRotationMatrix12;
	vec eye;
	vec at;
	vec up;
	eye.x = 0.0;
	eye.y = 0.0;
	eye.z = 0.0;
	at.x = normalBeforeRotationNormalised.x;
	at.y = normalBeforeRotationNormalised.y;
	at.z = normalBeforeRotationNormalised.z;
	if(side == 0)
	{
		subtractVectorsRT(&(currentPolygonInList->point1), &(currentPolygonInList->point2), &up);
	}
	else if(side == 1)
	{
		subtractVectorsRT(&(currentPolygonInList->point2), &(currentPolygonInList->point3), &up);
	}
	else if(side == 2)
	{
		subtractVectorsRT(&(currentPolygonInList->point1), &(currentPolygonInList->point3), &up);
	}
	else
	{
		cout << "error: illegal side" << endl;
		exit(0);
	}
	generateLookAtRotationMatrix(&at, &eye, &up, &xyzRotationMatrix12);

	transposeMatrix(&xyzRotationMatrix12);


	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	//openglUseMatrix = true;
	//opengl3DmatrixTransformation1aXRotationFactor = xRotationFactor1a;
	#else
	if(first)
	{
		storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(firstReferenceInInterpolated3DRGBMap);
	}
	else
	{
		restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(firstReferenceInInterpolated3DRGBMap);
	}
	//applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated3DRGBMap, &scaleMatrix1a);
	#endif

	#ifdef OR_DEBUG_METHOD_3DOD
	cout << "1.+2. Rotate object data on XYZ axis such that the object triangle side face normal is parallel with Z axis" << endl;
	cout << "normalBeforeRotation.x = " << normalBeforeRotation.x << endl;
	cout << "normalBeforeRotation.y = " << normalBeforeRotation.y << endl;
	cout << "normalBeforeRotation.z = " << normalBeforeRotation.z << endl;
	cout << "normalBeforeRotationNormalised.x = " << normalBeforeRotationNormalised.x << endl;
	cout << "normalBeforeRotationNormalised.y = " << normalBeforeRotationNormalised.y << endl;
	cout << "normalBeforeRotationNormalised.z = " << normalBeforeRotationNormalised.z << endl;
	//cout << "xRotationFactor1a = " << xRotationFactor1a << endl;
	//cout << "yRotationFactor1b = " << yRotationFactor1b << endl;
	//cout << "zRotationFactor2a = " << zRotationFactor2a << endl;
	cout << "xyzRotationMatrix12.a.x = " << xyzRotationMatrix12.a.x << endl;
	cout << "xyzRotationMatrix12.a.y = " << xyzRotationMatrix12.a.y << endl;
	cout << "xyzRotationMatrix12.a.z = " << xyzRotationMatrix12.a.z << endl;
	cout << "xyzRotationMatrix12.b.x = " << xyzRotationMatrix12.b.x << endl;
	cout << "xyzRotationMatrix12.b.y = " << xyzRotationMatrix12.b.y << endl;
	cout << "xyzRotationMatrix12.b.z = " << xyzRotationMatrix12.b.z << endl;
	cout << "xyzRotationMatrix12.c.x = " << xyzRotationMatrix12.c.x << endl;
	cout << "xyzRotationMatrix12.c.y = " << xyzRotationMatrix12.c.y << endl;
	cout << "xyzRotationMatrix12.c.z = " << xyzRotationMatrix12.c.z << endl;
	#endif

	//1b. tranform object triangle;
	vec vecNew;
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &xyzRotationMatrix12);
	copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &xyzRotationMatrix12);
	copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &xyzRotationMatrix12);
	copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	#ifdef OR_DEBUG_METHOD_3DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point1.z = " << transformedObjectTriangle->point1.z << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point2.z = " << transformedObjectTriangle->point2.z << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	cout << "transformedObjectTriangle->point3.z = " << transformedObjectTriangle->point3.z << endl;
	cout << transformedObjectTriangle->point1.x << " " << transformedObjectTriangle->point1.y << " " << transformedObjectTriangle->point1.z << endl;
	cout << transformedObjectTriangle->point2.x << " " << transformedObjectTriangle->point2.y << " " << transformedObjectTriangle->point2.z << endl;
	cout << transformedObjectTriangle->point3.x << " " << transformedObjectTriangle->point3.y << " " << transformedObjectTriangle->point3.z << endl;
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
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &xyzRotationMatrix12);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
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
			multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &xyzRotationMatrix12);
			copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}



	//3a. Translate the object data on all axes such that the mid point of the object triangle side passes through the Z axis coordinate 0.
	vec translationVector; //midPointBetweenSideApexes;
	if(side == 0)
	{
		calculateMidPointBetweenTwoPoints(&(transformedObjectTriangle->point1), &(transformedObjectTriangle->point2), &translationVector);
	}
	else if(side == 1)
	{
		calculateMidPointBetweenTwoPoints(&(transformedObjectTriangle->point2), &(transformedObjectTriangle->point3), &translationVector);
	}
	else if(side == 2)
	{
		calculateMidPointBetweenTwoPoints(&(transformedObjectTriangle->point3), &(transformedObjectTriangle->point1), &translationVector);
	}
	else
	{
		cout << "error misdefined side" << endl;
		exit(0);
	}

#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl3DmatrixTransformation3TranslationFactorX = translationVector.x;
	opengl3DmatrixTransformation3TranslationFactorY = translationVector.y;
	opengl3DmatrixTransformation3TranslationFactorZ = translationVector.z;

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS_ADV

	mat multipliedMatrix;
	mat matTemp;
	createIdentityMatrixRT(&multipliedMatrix);

	multiplyMatricies(&matTemp, &multipliedMatrix, &xyzRotationMatrix12);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	/*
	multiplyMatricies(&matTemp, &multipliedMatrix, &zRotateMatrix2ii);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &xyzRotationMatrix12);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &zRotationMatrix2a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &yRotationMatrix1b);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &xRotationMatrix1a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	*/

	copyMatrixTwoIntoMatrixOne(&opengl3DmultiplicationMatrix, &multipliedMatrix);

	#endif
#else
	/*
	xRotationMatrix1a
	yRotationMatrix1b
	zRotationMatrix2a
	zRotateMatrix2ii
	translationVector
	*/

	mat multipliedMatrix;
	mat matTemp;
	createIdentityMatrixRT(&multipliedMatrix);

	multiplyMatricies(&matTemp, &multipliedMatrix, &zRotateMatrix2ii);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &zRotationMatrix2a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &yRotationMatrix1b);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	multiplyMatricies(&matTemp, &multipliedMatrix, &xRotationMatrix1a);
	copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);

	applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated3DRGBMap, &multipliedMatrix);

	applyTranslationToAllReferencesIn2Dlist(firstReferenceInInterpolated3DRGBMap, &translationVector);
#endif


	#ifdef OR_DEBUG_METHOD_3DOD
	cout << "3a. Translate the object data on all axes such that the mid point of the object triangle side passes through the Z axis coordinate 0." << endl;
	cout << "translationVector.x = " << translationVector.x << endl;
	cout << "translationVector.y = " << translationVector.y << endl;
	cout << "translationVector.z = " << translationVector.z << endl;
	#endif

	//3a. transform object triangle
	transformedObjectTriangle->point1.x = transformedObjectTriangle->point1.x - translationVector.x;
	transformedObjectTriangle->point1.y = transformedObjectTriangle->point1.y - translationVector.y;
	transformedObjectTriangle->point1.z = transformedObjectTriangle->point1.z - translationVector.z;
	transformedObjectTriangle->point2.x = transformedObjectTriangle->point2.x - translationVector.x;
	transformedObjectTriangle->point2.y = transformedObjectTriangle->point2.y - translationVector.y;
	transformedObjectTriangle->point2.z = transformedObjectTriangle->point2.z - translationVector.z;
	transformedObjectTriangle->point3.x = transformedObjectTriangle->point3.x - translationVector.x;
	transformedObjectTriangle->point3.y = transformedObjectTriangle->point3.y - translationVector.y;
	transformedObjectTriangle->point3.z = transformedObjectTriangle->point3.z - translationVector.z;


	#ifdef OR_DEBUG_METHOD_3DOD
	cout << "transformedObjectTriangle->point1.x = " << transformedObjectTriangle->point1.x << endl;
	cout << "transformedObjectTriangle->point1.y = " << transformedObjectTriangle->point1.y << endl;
	cout << "transformedObjectTriangle->point1.z = " << transformedObjectTriangle->point1.z << endl;
	cout << "transformedObjectTriangle->point2.x = " << transformedObjectTriangle->point2.x << endl;
	cout << "transformedObjectTriangle->point2.y = " << transformedObjectTriangle->point2.y << endl;
	cout << "transformedObjectTriangle->point2.z = " << transformedObjectTriangle->point2.z << endl;
	cout << "transformedObjectTriangle->point3.x = " << transformedObjectTriangle->point3.x << endl;
	cout << "transformedObjectTriangle->point3.y = " << transformedObjectTriangle->point3.y << endl;
	cout << "transformedObjectTriangle->point3.z = " << transformedObjectTriangle->point3.z << endl;
	cout << transformedObjectTriangle->point1.x << " " << transformedObjectTriangle->point1.y << " " << transformedObjectTriangle->point1.z << endl;
	cout << transformedObjectTriangle->point2.x << " " << transformedObjectTriangle->point2.y << " " << transformedObjectTriangle->point2.z << endl;
	cout << transformedObjectTriangle->point3.x << " " << transformedObjectTriangle->point3.y << " " << transformedObjectTriangle->point3.z << endl;
	#endif

	//5c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			currentFeature->pointTransformed.x = currentFeature->pointTransformed.x + translationVector.x;
			currentFeature->pointTransformed.y = currentFeature->pointTransformed.y + translationVector.y;
			currentFeature->pointTransformed.z = currentFeature->pointTransformed.z + translationVector.z;
			#ifdef OR_DEBUG
			/*
			cout << "currentFeature->pointTransformed.x = " << currentFeature->pointTransformed.x << endl;
			cout << "currentFeature->pointTransformed.y = " << currentFeature->pointTransformed.y << endl;
			cout << "currentFeature->pointTransformed.z = " << currentFeature->pointTransformed.z << endl;
			*/
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
			currentFeature->pointTransformed.z = currentFeature->pointTransformed.z + translationVector.z;

			currentFeature = currentFeature->next;
		}
	}

	if(OR_METHOD_TRANSFORM_KEY_OT_FEATURES)
	{
		currentPolygonInList->point1Transformed.x = transformedObjectTriangle->point1.x;
		currentPolygonInList->point1Transformed.y = transformedObjectTriangle->point1.y;
		currentPolygonInList->point1Transformed.z = transformedObjectTriangle->point1.z;
		currentPolygonInList->point2Transformed.x = transformedObjectTriangle->point2.x;
		currentPolygonInList->point2Transformed.y = transformedObjectTriangle->point2.y;
		currentPolygonInList->point2Transformed.z = transformedObjectTriangle->point2.z;
		currentPolygonInList->point3Transformed.x = transformedObjectTriangle->point3.x;
		currentPolygonInList->point3Transformed.y = transformedObjectTriangle->point3.y;
		currentPolygonInList->point3Transformed.z = transformedObjectTriangle->point3.z;
	}

	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t end: 3ai. 3DOD normalised snapshot generation - transform data wrt polygon - matrix calc" << endl;
		}
		long time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygonEnd;
		time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygonEnd = getTimeAsLong();
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygon = " << time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygonEnd-time3aiNormalisedSnapshotGeneration3DODTransformDataWRTPolygonStart << endl;
		}
	}
}


	//not yet finished
void calculateEyePositionAndOrientation3DOD(vec* eyeFacingPoly, vec* viewAtFacingPoly, vec* viewUpFacingPoly, vec* viewPortWidthHeightDepth, ORpolygon* pol, int side)
{
	vec pt1;
	vec pt2;
	vec pt3;

	if(side == 0)
	{
		copyVectors(&pt1, &(pol->point1));
		copyVectors(&pt2, &(pol->point2));
		copyVectors(&pt3, &(pol->point3));
	}
	else if(side == 1)
	{
		copyVectors(&pt1, &(pol->point2));
		copyVectors(&pt2, &(pol->point3));
		copyVectors(&pt3, &(pol->point1));
	}
	else if(side == 2)
	{
		copyVectors(&pt1, &(pol->point3));
		copyVectors(&pt2, &(pol->point1));
		copyVectors(&pt3, &(pol->point2));
	}
	else
	{
		cout << "error: illegal side" << endl;
		exit(0);
	}

	vec midPointBetweenPt1AndPt2;
	calculateMidPointBetweenTwoPoints(&pt1, &pt2, &midPointBetweenPt1AndPt2);

	vec normalAtPt1;
	vec pt2MinusPt1;
	vec pt3MinusPt1;
	subtractVectorsRT(&pt2, &pt1, &pt2MinusPt1);
	subtractVectorsRT(&pt3, &pt1, &pt3MinusPt1);
	calculateNormal(&pt2MinusPt1, &pt3MinusPt1, &normalAtPt1);

	vec normalAtPt1Normalised;
	normaliseVectorRT(&normalAtPt1, &normalAtPt1Normalised);

	vec normalAtPt1WithDistanceU;
	#ifdef USE_OPENGL
	//normalised distance to size of side s
	double U = findMagnitudeOfVector(&pt2MinusPt1);
	#else
	double U = OR_CONSTANT_DISTANCE_EYE_TO_POLYGON_FOR_NN_SNAPSHOT_U;
	#endif
	multiplyVectorByScalarRT(&normalAtPt1Normalised, U, &normalAtPt1WithDistanceU);

	//1. calculates eyeFacingPoly
	addVectorsRT(&midPointBetweenPt1AndPt2, &normalAtPt1WithDistanceU, eyeFacingPoly);

	//2. calculates viewAtFacingPoly
	copyVectors(viewAtFacingPoly, &midPointBetweenPt1AndPt2);

	//3. calculates viewUpFacingPoly
	vec viewupFacingPolygonUnnormalised;
	vec pt1MinusMidpoint;	//or p2MinusMidPoint, or pt2MinusPt1; doesnt matter
	vec eyeMinusMidpoint;	//or normalAtPt1Normalised; doesnt matter
	subtractVectorsRT(&pt1, &midPointBetweenPt1AndPt2, &pt1MinusMidpoint);
	subtractVectorsRT(eyeFacingPoly, &midPointBetweenPt1AndPt2, &eyeMinusMidpoint);
	calculateNormal(&pt1MinusMidpoint, &eyeMinusMidpoint, &viewupFacingPolygonUnnormalised);
	normaliseVectorRT(&viewupFacingPolygonUnnormalised, viewUpFacingPoly);

	viewPortWidthHeightDepth->x = findMagnitudeOfVector(&pt2MinusPt1);
	viewPortWidthHeightDepth->z = findMagnitudeOfVector(&pt2MinusPt1);		//not used, the orthogonal viewport depth should be infinite
	vec pt3MinusMidpoint;
	subtractVectorsRT(&pt3, &midPointBetweenPt1AndPt2, &pt3MinusMidpoint);
	viewPortWidthHeightDepth->y = findMagnitudeOfVector(&pt3MinusMidpoint);

	#ifdef OR_DEBUG_METHOD3DOD_POV
	cout << "pt1.x = " << pt1.x << endl;
	cout << "pt1.y = " << pt1.y << endl;
	cout << "pt1.z = " << pt1.z << endl;
	cout << "pt2.x = " << pt2.x << endl;
	cout << "pt2.y = " << pt2.y << endl;
	cout << "pt2.z = " << pt2.z << endl;
	cout << "pt3.x = " << pt3.x << endl;
	cout << "pt3.y = " << pt3.y << endl;
	cout << "pt3.z = " << pt3.z << endl;
	cout << "midPointBetweenPt1AndPt2.x = " << midPointBetweenPt1AndPt2.x << endl;
	cout << "midPointBetweenPt1AndPt2.y = " << midPointBetweenPt1AndPt2.y << endl;
	cout << "midPointBetweenPt1AndPt2.z = " << midPointBetweenPt1AndPt2.z << endl;
	cout << "normalAtPt1.x = " << normalAtPt1.x << endl;
	cout << "normalAtPt1.y = " << normalAtPt1.y << endl;
	cout << "normalAtPt1.z = " << normalAtPt1.z << endl;
	cout << "normalAtPt1Normalised.x = " << normalAtPt1Normalised.x << endl;
	cout << "normalAtPt1Normalised.y = " << normalAtPt1Normalised.y << endl;
	cout << "normalAtPt1Normalised.z = " << normalAtPt1Normalised.z << endl;
	cout << "normalAtPt1WithDistanceU.x = " << normalAtPt1WithDistanceU.x << endl;
	cout << "normalAtPt1WithDistanceU.y = " << normalAtPt1WithDistanceU.y << endl;
	cout << "normalAtPt1WithDistanceU.z = " << normalAtPt1WithDistanceU.z << endl;
	cout << "eyeFacingPoly->x = " << eyeFacingPoly->x << endl;
	cout << "eyeFacingPoly->y = " << eyeFacingPoly->y << endl;
	cout << "eyeFacingPoly->z = " << eyeFacingPoly->z << endl;
	cout << "viewAtFacingPoly->x = " << viewAtFacingPoly->x << endl;
	cout << "viewAtFacingPoly->y = " << viewAtFacingPoly->y << endl;
	cout << "viewAtFacingPoly->z = " << viewAtFacingPoly->z << endl;
	cout << "viewupFacingPolygonUnnormalised.x = " << viewupFacingPolygonUnnormalised.x << endl;
	cout << "viewupFacingPolygonUnnormalised.y = " << viewupFacingPolygonUnnormalised.y << endl;
	cout << "viewupFacingPolygonUnnormalised.z = " << viewupFacingPolygonUnnormalised.z << endl;
	cout << "viewUpFacingPoly->x = " << viewUpFacingPoly->x << endl;
	cout << "viewUpFacingPoly->y = " << viewUpFacingPoly->y << endl;
	cout << "viewUpFacingPoly->z = " << viewUpFacingPoly->z << endl;
	#endif
}


void createInterpolated3DmeshReferenceListUsingPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointMapInterpolated, unsigned char* rgbMap, LDreference* firstReferenceInInterpolated3Dmap)
{
	LDreference* currentReferenceInInterpolated3DMap = firstReferenceInInterpolated3Dmap;

	for(int y = 0; y < (imageHeight); y++)
	{
		for(int x = 0; x < (imageWidth); x++)
		{
			ORpolygon polyArray[4];

			vec centreVec;
			vec v1;
			vec v2;
			vec v3;
			vec v4;

			getPointMapValue(x, y, (imageWidth), pointMap, &centreVec);
			getPointMapValue(x, y, (imageWidth+1), pointMapInterpolated, &v1);
			getPointMapValue(x+1, y, (imageWidth+1), pointMapInterpolated, &v2);
			getPointMapValue(x, y+1, (imageWidth+1), pointMapInterpolated, &v3);
			getPointMapValue(x+1, y+1, (imageWidth+1), pointMapInterpolated, &v4);

			vec nullPointVector;
			nullPointVector.x = 0.0;
			nullPointVector.y = 0.0;
			nullPointVector.z = 0.0;
			if(!compareVectors(&centreVec, &nullPointVector))
			{
				#ifdef OR_DEBUG
				/*
				cout << "centreVec.x = " << centreVec.x << endl;
				cout << "centreVec.y = " << centreVec.y << endl;
				cout << "centreVec.z = " << centreVec.z << endl;
				cout << "v1.x = " << v1.x << endl;
				cout << "v1.y = " << v1.y << endl;
				cout << "v1.z = " << v1.z << endl;
				cout << "v2.x = " << v2.x << endl;
				cout << "v2.y = " << v2.y << endl;
				cout << "v2.z = " << v2.z << endl;
				cout << "v3.x = " << v3.x << endl;
				cout << "v3.y = " << v3.y << endl;
				cout << "v3.z = " << v3.z << endl;
				cout << "v4.x = " << v4.x << endl;
				cout << "v4.y = " << v4.y << endl;
				cout << "v4.z = " << v4.z << endl;
				*/
				#endif

				multiplyVectorByScalarRT(&centreVec, OR_SNAPSHOT_SCALE_FACTOR, &centreVec);
				multiplyVectorByScalarRT(&v1, OR_SNAPSHOT_SCALE_FACTOR, &v1);
				multiplyVectorByScalarRT(&v2, OR_SNAPSHOT_SCALE_FACTOR, &v2);
				multiplyVectorByScalarRT(&v3, OR_SNAPSHOT_SCALE_FACTOR, &v3);
				multiplyVectorByScalarRT(&v4, OR_SNAPSHOT_SCALE_FACTOR, &v4);

				copyVectors(&(polyArray[0].point1), &centreVec);
				copyVectors(&(polyArray[0].point2), &v1);
				copyVectors(&(polyArray[0].point3), &v2);

				copyVectors(&(polyArray[1].point1), &centreVec);
				copyVectors(&(polyArray[1].point2), &v2);
				copyVectors(&(polyArray[1].point3), &v4);

				copyVectors(&(polyArray[2].point1), &centreVec);
				copyVectors(&(polyArray[2].point2), &v1);
				copyVectors(&(polyArray[2].point3), &v3);

				copyVectors(&(polyArray[3].point1), &centreVec);
				copyVectors(&(polyArray[3].point2), &v3);
				copyVectors(&(polyArray[3].point3), &v4);

				int numPolysPerPixel = 4;
				for(int i = 0; i < numPolysPerPixel; i++)
				{
					currentReferenceInInterpolated3DMap->type = REFERENCE_TYPE_TRI;
					copyVectors(&(currentReferenceInInterpolated3DMap->vertex1relativePosition), &(polyArray[i].point1));
					copyVectors(&(currentReferenceInInterpolated3DMap->vertex2relativePosition), &(polyArray[i].point2));
					copyVectors(&(currentReferenceInInterpolated3DMap->vertex3relativePosition), &(polyArray[i].point3));
					copyVectors(&(currentReferenceInInterpolated3DMap->vertex1absolutePosition), &(polyArray[i].point1));
					copyVectors(&(currentReferenceInInterpolated3DMap->vertex2absolutePosition), &(polyArray[i].point2));
					copyVectors(&(currentReferenceInInterpolated3DMap->vertex3absolutePosition), &(polyArray[i].point3));
					colour col;
					getRGBMapValues(x, y, imageWidth, rgbMap, &col);

					unsigned int colByte1 = (unsigned int)DAT_FILE_FIRST_RGB_COLOUR << (unsigned int)24;
					unsigned int colByte2 = (unsigned int)col.r << (unsigned int)16;
					unsigned int colByte3 = (unsigned int)col.g << (unsigned int)8;
					unsigned int colByte4 = (unsigned int)col.b;

					currentReferenceInInterpolated3DMap->colour = colByte1 | colByte2 | colByte3 | colByte4;
					currentReferenceInInterpolated3DMap->absoluteColour = colByte1 | colByte2 | colByte3 | colByte4;

					#ifdef OR_DEBUG
					/*
					cout << "colByte1 = " << colByte1 << endl;
					cout << "colByte2 = " << colByte2 << endl;
					cout << "colByte3 = " << colByte3 << endl;
					cout << "colByte4 = " << colByte4 << endl;
					cout << "colByte1 | colByte2 | colByte3 | colByte4 = " << (colByte1 | colByte2 | colByte3 | colByte4) << endl;
					cout << "currentReferenceInInterpolated3DMap->vertex1relativePosition.x = " << currentReferenceInInterpolated3DMap->vertex1relativePosition.x << endl;
					cout << "currentReferenceInInterpolated3DMap->vertex1relativePosition.y = " << currentReferenceInInterpolated3DMap->vertex1relativePosition.y << endl;
					cout << "currentReferenceInInterpolated3DMap->vertex1relativePosition.z = " << currentReferenceInInterpolated3DMap->vertex1relativePosition.z << endl;
					*/
					#endif

					LDreference* newReference = new LDreference();
					currentReferenceInInterpolated3DMap->next = newReference;
					currentReferenceInInterpolated3DMap = currentReferenceInInterpolated3DMap->next;
				}
			}
		}
	}
}


void create3DmeshUsingPointMap3DOD(int imageWidth, int imageHeight, double* pointMap, double* depthMap, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], bool useEdgeZeroCrossingMap, int contrastValChosen, RTviewInfo* vi)
{
	//#ifdef OR_USE_CONTRAST_CALC_METHOD_C
	#ifndef LINUX
	ORmeshPoint** meshPointInterpixelArray = new ORmeshPoint* [imageWidth*imageHeight];
	ORQFzeroCrossing** edgeZeroCrossingMap = new ORQFzeroCrossing* [imageWidth*imageHeight];
	#else
	ORmeshPoint* meshPointInterpixelArray[imageWidth*imageHeight];
	ORQFzeroCrossing* edgeZeroCrossingMap[imageWidth*imageHeight];
	#endif
	//#endif

	int interpixelContrastMapType;
	if(contrastValChosen == CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST)
	{
		interpixelContrastMapType = INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST;
	}
	else if(contrastValChosen == CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST)
	{
		interpixelContrastMapType = INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST;
	}

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

			if(contrastValChosen == CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_LUMINOSITY_CONTRAST)
			{
				//interpixelContrastMapType = INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST;
				generateEdgeListFromRGBmapWithQuadraticFit(rgbMap, edgeBoolMap, edgeZeroCrossingMap, imageWidth, imageHeight, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, OR_METHOD3DOD_DIMENSIONS, pointMap, depthMap, 1, vi, interpixelContrastMapType);

			}
			else if(contrastValChosen == CENTRE_FEATURES_CALCULATION_USING_MESH_LIST_CONTRAST_VALUE_POINT_NORMAL_CONTRAST)
			{
				//interpixelContrastMapType = INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST
				double* pointNormalMap = new double[imageWidth*imageHeight*VECTOR_NUM_VALUES];
				double* pointNormalContrastMap = new double[imageWidth*imageHeight];
				createPointNormalMapFromPointMap(imageWidth, imageHeight, pointMap, pointNormalMap);
				createPointNormalContrastMapFromPointNormalMap(imageWidth, imageHeight, pointNormalMap, pointNormalContrastMap);
				double sensitivity = 1.0;
				generateEdgeListFromContrastMapWithQuadraticFit(pointNormalContrastMap, edgeBoolMap, edgeZeroCrossingMap, imageWidth, imageHeight, EDGE_NORMAL_CONTRAST_THRESHOLD, OR_METHOD3DOD_DIMENSIONS, pointMap, depthMap, 1, vi, interpixelContrastMapType);
				delete pointNormalMap;
				delete pointNormalContrastMap;
			}
			else
			{
				cout << "invalid contrastValChosen" << endl;
				exit(0);
			}
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

	for(int y = 0; y < (imageHeight); y++)
	{
		for(int x = 0; x < (imageWidth); x++)
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

			double depth;
			depth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			#ifdef OR_METHOD_3DOD_USE_DYNAMIC_WORLD_COORD_DETERMINATION_USING_DEPTH
				vec xyzWorld;
				calculatePointMapValue(x, y, depth, &xyzWorld, vi);
			#else
				vec xyzWorld;
				getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);
			#endif

			currentMeshPointInMesh->col.r = col.r;
			currentMeshPointInMesh->col.g = col.g;
			currentMeshPointInMesh->col.b = col.b;
			currentMeshPointInMesh->xInt = x;
			currentMeshPointInMesh->yInt = y;
			currentMeshPointInMesh->luminosity = calculateLuminosityLevelFromRGBVal(&col);
			currentMeshPointInMesh->depth = depth;
			currentMeshPointInMesh->point.x = xyzWorld.x;
			currentMeshPointInMesh->point.y = xyzWorld.y;
			currentMeshPointInMesh->point.z = xyzWorld.z;

			if(OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
			{
				if(OR_METHOD3DOD_USE_QUADRATIC_FIT_EDGE_ZERO_CROSSING_MAP)
				{
					if(useEdgeZeroCrossingMap)
					{
						if(edgeBoolMap[y*imageWidth + x] ==  true)
						{
							currentMeshPointInInterpixelMesh->edge = true;
							currentMeshPointInInterpixelMesh->zeroCrossingValueX = edgeZeroCrossingMap[y*imageWidth + x]->zeroCrossingValueX;
							currentMeshPointInInterpixelMesh->zeroCrossingValueY = edgeZeroCrossingMap[y*imageWidth + x]->zeroCrossingValueY;
							currentMeshPointInInterpixelMesh->alpha = edgeZeroCrossingMap[y*imageWidth + x]->alpha;

							#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS
							currentMeshPointInInterpixelMesh->quadraticFitDepth = depth;
							//override point map values with those calculated during quadratic fit;
							vec* temppoint;
							temppoint = &(edgeZeroCrossingMap[y*imageWidth + x])->point;
							currentMeshPointInInterpixelMesh->quadraticFitPoint.x = temppoint->x;
							currentMeshPointInInterpixelMesh->quadraticFitPoint.y = temppoint->y;
							currentMeshPointInInterpixelMesh->quadraticFitPoint.z = temppoint->z;
							#endif

						}
					}
				}
			}

			#ifdef OR_DEBUG
			/*
			cout << "currentMeshPointInMesh->col.r = " << currentMeshPointInMesh->col.r << endl;
			cout << "currentMeshPointInMesh->col.g = " << currentMeshPointInMesh->col.g << endl;
			cout << "currentMeshPointInMesh->col.b = " << currentMeshPointInMesh->col.b << endl;
			cout << "currentMeshPointInMesh->xInt = " << currentMeshPointInMesh->xInt << endl;
			cout << "currentMeshPointInMesh->yInt = " << currentMeshPointInMesh->yInt << endl;
			cout << "currentMeshPointInMesh->luminosity = " << currentMeshPointInMesh->luminosity << endl;
			cout << "currentMeshPointInMesh->depth = " << currentMeshPointInMesh->depth << endl;
			cout << "currentMeshPointInMesh->point.x = " << currentMeshPointInMesh->point.x << endl;
			cout << "currentMeshPointInMesh->point.y = " << currentMeshPointInMesh->point.y << endl;
			cout << "currentMeshPointInMesh->point.z = " << currentMeshPointInMesh->point.z << endl;
			*/
			#endif

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

	for(int y = 0; y < (imageHeight); y++)
	{
		for(int x = 0; x < (imageWidth); x++)
		{
			/*
			for(int q = 0; q < 4; q++)
			{
				int kx;
				int ky;

				if(q < 2)
				{
					kx = x + (q*2)-1;
					ky = y;
				}
				else
				{
					kx = x;
					ky = y + ((q-2)*2)-1;
				}

				if((kx >= 0) && (kx <= imageWidth-1) && (ky >= 0) && (ky <= imageHeight-1))
				{
					//cout << "b" << endl;
					currentMeshPointInMesh->adjacentMeshPoint[q] = meshPointArray[ky*imageWidth + kx];
					currentMeshPointInMesh->adjacentMeshPointFilled[q] = true;

				}
				else
				{
					currentMeshPointInMesh->adjacentMeshPoint[q] = NULL;
					currentMeshPointInMesh->adjacentMeshPointFilled[q] = false;
				}

			}
			*/
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


	if(OR_USE_CONTRAST_CALC_METHOD_C)
	{
		//calculate mesh point properties
		for(int y = 0; y < (imageHeight-1); y++)
		{
			for(int x = 0; x < (imageWidth-1); x++)
			{
				calculateMeshPointInterpixelLuminosityContrast(meshPointArray[y*imageWidth + x]);

				bool alreadyFilledPointValues = false;
				bool passCondition = true;
				if(OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED)
				{
					if(useEdgeZeroCrossingMap)
					{
						if(meshPointInterpixelArray[y*imageWidth + x]->edge == true)
						{//use depth and point values from quadratic fit;
							meshPointInterpixelArray[y*imageWidth + x]->depth = meshPointInterpixelArray[y*imageWidth + x]->quadraticFitDepth;
							meshPointInterpixelArray[y*imageWidth + x]->point.x = meshPointInterpixelArray[y*imageWidth + x]->quadraticFitPoint.x;
							meshPointInterpixelArray[y*imageWidth + x]->point.y = meshPointInterpixelArray[y*imageWidth + x]->quadraticFitPoint.y;
							meshPointInterpixelArray[y*imageWidth + x]->point.z = meshPointInterpixelArray[y*imageWidth + x]->quadraticFitPoint.z;
							alreadyFilledPointValues = true;
						}
						else
						{
							calculateMeshPointInterpixelDepth(meshPointArray[y*imageWidth + x]);
						}
						passCondition = false;
					}
				}
				if(passCondition)
				{
					#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS
					calculateMeshPointInterpixelDepthWithForegroundDepthCheck(meshPointArray[y*imageWidth + x]);
					#else
					calculateMeshPointInterpixelDepth(meshPointArray[y*imageWidth + x]);
					#endif

				}

				if(!alreadyFilledPointValues)
				{
					double pixelXOffset;
					double pixelYOffset;
					if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_RGB_LUMINOSITY_DEPTH_OR_POINT)
					{
						//without OR_USE_CONTRAST_CALC_METHOD_C
						pixelXOffset = 0;
						pixelYOffset = 0;
					}
					else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_LUMINOSITY_OR_DEPTH_CONTRAST)
					{
						//such as luminosityContrast map with OR_USE_CONTRAST_CALC_METHOD_C
						pixelXOffset = 0.5;
						pixelYOffset = 0.5;
					}
					else if(interpixelContrastMapType == INTERPIXEL_CONTRAST_MAP_TYPE_NORMAL_OR_GRADIENT_CONTRAST)
					{
						//such as gradientContrast or pointNormalContrast map with OR_USE_CONTRAST_CALC_METHOD_C
						pixelXOffset = 0;
						pixelYOffset = 0;
					}
					vec xyzWorldInterpixel;
					double depth = meshPointInterpixelArray[y*imageWidth + x]->depth;
					calculatePointMapValue(x+pixelXOffset, y+pixelYOffset, depth, &xyzWorldInterpixel, vi);
					meshPointInterpixelArray[y*imageWidth + x]->point.x = xyzWorldInterpixel.x;
					meshPointInterpixelArray[y*imageWidth + x]->point.y = xyzWorldInterpixel.y;
					meshPointInterpixelArray[y*imageWidth + x]->point.z = xyzWorldInterpixel.z;
				}


				if(OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST)
				{
					//emulates createPointNormalMapFromPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointNormalMap):
					//and emulates createPointNormalContrastMapFromPointNormalMap(int imageWidth, int imageHeight, double* pointNormalMap, double* pointNormalContrastMap):
					calculateMeshPointInterpixelNormal(meshPointArray[y*imageWidth + x]);
					if((x >=1) && (y >=1))
					{
						calculateMeshPointInterpixelNormalContrast(meshPointArray[y*imageWidth + x]);
					}
					(meshPointInterpixelArray[y*imageWidth + x])->meshPointNormalFilled = true;
				}

				#ifdef OR_DEBUG
				/*
				cout << "meshPointInterpixelArray[y*imageWidth + x]->luminosityContrast = " << meshPointInterpixelArray[y*imageWidth + x]->luminosityContrast << endl;
				cout << "meshPointInterpixelArray[y*imageWidth + x]->meshPointNormal.x = " << meshPointInterpixelArray[y*imageWidth + x]->meshPointNormal.x << endl;
				cout << "meshPointInterpixelArray[y*imageWidth + x]->meshPointNormal.y = " << meshPointInterpixelArray[y*imageWidth + x]->meshPointNormal.y << endl;
				cout << "meshPointInterpixelArray[y*imageWidth + x]->meshPointNormal.z = " << meshPointInterpixelArray[y*imageWidth + x]->meshPointNormal.z << endl;
				*/
				#endif
			}
		}
	}
	else
	{
		//calculate mesh point properties
		for(int y = 1; y < (imageHeight-1); y++)
		{
			for(int x = 1; x < (imageWidth-1); x++)
			{
				calculateMeshPointLuminosityContrast(meshPointArray[y*imageWidth + x]);

				if(OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST)
				{
					//emulates createPointNormalMapFromPointMap(int imageWidth, int imageHeight, double* pointMap, double* pointNormalMap):
					//and emulates createPointNormalContrastMapFromPointNormalMap(int imageWidth, int imageHeight, double* pointNormalMap, double* pointNormalContrastMap):
					calculateMeshPointNormal(meshPointArray[y*imageWidth + x]);
					calculateMeshPointNormalContrast(meshPointArray[y*imageWidth + x]);
					(meshPointArray[y*imageWidth + x])->meshPointNormalFilled = true;
				}

				#ifdef OR_DEBUG
				/*
				cout << "meshPointArray[y*imageWidth + x]->luminosityContrast = " << meshPointArray[y*imageWidth + x]->luminosityContrast << endl;
				cout << "meshPointArray[y*imageWidth + x]->meshPointNormal.x = " << meshPointArray[y*imageWidth + x]->meshPointNormal.x << endl;
				cout << "meshPointArray[y*imageWidth + x]->meshPointNormal.y = " << meshPointArray[y*imageWidth + x]->meshPointNormal.y << endl;
				cout << "meshPointArray[y*imageWidth + x]->meshPointNormal.z = " << meshPointArray[y*imageWidth + x]->meshPointNormal.z << endl;
				cout << "meshPointArray[y*imageWidth + x]->point.x = " << meshPointArray[y*imageWidth + x]->point.x << endl;
				cout << "meshPointArray[y*imageWidth + x]->point.y = " << meshPointArray[y*imageWidth + x]->point.y << endl;
				cout << "meshPointArray[y*imageWidth + x]->point.z = " << meshPointArray[y*imageWidth + x]->point.z << endl;
				*/
				#endif
			}
		}
	}


	if(OR_METHOD_3DOD_USE_MESH_LISTS_COMBINED)
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
						getPointMapValue(x, y, imageWidth, pointMap, &borderPoint);
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

						if(calculateTheDistanceBetweenTwoPoints(&(nearestMeshpointInExistingMesh->point), &(meshpointInAdditionalMesh->point)) < OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE)
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

							if(OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST)
							{
								//now recalculate mesh point normal and normal contrast values [should recalculate other values also]
								calculateMeshPointNormal(nearestMeshpointInExistingMesh);	//or meshpointInAdditionalMesh
								calculateMeshPointNormalContrast(nearestMeshpointInExistingMesh);	//or meshpointInAdditionalMesh
							}
						}
					}
				}
			}

			//B) join foreground mesh points of depth contrast edges with existing mesh mesh points
			double* depthContrastMap = new double[imageWidth* imageHeight];
			bool* contrastBooleanMap = new bool[imageWidth*imageHeight];
			bool* depthContrastBooleanMap = new bool[imageWidth*imageHeight];

		#ifdef OR_USE_FOREGROUND_DEPTH_CHECKS_OLD
			createContrastMapFromMapWithForegroundDepthCheck(imageWidth, imageHeight, depthMap, depthMap, depthContrastMap, contrastBooleanMap, foregroundDepthCheckDepthContrastBooleanMap, EDGE_DEPTH_CONTRAST_THRESHOLD);
		#else
			createContrastMapFromMap(imageWidth, imageHeight, depthMap, depthContrastMap);
			createDepthContrastBooleanMap(imageWidth, imageHeight, depthContrastMap, depthContrastBooleanMap);	//need to check threshold here
		#endif

			currentMeshPointInMesh = firstNewMeshPointInMesh;
			for(int y = 0; y < (imageHeight); y++)
			{
				for(int x = 0; x < (imageWidth); x++)
				{
					if(getBooleanMapValue(x, y, imageWidth, depthContrastBooleanMap))
					{
						//depth contrast edge found, so join

						vec depthContrastEdgePoint;
						getPointMapValue(x, y, imageWidth, pointMap, &depthContrastEdgePoint);
						ORmeshPoint* nearestMeshpointInExistingMesh;
						bool hasFoundMeshPoint = false;
						nearestMeshpointInExistingMesh = findMeshPointIntInMesh(firstMeshPointInMeshList, &depthContrastEdgePoint, &hasFoundMeshPoint, numMeshPointsInExistingMesh);
						if(hasFoundMeshPoint == false)
						{
							cout << "error: cannot find adjacent meshpoint" << endl;
							exit(0);
						}

						ORmeshPoint* meshpointInAdditionalMesh;
						meshpointInAdditionalMesh = meshPointArray[y*imageWidth + x];

						if(calculateTheDistanceBetweenTwoPoints(&(nearestMeshpointInExistingMesh->point), &(meshpointInAdditionalMesh->point)) < OR_METHOD_3DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE)
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

							if(OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST)
							{
								//now recalculate mesh point normal and normal contrast values [should recalculate other values also]
								calculateMeshPointNormal(nearestMeshpointInExistingMesh);	//or meshpointInAdditionalMesh
								calculateMeshPointNormalContrast(nearestMeshpointInExistingMesh);	//or meshpointInAdditionalMesh
							}
						}
					}
				}
			}

			delete depthContrastMap;
			delete contrastBooleanMap;
			delete depthContrastBooleanMap;

		}
	}

	#ifndef LINUX
	delete meshPointInterpixelArray;
	delete edgeZeroCrossingMap;
	#endif

}





void create3DMeshReferenceListUsingPointMap(int imageWidth, int imageHeight, double* pointMap, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList)
{

	ORmeshPoint* currentMeshPointInMesh = firstMeshPointInMeshList;
	for(int y = 0; y < (imageHeight); y++)
	{
		for(int x = 0; x < (imageWidth); x++)
		{
			vec centreVec;
			getPointMapValue(x, y, imageWidth, pointMap, &centreVec);

			colour col;
			getRGBMapValues(x, y, imageWidth, rgbMap, &col);
			currentMeshPointInMesh->col.r = col.r;
			currentMeshPointInMesh->col.g = col.g;
			currentMeshPointInMesh->col.b = col.b;
			currentMeshPointInMesh->point.x = centreVec.x;
			currentMeshPointInMesh->point.y = centreVec.y;
			currentMeshPointInMesh->point.z = centreVec.z;
			currentMeshPointInMesh->xInt = x;
			currentMeshPointInMesh->yInt = x;

			ORmeshPoint* newMeshPoint = new ORmeshPoint();
			currentMeshPointInMesh->next = newMeshPoint;
			currentMeshPointInMesh = currentMeshPointInMesh->next;

		}
	}

	currentMeshPointInMesh = firstMeshPointInMeshList;
	for(int y = 0; y < (imageHeight); y++)
	{
		for(int x = 0; x < (imageWidth); x++)
		{
			for(int q = 0; q < 9; q++)
			{
				int kx;
				int ky;

				kx = x + q%3 - 1;
				ky = y + q/3 - 1;

				if((kx >= 0) && (kx <= imageWidth-1) && (ky >= 0) && (ky <= imageHeight-1))
				{
					bool hasFoundMeshPoint = false;
					currentMeshPointInMesh->adjacentMeshPoint[q] = findMeshPointIntInMesh(firstMeshPointInMeshList, kx, ky, &hasFoundMeshPoint);
					if(hasFoundMeshPoint == false)
					{
						cout << "error: cannot find adjacent meshpoint" << endl;
						exit(0);
					}
				}
				else
				{
					currentMeshPointInMesh->adjacentMeshPoint[q] = NULL;
				}
			}

			currentMeshPointInMesh = currentMeshPointInMesh->next;

		}
	}
}







#ifdef OR_METHOD_3DOD_USE_OLD_TESTED_BUT_BASIC_FEATURE_DETECTION
bool generateFeatureList3DOD(RTviewInfo* vi, double* depthMap, double* pointMap, bool* depthContrastBooleanMap, bool* luminosityContrastBooleanMap, bool* luminosityContrastMapMinusDepthContrastMap, ORfeature* firstFeatureInList, int trainOrTest)
{
	bool result = true;

	string trainOrTestString;
	if((trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST))
	{
		trainOrTestString = TRAIN_STRING;
	}
	else if(trainOrTest == TEST)
	{
		trainOrTestString = TEST_STRING;
	}

	int imageWidth = vi->imageWidth;
	int imageHeight = vi->imageHeight;


	bool* featuresUsingDepthContrastMap = new bool[imageWidth*imageHeight];
	bool* featuresUsingDepthContrastMapComplete = new bool[imageWidth*imageHeight];
	bool* featuresUsingLuminosityContrastMapMinusDepthContrastMap = new bool[imageWidth*imageHeight];
	bool* featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete = new bool[imageWidth*imageHeight];
	bool* featuresMapComplete = new bool[imageWidth*imageHeight];
	bool* featuresMapCompleteOfficial = new bool[imageWidth*imageHeight];


	//now locate features of object using depthContrastMap [and optionally using luminosityContrastMapMinusDepthContrastMap also]

	int maxDotProductResultXposArrayCompleteMap1[3][3][3];
	int maxDotProductResultYposArrayCompleteMap1[3][3][3];

	createFeaturesUsingBooleanMapUsingDepthMap(imageWidth, imageHeight, depthContrastBooleanMap, depthMap, featuresUsingDepthContrastMap, featuresUsingDepthContrastMapComplete, maxDotProductResultXposArrayCompleteMap1, maxDotProductResultYposArrayCompleteMap1, vi, pointMap);
	//THIS ALTERNATE METHOD DOESNT WORK WHY NOT?; createFeaturesUsingBooleanMapUsingPointMap(imageWidth, imageHeight, depthContrastBooleanMap, pointMap, featuresUsingDepthContrastMap, featuresUsingDepthContrastMapComplete, maxDotProductResultXposArrayCompleteMap1, maxDotProductResultYposArrayCompleteMap1);

	int maxDotProductResultXposArrayCompleteMap2[3][3][3];
	int maxDotProductResultYposArrayCompleteMap2[3][3][3];

	createFeaturesUsingBooleanMapUsingDepthMap(imageWidth, imageHeight, luminosityContrastMapMinusDepthContrastMap, depthMap, featuresUsingLuminosityContrastMapMinusDepthContrastMap, featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete, maxDotProductResultXposArrayCompleteMap2, maxDotProductResultYposArrayCompleteMap2, vi, pointMap);
	//THIS ALTERNATE METHOD DOESNT WORK WHY NOT?; createFeaturesUsingBooleanMapUsingPointMap(imageWidth, imageHeight, luminosityContrastMapMinusDepthContrastMap, pointMap, featuresUsingLuminosityContrastMapMinusDepthContrastMap, featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete, maxDotProductResultXposArrayCompleteMap2, maxDotProductResultYposArrayCompleteMap2);

	addBooleanMaps(imageWidth, imageHeight, featuresUsingDepthContrastMapComplete, featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete, featuresMapComplete);
	reconcileFeaturesMap(imageWidth, imageHeight, featuresMapComplete);	//may no longer be required
		//required after joining boolean corner maps

	//now define polygons for which transformations will occur for nn experience feeding features

	#ifndef OR_DEBUG_OLD_FEATURE_GENERATION_METHOD
	//NEW METHOD;
	generateFeatureListUsingFeatureArraysUsingPointMap(imageWidth, imageHeight, pointMap, maxDotProductResultXposArrayCompleteMap1, maxDotProductResultYposArrayCompleteMap1, firstFeatureInList);
	generateFeatureListUsingFeatureArraysUsingPointMap(imageWidth, imageHeight, pointMap, maxDotProductResultXposArrayCompleteMap2, maxDotProductResultYposArrayCompleteMap2, firstFeatureInList);
	#else
	//OLD METHOD;
	generateFeatureListUsingFeatureArraysUsingDepthMap(imageWidth, imageHeight, depthMap, maxDotProductResultXposArrayCompleteMap1, maxDotProductResultYposArrayCompleteMap1, firstFeatureInList, vi);
	generateFeatureListUsingFeatureArraysUsingDepthMap(imageWidth, imageHeight, depthMap, maxDotProductResultXposArrayCompleteMap2, maxDotProductResultYposArrayCompleteMap2, firstFeatureInList, vi);
	#endif

	//#ifdef OR_DEBUG
	ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		cout << "currentFeatureInList++" << endl;
		currentFeatureInList=currentFeatureInList->next;
	}
	//#endif

	generateBooleanMapFromFeatureList(imageWidth, imageHeight, firstFeatureInList, featuresMapCompleteOfficial, vi, 1);
	//reconcileFeaturesMap(imageWidth, imageHeight, featuresMapCompleteOfficial);	//featuresMapCompleteOfficial shouldnt need reconciliation as features have already been checked to not reside close to each other! - fix this!

	string featuresMapCompleteFileNameCPlus =  "featuresMapComplete" + trainOrTestString + PPM_EXTENSION;
	string featuresMapCompleteOfficialFileNameCPlus = "featuresMapCompleteOfficial" + trainOrTestString + PPM_EXTENSION;
	string featuresUsingDepthContrastMapCompleteFileNameCPlus = "featuresUsingDepthContrastMapComplete" + trainOrTestString + PPM_EXTENSION;
	string featuresUsingDepthContrastMapFileNameCPlus = "featuresUsingDepthContrastMap" + trainOrTestString + PPM_EXTENSION;
	string featuresUsingLuminosityContrastMapMinusDepthContrastMapCompleteFileNameCPlus = "featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete" + trainOrTestString + PPM_EXTENSION;
	string featuresUsingLuminosityContrastMapMinusDepthContrastMapFileNameCPlus = "featuresUsingLuminosityContrastMapMinusDepthContrastMap" + trainOrTestString + PPM_EXTENSION;

	char* featuresMapCompleteFileName = const_cast<char*>(featuresMapCompleteFileNameCPlus.c_str());
	char* featuresMapCompleteOfficialFileName = const_cast<char*>(featuresMapCompleteOfficialFileNameCPlus.c_str());
	char* featuresUsingDepthContrastMapCompleteFileName = const_cast<char*>(featuresUsingDepthContrastMapCompleteFileNameCPlus.c_str());
	char* featuresUsingDepthContrastMapFileName = const_cast<char*>(featuresUsingDepthContrastMapFileNameCPlus.c_str());
	char* featuresUsingLuminosityContrastMapMinusDepthContrastMapCompleteFileName = const_cast<char*>(featuresUsingLuminosityContrastMapMinusDepthContrastMapCompleteFileNameCPlus.c_str());
	char* featuresUsingLuminosityContrastMapMinusDepthContrastMapFileName = const_cast<char*>(featuresUsingLuminosityContrastMapMinusDepthContrastMapFileNameCPlus.c_str());

	generatePixmapFromBooleanMap(featuresMapCompleteFileName, imageWidth, imageHeight, featuresMapComplete);
	generatePixmapFromBooleanMap(featuresMapCompleteOfficialFileName, imageWidth, imageHeight, featuresMapCompleteOfficial);
	//generatePixmapFromBooleanMap(featuresUsingDepthContrastMapFileName, imageWidth, imageHeight, featuresUsingDepthContrastMap);
	generatePixmapFromBooleanMap(featuresUsingDepthContrastMapCompleteFileName, imageWidth, imageHeight, featuresUsingDepthContrastMapComplete);
	//generatePixmapFromBooleanMap(featuresUsingLuminosityContrastMapMinusDepthContrastMapFileName, imageWidth, imageHeight, featuresUsingLuminosityContrastMapMinusDepthContrastMap);
	generatePixmapFromBooleanMap(featuresUsingLuminosityContrastMapMinusDepthContrastMapCompleteFileName, imageWidth, imageHeight, featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete);

	delete featuresUsingDepthContrastMap;
	delete featuresUsingDepthContrastMapComplete;
	delete featuresUsingLuminosityContrastMapMinusDepthContrastMap;
	delete featuresUsingLuminosityContrastMapMinusDepthContrastMapComplete;
	delete featuresMapComplete;
	delete featuresMapCompleteOfficial;

	//{NB must use luminosity contrast rather than depth contrast for sphere detection (ie detect text on spheres)}

	return result;
}


	//may no longer be required?
void reconcileFeaturesMap(int imageWidth, int imageHeight, bool* featuresBooleanMap)
{
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			bool booleanVal = getBooleanMapValue(x, y, imageWidth, featuresBooleanMap);
			if(booleanVal)
			{
				int numberOfHitsFound = 0;
				for(int ky = y-(FEATURES_BOOLEAN_MAP_RECONCILIATION_HEIGHT/2); ky <= y+(FEATURES_BOOLEAN_MAP_RECONCILIATION_HEIGHT/2); ky++)
				{
					for(int kx = x-(FEATURES_BOOLEAN_MAP_RECONCILIATION_WIDTH/2); kx <= x+(FEATURES_BOOLEAN_MAP_RECONCILIATION_WIDTH/2); kx++)
					{
						if((ky >= 0) && (ky < imageHeight))
						{
							if((kx >= 0) && (kx < imageWidth))
							{
								bool booleanValInKernel = getBooleanMapValue(kx, ky, imageWidth, featuresBooleanMap);
								if(booleanValInKernel)
								{
									#ifdef OR_DEBUG
									/*
									cout << "hit found;" << endl;
									cout << "x = " << x << endl;
									cout << "y = " << y << endl;
									*/
									#endif

									numberOfHitsFound++;
								}
							}
						}
					}
				}

				if(numberOfHitsFound >= 2)
				{
					#ifdef OR_DEBUG
					/*
					cout << "numberOfHitsFound >= 2" << endl;
					cout << "x = " << x << endl;
					cout << "y = " << y << endl;
					*/
					#endif

					for(int ky = y-(FEATURES_BOOLEAN_MAP_RECONCILIATION_HEIGHT/2); ky <= y+(FEATURES_BOOLEAN_MAP_RECONCILIATION_HEIGHT/2); ky++)
					{
						for(int kx = x-(FEATURES_BOOLEAN_MAP_RECONCILIATION_WIDTH/2); kx <= x+(FEATURES_BOOLEAN_MAP_RECONCILIATION_WIDTH/2); kx++)
						{
							if((ky >= 0) && (ky < imageHeight))
							{
								if((kx >= 0) && (kx < imageWidth))
								{
									if((kx == x) && (ky == y))
									{//do nothing do not change curent pixel

									}
									else
									{
										#ifdef OR_DEBUG
										//cout << "\treconciling" << endl;
										#endif
										setBooleanMapValue(kx, ky, imageWidth, false, featuresBooleanMap);
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





void createFeaturesUsingBooleanMap(int imageWidth, int imageHeight, bool* booleanMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], RTviewInfo* vi)
{
	double imageSizeWidth = vi->imageWidth;
	double imageSizeHeight = vi->imageHeight;
	double viewSizeWidth = vi->viewWidth;
	double viewSizeHeight = vi->viewHeight;

	//for general features;

		//BASIC
	//1. only for all combinations of +x, +y, +z, -x, -y, -z;
	int unitVectorArray[2][VECTOR_NUM_VALUES];
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_X] = -1;
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_Y] = -1;
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_Z] = -1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_X] = 1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_Y] = 1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_Z] = 1;


		//COMPLETE
	//1. for all combinations of +x, +y, +z, -x, -y, -z;

	//2. for wedge extrusions
	//for all combinations of +x, +y, +z, -x, -y, -z, minus one axis;
		//{ie; [+x 0, +z, -x, 0, -z] , [+x +y, 0, -x, -y, 0], and [0, +y, +z, 0, -y, -z]}

	//3. For vertex extrusions
	//for all combinations of +x, +y, +z, -x, -y, -z, minus two axes;
		//{ie; [+x 0, +z, -x, 0, -z] , [+x +y, 0, -x, -y, 0], and [0, +y, +z, 0, -y, -z]}



	double maxDotProductResultArrayComplete[3][3][3];
	/*
	int maxDotProductResultXposArrayComplete[3][3][3];
	int maxDotProductResultYposArrayComplete[3][3][3];
	*/

	double maxDotProductResultArray[2][2][2];
	int maxDotProductResultXPosArray[2][2][2];
	int maxDotProductResultYPosArray[2][2][2];

	for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
			{
				maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex] = 0.0;
				maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] = 0;
				maxDotProductResultYPosArray[uvzIndex][uvyIndex][uvzIndex] = 0;
			}
		}
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				maxDotProductResultArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0.0;
				maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0;
				maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0;
			}
		}
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			bool boolVal = getBooleanMapValue(x, y, imageWidth, booleanMap);

			if(boolVal)
			{
					//now for all features;
					//use dot product [pixel* unit vector]

				//REQUIRED;


				for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
				{
					for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
					{
						for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
						{
							int uvx = unitVectorArray[uvxIndex][VECTOR_VAL_X];
							int uvy = unitVectorArray[uvyIndex][VECTOR_VAL_Y];
							int uvz = unitVectorArray[uvzIndex][VECTOR_VAL_Z];
							vec pixelVector;
							vec unitVector;

							pixelVector.x = x;
							pixelVector.y = y;
							pixelVector.z = 0;

							unitVector.x = (double)uvx;
							unitVector.y = (double)uvy;
							unitVector.z = (double)uvz;

							double bias = dotProduct(&pixelVector, &unitVector);
							if(bias > maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex])
							{
								maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex] = bias;
								maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] = x;
								maxDotProductResultYPosArray[uvzIndex][uvyIndex][uvzIndex] = y;
							}
						}
					}
				}


				//COMPLETE;

				for(int uvxIndex = -1; uvxIndex <= 1; uvxIndex++)
				{
					for(int uvyIndex = -1; uvyIndex <= 1; uvyIndex++)
					{
						for(int uvzIndex = -1; uvzIndex <= 1; uvzIndex++)
						{

							int uvx = uvxIndex;
							int uvy = uvyIndex;
							int uvz = uvzIndex;

							if((uvx == 0) && (uvy == 0) && (uvz == 0))
							{

							}
							else
							{
								vec pixelVector;
								vec unitVector;

								pixelVector.x = x;
								pixelVector.y = y;
								pixelVector.z = 0.0;

								unitVector.x = (double)uvx;
								unitVector.y = (double)uvy;
								unitVector.z = (double)uvz;

								double bias = dotProduct(&pixelVector, &unitVector);
								if(bias > maxDotProductResultArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1])
								{
									maxDotProductResultArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = bias;
									maxDotProductResultXposArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = x;
									maxDotProductResultYposArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = y;
								}
							}
						}
					}
				}
			}
		}
	}

		//initialise boolean maps;
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, featuresUsingContrastMap);
			setBooleanMapValue(x, y, imageWidth, false, featuresUsingContrastMapComplete);
		}
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
			{
				for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
				{
					for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
					{
						if((maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] == x) && (maxDotProductResultYPosArray[uvxIndex][uvyIndex][uvzIndex] == y))
						{
							setBooleanMapValue(x, y, imageWidth, true, featuresUsingContrastMap);
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
						if((maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex] == x) && (maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex] == y))
						{
							#ifdef OR_DEBUG
							/*
							double depth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);
							vec xyzWorld;
							getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);
							cout << "\n createFeaturesUsingBooleanMapUsingDepthMap() FOUND FEATURE;" << endl;
							cout << "x = " << x << endl;
							cout << "y = " << y << endl;
							cout << "depth = " << depth << endl;
							cout << "xyzWorld.x = " << xyzWorld.x << endl;
							cout << "xyzWorld.y = " << xyzWorld.y << endl;
							cout << "xyzWorld.z = " << xyzWorld.z << endl;
							*/
							#endif

							setBooleanMapValue(x, y, imageWidth, true, featuresUsingContrastMapComplete);
						}
					}
				}
			}
		}
	}

	//now we have the nearest 3 features to each corner using the outlined (common unit vector) scenarios
}


void createFeaturesUsingBooleanMapUsingDepthMap(int imageWidth, int imageHeight, bool* booleanMap, double* depthMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], RTviewInfo* vi, double* pointMap)
{
	double imageSizeWidth = vi->imageWidth;
	double imageSizeHeight = vi->imageHeight;
	double viewSizeWidth = vi->viewWidth;
	double viewSizeHeight = vi->viewHeight;

	//for general features;

		//BASIC
	//1. only for all combinations of +x, +y, +z, -x, -y, -z;
	int unitVectorArray[2][VECTOR_NUM_VALUES];
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_X] = -1;
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_Y] = -1;
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_Z] = -1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_X] = 1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_Y] = 1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_Z] = 1;


		//COMPLETE
	//1. for all combinations of +x, +y, +z, -x, -y, -z;

	//2. for wedge extrusions
	//for all combinations of +x, +y, +z, -x, -y, -z, minus one axis;
		//{ie; [+x 0, +z, -x, 0, -z] , [+x +y, 0, -x, -y, 0], and [0, +y, +z, 0, -y, -z]}

	//3. For vertex extrusions
	//for all combinations of +x, +y, +z, -x, -y, -z, minus two axes;
		//{ie; [+x 0, +z, -x, 0, -z] , [+x +y, 0, -x, -y, 0], and [0, +y, +z, 0, -y, -z]}



	double maxDotProductResultArrayComplete[3][3][3];
	/*
	int maxDotProductResultXposArrayComplete[3][3][3];
	int maxDotProductResultYposArrayComplete[3][3][3];
	*/

	double maxDotProductResultArray[2][2][2];
	int maxDotProductResultXPosArray[2][2][2];
	int maxDotProductResultYPosArray[2][2][2];

	for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
			{
				maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex] = 0.0;
				maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] = 0;
				maxDotProductResultYPosArray[uvzIndex][uvyIndex][uvzIndex] = 0;
			}
		}
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				maxDotProductResultArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0.0;
				maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0;
				maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0;
			}
		}
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			bool depthContrastBoolVal = getBooleanMapValue(x, y, imageWidth, booleanMap);
			double z = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

			if(depthContrastBoolVal)
			{
					//now for all features;
					//use dot product [pixel* unit vector]

				//REQUIRED;


				for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
				{
					for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
					{
						for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
						{
							int uvx = unitVectorArray[uvxIndex][VECTOR_VAL_X];
							int uvy = unitVectorArray[uvyIndex][VECTOR_VAL_Y];
							int uvz = unitVectorArray[uvzIndex][VECTOR_VAL_Z];
							vec pixelVector;
							vec unitVector;


							vec xyzWorld;
							calculatePointMapValue(x, y, z, &xyzWorld, vi);


							pixelVector.x = xyzWorld.x;
							pixelVector.y = xyzWorld.y;
							pixelVector.z = xyzWorld.z;

							unitVector.x = (double)uvx;
							unitVector.y = (double)uvy;
							unitVector.z = (double)uvz;

							double bias = dotProduct(&pixelVector, &unitVector);
							if(bias > maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex])
							{
								maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex] = bias;
								maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] = x;
								maxDotProductResultYPosArray[uvzIndex][uvyIndex][uvzIndex] = y;
							}
						}
					}
				}


				//COMPLETE;

				for(int uvxIndex = -1; uvxIndex <= 1; uvxIndex++)
				{
					for(int uvyIndex = -1; uvyIndex <= 1; uvyIndex++)
					{
						for(int uvzIndex = -1; uvzIndex <= 1; uvzIndex++)
						{

							int uvx = uvxIndex;
							int uvy = uvyIndex;
							int uvz = uvzIndex;

							if((uvx == 0) && (uvy == 0) && (uvz == 0))
							{

							}
							else
							{
								vec pixelVector;
								vec unitVector;

								vec xyzWorld;
								calculatePointMapValue(x, y, z, &xyzWorld, vi);
								pixelVector.x = xyzWorld.x;
								pixelVector.y = xyzWorld.y;
								pixelVector.z = xyzWorld.z;

								unitVector.x = (double)uvx;
								unitVector.y = (double)uvy;
								unitVector.z = (double)uvz;

								double bias = dotProduct(&pixelVector, &unitVector);
								if(bias > maxDotProductResultArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1])
								{
									maxDotProductResultArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = bias;
									maxDotProductResultXposArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = x;
									maxDotProductResultYposArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = y;
								}
							}
						}
					}
				}
			}
		}
	}

		//initialise boolean maps;
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, featuresUsingContrastMap);
			setBooleanMapValue(x, y, imageWidth, false, featuresUsingContrastMapComplete);
		}
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
			{
				for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
				{
					for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
					{
						if((maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] == x) && (maxDotProductResultYPosArray[uvxIndex][uvyIndex][uvzIndex] == y))
						{
							setBooleanMapValue(x, y, imageWidth, true, featuresUsingContrastMap);
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
						if((maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex] == x) && (maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex] == y))
						{
							#ifdef OR_DEBUG
							/*
							double depth = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);
							vec xyzWorld;
							getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);
							cout << "\n createFeaturesUsingBooleanMapUsingDepthMap() FOUND FEATURE;" << endl;
							cout << "x = " << x << endl;
							cout << "y = " << y << endl;
							cout << "depth = " << depth << endl;
							cout << "xyzWorld.x = " << xyzWorld.x << endl;
							cout << "xyzWorld.y = " << xyzWorld.y << endl;
							cout << "xyzWorld.z = " << xyzWorld.z << endl;
							*/
							#endif

							setBooleanMapValue(x, y, imageWidth, true, featuresUsingContrastMapComplete);
						}
					}
				}
			}
		}
	}

	//now we have the nearest 3 features to each corner using the outlined (common unit vector) scenarios
}

void createFeaturesUsingBooleanMapUsingPointMap(int imageWidth, int imageHeight, bool* booleanMap, double* pointMap, bool* featuresUsingContrastMap, bool* featuresUsingContrastMapComplete, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3])
{

	//for general features;

		//BASIC
	//1. only for all combinations of +x, +y, +z, -x, -y, -z;
	int unitVectorArray[2][VECTOR_NUM_VALUES];
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_X] = -1;
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_Y] = -1;
	unitVectorArray[MATRIX_VEC_A][VECTOR_VAL_Z] = -1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_X] = 1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_Y] = 1;
	unitVectorArray[MATRIX_VEC_B][VECTOR_VAL_Z] = 1;


		//COMPLETE
	//1. for all combinations of +x, +y, +z, -x, -y, -z;

	//2. for wedge extrusions
	//for all combinations of +x, +y, +z, -x, -y, -z, minus one axis;
		//{ie; [+x 0, +z, -x, 0, -z] , [+x +y, 0, -x, -y, 0], and [0, +y, +z, 0, -y, -z]}

	//3. For vertex extrusions
	//for all combinations of +x, +y, +z, -x, -y, -z, minus two axes;
		//{ie; [+x 0, +z, -x, 0, -z] , [+x +y, 0, -x, -y, 0], and [0, +y, +z, 0, -y, -z]}



	double maxDotProductResultArrayComplete[3][3][3];
	/*
	int maxDotProductResultXposArrayComplete[3][3][3];
	int maxDotProductResultYposArrayComplete[3][3][3];
	*/

	double maxDotProductResultArray[2][2][2];
	int maxDotProductResultXPosArray[2][2][2];
	int maxDotProductResultYPosArray[2][2][2];

	for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
			{
				maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex] = 0.0;
				maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] = 0;
				maxDotProductResultYPosArray[uvzIndex][uvyIndex][uvzIndex] = 0;
			}
		}
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				maxDotProductResultArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0.0;
				maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0;
				maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex] = 0;
			}
		}
	}



	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			bool depthContrastBoolVal = getBooleanMapValue(x, y, imageWidth, booleanMap);

			double xWorld;
			double yWorld;
			double zWorld;
			vec xyzWorld;
			getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);
			xWorld = xyzWorld.x;
			yWorld = xyzWorld.y;
			zWorld = xyzWorld.z;


			if(depthContrastBoolVal)
			{
					//now for all features;
					//use dot product [pixel* unit vector]

				//REQUIRED;


				for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
				{
					for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
					{
						for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
						{
							int uvx = unitVectorArray[uvxIndex][VECTOR_VAL_X];
							int uvy = unitVectorArray[uvyIndex][VECTOR_VAL_Y];
							int uvz = unitVectorArray[uvzIndex][VECTOR_VAL_Z];
							vec pixelVector;
							vec unitVector;

							pixelVector.x = xWorld;
							pixelVector.y = yWorld;
							pixelVector.z = zWorld;

							unitVector.x = (double)uvx;
							unitVector.y = (double)uvy;
							unitVector.z = (double)uvz;

							double bias = dotProduct(&pixelVector, &unitVector);
							if(bias > maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex])
							{
								maxDotProductResultArray[uvxIndex][uvyIndex][uvzIndex] = bias;
								maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] = x;
								maxDotProductResultYPosArray[uvzIndex][uvyIndex][uvzIndex] = y;

							}
						}
					}
				}


				//COMPLETE;

				for(int uvxIndex = -1; uvxIndex <= 1; uvxIndex++)
				{
					for(int uvyIndex = -1; uvyIndex <= 1; uvyIndex++)
					{
						for(int uvzIndex = -1; uvzIndex <= 1; uvzIndex++)
						{

							int uvx = uvxIndex;
							int uvy = uvyIndex;
							int uvz = uvzIndex;

							if((uvx == 0) && (uvy == 0) && (uvz == 0))
							{

							}
							else
							{
								vec pixelVector;
								vec unitVector;

								pixelVector.x = xWorld;
								pixelVector.y = yWorld;
								pixelVector.z = zWorld;

								unitVector.x = (double)uvx;
								unitVector.y = (double)uvy;
								unitVector.z = (double)uvz;

								double bias = dotProduct(&pixelVector, &unitVector);
								if(bias > maxDotProductResultArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1])
								{
									#ifdef OR_DEBUG
									/*
									cout << "found a corner" << endl;
									cout << "x = " << x << endl;
									cout << "y = " << y << endl;
									*/
									#endif

									maxDotProductResultArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = bias;
									maxDotProductResultXposArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = x;
									maxDotProductResultYposArrayComplete[uvxIndex+1][uvyIndex+1][uvzIndex+1] = y;
								}
							}
						}
					}
				}
			}
		}
	}

		//initialise boolean maps;
	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			setBooleanMapValue(x, y, imageWidth, false, featuresUsingContrastMap);
			setBooleanMapValue(x, y, imageWidth, false, featuresUsingContrastMapComplete);
		}
	}

	for(int y = 0; y < imageHeight; y++)
	{
		for(int x = 0; x < imageWidth; x++)
		{
			for(int uvxIndex = 0; uvxIndex < 2; uvxIndex++)
			{
				for(int uvyIndex = 0; uvyIndex < 2; uvyIndex++)
				{
					for(int uvzIndex = 0; uvzIndex < 2; uvzIndex++)
					{
						if((maxDotProductResultXPosArray[uvxIndex][uvyIndex][uvzIndex] == x) && (maxDotProductResultYPosArray[uvxIndex][uvyIndex][uvzIndex] == y))
						{
							setBooleanMapValue(x, y, imageWidth, true, featuresUsingContrastMap);
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
						if((maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex] == x) && (maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex] == y))
						{
							#ifdef OR_DEBUG
							/*
							cout << "placing corner;" << endl;
							cout << "x = " << x << endl;
							cout << "y = " << y << endl;
							*/
							#endif
							setBooleanMapValue(x, y, imageWidth, true, featuresUsingContrastMapComplete);
						}
					}
				}
			}
		}
	}

	//now we have the nearest 3 features to each corner using the outlined (common unit vector) scenarios
}


void generateFeatureListUsingFeatureArrays(int imageWidth, int imageHeight, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList, RTviewInfo* vi)
{
	//First create a list of unique features

	//set currentFeatureInList to last corner in list;
	ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				int x = maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				int y = maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex];

				vec corner;
				corner.x = x;
				corner.y = y;
				corner.z = 0.0;

				if(!checkFeatureListForCommonFeature(&corner, firstFeatureInList, MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD, false))
				{//add corner to list

					#ifdef OR_DEBUG
					/*
					cout << "added corner" << endl;
					cout << "\t crn; x= " << corner.x << endl;
					cout << "\t crn; y= " << corner.y << endl;
					cout << "\t crn; z= " << corner.z << endl;
					*/
					#endif

					//fix;
					bool zerozeroanomolyfound = false;
					if((corner.x == OR_ZERO_ANOMALY_FEATURE_X_POS) && (corner.y == OR_ZERO_ANOMALY_FEATURE_Y_POS))
					{
						zerozeroanomolyfound = true;
					}

					bool backgroundcornerfound = false;
					/*
					if(compareDoubles(corner.z, OR_BACKGROUND_ANOMALY_Z_POS))
					{
						backgroundcornerfound = true;
					}
					*/

					if(!zerozeroanomolyfound && !backgroundcornerfound)
					{
						#ifdef OR_DEBUG
						/*
						cout << "found and adding corner;" << endl;
						cout << "corner x = " << corner.x << endl;
						cout << "corner y = " << corner.y << endl;
						cout << "corner z = " << corner.z << endl;
						*/
						#endif

						currentFeatureInList->xViewport = x;
						currentFeatureInList->yViewport = y;

						copyVectorRT(&(currentFeatureInList->point), &corner);
						ORfeature* newFeature = new ORfeature();
						currentFeatureInList->next = newFeature;
						currentFeatureInList = currentFeatureInList->next;
					}
				}
			}
		}
	}


}


void generateFeatureListUsingFeatureArraysUsingDepthMap(int imageWidth, int imageHeight, double* depthMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList, RTviewInfo* vi)
{
	//First create a list of unique features

	//set currentFeatureInList to last corner in list;
	ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				#ifndef USE_ORIGINAL_WRONG_FEATURE_GEN
				int x = maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				int y = maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				double z = getLumOrContrastOrDepthMapValue(x, y, imageWidth, depthMap);

				double xWorld;
				double yWorld;
				double zWorld;

				vec xyzWorld;
				calculatePointMapValue(x, y, z, &xyzWorld, vi);

				vec corner;
				corner.x = xyzWorld.x;
				corner.y = xyzWorld.y;
				corner.z = xyzWorld.z;
				#else

				corner.x = x;
				corner.y = y;
				corner.z = z;
				#endif

				if(!checkFeatureListForCommonFeature(&corner, firstFeatureInList, MAX_FEATURE_DISTANCE_ERROR_USING_DEPTH_MAP_METHOD, false))
				{//add corner to list

					#ifdef OR_DEBUG
					/*
					cout << "added corner" << endl;
					cout << "\t crn; x= " << corner.x << endl;
					cout << "\t crn; y= " << corner.y << endl;
					cout << "\t crn; z= " << corner.z << endl;
					*/
					#endif

					//fix;
					bool zerozeroanomolyfound = false;
					if((corner.x == OR_ZERO_ANOMALY_FEATURE_X_POS) && (corner.y == OR_ZERO_ANOMALY_FEATURE_Y_POS))
					{
						zerozeroanomolyfound = true;
					}

					bool backgroundcornerfound = false;
					/*
					if(compareDoubles(corner.z, OR_BACKGROUND_ANOMALY_Z_POS))
					{
						backgroundcornerfound = true;
					}
					*/

					if(!zerozeroanomolyfound && !backgroundcornerfound)
					{
						#ifdef OR_DEBUG
						/*
						cout << "found and adding corner;" << endl;
						cout << "corner x = " << corner.x << endl;
						cout << "corner y = " << corner.y << endl;
						cout << "corner z = " << corner.z << endl;
						*/
						#endif

						currentFeatureInList->xViewport = x;
						currentFeatureInList->yViewport = y;

						copyVectorRT(&(currentFeatureInList->point), &corner);
						ORfeature* newFeature = new ORfeature();
						currentFeatureInList->next = newFeature;
						currentFeatureInList = currentFeatureInList->next;
					}
				}
			}
		}
	}


}

void generateFeatureListUsingFeatureArraysUsingPointMap(int imageWidth, int imageHeight, double* pointMap, int maxDotProductResultXposArrayComplete[3][3][3], int maxDotProductResultYposArrayComplete[3][3][3], ORfeature* firstFeatureInList)
{
	//First create a list of unique features

	//set currentFeatureInList to last corner in list;
	ORfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		currentFeatureInList = currentFeatureInList->next;
	}

	for(int uvxIndex = 0; uvxIndex < 3; uvxIndex++)
	{
		for(int uvyIndex = 0; uvyIndex < 3; uvyIndex++)
		{
			for(int uvzIndex = 0; uvzIndex < 3; uvzIndex++)
			{
				int x = maxDotProductResultXposArrayComplete[uvxIndex][uvyIndex][uvzIndex];
				int y = maxDotProductResultYposArrayComplete[uvxIndex][uvyIndex][uvzIndex];

				double xWorld;
				double yWorld;
				double zWorld;
				vec xyzWorld;
				getPointMapValue(x, y, imageWidth, pointMap, &xyzWorld);
				xWorld = xyzWorld.x;
				yWorld = xyzWorld.y;
				zWorld = xyzWorld.z;

				vec corner;
				corner.x = xWorld;
				corner.y = yWorld;
				corner.z = zWorld;

				if(!checkFeatureListForCommonFeature(&corner, firstFeatureInList, MAX_FEATURE_DISTANCE_ERROR_USING_POINT_MAP_METHOD, true))
				{//add corner to list

					#ifdef OR_DEBUG
					/*
					cout << "added corner" << endl;
					cout << "\t crn; x= " << corner.x << endl;
					cout << "\t crn; y= " << corner.y << endl;
					cout << "\t crn; z= " << corner.z << endl;
					*/
					#endif

					//fix;
					bool zerozeroanomolyfound = false;
					if((corner.x == OR_ZERO_ANOMALY_FEATURE_X_POS) && (corner.y == OR_ZERO_ANOMALY_FEATURE_Y_POS))
					{
						zerozeroanomolyfound = true;
					}

					bool backgroundcornerfound = false;
					/*
					if(compareDoubles(corner.z, OR_BACKGROUND_ANOMALY_Z_POS))
					{
						backgroundcornerfound = true;
					}
					*/

					if(!zerozeroanomolyfound && !backgroundcornerfound)
					{
						#ifdef OR_DEBUG
						/*
						cout << "found and adding unique corner;" << endl;
						cout << "corner x = " << corner.x << endl;
						cout << "corner y = " << corner.y << endl;
						cout << "corner z = " << corner.z << endl;
						*/
						#endif

						currentFeatureInList->xViewport = x;
						currentFeatureInList->yViewport = y;

						copyVectorRT(&(currentFeatureInList->point), &corner);
						ORfeature* newFeature = new ORfeature();
						currentFeatureInList->next = newFeature;
						currentFeatureInList = currentFeatureInList->next;
					}
				}
			}
		}
	}


}


#endif
