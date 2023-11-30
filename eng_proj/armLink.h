#pragma once
#include "DrawingUtilNG.h"
#include "fssimplewindow.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>

struct Point2D {
	float x, y;
};

class ArmLink {
private:
	double baseX, baseY, angle, length;
	double jointDiam = 40;
	double linkWid = jointDiam;
public:
	ArmLink() {
		baseX = 0;
		baseY = 0;
		angle = 90;
		length = 0;
	}

	ArmLink(double xpos, double ypos, double angleNew, double lengthNew) {
		baseX = xpos;
		baseY = ypos;
		angle = angleNew;
		length = lengthNew;
	}

	void drawLink() {
		double linkX = baseX - jointDiam / 2.0 * sin(angle * 3.14/180);
		double linkY = baseY - jointDiam / 2.0 * cos(angle * 3.14 / 180);
		glColor3ub(114, 121, 124);
		DrawingUtilNG::drawTiltRect(linkX, linkY, angle, length, linkWid, 1);
		glColor3ub(111, 214, 245);
		DrawingUtilNG::drawCircle(baseX, baseY,jointDiam, jointDiam, 1);
	}

	void rotate(double rotAngle, bool dir) {
		if (dir)
			angle += rotAngle;
		else
			angle -= rotAngle;
	}

	Point2D getPos() {
		double xPos = baseX + length * cos(angle * 3.14 / 180);
		double yPos = baseY - length * sin(angle * 3.14 / 180);
		Point2D nextBase = { xPos, yPos };
		return nextBase;
	}
	Point2D getPosLeft() {
		double xPos = baseX - jointDiam / 2.0 * sin(angle * 3.14 / 180) + length * cos(angle * 3.14 / 180);
		double yPos = baseY - jointDiam / 2.0 * cos(angle * 3.14 / 180) - length * sin(angle * 3.14 / 180);
		Point2D nextBaseLeft = { xPos, yPos };
		return nextBaseLeft;
	}

	Point2D getPosRight() {
		double xPos = baseX + jointDiam / 2.0 * sin(angle * 3.14 / 180) + length * cos(angle * 3.14 / 180);
		double yPos = baseY + jointDiam / 2.0 * cos(angle * 3.14 / 180) - length * sin(angle * 3.14 / 180);
		Point2D nextBaseRight = { xPos, yPos };
		return nextBaseRight;

	}


	//void drawEndEffector(double r, double g, double b) {
	//	double x1Pos = baseX - jointDiam / 2.0 * sin(angle * 3.14 / 180) + length * cos(angle * 3.14 / 180);
	//	double y1Pos = baseY - jointDiam / 2.0 * cos(angle * 3.14 / 180) - length * sin(angle * 3.14 / 180);
	//	double x2Pos = baseX + jointDiam / 2.0 * sin(angle * 3.14 / 180) + length * cos(angle * 3.14 / 180);
	//	double y2Pos = baseY + jointDiam / 2.0 * cos(angle * 3.14 / 180) - length * sin(angle * 3.14 / 180);
	//	glColor3ub(r, g, b);
	//	DrawingUtilNG::drawIsoscelesTriangle(x1Pos, y1Pos, x2Pos, y2Pos, 100, 1);
	//}

	void setAngle(double newAngle) {
		angle = newAngle;
	}

	double getAngle() {
		return angle;
	}
	//void drawGear();
};