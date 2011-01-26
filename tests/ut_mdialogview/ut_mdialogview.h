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

#ifndef UT_MDIALOGVIEW_H
#define UT_MDIALOGVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include <views/mdialogview.h>
#include <MDialog>
#include <MApplication>

//Q_DECLARE_METATYPE(MWidgetView*);

class Ut_MDialogView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void closeButtonVisibility();
    void titleBarVisibility();
    void spinnerVisibility();
    void buttonBoxCentering();
    void verticalAlignment();
    void buttonsFromModel();

private:
    QGraphicsWidget *fetchWidget(QGraphicsWidget &widget,
                                 const QString &objectName) const;
    QGraphicsWidget *fetchWidget(QGraphicsLayout &layout,
                                 const QString &objectName) const;

    MDialogView *subject;
    MDialog *controller;
    MDialogModel *model;
    MApplication *app;
};

#endif
