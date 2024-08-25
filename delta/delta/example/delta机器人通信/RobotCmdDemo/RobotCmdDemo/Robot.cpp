#include "stdafx.h"
#include "Robot.h"
#include "Ini.h"
#include "utility.h"
#include <math.h>


#define  RTCN_SYS_ERROR                 0

#define  MIN_TIMELIMIT                  400  // 参数加载失败或错误时，最小时间，单位：ms
#define  MAX_TIMELIMIT                  5000  // 参数加载失败或错误时，最大时间，单位：ms
#define  TRJ_ONE_DINTANCE               300

/*=============================================================================
	 NAME:  CRobot()
	 -------------------------------------------------------------------------- 
	 Function:构造函数
	 -------------------------------------------------------------------------- 
     Parameter:		m_nSimulation
	 -------------------------------------------------------------------------- 
	 Return Value:
 ------------------------------------------------------------------------------ 
	 Note:
	   1.成员变量初始化
	   2.把当前机器人对象加入到机器人管理对象中
=============================================================================*/
CRobot::CRobot(void)
{
// 成员变量初始化
	m_bPVTMoveOK = false;
	m_bConnect = false;	
	dwConnectCode = 0;
    m_nSimulation = 0;
    m_nSpeedRate = 100;
	m_fRobotSpeed = 0.1;
	m_nErrnoFromRTCN = 0;
    CoordCurrentPosition.fTCPx = 0;
    CoordCurrentPosition.fTCPy = 0;
    CoordCurrentPosition.fTCPz = -100;
    CoordCurrentPosition.fTCPRz = 0;
// 机器人管理类中增加一个机器人实例
	CRobotManager::GetInstance()->AddRobot(this);
}
/*=============================================================================
	 NAME:  ~CRobot
	 --------------------------------------------------------------------------- 
	 Function:析构函数
	 --------------------------------------------------------------------------- 
	 Parameter:		
	 --------------------------------------------------------------------------- 
	 Return Value:
 ------------------------------------------------------------------------------- 
	 Note:
=============================================================================*/
CRobot::~CRobot(void)
{
}
/*=============================================================================
	 NAME: int Init()
	 -------------------------------------------------------------------------- 
	 Function:机器人对象初始化函数
	 -------------------------------------------------------------------------- 
	 Parameter:
	 -------------------------------------------------------------------------- 
	 Return Value:	
					If the function fails, the return value is error code 
					If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ------------------------------------------------------------------------------ 
	 Note:
	    1.加载参数
	    2.连接机器人
=============================================================================*/
int CRobot::Init()
{
 //=======================================
 // 从配置文件中加载参数
    int nRet = LoadParameter();
 // 如果加载失败，则返回错误码
    if(UISDK_SUCCESS != nRet) return nRet;
 //	加载的参数写入到日志文件中
	PrintParameter(); 
 //=======================================
 // 连接机器人，并返回连接值 	
    nRet = doConnect();
	return nRet;
}
/*=============================================================================
	 NAME: void Uninit()
	 ------------------------------------------------------------------------- 
	 Function:机器人对象反初始化函数
	 ------------------------------------------------------------------------- 
	 Parameter:
	 ------------------------------------------------------------------------- 
	 Return Value:	
 ----------------------------------------------------------------------------- 
	 Note:
	       断开机器人连接
=============================================================================*/
void CRobot::Uninit()
{
    disConnect();
}
/*=============================================================================
	 NAME: int LoadParameter()
	 -------------------------------------------------------------------------- 
	 Function:从配置文件加载参数
	 -------------------------------------------------------------------------- 
	 Parameter:
	 -------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ------------------------------------------------------------------------------ 
	 Note:
	        
=============================================================================*/
int CRobot::LoadParameter()
{
//=======================================================
// 临时变量初始化：初始化配置文件名
	CIni ini( ROBOT_PARA_NAME ); // 配置文件操作对象
// 如果配置文件中，配置项不存在，则返回错误
	if (m_strRobotName.empty())
	{
//		recordRobotLog("no m_strRobotName");
		
		return ROBOT_RETURN_ERRORROBOTNAME;
	}
//=======================================================
// 从配置文件加载配置信息初始化成员变量
    // 机器人IP地址
	m_strIP = ini.GetStr(m_strRobotName,"IP");

	//轨迹跟踪相关参数
	//传送带参数
    m_fBeltAngle = ini.GetFloat(m_strRobotName,"BeltAngle");
	
	m_fZSafeHeght = ini.GetFloat(m_strRobotName,"ZSafeHeght");
	
	if(m_fZSafeHeght > 0)
	{
	    m_fZSafeHeght = -10.0;
	}
	
    m_fBeltSpeed = ini.GetFloat(m_strRobotName,"BeltSpeed");
	if (m_fBeltSpeed == 0)m_fBeltSpeed = 200;
	
    m_fHa = ini.GetFloat(m_strRobotName,"Ha");
	if (m_fHa == 0)	m_fHa = 10;
		
    m_fSa = ini.GetFloat(m_strRobotName,"Sa");
	if (m_fSa == 0)	m_fSa = 20;
		
    m_fHf = ini.GetFloat(m_strRobotName,"Hf");
	if (m_fHf == 0)	m_fHf = 10;

    m_fSf = ini.GetFloat(m_strRobotName,"Sf");
	if (m_fSf == 0)	m_fSf = 20;

	m_nT1 = ini.GetInt(m_strRobotName,"T1");
	if (m_nT1 == 0)	m_nT1 = MIN_TIMELIMIT;

	m_nT2 = ini.GetInt(m_strRobotName,"T2");
	if (m_nT2 == 0)	m_nT2 = 200;

	m_nT3 = ini.GetInt(m_strRobotName,"T3");
	if (m_nT3 == 0)	m_nT3 = MIN_TIMELIMIT;

	m_nMoveTimeMinLimit = ini.GetInt(m_strRobotName,"MoveTimeMinLimit");
	if (m_nMoveTimeMinLimit < MIN_TIMELIMIT)m_nMoveTimeMinLimit = MIN_TIMELIMIT;

	m_nMoveTimeMaxLimit = ini.GetInt(m_strRobotName,"MoveTimeMaxLimit");
	if (m_nMoveTimeMaxLimit > MAX_TIMELIMIT)m_nMoveTimeMaxLimit = MAX_TIMELIMIT;

	m_nCheckPos = ini.GetInt(m_strRobotName,"CheckPos");

    m_nSimulation = ini.GetInt(m_strRobotName,"Simulation");
	
    m_fRobotSpeed = ini.GetFloat( m_strRobotName, "RobotSpeed" );
	if(m_fRobotSpeed < 0.1)
    {
        m_fRobotSpeed = 0.1;
    }

    m_nSpeedRate = ini.GetInt(m_strRobotName,"SpeedRate");
    if(m_nSpeedRate > 100)
    {
        m_nSpeedRate = 100;
    }
    if(m_nSpeedRate < 30)
    {
        m_nSpeedRate = 30;
    }

	m_nThreshold = ini.GetInt(m_strRobotName, "Threshold");

	if (m_nThreshold <= 0)
		m_nThreshold = 5;
	// 成功返回
    return UISDK_SUCCESS;
}

/*=============================================================================
     NAME: void SetSpeedRate(int nSpeedRate)
     --------------------------------------------------------------------------
     Function:设置机器人运动速度系数
     --------------------------------------------------------------------------
     Parameter:
            nSpeedRate[In]:速度系数
     --------------------------------------------------------------------------
     Return Value:
 ------------------------------------------------------------------------------
     Note:

=============================================================================*/
void CRobot::SetSpeedRate(int nSpeedRate)
{
    if(m_nSpeedRate > 100)
        m_nSpeedRate = 100;
    if(m_nSpeedRate < 10)
        m_nSpeedRate = 10;

    m_nSpeedRate = nSpeedRate;

    CIni ini( ROBOT_PARA_NAME ); // 配置文件操作对象
    ini.setSection(m_strRobotName);

    ini.write("SpeedRate", nSpeedRate);
}


