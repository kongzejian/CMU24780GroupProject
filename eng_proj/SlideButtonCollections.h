#pragma once
#include<vector>
#include"DOFbutton.h"
class SlideButtonCollection{
private:
	std::vector<DOFbutton*> mybuttons;

public:
	void initial();
	void draw();
	int checkClick(int locX, int locY);
	std::vector<DOFbutton*> getMyButtons() { return mybuttons; }
};