#pragma once
/**
 * @file    SerialAssistant.h
 * @author  wangdong (wangdong1328@163.com)
 * @brief   串口助手代码
 * @version 0.1
 * @date    2023-12-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <QtWidgets/QWidget>
#include "ui_SerialAssistant.h"
#include <QSerialPort>
#include <QDateTime>

class SerialAssistant : public QWidget
{
	Q_OBJECT

public:
	SerialAssistant(QWidget* parent = nullptr);
	~SerialAssistant();

private:
	/**
	 * @brief 初始化
	 *
	 * @return void
	 */
	void Init();

	/**
	 * @brief 获取当前日期时间
	 *
	 * @return QString
	 */
	static QString GetDateTime() { return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"); }

private slots:
	// 打开关闭按钮槽函数
	void OnPushButtonOpenSlot();
	//发送按钮槽函数
	void OnPushButtonSendSlot();
	//接收消息槽函数
	void OnRecvMessageSlot();

private:
	Ui::SerialAssistantClass ui;
	//串口通信指针
	QSerialPort* m_pSerialport{ nullptr };
};
