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

#include "qtmaemo6window.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QCloseEvent>
#include <QDialog>
#include <QDebug>
#include <QPainter>

#include <mdeviceprofile.h>

#include "qtmaemo6dialogtitle.h"
#include "qtmaemo6style_p.h"

QtMaemo6Window::QtMaemo6Window(QWidget *originalWidget, QWidget *parent /*= NULL*/)
    : QWidget(parent)
    , m_window(originalWidget)
    , m_centralWidget(0)
    , m_scrollArea(0)
    , m_closeFromChild(false)
{
    setWindowFlags(Qt::Window
                   | Qt::CustomizeWindowHint
                   | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);

    //FIXME: this sort of layouting is not nice
    //       spacers and widgets should be handled within this class
    m_windowLayout = new QGridLayout(this);
    m_windowLayout->setMargin(0);
    m_windowLayout->setSpacing(0);

    setCentralWidget(m_window);
}

QtMaemo6Window::~QtMaemo6Window()
{
}

QSize QtMaemo6Window::maxViewportSize() const
{
    return m_centralWidget->maximumViewportSize();
}

void QtMaemo6Window::closeEvent(QCloseEvent *event)
{
    //prevent deleting the original Widget by Qt
    QWidget* w = 0;
    if (m_scrollArea)
        w = m_scrollArea->takeWidget();
    if(!w) {
        w = m_window;
        layout()->removeWidget(w);
    }

    //this must be set back to dialog, so that the dialog can be shown again!
    w->setParent(0);
    w->setAttribute(Qt::WA_DeleteOnClose, false);
    w->setWindowFlags(m_originalFlags);
    QWidget::closeEvent(event);
}

bool QtMaemo6Window::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Close:
        if (!m_closeFromChild) {
            m_closeFromChild = true;

            // the decoration is closed, even if the widget is only hidden,
            // because the decoration is created again, when the widget is
            // shown
            this->close();
            return true;
        }
        break;
    case QEvent::Resize:
        if (!qobject_cast<QDialog*>(obj))
            break;
        // Fall through for all dialog cases.
        // Both Resize and Show are needed to cover all cases
    case QEvent::Show: {
        if (m_scrollArea && m_scrollArea->widget())
            m_scrollArea->widget()->setMinimumWidth(maxViewportSize().width());
    }
    break;
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void QtMaemo6Window::paintEvent(QPaintEvent* e) {
    //force drawing the background on windows
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(palette().window());
    p.drawRect(e->rect());
    QWidget::paintEvent(e);
}

void QtMaemo6Window::showFastMaximized()
{
    // Size policy instead?
    resize(MDeviceProfile::instance()->resolution());
    show();
}

void QtMaemo6Window::setCentralWidget(QWidget *widget)
{
    if (widget) {
        m_window = widget;
        m_originalFlags = m_window->windowFlags();
        m_window->setWindowFlags(Qt::Widget);

        m_window->installEventFilter(this);

        //remove the current central widget (and scrollArea if set)
        // but don't delete the central widget
        if (m_scrollArea) {
            m_windowLayout->removeWidget(m_scrollArea);
            m_scrollArea->takeWidget();
            delete m_scrollArea;
            m_scrollArea = NULL;
        } else {
            if (m_centralWidget)
                m_windowLayout->removeWidget(m_centralWidget);
        }
        m_centralWidget = NULL;

        if (qobject_cast<QAbstractScrollArea *>(widget))
            m_centralWidget = qobject_cast<QAbstractScrollArea *>(widget);
        else {
            m_centralWidget = m_scrollArea = new QScrollArea();
            m_scrollArea->setFrameShape(QFrame::NoFrame);
            m_scrollArea->setWidget(widget);
        }

        //If the widget has size policy expanding, then care for the widget to
        //use at least the scroll area viewport's size
        if(widget->sizePolicy().horizontalPolicy() == QSizePolicy::Expanding)
            widget->setMinimumWidth(maxViewportSize().width());
        if(widget->sizePolicy().verticalPolicy() == QSizePolicy::Expanding)
            widget->setMinimumHeight(maxViewportSize().height());
    }
}

QWidget* QtMaemo6Window::centralWidget() const {
    if(!m_scrollArea)
        return m_window;
    return m_scrollArea;
}
