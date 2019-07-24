#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <vector>


#pragma once
class recognizer
{
public:
	recognizer();
	~recognizer();
	void takePicture();
	void train();
	void predictFromCam();
	void predictFromImage();

private:
	int _flag;
	const std::string _trainedFileName;
	const std::string _faceFileName;
	cv::CascadeClassifier _faceCascade;
	std::vector<cv::Mat> _pics;
	cv::Ptr<cv::face::FaceRecognizer> _model;

	void detectFace(const cv::Mat &src, cv::Mat &dst, int id, int &count);
	void readPictures(int userId);
};

