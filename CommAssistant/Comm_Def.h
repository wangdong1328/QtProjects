#pragma once

#include <QString>

//通信类型
enum ECommType
{
	ECOMMTYPE_UNKNOWN = 0x00,
	ECOMMTYPE_UDP, 
	ECOMMTYPE_TCP_SERVER,
	ECOMMTYPE_TCP_CLIENT,
	ECOMMTYPE_SERIEL,
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
