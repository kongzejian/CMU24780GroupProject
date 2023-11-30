#include "DOF_calculation.h"
MatrixXf DOFCalculation::calculatePseudoJacobian(float theta1, float theta2, float theta3) {
    MatrixXf Jacobian(2, 3);
    float L1 = 140;
    float L2 = 110;
    float L3 = 80;
    // Populate the Jacobian matrix (this would depend on your robot's geometry)
    Jacobian << -L1 * sin(theta1) - L2 * sin(theta1 + theta2) - L3 * sin(theta1 + theta2 + theta3),
        -L2 * sin(theta1 + theta2) - L3 * sin(theta1 + theta2 + theta3),
        -L3 * sin(theta1 + theta2 + theta3),
        L1* cos(theta1) + L2 * cos(theta1 + theta2) + L3 * cos(theta1 + theta2 + theta3),
        L2* cos(theta1 + theta2) + L3 * cos(theta1 + theta2 + theta3),
        L3* cos(theta1 + theta2 + theta3);
    Eigen::MatrixXf pinvj = Jacobian.completeOrthogonalDecomposition().pseudoInverse();
    return pinvj;
}

MatrixXf DOFCalculation::calculateInvJacobian(float theta1, float theta2, float theta3) {
    MatrixXf Jacobian(2, 3);
    float L1 = 140;
    float L2 = 110;
    float L3 = 80;
    // Populate the Jacobian matrix (this would depend on your robot's geometry)
    Jacobian << -L1 * sin(theta1) - L2 * sin(theta1 + theta2) - L3 * sin(theta1 + theta2 + theta3),
        -L2 * sin(theta1 + theta2) - L3 * sin(theta1 + theta2 + theta3),
        -L3 * sin(theta1 + theta2 + theta3),
        L1* cos(theta1) + L2 * cos(theta1 + theta2) + L3 * cos(theta1 + theta2 + theta3),
        L2* cos(theta1 + theta2) + L3 * cos(theta1 + theta2 + theta3),
        L3* cos(theta1 + theta2 + theta3);
    Eigen::MatrixXf invj = Jacobian.inverse();
    return invj;
}
MatrixXf DOFCalculation::applyAlgorithm(float positionX, float positionY,float bottomAngle, float middleAngle, float topAngle) {

    MatrixXf e(2, 1); //error
    MatrixXf Xd(2, 1); //final position
    MatrixXf FWD(2, 1);
    MatrixXf i_1_theta(3, 1);
    MatrixXf i_theta(3, 1); //starting angle
    i_theta(0, 0) = bottomAngle;
    i_theta(1, 0) =middleAngle;
    i_theta(2, 0) = topAngle;
    FWD = FWD_Kinemeatic(i_theta(0, 0), i_theta(1, 0), i_theta(2, 0));
    Xd(0, 0) = positionX; Xd(1, 0) = positionY;
    e = Xd - FWD;

    while ((std::abs(e(0, 0)) > 0.000001) || (std::abs(e(1, 0)) > 0.000001))
    {

        MatrixXf invJ = calculatePseudoJacobian(i_theta(0, 0), i_theta(1, 0), i_theta(2, 0));
        i_1_theta = i_theta + invJ * e;
        FWD = FWD_Kinemeatic(i_1_theta(0, 0), i_1_theta(1, 0), i_1_theta(2, 0));
        e = Xd - FWD;
        i_theta = i_1_theta;
    }

    return i_theta*180 / 3.14;
}

MatrixXf DOFCalculation::FWD_Kinemeatic(float theta1, float theta2, float theta3)
{
    float c1 = std::cos(theta1);
    float c2 = std::cos(theta2);
    float s1 = std::sin(theta1);
    float s2 = std::sin(theta2);

    float c12 = std::cos(theta1 + theta2);
    float s12 = std::sin(theta1 + theta2);

    float c123 = cos(theta1 + theta2 + theta3);
    float s123 = sin(theta1 + theta2 + theta3);
    float l1 = 140;
    float l2 = 110;
    float l3 = 80;


    MatrixXf FWD(2, 1);

    FWD(0, 0) = l1 * c1 + l2 * c12 + l3 * c123;
    FWD(1, 0) = l1 * s1 + l2 * s12 + l3 * s123;

    return FWD;
}
