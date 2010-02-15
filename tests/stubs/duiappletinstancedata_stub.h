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

#ifndef DUIAPPLETINSTANCEDATA_STUB
#define DUIAPPLETINSTANCEDATA_STUB

#include "duiappletinstancedata.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletInstanceDataStub : public StubBase
{
public:
    virtual void DuiAppletInstanceDataConstructor(DuiDataStore &baseDataStore, DuiAppletId id, const QString &desktopFile, const QString &instanceDataFilePath);
    virtual void DuiAppletInstanceDataDestructor();
    virtual DuiWidget *instantiateApplet();
    virtual DuiWidget *instantiateAppletPlaceHolder(const QString &packageName, const QMap<QString, QVariant> &metaData);
    virtual void storeData(DuiDataStore &store);
    virtual QString createKey(int type) const;
    virtual void setAppletHandleSizeHints(DuiAppletHandle &handle);
    virtual QSizeF qStringToQSizeF(const QString &string);
};

// 2. IMPLEMENT STUB
void DuiAppletInstanceDataStub::DuiAppletInstanceDataConstructor(DuiDataStore &baseDataStore, DuiAppletId id, const QString &desktopFile, const QString &instanceDataFilePath)
{
    Q_UNUSED(baseDataStore);
    Q_UNUSED(id);
    Q_UNUSED(desktopFile);
    Q_UNUSED(instanceDataFilePath);

}
void DuiAppletInstanceDataStub::DuiAppletInstanceDataDestructor()
{

}
DuiWidget *DuiAppletInstanceDataStub::instantiateApplet()
{
    stubMethodEntered("instantiateApplet");
    return stubReturnValue<DuiWidget *>("instantiateApplet");
}

DuiWidget *DuiAppletInstanceDataStub::instantiateAppletPlaceHolder(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    params.append(new Parameter<const QMap<QString, QVariant> & >(metaData));
    stubMethodEntered("instantiateAppletPlaceHolder", params);
    return stubReturnValue<DuiWidget *>("instantiateAppletPlaceHolder");
}

void DuiAppletInstanceDataStub::storeData(DuiDataStore &store)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiDataStore & >(store));
    stubMethodEntered("storeData", params);
}

QString DuiAppletInstanceDataStub::createKey(int type) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(type));
    stubMethodEntered("createKey", params);
    return stubReturnValue<QString>("createKey");
}

void DuiAppletInstanceDataStub::setAppletHandleSizeHints(DuiAppletHandle &handle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletHandle & >(handle));
    stubMethodEntered("setAppletHandleSizeHints", params);
}

QSizeF DuiAppletInstanceDataStub::qStringToQSizeF(const QString &string)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(string));
    stubMethodEntered("qStringToQSizeF", params);
    return stubReturnValue<QSizeF>("qStringToQSizeF");
}



// 3. CREATE A STUB INSTANCE
DuiAppletInstanceDataStub gDefaultDuiAppletInstanceDataStub;
DuiAppletInstanceDataStub *gDuiAppletInstanceDataStub = &gDefaultDuiAppletInstanceDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletInstanceData::DuiAppletInstanceData(DuiDataStore &baseDataStore, DuiAppletId id, const QString &desktopFile, const QString &instanceDataFilePath)
{
    gDuiAppletInstanceDataStub->DuiAppletInstanceDataConstructor(baseDataStore, id, desktopFile, instanceDataFilePath);
}

DuiAppletInstanceData::~DuiAppletInstanceData()
{
    gDuiAppletInstanceDataStub->DuiAppletInstanceDataDestructor();
}

DuiWidget *DuiAppletInstanceData::instantiateApplet()
{
    return gDuiAppletInstanceDataStub->instantiateApplet();
}

DuiWidget *DuiAppletInstanceData::instantiateAppletPlaceHolder(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    return gDuiAppletInstanceDataStub->instantiateAppletPlaceHolder(packageName, metaData);
}

void DuiAppletInstanceData::storeData(DuiDataStore &store)
{
    gDuiAppletInstanceDataStub->storeData(store);
}

QString DuiAppletInstanceData::createKey(KeyType type) const
{
    return gDuiAppletInstanceDataStub->createKey((int)type);
}

void DuiAppletInstanceData::setAppletHandleSizeHints(DuiAppletHandle &handle)
{
    gDuiAppletInstanceDataStub->setAppletHandleSizeHints(handle);
}

QSizeF DuiAppletInstanceData::qStringToQSizeF(const QString &string)
{
    return gDuiAppletInstanceDataStub->qStringToQSizeF(string);
}


#endif
