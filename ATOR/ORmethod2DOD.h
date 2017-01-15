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
 * File Name: ORmethod2DOD.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3c3a 16-November-2012
 *
 *******************************************************************************/
#ifndef HEADER_OR_METHOD_2DOD
#define HEADER_OR_METHOD_2DOD

#include "LDreferenceClass.h"
#include "ORpolygonList.h"
#include "ORglobalDefs.h"
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
