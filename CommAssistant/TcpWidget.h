#pragma once

#include <QWidget>
#include "ui_TcpWidget.h"
#include "Comm_Def.h"

class CBaseComm;
class CTcpWidget : public QWidget
{
	Q_OBJECT

public:
	CTcpWidget(QWidget* parent = nullptr);
	~CTcpWidget();

	/**
	 * @brief  :初始化UI
	 *
	 * @return :void
	 */
	void InitUI();

	/**
	 * @brief  :获取当前时间
	 *
	 * @return :QString
	 */
	static QString GetCurrentDateTime();

private:
	Ui::CTcpWidgetClass ui;
	//Tcp客户端通信指针
	CBaseComm* m_pTcpClientComm = nullptr;
	//Tcp服务端通信指针
	CBaseComm* m_pTcpServerComm = nullptr;
	//当前通信类指针
	CBaseComm* m_pCurrentComm = nullptr;
	//当前客户端端点
	QString m_strCurClientEndpoint = "";
};
