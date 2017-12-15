#ifndef IMAGEFOCUSQUALITYINTERFACE_H
#define IMAGEFOCUSQUALITYINTERFACE_H
#include <opencv2/opencv.hpp>

class ImageFocusQualityInterface{
public:
        // evalQaulity outputs the focus quality (0.0 to 1.0) of an input image (color or gray).
        virtual float evalQuality(const cv::Mat &im) = 0;
};

#endif // IMAGEFOCUSQUALITYINTERFACE_H
