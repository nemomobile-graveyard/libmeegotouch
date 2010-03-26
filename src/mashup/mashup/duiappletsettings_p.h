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

#ifndef DUIAPPLETSETTINGS_P_H
#define DUIAPPLETSETTINGS_P_H

#include <QString>

class DuiSettingsLanguageBinary;
class DuiGConfDataStore;
class DuiAppletId;
class DuiAggregateDataAccess;


class DuiAppletSettingsPrivate
{
public:
    DuiAppletSettingsPrivate();

    //! Applet instance settings file name
    QString instanceSettingsFileName;

    //! Applet global settings file name
    QString globalSettingsFileName;

    //! The prefix that is used for instance specific gconf settings
    QString instanceGConfPrefix;

    //! The prefix that is used for global gconf settings
    QString globalGConfPrefix;

    //! The instance settings binary representation
    mutable DuiSettingsLanguageBinary *instanceSettingsBinaryObject;
    //! A flag to tell if the instance settings binary has been created and is uptodate
    mutable bool instanceSettingsBinaryUptodate;

    //! The global settings binary representation
    mutable DuiSettingsLanguageBinary *globalSettingsBinaryObject;
    //! A flag to tell if the global settings binary has been created and is uptodate
    mutable bool globalSettingsBinaryUptodate;

    //! Aggregate datastore of the instance and global settings datastores
    mutable DuiAggregateDataAccess *settingsAggregate;

    //! Instance specific gconf datastore
    mutable DuiGConfDataStore *instanceGConfDataStore;
    //! Global gconf datastore
    mutable DuiGConfDataStore *globalGConfDataStore;
};

#endif // DUIAPPLETSETTINGS_P_H