/*=============================================================================
	 NAME: void PrintParameter()
	 -------------------------------------------------------------------------- 
	 Function:成员变量值写到日志文件中
	 -------------------------------------------------------------------------- 
	 Parameter:
	 -------------------------------------------------------------------------- 
	 Return Value:	
 ------------------------------------------------------------------------------ 
	 Note:
	        
=============================================================================*/
void CRobot::PrintParameter()
{
}
/*=============================================================================
	 NAME: int getRobotStatus()
	 -------------------------------------------------------------------------- 
	 Function:获取机器人状态
	 -------------------------------------------------------------------------- 
	 Parameter:
	         nRobotStatus[Out]:机器人状态
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ------------------------------------------------------------------------------ 
	 Note:	
	机器人状态
	   robotIdle = 0, // 空闲状态
	   robotMoving = 1, // 运动过程中
	   robotMotorErr = 2,// 电机错误：找到站点缺失，或是电机反馈错误
	   robotDisable = 3, // 电机脱机状态
	   robotUpdate = 4, // 控制器更新固件过程中
	   robotLocked = 5, // 机器人锁定：控制器出错锁定
	   robotFindHome = 6,  // 机器人正在找零点
	   robotErrorHand = 7  // 机器人左右手不正确
=============================================================================*/
int CRobot::getRobotStatus(int *nRobotStatus)
{
    return GetRobotStatus(dwConnectCode, nRobotStatus);
}
/*=============================================================================
	 NAME: int CurveMoveTo()
	 ---------------------------------------------------------------------------
	 Function:机器人曲线运动：标准运动中，去掉启动腿和停止腿
	 ---------------------------------------------------------------------------
	 Parameter:
	         CoordTo[In]:终点坐标
	         nMoveTime[In]: 运动时间，单位ms
	         fHgt[In]:曲线运动中间高度，单位mm
	 ---------------------------------------------------------------------------
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 -------------------------------------------------------------------------------
	 Note:
=============================================================================*/
int CRobot::CurveMoveTo(RobotCoordinate Coord, int nMoveTime, float fHgt /*= 20*/)
{
	SPPMoveParam moveParam;
	moveParam.Lg1 = 0;
	moveParam.Lg2 = 0;
	moveParam.Hgt = fHgt;
	moveParam.V1 = 0;
	moveParam.V2 = nMoveTime;
	moveParam.V3 = 0;	
	return SPPMoveTo(Coord, moveParam);
}
/*=============================================================================
     NAME: int CurveToMoveTo(RobotCoordinate CoordMid,RobotCoordinate CoordTo, int nMoveTime, bool bWaitFlag)
	 -------------------------------------------------------------------------- 
	 Function:机器人直线运动带中间点 
	 -------------------------------------------------------------------------- 
	 Parameter:
	         CoordMid[In]:中间点坐标
	         CoordTo[In]:终点坐标
	         nMoveTime[In]: 运动时间，单位ms
             bWaitFlag[In]:等待标志
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::CurveToMoveTo(RobotCoordinate CoordMid,RobotCoordinate CoordTo, int nMoveTime, bool bWaitFlag)
{
    if (0 != m_nSimulation)
    {
        CoordCurrentPosition = CoordTo;
        m_bPVTMoveOK = true;
        return UISDK_SUCCESS;
    }
//==========================================
// 出现RTCN错误，机器人不允许再次运行
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
    if((CoordMid.fTCPx == CoordTo.fTCPx) && (CoordMid.fTCPy == CoordTo.fTCPy)
       && (CoordMid.fTCPz == CoordTo.fTCPz)&& (CoordMid.fTCPRz == CoordTo.fTCPRz))
    {
        return ROBOT_RETURN_ERROR_PARAMETER;
    }
    CoordDestinationPosition = CoordTo;
    m_nMoveTime = nMoveTime*100/m_nSpeedRate + 1000;
	
//=======================================================
 // 局部变量初始化
	XMLNodePoint MoveNodePoint[2];// 运动参数节点信息：目前的运动类型最多2个节点
	int iNodePointIndex = 0;      // 节点序号，从0开始
	int iRetValue;                 // 函数返回值 
	
//=======================================================
// 根节点配置信息
	MoveNodePoint[iNodePointIndex].nodeType = ROBOTMOVE;
	MoveNodePoint[iNodePointIndex].nodeData.LineTomoveParameters.subMoveType = CurveToMove;
	MoveNodePoint[iNodePointIndex].nodeData.LineTomoveParameters.Tm = (float)nMoveTime;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
    MoveNodePoint[iNodePointIndex].speedRate = m_nSpeedRate;
	MoveNodePoint[iNodePointIndex].counts = 2;	
	iNodePointIndex++;
//=======================================================
// 中间点坐标参数
	// node 1	
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordMid.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordMid.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordMid.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordMid.fTCPRz;
	iNodePointIndex++;	
//=======================================================
	// 终止点坐标参数
	// node 2	
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordTo.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordTo.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordTo.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordTo.fTCPRz;
	iNodePointIndex++;	
//=======================================================
	// 判断机器人状态
	int iRobotStatus  = 0;
	iRetValue = getRobotStatus(&iRobotStatus);
	if (UISDK_SUCCESS != iRetValue)
	{
//		recordRobotLog("getRobotStatus Error %d", iRetValue);
		return iRetValue;
	}
	else
	{
		if (iRobotStatus != robotIdle)
		{
			RecordRobotStatusByCode(iRobotStatus);
			return ROBOT_RETURN_ERRORSTATS;
		}
	}
//=======================================================
// 清除机器人运动到位消息标记位，该标记位将在RTCN中进行赋值
	m_bPVTMoveOK = false;

// 发送运动指令
	iRetValue = SetRobotMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
	if (UISDK_SUCCESS != iRetValue)
	{
		// 区分算法错误，还是其它错误：算法错误则输出-301等值
// 		if (((iRetValue >> 28) & 0x0F) == 0x0F)
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		else
// 			recordRobotLog("SetRobotMove failed 0x%X", iRetValue);
		
		return iRetValue;
	}
	else
	{
//===========================================
// 等待运动结束标记，并且判断是否超时
        if (!bWaitFlag)
        {
            return UISDK_SUCCESS;
        }

        if (!WaitRobotDone(0))
		{
			//recordRobotLog("WaitFlag SetRobotMove TimeOut.");
			
			return ROBOT_RETURN_TIMEOUT;
		}
	}
//===========================================
// RTCN返回失败，将错误码返回函数
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
	//===========================================
	// 机器人当前坐标位置保存到成员变量中
    CoordCurrentPosition = CoordTo;
	//===========================================
	// 成功返回
	return UISDK_SUCCESS;
}

/*=============================================================================
     NAME: int LineMoveTo(RobotCoordinate CoordTo, int nMoveTime, bool bWaitFlag)
	 -------------------------------------------------------------------------- 
	 Function:机器人直线运动 
	 -------------------------------------------------------------------------- 
	 Parameter:
	         CoordTo[In]:终点坐标
	         nMoveTime[In]: 运动时间，单位ms
             bWaitFlag[In]:等待标志
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::LineMoveTo(RobotCoordinate CoordTo, int nMoveTime, bool bWaitFlag)
{
    if (0 != m_nSimulation)
    {
        CoordCurrentPosition = CoordTo;

        m_bPVTMoveOK = true;

        MySleep(nMoveTime * 100 / m_nSpeedRate);

        return UISDK_SUCCESS;
    }
//==========================================
// 出现RTCN错误，机器人不允许再次运行
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}

    CoordDestinationPosition = CoordTo;
    m_nMoveTime = nMoveTime*100/m_nSpeedRate + 5000;
	
//=======================================================
 // 局部变量初始化
	XMLNodePoint MoveNodePoint[2];// 运动参数节点信息：目前的运动类型最多2个节点
	int iNodePointIndex = 0;      // 节点序号，从0开始
	int iRetValue;                 // 函数返回值 
	
//=======================================================
// 根节点配置信息
	MoveNodePoint[iNodePointIndex].nodeType = ROBOTMOVE;
	MoveNodePoint[iNodePointIndex].nodeData.LineTomoveParameters.subMoveType = LineToMove;
	MoveNodePoint[iNodePointIndex].nodeData.LineTomoveParameters.Tm = (float)nMoveTime;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
    MoveNodePoint[iNodePointIndex].speedRate = m_nSpeedRate;
	MoveNodePoint[iNodePointIndex].counts = 1;	
//=======================================================
// 终止点坐标参数
	iNodePointIndex++;
	// node 1	
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordTo.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordTo.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordTo.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordTo.fTCPRz;
	iNodePointIndex++;	
//=======================================================
	// 判断机器人状态
	int iRobotStatus  = 0;
	iRetValue = getRobotStatus(&iRobotStatus);
	if (UISDK_SUCCESS != iRetValue)
	{
	//	recordRobotLog("getRobotStatus Error %d", iRetValue);
		return iRetValue;
	}
	else
	{
		if (iRobotStatus != robotIdle)
		{
			RecordRobotStatusByCode(iRobotStatus);
			return ROBOT_RETURN_ERRORSTATS;
		}
	}
//=======================================================
// 清除机器人运动到位消息标记位，该标记位将在RTCN中进行赋值
	m_bPVTMoveOK = false;

// 发送运动指令
	iRetValue = SetRobotMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
	if (UISDK_SUCCESS != iRetValue)
	{
		// 区分算法错误，还是其它错误：算法错误则输出-301等值
// 		if (((iRetValue >> 28) & 0x0F) == 0x0F)
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		else
// 			recordRobotLog("SetRobotMove failed 0x%X", iRetValue);
// 		
		return iRetValue;
	}
	else
	{
//===========================================
// 等待运动结束标记，并且判断是否超时
        if (!bWaitFlag)
        {
            return UISDK_SUCCESS;
        }

        if (!WaitRobotDone(0))
		{
            RobotCoordinate CoordTemp;
            iRetValue = GetRobotCoor(CoordTemp);

            if (UISDK_SUCCESS != iRetValue)
            {
//                recordRobotLog("GetRobotCoor failed 0x%x", iRetValue);
                return iRetValue;
            }
            else
            {
//                recordDeviceLog(DEBUG_LEVEL_DEBUG, "[%s] GetRobotCoor (%f, %f, %f, %f)-(%d).", __FUNCTION__,CoordTemp.fTCPx, CoordTemp.fTCPy, CoordTemp.fTCPz, CoordTemp.fTCPRz, nMoveTime);
            }

//			recordRobotLog("WaitFlag SetRobotMove TimeOut.");
			
			return ROBOT_RETURN_TIMEOUT;
		}
	}
//===========================================
// RTCN返回失败，将错误码返回函数
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
	//===========================================
	// 机器人当前坐标位置保存到成员变量中
    CoordCurrentPosition = CoordTo;
	//===========================================
	// 成功返回
	return UISDK_SUCCESS;
}
/*=============================================================================
     NAME: int CPPMoveTo(RobotCoordinate CoordMid,RobotCoordinate CoordTo, SPPMoveParam moveParam, bool bWaitFlag)
	 -------------------------------------------------------------------------- 
	 Function:机器人标准运动带中间点
	 -------------------------------------------------------------------------- 
	 Parameter:
	         CoordMid[In]:中间点坐标
	         CoordTo[In]:终点坐标
			 Hgt[In]: 中间曲线拱高，拱顶 - 终止腿上端点 之间距离,单位mm
	 		 Lg1[In]: 启动腿长度，单位mm
	 		 Lg2[In]: 终点腿长度，单位mm
	 		 V1[In]: 启动腿运动时间，单位ms
			 V2[In]: 中间曲线运动时间，单位ms
	 		 V3[In]: 终点腿运动时间，单位ms
             bWaitFlag[In]:等待标志
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::CPPMoveTo(RobotCoordinate CoordMid,RobotCoordinate CoordTo, SPPMoveParam moveParam, bool bWaitFlag)
{
    if (0 != m_nSimulation)
    {
        CoordCurrentPosition = CoordTo;

        m_bPVTMoveOK = true;

        MySleep((moveParam.V1 + moveParam.V2 + moveParam.V3) * 100 / m_nSpeedRate);

        return UISDK_SUCCESS;
    }
//==========================================
// 出现RTCN错误，机器人不允许再次运行
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
    if((CoordMid.fTCPx == CoordTo.fTCPx) && (CoordMid.fTCPy == CoordTo.fTCPy)
       && (CoordMid.fTCPz == CoordTo.fTCPz)&& (CoordMid.fTCPRz == CoordTo.fTCPRz))
    {
        return ROBOT_RETURN_ERROR_PARAMETER;
    }
    CoordDestinationPosition = CoordTo;
    m_nMoveTime = (moveParam.V1 + moveParam.V2 + moveParam.V3) * 100 / m_nSpeedRate + 1000;
	
 //===========================================
 // 当前坐标位置
   // UI_INITIALIZETCP_OBJ nowTcp = {  };
	RobotCoordinate CoordFrom;
    XMLNodePoint MoveNodePoint[4];
	int iNodePointIndex = 0;
	int iRetValue;
//==========================================
// 获取当前坐标
	iRetValue = GetRobotCoor(CoordFrom);
	if (UISDK_SUCCESS != iRetValue)
	{
		return iRetValue;
	}

// 根节点配置信息
	MoveNodePoint[iNodePointIndex].nodeType = ROBOTMOVE;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.subMoveType = CPPMove;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
    MoveNodePoint[iNodePointIndex].speedRate = m_nSpeedRate;
	MoveNodePoint[iNodePointIndex].counts = 3;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.tcpIndex = 0;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Lg1 = (float)moveParam.Lg1;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Lg2 = (float)moveParam.Lg2;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Hgt = (float)moveParam.Hgt;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.iStartSpeed = (int)moveParam.V1;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.iEndSpeed = (int)moveParam.V3;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.itrjV0 = 0;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.iMoveSpeed = (int)moveParam.V2;
	iNodePointIndex++;
	// node 1
// 起腿坐标参数
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordFrom.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordFrom.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordFrom.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordFrom.fTCPRz;
	iNodePointIndex++;
	
	// node 2
// 中间点坐标参数
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordMid.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordMid.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordMid.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordMid.fTCPRz;
	iNodePointIndex++;
// 终点坐标参数
	// node 2	
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordTo.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordTo.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordTo.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordTo.fTCPRz;

	iNodePointIndex++;

	
// 判断机器人状态
	int iRobotStatus  = 0;
	iRetValue = getRobotStatus(&iRobotStatus);
	if (UISDK_SUCCESS != iRetValue)
	{
//		recordRobotLog("getRobotStatus Error %d", iRetValue);
		return iRetValue;
	}
	else
	{
// 如果机器人状态不是空闲状态，则状态出错返回
		if (iRobotStatus != robotIdle)
		{
			RecordRobotStatusByCode(iRobotStatus);
			return ROBOT_RETURN_ERRORSTATS;
		}
	}
//===========================================
// 清除机器人运动到位消息标记位，该标记位将在RTCN中进行赋值
	m_bPVTMoveOK = false;

// 发送运动指令
	iRetValue = SetRobotMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
	if (UISDK_SUCCESS != iRetValue)
	{
// 		if (((iRetValue >> 28) & 0x0F) == 0x0F)
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		else
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		
		return iRetValue;
	}
	else
	{
        if(bWaitFlag)
        {
            // 等待运动结束标记，并且判断是否超时
            if (!WaitRobotDone(0))
            {
//                recordRobotLog("WaitFlag SetRobotMove TimeOut.");

                return ROBOT_RETURN_TIMEOUT;
            }
        }
	}
	
    //===========================================
    // RTCN返回失败，将错误码返回函数
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
	
	//===========================================
	// 机器人当前坐标位置保存到成员变量中
    CoordCurrentPosition = CoordTo;
	//===========================================
	// 成功返回
	return UISDK_SUCCESS;
}

/*=============================================================================
     NAME: int SPPMoveTo(RobotCoordinate CoordTo, SPPMoveParam moveParam, bool bWaitFlag)
	 -------------------------------------------------------------------------- 
	 Function:机器人标准运动 
	 -------------------------------------------------------------------------- 
	 Parameter:
	         CoordTo[In]:终点坐标
			 Hgt[In]: 中间曲线拱高，拱顶 - 终止腿上端点 之间距离,单位mm
	 		 Lg1[In]: 启动腿长度，单位mm
	 		 Lg2[In]: 终点腿长度，单位mm
	 		 V1[In]: 启动腿运动时间，单位ms
			 V2[In]: 中间曲线运动时间，单位ms
	 		 V3[In]: 终点腿运动时间，单位ms
             bWaitFlag[In]:等待标志
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::SPPMoveTo(RobotCoordinate CoordTo, SPPMoveParam moveParam, bool bWaitFlag)
{
    if (0 != m_nSimulation)
    {
        CoordCurrentPosition = CoordTo;

        m_bPVTMoveOK = true;

        MySleep((moveParam.V1 + moveParam.V2 + moveParam.V3) * 100 / m_nSpeedRate);

        return UISDK_SUCCESS;
    }
//==========================================
// 出现RTCN错误，机器人不允许再次运行
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}

    CoordDestinationPosition = CoordTo;
    m_nMoveTime = (moveParam.V1 + moveParam.V2 + moveParam.V3) * 100 / m_nSpeedRate + 1000;
	
 //===========================================
 // 当前坐标位置
   // UI_INITIALIZETCP_OBJ nowTcp = {  };
	RobotCoordinate CoordFrom;
	XMLNodePoint MoveNodePoint[3];
	int iNodePointIndex = 0;
	int iRetValue;
//==========================================
// 获取当前坐标
	iRetValue = GetRobotCoor(CoordFrom);
	if (UISDK_SUCCESS != iRetValue)
	{
		return iRetValue;
	}

// 根节点配置信息
	MoveNodePoint[iNodePointIndex].nodeType = ROBOTMOVE;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.subMoveType = SPPMove;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
    MoveNodePoint[iNodePointIndex].speedRate = m_nSpeedRate;
	MoveNodePoint[iNodePointIndex].counts = 2;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.tcpIndex = 0;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Lg1 = (float)moveParam.Lg1;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Lg2 = (float)moveParam.Lg2;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Hgt = (float)moveParam.Hgt;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.iStartSpeed = (int)moveParam.V1;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.iEndSpeed = (int)moveParam.V3;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.itrjV0 = 0;
	MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.iMoveSpeed = (int)moveParam.V2;
	iNodePointIndex++;
	// node 1
// 起腿坐标参数
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordFrom.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordFrom.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordFrom.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordFrom.fTCPRz;
	iNodePointIndex++;
// 终点坐标参数
	// node 2	
	MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = CoordTo.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = CoordTo.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = CoordTo.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = CoordTo.fTCPRz;

	iNodePointIndex++;

	
// 判断机器人状态
	int iRobotStatus  = 0;
	iRetValue = getRobotStatus(&iRobotStatus);
	if (UISDK_SUCCESS != iRetValue)
	{
//		recordRobotLog("getRobotStatus Error %d", iRetValue);
		return iRetValue;
	}
	else
	{
// 如果机器人状态不是空闲状态，则状态出错返回
		if (iRobotStatus != robotIdle)
		{
			RecordRobotStatusByCode(iRobotStatus);
			return ROBOT_RETURN_ERRORSTATS;
		}
	}
//===========================================
// 清除机器人运动到位消息标记位，该标记位将在RTCN中进行赋值
	m_bPVTMoveOK = false;

// 发送运动指令
	iRetValue = SetRobotMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
	if (UISDK_SUCCESS != iRetValue)
	{
// 		if (((iRetValue >> 28) & 0x0F) == 0x0F)
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		else
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		
		return iRetValue;
	}
	else
	{
        if(bWaitFlag)
        {
            // 等待运动结束标记，并且判断是否超时
            if (!WaitRobotDone(0))
            {
//                recordRobotLog("WaitFlag SetRobotMove TimeOut.");

                return ROBOT_RETURN_TIMEOUT;
            }
        }
	}
	
    //===========================================
    // RTCN返回失败，将错误码返回函数
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
	
	//===========================================
	// 机器人当前坐标位置保存到成员变量中
    CoordCurrentPosition = CoordTo;
	//===========================================
	// 成功返回
	return UISDK_SUCCESS;
}

bool CRobot::WaitRobotDone(int nTime)
{
    if (!WaitFlag(&m_bPVTMoveOK, m_nMoveTime - nTime))
    {
        return false;
    }

    return true;
}


void CRobot::OnPraseErrorMsg(int des)
{
	m_ErrorParse.PraseErrorCode(des);

#if 0
	switch (des)
	{
		case ERROR_UNKNOWN_FAILED:
			recordRobotLog("unknown error");
			break;
		case RTCN_SYS_ERROR:
			recordRobotLog("RTCN_SYS_ERROR");
			break;
		case ERROR_PVTWRITE_FAILED:
			recordRobotLog("Failed to write to PVT");
			break;
		case ERROR_URGENT_STOP_FAILED:
			recordRobotLog("Emergency stop");
			break;
		case ERROR_GETPVT_FAILED:
			recordRobotLog("Unable to generate PVT list");
			break;
		case ERROR_CAMERAOBJECT_NOTFOUND_FAILED:
			recordRobotLog("Robot program is not correct");
			break;
		case ERROR_ROBOT_STATE_FAILED:
			recordRobotLog("Robot status is not correct");
			break;
		case ERROR_ROBOT_COORDINATE_FAILED:
			recordRobotLog("Coordinate error");
			break;
		case ERROR_ROBOT_TRANSMODEL_STATE_FAILED:
			recordRobotLog("Transmission mode");
			break;
		case ERROR_ROBOT_FILE_NOT_EXIST_FAILED:
			recordRobotLog("file does not exist");
			break;
		case ERROR_ROBOT_FILE_NOT_PROGRAM_FAILED:
			recordRobotLog("Not a robot program");
			break;
		case ERROR_ROBOT_FILE_OPEN_FAILED:
			recordRobotLog("File open error");
			break;
		case ERROR_ROBOT_CAN_BUS_FAILED:
			recordRobotLog("Controller communication errorr");
			break;
		case ERROR_MOTOR_DISABLE_FAILED:
			recordRobotLog("OffLine Failed");
			break;
		case ERROR_ROBOT_FILE_SIZE_FAILED:
			recordRobotLog("File too large");
			break;
		case ERROR_ROBOT_MALLOC_MEMORY_FAILED:
			recordRobotLog("Application memory error");
			break;
		case ERROR_MOTOR_SOFTSAFE_ERR:
			recordRobotLog("Exceed software limitr");
			break;
		case RTCN_ERROR_COMMAND:
			recordRobotLog("Instruction syntax error");
			break;
		case RTCN_ERROR_PARAMETER:
			recordRobotLog("Instruction parameter error");
			break;
		case RTCN_ERROR_COMMANDINDEX:
			recordRobotLog("Command index (subscript) error");
			break;
		case RTCN_ERROR_MOVING:
			recordRobotLog("When the motor rotates, attempt to set the system interval");
			break;
		case RTCN_ERROR_SD:
			recordRobotLog("SD minus speed is less than DC");
			break;
		case RTCN_ERROR_ORIGNAL:
			recordRobotLog("When the motor rotates, attempt to set the absolute value encoder origin");
			break;
		case RTCN_ERROR_PVTSTART:
			recordRobotLog("Motor movement, attempting to set the starting point of PV motion");
			break;
		case RTCN_ERROR_PVTINDEX:
			recordRobotLog("QP/QV/QT subject code is greater than MP[6] (water level)");
			break;
		case RTCN_ERROR_PVTPORTFUNCTION:
			recordRobotLog("Conflict with IO port function settings");
			break;
		case RTCN_ERROR_PORTFUNCTION:
			recordRobotLog("Port configuration error");
			break;
		case ERROR_PARA_POINT_NULL:
			recordRobotLog("ERROR_PARA_POINT_NULL");
			break;
		case ERROR_PARA_TRJTM_OVER:
			recordRobotLog("ERROR_PARA_TRJTM_OVER");
			break;
		case ERROR_PARA_SPEED_ZERO:
			recordRobotLog("ERROR_PARA_SPEED_ZERO");
			break;
		case ERROR_BELT_SPEED_ZERO:
			recordRobotLog("ERROR_BELT_SPEED_ZERO");
			break;
		case ERROR_POSI_LIMIT_XYLH:
			recordRobotLog("ERROR_POSI_LIMIT_XYLH");
			break;
		case ERROR_POSI_LIMIT_XYLL:
			recordRobotLog("ERROR_POSI_LIMIT_XYLL");
			break;
		case ERROR_POSI_LIMIT_PZLH:
			recordRobotLog("ERROR_POSI_LIMIT_PZLH");
			break;
		case ERROR_POSI_LIMIT_PZLL:
			recordRobotLog("ERROR_POSI_LIMIT_PZLL");
			break;
		case ERROR_POSI_LIMIT_PRLH:
			recordRobotLog("ERROR_POSI_LIMIT_PRLH");
			break;
		case ERROR_POSI_LIMIT_PRLL:
			recordRobotLog("ERROR_POSI_LIMIT_PRLL");
			break;
		default:
			break;
	}
#endif	
}
/*=============================================================================
	 NAME: int RobotRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen)
	 -------------------------------------------------------------------------- 
	 Function:机器人RTCN回调函数
	 -------------------------------------------------------------------------- 
	 Parameter:
	         dwConnectType[In]:机器人连接字
	         cmd_type[In]: 消息类型
	         pMessageBuffer[In]: 消息字符串
			 iDataLen[In]: 消息长度
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
void CRobot::RobotRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen)
{
// 机器人反馈的消息处理
//============================================
// iDataLen消息所带数据长度，长度大于7表示错误返回
	if (iDataLen >= 13)
	{
		string strMsg = pMessageBuffer;
		// 返回的字符串：0|XX|YYYYYYYY,其中XX表示10进制表示错误类型，YYYYYYYY表示16进制表示错误码

		string strErr = strMsg.substr(5, 8);
		m_nErrnoFromRTCN = hex2dec(strErr.c_str());
//		recordRobotLog(pMessageBuffer);
		OnPraseErrorMsg(m_nErrnoFromRTCN);
		return;
	}
	switch (cmd_type)
	{
// 断开连接消息
		case UICMDSOCKETDISCONNECTED:
		{
			if (dwConnectType == UICONNECT_IP)
			{
			}
		}
		break;
// 紧急停止消息
		case   UICMDEMERGYSTOP:
		{
			if (pMessageBuffer[0] == '1')
			{
//				recordRobotLog("UICMDEMERGYSTOP");
			}
			else
			{

			}
		}
		break;
// 运动到位消息
		case UI_CMD_SET_ROBOTMOVE:
		{
			if (pMessageBuffer[0] == '1')
			{
//                recordDeviceLog(DEBUG_LEVEL_DEBUG, "[%s] Robot RTCN come.", __FUNCTION__);

				m_bPVTMoveOK = true;
			}
			else
			{
//				recordRobotLog("run PVT coord fail");
			}
		}
		break;
// 输入端口电平跳变
		case UICMDGETIOINPUTVALUE:
		{
           //   int iPort = pMessageBuffer[0]; // 端口序号，下标从1开始
            //  int iPortValue = pMessageBuffer[1];
			//  recordRobotLog("iPort=%d,iPortValue=%d",iPort,iPortValue);
		}
		break;
		case UICMDREALTIMENOTIFICATION:
		{

		}
		break;
	}
}
/*=============================================================================
	 NAME: bool CheckIsOnePos(RobotCoordinate pos1, RobotCoordinate pos2)
	 -------------------------------------------------------------------------- 
	 Function:检测两个坐标点是否相同
	 -------------------------------------------------------------------------- 
	 Parameter:
	         pos1[In]:机器人坐标1
	         pos2[In]:机器人坐标2
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		返回坐标是否相同
 ------------------------------------------------------------------------------ 
	 Note:	        
=============================================================================*/
bool CRobot::CheckIsOnePos(RobotCoordinate pos1, RobotCoordinate pos2)
{
//==============================================================
// 初始化临时变量
// 坐标相差距离长度阈值，单位mm，如果大于这个距离表示两个坐标不同；小于这个距离，表示相同坐标
   // int nThreshold = 5;// 坐标相差距离长度阈值
//==============================================================
// 判断坐标是否大于阈值
    bool arrived = (abs(pos1.fTCPx - pos2.fTCPx) < m_nThreshold)
        && (abs(pos1.fTCPy - pos2.fTCPy) < m_nThreshold)
        && (abs(pos1.fTCPz - pos2.fTCPz) < m_nThreshold)
        && (abs(pos1.fTCPRz - pos2.fTCPRz) < m_nThreshold);
//==============================================================
// 返回是否相同		
	return arrived;
}
/*=============================================================================
	 NAME: bool WaitFlag(bool *pBFlag, unsigned int dwTimeOutTime)
	 -------------------------------------------------------------------------- 
	 Function:在指定时间内等待标志位
	 -------------------------------------------------------------------------- 
	 Parameter:
	         pBFlag[In]:机器人坐标1
	         dwTimeOutTime[In]:超时时间，单位ms
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		返回标志位是滞被置位
 ------------------------------------------------------------------------------ 
	 Note:	        
=============================================================================*/
bool CRobot::WaitFlag(bool *pBFlag, unsigned int dwTimeOutTime)
{
//==============================================================
// 初始化临时变量
	bool bRunOK = false; // 是否标志位已经被置位
	unsigned int CurrTimeOut = 0;// 当前已经等待时间，单位ms
//==============================================================
// 等待标志位被置位，该标志位在回调函数RobotRevRtcnNotify中被修改置位
    if(0 == dwTimeOutTime)
    {
        if (*pBFlag)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

	while (CurrTimeOut < dwTimeOutTime)
	{
		if (*pBFlag)
		{
			bRunOK = true;
			break;
		}
		MySleep(10);
		CurrTimeOut += 10;
	}
//==============================================================
// 清除标志位 
	*pBFlag = false;
//==============================================================
// 返回是否标志位被置位，如果没置位，则表示等待超时
	return bRunOK;
}
/*=============================================================================
	 NAME: void SetTrjMotion6PFMotionPara(PFMoveParam PFParam)
	 -------------------------------------------------------------------------- 
	 Function:设置传送带6点跟随运动参数
	 -------------------------------------------------------------------------- 
	 Parameter:	 
			 belt.angle[In]:传送带流向与机器人 X+ 方向矢量夹角（度，有正负）
			 belt.speed[In]:跟随速度 = 传输带流速, mm/s
	         Ha[In]:Height,Aproaching，机器人准备进入抓取点运动的高度，单位mm
	         Sa[In]:Distance,Aproaching机器人准备进入抓取点运动的距离，单位mm
			 Hf[In]:Height,Taking off机器人传送带跟随结束后，离开传送带的高度，单位mm
			 Sf[In]:Distance,Taking off机器人传送带跟随结束后，离开传送带的距离，单位mm
			 T1[In]:机器人从起始点到抓取点的运动时间，单位ms
			 T2[In]:机器人传送带跟随的运动时间，单位ms
			 T3[In]:机器人传送带跟随结束后，向终止点坐标运动时间，单位ms
	 -------------------------------------------------------------------------- 
	 Return Value:		 
		  
 ------------------------------------------------------------------------------ 
	 Note:	    
	       修改机器人送带6点跟随运动参数
=============================================================================*/
void CRobot::SetTrjMotion6PFMotionPara(PFMoveParam PFParam)
{
	m_fBeltAngle = (float)PFParam.belt.angle; // 传送带运动方向与机器人X正方向的角度，单位度
	m_fBeltSpeed = (float)PFParam.belt.speed;  // 传送带运动速度，单位mm/s
	m_fHa = (float)PFParam.Ha; // 机器人准备进入抓取点运动的高度，单位mm
	m_fSa = (float)PFParam.Sa; // 机器人准备进入抓取点运动的距离，单位mm
	m_fHf = (float)PFParam.Hf; // 机器人传送带跟随结束后，离开传送带的高度，单位mm
	m_fSf = (float)PFParam.Sf; // 机器人传送带跟随结束后，离开传送带的距离，单位mm
	m_nT1 = (int)PFParam.T1; // 机器人从起始点到抓取点的运动时间，单位ms
	m_nT2 = (int)PFParam.T2; // 机器人传送带跟随的运动时间，单位ms
	m_nT3 = (int)PFParam.T3; // 机器人传送带跟随结束后，向终止点坐标运动时间，单位ms
}
/*=============================================================================
	 NAME: void TrjMotion6PF(RobotCoordinate BeginPoint, RobotCoordinate GroundPoint, RobotCoordinate EndPoint, bool bWaitFlag, int nDefautlSPDRate)
	 -------------------------------------------------------------------------- 
	 Function:向机器人发送传送带6点跟随运动参数，机器人将按参数设置进行运动
	 -------------------------------------------------------------------------- 
	 Parameter:
	         BeginPoint[In]:传送带6点跟随运动起始坐标
	         GroundPoint[In]:传送带6点跟随运动抓取坐标
			 EndPoint[In]:传送带6点跟随运动结束坐标
			 bWaitFlag[In]:是否需要等待机器人运动结束标志位
	 -------------------------------------------------------------------------- 
	 Return Value:		 
			 If the function fails, the return value is error code 
			 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ------------------------------------------------------------------------------ 
	 Note:	    
	        
=============================================================================*/
int CRobot::TrjMotion6PF(RobotCoordinate BeginPoint, RobotCoordinate GroundPoint, RobotCoordinate EndPoint, bool bWaitFlag /*= true*/)
{
    if (0 != m_nSimulation)
    {
        CoordCurrentPosition = EndPoint;
        m_bPVTMoveOK = true;
        return UISDK_SUCCESS;
    }
//==========================================
// 出现RTCN错误，机器人不允许再次运行
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
    }

//===========================================
 // 局部变量初始化
	XMLNodePoint MoveNodePoint[4]; // 运动参数节点信息：目前的运动类型最多4个节点
	int iNodePointIndex = 0;       // 节点序号，从0开始
	int nTotalMoveTime;
	int iRetValue;                 // 函数返回值 
	//CalcRunTime(BeginPoint, GroundPoint, EndPoint);
	//recordRobotLog("RunT1 %d", m_nRunT1);
	//recordRobotLog("T2 %d", m_nT2);
	//recordRobotLog("RunT3 %d", m_nRunT3);
	nTotalMoveTime = GetTotalTime()*100/m_nSpeedRate;

    CoordDestinationPosition = EndPoint;
    m_nMoveTime = nTotalMoveTime + 1000;
//===========================================
// 根节点配置信息
	iNodePointIndex = 0;

	MoveNodePoint[iNodePointIndex].nodeType = ROBOTMOVE;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.subMoveType = Tracking6PfMove;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
    MoveNodePoint[iNodePointIndex].speedRate = m_nSpeedRate;
	MoveNodePoint[iNodePointIndex].counts = 3;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.tcpIndex = 0;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.beltangel = m_fBeltAngle;// -90;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.beltspeed = m_fBeltSpeed;// 200;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.Ha = m_fHa;// 10; //h1
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.Hf = m_fHf;// 10; //h2

	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.Sa = m_fSa;// 20; //s1
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.Sf = m_fSf;// 20; //s3
	//T1,T3经过动态计算 ,T2为固定时间
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.T1 = m_nT1;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.T2 = m_nT2;
	MoveNodePoint[iNodePointIndex].nodeData.TKGPickPlaceParameters.T3 = m_nT3;

//	recordRobotLog("TrjMotion6PF BeltAngle:%f,BeltSpeed:%f,Ha:%f,Hf:%f,Sa:%f,Sf:%f,T1:%d,T2:%d,T3:%d",
	//	m_fBeltAngle, m_fBeltSpeed, m_fHa, m_fHf, m_fSa, m_fSf, m_nT1, m_nT2, m_nT3);
//===========================================
// 起始点坐标参数
	iNodePointIndex++;

	// BeginPoint
	MoveNodePoint[iNodePointIndex].counts = 0;
    MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = BeginPoint.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = BeginPoint.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = BeginPoint.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = BeginPoint.fTCPRz;
//===========================================
// 抓取点坐标参数
	iNodePointIndex++;

	// GroundPoint
	MoveNodePoint[iNodePointIndex].counts = 0;
    MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = GroundPoint.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = GroundPoint.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = GroundPoint.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = GroundPoint.fTCPRz;
//===========================================
// 终止点坐标参数
	iNodePointIndex++;

	// EndPoint
	MoveNodePoint[iNodePointIndex].counts = 0;
    MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
	memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = EndPoint.fTCPx;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = EndPoint.fTCPy;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = EndPoint.fTCPz;
	MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = EndPoint.fTCPRz;
	iNodePointIndex++;

	
//===========================================
	// 判断机器人状态
	int iRobotStatus  = 0;
//	recordRobotLog("BeginPoint x:%f,y:%f, z:%f", BeginPoint.fTCPx, BeginPoint.fTCPy, BeginPoint.fTCPz);
//	recordRobotLog("GroundPoint x:%f,y:%f,z:%f", GroundPoint.fTCPx, GroundPoint.fTCPy, GroundPoint.fTCPz);
//	recordRobotLog("EndPoint x:%f,y:%f,z:%f", EndPoint.fTCPx, EndPoint.fTCPy, EndPoint.fTCPz);

	iRetValue = getRobotStatus(&iRobotStatus);
	if (UISDK_SUCCESS != iRetValue)
	{
//		recordRobotLog("getRobotStatus Error %d", iRetValue);
		return iRetValue;
	}
	else
	{
// 如果机器人状态不是空闲状态，则状态出错返回
		if (iRobotStatus != robotIdle)
		{
			RecordRobotStatusByCode(iRobotStatus);
			return ROBOT_RETURN_ERRORSTATS;
		}
	}

//===========================================
// 清除机器人运动到位消息标记位，该标记位将在RTCN中进行赋值
	m_bPVTMoveOK = false;

// 发送运动指令
//	iRetValue = GetRobotSMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
	//RobotAsxMax RobotMax;
	//iRetValue = GetRobotMoveTM(dwConnectCode, MoveNodePoint, iNodePointIndex, &RobotMax);
	iRetValue = SetRobotMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
	if (UISDK_SUCCESS != iRetValue)
	{
		// 区分算法错误，还是其它错误：算法错误则输出-301等值
// 		if (((iRetValue >> 28) & 0x0F) == 0x0F)
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
// 		else
// 			recordRobotLog("SetRobotMove failed 0x%x", iRetValue);
        unsigned int iRobotErr = (unsigned int)iRetValue;
        switch (iRobotErr)
		{
		case ERROR_POSI_LIMIT_XYLH:
//			recordRobotLog("ERROR_POSI_LIMIT_XYLH");
			break;
		case ERROR_POSI_LIMIT_XYLL:
//			recordRobotLog("ERROR_POSI_LIMIT_XYLL");
			break;
		case ERROR_POSI_LIMIT_PZLH:
//			recordRobotLog("ERROR_POSI_LIMIT_PZLH");
			break;
		case ERROR_POSI_LIMIT_PZLL:
//			recordRobotLog("ERROR_POSI_LIMIT_PZLL");
			break;
		case ERROR_POSI_LIMIT_PRLH:
//			recordRobotLog("ERROR_POSI_LIMIT_PRLH");
			break;
		case ERROR_POSI_LIMIT_PRLL:
//			recordRobotLog("ERROR_POSI_LIMIT_PRLL");
			break;
		default:
//			recordRobotLog("UN PRAS CODE");
			break;
		}
		return iRetValue;
	}
	else
	{
//===========================================
// 如果不需要等待，则直接返回
		if (!bWaitFlag)
		{
			return UISDK_SUCCESS;
		}
//===========================================
// 等待运动结束标记，并且判断是否超时
        if (!WaitRobotDone(0))
		{
			RobotCoordinate CoordGet;
			iRetValue = GetRobotCoor(CoordGet);
			if ( UISDK_SUCCESS != iRetValue )
			{
				return iRetValue;
			}
//			recordRobotLog("Wait  CurrPoint:X:%f Y:%f Z:%f RZ:%f ", CoordGet.fTCPx, CoordGet.fTCPy, CoordGet.fTCPz, CoordGet.fTCPRz);
//			recordRobotLog("WaitFlag SetRobotMove TimeOut");
			bool bIsArrived = CheckIsOnePos(CoordGet, EndPoint);
			if (!bIsArrived)
			{
//				recordRobotLog("Not Arrived");
				return ROBOT_RETURN_NOTARRIVED;
			}
		}
	}
	
	//===========================================
    // RTCN返回失败，将错误码返回函数
	if(SDK_RETURN_SUCCESS != m_nErrnoFromRTCN)
	{
	    return m_nErrnoFromRTCN;
	}
	
	//===========================================
	// 机器人当前坐标位置保存到成员变量中
    CoordCurrentPosition = EndPoint;
	//===========================================
	// 成功返回
	return UISDK_SUCCESS;
}
/*=============================================================================
	 NAME: int doConnect()
	 ------------------------------------------------------------------------- 
	 Function:连接机器人 
	 ------------------------------------------------------------------------- 
	 Parameter:
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
=============================================================================*/
int CRobot::doConnect()
{
    if (0 != m_nSimulation)return UISDK_SUCCESS;
 //===========================================
 // 如果当前机器人没有连接，则返回
	if (m_bConnect)	return UISDK_SUCCESS;
	//===========================================
	// 初始化临时变量，机器人连接密码字符串：1234
	char passwd[5] = { '1', '2', '3', '4', '\0' };
	//===========================================
	// 连接机器人，重试3次
	for (int i = 0; i < 3; i++)
	{
		dwConnectCode = ConnectName(UICONNECT_IP, (char*)m_strIP.c_str());
		if (dwConnectCode)
		{
			break;
		}
		else
		{
			MySleep(300);
//			recordRobotLog("Connnect Robot Fail %d", i + 1);
		}
	}
	//===========================================
	// 检测连接是否正常
	if (UIM_HIGHWORD_VALUE(dwConnectCode) != 0)
	{
		m_bConnect = true;
		MySleep(200);
		// 向机器人发送登录密码，成功则表示连接可用，否则表示连接不正确
		if (Login(dwConnectCode, passwd, 4) == UISDK_SUCCESS)
        {
//			recordRobotLog("Robot Connect And Login OK");
			// 向机器人发送停止指令，让机器人返回待机状态
            return NormalStop();
		}
		else
		{
		// 断开机器人连接
			disConnect();
			
//			recordRobotLog("Robot Login Fail.");
			//===========================================
			// 返回错误码
			return ROBOT_RETURN_CONNECTFAILED;
		}
	}
	else
	{
		//===========================================
		// 返回错误码
		return ROBOT_RETURN_CONNECTFAILED;
	}
}
/*=============================================================================
	 NAME: void Uninit()
	 ------------------------------------------------------------------------- 
	 Function:机器人断开连接
	 ------------------------------------------------------------------------- 
	 Parameter:
	 ------------------------------------------------------------------------- 
	 Return Value:	
 ----------------------------------------------------------------------------- 
	 Note:
	       断开机器人连接
=============================================================================*/
void CRobot::disConnect()
{
    if (0 != m_nSimulation)return ;

    if  (0 != m_bConnect)
	{
		DisConnect(dwConnectCode);
		
		dwConnectCode = 0;
		
		m_bConnect = false;
	}
}

