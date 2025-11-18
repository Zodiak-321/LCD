#ifndef __LCD_APP_MAP__
#define __LCD_APP_MAP__

#include "LCD_GUI.h"




typedef struct MAP_CHAIN
{
    float x;
    float y;

    struct MAP_CHAIN *next_point;

}MAP_CHAIN;
extern MAP_CHAIN *pHead;

typedef struct MAP_Data
{
    int16_t xOrigin;
    int16_t yOrigin;

    uint16_t gridRowInterval;
    uint16_t gridColInterval;

    float mapScale;              // 真实地图尺度 厘米/格子
}MAP_Data;

static void MAP_ChangeScale(MAP_Data *MAP_data, float newScale);
static void MAP_ChangeInterval(MAP_Data *MAP_data, uint16_t newGridRowInterval, uint16_t newGridColInterval);
static void MAP_ChangeOrigin(MAP_Data *MAP_data, int16_t newXOrigin, int16_t newYOrigin);



void MAP_DrawGrid(LCD_Data *LCD_data, MAP_Data *MAP_data);
void MAP_DrawGridWithRealScale(LCD_Data *LCD_data, MAP_Data *MAP_data);
void MAP_DrawGridWithRealScale_Origin(LCD_Data *LCD_data, MAP_Data *MAP_data);

void MAP_ExtendGrid(MAP_Data *MAP_data);
void MAP_CompressGrid(MAP_Data *MAP_data);
void MAP_ZoomIn(MAP_Data *MAP_data);
void MAP_ZoomOut(MAP_Data *MAP_data);
void MAP_LeftTranslation(MAP_Data *MAP_data);
void MAP_RightTranslation(MAP_Data *MAP_data);
void MAP_UpTranslation(MAP_Data *MAP_data);
void MAP_DownTranslation(MAP_Data *MAP_data);
void MAP_SetHeadAsOrigin(MAP_Data *MAP_data, LCD_Data *LCD_data);

void MAP_AddPoint(LCD_Data *LCD_data, float x, float y);
void MAP_UpdataMap(LCD_Data *LCD_data, MAP_Data *MAP_data);

#endif
