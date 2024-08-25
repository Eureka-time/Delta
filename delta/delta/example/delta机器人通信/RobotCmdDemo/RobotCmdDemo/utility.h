#ifndef UTILITY
#define UTILITY

#include <string>
#include <vector>

using namespace std;

#ifdef WIN32
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif

#ifdef WIN32
#include <winsock.h>
int gettimeofday(struct timeval *tp, void *tzp);
#endif 

void MySleep(unsigned int msec);
void split(string& s, string& delim,vector< std::string >* ret);
int TimeGapMS(timeval &time1, timeval &time2);
int TimeNowGapMS(timeval &time);
float TimeGapS(timeval &StartTime, timeval &EndTime);
string GetCurrTimestring();
int CopyFile(const char* src, const char* des);
bool IsTheBeginOfHour(void);
bool IsTheBeginOfDay(void);
void GetFilelistFromDir(char* _path, char* _files);
bool GetLatestFileFromDir(char* _path, char* _filename);

#endif // UTILITY

