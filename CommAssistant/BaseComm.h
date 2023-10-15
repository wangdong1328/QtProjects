#pragma once

#include <QObject>
#include "Comm_Def.h"
#include <QAbstractsocket>

class CBaseComm  : public QObject
{
	Q_OBJECT

public:
	CBaseComm(QObject *parent);
	virtual ~CBaseComm();

	/**
	 * @brief  :发送消息接口
	 *
	 * @param  :baContent
	 * @param  :iTimeInterval
	 * @return :int
	 */
	virtual int Send(QByteArray baContent, int iTimeInterval = 10) = 0;


	/**
	 * @brief  :设置端点
	 *
	 * @param  :sEndPoint
	 * @return :void
	 */
	virtual void BindEndPoint(const SEndPointSettings sEndPoint) = 0;

	/**
	 * @brief  :解除绑定的端点
	 *
	 * @return :void
	 */
	virtual void UnBindEndPoint() = 0;

	/**
	 * @brief  : 这个重载可以选择实现,发送文件内容接口
	 *
	 * @param  :baContent
	 * @param  :iTimeInterval
	 * @param  :strClientInfo, 这个参数只供发送Tcp服务端使用
	 * @return :int
	 */
	virtual int Send(QByteArray baContent, QString strClientInfo, int iTimeInterval = 10) { return 0; }

	/**
	 * @brief  :获取客户端列表
	 *
	 * @return :QStringList
	 */
	virtual QStringList GetClientConnectList() { return QStringList(); }

protected slots:
	//处理错误信息
	void OnErrorOccurredSlot(QAbstractSocket::SocketError socketError);

signals:
	//接收消息信号
	void RecvMsgSignal(const QString strMsg);
	//接收消息信号
	void ConnectMsgSignal();
	//接收消息信号
	void DisConnectMsgSignal();

};
