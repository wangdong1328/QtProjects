#pragma once

#include <QString>

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
