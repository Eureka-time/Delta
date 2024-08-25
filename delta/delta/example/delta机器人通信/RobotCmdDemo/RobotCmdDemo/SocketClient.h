

#if !defined(AFX_SOCKETCLIENT_H__708C81C0_5743_40EF_ABC5_30CBE4703C8C__INCLUDED_)
#define AFX_SOCKETCLIENT_H__708C81C0_5743_40EF_ABC5_30CBE4703C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef int                 BOOL;

#include "UIRobotStruct.h"
#include "UIRobotProtocol.h"
#ifdef SDK_MMCPRODUCT_TEST
#define SOCKETSERVERPORT 18877
#else
#define SOCKETSERVERPORT 8877
#endif
using namespace std;
extern bool g_bLabViewFlag;
extern CallBackData g_staticCallBackData;

#define UIM_VALUE_FIRST_BYTE(var)	((var)&0xff)
#define UIM_VALUE_SECOND_BYTE(var)   ((var >>8)&0xff)
#define UIM_VALUE_THIRD_BYTE(var)	((var >>16)&0xff)
#define UIM_VALUE_FOURTH_BYTE(var) ((var>>24)&0xff)
#define UIM_LITTLE_END_VALUE_FROM_BYTE(first,second,third,forth)  (((first&0x0FF)|((second&0x0FF)<<8)|((third&0x0FF)<<16)|((forth&0x0FF)<<24))&0x0FFFFFFFF)
#define UIM_BIG_END_VALUE_FROM_BYTE(first,second,third,forth)  (((forth&0x0FF)|((third&0x0FF)<<8)|((second&0x0FF)<<16)|((first&0x0FF)<<24))&0x0FFFFFFFF)

#define ROBOT_MOVE_MAX_SPEED_VALUE (100)

#define UIBASEARMINDEX               0x01
#define UIMAINARMINDEX               0x02
#define UIASSISTANTARMINDEX          0x03
#define UIBARARMINDEX                0x04


#define UISDK_SOCKETPACKAGE_HEAD 0xAA
#define UISDK_SOCKETPACKAGE_TAIL 0xCC
#define UISDK_SOCKETRECPACKAGE_HEAD (UISDK_SOCKETPACKAGE_TAIL)
#define UISDK_SOCKETRECPACKAGE_TAIL (UISDK_SOCKETPACKAGE_HEAD)

typedef struct _UI_WRITEFILECONTENT_OBJ
{
	P_UI_FILECONTENT_OBJ pFileObj;
	void *pSocketClient;
}UI_WRITEFILECONTENT_OBJ, *P_UI_WRITEFILECONTENT_OBJ;

/*

数据包的格式，包括包头和包体两部分，具体格式如下：
数据包格式：以AA开头，CC结尾，第二个字节命令字，第三个字节命令所带数据长度，之后就是命令所带数据的内容,如下所示：

| 0xAA| cmd|len| | |...............|0xCC|
___包头___|______包体______|

*/
class CSocketClient
{
public:
	CSocketClient();
	virtual ~CSocketClient();
public:

	int ReceiveMsg_C(char* buff, int *iLen);				//客户端接收数据
	int SendMsg_C(char *buff, unsigned char cmd_type, int nSendSize);	//客户端发送数据
	int ConnectValueServer(unsigned int dwIP);		//连接服务器
	int ConnectNameServer(char * pIPAddr);
	void GetError(unsigned int error);					//返回错误信息
	int DisConnect();
	int SendSocketCommand(unsigned int dwConnectCode,unsigned char cmd_type, int nSendSize, char *InPutbuffer, char *Outbuffer, int *pOutLen);
    BOOL SocketStartBufProcThread(unsigned int dwConnectCode);
	int UpProgramFile(unsigned int dwConnectCode,P_UI_COMMANDBUFFER_OBJ robotFileObj);	
	int UpSimulatorFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj);
	void setConnectCode(unsigned int dwConnectCode){m_dwConnectCode = dwConnectCode;}
