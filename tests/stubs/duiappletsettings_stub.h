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

#ifndef DUIAPPLETSETTINGSSTUB_H
#define DUIAPPLETSETTINGSSTUB_H

#include <duiappletsettings.h>
#include <stubbase.h>
#include <duidataaccess.h>
#include <duiaggregatedataaccess.h>
#include <duigconfdatastore.h>
#include <duidatastore.h>
#include <duisettingslanguagebinary.h>
#include <duiappletid.h>


class DuiAppletSettingsStub : public StubBase
{
public:
    virtual void duiAppletSettingsConstructor(const QString &metaDataFileName, const DuiAppletId &appletId);
    virtual void duiAppletSettingsConstructor(const QString &metaDataFileName, const QString &appletId);
    virtual void duiAppletSettingsDestructor();
    virtual const DuiSettingsLanguageBinary *instanceSettingsBinary() const;
    virtual const DuiSettingsLanguageBinary *globalSettingsBinary() const;
    virtual bool hasSettings() const;
    virtual DuiDataStore *instanceDataStore() const;
    virtual DuiDataStore *globalDataStore() const;
    virtual DuiDataAccess *dataAccess() const;
    virtual void removeInstanceSettingValues() const;
};

void DuiAppletSettingsStub::duiAppletSettingsConstructor(const QString &metaDataFileName, const DuiAppletId &appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(metaDataFileName));
    params.append(new Parameter<DuiAppletId>(appletId));
    stubMethodEntered("duiAppletSettingsConstructor", params);
}

void DuiAppletSettingsStub::duiAppletSettingsConstructor(const QString &metaDataFileName, const QString &appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(metaDataFileName));
    params.append(new Parameter<QString>(appletId));
    stubMethodEntered("duiAppletSettingsConstructor", params);
}

void DuiAppletSettingsStub::duiAppletSettingsDestructor()
{
    stubMethodEntered("duiAppletSettingsDestructor");
}

const DuiSettingsLanguageBinary *DuiAppletSettingsStub::instanceSettingsBinary() const
{
    stubMethodEntered("instanceSettingsBinary");
    return stubReturnValue<const DuiSettingsLanguageBinary *>("instanceSettingsBinary");
}

const DuiSettingsLanguageBinary *DuiAppletSettingsStub::globalSettingsBinary() const
{
    stubMethodEntered("globalSettingsBinary");
    return stubReturnValue<const DuiSettingsLanguageBinary *>("globalSettingsBinary");
}

bool DuiAppletSettingsStub::hasSettings() const
{
    stubMethodEntered("hasSettings");
    return stubReturnValue<bool>("hasSettings");
}

DuiDataStore *DuiAppletSettingsStub::instanceDataStore() const
{
    stubMethodEntered("instanceDataStore");
    return stubReturnValue<DuiDataStore *>("instanceDataStore");
}

DuiDataStore *DuiAppletSettingsStub::globalDataStore() const
{
    stubMethodEntered("globalDataStore");
    return stubReturnValue<DuiDataStore *>("globalDataStore");
}

DuiDataAccess *DuiAppletSettingsStub::dataAccess() const
{
    stubMethodEntered("dataAccess");
    return stubReturnValue<DuiDataAccess *>("dataAccess");
}

void DuiAppletSettingsStub::removeInstanceSettingValues() const
{
    stubMethodEntered("removeInstanceSettingValues");
}


DuiAppletSettingsStub gDefaultDuiAppletSettingsStub;
DuiAppletSettingsStub *gDuiAppletSettingsStub = &gDefaultDuiAppletSettingsStub;

DuiAppletSettings::DuiAppletSettings(const QString &metaDataFileName, const DuiAppletId &appletId) :
    d_ptr(0)
{
    gDuiAppletSettingsStub->duiAppletSettingsConstructor(metaDataFileName, appletId);
}

DuiAppletSettings::DuiAppletSettings(const QString &metaDataFileName, const QString &appletId) :
    d_ptr(0)
{
    gDuiAppletSettingsStub->duiAppletSettingsConstructor(metaDataFileName, appletId);
}


DuiAppletSettings::~DuiAppletSettings()
{
    gDuiAppletSettingsStub->duiAppletSettingsDestructor();
}


const DuiSettingsLanguageBinary *
DuiAppletSettings::instanceSettingsBinary() const
{
    return gDuiAppletSettingsStub->instanceSettingsBinary();
}


const DuiSettingsLanguageBinary *
DuiAppletSettings::globalSettingsBinary() const
{
    return gDuiAppletSettingsStub->globalSettingsBinary();
}


bool
DuiAppletSettings::hasSettings() const
{
    return gDuiAppletSettingsStub->hasSettings();
}


DuiDataStore *
DuiAppletSettings::instanceDataStore() const
{
    return gDuiAppletSettingsStub->instanceDataStore();
}


DuiDataStore *
DuiAppletSettings::globalDataStore() const
{
    return gDuiAppletSettingsStub->globalDataStore();
}


DuiDataAccess *
DuiAppletSettings::dataAccess() const
{
    return gDuiAppletSettingsStub->dataAccess();
}


void
DuiAppletSettings::removeInstanceSettingValues() const
{
    gDuiAppletSettingsStub->removeInstanceSettingValues();
}

#endif
