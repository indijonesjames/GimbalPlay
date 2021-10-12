
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <intrin.h>


using namespace FlyCapture2;
using namespace std;

#define _FRAMERATE FRAMERATE_FORMAT7
#define _VIDEOMODE VIDEOMODE_FORMAT7


int main()
{
	cv::setBreakOnError(true);
	cout << "OpenCV version : " << CV_VERSION << endl;
	cout << "Major version : " << CV_MAJOR_VERSION << endl;
	cout << "Minor version : " << CV_MINOR_VERSION << endl;
	cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
	/*cv::Point centroid2;
	find_newObject(centroid2, 12);
	return 1;*/
	HANDLE serialHandle = { 0 };
	//********** servos start ************************************************************************************
	ServoInit(serialHandle);

	// config servo0 - gimbal0
	ServoConfig(serialHandle, 0, 0, 0);		// speed, acceleration, number
	// config servo1 - gimbal1
	ServoConfig(serialHandle, 0, 0, 1);		// speed, acceleration, number
	// config servo2 - camera0
	//ServoConfig(serialHandle, 5, 0, 2);		// speed, acceleration, number
	// config servo3 - camera1
	//ServoConfig(serialHandle, 0, 0, 3);		// speed, acceleration, number

	// Send servos to Initial/Calibration position
	extern int cuadrants[];
	extern int scanning_cuadrants[];
	extern double minArea;
	extern double rratio;
	int current_cuadrant = 8; // servos start at home
	int servos[] = { 0,1};
	ServosHome(serialHandle, servos); 

	// Read current status
	unsigned char servosStatus;
	servosStatus = ServosReadState(serialHandle);
	printf("States:\n");
	printf("%d\n", servosStatus);

	// Flir SDK information
	PrintBuildInfo();
	Error error;

	// Check folder permissions
	FILE* tempFile = fopen("images\\test.txt", "w+");
	if (tempFile == NULL)
	{
		cout << "Failed to create file in current folder.  Please check "
			"permissions."
			<< endl;
		return -1;
	}
	fclose(tempFile);
	remove("images\\test.txt");

	// look for connected cameras
	BusManager busMgr;
	PGRGuid guid;
	Camera cam;
	FindCameras(busMgr, guid);

	// connect to the camera
	Connect2Camera(guid, cam);
	
	// test images
	int numImages = 3;
	string type = "Test";
	TakeImages(cam, numImages, type);

	// take calibration images
	cout << "\First Pass to Find the calibration filters..." << endl;
	current_cuadrant = 1;
	ServosMoveCuadrant(serialHandle, current_cuadrant);
	numImages = 3;
	type = "Calibrate";
	for (int i = 0; i < 7; i++) {
		current_cuadrant = scanning_cuadrants[i];
		ServosMoveCuadrant(serialHandle, current_cuadrant);
		Sleep(500);
		type = "Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate";
		TakeImages(cam, numImages, type);
	}

	// Calculate the Calibration Filters
	cout << "\nSecond Pass to Find the calibration filters..." << endl;
	current_cuadrant = 1;
	ServosMoveCuadrant(serialHandle, current_cuadrant);
	numImages = 3;
	type = "Calibrate2";
	for (int i = 0; i < 7; i++) {
		current_cuadrant = scanning_cuadrants[i];
		ServosMoveCuadrant(serialHandle, current_cuadrant);
		Sleep(500);
		type = "Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate2";
		TakeImages(cam, numImages, type);
		calibrate_cuadrant(current_cuadrant);
	}

	printf("\nAccuracy settings are MinArea= %f, Similarity Ratio = %f.", minArea, rratio);
	cout << "\nCalibration finished! Introduce a new object, and press Enter to continue..." << endl;
	cin.ignore();

	// analyze new scene
	string input = "y";
	numImages = 1;
	cv::Point centroid;
	while (input == "y") {
		ServosHome(serialHandle, servos);
		for (int j = 0; j < 7; j++) {
			current_cuadrant = scanning_cuadrants[j];
			ServosMoveCuadrant(serialHandle, current_cuadrant);
			Sleep(750);
			type = "Cuadrant-" + to_string(current_cuadrant) + "\\Scan";
			TakeImages(cam, numImages, type);
			bool tracker = find_newObject(centroid, current_cuadrant);
			if (tracker)
			{
				ServosMoveCuadrant(serialHandle, current_cuadrant);
				Sleep(750);
				numImages = 3;
				type = "Cuadrant-" + to_string(current_cuadrant) + "\\Result";
				TakeImages(cam, numImages, type);
				display_object(current_cuadrant);
				break;
			}
		}
		cout << "\nDo you want to scan again? (y/n)" << endl;
		cin >> input;
	}

	// make sure movement ended
	ServosFinish(serialHandle);

	// Print and read positions
	//PrintServosPositions(serialHandle, servos);

	// Disconnect
	DisconnectCamera(cam);

	// send servos home
	ServosExit(serialHandle);
	printf("Servos Home, program ends.\n");
	//********** servos end **************************************************************************************/


	CloseHandle(serialHandle);
	return 0;
}