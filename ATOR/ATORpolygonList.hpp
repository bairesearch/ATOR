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
 * File Name: ATORpolygonList.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3l1d 01-June-2017
 *
 *******************************************************************************/


#ifndef HEADER_OR_POLYGONLIST
#define HEADER_OR_POLYGONLIST


#include "SHAREDvars.hpp"
#include "ATORglobalDefs.hpp"
#include "SHAREDvector.hpp"

class ORobjectReferenceList{
private:
	/*There are currently no private attributes of this class*/
public:

	ORobjectReferenceList(void); // constructor declaration
	~ORobjectReferenceList();	//  and destructor.

	long objectNumber;
	string objectName;
	int numMatchingSnapshots;

	ORobjectReferenceList* next;
};



class ORsnapshotIDreferenceList{
private:
	/*There are currently no private attributes of this class*/
public:

	ORsnapshotIDreferenceList(void); // constructor declaration
	~ORsnapshotIDreferenceList();	//  and destructor.

	long referenceID;

	ORsnapshotIDreferenceList* next;
	ORsnapshotIDreferenceList* previous;	//only used for ORdatabaseDecisionTree.cpp
};


class ORfeature{
private:
	/*There are currently no private attributes of this class*/
public:

	ORfeature(void); // constructor declaration
	~ORfeature();	//  and destructor.

	int numberOfFeaturePixelsUsedToGenerateFeature;

	int xViewport;
	int yViewport;

	double magnitude;	//not currently used
	vec point;
	vec pointNonWorldCoord;

	#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	string objectName;
	int trainOrTest;
	int viewIndex;
	int zoomIndex;
	int polyIndex;
	int sideIndex;
	#ifdef OR_METHOD_TRANSFORM_NEARBY_FEATURES_TAG_OT_FEATURES
	int OTpointIndex;	//1,2 or 3
	#endif
	#ifdef OR_METHOD_GEO_COMPARISON_DYNAMIC_ERROR_THRESHOLD
	double minWidthAndHeightOfOrigOT;
	#endif
	vec pointTransformed;

	#ifdef VERBOSE_OR_OUTPUT_GEO_COORDINATES
	bool matchFound;	//added 8 June 2012 for debugging
	#endif

	#endif

	//#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
	signed char dctCoeff[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	unsigned long dctCoeffArrayBinned;
	//#endif

	//#ifdef OR_IMAGE_COMPARISON_AVERAGE_RGB_DEV_BINNING
	colour avgCol;
	//#endif

	char snapshotMapsText[OR_IMAGE_COMPARISON_SQL_ADD_ALL_MAPS_TO_DATABASE_MAX_DATA_LENGTH];
	int snapshotMapsTextLength;

	bool lastFilledFeatureInList;	//added 8 June 2012 to fix overrun
	ORfeature* next;

	//bool nearestFeatureActive;	//added 13 June 2012 to prevent non-filled existent features from being used to create polys

};

class ORpolygon{
private:
	/*There are currently no private attributes of this class*/
public:

	ORpolygon(void); // constructor declaration
	~ORpolygon();	//  and destructor.

	vec point1;
	vec point2;
	vec point3;
	ORpolygon* next;

	#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	ORfeature* firstFeatureInNearestFeatureList;
	#endif

	//#ifdef OR_METHOD_TRANSFORM_KEY_OT_FEATURES
	vec point1Transformed;
	vec point2Transformed;
	vec point3Transformed;
	//#endif

};

#ifdef OR_METHOD_GEOMETRIC_COMPARISON
class ORfeatureContainer{
private:
	/*There are currently no private attributes of this class*/
public:

	ORfeatureContainer(void); // constructor declaration
	~ORfeatureContainer();	//  and destructor.

	int numVotes;
	double error;

	ORfeature* firstFeatureInFeatureList;
	ORfeatureContainer* next;
};
#endif


#define NOT_PROCESSED (1)
#define EDGE_FOUND (2)
#define NO_EDGE_FOUND (3)


class ORmeshPoint{
private:
	/*There are currently no private attributes of this class*/
public:

	ORmeshPoint(void); 	// constructor declaration
	~ORmeshPoint();	//  and destructor.

	vec point;
	int xInt;
	int yInt;
	colour col;
	double depth;	//should not be used - use point map value instead...
	double luminosity;
	double luminosityContrast;	//[emulates luminosity contrast map]

	//#ifdef OR_USE_CONTRAST_CALC_METHOD_C
	ORmeshPoint* interpixelMeshPoint;	//used to store contrast mesh as a submesh of an rgb mesh with a 0.5/0.5 offset
	bool interpixelMeshPointFilled;
	//#endif

	ORmeshPoint* adjacentMeshPoint[9];	//y:0,x:0->3, y:1,x:0->3, y:2,x:0->3
	bool adjacentMeshPointFilled[9]; 	//to specify the boundary conditions of the meshpoint

	//#ifdef OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST
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

	bool edge;	//currently only used if OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED
	//#ifdef OR_METHOD_QUADRATIC_FIT_FOR_MESH_LISTS_HAS_BEEN_PROGRAMMED
	double zeroCrossingValueX;
	double zeroCrossingValueY;
	double alpha;
	vec quadraticFitPoint;
	double quadraticFitDepth;
	//#endif



	ORmeshPoint* next;
};

class ORpolygonListClass
{
	private: SHAREDvectorClass SHAREDvector;
	private: SHAREDvarsClass SHAREDvars;
	public: ORmeshPoint* findMeshPointIntInMesh(ORmeshPoint* firstMeshPointInMeshList, const int x, const int y, bool* hasFoundMeshPoint);
	public: ORmeshPoint* findMeshPointIntInMesh(ORmeshPoint* firstMeshPointInMeshList, const vec* point, bool* hasFoundMeshPoint, const int meshZoneLimit);
	public: ORmeshPoint* findMeshPointIntInMesh(ORmeshPoint* firstMeshPointInMeshList, const int position);

	//#ifdef OR_METHOD3DOD_USE_MESH_NORMAL_AND_NORMAL_CONTRAST
	public: void calculateMeshPointNormal(ORmeshPoint* meshPoint);
	public: void calculateMeshPointNormalContrast(ORmeshPoint* meshPoint);
	//#ifdef OR_USE_CONTRAST_CALC_METHOD_C
	public: void calculateMeshPointInterpixelNormal(ORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelNormalContrast(ORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelLuminosityContrast(ORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelDepth(ORmeshPoint* meshPoint);
	public: void calculateMeshPointInterpixelDepthWithForegroundDepthCheck(ORmeshPoint* meshPoint);
	//#endif
	//#endif
	public: void calculateMeshPointLuminosityContrast(ORmeshPoint* meshPoint);
};



class ORpixelContiguous{
private:
	/*There are currently no private attributes of this class*/
public:

	ORpixelContiguous(void); // constructor declaration
	ORpixelContiguous(int newX, int newY, ORpixelContiguous* newPrevious);
	ORpixelContiguous(ORmeshPoint* newMeshPoint, ORpixelContiguous* newPrevious);
	~ORpixelContiguous();	//  and destructor.

	bool pathAlreadyCrawled;
	bool finalPathAlreadyCrawled;
	int xInt;			//only used by non ORmeshPoint functions
	int yInt;			//only used by non ORmeshPoint functions
	vec point;			//only used by non ORmeshPoint functions
	vec pointNonWorldCoord;		//only used by non ORmeshPoint functions

	ORmeshPoint* meshPoint;	//only used by ORmeshPoint functions

	ORpixelContiguous* next[9];
	ORpixelContiguous* previous;

	bool readyToDelete;
};





#endif
