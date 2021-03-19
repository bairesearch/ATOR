/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ATORmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: ATOR (Axis Transformation Object Recognition) Functions
 * Project Version: 3p3a 19-March-2021
 * /
 *******************************************************************************/


#define TM_STRUCT_YEAR_OFFSET 1900

#include "ATORmain.hpp"
#ifdef ATOR_USE_DATABASE
#endif

static char errmessage[] = "Usage:  ATOR.exe [options]\n\n\twhere options are any of the following\n"
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
"\n\t-inputfolder [string]: input directory name for input files (def: same as exe)"
"\n\t-exefolder [string]    : exe directory name for executables; ATOR.exe and FD.exe (def: same as exe)"
"\n\t-outputfolder [string]    : output directory name for temporary and output files (def: same as exe)"
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
"\n\t-multview [string]     : use multiview list [EXPERIMENTAL] (def: multViewList.txt) {3DOD every row; ObjectViewFileNameWithNoExtension|imageext|imageWidth|imageHeight|depthext|vieweyex|vieweyey|vieweyez|viewatx|viewaty|viewatz|viewupx|viewupy|viewupz|viewfocal|viewsizew|viewsizeh|scale. 2DOD every row; ObjectViewFileNameWithNoExtension|imageext|imageWidth|imageHeight|xOffset|yOffset}"
#ifdef ATOR_USE_DATABASE
"\n\t-dbfolder [string]     : file system database base folder path (def: /home/systemusername/source/ORfsdatabase)"
#endif
"\n\n\t-version        : print version"
"\n\n\tThis program either adds an object (imaged from a particular viewpoint) to a BAI OR database, or finds the closest matching object already existent in the BAI OR database.\n\n";