/*=============================================================================
	 NAME: int SetRobotOutPortDigitalValue(int nIoNum, bool bHighLevel)
	 ------------------------------------------------------------------------- 
	 Function:设置机器人DB25连接的IO输出
	 ------------------------------------------------------------------------- 
	 Parameter:
			 nIoNum[In]:输出端口序号，从1开始
			 bHighLevel[In]: 端口电平值，1表示高电平，0表示低电平
	 ------------------------------------------------------------------------- 
	 Return Value:		 
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::SetRobotOutPortDigitalValue(int nIoNum, bool bHighLevel)
{
    if (0 != m_nSimulation)return UISDK_SUCCESS;

     int ret = ::SetOutPortDigitalValue(dwConnectCode, nIoNum, bHighLevel);
    // 有错误，则返回错误码
    if(UISDK_SUCCESS != ret)return ret;

	// 成功返回
	return UISDK_SUCCESS;
}
/*=============================================================================
	 NAME: int GetRobotOutPortDigitalValue(int nIoNum, bool bHighLevel)
	 ------------------------------------------------------------------------- 
	 Function:获取机器人DB25连接的IO输出电平值
	 ------------------------------------------------------------------------- 
	 Parameter:
			 nIoNum[In]:输出端口序号，从1开始
			 nVal[In/Out]: 端口电平值，1表示高电平，0表示低电平
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::GetRobotOutPortDigitalValue(int nIoNum,int &nVal)
{
    if (0 != m_nSimulation)
    {
        nVal= 0;
        return UISDK_SUCCESS;
    }
//===========================================
 // 局部变量初始化，true表示高电平，false表示低电平
    bool bHighLevel = false;
	int ret = ::GetRobotOutPortValue(dwConnectCode, nIoNum, &bHighLevel);
	if(UISDK_SUCCESS != ret)
	{
//        recordRobotLog("GetRobotOutPortValue Failed %d",ret);
		throw string("GetRobotOutPortValue Failed");
        return ret;
	}
    if(bHighLevel)
		nVal = 1;
	else           
		nVal = 0;	
//===========================================
 // 成功返回
	return UISDK_SUCCESS;
}
/*=============================================================================
	 NAME: int NormalStop(int nIoNum, bool bHighLevel)
	 ------------------------------------------------------------------------- 
	 Function:向机器人发送停止消息
	 ------------------------------------------------------------------------- 
	 Parameter:
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::NormalStop()
{
    if (0 != m_nSimulation) return UISDK_SUCCESS;
//===========================================
 // 局部变量初始化 
    int iRetValue;            // 函数返回值
	int iRobotStatus  = 0;    // 判断机器人状态
//===========================================
// 获取机器人状态
	iRetValue = getRobotStatus(&iRobotStatus);
	if (UISDK_SUCCESS != iRetValue)
	{
//		recordRobotLog("getRobotStatus Error %d", iRetValue);
		return iRetValue;
	}
	else
	{	
		 // 机器人在以下指定状态下，不能发送停止消息
         switch(iRobotStatus)
        { // 可以解锁，但这些错误无法清除
          case robotMoving:
		  case robotUpdate:
		  case robotFindHome:
		  case robotErrorHand:
			RecordRobotStatusByCode(iRobotStatus);
			return ROBOT_RETURN_ERRORSTATS;  
        default:
            break;
        }
	}
//======================================================
// 获取机器人坐标，判断机器人连接通讯是否正常
    iRetValue = GetRobotCoor(CoordCurrentPosition);
    if (UISDK_SUCCESS != iRetValue)
    {
//        recordRobotLog("GetRobotCoor Error");
        OnPraseErrorMsg(iRetValue);
        return iRetValue;
    }
//======================================================
//发送紧急停止指令    
	iRetValue = ::NormalRobotStop(dwConnectCode);
	if (UISDK_SUCCESS != iRetValue)
	{
//		recordRobotLog("NormalRobotStop Error");
		OnPraseErrorMsg(iRetValue);
		return iRetValue;
	}
//等待500ms才能进入下一步操作，因为步进机器人情况下，紧急停止指令控制器需要重新启动   
	MySleep(500);
//======================================================
// 成功返回
	return UISDK_SUCCESS;
}
/*=============================================================================
	 NAME: int GetRobotInPortDigitalValue(int nIONum, int &nVal)
	 ------------------------------------------------------------------------- 
	 Function:获取机器人DB25连接的IO输入电平值
	 ------------------------------------------------------------------------- 
	 Parameter:
			 nIONum[In]:输入端口序号，从1开始
			 nVal[In/Out]: 端口电平值，1表示高电平，0表示低电平
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::GetRobotInPortDigitalValue(int nIONum, int &nVal)
{
    if (0 != m_nSimulation)
    {
        nVal = 0;
        return UISDK_SUCCESS;
    }
    ROBOTDIGITAL_VALUE_OBJ obj;
	int ret = ::GetInPortDigitalValue(dwConnectCode, &obj);
	if(UISDK_SUCCESS != ret)
	{
		OnPraseErrorMsg(ret);
//		recordRobotLog("GetRobotInPortDigitalValue failed:%d", ret);
		return ret;
	}
	//recordRobotLog("GetInPortDigitalValue nIOIndex=%d,",nIOIndex);
	//recordRobotLog("%d %d %d %d %d %d %d %d",obj.bP1DVA,obj.bP2DVA,obj.bP3DVA,obj.bP4DVA,obj.bP5DVA,obj.bP6DVA,obj.bP7DVA,obj.bP8DVA);
	int nIOIndex = nIONum - 1;
	nVal = (obj.uiDigitalValue >> nIOIndex)&0x01;
	return UISDK_SUCCESS;
}

/*=============================================================================
	 NAME: int GetRobotCoor(RobotCoordinate &coor)
	 ------------------------------------------------------------------------- 
	 Function:获取机器人坐标值
	 ------------------------------------------------------------------------- 
	 Parameter:			 
			 coor[In/Out]: 端口电平值，1表示高电平，0表示低电平
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        
=============================================================================*/
int CRobot::GetRobotCoor(RobotCoordinate &coor)
{
    if (0 != m_nSimulation)
    {
        coor = CoordCurrentPosition;
        return UISDK_SUCCESS;
    }
	UI_INITIALIZETCP_OBJ robotTCP;
	int iRetValue = GetTargetTCP(dwConnectCode, &robotTCP);
	if (UISDK_SUCCESS != iRetValue)
	{
//        recordRobotLog("GetTargetTCP Error : %x", iRetValue);
		OnPraseErrorMsg(iRetValue);
		return iRetValue;
	}
	coor.fTCPx = robotTCP.fTCPx;
	coor.fTCPy = robotTCP.fTCPy;
	coor.fTCPz = robotTCP.fTCPz;
	coor.fTCPRz = robotTCP.fTCPRz;

	return UISDK_SUCCESS;
}
int CRobot::checkPointValid(RobotCoordinate coor)
{
    if (m_nSimulation)
    {
        return UISDK_SUCCESS;
    }

    POS_XYZR pos;
    pos.x = coor.fTCPx;
    pos.y = coor.fTCPy;
    pos.z = coor.fTCPz;
    pos.r = coor.fTCPRz;
    return  ::CheckPointValid(dwConnectCode, pos);
}
int CRobot::GetRobotTCP(double *pTcpLength, double *pTcpHeight, double *pTcpAngle)
{
    if (m_nSimulation)
    {
        return UISDK_SUCCESS;
    }

    return ::GetRobotTCP(dwConnectCode, pTcpLength, pTcpHeight,pTcpAngle);
}

