#include "DOFbutton.h"
void DOFbutton::draw() //draw the sliding blocks
{	//the guide rail
	glColor3f(0,0, 255);
	glBegin(GL_QUADS);
	glVertex2i(centerX - railLength / 2, centerY + railWidth / 2);
	glVertex2i(centerX + railLength / 2, centerY + railWidth / 2);
	glVertex2i(centerX + railLength / 2, centerY - railWidth / 2);
	glVertex2i(centerX - railLength / 2, centerY - railWidth / 2);
	glEnd();

	glColor3f(255, 0, 0);
	//the circle button
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float radian = i * PI / 180;
		glVertex2f(buttonX + cos(radian) * buttonRadius / 2,
			buttonY + sin(radian) * buttonRadius / 2);
	}
	glEnd();
}

void DOFbutton::drag(int screenX) //Drag the button to the current position of the mouse
{
	if (screenX <= (centerX - railLength / 2) ) { // boundary check
		buttonX = (centerX - railLength / 2);
	}
	else if (screenX >= (centerX + railLength / 2)) {
		buttonX = (centerX + railLength / 2);
	}
	else {
		buttonX = screenX;
	}
}

float DOFbutton::ouputAngle() //The output Angle corresponds to the button for the position of the guide rail
{	angle=360 * (buttonX - centerX + railLength / 2) / railLength;
	return angle;
}

void DOFbutton::setbuttonX()
{
	if (angle > 360) {
		angle -= 360;
	}
	else if (angle <= 0){
		angle = 360 + angle;
	}
	buttonX = (angle / 360 * railLength) + (centerX - railLength / 2);//The button position corresponds to the Angle
}


