#include <opencv2/opencv.hpp>
//#include <opencv2/f>
#include <vector>


#pragma once
class recognizer
{
public:
	recognizer();
	~recognizer();
	void takePicture();
	void readPictures(int id);

private:
	const std::string _faceFileName;
	const std::string _winName;
	cv::CascadeClassifier _faceCascade;
	int _flag;
	std::vector<cv::Mat> _pics;
	std::vector<int> _labels;
	//Ptr<cv::face::FaceRecognizer> model;

	void detectFace(const cv::Mat &src, cv::Mat &dst, int id);
	void createRecognizer();
};

