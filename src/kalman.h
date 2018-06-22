#ifndef __KALMAN_H__
#define __KALMAN_H__

#include <eigen/dense>
#include "ADAFRUIT_9DOF.h"

using namespace Eigen;

class kalman : public ADAFRUIT_9DOF{
public:
	kalman();
	~kalman();
	void calcState();
	Matrix3f R;
	// Kalman Filter variables
	Matrix<float, 12, 1> state;
	Matrix<float, 12, 12> A;
	Matrix<float, 6, 1> control;
	Matrix<float, 12, 6> mu;
	Matrix<float, 12, 1> noise;
};

#endif