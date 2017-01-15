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
 * File Name: ATORmethod2DOD.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3j1c 14-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_METHOD_2DOD
#define HEADER_OR_METHOD_2DOD

#include "LDreferenceClass.h"
#include "ATORpolygonList.h"
#include "ATORglobalDefs.h"
#include "ATORfeatureGeneration.h"
#include "SHAREDvector.h"
#include "ATORpixelMaps.h"
#include "ATORoperations.h"
	#include "LDopengl.h"

#define OD2D_INTERPOLATED_MESH_PIXEL_WIDTH (1.0)
#define OD2D_INTERPOLATED_MESH_PIXEL_HEIGHT (1.0)
#define OD2D_INTERPOLATED_MESH_PIXEL_SIZE (OD2D_INTERPOLATED_MESH_PIXEL_WIDTH)

class ORmethod2DODClass
{
	private: SHAREDvectorClass SHAREDvector;
	private: RTpixelMapsClass RTpixelMaps;
	private: ORpolygonListClass ORpolygonList;
	private: ORfeatureGenerationClass ORfeatureGeneration;
	private: SHAREDvarsClass SHAREDvars;
	private: ORoperationsClass ORoperations;
	private: LDopenglClass LDopengl;
	public: void createInterpolated2DmeshReferenceListUsingRGBmap2DOD(int imageWidth, const int imageHeight, unsigned char* rgbMap, LDreference* firstReferenceInInterpolatedMap);
	public: void create2DmeshUsingRGBmap2DOD(int imageWidth, const int imageHeight, double imageXOffset, double imageYOffset, unsigned char* rgbMap, ORmeshPoint* firstMeshPointInMeshList, ORmeshPoint* meshPointArray[], const bool useEdgeZeroCrossingMap);

	public: void transformObjectData2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, const int side, const bool first, ORfeature* firstFeatureInList);
		private: double calculateXYorientationOfSide(const ORpolygon* currentPolygonInList, const int side);
		private: double calculateXYlengthOfSide(const ORpolygon* currentPolygonInList, const int side);
		private: double calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(const ORpolygon* transformedObjectTriangle, const int side);
		private: double calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(const ORpolygon* transformedObjectTriangle, const int side);

		private: void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, bool padBoundary);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced2(LDreference* firstReferenceInInterpolated2DrgbMap, ORpolygon* currentPolygonInList, vec* point4, vec* point5, bool padBoundary);
		private: void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(LDreference* firstReferenceInInterpolated2DrgbMap, const ORpolygon* currentPolygonInList, const bool padBoundary, const int side, const double shearFactor);

			private: void TEMPprintReferenceListVertexPositions2DOD(const LDreference* firstReferenceInInterpolated2DrgbMap);
};



#endif
