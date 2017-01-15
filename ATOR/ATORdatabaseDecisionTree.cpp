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
 * File Name: ATORdatabaseDecisionTree.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3j1c 14-January-2017
 *
 *******************************************************************************/


#include "ATORdatabaseDecisionTree.h"

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE

#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
string imageComparisonTreeBaseDirectory;
string imageComparisonTreeName;
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


#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
void ORdatabaseDecisionTreeClass::createAndOrParseIntoDirectory(string* folderNameFullPath, string* folderName, const bool create, const bool relativeOrAbsolute)
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

//#define OR_IMAGE_COMPARISON_GEOMETRIC_COMPARISON_BINNING
void ORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	bool addPermutationsOfTrainFeaturesForGeoBinning;
	int maxNumFeaturePermutations;

	const ORfeature* currentFeatureInTempList = firstFeatureInList;

	//now bin the features;
	int findex1 = 0;

	while(currentFeatureInTempList->next != NULL)
	{
		const ORfeature* currentFeatureInTempList2 = firstFeatureInList;

		if((!ignoreOTfeatures) || (currentFeatureInTempList->OTpointIndex == 0))
		{//perform binning of nearby features only (not OT features)

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
						geoxyBinDoubleTemp[0] = ORdatabaseSQL.determineGeoBinDoubleX(currentFeatureInTempList->pointTransformed.x);
						geoxyBinDoubleTemp[1] = ORdatabaseSQL.determineGeoBinDoubleY(currentFeatureInTempList->pointTransformed.y);
						geoxyBinDoubleTemp[2] = ORdatabaseSQL.determineGeoBinDoubleX(currentFeatureInTempList2->pointTransformed.x);
						geoxyBinDoubleTemp[3] = ORdatabaseSQL.determineGeoBinDoubleY(currentFeatureInTempList2->pointTransformed.y);
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
							#ifdef OR_DEBUG
							/*
							cout << "geoxyBinDoubleTemp[i] = " << geoxyBinDoubleTemp[i] << endl;
							cout << "geoxyBinTemp[i] = " << geoxyBinTemp[i] << endl;
							*/
							#endif

							if(geoxyBinDoubleTemp[i] >= double(geoxyBinTemp[i])+0.5-DOUBLE_MIN_PRECISION+OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD)
							{
								geoxyBinBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS;
							}
							else if(geoxyBinDoubleTemp[i] <= double(geoxyBinTemp[i])+0.5-DOUBLE_MIN_PRECISION-OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_THRESHOLD)
							{
								geoxyBinBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG;
							}
							else
							{
								geoxyBinBiasInt[i] = OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME;
							}
						}
						#endif

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
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_HIGH_REDUNDANCY_DETERMINISTIC_BY_ITERATING_OVER_ALL_ADJACENT_BINS_FAST_RECOG_BUT_USE_MORE_HD

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
							ORsnapshotIDreferenceList* firstReferenceInDTBinListGeo = new ORsnapshotIDreferenceList();
							ORsnapshotIDreferenceList* currentReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
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
										#ifdef OR_DEBUG
										//cout << "bit is off; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;
										#endif
										biasIsOnForThisBin = false;
									}
									else
									{//bit is on
										#ifdef OR_DEBUG
										//cout << "bit is on; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;
										#endif
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

										if(((geoExceptionIndex1>>geoExceptionIndex2)%2) == 0)
										{//bit is off
											#ifdef OR_DEBUG
											//cout << "bit is off; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;
											#endif
											biasIsOnForThisBin = false;
										}
										else
										{//bit is on
											#ifdef OR_DEBUG
											//cout << "bit is on; geoExceptionIndex = " << geoExceptionIndex1 << ", geoExceptionIndex2 = " << geoExceptionIndex2 << endl;
											#endif
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
										if(OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS < 100)
										{
											dtBinTemp = dtBinTemp + geoxyBins[geoExceptionIndex2]*(pow(100, geoExceptionIndex2));

											#ifdef OR_DEBUG
											//cout << "dtBinTemp = " << dtBinTemp << endl;
											//cout << " geoxyBins[geoExceptionIndex2] = " <<  geoxyBins[geoExceptionIndex2] << endl;
											#endif
										}
										else
										}
											cout << "invalid OR_METHOD_GEOMETRIC_COMPARISON_OPTIMISED_FILE_IO_V2_NO_X_BINS, must be < 100" << endl;
											exit(0);
										}
										#endif
										//CHECK THIS; do: apply check; only insert new row into dt if it is unique to the given snapshot ID
									}

									#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
									bool placedNewDTBinReference = false;
									bool dtBinAlreadyAddedToList = false;
									currentReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
									ORsnapshotIDreferenceList* previousReferenceInDTBinListGeo = firstReferenceInDTBinListGeo;
									bool DTBinListGeoIndex = 0;
									int currentDTBinValue = currentReferenceInDTBinListGeo->referenceID;
									#ifdef OR_DEBUG
									//cout << "dtBinTemp = " << dtBinTemp << endl;
									#endif
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
												ORsnapshotIDreferenceList* newReferenceInDTBinListGeo = new ORsnapshotIDreferenceList();
												newReferenceInDTBinListGeo->referenceID = dtBinTemp;
												#ifdef OR_DEBUG
												//cout << "add bin to list; newReferenceInDTBinListGeo->referenceID = " << newReferenceInDTBinListGeo->referenceID << endl;
												#endif
												if(DTBinListGeoIndex > 0)
												{
													ORsnapshotIDreferenceList* tempRef = previousReferenceInDTBinListGeo->next;
													tempRef->previous = newReferenceInDTBinListGeo;
													newReferenceInDTBinListGeo->next = tempRef;
													newReferenceInDTBinListGeo->previous = previousReferenceInDTBinListGeo;
													previousReferenceInDTBinListGeo->next = newReferenceInDTBinListGeo;
												}
												else
												{
													ORsnapshotIDreferenceList* tempRef = firstReferenceInDTBinListGeo;
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
											this->createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
											#endif

											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = "";
											#endif

											*currentDirectoryLength = currentDirectoryLengthInitial;

											char geobinxString[25];
											sprintf(geobinxString, "%2d", x);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobinxString;
											this->createAndOrParseIntoDirectory(&currentDirectory, &string(geobinxString), true, true);
											#else
											currentDirectory = currentDirectory + geobinxString;
											#endif

											char geobinyString[25];
											sprintf(geobinyString, "%2d", y);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobinyString;
											this->createAndOrParseIntoDirectory(&currentDirectory, &string(geobinyString), true, true);
											#else
											currentDirectory = currentDirectory + geobinyString;
											#endif

											char geobinx2String[25];
											sprintf(geobinx2String, "%2d", x2);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobinx2String;
											this->createAndOrParseIntoDirectory(&currentDirectory, &string(geobinx2String), true, true);
											#else
											currentDirectory = currentDirectory + geobinx2String;
											#endif

											char geobiny2String[25];
											sprintf(geobiny2String, "%2d", y2);
											#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											currentDirectory = currentDirectory + "/" + geobiny2String;
											this->createAndOrParseIntoDirectory(&currentDirectory, &string(geobiny2String), true, true);
											#else
											currentDirectory = currentDirectory + geobiny2String;
											#endif

											#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
											for(int i=0; i<currentDirectory.length(); i++)
											{
												currentDirectoryCharStar[*currentDirectoryLength] = currentDirectory[i];
												*currentDirectoryLength = *currentDirectoryLength + 1;
											}
											#ifdef OR_DEBUG
											//cout << "tt currentDirectoryLengthBeforeGeoAndRGB = " << currentDirectoryLengthBeforeGeoAndRGB << endl;
											//cout << "currentDirectoryLength = " << currentDirectoryLength << endl;
											//cout << "currentDirectory = " << currentDirectory << endl;
											//cout << "currentDirectoryCharStar = " << currentDirectoryCharStar << endl;
											#endif
											#endif

											if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
											{
												this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
											}
											else
											{
												this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
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
void ORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string* currentDirectory = initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	int rBinMid = (int)firstFeatureInList->avgCol.r;
	int gBinMid = (int)firstFeatureInList->avgCol.g;
	int bBinMid = (int)firstFeatureInList->avgCol.b;

	#ifdef OR_DEBUG
	//cout << "rBinMid = " << rBinMid << endl;
	//cout << "gBinMid = " << gBinMid << endl;
	//cout << "bBinMid = " << bBinMid << endl;
	#endif

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
				#ifdef OR_DEBUG
				/*
				string normalisedAverageHueDevName = OR_MYSQL_FIELD_NAME_COLOUR_AVERAGE_RGB_BINS;
				int currentCombinergbBin = r*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + g*OR_IMAGE_COMPARISON_AVERAGE_RGB_BINNING_NUM_DISTINCT_VALS_PER_COL + b;
				string currentCombinergbBinString = SHAREDvars.convertIntToString(currentCombinergbBin);
				*/
				#endif

				string currentrBinString = SHAREDvars.convertIntToString(r);
				string currentgBinString = SHAREDvars.convertIntToString(g);
				string currentbBinString = SHAREDvars.convertIntToString(b);

				#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				currentDirectory = initialDirectory;
				this->createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
				#endif

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				string currentDirectory = "";
				#endif

				*currentDirectoryLength = currentDirectoryLengthInitial;
				#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				currentDirectory = currentDirectory + "/" + currentrBinString;
				this->createAndOrParseIntoDirectory(&currentDirectory, &string(currentrBinString), true, true);
				currentDirectory = currentDirectory + "/" + currentgBinString;
				this->createAndOrParseIntoDirectory(&currentDirectory, &string(currentgBinString), true, true);
				currentDirectory = currentDirectory + "/" + currentbBinString;
				this->createAndOrParseIntoDirectory(&currentDirectory, &string(currentbBinString), true, true);
				#else
				currentDirectory = currentDirectory + currentrBinString;
				currentDirectory = currentDirectory + currentgBinString;
				currentDirectory = currentDirectory + currentbBinString;
				#endif
				#ifdef OR_DEBUG
				//cout << "currentDirectory 2 = " << currentDirectory << endl;
				#endif

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
				for(int i=0; i<currentDirectory.length(); i++)
				{
					currentDirectoryCharStar[*currentDirectoryLength] = currentDirectory[i];
					*currentDirectoryLength = *currentDirectoryLength + 1;
				}
				#endif

				this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
			}
		}
	}
}

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING
void ORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;
	int currentDirectoryLengthInitial = *currentDirectoryLength;

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		char dctCoeffArrayBinnedStringTemp[1000];
		int DCTCoeff64BitValueStringLengthTemp = 0;	//not used
		int concatonatedDctCoeffArrayBiasInt[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		//ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedStringTemp, &DCTCoeff64BitValueStringLengthTemp, concatonatedDctCoeffArrayBiasInt);
		#else
		//ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, concatonatedDctCoeffArrayBiasInt);
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
	ORsnapshotIDreferenceList* firstReferenceInDTBinListFourier = new ORsnapshotIDreferenceList();
	ORsnapshotIDreferenceList* currentReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
	#endif
	int numberOfIntelligentFourierBins = pow(2.0, OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS);
	#ifdef OR_DEBUG
	//int numberOfIntelligentFourierBins = countIncrements(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS);
	//cout << "numberOfIntelligentFourierBins = " << numberOfIntelligentFourierBins << endl;
	#endif
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

		#ifdef OR_DEBUG
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
		#endif

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_LIMIT_PERMUTATIONS
		int numberOfBinsThatAreBiased = 0;
		for(int fourierExceptionIndex2=0; fourierExceptionIndex2<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; fourierExceptionIndex2++)
		{
			bool biasIsOnForThisBin = false;
			if(((fourierExceptionIndex1>>fourierExceptionIndex2)%2) == 0)	//test 1/0 of bit x in fourierExceptionIndex1 (NB fourierExceptionIndex1 is 7 bit)
			{//bit is off
				#ifdef OR_DEBUG
				//cout << "bit is off; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;
				#endif
				biasIsOnForThisBin = false;
			}
			else
			{//bit is on
				#ifdef OR_DEBUG
				//cout << "bit is on; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;
				#endif
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
					#ifdef OR_DEBUG
					//cout << "bit is off; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;
					#endif
					biasIsOnForThisBin = false;
				}
				else
				{//bit is on
					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V2
					performAtLeastOneIntelligentGeoBinBias = true;
					#endif
					#ifdef OR_DEBUG
					//cout << "bit is on; fourierExceptionIndex = " << fourierExceptionIndex1 << ", fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;
					#endif
					biasIsOnForThisBin = true;
				}


				//This section updated 9 June 2012 to support OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS_V3
				int bias = 0;
				bool performIntelligentFourierBinBias = false;
				if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_NEG)
				{
					performIntelligentFourierBinBias = true;	//not used
					bias = -1*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
					#ifdef OR_DEBUG
					//cout << "bias = " << bias << endl;
					#endif
				}
				else if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_POS)
				{
					performIntelligentFourierBinBias = true;	//not used
					bias = 1*OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DISTINCT_VALS_PER_COL;
					#ifdef OR_DEBUG
					//cout << "bias = " << bias << endl;
					#endif
				}
				else if(concatonatedDctCoeffArrayBiasInt[fourierExceptionIndex2] == OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_SAME)
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

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
				if(OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS < 100)
				{
					int arrayValueSignedTemp = firstFeatureInList->dctCoeff[fourierExceptionIndex2];
					double arrayValueUnsignedDouble;
					unsigned int arrayValueUnsignedTemp = ORdatabaseSQL.determineDCTBinUnsigned(arrayValueSignedTemp, &arrayValueUnsignedDouble);

					dtBinTemp = dtBinTemp + arrayValueUnsignedTemp*(pow(100, fourierExceptionIndex2));
					#ifdef OR_DEBUG
					//cout << "fourierExceptionIndex2 = " << fourierExceptionIndex2 << endl;
					//cout << "dtBinTemp = " << dtBinTemp << endl;
					#endif
				}
				else
				{
					cout << "invalid OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINS, must be < 100" << endl;
					exit(0);
				}
				#endif
			}

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_DO_NOT_ALLOW_REPEATS
			bool placedNewDTBinReference = false;
			bool dtBinAlreadyAddedToList = false;
			currentReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
			ORsnapshotIDreferenceList* previousReferenceInDTBinListFourier = firstReferenceInDTBinListFourier;
			bool DTBinListFourierIndex = 0;
			int currentDTBinValue = currentReferenceInDTBinListFourier->referenceID;
			#ifdef OR_DEBUG
			//cout << "dtBinTemp = " << dtBinTemp << endl;
			#endif
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
						ORsnapshotIDreferenceList* newReferenceInDTBinListFourier = new ORsnapshotIDreferenceList();
						newReferenceInDTBinListFourier->referenceID = dtBinTemp;
						#ifdef OR_DEBUG
						//cout << "add bin to list; newReferenceInDTBinListFourier->referenceID = " << newReferenceInDTBinListFourier->referenceID << endl;
						#endif
						if(DTBinListFourierIndex > 0)
						{
							ORsnapshotIDreferenceList* tempRef = previousReferenceInDTBinListFourier->next;
							tempRef->previous = newReferenceInDTBinListFourier;
							newReferenceInDTBinListFourier->next = tempRef;
							newReferenceInDTBinListFourier->previous = previousReferenceInDTBinListFourier;
							previousReferenceInDTBinListFourier->next = newReferenceInDTBinListFourier;
						}
						else
						{
							ORsnapshotIDreferenceList* tempRef = firstReferenceInDTBinListFourier;
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
						*decisionTreeSQLmultipleRowInsertQueryLength = 0;
						ORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(OR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);

					}
					#endif

					#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = initialDirectory;
					this->createAndOrParseIntoDirectory(&currentDirectory, &currentDirectory, false, false);
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
						//ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedSignedDctCoeffArrayBiasIntTemp);
						#else
						char dctCoeffArrayBinnedString[1000];
						int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
						//ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED);
						#endif
					#else
						#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
						int concatonatedSignedDctCoeffArrayBiasIntTemp[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
						//firstFeatureInList->dctCoeffArrayBinned = ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff, concatonatedSignedDctCoeffArrayBiasIntTemp);
						#else
						firstFeatureInList->dctCoeffArrayBinned = ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(firstFeatureInList->dctCoeff);
						#endif

						char dctCoeffArrayBinnedString[25];
						sprintf(dctCoeffArrayBinnedString, "%ld", firstFeatureInList->dctCoeffArrayBinned);
					#endif

					#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
					currentDirectory = currentDirectory + "/" + dctCoeffArrayBinnedString;
					this->createAndOrParseIntoDirectory(currentDirectory, &string(dctCoeffArrayBinnedString), false, true);
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

					#ifdef OR_DEBUG
					//cout << "currentDirectoryLength = " <<* currentDirectoryLength << endl;
					//cout << "currentDirectory = " << currentDirectory << endl;
					//cout << "currentDirectoryCharStar = " << currentDirectoryCharStar << endl;
					//cout << "firstFeatureInList->dctCoeffArrayBinned = " << firstFeatureInList->dctCoeffArrayBinned << endl;
					#endif

					if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
					{
						this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					else if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
					{
						this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					else if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
					{
						this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}
					else
					{
						this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
					}


					#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
					if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
					{
						ORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
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

//#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON
void ORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  char* currentDirectoryCharStar, int* currentDirectoryLength, const string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string currentDirectory = *initialDirectory;

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	this->createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, &currentDirectory, currentDirectoryCharStar, currentDirectoryLength);
	#else
	this->createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, true, &currentDirectory, NULL, NULL);
	#endif

	if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else
	{
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, &currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	}
}


void ORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeLoopFinal(const int imageWidth, const int imageHeight, const unsigned char* rgbMapSmall, const ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures,  const char* currentDirectoryCharStar, const int* currentDirectoryLength, string* initialDirectory, char* decisionTreeMultipleRowInsertQueryTextCharStar, long* decisionTreeSQLmultipleRowInsertQueryLength)
{
	string* currentDirectory = initialDirectory;

#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
	ORdatabaseSQL.insertSnapshotIDreferenceIntoSQLdatabaseDecisionTreeIteration(currentDirectoryCharStar,* currentDirectoryLength, snapshotReferenceID, &databaseTableSizeDecisionTree, decisionTreeMultipleRowInsertQueryTextCharStar, decisionTreeSQLmultipleRowInsertQueryLength);
	#else
	ORdatabaseSQL.insertSnapshotIDreferenceIntoSQLdatabaseDecisionTree(OR_MYSQL_TABLE_NAME_DECISIONTREE, currentDirectoryCharStar,* currentDirectoryLength, snapshotReferenceID, &databaseTableSizeDecisionTree);
	#endif
#else

	string snapshotIDReferenceListFileName = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_LIST_FILE_NAME;
	*currentDirectory = *currentDirectory + "/" + snapshotIDReferenceListFileName;

	this->addSnapshotIDReferenceToList(currentDirectory, snapshotReferenceID);

	addSnapshotIDReferenceToImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, &currentDirectory, snapshotReferenceID);
#endif
}


