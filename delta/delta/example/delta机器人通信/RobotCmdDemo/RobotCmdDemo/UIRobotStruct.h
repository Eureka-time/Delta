#ifndef UIROBOTSTRUCT_H
#define UIROBOTSTRUCT_H

/*	File: UIRobotSDK_RT.h
	Description:		
*/
/*

宏定义UILABVIEW_SDK_FUNC为了适应LABVIEW调用时生成动态库，需要定义该编译开关
#ifdef UIROBOTSDKFUNC_EXPORTS
#define UIROBOTSDKFUNC_API_EXPORT  __declspec(dllexport)
#define UIROBOTSDKFUNC_API      extern "C" UIROBOTSDKFUNC_API_EXPORT
#define SDKSTDCALLAPI
#else
#ifdef LINUX
#ifdef SDK_C_FUNCTION_CALL
        #define UIROBOTSDKFUNC_API	    extern "C"
        #define SDKSTDCALLAPI
#else
        #define UIROBOTSDKFUNC_API
        #define SDKSTDCALLAPI
#endif
#elif WIN32
        #define UIROBOTSDKFUNC_API   extern "C"	__declspec(dllimport)
        #define SDKSTDCALLAPI
#else
   #define UIROBOTSDKFUNC_API
        #define SDKSTDCALLAPI
#endif
#endif
*/


#define  TRACK_MODE 4
 // labview调用时需要定义
//#define UILABVIEW_SDK_FUNC 1

#define UISDK_MAXPASSWORDLEN    (128)
#define UISDK_MAX_DATA_LEN      (1024*2)
#define UISDK_MAX_FILECONTENT_LEN      1024   //MMC 从200->1024
#define UISDK_MAX_SDKBUFFER_LEN       128    
#define UISDK_MAX_SDKBUFFER_DOUBLE_LEN       (UISDK_MAX_SDKBUFFER_LEN*2) 

#define  ROBOTDEFAULTMAXPOINTCOUNT  (50)   // 路点最多个数
#define NODEPOINT_NAME_MAX          (30)
#define FLOATDECIMALS_LENGTH			(2)
#define UISDK_MAX_USERFILENAME_LENGTH  80
#define ROBOTINPUTPORTNUM  (16)
#define ROBOTOUTPUTPORTNUM  (24)

#define MAX_GATEWAY_COUNT (5)
#define MAX_DEVICENODE_NUMBER (120)


// 机器人类型定义
#define UIROBOTTYPE_MAXTYPE     (7) //  

#define UIROBOTBASETYPE_SCARA     1
#define UIROBOTBASETYPE_DELTA     2


#define UIM_HIGHWORD_VALUE(var)    ((var >>16)&0xFF)
// 机器人连接类型
#define UICONNECT_IP    0x01
#define UICONNECT_BT    0x02
// 连接和通讯成功
#define UISDK_SUCCESS   0

