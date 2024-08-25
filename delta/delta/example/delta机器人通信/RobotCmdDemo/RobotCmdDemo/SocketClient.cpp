#include <winsock2.h>
#include "stdafx.h"
#include <stdio.h>
#pragma comment (lib,"ws2_32")


int     main(int argc, char* argv[])
{

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);//初始化

    SOCKET  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sockaddr;
    sockaddr.sin_family = PF_INET;
    sockaddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.55");
    sockaddr.sin_port = htons(400);//设置要连接的IP和端口

    connect(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));//tcp连接
    char    buffer[200] = { 0 };
    recv(s, buffer, 200, NULL);//

    printf("response:%s\n", buffer);
    WSACleanup();
    return 0;
}
