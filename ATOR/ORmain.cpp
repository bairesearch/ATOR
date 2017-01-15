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
 * File Name: ORmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3d4a 8-June-2014
 *
 *******************************************************************************/

#include <ctime>
#define TM_STRUCT_YEAR_OFFSET 1900

#include "ORmain.h"
#include "ORmethod.h"
#include "ORglobalDefs.h"
#include "XMLrulesClass.h"
#include "ORrules.h"
#ifdef OR_USE_DATABASE
#include "ORdatabaseFileIO.h"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;


static char errmessage[] = "Usage:  OpenOR.exe [options]\n\n\twhere options are any of the following\n"
"\n\t-trainortest [int]     : 0; input data is compared with current trained database, 1; data is added to database, 2; data is both added and compared"
"\n\t-object [string]       : input object name (def: object)"
"\n\t-imageext [string]     : input image extension name (def: .png) [all standard image formats]"
"\n\t-width [int]           : image width in pixels (def: 768)"
"\n\t-height [int]          : image height in pixels (def: 576)"
"\n\t-od3                   : use 3DOD instead of 2DOD"
"\n\t-sqlipaddress [string] : ip address of MySQL Server (def: localhost)"
"\n\t-sqlusername [string]  : username of MySQL Server BAI database"
"\n\t-sqlpassword [string]  : password of MySQL Server BAI database"
"\n\t-cleartrain            : clear train database"
"\n\t-workingfolder [string]: working directory name for input files (def: same as exe)"
"\n\t-exefolder [string]    : exe directory name for executables OpenOR.exe and FD.exe (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n\n single view options only \n"
"\n\t-view [int]            : input object view number (def: 0) [if > 0, must append \".viewX\" to file name]"
"\n\n single view 3DOD options only \n"
"\n\t-depthext [string]: input depth map extension name (def .depth24bit.png) {24bit image, with same width and height as input image, r*(2^16) + b*(2^8) + g}"
"\n\t-vieweyex [float] : viewport position of eye x (def: 20)"
"\n\t-vieweyey [float] : viewport position of eye y (def: 20)"
"\n\t-vieweyez [float] : viewport position of eye z (def: 20)"
"\n\t-viewatx [float]  : viewport position of object x (def: 0)"
"\n\t-viewaty [float]  : viewport position of object y (def: 0)"
"\n\t-viewatz [float]  : viewport position of object z (def: 0)"
"\n\t-viewupx [float]  : viewport vertical orientation of eye x (def: 0)"
"\n\t-viewupy [float]  : viewport vertical orientation of eye y (def: 0)"
"\n\t-viewupz [float]  : viewport vertical orientation of eye z (def: 100)"
"\n\t-viewfocal [float]: viewport focal distance (def: 4)"
"\n\t-viewsizew [float]: viewport width (def: 4)"
"\n\t-viewsizeh [float]: viewport height (def: 3)"
"\n\t-scale [float]    : mapping between depthmap bits and pov (defines resolution and maximum depth range/coverage of depth map)"
"\n\n multi view options only \n"
"\n\t-multview [string]     : use multiview list [NEEDS WORK] (def: multViewList.txt) {3DOD every row; ObjectViewFileNameWithNoExtension|imageext|imageWidth|imageHeight|depthext|vieweyex|vieweyey|vieweyez|viewatx|viewaty|viewatz|viewupx|viewupy|viewupz|viewfocal|viewsizew|viewsizeh|scale. 2DOD every row; ObjectViewFileNameWithNoExtension|imageext|imageWidth|imageHeight|xOffset|yOffset}"
#ifdef OR_USE_DATABASE
"\n\t-dbfolder          : file system database base folder path (def: /home/systemusername/source/ORfsdatabase)"
#endif
"\n\n\t-version        : print version"
"\n\n\tThis program either adds an object (imaged from a particular viewpoint) to a BAI OR database, or finds the closest matching object already existent in the BAI OR database.\n\n";

