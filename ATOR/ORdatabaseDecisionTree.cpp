/*******************************************************************************
 *
 * File Name: ORdatabaseDecisionTree.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a8a 14-June-2012
 *
 *******************************************************************************/

#include "ORdatabaseDecisionTree.h"
#include "ORdatabaseSQL.h"
#include "LDreferenceManipulation.h"
#include "RTpixelMaps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE

#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL

	string imageComparisonTreeBaseDirectory;
	string imageComparisonTreeName;


		//for mkdir, chdir, etc	[CreateDirectory, SetCurrentDirectory]	
	#ifdef LINUX
		//#include <direct.h>
		//#include <unistd.h>
		//#include <stdio.h>
		//#include <dir.h>
		#include <sys/stat.h> 	//is this needed?
		#include <sys/types.h>	//is this needed?
	#else
		#include <windows.h>
	#endif

#endif

/*
this is effectively a preconfigured neural network / decision tree - allowing instantaneous access to similar snapshots for comparison
this should only be executed on small images, where are determinisitic
new method; binary tree - if p1 r > p2 r, if p1 g > p2 g, if p1 b > p2 b, if p2 r > p3 r, etc... [store in local file system, at end of each path, store a list.txt file containing points to ID in mysql DB)
cannot account for strong local variations in coloured lighting (non white) be able to account 
Might need ext4;
	http://en.wikipedia.org/wiki/Ext3;	Directory contents 	Table, hashed B-tree with dir_index enabled
	http://en.wikipedia.org/wiki/Ext4; 	Directory contents 	Linked list, hashed B-tree
*/


#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
void createAndOrParseIntoDirectory(string * folderNameFullPath, string * folderName, bool create, bool relativeOrAbsolute)
{
	if(create)
	{
		if(relativeOrAbsolute)
		{
			#ifdef LINUX
			mkdir(folderName->c_str(), 0770);
			chdir(folderName->c_str());						
			#else
			::CreateDirectory(folderName->c_str(), NULL);
			::SetCurrentDirectory(folderName->c_str());
			#endif
		}
		else
		{
			//create;
			#ifdef LINUX
			mkdir(folderNameFullPath->c_str(), 0770);
			chdir(folderNameFullPath->c_str());
			#else
			::CreateDirectory(folderNameFullPath->c_str(), NULL);
			::SetCurrentDirectory(folderNameFullPath->c_str());
			#endif
		}
	}
	else
	{
		if(relativeOrAbsolute)
		{
			#ifdef LINUX
			chdir(folderName->c_str());						
			#else
			::SetCurrentDirectory(folderName->c_str());
			#endif
		}
		else
		{
			#ifdef LINUX
			chdir(folderNameFullPath->c_str());						
			#else
			::SetCurrentDirectory(folderNameFullPath->c_str());
			#endif		
		}
	}
}
#endif	

