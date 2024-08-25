 // UIRobotSdkFunc.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "stdio.h"
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace	std;


#include "UIRobotProtocol.h"
#include "UIRobotStruct.h"
#include "SocketClient.h"


#define ARMLIMITINDEX_X (0)
#define ARMLIMITINDEX_Y (1)
#define ARMLIMITINDEX_Z (2)
#define ARMLIMITTYPE_TOP (0)
#define ARMLIMITTYPE_BOTTOM (1)


PF_UIROBOTSDK_CALLBACK pfRobotRtcnCallBack=NULL;
/*
*/
//===========================
//	机器人文件写入数据块数据结构
//===========================



int SDKSendCommand(unsigned int dwConnectType, unsigned char cmd_type, int nSendSize, char *InPutbuffer, char *Outbuffer = NULL, int *pOutLen = NULL);




// 机器人连接保存结构体
typedef  struct _RobotSDKConnetData
{
	CSocketClient *m_pSocketClient;
    int m_iRobotType; // 机器人类型:1--SCARA; 2--Delta
	int m_iConnectType;	
	char chSocketInBuffer[UISDK_MAX_DATA_LEN]; // 输入值缓存	
	char chSocketOutBuffer[UISDK_MAX_DATA_LEN]; // 返回值缓存
	_RobotSDKConnetData()
	{
	   Reset();
	}
	void Reset()
	{
	   m_pSocketClient = NULL;
	   m_iRobotType = -1;
	   m_iConnectType = 0;
	   ClearBuffer();	   
	}
	void ClearBuffer()
	{
	   memset(chSocketInBuffer,0x0,sizeof(chSocketInBuffer));
	   memset(chSocketOutBuffer,0x0,sizeof(chSocketOutBuffer));
	}
	~_RobotSDKConnetData()
	{
	   if(m_pSocketClient)
	   {
	      FreePtr(m_pSocketClient);			
	   }
	}
}RobotSDKConnetData, *P_RobotSDKConnetData;

RobotSDKConnetData g_SocketClient[MAX_GATEWAY_COUNT];



SDK_CRITICAL_SECTION   g_hFileWriteCriSection;  //  文件写入临界区
ofstream g_outPutFile; // 输出文件
long g_outFileSize = 0;



#ifdef SDK_DEBUG_PROGRAM

#define UICONNECT_IP    0x01
using namespace std;



int main(int argc, char *argv[])
{
	int dwConnectCode = 0,iRetValue = 0;
	char IPAddress[] = "192.168.1.16";
	char passwd[5] = { '1', '2', '3', '4', '\0' };
	dwConnectCode = ConnectName(UICONNECT_IP, IPAddress);
	int iDataLen = 0;
	if (dwConnectCode)
	{
		if (Login(dwConnectCode, passwd, 4) == UISDK_SUCCESS)
		{
				// Hover mouse over "sz" while debugging to see its contents
		//cout << sz << endl;	//<================= Put a breakpoint here
		//(double x, double y, double z, double rz, double *angle_1, double *angle_2, double *angle_3, double *Toolz)
			double Roangle[4] = { 0 };
				
			POS_XYZR pos;
			ROBOTANG ang;
			pos.x = 250;
			pos.y = 250;
			pos.z = 0;
			pos.r = 0;
			int ret = getANGfromXYZ(pos, &ang);
			int iNodePointIndex = 0;
			int iNodePointCount = 0;
			int m_iMaxPointCount = 3;// 至多子节点个数
			int m_iMinPointCount = 2;// 至少子节点个数
			int m_iMotionType=ArchMove;// CURVE  = 0,PTP    = 1,LINE   = 2,Circle = 3,Arch   = 4
			XMLNodePoint ExMoveNodePoint;//
			XMLNodePoint MoveNodePoint[10];// = { 0 };		
			double fNodePoint[3][4] = { { 0.0 } };	

			iNodePointCount = 2; // 子节点个数
			if ((iNodePointCount > m_iMaxPointCount) || (iNodePointCount < m_iMinPointCount))
			{
				//strShowMsg = _T("路点个数不正确\r\n");
				//SetCommandAckText(strShowMsg);
				return 0;
			}
			//getNodePointValue(&fNodePoint[0][0], IDC_EDIT_MOVEPOINTX, IDC_EDIT_MOVEPOINTY, IDC_EDIT_MOVEPOINTZ, IDC_EDIT_MOVEPOINTRZ);
			//getNodePointValue(&fNodePoint[1][0], IDC_EDIT_MOVEPOINTX2, IDC_EDIT_MOVEPOINTY2, IDC_EDIT_MOVEPOINTZ2, IDC_EDIT_MOVEPOINTRZ2);
		//	getNodePointValue(&fNodePoint[2][0], IDC_EDIT_MOVEPOINTX3, IDC_EDIT_MOVEPOINTY3, IDC_EDIT_MOVEPOINTZ3, IDC_EDIT_MOVEPOINTRZ3);
	
		//	((CEdit*)GetDlgItem(IDC_EDIT_MOVEPOINTX))->GetWindowText(strTemp);
			iNodePointIndex = 0;
			if (m_iMotionType == ArchMove)
			{
				MoveNodePoint[iNodePointIndex].nodeType = MOVEARCH;		
				MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.subMoveType = 0;
				MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Lg1 = 10;// IDC_EDIT_HEIGHT1
				MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Lg2 = 10;// IDC_EDIT_HEIGHT2
				MoveNodePoint[iNodePointIndex].nodeData.SPPmoveParameters.Hgt = 10;// IDC_EDIT_MIDHEIGHT
					/*
				((CStatic*)GetDlgItem(IDC_EDIT_HEIGHT1))->GetWindowText(strTemp);
				MoveNodePoint[iNodePointIndex].nodeData.moveParameters.archMoveHeight[0] = (float)atof(strTemp);
				((CStatic*)GetDlgItem(IDC_EDIT_HEIGHT2))->GetWindowText(strTemp);
				MoveNodePoint[iNodePointIndex].nodeData.moveParameters.archMoveHeight[1] = (float)atof(strTemp);
				((CStatic*)GetDlgItem(IDC_EDIT_MIDHEIGHT))->GetWindowText(strTemp);
				MoveNodePoint[iNodePointIndex].nodeData.moveParameters.archMoveHeight[2] = (float)atof(strTemp);
				*/
			}
			else
			{
				MoveNodePoint[iNodePointIndex].nodeType = ROBOTMOVE;
				MoveNodePoint[iNodePointIndex].nodeData.moveParameters.subMoveType = m_iMotionType;
			}
			memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
			MoveNodePoint[iNodePointIndex].speedRate = 50;
			MoveNodePoint[iNodePointIndex].counts = iNodePointCount;
			/*
			((CStatic*)GetDlgItem(IDC_STATIC_SPEED))->GetWindowText(strTemp);
			MoveNodePoint[iNodePointIndex].speedRate = atoi(strTemp);
			if (MoveNodePoint[iNodePointIndex].speedRate <= 0)MoveNodePoint[0].speedRate = 10;
			MoveNodePoint[iNodePointIndex].counts = iNodePointCount;
			*/
			//strncpy_s((char *)NodeParent.strLabel, strlen(_T("ParentNodePoint")), _T("ParentNodePoint"), 30);
			//MoveNodePoint[iNodePointIndex].nodeData.moveParameters.userCoordindateSystemIndex = 0;
			MoveNodePoint[iNodePointIndex].nodeData.moveParameters.tcpIndex = 0;
			iNodePointIndex++;

				// node 1	
			MoveNodePoint[iNodePointIndex].counts = 0;
			MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
			MoveNodePoint[iNodePointIndex].speedRate = 0;
			memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
			MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = 251.28f;
			MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = 140.10f;
			MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = 0;
			MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = 4.10f;
			
			/*
			for (int i = 0; i < 4; i++)
			{
				MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[i] = (float)fNodePoint[0][i];
			}
			*/
			MoveNodePoint[iNodePointIndex].nodeData.waypoint.isTransitionPoint = 0;
			iNodePointIndex++;
			if (iNodePointCount > 1)
			{	
				// node 2		
				MoveNodePoint[iNodePointIndex].counts = 0;
				MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
				MoveNodePoint[iNodePointIndex].speedRate = 0;
				memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
				MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[0] = 251.28f;
				MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[1] = 159.90f;
				MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[2] = 0;
				MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[3] = 4.10f;
				/*
				for (int i = 0; i < 4; i++)
				{
					MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[i] = (float)fNodePoint[1][i];
				}
				*/
				MoveNodePoint[iNodePointIndex].nodeData.waypoint.isTransitionPoint = 0;
				iNodePointIndex++;
				/*
				if (iNodePointCount > 2)
				{
					
					// node 3			
					MoveNodePoint[iNodePointIndex].counts = 0;
					MoveNodePoint[iNodePointIndex].nodeType = WAYPOINT_COORDINATE;
					MoveNodePoint[iNodePointIndex].speedRate = 0;
					memcpy(MoveNodePoint[iNodePointIndex].aliasName, "NodePoint", strlen("NodePoint"));
					for (int i = 0; i < 4; i++)
					{
						MoveNodePoint[iNodePointIndex].nodeData.waypoint.point[i] = (float)fNodePoint[2][i];
					}
					MoveNodePoint[iNodePointIndex].nodeData.waypoint.isTransitionPoint = 0;
					iNodePointIndex++;
					
				}
				*/
				UI_INITIALIZETCP_OBJ cmdObj = { 0 };
				iRetValue = SetRobotMove(dwConnectCode, MoveNodePoint, iNodePointIndex);
				if (iRetValue)
				{
				  //cout << "test" << endl;
				}
			}
		
		}
		else
		{
			//cout << "test" << endl;
		}
	}

	// char sz[] = "Hello, World!";	// Hover mouse over "sz" while debugging to see its contents
	// cout << sz << endl;	//<================= Put a breakpoint here
	return 0;
}

#endif





// RobotPVT *g_pRobotPVT;   // 机器人全部轴 PVT（此处必须为指针）
char g_modelName[200] = { 0 };

// bool g_irobotLeftHand = 0;  // 机器人左右手类型:0--右手; 1--左手
bool g_bInitializeTrace = false;

//API_WINDOW_EXTERN int getDeltaCurvePVTtask(UIDelta *delta, int n, double posStr[], double posEnd[], double posMdlHgt, double timeRatio, int turnFlag, RobotPVTLists *task, int *totalTime, double *VelRtLmt, double *AccRtLmt, double *posMdlHgtUpdata);

//API_WINDOW_EXTERN int getDeltaCurvePVTtask(UIDelta *delta, int n, double posStr[], double posEnd[], double posMdlHgt, double timeRatio, int turnFlag, RobotPVTLists *task, int *totalTime, double *posMdlHgtUpdata,double TorqueAll[255][3], double angV[255][3], double TorqueMupVel[255][3], double paragroup[5]);

FILE *fTrace=NULL; 

int g_iFileLen = 0;
bool bInitializeTrace = false;
int g_iUartWiteIndex = 0;

CallBackData g_staticCallBackData;
// 连接类型:网络，蓝牙
unsigned int GetConnectDevType( unsigned int dwDevIndex )
{
	return ((dwDevIndex>> 16) & 0x0FF);
}
// 连接索引号
unsigned int GetDevID( unsigned int dwDevIndex )
{
	return dwDevIndex & 0x07F;
}
// 机器人类型 
unsigned int GetConnectRobotType( unsigned int dwDevIndex )
{
    if(!dwDevIndex)
    {
       return -1;
    }
	return ((dwDevIndex>> 8) & 0x0FF);;
}

unsigned int GetDevTypeIndex(unsigned int dwConnectCode, unsigned int dwIndex, unsigned int dwRobotType)
{
	return ((dwIndex&0x0FF)|((dwRobotType&0x0FF)<<8)|((dwConnectCode&0x0FF)<<16))&0x0FFFFFF;  
}





#ifdef LINUX   
SDK_CRITICAL_SECTION g_LogWritecsTrans; 
#endif
#if 0
void SDKOpenTraceFile()
{
	if(!bInitializeTrace)	
	{
#ifdef LINUX
				char m_szTraceFile[64] = { 0 }; 
				sprintf_s(m_szTraceFile, 64, "//lib//%s", "RobotSDKTrace.txt");
				if((access("//lib//",F_OK))==-1)   //文件不存在
				{  
					mkdir("//lib//", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);		
				} 
				fTrace =  fopen(m_szTraceFile, "wt+"); 
				long size =  ftell(fTrace);
				g_iFileLen = size;
				sprintf_s(m_szTraceFile, sizeof(m_szTraceFile), "SDKOpenTraceFile g_iFileLen=%d,size=%d", g_iFileLen,size);
//#ifdef MYDebug	
				printf(m_szTraceFile);
				robotdebug(m_szTraceFile);
//#endif	
				Sleep(1);
				SDK_InitializeCriticalSection(&g_LogWritecsTrans);
				bInitializeTrace = true;	
				g_iUartWiteIndex = 0;
#endif

	}
  
}
void SDKCloseTraceFile()
{

	if (bInitializeTrace)
	{
	  #ifdef LINUX	
	  SDK_DeleteCriticalSection(&g_LogWritecsTrans);
	  #endif
		if(fTrace)
		{
		  fclose(fTrace); 
		}	
	//	fTrace.close();
		bInitializeTrace = false;
	}
}


void SDKTraceFile(const char *fmt, ...)
{
      if(bInitializeTrace)
      {      
      
		char buf[512]={0}; 
		char Outbuf[1024]={0}; 
		char Timebuf[64]={0}; 
		time_t now;
		 
		 /*获取日历时间*/
		 time(&now);
		 /*转换成tm时间*/
#ifdef WIN32	
		SYSTEMTIME Debugtime;
		GetLocalTime(&Debugtime);
		sprintf_s(Timebuf, 64, "%d %d ",Debugtime.wSecond, (int)Debugtime.wMilliseconds);
		int i;
		va_list vlist;
		va_start(vlist,fmt);
		i = vsprintf_s(buf, sizeof(buf), fmt, vlist);
		va_end(vlist);
		sprintf_s(Outbuf, sizeof(Outbuf), "%s:%s ", Timebuf, buf);
		OutputDebugString(Outbuf);
		//	if(fTrace.is_open())
			{
			//   fTrace<<Outbuf;
			}
#elif LINUX	
      //   SDK_EnterCriticalSection(g_LogWritecsTrans);	
		 struct tm *tmlocal=NULL;
		 struct timeval nowtv;
		 gettimeofday(&nowtv, NULL);
		 tmlocal = localtime(&now); 		 
		 sprintf_s(Timebuf, sizeof(Timebuf), "%s %d", asctime(tmlocal),nowtv.tv_usec);		
		int i;
		va_list vlist;
		va_start(vlist,fmt);
		i = vsnprintf(buf, sizeof(buf), fmt, vlist);
		va_end(vlist);
		sprintf_s(Outbuf, sizeof(Outbuf), "%s:%s\r\n", Timebuf,buf);
//#ifdef MYDebug	
		printf(Outbuf);
	    robotdebug(Outbuf);
//#endif	
		if(bInitializeTrace)
		{
			int iStrlen =  strlen(Outbuf);
			g_iFileLen += iStrlen;			
			if (fTrace == NULL)
				return; 
			fwrite(Outbuf, strlen(Outbuf), 1, fTrace);
		//	fflush(fTrace);  //刷新
		    SDKTraceFileRefresh(true);
		}
	//  SDK_LeaveCriticalSection(g_LogWritecsTrans);
#endif
      }
}

