//This code includes functions for calculating the Jacobian matrix, 
// performing the inverse kinematics using the Jacobian, and updating the joint angles to achieve inverse kinematics
#include <iostream>
#include <Eigen/Dense> 
#include <cmath>
#pragma once

using namespace Eigen;
class DOFCalculation {
public:
	MatrixXf FWD_Kinemeatic(float theta1, float theta2, float theta3);
	MatrixXf calculatePseudoJacobian(float theta1, float theta2, float theta3);
	MatrixXf calculateInvJacobian(float theta1, float theta2, float theta3);
	MatrixXf applyAlgorithm(float positionX, float positionY, float bottomAngle, float middleAngle, float topAngle);
};