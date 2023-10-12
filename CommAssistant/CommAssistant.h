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
    /**
     * @brief  :初始化界面UI
     *
     * @return :void
     */
    void InitUI();

private:
    Ui::CCommAssistantClass ui;
};
