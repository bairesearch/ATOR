/*******************************************************************************
 *
 * File Name: ORrules.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3a7c 10-June-2012
 *
 *******************************************************************************/
 
 #ifndef HEADER_OR_RULES
#define HEADER_OR_RULES

#include "ORglobalDefs.h"
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
