/**
    LCD:
        lcd.setBrightness(uint8_t brightness);
        Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        Lcd.fillScreen(uint16_t color);
        Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        Lcd.setCursor(uint16_t x0, uint16_t y0);
        Lcd.setTextColor(uint16_t color);
        Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        Lcd.setTextSize(uint8_t size);
        Lcd.setTextWrap(boolean w);
        Lcd.printf();
        Lcd.print();
        Lcd.println();
        Lcd.drawCentreString(const char *string, int dX, int poY, int font);
        Lcd.drawRightString(const char *string, int dX, int poY, int font);
        Lcd.drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x, uint16_t y);
        Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
        Lcd.drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

    Button:
        BtnA/B/C.read();
        BtnA/B/C.isPressed();
        BtnA/B/C.isReleased();
        BtnA/B/C.wasPressed();
        BtnA/B/C.wasReleased();
        BtnA/B/C.wasreleasedFor()
        BtnA/B/C.pressedFor(uint32_t ms);
        BtnA/B/C.releasedFor(uint32_t ms);
        BtnA/B/C.lastChange();
 */
// #define ESP32

#ifndef _UI_H_
#define _UI_H_

#if defined(ESP32)

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "FS.h"
#include "SD.h"

#include "Display.h"
#include "utility/Config.h"
#include "utility/Button.h"
#include "Free_Fonts.h"

#define MAX_NODE 32
#define MAX_SUB 32
struct Node
{
  bool EN = false;
  struct Sub_node
  {
  };
  Sub_node sub[MAX_SUB];
  char*  titlePic;
  String title_1st;
  String title_2nd;
  uint16_t title1stColor = WHITE;
  uint16_t title2ndColor = WHITE;
  uint8_t titleSize = 2;
};

class UI
{
public:
  UI();
  void begin(bool LCDEnable = true, bool SDEnable = true, bool SerialEnable = true);
  void update();
  void setBrightness(uint8_t brightness);
  void main();
// Button API
#define DEBOUNCE_MS 10
  Button BtnA = Button(BUTTON_A_PIN, true, DEBOUNCE_MS);
  Button BtnB = Button(BUTTON_B_PIN, true, DEBOUNCE_MS);
  Button BtnC = Button(BUTTON_C_PIN, true, DEBOUNCE_MS);

  // LCD
  Display Lcd = Display();

  void node_init(uint8_t size = 1);
  void node_setTitle(uint8_t index, String title_1st = "", String title_2nd = "");
  void node_setTitleColor(uint8_t index,uint16_t title_1st, uint16_t title_2nd);
  void node_setAllTitleColor(uint16_t title_1st, uint16_t title_2nd);
  void node_setTitlePic(uint8_t index, char *path);

private:
//custom RGB565
#define AQUA 0x4B1D
//
#define MAIN 0
#define STAT 1
#define CONT 2
#define SETT 3
#define RETURN_MS 1000
#define POS_RETURN_MS 200
  //
  uint8_t panel = 0;
  //
  unsigned long long time, counter,return_c;
  bool isInited;
  uint8_t idealRefreshRate = 5;
  uint8_t refreshRate = 5;
  uint8_t period; // 1000/refreshRate miliseconds per frame
  uint16_t backgroundColor = BLACK;
  int fps = 0;
  //
  struct Menu
  {
    const String BACK = "  Back  ";
    const String OK = "   Ok   ";
    const String NEXT = "  Next  ";
    uint16_t fillColor = BLACK;
    uint16_t lineColor = WHITE;
    bool leftBtn_visible = true;
    bool midBtn_visible = true;
    bool rightBtn_visible = true;
    bool leftBtn_enable = false;
    bool midBtn_enable = true;
    bool rightBtn_enable = true;
    String leftText = BACK;
    String midText = OK;
    String rightText = NEXT;
  };
  Menu m;
  void menu_disp();
  //
  bool tick_frame();
  void framerate_update();
  //
  struct Main_panel
  {
    uint16_t fillColor = LIGHTGREY;
    uint16_t lineColor = WHITE;
    uint16_t selLineColor = WHITE;
    uint16_t selFillColor = AQUA;
    uint16_t titleColor = WHITE;
    uint8_t titleSize = 3;
    uint8_t select = 0b100;
    uint8_t lastSel = 0b000;
    String title = "Main Menu";
  };
  Main_panel m_panel;
  void main_panel();
  //
  struct Stat_panel
  {
    uint16_t fillColor = LIGHTGREY;
    uint16_t lineColor = WHITE;
    uint16_t selLineColor = WHITE;
    uint16_t selFillColor = AQUA;
    uint16_t titleColor = WHITE;
    uint8_t titleSize = 3;
    String title = "Status";
  };
  Stat_panel sa_panel;
  void stat_panel();
  //
  struct Cont_panel
  {
    uint8_t size = 1;
    int index = 0;
    int lastIndex = -1;
    uint16_t fillColor = BLACK;
    uint16_t lineColor = WHITE;
    uint16_t selLineColor = WHITE;
    uint16_t selFillColor = AQUA;
    uint16_t titleColor = WHITE;
    uint8_t titleSize = 3;
    String title = "Control";
  };
  Cont_panel c_panel;
  void cont_panel();
  //
  struct Sett_panel
  {
    uint16_t fillColor = LIGHTGREY;
    uint16_t lineColor = WHITE;
    uint16_t selLineColor = WHITE;
    uint16_t selFillColor = AQUA;
    uint16_t titleColor = WHITE;
    uint8_t titleSize = 3;
    uint8_t select = 0b100;
    uint8_t lastSel = 0b000;
    String title = "Settings";
  };
  Sett_panel st_panel;
  void sett_panel();
  //
  Node node[MAX_NODE];
  //
};
#else
#error “This library only supports boards with ESP32 processor.”
#endif
#endif
