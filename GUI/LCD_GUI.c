#include "LCD_GUI.h"
#include "LCD_Font.h"

void GUI_DrawPoint(LCD_Data *LCD_data, uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetCursor(LCD_data, x, y);
    LCD_WriteColor(LCD_data, color);
}

static void _GUI_DrawCircle(LCD_Data *LCD_data, uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color)
{
    GUI_DrawPoint(LCD_data, x + x1, y + y1, color);
    GUI_DrawPoint(LCD_data, x - x1, y + y1, color);
    GUI_DrawPoint(LCD_data, x + x1, y - y1, color);
    GUI_DrawPoint(LCD_data, x - x1, y - y1, color);
    GUI_DrawPoint(LCD_data, x + y1, y + x1, color);
    GUI_DrawPoint(LCD_data, x - y1, y + x1, color);
    GUI_DrawPoint(LCD_data, x + y1, y - x1, color);
    GUI_DrawPoint(LCD_data, x - y1, y - x1, color);
}

void GUI_DrawCircle(LCD_Data *LCD_data, uint16_t x, uint16_t y, uint16_t R, uint8_t fill, uint16_t color)
{
    int xc = 0, yc = R, yi = 0, d = 3 - 2 * R;

    if(fill == 1)
        while(xc <= yc)
        {
            for(yi = xc; yi <= yc; yi++)
                _GUI_DrawCircle(LCD_data, x, y, xc, yi, color);
            if(d < 0)
                d = d + 4 * xc + 6;
            else 
            {
                d = d + 4 * (xc - yc) + 10;
                yc--;
            }
            xc++;
        }
    else 
        while(xc <= yc)
        {
            _GUI_DrawCircle(LCD_data, x, y, xc, yc, color);
            if(d < 0)
                d = d + 4 * xc + 6;
            else 
            {
                d = d + 4 * (xc - yc) + 10;
                yc--;
            }
            xc++;
        }
}

void GUI_DrawLine(LCD_Data *LCD_data, int16_t xStart, int16_t yStart, int16_t xEnd, int16_t yEnd, uint16_t size, uint16_t color)
{
    // 获取屏幕边界
    int16_t screen_width = LCD_data->width;
    int16_t screen_height = LCD_data->height;
    
    int16_t dx = abs(xEnd - xStart);
    int16_t dy = abs(yEnd - yStart);
    int16_t sx = (xStart < xEnd) ? 1 : -1;
    int16_t sy = (yStart < yEnd) ? 1 : -1;
    int16_t err = dx - dy;
    int16_t x = xStart;
    int16_t y = yStart;
    
    while(1)
    {
        // 只绘制在屏幕范围内的点
        if(x >= 0 && x < screen_width && y >= 0 && y < screen_height)
        {
            if(size == 1)
                GUI_DrawPoint(LCD_data, x, y, color);
            else
                GUI_DrawCircle(LCD_data, x, y, size, 1, color);
        }
        
        if(x == xEnd && y == yEnd)
            break;
            
        int16_t e2 = 2 * err;
        if(e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if(e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

void GUI_DrawRectangle(LCD_Data *LCD_data, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint8_t fill, uint8_t size, uint16_t color)
{
    GUI_DrawLine(LCD_data, xStart, yStart, xEnd, yStart, size, color);
    GUI_DrawLine(LCD_data, xStart, yStart, xStart, yEnd, size, color);
    GUI_DrawLine(LCD_data, xEnd, yStart, xEnd, yEnd, size, color);
    GUI_DrawLine(LCD_data, xStart, yEnd, xEnd, yEnd, size, color);
    
    if(fill == 1)
        LCD_ClearPart(LCD_data, xStart, yStart, xEnd + 1, yEnd + 1, color);
}

void GUI_ShowChar(LCD_Data *LCD_data, uint16_t x, uint16_t y, char character, uint8_t size, uint16_t frontColor,uint16_t backColor)
{
    character -= ' ';
    uint8_t temp;
    
    LCD_SetWindow(LCD_data, x, y, x + size / 2 - 1, y + size - 1);

    for(int i = 0; i < size; i++)
    {
        if(size == 12) temp = asc2_1206[character][i];
        else if(size == 16) temp = asc2_1608[character][i];

        for(int j = 0; j < size / 2; j++)
        {
            if(temp & 0x01) LCD_WriteColor(LCD_data, frontColor);
            else LCD_WriteColor(LCD_data, backColor);
            temp >>= 1;
        }
    }

    LCD_SetWindow(LCD_data, 0, 0, LCD_data->width - 1, LCD_data->height - 1);
}

void GUI_ShowString(LCD_Data *LCD_data, uint16_t x, uint16_t y, char *string, uint8_t size, uint16_t frontColor,uint16_t backColor)
{
    while((*string >= ' ') && (*string <= '~'))
    {
        if(x > (LCD_data->width - 1) || y > (LCD_data->height - 1))
            return;
        GUI_ShowChar(LCD_data, x, y, *string, size, frontColor, backColor);
        x += size / 2;
        string++;
    }
}

void GUI_ShowNum(LCD_Data *LCD_data, uint16_t x, uint16_t y, int32_t num, uint8_t size, uint16_t frontColor, uint16_t backColor)
{
    char buffer[12];
    int index = 0;
    int32_t num_temp = num;
    
    if(num == 0) { GUI_ShowChar(LCD_data, x, y, '0', size, frontColor, backColor); return; }
    
    if(num < 0)
        num_temp = -num;
    
    while(num_temp > 0) 
    {
        buffer[index++] = (num_temp % 10) + '0';
        num_temp /= 10;
    }
    
    if(num < 0) 
        buffer[index++] = '-';
    
    for(int i = index - 1; i >= 0; i--) 
    {
        GUI_ShowChar(LCD_data, x, y, buffer[i], size, frontColor, backColor);
        x += size / 2;
    }
}

void GUI_ShowFloatNum(LCD_Data *LCD_data, uint16_t x, uint16_t y, float num, uint8_t float_num, uint8_t size, uint16_t frontColor, uint16_t backColor)
{
    int32_t value, multiplier = 1;
    
    if(num < 0) 
    {
        GUI_ShowChar(LCD_data, x, y, '-', size, frontColor, backColor);
        x += size / 2;
        num = -num;
    }
    
    for(int i = 0; i < float_num; i++) multiplier *= 10;
    value = (int32_t)(num * multiplier + 0.5f);
    
    int32_t integer = value / multiplier;
    if(integer == 0) 
    {
        GUI_ShowChar(LCD_data, x, y, '0', size, frontColor, backColor);
        x += size / 2;
    } 
    else 
    {
        char temp[10];
        uint8_t digits = 0;
        int32_t temp_val = integer;
        
        while(temp_val > 0) 
        {
            temp[digits++] = (temp_val % 10) + '0';
            temp_val /= 10;
        }
        
        for(int i = digits; i > 0; i--) 
        {
            GUI_ShowChar(LCD_data, x, y, temp[i - 1], size, frontColor, backColor);
            x += size / 2;
        }
    }
    
    GUI_ShowChar(LCD_data, x, y, '.', size, frontColor, backColor);
    x += size / 2;
    
    int32_t decimal = value % multiplier;
    for(int i = 0; i < float_num; i++) 
    {
        multiplier /= 10;
        GUI_ShowChar(LCD_data, x, y, (decimal / multiplier) % 10 + '0', size, frontColor, backColor);
        x += size / 2;
    }
}
