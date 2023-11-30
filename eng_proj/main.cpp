//Citation of Inverse Kinematics part:
//https://github.com/markusbuchholz/Inverse_Kinematics_Solver_in_CPP/tree/main
#include "armLink.h"
#include "EndEffector.h"
#include "DrawingUtilNG.h"
#include "SlideButtonCollections.h"
#include <iostream>
#include <math.h>
#include <vector>
#include "ButtonCollection.h"
#include "GraphicFont.h"
#include <string>
#include "StringPlus.h"
#include"DOF_calculation.h"
ButtonCollection theButtons;
using namespace std;

Point2D getModelCoords(int screenX, int screenY, float originX, float originY, float scale) {
	int wei, hei;
	FsGetWindowSize(wei, hei);
	return { screenX / scale + originX, (hei - screenY) / scale + originY };
}

//the automatic drawing mode
enum Mode { 
	Nothing,
	Circle,
	Rect,
	Triangle,
	Square
};

//the menu
void showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the screen:" << endl;
	cout << "    Left arrow: : Rotate bottom arm CCW" << endl;
	cout << "    Right arrow : Rotate bottom arm CW" << endl;

	cout << endl;
	cout << "    A : Rotate middle arm CCW" << endl;
	cout << "    D : Rotate middle arm CW" << endl;
	cout << endl;

	cout << "    J : Rotate top arm CCW" << endl;
	cout << "    L : Rotate top arm CW" << endl;


	cout << "   Space : Pen up/down" << endl;
	cout << "   C : clear all drawing"<< endl;

	cout << "   T : Draw triangle automatically" << endl;
	cout << "   S : Draw square automatically" << endl;
	cout << "   G : Draw circle automatically" << endl;
	cout << "   R : Draw rectangle automatically" << endl;
	cout << "   U : customize" << endl;
	cout << endl;

	cout << "  ESC : exit program" << endl;
}


void addButtons(GraphicFont *font) {
	// stuff for buttons
	int buttonKey = FSKEY_NULL;
	//ButtonCollection theButtons;
	//ComicSansFont theFont;
	int buttonX = 620;
	int buttonWidth = 100;
	int textX = 420;
	int textY = 20;
	int hei = 30;
	int spacing = 20;
	int currY = 10;

	theButtons.add(buttonX, currY, buttonWidth, hei, FSKEY_T, "triangle", font,
		"Draw a triangle");

	currY += hei + spacing;
	theButtons.add(buttonX, currY, buttonWidth, hei, FSKEY_S, "Square", font,
		"Draw a square");

	currY += hei + spacing;
	theButtons.add(buttonX, currY, buttonWidth, hei, FSKEY_G, "Circle", font,
		"Draw a circle");
	currY += hei + spacing;
	theButtons.add(buttonX, currY, buttonWidth, hei, FSKEY_R, "Rectangle", font,
		"Draw a rectangle");

	currY += hei + spacing;
	theButtons.add(buttonX, currY, buttonWidth, hei, FSKEY_U, "Customize", font,
		"customize the design");
}

//the customization panel for re-set the base location of the robotic arm
Point2D customize()
{
	// ask for maze size
	bool continueAsking = true;
	string userInput;
	string errorString = " ";
	float x=0, y=0;

	glLoadIdentity();  // get rid of zoom and pan

	while (continueAsking) {
		userInput = DrawingUtilNG::getStringFromScreen(errorString,
			"Enter coordinate x and y, moving the arm to the destination",
			"Two values, between 10 and 400 (ESC to cancel)");

		if (userInput.length() > 0) {
			// take two values, either space or comman separating them
			vector<string> splitInput = StringPlus::split(userInput, " ,xX");
			try {
				y = stof(splitInput.at(1));
				x = stof(splitInput.at(0));
				if (10 < x && x <= 400 && 10 < y && y <= 400) {
					continueAsking = false;
					//change the coordinate and generate angles
				}
				else
					errorString = "Values for coordinates were not within bounds";
			}
			catch (exception e) {
				errorString = "ERROR: Unable to convert ->" + userInput
					+ "<- into numbers";;
			}
		}
		else // blank entry for input so cancel 
			continueAsking = false;

	}
	return { x,y };
}