void SDKTraceFileRefresh(bool bReset)
{
    if(bInitializeTrace)
    {
			if(bReset)
			{
				// char buf[512] = { 0 }; 
				if(g_iFileLen > 1024*1024*10)
				{				  
#ifdef WIN32 
				//	fopen_s(&fTrace, buf, "wt+"); 
#else 
                    fseek(fTrace,0,SEEK_SET);
					//fTrace = fopen(buf, "wt+");
#endif			
				   
				   g_iFileLen = 0;
				   bInitializeTrace = true;
				}		
			}
			if(fTrace)
			{	
				fflush(fTrace);  //刷新
			}
	}
}
#endif






void SDKSTDCALLAPI setPointNull()
{
// 	createUIScara == NULL; releaseUIScara == NULL || setScaraFirstArmConf == NULL
// 		|| getScaraFirstArmConf == NULL || setScaraSecondArmConf == NULL || setbRightHand == NULL
// 		|| getScaraSecondArmConf == NULL || setScaraThirdArmConf == NULL || getScaraThirdArmConf == NULL
// 		|| setScaraFourthArmConf == NULL || getScaraFourthArmConf == NULL || setScaraPVTInterval == NULL
// 		|| getScaraPVTInterval == NULL || getScara3DPosWithPulse == NULL || getScaraPulseWith3DPos == NULL
// 		|| releaseNodePVT == NULL || getScara2DAngleWith2DPos == NULL || getScara2DPosWith2DAngle == NULL
// 		|| getScara3DAngleWith3DPos == NULL || getScara3DPosWith3DAngle == NULL || getTCPviateaching == NULL
// 		|| settableCoordinateMaxtrix == NULL || getGeneralCoordinateWithTable == NULL || getTableCoordinateWithGeneral == NULL
}

//////////////////////////////////////////////////////////////////////////////////////
int GetRobotRTCNError(unsigned int iCommandError,char *Outbuffer)
{
       const int iMaxTempLen = 3;
       char  tempBuffer[iMaxTempLen] = { 0 };  
	int i = 0 ; 
	for(i = 0; i < iMaxTempLen-1; i++)
	{
	     if(Outbuffer[i+2] == '\0')
	     {
	        break;
	     }
	     tempBuffer[i] = Outbuffer[i+2];						 
	}
	if(i < iMaxTempLen-2)
	{
	    return iCommandError;
	}
	tempBuffer[iMaxTempLen-1] = '\0'; 
	return atoi(tempBuffer);

}


int SDKSendCommand(unsigned int dwConnectCode,unsigned char cmd_type, int nSendSize, char *InPutbuffer, char *Outbuffer, int *pOutLen)
{      
	  unsigned int dwDevType = GetConnectDevType(dwConnectCode);   
	  unsigned int dwGWIndex = GetDevID(dwConnectCode);
	  if(!dwConnectCode)
	  {
	     return PROTOCOL_CMD_ERROR_PARAMETER;
	  }
      switch(dwDevType)
      	{
      	     case UICONNECT_IP:
		     {		 	
				 if (g_SocketClient[dwGWIndex].m_pSocketClient)
				 {
					 return g_SocketClient[dwGWIndex].m_pSocketClient->SendSocketCommand(dwConnectCode,cmd_type, nSendSize, InPutbuffer, Outbuffer, pOutLen);
				 }                  
		     }				
	     break;
			
      	}
	return PROTOCOL_CMD_ERROR_PARAMETER;

}




/*==============================================================================
	NAME: BOOL UIMRegRtcnCallBack(PF_UIROBOTSDK_CALLBACK pCallBackFunc)
	----------------------------------------------------------------------------
	Function:回调函数注册函数
	----------------------------------------------------------------------------
	Parameter:
	PF_UIROBOTSDK_CALLBACK pCallBackFunc:回调函数指针
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
	用于网关回调函数的注册
==============================================================================*/
int SDKSTDCALLAPI UIMRobotRegRtcnCallBack(PF_UIROBOTSDK_CALLBACK pCallBackFunc)
{
	//所有的设备共用同一个回调函数
	pfRobotRtcnCallBack = pCallBackFunc;
	return 0;
}

