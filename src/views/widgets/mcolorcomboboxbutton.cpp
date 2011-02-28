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

#include "mcolorcomboboxbutton.h"

#include "mlabel.h"
#include "mcolorwidget.h"

#include <QColor>
#include <QString>
#include <QGraphicsLinearLayout>

MColorComboBoxButton::MColorComboBoxButton(QGraphicsItem *parent)
    : MListItem(parent),
      titleLabel(0),
      colorWidget(0),
      layout(0)
{
    titleLabel = new MLabel(this);
    titleLabel->setTextElide(true);

    colorWidget = new MColorWidget(this);

    layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addItem(titleLabel);
    layout->addItem(colorWidget);
    layout->setAlignment(colorWidget, Qt::AlignVCenter);

    setLayout(layout);
}

void MColorComboBoxButton::setTitle(const QString &title)
{
    Q_ASSERT(titleLabel);
    titleLabel->setText(title);
}

void MColorComboBoxButton::setTitleStyleName(const QString &name)
{
    Q_ASSERT(titleLabel);
    titleLabel->setStyleName(name);
}

void MColorComboBoxButton::setColor(const QColor &color)
{
    Q_ASSERT(colorWidget);
    colorWidget->setColor(color);
}

void MColorComboBoxButton::setColorStyleName(const QString &name)
{
    Q_ASSERT(colorWidget);
    colorWidget->setStyleName(name);
}
