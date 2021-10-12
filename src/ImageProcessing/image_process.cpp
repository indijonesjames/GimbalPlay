#include"image_process.h"
#include "stdafx.h"

using namespace std;
using namespace cv;
using namespace xfeatures2d;

extern double rratio;
extern double minArea;

string GetMatDepth(const cv::Mat& mat)
{
	const int depth = mat.depth();

	switch (depth)
	{
	case CV_8U:  return "CV_8U";
	case CV_8S:  return "CV_8S";
	case CV_16U: return "CV_16U";
	case CV_16S: return "CV_16S";
	case CV_32S: return "CV_32S";
	case CV_32F: return "CV_32F";
	case CV_64F: return "CV_64F";
	default:
		return "Invalid depth type of matrix!";
	}
}

string GetMatType(const cv::Mat& mat)
{
	const int mtype = mat.type();

	switch (mtype)
	{
	case CV_8UC1:  return "CV_8UC1";
	case CV_8UC2:  return "CV_8UC2";
	case CV_8UC3:  return "CV_8UC3";
	case CV_8UC4:  return "CV_8UC4";

	case CV_8SC1:  return "CV_8SC1";
	case CV_8SC2:  return "CV_8SC2";
	case CV_8SC3:  return "CV_8SC3";
	case CV_8SC4:  return "CV_8SC4";

	case CV_16UC1: return "CV_16UC1";
	case CV_16UC2: return "CV_16UC2";
	case CV_16UC3: return "CV_16UC3";
	case CV_16UC4: return "CV_16UC4";

	case CV_16SC1: return "CV_16SC1";
	case CV_16SC2: return "CV_16SC2";
	case CV_16SC3: return "CV_16SC3";
	case CV_16SC4: return "CV_16SC4";

	case CV_32SC1: return "CV_32SC1";
	case CV_32SC2: return "CV_32SC2";
	case CV_32SC3: return "CV_32SC3";
	case CV_32SC4: return "CV_32SC4";

	case CV_32FC1: return "CV_32FC1";
	case CV_32FC2: return "CV_32FC2";
	case CV_32FC3: return "CV_32FC3";
	case CV_32FC4: return "CV_32FC4";

	case CV_64FC1: return "CV_64FC1";
	case CV_64FC2: return "CV_64FC2";
	case CV_64FC3: return "CV_64FC3";
	case CV_64FC4: return "CV_64FC4";

	default:
		return "Invalid type of matrix!";
	}
}

/**********************************************/
// Basic functions to open and display Images:
/*********************************************/


// opens one image and waits for keystroke to continue
void open_image(Mat mat, string name) 
{
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, mat);
	waitKey();
	destroyWindow(name);
}

// open two images in parallel and waits for keystroke
void open_images(Mat mat1,Mat mat2, string name)
{
	Mat mat;
	namedWindow(name, WINDOW_NORMAL);
	hconcat(mat1, mat2, mat);
	imshow(name, mat);
	waitKey();
	destroyWindow(name);
}

/*********************************************************/
// Functions to find centers and centroids. 
/*********************************************************/

// Finds the center of any image (does not matter size or content)
Point find_center(Mat& img)
{
	Mat blank = Mat::ones(img.rows, img.cols, CV_8UC1);
	Moments m = moments(blank, true);
	Point center(m.m10 / m.m00, m.m01 / m.m00);
	return center;
}

// Finds the centroid of the image. It can be used to find the centroid of an object if
// a mask of this one is passed.
Point find_centroid(Mat& img)
{
	Moments m = moments(img, true);
	Point centroid(m.m10 / m.m00, m.m01 / m.m00);
	return centroid;
}

/*****************************************************************************/
// Functions to operate and handle contours of an image
// Contours are found with OpenCV findcontours function, 
//then processed with the helper functions below
/*****************************************************************************/

// From a set of contours it finds the ID of the biggest one
int getMaxAreaContourId(vector <vector<cv::Point>> contours)
{
	double maxArea = 0;
	int maxAreaContourId = -1;
	for (int j = 0; j < contours.size(); j++) {
		double newArea = contourArea(contours.at(j));
		//cout << newArea << endl;
		if (newArea > maxArea) {
			maxArea = newArea;
			maxAreaContourId = j;
		}
	}
	return maxAreaContourId;
}

