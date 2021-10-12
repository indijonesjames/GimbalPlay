// Implementation file for the servos functions

#include "servos.h"
#include "stdafx.h"

using namespace std;

int CuadrantServo1(int cuadrant)
{
	switch (cuadrant)
	{
	case 1: return 4000;
	case 2: return 5000;
	case 3: return 6000;
	case 4: return 7000;
	case 5: return 8000;
	case 6: return 4000;
	case 7: return 5000;
	case 8: return 6000;
	case 9: return 7000;
	case 10: return 8000;
	case 11: return 4000;
	case 12: return 5000;
	case 13: return 6000;
	case 14: return 7000;
	case 15: return 8000;
	default:
		cout << "\nCuadrant out of range!" << endl;
		return -1;
	}
}

int CuadrantServo2(int cuadrant)
{
	switch (cuadrant)
	{
	case 1: return 7000;
	case 2: return 7000;
	case 3: return 7000;
	case 4: return 7000;
	case 5: return 7000;
	case 6: return 6000;
	case 7: return 6000;
	case 8: return 6000;
	case 9: return 6000;
	case 10: return 6000;
	case 11: return 5000;
	case 12: return 5000;
	case 13: return 5000;
	case 14: return 5000;
	case 15: return 5000;
	default:
		cout << "\nCuadrant out of range!" << endl;
		return -1;
	}
}
void ServoInit(HANDLE& serialHandle)
{
	// Open serial port
	serialHandle = CreateFile(L"\\\\.\\COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	// Do some basic settings
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);
	GetCommState(serialHandle, &serialParams);
	serialParams.BaudRate = 9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = 1;
	serialParams.Parity = 0;
	SetCommState(serialHandle, &serialParams);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 1;
	timeout.ReadTotalTimeoutConstant = 1;
	timeout.ReadTotalTimeoutMultiplier = 1;
	timeout.WriteTotalTimeoutConstant = 1;
	timeout.WriteTotalTimeoutMultiplier = 1;
	SetCommTimeouts(serialHandle, &timeout);
}

void ServoConfig(HANDLE& serialHandle, int servoSpeed, int servoAccel, unsigned char servoNum)
{
	DWORD numwritten;
	unsigned char servostr[4];

	servostr[0] = 0x87;		// speed
	servostr[1] = servoNum;
	servostr[2] = ((unsigned char)(servoSpeed & 0x7f));
	servostr[3] = ((unsigned char)((servoSpeed >> 7) & 0x7f));
	WriteFile(serialHandle, servostr, sizeof(servostr), &numwritten, NULL);

	servostr[0] = 0x89;		// acceleration
	servostr[1] = servoNum;
	servostr[2] = ((unsigned char)(servoAccel & 0x7f));
	servostr[3] = ((unsigned char)((servoAccel >> 7) & 0x7f));
	WriteFile(serialHandle, servostr, sizeof(servostr), &numwritten, NULL);
}

void ServoMove(HANDLE& serialHandle, int servoPosition, unsigned char servoNum)
{
	DWORD numwritten;
	unsigned char servostr[4];

	servostr[0] = 0x84;
	servostr[1] = servoNum;
	servostr[2] = ((unsigned char)(servoPosition & 0x7f));
	servostr[3] = ((unsigned char)((servoPosition >> 7) & 0x7f));
	WriteFile(serialHandle, servostr, sizeof(servostr), &numwritten, NULL);
}

void ServosMoveCuadrant(HANDLE& serialHandle, int cuadrant)
{
	ServoMove(serialHandle, CuadrantServo1(cuadrant), 0);
	ServoMove(serialHandle, CuadrantServo2(cuadrant), 1);
	ServosFinish(serialHandle);
}

int ServoPosition(HANDLE& serialHandle, unsigned char servoNum)
{
	DWORD numwritten, numread;
	unsigned char servostr[2];
	unsigned int position[2];

	servostr[0] = 0x90;
	servostr[1] = servoNum;
	WriteFile(serialHandle, servostr, sizeof(servostr), &numwritten, NULL);
	ReadFile(serialHandle, position, 1, &numread, NULL);
	return(*position);
}

void PrintServosPositions(HANDLE& serialHandle, int servos[])
{
	unsigned char servoPosition;
	for (int i = 0; i < (sizeof(servos) / sizeof(servos[0])); i++) {
		servoPosition = ServoPosition(serialHandle, servos[i]);
		cout << "Position of servo " << to_string(servos[i]) << " is:" << endl;
		printf("%d\n", servoPosition);
	}
	return;
}

void ServosExit(HANDLE& serialHandle) {
	DWORD numwritten;
	unsigned char servostr[1];
	servostr[0] = 0xA2;
	WriteFile(serialHandle, servostr, sizeof(servostr), &numwritten, NULL);
}

unsigned char ServosReadState(HANDLE& serialHandle)
{
	DWORD numwritten, numread;
	unsigned char servostr[1];

	servostr[0] = 0x93;
	WriteFile(serialHandle, servostr, sizeof(servostr), &numwritten, NULL);
	ReadFile(serialHandle, servostr, 1, &numread, NULL);
	return(*servostr);
}

void ServosHome(HANDLE& serialHandle, int servos[]) {
	for (int i = 0; i < (sizeof(servos) / sizeof(servos[0])); i++) {
		ServoMove(serialHandle, 6000, servos[i]);
	}
}

void ServosFinish(HANDLE& serialHandle) {
	unsigned char servosStatus;
	int niter = 0;
	//printf("Servos in motion...\n");
	while (ServosReadState(serialHandle)) {
		niter++;
		servosStatus = ServosReadState(serialHandle);
		//printf("State:");
		//printf("%d\n", servosStatus);
	}
	//printf("Motion ended.\n");
}