#include "UdpWidget.h"
#include "Comm_Def.h"
#include <QDateTime>
#include "CustomIntValidator.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include "UdpComm.h"

CUdpWidget::CUdpWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_pUdpComm = new CUdpComm(this);

	InitUI();
}

CUdpWidget::~CUdpWidget()
{
	if (m_pUdpComm)
	{
		delete m_pUdpComm;
		m_pUdpComm = nullptr;
	}
}

void CUdpWidget::InitUI()
{
	//设置输入格式
	ui.lineEdit_localPort->setValidator(new CCustomIntValidator(1, 65535, this));
	ui.lineEdit_peerPort->setValidator(new CCustomIntValidator(1, 65535, this));
	ui.lineEdit_localIp->setInputMask("000.000.000.000");
	ui.lineEdit_peerIp->setInputMask("000.000.000.000");

	//设置输入间隔
	ui.lineEdit_sendInterval->setValidator(new CCustomIntValidator(0, 10000, this));

	//打开按钮
	connect(ui.pushButton_open, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(true);
			ui.pushButton_send->setEnabled(true);
			ui.pushButton_open->setEnabled(false);

			SEndPointSettings sEndPointSettings;
			sEndPointSettings.sLocalEndPoint.strIPAddr = ui.lineEdit_localIp->text();
			sEndPointSettings.sLocalEndPoint.usPort = ui.lineEdit_localPort->text().toUShort();
			sEndPointSettings.sPeerEndPoint.strIPAddr = ui.lineEdit_peerIp->text();
			sEndPointSettings.sPeerEndPoint.usPort = ui.lineEdit_peerPort->text().toUShort();

			if (m_pUdpComm)
			{
				m_pUdpComm->BindEndPoint(sEndPointSettings);
			}
		});

	//处理关闭按钮
	connect(ui.pushButton_close, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(false);
			ui.pushButton_send->setEnabled(false);
			ui.pushButton_open->setEnabled(true);

			if (m_pUdpComm)
			{
				m_pUdpComm->UnBindEndPoint();
			}
		});

	//处理发送按钮
	connect(ui.pushButton_send, &QPushButton::clicked, this, [=]()
		{
			if (m_pUdpComm && (m_pUdpComm->Send(ui.lineEdit_msg->text().toUtf8()) > 0))
			{
				ui.plainTextEdit_content->appendPlainText("[S" + GetCurrentDateTime() + "] "
					+ ui.lineEdit_msg->text());

				ui.lineEdit_msg->clear();
			}

		});

	//处理发送按钮
	connect(ui.pushButton_clear, &QPushButton::clicked, this, [=]()
		{
			ui.plainTextEdit_content->clear();
		});

	//初始化禁用关闭服务器按钮
	ui.pushButton_close->setEnabled(false);
	ui.pushButton_send->setEnabled(false);

	//设置消息显示区只读
	ui.plainTextEdit_content->setReadOnly(true);

	connect(m_pUdpComm, &CUdpComm::RecvMsgSignal, this, [&](QString strMsg)
		{
			ui.plainTextEdit_content->appendPlainText("[R" + GetCurrentDateTime() + "] "
								+ strMsg);
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
					if (file.open(QIODevice::ReadOnly) && m_pUdpComm)
					{
						ui.pushButton_send->setEnabled(false);
						m_pUdpComm->Send(file.readAll(), ui.lineEdit_sendInterval->text().toInt());
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
}

QString CUdpWidget::GetCurrentDateTime()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz");
}