// From a set of contours it finds the biggest the area
double getMaxContoursArea(vector<vector<cv::Point>> contours)
{
	double maxArea = 0;
	int maxAreaContourId = -1;
	for (int j = 0; j < contours.size(); j++) {
		double newArea = contourArea(contours.at(j));
		if (newArea > maxArea) {
			maxArea = newArea;
			maxAreaContourId = j;
		}
	}
	return maxArea;
}

// This function does a quick check to make sure if any contour is bigger than a minimun area threshold
bool QuickContourCheck(Mat Mask, double smallest_area)
{
	// find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	findContours(Mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	Mat drawing = Mat::zeros(Mask.size(), CV_8UC3);

	// get Biggest countour area
	double area = getMaxContoursArea(contours);
	cout << "Biggest area: " << area << endl;
	return (area > smallest_area);
}

// It finds all contours of an image or mask and draws the biggest one
void drawBiggestContour(Mat& Mask, string name) 
{
	//dilate
	Mat dilate_out;
	dilate(Mask, dilate_out, getStructuringElement(MORPH_RECT, Size(5, 5)));
	open_image(dilate_out, name + "-dilate");

	// find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	// Morphological operations to bound the object as much as possible
	morphologyEx(dilate_out, dilate_out, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(9, 9)));
	findContours(dilate_out, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	// Draw bounding rectangle
	Rect rect = boundingRect(contours[getMaxAreaContourId(contours)]);

	// draw biggest contour
	Mat drawing = Mat::zeros(dilate_out.size(), CV_8UC3);
	rectangle(drawing, rect, Scalar((0, 255, 255)));
	Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	drawContours(drawing, contours, getMaxAreaContourId(contours), color, 2, LINE_8, hierarchy, 0);
	open_image(drawing, name + "-biggest-contour");
}

// Finds and draws all contours of an image or mask. It uses dilate and erode to fill the gaps and 
// output the most accurate object representation.
void drawAllContours(Mat& Mask, string name)
{
	//canny edge detection to find contours
	Mat canny_out;
	Canny(Mask, canny_out, 10, 30, 3);
	//open_image(canny_out, name + "-canny");

	//dilate
	Mat dilate_out;
	dilate(canny_out, dilate_out, getStructuringElement(MORPH_RECT, Size(5, 5)));
	//open_image(dilate_out, name + "-dilate");

	// find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	findContours(dilate_out, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	Mat drawing = Mat::zeros(canny_out.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	open_image(drawing, name + "-contours");
}



/***************************************************************************************/
// Functions to create and handle Masks. The mask can be easily found using the AbsDiff
// function from OpenCV. Then use the functions below to filter, process, and handle it.
/***************************************************************************************/

/* This function applies a binary threshold to a mask. This means that every pixel with 
a higher value than the threshold will be set to 255 (white), and everything lower 
will be left as it is. Normally foregroundMask is a mask of zeros and diffImage is the
image that you want to apply the threshold to.

The pixel value is calculated finding the distance of each channel value. If you need
to do this for a 1 channel image, just use the threshold openCV function.
*/
void threshold_3C(Mat& foregroundMask, Mat& diffImage, double threshold)
{
	double dist;
	for (int j = 0; j < diffImage.rows; j++)
		for (int i = 0; i < diffImage.cols; i++)
		{
			Vec3b pix = diffImage.at<Vec3b>(j, i);

			dist = (pix[0] * pix[0] + pix[1] * pix[1] + pix[2] * pix[2]);
			dist = sqrt(dist);

			if (dist > threshold)
			{
				foregroundMask.at<unsigned char>(j, i) = 255;
			}
		}
	return;
}

