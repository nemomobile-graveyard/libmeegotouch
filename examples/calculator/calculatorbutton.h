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

#ifndef CALCULATORBUTTON_H
#define CALCULATORBUTTON_H

#include <MButton>
class CalculatorWidget;

class CalculatorButton : public MButton
{
    Q_OBJECT
public:
    enum Button { Num0 = 0, Num9 = 9, Clear, Backspace, Sign, Decimal, Reciprocal, Power, Sqrt, Percentage, Divide, Multiply, Subtract, Add, Equals };

    CalculatorButton(Button _button, CalculatorWidget *_calculatorWidget);
    void retranslate();

public slots:
    void execute();

private:
    const Button button;
    CalculatorWidget *const calculatorWidget;
};

#endif