int main(int argc,char **argv)
{
	bool result = true;

	//print execution time
	struct tm *current;
	time_t now;
	time(&now);
	current = localtime(&now);
	char timeAndDateString[100];
	sprintf(timeAndDateString, "%i:%i:%i %i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, (current->tm_mon+1), (current->tm_year + TM_STRUCT_YEAR_OFFSET));
	cout << "OR execution time: " << timeAndDateString << " (start)" << endl;

	char currentFolder[EXE_FOLDER_PATH_MAX_LENGTH];
	getCurrentDirectory(currentFolder);

	#ifdef OR_USE_DATABASE
	string databaseFolderName =  OR_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME + OR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	#endif

	if (argumentExists(argc,argv,"-workingfolder"))
	{
		workingFolderCharStar=getCharArgument(argc,argv,"-workingfolder");
	}
	else
	{
		workingFolderCharStar = currentFolder;
	}
	if (argumentExists(argc,argv,"-exefolder"))
	{
		exeFolderCharStar=getCharArgument(argc,argv,"-exefolder");
	}
	else
	{
		exeFolderCharStar = currentFolder;
	}
	if (argumentExists(argc,argv,"-tempfolder"))
	{
		tempFolderCharStar=getCharArgument(argc,argv,"-tempfolder");
	}
	else
	{
		tempFolderCharStar = currentFolder;
	}

	setCurrentDirectory(workingFolderCharStar);

	if(!parseORrulesXMLfile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInORrulesExternVariables();

	setCurrentDirectory(tempFolderCharStar);

	int dimension;
	if (argumentExists(argc,argv,"-od3"))
	{
		dimension = OR_METHOD3DOD_DIMENSIONS;
	}
	else
	{
		dimension = OR_METHOD2DOD_DIMENSIONS;
	}

	bool clearTrainTable;
	if (argumentExists(argc,argv,"-cleartrain"))
	{
		clearTrainTable = true;
	}
	else
	{
		clearTrainTable = false;
	}

	int trainOrTest;
	if (argumentExists(argc,argv,"-trainortest"))
	{
		trainOrTest = int(getFloatArgument(argc,argv,"-trainortest"));

		if(!((trainOrTest == TEST) || (trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST)))
		{
			cout << "error: trainortest value is illegal" << endl;
			printORcommandLineErrorMessage();
			exit(0);
		}
	}
	else
	{
		cout << "error: trainortest is not defined" << endl;
		printORcommandLineErrorMessage();
		exit(0);

	}

	#ifndef OR_METHOD3DOD_TEST
	string sqlIPaddress;
	string sqlUsername;
	string sqlPassword;
	char * sqlipaddressCharStar = new char[100];
	char * sqlusernameCharStar = new char[100];
	char * sqlpasswordCharStar = new char[100];
	if (argumentExists(argc,argv,"-sqlipaddress"))
	{
		sqlipaddressCharStar=getCharArgument(argc,argv,"-sqlipaddress");
	}
	else
	{
		cout << "error: sqlipaddress is not defined" << endl;
		printORcommandLineErrorMessage();
		exit(0);
	}
	if (argumentExists(argc,argv,"-sqlusername"))
	{
		sqlusernameCharStar=getCharArgument(argc,argv,"-sqlusername");
	}
	else
	{
		cout << "error: sqlusername is not defined" << endl;
		printORcommandLineErrorMessage();
		exit(0);
	}

	if (argumentExists(argc,argv,"-sqlpassword"))
	{
		sqlpasswordCharStar=getCharArgument(argc,argv,"-sqlpassword");
	}
	else
	{
		cout << "error: sqlpassword is not defined" << endl;
		printORcommandLineErrorMessage();
		exit(0);
	}
	sqlIPaddress = sqlipaddressCharStar;
	sqlUsername = sqlusernameCharStar;
	sqlPassword = sqlpasswordCharStar;
	#endif

	ViewInfo vi;
	string ObjectNameArray[10];
	int imageWidthFacingPoly;
	int imageHeightFacingPoly;
	int maxNumberOfPolygons;
	int numberOfViewIndiciesPerObject;
	int numberOfObjects;
	int numberOfZoomIndicies;
	int numberOfViewIndiciesPerObjectWithUniquePolygons;
	int * numberOfPolys;
	int objectDataSource;

	//set defaults before overwriting with user specific values;
	if(dimension == OR_METHOD2DOD_DIMENSIONS)
	{

		vi.imageWidth = TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH;
		vi.imageHeight = TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT;
		vi.viewWidth = TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH;
		vi.viewHeight = TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT;
		vi.focalLength = TH_OR_METHOD2DOD_DEFAULT_FOCAL;
		vi.eye.x = TH_OR_METHOD2DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		vi.eye.y = TH_OR_METHOD2DOD_DEFAULT_EYE_Y;
		vi.eye.z = TH_OR_METHOD2DOD_DEFAULT_EYE_Z;
		vi.viewAt.x = TH_OR_METHOD2DOD_DEFAULT_VIEWAT_X;
		vi.viewAt.y = TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Y;
		vi.viewAt.z = TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Z;
		vi.viewUp.x = TH_OR_METHOD2DOD_DEFAULT_VIEWUP_X;
		vi.viewUp.y = TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Y;
		vi.viewUp.z = TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Z;

		ObjectNameArray[0] = OR_METHOD2DOD_OBJECT_0_NAME;


		imageWidthFacingPoly = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH;
		imageHeightFacingPoly = TH_OR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT;
		maxNumberOfPolygons = OR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
		numberOfViewIndiciesPerObject = 1;	//this will be changed based upon num references in multview list file
		numberOfObjects = 1;		//ORmain feeds max 1 object at a time into database
		numberOfZoomIndicies = OR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
		numberOfViewIndiciesPerObjectWithUniquePolygons = 1;		//always 1;
		numberOfPolys = new int[numberOfObjects*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies];

	}
	else if(dimension == OR_METHOD3DOD_DIMENSIONS)
	{
		vi.imageWidth = TH_OR_METHOD3DOD_DEFAULT_IMAGE_WIDTH;
		vi.imageHeight = TH_OR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT;
		vi.viewWidth = TH_OR_METHOD3DOD_DEFAULT_VIEWSIZE_WIDTH;
		vi.viewHeight = TH_OR_METHOD3DOD_DEFAULT_VIEWSIZE_HEIGHT;
		vi.focalLength = TH_OR_METHOD3DOD_DEFAULT_FOCAL;
		vi.eye.x = TH_OR_METHOD3DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		vi.eye.y = TH_OR_METHOD3DOD_DEFAULT_EYE_Y;
		vi.eye.z = TH_OR_METHOD3DOD_DEFAULT_EYE_Z;
		vi.viewAt.x = TH_OR_METHOD3DOD_DEFAULT_VIEWAT_X;
		vi.viewAt.y = TH_OR_METHOD3DOD_DEFAULT_VIEWAT_Y;
		vi.viewAt.z = TH_OR_METHOD3DOD_DEFAULT_VIEWAT_Z;
		vi.viewUp.x = TH_OR_METHOD3DOD_DEFAULT_VIEWUP_X;
		vi.viewUp.y = TH_OR_METHOD3DOD_DEFAULT_VIEWUP_Y;
		vi.viewUp.z = TH_OR_METHOD3DOD_DEFAULT_VIEWUP_Z;
		vi.depthScale =  OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE;

		ObjectNameArray[0] = OR_METHOD3DOD_OBJECT_0_NAME;

		imageWidthFacingPoly = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH;
		imageHeightFacingPoly = TH_OR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT;
		maxNumberOfPolygons = OR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
		numberOfViewIndiciesPerObject = 1;	//this will be changed based upon num references in multview list file
		numberOfObjects = 1;		//ORmain feeds max 1 object at a time into database
		numberOfZoomIndicies = OR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
		numberOfViewIndiciesPerObjectWithUniquePolygons = 1;		//always 1
		numberOfPolys = new int[numberOfObjects*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies];
	}
	else
	{
		cout << "illegal number of dimension" << endl;
		printORcommandLineErrorMessage();
		exit(0);
	}

	string multViewList;
	char * multViewListFileNameCharStar = new char[100];
	string multViewListFileName;
	bool useMultViewList = false;
	if(argumentExists(argc,argv,"-multview"))
	{
		multViewListFileNameCharStar=getCharArgument(argc,argv,"-multview");
		multViewListFileName = multViewListFileNameCharStar;
		useMultViewList = true;

		ViewInfo tempViewInfo;
		string multViewListFileNameWithFullPath = "";
		multViewListFileNameWithFullPath = multViewListFileNameWithFullPath + workingFolderCharStar + "/" + multViewListFileName;
		numberOfViewIndiciesPerObject = createViFromMultiViewList(&tempViewInfo, multViewListFileNameWithFullPath, 0, dimension);
		cout << "countNumViFromMultiViewList numberOfViewIndiciesPerObject = " << numberOfViewIndiciesPerObject << endl;

		objectDataSource = OR_OBJECT_DATA_SOURCE_USER_LIST;
	}
	else
	{
		objectDataSource = OR_OBJECT_DATA_SOURCE_USER_FILE;
	}

	char * imageFileNameCharStar = new char[100];
	string imageFileName;
	if (argumentExists(argc,argv,"-object"))
	{
		imageFileNameCharStar=getCharArgument(argc,argv,"-object");
		imageFileName = imageFileNameCharStar;
		ObjectNameArray[0] = imageFileName;
		vi.objectName = imageFileName;
	}
	else
	{
		cout << "error: must specify input object name" << endl;
		printORcommandLineErrorMessage();
		exit(0);
	}

	if (argumentExists(argc,argv,"-version"))
	{
		cout << "OpenOR.exe - Project Version: 3d4a 8-June-2014" << endl;
		exit(1);
	}

	char * imageExtensionNameCharStar = new char[100];
	string imageExtensionName;
	if (argumentExists(argc,argv,"-imageext"))
	{
		imageExtensionNameCharStar=getCharArgument(argc,argv,"-imageext");
		imageExtensionName = imageExtensionNameCharStar;
		vi.imageExtensionName = imageExtensionName;
	}
	else
	{
		if(!useMultViewList)
		{
			cout << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			printORcommandLineErrorMessage();
			exit(0);
		}
	}

	if (argumentExists(argc,argv,"-width"))
	{
		vi.imageWidth=getFloatArgument(argc,argv,"-width");
	}
	else
	{
		if(!useMultViewList)
		{
			cout << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			printORcommandLineErrorMessage();
			exit(0);
		}
	}

	if (argumentExists(argc,argv,"-height"))
	{
		vi.imageHeight=getFloatArgument(argc,argv,"-height");
	}
	else
	{
		if(!useMultViewList)
		{
			cout << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			printORcommandLineErrorMessage();
			exit(0);
		}
	}

	bool missingaPOVDescriptor = false;
	bool missingDepthMapExtensionDescriptor = false;

	char * depthExtensionNameCharStar = new char[100];
	string depthExtensionName;
	if(argumentExists(argc,argv,"-depthext"))
	{
		depthExtensionNameCharStar=getCharArgument(argc,argv,"-depthext");
		depthExtensionName = depthExtensionNameCharStar;
		vi.depthExtensionName = depthExtensionName;
	}
	else
	{
		missingaPOVDescriptor = true;
		missingDepthMapExtensionDescriptor = true;
	}

	if(argumentExists(argc,argv,"-vieweyex"))
	{
		vi.eye.x=getFloatArgument(argc,argv,"-vieweyex");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-vieweyey"))
	{
		vi.eye.y=getFloatArgument(argc,argv,"-vieweyey");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-vieweyez"))
	{
		vi.eye.z=getFloatArgument(argc,argv,"-vieweyez");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewatx"))
	{
		vi.viewAt.x=getFloatArgument(argc,argv,"-viewatx");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewaty"))
	{
		vi.viewAt.y=getFloatArgument(argc,argv,"-viewaty");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewatz"))
	{
		vi.viewAt.z=getFloatArgument(argc,argv,"-viewatz");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewupx"))
	{
		vi.viewUp.x=getFloatArgument(argc,argv,"-viewupx");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewupy"))
	{
		vi.viewUp.y=getFloatArgument(argc,argv,"-viewupy");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewupz"))
	{
		vi.viewUp.z=getFloatArgument(argc,argv,"-viewupz");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewfocal"))
	{
		vi.focalLength=getFloatArgument(argc,argv,"-viewfocal");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewsizew"))
	{
		vi.viewWidth=getFloatArgument(argc,argv,"-viewsizew");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-viewsizeh"))
	{
		vi.viewHeight=getFloatArgument(argc,argv,"-viewsizeh");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(argumentExists(argc,argv,"-scale"))
	{
		vi.depthScale=getFloatArgument(argc,argv,"-scale");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	int viewNumber = 0;
	if (argumentExists(argc,argv,"-view"))
	{
		viewNumber=getFloatArgument(argc,argv,"-view");

		if(useMultViewList)
		{
			cout << "error: cannot specify both a multview list and a single view view number" << endl;
			exit(0);
		}
	}

	#ifdef OR_USE_DATABASE
	if(argumentExists(argc,argv,"-dbfolder"))
	{
		databaseFolderName=getCharArgument(argc,argv,"-dbfolder");
		databaseFolderName = databaseFolderName + '/';
	}
	initialiseDatabase(databaseFolderName);
	#endif

	if(missingDepthMapExtensionDescriptor && !useMultViewList && (dimension == OR_METHOD3DOD_DIMENSIONS))
	{
		cout << "error: must either specify an input depth map extension and POV parameters (if not wanting defaults), or a mult (object/) view list for 3DOD" << endl;
		printORcommandLineErrorMessage();
		//exit(0);
	}


	if(trainOrTest == TRAIN)
	{
		if(!ORmethodTrain(dimension, numberOfObjects, ObjectNameArray, numberOfPolys, objectDataSource, &vi, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, numberOfViewIndiciesPerObject, numberOfViewIndiciesPerObjectWithUniquePolygons, numberOfZoomIndicies, trainOrTest, sqlIPaddress, sqlUsername, sqlPassword, clearTrainTable, viewNumber, multViewListFileName))
		{
			result = false;
			cout << "ORmethodTrain reports failure" << endl;
		}
		else
		{
			cout << "ORmethodTrain completed" << endl;
		}

	}
	else if((trainOrTest == TEST) || (trainOrTest == TRAIN_AND_TEST))
	{
		if(!ORmethodTest(dimension, numberOfObjects, ObjectNameArray, numberOfPolys, objectDataSource, &vi, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, numberOfViewIndiciesPerObject, numberOfViewIndiciesPerObjectWithUniquePolygons, numberOfZoomIndicies, trainOrTest, sqlIPaddress, sqlUsername, sqlPassword, clearTrainTable, viewNumber, multViewListFileName))
		{
			result = false;
			cout << "ORmethodTest reports failure" << endl;
		}
		else
		{
			cout << "ORmethodTest completed" << endl;
		}
	}

	//print execution time (end)
	time(&now);
	current = localtime(&now);
	sprintf(timeAndDateString, "%i:%i:%i %i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, (current->tm_mon+1), (current->tm_year + TM_STRUCT_YEAR_OFFSET));
	cout << "OR execution time: " << timeAndDateString << " (finish)" << endl;
}

void printORcommandLineErrorMessage()
{
	if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
	{
		cout << errmessage << endl;
	}
}




