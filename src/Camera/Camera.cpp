#include "Camera.h"
#include "stdafx.h"

using namespace FlyCapture2;
using namespace std;

void PrintBuildInfo()
{
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "."
		<< fc2Version.minor << "." << fc2Version.type << "."
		<< fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}

void PrintCameraInfo(CameraInfo* pCamInfo)
{
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number - " << pCamInfo->serialNumber << endl;
	cout << "Camera model - " << pCamInfo->modelName << endl;
	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl
		<< endl;
}

void PrintError(Error error) { error.PrintErrorTrace(); }

void FindCameras(BusManager& busMgr, PGRGuid &guid)
{
	unsigned int numCameras;
	Error error;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		exit (EXIT_FAILURE);
	}

	if (numCameras == 0) {
		cout << "Failed to find any camera" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "\nNumber of cameras detected: " << numCameras << endl;

	if (numCameras > 1) {
		cout << "Please introduce which camera to connect:" << endl;
		cin >> numCameras;
	}

	error = busMgr.GetCameraFromIndex(numCameras-1, &guid);

	if (error != PGRERROR_OK)
	{
		PrintError(error);
		exit(EXIT_FAILURE);
	}
	return;
}

void Connect2Camera(PGRGuid& guid, Camera& cam)
{
	Error error;
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		exit(EXIT_FAILURE);
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	PrintCameraInfo(&camInfo);

	// Get the camera configuration
	FC2Config config;
	error = cam.GetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	// Set the number of driver buffers used to 10.
	config.numBuffers = 10;

	// Set the camera configuration
	error = cam.SetConfiguration(&config);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}
	return;
}

void TakeImages(Camera& cam, int numImages, string type, string name)
{
	Error error;
	Image rawImage;
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}

	for (int imageCnt = 0; imageCnt < numImages; imageCnt++)
	{
		// Retrieve an image
		error = cam.RetrieveBuffer(&rawImage);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			continue;
		}

		//cout << "Image Grabbed" << endl;

		// Create a converted image
		Image convertedImage;

		// Convert the raw image
		error = rawImage.Convert(PIXEL_FORMAT_MONO8, &convertedImage);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return;
		}

		// Create a unique filename
		ostringstream filename;
		if (name == "empty") {
			filename << "images\\" << type << "-" << imageCnt << ".tiff";
		}
		else {
			filename << "images\\" << type << "-" << name << ".tiff";
		}

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		error = convertedImage.Save(filename.str().c_str());
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			exit(EXIT_FAILURE);
		}
	}
	// Stop capturing images
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		exit(EXIT_FAILURE);
	}
	return;
}

void TakeImages_in_Motion(HANDLE& serialHandle, Camera& cam, int numImages, string type)
{
	int niter = 0;
	while (ServosReadState(serialHandle))
	{
		TakeImages(cam, numImages, type, to_string(niter));
		Sleep(100);
		niter++;
	}
	return;
}

void DisconnectCamera(Camera& cam)
{
	Error error;
	// Disconnect the camera
	error = cam.Disconnect();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		exit(EXIT_FAILURE);
	}
	return;
}
