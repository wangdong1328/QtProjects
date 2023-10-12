#include "UdpWidget.h"
#include "Comm_Def.h"
#include "CommManager.h"
#include <QDateTime>

CUdpWidget::CUdpWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitUI();
}

CUdpWidget::~CUdpWidget()
{}

void CUdpWidget::InitUI()
{
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

			CCommManager::GetInstance()->Register(ECOMMTYPE_UDP, sEndPointSettings);
		});

	connect(ui.pushButton_close, &QPushButton::clicked, this, [=]()
		{
			ui.pushButton_close->setEnabled(false);
			ui.pushButton_send->setEnabled(false);
			ui.pushButton_open->setEnabled(true);

			CCommManager::GetInstance()->Unregister(ECOMMTYPE_UDP);
		});

	connect(ui.pushButton_send, &QPushButton::clicked, this, [=]()
		{
			QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz");

			if (CCommManager::GetInstance()->Send(ECOMMTYPE_UDP, ui.lineEdit_msg->text()) > 0)
			{
				ui.plainTextEdit_content->appendPlainText("[S" + strDateTime + "] " + 
					ui.lineEdit_msg->text());

				ui.lineEdit_msg->clear();
			}
			
		});

	//初始化禁用关闭服务器按钮
	ui.pushButton_close->setEnabled(false);
	ui.pushButton_send->setEnabled(false);

	//设置消息显示区只读
	ui.plainTextEdit_content->setReadOnly(true);

	//处理接收内容
	connect(CCommManager::GetInstance(), &CCommManager::RecvMsgSignal, this,
		[=](ECommType eCommType, const QString strMsg)
		{
			QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.zzz");

			if (ECOMMTYPE_UDP == eCommType)
			{
				ui.plainTextEdit_content->appendPlainText("[R" + strDateTime + "] " + strMsg);
			}
		});
}
