#pragma once

#include <QString>

//通信类型
enum ECommType
{
	ECOMMTYPE_UNKNOWN = 0x00,
	//UDP通信
	ECOMMTYPE_UDP, 
	//TCP通信服务端
	ECOMMTYPE_TCP_SERVER,
	//TCP通信客户端
	ECOMMTYPE_TCP_CLIENT,
	//串口通信
	ECOMMTYPE_SERIAL,  
	//通信类型总计
	ECOMMTYPE_TOTAL,
};

struct SEndPoint
{
	QString strIPAddr;
	unsigned short usPort;

	SEndPoint()
	{
		strIPAddr = "";
		usPort = 0;
	}
};

//端点设置
struct SEndPointSettings
{
	//本地端点
	SEndPoint sLocalEndPoint;

	//对端端点
	SEndPoint sPeerEndPoint;
};