// UI 接口命令
#define UICMDLOGIN                                 (0x01)  // 
#define UICMDSOCKETDISCONNECTED                    (0x02)  // 
// #define UICMDBLUETOOTHFLAG                    (0x02)                                                                        
#define UICMDSETPASWORD                            (0x03)  // 
#define UICMDGETNETCONFIG                          (0x04)  // 
#define UICMDSETNETCONFIG                          (0x05)  // 
#define UICMDSETTIME                               (0x06)  // 
#define UICMDGETTIME                               (0x07)  // 
#define UICMDGETVERSION                            (0x08)  // 
#define UICMDGETSTATUS                             (0x09)  // 
#define UICMDGETRUNPROGRAM                         (0x0A)  // 
#define UICMDGETPROGRAMLIST                        (0x0B)  // 
#define UICMDDELETEPROGRAM                         (0x0C)  // 
#define UICMDGETDEFAULTPROGRAM                     (0x0D)  // 
#define UICMDSETDEFAULTPROGRAM                     (0x0E)  // 
#define UICMDGETMOTORTEMPERATURE                   (0x0F)  // 
#define UICMDSETMACADDRESS                         (0x10)  // 
#define UICMDGETMACADDRESS                         (0x11)  // 
#define UICMDSETSERIALNUMBER                       (0x12)  // 
#define UICMDGETSERIALNUMBER                       (0x13)  // 
#define UICMDSETSAFEAREA                           (0x14)  // 设置安全区域                                                                                                     
#define UICMDGETSAFEAREA                           (0x15)  // 获取安全区域                                                                                                     
#define UICMDSETMASSPARA                           (0x16)  // 设置工具负载参数                                                                                               
#define UICMDGETMASSPARA                           (0x17)  // 获取工具负载参数                                                                                               
#define UICMDSETUSERCOOR                           (0x18)  // 设置用户坐标系                                                                                                  
#define UICMDGETUSERCOOR                           (0x19)  // 获取工具坐标系                                                                                                  
#define UICMDSETFIXDPOINT                          (0x1A)  // 设置固定点参数                                                                                                  
#define UICMDGETFIXDPOINT                          (0x1B)  // 获取固定点参数                                                                                                  
#define UICMDGETLOGLIST                            (0x1C)  // 获取日志文件文件列表                                                                                         
#define UICMDSETCACHE                              (0x1D)  // 设置缓存点参数                                                                                                  
#define UICMDGETCACHE                              (0x1E)  // 获取缓存点参数  
#define UI_CMD_GETROBOTMODEL                       (0x1F)  // 获取机器人参数结构体
#define UI_CMD_SETPRGTRACEFLAG                     (0x20)  // 
#define UI_CMD_GETPRGTRACEFLAG                     (0x21)  // 
#define UI_CMD_OPENSERIALPORT                      (0x22)  // 打开串口                                                                                                           
#define UI_CMD_WRITESERIALPORT                     (0x23)  // 写串口数据                                                                                                        
#define UI_CMD_SERIALPORTRTCN                      (0x24)  // 串口数据反馈                                                                                                     
#define UI_CMD_CLOSESERIALPORT                     (0x25)  // 关闭串口                                                                                                           
#define UI_CMD_CLEARBATTERYERROR                   (0x26)  // 清除电池报警                                                                                                     
#define UI_CMD_GETPRGBREAKPOINT                    (0x27)  // 设置断点  
#define UI_CMD_GETCAMERACONNECTSTATUS              (0x28)   // 获取相机连接状态  
#define UI_CMD_GETROBOTMODELNAME                   (0x29)   // 获取机器人配置文件名
#define UI_CMD_STARTUPDATEFIRMWARE                 (0x2A)   // 启动在线升级程序
#define UI_CMD_GETTRANSLATEPOSITION                (0x2B)   //切换工具对应的坐标值信息     
#define UI_CMD_SETROBOTCONFIG                      (0x2C)  // 设置配置信息                                                                                                     
#define UI_CMD_GETROBOTCONFIG                      (0x2D)  // 获取配置信息                                                                                                     
#define UI_CMD_GETUSERVARIABLE                     (0x2E)  // 获取用户变量值                                                                                                  
#define UI_CMD_PAUSEPRO                            (0x2F)  //  暂停                                                                                                                 
#define UICMDRUNPROGRAM                            (0x30)  //  运行程序                                                                                                           
#define UICMDSIMULATORRUNPROGRAM                   (0x31)  //  模拟运行程序                                                                                                     
#define UICMDEMERGYSTOP                            (0x32)  //  急停                                                                                                                 
#define UICMDRESETROBOT                            (0x33)  //  复位机器人                                                                                                        
#define UICMDGETTOOLCENTRECOORDINATE               (0x34)  //  获取夹具中心点                                                                                                  
#define UICMDSETTOOLCENTRECOORDINATE               (0x35)  //  设置夹具中心点                                                                                                  
#define UICMDSETTOOLINITIALIZE                     (0x36)  //  设置初始工具位置                                                                                               
#define UICMDGETTOOLINITIALIZE                     (0x37)  //  获取初始工具位置                                                                                               
#define UICMDGETTOOLCOORDINATE                     (0x38)  //  获取工具坐标                                                                                                     
#define UICMDSETTOOLCOORDINATE                     (0x39)  //  设置工具坐标                                                                                                     
#define UICMDOFFMOTOR                              (0x3A)  //  松开所有关节                                                                                                     
#define UICMDSETONMOTOR                            (0x3B)  //  锁住所有关节                                                                                                     
#define UICMDGETMOTORANGEL                         (0x3C)  //  获取各关键诶角度                                                                                               
#define UICMDMOVEMOTOR                             (0x3D)  //  持续移动机器人关节                                                                                            
#define UI_CMD_SETANGLE_ROTATEMOVE                 (0x3E)  //  角度持续运动                                                                                                     
#define UI_CMD_SETPOINTEX                          (0x3F)  // 按机械臂速度PTP运动                                                                                            
#define UICMDGETIOCONFIGURE                        (0x40)  // 
#define UICMDSETIOCONFIGURE                        (0x41)  // 
#define UICMDGETIOINPUTVALUE                       (0x42)  // 
#define UICMDSETIOOUPUTVALUE                       (0x43)  // 
#define UICMDGETIOANALOGY                          (0x44)  // 
#define UICMDSETIOPWMBASE                          (0x45)  // 
#define UICMDGETIOPWMBASE                          (0x46)  // 
#define UICMDSETIOPWMDUTY                          (0x47)  // 
#define UICMDGETIOPWMDUTY                          (0x48)  // 
#define UICMDGETIOOUTPUTVALUE                      (0x49)  // 
#define UI_CMD_IO_GET_VIRTUALINPUT_TTL             (0x4A)  // 
#define UI_CMD_IO_SET_VIRTUALINPUT_TTL             (0x4B)  // 
#define UI_CMD_IO_GET_VIRTUALOUTPUT_TTL            (0x4C)  // 
#define UI_CMD_IO_SET_VIRTUALOUTPUT_TTL            (0x4D)  // 
#define UI_CMD_SET_ROBOTMOVE                       (0x4E)  // 按算法运动  
#define UI_CMD_GET_ROBOTMOVESD                     (0x4F)//按算法获取模拟数据                   
#define UICMDDOWNLOADFILENAME                      (0x50)  // APP -> CONTROL  DOWNLOAD 应用层向控制层发送文件名传输协议                                                                 
#define UICMDDOWNLOADFILECONTENT                   (0x52)  // APP->CONTROL  DOWNLOAD 应用层向控制层发送文件内容传输协议                                                                 
#define UICMDREQUIREFILENAME                       (0x54)  // APP->CONTROL UPLOAD 应用层向控制层发送请求文件请求                                                                     
#define UICMDTRANSMITFILECONTENT                   (0x56)  // CONTROL ->APP  UPLOAD 控制层发送文件内容                                                                                   
#define UICMDRUNPROINDEX                           (0x57)  //  运行程序结点ID                                                                                                   
#define UI_CMD_RUNPRO_USERVARIABLE                 (0x58)            // 
#define UI_CMD_IO_SET_OUTPUT_TTL_EX                (0x59)  // 
#define UI_CMD_IO_GET_OUTPUT_TTL_EX                (0x5A)  // 
#define UICMDINSTALL                               (0x5B)  //  安装指令                                                                                                           
#define UI_CMD_ROBOT_UPDATE_STATUS                 (0x5C)  // 固件更新状态                                                                                                     
#define UI_CMD_ROBOT_UPDATE_FIRMWARE               (0x5D)  // 固件更新                                                                                                           
#define UI_CMD_ROBOT_UNLOCK                        (0x5E)  // 机器人解锁 
#define UI_CMD_GET_ROBOTMOVETM                     (0x5F)// 获取运动算法速度以及加速度数据
#define UICMDSHUTDOWN                              (0x60)  // 
#define UICMDBLUETOOTHSETPASSWORD                  (0x61)  // 蓝牙设置机器人密码                                                                                            
#define UICMDGETROBOTTYPE                          (0x62)  // 获取机器人型号指令                                                                                            
#define UI_CMD_NORMAL_STOP_ROBOT                   (0x63)  // 普通机器人停止命令                                                                                            
#define UI_CMD_AUTO_IO_TEST                        (0x64)  // 自动进行io测试 
#define UICMDREQUIRESFILENAME                      (0x65)  // APP->CONTROL UPLOAD 应用层向控制层发送请求模拟文件请求  
#define UICMDTRANSMITSFILECONTENT                  (0x66)  // CONTROL ->APP  UPLOAD 控制层发送文件内容    
#define UICMDCHECKPOINT                            (0x67)  // 测试坐标是否有效
#define UICMDGETNOTTCPPOINT                        (0x68)  // 获取TCP0对应的坐标
#define UI_CMD_GET_TORQUELIMIT                     (0x6A) 
#define UI_CMD_SET_TORQUELIMIT                     (0x6B)
#define UI_CMD_MOTORGOHOME                         (0x6C)
#define UI_CMD_SET_ROBOT_TCP                       (0x6D)  // 设置机器人TCP参数
#define UI_CMD_GET_ROBOT_TCP                       (0x6E)  // 获取机器人TCP参数
#define UI_CMD_GET_METERWHEEL                      (0x6F)  // 计机米轮数据
#define UICMDENTERCANMODE                          (0x71)  // 进入透传模式                                                                                                     
#define UICMDTRANSLATECANDATA                      (0x72)  // 下载CAN报文                                                                                                        
#define UICMDRECEIVECANDATA                        (0x73)  // 上传CAN报文                                                                                                        
#define UICMDEXITCANMODE                           (0x74)  // 退出透传模式                                                                                                     
#define UICMDSETCAMERACONFIGURE                    (0x75)  // 视觉系统配置                                                                                                     
#define UICMDGETCAMERACONFIGURE                    (0x76)  // 获取视觉系统配置                                                                                               
#define UICMDENTERCAMERAMODE                       (0x77)  // 进入视觉系统模式                                                                                               
#define UICMDEXITCAMERAMODE                        (0x78)  // 退出视觉系统模式                                                                                               
#define UI_CMD_ROBOT_GATEWAYTYPE                   (0x79)  // 设置网关类型                                                                                                     
#define UICMDGETPULSEFROMPOSITION                  (0x7A)  // getPulseFromPosition                                                                                                   
#define UICMDGETPOSITIONFROMPULSE                  (0x7B)  // getPositionFromPulse                                                                                                   
#define UICMDGETPOSITIONFROMANGLE                  (0x7C)  //getPositionFromAngle                                                                                                   
#define UICMDGETANGLEFROMPOSITION                  (0x7D)  // getAngleFromPosition                                                                                                   
#define UICMDREALTIMENOTIFICATION                  (0x90)  //实时反馈通知                                                                                                     
#define UICMDEXDOWNLOADFILENAME                    (0x80)  // APP -> CONTROL  DOWNLOAD应用层向控制层发送文件名传输协议                                                                 
#define UICMDEXDOWNLOADFILECONTENT                 (0x82)  // APP->CONTROL  DOWNLOAD 应用层向控制层发送文件内容传输协议                                                                 
#define UICMDEXREQUIREFILENAME                     (0x84)  // APP->CONTROL UPLOAD 应用层向控制层发送请求文件请求                                                                     
#define UICMDEXTRANSMITFILECONTENT                 (0x86)  // CONTROL ->APP  UPLOAD 控制层发送文件内容                                                                                   


#define ERROR_UNKNOWN_FAILED                        99   // 一般性未知错误

/***********************************错误类型********************/
#define UI_ERROR_TYPE_NULL                           0           //
#define UI_ERROR_TYPE_FORARM                         1           //机器人第一臂错误
#define UI_ERROR_TYPE_POSTARM                        2           //机器人第二臂错误
#define UI_ERROR_TYPE_HAND                           3           //机器人第三臂错误
#define UI_ERROR_TYPE_ROTATE                         4           //机器人第四臂错误
#define UI_ERROR_TYPE_SYSTEM                         5           //机器人软件错误
#define UI_ERROR_TYPE_RUNPROGRAM                     6           //机器人运行程序错误
#define EVENT_ROBOT_CHANGE_IDLE                      20           //机器人转为待机状态
#define EVENT_ROBOT_IDLE_MOVE2POINT                  21           //机器人运行到指定位置
#define EVENT_ROBOT_IDLE_LINEPVTMOVE                 22           //机器人长按运动
#define EVENT_ROBOT_IDLE_RESET                       23           //机器人复位
#define EVENT_ROBOT_MOTOR_DISABLE                    24           //机器人脱机
#define EVENT_ROBOT_MOTOR_ENABLE                     25           //机器人使能
#define EVENT_ROBOT_URGENT_STOP                      26           //机器人急停
#define EVENT_ROBOT_RECV_UI_FILE_FAIL                27           //机器接收程序文件
#define EVENT_ROBOT_SEND_UI_FILE_FAIL                28           //机器人发送程序文件






