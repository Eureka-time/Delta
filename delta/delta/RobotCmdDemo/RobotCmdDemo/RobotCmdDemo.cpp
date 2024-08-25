// ConsoleApplication10.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Robot.h"
#include <iostream>

using namespace std;

int main()
{
	CRobot robot;
	robot.SetRobotName("Delta700");
	robot.Init();
	int nRet = robot.doConnect();
	if (UISDK_SUCCESS != nRet)
	{
		cout << "Robot Connected fail";
		return nRet;
	}
	//获取机器人当前高度
	RobotCoordinate coord;
	int ret = robot.GetRobotCoor(coord);
	if (UISDK_SUCCESS != ret)
	{
		return -1;
	}
	//从当前位置抬升高度
	float fSafeHeight = -30;
	coord.fTCPz = fSafeHeight;
	if (UISDK_SUCCESS !=robot.LineMoveTo(coord, 120))
	{
		return -1;
	}
	//运动起点
	RobotCoordinate coorFrom{ -100,50,-100,0 };
	//运动终点
	RobotCoordinate coorTo{ 100,-50,-100,90 };
	//运动中间点
	RobotCoordinate coorMid{ 50,0,-100,90 };


	//直线运动测试
	if (1)
	{
		robot.SetSpeedRate(50);
		//500为运动500ms
		if (UISDK_SUCCESS != robot.LineMoveTo(coorFrom, 500))
		{
			return -1;
		}
	
		for (int  i = 0;i < 3; i++)
		{
				
			
			if (UISDK_SUCCESS != robot.LineMoveTo(coorFrom, 500))
			{
				
				return -1;
			}
			if(UISDK_SUCCESS == robot.LineMoveTo(coorFrom, 500))
			{
				robot.SetRobotOutPortDigitalValue(1, 1);
				Sleep(1000);
				
			}

			if (UISDK_SUCCESS != robot.LineMoveTo(coorTo, 500))
			{
				return -1;
			}
			if (UISDK_SUCCESS == robot.LineMoveTo(coorTo, 500))
			{
				robot.SetRobotOutPortDigitalValue(1, 0);
				Sleep(1000);
			}

			Sleep(100);
		}

	}




	//SPPMOVE 标准轨迹，2点运动，若指定了Lg1,lg2,那就是门型运动
	if (1)
	{
		//500为运动500ms
		if (UISDK_SUCCESS != robot.LineMoveTo(coorFrom, 500))
		{
			return -1;
		}
		//参数解释看结构定义
		SPPMoveParam moveParam;
		moveParam.Hgt = 30;
		moveParam.Lg1 = 20;
		moveParam.Lg2 = 20;
		moveParam.V1 = 100;
		moveParam.V2 = 300;
		moveParam.V3 = 100;


		
		for (int i = 0; i < 10; i++)
		{

			if (UISDK_SUCCESS != robot.SPPMoveTo(coorTo, moveParam))
			{
				return -1;

			}


			if (UISDK_SUCCESS != robot.SPPMoveTo(coorFrom, moveParam))
			{
				return -1;
			}


			Sleep(100);
		}


	}


	//CPPMOVE标准轨迹运动，加入个中间过度点
	if (1)
	{

		if (UISDK_SUCCESS != robot.LineMoveTo(coorFrom, 500))
		{
			return -1;
		}
		//参数解释看结构定义
		SPPMoveParam moveParam;
		moveParam.Hgt = 30;
		moveParam.Lg1 = 20;
		moveParam.Lg2 = 20;
		moveParam.V1 = 100;
		moveParam.V2 = 300;
		moveParam.V3 = 100;



		for (int i = 0; i < 10; i++)
		{

			if (UISDK_SUCCESS != robot.CPPMoveTo(coorMid, coorTo, moveParam))
			{
				return -1;
			}


			if (UISDK_SUCCESS != robot.CPPMoveTo(coorMid, coorFrom, moveParam))
			{
				return -1;
			}


			Sleep(100);
		}

	}

	//robot io 机器人IO测试
	if (1)
	{
		//io output  //输出共6个IO，对应索引9-14
		for (int i = 0; i < 6; i++)
		{
			robot.SetRobotOutPortDigitalValue(1+ i, 1);
			Sleep(500);
			robot.SetRobotOutPortDigitalValue(1 + i, 0);
			Sleep(500);
		}
		
		//循环检测输入IO
		while (true)
		{
			int nIOValue[6] = {0};
			//输入共6个IO
			for (int i = 0; i < 6; i++)
			{
				robot.GetRobotInPortDigitalValue(1 + i, nIOValue[i]);
			}
	
			cout << " IO_1:" << nIOValue[0]
				<< " IO_2:" << nIOValue[1]
				<< " IO_3:" << nIOValue[2]
				<< " IO_4:" << nIOValue[3]
				<< " IO_5:" << nIOValue[4]
				<< " IO_6:" << nIOValue[5]
				<< endl;

			Sleep(1000);
		}


	}
    return 0;
}


