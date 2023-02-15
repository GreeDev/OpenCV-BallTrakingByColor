#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "MySpecialLineDrawer.h"

int main() {
	cv::namedWindow("SimpleBallTracker", cv::WINDOW_AUTOSIZE);
	
	cv::VideoCapture cap(0);

	if (!cap.isOpened()) {
		std::cout << "Camera not opened" << std::endl;
		return -1;
	}

	MySpecialLineDrawer lineDrawer(30);

	while (true) {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			std::cout << "cannot capture image" << std::endl;
			exit(-1);
		}

		cv::Mat targetImg;
		frame.copyTo(targetImg);
		cv::medianBlur(targetImg, targetImg, 5);

		// - find area of ball by color(HSV)
		cv::Mat hsvImg;
		cv::cvtColor(targetImg, hsvImg, cv::COLOR_BGR2HSV);
		const cv::Scalar hsvLow = { 30, 70, 50 }, hsvHigh = { 40, 255, 255 };
		cv::inRange(hsvImg, hsvLow, hsvHigh, targetImg);

		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(targetImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		
		if (!contours.empty()) {
			std::vector<cv::Point> ballArea;

			for (size_t i = 0; i < contours.size(); i++) {
				// - get contour of ball by area
				//if (cv::contourArea(contours[i]) > 100)
				//	ballArea.insert(ballArea.end(), contours[i].begin(), contours[i].end());
			 
				// - get contour of ball by size of points
				if (contours[i].size() > 30)
					ballArea.insert(ballArea.end(), contours[i].begin(), contours[i].end());
			}

			if (!ballArea.empty()) {
				// - predict area of ball by finding enclosing circle
				cv::Point2f center;
				float radius;
				cv::minEnclosingCircle(ballArea, center, radius);
				cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 2);
				lineDrawer.push(center);
			}
		}
		
		// draw lines at current frame
		lineDrawer.drawLines(frame);
		cv::imshow("SimpleBallTracker", frame);

		if (cv::pollKey() == 27) {
			break;
		}
	}

	cv::destroyWindow("SimpleBallTracker");

	return 0;
}