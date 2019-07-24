#include "pch.h"
#include "recognizer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <map>


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


void recognizer::multiTrain()
{
	std::vector<std::vector<cv::Mat>> arrayOfPics;
	std::vector<std::vector<int>> arrayOfLabels;
	std::string tmpName;
	int userCount;
	std::cout << "Enter user count:" << std::endl;
	std::cin >> userCount;

	std::ofstream outFile("dataset/names.txt");
	for (int i = 0; i < userCount; i++)
	{
		std::cin >> tmpName;
		outFile << tmpName << std::endl;
	}
	outFile.close();
	std::cout << "[INFO] Training faces. It will take a few seconds. Wait..." << std::endl;

	for (int i = 0; i < userCount; i++)
	{
		readPictures(i);
		arrayOfPics.push_back(_pics);
		std::vector<int> label(_pics.size(), i);
		arrayOfLabels.push_back(label);
	}
	std::vector<cv::Mat> pics;
	std::vector<int> labels;
	for (auto i : arrayOfPics) for (auto j : i) pics.push_back(j);
	for (auto i : arrayOfLabels) for (auto j : i) labels.push_back(j);
	//for (auto &i : labels) std::cout << i << std::endl;
	_model->train(pics, labels);
	_model->write("trainer/multi_trainer.yml");

	std::cout << "[INFO] " << pics.size() << " faces trained." << std::endl;
}


void recognizer::predictFromCam()
{
	_model->read(_trainedFileName);

	cv::Mat frame;
	cv::VideoCapture cap(0);
	std::string winName("Predict");
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


void recognizer::multiPredictFromCam()
{
	_model->read("trainer/multi_trainer.yml");

	int threshold = 60;
	std::vector<std::string> names;
	std::string tmpNames;
	std::ifstream inFile("dataset/names.txt");
	while (inFile >> tmpNames) names.push_back(tmpNames);
	std::cout << names.size() << std::endl;

	cv::Mat frame;
	cv::VideoCapture cap(0);
	std::string winName("Multi Predict");
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
			_model->predict(cv::Mat(gray, face), label, conf);

			std::string s;
			if (conf < threshold) s = names[label];
			else s = "unknown";

			cv::putText(frame, s, cv::Point(face.x + 5, face.y - 5), font, 1, (0, 0, 255), 2);
		}

		cv::namedWindow(winName);
		cv::imshow(winName, frame);

		if (cv::waitKey(40) == 27) break;
	}
	std::cout << "[INFO] Multi prediction done." << std::endl;
	cap.release();
	cv::destroyAllWindows();
}


void recognizer::predictFromImage()
{
	std::multimap<double, int> confs;

	std::cout << "[INFO] Predicting faces. It will take a few seconds. Wait..." << std::endl;
	_model->read(_trainedFileName);
	double conf;
	int label;
	int count = 0;
	while (true)
	{
		std::string fileName = std::string("pred_image/") + std::to_string(count) + std::string(".jpg");
		cv::Mat pic = cv::imread(fileName, cv::IMREAD_GRAYSCALE);

		if (pic.data == NULL) break;

		std::vector<cv::Rect> faces;
		_faceCascade.detectMultiScale(pic, faces);
		for (auto & face : faces)
		{
			_model->predict(cv::Mat(pic, face), label, conf);
			//std::cout << count << ". " << conf << std::endl;
			confs.insert({ conf, count });
		}
		count++;
	}

	for (auto &i: confs)
	{
		std::cout << i.second << ". " << i.first << std::endl;
	}
	std::cout << "[INFO] Prediction done." << std::endl;
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