////////////////////////////////////////////////////////////////////////////////
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,//
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED	  //
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.	  //
//																			  //
//							Copyright (c) UIROBOT.							  //
//							 All rights reserved.							  //
///////////////////////////////////////////////////////////////////////////////
/*
*    该头文件用于错误解析时所使用函数接口定义
*
*     UIError.h
*
*
*/
#pragma once

#include <string>
using namespace std;



#define SDK_RETURN_SUCCESS                          (0x0)   // SDK成功返回

// 控制器错误码
#define RTCN_ERR_SYS_GWSD                            9   // 全组警告	
//620
#define RTCN_ERR_SYS_LOCK                            10	// 系统急停并上锁	
#define RTCN_ERR_SYS_STOP                            11	// 620 电机急停过程中
#define RTCN_ERR_SYS_OVHT                            12	// 620 系统温度过热
#define RTCN_ERR_SYS_POVL                            13	// 620 对外供电系统检测到短路/过载	
#define RTCN_ERR_SYS_SCME                            14	// 720 与 SERVO 通讯故障
#define RTCN_ERR_SYS_SRST                            15	// 720 与 SERVO 需要断电重启
#define RTCN_ERR_SYS_SALM                            16	// 720 检出 SERVO 报错
#define RTCN_ERR_SYS_SIOE                            17  // 720 与 SERVO IO故障  	
#define RTCN_ERR_SYS_SLAV                            18	// 附属芯片发生通讯故障							
#define RTCN_ERR_CTX_TMOT                            20	// CAN 报文发送超时（无人响应）
#define RTCN_ERR_MXN_ACC                             21	// 加速度超限
#define RTCN_ERR_MXN_EBRK                            22	// 刹车上锁, 无法BG
#define RTCN_ERR_MXN_NBRK                            23	// 刹车未锁, 无法脱机
#define RTCN_ERR_MXN_MOFF                            24	// 脱机状态, 无法 BG
#define RTCN_ERR_MXN_SPHL                            25	// 速度超限检出
#define RTCN_ERR_MXN_PALL                            26	// 反向工作位置超限检出
#define RTCN_ERR_MXN_PAHL                            27	// 正向工作位置超限检出
#define RTCN_ERR_MXN_SPEL                            28	// 速度误差超限检出（仅闭环控制）
#define RTCN_ERR_MXN_PQER                            29	// 位移误差超限检出（仅闭环控制）	
#define RTCN_ERR_MXN_ENCS                            30	// 编码器通讯错误
#define RTCN_ERR_MXN_ENCB                            31	// ABS 电池无电压
#define RTCN_ERR_INS_SYNT                            50	// 指令语法错误 620
#define RTCN_ERR_INS_NUMB                            51	// 指令数据错误 620
#define RTCN_ERR_INS_IDXR                            52	// 指令索引（下标）错误
#define RTCN_ERR_SYS_STTM                            53	// 电机转动中，无法更改系统时间
#define RTCN_ERR_MXN_DCSD                            60  // SD减速度 小于DC减速度
#define RTCN_ERR_MXN_RUNG                            61  // 电机转动中，无法更改
#define RTCN_ERR_MXN_STOG                            65	// 电机转动中，无法设置绝对值编码器原点	
#define RTCN_ERR_PVT_RUNG                            70	// 电机运动中，无法设置 PV 起始点 或 MP[0]
#define RTCN_ERR_PVT_WPOV                            71	// QP,QV,QT,QA 索引号大于MP[6](水位)
#define RTCN_ERR_PVT_IOFN                            72	// QA Mask 不符合 I/O 端口功能要求
#define RTCN_ERR_PVB_OVFL                            73  // PVT Buffer 溢出
#define RTCN_ERR_IOC_ADIO                            90	// I/O 端口功能配置错误
#define RTCN_ERR_URT_SHT                            255 // UART 报文小于5字节(包括所有)
#define RTCN_ERR_URT_OLN                            254 // UART 报文超过最大长度
#define RTCN_ERR_URT_TMO                            253 // UART 报文接受超时（字节间隔太长）
#define RTCN_ERR_URT_CRC                            252 // UART 报文CRC错误
#define RTCN_ERR_URT_NEN                            251 // UART 报文没有结尾符
#define RTCN_ERR_RRB_OVF                            250 // UART RRB overflow
#define RTCN_ERR_RRB_LVW                            249 // UART RRB level Warning
#define RTCN_ERR_CSB_FUL                            248 // UART CSB full Warning



