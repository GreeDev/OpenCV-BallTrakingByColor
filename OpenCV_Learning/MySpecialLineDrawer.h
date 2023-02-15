#pragma once

#include "opencv2/imgproc.hpp"

#include <deque>

class MySpecialLineDrawer
{
private:
	std::deque<cv::Point> points;
	size_t maxPoint;

public:
	MySpecialLineDrawer(size_t _maxPoint = 0) : maxPoint(_maxPoint) {
		if (maxPoint == 0)
			maxPoint = 1000000000; // unlimited
	}

	void push(cv::Point point);
	void drawLines(cv::InputOutputArray out);
};

