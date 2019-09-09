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
 * File Name: ATORmethod2DOD.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3n1a 10-August-2019
 * /
 *******************************************************************************/


#ifndef HEADER_OR_METHOD_2DOD
#define HEADER_OR_METHOD_2DOD

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
