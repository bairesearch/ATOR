 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORdatabaseDecisionTree.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3r1a 29-February-2024
 * /
 *******************************************************************************/


#include "ATORdatabaseDecisionTree.hpp"

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE

#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
string imageComparisonTreeBaseDirectory;
string imageComparisonTreeName;
#endif

#ifdef LINUX
#include <unistd.h>
#endif
/*
this is effectively a preconfigured neural network / decision tree - allowing instantaneous access to similar snapshots for comparison
this should only be executed on small images, where are determinisitic
new method; binary tree - if p1 r > p2 r, if p1 g > p2 g, if p1 b > p2 b, if p2 r > p3 r, etc... [store in local file system, at end of each path, store a list.txt file containing points to ID in mysql DB)
cannot account for strong local variations in coloured lighting (non white)
Might need ext4;
	http://en.wikipedia.org/wiki/Ext3;	Directory contents 	Table, hashed B-tree with dir_index enabled
	http://en.wikipedia.org/wiki/Ext4; 	Directory contents 	Linked list, hashed B-tree
*/


#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
void ATORdatabaseDecisionTreeClass::createAndOrParseIntoDirectory(string* folderNameFullPath, string* folderName, const bool create, const bool relativeOrAbsolute)
{
	if(create)
	{
		if(relativeOrAbsolute)
		{
			SHAREDvars.createDirectory(folderName);
			SHAREDvars.setCurrentDirectory(folderName);
		}
		else
		{
			//create;
			SHAREDvars.createDirectory(folderNameFullPath);
			SHAREDvars.setCurrentDirectory(folderNameFullPath);
		}
	}
	else
	{
		if(relativeOrAbsolute)
		{
			SHAREDvars.setCurrentDirectory(folderName);
		}
		else
		{
			SHAREDvars.setCurrentDirectory(folderNameFullPath);
		}
	}
}
#endif

