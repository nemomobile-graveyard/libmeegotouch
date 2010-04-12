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

#include "calculatorwidget.h"

#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MSceneManager>

#include "calculatorbutton.h"

CalculatorWidget::CalculatorWidget()
{
    mValue = 0;

    //Prevent the layout from changing in right-to-left.
    //The calculation line will still be reversed however.
    setLayoutDirection(Qt::LeftToRight);

    /* Create a MLayout attached to this widget */
    MLayout *layout = new MLayout(this);

    MGridLayoutPolicy *landscapePolicy = new MGridLayoutPolicy(layout);
    MGridLayoutPolicy *portraitPolicy = new MGridLayoutPolicy(layout);
    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    landscapePolicy->setObjectName("calculatorLandscape");
    portraitPolicy->setObjectName("calculatorPortrait");

    mCalculationLine = new MLabel;
    mCalculationLine->setObjectName("calculationLine");
    mCalculationLine->setAlignment(Qt::AlignRight);

    QList<CalculatorButton *> buttons;
    CalculatorButton *button;
    for (int i = 0; i < 10; i++) {
        button = new CalculatorButton(CalculatorButton::Button((int)CalculatorButton::Num0 + i), this);
        if (i == 0) {
            landscapePolicy->addItem(button, 3, 4);
            portraitPolicy->addItem(button, 6, 0);
        } else {
            landscapePolicy->addItem(button, 2 - (i - 1) / 3, 4 + (i - 1) % 3);
            portraitPolicy->addItem(button, 5 - (i - 1) / 3, (i - 1) % 3);
        }
    }

    CalculatorButton *buttonC = new CalculatorButton(CalculatorButton::Clear, this);
    CalculatorButton *buttonBackspace = new CalculatorButton(CalculatorButton::Backspace, this);
    CalculatorButton *buttonSign = new CalculatorButton(CalculatorButton::Sign, this);
    CalculatorButton *buttonDecimal = new CalculatorButton(CalculatorButton::Decimal, this);
    CalculatorButton *buttonReciprocal = new CalculatorButton(CalculatorButton::Reciprocal, this);
    CalculatorButton *buttonPower = new CalculatorButton(CalculatorButton::Power, this);
    CalculatorButton *buttonSqrt = new CalculatorButton(CalculatorButton::Sqrt, this);
    CalculatorButton *buttonPercentage = new CalculatorButton(CalculatorButton::Percentage, this);
    CalculatorButton *buttonDivide = new CalculatorButton(CalculatorButton::Divide, this);
    CalculatorButton *buttonMultiply = new CalculatorButton(CalculatorButton::Multiply, this);
    CalculatorButton *buttonSubtract = new CalculatorButton(CalculatorButton::Subtract, this);
    CalculatorButton *buttonAdd = new CalculatorButton(CalculatorButton::Add, this);
    CalculatorButton *buttonEquals = new CalculatorButton(CalculatorButton::Equals, this);
    buttonEquals->setObjectName("calculatorButtonEquals");

    landscapePolicy->addItem(mCalculationLine, 0, 0, 1, 4);
    landscapePolicy->addItem(buttonC, 1, 0);
    landscapePolicy->addItem(buttonBackspace, 1, 1);
    landscapePolicy->addItem(buttonSign, 1, 2);
    landscapePolicy->addItem(buttonDecimal, 1, 3);
    landscapePolicy->addItem(buttonReciprocal, 2, 0);
    landscapePolicy->addItem(buttonPower, 2, 1);
    landscapePolicy->addItem(buttonSqrt, 2, 2);
    landscapePolicy->addItem(buttonPercentage, 2, 3);
    landscapePolicy->addItem(buttonDivide, 3, 0);
    landscapePolicy->addItem(buttonMultiply, 3, 1);
    landscapePolicy->addItem(buttonSubtract, 3, 2);
    landscapePolicy->addItem(buttonAdd, 3, 3);
    landscapePolicy->addItem(buttonEquals, 3, 5, 1, 2);

    portraitPolicy->addItem(mCalculationLine, 0, 0, 1, 4);
    portraitPolicy->addItem(buttonC, 1, 0);
    portraitPolicy->addItem(buttonBackspace, 1, 1);
    portraitPolicy->addItem(buttonReciprocal, 1, 2);
    portraitPolicy->addItem(buttonDivide, 1, 3);
    portraitPolicy->addItem(buttonPower, 2, 0);
    portraitPolicy->addItem(buttonSqrt, 2, 1);
    portraitPolicy->addItem(buttonPercentage, 2, 2);
    portraitPolicy->addItem(buttonMultiply, 2, 3);
    portraitPolicy->addItem(buttonSubtract, 3, 3);
    portraitPolicy->addItem(buttonAdd, 4, 3);
    portraitPolicy->addItem(buttonEquals, 5, 3, 2, 1);
    portraitPolicy->addItem(buttonDecimal, 6, 1);
    portraitPolicy->addItem(buttonSign, 6, 2);

    setValue(mValue);
}

void CalculatorWidget::setValue(qreal value)
{
    mValue = value;
    mCalculationLine->setText(QString::number(mValue));
}
qreal CalculatorWidget::value() const
{
    return mValue;
}
void CalculatorWidget::setText(const QString &text)
{
    mValue = text.toDouble();
    mCalculationLine->setText(text);
}
QString CalculatorWidget::text() const
{
    return mCalculationLine->text();
}
