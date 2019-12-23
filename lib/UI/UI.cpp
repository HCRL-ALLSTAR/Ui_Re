// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "UI.h"

#define BLK_PWM_CHANNEL 7 // LEDC_CHANNEL_7

UI::UI() : isInited(0)
{
}
//init UI
void UI::begin(bool LCDEnable, bool SDEnable, bool SerialEnable)
{
  // Correct init once
  if (isInited == true)
  {
    return;
  }
  else
  {
    isInited = true;
  }

  // UART
  if (SerialEnable == true)
  {
    Serial.begin(115200);
    Serial.flush();
    delay(50);
    Serial.print("M5Stack initializing...");
  }

  // LCD INIT
  if (LCDEnable == true)
  {
    Lcd.begin(backgroundColor);
  }

  // TF Card
  if (SDEnable == true)
  {
    SD.begin(TFCARD_CS_PIN, SPI, 40000000);
  }

  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  if (SerialEnable == true)
  {
    Serial.println("OK");
  }
  this->period = 1000 / refreshRate;
}

//update all UI components
void UI::update()
{
  //Button update
  BtnA.read();
  BtnB.read();
  BtnC.read();
  //control
  if (BtnA.wasPressed())
  {
    if (panel != MAIN)
      Lcd.fillRect(0, 0, 320, 218, backgroundColor);
    if (panel == STAT)
    {

      this->panel = MAIN;
    }
    if (panel == CONT)
    {

      this->c_panel.lastIndex = -1;
      this->panel = MAIN;
    }
    if (panel == SETT)
    {

      this->panel = MAIN;
    }
  }

  if (BtnB.wasPressed())
  {
    if (panel == MAIN)
    {
      Lcd.fillRect(0, 0, 320, 218, backgroundColor);
      if (m_panel.select == 0b100)
      { //status
        this->panel = STAT;
      }
      else if (m_panel.select == 0b010)
      { //control
        this->panel = CONT;
      }
      else
      { //settings
        this->panel = SETT;
      }
      this->m_panel.lastSel = 0b000;
    }
    else if (panel == STAT)
    {
    }
    else if (panel == CONT)
    {

      this->c_panel.lastIndex = -1;
    }
    else if (panel == SETT)
    {
    }
  }

  if (BtnC.pressedFor(RETURN_MS))
  {
    if (panel == CONT)
    {
      this->c_panel.index = 0;
      this->return_c = millis();
    }
  }
  else if (BtnC.wasReleased())
  {
    if (panel == MAIN)
    {
      this->m_panel.select >>= 1;
      if (m_panel.select == 0)
        this->m_panel.select = 0b100;
    }
    if (panel == CONT && millis() - return_c > POS_RETURN_MS)
    {
      this->c_panel.index += 1;
      if (c_panel.index == c_panel.size)
        this->c_panel.index = 0;
    }
  }
  //main
  if (tick_frame())
  {
    main();
  }
  //frame control
  framerate_update();
}

void UI::setBrightness(uint8_t brightness)
{
  ledcWrite(BLK_PWM_CHANNEL, brightness);
}

void UI::main()
{
  if (panel == MAIN)
  {
    main_panel();
  }
  else if (panel == STAT)
  {
    stat_panel();
  }
  else if (panel == CONT)
  {
    cont_panel();
  }
  else if (panel == SETT)
  {
    sett_panel();
  }

  menu_disp(); //menu display don't change
}

