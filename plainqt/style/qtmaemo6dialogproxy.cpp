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

#include "qtmaemo6dialogproxy.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QCloseEvent>
#include <QDialog>
#include <QDebug>

#include "qtmaemo6dialogtitle.h"
#include "qtmaemo6style_p.h"

QtMaemo6DialogProxy::QtMaemo6DialogProxy(QWidget *mw, QWidget *parent)
    : QtMaemo6Window(mw, parent)
{
    setObjectName("Qt_Maemo6_DialogProxy");
    setAttribute(Qt::WA_TranslucentBackground);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QColor(0, 0, 0, 192)));
    setPalette(palette);

    //m_dialogTitle = new QtMaemo6DialogTitle(NULL);

    m_dialogWidget = new QWidget(NULL);
    m_dialogWidget->setObjectName("dialog_widget");
    m_dialogTitle = new QtMaemo6DialogTitle(m_dialogWidget);

    QVBoxLayout* dialogLayout = new QVBoxLayout(m_dialogWidget);
    dialogLayout->setSpacing(0);
    dialogLayout->setMargin(0);
    dialogLayout->addWidget(m_dialogTitle);
    dialogLayout->addWidget(centralWidget());

    QSpacerItem *leftSideSpacer = new QSpacerItem(9, 0);
    QSpacerItem *rightSideSpacer = new QSpacerItem(9, 0);
    QSpacerItem *topSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_windowLayout->addItem(topSpacer,         0, 0, 1, 3);
    m_windowLayout->addItem(leftSideSpacer,    1, 0, 2, 1);
    m_windowLayout->addWidget(m_dialogWidget,  1, 1, 1, 1);
    //m_windowLayout->addWidget(m_dialogTitle,  1, 1, 1, 1);
    //m_windowLayout->addWidget(centralWidget(), 2, 1, 1, 1);
    m_windowLayout->addItem(rightSideSpacer,   1, 2, 2, 1);

    //only works if mw is a QDialog, otherwise the connect simply fails
    connect(m_dialogTitle, SIGNAL(closeRequest()), mw, SLOT(reject()));

    mw->installEventFilter(this);
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

void QtMaemo6DialogProxy::ensureWidgetVisible(QWidget* widget, QRect visibleArea) {
    if(visibleArea.isValid()) {
        //that is the real visible area of the viewport, the navigation bar is excluded here
        QRect realVisibleRect = visibleArea.intersected(
            QRect(m_dialogWidget->mapToGlobal(QPoint(0,0)), m_dialogWidget->size() ));

        QRect globalWidgetRect = QRect(
                widget->mapToGlobal(QPoint(0,0)),
                widget->size()
                  );

        QPoint widgetGlobalPosition = widget->mapToGlobal(QPoint(0,0));

        //the widget is not fully covered by the visible Area
        if(globalWidgetRect.intersected(realVisibleRect) != globalWidgetRect) {
            QPoint originalViewportPos = m_dialogWidget->mapToGlobal(QPoint(0,0));
            m_originalWidgetPos.widget = m_dialogWidget;
            m_originalWidgetPos.position = m_dialogWidget->pos();

            int newYPos = realVisibleRect.top() + ((realVisibleRect.height() - widget->height()) / 2);

            //centered in visibleArea
            m_dialogWidget->move(m_dialogWidget->pos().x(), newYPos);
        }
    } else {
        if(m_originalWidgetPos.widget) {
            m_originalWidgetPos.widget->move(m_originalWidgetPos.position);
            m_originalWidgetPos.widget = 0;
        }
    }
}

void QtMaemo6DialogProxy::resizeEvent(QResizeEvent *) {
    if(centralWidget()) {
        if(QtMaemo6Style* s = qobject_cast<QtMaemo6Style*>(centralWidget()->style()))
            s->setPaletteBackground(centralWidget());
    }
}

void QtMaemo6DialogProxy::mousePressEvent(QMouseEvent *event)
{
    //close dialog if some click occurs outside the dialogs window
    if(!m_dialogWidget->geometry().contains(event->pos()))
        close();
}


bool QtMaemo6DialogProxy::eventFilter(QObject *obj, QEvent *event) {
    //in dialog case, also close the decoration on hide event, because the
    //dialogs are only hidden, not closed by default
    if(obj == widget() && event->type() == QEvent::Hide) {
        close();
    }

    if(event->type() == QEvent::Resize) {
        //don't show dialog bigger than needed
        // it's a little tricky because everything is inside a scrollarea
        if(QScrollArea* scrollArea = qobject_cast<QScrollArea*>(centralWidget())) {
            int left, top, right, bottom;
            QWidget* w = scrollArea->widget();
            w->getContentsMargins(&left, &top, &right, & bottom);
            int scrollAreaContentHeight = w->height() + top + bottom;
            scrollArea->setMaximumHeight(scrollAreaContentHeight);
        }
    }
    return QtMaemo6Window::eventFilter(obj, event);
}
