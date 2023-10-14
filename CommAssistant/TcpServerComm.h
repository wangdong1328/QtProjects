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

class CTcpServerComm  : public CBaseComm
{
	Q_OBJECT

public:
	CTcpServerComm(QObject *parent = nullptr);
	~CTcpServerComm();

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
};
