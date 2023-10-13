
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
	QString strErrorReason = "";

	switch (socketError)
	{
	case QAbstractSocket::ConnectionRefusedError:
		strErrorReason = "ConnectionRefusedError";
		break;
	case QAbstractSocket::RemoteHostClosedError:
		strErrorReason = "RemoteHostClosedError";
		break;
	case QAbstractSocket::HostNotFoundError:
		strErrorReason = "HostNotFoundError";
		break;
	case QAbstractSocket::SocketAccessError:
		strErrorReason = "SocketAccessError";
		break;
	case QAbstractSocket::SocketResourceError:
		strErrorReason = "SocketResourceError";
		break;
	case QAbstractSocket::SocketTimeoutError:
		strErrorReason = "SocketTimeoutError";
		break;
	case QAbstractSocket::DatagramTooLargeError:
		strErrorReason = "DatagramTooLargeError";
		break;
	case QAbstractSocket::NetworkError:
		strErrorReason = "NetworkError";
		break;
	case QAbstractSocket::AddressInUseError:
		strErrorReason = "AddressInUseError";
		break;
	case QAbstractSocket::SocketAddressNotAvailableError:
		strErrorReason = "SocketAddressNotAvailableError";
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		strErrorReason = "UnsupportedSocketOperationError";
		break;
	case QAbstractSocket::UnfinishedSocketOperationError:
		strErrorReason = "UnfinishedSocketOperationError";
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		strErrorReason = "ProxyAuthenticationRequiredError";
		break;
	case QAbstractSocket::SslHandshakeFailedError:
		strErrorReason = "SslHandshakeFailedError";
		break;
	case QAbstractSocket::ProxyConnectionRefusedError:
		strErrorReason = "ProxyConnectionRefusedError";
		break;
	case QAbstractSocket::ProxyConnectionClosedError:
		strErrorReason = "ProxyConnectionClosedError";
		break;
	case QAbstractSocket::ProxyConnectionTimeoutError:
		strErrorReason = "ProxyConnectionTimeoutError";
		break;
	case QAbstractSocket::ProxyNotFoundError:
		strErrorReason = "ProxyNotFoundError";
		break;
	case QAbstractSocket::ProxyProtocolError:
		strErrorReason = "ProxyProtocolError";
		break;
	case QAbstractSocket::OperationError:
		strErrorReason = "OperationError";
		break;
	case QAbstractSocket::SslInternalError:
		strErrorReason = "SslInternalError";
		break;
	case QAbstractSocket::SslInvalidUserDataError:
		strErrorReason = "SslInvalidUserDataError";
		break;
	case QAbstractSocket::TemporaryError:
		strErrorReason = "TemporaryError";
		break;
	case QAbstractSocket::UnknownSocketError:
		strErrorReason = "UnknownSocketError";
		break;
	default:
		break;
	}
	//上报网络错误结果
	CCommManager::GetInstance()->RecvMsg(ECOMMTYPE_UDP, QString("[UDP]: SocketError reason is %1. ")
		.arg(strErrorReason));
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