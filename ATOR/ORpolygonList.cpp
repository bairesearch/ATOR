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
 * File Name: ORpolygonList.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3c6c 22-July-2013
 *
 *******************************************************************************/

#include "ORpolygonList.h"
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


ObjectReferenceList::ObjectReferenceList(void)
{
	objectNumber = 0;
	objectName = "";
	numMatchingSnapshots = 0;

	next = NULL;
}

ObjectReferenceList::~ObjectReferenceList()
{
	if(next != NULL)
	{
		delete next;
	}
}


SnapshotIDreferenceList::SnapshotIDreferenceList(void)
{
	referenceID = 0;

	next = NULL;
	previous = NULL;
}

SnapshotIDreferenceList::~SnapshotIDreferenceList()
{
	if(next != NULL)
	{
		delete next;
	}
}


PolygonBAI::PolygonBAI(void)
{
	point1.x = 0.0;
	point1.y = 0.0;
	point1.z = 0.0;
	point2.x = 0.0;
	point2.y = 0.0;
	point2.z = 0.0;
	point3.x = 0.0;
	point3.y = 0.0;
	point3.z = 0.0;

	next = NULL;

#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	firstFeatureInNearestFeatureList = NULL;
#endif

	if(OR_METHOD_TRANSFORM_KEY_OT_FEATURES)
	{
		point1Transformed.x = 0.0;
		point1Transformed.y = 0.0;
		point1Transformed.z = 0.0;
		point2Transformed.x = 0.0;
		point2Transformed.y = 0.0;
		point2Transformed.z = 0.0;
		point3Transformed.x = 0.0;
		point3Transformed.y = 0.0;
		point3Transformed.z = 0.0;
	}

}

PolygonBAI::~PolygonBAI()
{
	//there will be a slow minor memory leak until the below code is sorted
	/*
#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES
	if(firstFeatureInNearestFeatureList != NULL)
	{
		delete firstFeatureInNearestFeatureList;
	}
#endif
	*/

	if(next != NULL)
	{
		delete next;
	}
}

Feature::Feature(void)
{
	point.x = 0.0;			//2DOD x pos relative to current snapshot / 3DOD world coord
	point.y = 0.0;			//2DOD y pos relative to current snapshot / 3DOD world coord
	point.z = 0.0;			//2DOD z pos relative to current snapshot / 3DOD world coord
	pointNonWorldCoord.x = 0.0;	//3DOD only - x pos relative to current snapshot
	pointNonWorldCoord.y = 0.0;	//3DOD only - y pos relative to current snapshot
	pointNonWorldCoord.z = 0.0;	//3DOD only - z pos relative to current snapshot (depth)
#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	objectName = "";
	trainOrTest = false;
	viewIndex = 0;
	zoomIndex = 0;
	polyIndex = 0;
	sideIndex = 0;
	#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	OTpointIndex = 0;
	#endif
	#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
	minWidthAndHeightOfOrigOT = 0.0;
	#endif

	pointTransformed.x = 0.0;
	pointTransformed.y = 0.0;
	pointTransformed.z = 0.0;

	#ifdef DEBUG_OR_OUTPUT_GEO_COORDINATES
	matchFound = false;
	#endif

#endif

//#ifdef OR_IMAGE_COMPARISON_SQL_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX; i++)
	{
		dctCoeff[i] = 0;
	}
	dctCoeffArrayBinned = 0;

//#endif

//#ifdef OR_IMAGE_COMPARISON_SQL_AVERAGE_RGB_DEV_BINNING
	avgCol.r = 0;
	avgCol.g = 0;
	avgCol.b = 0;
//#endif

	xViewport = 0;
	yViewport = 0;
	magnitude = 0.0;
	numberOfFeaturePixelsUsedToGenerateFeature = 0;

	lastFilledFeatureInList = true;
	next = NULL;

	//nearestFeatureActive = false;


}

#ifdef OR_METHOD_GEOMETRIC_COMPARISON

FeatureContainer::FeatureContainer(void)
{
	numVotes = 0;
	error=98765321;

	/*
	#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_BASIC_NO_SQL
	averageColour.r = 0;
	averageColour.g = 0;
	averageColour.b = 0;
	#endif
	*/

	firstFeatureInFeatureList = NULL;
	next = NULL;
}

