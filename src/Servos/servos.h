#pragma once

// Servo Ctrl:  using USB-COM to communicate to Pololu Micro Maesto 6, to drive 2 HiTec HS-645MG servos for gimbal and
//				2 HS-5085MG servos (modified for continuous rotation) for camera lens
//					- to enable this code to control the Maestro, need to first use Pololu Maestro Control Center GUI to 
//					  configure to use USB Dual Port mode under Serial Settings 
//					- need to set correct COM port for specific Windows mounting in ServoInit() ... use the Pololu Micro Maestro Command Port shown in 
//					  Windows Device Manager (typ the first of two COM ports associated with the USB-Maestro)
//		- servo 0 & 1 = gimbal:  Hitech HS-645MG, nominal is 6000, limits are 2000-10000 for ~180+ degree motion
//		- servo 2 & 3 = camera lens: Hitech HS-5085MG, nominal is 6000, limits are 3000-9000 for ~135+ degree motion
//						- both servos have uncontrolled speed and acceleration at 0;  controls begin at 1 and become more aggressive as # rises
//						- HS-5085MG modified for continuous rotation is stationary at 6000 and rate of motion determined by how far 
//						  target is from 6000
//

#include "..//stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <intrin.h>

// Start the Serial Communication
void ServoInit(HANDLE &serialHandle);

// Configure each servo
void ServoConfig(HANDLE& serialHandle, int servoSpeed, int servoAccel, unsigned char servoNum);

// Move Servo --> exits automatically
void ServoMove(HANDLE& serialHandle, int servoPosition, unsigned char servoNum);

// Move and wait for servos to reach the cuadrant
void ServosMoveCuadrant(HANDLE& serialHandle, int cuadrant);

// Returns the current pulse transmitted to the servo (can be used to infer the position)
int ServoPosition(HANDLE& serialHandle, unsigned char servoNum);

// Prints all given servos positions
void PrintServosPositions(HANDLE& serialHandle, int servos[]);

// Sends the servos home, stops the connection. 
void ServosExit(HANDLE& serialHandle);

// Reads current state --> 1 if moving, 0 otherwise.
unsigned char ServosReadState(HANDLE& serialHandle);

// Wait for the movement to end (does not work if speed is not defined)
void ServosFinish(HANDLE& serialHandle);

// Send servos to their initial position of 6000
void ServosHome(HANDLE& serialHandle, int servos[]);



