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
#include <QFile>
#include <QDir>

QtMaemo6WindowDecoration::QtMaemo6WindowDecoration(QWidget *mw, QWidget *parent /*= NULL*/)
    : QtMaemo6Window(mw, parent),
      m_menuBar(0),
      m_statusBar(0),
      m_statusBarParent(0),
      m_deviceStatusBarTimerId(-1)
{
    m_titleBar = new QtMaemo6TitleBar(NULL);
    m_titleBar->setMenuButtonVisible(false);
    m_titleBar->setTitle(mw->windowTitle());

    m_deviceStatusBar = new QLabel();
    //FIXME: set fixed height to 30, because haven't found a place where this is defined
    m_deviceStatusBar->setFixedHeight(30);
    m_deviceStatusBar->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    updateStatusBarSharedPixmap();
    //FIXME: use XDamage to update the pixmap. As long as this don't work, update every 2 sec
    m_deviceStatusBarTimerId = startTimer(2000);

    m_windowLayout->addWidget(m_deviceStatusBar, 0, 1);
    m_windowLayout->addWidget(m_titleBar,        1, 1);
    m_windowLayout->addWidget(centralWidget(),   2, 1);

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

#ifdef Q_WS_X11
void QtMaemo6WindowDecoration::updateStatusBarSharedPixmap()
{
    //destroyXDamageForSharedPixmap();

    Qt::HANDLE handle;
    if (fetchStatusBarSharedPixmapHandle(&handle)) {
        QPixmap statusBarPixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);
        m_deviceStatusBar->setPixmap(statusBarPixmap);
        statusBarPixmap.save("/tmp/statusbar.png");
    }

    /*
    if (!sharedPixmap.isNull()) {
        setupXDamageForSharedPixmap();
    }
    */
}

bool QtMaemo6WindowDecoration::fetchStatusBarSharedPixmapHandle(Qt::HANDLE *handle)
{
    QFile handleTempFile(QDir::temp().filePath("mstatusbar_pixmap_handle"));

    if (!handleTempFile.exists())
        return false;

    if (!handleTempFile.open(QIODevice::ReadOnly))
        return false;

    quint32 intHandle;
    QDataStream dataStream(&handleTempFile);
    dataStream >> intHandle;

    *handle = intHandle;

    return true;
}
#endif

void QtMaemo6WindowDecoration::setStatusBar(QStatusBar *statusBar)
{
    if (!m_windowLayout)
        return;

    if (statusBar) {
        m_statusBarParent = statusBar->parentWidget();
        m_windowLayout->addWidget(statusBar, 3, 1);
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

void QtMaemo6WindowDecoration::timerEvent(QTimerEvent *e) {
    if(e->timerId() == m_deviceStatusBarTimerId)
        updateStatusBarSharedPixmap();
}