int CRobot::SetRobotTCP(double TcpLength, double TcpHeight, double TcpAngle)
{
    if (m_nSimulation)
    {
        return UISDK_SUCCESS;
    }

    return ::SetRobotTCP(dwConnectCode, TcpLength, TcpHeight,TcpAngle);
}

bool * CRobot::GetPVTMoveFlag()
{
    if (m_nSimulation)
    {
        return &m_bPVTMoveOK;
    }

	return &m_bPVTMoveOK;
}
/*=============================================================================
	 NAME: int LooseAllJoints()
	 ------------------------------------------------------------------------- 
	 Function:松开机器人
	 ------------------------------------------------------------------------- 
	 Parameter:			 
			  
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	       机器人各个关节脱机 
=============================================================================*/
int CRobot::LooseAllJoints()
{
    if (0 != m_nSimulation) return UISDK_SUCCESS;
    return LooseAllJoint(dwConnectCode);
}
/*=============================================================================
	 NAME: int LooseAllJoints()
	 ------------------------------------------------------------------------- 
	 Function:使能机器人
	 ------------------------------------------------------------------------- 
	 Parameter:			 
			  
	 ------------------------------------------------------------------------- 
	 Return Value:	
		 If the function fails, the return value is error code 
		 If the function succeeds, the return value is SDK_RETURN_SUCCESS. 
 ----------------------------------------------------------------------------- 
	 Note:
	        机器人各个关节使能 
=============================================================================*/
int CRobot::LockAllJoints()
{
    if (0 != m_nSimulation)
    {
        return UISDK_SUCCESS;
    }

    return LockAllJoint(dwConnectCode);
}