/*Very useful function to filter a mask using a background one. The way it works is that if
mask1 is set 255 and mask2 is 0, then the output mask will be set to 255.

Parameters:
- Mask1: mask to be filtered
- Mask2: filter calculated during calibration
- Mask: output mask, it should be passed in as a mask of zeros to work best.
*/
void compareMasks(Mat mask1, Mat mask2, Mat& mask)
{
	for (int j = 0; j < mask1.rows; j++)
		for (int i = 0; i < mask1.cols; i++)
		{
			unsigned char value1 = mask1.at<unsigned char>(j, i);
			unsigned char value2 = mask2.at<unsigned char>(j, i);
			if (value1 == 255 && value2 == 0)
			{
				mask.at<unsigned char>(j, i) = 255;
			}
		}
}

// Finds max value in a Mat (does not need to be a mask or image, since it gets reshaped.
double findMax(Mat diffImage)
{
	Mat temp = diffImage;
	temp.reshape(1);
	double minVal;
	double maxVal;
	minMaxIdx(temp, &minVal, &maxVal);
	return maxVal;
}

/*Computes the similarity between two masks. And returns a double value.

It is done by calculate the norm between both images. There are different norms
that can be used for this purpose. I personally find CV_L1 to yield the best results,
although in the past I have also used CV_L2.
*/
double getSimilarity(const Mat A, const Mat B) 
{
	if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols) {
		// Calculate the L2 relative error between images.
		double errorL2 = norm(A, B, CV_L1);
		// Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
		double similarity = errorL2 / (double)(A.rows * A.cols);
		return similarity;
	}
	else {
		//Images have a different size
		return 100000000.0;  // Return a bad value
	}
}

// Calculate the histogram of two images, normalizes it, and calculates the similarity between them. 
// for greyscale images I find is not accurate enough to be relied on. However, for HSV or RGB should
// work better.
double findHistSimilarity(Mat A, Mat B)
{
	double similarity;
	int channels[] = { 0 };
	int bins = 256;
	int histSize[] = { 256 };
	float lranges[] = { 0,256 };
	const float* ranges[] = { lranges };
	Mat histA;
	Mat histB;
	calcHist(&A, 1, channels, Mat(), histA, 1, histSize, ranges, true, false);
	normalize(histA, histA, 0, 1, NORM_MINMAX, -1, Mat());
	calcHist(&B, 1, channels, Mat(), histB, 1, histSize, ranges, true, false);
	normalize(histB, histB, 0, 1, NORM_MINMAX, -1, Mat());
	similarity = compareHist(histA, histB, 1);
	cout << "Hist Similarity: " << similarity << endl;
	return similarity;
}

/***********************************************************************************************************/
// Functions to compute edge, surface, and feature detection use SURF or ORB algorithms.
/***********************************************************************************************************/

// Finds the unmatched points as expected, but unless the feature matching algorithm had a very high success ratio, the results will be besically useless
void findUnmatchedKeypoints(Mat& img, vector<KeyPoint> keypoints_All, vector<KeyPoint> keypoints_Matched, vector<KeyPoint>& unmatched)
{
	// find the difference in the keypoints
	vector<Point2f> initial_points;
	vector<Point2f> matched_points;
	vector<Point2f> difference_points;
	KeyPoint::convert(keypoints_All, initial_points);
	KeyPoint::convert(keypoints_Matched, matched_points);

	cout << "Sizes: " << keypoints_All.size() << " vs " << keypoints_Matched.size() << endl;

	for (size_t i = 0; i < initial_points.size(); i++)
	{
		if (find(matched_points.begin(), matched_points.end(), initial_points[i]) == matched_points.end())
			difference_points.push_back(initial_points[i]);
	}

	cout << difference_points.size() << endl;

	// draw the points without a match;
	KeyPoint::convert(difference_points, unmatched);
	Mat unmatch;
	drawKeypoints(img, unmatched, unmatch);
	open_image(unmatch, "unmatched");
	Mat matched;
	drawKeypoints(img, keypoints_Matched, matched);
	open_image(matched, "matched");
	Mat All;
	drawKeypoints(img, keypoints_All, All);
	open_image(All, "ALL");
}

