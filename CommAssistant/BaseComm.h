#pragma once

#include <QObject>
#include "Comm_Def.h"

class CBaseComm  : public QObject
{
	Q_OBJECT

public:
	CBaseComm(QObject *parent);
	virtual ~CBaseComm();

	/**
	 * @brief  :发送文件内容接口
	 *
	 * @param  :strMsg
	 * @return :int
	 */
	virtual int Send(const QString strMsg) = 0;

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

signals:
	void ReportInfoSignal(QString);
};