/*路点类型*/

#define  WAYPOINT_COORDINATE           1
#define  WAITTING_INPUT                2
#define  WAITTING_TIME                 3
#define  SET_OUTPUT                    4
#define  ROBOTMOVE                     5
#define  MOVECIRCLE                    6
#define  BEGIN_RECYCLE				   7    // WHILE_START
#define  END_RECYCLE				   8    // WHILE_END
#define  IFELSE_START				   9    // IF_START
#define  IF_START                      10
#define  START_ELSE                    11
#define  IF_END                        12
#define  OPERATION_NODE				   13   //赋值
#define  OJBECTWAYPOINT_COORDINATE     14 //  对象坐标
#define  VAR_WAYPOINT_COORDINATE        15 //  可变对象坐标
#define  SET_VIRTUALOUTPUT              16 //  设置虚拟IO输出
#define  WAITTING_VIRTUALINPUT      	17 //  等待虚拟IO输入
#define  WAYPOINT_ROOT                  18
#define  MOVEARCH                       19


#if 0
//////////////////////////////////////////////////////////////////

//						控制器(620,730)错 误 代 码 定 义

//////////////////////////////////////////////////////////////////
	#define RTCN_ERR_SYS_GWSD    9   // 全组警告	
	#define RTCN_ERR_SYS_LOCK	10	// 系统急停并上锁	
	#define RTCN_ERR_SYS_STOP	11	// 620 电机急停过程中
	#define RTCN_ERR_SYS_OVHT	12	// 620 系统温度过热
	#define RTCN_ERR_SYS_POVL	13	// 620 对外供电系统检测到短路/过载	
  	#define RTCN_ERR_SYS_SCME	14	// 720 与 SERVO 通讯故障
  	#define RTCN_ERR_SYS_SRST	15	// 720 与 SERVO 需要断电重启
  	#define RTCN_ERR_SYS_SALM	16	// 720 检出 SERVO 报错
  	#define RTCN_ERR_SYS_SIOE    17  // 720 与 SERVO IO故障  	
  	#define RTCN_ERR_SYS_SLAV	18	// 附属芯片发生通讯故障  	
	#define RTCN_ERR_CTX_TMOT 	20	// CAN 报文发送超时（无人响应）
	#define RTCN_ERR_MXN_EBRK	22	// 刹车上锁, 无法BG
	#define RTCN_ERR_MXN_NBRK	23	// 刹车未锁, 无法脱机
	#define RTCN_ERR_MXN_MOFF	24	// 脱机状态, 无法 BG
	#define RTCN_ERR_MXN_SPHL	25	// 速度超限检出
	#define RTCN_ERR_MXN_PALL	26	// 反向工作位置超限检出
	#define RTCN_ERR_MXN_PAHL	27	// 正向工作位置超限检出
	#define RTCN_ERR_MXN_SPEL	28	// 速度误差超限检出（仅闭环控制）
	#define RTCN_ERR_MXN_PQER	29	// 位移误差超限检出（仅闭环控制）	
	#define RTCN_ERR_MXN_ENCS	30	// 编码器通讯错误
	#define RTCN_ERR_MXN_ENCB	31	// ABS 电池无电压
	#define RTCN_ERR_INS_SYNT	50	// 指令语法错误 620
	#define RTCN_ERR_INS_NUMB	51	// 指令数据错误 620
	#define RTCN_ERR_INS_IDXR	52	// 指令索引（下标）错误	
	#define RTCN_ERR_SYS_STTM	53	// 电机转动中，无法更改系统时间
	#define RTCN_ERR_MXN_DCSD	60  // SD减速度 小于DC减速度
	#define RTCN_ERR_MXN_RUNG	61  // 电机转动中，无法更改
	#define RTCN_ERR_MXN_STOG	65	// 电机转动中，无法设置绝对值编码器原点	
	#define RTCN_ERR_PVT_RUNG	70	// 电机运动中，无法设置 PV 起始点 或 MP[0]
	#define RTCN_ERR_PVT_WPOV	71	// QP,QV,QT,QA 索引号大于MP[6](水位)
	#define RTCN_ERR_PVT_IOFN	72	// QA Mask 不符合 I/O 端口功能要求
	#define RTCN_ERR_PVB_OVFL    73  // PVT Buffer 溢出
	#define RTCN_ERR_IOC_ADIO	90	// I/O 端口功能配置错误
#endif	




#define RTCN_ERROR_COMMAND                          50
#define RTCN_ERROR_PARAMETER                        51
#define RTCN_ERROR_COMMANDINDEX                     52
#define RTCN_ERROR_MOVING                           53
#define RTCN_ERROR_SD                               60
#define RTCN_ERROR_ORIGNAL                          65
#define RTCN_ERROR_PVTSTART                         70
#define RTCN_ERROR_PVTINDEX                         71
#define RTCN_ERROR_PVTPORTFUNCTION                  72
#define RTCN_ERROR_PORTFUNCTION                     90



#define RTCN_DIO_P1L 1
#define RTCN_DIO_P1H 2
#define RTCN_DIO_P2L 3
#define RTCN_DIO_P2H 4
#define RTCN_DIO_P3L 5
#define RTCN_DIO_P3H 6
#define RTCN_DIO_P4L 7
#define RTCN_DIO_P4H 8
#define RTCN_DIO_P5L 9
#define RTCN_DIO_P5H 10
#define RTCN_DIO_P6L 11
#define RTCN_DIO_P6H 12
#define RTCN_DIO_P7L 13
#define RTCN_DIO_P7H 14
#define RTCN_DIO_P8L 15
#define RTCN_DIO_P8H 16
#define RTCN_DIO_P9L  17
#define RTCN_DIO_P9H  18
#define RTCN_DIO_P10L 19
#define RTCN_DIO_P10H 20
#define RTCN_DIO_P11L 21
#define RTCN_DIO_P11H 22
#define RTCN_DIO_P12L 23
#define RTCN_DIO_P12H 24
#define RTCN_DIO_P13L 25
#define RTCN_DIO_P13H 26
#define RTCN_DIO_P14L 27
#define RTCN_DIO_P14H 28
#define RTCN_DIO_P15L 29
#define RTCN_DIO_P15H 30
#define RTCN_DIO_P16L 31
#define RTCN_DIO_P16H 32
#define RTCN_MXN_STP  41
#define RTCN_MXN_ORG  42
#define RTCN_MXN_STL  43
#define RTCN_MXN_PVW  44
#define RTCN_MXN_PVS  45
#define RTCN_MXN_TORQUE  50
#define RTCN_UPG_PRT  60

enum RobotStatus
{
	robotIdle = 0, // 空闲状态
	robotMoving = 1, // 运动过程中
	robotMotorErr = 2,// 电机错误：找到站点缺失，或是电机反馈错误
	robotDisable = 3, // 电机脱机状态
	robotUpdate = 4, // 控制器更新固件过程中
	robotLocked = 5, // 机器人锁定：控制器出错锁定
	robotFindHome = 6,  // 机器人正在找零点
	robotErrorHand = 7  // 机器人左右手不正确
};

