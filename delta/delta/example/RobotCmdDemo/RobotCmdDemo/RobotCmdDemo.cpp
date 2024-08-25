// ConsoleApplication10.cpp : �������̨Ӧ�ó������ڵ㡣
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


	


	//��ȡ�����˵�ǰ�߶�
	RobotCoordinate coord;
	int ret = robot.GetRobotCoor(coord);
	if (UISDK_SUCCESS != ret)
	{
		return -1;
	}
	//�ӵ�ǰλ��̧���߶�
	float fSafeHeight = -30;
	coord.fTCPz = fSafeHeight;
	if (UISDK_SUCCESS !=robot.LineMoveTo(coord, 120))
	{
		return -1;
	}
	//�˶����
	RobotCoordinate coorFrom{ -100,50,-100,0 };
	//�˶��յ�
	RobotCoordinate coorTo{ 100,-50,-100,90 };
	//�˶��м��
	RobotCoordinate coorMid{ 50,0,-100,90 };


	//ֱ���˶�����
	if (1)
	{
		robot.SetSpeedRate(50);
	
		//500Ϊ�˶�500ms
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

			if (UISDK_SUCCESS != robot.LineMoveTo(coorTo, 500))
			{
				return -1;
			}


			Sleep(100);
		}

	}




	//SPPMOVE ��׼�켣��2���˶�����ָ����Lg1,lg2,�Ǿ��������˶�
	if (1)
	{
		//500Ϊ�˶�500ms
		if (UISDK_SUCCESS != robot.LineMoveTo(coorFrom, 500))
		{
			return -1;
		}
		//�������Ϳ��ṹ����
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


	//CPPMOVE��׼�켣�˶���������м���ȵ�
	if (1)
	{

		if (UISDK_SUCCESS != robot.LineMoveTo(coorFrom, 500))
		{
			return -1;
		}
		//�������Ϳ��ṹ����
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

	//robot io ������IO����
	if (1)
	{
		while (1)
		{
			robot.SetRobotOutPortDigitalValue(1, 1);
			Sleep(1000);
			robot.SetRobotOutPortDigitalValue(1, 0);
			Sleep(1000);
		}

		//io output  //�����6��IO����Ӧ����9-14
		/*

		for (int i = 0; i < 6; i++)
		{
			robot.SetRobotOutPortDigitalValue(9 + i, 1);
			Sleep(500);
			robot.SetRobotOutPortDigitalValue(9 + i, 0);
			Sleep(500);
		}
				*/
		//ѭ���������IO
		while (true)
		{
			int nIOValue[6] = {0};
			//���빲6��IO
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

