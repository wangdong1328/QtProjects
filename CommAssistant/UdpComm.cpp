#include "UdpComm.h"
#include <QNetworkDatagram>
#include "CommManager.h"

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
		//上报网络信息
		CCommManager::GetInstance()->RecvMsg(ECOMMTYPE_UDP, QString("[UDP]: bind port failed!"));
	}
}

void CUdpComm::UnBindEndPoint()
{
	m_pUdpSocket->disconnectFromHost();
	m_strPeerIPAddr = "";
	m_usPeerPort = 0;
}

int CUdpComm::Send(const QString strMsg)
{
	int iRet = 0;
	//发送消息不为空时，发送消息
	if (m_pUdpSocket && !strMsg.isEmpty())
	{
		iRet = m_pUdpSocket->writeDatagram(strMsg.toUtf8(), QHostAddress(m_strPeerIPAddr), 
			m_usPeerPort);
	}

	return iRet;
}

void CUdpComm::OnErrorOccurredSlot(QAbstractSocket::SocketError socketError)
{
	//上报网络错误结果
	CCommManager::GetInstance()->RecvMsg(ECOMMTYPE_UDP, QString("[UDP]: SocketError type is %1. ")
		.arg((int)socketError));
}

void CUdpComm::OnReadPendingDatagramsSlot()
{
	while (m_pUdpSocket->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = m_pUdpSocket->receiveDatagram();

		if ((datagram.senderAddress().toString() == m_strPeerIPAddr) && 
			(m_usPeerPort == datagram.senderPort()))
		{
			CCommManager::GetInstance()->RecvMsg(ECOMMTYPE_UDP, datagram.data());
		}
	}
}