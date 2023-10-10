#pragma once

#include <QObject>

class CBaseComm;

class CCommManager  : public QObject
{
	Q_OBJECT

	CCommManager(QObject *parent = nullptr);

public:
	~CCommManager();
	CCommManager(const CCommManager& cm) = delete;
	CCommManager& operator=(const CCommManager& cm) = delete;

	/**
	 * @brief  : 获取通信类CCommManager的指针
	 *
	 * @return :CCommManager*
	 */
	static CCommManager* GetInstance();

	/**
	 * @brief  :发送文件内容接口
	 *
	 * @param  :strMsg
	 * @return :int
	 */
	int Send(const QString strMsg);

	/**
	 * @brief  :接收消息分发
	 *
	 * @param  :strMsg
	 * @return :void
	 */
	void RecvMsg(const QString strMsg);

private:
	//Udp通信类指针
	CBaseComm* m_pUdpComm = nullptr;
};
