#include "TcpServerComm.h"

CTcpServerComm::CTcpServerComm(QObject *parent)
	: CBaseComm(parent)
{}

CTcpServerComm::~CTcpServerComm()
{}

int CTcpServerComm::Send(QByteArray baContent, int iTimeInterval)
{
	return 0;
}

void CTcpServerComm::BindEndPoint(const SEndPointSettings sEndPoint)
{
}

void CTcpServerComm::UnBindEndPoint()
{
}
