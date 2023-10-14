#include "TcpWidget.h"
#include "CustomIntValidator.h"
#include "CommManager.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

CTcpWidget::CTcpWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//默认创建的时TCP通信的客户端
	m_eCommType = ECOMMTYPE_TCP_CLIENT;

	InitUI();
}

CTcpWidget::~CTcpWidget()
{}

void CTcpWidget::InitUI()
{
	//设置服务器端点输入格式
	ui.lineEdit_peerPort->setValidator(new CCustomIntValidator(1, 65535, this));
	ui.lineEdit_peerIp->setInputMask("000.000.000.000");

	//设置输入间隔格式
	ui.lineEdit_sendInterval->setValidator(new CCustomIntValidator(0, 10000, this));

	//默认断开连接按钮置灰
	ui.pushButton_close->setEnabled(false);
	//初始化禁用发送按钮
	ui.pushButton_send->setEnabled(false);
	
	//打开按钮
	connect(ui.pushButton_open, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(true);
			ui.pushButton_send->setEnabled(true);
			ui.pushButton_open->setEnabled(false);

			SEndPointSettings sEndPointSettings;
			sEndPointSettings.sPeerEndPoint.strIPAddr = ui.lineEdit_peerIp->text();
			sEndPointSettings.sPeerEndPoint.usPort = ui.lineEdit_peerPort->text().toUShort();

			CCommManager::GetInstance()->Register(m_eCommType, sEndPointSettings);
		});

	//处理关闭按钮
	connect(ui.pushButton_close, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(false);
			ui.pushButton_send->setEnabled(false);
			ui.pushButton_open->setEnabled(true);

			CCommManager::GetInstance()->Unregister(m_eCommType);
		});

	//处理发送按钮
	connect(ui.pushButton_send, &QPushButton::clicked, this, [=]()
		{
			if (CCommManager::GetInstance()->Send(m_eCommType,
			ui.lineEdit_msg->text().toUtf8()) > 0)
			{
				ui.plainTextEdit_content->appendPlainText("[S" + GetCurrentDateTime() + "] "
					+ ui.lineEdit_msg->text());

				ui.lineEdit_msg->clear();
			}
		});

	//处理选中服务器
	connect(ui.checkBox_server, &QCheckBox::stateChanged, this, [&](int iValue)
		{
			if (iValue > 0)
			{
				m_eCommType = ECOMMTYPE_TCP_SERVER;
				ui.pushButton_open->setText("监听");
			}
			else
			{
				m_eCommType = ECOMMTYPE_TCP_CLIENT;
				ui.pushButton_open->setText("连接");
			}
		});

	//设置发送文件状态
	ui.lineEdit_sendInterval->setValidator(new QIntValidator(0, 10000));
	ui.lineEdit_sendInterval->setText(QString::number(1000));
	connect(ui.checkBox_sendFile, &QCheckBox::stateChanged, this, [=](int iValue)
		{
			//选中格式
			if (iValue > 0)
			{
				QString strFilePath = QFileDialog::getOpenFileName(this, "选择文件", ".", "*.*");

				if (!strFilePath.isEmpty())
				{
					QFile file(strFilePath);
					if (file.open(QIODevice::ReadOnly))
					{
						CCommManager::GetInstance()->Send(m_eCommType, file.readAll(), ui.lineEdit_sendInterval->text().toInt());
						file.close();
					}
					else
					{
						QMessageBox::warning(this, QString("警告"), QString("选择文件不能打开! "));
					}
				}
				else
				{
					QMessageBox::warning(this, QString("警告"), QString("未选择文件! "));
				}
			}
		});


	//处理发送按钮
	connect(ui.pushButton_clear, &QPushButton::clicked, this, [=]()
		{
			ui.plainTextEdit_content->clear();
		});
}


QString CTcpWidget::GetCurrentDateTime()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz");
}
