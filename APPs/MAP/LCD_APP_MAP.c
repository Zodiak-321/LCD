#include "LCD_APP_MAP.h"

MAP_CHAIN *pHead = NULL;  // 全局链表头指针

// 创建新节点
MAP_CHAIN *MAP_CreateNode(float x, float y)
{
    MAP_CHAIN *pNew = (MAP_CHAIN *)malloc(sizeof(MAP_CHAIN));
    if (pNew == NULL)
        return NULL;

    pNew->x = x;
    pNew->y = y;
    pNew->next_point = NULL;

    return pNew;
}

// 删除指定节点
void MAP_DeleteNode(MAP_CHAIN *nodeToDelete)
{   
    if (pHead == NULL || nodeToDelete == NULL)
        return;
        
    if (pHead == nodeToDelete)
    {
        pHead = nodeToDelete->next_point;
        free(nodeToDelete);
    } 
    else 
    {
        MAP_CHAIN *current = pHead;
        while (current != NULL && current->next_point != nodeToDelete)
            current = current->next_point;
        if (current != NULL)
        {
            current->next_point = nodeToDelete->next_point;
            free(nodeToDelete);
        }
    }
}

// 删除整个链表
void MAP_DeleteAllChain(void)
{
    MAP_CHAIN *current = pHead;
    MAP_CHAIN *nextNode;
    
    while (current != NULL) 
    {
        nextNode = current->next_point;
        free(current);
        current = nextNode;
    }
    
    pHead = NULL;
}

// 添加节点到链表末尾
void MAP_AddToEnd(MAP_CHAIN *point)
{
    if (point == NULL)
        return;
        
    if (pHead == NULL)
        pHead = point;
    else 
    {
        MAP_CHAIN *current = pHead;
        while (current->next_point != NULL)
            current = current->next_point;
        current->next_point = point;
    }
}

void MAP_AddToHead(MAP_CHAIN *point)
{
    if (point == NULL)
        return;

    point->next_point = pHead;
    pHead = point;
}

// 计算链表长度
int MAP_ChainCount(void)
{
    MAP_CHAIN *current = pHead;
    int count = 0;
    
    while (current != NULL)
    {
        current = current->next_point;
        count++;
    }

    return count;
}

/*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*/

// 改变屏幕尺度
static void MAP_ChangeScale(MAP_Data *MAP_data, float newScale)
{
    MAP_data->mapScale = newScale;
}

// 改变屏幕竖线间隔
static void MAP_ChangeInterval(MAP_Data *MAP_data, uint16_t newGridRowInterval, uint16_t newGridColInterval)
{
    MAP_data->gridRowInterval = newGridRowInterval;
    MAP_data->gridColInterval = newGridColInterval;
}

// 改变屏幕原点
static void MAP_ChangeOrigin(MAP_Data *MAP_data, int16_t newXOrigin, int16_t newYOrigin)
{
    MAP_data->xOrigin = newXOrigin;
    MAP_data->yOrigin = newYOrigin;
}

/*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*/

void MAP_DrawGrid(LCD_Data *LCD_data, MAP_Data *MAP_data)
{
    for(int i = 0; i < LCD_data->width / MAP_data->gridColInterval; i++)
        GUI_DrawLine(LCD_data, MAP_data->gridColInterval * i, 0, MAP_data->gridColInterval * i, LCD_data->height, 1, BLACK);
    for(int i = 0; i < LCD_data->height / MAP_data->gridRowInterval; i++)
        GUI_DrawLine(LCD_data, 0, MAP_data->gridRowInterval * i, LCD_data->width, MAP_data->gridRowInterval * i, 1, BLACK);
}

void MAP_DrawGridWithRealScale(LCD_Data *LCD_data, MAP_Data *MAP_data)
{
    for(int i = 0; i < LCD_data->width / MAP_data->gridColInterval; i++)
    {
        GUI_DrawLine(LCD_data, MAP_data->gridColInterval * i, 0, MAP_data->gridColInterval * i, LCD_data->height, 1, BLACK);
        GUI_ShowNum(LCD_data, MAP_data->gridColInterval * i + 1, 0, MAP_data->mapScale * MAP_data->gridColInterval * i, 12, BLACK, WHITE);
    }
    for(int i = 0; i < LCD_data->height / MAP_data->gridRowInterval; i++)
    {
        GUI_DrawLine(LCD_data, 0, MAP_data->gridRowInterval * i, LCD_data->width, MAP_data->gridRowInterval * i, 1, BLACK);
        GUI_ShowNum(LCD_data, 0, MAP_data->gridRowInterval * i + 1, MAP_data->mapScale * MAP_data->gridRowInterval * i, 12, BLACK, WHITE);
    }
}

