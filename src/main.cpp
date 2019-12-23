#include <Arduino.h>
#include<UI.h>

UI ui;
void setup(void)
{
	ui.begin();
	ui.node_init(10);
	for(int i = 0; i< 10;i++){
		char buffer[50];
		sprintf(buffer,"Room %d",i);
		ui.node_setTitle(i, (String)buffer,"abcdefghijkl");
		ui.node_setTitlePic(i,"/test1.png");
	}
	ui.node_setTitle(0,"Haruka","Bedroom");
	ui.node_setAllTitleColor(WHITE,CYAN);
}
void loop(void)
{
	ui.update();
}
