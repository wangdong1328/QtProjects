#pragma once

#include <QWidget>
#include "ui_TcpWidget.h"
#include "Comm_Def.h"

class CTcpWidget  : public QWidget
{
	Q_OBJECT

public:
	CTcpWidget(QWidget *parent = nullptr);
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
	//存储当前类型
	ECommType m_eCommType;
};
