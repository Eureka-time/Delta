// SocketBlockClient.cpp: implementation of the CSocketClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#ifdef LINUX
#include <syslog.h>
#include <netinet/tcp.h>
#endif
#include "fcntl.h"
#include "SocketClient.h"

#ifdef WIN32
#pragma comment(lib,"Ws2_32.lib")
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

#define SDK_SOCKET_TIMEOUT_TOTAL                 (10000)   // (SDK_SOCKET_TIMEOUT_TOTAL * 0.1)msSDK超时计数

extern PF_UIROBOTSDK_CALLBACK pfRobotRtcnCallBack;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketClient::CSocketClient()
{
	// 变量初始化
	m_bInit=TRUE;
    m_bSocketRcvThread = false;
	m_hSocket = INVALID_SOCKET;
	memset(m_chSocketSendBuf, 0x0, sizeof(m_chSocketSendBuf));
	m_bSocketRcvThread = false; // 是否允许接收
	m_bIsSocketConnected = false; // 是否连接状态
	m_outFileSize = 0;

}

CSocketClient::~CSocketClient()
{
	// 关闭socket
    m_bSocketRcvThread = false;
// 关闭SOCKET连接
	DisconnectSocket();
}
bool  CSocketClient::setSDKSocketBlock(bool bBlocked)
{
	// 设置堵塞：bBlocked=true为堵塞;
	//           bBlocked=false为不堵塞;
#ifdef WIN32
	u_long mode = 1;
	if (bBlocked)
	{
		mode = 0;   
	}	
	if (SOCKET_ERROR == ioctlsocket(m_hSocket, FIONBIO, &mode))
	{
		WSACleanup();//释放套接字资源;		   
		m_hSocket = INVALID_SOCKET;
		return false;		    
	}
#elif LINUX
	int flags = fcntl(m_hSocket, F_GETFL, 0);
	if (bBlocked)
	{
		flags =  flags & ~O_NONBLOCK;
	}
	else
	{
		flags = flags | O_NONBLOCK;
	}
	if (fcntl(m_hSocket, F_SETFL, flags) < 0)
	{
		m_hSocket = INVALID_SOCKET;
		return false;		
	}
#endif
	return true;		
}

unsigned int CSocketClient::SDKSocketClose()
{
	if (INVALID_SOCKET == m_hSocket)
	{
		return TRUE;
	}	
	setSDKSocketBlock(false);
	shutdown(m_hSocket, SD_BOTH);
#ifdef WIN32		
	closesocket(m_hSocket);
	WSACleanup();
#elif LINUX
	close(m_hSocket);
#endif
	m_hSocket = INVALID_SOCKET;
	return TRUE;
}

/*客户端连接服务器函数。
返回值:
0 成功.
1 失败，在返回1以前，显示错误原因。
参数：
IP:    服务器IP地址
nPort: 服务器端端口号。
*/
int CSocketClient::TCPConnectServer()
{	
	//SDKTraceFile("TCPConnectServer m_bInit=%d\n",m_bInit);
	if (!m_bInit)
	{
		return PROTOCOL_SOCKETTINITIALERROR;
	}
	if (m_hSocket != INVALID_SOCKET)
	{
		return PROTOCOL_SOCKETTINITIALERROR;	
	}
	else 
	{
#ifdef WIN32
		WSADATA wsaData;
		WORD version = MAKEWORD(2, 0);
		int ret = WSAStartup(version, &wsaData);
		if (ret != 0)
		{
			m_bInit = FALSE;
		}
#endif
		//创建新套接字
		m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
		//	ASSERT(m_hSocket!=NULL);
	}
	//SDKTraceFile("TCPConnectServer socket m_hSocket=%d\n",m_hSocket);
	//准备服务器的信息，这里需要指定服务器的地址
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(SOCKETSERVERPORT);//改变端口号的数据格式
	
	setSDKSocketBlock(false);
    connect(m_hSocket, (sockaddr*)&m_addr, sizeof(m_addr));
	
	setSDKSocketBlock(true);
    SDK_TIMEVAL timeval;
	timeval.tv_sec = 2;
	timeval.tv_usec = 500000;
	fd_set Write, Err;
	FD_ZERO(&Write);
	FD_ZERO(&Err);
	FD_SET(m_hSocket, &Write);
	FD_SET(m_hSocket, &Err);
	// check if the socket is ready
	select(0, NULL, &Write, &Err, &timeval);
	if (!FD_ISSET(m_hSocket, &Write))
	{
		setSDKSocketBlock(false);
		shutdown(m_hSocket, SD_BOTH);
#ifdef WIN32		
		closesocket(m_hSocket);
		WSACleanup();
#elif LINUX
		close(m_hSocket);
#endif
		return PROTOCOL_SOCKETTINITIALERROR;
	}
	
	m_bInit = true;
	return UISDK_SUCCESS;
}
unsigned int CSocketClient::SetTimeouts()
{
	if (INVALID_SOCKET == m_hSocket)
	{
		return FALSE;
	}

	int iResult;
	int timeout = 100; //单次写超时时间
	iResult = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
	if (iResult == SOCKET_ERROR)
	{
		SDKSocketClose();
		return FALSE;
	}
	timeout = 0; //读不等待
	iResult = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	if (iResult == SOCKET_ERROR)
	{
		SDKSocketClose();
		return FALSE;
	}
	return TRUE;
}
void CSocketClient::setTcpKeepAlive(int fd)
{
#ifdef WIN32
	BOOL bKeepAlive = TRUE;
	tcp_keepalive live, liveout;
	live.keepaliveinterval = 500; //每次检测的间隔 （单位毫秒）
	live.keepalivetime = 3000; //第一次开始发送的时间（单位毫秒）
	live.onoff = TRUE;
	int iRet = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));
	if (iRet == 0){
		DWORD dw;
		if (WSAIoctl(fd, SIO_KEEPALIVE_VALS,
			&live, sizeof(live), &liveout, sizeof(liveout),
			&dw, NULL, NULL) == SOCKET_ERROR){
			//Delete Client 
			return;
		}
	}

#elif LINUX
	int keepalive = 1; 		// 开启keepalive属性
	int keepidle = 30; 		// 如该连接在60秒内没有任何数据往来,则进行探测
	int keepinterval = 5; 	// 探测时发包的时间间隔为5 秒
	int keepcount = 3; 		// 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
	//BOOL bReuseaddr=true;
	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
	//setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&bReuseaddr,sizeof(BOOL));
	setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle, sizeof(keepidle));
	setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval, sizeof(keepinterval));
	setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount, sizeof(keepcount));
	linger m_sLinger;
	m_sLinger.l_onoff = 1;//(在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
	// 如果m_sLinger.l_onoff=0;则功能和2.)作用相同;
	m_sLinger.l_linger = 0;//(容许逗留的时间为5秒)
	setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(linger));
