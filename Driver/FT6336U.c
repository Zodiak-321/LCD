#include "FT6336U.h"

static FT6336U_StatusType FT6336U_I2C_Write(FT6336U_Data *FT6336U_data, uint8_t reg, uint8_t *data, uint16_t data_size)
{
    HAL_StatusTypeDef res;

    res = HAL_I2C_Mem_Write(FT6336U_data->hi2c, FT6336U_I2C_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, data, data_size, 0xFF00);

    switch(res)
    {
        case HAL_OK: return FT6336U_OK;
        case HAL_ERROR: return FT6336U_ERROR;
        case HAL_BUSY: return FT6336U_BUSY;
        case HAL_TIMEOUT: return FT6336U_TIMEOUT;
        default: return FT6336U_ERROR;
    }
}

static FT6336U_StatusType FT6336U_I2C_Read(FT6336U_Data *FT6336U_data, uint8_t reg, uint8_t *data, uint16_t data_size)
{
    HAL_StatusTypeDef res;

    res = HAL_I2C_Mem_Read(FT6336U_data->hi2c, FT6336U_I2C_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, data, data_size, 0xFF00);

    switch(res)
    {
        case HAL_OK: return FT6336U_OK;
        case HAL_ERROR: return FT6336U_ERROR;
        case HAL_BUSY: return FT6336U_BUSY;
        case HAL_TIMEOUT: return FT6336U_TIMEOUT;
        default: return FT6336U_ERROR;
    }
}

static void FT6336U_Delay(uint16_t ms)
{
    HAL_Delay(ms);
}

static FT6336U_StatusType FT6336U_WHOAMI_Check(FT6336U_Data *FT6336U_data)
{
    FT6336U_StatusType res;
    uint8_t reg;
    uint8_t data[2] = {0};

    reg = FT6336U_REG_ID_G_CIPHER_LOW;
    res = FT6336U_I2C_Read(FT6336U_data, reg, data, 1);     // 0x02
    if(res != FT6336U_OK) return res;
    if(data[0] != 0x02) return FT6336U_ERROR;

    reg = FT6336U_REG_ID_G_CIPHER_MID;
    res = FT6336U_I2C_Read(FT6336U_data, reg, data, 1);     // 0x26
    if(res != FT6336U_OK) return res;
    if(data[0] != 0x26) return FT6336U_ERROR;

    reg = FT6336U_REG_ID_G_CIPHER_HIGH;
    res = FT6336U_I2C_Read(FT6336U_data, reg, data, 1);     // 0x64
    if(res != FT6336U_OK) return res;
    if(data[0] != 0x64) return FT6336U_ERROR;

    reg = FT6336U_REG_ID_G_FOCALTECH_ID;
    res = FT6336U_I2C_Read(FT6336U_data, reg, data, 1);     // 0x11
    if(res != FT6336U_OK) return res;
    if(data[0] != 0x11) return FT6336U_ERROR;

    return res;
}

FT6336U_StatusType FT6336U_Init(FT6336U_Data *FT6336U_data)
{
    FT6336U_StatusType res;
    uint8_t reg;
    uint8_t data[2] = {0};

    FT6336U_RST_RESET();
    FT6336U_Delay(20);
    FT6336U_RST_SET();
    FT6336U_Delay(200);

    // 切换为正常模式
    reg = FT6336U_REG_MODE_SWITCH;
    data[0] = 0x00;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    res = FT6336U_WHOAMI_Check(FT6336U_data);
    if(res != FT6336U_OK) return res;

    /*=====================================================*/
    // 设置触摸阈值
    reg = FT6336U_REG_ID_G_THGROUP;
    data[0] = 0x55;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置peak阈值
    reg = FT6336U_REG_ID_G_PEAKTH;
    data[0] = 0x06;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置点滤波范围阈值
    reg = FT6336U_REG_ID_G_THDIFF;
    data[0] = 0x80;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    /*=====================================================*/
    // 设置INT模式
    reg = FT6336U_REG_ID_G_MODE;
    data[0] = 0x00;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置监测模式开关
    reg = FT6336U_REG_ID_G_CTRL;
    data[0] = 0x01;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    res = FT6336U_FullPowerMode(FT6336U_data);
    if(res != FT6336U_OK) return res;

    return res;
}