private:
	SDK_SOCKET_HANDLE m_hSocket;							//客户端的连接socket
	SDK_SOCKADDR_HANDLE m_addr;							//服务器端的ip地址
	unsigned int m_uPort;								//服务器的端口号				
	BOOL m_bInit;								//winsock初始化的信号量
	int TCPConnectServer();
	HANDLE m_hdSocketRcvThread; // 接收线程
	HANDLE m_hdSocketRtcnThread; // 用户接口线程处理RTCN
	BOOL m_bSocketRcvThread; // 是否允许接收
	BOOL m_bIsSocketConnected; // 是否连接状态
	//SDK_EVENT_HANDLE   m_hPrepareEvent; //  启动时使用的事件
	SDK_EVENT_HANDLE m_hCmdWaitEvent; // 发送命令等待事件
	SDK_CRITICAL_SECTION   m_hWaitSockCriSection; 
	SDK_CRITICAL_SECTION   m_hCmdSockCriSection;  //  发送命令临界区
	char m_chSocketReceiveBuffer[UICMDREALTIMENOTIFICATION+1][UISDK_MAX_DATA_LEN];
	int m_iSocketReceiveDataLen[UICMDREALTIMENOTIFICATION+1];
	char m_chSocketOutBuffer[UISDK_MAX_DATA_LEN] ; // 返回值缓存
	char m_chSocketInBuffer[UISDK_MAX_DATA_LEN] ; // 命令输入缓存
	char m_chSocketSendBuf[UISDK_MAX_DATA_LEN + 10];
	bool m_bReceiveCmdAck; // 是否接收到发送报文的反馈消息
	SDK_CRITICAL_SECTION   m_hFileWriteCriSection;  //  文件写入临界区
	ofstream m_outPutFile; // 输出文件
	long m_outFileSize;
	unsigned int m_dwConnectCode;
	bool  setSDKSocketBlock(bool bBlocked);
	unsigned int SetTimeouts();
	unsigned int SDKSocketClose();
	void setTcpKeepAlive(int fd);
	unsigned int DoSocketReceiveThread();
	unsigned int DoSocketFileWirteThreadProc(LPVOID param);
	unsigned int DoSocketRtcnThreadProc(LPVOID lpPara);
	static SDK_THREADPROC SocketReceiveThread(LPVOID param)
       {
	       CSocketClient* pPort = ((CSocketClient*)param);
#ifdef WIN32
		 return pPort->DoSocketReceiveThread();
#elif LINUX
		 pPort->DoSocketReceiveThread();
		 return NULL;
#endif
       }
        unsigned int DoSocketSFileWirteThreadProc(void * param);
       static SDK_THREADPROC SocketSFileWirteThreadProc(void * param)
       {
           P_UI_WRITEFILECONTENT_OBJ pParam = (P_UI_WRITEFILECONTENT_OBJ)(param);
           ((CSocketClient*)pParam->pSocketClient)->DoSocketSFileWirteThreadProc(pParam->pFileObj);
		   FreePtr(pParam);
	#ifdef WIN32
           return 0;
	#elif LINUX
		 	return NULL;
	#endif
       }
       static SDK_THREADPROC SocketFileWirteThreadProc(LPVOID param)
       {
           P_UI_WRITEFILECONTENT_OBJ pParam = (P_UI_WRITEFILECONTENT_OBJ)(param);
           ((CSocketClient*)pParam->pSocketClient)->DoSocketFileWirteThreadProc(pParam->pFileObj);
		   FreePtr(pParam);
		   #ifdef WIN32
           return 0;
		   #elif LINUX
		 	return NULL;
		   #endif
       }
       static SDK_THREADPROC SocketRtcnThreadProc(LPVOID param)
       {
           P_UI_RTCNCALLBACK_OBJ pRtcnObj = (P_UI_RTCNCALLBACK_OBJ)param;
           ((CSocketClient*)pRtcnObj->pSocketClient)->DoSocketRtcnThreadProc(pRtcnObj);
		   #ifdef WIN32
           return 0;
		   #elif LINUX
		 	return NULL;
		   #endif
       }
	   bool GetRobotSDKMessage(char *pBuffer, unsigned int &iDataLen, unsigned int &iMsgLen, int iMaxLen);

	void DisconnectSocket()
	{
		if (m_hSocket != INVALID_SOCKET)
		{
			shutdown(m_hSocket, SD_BOTH);
#ifdef WIN32		
			closesocket(m_hSocket);
			WSACleanup();
#elif LINUX
			close(m_hSocket);
#endif
			m_hSocket = INVALID_SOCKET;			
		}

	}
	int DataProcess_ServerPaket(unsigned int dwConnectCode,char *buff);
   

};

#endif // !defined(AFX_SOCKETBLOCKCLIENT_H__708C81C0_5743_40EF_ABC5_30CBE4703C8C__INCLUDED_)

