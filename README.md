# GIMBLE README

### OVERVIEW

This project uses a gimbal set up with a Flir camera to carry out object detection.

### Components

- Camera: Point Grey: CM3-U3-31S4M-CS
- CS to C lens Adapter
- Lens: Computar: M0814-MP2
- 2 Servo Gear: 32 Pitch 12 teeth -> Servocity: RSA32-2HS-12
- 2 Servos for Gimbal: Hitec: HS-645MG
- 2-Axis Gimbal Kit: Servocity: SPT100H
- Controller for Servos: ServoCity: Micro Maestro 6-Channel USB

In addition it has two additional servos for lens controlled that they are not being used:

- 2 Servos for Lens: Hitec: HS-5085MG

### Key Features

- The communication with the servos is done via USB-COM. A SerialHandle file is created at the beginning of the progam. It is important that the right COM port is specified. 
- To control the camera, the FLIR SDK must eb installed. FlyCapture is the lirbary imported, and it currently does not support live video.
- All the Image Processing is done using OpenCV.
- There are 3 ways to tune the accuracy of the program:
	- The parameters minArea and rratio defined in const.cpp. MinArea relates to the minimun area of the contour to be considered a new obkject instead of noise. Rratio is used to compare similarities, hihger ratio, better perfomance but a bigger chance of missing objects.
	- The size of the Kernel during the blur and dilation tasks also affects the accuracy.
- More information can be found in the source files.

### Required Libraries

- Targetver
- Flir SDK -> FLyCapture
- OpenCV
- OpenCv -> non free (used for edge detection - optional)

### Future Work

The scene is currently divided in 16 quadrants. If this number increases more, the final result would be better. Among the 16 quadrants only 6 are actually scanned since there is a lot of overlapping between them. 

However, because of the fixture, the range of motion of the Gimbal is limiited. This could be improved as well.
