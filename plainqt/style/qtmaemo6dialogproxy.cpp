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

#include "qtmaemo6dialogproxy.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QCloseEvent>
#include <QDialog>

#include "qtmaemo6dialogtitle.h"
#include "qtmaemo6style_p.h"

QtMaemo6DialogProxy::QtMaemo6DialogProxy(QWidget *mw, QWidget *parent)
    : QtMaemo6Window(mw, parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 192)));
    setPalette(palette);

    m_dialogTitle = new QtMaemo6DialogTitle(NULL);
    QtMaemo6StylePrivate::drawWindowBackground(m_dialogTitle);

    QSpacerItem *leftSideSpacer = new QSpacerItem(9, 0);
    QSpacerItem *rightSideSpacer = new QSpacerItem(9, 0);
    QSpacerItem *topSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_windowLayout->addItem(topSpacer, 0, 0, 1, 3);
    m_windowLayout->addItem(leftSideSpacer, 1, 0, 2, 1);
    m_windowLayout->addWidget(m_dialogTitle, 1, 1, 1, 1);
    m_windowLayout->addWidget(centralWidget(), 2, 1, 1, 1);
    m_windowLayout->addItem(rightSideSpacer, 1, 2, 2, 1);

    //only works if mw is a QDialog, otherwise the connect simply fails
    connect(m_dialogTitle, SIGNAL(closeRequest()), mw, SLOT(reject()));
}

QtMaemo6DialogProxy::~QtMaemo6DialogProxy()
{
}

void QtMaemo6DialogProxy::setTitle(const QString &text)
{
    m_dialogTitle->setTitle(text);
}

void QtMaemo6DialogProxy::setPixmap(const QPixmap &icon)
{
    m_dialogTitle->setPixmap(icon);
}