#endif

	SetTimeouts();
}

int CSocketClient::ConnectValueServer(unsigned int dwIP)
{
	memset(&m_addr, 0, sizeof(m_addr));
#ifdef WIN32
	m_addr.sin_addr.S_un.S_addr = htonl(dwIP);
#elif LINUX
	m_addr.sin_addr.s_addr = htonl(dwIP);
#endif
	
	return TCPConnectServer();
}

int CSocketClient::ConnectNameServer(char * pIPAddr)
{
    char TempIPAddr[100]={0};
	memcpy(TempIPAddr,pIPAddr,strlen(pIPAddr));
	memset(&m_addr, 0, sizeof(m_addr));
    char *pValue= NULL;
	unsigned int iValue = 0;
	char delim[2] = ".";
#ifdef WIN32
    char *pNext= NULL;
	pValue = strtok_s(TempIPAddr, delim, &pNext);
#elif LINUX
	pValue = strtok(TempIPAddr, ".");
#endif
	
	if (!pValue)
	{
		return -1;
	}
	iValue = atoi(pValue);
#ifdef WIN32
	pValue = strtok_s(NULL, delim, &pNext);
#elif LINUX
	pValue = strtok(NULL, delim);
#endif
	
	
	if (!pValue)
	{
		return -1;
	}
	iValue = iValue + ((atoi(pValue))<<8);
	
#ifdef WIN32
	pValue = strtok_s(NULL, delim, &pNext);
#elif LINUX
	pValue = strtok(NULL, delim);
#endif
	if (!pValue)
	{
		return -1;
	}
	iValue = iValue + ((atoi(pValue)) << 16);
#ifdef WIN32
	pValue = strtok_s(NULL, delim, &pNext);
#elif LINUX
	pValue = strtok(NULL, delim);
#endif	
	if (!pValue)
	{
		return -1;
	}
	iValue = iValue + ((atoi(pValue)) << 24);
#ifdef WIN32
	m_addr.sin_addr.S_un.S_addr = iValue;
#elif LINUX
	m_addr.sin_addr.s_addr = iValue;
#endif
	return TCPConnectServer();
}
/*客户端发送消息函数
返回值:
0 成功.
1 失败，在返回1以前，显示错误原因。
参数：
buff:发送的数据缓冲区。
*/
int CSocketClient::SendMsg_C(char *buff, unsigned char cmd_type, int nSendSize)
{
	if (!m_bInit)
	{
		return PROTOCOL_SOCKETTINITIALERROR;
	}
	char *dataBegin = NULL;
	int totalLen = 0;
	//数据包的总大小，包括数据头的大小，和数据体的大小
	if ((buff) && nSendSize)
	{
		for (int i = 0; i < nSendSize; i++)
		{
			m_chSocketSendBuf[i + 6] = buff[i];
		}
		totalLen = nSendSize + 7;
		m_chSocketSendBuf[2] = UIM_VALUE_FIRST_BYTE(nSendSize);
		m_chSocketSendBuf[3] = UIM_VALUE_SECOND_BYTE(nSendSize);
		m_chSocketSendBuf[4] = UIM_VALUE_THIRD_BYTE(nSendSize);
		m_chSocketSendBuf[5] = UIM_VALUE_FOURTH_BYTE(nSendSize);
	}
	else
	{
		totalLen = 7;
		m_chSocketSendBuf[2] = 0;
		m_chSocketSendBuf[3] = 0;
		m_chSocketSendBuf[4] = 0;
		m_chSocketSendBuf[5] = 0;
	}	
	m_chSocketSendBuf[0] = (char)((UISDK_SOCKETPACKAGE_HEAD) & 0xFF);
	m_chSocketSendBuf[1] = cmd_type;
	m_chSocketSendBuf[totalLen - 1] = (char)((UISDK_SOCKETPACKAGE_TAIL) & 0xFF);
	dataBegin = m_chSocketSendBuf;
#ifdef LINUX
	// check socket status
	struct tcp_info info;
	socklen_t len = sizeof(info);
	getsockopt(m_hSocket, IPPROTO_TCP, TCP_INFO, &info, &len);
	if (info.tcpi_state != TCP_ESTABLISHED)
	{
		return PROTOCOL_SOCKETSENDERROR;
	}    
#endif	
	// 发送网络报文
	for (int j = 0; j<totalLen / UISDK_MAX_DATA_LEN; j++)
	{
	       if(totalLen < UISDK_MAX_DATA_LEN)
	       {	       
			int end=send(m_hSocket,dataBegin,totalLen,0); 
			if(end==SOCKET_ERROR)
			{
				return PROTOCOL_SOCKETSENDERROR;
			}
			dataBegin+=totalLen;
	       }
		else
		{
			int end=send(m_hSocket,dataBegin,UISDK_MAX_DATA_LEN,0); 
			if(end==SOCKET_ERROR)
			{
				return PROTOCOL_SOCKETSENDERROR;
			}
			dataBegin+=UISDK_MAX_DATA_LEN;		
		}
	}
	if(totalLen%UISDK_MAX_DATA_LEN)
	{
		int end=send(m_hSocket,dataBegin,totalLen%UISDK_MAX_DATA_LEN,0); 
		if(end==SOCKET_ERROR)
		{
				return PROTOCOL_SOCKETSENDERROR;
		}
	}

	return UISDK_SUCCESS;

}


/*客户端接收消息函数
返回值:
0 成功.
1 失败，在返回1以前，显示错误原因。
参数：
buff:接收数据的缓冲区，填满实参所指缓冲区，注意，实参应是已经分配好空间的缓冲区。
*/
int CSocketClient::ReceiveMsg_C(char *buff, int *iLen)
{
	if(!m_bInit)
	{
		//::MessageBox(NULL, _T("winsock 初始化错误!"), _T("错误提示"), NULL);
		return PROTOCOL_SOCKETTINITIALERROR;
	}
	if (m_hSocket == INVALID_SOCKET)
	{
		return PROTOCOL_SOCKETTINITIALERROR;
	}
	//CString msg;
	char* p=buff;
	
	int ret = recv(m_hSocket, p, UISDK_MAX_DATA_LEN, 0);
	if ((m_hSocket == INVALID_SOCKET) || (!m_bSocketRcvThread))
	{
		return PROTOCOL_SOCKETTINITIALERROR;
	}
	if ((ret == SOCKET_ERROR) || (ret == 0))
	{
		if (m_hSocket == INVALID_SOCKET)
		{
			return PROTOCOL_SOCKETTINITIALERROR;		
		}
	}
	if(ret<UISDK_MAX_DATA_LEN)
	{
	       *iLen = ret;
		return UISDK_SUCCESS;
	}

	return UISDK_SUCCESS;
}

