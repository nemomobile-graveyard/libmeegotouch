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

#include "qtmaemo6dialogtitle.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>


QtMaemo6DialogTitle::QtMaemo6DialogTitle(QWidget *parent) : QWidget(parent)
{
    setObjectName(QString("Qt_Maemo6_DialogTitle"));

    m_titleLabel = new QLabel(this);

    m_closeButton = new QtMaemo6ClickLabel(this);
    m_closeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    m_closeButton->setMargin(8);
    connect(m_closeButton, SIGNAL(clicked()), this, SIGNAL(closeRequest()));

    m_titleBarLayout = new QHBoxLayout(this);
    m_titleBarLayout->setMargin(0);
    m_titleBarLayout->setSpacing(0);
    m_titleBarLayout->addWidget(m_titleLabel);
    m_titleBarLayout->addWidget(m_closeButton);

}

QtMaemo6DialogTitle::~QtMaemo6DialogTitle()
{

}

void QtMaemo6DialogTitle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QStyleOption option;
    option.initFrom(this);

    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void QtMaemo6DialogTitle::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
    m_titleLabel->setAlignment(Qt::AlignHCenter);
}

QString QtMaemo6DialogTitle::title() const
{
    return m_titleLabel->text();
}

void QtMaemo6DialogTitle::setPixmap(const QPixmap &icon)
{
    m_closeButton->setPixmap(icon);
}
