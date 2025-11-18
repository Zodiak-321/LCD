#include "LCD.h"


static void LCD_Delay(uint16_t ms)
{
    HAL_Delay(ms);
}

static LCD_StatusType LCD_SPI_WriteCommand(LCD_Data *LCD_Data, uint8_t command)
{
    HAL_StatusTypeDef res;

    LCD_CS_RESET();
    LCD_DC_RESET();
    res = HAL_SPI_Transmit(LCD_Data->hspi, &command, 1, 0x0F00);
    LCD_CS_SET();

    switch(res)
    {
        case HAL_OK: return LCD_OK;
        case HAL_ERROR: return LCD_ERROR;
        case HAL_BUSY: return LCD_BUSY;
        case HAL_TIMEOUT: return LCD_TIMEOUT;
        default: return LCD_ERROR;
    }
}

static LCD_StatusType LCD_SPI_WriteData(LCD_Data *LCD_Data, uint8_t *data, uint16_t data_size)
{
    HAL_StatusTypeDef res;

    LCD_CS_RESET();
    LCD_DC_SET();
    res = HAL_SPI_Transmit(LCD_Data->hspi, data, data_size, 0x0F00);
    LCD_CS_RESET();

    switch(res)
    {
        case HAL_OK: return LCD_OK;
        case HAL_ERROR: return LCD_ERROR;
        case HAL_BUSY: return LCD_BUSY;
        case HAL_TIMEOUT: return LCD_TIMEOUT;
        default: return LCD_ERROR;
    }
}

// static LCD_StatusType LCD_SPI_WriteRead(LCD_Data *LCD_Data, uint8_t reg, uint8_t *readbuf, uint8_t read_size)
// {
//     HAL_StatusTypeDef res;

//     LCD_CS(0);
//     res = HAL_SPI_TransmitReceive(LCD_Data->hspi, &reg, readbuf, read_size, 0xFF00);
//     LCD_CS(1);

//     switch(res)
//     {
//         case HAL_OK: return LCD_OK;
//         case HAL_ERROR: return LCD_ERROR;
//         case HAL_BUSY: return LCD_BUSY;
//         case HAL_TIMEOUT: return LCD_TIMEOUT;
//         default: return LCD_ERROR;
//     }
// }

static void LCD_Reset(void)
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    LCD_Delay(200);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    LCD_Delay(50);
}

static LCD_StatusType LCD_SetDirection(LCD_Data *LCD_Data)
{ 
    LCD_StatusType res;
    uint8_t command;
    uint8_t data;

    command = ST7796S_CMD_MADCTL;
	switch(LCD_Data->dir)
    {		  
		case 0:	data = (1<<3)|(1<<6); break;
		case 1: data = (1<<3)|(1<<5); break;
		case 2:	data = (1<<3)|(1<<7); break;
		case 3: data = (1<<3)|(1<<7)|(1<<6)|(1<<5); break;	
		default: return LCD_ERROR;
	}	

    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, &data, 1);
    if(res != LCD_OK) return res;

    return res;
}	 