int CSocketClient::DisConnect()
{
	if(m_bSocketRcvThread)
 	{
	 	setSDKSocketBlock(false);
		m_bSocketRcvThread = false;
		DisconnectSocket();
		
		while(m_hdSocketRcvThread)
		{
			Sleep(100);
		}
		

		Sleep(100);
		m_hdSocketRcvThread = 0;
		m_bIsSocketConnected = false;
		SDK_DELETETHREAD(m_hdSocketRcvThread);
		#ifdef WIN32
	 	SDK_EVENT_DELETE(m_hCmdWaitEvent);	 
		#endif
		m_bReceiveCmdAck = false;
		SDK_DeleteCriticalSection(&m_hCmdSockCriSection);
		SDK_DeleteCriticalSection(&m_hFileWriteCriSection);		
	 	SDK_DeleteCriticalSection(&m_hWaitSockCriSection); 
		Sleep(100);
 	}
	return 0;
}
/*==============================================================================
	NAME: unsigned int	SocketRtcnThreadProc(LPVOID lpPara)
	----------------------------------------------------------------------------
	Function:处理用户反馈消息线程
	----------------------------------------------------------------------------
	Parameter:
		  LPVOID lpPara:线程启动的参数，主要是网络通讯指令码
	----------------------------------------------------------------------------
	Return Value:  
				   If the function fails, the return value is false. 
				   If the function succeeds, the return value is true. 
	----------------------------------------------------------------------------
	Note:	 

==============================================================================*/
unsigned int CSocketClient::DoSocketRtcnThreadProc(LPVOID lpPara)
{
	P_UI_RTCNCALLBACK_OBJ pRtcnObj = (P_UI_RTCNCALLBACK_OBJ)lpPara;	
	switch(pRtcnObj->cmd_type)
	{
	     case UICMDSOCKETDISCONNECTED:
	     {
		 	DisconnectSocket();
   	     }
	     break;
	}	

	if (pfRobotRtcnCallBack)
	{
		pfRobotRtcnCallBack(pRtcnObj->m_dwConnectCode, pRtcnObj->cmd_type, pRtcnObj->m_chCommandDataBuffer,pRtcnObj->m_chCommandDataLen);
	}
	FreePtr(pRtcnObj);
	return 0;
}
bool CSocketClient::GetRobotSDKMessage(char *pBuffer, unsigned int &iDataLen, unsigned int &iMsgLen, int iMaxLen)
{
	unsigned int i = 0;
	int iPos = 0;
	bool bMsgComplete = FALSE;
	iMsgLen = iDataLen;
	if (iDataLen > 0)
	{
		//找消息头，找到将消息头后面的数据拷贝到Buf开始
		for (i = 0; i < iDataLen; i++)
		{
			if ((pBuffer[i] & 0x0FF) == UISDK_SOCKETRECPACKAGE_HEAD)
			{
				if (i > 0)
				{
					memcpy(pBuffer, pBuffer + i, iDataLen - i);
				}
				break;
			}
		}

		//如果没有找到消息头,清空Buf中数据
		if (i == iDataLen)
		{
			iDataLen = 0;
			memset((void*)pBuffer, 0, iMaxLen);
			iMsgLen = 0;
			return bMsgComplete;
		}
		else
		{
			//找到消息头，修改Buf中数据的长度
			iDataLen = iDataLen - i;
		}

		//找消息尾
		for (i = 0; i < iDataLen; i++)
		{
			//找消息尾之前，先找消息头，找到往前移

			if ((pBuffer[i] & 0x0FF) == UISDK_SOCKETRECPACKAGE_HEAD)
			{
				if (i > 0)
				{
					memcpy(pBuffer, pBuffer + i, iDataLen - i);
					iDataLen = iDataLen - i;
					i = 0;
				}
			}
			if ((pBuffer[i + 1] & 0x0FF )== UICMDREALTIMENOTIFICATION)
			{
				iPos = (pBuffer[i + 3]) & 0x0FF;
				iPos = i + iPos + 7;
			}
			else
			{
				iPos = UIM_LITTLE_END_VALUE_FROM_BYTE(pBuffer[i + 2], pBuffer[i + 3], pBuffer[i + 4], pBuffer[i + 5]);
				iPos = i + iPos + 6;
			}
			// (i + pBuffer[i + 2] + 3) & 0x0FFFF;
			if (iPos < iMaxLen)
			{
				    //找消息尾
				if ((pBuffer[iPos] & 0x0FF) == UISDK_SOCKETRECPACKAGE_TAIL)
				{
					iMsgLen = iPos + 1; // 完整报文的长度
					if (iMsgLen >= 4)
					{
						bMsgComplete = TRUE;
					}
					break;//找到消息尾
				}
				else
				{
					i = iPos;
				}
			}
			
		}

		//没有找到消息尾，继续读取数据
		if (i == iDataLen)
		{
            if (iDataLen == (unsigned int)iMaxLen)// 缓冲区满,杂乱数据，清空
			{
				iDataLen = 0;
				memset((void*)pBuffer, 0, iMaxLen);
			}
			//break;//跳出处理报文循环，开始读数据
		}
	}
	return bMsgComplete;
}
//客户端接收数据线程
unsigned int CSocketClient::DoSocketReceiveThread()
{
//===============================================
// 初始化临时变量
	char RcvBuf[UISDK_MAX_DATA_LEN] = { 0 };  // 接收缓冲区
	int iBufferLen = sizeof(RcvBuf);           // 缓冲区大小
	unsigned int uiDataNum = 0;                 // 缓冲区内的数据字节个数
	unsigned int uiMsgNum = 0;                 // 获取的一条报文字节长度
	int iRecDataLen = 0;                       // 本次接收到的数据字节长度
	memset(RcvBuf, 0x0, iBufferLen);	

	while (m_bSocketRcvThread)
	{
        unsigned int ret = (unsigned int)ReceiveMsg_C((char *)&RcvBuf + uiDataNum, &iRecDataLen);
		if ((ret == UISDK_SUCCESS) && (iRecDataLen > 0))
		{ 
			uiDataNum = iRecDataLen + uiDataNum;	
			// 获取命令及数据
		    while(GetRobotSDKMessage((char *)RcvBuf, uiDataNum, uiMsgNum, iBufferLen))
		    {// 解析命令		

			if (DataProcess_ServerPaket(m_dwConnectCode,RcvBuf) == 0)
			{		
			  switch (RcvBuf[1] & 0x0FF)
			  {
				case UICMDGETPROGRAMLIST: // 用户程序列表分段发送过来，开头"0|"表示结束
				{
					if ((RcvBuf[6] & 0x0FF) == '0')                         
					{
						// 获取文件列表时，需要多个报文，当报文所带数据第一个字节为“0”表示后续没有报文
#ifdef WIN32
						SDK_EVENT_SIGNAL(m_hCmdWaitEvent);		
#endif
						m_bReceiveCmdAck = true;
					}
					break;
				}		
				case UICMDREQUIREFILENAME:
				{
					// if((RcvBuf[3]&0x0FF)=='0') // 文件传输时，发送文件名后，反馈"0"表示失败,"1"表示将进行文件传输
					{
#ifdef WIN32
						SDK_EVENT_SIGNAL(m_hCmdWaitEvent);		
#endif
						m_bReceiveCmdAck = true;
					}
					break;
				}		

				case UICMDREALTIMENOTIFICATION:
				case UICMDTRANSMITFILECONTENT:
				case UICMDTRANSMITSFILECONTENT:
				{
					//RTCN不做SIGNAL SET					
				}
				break;
				default: 
				{
					if (m_chSocketSendBuf[1] == (RcvBuf[1] & 0x0FF))
					{
#ifdef WIN32
						SDK_EVENT_SIGNAL(m_hCmdWaitEvent);		
#endif
						m_bReceiveCmdAck = true;
		    		}
				}	       
				break;
			  }
			}  
			memcpy(RcvBuf, RcvBuf + uiMsgNum, iBufferLen - uiMsgNum);
			uiDataNum -= uiMsgNum;
			uiMsgNum = 0;
			

			}

		}
		else if(ret == PROTOCOL_SOCKETTINITIALERROR)
		{
			    P_UI_RTCNCALLBACK_OBJ pRtcnObj = new UI_RTCNCALLBACK_OBJ();
				pRtcnObj->cmd_type = UICMDSOCKETDISCONNECTED;
				pRtcnObj->pSocketClient = this;
				pRtcnObj->m_dwConnectCode = m_dwConnectCode;
#ifdef LINUX
				pthread_attr_t attr;
				if (0 != pthread_attr_init(&attr))
					return FALSE;
				if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
					return FALSE;
				if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
					return FALSE;
				SDK_CREATETHREAD(m_hdSocketRtcnThread,attr,SocketRtcnThreadProc,pRtcnObj);	
#else
				SDK_CREATETHREAD(m_hdSocketRtcnThread,NULL,SocketRtcnThreadProc,pRtcnObj);
#endif
				break;


		}
		else
		{
		    break;
		}
		
	}
	
	m_hdSocketRcvThread = 0;
	return 0;
}

