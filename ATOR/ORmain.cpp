/*******************************************************************************
 *
 * File Name: ORmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3a8a 14-June-2012
 *
 *******************************************************************************/

#include <ctime>
#define TM_STRUCT_YEAR_OFFSET 1900

#include "ORmain.h"
#include "ORmethod.h"
#include "ORglobalDefs.h"
#include "XMLrulesClass.h"
#include "ORrules.h"

	//for mkdir, chdir, etc	[CreateDirectory, SetCurrentDirectory]	
#ifdef LINUX
	#include <sys/stat.h> 
	#include <sys/types.h>
#else
	#include <windows.h>
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


static char errmessage[] = "Usage:  OR.exe [options]\n\n\twhere options are any of the following\n"
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
"\n\t-exefolder [string]    : exe directory name for executables OR.exe and FD.exe (def: same as exe)"
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
"\n\t-multview [string]     : use multiview list [NEEDS WORK] (def: multViewList.txt) {3DOD every row; ObjectViewFileNameWithNoExtension|imageext|imageWidth|imageHeight|depthext|vieweyex|vieweyey|vieweyez|viewatx|viewaty|viewatz|viewupx|viewupy|viewupz|viewfocal|viewsizew|viewsizeh|scale. 2DOD every row; ObjectViewFileNameWithNoExtension|imageext|imageWidth|imageHeight|xoffset|yoffset}"
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

	#ifdef LINUX
	getcwd(currentFolder, EXE_FOLDER_PATH_MAX_LENGTH);					
	#else
	::GetCurrentDirectory(EXE_FOLDER_PATH_MAX_LENGTH, currentFolder);
	#endif

	//cout << "currentFolder = " << currentFolder << endl;
			
	if (exists_argument(argc,argv,"-workingfolder"))
	{
		workingFolderCharStar=get_char_argument(argc,argv,"-workingfolder");
	}
	else
	{
		workingFolderCharStar = currentFolder;		
	}
	if (exists_argument(argc,argv,"-exefolder"))
	{
		exeFolderCharStar=get_char_argument(argc,argv,"-exefolder");
	}
	else
	{
		exeFolderCharStar = currentFolder;
	}
	if (exists_argument(argc,argv,"-tempfolder"))
	{
		tempFolderCharStar=get_char_argument(argc,argv,"-tempfolder");
	}
	else
	{
		tempFolderCharStar = currentFolder;
	}
	
	/*	
	cout << "workingFolderCharStar = " << workingFolderCharStar << endl;	
	cout << "exeFolderCharStar = " << exeFolderCharStar << endl;	
	cout << "tempFolderCharStar = " << tempFolderCharStar << endl;
	*/
	
	#ifdef LINUX
	chdir(workingFolderCharStar);						
	#else
	::SetCurrentDirectory(workingFolderCharStar);
	#endif
		
	
	if(!parseORRulesXMLFile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	fillInORRulesExternVariables();

	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif
		
	int dimension;
	if (exists_argument(argc,argv,"-od3"))
	{
		dimension = OR_METHOD3DOD_DIMENSIONS;
	}
	else
	{
		dimension = OR_METHOD2DOD_DIMENSIONS;
	}

	bool clearTrainTable;
	if (exists_argument(argc,argv,"-cleartrain"))
	{
		clearTrainTable = true;
	}
	else
	{
		clearTrainTable = false;
	}
	
	int trainOrTest;
	if (exists_argument(argc,argv,"-trainortest"))
	{
		trainOrTest = int(get_float_argument(argc,argv,"-trainortest"));
		
		if(!((trainOrTest == TEST) || (trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST)))
		{
			cout << "error: trainortest value is illegal" << endl;
			printORCommandLineErrorMessage();
			exit(0);		
		}
	}
	else
	{
		cout << "error: trainortest is not defined" << endl;
		printORCommandLineErrorMessage();
		exit(0);
		
	}

	string sqlipaddress;
	string sqlusername;
	string sqlpassword;
	char * sqlipaddressCharStar = new char[100];
	char * sqlusernameCharStar = new char[100];
	char * sqlpasswordCharStar = new char[100];	
	if (exists_argument(argc,argv,"-sqlipaddress"))
	{
		sqlipaddressCharStar=get_char_argument(argc,argv,"-sqlipaddress");
	}
	else
	{
		cout << "error: sqlipaddress is not defined" << endl;
		printORCommandLineErrorMessage();
		exit(0);
	}
	if (exists_argument(argc,argv,"-sqlusername"))
	{
		sqlusernameCharStar=get_char_argument(argc,argv,"-sqlusername");
	}
	else
	{
		cout << "error: sqlusername is not defined" << endl;
		printORCommandLineErrorMessage();
		exit(0);
	}
	
	if (exists_argument(argc,argv,"-sqlpassword"))
	{
		sqlpasswordCharStar=get_char_argument(argc,argv,"-sqlpassword");
	}
	else
	{
		cout << "error: sqlpassword is not defined" << endl;
		printORCommandLineErrorMessage();
		exit(0);
	}
	sqlipaddress = sqlipaddressCharStar;
	sqlusername = sqlusernameCharStar;
	sqlpassword = sqlpasswordCharStar;




	
	view_info vi;
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

		vi.imgwidth = TH_OR_METHOD2DOD_DEFAULT_IMAGE_WIDTH;
		vi.imgheight = TH_OR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT;
		vi.viewwidth = TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH;
		vi.viewheight = TH_OR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT;
		vi.focal_length = TH_OR_METHOD2DOD_DEFAULT_FOCAL;
		vi.eye.x = TH_OR_METHOD2DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		vi.eye.y = TH_OR_METHOD2DOD_DEFAULT_EYE_Y;
		vi.eye.z = TH_OR_METHOD2DOD_DEFAULT_EYE_Z;
		vi.viewat.x = TH_OR_METHOD2DOD_DEFAULT_VIEWAT_X;
		vi.viewat.y = TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Y;
		vi.viewat.z = TH_OR_METHOD2DOD_DEFAULT_VIEWAT_Z;
		vi.viewup.x = TH_OR_METHOD2DOD_DEFAULT_VIEWUP_X;
		vi.viewup.y = TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Y;
		vi.viewup.z = TH_OR_METHOD2DOD_DEFAULT_VIEWUP_Z;

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
		vi.imgwidth = TH_OR_METHOD3DOD_DEFAULT_IMAGE_WIDTH;
		vi.imgheight = TH_OR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT;
		vi.viewwidth = TH_OR_METHOD3DOD_DEFAULT_VIEWSIZE_WIDTH;
		vi.viewheight = TH_OR_METHOD3DOD_DEFAULT_VIEWSIZE_HEIGHT;
		vi.focal_length = TH_OR_METHOD3DOD_DEFAULT_FOCAL;
		vi.eye.x = TH_OR_METHOD3DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		vi.eye.y = TH_OR_METHOD3DOD_DEFAULT_EYE_Y;
		vi.eye.z = TH_OR_METHOD3DOD_DEFAULT_EYE_Z;
		vi.viewat.x = TH_OR_METHOD3DOD_DEFAULT_VIEWAT_X;
		vi.viewat.y = TH_OR_METHOD3DOD_DEFAULT_VIEWAT_Y;
		vi.viewat.z = TH_OR_METHOD3DOD_DEFAULT_VIEWAT_Z;
		vi.viewup.x = TH_OR_METHOD3DOD_DEFAULT_VIEWUP_X;
		vi.viewup.y = TH_OR_METHOD3DOD_DEFAULT_VIEWUP_Y;
		vi.viewup.z = TH_OR_METHOD3DOD_DEFAULT_VIEWUP_Z;
		vi.depthscale =  OR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE;

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
		printORCommandLineErrorMessage();
		exit(0);
	}

	string multViewList;
	char * multViewListFileNameCharStar = new char[100];
	string multViewListFileName;
	bool useMultViewList = false;
	if(exists_argument(argc,argv,"-multview"))
	{
		multViewListFileNameCharStar=get_char_argument(argc,argv,"-multview");
		multViewListFileName = multViewListFileNameCharStar;
		useMultViewList = true;

		view_info tempViewInfo;
		string multViewListFileNameWithFullPath = "";
		multViewListFileNameWithFullPath = multViewListFileNameWithFullPath + workingFolderCharStar + "/" + multViewListFileName;
		numberOfViewIndiciesPerObject = createViFromMultiViewList(&tempViewInfo, multViewListFileNameWithFullPath, 0, dimension);
		cout << "countNumViFromMultiViewList numberOfViewIndiciesPerObject = " << numberOfViewIndiciesPerObject << endl;
		
		objectDataSource = OR_OBJECT_DATA_SOURCE_USER_LIST;

		/*
		cout << "error: cannot currently use a mult (object/) view list - still under testing - must use OR test harness for this feature" << endl;
		printORCommandLineErrorMessage();
		exit(0);
		*/
	}
	else
	{
		objectDataSource = OR_OBJECT_DATA_SOURCE_USER_FILE;
	}

	char * imageFileNameCharStar = new char[100];
	string imageFileName;
	if (exists_argument(argc,argv,"-object"))
	{
		imageFileNameCharStar=get_char_argument(argc,argv,"-object");
		imageFileName = imageFileNameCharStar;
		ObjectNameArray[0] = imageFileName;
		vi.objectName = imageFileName;
		//cout << "ObjectNameArray[0] = " << ObjectNameArray[0] << endl;
	}
	else
	{
		cout << "error: must specify input object name" << endl;
		printORCommandLineErrorMessage();
		exit(0);
	}

	if (exists_argument(argc,argv,"-version"))
	{
		cout << "OR.exe - Project Version: 3a8a 14-June-2012" << endl;
		exit(1);
	}

	char * imageExtensionNameCharStar = new char[100];
	string imageExtensionName;
	if (exists_argument(argc,argv,"-imageext"))
	{
		imageExtensionNameCharStar=get_char_argument(argc,argv,"-imageext");
		imageExtensionName = imageExtensionNameCharStar;
		vi.imageExtensionName = imageExtensionName;
	}
	else
	{
		if(!useMultViewList)
		{
			cout << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			printORCommandLineErrorMessage();
			exit(0);
		}
	}

	if (exists_argument(argc,argv,"-width"))
	{
		vi.imgwidth=get_float_argument(argc,argv,"-width");
	}
	else
	{
		if(!useMultViewList)
		{
			cout << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			printORCommandLineErrorMessage();
			exit(0);
		}
	}

	if (exists_argument(argc,argv,"-height"))
	{
		vi.imgheight=get_float_argument(argc,argv,"-height");
	}
	else
	{
		if(!useMultViewList)
		{
			cout << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			printORCommandLineErrorMessage();
			exit(0);
		}
	}

	bool missingaPOVDescriptor = false;
	bool missingDepthMapExtensionDescriptor = false;
	
	char * depthExtensionNameCharStar = new char[100];
	string depthExtensionName;
	if(exists_argument(argc,argv,"-depthext"))
	{
		depthExtensionNameCharStar=get_char_argument(argc,argv,"-depthext");
		depthExtensionName = depthExtensionNameCharStar;
		vi.depthExtensionName = depthExtensionName;
	}
	else
	{
		missingaPOVDescriptor = true;
		missingDepthMapExtensionDescriptor = true;
	}

	if(exists_argument(argc,argv,"-vieweyex"))
	{
		vi.eye.x=get_float_argument(argc,argv,"-vieweyex");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-vieweyey"))
	{
		vi.eye.y=get_float_argument(argc,argv,"-vieweyey");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-vieweyez"))
	{
		vi.eye.z=get_float_argument(argc,argv,"-vieweyez");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewatx"))
	{
		vi.viewat.x=get_float_argument(argc,argv,"-viewatx");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewaty"))
	{
		vi.viewat.y=get_float_argument(argc,argv,"-viewaty");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewatz"))
	{
		vi.viewat.z=get_float_argument(argc,argv,"-viewatz");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewupx"))
	{
		vi.viewup.x=get_float_argument(argc,argv,"-viewupx");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewupy"))
	{
		vi.viewup.y=get_float_argument(argc,argv,"-viewupy");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewupz"))
	{
		vi.viewup.z=get_float_argument(argc,argv,"-viewupz");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewfocal"))
	{
		vi.focal_length=get_float_argument(argc,argv,"-viewfocal");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewsizew"))
	{
		vi.viewwidth=get_float_argument(argc,argv,"-viewsizew");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-viewsizeh"))
	{
		vi.viewheight=get_float_argument(argc,argv,"-viewsizeh");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(exists_argument(argc,argv,"-scale"))
	{
		vi.depthscale=get_float_argument(argc,argv,"-scale");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	int viewNumber = 0;
	if (exists_argument(argc,argv,"-view"))
	{
		viewNumber=get_float_argument(argc,argv,"-view");
		
		if(useMultViewList)
		{
			cout << "error: cannot specify both a multview list and a single view view number" << endl;
			exit(0);
		}
	}
	
	if(missingDepthMapExtensionDescriptor && !useMultViewList && (dimension == OR_METHOD3DOD_DIMENSIONS))
	{
		cout << "error: must either specify an input depth map extension and POV parameters (if not wanting defaults), or a mult (object/) view list for 3DOD" << endl;
		printORCommandLineErrorMessage();
		//exit(0);
	}

	
	if(trainOrTest == TRAIN)
	{
		if(!ORMethodTrain(dimension, numberOfObjects, ObjectNameArray, numberOfPolys, objectDataSource, &vi, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, numberOfViewIndiciesPerObject, numberOfViewIndiciesPerObjectWithUniquePolygons, numberOfZoomIndicies, trainOrTest, sqlipaddress, sqlusername, sqlpassword, clearTrainTable, viewNumber, multViewListFileName))
		{
			result = false;
			cout << "ORMethodTrain reports failure" << endl;
		}
		else
		{
			cout << "ORMethodTrain completed" << endl;
		}

	}
	else if((trainOrTest == TEST) || (trainOrTest == TRAIN_AND_TEST))
	{
		if(!ORMethodTest(dimension, numberOfObjects, ObjectNameArray, numberOfPolys, objectDataSource, &vi, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, numberOfViewIndiciesPerObject, numberOfViewIndiciesPerObjectWithUniquePolygons, numberOfZoomIndicies, trainOrTest, sqlipaddress, sqlusername, sqlpassword, clearTrainTable, viewNumber, multViewListFileName))
		{
			result = false;
			cout << "ORMethodTest reports failure" << endl;
		}
		else
		{
			cout << "ORMethodTest completed" << endl;
		}
	}
	
	//print execution time (end)
	time(&now);
	current = localtime(&now);
	sprintf(timeAndDateString, "%i:%i:%i %i/%.2i/%i", current->tm_hour, current->tm_min, current->tm_sec, current->tm_mday, (current->tm_mon+1), (current->tm_year + TM_STRUCT_YEAR_OFFSET));
	cout << "OR execution time: " << timeAndDateString << " (finish)" << endl;	
}

void printORCommandLineErrorMessage()
{
	if(OR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
	{
		cout << errmessage << endl;
	}
}




