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

#ifndef UT_DUIDIALOGVIEW_H
#define UT_DUIDIALOGVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include <views/duidialogview.h>
#include <DuiDialog>
#include <DuiApplication>

//Q_DECLARE_METATYPE(DuiWidgetView*);

class Ut_DuiDialogView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void closeButtonVisibility();
    void titleBarVisibility();

private:
    QGraphicsWidget *fetchWidget(QGraphicsWidget &widget,
                                 const QString &objectName) const;
    QGraphicsWidget *fetchWidget(QGraphicsLayout &layout,
                                 const QString &objectName) const;

    DuiDialogView *subject;
    DuiDialog *controller;
    DuiDialogModel *model;
    DuiApplication *app;
};

#endif
