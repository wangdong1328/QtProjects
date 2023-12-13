#include "SerialAssistant.h"
#include <QSerialPortInfo>
#include <QMessageBox>

SerialAssistant::SerialAssistant(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//创建串口对象
	m_pSerialport = new QSerialPort();

	Init();
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

	//初始化连接
	InitConnect();

	//获取可用串口
	for (auto& info : QSerialPortInfo::availablePorts())
	{
		ui.comboBox_serial->addItem(info.portName(), info.portName());
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
}

void SerialAssistant::InitConnect()
{
	// 打开关闭串口处理
	connect(ui.pushButton_open, &QPushButton::clicked, this, &SerialAssistant::OnPushButton_Open_Slot);

	// 发送按钮
	connect(ui.pushButton_send, &QPushButton::clicked, this, [&]()
		{
			if (m_pSerialport && m_pSerialport->isOpen())
			{
				// 获取要发送的内容
				QByteArray baContent = ui.plainTextEdit_send->toPlainText().toUtf8();

				// 处理发送头
				if (ui.radioButton_sendAscii->isChecked())
				{
					ui.plainTextEdit_history->appendPlainText(QString("[%1]# SEND ASCII>")
						.arg(GetDateTime()));

					m_pSerialport->write(baContent);
				}
				else
				{
					ui.plainTextEdit_history->appendPlainText(QString("[%1]# SEND HEX>")
						.arg(GetDateTime()));

					m_pSerialport->write(baContent.toHex(' '));
				}

				// 历史区添加内容
				ui.plainTextEdit_history->appendPlainText(baContent + "\n");
			}
			else
			{
				QMessageBox::warning(this, "警告", "发送失败，串口未连接或已经拔出!");
			}
		});

	// 接收消息
	connect(m_pSerialport, &QSerialPort::readyRead, this, [&]()
		{
			if (m_pSerialport)
			{
				if (ui.radioButton_recvAscii->isChecked())
				{
					//处理接收内容头
					ui.plainTextEdit_history->appendPlainText(QString("[%1]# RECV ASCII>")
						.arg(GetDateTime()));
					//添加内容
					ui.plainTextEdit_history->appendPlainText(m_pSerialport->readAll());
				}
				else
				{
					//处理接收内容头
					ui.plainTextEdit_history->appendPlainText(QString("[%1]# RECV HEX>")
						.arg(GetDateTime()));

					//添加内容
					ui.plainTextEdit_history->appendPlainText(QByteArray::fromHex(m_pSerialport->readAll()));
				}

				ui.plainTextEdit_history->appendPlainText("");
			}
		});
}

//打开关闭串口处理
void SerialAssistant::OnPushButton_Open_Slot()
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

		//关闭流控
		m_pSerialport->setFlowControl(QSerialPort::NoFlowControl);

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
