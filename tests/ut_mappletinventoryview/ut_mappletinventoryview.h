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

#ifndef UT_MAPPLETINVENTORYVIEW_H
#define UT_MAPPLETINVENTORYVIEW_H

#include <QObject>
#include <QPointF>
#include <mappletinventoryview.h>

class MApplication;
class MApplicationWindow;
class MAppletInventory;

class TestAppletInventoryView : public MAppletInventoryView
{
    M_VIEW(MAppletInventoryModel, MAppletInventoryStyle)

public:
    TestAppletInventoryView(MAppletInventory *controller);

    MAppletInventoryStyle *modifiableStyle() {
        MAppletInventoryStyleContainer &sc = style();
        const MAppletInventoryStyle *const_s = sc.operator ->();
        MAppletInventoryStyle *s = const_cast<MAppletInventoryStyle *>(const_s);
        return s;
    }
};

class TestSourceWidget : public MWidget
{
    Q_OBJECT
    Q_PROPERTY(QString installationSourceIcon READ installationSourceIcon)
    Q_PROPERTY(QString installationSourceTitle READ installationSourceTitle)
    Q_PROPERTY(QString installationSourceText READ installationSourceText)

public:
    QString installationSourceIcon()  {
        return installationSourceIcon_;
    };
    QString installationSourceTitle() {
        return installationSourceTitle_;
    };
    QString installationSourceText()  {
        return installationSourceText_;
    };

    void setInstallationSourceIcon(const QString &icon) {
        installationSourceIcon_ = icon;
        emit installationSourceIconChanged(icon);
    }
    void setInstallationSourceTitle(const QString &title) {
        installationSourceTitle_ = title;
        emit installationSourceTitleChanged(title);
    };
    void setInstallationSourceText(const QString &text) {
        installationSourceText_ = text;
        emit installationSourceTextChanged(text);
    };

    QString installationSourceIcon_;
    QString installationSourceTitle_;
    QString installationSourceText_;

signals:
    void installationSourceIconChanged(const QString &icon);
    void installationSourceTitleChanged(const QString &title);
    void installationSourceTextChanged(const QString &text);
};

class Ut_MAppletInventoryView : public QObject
{
    Q_OBJECT

public:
    static bool appearCalled;
    static bool disappearCalled;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    //! Test cases
    void testInitialization();
    void testModelModifiedWidgets();
    void testModelModifiedCloseButtonVisible();

private:
    //! MApplication for the test case
    MApplication *app;
    //! MApplicationWindow for the test case
    MApplicationWindow *appWin;
    //! A controller for the view
    MAppletInventory *controller;
    //! The view to be tested
    TestAppletInventoryView *view;
};

#endif
