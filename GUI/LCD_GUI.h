#ifndef __LCD_GUI_H__
#define __LCD_GUI_H__

#include "LCD.h"

void GUI_DrawPoint(LCD_Data *LCD_data, uint16_t x, uint16_t y, uint16_t color);
void GUI_DrawLine(LCD_Data *LCD_data, int16_t xStart, int16_t yStart, int16_t xEnd, int16_t yEnd, uint16_t size, uint16_t color);
void GUI_DrawCircle(LCD_Data *LCD_data, uint16_t x, uint16_t y, uint16_t R, uint8_t fill, uint16_t color);
void GUI_DrawRectangle(LCD_Data *LCD_data, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint8_t fill, uint8_t size, uint16_t color);
void GUI_ShowChar(LCD_Data *LCD_data, uint16_t x, uint16_t y, char character, uint8_t size, uint16_t frontColor,uint16_t backColor);
void GUI_ShowString(LCD_Data *LCD_data, uint16_t x, uint16_t y, char *string, uint8_t size, uint16_t frontColor,uint16_t backColor);
void GUI_ShowNum(LCD_Data *LCD_data, uint16_t x, uint16_t y, int32_t num, uint8_t size, uint16_t frontColor,uint16_t backColor);
void GUI_ShowFloatNum(LCD_Data *LCD_data, uint16_t x, uint16_t y, float num, uint8_t float_num, uint8_t size, uint16_t frontColor, uint16_t backColor);

#endif