/*Main function to carry feature detection. Given two masks it finds the keypoints and descriptors
of each. Then, it uses this information to calculate the matching points between both images.
There are many options available to carry this task:

- For keypoint and descriptor detection I find SURF to be the best, however ORB is a perfectly free alternative.
- For matching, I find Brute Force to be the faster, however instead of using the regular matching method, I recommend
using knnmatch to find two sets of matches for each point. Then use this information to find more accurate matches by 
applying the ratio test. If the distance of the first match is the half of the distance to second the match, this is
a good indicator that it will be a good match. In fact, using 0.7 will aldo yield perfectly valid matches.

Finally all the information from the good matches is used to find an Homography between the locations of the matching points
in the foreground and background. With this homography and allignment calculation can be done to warp the foreground image
and rotate it to match the orientation of the background.

However, this is one just of many applications for the algorithm found below.
*/
void FeaturesAnalyzer(Mat Background, Mat Foreground, Mat& Foreground_Aligned)
{
	try {
		// detecting keypoints
		int minHessian = 200;
		Ptr<SURF> detector = SURF::create(minHessian);
		vector<KeyPoint> keypoints_Background, keypoints_Foreground, keypoints_Matched, keypoints_Unmatched;
		Mat descriptors_Background, descriptors_Foreground;
		detector->detectAndCompute(Background, noArray(), keypoints_Background, descriptors_Background);
		detector->detectAndCompute(Foreground, noArray(), keypoints_Foreground, descriptors_Foreground);

		// matching descriptors
		Ptr<BFMatcher> matcher = BFMatcher::create(NORM_L2, false);
		//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
		//vector<DMatch> matches;
		vector<vector<DMatch>> matches;
		matcher->knnMatch(descriptors_Background, descriptors_Foreground, matches,2);
		
		// find candidates based on the match results using the distance ratio test
		vector<DMatch> good_matches;
		vector<DMatch> non_matches;
		vector<DMatch> empty;
		float Ratio = 0.5f; // 0.7 is the recommended normally, but we need very high accuracy
		for (size_t i = 0; i < matches.size(); ++i)
		{
			if (matches[i].size() < 2)
				continue;
			const DMatch& m1 = matches[i][0];
			const DMatch& m2 = matches[i][1];

			if (m1.distance <= Ratio * m2.distance)
				good_matches.push_back(m1);
			else {
				non_matches.push_back(m1);
			}
		}

		// drawing the results
		Mat img_matches;
		drawMatches(Background, keypoints_Background, Foreground, keypoints_Foreground, good_matches, img_matches);
		open_image(img_matches, "Matching Features");

		// now find the matching points location
		vector<Point2f> background_pts;
		vector<Point2f> foreground_pts;
		for (size_t i = 0; i < good_matches.size(); ++i)
		{
			background_pts.push_back(keypoints_Background[good_matches[i].queryIdx].pt);
			foreground_pts.push_back(keypoints_Foreground[good_matches[i].trainIdx].pt);
		}

		// Find HomoGraphy for Image Allignment
		Mat Homography = findHomography(background_pts, foreground_pts, RANSAC);

		// Warp the Image
		warpPerspective(Foreground, Foreground_Aligned, Homography, Background.size());
	}
	catch (cv::Exception& e)
	{
		cerr << e.msg << endl;
	}
}

// Quick test to show the functionality of the feature detection algorithm
void template_check()
{
	Mat object = imread("images\\Cuadrant-15\\Calibrate-2.tiff", IMREAD_GRAYSCALE);
	Mat match = imread("images\\templates\\test.tif", IMREAD_GRAYSCALE);
	Mat result;
	FeaturesAnalyzer(object, match, result);
}

/****************************************************************************************/
// Main Image processing functions. They use a combination of the functions above to
// calibrate and detect objects.
/****************************************************************************************/


