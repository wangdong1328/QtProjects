#pragma once
/**
 * @file   :TcpServerComm.h
 * @author :wangdong (wangdong1328@163.com)
 * @brief  :Tcp服务器通信类
 * @version: 0.1
 * @date   :2023-10-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "BaseComm.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class CTcpServerComm  : public CBaseComm
{
	Q_OBJECT

public:
	CTcpServerComm(QObject *parent = nullptr);
	~CTcpServerComm();

	/**
	 * 
	 */
	int Send(QByteArray baContent, int iTimeInterval = 10) override;

	/**
	 * @brief  :发送文件内容接口
	 *
	 * @param  :baContent
	 * @param  :iTimeInterval
	 * @param  :strClientInfo
	 * @return :int
	 */
	int Send(QByteArray baContent, QString strClientInfo, int iTimeInterval = 10) override;

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
	 * @brief  :获取客户端连接列表
	 *
	 * @return :QStringList
	 */
	QStringList GetClientConnectList();

private:
	/**
	 * @brief  :移除客户端连接
	 *
	 * @param  :pClientSocket
	 * @return :void
	 */
	void RemoveClientConnectSocket(QTcpSocket* pClientSocket);

	/**
	 * @brief  :处理客户端socket
	 *
	 * @return :void
	 */
	void DealWithClientSocketConnect(QTcpSocket* pClientSocket);

private:
	//TCP服务器通信指针
	QTcpServer* m_pTcpServer = nullptr;
	//TCP通信客户端Socket列表
	QMap<QString, QTcpSocket*> m_mapTcpClientSockets;
};
