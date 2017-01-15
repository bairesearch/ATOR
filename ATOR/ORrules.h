/*******************************************************************************
 *
 * File Name: ORrules.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3a6b 30-Apr-2012
 *
 *******************************************************************************/
 
 #ifndef HEADER_OR_RULES
#define HEADER_OR_RULES

#include "SHAREDglobalDefs.h"
#include "XMLrulesClass.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

bool convertStringToBool(string stringValue);

void fillInORRulesExternVariables();
void printORRulesExternVariables();

#endif
