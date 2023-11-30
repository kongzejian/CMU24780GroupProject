#pragma once
#include<vector>
#include"button.h"

class ButtonCollection {

private:	
	//customize buttons
	std::vector<Button> theButtons;
public:
	void paint();
	void add(int locX, int locY, int wid, int hei, int val, std::string text,
		GraphicFont* aFont, std::string description = "");
	void draw();
	int checkClick(int locX, int locY);
};
