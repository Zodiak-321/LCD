#ifndef __FT6336_H__
#define __FT6336_H__

#include "main.h"
#include "i2c.h"

#define FT6336U_I2C_ADDR                        0x38

/******************************************
 * FT6336U 触摸芯片寄存器定义
 * 基于官方寄存器表格整理
 ******************************************/

/* ====== 模式切换寄存器 ====== */
#define FT6336U_REG_MODE_SWITCH                 0x00  // 寄存器页面（模式）切换

/* ====== 状态与数据寄存器 ====== */
#define FT6336U_REG_TD_STATUS                   0x02  // 报点个数
#define FT6336U_REG_GEST_ID                     0x02  // 手势ID（与TD_STATUS相同地址）

/* 第一点触摸数据 */
#define FT6336U_REG_P1_XH                       0x03  // 第1点X坐标高4位 + 事件标志
#define FT6336U_REG_P1_XL                       0x04  // 第1点X坐标低8位
#define FT6336U_REG_P1_YH                       0x05  // 第1点Y坐标高4位 + ID
#define FT6336U_REG_P1_YL                       0x06  // 第1点Y坐标低8位
#define FT6336U_REG_P1_WEIGHT                   0x07  // 第1点重量
#define FT6336U_REG_P1_MISC                     0x08  // 第1点杂项

/* 第二点触摸数据 */
#define FT6336U_REG_P2_XH                       0x09  // 第2点X坐标高4位 + 事件标志
#define FT6336U_REG_P2_XL                       0x0A  // 第2点X坐标低8位
#define FT6336U_REG_P2_YH                       0x0B  // 第2点Y坐标高4位 + ID
#define FT6336U_REG_P2_YL                       0x0C  // 第2点Y坐标低8位
#define FT6336U_REG_P2_WEIGHT                   0x0D  // 第2点重量
#define FT6336U_REG_P2_MISC                     0x0E  // 第2点杂项

/* ====== 配置寄存器 - 阈值与滤波 ====== */
#define FT6336U_REG_ID_G_THGROUP                0x80  // 触摸阈值
#define FT6336U_REG_ID_G_PEAKTH                 0x81  // peak阈值
#define FT6336U_REG_ID_G_THDIFF                 0x85  // 点滤波范围阈值

/* ====== 配置寄存器 - 模式控制 ====== */
#define FT6336U_REG_ID_G_CTRL                   0x86  // monitor模式开关
#define FT6336U_REG_ID_G_TIMEENTERMONITOR       0x87  // 进入monitor延时
#define FT6336U_REG_ID_G_PERIODACTIVE           0x88  // 工作模式扫描周期
#define FT6336U_REG_ID_G_PERIODMONITOR          0x89  // MONITOR模式扫描周期
#define FT6336U_REG_ID_G_MODE                   0xA4  // 报点INT模式
#define FT6336U_REG_ID_G_PMODE                  0xA5  // 芯片工作模式
#define FT6336U_REG_ID_G_STATE                  0xBC  // 工作模式控制

/* ====== 配置寄存器 - 特殊功能 ====== */
#define FT6336U_REG_ID_G_FRQHOPFLG              0x8A  // 跳频标志
#define FT6336U_REG_ID_G_FREQ_HOPPING_EN        0x8B  // 充电器标识
#define FT6336U_REG_ID_G_CURFREQIDX             0x8C  // 当前工作频点
#define FT6336U_REG_ID_G_TEST_MODE_FILTER       0x96  // 测试模式滤波使能
#define FT6336U_REG_ID_G_VIRTUAL_KEY_THRES      0xA9  // 虚拟按键阈值
#define FT6336U_REG_ID_G_IS_CALLING             0xAD  // 通话标志字
#define FT6336U_REG_ID_G_FACE_DEC_MODE          0xB0  // 近距离感应使能

/* ====== 识别寄存器 ====== */
#define FT6336U_REG_ID_G_CIPHER_MID             0x9F  // 芯片代号（中字节）
#define FT6336U_REG_ID_G_CIPHER_LOW             0xA0  // 芯片代号（低字节）
#define FT6336U_REG_ID_G_LIB_VERSION_H          0xA1  // 库版本高字节
#define FT6336U_REG_ID_G_LIB_VERSION_L          0xA2  // 库版本低字节
#define FT6336U_REG_ID_G_CIPHER_HIGH            0xA3  // 芯片代号（高字节）
#define FT6336U_REG_ID_G_FIRMID                 0xA6  // 固件版本
#define FT6336U_REG_ID_G_FOCALTECH_ID           0xA8  // 供应商ID
#define FT6336U_REG_ID_G_RELEASE_CODE_ID        0xAF  // 公版release code ID

