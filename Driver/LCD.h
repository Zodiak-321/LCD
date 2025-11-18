#ifndef __LCD_H__
#define __LCD_H__

#include "main.h"
#include "spi.h"
#include "stdlib.h"

/* Command Table 1 */
#define ST7796S_CMD_NOP                 0x00
#define ST7796S_CMD_SWRESET             0x01
#define ST7796S_CMD_RDDID               0x04
#define ST7796S_CMD_RD_DSI_ERRORS       0x05
#define ST7796S_CMD_RDDST               0x09
#define ST7796S_CMD_RDDPM               0x0A
#define ST7796S_CMD_RDDMADCTL           0x0B
#define ST7796S_CMD_RDDCOLMOD           0x0C
#define ST7796S_CMD_RDDIM               0x0D
#define ST7796S_CMD_RDDSM               0x0E
#define ST7796S_CMD_RDDSDR              0x0F
#define ST7796S_CMD_SLPIN               0x10
#define ST7796S_CMD_SLPOUT              0x11
#define ST7796S_CMD_PTLON               0x12
#define ST7796S_CMD_NORON               0x13
#define ST7796S_CMD_INVOFF              0x20
#define ST7796S_CMD_INVON               0x21
#define ST7796S_CMD_DISPOFF             0x28
#define ST7796S_CMD_DISPON              0x29
#define ST7796S_CMD_CASET               0x2A
#define ST7796S_CMD_RASET               0x2B
#define ST7796S_CMD_RAMWR               0x2C
#define ST7796S_CMD_RAMRD               0x2E
#define ST7796S_CMD_PTLAR               0x30
#define ST7796S_CMD_VSCRDEF             0x33
#define ST7796S_CMD_TEOFF               0x34
#define ST7796S_CMD_TEON                0x35
#define ST7796S_CMD_MADCTL              0x36
#define ST7796S_CMD_VSCSAD              0x37
#define ST7796S_CMD_IDMOFF              0x38
#define ST7796S_CMD_IDMON               0x39
#define ST7796S_CMD_COLMOD              0x3A
#define ST7796S_CMD_WRMEMC              0x3C
#define ST7796S_CMD_RDMEMC              0x3E
#define ST7796S_CMD_STE                 0x44
#define ST7796S_CMD_GSCAN               0x45
#define ST7796S_CMD_WRDISBV             0x51
#define ST7796S_CMD_RDDISBV             0x52
#define ST7796S_CMD_WRCTRLD             0x53
#define ST7796S_CMD_RDCTRLD             0x54
#define ST7796S_CMD_WRCABC              0x55
#define ST7796S_CMD_RDCABC              0x56
#define ST7796S_CMD_WRCABCMB            0x5E
#define ST7796S_CMD_RDCABCMB            0x5F
#define ST7796S_CMD_RDFCS               0xAA
#define ST7796S_CMD_RDCFCS              0xAF
#define ST7796S_CMD_RDID1               0xDA
#define ST7796S_CMD_RDID2               0xDB
#define ST7796S_CMD_RDID3               0xDC

/* Command Table 2 */
#define ST7796S_CMD_IFMODE              0xB0
#define ST7796S_CMD_FRMCTR1             0xB1
#define ST7796S_CMD_FRMCTR2             0xB2
#define ST7796S_CMD_FRMCTR3             0xB3
#define ST7796S_CMD_DIC                 0xB4
#define ST7796S_CMD_BPC                 0xB5
#define ST7796S_CMD_DFC                 0xB6
#define ST7796S_CMD_EM                  0xB7
#define ST7796S_CMD_PWR1                0xC0
#define ST7796S_CMD_PWR2                0xC1
#define ST7796S_CMD_PWR3                0xC2
#define ST7796S_CMD_VCMPCTL             0xC5
#define ST7796S_CMD_VCM_OFFSET          0xC6
#define ST7796S_CMD_NVMADW              0xD0
#define ST7796S_CMD_NVMBPROG            0xD1
#define ST7796S_CMD_NVMSTRD             0xD2
#define ST7796S_CMD_RDID4               0xD3
#define ST7796S_CMD_PGC                 0xE0
#define ST7796S_CMD_NGC                 0xE1
#define ST7796S_CMD_DGC1                0xE2
#define ST7796S_CMD_DGC2                0xE3
#define ST7796S_CMD_DOCA                0xE8
#define ST7796S_CMD_CSCON               0xF0
#define ST7796S_CMD_SPIRC               0xFB

#define BLACK   0x0000
#define WHITE   0xFFFF

#define LCD_LED_SET()      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LCD_LED_RESET()    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define LCD_CS_SET()       HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_CS_RESET()     HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_DC_SET()       HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)
#define LCD_DC_RESET()     HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)

typedef struct 
{
    SPI_HandleTypeDef *hspi;

    int width;
    int height;

    uint8_t dir;
}LCD_Data;

typedef enum
{
    LCD_OK = 0,
    LCD_ERROR = 1,
    LCD_BUSY = 2,
    LCD_TIMEOUT = 3,
}LCD_StatusType;



LCD_StatusType LCD_Init(LCD_Data *LCD_Data);

LCD_StatusType LCD_SetWindow(LCD_Data *LCD_Data, uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
LCD_StatusType LCD_SetCursor(LCD_Data *LCD_Data, uint16_t x, uint16_t y);

LCD_StatusType LCD_WriteColor(LCD_Data *LCD_Data, uint16_t Color);
LCD_StatusType LCD_Clear(LCD_Data *LCD_Data, uint16_t Color);
LCD_StatusType LCD_ClearPart(LCD_Data *LCD_Data, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t Color);

#endif