void UI::main_panel()
{

  m.leftBtn_enable = false;
  m.midBtn_enable = true;
  m.rightBtn_enable = true;

  Lcd.setTextColor(m_panel.titleColor);
  Lcd.setCursor(10, 10);
  Lcd.setTextSize(m_panel.titleSize);
  Lcd.print(m_panel.title);
  //box
  if (m_panel.select != m_panel.lastSel)
  {
    if (m_panel.select & 0b100)
      Lcd.fillRoundRect(159 - 135, 50, 120, 120, 5, m_panel.selFillColor);
    else
      Lcd.fillRoundRect(159 - 135, 50, 120, 120, 5, m_panel.fillColor);
    if (m_panel.select & 0b010)
      Lcd.fillRoundRect(159 + 15, 50, 120, 120, 5, m_panel.selFillColor);
    else
      Lcd.fillRoundRect(159 + 15, 50, 120, 120, 5, m_panel.fillColor);
    if (m_panel.select & 0b001)
      Lcd.fillRoundRect(159 - 75, 180, 150, 30, 5, m_panel.selFillColor);
    else
      Lcd.fillRoundRect(159 - 75, 180, 150, 30, 5, m_panel.fillColor);

    if (m_panel.select & 0b100)
      Lcd.drawRoundRect(159 - 135, 50, 120, 120, 5, m_panel.selLineColor);
    else
      Lcd.drawRoundRect(159 - 135, 50, 120, 120, 5, m_panel.lineColor);
    if (m_panel.select & 0b010)
      Lcd.drawRoundRect(159 + 15, 50, 120, 120, 5, m_panel.selLineColor);
    else
      Lcd.drawRoundRect(159 + 15, 50, 120, 120, 5, m_panel.lineColor);
    if (m_panel.select & 0b001)
      Lcd.drawRoundRect(159 - 75, 180, 150, 30, 5, m_panel.selLineColor);
    else
      Lcd.drawRoundRect(159 - 75, 180, 150, 30, 5, m_panel.lineColor);
    this->m_panel.lastSel = m_panel.select;
  }
  //text
  Lcd.setTextColor(m_panel.lineColor);
  Lcd.setTextSize(2);
  Lcd.setCursor(159 - 70, 190);
  Lcd.print("  Settings  ");
  Lcd.setCursor(159 - 133, 105);
  Lcd.print("  Status ");
  Lcd.setCursor(159 + 23, 105);
  Lcd.print(" Control ");
}

void UI::stat_panel()
{

  m.leftBtn_enable = true;
  m.midBtn_enable = false;
  m.rightBtn_enable = false;

  Lcd.setTextColor(sa_panel.titleColor);
  Lcd.setCursor(10, 10);
  Lcd.setTextSize(sa_panel.titleSize);
  Lcd.print(sa_panel.title);
}

void UI::cont_panel()
{
  m.leftBtn_enable = true;
  m.midBtn_enable = true;
  m.rightBtn_enable = true;

  Lcd.setTextColor(c_panel.titleColor);
  Lcd.setCursor(10, 10);
  Lcd.setTextSize(c_panel.titleSize);
  Lcd.print(c_panel.title);

  if (c_panel.index != c_panel.lastIndex)
  {

    if (c_panel.index - 1 >= 0)
    {
      Lcd.fillRoundRect(160 - 65 - 10 - 130 - 10, 120 - 65, 130, 130, 5, c_panel.fillColor);
      Lcd.drawRoundRect(160 - 65 - 10 - 130 - 10, 120 - 65, 130, 130, 5, c_panel.lineColor);
    }
    else
    {
      Lcd.fillRoundRect(160 - 65 - 10 - 130 - 10, 120 - 65, 130, 130, 5, backgroundColor);
      Lcd.drawRoundRect(160 - 65 - 10 - 130 - 10, 120 - 65, 130, 130, 5, backgroundColor);
    }

    if (c_panel.index + 1 < c_panel.size)
    {
      Lcd.fillRoundRect(160 - 65 + 10 + 130 + 10, 120 - 65, 130, 130, 5, c_panel.fillColor);
      Lcd.drawRoundRect(160 - 65 + 10 + 130 + 10, 120 - 65, 130, 130, 5, c_panel.lineColor);
    }
    else
    {
      Lcd.fillRoundRect(160 - 65 - 10 + 130 + 10, 120 - 65, 130, 130, 5, backgroundColor);
      Lcd.drawRoundRect(160 - 65 - 10 + 130 + 10, 120 - 65, 130, 130, 5, backgroundColor);
    }

    Lcd.fillRoundRect(160 - 75, 120 - 75, 150, 150, 5, c_panel.fillColor);
    Lcd.drawRoundRect(160 - 75, 120 - 75, 150, 150, 5, c_panel.lineColor);
    Lcd.drawPngFile(SPIFFS, node[c_panel.index].titlePic, 160 - 75 + 22, 120 - 75 + 3);
    Lcd.setTextSize(node[c_panel.index].titleSize);
    Lcd.setCursor(160 - 75 + 5, 120 + 75 - 10 - 10 - 20);
    Lcd.print(node[c_panel.index].title_1st);
    Lcd.setTextColor(node[c_panel.index].title2ndColor);
    Lcd.setCursor(160 - 75 + 5, 120 + 75 - 10 - 10);
    Lcd.print(node[c_panel.index].title_2nd);

    this->c_panel.lastIndex = c_panel.index;
  }
}

