#include "pch.h"
#include <opencv2/opencv.hpp>

void detectFace(const cv::Mat &src, cv::Mat &dst, cv::CascadeClassifier &faceCascade, int id);

int main()
{
	//const std::string eyeFileName = "haarcascade-eye.xml" ;
	const std::string faceFileName = "haarcascade-frontalface-default.xml";
	const std::string winName = "Image";

	//cv::CascadeClassifier eyeCascade;
	cv::CascadeClassifier faceCascade;
	//eyeCascade.load( eyeFileName );
	faceCascade.load( faceFileName );

	cv::Mat frame;
	cv::VideoCapture cap(0);

	int userId;
	std::cout << "Enter user id:" << std::endl;
	std::cin >> userId;
	
	while (true)
	{
		cap >> frame;

		cv::Mat outFrame;
		detectFace(frame, outFrame, faceCascade, userId);

		cv::namedWindow( winName );
		cv::imshow( winName, outFrame );
		cv::waitKey( 1000 );
	}

	return 0;
}

void detectFace(const cv::Mat &src, cv::Mat &dst, cv::CascadeClassifier &faceCascade, int id)
{
	static int count = 0;

	cv::Mat gray;
	std::vector<cv::Rect> faces;

	cv::cvtColor( src, gray, cv::COLOR_BGR2GRAY );
	faceCascade.detectMultiScale( gray, faces );
	dst = src;

	for ( auto & face : faces )
	{
		cv::rectangle( dst, face, cv::Scalar( 0, 0, 255 ), 2 );

		std::string fileName = std::string("dataset/user") + std::to_string(id) + std::string(".") + std::to_string(count) + std::string(".jpg");
		cv::imwrite(fileName, cv::Mat(gray, face));
		count++;
	}
}