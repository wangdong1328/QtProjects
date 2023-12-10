#include "TcpClientComm.h"

CTcpClientComm::CTcpClientComm(QObject* parent)
	: CBaseComm(parent)
{
	m_pTcpSocket = new QTcpSocket(this);

	connect(m_pTcpSocket, &QTcpSocket::readyRead, this, [&]()
		{
			QByteArray ba = m_pTcpSocket->readAll();
			emit RecvMsgSignal(ba);
		});

	connect(m_pTcpSocket, &QTcpSocket::errorOccurred, this, &CTcpClientComm::OnErrorOccurredSlot);
}

CTcpClientComm::~CTcpClientComm()
{
	if (m_pTcpSocket)
	{
		delete m_pTcpSocket;
		m_pTcpSocket = nullptr;
	}
}

int CTcpClientComm::Send(QByteArray baContent, int iTimeInterval)
{
	int iRet = -1;
	if (m_pTcpSocket)
	{
		iRet = m_pTcpSocket->write(baContent);
	}
	return iRet;
}

void CTcpClientComm::BindEndPoint(const SEndPointSettings sEndPoint)
{
	if (m_pTcpSocket)
	{
		m_pTcpSocket->connectToHost(sEndPoint.sPeerEndPoint.strIPAddr, sEndPoint.sPeerEndPoint.usPort);
	}
}

void CTcpClientComm::UnBindEndPoint()
{
	if (m_pTcpSocket)
	{
		m_pTcpSocket->disconnectFromHost();
	}
}
