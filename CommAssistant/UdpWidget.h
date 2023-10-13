#pragma once

#include <QWidget>
#include "ui_UdpWidget.h"

class CUdpWidget : public QWidget
{
	Q_OBJECT

public:
	CUdpWidget(QWidget *parent = nullptr);
	~CUdpWidget();

	/**
	 * @brief  :初始化设置
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
	Ui::CUdpWidgetClass ui;
};