BOOL CSocketClient::SocketStartBufProcThread(unsigned int dwConnectCode)
{
       if(!m_bSocketRcvThread)
       {	       	
		SDK_InitializeCriticalSection(&m_hCmdSockCriSection);
		SDK_InitializeCriticalSection(&m_hFileWriteCriSection);	
	    SDK_InitializeCriticalSection(&m_hWaitSockCriSection);
#ifdef WIN32
	    SDK_EVENT_CREATE(m_hCmdWaitEvent);
	    SDK_EVENT_RESET(m_hCmdWaitEvent); 
#endif		
		m_bReceiveCmdAck = false;
		m_bSocketRcvThread= TRUE;
		m_dwConnectCode = dwConnectCode;
		//g_uiRS232RcvBuf = new UI_RCVBUF_OBJ;
		//memset(g_uiRS232RcvBuf, 0, sizeof(UI_RCVBUF_OBJ));
#ifdef LINUX
		pthread_attr_t attr;
		struct sched_param param;
		if (0 != pthread_attr_init(&attr))
			return FALSE;
		if (0 != pthread_attr_setschedpolicy(&attr, SCHED_RR))
			return FALSE;
		if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
			return FALSE;
		if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
			return FALSE;		
		param.sched_priority = 65;
		pthread_attr_setschedparam(&attr, &param);
		SDK_CREATETHREAD(m_hdSocketRcvThread, attr, SocketReceiveThread, this);
		pthread_attr_destroy(&attr);
#else
		SDK_CREATETHREAD(m_hdSocketRcvThread, NULL, SocketReceiveThread, this);
		::SetThreadPriority(m_hdSocketRtcnThread,65);				 //设置优先级 
		::ResumeThread(m_hdSocketRtcnThread); 

#endif
        Sleep(50);   	     
	     
       }
 
// SDKTraceFile("TCPConnectServer SocketStartBufProcThread m_bIsSocketConnected=%d \n",m_bIsSocketConnected);
	
//	if (!m_hdSocketRcvThread)
	//	return 2;

	m_bIsSocketConnected = true;
        return 0;
}

// 发送命令并等待命令返回值