/*=============================================================================
     NAME:  getUIRototType()
     -------------------------------------------------------------------------
     Function:获取机器人类型函数
     -------------------------------------------------------------------------
     Parameter:

     -------------------------------------------------------------------------
     Return Value: type of robot(scara/delta)
 -----------------------------------------------------------------------------
     Note:

=============================================================================*/
int CRobot::getUIRototType(int *piRobotType)
{
    if (0 != m_nSimulation)
    {
        *piRobotType = 1;
        return UISDK_SUCCESS;
    }

    return getRototType(dwConnectCode,piRobotType);    
}

/*=============================================================================
NAME:  RecordRobotStatusByCode()
-------------------------------------------------------------------------
Function:翻译机器人错误代码并写入日志
-------------------------------------------------------------------------
Parameter:int iRobotStatus

-------------------------------------------------------------------------
Return Value: 
-----------------------------------------------------------------------------
Note:

=============================================================================*/
void CRobot::RecordRobotStatusByCode(int iRobotStatus)
{
	switch (iRobotStatus)
	{
	case robotIdle:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotIdle");
		break;
	case robotMoving:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotMoving");
		break;
	case robotMotorErr:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotMotorErr");
		break;
	case robotDisable:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotDisable");
		break;
	case robotUpdate:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotUpdate");
		break;
	case robotLocked:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotLocked");
		break;
	case robotFindHome:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotFindHome");
		break;
	case robotErrorHand:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "robotErrorHand");
		break;
	default:
//		recordRobotLog("Robot Error Status %d, %s", iRobotStatus, "unknown status");
		break;
	}
}

