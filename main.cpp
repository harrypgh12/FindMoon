#include <opencv2\opencv.hpp>
#include <iostream>

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
	HoughCircles(grayImage, circles, HOUGH_GRADIENT, 1, grayImage.rows/8, 200, 10, 10, 20);

	cout << "Number of detected circles = " << circles.size() << endl;

	// If there are more than 1 circle, assume that the circle is the moon
	// Identify the index of the circle with the largest radius.

	// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));

		int radius = cvRound(circles[i][2]);
		cout << "Moon radius " << radius << endl;
		cout << "Moon center in pixel coord. " << circles[i][0] << " " << circles[i][1] << endl;
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


