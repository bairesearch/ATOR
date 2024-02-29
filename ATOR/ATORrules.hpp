 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORrules.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2008-2024 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3r1a 29-February-2024
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
