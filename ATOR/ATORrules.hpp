 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORrules.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_RULES
#define HEADER_ATOR_RULES

#include "ATORglobalDefs.hpp"
#include "XMLrulesClass.hpp"
#include "RTglobalDefs.hpp"
#include "SHAREDvars.hpp"

class ATORrulesClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void fillInATORrulesExternVariables();
	private: void printATORrulesExternVariables();
};

#endif