//#define OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopGeo(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;
	
	bool addPermutationsOfTrainFeaturesForGeoBinning;
	int maxNumFeaturePermutations;


	Feature * currentFeatureInTempList = firstFeatureInList;
		//now bin the features;
	int findex1 = 0;
	
	while(currentFeatureInTempList->next != NULL)
	{
		Feature * currentFeatureInTempList2 = firstFeatureInList;

		if((!ignoreOTfeatures) || (currentFeatureInTempList->OTpointIndex == 0))
		{//perform binning of nearby features only (not OT features)

				//now bin the features;
			int findex2 = 0;
			while(currentFeatureInTempList2->next != NULL)
			{
				
				if((!ignoreOTfeatures) || (currentFeatureInTempList2->OTpointIndex == 0))
				{//perform binning of nearby features only (not OT features)


					if(findex1 != findex2)
					{	

						
						int geoxyBinBiasInt[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
						bool insideBin = false;
						int geoxBin[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						int geoyBin[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						int geoxyBinTemp[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];						
						double geoxyBinDoubleTemp[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
						geoxyBinDoubleTemp[0] = determineGeoBinDoubleX(currentFeatureInTempList->pointTransformed.x);
						geoxyBinDoubleTemp[1] = determineGeoBinDoubleY(currentFeatureInTempList->pointTransformed.y);
						geoxyBinDoubleTemp[2] = determineGeoBinDoubleX(currentFeatureInTempList2->pointTransformed.x);
						geoxyBinDoubleTemp[3] = determineGeoBinDoubleY(currentFeatureInTempList2->pointTransformed.y);
						geoxBin[0] = geoxyBinDoubleTemp[0]; 
						geoyBin[0] = geoxyBinDoubleTemp[1];
						geoxBin[1] = geoxyBinDoubleTemp[2];
						geoyBin[1] = geoxyBinDoubleTemp[3];
						geoxyBinTemp[0] = geoxBin[0];
						geoxyBinTemp[1] = geoyBin[0];
						geoxyBinTemp[2] = geoxBin[1];
						geoxyBinTemp[3] = geoyBin[1];
						
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						for(int i=0; i<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2; i++)
						{
							/*
							cout << "geoxyBinDoubleTemp[i] = " << geoxyBinDoubleTemp[i] << endl;
							cout << "geoxyBinTemp[i] = " << geoxyBinTemp[i] << endl;
							*/
							if(geoxyBinDoubleTemp[i] >= double(geoxyBinTemp[i])+0.5-DOUBLE_MIN_PRECISION+OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD)
							{
								geoxyBinBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;
								//cout << "h2" << endl;
								//exit(0);
							}
							else if(geoxyBinDoubleTemp[i] <= double(geoxyBinTemp[i])+0.5-DOUBLE_MIN_PRECISION-OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD)
							{
								geoxyBinBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;
								//cout << "h1" << endl;
								//exit(0);
							}
							else
							{
								geoxyBinBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
							}														
						}
						#endif

						
						//cout << "3" << endl;

						if(geoxBin[0] > 0 )
						{
							if(geoxBin[0] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
							{
								if(geoyBin[0] > 0 )
								{
									if(geoyBin[0] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
									{
										if(geoxBin[1] > 0 )
										{
											if(geoxBin[1] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
											{
												if(geoyBin[1] > 0 )
												{
													if(geoyBin[1] < OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
													{
														insideBin = true;
														//this passes; cout << "inside bin during array formation - good" << endl;
													}
												}
											}
										}
									}
								}
							}
						}
										
						if(insideBin)
						{
							

							//int currentDirectoryLengthBeforeGeoAndRGB = currentDirectoryLength;


						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_HIGH_REDUNDANCY_DETERMINISTIC_BY_ITERATING_OVER_ALL_ADJACENT_BINS_FAST_RECOG_BUT_USE_MORE_HD
							//cout << "sad" << endl;

							int xBin = geoxBin[0];
							int x2Bin = geoxBin[1]; 
							int yBin = geoyBin[0];
							int y2Bin = geoyBin[1];


							int xBinIndex = 0;
							for(int x = xBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=xBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
							{

								int yBinIndex =0;
								for(int y = yBin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=yBin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
								{

									//check current bin and nearest bins
									int xBinIndex2 = 0;
									for(int x2 = x2Bin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2<=x2Bin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2++)
									{

										int y2BinIndex =0;
										for(int y2 = y2Bin-(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2<=y2Bin+(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2++)
										{

						#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD

							int geoxyBins[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
							geoxyBins[0] = geoxBin[0];
							geoxyBins[1] = geoyBin[0];
							geoxyBins[2] = geoxBin[1];
							geoxyBins[3] = geoyBin[1];

							for(int geoExceptionIndex1=0; geoExceptionIndex1<(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2)+1; geoExceptionIndex1++)
							{ 
								for(int geoExceptionModifier1=-1; geoExceptionModifier1<=1; geoExceptionModifier1=geoExceptionModifier1+2)
								{	
									if(geoExceptionIndex1 != (OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
									{
										geoxyBins[geoExceptionIndex1] = geoxyBins[geoExceptionIndex1]+geoExceptionModifier1;
									}

									#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2

									for(int geoExceptionIndex2=0; geoExceptionIndex2<(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2)+1; geoExceptionIndex2++)
									{ 
										for(int geoExceptionModifier2=-1; geoExceptionModifier2<=1; geoExceptionModifier2=geoExceptionModifier2+2)
										{	
											if(geoExceptionIndex2 != (OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
											{
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]+geoExceptionModifier2;
											}

									#endif									

											int x = geoxyBins[0];
											int y = geoxyBins[1];
											int x2 = geoxyBins[2];
											int y2 = geoxyBins[3];

						#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

							int geoxyBins[OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
							geoxyBins[0] = geoxBin[0];
							geoxyBins[1] = geoyBin[0];
							geoxyBins[2] = geoxBin[1];
							geoxyBins[3] = geoyBin[1];

							#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
							SnapshotIDReferenceList * firstReferenceInDTBinListGeo = new SnapshotIDReferenceList();
							SnapshotIDReferenceList * currentReferenceInDTBinListGeo = firstReferenceInDTBinListGeo; 
							#endif


							//int numberOfIntelligentGeoBins = countIncrements(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2);  //2^(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2);	
							int numberOfIntelligentGeoBins = pow(2.0, (OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2));
							for(int geoExceptionIndex1=0; geoExceptionIndex1<numberOfIntelligentGeoBins; geoExceptionIndex1++)
							{ 	
								#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
								long dtBinTemp = 0;
								#endif	

								#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
								bool performAtLeastOneIntelligentGeoBinBias = false;
								bool firstIntelligentBinPermutation = false;
								if(geoExceptionIndex1 == 0)
								{
									firstIntelligentBinPermutation = true;
								}
								#endif


								#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
								int numberOfBinsThatAreBiased = 0;
								for(int geoExceptionIndex2=0; geoExceptionIndex2<(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2); geoExceptionIndex2++)
								{
									bool biasIsOnForThisBin = false;
									if(((geoExceptionIndex1>>geoExceptionIndex2)%2) == 0)
									{//bit is off
										//cout << "bit is off; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;  
										biasIsOnForThisBin = false;
									}
									else
									{//bit is on
										//cout << "bit is on; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;  												
										biasIsOnForThisBin = true;

									}
									if(biasIsOnForThisBin)
									{
										numberOfBinsThatAreBiased++;
									}

								}
								if(numberOfBinsThatAreBiased <= double(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2)*OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS_MAX_NUM_BIAS_BINS_RATIO)
								{
								#endif

									for(int geoExceptionIndex2=0; geoExceptionIndex2<(OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2); geoExceptionIndex2++)
									{

										bool biasIsOnForThisBin = false;
										/*
										if(0%2 == 0)
										{
											cout << "bad" << endl;
											exit(0);
										}
										*/

										if(((geoExceptionIndex1>>geoExceptionIndex2)%2) == 0)
										{//bit is off
											//cout << "bit is off; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;  
											biasIsOnForThisBin = false;
										}
										else
										{//bit is on
											//cout << "bit is on; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;  												
											biasIsOnForThisBin = true;
										}

										int bias;
										bool performIntelligentGeoBinBias;
										if(!biasIsOnForThisBin)
										{//then assume central bin, no change in this geo bin
											performIntelligentGeoBinBias = false;	//not used
										}
										else
										{//then assume biased bin if necessary, positive or negative
											if(geoxyBinBiasInt[geoExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG)
											{
												#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
												performAtLeastOneIntelligentGeoBinBias = true;
												#endif
												performIntelligentGeoBinBias = true;	//not used
												bias = -1;
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]+bias;			
											}
											else if(geoxyBinBiasInt[geoExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS)
											{
												#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
												performAtLeastOneIntelligentGeoBinBias = true;
												#endif																	
												performIntelligentGeoBinBias = true;	//not used 
												bias = 1;
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]+bias;
											}
											else if(geoxyBinBiasInt[geoExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME)
											{
												performIntelligentGeoBinBias = false;	//not used
												//do not add
											}
										}


										#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
										#if (OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS < 100)
										//cout << "dtBinTempGeoGoing = " << dtBinTemp << endl;
										dtBinTemp = dtBinTemp + geoxyBins[geoExceptionIndex2]*(pow(100, geoExceptionIndex2));

										/*
										cout << "dtBinTemp temp = " << dtBinTemp << endl;
										cout << " geoxyBins[geoExceptionIndex2] = " <<  geoxyBins[geoExceptionIndex2] << endl;
										cout << " pow(100, geoExceptionIndex2) = " <<  pow(100, geoExceptionIndex2) << endl;																
										*/
										#else
										cout << "invalid OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS, must be < 100" << endl;
										exit(0);
										#endif
										#endif
										//do: apply check; only insert new row into dt if it is unique to the given snapshot ID														

									}

									#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
									bool placedNewDTBinReference = false;
									bool dtBinAlreadyAddedToList = false;
									currentReferenceInDTBinListGeo = firstReferenceInDTBinListGeo; 
									SnapshotIDReferenceList * previousReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
									bool DTBinListGeoIndex = 0;
									int currentDTBinValue = currentReferenceInDTBinListGeo->referenceID;
									//cout << "dtBinTemp = " << dtBinTemp << endl;
									//exit(0);
									while(!placedNewDTBinReference && !dtBinAlreadyAddedToList)
									{
										if(currentReferenceInDTBinListGeo->referenceID == dtBinTemp)
										{
											dtBinAlreadyAddedToList = true;
										}
										else
										{
											if(currentReferenceInDTBinListGeo->referenceID >= currentDTBinValue)
											{
												SnapshotIDReferenceList * newReferenceInDTBinListGeo = new SnapshotIDReferenceList();
												newReferenceInDTBinListGeo->referenceID = dtBinTemp;														
												//cout << "add bin to list; newReferenceInDTBinListGeo->referenceID = " << newReferenceInDTBinListGeo->referenceID << endl;
												if(DTBinListGeoIndex > 0)
												{
													SnapshotIDReferenceList * tempRef = previousReferenceInDTBinListGeo->next;
													tempRef->previous = newReferenceInDTBinListGeo;
													newReferenceInDTBinListGeo->next = tempRef;
													newReferenceInDTBinListGeo->previous = previousReferenceInDTBinListGeo; 
													previousReferenceInDTBinListGeo->next = newReferenceInDTBinListGeo;
												}
												else
												{
													SnapshotIDReferenceList * tempRef = firstReferenceInDTBinListGeo;
													tempRef->previous = newReferenceInDTBinListGeo;
													newReferenceInDTBinListGeo->next = tempRef;
													firstReferenceInDTBinListGeo = newReferenceInDTBinListGeo;
												}																		
												placedNewDTBinReference = true;
											}
										}

										if(DTBinListGeoIndex == 0)
										{
											previousReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
										}

										DTBinListGeoIndex++;
										currentDTBinValue = currentReferenceInDTBinListGeo->referenceID;
										previousReferenceInDTBinListGeo = currentReferenceInDTBinListGeo;
										currentReferenceInDTBinListGeo = currentReferenceInDTBinListGeo->next;
									}
									#endif


									#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
									if(!dtBinAlreadyAddedToList)
									{
									#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
									if(performAtLeastOneIntelligentGeoBinBias || firstIntelligentBinPermutation)
									{															
									#endif

										int x = geoxyBins[0];
										int y = geoxyBins[1];
										int x2 = geoxyBins[2];
										int y2 = geoxyBins[3];															

						#else 



							int x = geoxBin[0];
							int y = geoyBin[0];
							int x2 = geoxBin[1];
							int y2 = geoyBin[1];

						#endif
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = initialDirectory;
											createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
											#endif

											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = "";
											#endif										

											*currentDirectoryLength = currentDirectoryLengthInitial;
											
											char geobinxString[25];
											sprintf(geobinxString, "%2d", x);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobinxString;
											createAndOrParseIntoDirectory(&currentDirectory, &string(geobinxString), true, true);
											#else
											currentDirectory = currentDirectory + geobinxString;
											#endif

											char geobinyString[25];
											sprintf(geobinyString, "%2d", y);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobinyString;
											createAndOrParseIntoDirectory(&currentDirectory, &string(geobinyString), true, true);							
											#else
											currentDirectory = currentDirectory + geobinyString;
											#endif

											char geobinx2String[25];
											sprintf(geobinx2String, "%2d", x2);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobinx2String;
											createAndOrParseIntoDirectory(&currentDirectory, &string(geobinx2String), true, true);
											#else
											currentDirectory = currentDirectory + geobinx2String;
											#endif

											char geobiny2String[25];
											sprintf(geobiny2String, "%2d", y2);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobiny2String;
											createAndOrParseIntoDirectory(&currentDirectory, &string(geobiny2String), true, true);							
											#else
											currentDirectory = currentDirectory + geobiny2String;
											#endif



											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											for(int i=0; i<currentDirectory.length(); i++)
											{
												currentDirectoryCharStar[*currentDirectoryLength] = currentDirectory[i];
												*currentDirectoryLength = *currentDirectoryLength + 1;
											}
											//cout << "tt currentDirectoryLengthBeforeGeoAndRGB = " << currentDirectoryLengthBeforeGeoAndRGB << endl;	
											//cout << "currentDirectoryLength = " << currentDirectoryLength << endl;															
											//cout << "currentDirectory = " << currentDirectory << endl;	
											//cout << "currentDirectoryCharStar = " << currentDirectoryCharStar << endl;
											#endif

											if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
											{
												addSnapshotIDReferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
											}
											else
											{
												addSnapshotIDReferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);
											}
	
	

							#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_HIGH_REDUNDANCY_DETERMINISTIC_BY_ITERATING_OVER_ALL_ADJACENT_BINS_FAST_RECOG_BUT_USE_MORE_HD

										}
									}
								}
							}
							#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD

									#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2

											if(geoExceptionIndex2 != (OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
											{
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]-geoExceptionModifier2;																						
											}
										}
									}
									#endif	

									if(geoExceptionIndex1 != (OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
									{
										geoxyBins[geoExceptionIndex1] = geoxyBins[geoExceptionIndex1]-geoExceptionModifier1;																						
									}
								}
							}
							#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

								#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
								}
								#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
								}
								#endif
								#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
								}
								#endif															

								geoxyBins[0] = geoxBin[0];
								geoxyBins[1] = geoyBin[0];
								geoxyBins[2] = geoxBin[1];
								geoxyBins[3] = geoyBin[1];


							}	
							#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
							delete firstReferenceInDTBinListGeo;
							#endif														

							#endif
											
						}
					}
				}
				findex2++;
				currentFeatureInTempList2 = currentFeatureInTempList2->next;
			}
		}
		findex1++;
		currentFeatureInTempList = currentFeatureInTempList->next;
	}
		
}

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING
void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopAvgHueDev(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength)
{
	string * currentDirectory = initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;
	
	int rBinMid = (int)firstFeatureInList->avgCol.r;
	int gBinMid = (int)firstFeatureInList->avgCol.g;
	int bBinMid = (int)firstFeatureInList->avgCol.b;


	//cout << "rBinMid = " << rBinMid << endl;
	//cout << "gBinMid = " << gBinMid << endl;
	//cout << "bBinMid = " << bBinMid << endl;


	int rBinMin = rBinMid-1;
	int rBinMax = rBinMid+1;
	int gBinMin = gBinMid-1;
	int gBinMax = gBinMid+1;
	int bBinMin = bBinMid-1;
	int bBinMax = bBinMid+1;

	if(rBinMin < 0)
	{
		rBinMin = 0;
	}
	if(rBinMax > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
	{
		rBinMax = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
	}
	if(gBinMin < 0)
	{
		gBinMin = 0;
	}
	if(gBinMax > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
	{
		gBinMax = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
	}
	if(bBinMin < 0)
	{
		bBinMin = 0;
	}
	if(bBinMax > (OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
	{
		bBinMax = OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
	}

	for(int r = rBinMin; r <= rBinMax; r++)
	{
		for(int g = gBinMin; g <= gBinMax; g++)
		{
			for(int b = bBinMin; b <= bBinMax; b++)
			{

				/*							
				string normalisedAverageHueDevName = OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
				int currentCombinergbBin = r*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + g*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + b;
				char currentCombinergbBinString[25];
				sprintf(currentCombinergbBinString, "%d", currentCombinergbBin);
				*/
				char currentrBinString[25];
				sprintf(currentrBinString, "%d", r);
				char currentgBinString[25];
				sprintf(currentgBinString, "%d", g);
				char currentbBinString[25];
				sprintf(currentbBinString, "%d", b);


				#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				currentDirectory = initialDirectory;
				createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
				#endif

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				string currentDirectory = "";
				#endif
				
				*currentDirectoryLength = currentDirectoryLengthInitial;
				#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				currentDirectory = currentDirectory + "/" + currentrBinString;
				createAndOrParseIntoDirectory(&currentDirectory, &string(currentrBinString), true, true);	
				currentDirectory = currentDirectory + "/" + currentgBinString;
				createAndOrParseIntoDirectory(&currentDirectory, &string(currentgBinString), true, true);	
				currentDirectory = currentDirectory + "/" + currentbBinString;
				createAndOrParseIntoDirectory(&currentDirectory, &string(currentbBinString), true, true);	
				#else
				currentDirectory = currentDirectory + currentrBinString;
				currentDirectory = currentDirectory + currentgBinString;
				currentDirectory = currentDirectory + currentbBinString;									   
				#endif
				//cout << "currentDirectory 2 = " << currentDirectory << endl;

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				for(int i=0; i<currentDirectory.length(); i++)
				{
					currentDirectoryCharStar[*currentDirectoryLength] = currentDirectory[i];
					*currentDirectoryLength = *currentDirectoryLength + 1;
				}	
				#endif	

				addSnapshotIDReferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	

			}
		}
	}

}

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		char dctCoeffArrayBinnedStringTemp[1000];
		int DCTCoeff64BitValueStringLengthTemp = 0;	//not used
		int concatonatedDctCoeffArrayBiasInt[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedStringTemp, &DCTCoeff64BitValueStringLengthTemp, concatonatedDctCoeffArrayBiasInt);		
		#else
		convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(firstFeatureInList->dctCoeff, concatonatedDctCoeffArrayBiasInt);		
		#endif
	#endif	
		
			
#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD
	for(int fourierExceptionIndex1=0; fourierExceptionIndex1<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS+1; fourierExceptionIndex1++)
	{ 
		for(int fourierExceptionModifier1=-1; fourierExceptionModifier1<=1; fourierExceptionModifier1=fourierExceptionModifier1+2)
		{	
			if(fourierExceptionIndex1 != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
			{
				firstFeatureInList->dctCoeff[fourierExceptionIndex1] = firstFeatureInList->dctCoeff[fourierExceptionIndex1]+fourierExceptionModifier1;
			}

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2		
			for(int fourierExceptionIndex2=0; fourierExceptionIndex2<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS+1; fourierExceptionIndex2++)
			{ 		
				for(int fourierExceptionModifier2=-1; fourierExceptionModifier2<=1; fourierExceptionModifier2=fourierExceptionModifier2+2)
				{	
					if(fourierExceptionIndex2 != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
					{
						firstFeatureInList->dctCoeff[fourierExceptionIndex2] = firstFeatureInList->dctCoeff[fourierExceptionIndex2]+fourierExceptionModifier2;
					}
			#endif
#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

	signed char dctCoeffOriginal[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		dctCoeffOriginal[i] = firstFeatureInList->dctCoeff[i];
	}

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
	SnapshotIDReferenceList * firstReferenceInDTBinListFourier = new SnapshotIDReferenceList();
	SnapshotIDReferenceList * currentReferenceInDTBinListFourier = firstReferenceInDTBinListFourier; 
	#endif																				
	int numberOfIntelligentFourierBins = pow(2.0, OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS);
	//int numberOfIntelligentFourierBins = countIncrements(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS);

	//cout << "numberOfIntelligentFourierBins = " << numberOfIntelligentFourierBins << endl;
	//exit(0);

	for(int fourierExceptionIndex1=0; fourierExceptionIndex1<numberOfIntelligentFourierBins; fourierExceptionIndex1++)
	{

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
		bool performAtLeastOneIntelligentGeoBinBias = false;
		bool firstIntelligentBinPermutation = false;
		if(fourierExceptionIndex1 == 0)
		{
			firstIntelligentBinPermutation = true;
		}
		#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
		bool firstIntelligentBinPermutation = false;
		if(fourierExceptionIndex1 == 0)
		{
			firstIntelligentBinPermutation = true;
		}		
		#endif

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
		long dtBinTemp = 0;
		#endif	

		/*
		for(int fourierExceptionIndex2=0; fourierExceptionIndex2<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; fourierExceptionIndex2++)
		{
			if(((fourierExceptionIndex1>>fourierExceptionIndex2)%2) == 0)
			{
				cout << "0";
			}
			else
			{
				cout << "1";
			}
		}
		cout << endl;
		*/

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
		int numberOfBinsThatAreBiased = 0;
		for(int fourierExceptionIndex2=0; fourierExceptionIndex2<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; fourierExceptionIndex2++)
		{
			bool biasIsOnForThisBin = false;
			if(((fourierExceptionIndex1>>fourierExceptionIndex2)%2) == 0)	//test 1/0 of bit x in fourierExceptionIndex1 (NB fourierExceptionIndex1 is 7 bit)
			{//bit is off
				//cout << "bit is off; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;  
				biasIsOnForThisBin = false;
			}
			else
			{//bit is on
				//cout << "bit is on; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;  												
				biasIsOnForThisBin = true;

			}
			if(biasIsOnForThisBin)
			{
				numberOfBinsThatAreBiased++;
			}

		}
		if(numberOfBinsThatAreBiased <= double(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)*OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS_MAX_NUM_BIAS_BINS_RATIO)
		{//ie, numberOfBinsThatAreBiased <= 3
		#endif
		
			int numberOfBinsThatNeedBiasesApplied = 0; 
			for(int fourierExceptionIndex2=0; fourierExceptionIndex2<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; fourierExceptionIndex2++)
			{
				bool biasIsOnForThisBin = false;
				if(((fourierExceptionIndex1>>fourierExceptionIndex2)%2) == 0)	//test 1/0 of bit x in fourierExceptionIndex1 (NB fourierExceptionIndex1 is 7 bit)
				{//bit is off
					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
					performAtLeastOneIntelligentGeoBinBias = true;
					#endif													
					//cout << "bit is off; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;  
					biasIsOnForThisBin = false;
				}
				else
				{//bit is on
					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
					performAtLeastOneIntelligentGeoBinBias = true;
					#endif													
					//cout << "bit is on; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;  												
					biasIsOnForThisBin = true;
				}
				
				
				//This section updated 9 June 2012 to support OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3 
				int bias = 0;
				bool performIntelligentFourierBinBias = false;
				if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG)
				{
					//cout << "here1" << endl;
					performIntelligentFourierBinBias = true;	//not used
					bias = -1*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
					//cout << "bias = " << bias << endl;
				}
				else if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS)
				{
					//cout << "here2" << endl;
					performIntelligentFourierBinBias = true;	//not used
					bias = 1*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
					//cout << "bias = " << bias << endl;
				}
				else if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME)
				{
					//cout << "here3" << endl;
					performIntelligentFourierBinBias = false;	//not used
					//do not add
				}
				if(!biasIsOnForThisBin)
				{//then assume central bin, no change in this fourier bin
					performIntelligentFourierBinBias = false;	//not used
				}
				else
				{//then assume biased bin if necessary, positive or negative				
					if(performIntelligentFourierBinBias)
					{
						numberOfBinsThatNeedBiasesApplied++;
						firstFeatureInList->dctCoeff[fourierExceptionIndex2] = firstFeatureInList->dctCoeff[fourierExceptionIndex2]+bias;
					}
				}
				
				
				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
				#if (OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS < 100)

				int arrayValueSignedTemp = firstFeatureInList->dctCoeff[fourierExceptionIndex2];
				double arrayValueUnsignedDouble;
				unsigned int arrayValueUnsignedTemp = determineDCTBinUnsigned(arrayValueSignedTemp, &arrayValueUnsignedDouble);
					/*original 2010/2011 code; nb how it does not apply "/OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL"...
					if(arrayValueSignedTemp > OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
					{
						arrayValueSignedTemp = OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET; 
					}
					else if(arrayValueSignedTemp < -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET)
					{
						arrayValueSignedTemp = -OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS_SIGNED_OFFSET; 
					}
					unsigned int arrayValueUnsignedTemp = (unsigned int)(determineDCTBinUnsignedDouble(arrayValueSignedTemp));	//convert to unsigned
					*/

				dtBinTemp = dtBinTemp + arrayValueUnsignedTemp*(pow(100, fourierExceptionIndex2));
				/*
				cout << "pow(100, fourierExceptionIndex2) = " << pow(100, fourierExceptionIndex2) << endl;
				cout << "firstFeatureInList->dctCoeff[fourierExceptionIndex2] = " << int(firstFeatureInList->dctCoeff[fourierExceptionIndex2]) << endl;
				cout << "fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;
				cout << "dtBinTempFourierGoing = " << dtBinTemp << endl;
				*/
				#else
				cout << "invalid OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS, must be < 100" << endl;
				exit(0);
				#endif
				#endif												
			}
			
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
			bool placedNewDTBinReference = false;
			bool dtBinAlreadyAddedToList = false;
			currentReferenceInDTBinListFourier = firstReferenceInDTBinListFourier; 
			SnapshotIDReferenceList * previousReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
			bool DTBinListFourierIndex = 0;
			int currentDTBinValue = currentReferenceInDTBinListFourier->referenceID;
			//cout << "dtBinTemp = " << dtBinTemp << endl;
			//exit(0);
			while(!placedNewDTBinReference && !dtBinAlreadyAddedToList)
			{
				if(currentReferenceInDTBinListFourier->referenceID == dtBinTemp)
				{
					dtBinAlreadyAddedToList = true;
				}
				else
				{
					if(currentReferenceInDTBinListFourier->referenceID >= currentDTBinValue)
					{
						SnapshotIDReferenceList * newReferenceInDTBinListFourier = new SnapshotIDReferenceList();
						newReferenceInDTBinListFourier->referenceID = dtBinTemp;														
						//cout << "add bin to list; newReferenceInDTBinListFourier->referenceID = " << newReferenceInDTBinListFourier->referenceID << endl;
						if(DTBinListFourierIndex > 0)
						{
							SnapshotIDReferenceList * tempRef = previousReferenceInDTBinListFourier->next;
							tempRef->previous = newReferenceInDTBinListFourier;
							newReferenceInDTBinListFourier->next = tempRef;
							newReferenceInDTBinListFourier->previous = previousReferenceInDTBinListFourier; 
							previousReferenceInDTBinListFourier->next = newReferenceInDTBinListFourier;
						}
						else
						{
							SnapshotIDReferenceList * tempRef = firstReferenceInDTBinListFourier;
							tempRef->previous = newReferenceInDTBinListFourier;
							newReferenceInDTBinListFourier->next = tempRef;
							firstReferenceInDTBinListFourier = newReferenceInDTBinListFourier;
						}																		
						placedNewDTBinReference = true;
					}
				}

				if(DTBinListFourierIndex == 0)
				{
					previousReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
				}

				DTBinListFourierIndex++;
				currentDTBinValue = currentReferenceInDTBinListFourier->referenceID;
				previousReferenceInDTBinListFourier = currentReferenceInDTBinListFourier;
				currentReferenceInDTBinListFourier = currentReferenceInDTBinListFourier->next;
			}
			#endif	
			
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
			bool performUniqueIntelligentGeoBinBias = true;
			if(numberOfBinsThatNeedBiasesApplied < numberOfBinsThatAreBiased)
			{
				performUniqueIntelligentGeoBinBias = false;
			}
			#endif													

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
			if(!dtBinAlreadyAddedToList)
			{
			#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
			if(performAtLeastOneIntelligentGeoBinBias || firstIntelligentBinPermutation)
			{
			#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
			if(performUniqueIntelligentGeoBinBias || firstIntelligentBinPermutation)
			{																
			#endif											



#endif	


					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
					if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
					{
						*decisionTreeSQLMultipleRowInsertQueryLength = 0;
						insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeStart(OR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);
						
					}
					#endif
							

					//cout << "as9" << endl;

					#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = initialDirectory;
					createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
					#endif
					
					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = "";
					#endif
					*currentDirectoryLength = currentDirectoryLengthInitial;

					
					#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						char dctCoeffArrayBinnedString[1000];
						int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
						int concatonatedSignedDctCoeffArrayBiasIntTemp[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedSignedDctCoeffArrayBiasIntTemp);																			
						#else
						char dctCoeffArrayBinnedString[1000];
						int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
						convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED);					
						#endif

						/*OLD: before 10 June 2012 update
						#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
						cout << "error; OR_IMAGE_COMPARISON_DECISION_TREE_SQL requires OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT to be on" << endl;
						#endif
						*/			
					#else
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						int concatonatedSignedDctCoeffArrayBiasIntTemp[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						firstFeatureInList->dctCoeffArrayBinned = convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(firstFeatureInList->dctCoeff, concatonatedSignedDctCoeffArrayBiasIntTemp);																			
						#else
						firstFeatureInList->dctCoeffArrayBinned = convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(firstFeatureInList->dctCoeff);					
						#endif
											
						char dctCoeffArrayBinnedString[25];
						sprintf(dctCoeffArrayBinnedString, "%ld", firstFeatureInList->dctCoeffArrayBinned);
						
						/*OLD: before 10 June 2012 update
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL	
						cout << "error; no OR_IMAGE_COMPARISON_DECISION_TREE_SQL requires OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT to be off" << endl;					
						#endif
						*/
					#endif
					
					#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = currentDirectory + "/" + dctCoeffArrayBinnedString;
					createAndOrParseIntoDirectory(currentDirectory, &string(dctCoeffArrayBinnedString), false, true);
					#else
					currentDirectory = currentDirectory + dctCoeffArrayBinnedString;
					#endif

					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					for(int i=0; i<currentDirectory.length(); i++)
					{
						currentDirectoryCharStar[*currentDirectoryLength] = (currentDirectory)[i];
						*currentDirectoryLength = *currentDirectoryLength + 1;
					}	
					#endif	
					//cout << "currentDirectoryLength = " << *currentDirectoryLength << endl;															
					//cout << "currentDirectory = " << currentDirectory << endl;	
					//cout << "currentDirectoryCharStar = " << currentDirectoryCharStar << endl;									
					
					

						
				
					//cout << "firstFeatureInList->dctCoeffArrayBinned = " << firstFeatureInList->dctCoeffArrayBinned << endl;

						//cout << "as10" << endl;
						
						if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
						{
							addSnapshotIDReferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
						}
						else if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
						{
							addSnapshotIDReferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
						}
						else if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
						{
							addSnapshotIDReferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
						}
						else
						{
							addSnapshotIDReferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
						}
						
						
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
						if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
						{						
							insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeEnd(decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);
						}
						#endif


#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2
					if(fourierExceptionIndex2 != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
					{
						firstFeatureInList->dctCoeff[fourierExceptionIndex2] = firstFeatureInList->dctCoeff[fourierExceptionIndex2]-fourierExceptionModifier2;
					}
				}

			}
			#endif

			if(fourierExceptionIndex1 != OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
			{
				firstFeatureInList->dctCoeff[fourierExceptionIndex1] = firstFeatureInList->dctCoeff[fourierExceptionIndex1]-fourierExceptionModifier1;																						
			}
		}
	}
#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
		}
		#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
		}
		#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
		}
		#endif
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
		}
		#endif

		for(int i=0; i<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
		{
			firstFeatureInList->dctCoeff[i] = dctCoeffOriginal[i];
		}
	}
#endif	

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
	delete firstReferenceInDTBinListFourier;
	#endif
		
												
													
}

//OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON
void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
					
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	createOrParseSnapshotIDReferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, &currentDirectory, currentDirectoryCharStar, currentDirectoryLength);
	#else
	createOrParseSnapshotIDReferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, true, &currentDirectory, NULL, NULL);
	#endif
	
	if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);	
	}
	else
	{
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);
	}

}

													
void addSnapshotIDReferenceToImageComparisonDecisionTreeLoopFinal(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures,  char * currentDirectoryCharStar, int * currentDirectoryLength, string * initialDirectory, char * decisionTreeultipleRowInsertQueryTextCharStar, long * decisionTreeSQLMultipleRowInsertQueryLength)
{
	string * currentDirectory = initialDirectory;
	
#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
	insertSnapshotIDReferenceIntoSQLDatabaseDecisionTreeIteration(currentDirectoryCharStar, *currentDirectoryLength, snapshotReferenceID, &databaseTableSizeDecisionTree, decisionTreeultipleRowInsertQueryTextCharStar, decisionTreeSQLMultipleRowInsertQueryLength);
	#else
	insertSnapshotIDReferenceIntoSQLDatabaseDecisionTree(OR_MYSQL_TABLE_NAME_DECISIONTREE, currentDirectoryCharStar, *currentDirectoryLength, snapshotReferenceID, &databaseTableSizeDecisionTree);
	#endif
#else

	string snapshotIDReferenceListFileName = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_LIST_FILE_NAME;
	*currentDirectory = *currentDirectory + "/" + snapshotIDReferenceListFileName;
	//cout << "currentDirectory 5 = " << currentDirectory << endl;

	addSnapshotIDReferenceToList(currentDirectory, snapshotReferenceID);

	addSnapshotIDReferenceToImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, &currentDirectory, snapshotReferenceID);
#endif
}												
		
													
void addSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCombinations(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, Feature * firstFeatureInList, long snapshotReferenceID, bool ignoreOTfeatures)
{	
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
	char * decisionTreeultipleRowInsertQueryTextCharStar = new char[100000000];	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length 
	#else
	char * decisionTreeultipleRowInsertQueryTextCharStar = NULL;	//not initiated yet
	#endif
	long decisionTreeSQLMultipleRowInsertQueryLength = 0;		
																			
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		string initialDirectory = "";	//not used
		char currentDirectoryCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
		int currentDirectoryLength = 0;		
	#else
		string initialDirectory = imageComparisonTreeBaseDirectory + imageComparisonTreeName;
		cout << "initialDirectory = " << initialDirectory << endl;
		createAndOrParseIntoDirectory(&initialDirectory, &initialDirectory, true, false);
		initialDirectory = initialDirectory + "/"
	#endif
	string * currentDirectory = &initialDirectory;

	if(OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			decisionTreeSQLMultipleRowInsertQueryLength = 0;	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length 
			insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeStart(OR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);
		
		}
		else
		{
			//cannot initiate at this stage if DCT enabled, or else too many rows in SQL insert statement
		}
		#endif
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopDCT(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);
	
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{						
			insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeEnd(decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);
		}
		#endif	
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);	
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			decisionTreeSQLMultipleRowInsertQueryLength = 0;	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length 
			insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeStart(OR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);
		}
		#endif	
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);	
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{						
			insertAllSnapshotIDReferencesIntoSQLDatabaseDecisionTreeEnd(decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);
		}
		#endif		
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		addSnapshotIDReferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeultipleRowInsertQueryTextCharStar, &decisionTreeSQLMultipleRowInsertQueryLength);	
	}
	else
	{
		cout << "error - no DT conditions defined" << endl;
		exit(0);
	}
	
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
	delete decisionTreeultipleRowInsertQueryTextCharStar; 
	#endif	
											
						
}

void createSnapshotIDReferenceImageComparisonDecisionTreeString(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], char * currentDirectoryCharStar, int * currentDirectoryLength, string * currentDirectory)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	*currentDirectory = "";
	#else
	*currentDirectory = imageComparisonTreeBaseDirectory + imageComparisonTreeName;
	createAndOrParseIntoDirectory(currentDirectory, currentDirectory, false, false);	
	#endif

	*currentDirectoryLength = 0;
	for(int i=0; i<currentDirectory->length(); i++)
	{
		currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
		*currentDirectoryLength = *currentDirectoryLength + 1;
	}
	
		
	if(OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif	
		//cout << "read OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING" << endl;

		#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
			char dctCoeffArrayBinnedString[1000];
			int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
			/*
			for(int z=0; z<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; z++)
			{
				cout << "2 concatonatedSignedDctCoeffArrayRequirement[z] = " << int(concatonatedSignedDctCoeffArrayRequirement[z]) << endl;
			}			
			*/
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];			
			convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(concatonatedSignedDctCoeffArrayRequirement, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedDctCoeffArrayBiasIntNOTUSED);		
			#else
			convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(concatonatedSignedDctCoeffArrayRequirement, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED);
			#endif
			/*
			for(int q=0; q<DCTCoeff64BitValueStringLengthNOTUSED; q++)
			{
				cout << "dctCoeffArrayBinnedString[q] = " << int(dctCoeffArrayBinnedString[q]) << endl;
			} 
			*/
		#else
			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
			int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];			
			unsigned long dctCoeffArrayBinned = convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(concatonatedSignedDctCoeffArrayRequirement, concatonatedDctCoeffArrayBiasIntNOTUSED);
			#else
			unsigned long dctCoeffArrayBinned = convertDCTCoeffConcatonatedArrayToBinnedAllDCTCoeff64BitValue(concatonatedSignedDctCoeffArrayRequirement);			
			#endif
			//cout << "generated dctCoeffArrayBinned = " << dctCoeffArrayBinned << endl;
			char dctCoeffArrayBinnedString[25];
			sprintf(dctCoeffArrayBinnedString, "%ld", dctCoeffArrayBinned);		
		#endif


		#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = *currentDirectory + "/" + dctCoeffArrayBinnedString;
		createAndOrParseIntoDirectory(currentDirectory, &string(dctCoeffArrayBinnedString), false, true);
		#else
		*currentDirectory = *currentDirectory + dctCoeffArrayBinnedString;
		#endif

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		for(int i=0; i<currentDirectory->length(); i++)
		{
			currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
			*currentDirectoryLength = *currentDirectoryLength + 1;
		}	
		#endif		
	}

	//added 10 June 2012 for debugging...
	currentDirectoryCharStar[*currentDirectoryLength] = '\0';
	//cout << "1currentDirectory = " << *currentDirectory << endl;
	//cout << "1currentDirectoryCharStar = " << currentDirectoryCharStar << endl;
	
	if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		createOrParseSnapshotIDReferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, currentDirectory, currentDirectoryCharStar, currentDirectoryLength);
	}


	if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif	

		for(int featureNum=0; featureNum<OR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; featureNum++)
		{
			char geobinxString[25];
			sprintf(geobinxString, "%2d", pBinxRequirement[featureNum]);
			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + geobinxString;
			createAndOrParseIntoDirectory(currentDirectory, &string(geobinxString), false, true);
			#else
			*currentDirectory = *currentDirectory + geobinxString;
			#endif

			char geobinyString[25];
			sprintf(geobinyString, "%2d", pBinyRequirement[featureNum]);
			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + geobinyString;
			createAndOrParseIntoDirectory(currentDirectory, &string(geobinyString), false, true);	
			#else
			*currentDirectory = *currentDirectory + geobinyString;
			#endif						
		}

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		for(int i=0; i<currentDirectory->length(); i++)
		{
			currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
			*currentDirectoryLength = *currentDirectoryLength + 1;
		}	
		#endif

		//cout << "currentDirectory 3 = " << *currentDirectory << endl;
	}

	//added 10 June 2012 for debugging...
	currentDirectoryCharStar[*currentDirectoryLength] = '\0';
	//cout << "2currentDirectory = " << *currentDirectory << endl;
	//cout << "2currentDirectoryCharStar = " << currentDirectoryCharStar << endl;												
							
	if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
	
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif	

		int rBinMid = (int)normalisedAverageHueDeviationRequirement->r;
		int gBinMid = (int)normalisedAverageHueDeviationRequirement->g;
		int bBinMid = (int)normalisedAverageHueDeviationRequirement->b;
		char currentrBinString[25];
		sprintf(currentrBinString, "%d", rBinMid);
		char currentgBinString[25];
		sprintf(currentgBinString, "%d", gBinMid);
		char currentbBinString[25];
		sprintf(currentbBinString, "%d", bBinMid);
		#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = *currentDirectory + "/" + currentrBinString;
		createAndOrParseIntoDirectory(currentDirectory, &string(currentrBinString), false, true);
		*currentDirectory = *currentDirectory + "/" + currentgBinString;
		createAndOrParseIntoDirectory(currentDirectory, &string(currentgBinString), false, true);
		*currentDirectory = *currentDirectory + "/" + currentbBinString;			
		createAndOrParseIntoDirectory(currentDirectory, &string(currentbBinString), false, true);	
		#else
		*currentDirectory = *currentDirectory + currentrBinString;
		*currentDirectory = *currentDirectory + currentgBinString;
		*currentDirectory = *currentDirectory + currentbBinString;		  
		#endif

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		for(int i=0; i<currentDirectory->length(); i++)
		{
			currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
			*currentDirectoryLength = *currentDirectoryLength + 1;
		}	
		#endif	
		//cout << "currentDirectory 4 = " << *currentDirectory << endl;		
	}

}	
	
void createFeatureContainerListUsingUsingGetSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCheck(FeatureContainer * firstFeatureContainerInTestFeatureMatchingTrainBin, bool ignoreOTfeatures, int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], int trainOrTest)
{
	string currentDirectory = "";
	char currentDirectoryCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
	int currentDirectoryLength;
	
	createSnapshotIDReferenceImageComparisonDecisionTreeString(imageWidth, imageHeight, rgbMapSmall, pBinxyValueRequirement, pBinxRequirement, pBinyRequirement, normalisedAverageHueDeviationRequirement, concatonatedSignedDctCoeffArrayRequirement, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);

	createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(firstFeatureContainerInTestFeatureMatchingTrainBin, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, trainOrTest);
												
}


void getSnapshotIDReferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTCheck(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, long pBinxyValueRequirement, int pBinxRequirement[], int pBinyRequirement[], colour * normalisedAverageHueDeviationRequirement, signed char concatonatedSignedDctCoeffArrayRequirement[], SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList, int trainOrTest, string * DTbin)
{
	string currentDirectory = "";
	char currentDirectoryCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
	int currentDirectoryLength;
	
	createSnapshotIDReferenceImageComparisonDecisionTreeString(imageWidth, imageHeight, rgbMapSmall, pBinxyValueRequirement, pBinxRequirement, pBinyRequirement, normalisedAverageHueDeviationRequirement, concatonatedSignedDctCoeffArrayRequirement, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);
		
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		createSnapshotIDReferenceListUsingSQLDatabaseDecisionTreeTableQuery(firstReferenceInSnapshotIDReferenceList, OR_MYSQL_TABLE_NAME_DECISIONTREE, currentDirectoryCharStar, currentDirectoryLength, trainOrTest);
	#else
		getSnapshotIDReferencesInImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, firstReferenceInSnapshotIDReferenceList, &currentDirectory);
	#endif

	//added 10 June 2012 for debugging...
	currentDirectoryCharStar[currentDirectoryLength] = '\0';
	*DTbin = currentDirectoryCharStar;
	//cout << "4currentDirectory = " << currentDirectory << endl;
	//cout << "4currentDirectoryCharStar = " << currentDirectoryCharStar << endl;
												
}







#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
void getSnapshotIDReferencesInImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList, string * currentDirectory)
{
	if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		createOrParseSnapshotIDReferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, currentDirectory, NULL, NULL);
	}
	
	bool parseSuccessful = false;
	//cout << "currentDirectory 5 = " << *currentDirectory << endl;
	
	//parse;
	#ifdef LINUX
	if(access(currentDirectory->c_str(), 0 ) == 0)
	{
		parseSuccessful = true;
	}
	#else
	if((GetFileAttributes(currentDirectory->c_str()) != INVALID_FILE_ATTRIBUTES))  
	{
		parseSuccessful = true;
	}
	#endif						
		
	if(parseSuccessful)
	{
		string snapshotIDReferenceListFileName = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_LIST_FILE_NAME;
		*currentDirectory = *currentDirectory + "/" + snapshotIDReferenceListFileName;
		//cout << "currentDirectory 6 = " << *currentDirectory << endl;
		
		parseSnapshotIDReferenceList(currentDirectory, firstReferenceInSnapshotIDReferenceList);
	}
}
#endif


