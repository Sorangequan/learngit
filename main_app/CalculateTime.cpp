#include "stdafx.h"
#include"CalculateTime.h"
#include<opencv2\opencv.hpp>

CalculateTime::CalculateTime()
{
	t = 0;
	f = cvGetTickFrequency();
}

void CalculateTime::start()
{
	t = (double)cvGetTickCount();
}

double CalculateTime::stop()
{
	t = (double)cvGetTickCount() - t;
	return (t / (f * 1000*1000));
}

double CalculateTime::stop_start()
{
	t = (double)cvGetTickCount() - t;
	double val = (t / (f * 1000));
	t = (double)cvGetTickCount();
	return val;
}

CalculateTime::~CalculateTime()
{

}