// Finds the most suitable cuadrant given the centroid of the detected object
void find_next_cuadrant(int& current_cuadrant, int w, int h)
{

	if (w >= 500)
	{
		if (current_cuadrant == 5 || current_cuadrant == 10 || current_cuadrant == 15) current_cuadrant = current_cuadrant;
		else current_cuadrant = current_cuadrant + 1;
	}

	else if (w <= -500)
	{
		if (current_cuadrant == 1 || current_cuadrant == 6 || current_cuadrant == 11) current_cuadrant = current_cuadrant;

		else current_cuadrant = current_cuadrant - 1;
	}

	if (h >= 500 && current_cuadrant < 11) current_cuadrant = current_cuadrant + 5;

	else if (h <= -500 && current_cuadrant > 5) current_cuadrant = current_cuadrant - 5;

	return;
}

// Displays the detected object
void display_object(int current_cuadrant)
{
	Mat Result = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Result-2.tiff", IMREAD_GRAYSCALE);
	open_image(Result, "Detected Object");
	return;
}

/* This function calculates the filter masks used to remove noise and allignment uses when taking the images:

It should be called during the second sweep to find the calibration images. During this sweep the new image acquired will
be compared to the ones found during the first sweep. The same approach as the one used to find new objects will be used.
However, this time we expect to not find anything, so any potential results are defined as noise that neeeds to be filtered.

The first step is to find the max similarity (higher is worse) between each of the calibration images and store thde max value.
This one will be used as the baseline similarity between the background and foreground, and if the value is not bigger, we will
assume that not new objects have entered the scenee in that cuadrant.

After that, the absolute different between both images is found and saved. From here a mask where every noise or illumination difference
is set to 255 is found. This mask is saved and will be used with the compare masks function described above filter any noise.
*/
void calibrate_cuadrant(int current_cuadrant) 
{
	cout << "Calculating filters for Cuadrant " << current_cuadrant << "..." << endl;
	Mat image1 = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate-0.tiff", IMREAD_GRAYSCALE);
	Mat image2 = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate-1.tiff", IMREAD_GRAYSCALE);
	Mat image3 = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate-2.tiff", IMREAD_GRAYSCALE);
	Mat image = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate2-2.tiff",IMREAD_GRAYSCALE); // new image
	//open_images(image1, image, "Comparison");

	// Similarity check
	vector<double> similarity;
	GaussianBlur(image1, image1, Size(5,5), 3);
	GaussianBlur(image, image, Size(5, 5), 3);
	similarity.push_back(getSimilarity(image, image1));
	similarity.push_back(getSimilarity(image, image2));
	similarity.push_back(getSimilarity(image, image3));
	double maxSim = *max_element(similarity.begin(), similarity.end());
	cout << "Max similarity found: " << maxSim << endl;
	SetCuadrantSimilarity(current_cuadrant, maxSim);

	// Find missmatches between images
	Mat diffImage;
	absdiff(image1, image, diffImage);
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\CalibrationAbsDiff.tiff", diffImage);
	//open_image(diffImage, "AbsDiff");

	// Find the max value and save it
	double maxVal = findMax(diffImage);
	SetCuadrantThreshold(current_cuadrant, maxVal);

	// Find the mask
	Mat Mask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
	threshold(diffImage, Mask, 3, 255, THRESH_BINARY);
	//open_image(Mask, "Mask");
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\CalibrationMask.tiff", Mask);
}

