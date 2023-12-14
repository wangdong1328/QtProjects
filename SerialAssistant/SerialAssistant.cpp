#include "SerialAssistant.h"
#include <QSerialPortInfo>
#include <QMessageBox>

SerialAssistant::SerialAssistant(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//设置窗口默认大小
	this->setFixedSize(800, 600);

	//创建串口对象
	m_pSerialport = new QSerialPort();

	//初始化
	Init();

	// 打开关闭串口处理
	connect(ui.pushButton_open, &QPushButton::clicked, this, &SerialAssistant::OnPushButtonOpenSlot);
	// 发送按钮
	connect(ui.pushButton_send, &QPushButton::clicked, this, &SerialAssistant::OnPushButtonSendSlot);
	// 接收消息
	connect(m_pSerialport, &QSerialPort::readyRead, this, &SerialAssistant::OnRecvMessageSlot);
	// 清理发送区
	connect(ui.pushButton_clearSend, &QPushButton::clicked, this, [&]()
		{
			ui.plainTextEdit_send->clear();
		});

	// 清理消息区
	connect(ui.pushButton_clearMsg, &QPushButton::clicked, this, [&]()
		{
			ui.plainTextEdit_history->clear();
		});

	connect(ui.pushButton_clearRecv, &QPushButton::clicked, this, [&]()
		{
			ui.plainTextEdit_history->clear();
		});
}

SerialAssistant::~SerialAssistant()
{
	//关闭串口
	m_pSerialport->close();
	if (m_pSerialport)
	{
		delete m_pSerialport;
		m_pSerialport = nullptr;
	}
}

void SerialAssistant::Init()
{
	//默认时ASCII接收
	ui.radioButton_recvAscii->setChecked(true);
	ui.radioButton_sendAscii->setChecked(true);

	//获取可用串口
	for (auto& info : QSerialPortInfo::availablePorts())
	{
		ui.comboBox_serial->addItem(info.portName() + ":" + info.description(), info.portName());
	}

	// 添加波特率
	for (auto& baud : QSerialPortInfo::standardBaudRates())
	{
		ui.comboBox_baud->addItem(QString::number(baud), baud);
	}
	//设置默认波特率
	ui.comboBox_baud->setCurrentText("115200");

	// 添加数据位
	ui.comboBox_data->addItem("5", QSerialPort::Data5);
	ui.comboBox_data->addItem("6", QSerialPort::Data6);
	ui.comboBox_data->addItem("7", QSerialPort::Data7);
	ui.comboBox_data->addItem("8", QSerialPort::Data8);
	//设置默认数据位
	ui.comboBox_data->setCurrentText("8");

	// 添加校验位
	ui.comboBox_check->addItem("NoParity", QSerialPort::NoParity);
	ui.comboBox_check->addItem("EvenParity", QSerialPort::EvenParity);
	ui.comboBox_check->addItem("OddParity", QSerialPort::OddParity);
	ui.comboBox_check->addItem("SpaceParity", QSerialPort::SpaceParity);
	ui.comboBox_check->addItem("MarkParity", QSerialPort::MarkParity);

	// 添加停止位
	ui.comboBox_stop->addItem("1", QSerialPort::OneStop);
	ui.comboBox_stop->addItem("1.5", QSerialPort::OneAndHalfStop);
	ui.comboBox_stop->addItem("2", QSerialPort::TwoStop);

	// 添加流控
	ui.comboBox_flow->addItem("NoFlow", QSerialPort::NoFlowControl);
	ui.comboBox_flow->addItem("Hardware", QSerialPort::HardwareControl);
	ui.comboBox_flow->addItem("Software", QSerialPort::SoftwareControl);
}

//发送消息按钮点击
void SerialAssistant::OnPushButtonSendSlot()
{
	if (m_pSerialport && m_pSerialport->isOpen())
	{
		// 获取要发送的内容
		QByteArray baContent = ui.plainTextEdit_send->toPlainText().toUtf8();

		if (baContent.isEmpty())
		{
			QMessageBox::warning(this, "警告", "没有发送内容! ");
		}
		else
		{
			// 处理发送头
			if (ui.radioButton_sendAscii->isChecked())
			{
				ui.plainTextEdit_history->appendPlainText(QString("[%1]# SEND ASCII>")
					.arg(GetDateTime()));
			}
			else
			{
				ui.plainTextEdit_history->appendPlainText(QString("[%1]# SEND HEX>")
					.arg(GetDateTime()));

				baContent = baContent.toHex(' ');
			}
			m_pSerialport->write(baContent);

			// 历史区添加内容
			ui.plainTextEdit_history->appendPlainText(baContent + "\n");
		}
	}
	else
	{
		QMessageBox::warning(this, "警告", "发送失败，串口未连接或已经拔出! ");
	}
}

//接收消息槽函数
void SerialAssistant::OnRecvMessageSlot()
{
	if (m_pSerialport)
	{
		QByteArray baContent = m_pSerialport->readAll();

		if (ui.radioButton_recvAscii->isChecked())
		{
			//处理接收内容头
			ui.plainTextEdit_history->appendPlainText(QString("[%1]# RECV ASCII>")
				.arg(GetDateTime()));

			if (ui.radioButton_sendHex->isChecked())
			{
				// 如果是以16进制
				baContent = QByteArray::fromHex(baContent);
			}
		}
		else
		{
			//处理接收内容头
			ui.plainTextEdit_history->appendPlainText(QString("[%1]# RECV HEX>")
				.arg(GetDateTime()));

			if (ui.radioButton_sendAscii->isChecked())
			{
				baContent = baContent.toHex(' ');
			}
		}

		//添加内容
		ui.plainTextEdit_history->appendPlainText(baContent + "\n");
	}
}

//打开关闭串口处理
void SerialAssistant::OnPushButtonOpenSlot()
{
	if (m_pSerialport)
	{
		if (QString("关闭串口") == ui.pushButton_open->text())
		{
			ui.pushButton_open->setText("打开串口");
			m_pSerialport->close();
		}
		else
		{
			//获取串口名
			auto portName = ui.comboBox_serial->currentData().toString();
			m_pSerialport->setPortName(portName);

			//获取波特率
			auto baud = ui.comboBox_baud->currentData().value<QSerialPort::BaudRate>();
			m_pSerialport->setBaudRate(115200);

			//获取数据位
			auto data = ui.comboBox_data->currentData().value<QSerialPort::DataBits>();
			m_pSerialport->setDataBits(data);

			//获取校验位
			auto check = ui.comboBox_check->currentData().value<QSerialPort::Parity>();
			m_pSerialport->setParity(check);

			//获取停止位
			auto stop = ui.comboBox_stop->currentData().value<QSerialPort::StopBits>();
			m_pSerialport->setStopBits(stop);

			//获取流控
			auto flow = ui.comboBox_flow->currentData().value<QSerialPort::FlowControl>();
			m_pSerialport->setFlowControl(flow);

			//打开串口
			if (m_pSerialport->open(QIODevice::ReadWrite))
			{
				ui.pushButton_open->setText("关闭串口");
			}
			else
			{
				QMessageBox::critical(this, QString("错误"), QString("端口不存在或被其他程序占用! "));
			}
		}
	}
}
