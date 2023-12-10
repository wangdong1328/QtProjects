#include "CommAssistant.h"
#include <QDateTime>
#include <QMessageBox>

CCommAssistant::CCommAssistant(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.action_aboutApp, &QAction::triggered, this, [&]()
		{
			QMessageBox::about(this, "软件信息", "软件提供UDP和TCP类型的通信，未开发完全，有需要请联系作者！");
		});

	connect(ui.action_aboutAuthor, &QAction::triggered, this, [&]()
		{
			QMessageBox::about(this, "作者信息", "开发者：wangdong,有其他技术问题请联系邮箱：  \
                wangdong1328@163.com 或者 qq:1941591018!");
		});
}

CCommAssistant::~CCommAssistant()
{
}