// 算法错误
#define ERROR_CERTIFICATION_FAIL                    (0x80000100)  // 认证失败 
#define ERROR_OPEN_INIFILE_FAIL                     (0x80000101)   // 打开机器人类型定义文件失败 
#define ERROR_OPEN_CFGFILE_FAIL                     (0x80000102)  // 打开机器人结构参数文件失败 
#define ERROR_SET_ROBOTCFG_FAIL                     (0x80000103)  // 配置机器人结构参数失败
#define ERROR_INIT_ROBOT_NDEF                       (0x80000104)  // 机器人配置类型未定义
#define ERROR_OPEN_DATAFILE_FAIL                    (0x80000105)  // 创建仿真数据文件失败

#define ERROR_PARA_POINT_NULL                       (0x80000201)   // 参数指针为NULL
#define ERROR_PARA_TRJTM_OVER                       (0x80000202)  // 行程时间超长
#define ERROR_PARA_SPEED_ZERO                       (0x80000203)  // 行程速度为零
#define ERROR_BELT_SPEED_ZERO                       (0x80000204)  // 传输带速度为零

#define WARNG_POSI_LIMIT_XYLH                       (0x00000301)   // POS > 工作半径上部
#define WARNG_POSI_LIMIT_XYLL                       (0x00000302)    // POS > 工作半径下部
#define WARNG_POSI_LIMIT_PZLH                       (0x00000303)   // POS > 工作高度
#define WARNG_POSI_LIMIT_PZLL                       (0x00000304)  // POS < 工作高度
#define WARNG_POSI_LIMIT_PRLH                       (0x00000305)  // 中轴法兰 > +360°
#define WARNG_POSI_LIMIT_PRLL                       (0x00000306)   // 中轴法兰 < -360°

#define ERROR_POSI_LIMIT_XYLH                       (0x80000301)  // SCARA: 大于工作范围 / DELTA: 超出上部工作范围
#define ERROR_POSI_LIMIT_XYLL                       (0x80000302)  // SCARA: 小于工作范围 / DELTA: 超出上部工作范围
#define ERROR_POSI_LIMIT_PZLH                       (0x80000303)  // POS > 工作高度
#define ERROR_POSI_LIMIT_PZLL                       (0x80000304)  // POS < 工作高度
#define ERROR_POSI_LIMIT_PRLH                       (0x80000305)  // 中轴法兰 > +360°
#define ERROR_POSI_LIMIT_PRLL                       (0x80000306)  // 中轴法兰 < -360°

#define WARNG_AXIS_RANGE_POS1                       (0x00000311)   // Axis-1 轴角过大
#define WARNG_AXIS_RANGE_NEG1                       (0x00000312)  // Axis-1 轴角过小
#define WARNG_AXIS_RANGE_POS2                       (0x00000313)  // Axis-2 轴角过大
#define WARNG_AXIS_RANGE_NEG2                       (0x00000314)   // Axis-2 轴角过小
#define WARNG_AXIS_RANGE_POS3                       (0x00000315)  // Axis-3 轴角过大
#define WARNG_AXIS_RANGE_NEG3                       (0x00000316)  // Axis-3 轴角过小
#define WARNG_AXIS_RANGE_POS4                       (0x00000317)   // Axis-4 轴角过大
#define WARNG_AXIS_RANGE_NEG4                       (0x00000318)  // Axis-4 轴角过小

#define ERROR_AXIS_RANGE_POS1                       (0x80000311)  // Axis-1 轴角过大
#define ERROR_AXIS_RANGE_NEG1                       (0x80000312)  // Axis-1 轴角过小
#define ERROR_AXIS_RANGE_POS2                       (0x80000313)  // Axis-2 轴角过大
#define ERROR_AXIS_RANGE_NEG2                       (0x80000314)  // Axis-2 轴角过小
#define ERROR_AXIS_RANGE_POS3                       (0x80000315)  // Axis-3 轴角过大
#define ERROR_AXIS_RANGE_NEG3                       (0x80000316)  // Axis-3 轴角过小
#define ERROR_AXIS_RANGE_POS4                       (0x80000317)  // Axis-4 轴角过大
#define ERROR_AXIS_RANGE_NEG4                       (0x80000318)  // Axis-4 轴角过小

