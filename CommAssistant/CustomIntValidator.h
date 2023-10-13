#pragma once

#include <QIntValidator>

class CCustomIntValidator  : public QIntValidator
{
	Q_OBJECT

public:
	CCustomIntValidator(QObject* parent);
	CCustomIntValidator(int bottom, int top, QObject *parent);
	~CCustomIntValidator();

	/**
	 * @brief  :
	 *
	 * @param  :
	 * @param  :
	 * @return :QValidator::State
	 */
	QValidator::State validate(QString& strInput, int& iSize) const override;

	/**
	 * @brief  :
	 *
	 * @param  :input
	 * @return :void
	 */
	void fixup(QString& input) const override;
};
