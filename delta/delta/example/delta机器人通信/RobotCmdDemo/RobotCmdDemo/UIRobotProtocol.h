
#pragma once

/*	File: UIRobotSdkFunc.h
	Description:		
*/

#ifdef LINUX
	#define UIROBOTSDKFUNC_API
	#define SDKSTDCALLAPI
#elif WIN32
		#define UIROBOTSDKFUNC_API   extern "C"	__declspec(dllexport)
        #define SDKSTDCALLAPI
#endif


#include "UIRobotStruct.h"


//===========================
//	机器人RTCN数据块数据结构
//===========================
typedef struct _UI_RTCNCALLBACK_OBJ
{
	unsigned char cmd_type;
	char m_chCommandDataLen;
	unsigned int m_dwConnectCode;
	void *pSocketClient;
	char m_chCommandDataBuffer[UISDK_MAX_DATA_LEN];  // 命令数据缓存       
}UI_RTCNCALLBACK_OBJ, *P_UI_RTCNCALLBACK_OBJ;

unsigned int GetConnectDevType(unsigned int dwDevIndex);
// 连接索引号
unsigned int GetDevID(unsigned int dwDevIndex);

/*==============================================================================
	NAME: BOOL UIMRobotRegRtcnCallBack(PF_UIROBOTSDK_CALLBACK pCallBackFunc)
	----------------------------------------------------------------------------
	Function:回调函数注册函数
	----------------------------------------------------------------------------
	Parameter:
	PF_UIROBOTSDK_CALLBACK pCallBackFunc:回调函数指针
	----------------------------------------------------------------------------
	Return Value:
	If the function fails, the return value is false.
	If the function succeeds, the return value is true.
	----------------------------------------------------------------------------
	Note:
	用于网关回调函数的注册
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI UIMRobotRegRtcnCallBack(PF_UIROBOTSDK_CALLBACK pCallBackFunc);

/*==============================================================================
	NAME: int SDKSTDCALLAPI ConnectValue(unsigned int dwConnectType, int iIPValue)
	----------------------------------------------------------------------------
	Function:用连接数值来连接机器人
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectType:连接类型
		 int iIPValue:连接参数(网络连接时为IP地址值;蓝牙通讯时为串口号)
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回不为0的连接通讯编号
				   失败则返回0
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API  int SDKSTDCALLAPI ConnectValue(unsigned int dwConnectType, int iIPValue);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SDKTestRegCall(int a,int b,int (*func)(int iA,int iB));

/*==============================================================================
	NAME: int ConnectName(unsigned int dwConnectType, char *pConnectParam)
	----------------------------------------------------------------------------
	Function:用连接字符串连接机器人
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectType:连接类型
		 char *pConnectParam:连接参数(网络连接时为IP地址字符串;蓝牙通讯时为串口号字符串)
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回不为0的连接通讯编号
				   失败则返回0
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI ConnectName(unsigned int dwConnectType, char *pConnectParam);
/*==============================================================================
	NAME: int SDKSTDCALLAPI DisConnect(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:断开机器人连接
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
 UIROBOTSDKFUNC_API int SDKSTDCALLAPI DisConnect(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  Login(unsigned int dwConnectCode, char *pPassword, int iPasswordLen)
	----------------------------------------------------------------------------
	Function:机器人登录
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pPassword:密码
		 int iPasswordLen:密码长度
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI Login(unsigned int dwConnectCode, char *pPassword, int iPasswordLen);
/*==============================================================================
	NAME: int  SetPassword(unsigned int dwConnectCode, P_UI_SETPASSWORD_OBJ setPasswordObj)
	----------------------------------------------------------------------------
	Function:机器人设置密码
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_SETPASSWORD_OBJ:设置密码结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetPassword(unsigned int dwConnectCode, P_UI_SETPASSWORD_OBJ setPasswordObj);


/*==============================================================================
	NAME: int  SetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ setNetworkObj)
	----------------------------------------------------------------------------
	Function:机器人设置网络配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_SETPASSWORD_OBJ:设置网络配置结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ setNetworkInObj);

/*==============================================================================
	NAME: int  GetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ getNetworkObj)
	----------------------------------------------------------------------------
	Function:机器人设置网络配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_SETPASSWORD_OBJ:设置网络配置结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ getNetworkObj);

/*==============================================================================
	NAME: int  SetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ setDateTimeInObj)
	----------------------------------------------------------------------------
	Function:机器人设置时间
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_DATETIME_OBJ setDateTimeInObj:设置时间结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ setDateTimeInObj);

/*==============================================================================
	NAME: int  GetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ getDateTimeObj)
	----------------------------------------------------------------------------
	Function:机器人获取时间
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_DATETIME_OBJ getDateTimeObj:获取时间结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ getDateTimeObj);

/*==============================================================================
	NAME: int  GetRobotVersion(unsigned int dwConnectCode, char *pVersion)
	----------------------------------------------------------------------------
	Function:获取机器人软件版本号
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pVersion:版本号字符串缓冲区
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotVersion(unsigned int dwConnectCode, char *pVersion);

/*==============================================================================
	NAME: int  GetRobotStatus(unsigned int dwConnectCode, int *pStatus)
	----------------------------------------------------------------------------
	Function:获取机器人工作状态
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	char *pStatus:机器人工作状态变量地址
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotStatus(unsigned int dwConnectCode, int *pStatus);

/*==============================================================================
	NAME: int  GetRunningPogram(unsigned int dwConnectCode, P_UI_RUNNINGPROGRAM_OBJ runningProgramObj)
	----------------------------------------------------------------------------
	Function:获取机器人工作状态
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	P_UI_RUNNINGPROGRAM_OBJ runningProgramObj:机器人当前程序信息
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRunningPogram(unsigned int dwConnectCode, P_UI_RUNNINGPROGRAM_OBJ runningProgramObj);

/*==============================================================================
	NAME: int  GetUserProgramList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ programListObj)
	----------------------------------------------------------------------------
	Function:获取用户程序列表
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	P_UI_COMMANDBUFFER_OBJ programListObj:机器人用户程序列表
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetUserProgramList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ programListObj);

/*==============================================================================
	NAME: int  DeleteUserProgram(unsigned int dwConnectCode, char *pProgramName);
	----------------------------------------------------------------------------
	Function:删除机器人指定用户程序
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pProgramName:机器人用户程序名称
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI DeleteUserProgram(unsigned int dwConnectCode, char *pProgramName);

/*==============================================================================
	NAME: int  GetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:获取机器人默认用户程序
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pProgramName:机器人用户程序名称
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName);

/*==============================================================================
	NAME: int  GetLogList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ logListObj)
	----------------------------------------------------------------------------
	Function:获取用户程序列表
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	P_UI_COMMANDBUFFER_OBJ logListObj:机器人用户程序列表
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetLogList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ logListObj);

/*==============================================================================
	NAME: int  SetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:设置机器人默认用户程序
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	char *pProgramName:机器人用户程序名称
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
	==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName);

/*==============================================================================
	NAME: int  GetMotorTemperature(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:获取机器人各个臂的温度
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_MOTORTEMPERATURE_OBJ motorTemperature:机器人机械臂温度数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetMotorTemperature(unsigned int dwConnectCode, P_UI_MOTORTEMPERATURE_OBJ motorTemperature);

/*==============================================================================
	NAME: int  RunUserProgram(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:运行机器人用户程序
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pProgramName:机器人用户程序名称
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI RunUserProgram(unsigned int dwConnectCode, char *pProgramName);

/*==============================================================================
	NAME: int  SimulatorRunUserProgram(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:模拟运行机器人用户程序
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pProgramName:机器人用户程序名称
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SimulatorRunUserProgram(unsigned int dwConnectCode, char *pProgramName);

/*==============================================================================
	NAME: int  EmergyStop(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:机器人紧急停止
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI EmergyStop(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  ResetRobot(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:复位机器人
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI ResetRobot(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  GetRobotTCP(unsigned int dwConnectCode, double *pTcpLength, double *pTcpHeight, double *pTcpAgnle)
	----------------------------------------------------------------------------
	Function:获取机器人夹具中心点
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 double *pTcpLength:机器人夹具中心点长度
		 double *pTcpHeight:机器人夹具中心点高度
		 double *pTcpAgnle:TCP 工具臂(当 pos.r=0 度时)与基座坐标X轴之间的夹角
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotTCP(unsigned int dwConnectCode, double *pTcpLength, double *pTcpHeight, double *pTcpAgnle);

/*==============================================================================
	NAME: int  SetRobotTCP(unsigned int dwConnectCode,double TcpLength, double TcpHeight, double TcpAngle)
	----------------------------------------------------------------------------
	Function:设置机器人夹具中心点
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 double TcpLength:机器人夹具中心点长度
		 double TcpHeight:机器人夹具中心点高度
		 double TcpAngle:TCP 工具臂(当 pos.r=0 度时)与基座坐标X轴之间的夹角
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetRobotTCP(unsigned int dwConnectCode,double TcpLength, double TcpHeight, double TcpAngle);

/*==============================================================================
	NAME: int  SetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
	----------------------------------------------------------------------------
	Function:设置机器人初始工具位置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_INITIALIZETCP_OBJ initializeTCP:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP);

/*==============================================================================
	NAME: int  GetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
	----------------------------------------------------------------------------
	Function:获取机器人初始工具位置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_INITIALIZETCP_OBJ initializeTCP:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP);

/*==============================================================================
	NAME: int  SetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
	----------------------------------------------------------------------------
	Function:设置机器人工具坐标
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_INITIALIZETCP_OBJ initializeTCP:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP);

/*==============================================================================
	NAME: int  GetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
	----------------------------------------------------------------------------
	Function:获取机器人工具坐标
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_INITIALIZETCP_OBJ initializeTCP:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP);


UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRtcnBack(unsigned int dwConnectCode);
/*==============================================================================
	NAME: int  LooseAllJoint(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:机器人松开所有关节
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API  int SDKSTDCALLAPI LooseAllJoint(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  LockAllJoint(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:机器人锁住所有关节
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI LockAllJoint(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  GetJointAngle(unsigned int dwConnectCode, P_UI_JOINTANAGLE_OBJ jointAngle)
	----------------------------------------------------------------------------
	Function:获取机器人各关节角度
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_JOINTANAGLE_OBJ jointAngle:机器人各关节角度数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetJointAngle(unsigned int dwConnectCode, P_UI_JOINTANAGLE_OBJ jointAngle);

/*=======================================iDirectoryIndex====================================
	NAME:int  MoveArmJoint(unsigned int dwConnectCode, int iArmIndex)
	----------------------------------------------------------------------------
	Function:持续移动机器人关节
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	     int iDirectoryIndex:持续移动机器人方向 索引值 : 
	                              1：向Y轴正方向持续移动； 
                                     2：向Y轴负方向持续移动；
                                     3：向X轴正方向持续移动；
                                     4：向X轴负方向持续移动；
		 int iSpeed			:速度
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI MoveArmJoint(unsigned int dwConnectCode, int iDirectoryIndex, int iSpeed);

/*==============================================================================
	NAME: int  GetInPortDigitalValue(unsigned int dwConnectCode,P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj)
	----------------------------------------------------------------------------
	Function:设置某输出IO端口数字输出值
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj:输入端口电平值
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetInPortDigitalValue(unsigned int dwConnectCode, P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj);

/*==============================================================================
	NAME: int  SetOutPortDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel)
	----------------------------------------------------------------------------
	Function:设置某输出IO端口数字输出值
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int iPortIndex:输出端口索引号:0~15
		 bool bHighLevel:输出电平高或低:TRUE表示高电平；FALSE表示低电平
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetOutPortDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetRobotOutPortValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotOutPortValue(unsigned int dwConnectCode, int iPortIndex, bool *pHighLevel);

/*==============================================================================
	NAME: int  GetOutPortDigitalValue(unsigned int dwConnectCode,P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj)
	----------------------------------------------------------------------------
	Function:获取输出IO端口数字输出值
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj:输出端口电平值
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetOutPortDigitalValue(unsigned int dwConnectCode,P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj);


/*==============================================================================
	NAME: int  GetAnalogValue(unsigned int dwConnectCode, int iPortIndex, int * pAnalogValue)
	----------------------------------------------------------------------------
	Function:获取某输入IO端口的输入模拟量
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int iPortIndex:输出端口索引号:1~ROBOTINPUTPORTNUM
		 int * pAnalogValue:某输入IO端口的输入模拟量
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotAnalogValue(unsigned int dwConnectCode, int iNodeId, int * pAnalogValue);

/*==============================================================================
	NAME: int  Shutdown(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:关闭机器人
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI Shutdown(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  DownloadProgramFileName(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:应用层向控制层传输文件名字
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI DownloadProgramFileName(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj);

/*==============================================================================
	NAME: int  DownloadProgramFileContent(unsigned int dwConnectCode, P_UI_ROBOTFILE_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:应用层向控制层传输文件内容
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI DownloadProgramFileContent(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj);

/*==============================================================================
	NAME: int  UpProgramFile(unsigned int dwConnectCode, P_UI_ROBOTFILE_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:控制层向应用层传输文件
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI UpProgramFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI UpSimulateFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj);
/*==============================================================================
	NAME: int  EnterCANMode(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:进入透传模式
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI EnterCANMode(unsigned int dwConnectCode);

/*==============================================================================
	NAME: int  SendCANData(unsigned int dwConnectCode, P_UI_ROBOTSOCKETMSG_OBJ uiCANMsgObj)
	----------------------------------------------------------------------------
	Function:发送CAN数据报文
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_ROBOTSOCKETMSG_OBJ uiCANMsgObj: CAN数据报文
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SendCANData(unsigned int dwConnectCode, P_UI_ROBOTSOCKETMSG_OBJ uiCANMsgObj,P_UI_ROBOTSOCKETMSG_OBJ uiCANMsgObjOut = NULL);

/*==============================================================================
	NAME: int  ExitCANMode(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:退出透传模式
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI ExitCANMode(unsigned int dwConnectCode);


/*==============================================================================
	NAME: int releaseNodePVT(NodePVT pvt)
	----------------------------------------------------------------------------
	Function: 释放pvt列表;
	----------------------------------------------------------------------------
	Parameter:
		NodePVT pvt:pvt列表(空间需要释放);
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
 UIROBOTSDKFUNC_API int SDKSTDCALLAPI releaseSDKNodePVT(unsigned int dwConnectCode,NodePVT pvt);

 /*
 *	功能：根据示教得到的TCP获得工具坐标
 *  参数：
 *		double[IN][IN]:     示教的四个坐标点的信息
 *	返回值：
 *		0：	结果正常；
 *		其他：  结果异常；
 */
 UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetTCPViaTeaching(unsigned int dwConnectCode,double y[4][4], TCP *tcp, double *tcpL);




