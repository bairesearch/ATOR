 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORpolygonList.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_POLYGONLIST
#define HEADER_ATOR_POLYGONLIST


#include "SHAREDvars.hpp"
#include "ATORglobalDefs.hpp"
#include "SHAREDvector.hpp"

class ATORobjectReferenceList{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORobjectReferenceList(void); // constructor declaration
	~ATORobjectReferenceList();	//  and destructor.

	int64_t objectNumber;
	string objectName;
	int numMatchingSnapshots;

	ATORobjectReferenceList* next;
};



class ATORsnapshotIDreferenceList{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORsnapshotIDreferenceList(void); // constructor declaration
	~ATORsnapshotIDreferenceList();	//  and destructor.

	int64_t referenceID;

	ATORsnapshotIDreferenceList* next;
	ATORsnapshotIDreferenceList* previous;	//only used for ATORdatabaseDecisionTree.cpp
};


class ATORfeature{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORfeature(void); // constructor declaration
	~ATORfeature();	//  and destructor.

	int numberOfFeaturePixelsUsedToGenerateFeature;

	int xViewport;
	int yViewport;

	double magnitude;	//not currently used
	vec point;
	vec pointNonWorldCoord;

	#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
	string objectName;
	int trainOrTest;
	int viewIndex;
	int zoomIndex;
	int polyIndex;
	int sideIndex;
	#ifdef ATOR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	int OTpointIndex;	//1,2 or 3
	#endif
	#ifdef ATOR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
	double minWidthAndHeightOfOrigOT;
	#endif
	vec pointTransformed;

	#ifdef ATOR_VERBOSE_OUTPUT_GEO_COORDINATES
	bool matchFound;	//added 8 June 2012 for debugging
	#endif

	#endif

	//#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	schar dctCoeff[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	uint64_t dctCoeffArrayBinned;
	//#endif

	//#ifdef ATOR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	colour avgCol;
	//#endif

	char snapshotMapsText[ATOR_IMAGE_COMPARISON_ADD_ALL_MAPS_TO_DATABASE_MAX_DATA_LENGTH];
	int snapshotMapsTextLength;

	bool lastFilledFeatureInList;	//added 8 June 2012 to fix overrun
	ATORfeature* next;

	//bool nearestFeatureActive;	//added 13 June 2012 to prevent non-filled existent features from being used to create polys

};

class ATORpolygon{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORpolygon(void); // constructor declaration
	~ATORpolygon();	//  and destructor.

	vec point1;
	vec point2;
	vec point3;
	ATORpolygon* next;

	#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
	ATORfeature* firstFeatureInNearestFeatureList;
	#endif

	//#ifdef ATOR_METHOD_TRANSFORM_KEY_OT_FEATURES
	vec point1Transformed;
	vec point2Transformed;
	vec point3Transformed;
	//#endif

};

#ifdef ATOR_METHOD_GEOMETRIC_COMPARISON
class ATORfeatureContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORfeatureContainer(void); // constructor declaration
	~ATORfeatureContainer();	//  and destructor.

	int numVotes;
	double error;

	ATORfeature* firstFeatureInFeatureList;
	ATORfeatureContainer* next;
};
#endif


#define NOT_PROCESSED (1)
#define EDGE_FOUND (2)
#define NO_EDGE_FOUND (3)


class ATORmeshPoint{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORmeshPoint(void); 	// constructor declaration
	~ATORmeshPoint();	//  and destructor.

	vec point;
	int xInt;
	int yInt;
	colour col;
	double depth;	//should not be used - use point map value instead...
	double luminosity;
	double luminosityContrast;	//[emulates luminosity contrast map]

	//#ifdef ATOR_USE_CONTRAST_CALC_METHOD_C
	ATORmeshPoint* interpixelMeshPoint;	//used to store contrast mesh as a submesh of an rgb mesh with a 0.5/0.5 offset
	bool interpixelMeshPointFilled;
	//#endif

	ATORmeshPoint* adjacentMeshPoint[9];	//y:0,x:0->3, y:1,x:0->3, y:2,x:0->3
	bool adjacentMeshPointFilled[9]; 	//to specify the boundary conditions of the meshpoint

	//#ifdef ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST
	bool meshPointNormalFilled;
	vec meshPointNormals[4];	//from 4 tris creating using mesh point and two out of four surrounding meshpoints, x-, x+, y-, y+
	vec meshPointNormal; 		//average of four calculated meshpoint normals
	double meshPointNormalContrast;	//[emulates depth gradient contrast map]
		// note this will identify edges and points and not points specifically [dont get confused with the word point in this variable name]
		// note that we need to then apply heitger 2D feature detection to this normal contrast map to calculate corners in 3D space
		// (note we are currently using the depth gradient and depth gradient contrast map to do this, but we would like to use the point map instead)

	//double pointContrast [emulates depth contrast map - consider this, which calculates when there is a big difference between point positions - not currently used as dont want to be calculating points on the edges of spheres]

	//#endif

	int alreadyProcessed;
	//bool hasBeenProcessedInSearchForTheOuterBoundary;
	bool hasBeenProcessedInGenerationOfOuterBoundary;

	bool edge;	//currently only used if ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED
	//#ifdef ATOR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED
	double zeroCrossingValueX;
	double zeroCrossingValueY;
	double alpha;
	vec quadraticFitPoint;
	double quadraticFitDepth;
	//#endif



	ATORmeshPoint* next;
};

class ATORpolygonListClass
{
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	public: ATORmeshPoint* findMeshPointIntInMesh(ATORmeshPoint* firstMeshPointInMeshList, const int x, const int y, bool* hasFoundMeshPoint);
	public: ATORmeshPoint* findMeshPointIntInMesh(ATORmeshPoint* firstMeshPointInMeshList, const vec* point, bool* hasFoundMeshPoint, const int meshZoneLimit);
	public: ATORmeshPoint* findMeshPointIntInMesh(ATORmeshPoint* firstMeshPointInMeshList, const int position);

	//#ifdef ATOR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST
	public: void calculateMeshPointNormal(ATORmeshPoint* meshPoint);
	public: void calculateMeshPointNormalContrast(ATORmeshPoint* meshPoint);
	//#ifdef ATOR_USE_CONTRAST_CALC_METHOD_C
	public: void calculateMeshPointInterpixelNormal(ATORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelNormalContrast(ATORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelLuminosityContrast(ATORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelDepth(ATORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelDepthWithForegroundDepthCheck(ATORmeshPoint* meshPoint);
	//#endif
	//#endif
	public: void calculateMeshPointLuminosityContrast(ATORmeshPoint* meshPoint);
};



class ATORpixelContiguous{
private:
	/*There are currently no private attributes of this class*/
public:

	ATORpixelContiguous(void); // constructor declaration
	ATORpixelContiguous(int newX, int newY, ATORpixelContiguous* newPrevious);
	ATORpixelContiguous(ATORmeshPoint* newMeshPoint, ATORpixelContiguous* newPrevious);
	~ATORpixelContiguous();	//  and destructor.

	bool pathAlreadyCrawled;
	bool finalPathAlreadyCrawled;
	int xInt;			//only used by non ATORmeshPoint functions
	int yInt;			//only used by non ATORmeshPoint functions
	vec point;			//only used by non ATORmeshPoint functions
	vec pointNonWorldCoord;		//only used by non ATORmeshPoint functions

	ATORmeshPoint* meshPoint;	//only used by ATORmeshPoint functions

	ATORpixelContiguous* next[9];
	ATORpixelContiguous* previous;

	bool readyToDelete;
};





#endif
