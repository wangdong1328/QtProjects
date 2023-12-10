#pragma once
/**
 * @file    CustomIntValidator.h
 * @author  wangdong (wangdong1328@163.com)
 * @brief   重写整数验证类，目的是原整数验证器输入的内容不在范围内时，不会立刻返回
 * @version 0.1
 * @date    2023-12-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <QIntValidator>

class CCustomIntValidator : public QIntValidator
{
	Q_OBJECT

public:
	CCustomIntValidator(QObject* parent);
	CCustomIntValidator(int bottom, int top, QObject* parent);
	~CCustomIntValidator();

	/**
	 * @brief  : 验证输入的内容是否在上下范围内容
	 *
	 * @param  :
	 * @param  :
	 * @return :QValidator::State
	 */
	QValidator::State validate(QString& strInput, int& iSize) const override;

	/**
	 * @brief  : 当输入的内容不在范围内时，应修复，显示上限或下限
	 *
	 * @param  :input
	 * @return :void
	 */
	void fixup(QString& input) const override;
};