// 发送命令并等待命令返回值
int CSocketClient::SendSocketCommand(unsigned int dwConnectCode,unsigned char cmd_type, int nSendSize, char *InPutbuffer, char *Outbuffer, int *pOutLen)
{
	if (m_bIsSocketConnected)
	{ 
		 if ((nSendSize)&&(!InPutbuffer))     return PROTOCOL_CMD_ERROR_PARAMETER;	 
		
		SDK_EnterCriticalSection(&m_hCmdSockCriSection);
		SDK_EnterCriticalSection(&m_hWaitSockCriSection);
#ifdef WIN32		 
		SDK_EVENT_RESET(m_hCmdWaitEvent);
#endif
		m_bReceiveCmdAck = false;
		memset(&m_chSocketReceiveBuffer[cmd_type], 0x0, UISDK_MAX_DATA_LEN);
		m_iSocketReceiveDataLen[cmd_type] = 0;
		m_chSocketSendBuf[1]  = 0; //用于保存发送指令：在SendMsg_C()函数中会对该缓存进行赋值
		int ret = SendMsg_C(InPutbuffer, cmd_type, nSendSize);
		if (ret != UISDK_SUCCESS)
		{
			m_chSocketSendBuf[1]  = 0;
			SDK_LeaveCriticalSection(&m_hWaitSockCriSection);
			SDK_LeaveCriticalSection(&m_hCmdSockCriSection);
			return PROTOCOL_SOCKETSENDERROR;
		}
      if(!Outbuffer)  //没有接收缓存，表示不需要等待反馈
      {
          m_chSocketSendBuf[1]  = 0;
          SDK_LeaveCriticalSection(&m_hWaitSockCriSection);
          SDK_LeaveCriticalSection(&m_hCmdSockCriSection);
          return UISDK_SUCCESS;

      }
		if(UICMDTRANSLATECANDATA == cmd_type)
		{
		  m_chSocketSendBuf[1]  = UICMDRECEIVECANDATA;
		  cmd_type = UICMDRECEIVECANDATA;
		  m_iSocketReceiveDataLen[cmd_type] = 0;
		}
#ifdef LINUX	
			int iSleepCount = 0;
		// 等待SDK_SOCKET_TIMEOUT_TOTAL*0.1ms
	        while(iSleepCount < SDK_SOCKET_TIMEOUT_TOTAL)
	        {
	            iSleepCount++;
				usleep(100); // 0.1ms
				if(m_bReceiveCmdAck) break;
	        }		
			// 仍未收到反馈，失败退出
			if(!m_bReceiveCmdAck)
			{
				m_chSocketSendBuf[1]  = 0;
				SDK_LeaveCriticalSection(&m_hWaitSockCriSection);
				SDK_LeaveCriticalSection(&m_hCmdSockCriSection);				
				m_iSocketReceiveDataLen[cmd_type] = 0;
				return PROTOCOL_CMD_ERROR_TIMEOUT;
			}
#else
		int timeout = 2000;
		if (m_iSocketReceiveDataLen[cmd_type] == 0)
		{		
			if (SDK_WaitForSingleTimout(m_hCmdWaitEvent, m_hWaitSockCriSection, timeout))
			{
				m_chSocketSendBuf[1]  = 0;
				SDK_LeaveCriticalSection(&m_hWaitSockCriSection);
				SDK_LeaveCriticalSection(&m_hCmdSockCriSection);
				m_iSocketReceiveDataLen[cmd_type] = 0;
				return PROTOCOL_CMD_ERROR_TIMEOUT;
			}
		}
 
#endif
		m_chSocketSendBuf[1]  = 0;
		if (Outbuffer)
		{
			memcpy(Outbuffer, &m_chSocketReceiveBuffer[cmd_type], m_iSocketReceiveDataLen[cmd_type]);
			Outbuffer[m_iSocketReceiveDataLen[cmd_type]] = '\0';
		}
		if(pOutLen)
		{
			*pOutLen = m_iSocketReceiveDataLen[cmd_type];		
		}

		// m_iSocketReceiveDataLen[cmd_type] = 0;
		SDK_LeaveCriticalSection(&m_hWaitSockCriSection);	
		SDK_LeaveCriticalSection(&m_hCmdSockCriSection);
	
		return UISDK_SUCCESS;
	}

	return PROTOCOL_SOCKETTINITIALERROR;
}

