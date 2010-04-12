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

#ifndef MAPPLETSETTINGSSTUB_H
#define MAPPLETSETTINGSSTUB_H

#include <mappletsettings.h>
#include <stubbase.h>
#include <mdataaccess.h>
#include <maggregatedataaccess.h>
#include <mgconfdatastore.h>
#include <mdatastore.h>
#include <msettingslanguagebinary.h>
#include <mappletid.h>


class MAppletSettingsStub : public StubBase
{
public:
    virtual void mAppletSettingsConstructor(const QString &metaDataFileName, const MAppletId &appletId);
    virtual void mAppletSettingsConstructor(const QString &metaDataFileName, const QString &appletId);
    virtual void mAppletSettingsDestructor();
    virtual const MSettingsLanguageBinary *instanceSettingsBinary() const;
    virtual const MSettingsLanguageBinary *globalSettingsBinary() const;
    virtual bool hasSettings() const;
    virtual MDataStore *instanceDataStore() const;
    virtual MDataStore *globalDataStore() const;
    virtual MDataAccess *dataAccess() const;
    virtual void removeInstanceSettingValues() const;
};

void MAppletSettingsStub::mAppletSettingsConstructor(const QString &metaDataFileName, const MAppletId &appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(metaDataFileName));
    params.append(new Parameter<MAppletId>(appletId));
    stubMethodEntered("mAppletSettingsConstructor", params);
}

void MAppletSettingsStub::mAppletSettingsConstructor(const QString &metaDataFileName, const QString &appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(metaDataFileName));
    params.append(new Parameter<QString>(appletId));
    stubMethodEntered("mAppletSettingsConstructor", params);
}

void MAppletSettingsStub::mAppletSettingsDestructor()
{
    stubMethodEntered("mAppletSettingsDestructor");
}

const MSettingsLanguageBinary *MAppletSettingsStub::instanceSettingsBinary() const
{
    stubMethodEntered("instanceSettingsBinary");
    return stubReturnValue<const MSettingsLanguageBinary *>("instanceSettingsBinary");
}

const MSettingsLanguageBinary *MAppletSettingsStub::globalSettingsBinary() const
{
    stubMethodEntered("globalSettingsBinary");
    return stubReturnValue<const MSettingsLanguageBinary *>("globalSettingsBinary");
}

bool MAppletSettingsStub::hasSettings() const
{
    stubMethodEntered("hasSettings");
    return stubReturnValue<bool>("hasSettings");
}

MDataStore *MAppletSettingsStub::instanceDataStore() const
{
    stubMethodEntered("instanceDataStore");
    return stubReturnValue<MDataStore *>("instanceDataStore");
}

MDataStore *MAppletSettingsStub::globalDataStore() const
{
    stubMethodEntered("globalDataStore");
    return stubReturnValue<MDataStore *>("globalDataStore");
}

MDataAccess *MAppletSettingsStub::dataAccess() const
{
    stubMethodEntered("dataAccess");
    return stubReturnValue<MDataAccess *>("dataAccess");
}

void MAppletSettingsStub::removeInstanceSettingValues() const
{
    stubMethodEntered("removeInstanceSettingValues");
}


MAppletSettingsStub gDefaultMAppletSettingsStub;
MAppletSettingsStub *gMAppletSettingsStub = &gDefaultMAppletSettingsStub;

MAppletSettings::MAppletSettings(const QString &metaDataFileName, const MAppletId &appletId) :
    d_ptr(0)
{
    gMAppletSettingsStub->mAppletSettingsConstructor(metaDataFileName, appletId);
}

MAppletSettings::MAppletSettings(const QString &metaDataFileName, const QString &appletId) :
    d_ptr(0)
{
    gMAppletSettingsStub->mAppletSettingsConstructor(metaDataFileName, appletId);
}


MAppletSettings::~MAppletSettings()
{
    gMAppletSettingsStub->mAppletSettingsDestructor();
}


const MSettingsLanguageBinary *
MAppletSettings::instanceSettingsBinary() const
{
    return gMAppletSettingsStub->instanceSettingsBinary();
}


const MSettingsLanguageBinary *
MAppletSettings::globalSettingsBinary() const
{
    return gMAppletSettingsStub->globalSettingsBinary();
}


bool
MAppletSettings::hasSettings() const
{
    return gMAppletSettingsStub->hasSettings();
}


MDataStore *
MAppletSettings::instanceDataStore() const
{
    return gMAppletSettingsStub->instanceDataStore();
}


MDataStore *
MAppletSettings::globalDataStore() const
{
    return gMAppletSettingsStub->globalDataStore();
}


MDataAccess *
MAppletSettings::dataAccess() const
{
    return gMAppletSettingsStub->dataAccess();
}


void
MAppletSettings::removeInstanceSettingValues() const
{
    gMAppletSettingsStub->removeInstanceSettingValues();
}

#endif