FeatureContainer::~FeatureContainer()
{
	if(firstFeatureInFeatureList != NULL)
	{
		delete firstFeatureInFeatureList;
	}

	if(next != NULL)
	{
		delete next;
	}
}

#endif

Feature::~Feature()
{
	if(next != NULL)
	{
		delete next;
	}
}


MeshPoint::MeshPoint(void)
{
	xInt = 0;
	yInt = 0;
	depth = 0.0;

	col.r = 0;
	col.g = 0;
	col.b = 0;

	point.x = 0;
	point.y = 0;
	point.z = 0;

	luminosity = 0;
	luminosityContrast = 0;

	//#ifdef OR_USE_CONTRAST_CALC_METHOD_C
	interpixelMeshPoint = NULL;
	interpixelMeshPointFilled = false;
	//#endif

	adjacentMeshPoint[0] = NULL;
	adjacentMeshPoint[1] = NULL;
	adjacentMeshPoint[2] = NULL;
	adjacentMeshPoint[3] = NULL;
	adjacentMeshPoint[4] = NULL;
	adjacentMeshPoint[5] = NULL;
	adjacentMeshPoint[6] = NULL;
	adjacentMeshPoint[7] = NULL;
	adjacentMeshPoint[8] = NULL;

	adjacentMeshPointFilled[0] = false;
	adjacentMeshPointFilled[1] = false;
	adjacentMeshPointFilled[2] = false;
	adjacentMeshPointFilled[3] = false;

	//#ifdef OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST
	meshPointNormalFilled = false;
		vec temp;
		temp.x = 0.0;
		temp.y = 0.0;
		temp.z = 0.0;
	copyVectors(&(meshPointNormals[0]), &temp);
	copyVectors(&(meshPointNormals[1]), &temp);
	copyVectors(&(meshPointNormals[2]), &temp);
	copyVectors(&(meshPointNormals[3]), &temp);
	meshPointNormal.x = 0;
	meshPointNormal.y = 0;
	meshPointNormal.z = 0;
	meshPointNormalContrast = 0.0;
	//#endif

	alreadyProcessed = NOT_PROCESSED;
	//hasBeenProcessedInSearchForTheOuterBoundary = false;
	hasBeenProcessedInGenerationOfOuterBoundary = false;


	edge = false;	//currently only used if OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED

	//#ifdef OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED
	quadraticFitDepth = 0.0;
	zeroCrossingValueX = 0.0;
	zeroCrossingValueY = 0.0;
	alpha = 0;
	quadraticFitPoint.x = 0;
	quadraticFitPoint.y = 0;
	quadraticFitPoint.z = 0;
	//#endif

	next = NULL;
}

MeshPoint::~MeshPoint()
{
	if(next != NULL)
	{
		delete next;
	}
}


PixelContiguous::PixelContiguous(void)
{
	xInt = 0;
	yInt = 0;
	point.x = 0.0;
	point.y = 0.0;
	point.z = 0.0;
	pointNonWorldCoord.x = 0.0;
	pointNonWorldCoord.y = 0.0;
	pointNonWorldCoord.z = 0.0;
	pathAlreadyCrawled = false;
	finalPathAlreadyCrawled = false;
	meshPoint = NULL;
	next[0] = NULL;
	next[1] = NULL;
	next[2] = NULL;
	next[3] = NULL;
	next[4] = NULL;
	next[5] = NULL;
	next[6] = NULL;
	next[7] = NULL;
	next[8] = NULL;
	previous = NULL;

	readyToDelete = true;
}

PixelContiguous::PixelContiguous(int newX, int newY, PixelContiguous * newPrevious)
{
	xInt = newX;
	yInt = newY;
	point.x = 0.0;
	point.y = 0.0;
	point.z = 0.0;
	pointNonWorldCoord.x = 0.0;
	pointNonWorldCoord.y = 0.0;
	pointNonWorldCoord.z = 0.0;
	pathAlreadyCrawled = false;
	finalPathAlreadyCrawled = false;
	meshPoint = NULL;
	next[0] = NULL;
	next[1] = NULL;
	next[2] = NULL;
	next[3] = NULL;
	next[4] = NULL;
	next[5] = NULL;
	next[6] = NULL;
	next[7] = NULL;
	next[8] = NULL;
	previous = newPrevious;

	readyToDelete = true;
	previous->readyToDelete = false;
}

