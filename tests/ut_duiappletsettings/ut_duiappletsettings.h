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

#ifndef UT_DUIAPPLETSETTINGS_H
#define UT_DUIAPPLETSETTINGS_H

#include <QObject>
#include <QHash>
#include <duiappletid.h>
#include <QFileInfo>

class DuiAppletSettings;
class DuiSettingsLanguageBinary;

class Ut_DuiAppletSettings : public QObject
{
    Q_OBJECT

public:
    //! An applet id for as a parameter for the test subject
    static const DuiAppletId appletId;
    //! A settings binary object for instance settings
    static DuiSettingsLanguageBinary *instanceSettingsBinary;
    //! A settings binary object for global settings
    static DuiSettingsLanguageBinary *globalSettingsBinary;
    //! Specify the existence of applet instance settings file
    static bool instanceSettingsFileExists;
    //! Specify the existence of applet global settings file
    static bool globalSettingsFileExists;
    static QList<QFileInfo> readXmlFiles;
    //! The keys added to a DuiGConfDataStore object
    static QHash<QString, QString> gconfKeys;
    //! A list of keys unset by a DuiGConfDataStore object
    static QList<QString> gUnsetGConfKeys;

private:
    void initializeInstanceSettings();
    void initializeGlobalSettings();

    QString instanceKey(const QString &key);
    QString globalKey(const QString &key);

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testGettingEmptyInstanceSettingsBinary();
    void testGettingEmptyGlobalSettingsBinary();
    void testGettingInstanceSettingsBinary();
    void testGettingGlobalSettingsBinary();

    void testHasNoSettings();
    void testHasInstanceSettings();
    void testHasGlobalSettings();
    void testHasBothSettings();

    void testEmptyDataStore();
    void testInstanceSettingsDataStore();
    void testGlobalSettingsDataStore();
    void testBothSettingsDataStores();

    void testRemoveInstanceSettingValues();

private:
    DuiAppletSettings *m_subject;
};

#endif
