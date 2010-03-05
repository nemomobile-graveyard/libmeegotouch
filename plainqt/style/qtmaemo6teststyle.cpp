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

#include "qtmaemo6teststyle.h"
#include "qtmaemo6teststyle_p.h"
#include "qtmaemo6windowdecoration.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QFont>
#include <QDialog>

QtMaemo6TestStyleEventFilter::QtMaemo6TestStyleEventFilter(QtMaemo6TestStyle *parent)
    : QObject(parent)
{
    m_style = parent;
}

bool QtMaemo6TestStyleEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget *>(obj);
    if (widget) {
        QWidget *parent = qobject_cast<QWidget *>(widget->parent());

        switch (event->type()) {
        case QEvent::Show: {
            if (NULL != widget && (qobject_cast<QMainWindow *>(widget) || qobject_cast<QDialog *>(widget))) {
                //QMainWindow* main_window(qobject_cast<QMainWindow*>(widget));
                //if (NULL != main_window) {
                if (widget->windowFlags() & Qt::Window) {
                    if (!parent || !qobject_cast<QtMaemo6WindowDecoration *>(parent)) {
                        parent = m_style->m_windowDecoration = new QtMaemo6WindowDecoration(widget);
                    }
                    parent->showMaximized();
                }
            }
        }
        break;
        default:
            break;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

QtMaemo6TestStyle::QtMaemo6TestStyle(QtMaemo6TestStylePrivate &dd)
    : d_ptr(& dd), // this is a special case, since we start our shared d-pointer hierarchy within dui right here
      m_windowDecoration(NULL)
{
    Q_D(QtMaemo6TestStyle);
    if (d)
        d->q_ptr = this;
}

QtMaemo6TestStyle::QtMaemo6TestStyle()
    : QPlastiqueStyle(),
      d_ptr(new QtMaemo6TestStylePrivate()),
      m_windowEventFilter(0)
{
    Q_D(QtMaemo6TestStyle);
    d->q_ptr = this;
}


QtMaemo6TestStyle::~QtMaemo6TestStyle()
{
    delete d_ptr;
}

void QtMaemo6TestStyle::init()
{
    m_windowEventFilter = new QtMaemo6TestStyleEventFilter(this);
}

void QtMaemo6TestStyle::polish(QApplication *app)
{
    //FIXME: remove magic numbers!
    app->setGlobalStrut(QSize(48, 48));
    QFont font("Nokia Sans");
    font.setPointSize(25);
    app->setFont(font);
}

void QtMaemo6TestStyle::polish(QPalette &palette)
{
    Q_UNUSED(palette);
    /*
    QPixmap backgroundPixmap( ":/bg2.png" );

    //setTexture( palette, QPalette::Button, backgroundPixmap );
    palette.setBrush( QPalette::Background, Qt::black );
    setTexture( palette, QPalette::Mid, backgroundPixmap );
    setTexture( palette, QPalette::Window, backgroundPixmap );


    palette.setBrush( QPalette::Text, Qt::white );
    palette.setBrush( QPalette::ButtonText, Qt::white );
    palette.setBrush( QPalette::WindowText, Qt::white );

    QBrush half_transparent(QColor(0,0,0,128));

    palette.setBrush( QPalette::Button, half_transparent );
    palette.setBrush( QPalette::Base, half_transparent );
    */
}

void QtMaemo6TestStyle::polish(QWidget *widget)
{
    widget->installEventFilter(m_windowEventFilter);
}

void QtMaemo6TestStyle::setTexture(QPalette &palette, QPalette::ColorRole role,
                                   const QPixmap &pixmap)
{
    for (int i = 0; i < QPalette::NColorGroups; ++i) {
        QColor color = palette.brush(QPalette::ColorGroup(i), role).color();
        palette.setBrush(QPalette::ColorGroup(i), role, QBrush(color, pixmap));
    }
}