/*脚本节点类型*/
enum XMLNodePointNodeType
{
	RTNodePoint 		= 1,     	// 路径点
	RTNodeWaitCondition = 2,     	// 等待输入IO电平
	RTNodeWait  		= 3,		// 等待一定时间
	RTNodeSetGPIO 		= 4,       	// 设置输出IO电平
	RTNodeLineMove 		= 5,		// 运动，然后判断子类型。
	RTNodeCurveMove 	= 6,		// 设置曲线移动
	RTNodeStartWhile 	= 7,  			// 开始while循环
	RTNodeEndWhile 		= 8,            // 结束while循环
	RTNodeWaitTimer 	= 9,			//等待定时器
	RTNodeIf 			= 10,
	RTNodeElse 			= 11,
	RTNodeEndIf 		= 12,
	RTNodeAssign 		= 13,				//赋值
	RTNodObjectPoint 	= 14,     	  //  对象坐标
	RTNodVariablePoint 	= 15,     	  // 可变坐标
	RTNodeSetVirtualGPIO 		= 16,       	// 设置虚拟输出
	RTNodeWaitVirtualCondition 		= 17,       	// 等待虚拟输入
	RTNodeIfElseStart     = 18,
	RTNodeArchmove		= 19,
	RTNodeStartSwitch 	= 20,  			// 开始Switch 
	RTNodeEndSwitch 	= 21,  			// 结束Switch 
	RTNodeCase 	= 22,  			// 开始Case
	RTNodeEndCase 	= 23,  			// 结束Case
	RTNodebreak = 24,  			// 开始break
	RTNodedefault = 25,  			// 开始default
	RTNodeEnddefault = 26,  			// 结束default
	RTNodeContinue = 27,
	RTNodeStartDo 	= 28,  			// 开始Do循环
	RTNodeDoEnd = 29,  			// 结束Do循环
	RTNodeEndElse = 30,
	RTNodeEndIfElse = 31,
	RTNodeBlank = 32,  			//  
	RTNodeSensorAssign 		= 33,				//赋值
	RTNodeCameraShotInstruct = 34,  			//  CameraShotInstruct
	RTNodeCameraResetCount = 35,  			//  CameraShotInstruct	
	RTNodeEndScript = 36,  			//  
	RTNodeMAX
};

enum eScaraArms
{
   Scara_FIRSTARM,
   Scara_SECONDARM,
   Scara_THIRDARM,
   Scara_FOURTHARM,
   Scara_ARMMAX
};
   

   enum SubRobotMoveType
   {
	  CurveMove  = 0,
	  GoToMove	 = 1,
	  LineToMove	 = 2,
	  CircleMove = 3,
	  Tracking4PfMove = 4,
	  Tracking6PfMove = 5,
	  SPPMove = 6,	  
	  CurveToMove = 7,
	  CPPMove = 8
   };


   //===========================
   //  机器人各个数字输出端口数据结构
   //===========================
   
   typedef union _ROBOTDIGITAL_VALUE_OBJ
   {
	   struct
	   {
		   unsigned bP1DVA : 1;
		   unsigned bP2DVA : 1;
		   unsigned bP3DVA : 1;
		   unsigned bP4DVA : 1;
		   unsigned bP5DVA : 1;
		   unsigned bP6DVA : 1;
		   unsigned bP7DVA : 1;
		   unsigned bP8DVA : 1;
		   unsigned bP9DVA : 1;
		   unsigned bP10DVA : 1;
		   unsigned bP11DVA : 1;
		   unsigned bP12DVA : 1;
		   unsigned bP13DVA : 1;
		   unsigned bP14DVA : 1;
		   unsigned bP15DVA : 1;
		   unsigned bP16DVA : 1;
	   };
	   unsigned int uiDigitalValue;
   }ROBOTDIGITAL_VALUE_OBJ, *P_ROBOTDIGITAL_VALUE_OBJ;

//===========================
//	密码数据结构
//===========================
typedef struct _UI_SETPASSWORD_OBJ
{
	char OldPassword[UISDK_MAXPASSWORDLEN];  //  旧密码
	int iOldPasswordLen;
	char NewPassword[UISDK_MAXPASSWORDLEN]; // 新密码
	int iNewPasswordLen;
}UI_SETPASSWORD_OBJ, *P_UI_SETPASSWORD_OBJ;

//===========================
//	网络信息数据结构
//===========================
typedef struct _UI_NETWORKCONFIG_OBJ
{
	char		IPAddress[UISDK_MAXPASSWORDLEN];  // IP
	int iIPAddressLen;
	char		MaskAddress[UISDK_MAXPASSWORDLEN]; // 掩码
	int iMaskAddressLen;
	char		GatewayAddress[UISDK_MAXPASSWORDLEN]; // 网关
	int iGatewayAddressLen;
}UI_NETWORKCONFIG_OBJ, *P_UI_NETWORKCONFIG_OBJ;
//===========================
//	日期时间数据结构
//===========================
typedef struct _UI_DATETIME_OBJ
{
	 int dt_sec; /* seconds after the minute - [0,59] */
	 int dt_min; /* minutes after the hour - [0,59] */
	 int dt_hour; /* hours since midnight - [0,23] */
	 int dt_mday; /* day of the month - [1,31] */
	 int dt_mon; /* months since January - [0,11] */
	 int dt_year; /* years since 1900 */
}UI_DATETIME_OBJ, *P_UI_DATETIME_OBJ;

//===========================
//	机器人运行程序信息数据结构
//===========================
typedef struct _UI_RUNNINGPROGRAM_OBJ
{
	int iResult; // 程序运行结果：0错误; 1成功
	int dt_mday; /* day of the month - [1,31] */	
	int dt_hour; /* hours since midnight - [0,23] */
	int dt_min; /* minutes after the hour - [0,59] */
	int dt_sec; /* seconds after the minute - [0,59] */
	char name[UISDK_MAX_DATA_LEN]; //  程序名字
}UI_RUNNINGPROGRAM_OBJ, *P_UI_RUNNINGPROGRAM_OBJ;

//===========================
//	机器人数据通讯绶冲区数据结构
//===========================
typedef struct _UI_COMMANDBUFFER_OBJ
{
	int iCount; // 用户数据长度
	char chDataBuffer[UISDK_MAX_DATA_LEN]; //  用户数据内容
}UI_COMMANDBUFFER_OBJ, *P_UI_COMMANDBUFFER_OBJ;

//===========================
//	机器人机械臂温度数据结构
//===========================
typedef struct _UI_MOTORTEMPERATURE_OBJ
{
	int iBaseTemperature; // 基座温度
	int iMainArmTemperature;// 主臂温度
	int iAssistantArmTemperature;// 副臂温度
	int iBarArmTemperature;// 丝杆温度
}UI_MOTORTEMPERATURE_OBJ, *P_UI_MOTORTEMPERATURE_OBJ;
//===========================
//	机器人初始工具位置坐标数据结构
//===========================
typedef struct _UI_INITIALIZETCP_OBJ
{
    int iSpeedRatio;
	float fTCPx; // 工具中心点相对X坐标值
	float fTCPy;// 工具中心点Y坐标值
	float fTCPz;// 工具中心点Z坐标值
	float fTCPRz;// 工具中心点Z坐标值
	float fThetaX; // 工具中心点相对X方向角度值
	float fThetaY;// 工具中心点Y方向角度值
	float fThetaZ;// 工具中心点Z方向角度值	
	float fToolZ;// 工具中心点Z方向长度值	
}UI_INITIALIZETCP_OBJ, *P_UI_INITIALIZETCP_OBJ;
//===========================
//	机器人初始工具位置坐标数据结构
//===========================
typedef struct _UI_INITIALIZETCPEX_OBJ
{
    int iArmSpeed[4];
	float fTCPx; // 工具中心点相对X坐标值
	float fTCPy;// 工具中心点Y坐标值
	float fTCPz;// 工具中心点Z坐标值
	float fTCPRz;// 工具中心点Z坐标值
	float fThetaX; // 工具中心点相对X方向角度值
	float fThetaY;// 工具中心点Y方向角度值
	float fThetaZ;// 工具中心点Z方向角度值	
	float fToolZ;// 工具中心点Z方向长度值	
}UI_INITIALIZETCPEX_OBJ, *P_UI_INITIALIZETCPEX_OBJ;

//===========================
//	机器人各个关节角度数据结构
//===========================
typedef struct _UI_JOINTANAGLE_OBJ
{
	double fBaseAngle; // 基座角度
	double fMainArmAngle;// 主臂角度
	double fAssistantArmAngle;// 副臂角度
	double fBarArmAngle;// 丝杆角度
}UI_JOINTANAGLE_OBJ, *P_UI_JOINTANAGLE_OBJ;



//===========================
//	机器人文件传输数据块数据结构
//===========================
typedef struct _UI_FILECONTENT_OBJ
{
    unsigned int fileSize;        //文件大小
    unsigned int position;        //该段内容在文件中的位置
    unsigned int length;          //缓冲区中的有效数据长度
    char         data[UISDK_MAX_FILECONTENT_LEN];      //缓冲区
}UI_FILECONTENT_OBJ, *P_UI_FILECONTENT_OBJ;