PixelContiguous::PixelContiguous(MeshPoint * newMeshPoint, PixelContiguous * newPrevious)
{
	xInt = 0;
	yInt = 0;
	point.x = 0.0;
	point.y = 0.0;
	point.z = 0.0;
	pointNonWorldCoord.x = 0.0;
	pointNonWorldCoord.y = 0.0;
	pointNonWorldCoord.z = 0.0;
	pathAlreadyCrawled = false;
	finalPathAlreadyCrawled = false;
	meshPoint = newMeshPoint;
	next[0] = NULL;
	next[1] = NULL;
	next[2] = NULL;
	next[3] = NULL;
	next[4] = NULL;
	next[5] = NULL;
	next[6] = NULL;
	next[7] = NULL;
	next[8] = NULL;
	previous = newPrevious;

	readyToDelete = true;
	previous->readyToDelete = false;
}


PixelContiguous::~PixelContiguous()
{
	/*Cannot use recursion here;
	cout << "deleting contig" <<  endl;
	if(next[0] != NULL)
	{
		delete next[0];
	}
	if(next[1] != NULL)
	{
		delete next[1];
	}
	if(next[2] != NULL)
	{
		delete next[2];
	}
	if(next[3] != NULL)
	{
		delete next[3];
	}
	if(next[4] != NULL)
	{
		delete next[4];
	}
	if(next[5] != NULL)
	{
		delete next[5];
	}
	if(next[6] != NULL)
	{
		delete next[6];
	}
	if(next[7] != NULL)
	{
		delete next[7];
	}
	cout << "finished del" << endl;
	*/
	PixelContiguous * currentPixelContiguous = this;
	bool stillMoreToDelete = true;
	while(stillMoreToDelete)
	{
		bool foundAPixelContiguousToParseInto = false;
		PixelContiguous * pixelContiguousToParseInto;
		for(int q=0;q<9;q++)
		{
			if(q != 4)
			{
				if(currentPixelContiguous->next[q] != NULL)
				{
					if(currentPixelContiguous->next[q]->readyToDelete)
					{
						free(currentPixelContiguous->next[q]);
						currentPixelContiguous->next[q] = NULL;
					}
					else
					{
						if(!foundAPixelContiguousToParseInto)
						{
							foundAPixelContiguousToParseInto = true;
							pixelContiguousToParseInto = currentPixelContiguous->next[q];

						}
					}
				}
			}
		}
		if(foundAPixelContiguousToParseInto)
		{
			currentPixelContiguous = pixelContiguousToParseInto;
		}
		else
		{
			if(currentPixelContiguous->previous != NULL)
			{
				currentPixelContiguous->readyToDelete = true;
				currentPixelContiguous = currentPixelContiguous->previous;
			}
			else
			{
				stillMoreToDelete = false;
			}
		}
	}

}


MeshPoint * findMeshPointIntInMesh(MeshPoint * firstMeshPointInMeshList, int x, int y, bool * hasFoundMeshPoint)
{
	MeshPoint * foundMeshPoint = NULL;
	*hasFoundMeshPoint = false;

	MeshPoint * currentMeshPointInMesh = firstMeshPointInMeshList;
	while(currentMeshPointInMesh->next != NULL)
	{
		if((currentMeshPointInMesh->xInt == x) && (currentMeshPointInMesh->yInt == y))
		{
			foundMeshPoint = currentMeshPointInMesh;
			*hasFoundMeshPoint = true;
		}
		currentMeshPointInMesh = currentMeshPointInMesh->next;
	}

	return foundMeshPoint;
}

