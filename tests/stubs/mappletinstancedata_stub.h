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

#ifndef MAPPLETINSTANCEDATA_STUB
#define MAPPLETINSTANCEDATA_STUB

#include "mappletinstancedata.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletInstanceDataStub : public StubBase
{
public:
    virtual void MAppletInstanceDataConstructor(MDataStore &baseDataStore, MAppletId id, const QString &desktopFile, const QString &instanceDataFilePath);
    virtual void MAppletInstanceDataDestructor();
    virtual MWidget *instantiateApplet();
    virtual MWidget *instantiateAppletPlaceHolder(const QString &packageName, const QMap<QString, QVariant> &metaData);
    virtual void storeData(MDataStore &store);
    virtual QString createKey(int type) const;
    virtual void setAppletHandleSizeHints(MAppletHandle &handle);
    virtual QSizeF qStringToQSizeF(const QString &string);
};

// 2. IMPLEMENT STUB
void MAppletInstanceDataStub::MAppletInstanceDataConstructor(MDataStore &baseDataStore, MAppletId id, const QString &desktopFile, const QString &instanceDataFilePath)
{
    Q_UNUSED(baseDataStore);
    Q_UNUSED(id);
    Q_UNUSED(desktopFile);
    Q_UNUSED(instanceDataFilePath);

}
void MAppletInstanceDataStub::MAppletInstanceDataDestructor()
{

}
MWidget *MAppletInstanceDataStub::instantiateApplet()
{
    stubMethodEntered("instantiateApplet");
    return stubReturnValue<MWidget *>("instantiateApplet");
}

MWidget *MAppletInstanceDataStub::instantiateAppletPlaceHolder(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    params.append(new Parameter<const QMap<QString, QVariant> & >(metaData));
    stubMethodEntered("instantiateAppletPlaceHolder", params);
    return stubReturnValue<MWidget *>("instantiateAppletPlaceHolder");
}

void MAppletInstanceDataStub::storeData(MDataStore &store)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MDataStore & >(store));
    stubMethodEntered("storeData", params);
}

QString MAppletInstanceDataStub::createKey(int type) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(type));
    stubMethodEntered("createKey", params);
    return stubReturnValue<QString>("createKey");
}

void MAppletInstanceDataStub::setAppletHandleSizeHints(MAppletHandle &handle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletHandle & >(handle));
    stubMethodEntered("setAppletHandleSizeHints", params);
}

QSizeF MAppletInstanceDataStub::qStringToQSizeF(const QString &string)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(string));
    stubMethodEntered("qStringToQSizeF", params);
    return stubReturnValue<QSizeF>("qStringToQSizeF");
}



// 3. CREATE A STUB INSTANCE
MAppletInstanceDataStub gDefaultMAppletInstanceDataStub;
MAppletInstanceDataStub *gMAppletInstanceDataStub = &gDefaultMAppletInstanceDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletInstanceData::MAppletInstanceData(MDataStore &baseDataStore, MAppletId id, const QString &desktopFile, const QString &instanceDataFilePath)
{
    gMAppletInstanceDataStub->MAppletInstanceDataConstructor(baseDataStore, id, desktopFile, instanceDataFilePath);
}

MAppletInstanceData::~MAppletInstanceData()
{
    gMAppletInstanceDataStub->MAppletInstanceDataDestructor();
}

MWidget *MAppletInstanceData::instantiateApplet()
{
    return gMAppletInstanceDataStub->instantiateApplet();
}

MWidget *MAppletInstanceData::instantiateAppletPlaceHolder(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    return gMAppletInstanceDataStub->instantiateAppletPlaceHolder(packageName, metaData);
}

void MAppletInstanceData::storeData(MDataStore &store)
{
    gMAppletInstanceDataStub->storeData(store);
}

QString MAppletInstanceData::createKey(KeyType type) const
{
    return gMAppletInstanceDataStub->createKey((int)type);
}

void MAppletInstanceData::setAppletHandleSizeHints(MAppletHandle &handle)
{
    gMAppletInstanceDataStub->setAppletHandleSizeHints(handle);
}

QSizeF MAppletInstanceData::qStringToQSizeF(const QString &string)
{
    return gMAppletInstanceDataStub->qStringToQSizeF(string);
}


#endif
