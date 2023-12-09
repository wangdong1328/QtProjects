#include "TcpServerComm.h"

CTcpServerComm::CTcpServerComm(QObject *parent)
	: CBaseComm(parent)
{
	m_pTcpServer = new QTcpServer(this);
}

CTcpServerComm::~CTcpServerComm()
{
	if (m_pTcpServer)
	{
		delete m_pTcpServer;
		m_pTcpServer = nullptr;
	}
}

int CTcpServerComm::Send(QByteArray baContent, int iTimeInterval)
{
	int iRet = -1;
	for (auto iter = m_mapTcpClientSockets.begin(); iter != m_mapTcpClientSockets.end(); ++iter)
	{
		if (iter.value())
		{
			iRet = iter.value()->write(baContent);
		}
	}
	return iRet;
}

int CTcpServerComm::Send(QByteArray baContent, QString strClientInfo, int iTimeInterval)
{
	int iRet = -1;
	if (strClientInfo.isEmpty())
	{
		iRet = Send(baContent, iTimeInterval);
	}
	else
	{
		auto iter = m_mapTcpClientSockets.find(strClientInfo);
		if (iter != m_mapTcpClientSockets.end())
		{
			if (iter.value())
			{
				iRet = iter.value()->write(baContent);
			}
		}
	}

	return iRet;
}

void CTcpServerComm::BindEndPoint(const SEndPointSettings sEndPoint)
{
	if (m_pTcpServer)
	{
		if (m_pTcpServer->listen(QHostAddress(sEndPoint.sPeerEndPoint.strIPAddr),
			sEndPoint.sPeerEndPoint.usPort))
		{
			connect(m_pTcpServer, &QTcpServer::newConnection, this, [&]()
				{
					if (m_pTcpServer->hasPendingConnections())
					{
						QTcpSocket* pTcpSocket = m_pTcpServer->nextPendingConnection();

						DealWithClientSocketConnect(pTcpSocket);

						QString strClientEndPoint = QString("%1:%2")
							.arg(pTcpSocket->peerAddress().toString()).arg(pTcpSocket->peerPort());

						//存储连接的客户端
						m_mapTcpClientSockets[strClientEndPoint] = pTcpSocket;
						emit ConnectMsgSignal();
					}
				});
		}
		else
		{
			emit RecvMsgSignal("[TCP] listen faied!!!");
		}
	}
}

void CTcpServerComm::UnBindEndPoint()
{
	m_pTcpServer->close();

	for (auto iter = m_mapTcpClientSockets.begin(); iter != m_mapTcpClientSockets.end(); ++iter)
	{
		iter.value()->close();
		iter.value()->disconnectFromHost();
	}
	m_mapTcpClientSockets.clear();
}

QStringList CTcpServerComm::GetClientConnectList()
{
	QStringList lstClients;

	for (auto iter = m_mapTcpClientSockets.begin(); iter != m_mapTcpClientSockets.end(); ++iter)
	{
		lstClients << iter.key();
	}

	return lstClients;
}

void CTcpServerComm::RemoveClientConnectSocket(QTcpSocket* pClientSocket)
{
	for (auto iter = m_mapTcpClientSockets.begin(); iter != m_mapTcpClientSockets.end();)
	{
		if (pClientSocket == iter.value())
		{
			iter = m_mapTcpClientSockets.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	emit DisConnectMsgSignal();
}

void CTcpServerComm::DealWithClientSocketConnect(QTcpSocket* pClientSocket)
{
	//处理消息接收
	connect(pClientSocket, &QTcpSocket::readyRead, this, [=]()
		{
			QByteArray ba = pClientSocket->readAll();

			QString strClientEndPoint = QString("[%1:%2]")
				.arg(qobject_cast<QTcpSocket*>(sender())->peerAddress().toString())
				.arg(qobject_cast<QTcpSocket*>(sender())->peerPort());

			emit RecvMsgSignal(strClientEndPoint + ba);

		});

	//处理消息接收
	connect(pClientSocket, &QTcpSocket::disconnected, this, [=]()
		{
			RemoveClientConnectSocket(qobject_cast<QTcpSocket*>(sender()));
		});
}
