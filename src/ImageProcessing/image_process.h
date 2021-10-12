#pragma once


#include "..//stdafx.h"

using namespace std;

string GetMatType(const cv::Mat& mat);

string GetMatDepth(const cv::Mat& mat);

/**********************************************/
// Basic functions to open and display Images:
/*********************************************/

void open_image(cv::Mat mat, string name);

void open_images(cv::Mat mat1, cv::Mat mat2, string name);

/*********************************************************/
// Functions to find centers and centroids. 
/*********************************************************/

cv::Point find_center(cv::Mat& img);

cv::Point find_centroid(cv::Mat& img);

/*****************************************************************************/
// Functions to operate and handle contours of an image
// Contours are found with OpenCV findcontours function, 
//then processed with the helper functions below
/*****************************************************************************/

int getMaxAreaContourId(vector <vector<cv::Point>> contours);

double getMaxContoursArea(vector<vector<cv::Point>> contours);

bool QuickContourCheck(cv::Mat Mask, double smallest_area);

void drawBiggestContour(cv::Mat& Mask, string name);

void drawAllContours(cv::Mat& Mask, string name);

/***************************************************************************************/
// Functions to create and handle Masks. The mask can be easily found using the AbsDiff
// function from OpenCV. Then use the functions below to filter, process, and handle it.
/***************************************************************************************/

void threshold_3C(cv::Mat& foregroundMask, cv::Mat& diffImage, double threshold);

void compareMasks(cv::Mat mask1, cv::Mat mask2, cv::Mat& mask);

double findMax(cv::Mat diffImage);

double getSimilarity(const cv::Mat A, const cv::Mat B);

double findHistSimilarity(cv::Mat A, cv::Mat B);

/***********************************************************************************************************/
// Functions to compute edge, surface, and feature detection use SURF or ORB algorithms.
/***********************************************************************************************************/

void findUnmatchedKeypoints(cv::Mat& img, vector<cv::KeyPoint> keypoints_All, vector<cv::KeyPoint> keypoints_Matched, vector<cv::KeyPoint>& unmatched);

void FeaturesAnalyzer(cv::Mat Background, cv::Mat Foreground, cv::Mat& Foreground_Aligned);

void template_check();

/****************************************************************************************/
// Main Image processing functions. They use a combination of the functions above to
// calibrate and detect objects.
/****************************************************************************************/

void find_next_cuadrant(int& current_cuadrant, int w, int h);

void display_object(int current_cuadrant);

void calibrate_cuadrant(int current_cuadrant);

bool find_newObject(cv::Point& centroid, int& current_cuadrant);


/*******************************************************************************************/
// Testing ideas
/******************************************************************************************/

//void cluster();

//void canny_edge();