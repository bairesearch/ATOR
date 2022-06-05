 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORmain.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition)
 * Project Version: 3q1a 05-June-2022
 * /
 *******************************************************************************/


#ifndef HEADER_ATOR_MAIN
#define HEADER_ATOR_MAIN


#include "ATORmethod.hpp"
#include "ATORglobalDefs.hpp"
#include "XMLrulesClass.hpp"
#include "ATORrules.hpp"
#include "ATORdatabaseFileIO.hpp"

int main(const int argc,const char* *argv);

class ATORmainClass
{
	public: void printORcommandLineErrorMessage();
};

#endif
