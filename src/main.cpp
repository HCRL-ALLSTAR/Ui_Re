#include <Arduino.h>
#include<UI.h>

UI ui;
void setup(void)
{
	ui.begin();
	ui.node_init(5);
	for(int i = 0; i< 5;i++){
		char buffer[50];
		sprintf(buffer,"Item %d",i);
		ui.node_setTitle(i, (String)buffer,"abcdefghijkl");
		ui.node_setTitlePic(i, "/AIR/Air_GREEN.png", "/AIR/Air_GREEN_Hover.png");
	}

	ui.node_setAllTitleColor(WHITE, CYAN);

	ui.node_setType(0, FAN);
	ui.node_setTitle(0,"Fan","Near Door");
	ui.node_setTitlePic(0, "/FAN/Fan_BLUE.png", "/FAN/Fan_BLUE_Hover.png");

	ui.node_setType(1, AIR);
	ui.node_setTitle(1, "Air", " ");
	ui.node_setTitlePic(1, "/AIR/Air_RED.png", "/AIR/Air_RED_Hover.png");

	ui.node_setType(2, LIGHT);
	ui.node_setTitle(2, "LIGHT", "Overall");
	ui.node_setTitlePic(2, "/LIGHT/Li_YELLOW.png", "/LIGHT/Li_YELLOW_Hover.png");
}
void loop(void)
{
	ui.update();
}
