#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CommAssistant.h"

class CCommAssistant : public QMainWindow
{
    Q_OBJECT

public:
    CCommAssistant(QWidget *parent = nullptr);
    ~CCommAssistant();

private:
    Ui::CCommAssistantClass ui;
};
