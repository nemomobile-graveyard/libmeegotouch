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

#ifndef MAPPLETSETTINGS_P_H
#define MAPPLETSETTINGS_P_H

#include <QString>

class MSettingsLanguageBinary;
class MGConfDataStore;
class MAppletId;
class MAggregateDataAccess;


class MAppletSettingsPrivate
{
public:
    MAppletSettingsPrivate();

    //! Applet instance settings file name
    QString instanceSettingsFileName;

    //! Applet global settings file name
    QString globalSettingsFileName;

    //! The prefix that is used for instance specific gconf settings
    QString instanceGConfPrefix;

    //! The prefix that is used for global gconf settings
    QString globalGConfPrefix;

    //! The instance settings binary representation
    mutable MSettingsLanguageBinary *instanceSettingsBinaryObject;
    //! A flag to tell if the instance settings binary has been created and is uptodate
    mutable bool instanceSettingsBinaryUptodate;

    //! The global settings binary representation
    mutable MSettingsLanguageBinary *globalSettingsBinaryObject;
    //! A flag to tell if the global settings binary has been created and is uptodate
    mutable bool globalSettingsBinaryUptodate;

    //! Aggregate datastore of the instance and global settings datastores
    mutable MAggregateDataAccess *settingsAggregate;

    //! Instance specific gconf datastore
    mutable MGConfDataStore *instanceGConfDataStore;
    //! Global gconf datastore
    mutable MGConfDataStore *globalGConfDataStore;
};

#endif // MAPPLETSETTINGS_P_H
