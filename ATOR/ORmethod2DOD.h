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
 * Project Version: 3c5b 15-December-2012
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

void createInterpolated2DmeshReferenceListUsingRGBmap2DOD(int imageWidth, int imageHeight, unsigned char * rgbMap, Reference * firstReferenceInInterpolatedMap);
void create2DmeshUsingRGBmap2DOD(int imageWidth, int imageHeight, double imageXOffset, double imageYOffset, unsigned char * rgbMap, MeshPoint * firstMeshPointInMeshList, MeshPoint * meshPointArray[], bool useEdgeZeroCrossingMap);






void transformObjectData2DOD(Reference * firstReferenceInInterpolated2DrgbMap, PolygonBAI * currentPolygonInList, int side, bool first, Feature * firstFeatureInList);
	double calculateXYorientationOfSide(PolygonBAI * currentPolygonInList, int side);
	double calculateXYlengthOfSide(PolygonBAI * currentPolygonInList, int side);
	double calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(PolygonBAI * transformedObjectTriangle, int side);
	double calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(PolygonBAI * transformedObjectTriangle, int side);

	void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(Reference * firstReferenceInInterpolated2DrgbMap, PolygonBAI * currentPolygonInList);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DOD(Reference * firstReferenceInInterpolated2DrgbMap, PolygonBAI * currentPolygonInList, bool padBoundary);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced2(Reference * firstReferenceInInterpolated2DrgbMap, PolygonBAI * currentPolygonInList, vec * point4, vec * point5, bool padBoundary);
	void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(Reference * firstReferenceInInterpolated2DrgbMap, PolygonBAI * currentPolygonInList, bool padBoundary, int side, double shearFactor);

		void TEMPprintReferenceListVertexPositions2DOD(Reference * firstReferenceInInterpolated2DrgbMap);



#endif