void ORdatabaseDecisionTreeClass::addSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcombinations(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORfeature* firstFeatureInList, const long snapshotReferenceID, const bool ignoreOTfeatures)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
	char* decisionTreeMultipleRowInsertQueryTextCharStar = new char[100000000];	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length
	#else
	char* decisionTreeMultipleRowInsertQueryTextCharStar = NULL;	//not initiated yet
	#endif
	long decisionTreeSQLmultipleRowInsertQueryLength = 0;

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		string initialDirectory = "";	//not used
		char currentDirectoryCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
		int currentDirectoryLength = 0;
	#else
		string initialDirectory = imageComparisonTreeBaseDirectory + imageComparisonTreeName;
		this->createAndOrParseIntoDirectory(&initialDirectory, &initialDirectory, true, false);
		initialDirectory = initialDirectory + "/"
	#endif
	string* currentDirectory = &initialDirectory;

	if(OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
	{
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			decisionTreeSQLmultipleRowInsertQueryLength = 0;	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length
			ORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(OR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		else
		{
			//cannot initiate at this stage if DCT enabled, or else too many rows in SQL insert statement
		}
		#endif
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopDCT(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
		{
			ORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		#endif
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopSmallHueDevMap(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_GEOMETRIC_COMPARISON_BINNING)
	{
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			decisionTreeSQLmultipleRowInsertQueryLength = 0;	//requires 100MB in ram, for ~ 1000,000 sql inserted rows, each <= 100 characters in length
			ORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeStart(OR_MYSQL_TABLE_NAME_DECISIONTREE, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		#endif
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopGeo(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
		if(!OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING)
		{
			ORdatabaseSQL.insertAllSnapshotIDreferencesIntoSQLdatabaseDecisionTreeEnd(decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
		}
		#endif
	}
	else if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		this->addSnapshotIDreferenceToImageComparisonDecisionTreeLoopAvgHueDev(imageWidth, imageHeight, rgbMapSmall, firstFeatureInList, snapshotReferenceID, ignoreOTfeatures, currentDirectoryCharStar, &currentDirectoryLength, currentDirectory, decisionTreeMultipleRowInsertQueryTextCharStar, &decisionTreeSQLmultipleRowInsertQueryLength);
	}
	else
	{
		cout << "error - no DT conditions defined" << endl;
		exit(0);
	}

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SINGLE_INSERT_STATEMENT_OPTIMISATION
	delete decisionTreeMultipleRowInsertQueryTextCharStar;
	#endif


}

void ORdatabaseDecisionTreeClass::createSnapshotIDreferenceImageComparisonDecisionTreeString(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], char* currentDirectoryCharStar, int* currentDirectoryLength, string* currentDirectory)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
	*currentDirectory = "";
	#else
	*currentDirectory = imageComparisonTreeBaseDirectory + imageComparisonTreeName;
	this->createAndOrParseIntoDirectory(currentDirectory, currentDirectory, false, false);
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

	#ifdef OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_BINARY_TO_CHAR_CONVERSION_OPT
		char dctCoeffArrayBinnedString[1000];
		int DCTCoeff64BitValueStringLengthNOTUSED = 0;	//not used
		#ifdef OR_DEBUG
		/*
		for(int z=0; z<OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS; z++)
		{
			cout << "2 concatonatedSignedDctCoeffArrayRequirement[z] = " << int(concatonatedSignedDctCoeffArrayRequirement[z]) << endl;
		}
		*/
		#endif
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		//ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED, concatonatedDctCoeffArrayBiasIntNOTUSED);
		#else
		//ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, dctCoeffArrayBinnedString, &DCTCoeff64BitValueStringLengthNOTUSED);
		#endif
		#ifdef OR_DEBUG
		/*
		for(int q=0; q<DCTCoeff64BitValueStringLengthNOTUSED; q++)
		{
			cout << "dctCoeffArrayBinnedString[q] = " << int(dctCoeffArrayBinnedString[q]) << endl;
		}
		*/
		#endif
	#else
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_DETERMINISTIC_BY_INTELLIGENT_BINNING_FAST_RECOG_AND_USE_LOW_HD
		int concatonatedDctCoeffArrayBiasIntNOTUSED[OR_IMAGE_COMPARISON_PATTERN_RECOGNITION_FOURIER_TRANSFORM_BINNING_NUM_DCT_COEFFICIENT_BINNING_DIMENSIONS_MAX];
		//unsigned long dctCoeffArrayBinned = ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement, concatonatedDctCoeffArrayBiasIntNOTUSED);
		#else
		unsigned long dctCoeffArrayBinned = ORdatabaseSQL.convertDCTcoeffConcatonatedArrayToBinnedAllDCTcoeff64bitValue(concatonatedSignedDctCoeffArrayRequirement);
		#endif
		#ifdef OR_DEBUG
		//cout << "generated dctCoeffArrayBinned = " << dctCoeffArrayBinned << endl;
		#endif
		char dctCoeffArrayBinnedString[25];
		sprintf(dctCoeffArrayBinnedString, "%ld", dctCoeffArrayBinned);
	#endif

		#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = *currentDirectory + "/" + dctCoeffArrayBinnedString;
		this->createAndOrParseIntoDirectory(currentDirectory, &string(dctCoeffArrayBinnedString), false, true);
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

	currentDirectoryCharStar[*currentDirectoryLength] = '\0';

	if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		this->createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, currentDirectory, currentDirectoryCharStar, currentDirectoryLength);
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
			this->createAndOrParseIntoDirectory(currentDirectory, &string(geobinxString), false, true);
			#else
			*currentDirectory = *currentDirectory + geobinxString;
			#endif

			char geobinyString[25];
			sprintf(geobinyString, "%2d", pBinyRequirement[featureNum]);
			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + geobinyString;
			this->createAndOrParseIntoDirectory(currentDirectory, &string(geobinyString), false, true);
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
	}

	currentDirectoryCharStar[*currentDirectoryLength] = '\0';

	if(OR_IMAGE_COMPARISON_DECISION_TREE_AVERAGE_RGB_DEV_BINNING)
	{
		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = "";
		#endif

		int rBinMid = (int)normalisedAverageHueDeviationRequirement->r;
		int gBinMid = (int)normalisedAverageHueDeviationRequirement->g;
		int bBinMid = (int)normalisedAverageHueDeviationRequirement->b;
		string currentrBinString = SHAREDvars.convertIntToString(rBinMid);
		string currentgBinString = SHAREDvars.convertIntToString(gBinMid);
		string currentbBinString = SHAREDvars.convertIntToString(bBinMid);
		#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		*currentDirectory = *currentDirectory + "/" + currentrBinString;
		this->createAndOrParseIntoDirectory(currentDirectory, &string(currentrBinString), false, true);
		*currentDirectory = *currentDirectory + "/" + currentgBinString;
		this->createAndOrParseIntoDirectory(currentDirectory, &string(currentgBinString), false, true);
		*currentDirectory = *currentDirectory + "/" + currentbBinString;
		this->createAndOrParseIntoDirectory(currentDirectory, &string(currentbBinString), false, true);
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
	}
}

void ORdatabaseDecisionTreeClass::createFeatureContainerListUsingUsingGetSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(ORfeatureContainer* firstFeatureContainerInTestFeatureMatchingTrainBin, const bool ignoreOTfeatures, int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], const int trainOrTest)
{
	string currentDirectory = "";
	char currentDirectoryCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
	int currentDirectoryLength;

	this->createSnapshotIDreferenceImageComparisonDecisionTreeString(imageWidth, imageHeight, rgbMapSmall, pBinxyValueRequirement, pBinxRequirement, pBinyRequirement, normalisedAverageHueDeviationRequirement, concatonatedSignedDctCoeffArrayRequirement, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);

	ORdatabaseSQL.createFeatureContainerListUsingSQLDatabaseDecisionTreeTableQuery(firstFeatureContainerInTestFeatureMatchingTrainBin, ignoreOTfeatures, currentDirectoryCharStar, currentDirectoryLength, trainOrTest);
}


void ORdatabaseDecisionTreeClass::getSnapshotIDreferenceToImageComparisonDecisionTreeWithGeoAvgHueDevAndDCTcheck(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const long pBinxyValueRequirement, const int pBinxRequirement[], const int pBinyRequirement[], const colour* normalisedAverageHueDeviationRequirement, const signed char concatonatedSignedDctCoeffArrayRequirement[], ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, const int trainOrTest, string* DTbin)
{
	string currentDirectory = "";
	char currentDirectoryCharStar[OR_IMAGE_COMPARISON_DECISION_TREE_BIN_MAX_LENGTH];
	int currentDirectoryLength;

	this->createSnapshotIDreferenceImageComparisonDecisionTreeString(imageWidth, imageHeight, rgbMapSmall, pBinxyValueRequirement, pBinxRequirement, pBinyRequirement, normalisedAverageHueDeviationRequirement, concatonatedSignedDctCoeffArrayRequirement, currentDirectoryCharStar, &currentDirectoryLength, &currentDirectory);

	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
		ORdatabaseSQL.createSnapshotIDreferenceListUsingSQLdatabaseDecisionTreeTableQuery(firstReferenceInSnapshotIDreferenceList, OR_MYSQL_TABLE_NAME_DECISIONTREE, currentDirectoryCharStar, currentDirectoryLength, trainOrTest);
	#else
		this->getSnapshotIDreferencesInImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, firstReferenceInSnapshotIDreferenceList, &currentDirectory);
	#endif

	currentDirectoryCharStar[currentDirectoryLength] = '\0';
	*DTbin = currentDirectoryCharStar;
}