void MAP_DrawGridWithRealScale_Origin(LCD_Data *LCD_data, MAP_Data *MAP_data)
{
    for(int i = 0; i < (LCD_data->width - MAP_data->xOrigin) / MAP_data->gridColInterval; i++)
    {
        GUI_DrawLine(LCD_data, MAP_data->xOrigin + MAP_data->gridColInterval * i, 0, MAP_data->xOrigin + MAP_data->gridColInterval * i, LCD_data->height, 1, BLACK);
        GUI_ShowNum(LCD_data, MAP_data->xOrigin + MAP_data->gridColInterval * i + 1, 0, 0 + MAP_data->mapScale * MAP_data->gridColInterval * i, 12, BLACK, WHITE);
    }
    for(int i = 1; i < MAP_data->xOrigin / MAP_data->gridColInterval; i++)
    {
        GUI_DrawLine(LCD_data, MAP_data->xOrigin - MAP_data->gridColInterval * i, 0, MAP_data->xOrigin - MAP_data->gridColInterval * i, LCD_data->height, 1, BLACK);
        GUI_ShowNum(LCD_data, MAP_data->xOrigin - MAP_data->gridColInterval * i + 1, 0, 0 - MAP_data->mapScale * MAP_data->gridColInterval * i, 12, BLACK, WHITE);
    }
    for(int i = 0; i < (LCD_data->height - MAP_data->yOrigin) / MAP_data->gridRowInterval; i++)
    {
        GUI_DrawLine(LCD_data, 0, MAP_data->yOrigin + MAP_data->gridRowInterval * i, LCD_data->width, MAP_data->yOrigin + MAP_data->gridRowInterval * i, 1, BLACK);
        GUI_ShowNum(LCD_data, 0, MAP_data->yOrigin + MAP_data->gridRowInterval * i + 1, 0 + MAP_data->mapScale * MAP_data->gridRowInterval * i, 12, BLACK, WHITE);
    }
    for(int i = 1; i < MAP_data->yOrigin / MAP_data->gridRowInterval; i++)
    {
        GUI_DrawLine(LCD_data, 0, MAP_data->yOrigin - MAP_data->gridRowInterval * i, LCD_data->width, MAP_data->yOrigin - MAP_data->gridRowInterval * i, 1, BLACK);
        GUI_ShowNum(LCD_data, 0, MAP_data->yOrigin - MAP_data->gridRowInterval * i + 1, 0 - MAP_data->mapScale * MAP_data->gridRowInterval * i, 12, BLACK, WHITE);
    }
}

/*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*/

void MAP_ExtendGrid(MAP_Data *MAP_data)
{
    MAP_ChangeInterval(MAP_data, MAP_data->gridRowInterval + 5, MAP_data->gridColInterval + 5);
}

void MAP_CompressGrid(MAP_Data *MAP_data)
{
    if(MAP_data->gridRowInterval - 5 >= 1 && MAP_data->gridColInterval - 5 >= 1)
        MAP_ChangeInterval(MAP_data, MAP_data->gridRowInterval - 5, MAP_data->gridColInterval - 5);
}

void MAP_ZoomIn(MAP_Data *MAP_data)
{
    MAP_ChangeScale(MAP_data, MAP_data->mapScale + 2);
}

void MAP_ZoomOut(MAP_Data *MAP_data)
{
    if(MAP_data->mapScale - 2 >= 1)
        MAP_ChangeScale(MAP_data, MAP_data->mapScale - 2);
}

void MAP_LeftTranslation(MAP_Data *MAP_data)
{
    MAP_ChangeOrigin(MAP_data, MAP_data->xOrigin - 10, MAP_data->yOrigin);
}

void MAP_RightTranslation(MAP_Data *MAP_data)
{
    MAP_ChangeOrigin(MAP_data, MAP_data->xOrigin + 10, MAP_data->yOrigin);
}

void MAP_UpTranslation(MAP_Data *MAP_data)
{
    MAP_ChangeOrigin(MAP_data, MAP_data->xOrigin, MAP_data->yOrigin + 10);
}

void MAP_DownTranslation(MAP_Data *MAP_data)
{
    MAP_ChangeOrigin(MAP_data, MAP_data->xOrigin, MAP_data->yOrigin - 10);
}

void MAP_SetHeadAsOrigin(MAP_Data *MAP_data, LCD_Data *LCD_data)
{
    MAP_data->xOrigin = LCD_data->width / 2 - pHead->x / MAP_data->mapScale;
    MAP_data->yOrigin = LCD_data->height / 2 - pHead->y / MAP_data->mapScale;
}

/*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*//*==========*/

void MAP_AddPoint(LCD_Data *LCD_data, float x, float y)
{
    MAP_CHAIN *node = MAP_CreateNode(x, y);
    MAP_AddToHead(node);
}

void MAP_UpdataMap(LCD_Data *LCD_data, MAP_Data *MAP_data)
{
    MAP_CHAIN *current = pHead;

    while (current != NULL)
    {
        int16_t fix_current_x = current->x / MAP_data->mapScale + MAP_data->xOrigin;
        int16_t fix_current_y = current->y / MAP_data->mapScale + MAP_data->yOrigin;
        int16_t fix_next_x = current->next_point->x / MAP_data->mapScale + MAP_data->xOrigin;
        int16_t fix_next_y = current->next_point->y / MAP_data->mapScale + MAP_data->yOrigin;

        if(fix_current_x >= 0 && fix_current_x <= LCD_data->width && fix_current_y >= 0 && fix_current_y <= LCD_data->height)
            if(current == pHead)
                GUI_DrawCircle(LCD_data, fix_current_x, fix_current_y, 7, 1, 0xF123);
            else
                GUI_DrawCircle(LCD_data, fix_current_x, fix_current_y, 5, 1, 0x123F);

        if(current->next_point != NULL)
            GUI_DrawLine(LCD_data, fix_current_x, fix_current_y, fix_next_x, fix_next_y, 3, 0x7567);

        current = current->next_point;
    }
}