MeshPoint * findMeshPointIntInMesh(MeshPoint * firstMeshPointInMeshList, vec * point, bool * hasFoundMeshPoint, int meshZoneLimit)
{
	MeshPoint * foundMeshPoint = NULL;
	*hasFoundMeshPoint = false;
	int numMeshPointsCounted = 0;

	MeshPoint * currentMeshPointInMesh = firstMeshPointInMeshList;
	while((currentMeshPointInMesh->next != NULL) && (numMeshPointsCounted < meshZoneLimit))
	{
		if(compareVectors(&(currentMeshPointInMesh->point), point))
		{
			foundMeshPoint = currentMeshPointInMesh;
			*hasFoundMeshPoint = true;
		}
		currentMeshPointInMesh = currentMeshPointInMesh->next;
		numMeshPointsCounted++;
	}

	return foundMeshPoint;
}

MeshPoint * findMeshPointIntInMesh(MeshPoint * firstMeshPointInMeshList, int position)
{
	MeshPoint * currentMeshPointInMesh = firstMeshPointInMeshList;
	for(int i=0; i < position; i++)
	{
		currentMeshPointInMesh = currentMeshPointInMesh->next;
	}

	return currentMeshPointInMesh;
}

//#ifdef OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST

//#ifdef OR_USE_CONTRAST_CALC_METHOD_C
//see calculateMeshPointNormalsUsingPointMap
//requires;
//vec meshPointNormals[4];	//from 4 tris creating using mesh point and two out of four surrounding meshpoints, x-, x+, y-, y+
//vec meshPointNormal; 		//average of four calculated meshpoint normals
//no boundary checks - these can easily be implemented using adjacentMeshPointFilled
void calculateMeshPointInterpixelNormal(MeshPoint * meshPoint)
{
	/*
	2 4
	1 3
	*/

	/*
	q;
	0.1.2    0 1
	3.4.5 => 3 4
	6 7 8
	*/

	vec averageNormalVector;
	averageNormalVector.x = 0.0;
	averageNormalVector.y = 0.0;
	averageNormalVector.z = 0.0;

	vec * point1;
	vec * point2;
	vec * point3;
	vec * point4;

	point1 = &(meshPoint->adjacentMeshPoint[3]->point);
	point2 = &(meshPoint->adjacentMeshPoint[0]->point);
	point3 = &(meshPoint->adjacentMeshPoint[4]->point);
	point4 = &(meshPoint->adjacentMeshPoint[1]->point);

	vec currentNormalVec;
	calculateNormalOfTri(point1, point2, point3, &currentNormalVec);

	averageNormalVector.x = averageNormalVector.x + currentNormalVec.x;
	averageNormalVector.y = averageNormalVector.y + currentNormalVec.y;
	averageNormalVector.z = averageNormalVector.z + currentNormalVec.z;

	#ifndef DO_NOT_NORMALISE_POINT_NORMAL_MAP
	normaliseVector(&averageNormalVector);
	#endif

	calculateNormalOfTri(point3, point2, point4, &currentNormalVec);

	averageNormalVector.x = averageNormalVector.x + currentNormalVec.x;
	averageNormalVector.y = averageNormalVector.y + currentNormalVec.y;
	averageNormalVector.z = averageNormalVector.z + currentNormalVec.z;


	#ifndef DO_NOT_NORMALISE_POINT_NORMAL_MAP
	normaliseVector(&averageNormalVector);
	#endif

	copyVectors(&(meshPoint->interpixelMeshPoint->meshPointNormal),  &averageNormalVector);
}

