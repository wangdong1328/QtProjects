#include "TcpWidget.h"
#include "CustomIntValidator.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include "TcpClientComm.h"
#include "TcpServerComm.h"

CTcpWidget::CTcpWidget(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);

	m_strCurClientEndpoint = "";
	//创建通信类指针
	m_pTcpClientComm = new CTcpClientComm(this);
	m_pTcpServerComm = new CTcpServerComm(this);
	m_pCurrentComm = m_pTcpClientComm;

	InitUI();
}

CTcpWidget::~CTcpWidget()
{
	if (m_pTcpClientComm)
	{
		delete m_pTcpClientComm;
		m_pTcpClientComm = nullptr;
	}

	if (m_pTcpServerComm)
	{
		delete m_pTcpServerComm;
		m_pTcpServerComm = nullptr;
	}

	m_pCurrentComm = nullptr;
}

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

	//设置不可见
	ui.checkBox_clients->setVisible(false);
	ui.comboBox_clients->setVisible(false);
	ui.label_clientList->setVisible(false);

	//打开按钮
	connect(ui.pushButton_open, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(true);
			ui.pushButton_send->setEnabled(true);
			ui.pushButton_open->setEnabled(false);

			SEndPointSettings sEndPointSettings;
			sEndPointSettings.sPeerEndPoint.strIPAddr = ui.lineEdit_peerIp->text();
			sEndPointSettings.sPeerEndPoint.usPort = ui.lineEdit_peerPort->text().toUShort();

			if (m_pCurrentComm)
			{
				m_pCurrentComm->BindEndPoint(sEndPointSettings);
			}
		});

	//处理关闭按钮
	connect(ui.pushButton_close, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(false);
			ui.pushButton_send->setEnabled(false);
			ui.pushButton_open->setEnabled(true);

			if (m_pCurrentComm)
			{
				m_pCurrentComm->UnBindEndPoint();
			}
		});

	//处理发送按钮
	connect(ui.pushButton_send, &QPushButton::clicked, this, [=]()
		{
			if (m_pCurrentComm == m_pTcpClientComm)
			{
				if (m_pCurrentComm->Send(ui.lineEdit_msg->text().toUtf8()) > 0)
				{
					ui.plainTextEdit_content->appendPlainText("[S" + GetCurrentDateTime() + "] "
						+ ui.lineEdit_msg->text());
				}
			}
			else if (m_pCurrentComm == m_pTcpServerComm)
			{
				if (m_pCurrentComm->Send(ui.lineEdit_msg->text().toUtf8(), m_strCurClientEndpoint) > 0)
				{
					ui.plainTextEdit_content->appendPlainText("[S" + GetCurrentDateTime() + "] "
						+ ui.lineEdit_msg->text());
				}
			}
			ui.lineEdit_msg->clear();
		});

	//处理选中服务器
	connect(ui.checkBox_server, &QCheckBox::stateChanged, this, [&](int iValue)
		{
			if (iValue > 0)
			{
				m_pCurrentComm = m_pTcpServerComm;
				ui.pushButton_open->setText("监听");

				ui.checkBox_clients->setVisible(true);
				ui.comboBox_clients->setVisible(true);
				ui.label_clientList->setVisible(true);
			}
			else
			{
				m_pCurrentComm = m_pTcpClientComm;
				ui.pushButton_open->setText("连接");

				ui.checkBox_clients->setVisible(false);
				ui.comboBox_clients->setVisible(false);
				ui.label_clientList->setVisible(false);
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
					if (file.open(QIODevice::ReadOnly) && m_pCurrentComm)
					{
						ui.pushButton_send->setEnabled(false);
						m_pCurrentComm->Send(file.readAll(), ui.lineEdit_sendInterval->text().toInt());
						ui.pushButton_send->setEnabled(true);
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

	//处理客户端消息接收
	connect(m_pTcpClientComm, &CTcpClientComm::RecvMsgSignal, this, [&](QString strMsg)
		{
			ui.plainTextEdit_content->appendPlainText("[R" + GetCurrentDateTime() + "] "
				+ strMsg);
		});

	//处理服务端消息接收
	connect(m_pTcpServerComm, &CTcpServerComm::RecvMsgSignal, this, [&](QString strMsg)
		{
			ui.plainTextEdit_content->appendPlainText("[R" + GetCurrentDateTime() + "] "
				+ strMsg);
		});

	//处理服务端消息接收
	connect(m_pTcpServerComm, &CTcpServerComm::ConnectMsgSignal, this, [&]()
		{
			if (m_pCurrentComm)
			{
				ui.comboBox_clients->clear();
				ui.comboBox_clients->addItems(m_pCurrentComm->GetClientConnectList());
			}

		});

	//处理服务端消息接收
	connect(m_pTcpServerComm, &CTcpServerComm::DisConnectMsgSignal, this, [&]()
		{
			if (m_pCurrentComm)
			{
				ui.comboBox_clients->clear();
				ui.comboBox_clients->addItems(m_pCurrentComm->GetClientConnectList());
			}
		});

	connect(ui.checkBox_clients, &QCheckBox::stateChanged, this, [=](int iValue)
		{
			if (iValue > 0)
			{
				m_strCurClientEndpoint = "";
			}
			else
			{
				m_strCurClientEndpoint = ui.comboBox_clients->currentText();
			}
		});


	connect(ui.comboBox_clients, &QComboBox::currentTextChanged, this, [=](QString strComBoxText)
		{
			if (!ui.checkBox_clients->isChecked())
			{
				m_strCurClientEndpoint = strComBoxText;
			}
		});
}


QString CTcpWidget::GetCurrentDateTime()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz");
}