//===========================
//		SOCKETMSG CAN 结构定义
//===========================
#pragma pack(push, 1)
typedef struct _UI_ROBOTSOCKETMSG_OBJ
{
    unsigned int   ID;  // 报文 ID  = SID(11位) | EID (18位)
    char    can_dlc;// 数据长度，长度不能超过8;
    char    Data[8];// __attribute__((aligned(8))); //   数据
}UI_ROBOTSOCKETMSG_OBJ, *P_UI_ROBOTSOCKETMSG_OBJ;

#pragma pack(pop)

//===========================
//		机器人配置信息
//===========================
typedef struct _UI_ROBOTCONFIG_OBJ
{
	char title[30];
	char key[30];
	char value[30];
}UI_ROBOTCONFIG_OBJ, *P_UI_ROBOTCONFIG_OBJ;



/* TCP 坐标*/
typedef struct _TCP
{
	double x;
	double y;
	double z;
	double rz;
	double theta;
	_TCP()
	{
	   x = 0;
	   y = 0;
	   z = 0;
	   rz = 0;	
	   theta = 0;	
	}
}TCP;

/*  */
typedef struct _NodePVT
{
	int *P; /* PVT列表中的QP */
	int *V; /* PVT列表中的QV */
	int *T; /* PVT列表中的QT */
	int *A; /* 保留 */
	_NodePVT()
	{
		P = 0;
		V = 0;
		T = 0;
		A = 0;
	}
}NodePVT;

// 以下算法库头文件

typedef struct
{
	int uc[4][255];

} MotorUcode; // 单轴 U-代码
typedef  union
{
	struct
	{
		MotorUcode axis1; // # 1 电机 uCode
		MotorUcode axis2; // # 2 电机 uCode
		MotorUcode axis3; // # 3 电机 uCode
		MotorUcode axis4; // # 4 电机 uCode（中间旋转轴）

		int size;  // 有效 uCode 数据行数
	};

	struct
	{
		MotorUcode axis[4]; // 电机 uCode
		int dummy;          // 有效 uCode 数据行数
	};

} RobotUcode; // 机器人各轴 U-代码
typedef struct
{
	unsigned char kc[3][200][8];  // Sa:Sb | Sc:Sd | St:Qt (本段轨迹时间) (期望 QT)

	int Bgn; // 起点位置（单位：脉冲）
	int End; // 终点位置（单位：脉冲）

} MotorKcode; // 单轴 U-系数
typedef  union
{
	struct
	{
		MotorKcode axis1; // # 1 电机 uCode
		MotorKcode axis2; // # 2 电机 uCode
		MotorKcode axis3; // # 3 电机 uCode
		MotorKcode axis4; // # 4 电机 uCode（中间旋转轴）
		int size;  // 有效 uCode 数据行数
	};

	struct
	{
		MotorKcode axis[4]; // 电机 uCode
		int dummy;          // 有效 uCode 数据行数
	};

} RobotKcode; // 机器人各轴 U-系数

typedef struct
{
	int size;	// 有效数据行数
	// 各轴 速度极限(pps/ms)
	double Axs1Vmax;  // Delta: 主轴 / Scara:第1轴
	double Axs2Vmax;  // Delta: 主轴 / Scara:第2轴
	double Axs3Vmax;  // Delta: 主轴 / Scara:第3轴
	double Axs4Vmax;  // Delta: 中轴 / Scara:第4轴
	// 各轴 加速度极限(pps/ms)
	double Axs1Amax;  // Delta: 主轴 / Scara:第1轴
	double Axs2Amax;  // Delta: 主轴 / Scara:第2轴
	double Axs3Amax;  // Delta: 主轴 / Scara:第3轴
	double Axs4Amax;  // Delta: 中轴 / Scara:第4轴
}RobotAsxMax;

typedef  union
{
	struct  
	{
		// 通用
		int    RBTtype;   // 机器人类型 1 = Scara，2 = Delta

		// Scara 配置
		int    RightHd;   // 左右手定义 0=左手，1=右手

		// 通用
		int    MotorPPR;  // 电机每转脉冲数（脉冲）
		int    SmTime0;   // 轨迹起点平滑时间(ms) 默认 10 ms, 不得小于 5
		int    SmTime1;   // 轨迹终点平滑时间(ms) 默认 10 ms, 不得小于 5
		int    PvtQTdsr;  // 轨迹PVT之QT取值(ms)  默认 10 ms, 不得小于 5

		// Delta 配置
		double BaseArmR;  // Delta: 上臂轴-基座中心半径(mm)
		double PlatLnkR;  // Delta: 移台轴-移台中心半径(mm)
		double PlatHigh;  // Delta: 移台厚度
		double ArmUpLmt;  // Delta: 主臂上物理限位(度)

		// 机械结构(mm)
		double Arm1Len;   // Delta: 主臂长度 / Scara: 第1轴臂长(mm)
		double Arm2Len;   // Delta: 拉杆长度 / Scara: 第2轴臂长(mm)
		double TcpLength; // TCP 工具臂长
		double TcpHeight; // TCP 工具厚度

		// 减速比( +/- 号表示减速机输入/输出是同向/反向 )
		double Axis1Gear; // Delta: 主轴减速比 / Scara: 第1轴减速比 
		double Axis2Gear; // Delta: =Axis1Gear / Scara: 第2轴减速比
		double Axis3Gear; // Delta: =Axis1Gear / Scara: 丝杆齿轮减速比
		double Axis4Gear; // Delta: 中轴减速比 / Scara: Rz 齿轮减速比 
		double PzLead;    // Delta: =0         / Scara: 直线运动丝杆导程

		// 工作范围限制(mm)
		double PzLimitH;  // 工作高度(Z轴)上限(mm)
		double PzLimitL;  // 工作高度(Z轴)下限(mm)
		double HorizonR;  // Delta:工作范围半径(mm) / Scara: = 0;

		// 各轴 加速度(ms)
		double Axis1Acc;  // Delta: 主轴 / Scara:第1轴
		double Axis2Acc;  // Delta: 主轴 / Scara:第2轴
		double Axis3Acc;  // Delta: 主轴 / Scara:第3轴
		double Axis4Acc;  // Delta: 中轴 / Scara:第4轴（RZ）
		// 各轴 减速度(ms)
		double Axis1Dcc;  // Delta: 主轴 / Scara:第1轴
		double Axis2Dcc;  // Delta: 主轴 / Scara:第2轴
		double Axis3Dcc;  // Delta: 主轴 / Scara:第3轴
		double Axis4Dcc;  // Delta: 中轴 / Scara:第4轴
		// 各轴 摆角正极限(度)
		double Axs1Pmax;  // Delta: 主轴 / Scara:第1轴
		double Axs2Pmax;  // Delta: 主轴 / Scara:第2轴
		double Axs3Pmax;  // Delta: 主轴 / Scara:第3轴
		double Axs4Pmax;  // Delta: 中轴 / Scara:第4轴
		// 各轴 摆角负极限(度)
		double Axs1Pmin;  // Delta: 主轴 / Scara:第1轴
		double Axs2Pmin;  // Delta: 主轴 / Scara:第2轴
		double Axs3Pmin;  // Delta: 主轴 / Scara:第3轴
		double Axs4Pmin;  // Delta: 中轴 / Scara:第4轴
		// 各轴 速度极限(pps/ms)
		double Axs1Vmax;  // Delta: 主轴 / Scara:第1轴
		double Axs2Vmax;  // Delta: 主轴 / Scara:第2轴
		double Axs3Vmax;  // Delta: 主轴 / Scara:第3轴
		double Axs4Vmax;  // Delta: 中轴 / Scara:第4轴
		// 各轴 加速度极限(pps/ms)
		double Axs1Amax;  // Delta: 主轴 / Scara:第1轴
		double Axs2Amax;  // Delta: 主轴 / Scara:第2轴
		double Axs3Amax;  // Delta: 主轴 / Scara:第3轴
		double Axs4Amax;  // Delta: 中轴 / Scara:第4轴
				
		double LoadMass;  // 负载质量(Kg)
	};

	struct
	{
		int    val0[ 6];   // Dummy
		double val1[16];  // Dummy

		// 各轴 加速度(ms)
		double acc[4];
		// 各轴 减速度(ms)
		double dcc[4];
		// 各轴 摆角正极限(度)
		double pmax[4];
		// 各轴 摆角负极限(度)
		double pmin[4];
		// 各轴 速度极限(pps/ms)
		double vmax[4];
		// 各轴 加速度极限(pps/ms)
		double amax[4];

		double val2;  // Dummy
	};

	double *ptr;

} UIROBOT;    // 机器人结构参数