/*Main function to find the any new object that enters the scene/cuadrant. The algorithm uses three images:

- The Calibration Image from the first sweep.
- The calibration Image from the second sweep.
- The image taken during the scan task.

Each image is then blurred to reduce the noise. The size of the kernel is one of the main tools to increase or 
decrease the accuracy of the process. In the past a 9 by 9 was used. However, after the improvements to the camera
saturation and exposure levels, a 5 by 5 was good enough to filter out all noise.

After the blur, the similiraity levels between the scan image and the two calibration ones are found and compare to
the value stored during the calibration task. Here, a new accuracy input is introduced, rratio (declared in const.cpp).
This variable controls the aceptable difference between the calibration similarity and the scanned one. The higher the ratio
the higer the acceptable difference. This can helps speeding up the scanning tasks, but if too large it can be the reason
why new objects are missed.

If the similarity checks indicates that a new object has entered the scene, the next step is to calculate the difference
between the scan image and the calibration background. A mask will be found from the difference image, and
the calibration mask filter will be applied to it.

After this, a last noise check will be done. This one is done using the Quick Contour Check to make sure that a contour
with an area bigger than a fixed threshold is found. This is because noise will have a very small area and can be filtered
out easily this way. This task introduces the last accuracy variable, MinArea, which is declared in const.cpp and can be 
tuned as needed.

After this last check, if passed we assume that there is a new object in the cuadrant. The remaining operations are used to
extract as much information of the object as possible. A morph dilate is used to fill the possible blanks
in the object, then the counters of the image are found and the biggest one is selected (which should be the
object if the algorithm worked). During the contour finding operation an open close operation is also used with 
the same goal of finding the whole volume of the object. 

Once the object5 has been isolated as much as possible, the centroid of the mask is found and compared to the actual
center of the image. From this comparison the most suitable cuadrant to take the image is found.
*/
bool find_newObject(Point& centroid, int& current_cuadrant)
{
	cout << "\nScanning Cuadrant: " << current_cuadrant << endl;
	//cout << "Accuracy parameters are: MinArea = " << minArea << " and Similarity ratios = " << rratio << endl;
	Mat Background = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate-2.tiff",IMREAD_GRAYSCALE);
	Mat Background2 = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Calibrate2-2.tiff", IMREAD_GRAYSCALE);
	Mat Foreground = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\Scan-0.tiff", IMREAD_GRAYSCALE);
	//open_images(Background, Foreground, "Comparison");

	// Blur each image to reduce noise 
	//bilateralFilter(background, backgroundImage, 50, 50, 5);
	//bilateralFilter(current, currentImage, 50, 50, 5);
	GaussianBlur(Background, Background, Size(5, 5), 3);
	GaussianBlur(Background2, Background2, Size(5, 5), 3);
	GaussianBlur(Foreground, Foreground, Size(5, 5), 3);

	// First quick check if there is any relevant differences in the images
	double calibSimilarity = GetCuadrantSimilarity(current_cuadrant);
	double similarity = getSimilarity(Background, Foreground);
	double similarity2 = getSimilarity(Background2, Foreground);
	//printf("Calibration similarity results were: %f...\n", calibSimilarity);
	//printf("First similarity results are: %f, %f...\n", similarity, similarity2);

	if (similarity <= rratio* calibSimilarity || similarity2 <= rratio*calibSimilarity)
	{
		cout << "No new object found in Cuadrant " << current_cuadrant << ", moving to the next." << endl;
		return false;
	}


	// Find new objects
	Mat diffImage;
	absdiff(Background, Foreground, diffImage);
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\diffImage.tiff", diffImage);
	//open_image(diffImage, "AbsDiff");

	// Apply Calibration Filter
	Mat CaliDiffImage = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\CalibrationAbsDiff.tiff", IMREAD_GRAYSCALE);
	//open_images(CaliDiffImage, diffImage, "AbsDiff-Compare");
	Mat FinalDiff;
	absdiff(CaliDiffImage, diffImage,FinalDiff);
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\FinalDiff.tiff", FinalDiff);

	// Calculate the Mask
	Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);
	threshold(FinalDiff, foregroundMask, 15, 255, THRESH_BINARY);
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\foregroundMask.tiff", foregroundMask);

	// Filter the mask noise
	Mat CalibMask = imread("images\\Cuadrant-" + to_string(current_cuadrant) + "\\CalibrationMask.tiff", IMREAD_GRAYSCALE);
	//open_images(CalibMask, foregroundMask, "Masks compare");
	Mat absMask = Mat::zeros(foregroundMask.rows, foregroundMask.cols, CV_8UC1);;
	compareMasks(foregroundMask, CalibMask, absMask);
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\absMask.tiff", absMask);
	//open_images(foregroundMask, absMask, "AbsMask");

	// Last check is find the biggest contour at the moment and making sure is bigger than a threshold size (it will need to be changed as needed)
	if (QuickContourCheck(absMask, minArea) == false)
	{
		cout << "No new object found in Cuadrant " << current_cuadrant << ", moving to the next." << endl;
		return false;
	}

	// At this point we assume there is a new object, and we try to identify it
	// Apply a Morphological Transformation to fill the missing parts
	Mat MorphMask;
	morphologyEx(absMask, MorphMask, MORPH_DILATE, getStructuringElement(MORPH_RECT, Size(5, 5)));
	imwrite("images\\Cuadrant-" + to_string(current_cuadrant) + "\\morphmask.tiff", MorphMask);
	//open_images(absMask, MorphMask, "MorphMask");

	// Draw the biggest contour (it will use appply another morphological transformation)
	drawBiggestContour(MorphMask, "Cuadrant" + to_string(current_cuadrant));

	//find centroid
	centroid = find_centroid(absMask);
	Point center = find_center(absMask);
	cout << "Center of the image: " << center.x << "," << center.y << endl;
	cout << "Centroid of the object: " << centroid.x << "," << centroid.y << endl;

	// Only return positive values
	if ((centroid.x < 0) || (centroid.y < 0)) {
		centroid = Point(0, 0);
		return false;
	}

	// Calculate next cuadrant to move to
	int w = centroid.x - center.x;
	int h = centroid.y - center.y;
	cout << " W and H: " << w << ", " << h << endl;
	find_next_cuadrant(current_cuadrant, w, h);

	cout << "\nObjected detected at: " << centroid.x << "," << centroid.y << endl;
	cout << "Size of the Image: " << Foreground.cols << "," << Foreground.rows << endl; 
	cout << "Best Cuadrant is: " << current_cuadrant << endl;
	return true;
}

