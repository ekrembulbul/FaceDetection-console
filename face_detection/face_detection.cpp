#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

void detectFaceAndEye( const cv::Mat & src, cv::Mat & dst, cv::CascadeClassifier & eyeCascade, cv::CascadeClassifier & faceCascade );

int main()
{
	const std::string eyeFileName = "haarcascade-eye.xml" ;
	const std::string faceFileName = "haarcascade-frontalface-default.xml";
	const std::string winName = "Image";

	cv::CascadeClassifier eyeCascade;
	cv::CascadeClassifier faceCascade;
	eyeCascade.load( eyeFileName );
	faceCascade.load( faceFileName );

	cv::Mat frame;
	cv::VideoCapture cap(0);
	while ( true )
	{
		cap >> frame;

		cv::Mat outFrame;
		detectFaceAndEye( frame, outFrame, eyeCascade, faceCascade );

		cv::namedWindow( winName );
		cv::imshow( winName, outFrame );
		cv::waitKey( 40 );
	}

	return 0;
}

void detectFaceAndEye( const cv::Mat & src, cv::Mat & dst, cv::CascadeClassifier & eyeCascade, cv::CascadeClassifier & faceCascade )
{
	cv::Mat gray;
	std::vector<cv::Rect> faces;

	cv::cvtColor( src, gray, cv::COLOR_BGR2GRAY );
	faceCascade.detectMultiScale( gray, faces );
	dst = src;

	for ( auto & face : faces )
	{
		cv::rectangle( dst, face, cv::Scalar( 0, 0, 255 ), 2 );
	}
}