//see calculateMeshPointNormalsUsingPointMap
//no boundary checks - these can easily be implemented using adjacentMeshPointFilled
void calculateMeshPointInterpixelNormalContrast(MeshPoint * meshPoint)
{
	/*
	q;
	0.1.2    0 1
	3.4.5 => 3 4
	6 7 8
	*/

	double contrastLevel = 0.0;


	int qMapping[2][2];
	qMapping[0][0] = 0;
	qMapping[0][1] = 1;
	qMapping[1][0] = 3;
	qMapping[1][1] = 4;

	for(int y=0; y<=1; y++)
	{
		//cout << "y = " << y << endl;
		int q;
		q = qMapping[0][y];
		vec * interpixelNormalValue = &(meshPoint->adjacentMeshPoint[q]->interpixelMeshPoint->point);
		q = qMapping[1][y];
		vec * adjacentInterpixelNormalValue = &(meshPoint->adjacentMeshPoint[q]->interpixelMeshPoint->point);

		double currentContrastLevelX = absDouble(interpixelNormalValue->x - adjacentInterpixelNormalValue->x);
		double currentContrastLevelY = absDouble(interpixelNormalValue->y - adjacentInterpixelNormalValue->y);
		double currentContrastLevelZ = absDouble(interpixelNormalValue->z - adjacentInterpixelNormalValue->z);

		contrastLevel = contrastLevel + currentContrastLevelX+currentContrastLevelY+currentContrastLevelZ;
	}

	for(int x=0; x<=1; x++)
	{
		//cout << "x = " << x << endl;
		int q;
		q = qMapping[x][0];
		vec * interpixelNormalValue = &(meshPoint->adjacentMeshPoint[q]->interpixelMeshPoint->point);
		q = qMapping[x][1];
		vec * adjacentInterpixelNormalValue = &(meshPoint->adjacentMeshPoint[q]->interpixelMeshPoint->point);

		double currentContrastLevelX = absDouble(interpixelNormalValue->x - adjacentInterpixelNormalValue->x);
		double currentContrastLevelY = absDouble(interpixelNormalValue->y - adjacentInterpixelNormalValue->y);
		double currentContrastLevelZ = absDouble(interpixelNormalValue->z - adjacentInterpixelNormalValue->z);

		contrastLevel = contrastLevel + currentContrastLevelX+currentContrastLevelY+currentContrastLevelZ;
	}

	meshPoint->meshPointNormalContrast = contrastLevel;
}

void calculateMeshPointInterpixelLuminosityContrast(MeshPoint * meshPoint)
{
	/*
	q;
				OLD; 	0 1 2
					3 4 5 =>  4 5
					6 7 8     7 8
	0.1.2    0 1
	3.4.5 => 3 4
	6 7 8
	*/

	double contrastLevel = 0.0;

	vec * point1;
	vec * point2;
	vec * point3;
	vec * point4;

	int qMapping[2][2];
	qMapping[0][0] = 0;
	qMapping[0][1] = 1;
	qMapping[1][0] = 3;
	qMapping[1][1] = 4;

	for(int y=0; y<=1; y++)
	{
		//cout << "y = " << y << endl;
		int q;
		q = qMapping[0][y];
		double pixelLuminosityMeasurement = meshPoint->adjacentMeshPoint[q]->luminosity;
		q = qMapping[1][y];
		double adjacentPixelLuminosityMeasurement = meshPoint->adjacentMeshPoint[q]->luminosity;

		double currentContrastLevel = absDouble(pixelLuminosityMeasurement - adjacentPixelLuminosityMeasurement);

		contrastLevel = contrastLevel + currentContrastLevel;
	}

	for(int x=0; x<=1; x++)
	{
		//cout << "x = " << x << endl;

		int q;
		q = qMapping[x][0];
		double pixelLuminosityMeasurement = meshPoint->adjacentMeshPoint[q]->luminosity;
		q = qMapping[x][1];
		double adjacentPixelLuminosityMeasurement = meshPoint->adjacentMeshPoint[q]->luminosity;

		double currentContrastLevel = absDouble(pixelLuminosityMeasurement - adjacentPixelLuminosityMeasurement);

		contrastLevel = contrastLevel + currentContrastLevel;
	}

	meshPoint->interpixelMeshPoint->luminosityContrast = contrastLevel;
}

void calculateMeshPointInterpixelDepth(MeshPoint * meshPoint)
{
	/*
	q;
	0.1.2    0 1
	3.4.5 => 3 4
	6 7 8
	*/

	double averageDepth = 0.0;

	vec * centrePixelNormal = &(meshPoint->meshPointNormal);

	double point1Depth;
	double point2Depth;
	double point3Depth;
	double point4Depth;

	point1Depth = meshPoint->adjacentMeshPoint[0]->depth;
	point2Depth = meshPoint->adjacentMeshPoint[1]->depth;
	point3Depth = meshPoint->adjacentMeshPoint[3]->depth;
	point4Depth = meshPoint->adjacentMeshPoint[4]->depth;

	averageDepth = (point1Depth+point2Depth+point3Depth+point4Depth)/4.0;
	meshPoint->interpixelMeshPoint->depth = averageDepth;

}