#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_3_LUMINOSITY_ONLY
void createOrParseSnapshotIDReferenceImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, bool createOrParse, string * currentDirectory, char * currentDirectoryCharStar, int * currDirIndex)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#endif

	double * luminosityMap = new double[imageWidth*imageHeight];		//CHECK THIS; these maps need to be created inside ORmethod.cpp main loop for efficiency
	double * luminosityContrastMap = new double[imageWidth*imageHeight];	//CHECK THIS; these maps need to be created inside ORmethod.cpp main loop for efficiency
	createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMapSmall, luminosityMap);
	createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);
		
	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
	int numTrue = 0;
	#endif
	
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			
			string nextDirectory = "";

			double contrastVal = getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityContrastMap);
			//cout << "contrastVal = " << contrastVal << endl;
			
			bool passContrastThreshold = false;
			int contrastThreshold;

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
			contrastThreshold = EDGE_LUMINOSITY_CONTRAST_THRESHOLD*OR_IMAGE_COMPARISON_DECISION_TREE_CONTRAST_THRESHOLD_FACTOR;	//needs to be dynamic based upon normalisation based upon average rgb values in image
			#else
			contrastThreshold = 0;
			#endif


			if(contrastVal > contrastThreshold)
			{
				nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GT_NODE_NAME;
				passContrastThreshold = true;
				#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
				numTrue++;
				#endif

			}
			else
			{
				nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_NC_NODE_NAME;
				passContrastThreshold = false;
			}

		//cout << "currentDirectory 7 = " << *currentDirectory << endl;

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
			if(index == 8)
			{
				//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
				index = 0;
				currentDirectoryCharStar[*currDirIndex] = binaryConvertedToChar;
				*currDirIndex = *currDirIndex + 1;
				binaryConvertedToChar = 0x00;
			}

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
			if(passContrastThreshold)
			{
				binaryConvertedToChar = binaryConvertedToChar | (0x01 << index);
			}
			else
			{
				binaryConvertedToChar = binaryConvertedToChar | (0x00 << index);
			}
			index++;						
			#endif

			//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;


		#else
			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + nextDirectory;
			createAndOrParseIntoDirectory(currentDirectory, &nextDirectory, createOrParse, true);	
			#else
			*currentDirectory = *currentDirectory + nextDirectory;
			#endif

		#endif

		}
	}
	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
	cout << "numTrue = " << numTrue << endl;
	#endif
	
	delete luminosityMap;
	delete luminosityContrastMap;
	
}