void main() {
	// ComicSansFont buttonFont;
	std::vector<Point2D> DrawPoints;
	int key = FSKEY_NULL;
	bool rotateDir;
	double link1X = 380;
	double link1Y = 400;
	double link1Ang = 90;
	double link1Len = 140;
	double link2Len = 110;
	double link3Len = 80;
	double link2Ang = 90;
	double link3Ang = 90;

	double link2X = link1X + cos(link1Ang * 3.14/180) * link1Len;
	double link2Y = link1Y - sin(link1Ang * 3.14 / 180) * link1Len;
	//ArmLink link1(link1X, link1Y, link1Ang, link1Len);
	//ArmLink link2(link2X, link2Y, link2Ang, link2Len);
	double link3X = link2X + cos(link2Ang * 3.14 / 180) * link2Len;
	double link3Y = link2Y - sin(link2Ang * 3.14 / 180) * link2Len;
	//ArmLink link3(link3X, link3Y, link2Ang, link3Len);
	int height = 600;
	int width = 800;
	int mouseEvent;
	int leftButton, middleButton, rightButton;
	int screenX, screenY;
	int prevScreenX, prevScreenY;
	int buttonKey, buttonKey2 = FSKEY_NULL;
	float originX = 0., originY = 0.;
	float scale = 1.;
	SlideButtonCollection myButtons;
	myButtons.initial();
	bool buttonisPressed = false;
	FsOpenWindow(16, 16, width, height, 1, "Flying Penguins Group Project");
	bool terminate = false;
	bool draw = false;
	Mode drawShape = Nothing;
	int count = 0;
	myButtons.getMyButtons()[0]->setAngle(link1Ang);
	myButtons.getMyButtons()[0]->setbuttonX();
	myButtons.getMyButtons()[1]->setAngle(link1Ang);
	myButtons.getMyButtons()[1]->setbuttonX();
	myButtons.getMyButtons()[2]->setAngle(link2Ang);
	myButtons.getMyButtons()[2]->setbuttonX();
	ComicSansFont buttonFont;
	DOFCalculation caculator;
	addButtons(&buttonFont);
	showMenu();

	while(!terminate){
		glLoadIdentity();
		glTranslated(0, height, 0); // make bottom-left the origin
		glScaled(scale, -scale, 1.);  // scale everything, with positive y going up
		glTranslated(-originX, -originY, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		FsPollDevice();
		key = FsInkey();
		mouseEvent = FsGetMouseEvent(leftButton, middleButton,
			rightButton, screenX, screenY);
		// Check if the left mouse button was clicked
		if (key == FSKEY_NULL && mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
			if (!buttonisPressed) {
				buttonisPressed = true;
				buttonKey = myButtons.checkClick(screenX, height - screenY);
				if (buttonKey != -1) {
					myButtons.getMyButtons()[buttonKey]->drag(screenX);
				}
				else {
					buttonKey2 = theButtons.checkClick(screenX, screenY);
					key = buttonKey2;
				}
			}
		}
		// Handle dragging when the mouse moves with the left button down
		else if (buttonisPressed && mouseEvent == FSMOUSEEVENT_MOVE) {
			if (buttonKey != -1) {
				myButtons.getMyButtons()[buttonKey]->drag(screenX);
			}
		}
		// Handle mouse button release
		else if (mouseEvent == FSMOUSEEVENT_LBUTTONUP) {
			buttonisPressed = false;
			buttonKey = -1;
		}

		buttonKey2= key;
		//the customize mode
		if (buttonKey2 == 32) {
			Point2D newLocation=customize();
			link1X = newLocation.x;
			link1Y = newLocation.y;
			buttonKey2 = FSKEY_NULL;
			key = FSKEY_NULL;
		}
		//automatic drawing circle mode
		else if (buttonKey2 == 18) {
			drawShape = Circle;
			count = 120;
		}
		//automatic drawing rectangle mode
		else if (buttonKey2 == 29) {
			drawShape = Rect;
			count = 50;
		}

		ArmLink link1(link1X, link1Y, link1Ang, link1Len);
		double angle1 = link1.getAngle();
		Point2D link2Base = link1.getPos();

		double link2X = link2Base.x;

		//link1X + cos(link1Ang * 3.14 / 180) * link1Len;
		double link2Y = link2Base.y;
		//link1Y - sin(link1Ang * 3.14 / 180) * link1Len;
		ArmLink link2(link2X, link2Y, link2Ang, link2Len);

		Point2D link3Base = link2.getPos();
		double angle2 = link2.getAngle();

		double link3X = link3Base.x;

		//link2X + cos(link2Ang * 3.14 / 180) * link2Len;
		double link3Y = link3Base.y;
		ArmLink link3(link3X, link3Y, link3Ang, link3Len);
		
		//ArmLink link3(link3X, link3Y, link3Ang, link3Len);
		switch (key) {
		case FSKEY_SPACE: // pen up/down
			draw=!draw;
			break;
		case FSKEY_LEFT: // rotate the bottom arm CCW
			rotateDir = 1;
			link1Ang = link1.getAngle() + 2;
			myButtons.getMyButtons()[0]->setAngle(link1Ang);//link the location of the sliding block to the angle of bottom arm
			myButtons.getMyButtons()[0]->setbuttonX();
			//link1.rotate(5, rotateDir);
			break;
		case FSKEY_RIGHT: // rotate the bottom arm CW 
			rotateDir = 0;
			link1Ang = link1.getAngle() - 2;
			myButtons.getMyButtons()[0]->setAngle(link1Ang);//link the location of the sliding block to the angle of bottom arm
			myButtons.getMyButtons()[0]->setbuttonX();
			break;
		case FSKEY_A: // rotate the middle arm CCW
			rotateDir = 1;
			link2Ang = link2.getAngle() + 2;
			myButtons.getMyButtons()[1]->setAngle(link2Ang);//link the location of the sliding block to the angle of middle arm
			myButtons.getMyButtons()[1]->setbuttonX();
			break;
		case FSKEY_D: // rotate the middle arm CW
			rotateDir = 0;
			link2Ang = link2.getAngle() - 2;
			myButtons.getMyButtons()[1]->setAngle(link2Ang);//link the location of the sliding block to the angle of middle arm 
			myButtons.getMyButtons()[1]->setbuttonX();
			break;
		case FSKEY_J: // rotate the top arm CCW
			rotateDir = 1;
			link3Ang = link3.getAngle() + 2;
			myButtons.getMyButtons()[2]->setAngle(link3Ang);//link the location of the sliding block to the angle of top arm 
			myButtons.getMyButtons()[2]->setbuttonX();
			break;
		case FSKEY_L: // rotate the top arm CW
			rotateDir = 0;
			link3Ang = link3.getAngle() - 2;
			myButtons.getMyButtons()[2]->setAngle(link3Ang); //link the location of the sliding block to the angle of top arm 
			myButtons.getMyButtons()[2]->setbuttonX();
			break;
		case FSKEY_C: //clear all drawing 
			DrawPoints.clear();
			//customize();
			break;
		case FSKEY_ESC: //exit the loop
			terminate = 1;
			break;
		}

		//start to draw the circle
		if (drawShape == Circle && count == 120) {
			draw = !draw;
			link1Ang = link1.getAngle() - 2;
			myButtons.getMyButtons()[0]->setAngle(link1Ang);
			myButtons.getMyButtons()[0]->setbuttonX();
			count -= 1;
		}
		else if (drawShape == Circle && count >=0) {
			link1Ang = link1.getAngle() - 2;
			myButtons.getMyButtons()[0]->setAngle(link1Ang);
			myButtons.getMyButtons()[0]->setbuttonX();
			count -= 1;
		}
		//end drawing the circle
		else if (drawShape == Circle&&count==-1) {
			draw = !draw;
		}
		//In this part, we use inverse kinematics, give locations, output angle of the arm, it has some bugs and will be solved
		/* To Do
		if (drawShape == Rect && count == 50) {
			draw = !draw;
			MatrixXf thetaRobot = caculator.applyAlgorithm(710-link1X,400-link1Y,link1Ang, link2Ang, link3Ang);
			link1Ang = thetaRobot(0, 0);
			link2Ang = thetaRobot(1, 0);
			link3Ang = thetaRobot(2, 0);
			myButtons.getMyButtons()[0]->setAngle(link1Ang);
			myButtons.getMyButtons()[0]->setbuttonX();
			myButtons.getMyButtons()[1]->setAngle(link2Ang);
			myButtons.getMyButtons()[1]->setbuttonX();
			myButtons.getMyButtons()[2]->setAngle(link3Ang);
			myButtons.getMyButtons()[2]->setbuttonX();
			count -= 1;
		}
		else if (drawShape == Rect && count >= 0) {
			MatrixXf thetaRobot = caculator.applyAlgorithm(710, 400, link1Ang, link2Ang, link3Ang);
			link1Ang = thetaRobot(0, 0);
			link2Ang = thetaRobot(1, 0);
			link3Ang = thetaRobot(2, 0);
			myButtons.getMyButtons()[0]->setAngle(link1Ang);
			myButtons.getMyButtons()[0]->setbuttonX();
			myButtons.getMyButtons()[1]->setAngle(link2Ang);
			myButtons.getMyButtons()[1]->setbuttonX();
			myButtons.getMyButtons()[2]->setAngle(link3Ang);
			myButtons.getMyButtons()[2]->setbuttonX();
			count -= 1;
		}
		else if (drawShape == Rect && count == -1) {
			draw = !draw;
		}
		*/


		//link the locations of sliding blocks to the angles of each arm
		link1Ang = myButtons.getMyButtons()[0]->ouputAngle();
		link2Ang = myButtons.getMyButtons()[1]->ouputAngle();
		link3Ang = myButtons.getMyButtons()[2]->ouputAngle();
		// draw buttons,arms buttons
		myButtons.draw();
		glLoadIdentity();
		theButtons.paint();
		Point2D base1 = link3.getPosLeft();
		Point2D base2 = link3.getPosRight();
		//draw the nib
		EndEffector effector1(base1, base2, 40, 12, 78, 128);
		Point2D currNib = effector1.drawEndEffector();
		//draw the arm
		link1.drawLink();
		link2.drawLink();
		link3.drawLink();
		//when the nib is not drawing, use (-1,-1) as a seperator ,record the points of drawing in the vector
		if (DrawPoints.empty()) {
			DrawPoints.push_back({-1,-1});
		}
		else {
			if (draw && (DrawPoints.back().x == -1 && DrawPoints.back().y == -1)) {
				DrawPoints.push_back(currNib);
			}
			else if (draw && (DrawPoints.back().x != currNib.x || DrawPoints.back().y != currNib.y)) { //it is drawing and the point is diferent from previous one
				DrawPoints.push_back(currNib);
			}
			else if (!draw && DrawPoints.empty()) {
				DrawPoints.push_back({ -1,-1 });
			}
			else if (!draw && DrawPoints.back().x != -1 && DrawPoints.back().y != -1) {
				DrawPoints.push_back({ -1,-1 });
			}
		}
		int DrawPointsLength = DrawPoints.size();
		bool isFirstPoint = true;
		glColor3f(255, 0, 0);
		
		//draw the track, allowing pen up and pen down
		if (DrawPointsLength > 2) {
			for (int i = 1; i < DrawPointsLength; i++) {
				if (DrawPoints[i].x == -1 && DrawPoints[i].y == -1) {
					if (!isFirstPoint) {
						glEnd();
						isFirstPoint = true;
					}
					continue; 
				}
				if (isFirstPoint) {
					glBegin(GL_LINES);
					isFirstPoint = false;
				}
				else {
					glVertex2f(DrawPoints[i - 1].x, DrawPoints[i - 1].y);
					glVertex2f(DrawPoints[i].x, DrawPoints[i].y);
				}
			}
			if (!isFirstPoint) {
				glEnd();
			}
		}
		//link3.drawEndEffector(20, 20, 20);
		FsSwapBuffers();
		FsSleep(10);
	}
}