#include "UdpComm.h"
#include <QNetworkDatagram>

CUdpComm::CUdpComm(QObject* parent)
	: CBaseComm(parent)
{
	m_pUdpSocket = new QUdpSocket(this);
}


CUdpComm::~CUdpComm()
{
	if (m_pUdpSocket)
	{
		delete m_pUdpSocket;
		m_pUdpSocket = nullptr;
	}
}

void CUdpComm::SetLocalEndPoint(const QString strLocalIPAddr, const unsigned short usPort)
{
	if (m_pUdpSocket->bind(QHostAddress(strLocalIPAddr), usPort))
	{
		connect(m_pUdpSocket, &QUdpSocket::readyRead, this, &CUdpComm::OnReadPendingDatagramsSlot);
		connect(m_pUdpSocket, &QUdpSocket::errorOccurred, this, &CUdpComm::OnErrorOccurredSlot);
	}
	else
	{
		//上报网络信息
		emit ReportInfoSignal("[UDP]: bind port failed! ");
	}

}

void CUdpComm::SetPeerEndPoint(const QString strPeerIPAddr, const unsigned short usPeerPort)
{
	m_strPeerIPAddr = strPeerIPAddr;
	m_usPeerPort = usPeerPort;
}

int CUdpComm::Send(const QString strMsg)
{
	int iRet = 0;
	//发送消息不为空时，发送消息
	if (m_pUdpSocket && !strMsg.isEmpty())
	{
		iRet = m_pUdpSocket->writeDatagram(strMsg.toUtf8(), QHostAddress(m_strPeerIPAddr), m_usPeerPort);
	}

	return iRet;
}

void CUdpComm::OnErrorOccurredSlot(QAbstractSocket::SocketError socketError)
{
	//上报网络错误结果
	emit ReportInfoSignal(QString("[UDP]: SocketError type is %1. ").arg((int)socketError));
}

void CUdpComm::OnReadPendingDatagramsSlot()
{
	while (m_pUdpSocket->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = m_pUdpSocket->receiveDatagram();
		//processTheDatagram(datagram);

		if ((datagram.senderAddress().toString() == m_strPeerIPAddr) && (m_usPeerPort == datagram.senderPort()))
		{

		}
	}
}