/* ====== 手势识别寄存器 ====== */
#define FT6336U_REG_ID_G_SPEC_GESTURE_ENABLE    0xD0  // 特殊手势模式使能
#define FT6336U_REG_GESTURE_CONFIG1             0xD1  // 手势配置1
#define FT6336U_REG_GESTURE_CONFIG2             0xD2  // 手势配置2
#define FT6336U_REG_GESTURE_ID                  0xD3  // 手势ID号
#define FT6336U_REG_GESTURE_CONFIG3             0xD5  // 手势配置3
#define FT6336U_REG_GESTURE_CONFIG4             0xD6  // 手势配置4
#define FT6336U_REG_GESTURE_CONFIG5             0xD7  // 手势配置5
#define FT6336U_REG_GESTURE_CONFIG6             0xD8  // 手势配置6

/* ====== 工厂模式寄存器 ====== */
#define FT6336U_FACTORY_REG_WORK_MODE           0x02  // 工作模式
#define FT6336U_FACTORY_REG_RS_MODE_EN          0x07  // 微短测试开关
#define FT6336U_FACTORY_REG_AcmdREG             0x08  // 通信ACK寄存器
#define FT6336U_FACTORY_REG_TP_CHANNEL_NUM      0x0A  // 通道数
#define FT6336U_FACTORY_REG_TP_KEY_NUM          0x0B  // 虚拟按键数
#define FT6336U_FACTORY_REG_AFE_SAMPLE_CYCLE    0x0C  // 扫描采样次数
#define FT6336U_FACTORY_REG_AREA1_K1_DELAY      0x0D  // 第1区充电延时
#define FT6336U_FACTORY_REG_AREA1_K2_DELAY      0x0E  // 第1区采样延时
#define FT6336U_FACTORY_REG_AREA2_K1_DELAY      0x0F  // 第2区充电延时
#define FT6336U_FACTORY_REG_AREA2_K2_DELAY      0x10  // 第2区采样延时
#define FT6336U_FACTORY_REG_AREA3_K1_DELAY      0x11  // 第3区充电延时
#define FT6336U_FACTORY_REG_AREA3_K2_DELAY      0x12  // 第3区采样延时
#define FT6336U_FACTORY_REG_AREA2_K_CNT         0x13  // 第2区起始通道
#define FT6336U_FACTORY_REG_AREA3_K_CNT         0x14  // 第3区起始通道
#define FT6336U_FACTORY_REG_AFE_SAMPLE_MODE     0x15  // AFE采样方式
#define FT6336U_FACTORY_REG_AFE_SPLIT_EN        0x16  // Split功能
#define FT6336U_FACTORY_REG_AFE_SPLIT_SEL       0x17  // Split模式选择
#define FT6336U_FACTORY_REG_WATER_PROOF_LEVEL   0x18  // 防水功能等级
#define FT6336U_FACTORY_REG_CLOSE_MODE_EN       0x19  // 近距离感应模式开关
#define FT6336U_FACTORY_REG_VDD5_VALUE          0x1A  // VDD5电压等级
#define FT6336U_FACTORY_REG_CF_VALUE            0x1B  // CF范围
#define FT6336U_FACTORY_REG_AUTO_CLB_SAMPLE_CYCLES 0x1C  // 自动校正扫描次数
#define FT6336U_FACTORY_REG_2ND_AUTO_CLB        0x1D  // 二次自动校正
#define FT6336U_FACTORY_REG_MID5_EN             0x1E  // Mid5算法
#define FT6336U_FACTORY_REG_NLC_EN              0x1F  // NLC算法
#define FT6336U_FACTORY_REG_NLC_THR             0x20  // NLC阈值
#define FT6336U_FACTORY_REG_IIR_EN              0x21  // IIR算法
#define FT6336U_FACTORY_REG_SYNC_MODE           0x22  // Sync模式选择
#define FT6336U_FACTORY_REG_VSYNC_WIDTH         0x23  // Vsync信号时间宽度
#define FT6336U_FACTORY_REG_VSYNC_FRONT_PORCH   0x25  // vsync front porch时间
#define FT6336U_FACTORY_REG_VSYNC_BACK_PORCH    0x26  // vsync back porch时间
#define FT6336U_FACTORY_REG_HSYNC_OFFSET1       0x27  // 第1分区Hsync延时
#define FT6336U_FACTORY_REG_HSYNC_OFFSET2       0x28  // 第2分区Hsync延时
#define FT6336U_FACTORY_REG_HSYNC_OFFSET3       0x29  // 第3分区Hsync延时
#define FT6336U_FACTORY_REG_HSYNC_CYCLE         0x2A  // Hsync采样次数
#define FT6336U_FACTORY_REG_SSCG_MODE           0x2B  // SSCG模式
#define FT6336U_FACTORY_REG_SSCG_DELAY          0x2C  // SSCG延时
#define FT6336U_FACTORY_REG_BIG_NOISE_MASK      0x2D  // 大噪声区域屏蔽
#define FT6336U_FACTORY_REG_BIG_NOISE_DUMMY     0x2E  // 大噪声区域dummy
#define FT6336U_FACTORY_REG_CHIP_TYPE           0x2F  // IC型号
#define FT6336U_FACTORY_REG_CHANNEL_ORDER_ADDR_R 0x31 // 读通道顺序开始地址
#define FT6336U_FACTORY_REG_CB_ADDR_W           0x32  // 写CB开始地址
#define FT6336U_FACTORY_REG_CB_ADDR_R           0x33  // 读CB开始地址
#define FT6336U_FACTORY_REG_RAWDATA_ADDR_R      0x34  // 读Rawdata开始地址
#define FT6336U_FACTORY_REG_RAWDATA_BUF         0x35  // Raw Data数据
#define FT6336U_FACTORY_REG_CHANNEL_ORDER_BUF   0x37  // 通道顺序
#define FT6336U_FACTORY_REG_CB_BUF              0x39  // CB数据
#define FT6336U_FACTORY_REG_TEST_1XCB_8XCB      0x41  // 工厂Test_modeX Cb选择
#define FT6336U_FACTORY_REG_ID_G_MODE12_GET_RAW 0x42  // 模式切换扫描标志
#define FT6336U_FACTORY_REG_ID_G_FACTORY_MODE   0xAE  // 工厂模式种类
#define FT6336U_FACTORY_REG_RS_DATA_BUF         0xF4  // 微短测试数据

