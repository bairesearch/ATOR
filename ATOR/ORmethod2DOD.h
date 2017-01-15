/*******************************************************************************
 *
 * File Name: ORmethod2DOD.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a6a 20-Mar-2012
 *
 *******************************************************************************/
#ifndef HEADER_OR_METHOD_2DOD
#define HEADER_OR_METHOD_2DOD

#include "LDreferenceClass.h"
#include "ORpolygonList.h"
#include "SHAREDglobalDefs.h"
#include "ORfeature.h"

#define OD2D_INTERPOLATED_MESH_PIXEL_WIDTH (1.0)
#define OD2D_INTERPOLATED_MESH_PIXEL_HEIGHT (1.0)
#define OD2D_INTERPOLATED_MESH_PIXEL_SIZE (OD2D_INTERPOLATED_MESH_PIXEL_WIDTH)

void createInterpolated2DMeshReferenceListUsingRGBMap2DOD(int imageWidth, int imageHeight, unsigned char * rgbMap, Reference * firstReferenceInInterpolatedMap);
void create2DMeshUsingRGBMap2DOD(int imageWidth, int imageHeight, double imageXOffset, double imageYOffset, unsigned char * rgbMap, MeshPoint * firstMeshPointInMeshList, MeshPoint * meshPointArray[], bool useEdgeZeroCrossingMap);






void transformObjectData2DOD(Reference * firstReferenceInInterpolated2DRGBMap, PolygonBAI * currentPolygonInList, int side, bool first, Feature * firstFeatureInList);
	double calculateXYOrientationOfSide(PolygonBAI * currentPolygonInList, int side);
	double calculateXYLengthOfSide(PolygonBAI * currentPolygonInList, int side);
	double calculateperpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(PolygonBAI * transformedObjectTriangle, int side);
	double calculatexAxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(PolygonBAI *transformedObjectTriangle, int side);

	void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(Reference * firstReferenceInInterpolated2DRGBMap, PolygonBAI * currentPolygonInList);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DOD(Reference * firstReferenceInInterpolated2DRGBMap, PolygonBAI * currentPolygonInList, bool padBoundary);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DODAdvanced2(Reference * firstReferenceInInterpolated2DRGBMap, PolygonBAI * currentPolygonInList, vec * point4, vec * point5, bool padBoundary);
	void disableReferencesThatAreNotContainedInTheObjectSquare2DODAdvanced(Reference * firstReferenceInInterpolated2DRGBMap, PolygonBAI * currentPolygonInList, bool padBoundary, int side, double shearFactor);

		void TEMPprintReferenceListVertexPositions2DOD(Reference * firstReferenceInInterpolated2DRGBMap);



#endif
