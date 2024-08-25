
#include "stdafx.h"
#include "UIError.h"
/*
* 将字符转换为数值
* */
int c2i(char ch)
{
	// 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2  
	if (isdigit(ch))
		return ch - 48;

	// 如果是字母，但不是A~F,a~f则返回  
	if (ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z')
		return -1;

	// 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10  
	// 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10  
	if (isalpha(ch))
		return isupper(ch) ? ch - 55 : ch - 87;

	return -1;
}

/*
* 功能：将十六进制字符串转换为整型(int)数值
* */
int hex2dec(const char *hex)
{
	int len;
	int num = 0;
	int temp;
	int bits;
	int i;

	// 此例中 hex = "1de" 长度为3, hex是main函数传递的  
	len = strlen(hex);

	for (i = 0, temp = 0; i < len; i++, temp = 0)
	{
		// 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1  
		// 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13  
		// 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14  
		temp = c2i(*(hex + i));
		// 总共3位，一个16进制位用 4 bit保存  
		// 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位  
		// 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位  
		// 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位  
		bits = (len - i - 1) * 4;
		temp = temp << bits;

		// 此处也可以用 num += temp;进行累加  
		num = num | temp;
	}

	// 返回结果  
	return num;
}
/*=============================================================================
	 NAME:  CUIError()
	 -------------------------------------------------------------------------- 
	 Function:错误代码解析类构造函数
	 -------------------------------------------------------------------------- 
	 Parameter:		
	 -------------------------------------------------------------------------- 
	 Return Value:
 ------------------------------------------------------------------------------ 
	 Note:
=============================================================================*/
CUIError::CUIError()
{
    
}
/*=============================================================================
	 NAME:  ~CUIError()
	 -------------------------------------------------------------------------- 
	 Function:错误代码解析类析构函数
	 -------------------------------------------------------------------------- 
	 Parameter:		
	 -------------------------------------------------------------------------- 
	 Return Value:
 ------------------------------------------------------------------------------ 
	 Note:
=============================================================================*/
CUIError::~CUIError()
{

}


/*=============================================================================
	 NAME:  char * getErrorCode(int iErrorCode)
	 -------------------------------------------------------------------------- 
	 Function:错误代码解析类析构函数
	 -------------------------------------------------------------------------- 
	 Parameter:		
	 -------------------------------------------------------------------------- 
	 Return Value:
 ------------------------------------------------------------------------------ 
	 Note:
=============================================================================*/
string CUIError::getErrorCode(int iErrorCode)
{
    unsigned int uiErrCode = iErrorCode;
    switch(uiErrCode)
    {
        case SDK_RETURN_SUCCESS:
        {
            m_strError = "Success!";
        }
        break;
        case ERROR_CERTIFICATION_FAIL:
        {
            m_strError = "Error:Robot algorithms authentication fail.";
        }
        break;
        case ERROR_OPEN_INIFILE_FAIL:
        {
            m_strError = "Error:Robot algorithms open inifile fail.";
        }
        break;
        case ERROR_OPEN_CFGFILE_FAIL:
        {
           m_strError = "Error:Robot algorithms open config file fail.";
        }
        break;
        case ERROR_INIT_ROBOT_NDEF:
        {
           m_strError = "Error:Robot algorithms configuration type not defined";
        }
        break;
        case ERROR_OPEN_DATAFILE_FAIL:
        {
            m_strError = "Error:Robot algorithms create simulation data file fail.";

        }
        break;
        case ERROR_PARA_POINT_NULL:
        {
            m_strError = "Error:Robot algorithms parameter point is null.";
        }
        break;
        case ERROR_PARA_TRJTM_OVER:
        {
            m_strError = "Error:Robot algorithms parameter travel time is too long.";

        }
        break;
        case ERROR_PARA_SPEED_ZERO:
        {
            m_strError = "Robot algorithms parameter speed is zero.";

        }
        break;
        case ERROR_BELT_SPEED_ZERO:
        {
           m_strError = "Error:Robot algorithms belt speed is zero.";
        }
        break;
        case WARNG_POSI_LIMIT_XYLH:
        {
            m_strError = "Warning:Robot algorithms XY position is upper limit.";

        }
        break;
        case WARNG_POSI_LIMIT_XYLL:
        {
            m_strError = "Warning:Robot algorithms XY position is lower limit.";

        }
        break;
        case WARNG_POSI_LIMIT_PZLH:
        {
            m_strError = "Warning:Robot algorithms Z position is upper limit.";

        }
        break;
        case WARNG_POSI_LIMIT_PZLL:
        {
            m_strError = "Warning:Robot algorithms Z position is lower limit.";
        }
        break;
        case WARNG_POSI_LIMIT_PRLH:
        {
            m_strError = "Warning:Robot algorithms rotation position is upper limit.";

        }
        break;
        case WARNG_POSI_LIMIT_PRLL:
        {
            m_strError = "Warning:Robot algorithms rotation position is lower limit.";

        }
        break;
        case ERROR_POSI_LIMIT_XYLH:
        {
            m_strError = "Error:Robot algorithms XY position is upper limit.";

        }
        break;
        case ERROR_POSI_LIMIT_XYLL:
        {
            m_strError = "Error:Robot algorithms XY position is lower limit.";

        }
        break;
        case ERROR_POSI_LIMIT_PZLH:
        {
            m_strError = "Error:Robot algorithms Z position is upper limit.";

        }
        break;
        case ERROR_POSI_LIMIT_PZLL:
        {
            m_strError = "Error:Robot algorithms Z position is lower limit.";

        }
        break;
        case ERROR_POSI_LIMIT_PRLH:
        {
            m_strError = "Error:Robot algorithms rotation position is upper limit.";

        }
        break;
        case ERROR_POSI_LIMIT_PRLL:
        {
            m_strError = "Error:Robot algorithms rotation position is lower limit.";

        }
        break;
        case WARNG_AXIS_RANGE_POS1:
        {
            m_strError = "Warning:Robot algorithms axis 1 angle is upper limit.";

        }
        break;
        case WARNG_AXIS_RANGE_NEG1:
        {
            m_strError = "Warning:Robot algorithms axis 1 angle is lower limit.";

        }
        break;
        case WARNG_AXIS_RANGE_POS2:
        {
            m_strError = "Warning:Robot algorithms axis 2 angle is upper limit.";

        }
        break;
        case WARNG_AXIS_RANGE_NEG2:
        {
            m_strError = "Warning:Robot algorithms axis 2 angle is lower limit.";

        }
        break;
        case WARNG_AXIS_RANGE_POS3:
        {
            m_strError = "Warning:Robot algorithms axis 3 angle is upper limit.";

        }
        break;
        case WARNG_AXIS_RANGE_NEG3:
        {
            m_strError = "Warning:Robot algorithms axis 3 angle is lower limit.";

        }
        break;
        case WARNG_AXIS_RANGE_POS4:
        {
            m_strError = "Warning:Robot algorithms axis 4 angle is upper limit.";

        }
        break;
        case WARNG_AXIS_RANGE_NEG4:
        {
            m_strError = "Warning:Robot algorithms axis 4 angle is lower limit.";

        }
        break;

        case ERROR_AXIS_RANGE_POS1:
        {
            m_strError = "Error:Robot algorithms axis 1 angle is upper limit.";

        }
        break;
        case ERROR_AXIS_RANGE_NEG1:
        {
            m_strError = "Error:Robot algorithms axis 1 angle is lower limit.";

        }
        break;
        case ERROR_AXIS_RANGE_POS2:
        {
            m_strError = "Error:Robot algorithms axis 2 angle is upper limit.";

        }
        break;
        case ERROR_AXIS_RANGE_NEG2:
        {
            m_strError = "Error:Robot algorithms axis 2 angle is lower limit.";

        }
        break;
        case ERROR_AXIS_RANGE_POS3:
        {
            m_strError = "Error:Robot algorithms axis 3 angle is upper limit.";

        }
        break;
        case ERROR_AXIS_RANGE_NEG3:
        {
            m_strError = "Error:Robot algorithms axis 3 angle is lower limit.";

        }
        break;
        case ERROR_AXIS_RANGE_POS4:
        {
            m_strError = "Error:Robot algorithms axis 4 angle is upper limit.";

        }
        break;
        case ERROR_AXIS_RANGE_NEG4:
        {
            m_strError = "Error:Robot algorithms axis 4 angle is lower limit.";

        }
        break;
        case WARNG_AXIS_SPEED_HIGH1:
        {
            m_strError = "Warning:Robot algorithms axis 1 speed is upper limit.";

        }
        break;
        case WARNG_AXIS_SPEED_HIGH2:
        {
            m_strError = "Warning:Robot algorithms axis 2 speed is upper limit.";

        }
        break;
        case WARNG_AXIS_SPEED_HIGH3:
        {
           m_strError = "Warning:Robot algorithms axis 3 speed is upper limit.";

        }
        break;
        case WARNG_AXIS_SPEED_HIGH4:
        {
           m_strError = "Warning:Robot algorithms axis 4 speed is upper limit.";

        }
        break;
        case ERROR_DIVIDED_BYZERO:
        {
          m_strError = "Error:Robot algorithms divided by zero.";

        }
        break;
        case ERROR_SDK_ERROR_UART_HADLE:
        {
             m_strError = "Error:UIRobot SDK open uart fail.";

        }
        break;
        case ERROR_SDK_ERROR_BWRT_NDATA:
        {
            m_strError = "Error:UIRobot SDK uart write function parameter error.";

        }
        break;
        case ERROR_SDK_ERROR_BWRT_TMOUT:
        {
           m_strError = "Error:UIRobot SDK uart write time out.";

        }
        break;
        case ERROR_SDK_ERROR_BWRT_ERPNT:
        {
            m_strError = "Error:UIRobot SDK uart write index number error.";

        }
        break;
        case ERROR_SDK_ERROR_BWRT_REWRT:
        {
           m_strError = "Error:UIRobot SDK uart write retry max times.";

        }
        break;
        case ERROR_SDK_ERROR_SHUTDOWN:
        {
          m_strError = "Error:UIRobot SDK is closed.";

        }
        break;
        case ERROR_SDK_ERROR_PACKAGE:
        {
             m_strError = "Error:UIRobot SDK package message fail.";

        }
        break;
        case ERROR_SDK_ERROR_TIMEOUT:
        {
            m_strError = "Error:UIRobot SDK command wait time out.";

        }
        break;
        case ERROR_SDK_ERROR_ANAYLSEPACKAGE:
        {
            m_strError = "Error:UIRobot SDK receive message error.";

        }
        break;
        case ERROR_SDK_ERROR_ANAYLSENOTPACKAGE:
        {
            m_strError = "Error:UIRobot SDK anaylse message in buffer is null.";

        }
        break;
        case ERROR_SDK_ERROR_NOT_OUTBUFFER:
        {
            m_strError = "Error:UIRobot SDK anaylse message out buffer is null.";

        }
        break;
        case ERROR_SDK_ERROR_RECEIVE_ERROR:
        {
            m_strError = "Error:UIRobot SDK receive error command message.";

        }
        break;
        case ERROR_SDK_ERROR_ANAYLSE_ERROR_LEN:
        {
            m_strError = "Error:UIRobot SDK receive error command message data length.";

        }
        break;
        case ERROR_SDK_ERROR_PARAMETER:
        {
           m_strError = "Error:UIRobot SDK function in parameter error.";

        }
        break;
        case ERROR_SDK_ERROR_WRITE_ERROR:
        {
            m_strError = "Error:UIRobot SDK write fail.";

        }
        break;
        case ERROR_SDK_ERROR_TCW_ERROR:
        {
            m_strError = "Error:UIRobot SDK error command.";

        }
        break;
        case ERROR_SDK_ERROR_PORT_ERROR:
        {
           m_strError = "Error:UIRobot SDK port error.";

        }
        break;
        case ERROR_SDK_ERROR_INITIAL_ERROR:
        {
            m_strError = "Error:UIRobot SDK initialize error.";

        }
        break;
        case ERROR_USBC_ERROR_NO_DEVICE:
        {
            m_strError = "Error:UIRobot SDK usb no device.";

        }
        break;
        case ERROR_USBC_ERROR_USB_WRITE:
        {
           m_strError = "Error:UIRobot SDK usb write error.";

        }
        break;
        case ERROR_USBC_ERROR_USB_READ:
        {
            m_strError = "Error:UIRobot SDK usb read error.";

        }
        break;
        case RTCN_ROBOT_SYS_ERROR:
        {
            m_strError = "Error:UIRobot robot sys error.";

        }
        break;
        case ERROR_PVTWRITE_FAILED:
        {
            m_strError = "Error:UIRobot robot write pvt fail.";

        }
        break;
        case ERROR_URGENT_STOP_FAILED:
        {
           m_strError = "Error:UIRobot robot emergy stop fail.";

        }
        break;
        case ERROR_GETPVT_FAILED:
        {
             m_strError = "Error:UIRobot robot get pvt list fail.";

        }
        break;
        case ERROR_CAMERAOBJECT_NOTFOUND_FAILED:
        {
             m_strError = "Error:UIRobot robot camera index object fail.";

        }
        break;
        case ERROR_ROBOT_STATE_FAILED:
        {
            m_strError = "Error:UIRobot robot stat error.";

        }
        break;
        case ERROR_ROBOT_COORDINATE_FAILED:
        {
            m_strError = "Error:UIRobot robot error coordinate.";

        }
        break;
        case ERROR_ROBOT_TRANSMODEL_STATE_FAILED:
        {
            m_strError = "Error:UIRobot robot state is translate mode communication.";

        }
        break;
        case ERROR_ROBOT_FILE_NOT_EXIST_FAILED:
        {
            m_strError = "Error:UIRobot robot file not exist.";

        }
        break;
        case ERROR_ROBOT_FILE_NOT_PROGRAM_FAILED:
        {
            m_strError = "Error:UIRobot robot is not pragram file.";

        }
        break;
        case ERROR_ROBOT_TEMPERATURE:
        {
            m_strError = "Error:UIRobot robot motor controler temperature is upper limit.";

        }
        break;
        case ERROR_ROBOT_EMERGYSTOP:
        {
           m_strError = "Error:UIRobot robot emergy stop fail.";

        }
        break;
        case ERROR_ROBOT_FILE_OPEN_FAILED:
        {
            m_strError = "Error:UIRobot robot file open fail.";

        }
        break;
        case ERROR_ROBOT_CAN_BUS_FAILED:
        {
            m_strError = "Error:UIRobot robot CAN communication fail.";

        }
        break;
        case ERROR_ROBOT_STALL_FAILED:
        {
            m_strError = "Error:UIRobot robot stall.";

        }
        break;
        case ERROR_MOTOR_DISABLE_FAILED:
        {
            m_strError = "Error:UIRobot robot motor controler is disable.";

        }
        break;
        case ERROR_ROBOT_FILE_SIZE_FAILED:
        {
            m_strError = "Error:UIRobot robot file size is upper limit.";

        }
        break;
        case ERROR_ROBOT_RECV_UI_FILE_TIMEOUT_FAILED:
        {
            m_strError = "Error:UIRobot robot receive file time out.";

        }
        break;
        case ERROR_ROBOT_FILE_ROBOTTYPE_FAILED:
        {
            m_strError = "Error:UIRobot robot file error type.";

        }
        break;
        case ERROR_ROBOT_MOTOR_RTCN_FAILED:
        {
            m_strError = "Error:UIRobot robot motor controler error.";

        }
        break;
        case ERROR_ROBOT_MALLOC_MEMORY_FAILED:
        {
            m_strError = "Error:UIRobot robot alloc memory fail.";

        }
        break;
        case ERROR_MOTOR_SOFTSAFE_ERR:
        {
            m_strError = "Error:UIRobot robot coordinate error.";

        }
        break;
        case ERROR_MOTORCOUNT:
        {
           m_strError = "Error:UIRobot robot number of motor controler error.";

        }
        break;
        case ERROR_BATTERYLOW:
        {
            m_strError = "Error:UIRobot robot motor controler battery is lower limit.";

        }
        break;
        case ERROR_ROBOT_POSITION_COORDINATE_FAILED:
        {
            m_strError = "Error:UIRobot robot coordinate position error.";

        }
        break;
        case ERROR_MOTOR_LOCKED_FAILED:
        {
           m_strError = "Error:UIRobot robot motor controler locked.";

        }
        break;
        case ERROR_TASK_PVT_COUNT_ZERO:
        {
            m_strError = "Error:UIRobot robot get empty pvt.";

        }
        break;
        case ERROR_PVT_EPCHK_FAILED:
        {
            m_strError = "Error:UIRobot robot end position check fail.";

        }
        break;
        case ERROR_ROBOT_SAFEAREA_X_FAILED:
        {
            m_strError = "Error:UIRobot robot x safe position fail.";

        }
        break;
        case ERROR_ROBOT_SAFEAREA_Y_FAILED:
        {
            m_strError = "Error:UIRobot robot y safe position fail.";

        }
        break;
        case ERROR_ROBOT_SAFEAREA_Z_FAILED:
        {
            m_strError = "Error:UIRobot robot z safe position fail.";

        }
        break;
        case ERROR_ROBOT_CURRENT_COORDINATE_FAILED:
        {
            m_strError = "Error:UIRobot robot start position is not current position.";

        }
        break;
        case ROBOT_RETURN_TIMEOUT:
        {
            m_strError = "Error:Application wait robot time out.";

        }
        break;
        case ROBOT_RETURN_ERRORSTATS:
        {
            m_strError = "Error:Application robot state fail.";

        }
        break;
        case ROBOT_RETURN_CONNECTFAILED:
        {
            m_strError = "Error:Application robot connect fail.";

        }
        break;
        case ROBOT_RETURN_NOTARRIVED:
        {
            m_strError = "Error:Application robot move end position fail.";

        }
        break;
        case ROBOT_RETURN_ERRORGROUPNAME:
        {
            m_strError = "Error:Application parameter file robot group name fail.";

        }
        break;
        case ROBOT_RETURN_ERRORROBOTNAME:
        {
            m_strError = "Error:Application parameter file robot name fail.";

        }
        break;		
        case ROBOT_RETURN_ERROR_PARAMETER:
        {
            m_strError = "Error:Application robot move error parameter.";

        }
        break;
        case UIMODULE_RETURN_TIMEOUT:
        {
            m_strError = "Error:Application uimodule wait time out.";

        }
        break;
        case UIMODULE_RETURN_ERRORSTATS:
        {
           m_strError = "Error:Application uimodule state fail.";

        }
        break;
        case UIMODULE_RETURN_ERRORPARAM:
        {
            m_strError = "Error:Application uimodule parameter fail.";

        }
        break;
        case UIMODULE_RETURN_INITERROR:
        {
            m_strError = "Error:Application uimodule not initialize.";

        }
        break;
        case METERCOUNTINGWHEEL_RETURN_INITFAILED:
        {
           m_strError = "Error:Application meter wheel init fail.";

        }
        break;
        case PROTOCOL_CMD_RETURN_FAILED:
        {
            m_strError = "Error:Application protocol return error.";

        }
        break;
        case PROTOCOL_CMD_ERROR_PARAMETER:
        {
           m_strError = "Error:Application protocol in parameter fail.";

        }
        break;
        case PROTOCOL_CMD_ERROR_TIMEOUT:
        {
            m_strError = "Error:Application protocol command wait time out.";

        }
        break;
        case PROTOCOL_SOCKETTINITIALERROR:
        {
           m_strError = "Error:Application protocol init fail.";

        }
        break;
        case PROTOCOL_SOCKETSENDERROR:
        {
            m_strError = "Error:Application protocol socket send fail.";

        }
        break;
        default:
        {
            if(RTCN_ROBOT_SYS_ERROR == (iErrorCode & RTCN_ROBOT_SYS_ERROR))
            {//  ERROR_ROBOT_RTCN_SYS_ERROR | ((pMsgObj->canNodeId<< 16)&0x0FF0000) |((pMsgObj->Data[1]<< 8)&0x0FF00)  | (pMsgObj->Data[0]&0x0FF);
                int iMotorId = ((iErrorCode >> 16)&0x0FF);
                int iMotorErrorCode = ((iErrorCode >> 8)&0x0FF);
                char chErrorBuffer[128] = {0};
               // string strError;
                switch(iMotorErrorCode)
                {
                    case RTCN_ERR_SYS_GWSD:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d goup alarm.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_LOCK:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d goup locked.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_STOP:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d emergy stop.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_OVHT:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d temperature is upper limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_POVL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d power supply over load.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_SCME:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d servo communication fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_SRST:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d servo restart.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_SALM:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d servo alarm.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_SIOE:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d servo io fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_SLAV:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d slave fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_CTX_TMOT:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d CAN time out.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_ACC:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d acceleration limited.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_EBRK:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d brake locked.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_NBRK:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d brake not locked.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_MOFF:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d is disable.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_SPHL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d speed is upper limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_PALL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d position is lower limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_PAHL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d position is upper limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_SPEL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d speed error is upper limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_PQER:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d position error is upper limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_ENCS:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d encoder communicate fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_ENCB:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d battery fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_INS_SYNT:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d instructions syntax error.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_INS_NUMB:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d instructions data error.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_INS_IDXR:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d instructions index error.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_SYS_STTM:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d is moving.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_DCSD:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d SD lower DC.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_RUNG:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d set on moving.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_MXN_STOG:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d origin on moving.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_PVT_RUNG:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d set pv on moving.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_PVT_WPOV:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d MP[6] fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_PVT_IOFN:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d QA fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_PVB_OVFL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d PVT buffer over flow.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_IOC_ADIO:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d io config fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_URT_SHT:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART length lower limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_URT_OLN:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART length upper limit.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_URT_TMO:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART time out.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_URT_CRC:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART crc fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_URT_NEN:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART end fail.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_RRB_OVF:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART RRB overflow.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_RRB_LVW:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART RRB level Warning.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                    case RTCN_ERR_CSB_FUL:
                    {
                        sprintf(chErrorBuffer,"Error:Controler node %d UART CSB full Warning.",iMotorId);
                        m_strError = chErrorBuffer;
                    }
                    break;
                }

            }
        }
        break;
    }

    return m_strError;
}

int CUIError::PraseErrorCode(int iErrorCode)
{
    getErrorCode(iErrorCode);

    return SDK_RETURN_SUCCESS;
}


