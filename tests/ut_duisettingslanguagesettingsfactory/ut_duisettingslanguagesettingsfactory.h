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

#ifndef UT_DUISETTINGSLANGUAGESETTINGSFACTORY_H
#define UT_DUISETTINGSLANGUAGESETTINGSFACTORY_H

#include <QObject>

class DuiApplication;
class DuiSettingsLanguageSelection;
class DuiSettingsLanguageText;
class DuiSettingsLanguageBoolean;
class DuiSettingsLanguageInteger;
class DuiWidgetController;
class DuiAction;
class DuiDataStore;

class Ut_DuiSettingsLanguageSettingsFactory : public QObject
{
    Q_OBJECT

public:
    static int indicatorWidgets;
    static int containerIndicators;
    static QList<QString> containers;
    static QList<const DuiSettingsLanguageSelection *> settingsSelections;
    static QList<const DuiSettingsLanguageText *> settingsTexts;
    static QList<const DuiSettingsLanguageBoolean *> settingsBooleans;
    static QList<const DuiSettingsLanguageInteger *> settingsIntegers;
    static QList<DuiDataStore *> settingsSelectionsDataStores;
    static QList<DuiDataStore *> settingsTextsDataStores;
    static QList<DuiDataStore *> settingsBooleansDataStores;
    static QList<DuiDataStore *> settingsIntegersDataStores;
    static DuiWidgetController *duiSettingsLanguageSelectionFactoryWidget;
    static QString duiActionProvider_getDefaultActionUri;
    static DuiAction *duiActionProvider_action;

private:
    DuiApplication *app;
    DuiDataStore *dataStore;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    void testChildrenCreation();
};

#endif
