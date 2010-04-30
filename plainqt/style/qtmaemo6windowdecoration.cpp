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

#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6titlebar.h"
#include "qtmaemo6menuproxy.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QCloseEvent>
#include <MNavigationBar>
#include <QGraphicsLinearLayout>
#include <QStatusBar>
#include <QMenuBar>

QtMaemo6WindowDecoration::QtMaemo6WindowDecoration(QWidget *mw, QWidget *parent /*= NULL*/)
    : QtMaemo6Window(mw, parent),
      m_menuBar(0),
      m_statusBar(0),
      m_statusBarParent(0)
{
    m_titleBar = new QtMaemo6TitleBar(NULL);
    m_titleBar->setMenuButtonVisible(false);
    m_titleBar->setTitle(mw->windowTitle());

    m_windowLayout->addWidget(m_titleBar, 0, 1);
    m_windowLayout->addWidget(centralWidget(), 1, 1);

    connect(m_titleBar, SIGNAL(closeButtonClicked()), this, SLOT(close()));
    connect(m_titleBar, SIGNAL(minimizeButtonClicked()), this, SLOT(showMinimized()));
    connect(m_titleBar, SIGNAL(menuLabelClicked()), this, SLOT(showMenuBar()));
}

QtMaemo6WindowDecoration::~QtMaemo6WindowDecoration()
{
    //the statusBar must not be deleted by the window decoration on destruction
    if (m_statusBar) {
        m_windowLayout->removeWidget(m_statusBar);
        m_statusBar->setParent(m_statusBarParent);
    }
}

void QtMaemo6WindowDecoration::setStatusBar(QStatusBar *statusBar)
{
    if (!m_windowLayout)
        return;

    if (statusBar) {
        m_statusBarParent = statusBar->parentWidget();
        m_windowLayout->addWidget(statusBar, 2, 1);
    } else {
        if (m_statusBar) {
            m_windowLayout->removeWidget(m_statusBar);
            m_statusBar->setParent(m_statusBarParent);
            m_statusBarParent = NULL;
        }
    }

    m_statusBar = statusBar;
}

void QtMaemo6WindowDecoration::setMenuBar(QMenuBar *menuBar)
{
    if (menuBar) {
        m_menuBar = menuBar;
        m_titleBar->setMenuButtonVisible(m_menuBar->actions().count() > 0);
    } else {
        m_menuBar = NULL;
        m_titleBar->setMenuButtonVisible(false);
    }
}

void QtMaemo6WindowDecoration::showMenuBar()
{
    if (m_menuBar) {
        QtMaemo6MenuProxy* menuProxy = new QtMaemo6MenuProxy(m_menuBar, this);
        menuProxy->showFastMaximized();
    }
}

void QtMaemo6WindowDecoration::showNavigationBar( bool visible )
{
    m_titleBar->setVisible(visible);
}

bool QtMaemo6WindowDecoration::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::WindowTitleChange) {
        m_titleBar->setTitle(widget()->windowTitle());
    }

    return QtMaemo6Window::eventFilter(watched, event);
}