LCD_StatusType LCD_Init(LCD_Data *LCD_Data)
{
    LCD_StatusType res;
    uint8_t command;
    uint8_t data[14] = {0};

    LCD_LED_SET();     // 开启背光
    LCD_Reset();

    // 退出睡眠
    command = ST7796S_CMD_SLPOUT;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;

    // LCD_Delay(120);

    // 刷新顺序
    command = ST7796S_CMD_MADCTL;
    data[0] = 0x48;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 接口像素格式
    command = ST7796S_CMD_COLMOD;
    data[0] = 0x55;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 开启高级模式Command table II(1)
    command = ST7796S_CMD_CSCON;
    data[0] = 0xC3;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 开启高级模式Command table II(2)
    command = ST7796S_CMD_CSCON;
    data[0] = 0x96;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 显示倒转控制
    command = ST7796S_CMD_DIC;
    data[0] = 0x01;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;
    
    // 配置工作模式
    command = ST7796S_CMD_EM;
    data[0] = 0xC6;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 电源配置
    command = ST7796S_CMD_PWR1;
    data[0] = 0x80;
    data[1] = 0x45;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 2);
    if(res != LCD_OK) return res;

    command = ST7796S_CMD_PWR2;
    data[0] = 0x12;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    command = ST7796S_CMD_PWR3;
    data[0] = 0xA7;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    command = ST7796S_CMD_VCMPCTL;
    data[0] = 0x0A;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 显示输出控制调整
    command = ST7796S_CMD_DOCA;
    data[0] = 0x40;
    data[1] = 0x8A;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = 0x29;
    data[5] = 0x19;
    data[6] = 0xA5;
    data[7] = 0x33;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 8);
    if(res != LCD_OK) return res; 

    // 调整伽马值
    command = ST7796S_CMD_PGC;
    data[0] = 0xD0;
    data[1] = 0x08;
    data[2] = 0x0F;
    data[3] = 0x06;
    data[4] = 0x06;
    data[5] = 0x33;
    data[6] = 0x30;
    data[7] = 0x33;
    data[8] = 0x47;
    data[9] = 0x17;
    data[10] = 0x13;
    data[11] = 0x13;
    data[12] = 0x2B;
    data[13] = 0x31;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 14);
    if(res != LCD_OK) return res; 

    command = ST7796S_CMD_NGC;
    data[0] = 0xD0;
    data[1] = 0x0A;
    data[2] = 0x11;
    data[3] = 0x0B;
    data[4] = 0x09;
    data[5] = 0x07;
    data[6] = 0x2F;
    data[7] = 0x33;
    data[8] = 0x47;
    data[9] = 0x38;
    data[10] = 0x15;
    data[11] = 0x16;
    data[12] = 0x2C;
    data[13] = 0x32;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 14);
    if(res != LCD_OK) return res;

    // 退出高级模式Command table II(1)
    command = ST7796S_CMD_CSCON;
    data[0] = 0x3C;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    // 退出高级模式Command table II(2)
    command = ST7796S_CMD_CSCON;
    data[0] = 0x69;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 1);
    if(res != LCD_OK) return res;

    LCD_Delay(120);

    command = ST7796S_CMD_INVON;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;

    // 开启显示
    command = ST7796S_CMD_DISPON;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;

    // 调整显示方向
    res = LCD_SetDirection(LCD_Data);

    res = LCD_Clear(LCD_Data, WHITE);

    return res;
}

LCD_StatusType LCD_SetWindow(LCD_Data *LCD_Data, uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_StatusType res;
    uint8_t command;
    uint8_t data[4] = {0};
    
    command = ST7796S_CMD_CASET;
    data[0] = (uint8_t)(xStar >> 8);
    data[1] = (uint8_t)(xStar & 0x00FF);
    data[2] = (uint8_t)(xEnd >> 8);
    data[3] = (uint8_t)(xEnd & 0x00FF);
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 4);
    if(res != LCD_OK) return res;

    command = ST7796S_CMD_RASET;
    data[0] = (uint8_t)(yStar >> 8);
    data[1] = (uint8_t)(yStar & 0x00FF);
    data[2] = (uint8_t)(yEnd >> 8);
    data[3] = (uint8_t)(yEnd & 0x00FF);
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;
    res = LCD_SPI_WriteData(LCD_Data, data, 4);
    if(res != LCD_OK) return res;

    // 预准备写入数据
    command = ST7796S_CMD_RAMWR;
    res = LCD_SPI_WriteCommand(LCD_Data, command);
    if(res != LCD_OK) return res;

    return res;
}

LCD_StatusType LCD_SetCursor(LCD_Data *LCD_Data, uint16_t x, uint16_t y)
{
    return LCD_SetWindow(LCD_Data, x, y, x, y);
}

