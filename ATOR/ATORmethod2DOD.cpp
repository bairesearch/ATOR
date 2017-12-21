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
 * File Name: ATORmethod2DOD.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3m6a 10-December-2017
 * NB pointmap is a new addition for test streamlining; NB in test scenarios 2 and 3, there will be no pointmap available; the pointmap will have to be generated after depth map is obtained by using calculatePointUsingTInWorld()
 *******************************************************************************/


#include "ATORmethod2DOD.hpp"
#ifdef USE_OPENGL
#endif

double ORmethod2DODClass::calculateXYorientationOfSide(const ORpolygon* currentPolygonInList, const int side)
{
	double orientationOfSide;

	if(side == 0)
	{
		orientationOfSide = SHAREDvector.calculateAngleBetweenVectors2D(&(currentPolygonInList->point1), &(currentPolygonInList->point2));
	}
	else if(side == 1)
	{
		orientationOfSide = SHAREDvector.calculateAngleBetweenVectors2D(&(currentPolygonInList->point2), &(currentPolygonInList->point3));
	}
	else if(side == 2)
	{
		orientationOfSide = SHAREDvector.calculateAngleBetweenVectors2D(&(currentPolygonInList->point3), &(currentPolygonInList->point1));
	}
	else
	{
		cerr << "error misdefined side" << endl;
		exit(EXIT_ERROR);
	}
	return orientationOfSide;
}

double ORmethod2DODClass::calculateXYlengthOfSide(const ORpolygon* currentPolygonInList, const int side)
{
	double lengthOfSide;

	if(side == 0)
	{
		lengthOfSide = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&(currentPolygonInList->point1), &(currentPolygonInList->point2));
	}
	else if(side == 1)
	{
		lengthOfSide = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&(currentPolygonInList->point2), &(currentPolygonInList->point3));
	}
	else if(side == 2)
	{
		lengthOfSide = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&(currentPolygonInList->point3), &(currentPolygonInList->point1));
	}
	else
	{
		cerr << "error misdefined side" << endl;
		exit(EXIT_ERROR);
	}
	return lengthOfSide;
}

double ORmethod2DODClass::calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(const ORpolygon* transformedObjectTriangle, const int side)
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
		cerr << "error misdefined side" << endl;
		exit(EXIT_ERROR);
	}



	return perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide;
}

double ORmethod2DODClass::calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(const ORpolygon* transformedObjectTriangle, const int side)
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
		cerr << "error misdefined side" << endl;
		exit(EXIT_ERROR);
	}

	return xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex;
}













void ORmethod2DODClass::TEMPprintReferenceListVertexPositions2DOD(const LDreference* firstReferenceInInterpolated2DrgbMap)
{
	const LDreference* currentReference = firstReferenceInInterpolated2DrgbMap;
	while(currentReference->next != NULL)
	{
		cout << "TESTX currentReference->vertex1absolutePosition.x  = " << currentReference->vertex1absolutePosition.x  << endl;
		cout << "TESTX currentReference->vertex1absolutePosition.y  = " << currentReference->vertex1absolutePosition.y  << endl;
		cout << "TESTX currentReference->vertex2absolutePosition.x  = " << currentReference->vertex2absolutePosition.x  << endl;
		cout << "TESTX currentReference->vertex2absolutePosition.y  = " << currentReference->vertex2absolutePosition.y  << endl;

		currentReference = currentReference->next;
	}



}



