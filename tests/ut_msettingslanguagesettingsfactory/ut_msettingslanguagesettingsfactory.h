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

#ifndef UT_MSETTINGSLANGUAGESETTINGSFACTORY_H
#define UT_MSETTINGSLANGUAGESETTINGSFACTORY_H

#include <QObject>

class MApplication;
class MSettingsLanguageSelection;
class MSettingsLanguageText;
class MSettingsLanguageBoolean;
class MSettingsLanguageInteger;
class MWidgetController;
class MAction;
class MDataStore;

class Ut_MSettingsLanguageSettingsFactory : public QObject
{
    Q_OBJECT

public:
    static int indicatorWidgets;
    static int containerIndicators;
    static QList<QString> containers;
    static QList<const MSettingsLanguageSelection *> settingsSelections;
    static QList<const MSettingsLanguageText *> settingsTexts;
    static QList<const MSettingsLanguageBoolean *> settingsBooleans;
    static QList<const MSettingsLanguageInteger *> settingsIntegers;
    static QList<MDataStore *> settingsSelectionsDataStores;
    static QList<MDataStore *> settingsTextsDataStores;
    static QList<MDataStore *> settingsBooleansDataStores;
    static QList<MDataStore *> settingsIntegersDataStores;
    static MWidgetController *mSettingsLanguageSelectionFactoryWidget;

private:
    MApplication *app;
    MDataStore *dataStore;

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
