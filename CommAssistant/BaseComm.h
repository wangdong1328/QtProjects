#pragma once

#include <QObject>

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

signals:
	void ReportInfoSignal(QString);
};