// 解包函数
int CSocketClient::DataProcess_ServerPaket(unsigned int dwConnectCode,char *buff)
{
	if (buff)
	{
		//CString strTemp = _T(""); // 本命令反馈消息
		unsigned char  chCommand = buff[1]; // 命令字	
		switch (chCommand)
		{	
			case UICMDLOGIN                   :
			case UICMDSOCKETDISCONNECTED      :
			case UICMDSETPASWORD              :
			case UICMDGETNETCONFIG            :
			case UICMDSETNETCONFIG            :
			case UICMDSETTIME                 :
			case UICMDGETTIME                 :
			case UICMDGETVERSION              :
			case UICMDGETSTATUS               :
			case UICMDGETRUNPROGRAM           :
			case UICMDDELETEPROGRAM           :
			case UICMDGETDEFAULTPROGRAM       :
			case UICMDSETDEFAULTPROGRAM       :
			case UICMDGETMOTORTEMPERATURE     :
			case UICMDSETMACADDRESS                :
			case UICMDGETMACADDRESS                :
			case UICMDSETSERIALNUMBER               :
			case UICMDGETSERIALNUMBER               :
			case UICMDRUNPROGRAM              :
			case UICMDSIMULATORRUNPROGRAM     :
			case UICMDEMERGYSTOP              :
			case UICMDRESETROBOT              :
			case UICMDGETTOOLCENTRECOORDINATE :
			case UICMDSETTOOLCENTRECOORDINATE :
			case UICMDSETTOOLINITIALIZE       :
			case UICMDGETTOOLINITIALIZE       :
			case UICMDGETTOOLCOORDINATE       :
			case UICMDSETTOOLCOORDINATE       :
			case UICMDOFFMOTOR                :
			case UICMDSETONMOTOR              :
			case UICMDGETMOTORANGEL           :
			case UICMDMOVEMOTOR               :
			case UICMDGETIOCONFIGURE          :
			case UICMDSETIOCONFIGURE          :
			case UICMDGETIOINPUTVALUE         :
			case UICMDSETIOOUPUTVALUE         :
			case UICMDGETIOANALOGY            :
			case UICMDSETIOPWMBASE            :
			case UICMDGETIOPWMBASE            :
			case UICMDSETIOPWMDUTY            :
			case UICMDGETIOPWMDUTY            :
			case UICMDGETIOOUTPUTVALUE        :
			case UICMDDOWNLOADFILENAME        :
			case UICMDDOWNLOADFILECONTENT     :
			case UICMDREQUIREFILENAME         :
			case UICMDSHUTDOWN                :
			case UICMDBLUETOOTHSETPASSWORD    :
			case UICMDGETROBOTTYPE            :
			case UICMDENTERCANMODE            :
			case UICMDTRANSLATECANDATA        :
			case UICMDEXITCANMODE             :
			case UICMDSETCAMERACONFIGURE      :
			case UICMDGETCAMERACONFIGURE      :
			case UICMDENTERCAMERAMODE         :
			case UICMDEXITCAMERAMODE          :
			case UICMDGETMASSPARA			  :
			case UICMDGETUSERCOOR             :
			case UICMDGETFIXDPOINT			  :
			case UICMDGETSAFEAREA			  :
			case UICMDSETMASSPARA			  :
			case UICMDSETUSERCOOR			  :
			case UICMDSETSAFEAREA			  :
			case UICMDSETFIXDPOINT			  :
			case UICMDGETLOGLIST			  :
			case UI_CMD_PAUSEPRO :
			case UI_CMD_GETUSERVARIABLE:
			case UI_CMD_GETROBOTCONFIG:
			case UI_CMD_SETROBOTCONFIG:
			case UICMDSETCACHE:
			case UICMDGETCACHE:
			case UI_CMD_IO_GET_VIRTUALINPUT_TTL:
			case UI_CMD_IO_SET_VIRTUALINPUT_TTL:
			case UI_CMD_IO_GET_VIRTUALOUTPUT_TTL:
			case UI_CMD_IO_SET_VIRTUALOUTPUT_TTL:
			case UI_CMD_SET_ROBOTMOVE:	
			case UI_CMD_SETPOINTEX:
			case UI_CMD_OPENSERIALPORT:		//2017.05.31
			case UI_CMD_WRITESERIALPORT:
			case UI_CMD_SERIALPORTRTCN:
			case UI_CMD_CLOSESERIALPORT:
			case UI_CMD_ROBOT_UPDATE_STATUS:
			case UI_CMD_ROBOT_UPDATE_FIRMWARE:
			case UI_CMD_NORMAL_STOP_ROBOT:
			case UI_CMD_SETPRGTRACEFLAG:
			case UI_CMD_GETPRGTRACEFLAG:
			case UI_CMD_GETPRGBREAKPOINT:
			case UI_CMD_GETCAMERACONNECTSTATUS:
			case UI_CMD_GETROBOTMODEL:
			case UI_CMD_STARTUPDATEFIRMWARE:
			case UI_CMD_GETROBOTMODELNAME:
			case UI_CMD_GET_TORQUELIMIT:
			case UI_CMD_SET_TORQUELIMIT:
			case UI_CMD_MOTORGOHOME:
			case UI_CMD_GET_ROBOTMOVESD:
			case UICMDREQUIRESFILENAME:
			case UI_CMD_GET_ROBOTMOVETM:
			case UICMDCHECKPOINT:
			case UI_CMD_SET_ROBOT_TCP:
			case UI_CMD_GET_ROBOT_TCP:
			case UI_CMD_GET_METERWHEEL:
			{
				m_iSocketReceiveDataLen[chCommand] = UIM_LITTLE_END_VALUE_FROM_BYTE(buff[2], buff[3], buff[ 4], buff[5]); 
				memcpy(m_chSocketReceiveBuffer[chCommand], &buff[6], m_iSocketReceiveDataLen[chCommand]);
				m_chSocketReceiveBuffer[chCommand][m_iSocketReceiveDataLen[chCommand]] = '\0';
				return 0;
			}
			case UICMDRECEIVECANDATA:
			{
			   if ((m_chSocketSendBuf[6]& 0x07F) == (buff[6] & 0x07F))
			   {			   
				   m_iSocketReceiveDataLen[chCommand] = UIM_LITTLE_END_VALUE_FROM_BYTE(buff[2], buff[3], buff[ 4], buff[5]); 
				   memcpy(m_chSocketReceiveBuffer[chCommand], &buff[6], m_iSocketReceiveDataLen[chCommand]);
				   m_chSocketReceiveBuffer[chCommand][m_iSocketReceiveDataLen[chCommand]] = '\0';
				   return 0;
			   }
			   else
			   {
			      return 1;
			   }
			}
		    case UICMDREALTIMENOTIFICATION :// 实时通知
			{
				int iType = (buff[2]) & 0x0FF;				
				switch (iType)
				{
			 		case UICMDSOCKETDISCONNECTED:
					{
						DisconnectSocket();
					}
					break;
				}
//#ifdef LINUX
#if 0
				int iMaxCopyLen = 0;
				char m_chCommandDataBuffer[UISDK_MAX_SDKBUFFER_LEN] = {0}; // 命令数据缓存	
				int iDataLen = SDKMIN(UIM_LITTLE_END_VALUE_FROM_BYTE(buff[3], buff[4], buff[5], buff[6]), UISDK_MAX_SDKBUFFER_LEN);
				if(iDataLen <= UISDK_MAX_SDKBUFFER_LEN)
				{
				    memcpy(m_chCommandDataBuffer,&buff[7], iDataLen);
				}
				if (pfRobotRtcnCallBack)
				{
					pfRobotRtcnCallBack(dwConnectCode, (buff[2])&0x0FF, m_chCommandDataBuffer,iDataLen);
				}	
				
				SDKTraceFile("end chCommand=0x%X,iType=0x%X\r\n",chCommand,iType);
#endif				
//#else
				P_UI_RTCNCALLBACK_OBJ pRtcnObj = new UI_RTCNCALLBACK_OBJ;
				pRtcnObj->cmd_type = (buff[2])&0x0FF;
				pRtcnObj->m_dwConnectCode = dwConnectCode;
                int iDataLen = SDKMIN(UIM_LITTLE_END_VALUE_FROM_BYTE(buff[3], buff[4], buff[5], buff[6]), UISDK_MAX_SDKBUFFER_LEN);
				if(iDataLen <= UISDK_MAX_SDKBUFFER_LEN)
				{
				    memcpy(pRtcnObj->m_chCommandDataBuffer,&buff[7], iDataLen);
				}
				pRtcnObj->m_chCommandDataLen = iDataLen;
				pRtcnObj->pSocketClient = this;
#ifdef LINUX				
				pthread_attr_t attr;
				struct sched_param param;
				if (0 != pthread_attr_init(&attr))
					return FALSE;				
				if (0 != pthread_attr_setschedpolicy(&attr, SCHED_RR))
					return FALSE;
				if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
					return FALSE;
				if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
					return FALSE;				
				param.sched_priority = 55;
				pthread_attr_setschedparam(&attr, &param);
				SDK_CREATETHREAD(m_hdSocketRtcnThread, attr, SocketRtcnThreadProc, pRtcnObj);				
				pthread_attr_destroy(&attr);
				
#else				
				SDK_CREATETHREAD(m_hdSocketRtcnThread, NULL, SocketRtcnThreadProc, pRtcnObj);
				::SetThreadPriority(m_hdSocketRtcnThread,55);				 //设置优先级 
				::ResumeThread(m_hdSocketRtcnThread); 

#endif
				return 0;
//#endif
			}
			return 0;
			case UICMDGETPROGRAMLIST:  // 获取文件列表
			{
				int iReceiveLen = UIM_LITTLE_END_VALUE_FROM_BYTE(buff[2], buff[3], buff[4], buff[5]);
				if(iReceiveLen > 2)
				{
					int iLen = m_iSocketReceiveDataLen[chCommand];	
					 //  接收的数据不能大于缓存
					if((iLen + iReceiveLen -1) >= UISDK_MAX_DATA_LEN)
					{
					    iReceiveLen = UISDK_MAX_DATA_LEN - iLen +1;
					}
					if(iReceiveLen > 2)
					{
						memcpy(&m_chSocketReceiveBuffer[chCommand][iLen], &buff[6+2], iReceiveLen - 2); // 去掉数据开头的两个字节
						iLen = iLen + iReceiveLen - 2;
						m_chSocketReceiveBuffer[chCommand][iLen] = '|';
						m_chSocketReceiveBuffer[chCommand][iLen+1] = '\0'; // 接收到的字符串少一个数据
						m_iSocketReceiveDataLen[chCommand] = iLen+1;
					}
				}
				else if (iReceiveLen == 1)
				{
					int iLen = m_iSocketReceiveDataLen[chCommand];
					m_chSocketReceiveBuffer[chCommand][iLen] = buff[6 + 2];// 去掉数据开头的两个字节
					m_iSocketReceiveDataLen[chCommand] = iLen;
				}
				return 0;
			}
			case UICMDTRANSMITSFILECONTENT:
			{
                 //  int iFileContentLen = UIM_LITTLE_END_VALUE_FROM_BYTE(buff[2], buff[3], buff[4], buff[5]);
				   P_UI_FILECONTENT_OBJ pFilecontentObj = (P_UI_FILECONTENT_OBJ)(&buff[6]);
				   P_UI_FILECONTENT_OBJ pFileObj = new UI_FILECONTENT_OBJ();
				   P_UI_WRITEFILECONTENT_OBJ pWriteParam = new UI_WRITEFILECONTENT_OBJ();
					pFileObj->length = (pFilecontentObj->length)&0x0FFFF;
					pFileObj->fileSize = pFilecontentObj->fileSize;
					memcpy(pFileObj->data,pFilecontentObj->data, pFilecontentObj->length);
					pWriteParam->pFileObj = pFileObj;
					pWriteParam->pSocketClient = this;
							
#ifdef LINUX
					pthread_attr_t attr;
					if (0 != pthread_attr_init(&attr))
						return FALSE;
					if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
						return FALSE;
					if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
						return FALSE;
					SDK_CREATETHREAD(m_hdSocketRtcnThread,attr,SocketSFileWirteThreadProc,pWriteParam);	
#else
					SDK_CREATETHREAD(m_hdSocketRtcnThread,NULL,SocketSFileWirteThreadProc,pWriteParam);
#endif
			}
			break;
			case UICMDTRANSMITFILECONTENT:// 控制层发送文件内容
			{ // 前一个指令是文件名传输，所以把数据放到文件名传输缓冲区
            //	int iFileContentLen = UIM_LITTLE_END_VALUE_FROM_BYTE(buff[2], buff[3], buff[4], buff[5]);
			       P_UI_FILECONTENT_OBJ pFilecontentObj = (P_UI_FILECONTENT_OBJ)(&buff[6]);
				   P_UI_FILECONTENT_OBJ pFileObj = new UI_FILECONTENT_OBJ();
				   P_UI_WRITEFILECONTENT_OBJ pWriteParam = new UI_WRITEFILECONTENT_OBJ();
					pFileObj->length = (pFilecontentObj->length)&0x0FFFF;
					pFileObj->fileSize = pFilecontentObj->fileSize;
					memcpy(pFileObj->data,pFilecontentObj->data, pFilecontentObj->length);
					pWriteParam->pFileObj = pFileObj;
					pWriteParam->pSocketClient = this;
				
#ifdef LINUX
		pthread_attr_t attr;
		if (0 != pthread_attr_init(&attr))
			return FALSE;
		if (0 != pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM))
			return FALSE;
		if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
			return FALSE;
		SDK_CREATETHREAD(m_hdSocketRtcnThread,attr,SocketFileWirteThreadProc,pWriteParam);	
#else
		SDK_CREATETHREAD(m_hdSocketRtcnThread,NULL,SocketFileWirteThreadProc,pWriteParam);
#endif
				return 0;
			}
		}
	//	strAckText = strTemp;


	}

	return 0;
}
/*==============================================================================
	NAME: unsigned int	SocketFileWirteThreadProc(LPVOID lpPara)
	----------------------------------------------------------------------------
	Function:处理接收到的文件保存
	----------------------------------------------------------------------------
	Parameter:
		  LPVOID lpPara:线程启动的参数，主要是数据缓存
	----------------------------------------------------------------------------
	Return Value:  
				   If the function fails, the return value is false. 
				   If the function succeeds, the return value is true. 
	----------------------------------------------------------------------------
	Note:	 

==============================================================================*/
unsigned int CSocketClient::DoSocketFileWirteThreadProc(LPVOID param)
{
	SDK_EnterCriticalSection(&m_hFileWriteCriSection);	
	P_UI_FILECONTENT_OBJ pFileObj = (P_UI_FILECONTENT_OBJ)param;
	if(m_outPutFile.is_open())
	{
	     if(pFileObj) 
	     {
			m_outPutFile.write(pFileObj->data, pFileObj->length);
			m_outFileSize = m_outFileSize + pFileObj->length;				   
            if((unsigned int)m_outFileSize == pFileObj->fileSize)
			{
				m_outPutFile.flush();
				m_bReceiveCmdAck = true;
				#ifdef WIN32
				SDK_EVENT_SIGNAL(m_hCmdWaitEvent);		
				#endif
			}
			char InPutbuffer[2]="1";
			SendMsg_C(InPutbuffer, UICMDTRANSMITFILECONTENT, 1);	
	     }
	}
	if(pFileObj)
	{	    
	    FreePtr(pFileObj);
	}
	SDK_LeaveCriticalSection(&m_hFileWriteCriSection);
	return 0;
}
unsigned int CSocketClient::DoSocketSFileWirteThreadProc(LPVOID param)
{
	SDK_EnterCriticalSection(&m_hFileWriteCriSection);	
	P_UI_FILECONTENT_OBJ pFileObj = (P_UI_FILECONTENT_OBJ)param;
	if(m_outPutFile.is_open())
	{
	     if(pFileObj) 
	     {
			m_outPutFile.write(pFileObj->data, pFileObj->length);
			m_outFileSize = m_outFileSize + pFileObj->length;				   
            if((unsigned int)m_outFileSize == pFileObj->fileSize)
			{
				m_outPutFile.flush();
				m_bReceiveCmdAck = true;
				#ifdef WIN32
				SDK_EVENT_SIGNAL(m_hCmdWaitEvent);		
				#endif
			}
			char InPutbuffer[2]="1";
			SendMsg_C(InPutbuffer, UICMDTRANSMITSFILECONTENT, 1);	
	     }
	}
	if(pFileObj)
	{	    
	    FreePtr(pFileObj);
	}
	SDK_LeaveCriticalSection(&m_hFileWriteCriSection);
	return 0;
}
/*==============================================================================
	NAME: int  UpProgramFile(unsigned int dwConnectType, P_UI_COMMANDBUFFER_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:控制层向应用层传输文件
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectType:连接类型
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int CSocketClient::UpProgramFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
{

	 if (!robotFileObj)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	char chSocketOutBuffer[UISDK_MAX_DATA_LEN] = { 0 }; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_DATA_LEN] = { 0 }; // 输入值缓存

	if(m_outPutFile.is_open())
	{
		return PROTOCOL_CMD_RETURN_FAILED;
	}
	 m_outFileSize = 0;
	 if(robotFileObj->chDataBuffer[0]=='~')  
	 {
	     m_outPutFile.open(robotFileObj->chDataBuffer,ofstream::binary);
	     robotFileObj->iCount = robotFileObj->iCount - 1;
	     memcpy(chSocketInBuffer,&(robotFileObj->chDataBuffer[1]),robotFileObj->iCount);
	 }
	 else
	 {
	     m_outPutFile.open(robotFileObj->chDataBuffer,ofstream::binary);
	     memcpy(chSocketInBuffer,robotFileObj->chDataBuffer,robotFileObj->iCount);
	 }
	 
	 if(!m_outPutFile.is_open())
	 {
	    return PROTOCOL_CMD_RETURN_FAILED;
	 }
	 m_bReceiveCmdAck = false;
	  int ret = SendSocketCommand(dwConnectCode,UICMDREQUIREFILENAME,robotFileObj->iCount, chSocketInBuffer,  chSocketOutBuffer,NULL);
	 if (ret == UISDK_SUCCESS)
	 {
	        if (chSocketOutBuffer[0] == '1') 
		   {
			SDK_EnterCriticalSection(&m_hCmdSockCriSection);			
#ifdef LINUX	
			int iSleepCount = 0;
		// 等待SDK_SOCKET_TIMEOUT_TOTAL*0.1ms
			while(iSleepCount < SDK_SOCKET_TIMEOUT_TOTAL)
			{
				iSleepCount++;
				usleep(100); // 0.1ms
				if(m_bReceiveCmdAck) break;
			}		
			// 仍未收到反馈，失败退出
			if(!m_bReceiveCmdAck)
			{
				m_outPutFile.close();
				SDK_LeaveCriticalSection(&m_hCmdSockCriSection);							  
				return PROTOCOL_CMD_ERROR_TIMEOUT;
			}
#else
			int timeout = 50500;
			if (SDK_WaitForSingleTimout(m_hCmdWaitEvent, m_hCmdSockCriSection, timeout))
			{
				  m_outPutFile.close();
				  SDK_LeaveCriticalSection(&m_hCmdSockCriSection);								
				  return PROTOCOL_CMD_ERROR_TIMEOUT;
			}
#endif                                                     
			m_outPutFile.close();
            SDK_LeaveCriticalSection(&m_hCmdSockCriSection);
		 }
		 else 
		 {				                            
			 m_outPutFile.close();
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 m_outPutFile.close();
	 return ret;	

}
/*==============================================================================
	NAME: int  UpSimulatorFile(unsigned int dwConnectType, P_UI_COMMANDBUFFER_OBJ robotFileObj)
	----------------------------------------------------------------------------
	Function:控制层向应用层传输文件
	----------------------------------------------------------------------------
	Parameter:
		 unsigned int dwConnectType:连接类型
		 P_UI_COMMANDBUFFER_OBJ robotFileObj::机器人程序文件数据结构
	----------------------------------------------------------------------------
	Return Value:  
				   成功则返回0
				   失败则返回失败原因
	----------------------------------------------------------------------------
	Note:
==============================================================================*/
int CSocketClient::UpSimulatorFile(unsigned int dwConnectCode, P_UI_COMMANDBUFFER_OBJ robotFileObj)
{

	 if (!robotFileObj)
	 {
	    return PROTOCOL_CMD_ERROR_PARAMETER;
	 }
	char chSocketOutBuffer[UISDK_MAX_DATA_LEN] = { 0 }; // 返回值缓存
	char chSocketInBuffer[UISDK_MAX_DATA_LEN] = { 0 }; // 输入值缓存

	if(m_outPutFile.is_open())
	{
		return PROTOCOL_CMD_RETURN_FAILED;
	}
	 m_outFileSize = 0;
	 if(robotFileObj->chDataBuffer[0]=='~')  
	 {
	     m_outPutFile.open(robotFileObj->chDataBuffer,ofstream::binary);
	     robotFileObj->iCount = robotFileObj->iCount - 1;
	     memcpy(chSocketInBuffer,&(robotFileObj->chDataBuffer[1]),robotFileObj->iCount);
	 }
	 else
	 {
	     m_outPutFile.open(robotFileObj->chDataBuffer,ofstream::binary);
	     memcpy(chSocketInBuffer,robotFileObj->chDataBuffer,robotFileObj->iCount);
	 }
	 
	 if(!m_outPutFile.is_open())
	 {
	    return PROTOCOL_CMD_RETURN_FAILED;
	 }
	 m_bReceiveCmdAck = false;
	  int ret = SendSocketCommand(dwConnectCode,UICMDREQUIRESFILENAME,robotFileObj->iCount, chSocketInBuffer,  chSocketOutBuffer,NULL);
	 if (ret == UISDK_SUCCESS)
	 {
	        if (chSocketOutBuffer[0] == '1') 
		   {
			SDK_EnterCriticalSection(&m_hCmdSockCriSection);			
#ifdef LINUX	
			int iSleepCount = 0;
		// 等待SDK_SOCKET_TIMEOUT_TOTAL*0.1ms
			while(iSleepCount < SDK_SOCKET_TIMEOUT_TOTAL)
			{
				iSleepCount++;
				usleep(100); // 0.1ms
				if(m_bReceiveCmdAck) break;
			}		
			// 仍未收到反馈，失败退出
			if(!m_bReceiveCmdAck)
			{
				m_outPutFile.close();
				SDK_LeaveCriticalSection(&m_hCmdSockCriSection);							  
				return PROTOCOL_CMD_ERROR_TIMEOUT;
			}
#else
			int timeout = 50500;
			if (SDK_WaitForSingleTimout(m_hCmdWaitEvent, m_hCmdSockCriSection, timeout))
			{
				  m_outPutFile.close();
				  SDK_LeaveCriticalSection(&m_hCmdSockCriSection);								
				  return PROTOCOL_CMD_ERROR_TIMEOUT;
			}
#endif                                                     
			m_outPutFile.close();
            SDK_LeaveCriticalSection(&m_hCmdSockCriSection);
		 }
		 else 
		 {				                            
			 m_outPutFile.close();
		      return PROTOCOL_CMD_RETURN_FAILED;
		 }
	 }
	 m_outPutFile.close();
	 return ret;	

}
