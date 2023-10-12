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

private:
	Ui::CUdpWidgetClass ui;
};