/*=============================================================================
	 NAME:  CRobotManager()
	 ------------------------------------------------------------------------- 
	 Function:机器人管理类构造函数
	 ------------------------------------------------------------------------- 
	 Parameter:			 
			  
	 ------------------------------------------------------------------------- 
	 Return Value:	
 ----------------------------------------------------------------------------- 
	 Note:
	       
=============================================================================*/
CRobotManager::CRobotManager(void)
{
// 注册机器人RTCN回调函数
	UIMRobotRegRtcnCallBack(RobotStaticRevRtcnNotify);

}
/*=============================================================================
	 NAME:  CRobotManager()
	 ------------------------------------------------------------------------- 
	 Function:机器人管理类析构函数
	 ------------------------------------------------------------------------- 
	 Parameter:			 
			  
	 ------------------------------------------------------------------------- 
	 Return Value:	
 ----------------------------------------------------------------------------- 
	 Note:
	       
=============================================================================*/
CRobotManager::~CRobotManager(void)
{
}
/*=============================================================================
	 NAME: void RobotStaticRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen)
	 ------------------------------------------------------------------------- 
	 Function:机器人回调函数处理函数
	 ------------------------------------------------------------------------- 
	 Parameter:			 
		 dwConnectType[In]:机器人连接数值
		 cmd_type[In]:机器人RTCN消息所带的消息类型
		 pMessageBuffer[In]:消息所带的数据缓冲区数据
		 iDataLen[In]:消息所带的数据缓冲区数据字节数
	 ------------------------------------------------------------------------- 
	 Return Value:	
 ----------------------------------------------------------------------------- 
	 Note:
	       
=============================================================================*/
void CRobotManager::RobotStaticRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen)
{
	CRobotManager::GetInstance()->DoRobotStaticRevRtcnNotify(dwConnectType, cmd_type, pMessageBuffer, iDataLen);
}

/*=============================================================================
	 NAME: void DoRobotStaticRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen)
	 ------------------------------------------------------------------------- 
	 Function:机器人回调函数处理函数
	 ------------------------------------------------------------------------- 
	 Parameter:			 
		 dwConnectType[In]:机器人连接数值
		 cmd_type[In]:机器人RTCN消息所带的消息类型
		 pMessageBuffer[In]:消息所带的数据缓冲区数据
		 iDataLen[In]:消息所带的数据缓冲区数据字节数
	 ------------------------------------------------------------------------- 
	 Return Value:	
 ----------------------------------------------------------------------------- 
	 Note:
	       向各个注册的机器人发送RTCN消息
=============================================================================*/
void CRobotManager::DoRobotStaticRevRtcnNotify(unsigned int dwConnectType, unsigned char cmd_type, char *pMessageBuffer, int iDataLen)
{
	for (unsigned int i = 0; i < m_RobotVetor.size(); i++)
		if (m_RobotVetor[i]->DwConnectCode() == dwConnectType)
			m_RobotVetor[i]->RobotRevRtcnNotify(dwConnectType, cmd_type, pMessageBuffer, iDataLen);
}
