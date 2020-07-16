#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdbool.h>

using namespace cv;
using namespace std;

// Use HoughCircles to detect the circles from the image
// image: The input image.
// circles: output
// method: Detection method
// dp: the Inverse ratio of accumulator resolution and image resolution.
// mindst : minimum distance between centers of detected circles
// param_1 and param_2 : method specific parameters.
// min_Radius : minimum radius of the circle to be detected.
// max_Radius : maximum radius of the circle to be detected.

// We assume that a full moon has the brightest pixels in the image.
// Find max brightness value from the image
int getMaxBrightness(Mat image)
{
	int maxBrightness = 0;
	for (int r = 0; r < image.rows; ++r)
	{
		uchar* ptr = image.ptr<uchar>(r); // row

		for (int c = 0; c < image.cols; ++c)  // col
		{
			int brightness = (int)ptr[c];
			if (maxBrightness < brightness)
			{
				maxBrightness = brightness;
			}	
		}
	}

	return maxBrightness;
}

// check if it is a full circle in the image
bool isFullCircle(Mat image, vector<Vec3f> circles, int index)
{
	int radius;
	int crow, ccol;

	radius = cvRound(circles[index][2]);
	crow = circles[index][1];
	ccol = circles[index][0];
	if (crow + radius >= image.rows || crow - radius < 0 || ccol + radius >= image.cols || ccol - radius < 0)
	{ 
		return false;
	}
	else
	{
		return true;
	}	
}

// check if the circle is bright enough to be the moon.
bool isBright(Mat image, vector<Vec3f> circles, int maxBrightness, int index)
{
	int radius;
	int crow, ccol;

	radius = cvRound(circles[index][2]);
	crow = circles[index][1];
	ccol = circles[index][0];

	int brightness;
	int sum = 0;
	int totalPixels = 0;
	for (int r = crow - radius ; r < crow + radius; ++r)
	{
		uchar* ptr = image.ptr<uchar>(r); // row
		for (int c = ccol - radius; c < ccol + radius; ++c)
		{
			totalPixels++;
			brightness = (int)ptr[c];

			// see how many pixels in the image are close to max brightness in the image 
			if (brightness < (int)(0.7 * maxBrightness))
			{
				sum++;
			}
		}	
	}

	// if the total number of pixels with less brightness is less than half of total pixels in the circle,
	// the circle has more consistent brightness.
	if (sum < (int) (0.2 * totalPixels))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	Mat image = imread("Moon.jpg");

	// Check for invalid input
	if (!image.data)                              
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	// convert the image to gray
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);

	//if necessary, reduce noise to avoid false circle detection
	//GaussianBlur(grayImage, grayImage, Size(9, 9), 2, 2);

	// Apply the Hough Transform to find the circles
	vector<Vec3f> circles;
	HoughCircles(grayImage, circles, HOUGH_GRADIENT, 1, grayImage.rows/8, 200, 10, 5, 200);

	cout << "Number of detected circles = " << circles.size() << endl;

	// If there are more than 1 circle, assume that the circle is the moon
	// Identify the index of the circle with the largest radius.

	// get max brightness from the image
	int maxBrightness = getMaxBrightness(grayImage);
	// Draw the circles detected
	for (int i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		int crow = circles[i][1];
		int ccol = circles[i][0];

		cout << endl;
		cout << "radius " << radius << " center pixel coord. =  row " << crow << " col " << ccol << endl;

		// if it is not a full circle in the image, ignore
		if (isFullCircle(grayImage,circles, i) == false)
		{		
			cout << "This circle " << " is ignored because it is not a full circle." << endl;
			continue;
		}
		// if the pixels in the full circle does not consistent brightness , ignore
		if (isBright(grayImage, circles, maxBrightness, i) == false)
		{
			cout << "This circle " << " is ignored because it doesn't have the consistent brightness." << endl;
			continue;
		}

		cout << "This circle " << " is a full moon unless there are other bright circles in the image." << endl;
		// draw circle center in blue
		circle(image, center, 3, Scalar(255, 0, 0), -1, 8, 0);
	
		// draw circle outline in red
		circle(image, center, radius, Scalar(0, 0, 255), 2, 8, 0);
	}

	// show the result
	imshow("Moon", image);
	waitKey(0);

	return 0;
}