void ORmethod2DODClass::transformObjectData2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, const int side, const bool first, ORfeature* firstFeatureInList)
{
	long time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart;
	if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS)
	{
		if(OR_PRINT_ALGORITHM_AND_TIME_DETAILS_ALL)
		{
			cout << "\t\t\t\t start: 3ai. 2DOD normalised snapshot generation - transform data wrt polygon - matrix calc" << endl;
		}
		time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart = SHAREDvars.getTimeAsLong();
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


	double orientationOfObjectTriangleSide = this->calculateXYorientationOfSide(currentPolygonInList, side);
	double lengthOfObjectTriangleSide = this->calculateXYlengthOfSide(currentPolygonInList, side);
	double lengthOfPredefinedTriangleSide = SHAREDvector.calculateTheDistanceBetweenTwoPointsXYOnly(&(predefinedTriangle->point1), &(predefinedTriangle->point2));	//=1 for equilateral triangle


	//1a. Scale object data such that the object triangle side is of same length as a predefined side of a predefined triangle
	double scaleFactor1a = lengthOfPredefinedTriangleSide/lengthOfObjectTriangleSide;


	mat scaleMatrix1a;
	SHAREDvector.createScaleMatrix2D(&scaleMatrix1a, scaleFactor1a);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	//openglUseMatrix = true;
	opengl2DmatrixTransformation1ScaleFactor = scaleFactor1a;

	#else
	if(first)
	{
		ORoperations.storeBackupVertexAbsPositionsForAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap);
	}
	else
	{
		ORoperations.restoreBackupVertexAbsPositionsForAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap);
	}
	//ORoperations.applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &scaleMatrix1a);
	#endif


	//1b. tranform object triangle; Scale object triangle such that the object triangle side is of same length as a predefined side of a predefined triangle
	vec vecNew;
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &scaleMatrix1a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &scaleMatrix1a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &scaleMatrix1a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point4), &scaleMatrix1a);
	SHAREDvector.copyVectors(&(point4), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point5), &scaleMatrix1a);
	SHAREDvector.copyVectors(&(point5), &vecNew);
	*/


	//1c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature;	//startup
		currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
			if(SHAREDvector.compareVectors(&(currentFeature->point), &(currentPolygonInList->point1)))
			{
				currentFeature->OTpointIndex = 1;
			}
			else if(SHAREDvector.compareVectors(&(currentFeature->point), &(currentPolygonInList->point2)))
			{
				currentFeature->OTpointIndex = 2;
			}
			else if(SHAREDvector.compareVectors(&(currentFeature->point), &(currentPolygonInList->point3)))
			{
				currentFeature->OTpointIndex = 3;
			}
			else
			{
				currentFeature->OTpointIndex = 0;
			}
			#endif

			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &(currentFeature->point));		//startup
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix1a);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
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
			if(SHAREDvector.compareVectors(&(currentFeature->point), &(currentPolygonInList->point1)))
			{
				currentFeature->OTpointIndex = 1;
			}
			else if(SHAREDvector.compareVectors(&(currentFeature->point), &(currentPolygonInList->point2)))
			{
				currentFeature->OTpointIndex = 2;
			}
			else if(SHAREDvector.compareVectors(&(currentFeature->point), &(currentPolygonInList->point3)))
			{
				currentFeature->OTpointIndex = 3;
			}
			else
			{
				currentFeature->OTpointIndex = 0;
			}
			#endif

			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &(currentFeature->point));		//startup
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix1a);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}


	//2ia. rotate object data such that the object triangle side is parallel with X axis [and 2ii. third apex is above the lowest 2 apexes]
	double rotationFactor2i = -orientationOfObjectTriangleSide;

	mat rotateMatrix2i;
	SHAREDvector.createRotationMatrix2D(&rotateMatrix2i, rotationFactor2i);
	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation2iRotationFactor = rotationFactor2i;
	#else
	//ORoperations.applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &rotateMatrix2i);
	#endif

	//2ib. tranform object triangle;  rotate object triangle such that the object triangle side is parallel with X axis
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &rotateMatrix2i);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &rotateMatrix2i);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &rotateMatrix2i);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point4), &rotateMatrix2i);
	SHAREDvector.copyVectors(&(point4), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point5), &rotateMatrix2i);
	SHAREDvector.copyVectors(&(point5), &vecNew);
	*/

	//2ic. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2i);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2i);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
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
		cerr << "error: illegal side" << endl;
		exit(EXIT_ERROR);
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
	SHAREDvector.createRotationMatrix2D(&rotateMatrix2ii, rotationFactor2ii);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation2iiRotationFactor = rotationFactor2ii;
	#else
	//ORoperations.applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &rotateMatrix2ii);
	#endif

	//2iib. tranform object triangle; rotate object triangle by 180 degrees if necessary
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &rotateMatrix2ii);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &rotateMatrix2ii);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &rotateMatrix2ii);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point4), &rotateMatrix2ii);
	SHAREDvector.copyVectors(&(point4), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point5), &rotateMatrix2ii);
	SHAREDvector.copyVectors(&(point5), &vecNew);
	*/

	//2iic. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2ii);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &rotateMatrix2ii);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}






	double perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide = predefinedTriangle->point3.y;
