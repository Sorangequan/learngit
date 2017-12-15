#ifndef IMAGEFOCUSQUALITYSSIM_H
#define IMAGEFOCUSQUALITYSSIM_H

#include "imagefocusqualityinterface.h"

#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;




class ImageFocusQualitySSIM : public ImageFocusQualityInterface  //�̳л��������
{

public:
	float evalQuality(const cv::Mat &im);

	ImageFocusQualitySSIM();
	~ImageFocusQualitySSIM();

public:
	Scalar IFQI(Mat, Mat);

private:
protected:
};



#endif // IMAGEFOCUSQUALITYSSIM_H
