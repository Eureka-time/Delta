#include "stdafx.h"

#include "utility.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#ifdef WIN32
#include <windows.h>
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#else
#include <sys/time.h>
#include <unistd.h>
#endif

void MySleep(unsigned int msec)
{
#ifdef WIN32
	Sleep(msec);
#else
	usleep(msec * 1000);
#endif
}

void split(string& s, string& delim,vector< std::string >* ret)  
{  
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=string::npos)  
	{  
		ret->push_back(s.substr(last,index-last));  
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index-last>0)  
	{  
		ret->push_back(s.substr(last,index-last));  
	}  
}  

int TimeGapMS(timeval &time1, timeval &time2)
{
	int nS = time1.tv_sec - time2.tv_sec;
	int nUs =  time1.tv_usec - time2.tv_usec;
	int nMs = nS *1000 + nUs /1000;
	return nMs;
}

int TimeNowGapMS(timeval &time)
{
	timeval NowTime;
	gettimeofday(&NowTime, NULL);
	int nS = NowTime.tv_sec - time.tv_sec;
	int nUs =  NowTime.tv_usec - time.tv_usec;
	int nMs =nS *1000 + nUs /1000;
	return nMs;
}

float TimeGapS(timeval &StartTime, timeval &EndTime)
{
    int nS = StartTime.tv_sec - EndTime.tv_sec;
    int nUs =  StartTime.tv_usec - EndTime.tv_usec;
	float fMs = nS + (float)(nUs / 1000000.0);
    return fMs;
}


int CopyFile(const char* src, const char* des)
{
    int nRet = 0;
    FILE* pSrc = NULL, *pDes = NULL;
    pSrc = fopen(src, "r");
    pDes = fopen(des, "w+");

    if (pSrc && pDes)
    {
        int nLen = 0;
        char szBuf[4096] = {0};
        while((nLen = fread(szBuf, 1, sizeof szBuf, pSrc)) > 0)
        {
            fwrite(szBuf, 1, nLen, pDes);
        }
    }
    else
    {
        nRet = -1;
    }

    if (pSrc)
    {
        fclose(pSrc);
        pSrc = NULL;
    }

    if (pDes)
    {
        fclose(pDes);
        pDes = NULL;
    }

    return nRet;
}



/************************************************************************

Function: IsTheBeginOfDay

Description: Check if it is the begin of the day.

Input parameters: None.

Output parameters: None.

Return value:
    true : yes
    false : no

***********************************************************************/
bool IsTheBeginOfDay(void)
{
#ifdef LINUX
    time_t now_time;
    tm * local;
    static int iLastDay = -1;

    time(&now_time);

    local = localtime(&now_time);

    if ((0 == local->tm_sec)&&(0 == local->tm_min)&&(0 == local->tm_hour))
    {
        if(iLastDay != local->tm_mday)
        {
            iLastDay = local->tm_mday;

            return true;
        }
    }
#endif
    return false;
}


/************************************************************************

Function: IsTheBeginOfHour

Description: Check if it is the begin of the hour.

Input parameters: None.

Output parameters: None.

Return value:
    true : yes
    false : no

***********************************************************************/
bool IsTheBeginOfHour(void)
{
#ifdef LINUX
    time_t now_time;
    tm * local;
    static int iLastHour = -1;

    time(&now_time);

    local = localtime(&now_time);

    if ((0 == local->tm_sec)&&(0 == local->tm_min))
    {
        if(iLastHour != local->tm_hour)
        {
            iLastHour = local->tm_hour;

            return true;
        }
    }
#endif
    return false;
}
