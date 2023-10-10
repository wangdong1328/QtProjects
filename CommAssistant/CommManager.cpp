#include "CommManager.h"
#include "UdpComm.h"

CCommManager::CCommManager(QObject *parent)
	: QObject(parent)
{
	m_pUdpComm = new CUdpComm(NULL);
}

CCommManager::~CCommManager()
{
}

CCommManager* CCommManager::GetInstance()
{
	static CCommManager instance;
	return &instance;
}

int CCommManager::Send(const QString strMsg)
{
	return m_pUdpComm->Send(strMsg);
}

void CCommManager::RecvMsg(const QString strMsg)
{
	qDebug() << "recv content is " << strMsg;
}
