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
	 * @brief  :设置本地端点
	 *
	 * @param  :strLocalIPAddr,[I],本地IP地址
	 * @param  :usLocalPort,[I],本地端口
	 * @return :void
	 */
	void SetLocalEndPoint(const QString strLocalIPAddr, const unsigned short usLocalPort);

	/**
	 * @brief  :设置对端端口
	 *
	 * @param  :strPeerIPAddr
	 * @param  :usPeerPort
	 * @return :void
	 */
	void SetPeerEndPoint(const QString strPeerIPAddr, const unsigned short usPeerPort);

	/**
	 * @brief  :发送消息
	 *
	 * @param  :strMsg
	 * @return :int
	 */
	int Send(const QString strMsg) override;

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