UIROBOTSDKFUNC_API int SDKSTDCALLAPI getRobotArmLimit(unsigned int dwConnectCode,int iArmIndex,int iType, double *pdLimitValue);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI StartUpdateFirmware(unsigned int dwConnectCode);


/*==============================================================================
	NAME: int setRototType(int iType)
	----------------------------------------------------------------------------
	Function: 设置机器人类型
	----------------------------------------------------------------------------
	Parameter:
	----------------------------------------------------------------------------
	Return Value:
		成功则返回0
		失败则返回失败原因
	----------------------------------------------------------------------------
Note:
       脱机模式下使用
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI setRototType(unsigned int dwConnectCode, int iType);

/*==============================================================================
	NAME: int getRototType(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function: 获取机器人类型
	----------------------------------------------------------------------------
	Parameter:
	         unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:
		机器人类型
	----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI getRototType(unsigned int dwConnectCode, int *piRobotType);
/*==============================================================================
	NAME: int  GetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode)
	----------------------------------------------------------------------------
	Function:获取机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_CAMERAMODE_OBJ cameraModel:机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode);
/*==============================================================================
	NAME: int  SetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode)
	----------------------------------------------------------------------------
	Function:设置机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_CAMERAMODE_OBJ cameraMode机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/

UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode);
/*==============================================================================
	NAME: int  EnterCameraMode(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:进入视觉系统模式
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI EnterCameraMode(unsigned int dwConnectCode);
/*==============================================================================
	NAME: int  ExitCameraMode(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:退出视觉系统模式
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI ExitCameraMode(unsigned int dwConnectCode);


/*===========================================================================
	NAME: int  SetMACAddress(unsigned int dwConnectCode, char *pMACAddress)
	----------------------------------------------------------------------------
	Function:设置机器人MAC地址
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pMACAddress:MAC地址字符串缓冲区
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetMACAddress(unsigned int dwConnectCode, char *pMACAddress);
/*==============================================================================
	NAME: int  GetMACAddress(unsigned int dwConnectCode, char *pMACAddress)
	----------------------------------------------------------------------------
	Function:获取机器人MAC地址
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pMACAddress:MAC地址字符串缓冲区
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetMACAddress(unsigned int dwConnectCode, char *pMACAddress);
/*===========================================================================
	NAME: int  SetMACAddress(unsigned int dwConnectCode, char *pSerialNumber)
	----------------------------------------------------------------------------
	Function:设置机器人序列号
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pSerialNumber:序列号字符串缓冲区
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetSerialNumber(unsigned int dwConnectCode, char *pSerialNumber);
/*==============================================================================
	NAME: int  GetSerialNumber(unsigned int dwConnectCode, char *pSerialNumber)
	----------------------------------------------------------------------------
	Function:获取机器人序列号
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pSerialNumber:序列号字符串缓冲区
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetSerialNumber(unsigned int dwConnectCode, char *pSerialNumber);

/*==============================================================================
NAME: int SetToolMassPara(unsigned int dwConnectCode, P_UI_TOOLMASS_OBJ setToolMassObj)
----------------------------------------------------------------------------
Function:机器人设置工具负载参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_DATETIME_OBJ setToolMassObj:设置负载参数结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetToolMassPara(unsigned int dwConnectCode, P_UI_TOOLMASS_OBJ toolMassObj);

/*==============================================================================
NAME: int GetToolMassPara(unsigned int dwConnectCode, P_UI_TOOLMASS_OBJ toolMassObj)
----------------------------------------------------------------------------
Function:机器人设置工具负载参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_DATETIME_OBJ setToolMassObj:设置负载参数结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetToolMassPara(unsigned int dwConnectCode, int InPutbuffer, P_UI_TOOLMASS_OBJ toolMassObj);


/*==============================================================================
NAME: int SetUserCoor(unsigned int dwConnectCode, P_UI_TOOLMASS_OBJ userCoolObj)
----------------------------------------------------------------------------
Function:机器人设置用户坐标系
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_DATETIME_OBJ setToolMassObj:设置用户坐标结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetUserCoor(unsigned int dwConnectCode, P_UI_USERCOOR_OBJ userCoolObj);

/*==============================================================================
NAME: int GetUserCoor(unsigned int dwConnectCode, char *InPutbuffer, P_UI_USERCOOR_OBJ userCoolObj)
----------------------------------------------------------------------------
Function:机器人获取用户坐标系
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_USERCOOR_OBJ userCoolObj:获取用户坐标系参数
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetUserCoor(unsigned int dwConnectCode, int InPutbuffer, P_UI_USERCOOR_OBJ userCoolObj);

/*==============================================================================
NAME: int SetSafeArea(unsigned int dwConnectCode, P_UI_SAFEAREA_OBJ safeAreaObj)
----------------------------------------------------------------------------
Function:机器人设置安全区域参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_DATETIME_OBJ setToolMassObj:设置安全区域结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetSafeArea(unsigned int dwConnectCode, P_UI_SAFEAREA_OBJ safeAreaObj);

/*==============================================================================
NAME: int GetSafeArea(unsigned int dwConnectCode, char *InPutbuffer, P_UI_SAFEAREA_OBJ safeAreaObj)
----------------------------------------------------------------------------
Function:机器人设置安全区域参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_DATETIME_OBJ setToolMassObj:设置安全区域结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetSafeArea(unsigned int dwConnectCode, int InPutbuffer, P_UI_SAFEAREA_OBJ safeAreaObj);

/*==============================================================================
NAME: int SetFixedPoint(unsigned int dwConnectCode, P_UI_FIXEDPOINT_OBJ fixedPointObj)
----------------------------------------------------------------------------
Function:机器人设置固定点位参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_DATETIME_OBJ setToolMassObj:设置固定点位结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetFixedPoint(unsigned int dwConnectCode, P_UI_FIXEDPOINT_OBJ fixedPointObj);

/*==============================================================================
NAME: int GetFixedPoint(unsigned int dwConnectCode, char *InPutbuffer, P_UI_FIXEDPOINT_OBJ fixedPointObj)
----------------------------------------------------------------------------
Function:机器人设置安全区域参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_FIXEDPOINT_OBJ fixedPointObj:设置安全区域结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetFixedPoint(unsigned int dwConnectCode, int InPutbuffer, P_UI_FIXEDPOINT_OBJ fixedPointObj);
/*==============================================================================
NAME: int  GetUserVariable(unsigned int dwConnectCode, float *pfVariableValue)
----------------------------------------------------------------------------
Function:获取机器人用户变量值
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
float *pfVariableValue:用户变量值列表：最多200个用户变量
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetUserVariable(unsigned int dwConnectCode, float *pfVariableValue);
/*==============================================================================
NAME: int  GetRobotConfig(unsigned int dwConnectCode, P_UI_ROBOTCONFIG_OBJ pRobotConfig)
----------------------------------------------------------------------------
Function:获取机器人配置信息
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_ROBOTCONFIG_OBJ pRobotConfig:机器人配置信息
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotConfig(unsigned int dwConnectCode, P_UI_ROBOTCONFIG_OBJ pRobotConfig);
/*==============================================================================
NAME: int SetRobotConfig(unsigned int dwConnectCode, P_UI_ROBOTCONFIG_OBJ pRobotConfig)
----------------------------------------------------------------------------
Function:设置机器人配置信息
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_ROBOTCONFIG_OBJ pRobotConfig:机器人配置信息
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetRobotConfig(unsigned int dwConnectCode, P_UI_ROBOTCONFIG_OBJ pRobotConfig);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI Get3dPosLimit(unsigned int dwConnectCode,int type,double x, double y, double z, double PosRz, int nType, double *targetx, double *targety, double *targetz, double *targetRz);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI MoveJointToTargetPos(unsigned int dwConnectCode, int iSpeed, int iType, double x, double y, double z, double rz, double theta_1, double theta_2, double theta_3, double Toolz);


UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetCachePara(unsigned int dwConnectCode, P_UI_CACHE_OBJ cacheObj);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetCachePara(unsigned int dwConnectCode, int InPutbuffer, P_UI_CACHE_OBJ cacheObj);

/*==============================================================================
NAME: int SetRobotMove(unsigned int dwConnectCode, NodePoint *pNodePoint, int iTotalNodeCount)
----------------------------------------------------------------------------
Function:机器人按节点参数设置进行运动
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
NodePoint *pNodePoint:运动节点信息指针
int iTotalNodeCount:所有节点个数
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetRobotMove(unsigned int dwConnectCode, P_XMLNodePoint pNodePoint, int iTotalNodeCount);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotSMove(unsigned int dwConnectCode, P_XMLNodePoint pNodePoint, int iTotalNodeCount);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotMoveTM(unsigned int dwConnectCode, P_XMLNodePoint pNodePoint, int iTotalNodeCount, RobotAsxMax *pRobotMax);
/*==============================================================================
NAME: int CheckPointValid(unsigned int dwConnectCode, POS_XYZR pos)
----------------------------------------------------------------------------
Function:测试机器人空间坐标是否有效
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
POS_XYZR pos:机器人空间坐标
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI CheckPointValid(unsigned int dwConnectCode, POS_XYZR pos);

/*===========================================================================
NAME: int  InstallMacAddress(unsigned int dwConnectCode)
----------------------------------------------------------------------------
Function:设置机器人序列号
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add 2017.06.05
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI InstallSHFile(unsigned int dwConnectCode);
 /*==============================================================================
	NAME: int  SetTargetTCPEx(unsigned int dwConnectCode, P_UI_INITIALIZETCPEX_OBJ initializeTCPEX)
	----------------------------------------------------------------------------
	Function:设置机器人工具坐标，让机器人PTP运动到指定坐标，各个轴速度可以设置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_INITIALIZETCPEX_OBJ initializeTCPEX:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/

UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetTargetTCPEx(unsigned int dwConnectCode, P_UI_INITIALIZETCPEX_OBJ initializeTCPEX);


UIROBOTSDKFUNC_API int SDKSTDCALLAPI  SDKTestRegCall(int a, int b, int(*func)(int iA, int iB));

/*===========================================================================
NAME: int  WriteSerialMessage(unsigned int dwConnectCode, char *pSerialString)
----------------------------------------------------------------------------
Function:SDK透传串口写入数据
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
char *pSerialString:透传报文内容
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add in 2017.05.31
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI WriteSerialMessage(unsigned int dwConnectCode, char *pSerialString, int iStrLen);

/*===========================================================================
NAME: int  CloseSerial(unsigned int dwConnectCode)
----------------------------------------------------------------------------
Function:打开串口
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add in 2017.05.31
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI CloseSerial(unsigned int dwConnectCode, int InPutbuffer);
/*===========================================================================
NAME: int  ClearBatteryError(unsigned int dwConnectCode)
----------------------------------------------------------------------------
Function:清除电池报警错误
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add in 2019.01.03
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI ClearBatteryError(unsigned int dwConnectCode);

/*===========================================================================
NAME: int  OpenSerial(unsigned int dwConnectCode)
----------------------------------------------------------------------------
Function:打开串口
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add in 2017.05.31
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI OpenSerial(unsigned int dwConnectCode, char *pSerialString, int iStrLen);
/*===========================================================================
NAME: int  GetRobotUpdateStatus(unsigned int dwConnectCode)
----------------------------------------------------------------------------
Function:获取机器人固件更新状态
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add in 2019.04.19
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotUpdateStatus(unsigned int dwConnectCode, int * piCurrentIndex, int * piTotalCount);

/*===========================================================================
NAME: int  ClearRobotUpdateStatus(unsigned int dwConnectCode)
----------------------------------------------------------------------------
Function:清除机器人固件更新状态
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:add in 2019.04.19
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI ClearRobotUpdateStatus(unsigned int dwConnectCode);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI UpdateRobotFirmware(unsigned int dwConnectCode, char *pProgramName);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SDKUIRobotUpdateFirmware(char *pchFileName);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI setRobotServoFlag(unsigned int dwConnectCode, int servoFlag);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI NormalRobotStop(unsigned int dwConnectCode);
UIROBOTSDKFUNC_API int getRobotTrackingtask(unsigned int dwConnectCode,double posStartPoint[4],float beltA,float beltV, float trjH[4],float	trjS[4],float trjV[4], POS_XYZR *trjB,POS_XYZR *trjF,POS_XYZR *trjE,TRJ_TIME  *trjT);

UIROBOTSDKFUNC_API int SDKSTDCALLAPI RunRobotAutoIOTest(unsigned int dwConnectCode);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI setRobotUnlocked(unsigned int dwConnectCode);

/*==============================================================================
NAME: int  SetUserProgramPauseStatus(unsigned int dwConnectCode, char *pStatus)
----------------------------------------------------------------------------
Function:设置用户程序暂停/恢复用户程序运行
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
char *pStatus:用户程序运行状态："1"表示暂停；"0"表示恢复运行
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI SetUserProgramStepRun(unsigned int dwConnectCode, char *pStatus);

UIROBOTSDKFUNC_API int SDKSTDCALLAPI setPRGTraceFlag(unsigned int dwConnectCode, char *pTraceFlag);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI getPRGTraceFlag(unsigned int dwConnectCode, char * pTraceFlag);
/*==============================================================================
	NAME: int setPRGBreakpoint(unsigned int dwConnectCode, int iNodeId)
	----------------------------------------------------------------------------
	Function:获取机器人脚本调试模式标志
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int iNodeId:断点ID号
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
UIROBOTSDKFUNC_API int SDKSTDCALLAPI setPRGBreakpoint(unsigned int dwConnectCode, int iNodeId);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI GetRobotCameraConnectStatus(unsigned int dwConnectCode, int *pStatus);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI setRobotTorqueLimit(unsigned int dwConnectCode, int iTorqueLimit);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI getRobotTorqueLimit(unsigned int dwConnectCode, int * piTorqueLimit);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI setRobotGoHome(unsigned int dwConnectCode);
UIROBOTSDKFUNC_API int SDKSTDCALLAPI getMeterWheelPosition(unsigned int dwConnectCode, int * piPosition);


