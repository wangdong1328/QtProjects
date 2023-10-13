#pragma once
/**
 * @file   ：UdpComm.h
 * @author ：wangdong (wangdong1328@163.com)
 * @brief  ：.
 * @version: 0.1
 * @date   ：2023-10-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <QUdpSocket>
#include <QThread>
#include "BaseComm.h"

class CUdpComm  : public CBaseComm
{
	Q_OBJECT

public:
	CUdpComm(QObject *parent);
	~CUdpComm();

	/**
	 * @brief  :设置端点
	 *
	 * @param  :sEndPoint
	 * @return :void
	 */
	void BindEndPoint(const SEndPointSettings sEndPoint) override;

	/**
	 * @brief  :解除绑定的端点
	 *
	 * @return :void
	 */
	void UnBindEndPoint() override;

	/**
	 * @brief  :发送消息
	 *
	 * @param  :baContent,发送的内容
	 * @iTimeInterval  :时间间隔
	 * @return :int
	 */
	int Send(QByteArray baContent, int iTimeInterval) override;

private slots:
	//接收报文槽函数
	void OnReadPendingDatagramsSlot();
	//处理网络错误槽函数
	void OnErrorOccurredSlot(QAbstractSocket::SocketError);

private:
	//网络通信指针
	QUdpSocket* m_pUdpSocket = nullptr;
	//对端IP地址
	QString m_strPeerIPAddr = "";
	//对端端口
	unsigned short m_usPeerPort = 0;
};
