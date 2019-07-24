#include "pch.h"
#include "recognizer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>


//public


recognizer::recognizer() :
	_faceFileName("haarcascade-frontalface-default.xml"),
	_trainedFileName("trainer/trainer.yml"),
	_flag(false),
	_model(cv::face::LBPHFaceRecognizer::create())
{
	_faceCascade.load(_faceFileName);
}


recognizer::~recognizer()
{
}


void recognizer::takePicture()
{
	cv::Mat frame;
	cv::VideoCapture cap(0);
	std::string winName("Cam");

	int userId;
	int count = 0;
	std::cout << "Enter user id:" << std::endl;
	std::cin >> userId;
	std::cout << "[INFO] Initializing face capture. Look the camera and press space..." << std::endl;

	while (true)
	{
		cap >> frame;

		int key = cv::waitKey(40);
		if (key == 27) break;
		else if (key == 32) _flag = true;

		cv::Mat outFrame;
		detectFace(frame, outFrame, userId, count);

		cv::namedWindow(winName);
		cv::imshow(winName, outFrame);
	}

	cv::destroyWindow(winName);
	std::cout << "[INFO] Face capture is done." << std::endl;
}


void recognizer::train()
{
	int userId;
	std::cout << "Enter user id:" << std::endl;
	std::cin >> userId;

	std::cout << "[INFO] Training faces. It will take a few seconds. Wait..." << std::endl;
	readPictures(userId);
	std::vector<int> label(_pics.size(), userId);
	_model->train(_pics, label);
	_model->write(_trainedFileName);
	std::cout << "[INFO] " << _pics.size() << " faces trained." << std::endl;
}


void recognizer::predictFromCam()
{
	_model->read(_trainedFileName);

	cv::Mat frame;
	cv::VideoCapture cap(0);
	std::string winName("Cam");
	cv::HersheyFonts font = cv::FONT_HERSHEY_SIMPLEX;

	while (true)
	{
		cap >> frame;

		cv::Mat gray;
		std::vector<cv::Rect> faces;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		_faceCascade.detectMultiScale(gray, faces);
		for (auto & face : faces)
		{
			cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
			int label;
			double conf;
			//int confI;
			_model->predict(cv::Mat(gray, face), label, conf);
			//if (conf < 50) {
			//	id0 = "Ekrem";
			//	confI = (int) std::round(50 - conf);
			//}
			//else
			//{
			//	id0 = "Unknown";
			//	confI = (int)std::round(50 - conf);
			//}
			//std::string s = std::to_string(confI) + std::string(" - ") + id0;
			cv::putText(frame, std::to_string(conf), cv::Point(face.x + 5, face.y - 5), font, 1, (0, 0, 255), 2);
			//cv::putText(frame, std::to_string(confI), cv::Point(face.x + 5, face.y + 10), font, 1, (0, 0, 255), 2);
		}
		
		cv::namedWindow(winName);
		cv::imshow(winName, frame);

		if (cv::waitKey(40) == 27) break;
	}

	std::cout << "[INFO] Prediction done." << std::endl;
	cap.release();
	cv::destroyAllWindows();
}


void recognizer::readImagesAndPredict()
{
	int count = 0;
	_pics.clear();
	while (true)
	{
		std::string fileName = std::string("pred_image/") + std::to_string(count) + std::string(".jpg");
		cv::Mat pic = cv::imread(fileName, cv::IMREAD_GRAYSCALE);

		if (pic.data == NULL) break;

		_pics.push_back(pic);
		count++;
	}
}


//private


void recognizer::detectFace(const cv::Mat &src, cv::Mat &dst, int id, int &count)
{
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


void recognizer::readPictures(int userId)
{
	int count = 0;
	_pics.clear();
	while (true)
	{
		std::string fileName = std::string("dataset/user") + std::to_string(userId) + std::string(".") + std::to_string(count) + std::string(".jpg");
		cv::Mat pic = cv::imread(fileName, cv::IMREAD_GRAYSCALE);
		
		if (pic.data == NULL) return;

		_pics.push_back(pic);
		count++;
	}
}


void recognizer::predictFromImage(const cv::Mat &frame, double &conf)
{
	_model->read(_trainedFileName);

	cv::Mat gray;
	std::string winName("Image");
	std::vector<cv::Rect> faces;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	_faceCascade.detectMultiScale(gray, faces);
	for (auto & face : faces)
	{
		cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
		int label;
		double conf;
		_model->predict(cv::Mat(gray, face), label, conf);
	}
}
