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
#include <QPaintEvent>
#include <QPainter>

#include <duiapplicationmenustyle.h>
#include <QDebug>

/* unforunately this is required to force widgets drawing it's background, even
   if there are widget attributes are set, that suppress the drawing!
*/
class DrawBackgroundEventFilter : public QObject {
protected:
    bool eventFilter(QObject* obj, QEvent* e) {
        if(QPaintEvent* pe = dynamic_cast<QPaintEvent*>(e)) {
            if(QWidget* w = qobject_cast<QWidget*>(obj)) {
                qCritical() << "EventFilter:" << w->objectName() << w->geometry() << pe->rect();

                Q_UNUSED(pe);
                QPainter p(w);
                p.setPen(Qt::NoPen);
                p.setBrush(w->palette().window());
                p.drawRect(pe->rect());
            }
        }
        return QObject::eventFilter(obj, e);
    };
};

QtMaemo6MenuProxy::QtMaemo6MenuProxy(QMenuBar *mb, QWidget *parent)
    : QtMaemo6Window(NULL, parent),
      m_menuBar(mb)
{
    //must be set, to activate the compositing
    setAttribute(Qt::WA_TranslucentBackground);
    QPalette palette;
    palette.setBrush(QPalette::Window, Qt::transparent);
    setPalette(palette);

    QWidget* appArea = new QWidget();
    appArea->setObjectName("appArea");
    appArea->installEventFilter(new DrawBackgroundEventFilter());

    appArea->setAttribute(Qt::WA_TranslucentBackground);
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 192)));
    appArea->setPalette(palette);

    QGridLayout* gridLayout = new QGridLayout(appArea);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);

    m_menu = new QtMaemo6Menu(mb, NULL);
    m_menu->setAttribute(Qt::WA_TranslucentBackground, false);
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 0)));
    m_menu->setPalette(palette);
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

    gridLayout->addItem(leftSpacer,        0, 0, 1, 1);
    gridLayout->addWidget(centralWidget(), 0, 1, 1, 1);
    gridLayout->addItem(rightSpacer,       0, 2, 1, 1);
    gridLayout->addItem(bottomSpacer,      1, 0, 1, 3);

    m_windowLayout->addItem(topSpacer, 0, 0, 1, 1);
    m_windowLayout->addWidget(appArea, 1, 0, 1, 1);
}

QtMaemo6MenuProxy::~QtMaemo6MenuProxy()
{
    //delete manually, because it is removed from the scroll area bevore deleting
    delete m_menu;
}

void QtMaemo6MenuProxy::mousePressEvent(QMouseEvent *event)
{
    close();
    QtMaemo6Window::mousePressEvent(event);
}