#ifndef OR_METHOD_2DOD_ASSUME_NO_3D_ROTATION

	//3a. Scale object data on Y axis such that the third apex is the same perpendicular distance away from the side as is the case for the predefined triangle.
	double perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide = this->calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(transformedObjectTriangle, side);

	mat scaleMatrix3a;
	double scaleFactor3a = perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide/perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide;

	//OLD: SHAREDvector.createYAxisScaleMatrix2D(&scaleMatrix3a, lengthOfPredefinedTriangleSide/perpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide);
	SHAREDvector.createYAxisScaleMatrix2D(&scaleMatrix3a, scaleFactor3a);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation3aScaleFactor = scaleFactor3a;
	#else
	//ORoperations.applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &scaleMatrix3a);
	#endif


	//3b. tranform object triangle; Scale object triangle on Y axis such that the third apex is the same perpendicular distance away from the side as is the case for the predefined triangle.
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &scaleMatrix3a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &scaleMatrix3a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &scaleMatrix3a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point4), &scaleMatrix3a);
	SHAREDvector.copyVectors(&(point4), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point5), &scaleMatrix3a);
	SHAREDvector.copyVectors(&(point5), &vecNew);
	*/


	//3c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix3a);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &scaleMatrix3a);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}


	//4a. shear object data along X axis such that object triangle apexes are coincident with predefined triangle apexes
	double xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex = this->calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(transformedObjectTriangle, side);
		//double shearRequired = (xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex - (lengthOfPredefinedTriangleSide/2))/lengthOfPredefinedTriangleSide;	OLD
	double shearRequired4a = (xAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex - (lengthOfPredefinedTriangleSide/2))/perpendicularDistanceBetweenThirdApexOfPredefinedTriangleAndSide;


	mat shearMatrix4a;
	SHAREDvector.createXAxisShearMatrix2D(&shearMatrix4a, shearRequired4a);

	#ifdef USE_OPENGL_PREDEFINED_OD_MATRIX_OPERATIONS
	opengl2DmatrixTransformation4aShearFactor = shearRequired4a;
	#else
	//ORoperations.applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &shearMatrix4a);
	#endif


	//4b. tranform object triangle; shear object trianlge along X axis such that object triangle apexes are coincident with predefined triangle apexes
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point1), &shearMatrix4a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point1), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point2), &shearMatrix4a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point2), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(transformedObjectTriangle->point3), &shearMatrix4a);
	SHAREDvector.copyVectors(&(transformedObjectTriangle->point3), &vecNew);

	/*
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point4), &shearMatrix4a);
	SHAREDvector.copyVectors(&(point4), &vecNew);
	SHAREDvector.multiplyVectorByMatrix(&vecNew, &(point5), &shearMatrix4a);
	SHAREDvector.copyVectors(&(point5), &vecNew);
	*/


	//4c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &shearMatrix4a);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
			currentFeature = currentFeature->next;
		}
	}
	if(OR_METHOD_TRANSFORM_ALL_FEATURES)
	{
		ORfeature* currentFeature = firstFeatureInList;
		while(currentFeature->next != NULL)
		{
			SHAREDvector.multiplyVectorByMatrix(&vecNew, &(currentFeature->pointTransformed), &shearMatrix4a);
			SHAREDvector.copyVectors(&(currentFeature->pointTransformed), &vecNew);
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
	SHAREDvector.createIdentityMatrixRT(&multipliedMatrix);

	#ifndef OR_METHOD_2DOD_ASSUME_NO_3D_ROTATION
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &shearMatrix4a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix3a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	#endif
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2ii);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2i);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix1a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);

	SHAREDvector.copyMatrixTwoIntoMatrixOne(&opengl2DmultiplicationMatrix, &multipliedMatrix);

	/*
	//now create inverted matrix;
	mat multipliedMatrixInverted;
	SHAREDvector.createIdentityMatrixRT(&multipliedMatrixInverted);

	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrixInverted, &scaleMatrix1a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrixInverted, &rotateMatrix2i);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrixInverted, &rotateMatrix2ii);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrixInverted, &scaleMatrix3a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrixInverted, &shearMatrix4a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrixInverted, &matTemp);

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
	SHAREDvector.createIdentityMatrixRT(&multipliedMatrix);

	#ifndef OR_METHOD_2DOD_ASSUME_NO_3D_ROTATION
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &shearMatrix4a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix3a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	#endif
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2ii);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &rotateMatrix2i);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);
	SHAREDvector.multiplyMatricies(&matTemp, &multipliedMatrix, &scaleMatrix1a);
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&multipliedMatrix, &matTemp);

	ORoperations.applyTransformationMatrixToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &multipliedMatrix);

	ORoperations.applyTranslationToAllReferencesIn2Dlist(firstReferenceInInterpolated2DrgbMap, &translationVector);
#endif


	//5b. tranform object triangle; translate the object triangle on X and Y axis such that the object triangle is centred at 0,0
	transformedObjectTriangle->point1.x = transformedObjectTriangle->point1.x + translationVector.x;
	transformedObjectTriangle->point1.y = transformedObjectTriangle->point1.y + translationVector.y;
	transformedObjectTriangle->point2.x = transformedObjectTriangle->point2.x + translationVector.x;
	transformedObjectTriangle->point2.y = transformedObjectTriangle->point2.y + translationVector.y;
	transformedObjectTriangle->point3.x = transformedObjectTriangle->point3.x + translationVector.x;
	transformedObjectTriangle->point3.y = transformedObjectTriangle->point3.y + translationVector.y;



	//5c. tranform nearest features
	if(OR_METHOD_TRANSFORM_NEARBY_FEATURES)
	{
		ORfeature* currentFeature = currentPolygonInList->firstFeatureInNearestFeatureList;
		while(currentFeature->next != NULL)
		{
			currentFeature->pointTransformed.x = currentFeature->pointTransformed.x + translationVector.x;
			currentFeature->pointTransformed.y = currentFeature->pointTransformed.y + translationVector.y;
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
		time3aiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd = SHAREDvars.getTimeAsLong();
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

		time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonStart = SHAREDvars.getTimeAsLong();
	}

#ifdef USE_OPENGL_OPTIMISED_TRIANGLE_RENDERING
	//setViewPort(100, 100, transformedObjectTriangle->point1.x, transformedObjectTriangle->point1.y);
	LDopengl.setViewPort2Dortho(-1.0, 1.0, -1.0, 1.0);
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
				this->disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(firstReferenceInInterpolated2DrgbMap, currentPolygonInList, padBoundary, side, shearRequired4a);
				//disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced2(firstReferenceInInterpolated2DrgbMap, currentPolygonInList, &point4, &point5, padBoundary);

			}
			else
			{
				this->disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(firstReferenceInInterpolated2DrgbMap, currentPolygonInList);
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

	double minXOfTriangle = SHAREDvars.minDouble(SHAREDvars.minDouble(currentPolygonInList->point1.x, currentPolygonInList->point2.x), currentPolygonInList->point3.x);
	double minYOfTriangle = SHAREDvars.minDouble(SHAREDvars.minDouble(currentPolygonInList->point1.y, currentPolygonInList->point2.y), currentPolygonInList->point3.y);
	double maxXOfTriangle = SHAREDvars.maxDouble(SHAREDvars.maxDouble(currentPolygonInList->point1.x, currentPolygonInList->point2.x), currentPolygonInList->point3.x);
	double maxYOfTriangle =  SHAREDvars.maxDouble(SHAREDvars.maxDouble(currentPolygonInList->point1.y, currentPolygonInList->point2.y), currentPolygonInList->point3.y);
	double widthOfOriginalTriangle = SHAREDvars.absDouble(minXOfTriangle - maxXOfTriangle);
	double heightOfOriginalTriangle = SHAREDvars.absDouble(minYOfTriangle - maxYOfTriangle);
	double minwidthheightOfOriginalTriangle = SHAREDvars.minDouble(widthOfOriginalTriangle, heightOfOriginalTriangle);


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
		time3aiiNormalisedSnapshotGeneration2DODTransformDataWRTPolygonEnd = SHAREDvars.getTimeAsLong();
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




void ORmethod2DODClass::disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList, const bool padBoundary, const int side, const double shearFactor)
{

	vec A;
	vec B;
	vec C;
	vec D;

	vec pointApexOfTri;
	SHAREDvector.initialiseVector(&pointApexOfTri);

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
		cerr << "error misdefined side" << endl;
		exit(EXIT_ERROR);
	}

	SHAREDvector.initialiseVector(&C);
	SHAREDvector.initialiseVector(&D);

	vec parallelVectorToTriangleBase;
	SHAREDvector.initialiseVector(&parallelVectorToTriangleBase);
	SHAREDvector.subtractVectorsRT(&(B), &(A), &parallelVectorToTriangleBase);		//vect = vect1 - vect2

	SHAREDvector.multiplyVectorByScalar(&parallelVectorToTriangleBase, (1.0/shearFactor));		//NB 1.2 is to compensate for aliasing errors

	vec p3plusparallelVectorToTriangleBase;
	vec p3minusparallelVectorToTriangleBase;

	SHAREDvector.initialiseVector(&p3plusparallelVectorToTriangleBase);
	SHAREDvector.initialiseVector(&p3minusparallelVectorToTriangleBase);

	SHAREDvector.addVectorsRT(&(pointApexOfTri), &parallelVectorToTriangleBase, &p3plusparallelVectorToTriangleBase);
	SHAREDvector.subtractVectorsRT(&(pointApexOfTri), &parallelVectorToTriangleBase, &p3minusparallelVectorToTriangleBase);

	C.x = p3plusparallelVectorToTriangleBase.x;
	C.y = p3plusparallelVectorToTriangleBase.y;
	C.z = 0.0;
	D.x = p3minusparallelVectorToTriangleBase.x;
	D.y = p3minusparallelVectorToTriangleBase.y;
	D.z = 0.0;


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
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}
*/


