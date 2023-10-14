#include "TcpClientComm.h"

CTcpClientComm::CTcpClientComm(QObject *parent)
	: CBaseComm(parent)
{}

CTcpClientComm::~CTcpClientComm()
{}

int CTcpClientComm::Send(QByteArray baContent, int iTimeInterval)
{
	return 0;
}

void CTcpClientComm::BindEndPoint(const SEndPointSettings sEndPoint)
{
}

void CTcpClientComm::UnBindEndPoint()
{
}