typedef  union   
{
	struct
	{
		double angle;  // 传送带流向与机器人 X+ 方向矢量夹角（度，有正负）
		double speed;  // 跟随速度 = 传输带流速, mm/s
	};

	double val[2];	   // 数组表示法

} CONVEYOR;   // 传输带结构参数
typedef  union   
{
	struct
	{
		int axis1;	// 分轴表示法
		int axis2;
		int axis3;
		int axis4;
	};

	int axis[4];	// 数组表示法

} ROBOTPLS;   // Robot Pulse 机器人各轴的脉冲(脉冲)
typedef  union
{
	struct
	{
		int axis1;	// 分轴表示法
		int axis2;
		int axis3;
		int axis4;
	};

	int axis[4];	// 数组表示法

} ROBOTSPD;   // Robot Pulse 机器人各轴的速度(脉冲/秒，pps)
typedef  union
{
	struct
	{
		double axis1;	// 分轴表示法
		double axis2;
		double axis3;
		double axis4;
	};

	double axis[4];	// 数组表示法

} ROBOTANG;   // Robot Angle 机器人各轴的角度(度)

typedef  union  
{
	struct
	{
		double x;  // 坐标表示法
		double y;
		double z;
		double r;
	};
	
	double val[4];	// 数组表示法

} POS_XYZR;   // 机器人坐标系中坐标点(mm,度)

typedef struct
{
	double pls[32000]; // 脉冲 pls
	double pps[32000]; // 速度 pps
	double acc[32000]; // Acc  pps/ms
	double tim[32000]; // 时刻表 ms

	double Vmax;        // 最大速度
	double Amax;        // 最大加速度

	int size;          // 有效数据行数
} MotorSdata; // 机器人单轴仿真数据 脉冲/速度/加速度/时刻表
typedef struct
{
	MotorSdata axis1;  // # 1 电机 仿真数据
	MotorSdata axis2;  // # 2 电机 仿真数据
	MotorSdata axis3;  // # 3 电机 仿真数据
	MotorSdata axis4;  // # 4 电机 仿真数据（中间旋转轴）

	POS_XYZR   pos[32000]; // 3D 位置 XYZR
	
	int size;   // 有效数据行数
} RobotSdata; // 机器人各轴仿真数据 脉冲/速度/加速度/时刻表

typedef  union   
{
	struct
	{
		int total;  // 坐标表示法
		int sect1;
		int sect2;
		int sect3;
	};

	int val[4];	// 数组表示法

} TRJ_TIME;   // 轨迹区段行程时间（ms）

// GOTO

typedef  union
{
	struct
	{
		POS_XYZR  Bgn;   // [IN ] 当前点
		POS_XYZR  End;   // [IN ] 目标点
		double    Tm;   // [IN ] 行程时间 ( ms ) 
	};

	double val[9];	     // 数组表示法

} PTPMDL_GTO; // 运动参数 Parameter
typedef  union
{
	struct
	{
		ROBOTPLS pa; // [OUT] 绝对位置
		ROBOTSPD sp; // [OUT] 速度
	};

	int val[8];	     // 数组表示法

} PTPSLN_GTO; // 解决方案 Solution

// LINETO

typedef  union
{
	struct
	{
		POS_XYZR  Bgn;   // [IN ] 当前点
		POS_XYZR  End;   // [IN ] 目标点
		double    Tm;   // [IN ] 行程时间 ( ms ) 
	};

	double val[9];	     // 数组表示法

} TRJMDL_LTO; // 运动参数 Parameter
typedef  union
{
	struct
	{
		POS_XYZR  Bgn;  // [IN ] 当前点
		POS_XYZR  End;  // [IN ] 目标点
		double    Tm;   // [IN ] 行程时间 ( ms ) 
	};

	double val[9];	    // 数组表示法

} TRJSLN_LTO; // 解决方案 Solution

// 标准轨迹

typedef  union   
{
	struct
	{
		POS_XYZR  Bgn;   // [IN ] 轨迹起点
		POS_XYZR  End;   // [IN ] 轨迹终点
		double    Hgt;   // [IN ] 拱形平移轨迹的拱高：拱顶 - 终止腿上端点 之间距离
		double    Lg1;   // [IN ] 起点腿长（永远为正）
		double    Lg2;   // [IN ] 终点腿长（永远为正）
		double    Vop;   // [IN ] Velocity Option 速度表示方式选项 0=行程时间方式（ms） / 1=速度方式（mm/s）
		double     V1;   // [IN ] Velocity 起点腿 速度(ms 或 mm/s) 
		double     V2;   // [IN ] Velocity 平移   速度(ms 或 mm/s)
		double     V3;   // [IN ] Velocity 终点腿 速度(ms 或 mm/s) 
	};

	double val[15];	     // 数组表示法

} TRJMDL_STD; // 模型参数 Standard Point - Point Trjectory
typedef  union   
{
	struct
	{
		POS_XYZR  Bgn;   // [IN ] 轨迹起点
		POS_XYZR  Cv0;   // [IN ] 拱形平移起点
		POS_XYZR  Cv1;   // [IN ] 拱形平移拱顶
		POS_XYZR  Cv2;   // [IN ] 拱形平移终点
		POS_XYZR  End;   // [IN ] 轨迹终点

		TRJ_TIME   Tm;
		// T.total:  [OUT] 全程轨迹行程时间 ms
		// T.sect1;  [OUT] Leg1 行程时间 ms
		// T.sect2:  [OUT] Arc  行程时间 ms
		// T.sect3:  [OUT] Leg2 行程时间 ms
	};
	
	struct
	{
		POS_XYZR  pos[5];
		int        tm[4];
	};

} TRJSLN_STD; // 解决方案 Standard Point - Point Solution 

// 跟随轨迹
typedef  union   
{
	struct
	{
		POS_XYZR Bgn;  // [IN ] 起点 mm
		POS_XYZR Gnd;  // [IN ] 跟随起点 mm
		POS_XYZR End;  // [IN ] 终点 mm

		CONVEYOR Belt; // [IN ] 传输带参数
		
		double    Ha;  // [IN ] Height,mm   Aproaching，进近高度(相对Gnd点)
		double    Ht;  // [IN ] Height,mm   Taxing,     滑行高度(相对Gnd点)，补偿传输带倾斜度, 保留=0
		double    Hf;  // [IN ] Height,mm   Taking off, 起飞高度(相对Gnd点)
		
		double    Sa;  // [IN ] Distance,mm Aproaching，进近距离
		double    Sf;  // [IN ] Distance,mm Taking off, 起飞距离
		// 注：跟随距离不允许设置，将根据跟随时间（V2）和传输带速度计算
		
		double    T1;  // [IN ] Velocity,ms 行程时间，6PF：B起始点->G跟随点的时间，4PF: C->G 进近段行程时间
		double    T2;  // [IN ] Velocity,ms 行程时间，即跟随时间
		double    T3;  // [IN ] Velocity,ms 行程时间，6PF：F起飞点->E终点的时间，4PF: F->D 起飞段行程时间
		// 注,速度的设置仅按行程时间（ms）方式，无法使用实际速度
	};

	struct
	{
		POS_XYZR pos[3];
		CONVEYOR belt;
		double     H[3];  // [IN ] Height   行程高度 mm，trjH[0] 进近高度，trjH[1]=0保留  ，trjH[2]起飞高度，所有高度均相对 G 点
		double     S[2];  // [IN ] Distance 行程距离 mm，trjS[0] 进近距离，trjS[1]起飞距离
		double     V[3];  // [IN ] Velocity 行程速度 ms，trjV[0] 6PF: B->G时间，4PF: C->G时间; trjV[1]G->F跟随时间，trjV[2] 6PF: F->E时间, 4PF: F->D时间
	};

} TRJMDL_6PF; // 模型参数 Point - Follow - Point Trajectory
typedef  union   
{
	struct
	{
		POS_XYZR  Bgn;  // [IN ] 起点   mm
		POS_XYZR  Cin;  // [OUT] 切入点 mm
		POS_XYZR  Gnd;  // [IN ] 触地点 mm（跟随起点）
		POS_XYZR  Fly;  // [OUT] 起飞点 mm（跟随终点）
		POS_XYZR  Dpt;  // [OUT] 切出点 mm
		POS_XYZR  End;  // [IN ] 终点   mm

		TRJ_TIME   Tm;
		// T.total:  [OUT] 全程轨迹行程时间 ms
		// T.sect1;  [OUT] 6PF: B->G 行程时间; 4PF: C->G 行程时间
		// T.sect2:  [OUT] G->F 跟随段时间
		// T.sect3:  [OUT] 6PF: F->D 行程时间; 4PF: F->D 行程时间
		// 各点定义见相关（6PF/4PF）函数说明
	};

	struct
	{
		POS_XYZR  pos[6];
		int        tm[4];
	};

} TRJSLN_6PF; // 解决方案 Point - Follow - Point Solution




