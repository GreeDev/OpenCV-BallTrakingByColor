#include "MySpecialLineDrawer.h"

void MySpecialLineDrawer::push(cv::Point point)
{
	points.push_back(point);
	if (points.size() > maxPoint) {
		points.pop_front();
	}
}

void MySpecialLineDrawer::drawLines(cv::InputOutputArray img)
{
	cv::Mat colorHSV(1, 1, CV_8UC3,cv::Scalar(0, 255, 255));
	for (size_t i = 0; i + 1 < points.size(); i++) {
		cv::Mat color;
		cv::cvtColor(colorHSV, color, cv::COLOR_HSV2BGR);
		cv::line(img, points[i], points[i + 1], cv::Scalar(color.data[0], color.data[1], color.data[2]), 2);
		colorHSV.data[0] += 8 - ((i & 0x20) << 3);
	}
}