/*==============================================================================
	NAME: int SDKSTDCALLAPI ConnectValue(unsigned int dwConnectCode, int iIPValue)
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
int SDKSTDCALLAPI ConnectValue(unsigned int dwConnectType, int iIPValue)
{
      int iRetCode = 0;
	  unsigned int dwGWIndex = 0;
      switch(dwConnectType)
      {
      	 case UICONNECT_IP:
	     {		
		 	 int i = 0;
		 	 for(i = 0; i < MAX_GATEWAY_COUNT-1; i++)
		 	 {
		 	    if(g_SocketClient[i].m_pSocketClient == NULL)
		 	    {
		 	       break;
		 	    }
		 	 }
			 dwGWIndex = i;
		 	 g_SocketClient[dwGWIndex].m_pSocketClient = new CSocketClient();
			 int ret = g_SocketClient[dwGWIndex].m_pSocketClient->ConnectValueServer(iIPValue);
		//如果连接成功，客户端开始接收线程。			
			
			 if (ret == 0)
			 {
				iRetCode = GetDevTypeIndex(UICONNECT_IP, dwGWIndex,0);	 
				ret = g_SocketClient[dwGWIndex].m_pSocketClient->SocketStartBufProcThread(iRetCode);				 
				 int iRobotType = -1;
				 ret =  getRototType(iRetCode, &iRobotType);
				if((iRobotType >= 0) && (iRobotType < UIROBOTTYPE_MAXTYPE))
				{
				    iRetCode = GetDevTypeIndex(UICONNECT_IP, dwGWIndex, iRobotType); 
					g_SocketClient[dwGWIndex].m_pSocketClient->setConnectCode(iRetCode);
					
					
// 					int *pIndex;
// 					if (GetRobotTCP(iRetCode, pIndex) > 0)
// 					{
// 					
// 					}	
					return iRetCode;
				}
				else
			    {
				     DisConnect(iRetCode);
			    }
				
			}
			 
	//		SDKTraceFile("TCPConnectServer ConnectValue iRetCode=%d\n",iRetCode);
			return iRetCode;			
	     }				
	     break;
  	    
      	}
	    return 0;
}
/*==============================================================================
	NAME: int ConnectName(unsigned int dwConnectCode, char *pConnectParam)
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
 int SDKSTDCALLAPI ConnectName(unsigned int dwConnectType, char *pConnectParam)
 {
	 int iRetCode = 0;
	 unsigned int dwGWIndex = 0;
	 switch (dwConnectType)
	 {
	 	 case UICONNECT_IP: 
		 {
			 int i = 0;
			 for(i = 0; i < MAX_GATEWAY_COUNT-1; i++)
			 {
			   if(g_SocketClient[i].m_pSocketClient == NULL)
			   {
				  break;
			   }
			 }
			 dwGWIndex = i;
			 g_SocketClient[dwGWIndex].m_pSocketClient = new CSocketClient();
			 int ret = g_SocketClient[dwGWIndex].m_pSocketClient->ConnectNameServer(pConnectParam);
			 //如果连接成功，客户端开始接收线程。

			 if (ret == 0)
			 {
				 iRetCode = GetDevTypeIndex(UICONNECT_IP,dwGWIndex,0);	
				 ret = g_SocketClient[dwGWIndex].m_pSocketClient->SocketStartBufProcThread(dwConnectType);
				 int iRobotType = -1;
				 ret = getRototType(iRetCode, &iRobotType);
				 if((iRobotType >= 0) && (iRobotType < UIROBOTTYPE_MAXTYPE))
				 {
					 iRetCode = GetDevTypeIndex(UICONNECT_IP,dwGWIndex,iRobotType); 					 
					 g_SocketClient[dwGWIndex].m_pSocketClient->setConnectCode(iRetCode);
                     g_SocketClient[dwGWIndex].m_iRobotType = iRobotType;
					 return iRetCode;
				 }
				 else
				 {
					 DisConnect(iRetCode);
					 iRetCode = 0;
				 }
			 }

			 return iRetCode;
		 }
	 break;
	 
	 }
	 return 0;
 }

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
 int SDKSTDCALLAPI DisConnect(unsigned int dwConnectCode)
{
	unsigned int dwConnectType = GetConnectDevType(dwConnectCode);   
	unsigned int dwGWIndex = GetDevID(dwConnectCode);
    switch(dwConnectType)
  	{
  	     case UICONNECT_IP:
        {		
			if(g_SocketClient[dwGWIndex].m_pSocketClient)
			{
				g_SocketClient[dwGWIndex].m_pSocketClient->DisConnect();
				FreePtr(g_SocketClient[dwGWIndex].m_pSocketClient);			
			}			
       }				
       break;  	
  	}
	return PROTOCOL_CMD_ERROR_PARAMETER;
}

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
int SDKSTDCALLAPI Login(unsigned int dwConnectCode, char *pPassword, int iPasswordLen)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret  = SDKSendCommand(dwConnectCode,UICMDLOGIN, iPasswordLen, pPassword, chSocketOutBuffer);			 
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			
}

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
int SDKSTDCALLAPI SetPassword(unsigned int dwConnectCode, P_UI_SETPASSWORD_OBJ setPasswordObj)
{
	 if(setPasswordObj->iOldPasswordLen + setPasswordObj->iNewPasswordLen >= UISDK_MAX_SDKBUFFER_LEN)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }	 
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
	 memcpy(chSocketInBuffer, setPasswordObj->OldPassword , setPasswordObj->iOldPasswordLen);
	 chSocketInBuffer[setPasswordObj->iOldPasswordLen] = '|';
	 memcpy(&chSocketInBuffer[setPasswordObj->iOldPasswordLen + 1], setPasswordObj->NewPassword , setPasswordObj->iNewPasswordLen);
              chSocketInBuffer[setPasswordObj->iOldPasswordLen +setPasswordObj->iNewPasswordLen + 1] = '\0';	
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETPASWORD,setPasswordObj->iOldPasswordLen + setPasswordObj->iNewPasswordLen + 1, (char *)chSocketInBuffer,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			
}

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
int SDKSTDCALLAPI SetToolMassPara(unsigned int dwConnectCode, P_UI_TOOLMASS_OBJ toolMassObj)
{
	if (!toolMassObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer,
		UISDK_MAX_SDKBUFFER_DOUBLE_LEN,
		"%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f",
		toolMassObj->index,
		toolMassObj->name,
		toolMassObj->relativePos[0],
		toolMassObj->relativePos[1],
		toolMassObj->relativePos[2],
		toolMassObj->relativePos[3],
		toolMassObj->massPos[0],
		toolMassObj->massPos[1],
		toolMassObj->massPos[2],
		toolMassObj->massPos[3]);
#elif LINUX	
	sprintf_s(chSocketInBuffer,
		sizeof chSocketInBuffer,
		"%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f",
		toolMassObj->index,
		toolMassObj->name,
		toolMassObj->relativePos[0],
		toolMassObj->relativePos[1],
		toolMassObj->relativePos[2],
		toolMassObj->relativePos[3],
		toolMassObj->massPos[0],
		toolMassObj->massPos[1],
		toolMassObj->massPos[2],
		toolMassObj->massPos[3]);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDSETMASSPARA, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}
/*==============================================================================
NAME: int GetToolMassPara(unsigned int dwConnectCode, int InPutbuffer, P_UI_TOOLMASS_OBJ toolMassObj)
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
int SDKSTDCALLAPI GetToolMassPara(unsigned int dwConnectCode, int InPutbuffer, P_UI_TOOLMASS_OBJ toolMassObj)
{
	if (!toolMassObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}	
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
	int iOutLen = 0;
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, "%d", InPutbuffer);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer,  "%d", InPutbuffer);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDGETMASSPARA, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		int iReceiveLen = iOutLen;
		const int iMaxTempLen = 100;
		char tempBuffer[iMaxTempLen] = { 0 };
		int iVariableIndex = 0; // toolMassObj data index
		char chSocket;
		int iTempIndex = 0;// tempBuffer index
		int iReceveIndex = 0; // chSocketOutBuffer index
		float *pMassToolPoint[8];
		char *pMassToolName = NULL;
		unsigned int *pMassToolIndex;
		pMassToolIndex = &toolMassObj->index;
		pMassToolName = &toolMassObj->name[0];
		memset(pMassToolName, 0x0, 30);
		pMassToolPoint[0] = &toolMassObj->relativePos[0];
		pMassToolPoint[1] = &toolMassObj->relativePos[1];
		pMassToolPoint[2] = &toolMassObj->relativePos[2];
		pMassToolPoint[3] = &toolMassObj->relativePos[3];
		pMassToolPoint[4] = &toolMassObj->massPos[0];
		pMassToolPoint[5] = &toolMassObj->massPos[1];
		pMassToolPoint[6] = &toolMassObj->massPos[2];
		pMassToolPoint[7] = &toolMassObj->massPos[3];
		for (iVariableIndex = 0; iVariableIndex < 10; iVariableIndex++)
		{
			if (iReceveIndex < iReceiveLen)
			{
				memset(tempBuffer, 0x0, iMaxTempLen);
				for (iTempIndex = 0; (iTempIndex < iMaxTempLen - 1) && (iReceveIndex < iReceiveLen); ++iTempIndex)
				{
					chSocket = chSocketOutBuffer[iReceveIndex];
					tempBuffer[iTempIndex] = chSocket;
					iReceveIndex++;
					if ((chSocket == '|') || (chSocket == '\0'))
					{
						tempBuffer[iTempIndex] = '\0';
						break;
					}
				}
				if (iTempIndex == iMaxTempLen - 1)
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
				tempBuffer[iMaxTempLen - 1] = '\0';

				switch (iVariableIndex)
				{
				case 0:
					*pMassToolIndex = atoi(tempBuffer);
					break;
				case 1:
					memcpy(pMassToolName, tempBuffer, strlen(tempBuffer));
					break;
				default:
					*pMassToolPoint[iVariableIndex - 2] = (float)atof(tempBuffer);
					break;
				}
			}
			else
			{
				return PROTOCOL_CMD_RETURN_FAILED;
			}
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI SetUserCoor(unsigned int dwConnectCode, P_UI_USERCOOR_OBJ userCoolObj)
{
	if (!userCoolObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}	
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer,
		UISDK_MAX_SDKBUFFER_DOUBLE_LEN,
		"%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", 
		userCoolObj->index,
		userCoolObj->name,
		userCoolObj->point1[0],
		userCoolObj->point1[1],
		userCoolObj->point1[2], 
		userCoolObj->point2[0],
		userCoolObj->point2[1],
		userCoolObj->point2[2],
		userCoolObj->point3[0],
		userCoolObj->point3[1],
		userCoolObj->point3[2]);
#elif LINUX	
	sprintf_s(chSocketInBuffer,
		sizeof chSocketInBuffer,
		"%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", 
		userCoolObj->index,
		userCoolObj->name,
		userCoolObj->point1[0],
		userCoolObj->point1[1],
		userCoolObj->point1[2], 
		userCoolObj->point2[0],
		userCoolObj->point2[1],
		userCoolObj->point2[2],
		userCoolObj->point3[0],
		userCoolObj->point3[1],
		userCoolObj->point3[2]);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDSETUSERCOOR, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI GetUserCoor(unsigned int dwConnectCode, int InPutbuffer, P_UI_USERCOOR_OBJ userCoolObj)
{
	if (!userCoolObj)
	{
		return PROTOCOL_CMD_RETURN_FAILED;
	}
	
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, "%d", InPutbuffer);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d", InPutbuffer);
#endif	

	int ret = SDKSendCommand(dwConnectCode, UICMDGETUSERCOOR, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		int iReceiveLen = iOutLen;
		const int iMaxTempLen = 100;
		char tempBuffer[iMaxTempLen] = { 0 };
		int iVariableIndex = 0; // toolMassObj data index
		char chSocket;
		int iTempIndex = 0;// tempBuffer index
		int iReceveIndex = 0; // chSocketOutBuffer index
		float *pUserCoorPoint[9];
		char *pUserCoorName = NULL;
		unsigned int *pUserCoorIndex;
		pUserCoorIndex = &userCoolObj->index;
		pUserCoorName = &userCoolObj->name[0];
		memset(pUserCoorName, 0x0, 30);
		pUserCoorPoint[0] = &userCoolObj->point1[0];
		pUserCoorPoint[1] = &userCoolObj->point1[1];
		pUserCoorPoint[2] = &userCoolObj->point1[2];
		pUserCoorPoint[3] = &userCoolObj->point2[0];
		pUserCoorPoint[4] = &userCoolObj->point2[1];
		pUserCoorPoint[5] = &userCoolObj->point2[2];
		pUserCoorPoint[6] = &userCoolObj->point3[0];
		pUserCoorPoint[7] = &userCoolObj->point3[1];
		pUserCoorPoint[8] = &userCoolObj->point3[2];

		for (iVariableIndex = 0; iVariableIndex < 11; iVariableIndex++)
		{
			if (iReceveIndex < iReceiveLen)
			{
				memset(tempBuffer, 0x0, iMaxTempLen);
				for (iTempIndex = 0; (iTempIndex < iMaxTempLen - 1) && (iReceveIndex < iReceiveLen); ++iTempIndex)
				{
					chSocket = chSocketOutBuffer[iReceveIndex];
					tempBuffer[iTempIndex] = chSocket;
					iReceveIndex++;
					if ((chSocket == '|') || (chSocket == '\0'))
					{
						tempBuffer[iTempIndex] = '\0';
						break;
					}
				}
				if (iTempIndex == iMaxTempLen - 1)
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
				tempBuffer[iMaxTempLen - 1] = '\0';

				switch (iVariableIndex)
				{
				case 0:
					*pUserCoorIndex = atoi(tempBuffer);
					break;
				case 1:
					memcpy(pUserCoorName, tempBuffer, strlen(tempBuffer));
					break;
				default:
					*pUserCoorPoint[iVariableIndex - 2] = (float)atof(tempBuffer);
					break;
				}
			}
			else
			{
				return PROTOCOL_CMD_RETURN_FAILED;
			}
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI SetSafeArea(unsigned int dwConnectCode, P_UI_SAFEAREA_OBJ safeAreaObj)
{
	if (!safeAreaObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|%d", 
		safeAreaObj->index,safeAreaObj->name,
		safeAreaObj->Point1[0],safeAreaObj->Point1[1],safeAreaObj->Point1[2],
		safeAreaObj->Point2[0],safeAreaObj->Point2[1],safeAreaObj->Point2[2],
		safeAreaObj->portIndex,safeAreaObj->portValue);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|%d", 
		safeAreaObj->index,safeAreaObj->name,
		safeAreaObj->Point1[0],safeAreaObj->Point1[1],safeAreaObj->Point1[2],
		safeAreaObj->Point2[0],safeAreaObj->Point2[1],safeAreaObj->Point2[2],
		safeAreaObj->portIndex,safeAreaObj->portValue);
#endif	
	
	int ret = SDKSendCommand(dwConnectCode, UICMDSETSAFEAREA, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI GetSafeArea(unsigned int dwConnectCode, int InPutbuffer, P_UI_SAFEAREA_OBJ safeAreaObj)
{
	if (!safeAreaObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
	int iOutLen = 0;
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, "%d", InPutbuffer);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d", InPutbuffer);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDGETSAFEAREA, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		int iReceiveLen = iOutLen;
		const int iMaxTempLen = 100;
		char tempBuffer[iMaxTempLen] = { 0 };
		int iVariableIndex = 0; // toolMassObj data index
		char chSocket;
		int iTempIndex = 0;// tempBuffer index
		int iReceveIndex = 0; // chSocketOutBuffer index

		float *pSafeAreaPoint[6];
		char *pSafeAreaName = NULL;
		unsigned int *pSafeAreaIndex;
		unsigned int *pSafeAreaPortIndex;
		unsigned int *pSafeAreaPortValue;

		pSafeAreaIndex = &safeAreaObj->index;
		pSafeAreaName = &safeAreaObj->name[0];
		memset(pSafeAreaName, 0x0, 30);
		pSafeAreaPoint[0] = &safeAreaObj->Point1[0];
		pSafeAreaPoint[1] = &safeAreaObj->Point1[1];
		pSafeAreaPoint[2] = &safeAreaObj->Point1[2];
		pSafeAreaPoint[3] = &safeAreaObj->Point2[0];
		pSafeAreaPoint[4] = &safeAreaObj->Point2[1];
		pSafeAreaPoint[5] = &safeAreaObj->Point2[2];
		pSafeAreaPortIndex = &safeAreaObj->portIndex;
		pSafeAreaPortValue = &safeAreaObj->portValue;


		for (iVariableIndex = 0; iVariableIndex < 10; iVariableIndex++)
		{
			if (iReceveIndex < iReceiveLen)
			{
				memset(tempBuffer, 0x0, iMaxTempLen);
				for (iTempIndex = 0; (iTempIndex < iMaxTempLen - 1) && (iReceveIndex < iReceiveLen); ++iTempIndex)
				{
					chSocket = chSocketOutBuffer[iReceveIndex];
					tempBuffer[iTempIndex] = chSocket;
					iReceveIndex++;
					if ((chSocket == '|') || (chSocket == '\0'))
					{
						tempBuffer[iTempIndex] = '\0';
						break;
					}
				}
				if (iTempIndex == iMaxTempLen - 1)
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
				tempBuffer[iMaxTempLen - 1] = '\0';

				switch (iVariableIndex)
				{
				case 0:
					*pSafeAreaIndex = atoi(tempBuffer);
					break;
				case 1:
					memcpy(pSafeAreaName, tempBuffer, strlen(tempBuffer));
					break;
				case 8:
					*pSafeAreaPortIndex = atoi(tempBuffer);
					break;
				case 9:
					*pSafeAreaPortValue = atoi(tempBuffer);
					break;
				default:
					*pSafeAreaPoint[iVariableIndex - 2] = (float)atof(tempBuffer);
					break;
				}
			}
			else
			{
				return PROTOCOL_CMD_RETURN_FAILED;
			}
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI SetFixedPoint(unsigned int dwConnectCode, P_UI_FIXEDPOINT_OBJ fixedPointObj)
{
	if (!fixedPointObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
		sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%d|%s|%.4f|%.4f|%.4f|%d|%d",
		fixedPointObj->index, fixedPointObj->name,
		fixedPointObj->point[0], fixedPointObj->point[1], fixedPointObj->point[2],
		fixedPointObj->portIndex, fixedPointObj->portValue);
#elif LINUX
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%s|%.4f|%.4f|%.4f|%d|%d",
		fixedPointObj->index, fixedPointObj->name,
		fixedPointObj->point[0], fixedPointObj->point[1], fixedPointObj->point[2],
		fixedPointObj->portIndex, fixedPointObj->portValue);
#endif		
	int ret = SDKSendCommand(dwConnectCode, UICMDSETFIXDPOINT, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI GetFixedPoint(unsigned int dwConnectCode, int InPutbuffer, P_UI_FIXEDPOINT_OBJ fixedPointObj)
{
	if (!fixedPointObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, "%d", InPutbuffer);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d", InPutbuffer);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDGETFIXDPOINT, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		int iReceiveLen = iOutLen;
		const int iMaxTempLen = 100;
		char tempBuffer[iMaxTempLen] = { 0 };
		int iVariableIndex = 0; // toolMassObj data index
		char chSocket;
		int iTempIndex = 0;// tempBuffer index
		int iReceveIndex = 0; // chSocketOutBuffer index

		float *pFixedPoint[3];
		char *pFixedPointName = NULL;
		unsigned int *pFixedPointIndex;
		unsigned int *pFixedPointPortIndex;
		unsigned int *pFixedPointPortValue;

		pFixedPointIndex = &fixedPointObj->index;
		pFixedPointName = &fixedPointObj->name[0];
		memset(pFixedPointName, 0x0, 30);
		pFixedPoint[0] = &fixedPointObj->point[0];
		pFixedPoint[1] = &fixedPointObj->point[1];
		pFixedPoint[2] = &fixedPointObj->point[2];
		pFixedPointPortIndex = &fixedPointObj->portIndex;
		pFixedPointPortValue = &fixedPointObj->portValue;


		for (iVariableIndex = 0; iVariableIndex < 7; iVariableIndex++)
		{
			if (iReceveIndex < iReceiveLen)
			{
				memset(tempBuffer, 0x0, iMaxTempLen);
				for (iTempIndex = 0; (iTempIndex < iMaxTempLen - 1) && (iReceveIndex < iReceiveLen); ++iTempIndex)
				{
					chSocket = chSocketOutBuffer[iReceveIndex];
					tempBuffer[iTempIndex] = chSocket;
					iReceveIndex++;
					if ((chSocket == '|') || (chSocket == '\0'))
					{
						tempBuffer[iTempIndex] = '\0';
						break;
					}
				}
				if (iTempIndex == iMaxTempLen - 1)
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
				tempBuffer[iMaxTempLen - 1] = '\0';

				switch (iVariableIndex)
				{
				case 0:
					*pFixedPointIndex = atoi(tempBuffer);
					break;
				case 1:
					memcpy(pFixedPointName, tempBuffer, strlen(tempBuffer));
					break;
				case 5:
					*pFixedPointPortIndex = atoi(tempBuffer);
					break;
				case 6:
					*pFixedPointPortValue = atoi(tempBuffer);
					break;
				default:
					*pFixedPoint[iVariableIndex - 2] = (float)atof(tempBuffer);
					break;
				}
			}
			else
			{
				return PROTOCOL_CMD_RETURN_FAILED;
			}
		}
	}
	return ret;
}


/*==============================================================================
	NAME: int  SetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ setNetworkInObj)
	----------------------------------------------------------------------------
	Function:机器人设置网络配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_SETPASSWORD_OBJ setNetworkInObj:设置网络配置结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI SetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ setNetworkInObj)
{
 	 int iDataLen = 0;
	 if (!setNetworkInObj)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 if (setNetworkInObj->iIPAddressLen + setNetworkInObj->iMaskAddressLen + setNetworkInObj->iGatewayAddressLen >= UISDK_MAX_SDKBUFFER_DOUBLE_LEN)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存

	 iDataLen = setNetworkInObj->iIPAddressLen;
	 memcpy(chSocketInBuffer, setNetworkInObj->IPAddress, iDataLen);
	 chSocketInBuffer[iDataLen] = '|';
	 iDataLen = setNetworkInObj->iIPAddressLen + 1;
	 memcpy(&chSocketInBuffer[iDataLen], setNetworkInObj->MaskAddress, setNetworkInObj->iMaskAddressLen);
	 iDataLen = setNetworkInObj->iIPAddressLen + setNetworkInObj->iMaskAddressLen + 1;
              chSocketInBuffer[iDataLen] = '|';
			  iDataLen = setNetworkInObj->iIPAddressLen + setNetworkInObj->iMaskAddressLen + 1 + 1;
			  memcpy(&chSocketInBuffer[iDataLen], setNetworkInObj->GatewayAddress, setNetworkInObj->iGatewayAddressLen);
			  iDataLen = setNetworkInObj->iIPAddressLen + setNetworkInObj->iMaskAddressLen + setNetworkInObj->iGatewayAddressLen + 1 + 1;
              chSocketInBuffer[iDataLen] = '\0';		
	 int ret = SDKSendCommand(dwConnectCode, UICMDSETNETCONFIG,iDataLen, (char *)chSocketInBuffer,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}

/*==============================================================================
	NAME: int  GetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ getNetworkObj)
	----------------------------------------------------------------------------
	Function:机器人设置网络配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_SETPASSWORD_OBJ:获取网络配置输入的结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetNetworkConfig(unsigned int dwConnectCode, P_UI_NETWORKCONFIG_OBJ getNetworkObj)
{
	int iDataLen = 0;
	int iIPLen = 0;
	char chSocket;
	if (!getNetworkObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETNETCONFIG,0, NULL,  chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
	    for(int i = 0; i < UISDK_MAX_DATA_LEN; i++)
	    {
	         chSocket = chSocketOutBuffer[iDataLen];
	      	iDataLen++;	 
	         if(chSocket != '|')
	         {
				 if (chSocket != '\0')
				 {
					 iIPLen++;
					 getNetworkObj->IPAddress[i] = chSocket;
				 }							 
	         }
	      else
	      {
			  getNetworkObj->IPAddress[iIPLen] = '\0';
			  getNetworkObj->iIPAddressLen = iIPLen;
	          break;
	      }
	    }
		iIPLen = 0;
	 if( iDataLen < UISDK_MAX_DATA_LEN)
	 {
	        for(int i = 0; i < UISDK_MAX_DATA_LEN; i++)
	        {
	             chSocket = chSocketOutBuffer[iDataLen];
		      iDataLen++;	 
	             if(chSocket != '|')
	             {
					 if (chSocket != '\0')
					 {
						 iIPLen++;
						 getNetworkObj->MaskAddress[i] = chSocket;
					 }								 
	             }
		      else
		      {
				  getNetworkObj->MaskAddress[iIPLen] = '\0';
				  getNetworkObj->iMaskAddressLen = iIPLen;
		          break;
		      }
	        }
	 }
	 iIPLen = 0;
	 if( iDataLen < UISDK_MAX_DATA_LEN)
	 {
	        for(int i = 0; i < UISDK_MAX_DATA_LEN; i++)
	        {
	             chSocket = chSocketOutBuffer[iDataLen];
		      iDataLen++;	 
	             if(chSocket != '|')
	             {
					 if (chSocket != '\0')
					 {
						 iIPLen++;
						 getNetworkObj->GatewayAddress[i] = chSocket;
					 }
					 else
					 {
						 getNetworkObj->GatewayAddress[iIPLen] = '\0';
						 getNetworkObj->iGatewayAddressLen = iIPLen;
						 break;
					 }
					 
	             }
		      else
		      {
				  getNetworkObj->GatewayAddress[iIPLen] = '\0';
				  getNetworkObj->iGatewayAddressLen = iIPLen;
		          break;
		      }
	        }
	 }
	}
	if (getNetworkObj->iIPAddressLen == 0 || getNetworkObj->iMaskAddressLen == 0 || getNetworkObj->iGatewayAddressLen == 0)
	{
	 return PROTOCOL_CMD_RETURN_FAILED;
	}
	return ret;		
}


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
int SDKSTDCALLAPI SetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ setDateTimeInObj)
{
	 if (!setDateTimeInObj)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
#ifdef WIN32	
    sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_LEN, "%04d%02d%02d%02d%02d%02d", 
		   	setDateTimeInObj->dt_year,setDateTimeInObj->dt_mon,setDateTimeInObj->dt_mday,setDateTimeInObj->dt_hour,setDateTimeInObj->dt_min,setDateTimeInObj->dt_sec);
#elif LINUX	
    sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%04d%02d%02d%02d%02d%02d", 
		   	setDateTimeInObj->dt_year,setDateTimeInObj->dt_mon,setDateTimeInObj->dt_mday,setDateTimeInObj->dt_hour,setDateTimeInObj->dt_min,setDateTimeInObj->dt_sec);
#endif	
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETTIME,14, (char *)chSocketInBuffer,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		  
}

/*==============================================================================
	NAME: int  GetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ getDateTimeObj)
	----------------------------------------------------------------------------
	Function:机器人获取时间
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_DATETIME_OBJ setDateTimeInObj:获取时间结构体
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetDateTime(unsigned int dwConnectCode, P_UI_DATETIME_OBJ getDateTimeObj)
{
	 int iDataLen = 0;
	 char chSocket;
	 const int iMaxTempLen = 5;
     char  tempBuffer[iMaxTempLen] = { 0 }; //  
	 if (!getDateTimeObj)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETTIME,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {      
		 // get year
	        for(int i = 0; i < 4; i++)
	        {
	             chSocket = chSocketOutBuffer[iDataLen];
		      	iDataLen++;	 
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}	
				else
				{
	               return PROTOCOL_CMD_RETURN_FAILED;
				}
	        }
		 tempBuffer[4] = '\0';
	        getDateTimeObj->dt_year = atoi(tempBuffer);
		 memset(tempBuffer,0x0,sizeof(tempBuffer));
		 // get month
	        for(int i = 0; i < 2; i++)
	        {
	            chSocket = chSocketOutBuffer[iDataLen];
		        iDataLen++;	 
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}	
				else
				{
	                return PROTOCOL_CMD_RETURN_FAILED;
				}
	        }
		 tempBuffer[2] = '\0';
	        getDateTimeObj->dt_mon = atoi(tempBuffer);
		 memset(	tempBuffer,0x0,sizeof(tempBuffer));
		 // get day
	        for(int i = 0; i < 2; i++)
	        {
	            chSocket = chSocketOutBuffer[iDataLen];
		      	iDataLen++;	 
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}	
				else
				{
	                return PROTOCOL_CMD_RETURN_FAILED;
				}
	        }
		 tempBuffer[2] = '\0';
	        getDateTimeObj->dt_mday = atoi(tempBuffer);
		 memset(	tempBuffer,0x0,sizeof(tempBuffer));
		 // get hour
	        for(int i = 0; i < 2; i++)
	        {
	            chSocket = chSocketOutBuffer[iDataLen];
		      	iDataLen++;	 
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}	
				else
				{
	                return PROTOCOL_CMD_RETURN_FAILED;
				}
	        }
		 tempBuffer[2] = '\0';
	        getDateTimeObj->dt_hour = atoi(tempBuffer);
		 memset(	tempBuffer,0x0,sizeof(tempBuffer));
		 // get min
	        for(int i = 0; i < 2; i++)
	        {
	            chSocket = chSocketOutBuffer[iDataLen];
		        iDataLen++;	 
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}	
				else
				{
	                return PROTOCOL_CMD_RETURN_FAILED;
				}
	        }
		 tempBuffer[2] = '\0';
	        getDateTimeObj->dt_min = atoi(tempBuffer);
		 memset(	tempBuffer,0x0,sizeof(tempBuffer));
		 // get sec
	        for(int i = 0; i < 2; i++)
	        {
	            chSocket = chSocketOutBuffer[iDataLen];
		        iDataLen++;	 
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}	
				else
				{
	                return PROTOCOL_CMD_RETURN_FAILED;
				}
	        }
		 tempBuffer[2] = '\0';
	        getDateTimeObj->dt_sec = atoi(tempBuffer);
	 }
	 return ret;		
}


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
int SDKSTDCALLAPI GetRobotVersion(unsigned int dwConnectCode, char *pVersion)
{
	 if (!pVersion)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETVERSION,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	        memcpy(pVersion, chSocketOutBuffer, strlen(chSocketOutBuffer));
	 }
	 return ret;			

}


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
int SDKSTDCALLAPI GetRobotStatus(unsigned int dwConnectCode, int *pStatus)
{
	if (!pStatus)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETSTATUS, 0, NULL, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{   
		if (strlen(chSocketOutBuffer) == 2)
		{
			*pStatus = atoi(chSocketOutBuffer);
		}
		else
		{

			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}


/*==============================================================================
	NAME: int  GetRobotStatus(unsigned int dwConnectCode, P_UI_RUNNINGPROGRAM_OBJ runningProgramObj)
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
int SDKSTDCALLAPI GetRunningPogram(unsigned int dwConnectCode, P_UI_RUNNINGPROGRAM_OBJ runningProgramObj)
{
	if (!runningProgramObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_FILECONTENT_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETRUNPROGRAM, 0, NULL, chSocketOutBuffer,&iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		int iDataLen = iOutLen;
		int iPos = 0;
		runningProgramObj->iResult = chSocketOutBuffer[0];
		if ((runningProgramObj->iResult == '1') && (iDataLen > 9))
		{			
			int iNameLen = iDataLen - 9;
			if(iDataLen >= UISDK_MAX_FILECONTENT_LEN)
			{
				return PROTOCOL_CMD_RETURN_FAILED;
			}
			memcpy(runningProgramObj->name, &chSocketOutBuffer[2], iDataLen);
			for(int i = 0; i < iDataLen; i++)
			{ 
			      if(runningProgramObj->name[i] == '|')
			      {
			          runningProgramObj->name[i] = '\0';
				   iNameLen = i;
				   break;
			      }			      
			}
			runningProgramObj->name[iNameLen] = '\0';
			const  int iMaxTempLen =3;
			char  tempBuffer[3] = { 0 }; //  
			char chSocket;
			iPos = iNameLen + 3;
			
			for (int i = 0; i < 2; i++)
			{
				chSocket = chSocketOutBuffer[iPos];
				iPos++;
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}
				else
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}					
			}
			tempBuffer[iMaxTempLen-1] = '\0';
			runningProgramObj->dt_mday = atoi(tempBuffer);
			for (int i = 0; i < 2; i++)
			{
				chSocket = chSocketOutBuffer[iPos];
				iPos++;
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}
				else
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
			}
			tempBuffer[iMaxTempLen-1] = '\0';
			runningProgramObj->dt_hour = atoi(tempBuffer);
			for (int i = 0; i < 2; i++)
			{
				chSocket = chSocketOutBuffer[iPos];
				iPos++;
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}
				else
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
			}
			tempBuffer[iMaxTempLen-1] = '\0';
			runningProgramObj->dt_min = atoi(tempBuffer);
			for (int i = 0; i < 2; i++)
			{
				chSocket = chSocketOutBuffer[iPos];
				iPos++;
				if (chSocket != '\0')
				{
					tempBuffer[i] = chSocket;
				}
				else
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
			}
			runningProgramObj->dt_sec = atoi(tempBuffer);

		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}
/*==============================================================================
	NAME: int  GetUserProgramList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ programListObj)
	----------------------------------------------------------------------------
	Function:获取用户程序列表
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	P_UI_PROGRAMLIST_OBJ programListObj:机器人用户程序列表
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
bool cmp(string a, string b)
{
	if (a < b) // 从小到大 
	{
		return true;
	}
	else
	{
		return false;
	}
}
int SDKSTDCALLAPI GetUserProgramList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ programListObj)
{
	if (!programListObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_FILECONTENT_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETPROGRAMLIST, 0, NULL, chSocketOutBuffer,&iOutLen);
	if (iOutLen > UISDK_MAX_FILECONTENT_LEN)
	{
		iOutLen = UISDK_MAX_FILECONTENT_LEN;
	}
	if (ret == UISDK_SUCCESS)
	{
	    char chSocket;
		int iListCount = 0;
		int iStartPos = 0; //字符串起始位置
		memcpy(programListObj->chDataBuffer, chSocketOutBuffer, iOutLen);
		vector<string> vect;
		string strtmp;
	       for(int i = 0; i < UISDK_MAX_FILECONTENT_LEN; i++)
	       {
	           chSocket = programListObj->chDataBuffer[i];	
			   // 把列表中的'|'都转成'\0'
	           if(chSocket == '|')
	           {
			programListObj->chDataBuffer[i] = '\0';
			strtmp = &programListObj->chDataBuffer[iStartPos];
			if (strtmp.length())
			{
				vect.push_back(strtmp);
				iListCount++;
			}
			
			iStartPos = i + 1;
			
	           }
		    else if(chSocket == '\0')
		    {
		        break;
		    }
	       }			   
		if (iListCount == 0)
		{
			return UISDK_SUCCESS;
		}
		else
		{
			/* 
			 // 需要支持rob,xml两种脚本文件，不能删除程序扩展名
		       for (int i = 0; i < iListCount; i++)//  重新赋值
			{
				string strFileName = vect[i];
				string strTemp = strFileName;// 把当前字符串放到临时小写字符串
				transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);  // 转成小写字符串
				size_t pos = strTemp.rfind(".rob"); // 获取扩展名位置
				int szPattern=strFileName.size();
                if (pos != -1) 
                {
                    strFileName.erase(pos);// 删除文件扩展名
					vect[i] = strFileName;
                }
							 
			}
			
			sort(vect.begin(), vect.end(), cmp); // 排序
			for (int i = 0; i < iListCount; i++)//  重新赋值
			{
				string strFileName = vect[i];
				strFileName = strFileName + ".rob";
				vect[i] = strFileName;
				
			}
			*/
			sort(vect.begin(), vect.end(), cmp); // 排序
			iStartPos = 0;
			for (int i = 0; i < iListCount; i++)//  重新赋值
			{
				memcpy(&programListObj->chDataBuffer[iStartPos], vect[i].c_str(), vect[i].length());
				iStartPos = iStartPos + vect[i].length();
				programListObj->chDataBuffer[iStartPos] = '\0';
				iStartPos = iStartPos + 1;
			}
		    programListObj->iCount =iListCount;
		}
	}
	return ret;
}
/*==============================================================================
NAME: int  GetLogList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ logListObj)
----------------------------------------------------------------------------
Function:获取用户程序列表
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_PROGRAMLIST_OBJ programListObj:机器人用户程序列表
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
int SDKSTDCALLAPI GetLogList(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ logListObj)
{
	if (!logListObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_FILECONTENT_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode, UICMDGETLOGLIST, 0, NULL, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		char chSocket;
		int iListCount = 0;
		int iStartPos = 0; //字符串起始位置
		memcpy(logListObj->chDataBuffer, chSocketOutBuffer, iOutLen);
		vector<string> vect;
		string strtmp;
		for (int i = 0; i < UISDK_MAX_DATA_LEN; i++)
		{
			chSocket = logListObj->chDataBuffer[i];
			// 把列表中的'|'都转成'\0'
			if ((chSocket == '|')||(chSocket=='\0'))
			{
				logListObj->chDataBuffer[i] = '\0';
				strtmp = &logListObj->chDataBuffer[iStartPos];
				if (strtmp.length())
				{
					vect.push_back(strtmp);
					iListCount++;
				}

				iStartPos = i + 1;

			}
			else if ((chSocket == '\0')&&(logListObj->chDataBuffer[i+1]=='\0'))
			{
				break;
			}
		}
		if (iListCount == 0)
		{
			return UISDK_SUCCESS;
		}
		else
		{

			for (int i = 1; i < iListCount; i++)//  重新赋值
			{
				string strFileName = vect[i];
				string strTemp = strFileName;// 把当前字符串放到临时小写字符串
				transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::tolower);  // 转成小写字符串
                size_t pos = strTemp.rfind(".log"); // 获取扩展名位置
                if ((int)pos != -1)
				{
					strFileName.erase(pos);// 删除文件扩展名
					vect[i] = strFileName;
				}

			}
			sort(vect.begin(), vect.end(), cmp); // 排序
			for (int i = 1; i < iListCount; i++)//  重新赋值,添加后缀名
			{
				string strFileName = vect[i];
				strFileName = strFileName + ".log";
				vect[i] = strFileName;
			}
			iStartPos = 0;
			for (int i = 0; i < iListCount; i++)//  重新赋值
			{
				memcpy(&logListObj->chDataBuffer[iStartPos], vect[i].c_str(), vect[i].length());
				iStartPos = iStartPos + vect[i].length();
				logListObj->chDataBuffer[iStartPos] = '\0';
				iStartPos = iStartPos + 1;
			}
			logListObj->iCount = iListCount;
		}
	}
	return ret;
}

/*==============================================================================
	NAME: int  DeleteUserProgram(unsigned int dwConnectCode, char *pProgramName)
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
int SDKSTDCALLAPI DeleteUserProgram(unsigned int dwConnectCode, char *pProgramName)
{
	 if (!pProgramName)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDDELETEPROGRAM, strlen(pProgramName), pProgramName, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	         if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}


/*==============================================================================
	NAME: int  GetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:获取机器人默认用户程序
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 char *pProgramName: 返回机器人默认用户程序名称
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName)
{
	 if (!pProgramName)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETDEFAULTPROGRAM,strlen(pProgramName), pProgramName,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}
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
int SDKSTDCALLAPI SetDefaultUserProgram(unsigned int dwConnectCode, char *pProgramName)
{
	 if (!pProgramName)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETDEFAULTPROGRAM,strlen(pProgramName), pProgramName,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}
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
int SDKSTDCALLAPI GetMotorTemperature(unsigned int dwConnectCode, P_UI_MOTORTEMPERATURE_OBJ motorTemperature)
{
	 if (!motorTemperature)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }			 
	 int iOutLen = 0;
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETMOTORTEMPERATURE,0, NULL,  chSocketOutBuffer,&iOutLen);
	 if (ret == UISDK_SUCCESS)
	 {
	              int iReceiveLen = iOutLen;
		       const int iMaxTempLen = 5;
		       char  tempBuffer[iMaxTempLen] = { 0 };  
			int iIndexPos = 0;
			char chSocket;
			int i = 0 ; 
			for(i = 0; i < iMaxTempLen-1; i++)
			{
			       chSocket = chSocketOutBuffer[iIndexPos];	
				tempBuffer[i] = chSocket;
				iIndexPos ++;
				if ((chSocket == '|') ||(chSocket == '\0'))
				{ 
				       tempBuffer[i] = '\0';
					break;
				}
			}
			if(i == iMaxTempLen-1)
			{
			    return PROTOCOL_CMD_RETURN_FAILED;
			}
			 tempBuffer[iMaxTempLen-1] = '\0';
			motorTemperature->iBaseTemperature = atoi(tempBuffer);
	                       for(i = 0; i < iMaxTempLen-1; i++)
			{
			       chSocket = chSocketOutBuffer[iIndexPos];	
				tempBuffer[i] = chSocket;
				iIndexPos ++;
				if ((chSocket == '|') ||(chSocket == '\0'))
				{ 
				       tempBuffer[i] = '\0';
					break;
				}
			}
			if(i == iMaxTempLen-1)
			{
			    return PROTOCOL_CMD_RETURN_FAILED;
			}
			tempBuffer[iMaxTempLen-1] = '\0';
			motorTemperature->iMainArmTemperature = atoi(tempBuffer);
	                       for(i = 0; i < iMaxTempLen-1; i++)
			{
			       chSocket = chSocketOutBuffer[iIndexPos];	
				tempBuffer[i] = chSocket;
				iIndexPos ++;
				if ((chSocket == '|') ||(chSocket == '\0'))
				{ 
				       tempBuffer[i] = '\0';
					break;
				}
			}
			if(i == iMaxTempLen-1)
			{
			    return PROTOCOL_CMD_RETURN_FAILED;
			}
			tempBuffer[iMaxTempLen-1] = '\0';
			motorTemperature->iAssistantArmTemperature = atoi(tempBuffer);
	                       if(iIndexPos < iReceiveLen)
	                       {
	                              for(i = 0; (i < iMaxTempLen-1)&&(iIndexPos < iReceiveLen); i++)
				{
				       chSocket = chSocketOutBuffer[iIndexPos];	
					tempBuffer[i] = chSocket;
					iIndexPos ++;
					if ((chSocket == '|') ||(chSocket == '\0'))
					{ 
					       tempBuffer[i] = '\0';
						break;
					}
				}
				if (i == iMaxTempLen - 1)
				{
				    return PROTOCOL_CMD_RETURN_FAILED;
				}
				tempBuffer[iMaxTempLen-1] = '\0';
				motorTemperature->iBarArmTemperature = atoi(tempBuffer);
	                       }
			else
			{
			    return PROTOCOL_CMD_RETURN_FAILED;
			}
			
	 }
	 return ret;		
}

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
int SDKSTDCALLAPI RunUserProgram(unsigned int dwConnectCode, char *pProgramName)
{
	 if (!pProgramName)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDRUNPROGRAM,strlen(pProgramName), pProgramName,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;			

}
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
int SDKSTDCALLAPI SimulatorRunUserProgram(unsigned int dwConnectCode, char *pProgramName)
{
	 if (!pProgramName)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode, UICMDSIMULATORRUNPROGRAM, strlen(pProgramName), pProgramName, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	         if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;		

}

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
int SDKSTDCALLAPI EmergyStop(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDEMERGYSTOP,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}
/*==============================================================================
	NAME: int  NormalRobotStop(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:机器人普通停止
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
int SDKSTDCALLAPI NormalRobotStop(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_NORMAL_STOP_ROBOT,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		

}


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
int SDKSTDCALLAPI ResetRobot(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDRESETROBOT,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	     if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;		

}

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
int SDKSTDCALLAPI GetRobotTCP(unsigned int dwConnectCode, double *pTcpLength, double *pTcpHeight, double *pTcpAgnle)
{
	if ((!pTcpLength) || (!pTcpHeight)|| (!pTcpAgnle))
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_GET_ROBOT_TCP, 0, NULL, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		const int iMaxTempLen = 100;
		char  tempBuffer[iMaxTempLen] = { 0 };  
		int iIndexPos = 0;
		char chSocket;
		int i = 0;
         double *pfRobotTCP[3];
		 pfRobotTCP[0] = pTcpLength;
		 pfRobotTCP[1] = pTcpHeight;
		 pfRobotTCP[2] = pTcpAgnle;
		 for (int iIndex = 0; iIndex < 3; ++iIndex)
		 {
			 if (iIndexPos < iOutLen)
			 {
				 for (i = 0; (i < iMaxTempLen - 1) && (iIndexPos < iOutLen); i++)
				 {
					 chSocket = chSocketOutBuffer[iIndexPos];
					 tempBuffer[i] = chSocket;
					 iIndexPos++;
					 if ((chSocket == '|') || (chSocket == '\0'))
					 {
						 tempBuffer[i] = '\0';
						 break;
					 }
				 }
				 if (i == iMaxTempLen - 1)
				 {
					 return PROTOCOL_CMD_RETURN_FAILED;
				 }
				 tempBuffer[iMaxTempLen - 1] = '\0';
                 *pfRobotTCP[iIndex] = atof(tempBuffer);
			 }
			 else
			 {
				 return PROTOCOL_CMD_RETURN_FAILED;
			 }
		}
	}
	else
	{
		return PROTOCOL_CMD_RETURN_FAILED;
	}
	return ret;		
}
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
int SDKSTDCALLAPI SetRobotTCP(unsigned int dwConnectCode,double TcpLength, double TcpHeight, double TcpAngle)
{
     char buffSend[100] = { 0 };// 输入值缓存
#ifdef WIN32	
    sprintf_s(buffSend, "%.4f|%.4f|%.4f", (float)TcpLength,(float)TcpHeight,(float)TcpAngle);
#elif LINUX	
    sprintf_s(buffSend, sizeof buffSend, "%.4f|%.4f|%.4f", (float)TcpLength,(float)TcpHeight,(float)TcpAngle);
#endif	
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode, UI_CMD_SET_ROBOT_TCP, strlen(buffSend), buffSend, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	     if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}


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
int SDKSTDCALLAPI SetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
{
	 if (!initializeTCP)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", initializeTCP->fTCPx, initializeTCP->fTCPy, initializeTCP->fTCPz, initializeTCP->fTCPRz, initializeTCP->fThetaX, initializeTCP->fThetaY, initializeTCP->fThetaZ, initializeTCP->fToolZ);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", initializeTCP->fTCPx, initializeTCP->fTCPy, initializeTCP->fTCPz, initializeTCP->fTCPRz, initializeTCP->fThetaX, initializeTCP->fThetaY, initializeTCP->fThetaZ, initializeTCP->fToolZ);
#endif	
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETTOOLINITIALIZE,strlen(chSocketInBuffer), chSocketInBuffer,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	     if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}

/*==============================================================================
	NAME: int  GetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
	----------------------------------------------------------------------------
	Function:获取机器人初始工具位置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode: 
		 P_UI_INITIALIZETCP_OBJ robotTCP:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetInitializeTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
{
	 if (!initializeTCP)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
 	 int iOutLen = 0;
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETTOOLINITIALIZE,0, NULL, chSocketOutBuffer,&iOutLen);
	 if (ret == UISDK_SUCCESS)
	 {
	              int iReceiveLen = iOutLen;
		       const int iMaxTempLen = 100;
		       char  tempBuffer[iMaxTempLen] = { 0 };  
			int iIndexPos = 0;
			char chSocket;
			int i = 0 ; 
			float *fTCPPointer[8];
			fTCPPointer[0] = &initializeTCP->fTCPx;
			fTCPPointer[1] = &initializeTCP->fTCPy;
			fTCPPointer[2] = &initializeTCP->fTCPz;
			fTCPPointer[3] = &initializeTCP->fTCPRz;
			fTCPPointer[4] = &initializeTCP->fThetaX;
			fTCPPointer[5] = &initializeTCP->fThetaY;
			fTCPPointer[6] = &initializeTCP->fThetaZ;
			fTCPPointer[7] = &initializeTCP->fToolZ;
			for(int iIndex = 0; iIndex < 8; ++iIndex)
			{			
				if(iIndexPos < iReceiveLen)
				{
					for(i = 0; (i < iMaxTempLen-1)&&(iIndexPos < iReceiveLen); i++)
					{
						chSocket = chSocketOutBuffer[iIndexPos];	
						tempBuffer[i] = chSocket;
						iIndexPos ++;
						if ((chSocket == '|') ||(chSocket == '\0'))
						{ 
							tempBuffer[i] = '\0';
							break;
						}
					}
					if(i == iMaxTempLen-1)
					{
						return PROTOCOL_CMD_RETURN_FAILED;
					}
					tempBuffer[iMaxTempLen-1] = '\0';
					*fTCPPointer[iIndex] = (float)atof(tempBuffer);
				}
				else
				{
				    return PROTOCOL_CMD_RETURN_FAILED;
				}
			}

			
	 }
	 return ret;	
}

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
int SDKSTDCALLAPI SetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
{
	 if (!initializeTCP)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%d|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", initializeTCP->iSpeedRatio,  initializeTCP->fTCPx,initializeTCP->fTCPy,initializeTCP->fTCPz,initializeTCP->fTCPRz, initializeTCP->fThetaX,initializeTCP->fThetaY,initializeTCP->fThetaZ, initializeTCP->fToolZ);
#elif LINUX	
	 sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", initializeTCP->iSpeedRatio,  initializeTCP->fTCPx,initializeTCP->fTCPy,initializeTCP->fTCPz,initializeTCP->fTCPRz, initializeTCP->fThetaX,initializeTCP->fThetaY,initializeTCP->fThetaZ, initializeTCP->fToolZ);
#endif	
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETTOOLCOORDINATE, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;			

}


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

int SDKSTDCALLAPI SetTargetTCPEx(unsigned int dwConnectCode, P_UI_INITIALIZETCPEX_OBJ initializeTCPEX)
{
	 if (!initializeTCPEX)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
#ifdef WIN32	
	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%d|%d|%d|%d|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", initializeTCPEX->iArmSpeed[0],initializeTCPEX->iArmSpeed[1],initializeTCPEX->iArmSpeed[2],initializeTCPEX->iArmSpeed[3],initializeTCPEX->fTCPx,initializeTCPEX->fTCPy,initializeTCPEX->fTCPz,initializeTCPEX->fTCPRz, initializeTCPEX->fThetaX,initializeTCPEX->fThetaY,initializeTCPEX->fThetaZ, initializeTCPEX->fToolZ);
#elif LINUX	
	 sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%d|%d|%d|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", initializeTCPEX->iArmSpeed[0],initializeTCPEX->iArmSpeed[1],initializeTCPEX->iArmSpeed[2],initializeTCPEX->iArmSpeed[3],  initializeTCPEX->fTCPx,initializeTCPEX->fTCPy,initializeTCPEX->fTCPz,initializeTCPEX->fTCPRz, initializeTCPEX->fThetaX,initializeTCPEX->fThetaY,initializeTCPEX->fThetaZ, initializeTCPEX->fToolZ);
#endif	
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_SETPOINTEX, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;			

}


/*==============================================================================
	NAME: int  GetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
	----------------------------------------------------------------------------
	Function:获取机器人工具坐标
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_INITIALIZETCP_OBJ robotTCP:机器人初始工具位置数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetTargetTCP(unsigned int dwConnectCode, P_UI_INITIALIZETCP_OBJ initializeTCP)
{
	 if (!initializeTCP)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
 	 int iOutLen = 0;
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETTOOLCOORDINATE,0, NULL,  chSocketOutBuffer,&iOutLen);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (iOutLen == 4)
		 {
			 memcpy(&ret, chSocketOutBuffer, 4);
			 return ret;
		 }
		 int iReceiveLen = iOutLen;
		 const int iMaxTempLen = 100;
		 char  tempBuffer[iMaxTempLen] = { 0 };  
		 int iIndexPos = 0;
		 char chSocket;
		 int i = 0;
		 float *fTCPPointer[8];
		 fTCPPointer[0] = &initializeTCP->fTCPx;
		 fTCPPointer[1] = &initializeTCP->fTCPy;
		 fTCPPointer[2] = &initializeTCP->fTCPz;
		 fTCPPointer[3] = &initializeTCP->fTCPRz;
		 fTCPPointer[4] = &initializeTCP->fThetaX;
		 fTCPPointer[5] = &initializeTCP->fThetaY;
		 fTCPPointer[6] = &initializeTCP->fThetaZ;
		 fTCPPointer[7] = &initializeTCP->fToolZ;
		 for (int iIndex = 0; iIndex < 8; ++iIndex)
		 {
			 if (iIndexPos < iReceiveLen)
			 {
				 for (i = 0; (i < iMaxTempLen - 1) && (iIndexPos < iReceiveLen); i++)
				 {
					 chSocket = chSocketOutBuffer[iIndexPos];
					 tempBuffer[i] = chSocket;
					 iIndexPos++;
					 if ((chSocket == '|') || (chSocket == '\0'))
					 {
						 tempBuffer[i] = '\0';
						 break;
					 }
				 }
				 if (i == iMaxTempLen - 1)
				 {
					 return PROTOCOL_CMD_RETURN_FAILED;
				 }
				 tempBuffer[iMaxTempLen - 1] = '\0';
				 *fTCPPointer[iIndex] = (float)atof(tempBuffer);
			 }
			 else
			 {
				 return PROTOCOL_CMD_RETURN_FAILED;
			 }
		 }
	 }
	 return ret;		
}

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
int SDKSTDCALLAPI LooseAllJoint(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDOFFMOTOR,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	        if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;		
}

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
int SDKSTDCALLAPI LockAllJoint(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETONMOTOR,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	        if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;			
}


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
int SDKSTDCALLAPI GetJointAngle(unsigned int dwConnectCode, P_UI_JOINTANAGLE_OBJ jointAngle)
{
	if (!jointAngle)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
 	 int iOutLen = 0;
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETTOOLCENTRECOORDINATE,0, NULL,  chSocketOutBuffer,&iOutLen);
	if (ret == UISDK_SUCCESS)
	{
	          int iReceiveLen = iOutLen;
	       const int iMaxTempLen = 100;
	       char  tempBuffer[iMaxTempLen] = { 0 };  
		int iIndexPos = 0;
		char chSocket;
		int i = 0 ; 
		for(i = 0; i < iMaxTempLen-1; i++)
		{
		       chSocket = chSocketOutBuffer[iIndexPos];	
			tempBuffer[i] = chSocket;
			iIndexPos ++;
			if ((chSocket == '|') ||(chSocket == '\0'))
			{ 
			       tempBuffer[i] = '\0';
				break;
			}
		}
		if(i == iMaxTempLen-1)
		{
		    return PROTOCOL_CMD_RETURN_FAILED;
		}
		 tempBuffer[iMaxTempLen-1] = '\0';
		jointAngle->fBaseAngle = atof(tempBuffer);
	                   for(i = 0; i < iMaxTempLen-1; i++)
		{
		       chSocket = chSocketOutBuffer[iIndexPos];	
			tempBuffer[i] = chSocket;
			iIndexPos ++;
			if ((chSocket == '|') ||(chSocket == '\0'))
			{ 
			       tempBuffer[i] = '\0';
				break;
			}
		}
		if(i == iMaxTempLen-1)
		{
		    return PROTOCOL_CMD_RETURN_FAILED;
		}
		tempBuffer[iMaxTempLen-1] = '\0';
		jointAngle->fMainArmAngle = atof(tempBuffer);
			for(i = 0; i < iMaxTempLen-1; i++)
		{
		       chSocket = chSocketOutBuffer[iIndexPos];	
			tempBuffer[i] = chSocket;
			iIndexPos ++;
			if ((chSocket == '|') ||(chSocket == '\0'))
			{ 
			       tempBuffer[i] = '\0';
				break;
			}
		}
		if(i == iMaxTempLen-1)
		{
		    return PROTOCOL_CMD_RETURN_FAILED;
		}
		tempBuffer[iMaxTempLen-1] = '\0';
		jointAngle->fAssistantArmAngle = atof(tempBuffer);  
		if(iIndexPos < iReceiveLen)
	                   {
	                          for(i = 0; (i < iMaxTempLen-1)&&(iIndexPos < iReceiveLen); i++)
			{
			       chSocket = chSocketOutBuffer[iIndexPos];	
				tempBuffer[i] = chSocket;
				iIndexPos ++;
				if ((chSocket == '|') ||(chSocket == '\0'))
				{ 
				       tempBuffer[i] = '\0';
					break;
				}
			}
			if(i == iMaxTempLen-1)
			{
			    return PROTOCOL_CMD_RETURN_FAILED;
			}
			tempBuffer[iMaxTempLen-1] = '\0';
			jointAngle->fBarArmAngle = atof(tempBuffer);
	                   }
		else
		{
		    return PROTOCOL_CMD_RETURN_FAILED;
		}
		
	}
	return ret;		
}

/*==============================================================================
	NAME: int MoveArmJoint(unsigned int dwConnectCode, int iDirectoryIndex)
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

	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI MoveArmJoint(unsigned int dwConnectCode, int iDirectoryIndex, int iSpeed)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
#ifdef WIN32	
 	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_LEN, "%d|%d",iDirectoryIndex,iSpeed);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%d", iDirectoryIndex, iSpeed);
#endif	
	 int ret = SDKSendCommand(dwConnectCode,UICMDMOVEMOTOR,strlen(chSocketInBuffer), chSocketInBuffer,chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;			
}


int SDKSTDCALLAPI MoveJointToTargetPos(unsigned int dwConnectCode, int iSpeed,int iType, double x, double y, double z, double rz, double theta_1, double theta_2, double theta_3, double Toolz)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%d|%d|%f|%f|%f|%f|%f|%f|%f|%f", iSpeed, iType, x, y, z, rz, theta_1, theta_2, theta_3, Toolz);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%d|%f|%f|%f|%f|%f|%f|%f|%f", iSpeed, iType, x, y, z, rz, theta_1, theta_2, theta_3, Toolz);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDMOVEMOTOR, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		}
	}
	return ret;
}
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
int SDKSTDCALLAPI GetInPortDigitalValue(unsigned int dwConnectCode,P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETIOINPUTVALUE, 0, NULL, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			int iDigitalValue = 0;
			int iIndexPos = 2;
			char chSocket;
			for (int i = 0; i < ROBOTINPUTPORTNUM; i++)
			{
				chSocket = chSocketOutBuffer[iIndexPos + i];
				if (chSocket == '\0')
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
				else  if (chSocket == '1')
				{
					iDigitalValue = iDigitalValue | (0x1 << (i));
				}
			}
			digitalValueObj->uiDigitalValue = iDigitalValue;
			return UISDK_SUCCESS;
		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}

/*==============================================================================
	NAME: int  SetDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel)
	----------------------------------------------------------------------------
	Function:设置某输出IO端口数字输出值
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int iPortIndex:输出端口索引号:1~ROBOTOUTPUTPORTNUM
		 bool bHighLevel:输出电平高或低:TRUE表示高电平；FALSE表示低电平
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI SetOutPortDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel)
{
	 if ((iPortIndex<1)||(iPortIndex>ROBOTOUTPUTPORTNUM))
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 if((iPortIndex <= 0) && (iPortIndex > ROBOTOUTPUTPORTNUM)) return PROTOCOL_CMD_ERROR_PARAMETER;
#ifdef WIN32	
	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_LEN, "%d|%d", iPortIndex,bHighLevel==true?1:0);
#elif LINUX	
	 sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%d", iPortIndex,bHighLevel==true?1:0);
#endif		 
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETIOOUPUTVALUE,strlen(chSocketInBuffer), chSocketInBuffer,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	           if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		       return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}
/*==============================================================================
	NAME: int  SetDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel)
	----------------------------------------------------------------------------
	Function:设置某输出IO端口数字输出值
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int iPortIndex:输出端口索引号:1~ROBOTOUTPUTPORTNUM
		 bool bHighLevel:输出电平高或低:TRUE表示高电平；FALSE表示低电平
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI SetRobotOutPortValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel)
{
	 if ((iPortIndex<1)||(iPortIndex>ROBOTOUTPUTPORTNUM))
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 if((iPortIndex <= 0) && (iPortIndex > ROBOTOUTPUTPORTNUM)) return PROTOCOL_CMD_ERROR_PARAMETER;
#ifdef WIN32	
	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_LEN, "%d|%d", iPortIndex,bHighLevel==true?1:0);
#elif LINUX	
	 sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%d", iPortIndex,bHighLevel==true?1:0);
#endif		 
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETIOOUPUTVALUE,strlen(chSocketInBuffer), chSocketInBuffer,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	           if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		       return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}
/*==============================================================================
	NAME: int  GetOutPortDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool *pHighLevel)
	----------------------------------------------------------------------------
	Function:获取某输出IO端口数字输出值
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int iPortIndex:输出端口索引号:1~ROBOTOUTPUTPORTNUM
		 bool *pHighLevel:返回输出电平高或低缓冲区:TRUE表示高电平；FALSE表示低电平
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetRobotOutPortValue(unsigned int dwConnectCode, int iPortIndex, bool *pHighLevel)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 if((iPortIndex <= 0) && (iPortIndex > ROBOTOUTPUTPORTNUM)) return PROTOCOL_CMD_ERROR_PARAMETER;

	 int ret = SDKSendCommand(dwConnectCode,UICMDGETIOOUTPUTVALUE, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 int iIndexPos = 2;
			 char chSocket;
			 if(pHighLevel)
			 {
			    chSocket = chSocketOutBuffer[(iPortIndex - 1) + iIndexPos];
				if(chSocket == '1')
				 {
					 *pHighLevel = true;
				 }
				else
				{
				    *pHighLevel = false;
				}
				
			 }			 
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;
}
/*==============================================================================
	NAME: int  SetDigitalValue(unsigned int dwConnectCode, int iPortIndex, bool bHighLevel)
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
int SDKSTDCALLAPI GetOutPortDigitalValue(unsigned int dwConnectCode,P_ROBOTDIGITAL_VALUE_OBJ digitalValueObj)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETIOOUTPUTVALUE,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	        if (chSocketOutBuffer[0] == '1')
		 {
		        int iDigitalValue = 0;
			 int iIndexPos = 2;
			 char chSocket;
		        for(int i = 0; i < ROBOTOUTPUTPORTNUM; i++)
		        {
		             chSocket = chSocketOutBuffer[iIndexPos+i];
		             if(chSocket == '\0')
		             {
		                return PROTOCOL_CMD_RETURN_FAILED;
		             }
			      else  if(chSocket == '1')
		             {
		                  iDigitalValue = iDigitalValue | (0x1<<(i));
		             }
		        }
			  digitalValueObj->uiDigitalValue = iDigitalValue;
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		       return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}
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
int SDKSTDCALLAPI GetRobotAnalogValue(unsigned int dwConnectCode, int iNodeId, int * pAnalogValue)
{
	 if ((!pAnalogValue)||((iNodeId < 11)||(iNodeId > 14)))
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
#ifdef WIN32	
       sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_LEN, "%02d", iNodeId);
#elif LINUX	
       sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%02d", iNodeId);
#endif	

 	 int iOutLen = 0;
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETIOANALOGY,strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer, &iOutLen);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
// snprintf(buffer,90,  "1|%02d|%d", nodeId, iValue);
			 *pAnalogValue = atoi(&chSocketOutBuffer[5]);
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
		       return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}


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
int SDKSTDCALLAPI Shutdown(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDSHUTDOWN,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	         if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;		
}

/*==============================================================================
	NAME: int  DownloadProgramFileName(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:应用层向控制层传输文件名字
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件名数据 
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI DownloadProgramFileName(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
{
 	 if (!robotFileObj)
	 {
	      return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 if(robotFileObj->iCount >UISDK_MAX_USERFILENAME_LENGTH)
	 {
	      return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	  int ret = SDKSendCommand(dwConnectCode,UICMDDOWNLOADFILENAME, robotFileObj->iCount, robotFileObj->chDataBuffer, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	        if (chSocketOutBuffer[0] == '1')
		 {
			return UISDK_SUCCESS;
		 }
		 else 
		 {
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}


/*==============================================================================
	NAME: int  DownloadProgramFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:应用层向控制层传输文件内容
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件内容数据 
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI DownloadProgramFileContent(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
{
 	 if (!robotFileObj)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	  int ret = SDKSendCommand(dwConnectCode,UICMDDOWNLOADFILECONTENT, robotFileObj->iCount, robotFileObj->chDataBuffer, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	 	 if (chSocketOutBuffer[0] == '1')
 		{
 		    return UISDK_SUCCESS;
 		}
		  else 
		 {
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}
/*==============================================================================
	NAME: int  UpProgramFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
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
int SDKSTDCALLAPI UpProgramFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
{
	unsigned int dwConnectType = GetConnectDevType(dwConnectCode);	 
	unsigned int dwGWIndex = GetDevID(dwConnectCode);
	switch (dwConnectType)
      	{
      	     case UICONNECT_IP:
	     {		 	
			 return g_SocketClient[dwGWIndex].m_pSocketClient->UpProgramFile(dwConnectCode,robotFileObj);
	     }				
	     break;
      	    
      	}
	return PROTOCOL_CMD_ERROR_PARAMETER;
}
/*==============================================================================
	NAME: int  UpSimulateFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
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
int SDKSTDCALLAPI UpSimulateFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
{
	unsigned int dwConnectType = GetConnectDevType(dwConnectCode);	 
	unsigned int dwGWIndex = GetDevID(dwConnectCode);
	switch (dwConnectType)
      	{
      	     case UICONNECT_IP:
	     {		 	
			 return g_SocketClient[dwGWIndex].m_pSocketClient->UpSimulatorFile(dwConnectCode,robotFileObj);
	     }				
	     break;
      	}
	return PROTOCOL_CMD_ERROR_PARAMETER;
}
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
int SDKSTDCALLAPI EnterCANMode(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDENTERCANMODE,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
	         if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
		       return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}


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
int SDKSTDCALLAPI ExitCANMode(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDEXITCANMODE,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			   return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}


/*==============================================================================
	NAME: int  SendCANData(unsigned int dwConnectCode, P_UI_SOCKETMSG_OBJ uiCANMsgObj)
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
int SDKSTDCALLAPI SendCANData(unsigned int dwConnectCode, P_UI_ROBOTSOCKETMSG_OBJ uiCANMsgObj,P_UI_ROBOTSOCKETMSG_OBJ uiCANMsgObjOut)
{ 
        if(!uiCANMsgObj)
        {
      	     return PROTOCOL_CMD_ERROR_PARAMETER;
      	 }
		char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
		char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
	    memcpy(chSocketInBuffer,uiCANMsgObj,sizeof(UI_ROBOTSOCKETMSG_OBJ));
		int ret = SDKSendCommand(dwConnectCode, UICMDTRANSLATECANDATA, sizeof(UI_ROBOTSOCKETMSG_OBJ), chSocketInBuffer, chSocketOutBuffer);
		if (uiCANMsgObjOut)
		{
			memcpy(uiCANMsgObjOut, chSocketOutBuffer, sizeof(UI_ROBOTSOCKETMSG_OBJ));
		}
	 return ret;		
}



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
int SDKSTDCALLAPI getRototType(unsigned int dwConnectCode, int *piRobotType)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode,UICMDGETROBOTTYPE,0, NULL,chSocketOutBuffer ,NULL);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			if ((chSocketOutBuffer[1] == '0') && (chSocketOutBuffer[2] == '1'))
			{		
			    if(piRobotType)*piRobotType = UIROBOTBASETYPE_SCARA;
			}
		}
		else if (chSocketOutBuffer[0] == '2')
		{
			if ((chSocketOutBuffer[1] == '0') && (chSocketOutBuffer[2] == '1'))
			{
				if(piRobotType)*piRobotType =   UIROBOTBASETYPE_DELTA;
			}
		}        
	}
	return ret; 
}

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
int SDKSTDCALLAPI setRototType(unsigned int dwConnectCode, int iType)
{

	return UISDK_SUCCESS;
}


/*==============================================================================
	NAME: int  GetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode)
	----------------------------------------------------------------------------
	Function:获取机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_CAMERAMODE_OBJ cameraMode:机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode)
{
	if (!cameraMode)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode, UICMDGETCAMERACONFIGURE, 0, NULL, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		if (iOutLen > 0)
		{
			memcpy(cameraMode, chSocketOutBuffer, iOutLen);
		}

		
	}
	return ret; 	
}

/*==============================================================================
	NAME: int  SetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode)
	----------------------------------------------------------------------------
	Function:设置机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 P_UI_CAMERAMODE_OBJ cameraMode:机器人视觉系统参数配置
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/

int SDKSTDCALLAPI SetCameraConfigure(unsigned int dwConnectCode, P_UI_CAMERAMODE_OBJ cameraMode)
{
	 if (!cameraMode)
	 {
		return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 缓存
	 memcpy(chSocketInBuffer,cameraMode,sizeof(UI_CAMERAMODE_OBJ));
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETCAMERACONFIGURE,sizeof(UI_CAMERAMODE_OBJ), chSocketInBuffer,	chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			   if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else 
		 {
			  return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}


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
int SDKSTDCALLAPI EnterCameraMode(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDENTERCAMERAMODE,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			   return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}
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
int SDKSTDCALLAPI ExitCameraMode(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDEXITCAMERAMODE,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			   return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;		
}
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
int SDKSTDCALLAPI SetMACAddress(unsigned int dwConnectCode, char *pMACAddress)
{

	 if (!pMACAddress)
	 {
		return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETMACADDRESS,strlen(pMACAddress), pMACAddress,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 ret =	UISDK_SUCCESS;
		 }
		 else
		 {
			 ret = PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }	
	 return ret;
}


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
int SDKSTDCALLAPI GetMACAddress(unsigned int dwConnectCode, char *pMACAddress)
{
	 if (!pMACAddress)
	 {
		return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETMACADDRESS,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			memcpy(pMACAddress, chSocketOutBuffer, strlen(chSocketOutBuffer));
	 }
	 else
	 {
		ret = PROTOCOL_CMD_RETURN_FAILED;
	 }
	 return ret;			

}
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
int SDKSTDCALLAPI SetSerialNumber(unsigned int dwConnectCode, char *pSerialNumber)
{

	 if (!pSerialNumber)
	 {
		return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDSETSERIALNUMBER,strlen(pSerialNumber), pSerialNumber, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			 if (chSocketOutBuffer[0] == '1')
		 {
			 ret =	UISDK_SUCCESS;
		 }
		 else
		 {
			 ret = PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }	
	 return ret;
}


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
int SDKSTDCALLAPI GetSerialNumber(unsigned int dwConnectCode, char *pSerialNumber)
{
	 if (!pSerialNumber)
	 {
		return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UICMDGETSERIALNUMBER,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
			memcpy(pSerialNumber, chSocketOutBuffer, strlen(chSocketOutBuffer));
	 }
	 else
	 {
		ret = PROTOCOL_CMD_RETURN_FAILED;
	 }
	 return ret;			

}


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
int SDKSTDCALLAPI GetUserVariable(unsigned int dwConnectCode, float *pfVariableValue)
{
	if (!pfVariableValue)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_GETUSERVARIABLE, 0, NULL, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] != 0)
		{
			int iPosIndex = 0;
			int iCount = 0;
			char tempBuffer[100] = { 0 };
			memcpy(tempBuffer, chSocketOutBuffer, 2);
			iPosIndex = iPosIndex + 3;
			iCount = atoi(tempBuffer);
			if ((iCount > 0) && (iCount < 200))
			{
				for (int i = 0; i < iCount; i++)
				{
					memcpy(&pfVariableValue[i], &chSocketOutBuffer[iPosIndex], sizeof(float));
					iPosIndex = iPosIndex + sizeof(float);
				}
			}
			return UISDK_SUCCESS;
		}
	}
	else
	{
		ret = PROTOCOL_CMD_RETURN_FAILED;
	}
	return ret;

}


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
int SDKSTDCALLAPI GetRobotConfig(unsigned int dwConnectCode, P_UI_ROBOTCONFIG_OBJ pRobotConfig)
{
	if (!pRobotConfig)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN] = { 0 }; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN] = { 0 }; // 返回值缓存
#ifdef WIN32 
	sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%s|%s|", pRobotConfig->title, pRobotConfig->key);
#elif LINUX	
	sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%s|%s|", pRobotConfig->title, pRobotConfig->key);
#endif		
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_GETROBOTCONFIG, strlen(chSocketInBuffer),chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			char *p = NULL;
			char strDelim[] = "|";
#ifdef WIN32
            char *next_token = NULL;
            p = strtok_s(chSocketOutBuffer, strDelim, &next_token);
#elif LINUX
			p = strtok(chSocketOutBuffer, strDelim);
#endif			
			
			if (p)
			{// title
#ifdef WIN32
				p = strtok_s(NULL, strDelim, &next_token);
#elif LINUX
				p = strtok(NULL, strDelim);
#endif	
				if (p)
				{// key
#ifdef WIN32
					p = strtok_s(NULL, strDelim, &next_token);
#elif LINUX
					p = strtok(NULL, strDelim);
#endif					
					
					if (p)
					{// value
#ifdef WIN32
						p = strtok_s(NULL, strDelim, &next_token);
#elif LINUX
						p = strtok(NULL, strDelim);
#endif	
						
						if (p)
						{
					#ifdef WIN32 
						sprintf_s(pRobotConfig->value, 20, "%s", p);
					#elif LINUX	
						sprintf_s(pRobotConfig->value, sizeof pRobotConfig->value, "%s", p);
					#endif	
							return UISDK_SUCCESS;
						}
						
					}
				}
			}			
		}
		return PROTOCOL_CMD_RETURN_FAILED;
	}
	else
	{
		ret = PROTOCOL_CMD_RETURN_FAILED;
	}
	return ret;
}


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
int SDKSTDCALLAPI SetRobotConfig(unsigned int dwConnectCode, P_UI_ROBOTCONFIG_OBJ pRobotConfig)
{
	if (!pRobotConfig)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN] = { 0 }; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN] = { 0 }; // 返回值缓存
	#ifdef WIN32 
		sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%s|%s|%s|", pRobotConfig->title, pRobotConfig->key, pRobotConfig->value);
	#elif LINUX	
		sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%s|%s|%s|", pRobotConfig->title, pRobotConfig->key, pRobotConfig->value);
	#endif	
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_SETROBOTCONFIG, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		return PROTOCOL_CMD_RETURN_FAILED;
	}
	else
	{
		ret = PROTOCOL_CMD_RETURN_FAILED;
	}
	return ret;
}
/*==============================================================================
NAME: int SetCachePara(unsigned int dwConnectCode, P_UI_CACHE_OBJ cacheObj)
----------------------------------------------------------------------------
Function:机器人设置工具负载参数
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_CACHE_OBJ cacheObj:设置缓存点结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
int SDKSTDCALLAPI SetCachePara(unsigned int dwConnectCode, P_UI_CACHE_OBJ cacheObj)
{
	if (!cacheObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN] = { 0 }; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN] = { 0 }; // 返回值缓存
	#ifdef WIN32 
		sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", cacheObj->index, cacheObj->name, cacheObj->toolPos[0], cacheObj->toolPos[1], cacheObj->toolPos[2], cacheObj->toolPos[3],
			cacheObj->jointPos[0], cacheObj->jointPos[1], cacheObj->jointPos[2], cacheObj->jointPos[3]);
	#elif LINUX	
		sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%d|%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f", cacheObj->index, cacheObj->name, cacheObj->toolPos[0], cacheObj->toolPos[1], cacheObj->toolPos[2], cacheObj->toolPos[3],
			cacheObj->jointPos[0], cacheObj->jointPos[1], cacheObj->jointPos[2], cacheObj->jointPos[3]);
	#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDSETCACHE, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}
/*==============================================================================
NAME: int GetCachePara(unsigned int dwConnectCode, int InPutbuffer, P_UI_CACHE_OBJ cacheObj)
----------------------------------------------------------------------------
Function:机器人缓存点获取
----------------------------------------------------------------------------
Parameter:
unsigned int dwConnectCode:连接通讯编号
P_UI_CACHE_OBJ cacheObj:设置缓存点结构体
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/
int SDKSTDCALLAPI GetCachePara(unsigned int dwConnectCode, int InPutbuffer, P_UI_CACHE_OBJ cacheObj)
{
	if (!cacheObj)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iOutLen = 0;
	char buffSend[10] = { 0 };
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN] = { 0 }; // 返回值缓存
#ifdef WIN32 
	sprintf_s(buffSend, "%d", InPutbuffer);
#elif LINUX	
	sprintf_s(buffSend, sizeof buffSend, "%d", InPutbuffer);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDGETCACHE, strlen(buffSend), buffSend, chSocketOutBuffer, &iOutLen);
	if (ret == UISDK_SUCCESS)
	{
		int iReceiveLen = iOutLen;
		const int iMaxTempLen = 100;
		char tempBuffer[iMaxTempLen] = { 0 };
		int iVariableIndex = 0; // toolMassObj data index
		char chSocket;
		int iTempIndex = 0;// tempBuffer index
		int iReceveIndex = 0; // chSocketOutBuffer index
		float *pMassToolPoint[8];
		char *pMassToolName = NULL;
		unsigned int *pMassToolIndex;
		pMassToolIndex = &cacheObj->index;
		pMassToolName = &cacheObj->name[0];
		memset(pMassToolName, 0x0, 30);
		pMassToolPoint[0] = &cacheObj->toolPos[0];
		pMassToolPoint[1] = &cacheObj->toolPos[1];
		pMassToolPoint[2] = &cacheObj->toolPos[2];
		pMassToolPoint[3] = &cacheObj->toolPos[3];
		pMassToolPoint[4] = &cacheObj->jointPos[0];
		pMassToolPoint[5] = &cacheObj->jointPos[1];
		pMassToolPoint[6] = &cacheObj->jointPos[2];
		pMassToolPoint[7] = &cacheObj->jointPos[3];
		for (iVariableIndex = 0; iVariableIndex < 10; iVariableIndex++)
		{
			if (iReceveIndex < iReceiveLen)
			{
				memset(tempBuffer, 0x0, iMaxTempLen);
				for (iTempIndex = 0; (iTempIndex < iMaxTempLen - 1) && (iReceveIndex < iReceiveLen); ++iTempIndex)
				{
					chSocket = chSocketOutBuffer[iReceveIndex];
					tempBuffer[iTempIndex] = chSocket;
					iReceveIndex++;
					if ((chSocket == '|') || (chSocket == '\0'))
					{
						tempBuffer[iTempIndex] = '\0';
						break;
					}
				}
				if (iTempIndex == iMaxTempLen - 1)
				{
					return PROTOCOL_CMD_RETURN_FAILED;
				}
				tempBuffer[iMaxTempLen - 1] = '\0';

				switch (iVariableIndex)
				{
				case 0:
					*pMassToolIndex = atoi(tempBuffer);
					break;
				case 1:
					memcpy(pMassToolName, tempBuffer, strlen(tempBuffer));
					break;
				default:
					*pMassToolPoint[iVariableIndex - 2] = (float)atof(tempBuffer);
					break;
				}
			}
			else
			{
				return PROTOCOL_CMD_RETURN_FAILED;
			}
		}
	}
	return ret;
}


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
int SDKSTDCALLAPI SetRobotMove(unsigned int dwConnectCode, P_XMLNodePoint pNodePoint, int iTotalNodeCount)
{
	if (!pNodePoint)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iTotalDataLength = iTotalNodeCount*sizeof(XMLNodePoint);
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_FILECONTENT_LEN]={0}; // 缓存
	int iRetValue = UISDK_SUCCESS;
	memcpy(chSocketInBuffer, (char *)pNodePoint, iTotalDataLength);
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_SET_ROBOTMOVE, iTotalDataLength, chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{	   
		memcpy(&iRetValue,chSocketOutBuffer,4);
	#if 0
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return UISDK_SETROBOTMOVEERROR;
		}
	#else	
		return iRetValue;
	#endif	
	}
	return ret;
}

/*==============================================================================
NAME: int GetRobotMoveTM(unsigned int dwConnectCode, NodePoint *pNodePoint, int iTotalNodeCount, RobotAsxMax *pRobotMax)
----------------------------------------------------------------------------
Function:获取机器人按节点参数最大速度以及最大加速度
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
int SDKSTDCALLAPI GetRobotMoveTM(unsigned int dwConnectCode, P_XMLNodePoint pNodePoint, int iTotalNodeCount, RobotAsxMax *pRobotMax)
{
	if (!pNodePoint)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iTotalDataLength = iTotalNodeCount*sizeof(XMLNodePoint);
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_FILECONTENT_LEN]={0}; // 缓存
	memcpy(chSocketInBuffer, (char *)pNodePoint, iTotalDataLength);
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_GET_ROBOTMOVETM, iTotalDataLength, chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{	   
        if(pRobotMax)
		{
            memcpy(pRobotMax,chSocketOutBuffer,sizeof(RobotAsxMax));
		}		
	}
	return ret;
}

int SDKSTDCALLAPI GetRobotSMove(unsigned int dwConnectCode, P_XMLNodePoint pNodePoint, int iTotalNodeCount)
{
	if (!pNodePoint)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	int iTotalDataLength = iTotalNodeCount*sizeof(XMLNodePoint);
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_FILECONTENT_LEN]={0}; // 缓存
	memcpy(chSocketInBuffer, (char *)pNodePoint, iTotalDataLength);
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_GET_ROBOTMOVESD, iTotalDataLength, chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{	
	// 模拟数据文件5个：UIRdata_Ax01.txt，UIRdata_Ax02.txt，UIRdata_Ax03.txtUIRdata_Ax04.txt，UIRdata_XYZR.txt
	// 文件名最长200个字节
	    char chSocketInFileName[5][200]={0};
		UI_COMMANDBUFFER_OBJ cmdObj={0,{0}};
		// 初始化文件名
		for(int i = 0; i < 4; ++i)
		{
		     memset(chSocketInFileName[i], 0x0, 200);		
			 sprintf_s(chSocketInFileName[i],200, "UIRdata_Ax%02d.txt", i + 1);
		}
		memset(chSocketInFileName[4], 0x0, 200);		
		sprintf_s(chSocketInFileName[4], 200, "UIRdata_XYZR.txt");
		// 上传文件
		for(int i = 0; i < 5; ++i)
		{
			 cmdObj.iCount = strlen(chSocketInFileName[i]);
			 memcpy(cmdObj.chDataBuffer, chSocketInFileName[i], cmdObj.iCount);	
			 ret = UpSimulateFile(dwConnectCode, &cmdObj);
			 if(ret != UISDK_SUCCESS)   break;
		}
		return ret;
	}
	return ret;
}
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
int SDKSTDCALLAPI CheckPointValid(unsigned int dwConnectCode, POS_XYZR pos)
{
     int iRetValue = UISDK_SUCCESS;
     char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0};       // 返回值缓存
	 char chSocketInBuffer[UISDK_MAX_SDKBUFFER_DOUBLE_LEN]={0}; // 缓存
#ifdef WIN32	
	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_DOUBLE_LEN, "%.4f|%.4f|%.4f|%.4f", pos.x,pos.y,pos.z,pos.r);
#elif LINUX	
	 sprintf_s(chSocketInBuffer, sizeof chSocketInBuffer, "%.4f|%.4f|%.4f|%.4f", pos.x,pos.y,pos.z,pos.r);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UICMDCHECKPOINT, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{	   
        if(chSocketOutBuffer[0] != 0)
        {
            memcpy(&iRetValue,chSocketOutBuffer,4);
        }
		return iRetValue;
	}
	return ret;
}




/*==============================================================================
NAME: int GetLinePVTLists(double positionA[], double positionB[],double posMdlHgt,int iSpeedRate, int turnFlag,RobotPVTLists *task)
----------------------------------------------------------------------------
Function:计算scara机器人曲线运行时的PVT列表
----------------------------------------------------------------------------
Parameter:
double positionA:曲线起点坐标;
double positionB:曲线终点坐标；
double posMdlHgt:曲线中间点高度；
int iSpeedRate:速度的百分比:1~100
int turnFlag:是否需要往返，0表示不需要返回，1表示需要返回
RobotPVTLists *task:pvt列表(空间需要用户自己释放);
----------------------------------------------------------------------------
Return Value:
成功则返回0
失败则返回失败原因
----------------------------------------------------------------------------
Note:
==============================================================================*/


 


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
int SDKSTDCALLAPI OpenSerial(unsigned int dwConnectCode, char *pSerialString, int iStrLen)
{
	//UICMDSETMACADDRESS
	if (!pSerialString)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}

	int ret = SDKSendCommand(dwConnectCode, UI_CMD_OPENSERIALPORT, iStrLen, pSerialString, NULL);
	if (ret == UISDK_SUCCESS)
	{
		// 		if (chSocketOutBuffer[0] == '1')
		// 		{
		ret = UISDK_SUCCESS;
		// 		}
		// 		else
		// 		{
		// 			ret = UISDK_OPENSERIALERROR;
		// 		}
	}
	return ret;
}

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
int SDKSTDCALLAPI CloseSerial(unsigned int dwConnectCode, int InPutbuffer)
{
	char buffSend[10] = { 0 };
#ifdef WIN32	
	sprintf_s(buffSend, "%d", InPutbuffer);
#elif LINUX	
	sprintf_s(buffSend, sizeof buffSend, "%d", InPutbuffer);
#endif	
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_CLOSESERIALPORT, strlen(buffSend), buffSend, NULL);
	if (ret == UISDK_SUCCESS)
	{
		ret = UISDK_SUCCESS;
	}
	return ret;
}
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
int SDKSTDCALLAPI ClearBatteryError(unsigned int dwConnectCode)
{
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_CLEARBATTERYERROR, 0, NULL, NULL);
	if (ret == UISDK_SUCCESS)
	{
		ret = UISDK_SUCCESS;
	}
	return ret;
}

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
int SDKSTDCALLAPI WriteSerialMessage(unsigned int dwConnectCode, char *pSerialString, int iStrLen)
{
	//UICMDSETMACADDRESS
	if (!pSerialString)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN] = { 0 }; // 返回值缓存
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_WRITESERIALPORT, iStrLen, pSerialString, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			ret = UISDK_SUCCESS;
		}
		else
		{
			ret = PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}

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
int SDKSTDCALLAPI GetRobotUpdateStatus(unsigned int dwConnectCode, int * piCurrentIndex, int * piTotalCount)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN] = { 0 }; // 返回值缓存
	char chInput[2] = { '0','\0'};  
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_ROBOT_UPDATE_STATUS, 1, chInput, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] != '\0')
		{
			if ((piCurrentIndex) && (piTotalCount))
			{
				SDKsscanf((char *)chSocketOutBuffer, "%d;%d", piCurrentIndex, piTotalCount);
			}			
			ret = UISDK_SUCCESS;
		}
		else
		{
			ret = PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}

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
int SDKSTDCALLAPI ClearRobotUpdateStatus(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN] = { 0 }; // 返回值缓存
	char chInput[2] = { '1','\0'};  
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_ROBOT_UPDATE_STATUS, 1, chInput, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			ret = UISDK_SUCCESS;
		}
		else
		{
			ret = PROTOCOL_CMD_RETURN_FAILED;
		}
	}
	return ret;
}