// *************************************************** Test Ideas ***************************************************************************
// ******************************************************************************************************************************************

/*
void cluster()
{
	const int channels[] = { 0, 1, 2 };
	const int histSize[] = { 32, 32, 32 };
	const float rgbRange[] = { 0, 256 };
	const float* ranges[] = { rgbRange, rgbRange, rgbRange };

	Mat hist;
	Mat im32fc3, backpr32f, backpr8u, backprBw, kernel;

	Mat im = imread("images\\templates\\foreground.tif");

	im.convertTo(im32fc3, CV_32FC3);
	calcHist(&im32fc3, 1, channels, Mat(), hist, 3, histSize, ranges, true, false);
	calcBackProject(&im32fc3, 1, channels, hist, backpr32f, ranges);

	double minval, maxval;
	minMaxIdx(backpr32f, &minval, &maxval);
	threshold(backpr32f, backpr32f, maxval / 32, 255, THRESH_TOZERO);
	backpr32f.convertTo(backpr8u, CV_8U, 255.0 / maxval);
	threshold(backpr8u, backprBw, 10, 255, THRESH_BINARY);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

	dilate(backprBw, backprBw, kernel);
	morphologyEx(backprBw, backprBw, MORPH_CLOSE, kernel, Point(-1, -1), 2);

	backprBw = 255 - backprBw;

	morphologyEx(backprBw, backprBw, MORPH_OPEN, kernel, Point(-1, -1), 2);
	erode(backprBw, backprBw, kernel);

	Mat mask(backpr8u.rows, backpr8u.cols, CV_8U);

	mask.setTo(GC_PR_BGD);
	mask.setTo(GC_PR_FGD, backprBw);

	Mat bgdModel, fgdModel;
	grabCut(im, mask, Rect(), bgdModel, fgdModel, GC_INIT_WITH_MASK);

	Mat fg = mask == GC_PR_FGD;

	imshow("res", fgdModel);
	waitKey();
	return;
}

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

Mat src, src_gray;
Mat dst, detected_edges;
int lowThreshold = 0;
const int max_lowThreshold = 100;
const int rratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

static void CannyThreshold(int, void*)
{
	blur (src_gray, detected_edges, Size(3,3));
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * rratio, kernel_size);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}
void canny_edge()
{
	src = imread("images\\templates\\Scan-0.tiff", IMREAD_GRAYSCALE); // Load an image
	namedWindow(window_name, WINDOW_AUTOSIZE);
	src_gray = src;
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0, 0);
	waitKey(0);
	return;
}
*/