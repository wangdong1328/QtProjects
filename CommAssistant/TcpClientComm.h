#pragma once
/**
 * @file   :TcpClientComm.h
 * @author :wangdong (wangdong1328@163.com)
 * @brief  :TCP客户端通信类
 * @version: 0.1
 * @date   :2023-10-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "BaseComm.h"
#include <QTcpSocket>

class CTcpClientComm  : public CBaseComm
{
	Q_OBJECT

public:
	CTcpClientComm(QObject *parent = nullptr);
	~CTcpClientComm();

	/**
	 * @brief  :发送文件内容接口
	 *
	 * @param  :baContent, 发送的内容
	 * @return :int
	 */
	int Send(QByteArray baContent, int iTimeInterval) override;

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

private:
	//网络通信指针
	QTcpSocket* m_pTcpSocket = nullptr;
};
