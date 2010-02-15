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

#include "qtmaemo6menuproxy.h"
#include "qtmaemo6menu.h"
#include "qtmaemo6style_p.h"

#include <QGridLayout>
#include <QWidget>
#include <QEvent>
#include <QCloseEvent>
#include <QStyleOption>
#include <QPushButton>

#include <duiapplicationmenustyle.h>

QtMaemo6MenuProxy::QtMaemo6MenuProxy(QMenuBar *mb, QWidget *parent)
    : QtMaemo6Window(NULL, parent),
      m_menuBar(mb)
{
    setAttribute(Qt::WA_TranslucentBackground);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 192)));
    setPalette(palette);

    m_menu = new QtMaemo6Menu(mb, NULL);
    setCentralWidget(m_menu);

    QStyleOption option;
    option.initFrom(mb);

    const DuiApplicationMenuStyle *style =
        static_cast<const DuiApplicationMenuStyle *>(
            QtMaemo6StylePrivate::duiStyle(option.state, "DuiApplicationMenuStyle"));

    QSpacerItem *topSpacer = new QSpacerItem(1, style->paddingTop());
    QSpacerItem *rightSpacer = new QSpacerItem(style->paddingRight(), 1);
    QSpacerItem *bottomSpacer = new QSpacerItem(1, style->paddingBottom());
    QSpacerItem *leftSpacer = new QSpacerItem(style->paddingLeft(), 1);

    m_windowLayout->addItem(topSpacer, 0, 0, 1, 3);
    m_windowLayout->addItem(leftSpacer, 1, 0, 1, 1);
    m_windowLayout->addItem(rightSpacer, 1, 2, 1, 1);
    m_windowLayout->addItem(bottomSpacer, 2, 0, 1, 3);
}

QtMaemo6MenuProxy::~QtMaemo6MenuProxy()
{
    //delete manually, because it is removed from the scroll area bevore deleting
    delete m_menu;
}

void QtMaemo6MenuProxy::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    close();
}