// 算法库头文件结束

/* 机器人的PVT列表 */
typedef struct _RobotPVTLists
{
	NodePVT firstArmPVT; /* 第一关节的PVT列表结点 */
	NodePVT secondArmPVT; /* 第二关节的PVT列表结点 */
	NodePVT thirdArmPVT; /* 第三关节的PVT列表结点 */
	NodePVT fourthArmPVT; /* 第四关节的PVT列表结点 */
	int pvtCount; /* PVT列表中数据个数 */
	_RobotPVTLists()
	{
		pvtCount = 0;
	}
}RobotPVTLists;

// 数据结构如下所示：
typedef struct _UI_CAMERAMODE_OBJ
{
	char 	camModelServIP[16];/*视觉服务器IP地址*/
	int		camModelServPort;/*视觉服务器TCP端口*/
	float	camModelStartX;/*抓取动作初始位置X*/
	float	camModelStartY;
	float	camModelStartZ;
	float 	camModelStartRZ;
	int		camModelIOPort;/*抓取动作使用机器人输出端口*/
	float	camModelMidZ;/*抓取动作抓取过程中可能经过的点，只限Z轴*/
	float	camModelEndZ;/*抓取动作最后点Z轴坐标*/
	int		camModelDelayMS;/*抓取动作延迟时间，单位：毫秒*/
	float	camModelZeroX;	//相对原点x值坐标;
	float 	camModelZeroY;	//相对原点y值坐标;
	float	camModelZeroRZ; //相对原点rz值坐标;
	float	camSensorZeroX;	// 原点在相机中的坐标x值坐标;
	float 	camSensorZeroY;	//原点在相机中的坐标y值坐标;
	int     camMoveObjectPoint; // 1表示走位，0表示抓取功能
	float   camMaxValue[4]; // 最大值
	float   camMinValue[4]; // 最小值
	float  camfOjbectSpeed;
	float  camfPixelSize; // 每个像素大小, 单位为: mm/pixel
	int  camiObjectMoveDirect;// 0:X方向运动; 1:Y方向运动
	int  camfCaptureTime; // 相机连续拍照的频率 ms/times   每次毫秒数	
	//double  camfTotalDistance; // 校准时，相对位移量 单位为mm 
	int    cambSensorTrigger; // 传感器触发拍照，区别于连续脉冲方式
	int camiXInverse;
	int camiYInverse;
	int camiSensorXInverse;
	int camiSensorYInverse;
	int cambSensorRawData;
	char plcServIP[16]; // plc 通讯,PLC通讯端口个数存在plcServPortCout:一个端口则存在plcServPort;两个端口则分别存在:plcServReadPort,plcServWritePort
	int plcServPort;
	int plcServReadPort;
	int plcServWritePort;
	int plcServPortCout;
	int  camiHardwareSync; // 硬件同步时间
	int  iSensorTriggerPort; // 与相机触发同步信号的输入端口索引号:从1开始。
	int  iPulseOutPort; // 机器人发送的同步信号输出端口索引号:从数值从0~7对应P9~P16。
	int  camiObjectMoveAdd;// 0:负方向;1:正方向 
	int  camisServer;// 机器人相机接口，0:客户端;1:服务端
}UI_CAMERAMODE_OBJ, *P_UI_CAMERAMODE_OBJ;

//===========================
//		工具负载参数结构
//===========================
typedef struct _UI_TOOLMASS_OBJ
{
	unsigned int index;					//索引值
	char name[30];		                //名字
	float relativePos[4];				//相对坐标值
	float massPos[4];					//负载坐标值
}UI_TOOLMASS_OBJ, *P_UI_TOOLMASS_OBJ;
//===========================
//		用户坐标系参数结构
//===========================
typedef struct _UI_USERCOOR_OBJ
{
	unsigned int index;
	char name[30];
	float point1[3];
	float point2[3];
	float point3[3];
}UI_USERCOOR_OBJ, *P_UI_USERCOOR_OBJ;
//===========================
//		安全区域参数结构
//===========================
typedef struct _UI_SAFEAREA_OBJ
{
	unsigned int index;
	char name[30];
	float Point1[3];
	float Point2[3];
	unsigned int portIndex;
	unsigned int portValue;
}UI_SAFEAREA_OBJ, *P_UI_SAFEAREA_OBJ;
//===========================
//		固定点位参数结构
//===========================
typedef struct _UI_FIXEDPOINT_OBJ
{
	unsigned int index;
	char name[30];
	float point[3];
	unsigned int portIndex;
	unsigned int portValue;
}UI_FIXEDPOINT_OBJ, *P_UI_FIXEDPOINT_OBJ;
//===========================
//		缓存点系参数结构
//===========================
typedef struct _UI_CACHE_OBJ
{
	unsigned int index;
	char name[30];
	float toolPos[4];				//相对坐标值
	float jointPos[4];					//负载坐标值
}UI_CACHE_OBJ, *P_UI_CACHE_OBJ;


typedef struct _XMLDataNode
{
	int id;
	// USERVAROPT_VALUE_TYPE_USERKEY USERVAROPT_VALUE_TYPE_VARARRAYMEMBER
	// USERVAROPT_VALUE_TYPE_VARPOINTMEMBER USERVAROPT_VALUE_TYPE_VARPOINTARRAYMEMBER
	char DataType;
	char Name[NODEPOINT_NAME_MAX];
	union
	{
		float fConstValue; // 常量
		int iVarIndex;
		int iPointIndex;
		int iPortIndex;
		int iCamIndex;
		struct
		{
			int iPointIndex;
			int iMemberIndex;
		}VarPointMember;
		struct
		{
			int iVarIndex;
			int iVarArrayIndex;
		}VarArrayMember;
		struct
		{
			int iPointIndex;
			int iPointArrayIndex;
		}VarPointArrayPoint;
		struct
		{
			int iPointIndex;
			int iPointArrayIndex;
			int iMemberIndex;
		}VarPointArrayMemberAxis;
	}DataValue;
	void Reset()
	{
		DataValue.VarPointArrayMemberAxis.iPointArrayIndex = 0;
		DataValue.VarPointArrayMemberAxis.iPointIndex = 0;
		DataValue.VarPointArrayMemberAxis.iMemberIndex = 0;
	}
}XMLDataNode, *P_XMLDataNode; // 左值或右值结构体
typedef struct _XMLoperation
{
	P_XMLDataNode pResult;			// result
	P_XMLDataNode pValueA;			//value, if valueA_type==1 then valueA is int value;
	P_XMLDataNode pValueB;			//value, if valueB_type==0 then valueB is int value;
	// 2: -;
	// 3: *;
	// 4: /
	// 5: >
	// 6: <
	// 7: ==
	// 8: >=
	// 9: <=
	_XMLoperation* pCompValueA;			//value, if valueA_type==1 then valueA is int value;
	_XMLoperation* pCompValueB;			//value, if valueB_type==0 then valueB is int value;
	int opt;			// 1: +;
	void Reset()
	{
		opt = 0;
		pResult = 0;
		pValueA = 0;
		pValueB = 0;
		pCompValueA = 0;
		pCompValueB = 0;

	};
} XMLOperationObj, *P_XMLOperationObj; // 赋值运算结构体