#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
void ORdatabaseDecisionTreeClass::getSnapshotIDreferencesInImageComparisonDecisionTree(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList, string* currentDirectory)
{
	if(OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON)
	{
		this->createOrParseSnapshotIDreferenceImageComparisonDecisionTree(imageWidth, imageHeight, rgbMapSmall, false, currentDirectory, NULL, NULL);
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
		string snapshotIDReferenceListFileName = OR_IMAGE_COMPARISON_DECISION_TREE_REFERENCE_DEFAULT_LIST_FILE_NAME;
		*currentDirectory = *currentDirectory + "/" + snapshotIDReferenceListFileName;

		this->parseSnapshotIDreferenceList(currentDirectory, firstReferenceInSnapshotIDreferenceList);
	}
}
#endif


#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_APPLY_CONTRAST_THRESHOLD_METHOD_3_LUMINOSITY_ONLY
void ORdatabaseDecisionTreeClass::createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
	int index = 0;		//used to convert binary to char
	char binaryConvertedToChar = 0x00;
	#endif

	double* luminosityMap = new double[imageWidth*imageHeight];		//CHECK THIS; these maps need to be created inside ORmethod.cpp main loop for efficiency
	double* luminosityContrastMap = new double[imageWidth*imageHeight];	//CHECK THIS; these maps need to be created inside ORmethod.cpp main loop for efficiency
	RTpixelMaps.createLuminosityMapFromRGBMap(imageWidth, imageHeight, rgbMapSmall, luminosityMap);
	RTpixelMaps.createContrastMapFromMap(imageWidth, imageHeight, luminosityMap, luminosityContrastMap);

	#ifdef DEBUG_OR_IMAGE_COMPARISON_DECISION_TREE_SMALL_HUE_DEV_MAP_COMPARISON_THRESHOLD
	int numTrue = 0;
	#endif

	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			string nextDirectory = "";

			double contrastVal = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidth, luminosityContrastMap);
			#ifdef OR_DEBUG
			//cout << "contrastVal = " << contrastVal << endl;
			#endif

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

		#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
			if(index == 8)
			{
				#ifdef OR_DEBUG
				//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
				#endif
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

			#ifdef OR_DEBUG
			//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
			#endif
		#else
			#ifndef OR_IMAGE_COMPARISON_DECISION_TREE_SQL
			*currentDirectory = *currentDirectory + "/" + nextDirectory;
			this->createAndOrParseIntoDirectory(currentDirectory, &nextDirectory, createOrParse, true);
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
void ORdatabaseDecisionTreeClass::createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex)
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

			#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
				if(index == 8)
				{
					#ifdef OR_DEBUG
					//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
					#endif
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

				#ifdef OR_DEBUG
				//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
				#endif
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
void ORdatabaseDecisionTreeClass::createOrParseSnapshotIDreferenceImageComparisonDecisionTree(int imageWidth, const int imageHeight, unsigned char* rgbMapSmall, const bool createOrParse, string* currentDirectory, char* currentDirectoryCharStar, int* currDirIndex)
{
	#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP
	double averageLuminosity = calculateAverageLuminosity(imageWidth, imageHeight, rgbMapSmall);
	double averageNormalisedLuminosity = MAX_LUMINOSITY*OR_IMAGE_COMPARISON_DECISION_TREE_NORMALISE_RGB_MAP_AVERAGED_NORMALISED_LUMINOSITY_FRACTION;
	double contrastThresholdNormalisationFactor = averageLuminosity/averageNormalisedLuminosity;
	/*
	unsigned char* normalisedRgbMapSmall = new unsigned char[imageWidth*imageHeight*RGB_NUM];
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

				#ifdef OR_IMAGE_COMPARISON_DECISION_TREE_BINARY_TO_CHAR_CONVERSION_OPT
					if(index == 8)
					{
						#ifdef OR_DEBUG
						//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
						#endif
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
					#ifdef OR_DEBUG
					//cout << "binaryConvertedToChar = " << binaryConvertedToChar << endl;
					#endif

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
void ORdatabaseDecisionTreeClass::addSnapshotIDReferenceToList(const string* parseFileName, const long snapshotID)
{
	ofstream parseFileObject(parseFileName->c_str(), ios::app);		//append to snapshot id reference list

	string snapshotIDString = SHAREDvars.convertLongToString(snapshotID);

	parseFileObject.write(snapshotIDString);
	parseFileObject.write(STRING_NEWLINE);
}


void ORdatabaseDecisionTreeClass::parseSnapshotIDreferenceList(const string* parseFileName, ORsnapshotIDreferenceList* firstReferenceInSnapshotIDreferenceList)
{
	ORsnapshotIDreferenceList* currentReferenceInSnapshotIDReferenceList = firstReferenceInSnapshotIDreferenceList;

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
				#ifdef OR_DEBUG
				//cout << "currentReferenceInSnapshotIDReferenceList->referenceID = " << currentReferenceInSnapshotIDReferenceList->referenceID << endl;
				#endif

				ORsnapshotIDreferenceList* newReferenceInSnapshotIDReferenceList = new ORsnapshotIDreferenceList();
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