#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_2_ALL_RGB_COMPONENTS
void createOrParseSnapshotIDReferenceImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, bool createOrParse, string * currentDirectory, char * currentDirectoryCharStar, int * currDirIndex)
{
	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
	int numTrue = 0;
	#endif
	
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#endif
	
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			for(int col=0; col<RGB_NUM; col++)
			{
				string nextDirectory = "";

				double contrastVal = calculateContrastLevelWithinKernelRGBComponent(x, y, rgbMapSmall, col, RGB_CONTRAST_BOOLEAN_MAP_GENERATION_KERNEL_WIDTH, RGB_CONTRAST_BOOLEAN_MAP_GENERATION_KERNEL_HEIGHT, imageWidth, imageHeight);

				bool passContrastThreshold = false;
				int contrastThreshold;

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
				contrastThreshold = (EDGE_LUMINOSITY_CONTRAST_THRESHOLD*OR_IMAGE_COMPARISON_DECISION_TREE_CONTRAST_THRESHOLD_FACTOR)/RGB_NUM;	//needs to be dynamic based upon normalisation based upon average rgb values in image
				#else
				contrastThreshold = 0;
				#endif

				if(col == 0)
				{
					if(contrastVal > contrastThreshold)
					{
						nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_GT_NODE_NAME;
						passContrastThreshold = true;
						#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
						numTrue++;
						#endif

					}
					else
					{
						nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_NC_NODE_NAME;
						passContrastThreshold = false;
					}

				}
				else if(col == 1)
				{
					if(contrastVal > contrastThreshold)
					{
						nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_GT_NODE_NAME;
						passContrastThreshold = true;
						#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
						numTrue++;
						#endif
					}
					else
					{
						nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_NC_NODE_NAME;
						passContrastThreshold = false;
					}						
				}
				else if(col == 2)
				{
					if(contrastVal > contrastThreshold)
					{
						nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_GT_NODE_NAME;
						passContrastThreshold = true;
						#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
						numTrue++;
						#endif

					}
					else
					{
						nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_NC_NODE_NAME;
						passContrastThreshold = false;
					}						
				}	



			//cout << "currentDirectory 7 = " << *currentDirectory << endl;

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
				if(index == 8)
				{
					//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
					index = 0;
					currentDirectoryCharStar[*currDirIndex] = binaryConvertedToChar;
					*currDirIndex = *currDirIndex + 1;
					binaryConvertedToChar = 0x00;
				}

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
				if(passContrastThreshold)
				{
					binaryConvertedToChar = binaryConvertedToChar | (0x01 << index);
				}
				else
				{
					binaryConvertedToChar = binaryConvertedToChar | (0x00 << index);
				}
				index++;						
				#endif

				//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;


			#else
				#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				*currentDirectory = *currentDirectory + "/" + nextDirectory;
				createAndOrParseIntoDirectory(currentDirectory, &nextDirectory, createOrParse, true);	
				#else
				*currentDirectory = *currentDirectory + nextDirectory;
				#endif

			#endif

			}
		}
	}
	
	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
	cout << "numTrue = " << numTrue << endl;
	#endif
}

