#include <Arduino.h>
#include<UI.h>

UI ui;
void setup(void)
{
	ui.begin();
	ui.node_init(3);
	for(int i = 0; i< 3;i++){
		char buffer[50];
		sprintf(buffer,"Room %d",i);
		ui.node_setTitle(i, (String)buffer,"abcdefghijkl");
		ui.node_setType(i,AIR);
	}
	ui.node_setType(0,FAN);
	ui.node_setTitle(0,"Haruka","Bedroom");
	ui.node_setAllTitleColor(WHITE,CYAN);
}
void loop(void)
{
	ui.update();
}