void calculateMeshPointInterpixelDepthWithForegroundDepthCheck(MeshPoint * meshPoint)
{
	/*
	q;
	0.1.2    0 1
	3.4.5 => 3 4
	6 7 8
	*/

	double minDepthForNearbyPoints = REALLY_FAR_AWAY;

	int qMapping[4];
	qMapping[0] = 0;
	qMapping[1] = 1;
	qMapping[2] = 3;
	qMapping[3] = 4;

	for(int p=0; p<4; p++)
	{
		int q = qMapping[p];
		double pointDepth = meshPoint->adjacentMeshPoint[q]->depth;
		if(pointDepth < minDepthForNearbyPoints)
		{
			minDepthForNearbyPoints = pointDepth;
		}
	}

	meshPoint->interpixelMeshPoint->depth = minDepthForNearbyPoints;

}


void calculateMeshPointInterpixelDepthWithForegroundDepthCheckOLD(MeshPoint * meshPoint)
{
	double averageDepth = 0.0;
	int numberOfDepthValsRecorded = 0;
	/*
	q;
	0.1.2    0 1
	3.4.5 => 3 4
	6 7 8
	*/


	int qMapping[2][2];
	qMapping[0][0] = 0;
	qMapping[0][1] = 1;
	qMapping[1][0] = 3;
	qMapping[1][1] = 4;

	for(int y=0; y<=1; y++)
	{
		//cout << "y = " << y << endl;
		int q;
		q = qMapping[0][y];
		double interpixelDepthValue = meshPoint->adjacentMeshPoint[q]->depth;
		q = qMapping[1][y];
		double adjacentInterpixelDepthValue = meshPoint->adjacentMeshPoint[q]->depth;

		#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
		if(compareDoubles(interpixelDepthValue, RT_RAYTRACE_NO_HIT_DEPTH_T))
		{
			interpixelDepthValue = ESTIMATE_MAX_DEPTH_T_REAL;
		}
		#endif
		#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
		if(compareDoubles(adjacentInterpixelDepthValue, RT_RAYTRACE_NO_HIT_DEPTH_T))
		{
			adjacentInterpixelDepthValue = ESTIMATE_MAX_DEPTH_T_REAL;
		}
		#endif

		if(interpixelDepthValue < (adjacentInterpixelDepthValue+OR_MAX_DEPTH_NOISE))
		{
			averageDepth = averageDepth + interpixelDepthValue;
			numberOfDepthValsRecorded++;
		}
		else if(adjacentInterpixelDepthValue < (interpixelDepthValue+OR_MAX_DEPTH_NOISE))
		{
			averageDepth = averageDepth + adjacentInterpixelDepthValue;
			numberOfDepthValsRecorded++;
		}
	}

	for(int x=0; x<=1; x++)
	{
		//cout << "x = " << x << endl;

		int q;
		q = qMapping[x][0];
		double interpixelDepthValue = meshPoint->adjacentMeshPoint[q]->depth;
		q = qMapping[x][1];
		double adjacentInterpixelDepthValue = meshPoint->adjacentMeshPoint[q]->depth;
		#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
		if(compareDoubles(interpixelDepthValue, RT_RAYTRACE_NO_HIT_DEPTH_T))
		{
			interpixelDepthValue = ESTIMATE_MAX_DEPTH_T_REAL;
		}
		#endif
		#ifdef OR_METHOD3DOD_GENERATE_IMAGE_DATA
		if(compareDoubles(adjacentInterpixelDepthValue, RT_RAYTRACE_NO_HIT_DEPTH_T))
		{
			adjacentInterpixelDepthValue = ESTIMATE_MAX_DEPTH_T_REAL;
		}
		#endif

		if(interpixelDepthValue < (adjacentInterpixelDepthValue+OR_MAX_DEPTH_NOISE))
		{
			averageDepth = averageDepth + interpixelDepthValue;
			numberOfDepthValsRecorded++;
		}
		else if(adjacentInterpixelDepthValue < (interpixelDepthValue+OR_MAX_DEPTH_NOISE))
		{
			averageDepth = averageDepth + adjacentInterpixelDepthValue;
			numberOfDepthValsRecorded++;
		}
	}

	if(numberOfDepthValsRecorded > 0)
	{
		averageDepth = averageDepth/numberOfDepthValsRecorded;
	}
	meshPoint->interpixelMeshPoint->depth = averageDepth;

}