LCD_StatusType LCD_WriteColor(LCD_Data *LCD_Data, uint16_t Color)
{
    LCD_StatusType res;
    uint8_t data[2] = {0};

    data[0] = (uint8_t)(Color >> 8);        
    data[1] = (uint8_t)(Color & 0x00FF); 

    res = LCD_SPI_WriteData(LCD_Data, data, 2);
    if(res != LCD_OK) return res;

    return res;
}

// LCD_StatusType LCD_Clear(LCD_Data *LCD_Data, uint16_t Color)
// {
//     LCD_StatusType res; 

//     LCD_SetWindow(LCD_Data, 0, 0, LCD_Data->width - 1, LCD_Data->height - 1);
//     for(int i = 0; i < LCD_Data->height; i++)
//         for(int j = 0; j < LCD_Data->width; j++)
//         {
//             res = LCD_WriteColor(LCD_Data, Color);
//             if(res != LCD_OK) return res;
//         }

//     return res;
// }

// LCD_StatusType LCD_Clear(LCD_Data *LCD_Data, uint16_t Color)
// {
//     LCD_StatusType res; 

//     int a = 100;
//     uint8_t data[200] = {0};
    
//     for(int i = 0; i < a; i++)
//     {
//         data[i * 2]= (uint8_t)(Color >> 8); 
//         data[i * 2 + 1] = (uint8_t)(Color & 0x00FF); 
//     }

//     LCD_SetWindow(LCD_Data, 0, 0, LCD_Data->width - 1, LCD_Data->height - 1);

//     for(int i = 0; i < (LCD_Data->width * LCD_Data->height) / a; i++)
//     {
//         res = LCD_SPI_WriteData(LCD_Data, data, a * 2);
//         if(res != LCD_OK) return res;
//     }

//     return res;
// }

LCD_StatusType LCD_Clear(LCD_Data *LCD_Data, uint16_t Color)
{
    LCD_StatusType res; 

    int a = 64;
    uint8_t *data = (uint8_t *)malloc(a * 2 * sizeof(uint8_t));
    
    for(int i = 0; i < a; i++)
    {
        data[i * 2]= (uint8_t)(Color >> 8); 
        data[i * 2 + 1] = (uint8_t)(Color & 0x00FF); 
    }

    LCD_SetWindow(LCD_Data, 0, 0, LCD_Data->width - 1, LCD_Data->height - 1);

    for(int i = 0; i < (LCD_Data->width * LCD_Data->height) / a; i++)
    {
        res = LCD_SPI_WriteData(LCD_Data, data, a * 2);
        if(res != LCD_OK) break;
    }

    free(data);
    return res;
}

// LCD_StatusType LCD_ClearPart(LCD_Data *LCD_Data, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t Color)
// {
//     LCD_StatusType res; 

//     LCD_SetWindow(LCD_Data, xStart, yStart, xEnd - 1, yEnd - 1);
//     for(int i = 0; i < yEnd - yStart; i++)
//         for(int j = 0; j < xEnd - xStart; j++)
//         {
//             res = LCD_WriteColor(LCD_Data, Color);
//             if(res != LCD_OK) return res;
//         }

//     return res;
// }

LCD_StatusType LCD_ClearPart(LCD_Data *LCD_Data, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t Color)
{
    LCD_StatusType res; 

    int a = yEnd - yStart;
    uint8_t *data = (uint8_t *)malloc(a * 2 * sizeof(uint8_t));
    
    for(int i = 0; i < a; i++)
    {
        data[i * 2]= (uint8_t)(Color >> 8); 
        data[i * 2 + 1] = (uint8_t)(Color & 0x00FF); 
    }

    LCD_SetWindow(LCD_Data, xStart, yStart, xEnd - 1, yEnd - 1);

    for(int i = 0; i < ((yEnd - yStart) * (xEnd - xStart)) / a; i++)
    {
        res = LCD_SPI_WriteData(LCD_Data, data, a * 2);
        if(res != LCD_OK) break;
    }

    free(data);
    return res;
}
