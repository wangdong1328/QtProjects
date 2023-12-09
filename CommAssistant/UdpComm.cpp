
#include "UdpComm.h"
#include <QNetworkDatagram>

static const int MAX_LENGTH = 2048;

CUdpComm::CUdpComm(QObject* parent)
	: CBaseComm(parent)
{
	m_pUdpSocket = new QUdpSocket(this);
	connect(m_pUdpSocket, &QUdpSocket::readyRead, this, &CUdpComm::OnReadPendingDatagramsSlot);
	connect(m_pUdpSocket, &QUdpSocket::errorOccurred, this, &CUdpComm::OnErrorOccurredSlot);
}

CUdpComm::~CUdpComm()
{
	m_pUdpSocket->close();
	if (m_pUdpSocket)
	{
		delete m_pUdpSocket;
		m_pUdpSocket = nullptr;
	}
}

void CUdpComm::BindEndPoint(const SEndPointSettings sEndPoint)
{
	if (m_pUdpSocket->bind(QHostAddress(sEndPoint.sLocalEndPoint.strIPAddr), sEndPoint.sLocalEndPoint.usPort))
	{
		m_strPeerIPAddr = sEndPoint.sPeerEndPoint.strIPAddr;
		m_usPeerPort = sEndPoint.sPeerEndPoint.usPort;
	}
	else
	{
		emit RecvMsgSignal("[UDP]: bind port failed!");
	}
}

void CUdpComm::UnBindEndPoint()
{
	m_pUdpSocket->disconnectFromHost();
	m_strPeerIPAddr = "";
	m_usPeerPort = 0;
}

int CUdpComm::Send(QByteArray baContent, int iTimeInterval)
{
	int iRet = 0;
	//发送消息不为空时，发送消息
	if (m_pUdpSocket && (!baContent.isEmpty()))
	{
		int iSize = baContent.count();
		int iSendPackageCount = baContent.count() / MAX_LENGTH;
		if (0 != (baContent.count() % MAX_LENGTH))
		{
			iSendPackageCount++;
		}

		for (size_t iIndex = 0; iIndex < iSendPackageCount; iIndex++)
		{
			if ((iIndex + 1) == iSendPackageCount)
			{
				m_pUdpSocket->writeDatagram(baContent, QHostAddress(m_strPeerIPAddr),
					m_usPeerPort);
			}
			else
			{
				m_pUdpSocket->writeDatagram(baContent.mid(0, MAX_LENGTH),
					QHostAddress(m_strPeerIPAddr), m_usPeerPort);

				baContent = baContent.mid(MAX_LENGTH);
			}
		}
	}

	return iRet;
}


void CUdpComm::OnReadPendingDatagramsSlot()
{
	while (m_pUdpSocket->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = m_pUdpSocket->receiveDatagram();

		if ((datagram.senderAddress().toString() == m_strPeerIPAddr) &&
			(m_usPeerPort == datagram.senderPort()))
		{
			emit RecvMsgSignal(datagram.data());
		}
	}
}