//#endif



//see calculateMeshPointNormalsUsingPointMap
//requires;
//vec meshPointNormals[4];	//from 4 tris creating using mesh point and two out of four surrounding meshpoints, x-, x+, y-, y+
//vec meshPointNormal; 		//average of four calculated meshpoint normals
//no boundary checks - these can easily be implemented using adjacentMeshPointFilled
void calculateMeshPointNormal(MeshPoint * meshPoint)
{
	vec averageNormalVector;
	averageNormalVector.x = 0.0;
	averageNormalVector.y = 0.0;
	averageNormalVector.z = 0.0;

	for(int q=1; q<9; q=q+2)
	{
		vec point2;
		vec point3;

		int r=(q+2)%8;

		//calculate normal of vector using 3 points
		vec currentNormalVec;
		calculateNormalOfTri(&(meshPoint->point), &(meshPoint->adjacentMeshPoint[q]->point), &(meshPoint->adjacentMeshPoint[r]->point), &currentNormalVec);

		copyVectors(&(meshPoint->meshPointNormals[(q-1)/2]),  &currentNormalVec);

		averageNormalVector.x = averageNormalVector.x + currentNormalVec.x;
		averageNormalVector.y = averageNormalVector.y + currentNormalVec.y;
		averageNormalVector.z = averageNormalVector.z + currentNormalVec.z;
	}

	averageNormalVector.x = averageNormalVector.x/4;
	averageNormalVector.y = averageNormalVector.y/4;
	averageNormalVector.z = averageNormalVector.z/4;

	#ifndef DO_NOT_NORMALISE_POINT_NORMAL_MAP
	normaliseVector(&averageNormalVector);
	#endif

	copyVectors(&(meshPoint->meshPointNormal),  &averageNormalVector);
}

//see calculateMeshPointNormalsUsingPointMap
//no boundary checks - these can easily be implemented using adjacentMeshPointFilled
void calculateMeshPointNormalContrast(MeshPoint * meshPoint)
{
	double contrastLevel = 0.0;

	vec * centrePixelNormal = &(meshPoint->meshPointNormal);
	for(int q = 1; q < 9; q=q+2)
	{
		vec * currentPixelNormal = &(meshPoint->adjacentMeshPoint[q]->meshPointNormal);

		//calc point normal vector diff between centre pixel and current surrounding kernel pixel
		vec diff;
		subtractVectors(&diff, centrePixelNormal, currentPixelNormal);
		contrastLevel = maxDouble(contrastLevel, findMagnitudeOfVector(&diff));
	}
	meshPoint->meshPointNormalContrast = contrastLevel;
}
//#endif

void calculateMeshPointLuminosityContrast(MeshPoint * meshPoint)
{
	double contrastLevel = 0.0;

	double centrePixelPositionInLummapLuminosity = meshPoint->luminosity;

	for(int q = 0; q < 9; q++)
	{
		double kernelCurrentPixelPositionInLummapLuminosity = meshPoint->adjacentMeshPoint[q]->luminosity;
		double currentContrastLevel = absDouble(centrePixelPositionInLummapLuminosity - kernelCurrentPixelPositionInLummapLuminosity);
		contrastLevel = maxDouble(contrastLevel, currentContrastLevel);

		//cout << "contrastLevel = " << contrastLevel << endl;
		//cout << "currentContrastLevel = " << currentContrastLevel << endl;
	}
	meshPoint->luminosityContrast = contrastLevel;

	//cout << "meshPoint->luminosityContrast = " << meshPoint->luminosityContrast << endl;
}



