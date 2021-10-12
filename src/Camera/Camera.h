#pragma once

#include "..//stdafx.h"
#include "FlyCapture2.h"

using namespace FlyCapture2;
using namespace std;

void PrintBuildInfo();

void PrintCameraInfo(CameraInfo* pCamInfo);

void PrintError(Error error);

void FindCameras(BusManager& busMgr, PGRGuid& guid);

void Connect2Camera(PGRGuid& guid, Camera& cam);

void TakeImages(Camera& cam, int numImages, string type, string name="empty");

void TakeImages_in_Motion(HANDLE& serialHandle, Camera& cam, int numImages, string type);

void DisconnectCamera(Camera& cam);
