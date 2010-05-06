/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "calculatorbutton.h"

#include <math.h> //for sqrt
#include <QString>
#include "calculatorwidget.h"

CalculatorButton::CalculatorButton(Button _button, CalculatorWidget *_calculatorWidget) :
    MButton(), button(_button), calculatorWidget(_calculatorWidget)
{
    setObjectName("calculatorButton");
    connect(this, SIGNAL(clicked(bool)), SLOT(execute()));
    retranslate();
}
void CalculatorButton::retranslate()
{
    if (button >= Num0 && button <= Num9) {
        setText(QString::number(button));
        return;
    }
    switch (button) {
    case Clear:
        setText("C");
        return;
    case Backspace:
        setText("Del");
        return;
    case Sign:
        setText(QChar(0x00B1));
        return;
    case Decimal:
        setText(".");
        return;
    case Reciprocal:
        setText("1/x");
        return;
    case Power:
        setText("x^y");
        return;
    case Sqrt:
        setText(QChar(0x221a));
        return;
    case Percentage:
        setText("%");
        return;
    case Divide:
        setText(QChar(0x00F7));
        return;
    case Multiply:
        setText(QChar(0x00D7));
        return;
    case Subtract:
        setText("-");
        return;
    case Add:
        setText("+");
        return;
    case Equals:
        setText("=");
        return;
    default:
        return;
    }
}

void CalculatorButton::execute()
{
    if (button >= Num0 && button <= Num9) {
        if (calculatorWidget->text() == "0")
            calculatorWidget->setValue(button);
        else
            calculatorWidget->setText(calculatorWidget->text() + QString::number(button));
        return;
    }

    switch (button) {
    case Clear:
        calculatorWidget->setValue(0);
        return;
    case Backspace: {
        QString text = calculatorWidget->text();
        text.chop(1);
        calculatorWidget->setText(text);
        return;
    }
    case Sign:
        // TODO improve
        calculatorWidget->setValue(-calculatorWidget->value());
        return;
    case Decimal:
        //TODO check we don't already have a .
        calculatorWidget->setText(calculatorWidget->text() + '.');
        return;
    case Reciprocal:
        //TODO error check
        calculatorWidget->setValue(1 / calculatorWidget->value());
        return;
    case Power:
        //TODO implement
        return;
    case Sqrt:
        calculatorWidget->setValue(sqrt(calculatorWidget->value()));
        return;
    case Percentage:
        // TODO improve
        calculatorWidget->setValue(calculatorWidget->value() / 100);
        return;
    case Divide:
        // TODO implement
        return;
    case Multiply:
        // TODO implement
        return;
    case Subtract:
        // TODO implement
        return;
    case Add:
        // TODO implement
        return;
    case Equals:
        // TODO implement
        return;
    default:
        return;
    }
}