#define WARNG_AXIS_SPEED_HIGH1                      (0x00000350) // Axis-1 速度较大, > 2400rpm
#define WARNG_AXIS_SPEED_HIGH2                      (0x00000351)  // Axis-2 速度较大, > 2400rpm 
#define WARNG_AXIS_SPEED_HIGH3                      (0x00000352)   // Axis-3 速度较大, > 2400rpm
#define WARNG_AXIS_SPEED_HIGH4                      (0x00000353)  // Axis-4 速度较大, > 2400rpm

#define ERROR_DIVIDED_BYZERO                        (0x80000500)  // 除数（即分子）为零
// UART操作错误
#define ERROR_SDK_ERROR_UART_HADLE                  (0x8000000A)   // UART 文件打开失败
#define ERROR_SDK_ERROR_BWRT_NDATA                  (0x8000000B)   // Message Write 传入数据无效
#define ERROR_SDK_ERROR_BWRT_TMOUT                  (0x8000000C) // Message Write 等待“正确接收”反馈超时
#define ERROR_SDK_ERROR_BWRT_ERPNT                  (0x8000000D) // Message Write 无法找到续传所要的断点
#define ERROR_SDK_ERROR_BWRT_REWRT                  (0x8000000E) // Message Write 断点重启发送多次失败
// SDK错误
#define ERROR_SDK_ERROR_SHUTDOWN                    (0x90000002)  // SDK已经退出
#define ERROR_SDK_ERROR_PACKAGE                     (0x90000003)  // 封装报文错误
#define ERROR_SDK_ERROR_TIMEOUT                     (0x90000004)  //  指令等待超时
#define ERROR_SDK_ERROR_ANAYLSEPACKAGE              (0x90000005)  //  解析报文失败
#define ERROR_SDK_ERROR_ANAYLSENOTPACKAGE           (0x90000006)  // 解析报文时没有报文输入
#define ERROR_SDK_ERROR_NOT_OUTBUFFER               (0x90000007) // 解析报文时没有传入输出地址
#define ERROR_SDK_ERROR_RECEIVE_ERROR               (0x90000008)  // 接收到错误报文
#define ERROR_SDK_ERROR_ANAYLSE_ERROR_LEN           (0x90000009)  // 接收到错误长度报文
#define ERROR_SDK_ERROR_PARAMETER                   (0x90000010)  // 输入参数错误
#define ERROR_SDK_ERROR_WRITE_ERROR                 (0x90000011)  // 写失败
#define ERROR_SDK_ERROR_TCW_ERROR                   (0x90000012)  // 错误指令
#define ERROR_SDK_ERROR_PORT_ERROR                  (0x90000013)  // 端口操作失败
#define ERROR_SDK_ERROR_INITIAL_ERROR               (0x90000014)  // 初始化失败
#define ERROR_USBC_ERROR_NO_DEVICE                  (0x90000015)  // usb设备不存在
#define ERROR_USBC_ERROR_USB_WRITE                  (0x90000016) // usb设备写错误
#define ERROR_USBC_ERROR_USB_READ                   (0x90000017) // usb设备读错误

