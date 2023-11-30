#include "SlideButtonCollections.h"

void SlideButtonCollection::initial() //Initializes three buttons and adds them to the list
{
	DOFbutton* top=new DOFbutton(400, 80);
	DOFbutton* middle = new DOFbutton(400, 50);
	DOFbutton* bottom = new DOFbutton(400, 20);
	mybuttons.push_back(top);
	mybuttons.push_back(middle);
	mybuttons.push_back(bottom);
}

void SlideButtonCollection::draw() //draw all sliding blocks
{
	for (int i = 0; i < mybuttons.size(); i++) {
		mybuttons[i]->draw();
	}
}

int SlideButtonCollection::checkClick(int locX, int locY) //Check that the mouse is on this button
{
	for (int i = 0; i < mybuttons.size(); i++) {
		if (mybuttons[i]->isInRange(locX, locY)) {
			return i;
		}
}
	return -1;
}
