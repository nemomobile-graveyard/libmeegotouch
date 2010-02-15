/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <DuiWidgetController>
#include <QList>

class DuiButton;
class DuiLabel;
class DuiGridLayoutPolicy;

class CalculatorWidget : public DuiWidget
{
public:
    CalculatorWidget();
    void setValue(qreal value);
    qreal value() const;
    void setText(const QString &text);
    QString text() const;

private:
    DuiLabel *mLabel;
    DuiLabel *mCalculationLine;

    qreal mValue;
};

#endif
