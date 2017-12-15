#pragma once
class CalculateTime
{
public:
	CalculateTime();
	~CalculateTime();
	void start();
	double stop();
	double stop_start();
private:
	double f;
	double t;
};