int main(const int argc,const char* *argv)
{
	bool result = true;

	#ifdef ATOR_PRINT_EXECUTION_TIME
	SHAREDvarsClass().printTime("ATOR execution time: ", " (start)");
	#endif
	
	string currentFolder = SHAREDvarsClass().getCurrentDirectory();

	#ifdef ATOR_USE_DATABASE
	string databaseFolderName =  ATOR_DATABASE_FILESYSTEM_DEFAULT_SERVER_OR_MOUNT_NAME + ATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME;
	#endif

	if(SHAREDvarsClass().argumentExists(argc, argv, "-inputfolder"))
	{
		inputFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-inputfolder");
	}
	else
	{
		inputFolder = currentFolder;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-exefolder"))
	{
		exeFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-exefolder");
	}
	else
	{
		exeFolder = currentFolder;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-outputfolder"))
	{
		outputFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-outputfolder");
	}
	else
	{
		outputFolder = currentFolder;
	}

	SHAREDvarsClass().setCurrentDirectory(inputFolder);

	if(!XMLrulesClassClass().parseATORrulesXMLfile())
	{
		cerr << "error: no rules file detected" << endl;
		exit(EXIT_ERROR);
	}
	ATORrulesClass().fillInATORrulesExternVariables();

	SHAREDvarsClass().setCurrentDirectory(outputFolder);

	int dimension;
	if(SHAREDvarsClass().argumentExists(argc, argv, "-od3"))
	{
		dimension = ATOR_METHOD3DOD_DIMENSIONS;
	}
	else
	{
		dimension = ATOR_METHOD2DOD_DIMENSIONS;
	}

	bool clearTrainTable;
	if(SHAREDvarsClass().argumentExists(argc, argv, "-cleartrain"))
	{
		clearTrainTable = true;
	}
	else
	{
		clearTrainTable = false;
	}

	int trainOrTest;
	if(SHAREDvarsClass().argumentExists(argc, argv, "-trainortest"))
	{
		trainOrTest = int(SHAREDvarsClass().getFloatArgument(argc, argv, "-trainortest"));

		if(!((trainOrTest == TEST) || (trainOrTest == TRAIN) || (trainOrTest == TRAIN_AND_TEST)))
		{
			cerr << "error: trainortest value is illegal" << endl;
			ATORmainClass().printORcommandLineErrorMessage();
			exit(EXIT_ERROR);
		}
	}
	else
	{
		cerr << "error: trainortest is not defined" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		exit(EXIT_ERROR);

	}

	#ifndef ATOR_METHOD3DOD_TEST
	string sqlIPaddress = "";
	string sqlUsername = "";
	string sqlPassword = "";
	if(SHAREDvarsClass().argumentExists(argc, argv, "-sqlipaddress"))
	{
		sqlIPaddress = SHAREDvarsClass().getStringArgument(argc, argv, "-sqlipaddress");
	}
	else
	{
		cerr << "error: sqlipaddress is not defined" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		exit(EXIT_ERROR);
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-sqlusername"))
	{
		sqlUsername = SHAREDvarsClass().getStringArgument(argc, argv, "-sqlusername");
	}
	else
	{
		cerr << "error: sqlusername is not defined" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		exit(EXIT_ERROR);
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-sqlpassword"))
	{
		sqlPassword = SHAREDvarsClass().getStringArgument(argc, argv, "-sqlpassword");
	}
	else
	{
		cerr << "error: sqlpassword is not defined" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		exit(EXIT_ERROR);
	}
	#endif

	RTviewInfo vi;
	string ObjectNameArray[10];
	int imageWidthFacingPoly;
	int imageHeightFacingPoly;
	int maxNumberOfPolygons;
	int numberOfViewIndiciesPerObject;
	int numberOfObjects;
	int numberOfZoomIndicies;
	int numberOfViewIndiciesPerObjectWithUniquePolygons;
	int* numberOfPolys;
	int objectDataSource;

	//set defaults before overwriting with user specific values;
	if(dimension == ATOR_METHOD2DOD_DIMENSIONS)
	{

		vi.imageWidth = TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_WIDTH;
		vi.imageHeight = TH_ATOR_METHOD2DOD_DEFAULT_IMAGE_HEIGHT;
		vi.viewWidth = TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_WIDTH;
		vi.viewHeight = TH_ATOR_METHOD2DOD_DEFAULT_VIEWSIZE_HEIGHT;
		vi.focalLength = TH_ATOR_METHOD2DOD_DEFAULT_FOCAL;
		vi.eye.x = TH_ATOR_METHOD2DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		vi.eye.y = TH_ATOR_METHOD2DOD_DEFAULT_EYE_Y;
		vi.eye.z = TH_ATOR_METHOD2DOD_DEFAULT_EYE_Z;
		vi.viewAt.x = TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_X;
		vi.viewAt.y = TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Y;
		vi.viewAt.z = TH_ATOR_METHOD2DOD_DEFAULT_VIEWAT_Z;
		vi.viewUp.x = TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_X;
		vi.viewUp.y = TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Y;
		vi.viewUp.z = TH_ATOR_METHOD2DOD_DEFAULT_VIEWUP_Z;

		ObjectNameArray[0] = ATOR_METHOD2DOD_OBJECT_0_NAME;


		imageWidthFacingPoly = TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH;
		imageHeightFacingPoly = TH_ATOR_METHOD2DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT;
		maxNumberOfPolygons = ATOR_METHOD2DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
		numberOfViewIndiciesPerObject = 1;	//this will be changed based upon num references in multview list file
		numberOfObjects = 1;		//ATORmain feeds max 1 object at a time into database
		numberOfZoomIndicies = ATOR_METHOD2DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
		numberOfViewIndiciesPerObjectWithUniquePolygons = 1;		//always 1;
		numberOfPolys = new int[numberOfObjects*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies];

	}
	else if(dimension == ATOR_METHOD3DOD_DIMENSIONS)
	{
		vi.imageWidth = TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_WIDTH;
		vi.imageHeight = TH_ATOR_METHOD3DOD_DEFAULT_IMAGE_HEIGHT;
		vi.viewWidth = TH_ATOR_METHOD3DOD_DEFAULT_VIEWSIZE_WIDTH;
		vi.viewHeight = TH_ATOR_METHOD3DOD_DEFAULT_VIEWSIZE_HEIGHT;
		vi.focalLength = TH_ATOR_METHOD3DOD_DEFAULT_FOCAL;
		vi.eye.x = TH_ATOR_METHOD3DOD_DEFAULT_EYE_X;		//CHECK THIS; preferably the eye moves around the object
		vi.eye.y = TH_ATOR_METHOD3DOD_DEFAULT_EYE_Y;
		vi.eye.z = TH_ATOR_METHOD3DOD_DEFAULT_EYE_Z;
		vi.viewAt.x = TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_X;
		vi.viewAt.y = TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_Y;
		vi.viewAt.z = TH_ATOR_METHOD3DOD_DEFAULT_VIEWAT_Z;
		vi.viewUp.x = TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_X;
		vi.viewUp.y = TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_Y;
		vi.viewUp.z = TH_ATOR_METHOD3DOD_DEFAULT_VIEWUP_Z;
		vi.depthScale =  ATOR_METHOD_3DOD_DEPTH_MAP_TO_IMAGE_CONVERSION_SCALE;

		ObjectNameArray[0] = ATOR_METHOD3DOD_OBJECT_0_NAME;

		imageWidthFacingPoly = TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_WIDTH;
		imageHeightFacingPoly = TH_ATOR_METHOD3DOD_FACING_POLY_DEFAULT_IMAGE_HEIGHT;
		maxNumberOfPolygons = ATOR_METHOD3DOD_MAX_NUMBER_OF_POLYGONS_TRAIN;
		numberOfViewIndiciesPerObject = 1;	//this will be changed based upon num references in multview list file
		numberOfObjects = 1;		//ATORmain feeds max 1 object at a time into database
		numberOfZoomIndicies = ATOR_METHOD3DOD_NUMBER_OF_SNAPSHOT_ZOOM_LEVELS;
		numberOfViewIndiciesPerObjectWithUniquePolygons = 1;		//always 1
		numberOfPolys = new int[numberOfObjects*numberOfViewIndiciesPerObjectWithUniquePolygons*numberOfZoomIndicies];
	}
	else
	{
		cerr << "illegal number of dimension" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		exit(EXIT_ERROR);
	}

	string multViewList = "";
	string multViewListFileName = "";
	bool useMultViewList = false;
	if(SHAREDvarsClass().argumentExists(argc, argv, "-multview"))
	{
		multViewListFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-multview");
		useMultViewList = true;

		RTviewInfo tempViewInfo;
		string multViewListFileNameWithFullPath = "";
		multViewListFileNameWithFullPath = multViewListFileNameWithFullPath + inputFolder + "/" + multViewListFileName;
		numberOfViewIndiciesPerObject = ATORmethodClass().createViFromMultiViewList(&tempViewInfo, multViewListFileNameWithFullPath, 0, dimension);
		cout << "countNumViFromMultiViewList numberOfViewIndiciesPerObject = " << numberOfViewIndiciesPerObject << endl;

		objectDataSource = ATOR_OBJECT_DATA_SOURCE_USER_LIST;
	}
	else
	{
		objectDataSource = ATOR_OBJECT_DATA_SOURCE_USER_FILE;
	}

	string imageFileName = "";
	if(SHAREDvarsClass().argumentExists(argc, argv, "-object"))
	{
		imageFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-object");
		ObjectNameArray[0] = imageFileName;
		vi.objectName = imageFileName;
	}
	else
	{
		cerr << "error: must specify input object name" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		exit(EXIT_ERROR);
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-version"))
	{
		cerr << "ATOR.exe - Project Version: 3p3a 19-March-2021" << endl;
		exit(EXIT_OK);
	}

	string imageExtensionName = "";
	if(SHAREDvarsClass().argumentExists(argc, argv, "-imageext"))
	{
		imageExtensionName = SHAREDvarsClass().getStringArgument(argc, argv, "-imageext");
		vi.imageExtensionName = imageExtensionName;
	}
	else
	{
		if(!useMultViewList)
		{
			cerr << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			ATORmainClass().printORcommandLineErrorMessage();
			exit(EXIT_ERROR);
		}
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-width"))
	{
		vi.imageWidth = SHAREDvarsClass().getFloatArgument(argc, argv, "-width");
	}
	else
	{
		if(!useMultViewList)
		{
			cerr << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			ATORmainClass().printORcommandLineErrorMessage();
			exit(EXIT_ERROR);
		}
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-height"))
	{
		vi.imageHeight = SHAREDvarsClass().getFloatArgument(argc, argv, "-height");
	}
	else
	{
		if(!useMultViewList)
		{
			cerr << "error: must either specify image extension, and width/height or a mult (object/) view list" << endl;
			ATORmainClass().printORcommandLineErrorMessage();
			exit(EXIT_ERROR);
		}
	}

	bool missingaPOVDescriptor = false;
	bool missingDepthMapExtensionDescriptor = false;

	string depthExtensionName = "";
	if(SHAREDvarsClass().argumentExists(argc, argv, "-depthext"))
	{
		depthExtensionName = SHAREDvarsClass().getStringArgument(argc, argv, "-depthext");
		vi.depthExtensionName = depthExtensionName;
	}
	else
	{
		missingaPOVDescriptor = true;
		missingDepthMapExtensionDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-vieweyex"))
	{
		vi.eye.x = SHAREDvarsClass().getFloatArgument(argc, argv, "-vieweyex");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-vieweyey"))
	{
		vi.eye.y = SHAREDvarsClass().getFloatArgument(argc, argv, "-vieweyey");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-vieweyez"))
	{
		vi.eye.z = SHAREDvarsClass().getFloatArgument(argc, argv, "-vieweyez");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewatx"))
	{
		vi.viewAt.x = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewatx");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewaty"))
	{
		vi.viewAt.y = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewaty");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewatz"))
	{
		vi.viewAt.z = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewatz");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewupx"))
	{
		vi.viewUp.x = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewupx");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewupy"))
	{
		vi.viewUp.y = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewupy");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewupz"))
	{
		vi.viewUp.z = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewupz");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewfocal"))
	{
		vi.focalLength = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewfocal");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewsizew"))
	{
		vi.viewWidth = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewsizew");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-viewsizeh"))
	{
		vi.viewHeight = SHAREDvarsClass().getFloatArgument(argc, argv, "-viewsizeh");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-scale"))
	{
		vi.depthScale = SHAREDvarsClass().getFloatArgument(argc, argv, "-scale");
	}
	else
	{
		missingaPOVDescriptor = true;
	}

	int viewNumber = 0;
	if(SHAREDvarsClass().argumentExists(argc, argv, "-view"))
	{
		viewNumber = SHAREDvarsClass().getFloatArgument(argc, argv, "-view");

		if(useMultViewList)
		{
			cerr << "error: cannot specify both a multview list and a single view view number" << endl;
			exit(EXIT_ERROR);
		}
	}

	#ifdef ATOR_USE_DATABASE
	if(SHAREDvarsClass().argumentExists(argc, argv, "-dbfolder"))
	{
		databaseFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-dbfolder");
		databaseFolderName = databaseFolderName + '/';
	}
	ATORdatabaseFileIOClass().initialiseDatabase(databaseFolderName);
	#endif

	if(missingDepthMapExtensionDescriptor && !useMultViewList && (dimension == ATOR_METHOD3DOD_DIMENSIONS))
	{
		cerr << "error: must either specify an input depth map extension and POV parameters (if not wanting defaults), or a mult (object/) view list for 3DOD" << endl;
		ATORmainClass().printORcommandLineErrorMessage();
		//exit(EXIT_ERROR);
	}


	if(trainOrTest == TRAIN)
	{
		if(!ATORmethodClass().ATORmethodTrain(dimension, numberOfObjects, ObjectNameArray, numberOfPolys, objectDataSource, &vi, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, numberOfViewIndiciesPerObject, numberOfViewIndiciesPerObjectWithUniquePolygons, numberOfZoomIndicies, trainOrTest, sqlIPaddress, sqlUsername, sqlPassword, clearTrainTable, viewNumber, multViewListFileName))
		{
			result = false;
			cout << "ATORmethodTrain reports failure" << endl;
		}
		else
		{
			cout << "ATORmethodTrain completed" << endl;
		}

	}
	else if((trainOrTest == TEST) || (trainOrTest == TRAIN_AND_TEST))
	{
		if(!ATORmethodClass().ATORmethodTest(dimension, numberOfObjects, ObjectNameArray, numberOfPolys, objectDataSource, &vi, imageWidthFacingPoly, imageHeightFacingPoly, maxNumberOfPolygons, numberOfViewIndiciesPerObject, numberOfViewIndiciesPerObjectWithUniquePolygons, numberOfZoomIndicies, trainOrTest, sqlIPaddress, sqlUsername, sqlPassword, clearTrainTable, viewNumber, multViewListFileName))
		{
			result = false;
			cout << "ATORmethodTest reports failure" << endl;
		}
		else
		{
			cout << "ATORmethodTest completed" << endl;
		}
	}

	#ifdef ATOR_PRINT_EXECUTION_TIME
	SHAREDvarsClass().printTime("ATOR execution time: ", " (finish)");
	#endif
}

void ATORmainClass::printORcommandLineErrorMessage()
{
	if(ATOR_GENERATE_IMAGE_COMPARITOR_RESULTS_ALLOW_CONFIDENTIAL)
	{
		cout << errmessage << endl;
	}
}




