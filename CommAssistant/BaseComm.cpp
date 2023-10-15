#include "BaseComm.h"

CBaseComm::CBaseComm(QObject *parent)
	: QObject(parent)
{}

CBaseComm::~CBaseComm()
{}

void CBaseComm::OnErrorOccurredSlot(QAbstractSocket::SocketError socketError)
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
	emit RecvMsgSignal(QString("[UDP]: SocketError reason is %1. ").arg(strErrorReason));
}