// NodePoint的扩展，用于在A9内部使用
typedef struct _XMLNodePoint
{
	unsigned char		nodeType;             //  节点类型：坐标，IO输入，IO输出,运动节点，赋值，可变对象坐标， 等待虚拟IO输入等等
	unsigned char		speedRate;           // 移动速度,取值范围:1~00
	unsigned char		counts;		         // 子节点个数
	int m_iNodeId;                          //  节点的序号：从0开始
	bool m_bRunStop;                        // 是否可以设置断点，用于脚本调试时，单步运行
	char aliasName[NODEPOINT_NAME_MAX];		// 节点的名称
	char xmlInstruct[NODEPOINT_NAME_MAX];   // 预留
	union
	{ // nodeData
		struct
		{
			float		point[4];  		       //路点的坐标
			unsigned char isTransitionPoint; // 是否过度点
		}waypoint;
		float		waitTimes;		         //等待时间
		unsigned int	waitCond[3];    	// waitCond[0]: 端口; waitCond[1]: 条件; waitCond[2]:值
		unsigned int	gpioSet[4];			// gpioSet[0]: 类型, 4:TTL, 3:PWM,   gpioSet[1]: 端口； gpioSet[2]: 值;gpioSet[3]:PWM 时基
		unsigned int nVarPointIndex;       // 可变点索引序号
		int nSensorIndex;                 // 相机索引序号
		struct
		{
			int iVarPointIndex;          // 可变点索引序号
			int iPointArrayIndex;        // 可变点数组索引序号
			int iType;                   // USERVAROPT_VALUE_TYPE_POINT_VARIABLES,USERVAROPT_VALUE_TYPE_VARPOINTARRAYVARPOINT,USERVAROPT_VALUE_TYPE_VARPOINTARRAYCONSTPOINT
		}VarPointArrayPoint;
	
        struct
		{
            unsigned char subMoveType;                    //    运动类型：moveCurve=0,movePTP=0,moveLine=0,moveCircle=0
			unsigned char userCoordindateSystemIndex;//  用户坐标系索引..
			unsigned char tcpIndex;                    //  工具坐标index			
			unsigned short uiAddStopTime;              // 增加的停止时间
			int iIOTriggerTime;                        //   IO触发时间
			float Lg1;// [IN ] 起点腿长（永远为正）
			float Lg2;// [IN ] 终点腿长（永远为正）
			float Hgt; // [IN ] 平移行程拱高（必须为正值）
			int iStartSpeed;                          // 起点腿速度(ms 或 mm/s) 
			int iEndSpeed;                           // 起点腿速度(ms 或 mm/s) 
			int iMoveSpeed;                           // 平移速度(ms 或 mm/s)
			int itrjV0;                               // 速度表示方式选项 0=行程时间方式（ms） / 1=速度方式（mm/s）
			int iReserve[6];                           // 保留
		}SPPmoveParameters;                             //  拱形运动参数表
		struct
		{
			unsigned char subMoveType;                   //运动类型：moveCurve=0,movePTP=0,moveLine=0,moveCircle=0 
			unsigned char userCoordindateSystemIndex; //用户坐标系索引..
			unsigned char tcpIndex;                      //工具坐标index
			unsigned short uiAddStopTime;               // 增加的停止时间
			float    beltangel;                         // 传送带流向与机器人 X+ 方向矢量夹角（度，有正负）
			float    beltspeed;                         // 跟随速度 = 传输带流速, mm/s
			float	  Ha;  // [IN ] Height,mm	Aproaching，进近高度(相对Gnd点)
			float	  Ht;  // [IN ] Height,mm	Taxing, 	滑行高度(相对Gnd点)，补偿传输带倾斜度, 保留=0
			float	  Hf;  // [IN ] Height,mm	Taking off, 起飞高度(相对Gnd点)			
			float	  Sa;  // [IN ] Distance,mm Aproaching，进近距离
			float	  Sf;  // [IN ] Distance,mm Taking off, 起飞距离
			// 注：跟随距离不允许设置，将根据跟随时间（V2）和传输带速度计算
			int	  T1;  // [IN ] Velocity,ms 行程时间，6PF：B起始点->G跟随点的时间，4PF: C->G 进近段行程时间
			int	  T2;  // [IN ] Velocity,ms 行程时间，即跟随时间
			int	  T3;  // [IN ] Velocity,ms 行程时间，6PF：F起飞点->E终点的时间，4PF: F->D 起飞段行程时间
			int iReserve[4]; // 保留
		}TKGPickPlaceParameters;// 64个字节             //  新的传送带跟随运动参数表
		struct
		{
			unsigned char subMoveType; //  运动类型
			float Tm;           // [IN ] 行程时间 ( ms ) 
		}LineTomoveParameters;// 
		struct
		{
			long		waitTimerSecond;	//等待计时器秒时间(从1970-1-1到现在的秒数)
			long		waitTimerUS;		//等待计时器微秒
		}waitTimer;

		struct
		{
			int result_type;   //type: 0:check； 1:count
			int result;			// result
			int valueA_type;	//type: 1: int; 2:var; 3: io
			int valueA;			//value, if valueA_type==1 then valueA is int value;
			// 		 if valueA_type==2 then valueA is user var index;
			//		 if valueA_type==2 then valueA is io port;

			int valueB_type;	//type: 1: int; 2:var; 3: io

			int valueB;			//value, if valueB_type==0 then valueB is int value;
			// 		 if valueB_type==1 then valueB is user var index;
			//		 if valueB_type==2 then valueB is io port;
			int opt;			// 1: +;
			// 2: -;
			// 3: *;
			// 4: /
			// 5: >
			// 6: <
			// 7: ==
			// 8: >=
			// 9: <=

			void Reset()
			{
				result_type = 0;
				result = 0;
				valueA_type = 0;
				valueA = 0;
				valueB_type = 0;
				valueB = 0;
				opt = 0;

			}
			bool CopyFromAnother(_XMLNodePoint *Source)
			{
				result_type = Source->nodeData.operation.result_type;
				result = Source->nodeData.operation.result;
				valueA_type = Source->nodeData.operation.valueA_type;
				valueA = Source->nodeData.operation.valueA;
				valueB_type = Source->nodeData.operation.valueB_type;
				valueB = Source->nodeData.operation.valueB;
				opt = Source->nodeData.operation.opt;
				return true;
			}
		} operation;
		XMLOperationObj XMLoperation;
	}nodeData;
	_XMLNodePoint()
	{
	   Reset();	   
	}
	void Reset()
	{
		nodeData.XMLoperation.Reset();
		// memset(xmlInstruct, 0x0, sizeof(xmlInstruct));
		m_iNodeId = -1;
		m_bRunStop = false;	  
	}

}XMLNodePoint, *P_XMLNodePoint;


typedef  struct _CallBackData
{
	int m_iConnectType;
	int m_iRtcnCmd;
	char m_chMessageBuffer[10];
}CallBackData, *pCallBackData;

//===========================
//	机器人工具中心点坐标数据结构
//===========================
typedef struct _UI_ROBOTTCP_OBJ
{
	double fTCPx; // 工具中心点相对X坐标值
	double fTCPy;// 工具中心点Y坐标值
	double fTCPz;// 工具中心点Z坐标值
	double fTheta;// 工具中心点角度
	double fpayloadX; // 负载中心点相对X坐标值
	double fpayloadY; // 负载中心点Y坐标值
	double fpayloadz; // 负载中心点Z坐标值
	double fmass;      // 负载重量 kg	
	_UI_ROBOTTCP_OBJ()
	{
	     fTCPx = 0.0;
	     fTCPy = 0.0;
	     fTCPz = 0.0;
	     fTheta = 0.0;
	     fpayloadX = 0.0;
	     fpayloadY = 0.0;
	     fpayloadz = 0.0;
	     fmass = 0.0;		 
	}
}UI_ROBOTTCP_OBJ, *P_UI_ROBOTTCP_OBJ;

typedef void (*PF_UIROBOTSDK_CALLBACK)(unsigned int dwConnectCode, unsigned char cmd_type, char *pMessageBuffer, int iDataLen);

#endif//UIROBOTSTRUCT_H