/*==============================================================================
	NAME: int  UpdateRobotFirmware(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:更新机器人固件
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
int SDKSTDCALLAPI UpdateRobotFirmware(unsigned int dwConnectCode, char *pProgramName)
{
	 if (!pProgramName)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_ROBOT_UPDATE_FIRMWARE,strlen(pProgramName), pProgramName,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		 }
	 }
	 return ret;			

}


/*==============================================================================
	NAME: int  RunRobotAutoIOTest(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:机器人IO测试
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
int SDKSTDCALLAPI RunRobotAutoIOTest(unsigned int dwConnectCode)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_AUTO_IO_TEST, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}
/*==============================================================================
	NAME: int  setRobotUnlocked(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:机器人控制器解除锁定状态
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
int SDKSTDCALLAPI setRobotUnlocked(unsigned int dwConnectCode)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_ROBOT_UNLOCK, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}
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
int SDKSTDCALLAPI SetUserProgramStepRun(unsigned int dwConnectCode, char *pStatus)
{
	if (!pStatus)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	int ret = SDKSendCommand(dwConnectCode, UI_CMD_PAUSEPRO, strlen(pStatus), pStatus, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{
		if (chSocketOutBuffer[0] == '1')
		{
			return UISDK_SUCCESS;
		}
		else
		{
			return GetRobotRTCNError(PROTOCOL_CMD_RETURN_FAILED, chSocketOutBuffer);
		}
	}
	return ret;
}


/*==============================================================================
	NAME: int setPRGTraceFlag(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:设置机器人脚本调试模式
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
int SDKSTDCALLAPI setPRGTraceFlag(unsigned int dwConnectCode, char *pTraceFlag)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode, UI_CMD_SETPRGTRACEFLAG, strlen(pTraceFlag), pTraceFlag, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}

/*==============================================================================
	NAME: int getPRGTraceFlag(unsigned int dwConnectCode, int * piTraceFlag)
	----------------------------------------------------------------------------
	Function:获取机器人脚本调试模式标志
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int * piTraceFlag:调试模式标示
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI getPRGTraceFlag(unsigned int dwConnectCode, char * piTraceFlag)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode, UI_CMD_GETPRGTRACEFLAG, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] != '\0')
		 {		 
			 if (piTraceFlag)
			 {
              //   SDKsscanf((char *)chSocketOutBuffer, "%c", piTraceFlag);
			 }			 
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}

/*==============================================================================
	NAME: int setPRGBreakpoint(unsigned int dwConnectCode, int iNodeId)
	----------------------------------------------------------------------------
	Function:设置机器人脚本调试断点
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
int SDKSTDCALLAPI setPRGBreakpoint(unsigned int dwConnectCode, int iNodeId)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode, UI_CMD_GETPRGBREAKPOINT, sizeof(int), (char *)&iNodeId, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] != '\0')
		 {		 
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}

/*==============================================================================
	NAME: int  GetRobotCameraConnectStatus(unsigned int dwConnectCode, int *pStatus)
	----------------------------------------------------------------------------
	Function:获取机器人连接相机状态
	----------------------------------------------------------------------------
	Parameter:
	unsigned int dwConnectCode:连接通讯编号
	char *pStatus:机器人连接相机状态
	----------------------------------------------------------------------------
	Return Value:
	成功则返回0
	失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI GetRobotCameraConnectStatus(unsigned int dwConnectCode, int *pStatus)
{
	if (!pStatus)
	{
		return PROTOCOL_CMD_ERROR_PARAMETER;
	}
	char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	int ret = SDKSendCommand(dwConnectCode,UI_CMD_GETCAMERACONNECTSTATUS, 0, NULL, chSocketOutBuffer);
	if (ret == UISDK_SUCCESS)
	{   
		  if (chSocketOutBuffer[0] != '\0')
		 {		 
		      if (chSocketOutBuffer[0] == '1')
		      {
		         *pStatus = 1;
		      }
			  else
			  {
			    *pStatus = 0;
			  }
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
		
	}
	return ret;
}

/*==============================================================================
	NAME: int  StartUpdateFirmware(unsigned int dwConnectCode)
	----------------------------------------------------------------------------
	Function:启动在线更新程序
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
int SDKSTDCALLAPI StartUpdateFirmware(unsigned int dwConnectCode)
{
	char chSocketOutBuffer[UISDK_MAX_FILECONTENT_LEN] = { 0 }; // 返回值缓存
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_STARTUPDATEFIRMWARE,0, NULL,  chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] != '\0')
		 {		 
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_ERROR_PARAMETER;
		 }
	 }
	 return ret;
}



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
int SDKSTDCALLAPI InstallSHFile(unsigned int dwConnectCode)
{
	int ret = SDKSendCommand(dwConnectCode, UICMDINSTALL, 0, NULL, NULL);
	return ret;
}



 
 int SDKSTDCALLAPI setRobotServoFlag(unsigned int dwConnectCode, int servoFlag)
 {

        return UISDK_SUCCESS;
 }

 int SDKSTDCALLAPI GetTCPViaTeaching(unsigned int dwConnectCode, double y[4][4], TCP *tcp, double *tcpL)
 {
   
	 return UISDK_SUCCESS;
 }

 int GetLoopCountByType(unsigned int dwConnectCode)
  {
	  int nLoopCount = 400;
	  unsigned int dwDevType = GetConnectRobotType(dwConnectCode);    
	  switch (dwDevType)
	  {
	  case UIROBOTBASETYPE_SCARA:
		  nLoopCount = 400;
		  break;
	  
	  case UIROBOTBASETYPE_DELTA:
		  nLoopCount = 700;
		  break;
	  default:
		  nLoopCount = 400;
		  break;
	  }
	  return nLoopCount;
  
  }

 


  

int SDKSTDCALLAPI setRobotTorqueLimit(unsigned int dwConnectCode, int iTorqueLimit)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 char chSocketInBuffer[UISDK_MAX_DATA_LEN]; // 输入值缓存	
	 sprintf_s(chSocketInBuffer, UISDK_MAX_SDKBUFFER_LEN, "%d", iTorqueLimit);
	 int ret = SDKSendCommand(dwConnectCode, UI_CMD_SET_TORQUELIMIT, strlen(chSocketInBuffer), chSocketInBuffer, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS) 
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}

/*==============================================================================
	NAME: int getPRGTraceFlag(unsigned int dwConnectCode, int * piTraceFlag)
	----------------------------------------------------------------------------
	Function:获取机器人脚本调试模式标志
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int * piTraceFlag:调试模式标示
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI getRobotTorqueLimit(unsigned int dwConnectCode, int * piTorqueLimit)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode, UI_CMD_GET_TORQUELIMIT, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] != '\0')
		 {		 
			 if (piTorqueLimit)
			 {
				 SDKsscanf((char *)chSocketOutBuffer, "%d", piTorqueLimit);
			 }			 
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;	
}
/*==============================================================================
	NAME: int  setRobotUnlocked(unsigned int dwConnectCode, char *pProgramName)
	----------------------------------------------------------------------------
	Function:机器人控制器解除锁定状态
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
int SDKSTDCALLAPI setRobotGoHome(unsigned int dwConnectCode)
{
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_MOTORGOHOME, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] == '1')
		 {
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;			

}
/*==============================================================================
	NAME: int  getMeterWheelPosition(unsigned int dwConnectCode, int * piPosition)
	----------------------------------------------------------------------------
	Function:获取机器人计米轮位置值
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectCode:连接通讯编号
		 int * piPosition:位置值
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int SDKSTDCALLAPI getMeterWheelPosition(unsigned int dwConnectCode, int * piPosition)
{
     //int dwGWIndex = GetDevID(dwConnectCode);
	 char chSocketOutBuffer[UISDK_MAX_SDKBUFFER_LEN]={0}; // 返回值缓存	
	 int ret = SDKSendCommand(dwConnectCode,UI_CMD_GET_METERWHEEL, 0, NULL, chSocketOutBuffer);
	 if (ret == UISDK_SUCCESS)
	 {
		 if (chSocketOutBuffer[0] !=  '\0')
		 {		 
			 if (piPosition)
			 {
				 SDKsscanf((char *)chSocketOutBuffer, "%d", piPosition);
			 }			 
			 return UISDK_SUCCESS;
		 }
		 else
		 {
			 return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 return ret;
}



 