void ORmethod2DODClass::disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList)
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
			SHAREDvector.subtractVectorsRT(&C, &A, &v0);	//vect = vect1 - vect2
			//v1 = B - A
			SHAREDvector.subtractVectorsRT(&B, &A, &v1);	//vect = vect1 - vect2
			//v2 = P - A
			SHAREDvector.subtractVectorsRT(&P, &A, &v2);	//vect = vect1 - vect2

			// Compute dot products
			double dot00 = SHAREDvector.dotProduct(&v0, &v0);
			double dot01 = SHAREDvector.dotProduct(&v0, &v1);
			double dot02 = SHAREDvector.dotProduct(&v0, &v2);
			double dot11 = SHAREDvector.dotProduct(&v1, &v1);
			double dot12 = SHAREDvector.dotProduct(&v1, &v2);

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
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}
}



/*NEWER, Barycentric Technique - descriptive but less efficient;

void ORmethod2DODClass::disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList)
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
void ORmethod2DODClass::disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList)
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

		double AreaPP1P2 = calculateAreaOfTriangle(&P, &A, &B);
		double AreaPP2P3 = calculateAreaOfTriangle(&P, &B, &C);
		double AreaPP3P1 = calculateAreaOfTriangle(&P, &C, &A);
		double AreaP1P2P3 = calculateAreaOfTriangle(&A, &B, &C);


		if(compareDoubles((AreaPP1P2 + AreaPP2P3 + AreaPP3P1),  AreaP1P2P3))
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

void ORmethod2DODClass::disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList)
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


			double AreaPP1P2 = calculateAreaOfTriangle(&P, &A, &B);
			double AreaPP2P3 = calculateAreaOfTriangle(&P, &B, &C);
			double AreaPP3P1 = calculateAreaOfTriangle(&P, &C, &A);
			double AreaP1P2P3 = calculateAreaOfTriangle(&A, &B, &C);


			if(compareDoubles((AreaPP1P2 + AreaPP2P3 + AreaPP3P1),  AreaP1P2P3))
			{
				numberOfApexesOfReferenceQuadThatLieInsideTriangle++;
			}
			else
			{
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
		}
		else
		{
			currentReference->referenceEnabledMethod2DOD = false;
		}

		currentReference = currentReference->next;
	}



}
*/