/* ====== 模式切换值定义 ====== */
#define FT6336U_MODE_NORMAL                     0x00  // 普通模式
#define FT6336U_MODE_FACTORY                    0x40  // 工厂模式

/* ====== 工作模式定义 ====== */
#define FT6336U_WORK_MODE_INFO                  0x00  // InFO模式
#define FT6336U_WORK_MODE_NORMAL                0x01  // 正常工作模式
#define FT6336U_WORK_MODE_FACTORY               0x03  // 工厂模式
#define FT6336U_WORK_MODE_AUTO_CLB              0x04  // 自动校正模式

/* ====== 手势ID定义 ====== */
#define FT6336U_GESTURE_NONE                    0x00  // 无手势
#define FT6336U_GESTURE_UP                      0x10  // 向上滑动
#define FT6336U_GESTURE_DOWN                    0x14  // 向下滑动
#define FT6336U_GESTURE_LEFT                    0x18  // 向左滑动
#define FT6336U_GESTURE_RIGHT                   0x1C  // 向右滑动
#define FT6336U_GESTURE_DOUBLE_CLICK            0x22  // 双击

/* ====== 芯片型号定义 ====== */
#define FT6336U_CHIP_TYPE_FT6236G               0x00
#define FT6336U_CHIP_TYPE_FT6336G               0x01
#define FT6336U_CHIP_TYPE_FT6336U               0x02  // 当前芯片型号
#define FT6336U_CHIP_TYPE_FT6436U               0x03

/* ====== 触摸事件标志位掩码 ====== */
#define FT6336U_TOUCH_EVENT_MASK                0xC0  // 事件标志位在XH寄存器的高2位
#define FT6336U_TOUCH_ID_MASK                   0xF0  // ID标志位在YH寄存器的高4位
#define FT6336U_COORDINATE_MASK                 0x0F  // 坐标高4位掩码

/* ====== 触摸事件类型 ====== */
#define FT6336U_EVENT_PRESS_DOWN                0x00  // 按下
#define FT6336U_EVENT_LIFT_UP                   0x01  // 抬起
#define FT6336U_EVENT_CONTACT                   0x02  // 接触
#define FT6336U_EVENT_RESERVED                  0x03  // 保留



#define FT6336U_RST_SET()   HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET)
#define FT6336U_RST_RESET()   HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_RESET)




typedef struct FT6336U_Data
{
    I2C_HandleTypeDef *hi2c;

    uint8_t eventPoint1;
    uint8_t IDPoint1;
    uint16_t xPoint1;
    uint16_t yPoint1;
    uint8_t weightPoint1;

    uint8_t eventPoint2;
    uint8_t IDPoint2;
    uint16_t xPoint2;
    uint16_t yPoint2;
    uint8_t weightPoint2;

}FT6336U_Data;

typedef enum FT6336U_StatusType
{
    FT6336U_OK = 0,
    FT6336U_ERROR = 1,
    FT6336U_BUSY = 2,
    FT6336U_TIMEOUT = 3,
}FT6336U_StatusType;


FT6336U_StatusType FT6336U_Init(FT6336U_Data *FT6336U_data);

FT6336U_StatusType FT6336U_FullPowerMode(FT6336U_Data *FT6336U_data);
FT6336U_StatusType FT6336U_LowPowerMode(FT6336U_Data *FT6336U_data);

FT6336U_StatusType FT6336U_ReadPoint(FT6336U_Data *FT6336U_data);

#endif