FT6336U_StatusType FT6336U_FullPowerMode(FT6336U_Data *FT6336U_data)
{
    FT6336U_StatusType res;
    uint8_t reg;
    uint8_t data[2] = {0};

    // 设置工作模式
    reg = FT6336U_REG_ID_G_PMODE;
    data[0] = 0x00;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置监测模式检测时间
    reg = FT6336U_REG_ID_G_TIMEENTERMONITOR;
    data[0] = 0x1E;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置正常工作扫描周期
    reg = FT6336U_REG_ID_G_PERIODACTIVE;
    data[0] = 0x04;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置检测模式扫描周期
    reg = FT6336U_REG_ID_G_PERIODMONITOR;
    data[0] = 0x08;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    return res;
}

FT6336U_StatusType FT6336U_LowPowerMode(FT6336U_Data *FT6336U_data)
{
    FT6336U_StatusType res;
    uint8_t reg;
    uint8_t data[2] = {0};

    // 设置工作模式
    reg = FT6336U_REG_ID_G_PMODE;
    data[0] = 0x01;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置监测模式检测时间
    reg = FT6336U_REG_ID_G_TIMEENTERMONITOR;
    data[0] = 0x11;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置正常工作扫描周期
    reg = FT6336U_REG_ID_G_PERIODACTIVE;
    data[0] = 0x14;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    // 设置检测模式扫描周期
    reg = FT6336U_REG_ID_G_PERIODMONITOR;
    data[0] = 0x14;
    res = FT6336U_I2C_Write(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    return res;
}

FT6336U_StatusType FT6336U_ReadPoint(FT6336U_Data *FT6336U_data)
{
    FT6336U_StatusType res;
    uint8_t reg;
    uint8_t data[12] = {0};

    // 读取点个数
    reg = FT6336U_REG_TD_STATUS;
    res = FT6336U_I2C_Read(FT6336U_data, reg, data, 1);
    if(res != FT6336U_OK) return res;

    if(data[0] == 1)
    {
        reg = FT6336U_REG_P1_XH;
        res = FT6336U_I2C_Read(FT6336U_data, reg, data, 6);
        if(res != FT6336U_OK) return res;

        FT6336U_data->xPoint1 = ((data[0] & 0x0F) << 8) | data[1];
        FT6336U_data->yPoint1 = ((data[2] & 0x0F) << 8) | data[3];
        FT6336U_data->eventPoint1 = (data[0] & 0xC0) >> 6;
        FT6336U_data->IDPoint1 = (data[2] & 0xF0) >> 4;
        FT6336U_data->weightPoint1 = data[4];
    }
    else if(data[0] == 2)
    {
        reg = FT6336U_REG_P1_XH;
        res = FT6336U_I2C_Read(FT6336U_data, reg, data, 12);
        if(res != FT6336U_OK) return res;

        FT6336U_data->xPoint1 = ((data[0] & 0x0F) << 8) | data[1];
        FT6336U_data->yPoint1 = ((data[2] & 0x0F) << 8) | data[3];
        FT6336U_data->eventPoint1 = (data[0] & 0xC0) >> 6;
        FT6336U_data->IDPoint1 = (data[2] & 0xF0) >> 4;
        FT6336U_data->weightPoint1 = data[4];
        
        FT6336U_data->xPoint2 = ((data[6] & 0x0F) << 8) | data[7];
        FT6336U_data->yPoint2 = ((data[8] & 0x0F) << 8) | data[9];
        FT6336U_data->eventPoint2 = (data[6] & 0xC0) >> 6;
        FT6336U_data->IDPoint2 = (data[7] & 0xF0) >> 4;
        FT6336U_data->weightPoint2 = data[10];
    }

    return res;
}