#elif defined OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_1_ALL_RGB_COMPONENTS_WITH_DIRECTION
void createOrParseSnapshotIDReferenceImageComparisonDecisionTree(int imageWidth, int imageHeight, unsigned char * rgbMapSmall, bool createOrParse, string * currentDirectory, char * currentDirectoryCharStar, int * currDirIndex)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP
	double averageLuminosity = calculateAverageLuminosity(imageWidth, imageHeight, rgbMapSmall);
	double averageNormalisedLuminosity = MAX_LUMINOSITY*OR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP_AVERAGED_NORMALISED_LUMINOSITY_FRACTION;
	double contrastThresholdNormalisationFactor = averageLuminosity/averageNormalisedLuminosity;	
	/*
	unsigned char * normalisedRgbMapSmall = new unsigned char[imageWidth*imageHeight*RGB_NUM];
	void normaliseRGBMapBasedOnAverageLuminosity(normalisedRgbMapSmall, imageWidth, imageHeight, rgbMapSmall)
	rgbMapSmall = normalisedRgbMapSmall;
	*/
	#endif
	
	
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#endif
	
	for(int y=0; y<imageHeight-1; y++)
	{
		for(int x=0; x<imageWidth-1; x++)
		{
			colour colCurrent;
			getRGBMapValues(x, y, imageWidth, rgbMapSmall, &colCurrent);
			colour colNext[2];
			getRGBMapValues(x+1, y, imageWidth, rgbMapSmall, &(colNext[0]));
			getRGBMapValues(x, y+1, imageWidth, rgbMapSmall, &(colNext[1]));
				
			for(int comparisonDirection=0; comparisonDirection<2; comparisonDirection++)
			{					
				for(int col=0; col<RGB_NUM; col++)
				{
					string nextDirectory = "";
					
					bool forkDir = false;
					bool passContrastThreshold = false;
					int contrastThreshold;
					
					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP
						contrastThreshold = (OR_IMAGE_COMPARISON_DECISION_TREE_RGB_CONTRAST_THRESHOLD_FRACTION*MAX_RGB_VAL)*contrastThresholdNormalisationFactor;	//WRONG: normalisation should apply to entire image, not individual snapshots.. 
						#else
						contrastThreshold = (OR_IMAGE_COMPARISON_DECISION_TREE_RGB_CONTRAST_THRESHOLD_FRACTION*MAX_RGB_VAL);					
						//contrastThreshold = (LUMINOSITY_CONTRAST_FRACTION_THRESHOLD*MAX_LUMINOSITY)*OR_IMAGE_COMPARISON_DECISION_TREE_CONTRAST_THRESHOLD_FACTOR;	//OLD			
						#endif
					#else
					contrastThreshold = 0;
					#endif
					
					if(col == 0)
					{
						if(colCurrent.r > (colNext[comparisonDirection].r + contrastThreshold))
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_GT_NODE_NAME;
							forkDir = true;
							passContrastThreshold = true;
							
						}
						else if(colCurrent.r <= (colNext[comparisonDirection].r - contrastThreshold))
						{	
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_LT_NODE_NAME;
							forkDir = false;
							passContrastThreshold = true;
							
						}
						else
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_NC_NODE_NAME;
							forkDir = false;
							passContrastThreshold = false;
						}

					}
					else if(col == 1)
					{
						if(colCurrent.g > (colNext[comparisonDirection].g + contrastThreshold))
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_GT_NODE_NAME;
							forkDir = true;
							passContrastThreshold = true;
						}
						else if(colCurrent.g <= (colNext[comparisonDirection].g - contrastThreshold))
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_LT_NODE_NAME;	
							forkDir = false;	
							passContrastThreshold = true;		
						}
						else
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_NC_NODE_NAME;
							forkDir = false;
							passContrastThreshold = false;
						}						
					}
					else if(col == 2)
					{
						if(colCurrent.b > (colNext[comparisonDirection].b + contrastThreshold))
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_GT_NODE_NAME;
							forkDir = true;
							passContrastThreshold = true;
								
						}
						else if(colCurrent.b <= (colNext[comparisonDirection].b - contrastThreshold))
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_LT_NODE_NAME;	
							forkDir = false;
							passContrastThreshold = true;
						}
						else
						{
							nextDirectory = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_NC_NODE_NAME;
							forkDir = false;
							passContrastThreshold = false;
						}						
					}	


							
				//cout << "currentDirectory 7 = " << *currentDirectory << endl;

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
					if(index == 8)
					{
						//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
						index = 0;
						currentDirectoryCharStar[*currDirIndex] = binaryConvertedToChar;
						*currDirIndex = *currDirIndex + 1;
						binaryConvertedToChar = 0x00;
					}

					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
					if(passContrastThreshold)
					{
						binaryConvertedToChar = binaryConvertedToChar | (0x01 << index);
					}
					else
					{
						binaryConvertedToChar = binaryConvertedToChar | (0x00 << index);
					}
					index++;						
					#endif

					if(forkDir)
					{
						binaryConvertedToChar = binaryConvertedToChar | (0x01 << index);
					}
					else
					{
						binaryConvertedToChar = binaryConvertedToChar | (0x00 << index);
					}
					index++;
					//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;


				#else
					#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					*currentDirectory = *currentDirectory + "/" + nextDirectory;
					createAndOrParseIntoDirectory(currentDirectory, &nextDirectory, createOrParse, true);	
					#else
					*currentDirectory = *currentDirectory + nextDirectory;
					#endif

				#endif
						
				}
			}
		}
	}
	
}
#endif

