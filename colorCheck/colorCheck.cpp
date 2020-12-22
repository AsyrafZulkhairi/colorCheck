// colorCheck.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp> // imshow(); waitKey();
#include <opencv2/imgcodecs.hpp> // imread();
#include <opencv2/imgproc.hpp> // matchTemplate();

void colorCheck(cv::Mat img_bgr, cv::Rect roi, int v_min, int v_max, double& score);
void getColorRange(cv::Mat img_bgr, cv::Rect roi, int& v_min, int& v_max);

int main()
{
    cv::Mat img_bgr = cv::imread("input.jpg", 1);

	// Sample roi for Reference
    cv::Rect rectROI(20, 20, 20, 20);

	// Get Color Range
	int v_min, v_max;
	getColorRange(img_bgr, rectROI, v_min, v_max);
	std::cout << "V_min:" << v_min << "  V_max:" << v_max << std::endl;

	// Sample roi for check
	cv::Rect rectCheck(55, 20, 20, 20);
	
	// Color Check
	double score;
	colorCheck(img_bgr, rectCheck, v_min, v_max, score);
	std::cout << "Score:" << score << std::endl;

	// Display rect positions
	cv::rectangle(img_bgr, rectROI, cv::Scalar(0, 255, 255), 1, 8, 0); //Yellow color
	cv::imshow("Reference roi", img_bgr);
	cv::rectangle(img_bgr, rectCheck, cv::Scalar(255, 0, 0), 1, 8, 0); //Blue color
	cv::imshow("Color Check roi", img_bgr);

	cv::waitKey(0);
}

void colorCheck(cv::Mat img_bgr, cv::Rect roi, int v_min, int v_max, double& score) {
	
	cv::Mat img_hsv;
	cv::cvtColor(img_bgr, img_hsv, CV_BGR2HSV);
	int x_min = roi.x, y_min = roi.y, x_max = roi.x + roi.width, y_max = roi.y + roi.height;
	int in_range = 0; int count = 0;
	for (int y = y_min; y < y_max; y++) {
		for (int x = x_min; x < x_max; x++) {

			count++;
			cv::Vec3b hsv = img_hsv.at<cv::Vec3b>(y, x);
			int V = hsv.val[2];
			if (V >= v_min && V <= v_max)
				in_range++;

		}
	}

	// Calculate Score
	score = double(in_range) / (double(roi.width) * roi.height);
}

void getColorRange(cv::Mat img_bgr, cv::Rect roi, int& v_min, int& v_max) {

	cv::Mat img_hsv;
	cv::cvtColor(img_bgr, img_hsv, CV_BGR2HSV);
	int x_min = roi.x, y_min = roi.y, x_max = roi.x + roi.width, y_max = roi.y + roi.height;
	v_min = 255, v_max = 0; //set initial value
	for (int y = y_min; y < y_max; y++) {
		for (int x = x_min; x < x_max; x++) {

			cv::Vec3b hsv = img_hsv.at<cv::Vec3b>(y, x);
			int V = hsv.val[2];
			if (V > v_max)
				v_max = V;
			else if (V < v_min)
				v_min = V;

		}
	}
}
