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

#include "qtmaemo6style.h"
#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6titlebar.h"
#include "qtmaemo6menuproxy.h"
#include "qtmaemo6style_p.h"

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
#include <MDebug>
#include <mdeviceprofile.h>

const int QtMaemo6WindowDecoration::layoutOffset = 2;


QtMaemo6WindowDecoration::QtMaemo6WindowDecoration(QWidget *mw, QWidget *parent /* = NULL*/)
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
    m_deviceStatusBar->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    updateStatusBarSharedPixmap();
    //FIXME: use XDamage to update the pixmap. As long as this don't work, update every 2 sec
    m_deviceStatusBarTimerId = startTimer(500);

    //build the layout, assuming orientation is Angle0
    m_windowLayout->addWidget(m_deviceStatusBar, layoutOffset-2, layoutOffset);
    m_windowLayout->addWidget(m_titleBar, layoutOffset-1, layoutOffset);
    m_windowLayout->addWidget(centralWidget(), layoutOffset, layoutOffset);


    connect(m_titleBar, SIGNAL(closeButtonClicked()), this, SLOT(close()));
    connect(m_titleBar, SIGNAL(minimizeButtonClicked()), this, SLOT(showMinimized()));
    connect(m_titleBar, SIGNAL(menuLabelClicked()), this, SLOT(showMenuBar()));
    connect(this, SIGNAL(orientationChanged(M::OrientationAngle)),
            m_titleBar, SLOT(setOrientation(M::OrientationAngle)));
    //also update the statusbar pixmap to get correct orientation
    connect(this, SIGNAL(orientationChanged(M::OrientationAngle)),
            this, SLOT(updateStatusBarSharedPixmap(M::OrientationAngle)));

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
void QtMaemo6WindowDecoration::updateStatusBarSharedPixmap(M::OrientationAngle angle)
{
    //destroyXDamageForSharedPixmap();

    Qt::HANDLE handle;
    QPixmap statusBarPixmap;
    if (fetchStatusBarSharedPixmapHandle(&handle)) {
        statusBarPixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);
        //m_deviceStatusBar->setPixmap(statusBarPixmap);
    }

    QPixmap modifiedPixmap;

    //set label size due to orientation
    if(angle == M::Angle0 || angle == M::Angle180) {
        m_deviceStatusBar->setFixedHeight(30);
        m_deviceStatusBar->setMinimumWidth(0);
        m_deviceStatusBar->setMaximumWidth(9999);

        modifiedPixmap = statusBarPixmap.copy(0, 0, statusBarPixmap.width(), 30);
        if(angle == M::Angle180) {
            QMatrix m;
            m.rotate(180);
            modifiedPixmap = modifiedPixmap.transformed(m);
            m_deviceStatusBar->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        } else
            m_deviceStatusBar->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    }
    else {
        m_deviceStatusBar->setFixedWidth(30);
        m_deviceStatusBar->setMinimumHeight(0);
        m_deviceStatusBar->setMaximumHeight(9999);
        QMatrix m;
        m.rotate(angle);
        modifiedPixmap = statusBarPixmap.copy(0, 30, width(), 30).transformed(m);
        if(angle == M::Angle90)
            m_deviceStatusBar->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
        else
            m_deviceStatusBar->setAlignment(Qt::AlignRight | Qt::AlignTop);
    }
    m_deviceStatusBar->setPixmap(modifiedPixmap);

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
        m_windowLayout->addWidget(statusBar, layoutOffset+1, layoutOffset);
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


void QtMaemo6WindowDecoration::setOrientation(M::OrientationAngle angle) {
    //currently this method does nothing but calling the base method.
    // reimplement an custom orientation change here
    QtMaemo6Window::setOrientation(angle);
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

void QtMaemo6WindowDecoration::showDeviceStatusBar( bool visible ) {
    m_deviceStatusBar->setVisible(visible);
}

void QtMaemo6WindowDecoration::ensureWidgetVisible(QWidget* widget, QRect visibleArea) {
    if(visibleArea.isValid()) {
        QAbstractScrollArea* sa = qobject_cast<QAbstractScrollArea*>(centralWidget());
        if(sa) {
            QWidget* viewport = sa->viewport();

            //that is the real visible area of the viewport, the navigation bar is excluded here
            QRect realVisibleRect = visibleArea.intersected(
                QRect(viewport->mapToGlobal(QPoint(0,0)), viewport->size() ));

            QRect globalWidgetRect = QRect(
                    widget->mapToGlobal(QPoint(0,0)),
                    widget->size()
                      );

            QPoint widgetGlobalPosition = widget->mapToGlobal(QPoint(0,0));

            //the widget is not fully covered by the visible Area
            if(globalWidgetRect.intersected(realVisibleRect) != globalWidgetRect) {
                QPoint originalViewportPos = viewport->mapToGlobal(QPoint(0,0));
                m_originalWidgetPos.widget = viewport;
                m_originalWidgetPos.position = viewport->pos();

                int newXPos = realVisibleRect.top() + ((realVisibleRect.height() - widget->height()) / 2);
                QPoint moveBy = QPoint(0, widgetGlobalPosition.y() - newXPos);

                //centered in visibleArea
                viewport->move(-moveBy);
            }
        } else {
//             mDebug("PlainQt Style") << "Can't focus on" << widget << "because scroll area contains no viewport";
        }
    } else {
        if(m_originalWidgetPos.widget) {
            m_originalWidgetPos.widget->move(m_originalWidgetPos.position);
            m_originalWidgetPos.widget = 0;
        }
    }
}

bool QtMaemo6WindowDecoration::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::WindowTitleChange) {
        m_titleBar->setTitle(widget()->windowTitle());
    }

    return QtMaemo6Window::eventFilter(watched, event);
}

void QtMaemo6WindowDecoration::resizeEvent(QResizeEvent * e) {
    QWidget::resizeEvent(e);
    if(QtMaemo6Style* s = qobject_cast<QtMaemo6Style*>(style()))
        s->setPaletteBackground(this);
}

void QtMaemo6WindowDecoration::timerEvent(QTimerEvent *e) {
    if(e->timerId() == m_deviceStatusBarTimerId)
        updateStatusBarSharedPixmap(orientation());
}
