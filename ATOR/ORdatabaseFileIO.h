/*******************************************************************************
 *
 * File Name: ORdatabaseFileIO.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a8a 14-June-2012
 *
 *******************************************************************************/



#ifndef HEADER_OR_DATABASE_FILEIO
#define HEADER_OR_DATABASE_FILEIO

#include "ORglobalDefs.h"
#include "SHAREDvars.h"
#include "ORpolygonList.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <time.h>
	#include <math.h>
	using namespace std;


#ifdef OR_METHOD_GEOMETRIC_COMPARISON
	bool compareFeaturesListForMatch(Feature * testfirstFeatureInNearestFeatureList, Feature * trainfirstFeatureInNearestFeatureList, int dimension, bool * exactMatchFound);
	void addFeatureToEndOfFeatureList(Feature * firstFeatureInList, Feature * featureToAdd);
	void createTransformedFeaturesFile(Feature * firstFeatureInList, string fileName, string objectName, int viewIndex, int zoomIndex, int polyIndex, int sideIndex, int trainOrTest);
	void createFeaturesListUsingFeaturesFile(string fileName, Feature * firstFeatureInList, bool createFeatureObjects, bool appendToList, bool ignoreOTfeatures);	
#endif

#endif




