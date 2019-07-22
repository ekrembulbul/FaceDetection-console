#include "pch.h"
#include <opencv2/opencv.hpp>

void detectFace(const cv::Mat &src, cv::Mat &dst, cv::CascadeClassifier &faceCascade, int id, int &flag);

int main()
{
	//const std::string eyeFileName = "haarcascade-eye.xml" ;
	const std::string faceFileName = "haarcascade-frontalface-default.xml";
	const std::string winName = "Image";

	//cv::CascadeClassifier eyeCascade;
	cv::CascadeClassifier faceCascade;
	//eyeCascade.load( eyeFileName );
	faceCascade.load( faceFileName );

	int flag = false;
	cv::Mat frame;
	cv::VideoCapture cap(0);
	
	int userId;
	std::cout << "Enter user id:" << std::endl;
	std::cin >> userId;
	std::cout << "[INFO] Initializing face capture. Look the camera and wait..." << std::endl;
	
	while (true)
	{
		cap >> frame;

		int key = cv::waitKey(40);
		if (key == 27) break;
		else if (key == 32) flag = true;

		cv::Mat outFrame;
		detectFace(frame, outFrame, faceCascade, userId, flag);

		cv::namedWindow( winName );
		cv::imshow( winName, outFrame );
	}

	std::cout << "[INFO] Face capture is done." << std::endl;
	return 0;
}

void detectFace(const cv::Mat &src, cv::Mat &dst, cv::CascadeClassifier &faceCascade, int id, int &flag)
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

		if (flag)
		{
			std::string fileName = std::string("dataset/user") + std::to_string(id) + std::string(".") + std::to_string(count) + std::string(".jpg");
			cv::imwrite(fileName, cv::Mat(gray, face));
			count++;
		}
	}
	flag = false;
}