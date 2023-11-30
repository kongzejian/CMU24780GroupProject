#pragma once
#include "DrawingUtilNG.h"
#include "armLink.h"
#include "fssimplewindow.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>


class EndEffector {
private:
	Point2D baseLeft, baseRight;
	double height, r, g, b;

public:
	EndEffector() {
		baseLeft = { 0, 0 };
		baseRight = { 0, 0 };
		height = 0;
		r = g = b = 0;
	}
	EndEffector(Point2D base1, Point2D base2, double heightNew, double r1, double g1, double b1) {
		baseLeft = base1;
		baseRight = base2;
		height = heightNew;
		r = r1;
		g = g1;
		b = b1;
	}
	Point2D drawEndEffector() {
		double x1Pos = baseLeft.x;
		double y1Pos = baseLeft.y;
		double x2Pos = baseRight.x;
		double y2Pos = baseRight.y;
		glColor3ub(r, g, b);
		DrawingUtilNG::drawIsoscelesTriangle(x1Pos, y1Pos, x2Pos, y2Pos, height, 1);
		float midPointX = (x1Pos + x2Pos) / 2;
		float midPointY = (y1Pos + y2Pos) / 2;
		float angleIncline = atan2((y2Pos - y1Pos), (x2Pos - x1Pos));
		float vert3X = midPointX + height * sin(angleIncline);
		float vert3Y = midPointY - height * cos(angleIncline);
		return {vert3X,vert3Y };
	}
};