//#define ATOR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
void ATORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(const int imageWidth, const int imageHeight, const uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	bool addPermutationsOfTrainFeaturesForGeoBinning;
	int maxNumFeaturePermutations;

	const ATORfeature* currentFeatureInTempList = firstFeatureInList;

	//now bin the features;
	int findex1 = 0;

	while(currentFeatureInTempList->next != NULL)
	{
		const ATORfeature* currentFeatureInTempList2 = firstFeatureInList;

		if((!ignoreOTfeatures) || (currentFeatureInTempList->OTpointIndex == 0))
		{//perform binning of nearby features only (not OT features)

			int findex2 = 0;
			while(currentFeatureInTempList2->next != NULL)
			{
				if((!ignoreOTfeatures) || (currentFeatureInTempList2->OTpointIndex == 0))
				{//perform binning of nearby features only (not OT features)

					if(findex1 != findex2)
					{
						int geoxyBinBiasInt[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
						bool insideBin = false;
						int geoxBin[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						int geoyBin[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS];
						int geoxyBinTemp[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
						double geoxyBinDoubleTemp[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
						geoxyBinDoubleTemp[0] = ATORdatabaseDecisionTreeOperations.determineGeoBinDoubleX(currentFeatureInTempList->pointTransformed.x);
						geoxyBinDoubleTemp[1] = ATORdatabaseDecisionTreeOperations.determineGeoBinDoubleY(currentFeatureInTempList->pointTransformed.y);
						geoxyBinDoubleTemp[2] = ATORdatabaseDecisionTreeOperations.determineGeoBinDoubleX(currentFeatureInTempList2->pointTransformed.x);
						geoxyBinDoubleTemp[3] = ATORdatabaseDecisionTreeOperations.determineGeoBinDoubleY(currentFeatureInTempList2->pointTransformed.y);
						geoxBin[0] = geoxyBinDoubleTemp[0];
						geoyBin[0] = geoxyBinDoubleTemp[1];
						geoxBin[1] = geoxyBinDoubleTemp[2];
						geoyBin[1] = geoxyBinDoubleTemp[3];
						geoxyBinTemp[0] = geoxBin[0];
						geoxyBinTemp[1] = geoyBin[0];
						geoxyBinTemp[2] = geoxBin[1];
						geoxyBinTemp[3] = geoyBin[1];

						#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						for(int i=0; i<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2; i++)
						{

							if(geoxyBinDoubleTemp[i] >= double(geoxyBinTemp[i])+0.5-DOUBLE_MIN_PRECISION+ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD)
							{
								geoxyBinBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;
							}
							else if(geoxyBinDoubleTemp[i] <= double(geoxyBinTemp[i])+0.5-DOUBLE_MIN_PRECISION-ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD)
							{
								geoxyBinBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;
							}
							else
							{
								geoxyBinBiasInt[i] = ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
							}
						}
						#endif

						if(geoxBin[0] > 0 )
						{
							if(geoxBin[0] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
							{
								if(geoyBin[0] > 0 )
								{
									if(geoyBin[0] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
									{
										if(geoxBin[1] > 0 )
										{
											if(geoxBin[1] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS-1)
											{
												if(geoyBin[1] > 0 )
												{
													if(geoyBin[1] < ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_Y_BINS-1)
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
						#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_HIGH_REDUNDANCY_DETERMINISTIC_BY_ITERATING_OVER_ALL_ADJACENT_BINS_FAST_RECOG_BUT_USE_MORE_HD

							int xBin = geoxBin[0];
							int x2Bin = geoxBin[1];
							int yBin = geoyBin[0];
							int y2Bin = geoyBin[1];

							int xBinIndex = 0;
							for(int x = xBin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x<=xBin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x++)
							{
								int yBinIndex =0;
								for(int y = yBin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y<=yBin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y++)
								{
									//check current bin and nearest bins
									int xBinIndex2 = 0;
									for(int x2 = x2Bin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2<=x2Bin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_X/2); x2++)
									{
										int y2BinIndex =0;
										for(int y2 = y2Bin-(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2<=y2Bin+(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_BINS_TO_COMPARE_Y/2); y2++)
										{

						#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD

							int geoxyBins[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
							geoxyBins[0] = geoxBin[0];
							geoxyBins[1] = geoyBin[0];
							geoxyBins[2] = geoxBin[1];
							geoxyBins[3] = geoyBin[1];

							for(int geoExceptionIndex1=0; geoExceptionIndex1<(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2)+1; geoExceptionIndex1++)
							{
								for(int geoExceptionModifier1=-1; geoExceptionModifier1<=1; geoExceptionModifier1=geoExceptionModifier1+2)
								{
									if(geoExceptionIndex1 != (ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
									{
										geoxyBins[geoExceptionIndex1] = geoxyBins[geoExceptionIndex1]+geoExceptionModifier1;
									}

									#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2

									for(int geoExceptionIndex2=0; geoExceptionIndex2<(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2)+1; geoExceptionIndex2++)
									{
										for(int geoExceptionModifier2=-1; geoExceptionModifier2<=1; geoExceptionModifier2=geoExceptionModifier2+2)
										{
											if(geoExceptionIndex2 != (ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
											{
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]+geoExceptionModifier2;
											}

									#endif

											int x = geoxyBins[0];
											int y = geoxyBins[1];
											int x2 = geoxyBins[2];
											int y2 = geoxyBins[3];

						#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

							int geoxyBins[ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2];
							geoxyBins[0] = geoxBin[0];
							geoxyBins[1] = geoyBin[0];
							geoxyBins[2] = geoxBin[1];
							geoxyBins[3] = geoyBin[1];

							#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
							ATORsnapshotIDreferenceList* firstReferenceInDTBinListGeo = new ATORsnapshotIDreferenceList();
							ATORsnapshotIDreferenceList* currentReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
							#endif

							//int numberOfIntelligentGeoBins = countIncrements(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2);  //2^(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2);
							int numberOfIntelligentGeoBins = pow(2.0, (ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2));
							for(int geoExceptionIndex1=0; geoExceptionIndex1<numberOfIntelligentGeoBins; geoExceptionIndex1++)
							{
								#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
								int64_t dtBinTemp = 0;
								#endif

								#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
								bool performAtLeastOneIntelligentGeoBinBias = false;
								bool firstIntelligentBinPermutation = false;
								if(geoExceptionIndex1 == 0)
								{
									firstIntelligentBinPermutation = true;
								}
								#endif

								#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
								int numberOfBinsThatAreBiased = 0;
								for(int geoExceptionIndex2=0; geoExceptionIndex2<(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2); geoExceptionIndex2++)
								{
									bool biasIsOnForThisBin = false;
									if(((geoExceptionIndex1>>geoExceptionIndex2)%2) == 0)
									{//bit is off
										biasIsOnForThisBin = false;
									}
									else
									{//bit is on
										biasIsOnForThisBin = true;

									}
									if(biasIsOnForThisBin)
									{
										numberOfBinsThatAreBiased++;
									}
								}
								if(numberOfBinsThatAreBiased <= double(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2)*ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS_MAX_NUM_BIAS_BINS_RATIO)
								{
								#endif
									for(int geoExceptionIndex2=0; geoExceptionIndex2<(ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2); geoExceptionIndex2++)
									{
										bool biasIsOnForThisBin = false;

										if(((geoExceptionIndex1>>geoExceptionIndex2)%2) == 0)
										{//bit is off
											biasIsOnForThisBin = false;
										}
										else
										{//bit is on
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
											if(geoxyBinBiasInt[geoExceptionIndex2] == ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG)
											{
												#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
												performAtLeastOneIntelligentGeoBinBias = true;
												#endif
												performIntelligentGeoBinBias = true;	//not used
												bias = -1;
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]+bias;
											}
											else if(geoxyBinBiasInt[geoExceptionIndex2] == ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS)
											{
												#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
												performAtLeastOneIntelligentGeoBinBias = true;
												#endif
												performIntelligentGeoBinBias = true;	//not used
												bias = 1;
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]+bias;
											}
											else if(geoxyBinBiasInt[geoExceptionIndex2] == ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME)
											{
												performIntelligentGeoBinBias = false;	//not used
												//do not add
											}
										}

										#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
										if(ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS < 100)
										{
											dtBinTemp = dtBinTemp + geoxyBins[geoExceptionIndex2]*(pow(100, geoExceptionIndex2));

										}
										else
										}
											cerr << "invalid ATOR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS, must be < 100" << endl;
											exit(EXIT_ERROR);
										}
										#endif
										//CHECK THIS; do: apply check; only insert new row into dt if it is unique to the given snapshot ID
									}

									#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
									bool placedNewDTBinReference = false;
									bool dtBinAlreadyAddedToList = false;
									currentReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
									ATORsnapshotIDreferenceList* previousReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
									bool DTBinListGeoIndex = 0;
									int currentDTBinValue = currentReferenceInDTBinListGeo->referenceID;
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
												ATORsnapshotIDreferenceList* newReferenceInDTBinListGeo = new ATORsnapshotIDreferenceList();
												newReferenceInDTBinListGeo->referenceID = dtBinTemp;
												if(DTBinListGeoIndex > 0)
												{
													ATORsnapshotIDreferenceList* tempRef = previousReferenceInDTBinListGeo->next;
													tempRef->previous = newReferenceInDTBinListGeo;
													newReferenceInDTBinListGeo->next = tempRef;
													newReferenceInDTBinListGeo->previous = previousReferenceInDTBinListGeo;
													previousReferenceInDTBinListGeo->next = newReferenceInDTBinListGeo;
												}
												else
												{
													ATORsnapshotIDreferenceList* tempRef = firstReferenceInDTBinListGeo;
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


									#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
									if(!dtBinAlreadyAddedToList)
									{
									#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
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
											#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = *initialDirectory;
											createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
											#endif

											#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = "";
											#endif

											*currentDirectoryLength = currentDirectoryLengthInitial;

											char geobinxString[25];
											sprintf(geobinxString, "%2d", x);
											#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											string geobinxStringS = string(geobinxString);
											currentDirectory = currentDirectory + "/" + geobinxString;
											createAndOrParseIntoDirectory(&currentDirectory, &geobinxStringS, true, true);
											#else
											currentDirectory = currentDirectory + geobinxString;
											#endif

											char geobinyString[25];
											sprintf(geobinyString, "%2d", y);
											#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											string geobinyStringS = string(geobinyString);
											currentDirectory = currentDirectory + "/" + geobinyString;
											createAndOrParseIntoDirectory(&currentDirectory, &geobinyStringS, true, true);
											#else
											currentDirectory = currentDirectory + geobinyString;
											#endif

											char geobinx2String[25];
											sprintf(geobinx2String, "%2d", x2);
											#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											string geobinx2StringS = string(geobinx2String);
											currentDirectory = currentDirectory + "/" + geobinx2String;
											createAndOrParseIntoDirectory(&currentDirectory, &geobinx2StringS, true, true);
											#else
											currentDirectory = currentDirectory + geobinx2String;
											#endif

											char geobiny2String[25];
											sprintf(geobiny2String, "%2d", y2);
											#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											string geobiny2StringS = string(geobiny2String);
											currentDirectory = currentDirectory + "/" + geobiny2String;
											createAndOrParseIntoDirectory(&currentDirectory, &geobiny2StringS, true, true);
											#else
											currentDirectory = currentDirectory + geobiny2String;
											#endif

											#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
											for(int i=0; i<currentDirectory.length(); i++)
											{
												currentDirectoryCharStar[*currentDirectoryLength] = currentDirectory[i];
												*currentDirectoryLength = *currentDirectoryLength + 1;
											}
											#endif

											if(ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
											{
												addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
											}
											else
											{
												addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
											}

							#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_HIGH_REDUNDANCY_DETERMINISTIC_BY_ITERATING_OVER_ALL_ADJACENT_BINS_FAST_RECOG_BUT_USE_MORE_HD

										}
									}
								}
							}
							#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD

									#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2

											if(geoExceptionIndex2 != (ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
											{
												geoxyBins[geoExceptionIndex2] = geoxyBins[geoExceptionIndex2]-geoExceptionModifier2;
											}
										}
									}
									#endif

									if(geoExceptionIndex1 != (ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS*2))
									{
										geoxyBins[geoExceptionIndex1] = geoxyBins[geoExceptionIndex1]-geoExceptionModifier1;
									}
								}
							}
							#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

								#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
								}
								#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
								}
								#endif
								#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
								}
								#endif

								geoxyBins[0] = geoxBin[0];
								geoxyBins[1] = geoyBin[0];
								geoxyBins[2] = geoxBin[1];
								geoxyBins[3] = geoyBin[1];


							}
							#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
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

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING
void ATORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(const int imageWidth, const int imageHeight, const uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	int rBinMid = (int)firstFeatureInList->avgCol.r;
	int gBinMid = (int)firstFeatureInList->avgCol.g;
	int bBinMid = (int)firstFeatureInList->avgCol.b;


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
	if(rBinMax > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
	{
		rBinMax = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
	}
	if(gBinMin < 0)
	{
		gBinMin = 0;
	}
	if(gBinMax > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
	{
		gBinMax = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
	}
	if(bBinMin < 0)
	{
		bBinMin = 0;
	}
	if(bBinMax > (ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1))
	{
		bBinMax = ATOR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL-1;
	}

	for(int r = rBinMin; r <= rBinMax; r++)
	{
		for(int g = gBinMin; g <= gBinMax; g++)
		{
			for(int b = bBinMin; b <= bBinMax; b++)
			{

				string currentrBinString = SHAREDvars.convertIntToString(r);
				string currentgBinString = SHAREDvars.convertIntToString(g);
				string currentbBinString = SHAREDvars.convertIntToString(b);

				#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
				currentDirectory = *initialDirectory;
				createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
				#endif

				#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
				string currentDirectory = "";
				#endif

				*currentDirectoryLength = currentDirectoryLengthInitial;
				#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
				currentDirectory = currentDirectory + "/" + currentrBinString;
				createAndOrParseIntoDirectory(&currentDirectory, &currentrBinString, true, true);
				currentDirectory = currentDirectory + "/" + currentgBinString;
				createAndOrParseIntoDirectory(&currentDirectory, &currentgBinString, true, true);
				currentDirectory = currentDirectory + "/" + currentbBinString;
				createAndOrParseIntoDirectory(&currentDirectory, &currentbBinString, true, true);
				#else
				currentDirectory = currentDirectory + currentrBinString;
				currentDirectory = currentDirectory + currentgBinString;
				currentDirectory = currentDirectory + currentbBinString;
				#endif

				#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
				for(int i=0; i<currentDirectory.length(); i++)
				{
					currentDirectoryCharStar[*currentDirectoryLength] = currentDirectory[i];
					*currentDirectoryLength = *currentDirectoryLength + 1;
				}
				#endif

				addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
			}
		}
	}
}

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void ATORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, const int imageHeight, uchar* rgbMapSmall, ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		char dctCoeffArrayBinnedStringTemp[1000];
		int DCTCoeff64BitValueStringLengthTemp = 0;	//not used
		int concatonatedDctCoeffArrayBiasInt[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		//ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedStringTemp, &DCTCoeff64BitValueStringLengthTemp, concatonatedDctCoeffArrayBiasInt);
		#else
		//ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, concatonatedDctCoeffArrayBiasInt);
		#endif
	#endif

#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD
	for(int fourierExceptionIndex1=0; fourierExceptionIndex1<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS+1; fourierExceptionIndex1++)
	{
		for(int fourierExceptionModifier1=-1; fourierExceptionModifier1<=1; fourierExceptionModifier1=fourierExceptionModifier1+2)
		{
			if(fourierExceptionIndex1 != ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
			{
				firstFeatureInList->dctCoeff[fourierExceptionIndex1] = firstFeatureInList->dctCoeff[fourierExceptionIndex1]+fourierExceptionModifier1;
			}

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2
			for(int fourierExceptionIndex2=0; fourierExceptionIndex2<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS+1; fourierExceptionIndex2++)
			{
				for(int fourierExceptionModifier2=-1; fourierExceptionModifier2<=1; fourierExceptionModifier2=fourierExceptionModifier2+2)
				{
					if(fourierExceptionIndex2 != ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
					{
						firstFeatureInList->dctCoeff[fourierExceptionIndex2] = firstFeatureInList->dctCoeff[fourierExceptionIndex2]+fourierExceptionModifier2;
					}
			#endif
#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

	schar dctCoeffOriginal[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
	for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
	{
		dctCoeffOriginal[i] = firstFeatureInList->dctCoeff[i];
	}

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
	ATORsnapshotIDreferenceList* firstReferenceInDTBinListFourier = new ATORsnapshotIDreferenceList();
	ATORsnapshotIDreferenceList* currentReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
	#endif
	int numberOfIntelligentFourierBins = pow(2.0, ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS);
	for(int fourierExceptionIndex1=0; fourierExceptionIndex1<numberOfIntelligentFourierBins; fourierExceptionIndex1++)
	{
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
		bool performAtLeastOneIntelligentGeoBinBias = false;
		bool firstIntelligentBinPermutation = false;
		if(fourierExceptionIndex1 == 0)
		{
			firstIntelligentBinPermutation = true;
		}
		#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
		bool firstIntelligentBinPermutation = false;
		if(fourierExceptionIndex1 == 0)
		{
			firstIntelligentBinPermutation = true;
		}
		#endif

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
		int64_t dtBinTemp = 0;
		#endif


		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
		int numberOfBinsThatAreBiased = 0;
		for(int fourierExceptionIndex2=0; fourierExceptionIndex2<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; fourierExceptionIndex2++)
		{
			bool biasIsOnForThisBin = false;
			if(((fourierExceptionIndex1>>fourierExceptionIndex2)%2) == 0)	//test 1/0 of bit x in fourierExceptionIndex1 (NB fourierExceptionIndex1 is 7 bit)
			{//bit is off
				biasIsOnForThisBin = false;
			}
			else
			{//bit is on
				biasIsOnForThisBin = true;

			}
			if(biasIsOnForThisBin)
			{
				numberOfBinsThatAreBiased++;
			}

		}
		if(numberOfBinsThatAreBiased <= double(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)*ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS_MAX_NUM_BIAS_BINS_RATIO)
		{//ie, numberOfBinsThatAreBiased <= 3
		#endif

			int numberOfBinsThatNeedBiasesApplied = 0;
			for(int fourierExceptionIndex2=0; fourierExceptionIndex2<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; fourierExceptionIndex2++)
			{
				bool biasIsOnForThisBin = false;
				if(((fourierExceptionIndex1>>fourierExceptionIndex2)%2) == 0)	//test 1/0 of bit x in fourierExceptionIndex1 (NB fourierExceptionIndex1 is 7 bit)
				{//bit is off
					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
					performAtLeastOneIntelligentGeoBinBias = true;
					#endif
					biasIsOnForThisBin = false;
				}
				else
				{//bit is on
					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
					performAtLeastOneIntelligentGeoBinBias = true;
					#endif
					biasIsOnForThisBin = true;
				}


				//This section updated 9 June 2012 to support ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
				int bias = 0;
				bool performIntelligentFourierBinBias = false;
				if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG)
				{
					performIntelligentFourierBinBias = true;	//not used
					bias = -1*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
				}
				else if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS)
				{
					performIntelligentFourierBinBias = true;	//not used
					bias = 1*ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
				}
				else if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME)
				{
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

				#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
				if(ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS < 100)
				{
					int arrayValueSignedTemp = firstFeatureInList->dctCoeff[fourierExceptionIndex2];
					double arrayValueUnsignedDouble;
					uint32_t arrayValueUnsignedTemp = ATORdatabaseSQL.determineDCTBinUnsigned(arrayValueSignedTemp, &arrayValueUnsignedDouble);

					dtBinTemp = dtBinTemp + arrayValueUnsignedTemp*(pow(100, fourierExceptionIndex2));
				}
				else
				{
					cerr << "invalid ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS, must be < 100" << endl;
					exit(EXIT_ERROR);
				}
				#endif
			}

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
			bool placedNewDTBinReference = false;
			bool dtBinAlreadyAddedToList = false;
			currentReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
			ATORsnapshotIDreferenceList* previousReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
			bool DTBinListFourierIndex = 0;
			int currentDTBinValue = currentReferenceInDTBinListFourier->referenceID;
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
						ATORsnapshotIDreferenceList* newReferenceInDTBinListFourier = new ATORsnapshotIDreferenceList();
						newReferenceInDTBinListFourier->referenceID = dtBinTemp;
						if(DTBinListFourierIndex > 0)
						{
							ATORsnapshotIDreferenceList* tempRef = previousReferenceInDTBinListFourier->next;
							tempRef->previous = newReferenceInDTBinListFourier;
							newReferenceInDTBinListFourier->next = tempRef;
							newReferenceInDTBinListFourier->previous = previousReferenceInDTBinListFourier;
							previousReferenceInDTBinListFourier->next = newReferenceInDTBinListFourier;
						}
						else
						{
							ATORsnapshotIDreferenceList* tempRef = firstReferenceInDTBinListFourier;
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

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
			bool performUniqueIntelligentGeoBinBias = true;
			if(numberOfBinsThatNeedBiasesApplied < numberOfBinsThatAreBiased)
			{
				performUniqueIntelligentGeoBinBias = false;
			}
			#endif

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
			if(!dtBinAlreadyAddedToList)
			{
			#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
			if(performAtLeastOneIntelligentGeoBinBias || firstIntelligentBinPermutation)
			{
			#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
			if(performUniqueIntelligentGeoBinBias || firstIntelligentBinPermutation)
			{
			#endif
#endif
					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
					if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
					{
						*decisionTreeSQLmultipleRowInsertQueryLength = 0;
						ATORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(ATOR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);

					}
					#endif

					#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = *initialDirectory;
					createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
					#endif

					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = "";
					#endif
					*currentDirectoryLength = currentDirectoryLengthInitial;

					#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
						#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						char dctCoeffArrayBinnedString[1000];
						int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
						int concatonatedSignedDctCoeffArrayBiasIntTemp[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						//ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedSignedDctCoeffArrayBiasIntTemp);
						#else
						char dctCoeffArrayBinnedString[1000];
						int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
						//ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED);
						#endif
					#else
						#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						int concatonatedSignedDctCoeffArrayBiasIntTemp[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						//firstFeatureInList->dctCoeffArrayBinned = ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, concatonatedSignedDctCoeffArrayBiasIntTemp);
						#else
						firstFeatureInList->dctCoeffArrayBinned = ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff);
						#endif

						char dctCoeffArrayBinnedString[25];
						sprintf(dctCoeffArrayBinnedString, "%ld", firstFeatureInList->dctCoeffArrayBinned);
					#endif

					#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = currentDirectory + "/" + dctCoeffArrayBinnedString;
					string dctCoeffArrayBinnedStringS = string(dctCoeffArrayBinnedString);
					createAndOrParseIntoDirectory(&currentDirectory, &dctCoeffArrayBinnedStringS, false, true);
					#else
					currentDirectory = currentDirectory + dctCoeffArrayBinnedString;
					#endif

					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
					for(int i=0; i<currentDirectory.length(); i++)
					{
						currentDirectoryCharStar[*currentDirectoryLength] = (currentDirectory)[i];
						*currentDirectoryLength = *currentDirectoryLength + 1;
					}
					#endif


					if(ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
					{
						addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					else if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
					{
						addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					else if(ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
					{
						addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					else
					{
						addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}


					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
					if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
					{
						ATORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					#endif

#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_ALLOWING_EXCEPTIONS_FAST_BUT_USE_MORE_HD_x2
					if(fourierExceptionIndex2 != ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
					{
						firstFeatureInList->dctCoeff[fourierExceptionIndex2] = firstFeatureInList->dctCoeff[fourierExceptionIndex2]-fourierExceptionModifier2;
					}
				}
			}
			#endif

			if(fourierExceptionIndex1 != ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS)
			{
				firstFeatureInList->dctCoeff[fourierExceptionIndex1] = firstFeatureInList->dctCoeff[fourierExceptionIndex1]-fourierExceptionModifier1;
			}
		}
	}
#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
		}
		#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
		}
		#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
		}
		#endif
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
		}
		#endif

		for(int i=0; i<ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; i++)
		{
			firstFeatureInList->dctCoeff[i] = dctCoeffOriginal[i];
		}
	}
#endif

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
	delete firstReferenceInDTBinListFourier;
	#endif
}

//#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON
void ATORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
	createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, &currentDirectory, currentDirectoryCharStar, currentDirectoryLength);
	#else
	createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, true, &currentDirectory, NULL, NULL);
	#endif

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else if(ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else
	{
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	}
}


void ATORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(const int imageWidth, const int imageHeight, const uchar* rgbMapSmall, const ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures,  const char* currentDirectoryCharStar, const int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, int64_t* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string* currentDirectory = initialDirectory;

#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
	ATORdatabaseSQL.insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(currentDirectoryCharStar,* currentDirectoryLength, snapshotReferenceID, &databaseTableSizeDecisionTree, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	#else
	ATORdatabaseSQL.insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(ATOR_MYSQL_TABLE_NAME_DECISIONTREE, currentDirectoryCharStar,* currentDirectoryLength, snapshotReferenceID, &databaseTableSizeDecisionTree);
	#endif
#else

	string snapshotIDReferenceListFileName = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_LIST_FILE_NAME;
	*currentDirectory = *currentDirectory + "/" + snapshotIDReferenceListFileName;

	addSnapshotIDReferenceToList(currentDirectory, snapshotReferenceID);

	//addSnapshotIDReferenceToImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, &currentDirectory, snapshotReferenceID);	//removed 29 Feb 2024
#endif
}


void ATORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(int imageWidth, const int imageHeight, uchar* rgbMapSmall, ATORfeature* firstFeatureInList, const int64_t snapshotReferenceID, const bool ignoreOTfeatures)
{
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
	char* decisionTreeMultipleRowInsertQueryTextCharStar = new char[100000000];	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length
	#else
	char* decisionTreeMultipleRowInsertQueryTextCharStar = NULL;	//not initiated yet
	#endif
	int64_t decisionTreeSQLmultipleRowInsertQueryLength = 0;

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		string initialDirectory = "";	//not used
		char currentDirectoryCharStar[ATOR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
		int currentDirectoryLength = 0;
	#else
		string initialDirectory = imageComparisonTreeBaseDirectory + imageComparisonTreeName;
		createAndOrParseIntoDirectory(&initialDirectory, &initialDirectory, true, false);
		initialDirectory = initialDirectory + "/";
		char currentDirectoryCharStar[ATOR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];	//NOT USED
		int currentDirectoryLength = 0;	//NOT USED
	#endif
	string* currentDirectory = &initialDirectory;

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			decisionTreeSQLmultipleRowInsertQueryLength = 0;	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length
			ATORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(ATOR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		else
		{
			//cannot initiate at this stage if DCT enabled, or else too many rows in SQL insert statement
		}
		#endif
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			ATORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		#endif
	}
	else if(ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			decisionTreeSQLmultipleRowInsertQueryLength = 0;	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length
			ATORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(ATOR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		#endif
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			ATORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		#endif
	}
	else if(ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else
	{
		cerr << "error - no DT conditions defined" << endl;
		exit(EXIT_ERROR);
	}

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_SINGLE_INSERT_STATEMENT_OPTIMISATION
	delete decisionTreeMultipleRowInsertQueryTextCharStar;
	#endif


}

void ATORdatabaseDecisionTreeClass::createSnapshotIDreferenceImageComparisonDecisionTreeString(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], char* currentDirectoryCharStar, int* currentDirectoryLength, string* currentDirectory)
{
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
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


	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif

	#ifdef ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		char dctCoeffArrayBinnedString[1000];
		int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int concatonatedDctCoeffArrayBiasIntNOTUSED[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		//ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedDctCoeffArrayBiasIntNOTUSED);
		#else
		//ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED);
		#endif
	#else
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int concatonatedDctCoeffArrayBiasIntNOTUSED[ATOR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		//uint64_t dctCoeffArrayBinned = ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, concatonatedDctCoeffArrayBiasIntNOTUSED);
		#else
		uint64_t dctCoeffArrayBinned = ATORdatabaseDecisionTreeOperations.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement);
		#endif
		char dctCoeffArrayBinnedString[25];
		sprintf(dctCoeffArrayBinnedString, "%ld", dctCoeffArrayBinned);
	#endif

		#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = *currentDirectory + "/" + dctCoeffArrayBinnedString;
		string dctCoeffArrayBinnedStringS = dctCoeffArrayBinnedString;
		createAndOrParseIntoDirectory(currentDirectory, &dctCoeffArrayBinnedStringS, false, true);
		#else
		*currentDirectory = *currentDirectory + dctCoeffArrayBinnedString;
		#endif

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		for(int i=0; i<currentDirectory->length(); i++)
		{
			currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
			*currentDirectoryLength = *currentDirectoryLength + 1;
		}
		#endif
	}

	currentDirectoryCharStar[*currentDirectoryLength] = '\0';

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, currentDirectory, currentDirectoryCharStar, currentDirectoryLength);
	}

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif

		for(int featureNum=0; featureNum<ATOR_IMAGE_COMPARISON_SQL_GEOMETRIC_COMPARISON_BINNING_NUM_GEO_BINNING_DIMENSIONS; featureNum++)
		{
			char geobinxString[25];
			sprintf(geobinxString, "%2d", pBinxRequirement[featureNum]);
			#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + geobinxString;
			string geobinxStringS = string(geobinxString);
			createAndOrParseIntoDirectory(currentDirectory, &geobinxStringS, false, true);
			#else
			*currentDirectory = *currentDirectory + geobinxString;
			#endif

			char geobinyString[25];
			sprintf(geobinyString, "%2d", pBinyRequirement[featureNum]);
			#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + geobinyString;
			string geobinyStringS = string(geobinyString);
			createAndOrParseIntoDirectory(currentDirectory, &geobinyStringS, false, true);
			#else
			*currentDirectory = *currentDirectory + geobinyString;
			#endif
		}

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		for(int i=0; i<currentDirectory->length(); i++)
		{
			currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
			*currentDirectoryLength = *currentDirectoryLength + 1;
		}
		#endif
	}

	currentDirectoryCharStar[*currentDirectoryLength] = '\0';

	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif

		int rBinMid = (int)normalisedAverageHueDeviationRequirement->r;
		int gBinMid = (int)normalisedAverageHueDeviationRequirement->g;
		int bBinMid = (int)normalisedAverageHueDeviationRequirement->b;
		string currentrBinString = SHAREDvars.convertIntToString(rBinMid);
		string currentgBinString = SHAREDvars.convertIntToString(gBinMid);
		string currentbBinString = SHAREDvars.convertIntToString(bBinMid);
		#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = *currentDirectory + "/" + currentrBinString;
		createAndOrParseIntoDirectory(currentDirectory, &currentrBinString, false, true);
		*currentDirectory = *currentDirectory + "/" + currentgBinString;
		createAndOrParseIntoDirectory(currentDirectory, &currentgBinString, false, true);
		*currentDirectory = *currentDirectory + "/" + currentbBinString;
		createAndOrParseIntoDirectory(currentDirectory, &currentbBinString, false, true);
		#else
		*currentDirectory = *currentDirectory + currentrBinString;
		*currentDirectory = *currentDirectory + currentgBinString;
		*currentDirectory = *currentDirectory + currentbBinString;
		#endif

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		for(int i=0; i<currentDirectory->length(); i++)
		{
			currentDirectoryCharStar[*currentDirectoryLength] = (*currentDirectory)[i];
			*currentDirectoryLength = *currentDirectoryLength + 1;
		}
		#endif
	}
}

#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL_DIRECT_ACCESS
void ATORdatabaseDecisionTreeClass::createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(ATORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, int imageWidth, const int imageHeight, uchar* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], const int trainOrTest)
{
	string currentDirectory = "";
	char currentDirectoryCharStar[ATOR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
	int currentDirectoryLength;

	createSnapshotIDreferenceImageComparisonDecisionTreeString(imageWidth, imageHeight, rgbMapSmall, pBinxyValueRequirement, pBinxRequirement, pBinyRequirement, normalisedAverageHueDeviationRequirement, concatonatedSignedDctCoeffArrayRequirement, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);

	ATORdatabaseSQL.createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(firstFeatureContainerInTestFeatureMatchingTrainBin, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, trainOrTest);
}
#endif

void ATORdatabaseDecisionTreeClass::getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const int64_t pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const schar concatonatedSignedDctCoeffArrayRequirement[], ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const int trainOrTest, string* DTbin)
{
	string currentDirectory = "";
	char currentDirectoryCharStar[ATOR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
	int currentDirectoryLength;

	createSnapshotIDreferenceImageComparisonDecisionTreeString(imageWidth, imageHeight, rgbMapSmall, pBinxyValueRequirement, pBinxRequirement, pBinyRequirement, normalisedAverageHueDeviationRequirement, concatonatedSignedDctCoeffArrayRequirement, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
		ATORdatabaseSQL.createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(firstReferenceInSnapshotIDreferenceList, ATOR_MYSQL_TABLE_NAME_DECISIONTREE, currentDirectoryCharStar, currentDirectoryLength, trainOrTest);
	#else
		getSnapshotIDreferencesInImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, firstReferenceInSnapshotIDreferenceList, &currentDirectory);
	#endif

	currentDirectoryCharStar[currentDirectoryLength] = '\0';
	*DTbin = currentDirectoryCharStar;
}







#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
void ATORdatabaseDecisionTreeClass::getSnapshotIDreferencesInImageComparisonDecisionTree(int imageWidth, const int imageHeight, uchar* rgbMapSmall, ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, string* currentDirectory)
{
	if(ATOR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, currentDirectory, NULL, NULL);
	}

	bool parseSuccessful = false;

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
		string snapshotIDReferenceListFileName = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_LIST_FILE_NAME;
		*currentDirectory = *currentDirectory + "/" + snapshotIDReferenceListFileName;

		parseSnapshotIDreferenceList(currentDirectory, firstReferenceInSnapshotIDreferenceList);
	}
}
#endif


#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_3_LUMINOSITY_ONLY
void ATORdatabaseDecisionTreeClass::createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex)
{
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#endif

	double* luminosityMap = new double[imageWidth*imageHeight];		//CHECK THIS; these maps need to be created inside ATORmethod.cpp main loop for efficiency
	double* luminosityContrastMap = new double[imageWidth*imageHeight];	//CHECK THIS; these maps need to be created inside ATORmethod.cpp main loop for efficiency
	RTpixelMaps.createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMapSmall, luminosityMap);
	RTpixelMaps.createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);


	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			string nextDirectory = "";

			double contrastVal = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityContrastMap);

			bool passContrastThreshold = false;
			int contrastThreshold;

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
			contrastThreshold = EDGE_LUMINOSITY_CONTRAST_THRESHOLD*ATOR_IMAGE_COMPARISON_DECISION_TREE_CONTRAST_THRESHOLD_FACTOR;	//needs to be dynamic based upon normalisation based upon average rgb values in image
			#else
			contrastThreshold = 0;
			#endif

			if(contrastVal > contrastThreshold)
			{
				nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GT_NODE_NAME;
				passContrastThreshold = true;

			}
			else
			{
				nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_NC_NODE_NAME;
				passContrastThreshold = false;
			}

		#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
			if(index == 8)
			{
				index = 0;
				currentDirectoryCharStar[*currDirIndex] = binaryConvertedToChar;
				*currDirIndex = *currDirIndex + 1;
				binaryConvertedToChar = 0x00;
			}

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
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

		#else
			#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + nextDirectory;
			createAndOrParseIntoDirectory(currentDirectory, &nextDirectory, createOrParse, true);
			#else
			*currentDirectory = *currentDirectory + nextDirectory;
			#endif
		#endif

		}
	}

	delete luminosityMap;
	delete luminosityContrastMap;

}

#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_2_ALL_RGB_COMPONENTS
void ATORdatabaseDecisionTreeClass::createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex)
{

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
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

				#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
				contrastThreshold = (EDGE_LUMINOSITY_CONTRAST_THRESHOLD*ATOR_IMAGE_COMPARISON_DECISION_TREE_CONTRAST_THRESHOLD_FACTOR)/RGB_NUM;	//needs to be dynamic based upon normalisation based upon average rgb values in image
				#else
				contrastThreshold = 0;
				#endif

				if(col == 0)
				{
					if(contrastVal > contrastThreshold)
					{
						nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_GT_NODE_NAME;
						passContrastThreshold = true;
					}
					else
					{
						nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_NC_NODE_NAME;
						passContrastThreshold = false;
					}

				}
				else if(col == 1)
				{
					if(contrastVal > contrastThreshold)
					{
						nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_GT_NODE_NAME;
						passContrastThreshold = true;
					}
					else
					{
						nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_NC_NODE_NAME;
						passContrastThreshold = false;
					}
				}
				else if(col == 2)
				{
					if(contrastVal > contrastThreshold)
					{
						nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_GT_NODE_NAME;
						passContrastThreshold = true;

					}
					else
					{
						nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_NC_NODE_NAME;
						passContrastThreshold = false;
					}
				}

			#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
				if(index == 8)
				{
					index = 0;
					currentDirectoryCharStar[*currDirIndex] = binaryConvertedToChar;
					*currDirIndex = *currDirIndex + 1;
					binaryConvertedToChar = 0x00;
				}

				#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
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

			#else
				#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
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

#elif defined ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_1_ALL_RGB_COMPONENTS_WITH_DIRECTION
void ATORdatabaseDecisionTreeClass::createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, uchar* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex)
{
	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP
	double averageLuminosity = calculateAverageLuminosity(imageWidth, imageHeight, rgbMapSmall);
	double averageNormalisedLuminosity = MAX_LUMINOSITY*ATOR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP_AVERAGED_NORMALISED_LUMINOSITY_FRACTION;
	double contrastThresholdNormalisationFactor = averageLuminosity/averageNormalisedLuminosity;
	/*
	uchar* normalisedRgbMapSmall = new uchar[imageWidth*imageHeight*RGB_NUM];
	void normaliseRGBMapBasedOnAverageLuminosity(normalisedRgbMapSmall, imageWidth, imageHeight, rgbMapSmall)
	rgbMapSmall = normalisedRgbMapSmall;
	*/
	#endif

	#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#endif

	for(int y=0; y<imageHeight-1; y++)
	{
		for(int x=0; x<imageWidth-1; x++)
		{
			colour colCurrent;
			RTpixelMaps.getRGBMapValues(x, y, imageWidth, rgbMapSmall, &colCurrent);
			colour colNext[2];
			RTpixelMaps.getRGBMapValues(x+1, y, imageWidth, rgbMapSmall, &(colNext[0]));
			RTpixelMaps.getRGBMapValues(x, y+1, imageWidth, rgbMapSmall, &(colNext[1]));

			for(int comparisonDirection=0; comparisonDirection<2; comparisonDirection++)
			{
				for(int col=0; col<RGB_NUM; col++)
				{
					string nextDirectory = "";

					bool forkDir = false;
					bool passContrastThreshold = false;
					int contrastThreshold;

					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP
					contrastThreshold = (ATOR_IMAGE_COMPARISON_DECISION_TREE_RGB_CONTRAST_THRESHOLD_FRACTION*MAX_RGB_VAL)*contrastThresholdNormalisationFactor;	//WRONG: normalisation should apply to entire image, not individual snapshots..
					#else
					contrastThreshold = (ATOR_IMAGE_COMPARISON_DECISION_TREE_RGB_CONTRAST_THRESHOLD_FRACTION*MAX_RGB_VAL);
					//contrastThreshold = (LUMINOSITY_CONTRAST_FRACTION_THRESHOLD*MAX_LUMINOSITY)*ATOR_IMAGE_COMPARISON_DECISION_TREE_CONTRAST_THRESHOLD_FACTOR;	//OLD
					#endif
					#else
					contrastThreshold = 0;
					#endif

					if(col == 0)
					{
						if(colCurrent.r > (colNext[comparisonDirection].r + contrastThreshold))
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_GT_NODE_NAME;
							forkDir = true;
							passContrastThreshold = true;

						}
						else if(colCurrent.r <= (colNext[comparisonDirection].r - contrastThreshold))
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_LT_NODE_NAME;
							forkDir = false;
							passContrastThreshold = true;

						}
						else
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_RED_NC_NODE_NAME;
							forkDir = false;
							passContrastThreshold = false;
						}
					}
					else if(col == 1)
					{
						if(colCurrent.g > (colNext[comparisonDirection].g + contrastThreshold))
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_GT_NODE_NAME;
							forkDir = true;
							passContrastThreshold = true;
						}
						else if(colCurrent.g <= (colNext[comparisonDirection].g - contrastThreshold))
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_LT_NODE_NAME;
							forkDir = false;
							passContrastThreshold = true;
						}
						else
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_GREEN_NC_NODE_NAME;
							forkDir = false;
							passContrastThreshold = false;
						}
					}
					else if(col == 2)
					{
						if(colCurrent.b > (colNext[comparisonDirection].b + contrastThreshold))
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_GT_NODE_NAME;
							forkDir = true;
							passContrastThreshold = true;

						}
						else if(colCurrent.b <= (colNext[comparisonDirection].b - contrastThreshold))
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_LT_NODE_NAME;
							forkDir = false;
							passContrastThreshold = true;
						}
						else
						{
							nextDirectory = ATOR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_BLUE_NC_NODE_NAME;
							forkDir = false;
							passContrastThreshold = false;
						}
					}

				#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
					if(index == 8)
					{
						index = 0;
						currentDirectoryCharStar[*currDirIndex] = binaryConvertedToChar;
						*currDirIndex = *currDirIndex + 1;
						binaryConvertedToChar = 0x00;
					}

					#ifdef ATOR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD
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

				#else
					#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
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

#ifndef ATOR_IMAGE_COMPARISON_DECISION_TREE_SQL
void ATORdatabaseDecisionTreeClass::addSnapshotIDReferenceToList(const string* parseFileName, const int64_t snapshotID)
{
	ofstream parseFileObject(parseFileName->c_str(), ios::app);		//append to snapshot id reference list

	string snapshotIDString = SHAREDvars.convertLongToString(snapshotID);

	parseFileObject << snapshotIDString;
	parseFileObject << STRING_NEWLINE;
}


void ATORdatabaseDecisionTreeClass::parseSnapshotIDreferenceList(const string* parseFileName, ATORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList)
{
	ATORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDreferenceList;

	ifstream parseFileObject(parseFileName->c_str());		//append to snapshot id reference list

	if(parseFileObject.rdbuf()->is_open())
	{
		bool readingSnapshotID = true;
		string snapshotIDString = "";
		char c;
		int charCount = 0;
		while (parseFileObject.get(c))
		{
			charCount++;
			if((readingSnapshotID) && (c == '\n'))
			{
				currentReferenceInSnapshotIDReferenceList->referenceID = SHAREDvars.convertStringToLong(snapshotIDString);

				ATORsnapshotIDreferenceList* newReferenceInSnapshotIDReferenceList = new ATORsnapshotIDreferenceList();
				currentReferenceInSnapshotIDReferenceList->next = newReferenceInSnapshotIDReferenceList;
				currentReferenceInSnapshotIDReferenceList = currentReferenceInSnapshotIDReferenceList->next;

				snapshotIDString = "";
			}
			else if(readingSnapshotID)
			{
				snapshotIDString = snapshotIDString + c;
			}
		}
	}
}
//#endif

#endif

