#pragma once
#include "UIRobotStruct.h"
#include "UIError.h"
#include <vector>

#define ROBOT_PARA_NAME			                     ("Para.ini")

using namespace std;
// 标准运动参数
typedef struct _SPPMoveParam
{
	struct
	{
		double    Hgt;   // [IN ] 拱形平移轨迹的拱高：拱顶 - 终止腿上端点 之间距离
		double    Lg1;   // [IN ] 起点腿长（永远为正）
		double    Lg2;   // [IN ] 终点腿长（永远为正）
		double    Vop;   // [IN ] Velocity Option 速度表示方式选项 0=行程时间方式（ms） / 1=速度方式（mm/s）
		double     V1;   // [IN ] Velocity 起点腿 速度(ms 或 mm/s) 
		double     V2;   // [IN ] Velocity 平移   速度(ms 或 mm/s)
		double     V3;   // [IN ] Velocity 终点腿 速度(ms 或 mm/s) 
	};
	double val[7];	     // 数组表示法
}SPPMoveParam, *P_SPPMoveParam;
// 跟随运动参数
typedef struct _PFMoveParam
{
	struct
	{
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
		CONVEYOR belt;
		double     H[3];  // [IN ] Height   行程高度 mm，trjH[0] 进近高度，trjH[1]=0保留  ，trjH[2]起飞高度，所有高度均相对 G 点
		double     S[2];  // [IN ] Distance 行程距离 mm，trjS[0] 进近距离，trjS[1]起飞距离
		double     V[3];  // [IN ] Velocity 行程速度 ms，trjV[0] 6PF: B->G时间，4PF: C->G时间; trjV[1]G->F跟随时间，trjV[2] 6PF: F->E时间, 4PF: F->D时间
	};
}PFMoveParam, *P_PFMoveParam;


// 坐标结构体
class RobotCoordinate
{
public:
	RobotCoordinate()
	{

	}

	RobotCoordinate(float fTCPx, float fTCPy, float fTCPz, float fTCPRz)
	{
		this->fTCPx = fTCPx;
		this->fTCPy = fTCPy;
		this->fTCPz = fTCPz;
		this->fTCPRz = fTCPRz;
	}

	float fTCPx; // 工具中心点X坐标值
	float fTCPy;// 工具中心点Y坐标值
	float fTCPz;// 工具中心点Z坐标值
	float fTCPRz;// 工具中心点Z坐标值
};

class CRobot
{
public:
	CRobot(void);
	~CRobot(void);
	//ini格式 section名字
	void SetRobotName(string val) { m_strRobotName = val; }

	//初始化
	int Init();
	void Uninit();
    int LoadParameter();
    void PrintParameter();
	
	//连接，断开机器人
	int doConnect();
	void disConnect();
	//机器人当前连接ID
    void DwConnectCode(unsigned int val) { dwConnectCode = val; }

	//急停
	int NormalStop();
    unsigned int DwConnectCode() const { return dwConnectCode; }

	//IO输出设置函数
	int SetRobotOutPortDigitalValue(int nIONum, bool bHighLevel);

	//IO输入查询
	int GetRobotInPortDigitalValue(int nIONum, int &nVal);
	int GetRobotOutPortDigitalValue(int nIoNum, int &nVal);