// 机器人错误
#define RTCN_ROBOT_SYS_ERROR                        (0xA0000000)
#define ERROR_PVTWRITE_FAILED                       (0xA0000001)   // 写入PVT失败
#define ERROR_URGENT_STOP_FAILED                    (0xA0000002) // 紧急停止
#define ERROR_GETPVT_FAILED                         (0xA0000003)  // 无法生成PVT列表
#define ERROR_CAMERAOBJECT_NOTFOUND_FAILED          (0xA0000004)  // 相机用户变量未找到
#define ERROR_ROBOT_STATE_FAILED                    (0xA0000005)  // 机器人状态不正确
#define ERROR_ROBOT_COORDINATE_FAILED               (0xA0000006)  // 坐标错误
#define ERROR_ROBOT_TRANSMODEL_STATE_FAILED         (0xA0000007)  // 透传模式下
#define ERROR_ROBOT_FILE_NOT_EXIST_FAILED           (0xA0000008)  // 文件不存在
#define ERROR_ROBOT_FILE_NOT_PROGRAM_FAILED         (0xA0000009)  // 不是机器人程序
#define ERROR_ROBOT_TEMPERATURE                     (0xA000000A) // 温度报警
#define ERROR_ROBOT_EMERGYSTOP                      (0xA000000B) // 紧急停止
#define ERROR_ROBOT_FILE_OPEN_FAILED                (0xA000000E)  // 文件打开出错
#define ERROR_ROBOT_CAN_BUS_FAILED                  (0xA000000F)  // CAN通讯出错
#define ERROR_ROBOT_STALL_FAILED                    (0xA0000010)  // 堵转
#define ERROR_MOTOR_DISABLE_FAILED                  (0xA0000011)  // 电机脱机错误
#define ERROR_ROBOT_FILE_SIZE_FAILED                (0xA0000012)  // 文件太大
#define ERROR_ROBOT_RECV_UI_FILE_TIMEOUT_FAILED     (0xA0000013)  // 接收文件超时
#define ERROR_ROBOT_FILE_ROBOTTYPE_FAILED           (0xA0000015)  //  脚本中机器人类型不正确
#define ERROR_ROBOT_MOTOR_RTCN_FAILED               (0xA0000016)  // 电机报错
#define ERROR_ROBOT_MALLOC_MEMORY_FAILED            (0xA000001F)  // 申请内存出错
#define ERROR_MOTOR_SOFTSAFE_ERR                    (0xA0000020) // 超过软件限位
#define ERROR_MOTORCOUNT                            (0xA0000021)  // 电机个数不一致  
#define ERROR_BATTERYLOW                            (0xA0000022)  // 电池电量不足
#define ERROR_ROBOT_POSITION_COORDINATE_FAILED      (0xA0000023)  // 坐标和位置不一致
#define ERROR_MOTOR_LOCKED_FAILED                   (0xA0000024)  // 电机锁定
#define ERROR_TASK_PVT_COUNT_ZERO                   (0xA0000025)  // 没有PVT可以运行
#define ERROR_PVT_EPCHK_FAILED                      (0xA0000026)  // PVT运动结束时，有电机脉冲误差太大
#define ERROR_ROBOT_SAFEAREA_X_FAILED               (0xA0000029)  //  坐标x在安全区域之外
#define ERROR_ROBOT_SAFEAREA_Y_FAILED               (0xA000002A)  //  坐标y在安全区域之外
#define ERROR_ROBOT_SAFEAREA_Z_FAILED               (0xA000002B)  //  坐标z在安全区域之外
#define ERROR_ROBOT_CURRENT_COORDINATE_FAILED       (0xA000002C)  //  当前机器人坐标与运动起始点坐标不一致

// 应用层错误
#define ROBOT_RETURN_TIMEOUT                        (0xB0000001)     // 超时
#define ROBOT_RETURN_ERRORSTATS                     (0xB0000002)     // 状态不正确
#define ROBOT_RETURN_CONNECTFAILED                  (0xB0000003)     // 连接失败
#define ROBOT_RETURN_NOTARRIVED                     (0xB0000004)     // 机器人没有运动到指定坐标
#define ROBOT_RETURN_ERRORGROUPNAME                 (0xB0000005)     // 配置参数组名称不存在
#define ROBOT_RETURN_ERRORROBOTNAME                 (0xB0000006)     // 配置参数机器人项名称不存在
#define ROBOT_RETURN_ERROR_PARAMETER                (0xB0000007)     //  机器人运动参数错误
#define UIMODULE_RETURN_TIMEOUT                     (0xB0000101)     // 超时
#define UIMODULE_RETURN_ERRORSTATS                  (0xB0000102)     // 状态不正确
#define UIMODULE_RETURN_ERRORPARAM                  (0xB0000103)     // 配置参数错误
#define UIMODULE_RETURN_INITERROR                   (0xB0000104)     // 未初始化
#define METERCOUNTINGWHEEL_RETURN_INITFAILED        (0xB0000201)     // 初始化失败
#define PROTOCOL_CMD_RETURN_FAILED                  (0xB0000301)     // 机器人通讯协议函数返回失败
#define PROTOCOL_CMD_ERROR_PARAMETER                (0xB0000302)     // 机器人通讯协议输入参数错误
#define PROTOCOL_CMD_ERROR_TIMEOUT                  (0xB0000303)     // 机器人通讯协议超时
#define PROTOCOL_SOCKETTINITIALERROR                (0xB0000304)     // 机器人通讯协议始始化失败
#define PROTOCOL_SOCKETSENDERROR                    (0xB0000305)     // 机器人通讯协议发送失败

#define ERROR_WAIT_TIME_OUT                         (0xB0001001)

class CUIError
{
public:
	CUIError();
	~CUIError();
    string getErrorCode(int iErrorCode);
	int PraseErrorCode(int iErrorCode);
private:
	string m_strError;
};
int hex2dec(const char *hex);

