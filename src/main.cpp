#include <Arduino.h>
#include<UI.h>

UI ui;
void setup(void)
{
	ui.begin();
}
void loop(void)
{
	ui.update();
}
