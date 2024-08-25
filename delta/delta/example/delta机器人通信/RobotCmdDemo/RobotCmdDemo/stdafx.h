// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//


#pragma once

#ifndef WIN32
#define LINUX 1 
#endif


#include "fstream"
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <vector>
using namespace std;

//#pragma comment(lib,"UIRobotFunc.lib") 


// 跨平台有关定义
#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h> 
#include <WS2tcpip.h>
#include <MSTCPiP.h>
#endif
#include <assert.h>
#include "io.h"
#elif LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <termios.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string>
#include <linux/can.h>
#include <linux/can/raw.h> 
#include <linux/can/error.h> 
#include <sys/ioctl.h> 
#include <arpa/inet.h> 
#include <net/if.h>
#include <stdarg.h>
#include <limits.h>
#include <syslog.h>
#include <cmath>


#ifdef LINUX
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <algorithm>

#ifndef VOID
#define VOID void
#endif

typedef 	void*				  LPVOID;
typedef int BOOL;
#define TRUE  		1
#define FALSE 		0

#endif

#include "UIRobotStruct.h"
#include "UIError.h"
#include "UIRobotProtocol.h"



#ifdef LINUX
#define SDKsscanf sscanf
#define  sprintf_s  snprintf
#else
#define SDKsscanf sscanf_s
#endif


// TODO:  在此处引用程序需要的其他头文件

// 跨平台有关定义
#ifdef WIN32

#define SDK_CRITICAL_SECTION  CRITICAL_SECTION
#define SDK_EVENT_HANDLE      HANDLE
#define SDK_EVENT_SIGNAL(event)	  SetEvent(event)
#define SDK_EVENT_RESET(event)	  ResetEvent(event)
#define SDK_EVENT_CREATE(event)   event=CreateEvent(NULL, FALSE, FALSE, NULL)
#define SDK_EVENT_DELETE(event)   CloseHandle(event)

#define SDK_InitializeCriticalSection(mutex)      InitializeCriticalSection(mutex)
#define SDK_DeleteCriticalSection(mutex)          DeleteCriticalSection(mutex)
#define SDK_EnterCriticalSection(mutex)	          EnterCriticalSection(mutex)
#define SDK_LeaveCriticalSection(mutex)	          LeaveCriticalSection(mutex)
#define SDK_THREADPROC                            DWORD WINAPI
#define SDK_CREATETHREAD(handle,attr,proc,param)  handle = CreateThread(0, 0, proc, (LPVOID)(param), 0, 0)
#define SDK_DELETETHREAD(handle)	 CloseHandle(handle)
#define SDK_WaitForSingleTimout(event,mutex,timeout)  (WAIT_TIMEOUT == WaitForSingleObject(event, timeout))
#define SDKOutputDebugString(str)  //     SDKTraceFile(str)

#define SDK_SOCKET_HANDLE  SOCKET
#define SDK_SOCKADDR_HANDLE  SOCKADDR_IN
#define CAN_EFF_FLAG 0x80000000U 
#define SDK_PTHREAD_COND_INITIALIZER  NULL


#define SDKMIN(a,b)(((a) < (b)) ? (a) : (b))

#define SDK_TIMEVAL           TIMEVAL

#elif LINUX
#define SDK_CRITICAL_SECTION  pthread_mutex_t
#define SDK_EVENT_HANDLE	  pthread_cond_t
#define SDK_EVENT_SIGNAL(event)	 pthread_cond_signal(&event) //{char chTraceBuffer[256]; sprintf_s(chTraceBuffer, 256, "SDK_EVENT_SIGNAL %d,%s\n", __LINE__,__FUNCTION__);debug(chTraceBuffer);} // pthread_cond_signal(&event)
#define SDK_EVENT_RESET(event)	  (event)//
#define SDK_EVENT_CREATE(event)  pthread_cond_init(&event, NULL) //{char chTraceBuffer[256]; sprintf_s(chTraceBuffer, 256, "SDK_EVENT_CREATE %d,%s\n", __LINE__,__FUNCTION__);debug(chTraceBuffer);} // pthread_cond_init(&event, NULL);
#define SDK_EVENT_DELETE(event)	  pthread_cond_destroy(&event)

#define SDK_InitializeCriticalSection(mutex)  pthread_mutex_init(mutex,NULL)
#define SDK_DeleteCriticalSection(mutex)  pthread_mutex_destroy(mutex )
#define SDK_EnterCriticalSection(mutex)	  pthread_mutex_lock(mutex)
#define SDK_LeaveCriticalSection(mutex)	  pthread_mutex_unlock(mutex)
#define SDK_THREADPROC				  void *
#define SDK_CREATETHREAD(handle,attr,proc,param)	 pthread_create(&handle, &attr, proc, (LPVOID)(param));
#define SDK_DELETETHREAD(handle)	 pthread_join(handle, NULL)
#define SDK_WaitForSingleTimout(event,mutex,timeout)  0 != pthread_cond_timedwait(&event, &mutex, &timeout)

#define SDK_PTHREAD_COND_INITIALIZER  PTHREAD_COND_INITIALIZER

#define SDK_SOCKET_HANDLE  int
#define SDK_SOCKADDR_HANDLE  struct sockaddr_in
#define SDK_TIMEVAL    struct timeval
#define SD_BOTH 2
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#define SDKMIN(a,b) (a)<(b)?(a):(b)
#define  sprintf_s  snprintf
#define _access     access
#define  MAX_PATH  _PC_PATH_MAX

#define robotdebug(fmt, args...)    //    do {syslog(LOG_DEBUG, fmt, ##args); } while(0)
#define robotprintf  printf
#define SDKOutputDebugString(str)    //     SDKTraceFile(str)


#define Sleep(T)	usleep(T*1000)
typedef      unsigned long int   HANDLE;
#define API_WINDOW_EXTERN

#endif



#define FreePtr(p) \
do{\
 if(p != NULL)\
	{\
	   delete p;\
	   p = NULL;\
	};\
}while(0)

