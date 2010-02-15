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

#ifndef PT_DUIICONTESTCASE_H
#define PT_DUIICONTESTCASE_H

#include <QtTest/QtTest>
#include <QObject>

class DuiButton;
class DuiButtonView;
class DuiButtonIconView;
class DuiButtonObjectMenuItemView;
class DuiButtonSpinView;
class DuiButtonViewMenuItemView;
class DuiButtonViewMenuView;
class DuiWidgetView;

class Pt_DuiButton : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    // paint handler performance

    void paintPerformance();
    void paintPerformance_data();

private:
    DuiButton *m_subject;
    enum ViewName {
        View = 0,
        IconView,
        ObjectMenuItemView,
        SpinView,
        ViewMenuItemView,
        ViewMenuView,
        NoViews
    };
    DuiWidgetView *views[ NoViews ];

    qint32 currentViewIndex;
    QPixmap *pixmap;
    QPainter *painter;
    qint32 width;
    qint32 height;
    QList<QString> written;
};

#endif