void UI::sett_panel()
{

  m.leftBtn_enable = true;
  m.midBtn_enable = true;
  m.rightBtn_enable = true;

  Lcd.setTextColor(st_panel.titleColor);
  Lcd.setCursor(10, 10);
  Lcd.setTextSize(st_panel.titleSize);
  Lcd.print(st_panel.title);
}

void UI::menu_disp()
{
  //box
  if (m.fillColor != BLACK)
  {
    if (m.leftBtn_visible)
      Lcd.fillRoundRect(0, 219, 105, 19, 5, m.fillColor);
    if (m.midBtn_visible)
      Lcd.fillRoundRect(107, 219, 105, 19, 5, m.fillColor);
    if (m.rightBtn_visible)
      Lcd.fillRoundRect(214, 219, 105, 19, 5, m.fillColor);
  }
  if (m.leftBtn_visible)
    Lcd.drawRoundRect(0, 219, 105, 19, 5, m.lineColor);
  if (m.midBtn_visible)
    Lcd.drawRoundRect(107, 219, 105, 19, 5, m.lineColor);
  if (m.rightBtn_visible)
    Lcd.drawRoundRect(214, 219, 105, 19, 5, m.lineColor);
  //text
  if (m.leftBtn_visible && m.leftBtn_enable)
    Lcd.setTextColor(m.lineColor);
  else
    Lcd.setTextColor(backgroundColor);
  Lcd.setTextSize(2);
  Lcd.setCursor(5, 221);
  Lcd.print(m.leftText);

  if (m.midBtn_visible && m.midBtn_enable)
    Lcd.setTextColor(m.lineColor);
  else
    Lcd.setTextColor(backgroundColor);
  Lcd.setTextSize(2);
  Lcd.setCursor(112, 221);
  Lcd.print(m.midText);

  if (m.rightBtn_visible && m.rightBtn_enable)
    Lcd.setTextColor(m.lineColor);
  else
    Lcd.setTextColor(backgroundColor);
  Lcd.setTextSize(2);
  Lcd.setCursor(219, 221);
  Lcd.print(m.rightText);
}

bool UI::tick_frame()
{
  if (millis() - time > period)
  {
    this->time = millis();
    this->fps += 1;
    return true;
  }
  else
    return false;
}

void UI::framerate_update()
{
  if (millis() - counter > 1000)
  {
    //Serial.print("Framerate >> ");
    //Serial.println(fps);
    if (fps < idealRefreshRate)
    {
      this->refreshRate += 1;
    }
    else if (fps > idealRefreshRate)
    {
      this->refreshRate -= 1;
    }
    this->period = 1000 / refreshRate;
    this->fps = 0;
    this->counter = millis();
  }
}

/*Node initializing
          size:     number of node to be used
          default:  1
          max:      32
*/
void UI::node_init(uint8_t size)
{
  for (int i = 0; i < size; i++)
  {
    this->node[i].EN = true;
  }
  this->c_panel.size = size;
}
/*set the title of node at specific index
  title_1st: Title on first Line
  title_2nd: Title on second Line
  default: no change
  charater limit: 12
  *** Index must not exceed init size
*/
void UI::node_setTitle(uint8_t index, String title_1st, String title_2nd)
{
  if (node[index].EN)
  {
    if (title_1st != "")
      this->node[index].title_1st = title_1st;
    if (title_2nd != "")
      this->node[index].title_2nd = title_2nd;
  }
}

/*set node title picture at specific index
  put png file in data folder and put the path here
  example: "/test1.png"
  *** 103 x 103 pixels image
*/
void UI::node_setTitlePic(uint8_t index, char *path)
{
  if (node[index].EN)
  {
    this->node[index].titlePic = path;
  }
}

//set node title color at specific index
void UI::node_setTitleColor(uint8_t index, uint16_t title_1st, uint16_t title_2nd)
{
  if (node[index].EN)
  {
    this->node[index].title1stColor = title_1st;
    this->node[index].title2ndColor = title_2nd;
  }
}

//set all node title color
void UI::node_setAllTitleColor(uint16_t title_1st, uint16_t title_2nd)
{
  for (int i = 0; i < c_panel.size; i++)
  {
    this->node[i].title1stColor = title_1st;
    this->node[i].title2ndColor = title_2nd;
  }
}