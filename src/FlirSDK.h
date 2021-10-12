// Helper functions to connect to the FLIR camera

#pragma once

#include "stdafx.h"
#include "FlyCapture2.h"
#include <iostream>
#include <sstream>

using namespace FlyCapture2;
using namespace std;

void PrintBuildInfo();

void PrintCameraInfo(CameraInfo* pCamInfo);

