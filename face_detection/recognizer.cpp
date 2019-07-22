#include "pch.h"
#include "recognizer.h"
#include <opencv2/opencv.hpp>


recognizer::recognizer():
	_faceFileName("haarcascade-frontalface-default.xml"),
	_winName("Image"),
	_flag(false)
{
}


recognizer::~recognizer()
{
}


void recognizer::takePicture()
{
	_faceCascade.load(_faceFileName);

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
		else if (key == 32) _flag = true;

		cv::Mat outFrame;
		detectFace(frame, outFrame, userId);

		cv::namedWindow(_winName);
		cv::imshow(_winName, outFrame);
	}

	std::cout << "[INFO] Face capture is done." << std::endl;
}


void recognizer::detectFace(const cv::Mat &src, cv::Mat &dst, int id)
{
	static int count = 0;

	cv::Mat gray;
	std::vector<cv::Rect> faces;

	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	_faceCascade.detectMultiScale(gray, faces);
	dst = src;

	for (auto & face : faces)
	{
		cv::rectangle(dst, face, cv::Scalar(0, 0, 255), 2);

		if (_flag)
		{
			std::string fileName = std::string("dataset/user") + std::to_string(id) + std::string(".") + std::to_string(count) + std::string(".jpg");
			cv::imwrite(fileName, cv::Mat(gray, face));
			count++;
		}
	}
	_flag = false;
}


void recognizer::readPictures(int id)
{
	int count = 0;
	while (true)
	{
		std::string fileName = std::string("dataset/user") + std::to_string(id) + std::string(".") + std::to_string(count) + std::string(".jpg");
		cv::Mat pic = cv::imread(fileName);
		
		if (pic.data == NULL) 
		{
			std::cout << _pics.size() << std::endl;
			return;
		}
		
		_pics.push_back(pic);
		count++;
	}
}


void recognizer::createRecognizer()
{

}
