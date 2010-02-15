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

#ifndef UT_DUIAPPLETINVENTORYVIEW_H
#define UT_DUIAPPLETINVENTORYVIEW_H

#include <QObject>
#include <QPointF>
#include <duiappletinventoryview.h>

class DuiApplication;
class DuiApplicationWindow;
class DuiAppletInventory;

class TestAppletInventoryView : public DuiAppletInventoryView
{
    DUI_VIEW(DuiAppletInventoryModel, DuiAppletInventoryStyle)

public:
    TestAppletInventoryView(DuiAppletInventory *controller);

    DuiAppletInventoryStyle *modifiableStyle() {
        DuiAppletInventoryStyleContainer &sc = style();
        const DuiAppletInventoryStyle *const_s = sc.operator ->();
        DuiAppletInventoryStyle *s = const_cast<DuiAppletInventoryStyle *>(const_s);
        return s;
    }
};

class TestSourceWidget : public DuiWidget
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

class Ut_DuiAppletInventoryView : public QObject
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

    //! Test that changing the widgets puts them into the layout
    void testModelModifiedWidgets();
    //! Test that changing the close button visibility shows and hides the close button
    void testModelModifiedCloseButtonVisible();
    //! Test that changing the installation sources puts them into the layout
    void testModelModifiedInstallationSources();

private:
    //! DuiApplication for the test case
    DuiApplication *app;
    //! DuiApplicationWindow for the test case
    DuiApplicationWindow *appWin;
    //! A controller for the view
    DuiAppletInventory *controller;
    //! The view to be tested
    TestAppletInventoryView *view;
};

#endif
