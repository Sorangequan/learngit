#include "imagefocusqualityssim.h"
#include <opencv2\opencv.hpp>

#include<iostream>
#include "CalculateTime.h"
using namespace std;
using namespace cv;

ImageFocusQualitySSIM::ImageFocusQualitySSIM()
{

}


ImageFocusQualitySSIM::~ImageFocusQualitySSIM()
{

}


Scalar ImageFocusQualitySSIM::IFQI(Mat  inputimage1, Mat inputimage2)
{
	Mat image1 = inputimage1;
	Mat image2 = inputimage2;
	//const float K1 = 0.01;   //  列出各参数值
	//const float K2 = 0.03;
	//const int L = 255;
	//double C1 = (K1*L) *(K1*L);
	//double C2 = (K2*L) *(K2*L);
	//double C3 = C2 / 2;


	double C1 = 6.5025, C2 = 58.5225;
	double C3 = C2 / 2;

	int d = CV_32F;
	Mat Image1, Image2;   //图像深度转换
	image1.convertTo(Image1, d);
	image2.convertTo(Image2, d);
	Mat Image2_2 = Image2.mul(Image2);
	Mat Image1_2 = Image1.mul(Image1);
	Mat Image1_Image2 = Image1.mul(Image2);

	Mat mu1, mu2;
	GaussianBlur(Image1, mu1, Size(11, 11), 1.5);
	GaussianBlur(Image2, mu2, Size(11, 11), 1.5);

	Mat mu1_2 = mu1.mul(mu1);// I2^2
	Mat mu2_2 = mu2.mul(mu2);// I1^2
	Mat mu1_mu2 = mu1.mul(mu2);// I1 * I2
	Mat sigma1_2, sigma2_2, sigma12;
	GaussianBlur(Image1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;
	GaussianBlur(Image2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;
	GaussianBlur(Image1_Image2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;


	Mat AA1, AA2, AA3;
	AA1 = 2 * mu1_mu2 + C1;
	AA2 = 2 * sigma12 + C2;
	AA3 = AA1.mul(AA2);// AA3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2)) 
	AA1 = mu1_2 + mu2_2 + C1;
	AA2 = sigma1_2 + sigma2_2 + C2;
	AA1 = AA1.mul(AA2);// AA1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
	Mat ssim_map;
	divide(AA3, AA1, ssim_map);// ssim_map =  AA3./AA1;
	Scalar mssim = mean(ssim_map);
	return mssim;
}


float ImageFocusQualitySSIM::evalQuality(const cv::Mat &im)
{
    Mat SrcImage = im(cv::Rect(im.cols  /4, im.rows  / 4, im.cols / 2, im.rows / 2));
	//printf("New Image load is %d %d\n", SrcImage.cols, SrcImage.rows);

	Mat BlurImage3, BlurImage9;
	Mat tmpImage, dstImage;
	dstImage = SrcImage;
	GaussianBlur(dstImage, BlurImage3, Size(3, 3), 0, 0);
    GaussianBlur(dstImage, BlurImage9, Size(19, 19), 0, 0);

    printf("evaluQaulity before IFQI\n");
	Scalar SSIM3 = IFQI(dstImage, BlurImage3);
	Scalar SSIM9 = IFQI(dstImage, BlurImage9);
    printf("evaluQaulity after IFQI\n");

	float A1 = float((SSIM3.val[2] + SSIM3.val[1] + SSIM3.val[0]) / 3 * (100));
	float A2 = float((SSIM9.val[2] + SSIM9.val[1] + SSIM9.val[0]) / 3 * (100));
    float A = float((A1 - A2*0.85) / (A1 / 0.98 - A2*0.85));
	return A;


}
