 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORmethod2DOD.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_METHOD_2DOD
#define HEADER_ATOR_METHOD_2DOD

#include "LDreferenceClass.hpp"
#include "ATORpolygonList.hpp"
#include "ATORglobalDefs.hpp"
#include "ATORfeatureGeneration.hpp"
#include "SHAREDvector.hpp"
#include "ATORpixelMaps.hpp"
#include "ATORoperations.hpp"
	#include "LDopengl.hpp"

#define OD2D_INTERPOLATED_MESH_PIXEL_WIDTH (1.0)
#define OD2D_INTERPOLATED_MESH_PIXEL_HEIGHT (1.0)
#define OD2D_INTERPOLATED_MESH_PIXEL_SIZE (OD2D_INTERPOLATED_MESH_PIXEL_WIDTH)

class ATORmethod2DODClass
{
	private: SHAREDvectorClass SHAREDvector;
	private: RTpixelMapsClass RTpixelMaps;
	private: ATORpolygonListClass ATORpolygonList;
	private: ATORfeatureGenerationClass ATORfeatureGeneration;
	private: SHAREDvarsClass SHAREDvars;
	private: ATORoperationsClass ATORoperations;
	private: LDopenglClass LDopengl;
	public: void createInterpolated2DmeshReferenceListUsingRGBmap2DOD(int imageWidth, const int imageHeight, uchar* rgbMap, LDreference* firstReferenceInInterpolatedMap);
	public: void create2DmeshUsingRGBmap2DOD(int imageWidth, const int imageHeight, double imageXOffset, double imageYOffset, uchar* rgbMap, ATORmeshPoint* firstMeshPointInMeshList, ATORmeshPoint* meshPointArray[], const bool useEdgeZeroCrossingMap);

	public: void transformObjectData2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ATORpolygon* currentPolygonInList, const int side, const bool first, ATORfeature* firstFeatureInList);
		private: double calculateXYorientationOfSide(const ATORpolygon* currentPolygonInList, const int side);
		private: double calculateXYlengthOfSide(const ATORpolygon* currentPolygonInList, const int side);
		private: double calculatePerpendicularDistanceBetweenThirdApexOfObjectTriangleAndSide(const ATORpolygon* transformedObjectTriangle, const int side);
		private: double calculateXaxisDistanceBetweenThirdApexOfObjectTriangleAndSideLeftApex(const ATORpolygon* transformedObjectTriangle, const int side);

		private: void disableReferencesThatAreNotContainedInTheObjectTriangle2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, const ATORpolygon* currentPolygonInList);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DOD(LDreference* firstReferenceInInterpolated2DrgbMap, ATORpolygon* currentPolygonInList, bool padBoundary);
	//void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced2(LDreference* firstReferenceInInterpolated2DrgbMap, ATORpolygon* currentPolygonInList, vec* point4, vec* point5, bool padBoundary);
		private: void disableReferencesThatAreNotContainedInTheObjectSquare2DODadvanced(LDreference* firstReferenceInInterpolated2DrgbMap, const ATORpolygon* currentPolygonInList, const bool padBoundary, const int side, const double shearFactor);

			private: void TEMPprintReferenceListVertexPositions2DOD(const LDreference* firstReferenceInInterpolated2DrgbMap);
};



#endif