	//回调函数，主要接受机器人实时消息
	void RobotRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen);

	//错误解析函数
	void OnPraseErrorMsg(int des);
	//检测机器人坐标是否是一个点
	bool CheckIsOnePos(RobotCoordinate pos1, RobotCoordinate pos2);

	//机器人曲线运动
	int CurveMoveTo(RobotCoordinate Coord, int nMoveTime, float fHgt = 20);	
	// 机器人直线运动带中间点 
	int CurveToMoveTo(RobotCoordinate CoordMid,RobotCoordinate CoordTo, int nMoveTime, bool bWaitFlag = true);
	//机器人直线运动
    int LineMoveTo(RobotCoordinate CoordTo, int nMoveTime, bool bWaitFlag = true);
    //机器人直线运动
    int LineMoveToNoDelay(RobotCoordinate CoordTo, int nMoveTime)
    {
        return LineMoveTo(CoordTo, nMoveTime, false);
    }
	// 机器人标准运动带中间点
	int CPPMoveTo(RobotCoordinate CoordMid,RobotCoordinate CoordTo, SPPMoveParam moveParam, bool bWaitFlag = true);
	//机器人标准运动
    int SPPMoveTo(RobotCoordinate CoordTo, SPPMoveParam moveParam, bool bWaitFlag = true);

	//轨迹运动参数，只需设置一次，若需要动态变换，根据实际场景设置
	//机器人送带6点跟随运动参数
	void SetTrjMotion6PFMotionPara(PFMoveParam PFParam);
	//执行轨迹运动
    int TrjMotion6PF(RobotCoordinate BeginPoint, RobotCoordinate GroundPoint, RobotCoordinate EndPoint, bool bWaitFlag = true);

    //锁定解锁机器人
    int LockAllJoints();
    int LooseAllJoints();

    int getRobotStatus(int *nRobotStatus);

    float GetRobotSpeed() {return m_fRobotSpeed;}
    int GetSpeedRate() {return m_nSpeedRate;}
    void SetSpeedRate(int nSpeedRate);

	//获取当前机器人坐标
    int GetRobotCoor(RobotCoordinate &coor);

    //同步当前机器人坐标
    int SyncRobotCoor() {return GetRobotCoor(CoordCurrentPosition);}

	//获取PVT运动标志位g
	bool *GetPVTMoveFlag();

	int GetT1() const { return m_nT1; }
	int GetT2() const { return m_nT2; }
	int GetT3() const { return m_nT3; }
	int GetTotalTime() { return m_nT1 + m_nT2 + m_nT3; }
	//标志位等待函数
    bool WaitFlag(bool *pBFlag, unsigned int dwTimeOutTime = 10000);
    RobotCoordinate GetDestinationPosition(){return CoordDestinationPosition;}
    RobotCoordinate GetCurrentPosition(){return CoordCurrentPosition;}
	
	//设置传送带速度
    void SetBeltSpeed(float fBeltSpeed){m_fBeltSpeed = fBeltSpeed;}
    // 检测坐标是否有效
    int checkPointValid(RobotCoordinate coor);
    // 获取工具坐标
    int GetRobotTCP(double *pTcpLength, double *pTcpHeight, double *pTcpAngle);
    // 设置工具坐标
    int SetRobotTCP(double TcpLength, double TcpHeight, double TcpAngle);
    // 等待函数
    bool WaitRobotDone(int nTime);

    int GetErrnoFromRTCN(){ return m_nErrnoFromRTCN; }
	int getUIRototType(int *piRobotType);// 获取机器人类型

    string getRobotIP(){ return m_strIP;}

    CUIError m_ErrorParse;
	
	//初始化安全高度
	float m_fZSafeHeght;
	
	//将机器人状态代码翻译写入日志
	void RecordRobotStatusByCode(int iRobotStatus);
private:
    //机器人目的位置
    RobotCoordinate CoordDestinationPosition;
    //机器人当前位置
    RobotCoordinate CoordCurrentPosition;

    //机器人当前运动时间
    int m_nMoveTime;
	//机器人运动最短时间限制
	int m_nMoveTimeMinLimit;
	//机器人运动最长时间限制
	int m_nMoveTimeMaxLimit;

	//机器人运动速度系数(1~100)
    int m_nSpeedRate;
	//机器人运动速度(mm/ms)
	float m_fRobotSpeed;
	
	int m_nThreshold;
	//RTCN错误
	int m_nErrnoFromRTCN;

	//传送带角度，传送带速度
	float m_fBeltAngle, m_fBeltSpeed;
	//设置轨迹进近,起飞参数
	//进近参数:高度，滑行距离
	float m_fHa, m_fSa;
	//起飞参数：高度，起飞距离
	float m_fHf, m_fSf;

	//机器人轨迹运动跟随时间
	int m_nT2;
	//机器人轨迹运动起点到抓起点时间，动态计算,若不动态计算，则取m_nTrjMoveOneTime值
	int m_nT1;
	//机器人轨迹运动抓取点到结束点点时间，动态计算，若不动态计算，则取m_nTrjMoveOneTime值
	int	m_nT3;

	//运动结束后是否检查位置
	int m_nCheckPos;
    //是否模拟
    int m_nSimulation;

	//机器人连接ID
    unsigned int dwConnectCode;
	//机器人IP
	string m_strIP;
	//机器人配置名
	string m_strRobotName;
	//机器人运动到位标志
	bool m_bPVTMoveOK;
	//机器人是否已连接
    bool m_bConnect;
};


class CRobotManager
{
public:
	CRobotManager(void);
	~CRobotManager(void);
	static CRobotManager *GetInstance()
	{
		static CRobotManager instance;
		return &instance;
	}
	void AddRobot(CRobot *pRobot)
	{
		m_RobotVetor.push_back(pRobot);
	}
	static void RobotStaticRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen);
	void DoRobotStaticRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen);
	vector<CRobot *> m_RobotVetor;
};