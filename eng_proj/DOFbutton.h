#pragma once
#include"fssimplewindow.h"
#include<math.h>
#define PI 3.14159
//����ĽǶȣ�����360�ȣ����ڰ�ť�ڵ����λ��ʵʱ��Ӧ�������е���Զ�ת����ʱ�򣬰�ťҲ���Լ�����
class DOFbutton {
private:
	int railLength = 300;
	int railWidth = 4;
	int centerX;
	int centerY;
	int buttonX;
	int buttonY;
	int buttonRadius = 20;
	float angle = 0;
public:
	DOFbutton(int centerX,int centerY){
		this->centerX = centerX;
		this->centerY = centerY;
		this->buttonX = centerX - railLength / 2;
		this->buttonY=centerY;
	}
	void draw();
	void drag(int screenX);
	float ouputAngle();//����Ƕ�
	void setbuttonX();
	void angleAdd(int offset) { angle += offset; }
	bool isInRange(int x, int y) { 
		return pow((x - buttonX),2) + pow((y - buttonY),2) <= pow(buttonRadius,2); }
	int getButtonX() { return buttonX; }
	void setAngle(float newAngle) { angle = newAngle; }
};