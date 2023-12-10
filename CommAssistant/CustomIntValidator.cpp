#include "CustomIntValidator.h"

CCustomIntValidator::CCustomIntValidator(QObject* parent)
	:QIntValidator(parent)
{
}

CCustomIntValidator::CCustomIntValidator(int bottom, int top, QObject* parent)
	:QIntValidator(bottom, top, parent)
{
}

CCustomIntValidator::~CCustomIntValidator()
{}

/**
 * @brief  :校验输入内容
 *
 * @param  :strInput,输入内容
 * @param  :iSize
 * @return :QValidator::State
 */
QValidator::State CCustomIntValidator::validate(QString& strInput, int& iSize) const
{
	if (strInput.isEmpty())
	{
		return QValidator::Intermediate;
	}
	//这里判断超过位数了，变成无效的参数
	int intValue = strInput.toInt();
	if (intValue > top() || intValue < bottom())
	{
		return QIntValidator::Invalid;
	}
	return QIntValidator::validate(strInput, iSize);
}

//当验证通不过时，通过调用 QValidator::fixedup()是这个函数修复错误。
void CCustomIntValidator::fixup(QString& input) const
{
	int intValue = input.toInt();
	if (intValue < bottom())
	{
		input = QString("%1").arg(bottom());
	}
	else if (intValue > top())
	{
		input = QString("%1").arg(top());
	}
}