void ORmethod2DODClass::createInterpolated2DmeshReferenceListUsingRGBmap2DOD(int imageWidth, const int imageHeight, unsigned char* rgbMap, LDreference* firstReferenceInInterpolatedMap)
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
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex1relativePosition), &v1);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex2relativePosition), &v2);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex3relativePosition), &v3);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex4relativePosition), &v4);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex1absolutePosition), &v1);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex2absolutePosition), &v2);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex3absolutePosition), &v3);
			SHAREDvector.copyVectors(&(currentReferenceInInterpolated2DMap->vertex4absolutePosition), &v4);

			colour col;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &col);

			unsigned int colByte1 = (unsigned int)DAT_FILE_FIRST_RGB_COLOUR << (unsigned int)24;
			unsigned int colByte2 = (unsigned int)col.r << (unsigned int)16;
			unsigned int colByte3 = (unsigned int)col.g << (unsigned int)8;
			unsigned int colByte4 = (unsigned int)col.b;


			currentReferenceInInterpolated2DMap->colour = colByte1 | colByte2 | colByte3 | colByte4;
			currentReferenceInInterpolated2DMap->absoluteColour = colByte1 | colByte2 | colByte3 | colByte4;


			LDreference* newReference = new LDreference();
			currentReferenceInInterpolated2DMap->next = newReference;
			currentReferenceInInterpolated2DMap = currentReferenceInInterpolated2DMap->next;
		}
	}
}