#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
void addSnapshotIDReferenceToList(string * parseFileName, long snapshotID)
{
	ofstream parseFileObject(parseFileName->c_str(), ios::app);		//append to snapshot id reference list
	
	char snapshotIDString[25];
	sprintf(snapshotIDString, "%ld", snapshotID);
	int length = strlen(snapshotIDString);
	
	parseFileObject.write(snapshotIDString, (sizeof(char)*length));
	parseFileObject.write("\n", (sizeof(char)));	//write new line
}


void parseSnapshotIDReferenceList(string * parseFileName, SnapshotIDReferenceList * firstReferenceInSnapshotIDReferenceList)
{	
	SnapshotIDReferenceList * currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDReferenceList;
	
	ifstream parseFileObject(parseFileName->c_str());		//append to snapshot id reference list
	
	if(parseFileObject.rdbuf( )->is_open( ))
	{
		bool readingSnapshotID = true;
		char snapshotIDString[25];
		snapshotIDString[0] = '\0';
		char c;
		int charCount = 0;
		while (parseFileObject.get(c))
		{
			//cout << c;
			charCount++;
			if((readingSnapshotID) && (c == '\n'))
			{
				
				currentReferenceInSnapshotIDReferenceList->referenceID = (long)(atol(snapshotIDString));
				//cout << "currentReferenceInSnapshotIDReferenceList->referenceID = " << currentReferenceInSnapshotIDReferenceList->referenceID << endl;
				
				SnapshotIDReferenceList * newReferenceInSnapshotIDReferenceList = new SnapshotIDReferenceList();
				currentReferenceInSnapshotIDReferenceList->next = newReferenceInSnapshotIDReferenceList;
				currentReferenceInSnapshotIDReferenceList = currentReferenceInSnapshotIDReferenceList->next;
				
				snapshotIDString[0] = '\0';
				
			}
			else if(readingSnapshotID)
			{
				char typeString[2];
				typeString[0] = c;
				typeString[1] = '\0';
				strcat(snapshotIDString, typeString);
			}
		}	
	}	
}
//#endif

#endif

