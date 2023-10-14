#include "CommManager.h"
#include "UdpComm.h"

CCommManager::CCommManager(QObject *parent)
	: QObject(parent)
{
	for (size_t i = ECOMMTYPE_UNKNOWN; i < ECOMMTYPE_TOTAL; i++)
	{
		//Udp通信类指针
		CBaseComm* m_pUdpComm = nullptr;

		switch (ECommType(i))
		{
		case ECOMMTYPE_UDP:
			m_pUdpComm = new CUdpComm();
			m_mapComms[ECOMMTYPE_UDP] = m_pUdpComm;
			break;
		case ECOMMTYPE_TCP_SERVER:
			break;
		case ECOMMTYPE_TCP_CLIENT:
			break;
		case ECOMMTYPE_SERIAL:
			break;
		case ECOMMTYPE_TOTAL:
			break;
		default:
			break;
		}
	}
}

CCommManager::~CCommManager()
{
	for (auto iter = m_mapComms.begin(); iter != m_mapComms.end(); ++iter)
	{
		if (iter.value())
		{
			delete iter.value();
		}
	}

	m_mapComms.clear();
}

CCommManager* CCommManager::GetInstance()
{
	static CCommManager instance;
	return &instance;
}

int CCommManager::Send(ECommType eCommType, const QByteArray baContent, int iTimeInterval)
{
	int iRet = -1;
	QMap<ECommType, CBaseComm*>::iterator iter = m_mapComms.find(eCommType);
	if (iter != m_mapComms.end())
	{
		iRet = iter.value()->Send(baContent, iTimeInterval);
	}

	return iRet;
}

void CCommManager::RecvMsg(ECommType eCommType, const QByteArray baContent)
{
	emit RecvMsgSignal(eCommType, baContent);
}

void CCommManager::Register(ECommType eCommType, const SEndPointSettings sEndPoint)
{
	QMap<ECommType, CBaseComm*>::iterator iter = m_mapComms.find(eCommType);
	if (iter != m_mapComms.end())
	{
		iter.value()->BindEndPoint(sEndPoint);
	}
}

void CCommManager::Unregister(ECommType eCommType)
{
	//判断是否注册过此通信类型，注册过，则删除
	QMap<ECommType, CBaseComm*>::iterator iter = m_mapComms.find(eCommType);
	if (iter != m_mapComms.end())
	{
		iter.value()->UnBindEndPoint();
	}
}
