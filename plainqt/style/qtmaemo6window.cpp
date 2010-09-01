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
#include <QPainter>

#include <mdeviceprofile.h>

#include "qtmaemo6dialogtitle.h"
#include "qtmaemo6style_p.h"

QtMaemo6Window::QtMaemo6Window(QWidget *originalWidget, QWidget *parent /* = NULL*/)
    : QWidget(parent)
    , m_window(originalWidget)
    , m_centralWidget(0)
    , m_scrollArea(0)
    , m_closeFromChild(false)
    , m_orientationAngle(M::Angle0)
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

    connect(style(), SIGNAL(orientationChanged(M::OrientationAngle)),
            this, SLOT(setOrientation(M::OrientationAngle)));

    if(centralWidget())
        centralWidget()->setAutoFillBackground(true);
}

QtMaemo6Window::~QtMaemo6Window()
{
}

void QtMaemo6Window::doLayoutOrientation()
{
    //clear layout
    QGridLayout* newLayout = new QGridLayout(this);
    newLayout->setMargin(0);
    newLayout->setSpacing(0);

    //iterate through the layout in reverse order
    for(int i = m_windowLayout->count()-1; i >= 0; --i) {
        int row, col, rowspan, colspan;
        m_windowLayout->getItemPosition(i, &row, &col, &rowspan, &colspan);
        QLayoutItem* item = m_windowLayout->takeAt(i);
        //insert item into new layout with reversed row and col and reversed spans
        newLayout->addItem(item, col, row, colspan, rowspan);
    }
    delete layout();

    setLayout(newLayout);
    m_windowLayout = newLayout;
    m_windowLayout->activate();
}

void QtMaemo6Window::setOrientation(M::OrientationAngle angle) {
    //in this case orientation changes betweend landscape and portrait.
    // We need to relayout then, otherwise flipping the layout orientation is enough
    if((m_orientationAngle - angle) % 180 != 0)
        doLayoutOrientation();

    Qt::Corner corner;
    switch(angle) {
        default:
        case M::Angle0: corner = Qt::TopLeftCorner; break;
        case M::Angle90:
            //if RTL Layout is set, reverse the whole thing in portrait view
            if(qApp->isRightToLeft())
                corner = Qt::TopLeftCorner;
            else
                corner = Qt::TopRightCorner;
            break;
        case M::Angle180: corner = Qt::BottomRightCorner; break;
        case M::Angle270:
            if(qApp->isRightToLeft())
                corner = Qt::BottomRightCorner;
            else
                corner = Qt::BottomLeftCorner;
            break;
    }
    m_windowLayout->setOriginCorner(corner);
    m_windowLayout->update();
    update();

    m_orientationAngle = angle;
    emit orientationChanged(m_orientationAngle);
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

void QtMaemo6Window::resizeEvent(QResizeEvent *e) {
    QWidget::resizeEvent(e);

    if(QtMaemo6Style* s = qobject_cast<QtMaemo6Style*>(centralWidget()))
        s->setPaletteBackground(this);
}

void QtMaemo6Window::paintEvent(QPaintEvent* e) {
    //by default a window with no content is transparent.
    // in derived classes you have to care for the content to draw its own
    // background
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(palette().window());
    p.drawRect(e->rect());
    QWidget::paintEvent(e);
}

void QtMaemo6Window::showFastMaximized()
{
    // Size policy instead?
    //set fixed size, to force size also when orientation changes!
    setFixedSize(MDeviceProfile::instance()->resolution());
    show();
}

void QtMaemo6Window::setCentralWidget(QWidget *widget)
{
    if (widget) {
        QWidget* focusWidget = widget->focusWidget();
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

        if(focusWidget)
            focusWidget->setFocus();
    }
}

QWidget* QtMaemo6Window::centralWidget() const {
    if(!m_scrollArea)
        return m_window;
    return m_scrollArea;
}
