#pragma once

#include <QObject>
#include "Comm_Def.h"
#include <QMap>
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
	 * @param  :eCommType, 通信类型
	 * @return :int
	 */
	int Send(ECommType eCommType, const QString strMsg);

	/**
	 * @brief  :接收消息分发
	 *
	 * @param  :strMsg
	 * @param  :eCommType, 通信类型
	 * @return :void
	 */
	void RecvMsg(ECommType eCommType, const QString strMsg);

	/**
	 * @brief  : 根据通信类型注册对应通信类
	 * 
	 *
	 * @param  :eCommType, 通信类型
	 * @param  :sEndPointSettings, 通信类型需要的IP和端口
	 * @return :void
	 */
	void Register(ECommType eCommType, const SEndPointSettings sEndPointSettings);
	
	/**
	 * @brief  : 根据通信类型注册对应通信类
	 *
	 *
	 * @param  :eCommType, 通信类型
	 * @return :void
	 */
	void Unregister(ECommType eCommType);

signals:
	//接收消息信号机
	void RecvMsgSignal(ECommType eCommType, const QString strMsg);

private:
	//通信类型保存
	QMap<ECommType, CBaseComm*> m_mapComms;
};