void ORmethod2DODClass::create2DmeshUsingRGBmap2DOD(int imageWidth, const int imageHeight, double imageXOffset, double imageYOffset, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], const bool useEdgeZeroCrossingMap)
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
			ORfeatureGeneration.generateEdgeListFromRGBmapWithQuadraticFit(rgbMap, edgeBoolMap, edgeZeroCrossingMap, imageWidth, imageHeight, EDGE_LUMINOSITY_CONTRAST_THRESHOLD, OR_METHOD2DOD_DIMENSIONS, NULL, NULL, 1.0, NULL, interpixelContrastMapType);
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
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMap, &col);

			currentMeshPointInMesh->col.r = col.r;
			currentMeshPointInMesh->col.g = col.g;
			currentMeshPointInMesh->col.b = col.b;
			currentMeshPointInMesh->point.x = x+imageXOffset;
			currentMeshPointInMesh->point.y = y+imageYOffset;
			currentMeshPointInMesh->point.z = 0.0;
			currentMeshPointInMesh->xInt = x;
			currentMeshPointInMesh->yInt = y;
			currentMeshPointInMesh->luminosity = RTpixelMaps.calculateLuminosityLevelFromRGBVal(&col);

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
				ORpolygonList.calculateMeshPointInterpixelLuminosityContrast(meshPointArray[y*imageWidth + x]);
			}
		}
	}
	else
	{
		for(int y = 1; y < (imageHeight-1); y++)
		{
			for(int x = 1; x < (imageWidth-1); x++)
			{
				ORpolygonList.calculateMeshPointLuminosityContrast(meshPointArray[y*imageWidth + x]);
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
						nearestMeshpointInExistingMesh = ORpolygonList.findMeshPointIntInMesh(firstMeshPointInMeshList, &borderPoint, &hasFoundMeshPoint, numMeshPointsInExistingMesh);
						if(hasFoundMeshPoint == false)
						{
							cerr << "error: cannot find adjacent meshpoint" << endl;
							exit(EXIT_ERROR);
						}

						ORmeshPoint* meshpointInAdditionalMesh;
						meshpointInAdditionalMesh = meshPointArray[y*imageWidth + x];

						/*old inefficient;
						hasFoundMeshPoint = false;
						meshpointInAdditionalMesh = ORpolygonList.findMeshPointIntInMesh(firstNewMeshPointInMesh, x, y, &hasFoundMeshPoint);
						if(hasFoundMeshPoint == false)
						{
							cerr << "error: cannot find adjacent meshpoint" << endl;
							exit(EXIT_ERROR);
						}
						*/

						if(SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(nearestMeshpointInExistingMesh->point), &(meshpointInAdditionalMesh->point)) < OR_METHOD_2DOD_USE_ADVANCED_INTERP_MESH_JOINING_MAXIMUM_RECONCILIATION_DISTANCE)
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




