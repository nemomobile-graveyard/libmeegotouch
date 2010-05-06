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

#ifndef UT_MAPPLETSETTINGSDIALOG_H
#define UT_MAPPLETSETTINGSDIALOG_H

#include <QObject>
#include <msettingslanguagebinary.h>

class MAppletSettingsDialog;
class MAppletSettings;

class Ut_MAppletSettingsDialog : public QObject
{
    Q_OBJECT

public:
    //! Settings language binary representing the instance settings
    static const MSettingsLanguageBinary instanceSettingsBinary;
    //! Settings language binary representing the global settings
    static const MSettingsLanguageBinary globalSettingsBinary;
    //! Specifies if the applet settings dialog is created
    static bool dialogCreated;
    //! A counter for how many instance settings widgets get created
    static int instanceSettingsWidgetsCount;
    //! A counter for how many global settings widgets get created
    static int globalSettingsWidgetsCount;
    //! Title of the container
    static QString containerTitle;


private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSettingsDialogCreation();
    void testSettingsContainerCreation();

private:
    MAppletSettings *appletSettings;
};

#endif
