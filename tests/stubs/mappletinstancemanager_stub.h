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

#ifndef MAPPLETINSTANCEMANAGER_STUB
#define MAPPLETINSTANCEMANAGER_STUB

#include "mappletinstancemanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletInstanceManagerStub : public StubBase
{
public:
    virtual void MAppletInstanceManagerConstructor(const QString &identifier, MDataStore *dataStore);
    virtual void MAppletInstanceManagerDestructor();
    virtual bool restoreApplets();
    virtual bool instantiateApplet(const QString &metadataFile);
    virtual void instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData);
    virtual void removeActionTriggered(bool checked);
    virtual void appletUninstalled(const QString &desktopFile);
    virtual bool removeApplet(MAppletId appletId);
    virtual void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    virtual void setAppletTitle(const QString &title);
    virtual void receiveOperation(QDBusPendingCallWatcher *watcher);
    virtual void updateApplet(const MDesktopEntry &appletData);
    virtual void init(const QString &mashupCanvasName, MDataStore *dataStore);
    virtual bool instantiateApplet(MAppletId appletId);
    virtual bool instantiateOutOfProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata);
    virtual bool instantiateInProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata);
    virtual void instantiateAppletPlaceHolder(MAppletInstanceData *data);
    virtual void queryInstallationStatus(MAppletInstanceData *data);
    virtual void createDataStore();
    virtual void readAppletData();
    virtual MAppletInstanceData *createAppletInstanceDataFromInstantiationData(const MAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData);
    virtual MAppletInstanceData *createAppletInstanceDataFromPackageMetaData(const MAppletId &appletId, const QMap<QString, QVariant> &packageMetaData);
    virtual void setAppletHandleSizeHints(MAppletHandle &handle, MAppletInstanceData &data);
    virtual QSizeF qStringToQSizeF(const QString &string);
    virtual QString dataPath() const;
    virtual QString createAppletInstanceDataFileName(MAppletId id) const;
    virtual void freeAppletInstanceID(MAppletId id);
    virtual MAppletId appletIDForWidget(QGraphicsWidget *widget) const;
    virtual MAppletId appletIDForPackageName(const QString &packageName) const;
    virtual MAppletId::AppletInstanceID appletInstanceIDFromKey(const QString &key);
    virtual void removeAppletInstanceData(MAppletId appletId);
    virtual MAppletId getUnusedAppletID();
    virtual bool isValidKey(const QString &key);
    virtual QString getParameterName(const QString &key);
    virtual QString createKey(const MAppletId &appletId, int type);
    virtual void storeData(MAppletInstanceData *data);
    virtual MAppletId createAppletId(MAppletId::AppletInstanceID instanceId) const;
};

// 2. IMPLEMENT STUB
void MAppletInstanceManagerStub::MAppletInstanceManagerConstructor(const QString &identifier, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(identifier));
    params.append(new Parameter<MDataStore *>(dataStore));
    stubMethodEntered("constructor", params);
}
void MAppletInstanceManagerStub::MAppletInstanceManagerDestructor()
{

}
bool MAppletInstanceManagerStub::restoreApplets()
{
    stubMethodEntered("restoreApplets");
    return stubReturnValue<bool>("restoreApplets");
}

bool MAppletInstanceManagerStub::instantiateApplet(const QString &metadataFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(metadataFile));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<bool>("instantiateApplet");
}

void MAppletInstanceManagerStub::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    params.append(new Parameter<const QMap<QString, QVariant> & >(metaData));
    stubMethodEntered("instantiateAppletFromPackage", params);
}

void MAppletInstanceManagerStub::removeActionTriggered(bool checked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(checked));
    stubMethodEntered("removeActionTriggered", params);
}

void MAppletInstanceManagerStub::appletUninstalled(const QString &desktopFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(desktopFile));
    stubMethodEntered("appletUninstalled", params);
}

bool MAppletInstanceManagerStub::removeApplet(MAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId >(appletId));
    stubMethodEntered("removeApplet", params);
    return stubReturnValue<bool>("removeApplet");
}

void MAppletInstanceManagerStub::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(operation));
    params.append(new Parameter<const QString & >(pkg));
    params.append(new Parameter<const QString & >(error));
    stubMethodEntered("operationComplete", params);
}

void MAppletInstanceManagerStub::setAppletTitle(const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(title));
    stubMethodEntered("setAppletTitle", params);
}

void MAppletInstanceManagerStub::receiveOperation(QDBusPendingCallWatcher *watcher)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QDBusPendingCallWatcher * >(watcher));
    stubMethodEntered("receiveOperation", params);
}

void MAppletInstanceManagerStub::updateApplet(const MDesktopEntry &appletData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MDesktopEntry& >(appletData));
    stubMethodEntered("updateApplet", params);
}

void MAppletInstanceManagerStub::init(const QString &mashupCanvasName, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(mashupCanvasName));
    params.append(new Parameter<MDataStore * >(dataStore));
    stubMethodEntered("init", params);
}

bool MAppletInstanceManagerStub::instantiateApplet(MAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId >(appletId));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<bool>("instantiateApplet");
}

bool MAppletInstanceManagerStub::instantiateOutOfProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletInstanceData * >(data));
    params.append(new Parameter<const MAppletMetaData & >(metadata));
    stubMethodEntered("instantiateOutOfProcessApplet", params);
    return stubReturnValue<bool>("instantiateOutOfProcessApplet");
}

bool MAppletInstanceManagerStub::instantiateInProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletInstanceData * >(data));
    params.append(new Parameter<const MAppletMetaData & >(metadata));
    stubMethodEntered("instantiateInProcessApplet", params);
    return stubReturnValue<bool>("instantiateInProcessApplet");
}

void MAppletInstanceManagerStub::instantiateAppletPlaceHolder(MAppletInstanceData *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletInstanceData * >(data));
    stubMethodEntered("instantiateAppletPlaceHolder", params);
}

void MAppletInstanceManagerStub::queryInstallationStatus(MAppletInstanceData *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletInstanceData * >(data));
    stubMethodEntered("queryInstallationStatus", params);
}

void MAppletInstanceManagerStub::createDataStore()
{
    stubMethodEntered("createDataStore");
}

void MAppletInstanceManagerStub::readAppletData()
{
    stubMethodEntered("readAppletData");
}

MAppletInstanceData *MAppletInstanceManagerStub::createAppletInstanceDataFromInstantiationData(const MAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MAppletId & >(appletId));
    params.append(new Parameter<const QMap<QString, QVariant> & >(appletInstantiationData));
    stubMethodEntered("createAppletInstanceDataFromInstantiationData", params);
    return stubReturnValue<MAppletInstanceData *>("createAppletInstanceDataFromInstantiationData");
}

MAppletInstanceData *MAppletInstanceManagerStub::createAppletInstanceDataFromPackageMetaData(const MAppletId &appletId, const QMap<QString, QVariant> &packageMetaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MAppletId & >(appletId));
    params.append(new Parameter<const QMap<QString, QVariant> & >(packageMetaData));
    stubMethodEntered("createAppletInstanceDataFromPackageMetaData", params);
    return stubReturnValue<MAppletInstanceData *>("createAppletInstanceDataFromPackageMetaData");
}

void MAppletInstanceManagerStub::setAppletHandleSizeHints(MAppletHandle &handle, MAppletInstanceData &data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletHandle & >(handle));
    params.append(new Parameter<MAppletInstanceData & >(data));
    stubMethodEntered("setAppletHandleSizeHints", params);
}

QSizeF MAppletInstanceManagerStub::qStringToQSizeF(const QString &string)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(string));
    stubMethodEntered("qStringToQSizeF", params);
    return stubReturnValue<QSizeF>("qStringToQSizeF");
}

QString MAppletInstanceManagerStub::dataPath() const
{
    stubMethodEntered("dataPath");
    return stubReturnValue<QString>("dataPath");
}

QString MAppletInstanceManagerStub::createAppletInstanceDataFileName(MAppletId id) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId >(id));
    stubMethodEntered("createAppletInstanceDataFileName", params);
    return stubReturnValue<QString>("createAppletInstanceDataFileName");
}

void MAppletInstanceManagerStub::freeAppletInstanceID(MAppletId id)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId >(id));
    stubMethodEntered("freeAppletInstanceID", params);
}

MAppletId MAppletInstanceManagerStub::appletIDForWidget(QGraphicsWidget *widget) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsWidget * >(widget));
    stubMethodEntered("appletIDForWidget", params);
    return stubReturnValue<MAppletId>("appletIDForWidget");
}

MAppletId MAppletInstanceManagerStub::appletIDForPackageName(const QString &packageName) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    stubMethodEntered("appletIDForPackageName", params);
    return stubReturnValue<MAppletId>("appletIDForPackageName");
}

MAppletId::AppletInstanceID MAppletInstanceManagerStub::appletInstanceIDFromKey(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("appletInstanceIDFromKey", params);
    return stubReturnValue<MAppletId::AppletInstanceID>("appletInstanceIDFromKey");
}

void MAppletInstanceManagerStub::removeAppletInstanceData(MAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId >(appletId));
    stubMethodEntered("removeAppletInstanceData", params);
}

MAppletId MAppletInstanceManagerStub::getUnusedAppletID()
{
    stubMethodEntered("getUnusedAppletID");
    return stubReturnValue<MAppletId>("getUnusedAppletID");
}

bool MAppletInstanceManagerStub::isValidKey(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("isValidKey", params);
    return stubReturnValue<bool>("isValidKey");
}

QString MAppletInstanceManagerStub::getParameterName(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("getParameterName", params);
    return stubReturnValue<QString>("getParameterName");
}

QString MAppletInstanceManagerStub::createKey(const MAppletId &appletId, int type)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MAppletId & >(appletId));
    params.append(new Parameter<int >(type));
    stubMethodEntered("createKey", params);
    return stubReturnValue<QString>("createKey");
}

void MAppletInstanceManagerStub::storeData(MAppletInstanceData *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletInstanceData * >(data));
    stubMethodEntered("storeData", params);
}

MAppletId MAppletInstanceManagerStub::createAppletId(MAppletId::AppletInstanceID instanceId) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAppletId::AppletInstanceID >(instanceId));
    stubMethodEntered("createAppletId", params);
    return stubReturnValue<MAppletId>("createAppletId");
}



// 3. CREATE A STUB INSTANCE
MAppletInstanceManagerStub gDefaultMAppletInstanceManagerStub;
MAppletInstanceManagerStub *gMAppletInstanceManagerStub = &gDefaultMAppletInstanceManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletInstanceManager::MAppletInstanceManager(const QString &identifier, MDataStore *dataStore)
  : dataStore( NULL ),
    fileDataStore( NULL ),
    watcher( NULL ),
    lastAppletInstanceID( 0 )
{
    gMAppletInstanceManagerStub->MAppletInstanceManagerConstructor(identifier, dataStore);
}

MAppletInstanceManager::~MAppletInstanceManager()
{
    gMAppletInstanceManagerStub->MAppletInstanceManagerDestructor();
}

bool MAppletInstanceManager::restoreApplets()
{
    return gMAppletInstanceManagerStub->restoreApplets();
}

bool MAppletInstanceManager::instantiateApplet(const QString &metadataFile)
{
    return gMAppletInstanceManagerStub->instantiateApplet(metadataFile);
}

void MAppletInstanceManager::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    gMAppletInstanceManagerStub->instantiateAppletFromPackage(packageName, metaData);
}

void MAppletInstanceManager::removeActionTriggered(bool checked)
{
    gMAppletInstanceManagerStub->removeActionTriggered(checked);
}

void MAppletInstanceManager::appletUninstalled(const QString &desktopFile)
{
    gMAppletInstanceManagerStub->appletUninstalled(desktopFile);
}

bool MAppletInstanceManager::removeApplet(MAppletId appletId)
{
    return gMAppletInstanceManagerStub->removeApplet(appletId);
}

void MAppletInstanceManager::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    gMAppletInstanceManagerStub->operationComplete(operation, pkg, error);
}

void MAppletInstanceManager::setAppletTitle(const QString &title)
{
    gMAppletInstanceManagerStub->setAppletTitle(title);
}

void MAppletInstanceManager::receiveOperation(QDBusPendingCallWatcher *watcher)
{
    gMAppletInstanceManagerStub->receiveOperation(watcher);
}

void MAppletInstanceManager::updateApplet(const MDesktopEntry &appletData)
{
    gMAppletInstanceManagerStub->updateApplet(appletData);
}

void MAppletInstanceManager::init(const QString &mashupCanvasName, MDataStore *dataStore)
{
    gMAppletInstanceManagerStub->init(mashupCanvasName, dataStore);
}

bool MAppletInstanceManager::instantiateApplet(MAppletId appletId)
{
    return gMAppletInstanceManagerStub->instantiateApplet(appletId);
}

bool MAppletInstanceManager::instantiateOutOfProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata)
{
    return gMAppletInstanceManagerStub->instantiateOutOfProcessApplet(data, metadata);
}

bool MAppletInstanceManager::instantiateInProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata)
{
    return gMAppletInstanceManagerStub->instantiateInProcessApplet(data, metadata);
}

void MAppletInstanceManager::instantiateAppletPlaceHolder(MAppletInstanceData *data)
{
    gMAppletInstanceManagerStub->instantiateAppletPlaceHolder(data);
}

void MAppletInstanceManager::queryInstallationStatus(MAppletInstanceData *data)
{
    gMAppletInstanceManagerStub->queryInstallationStatus(data);
}

void MAppletInstanceManager::createDataStore()
{
    gMAppletInstanceManagerStub->createDataStore();
}

void MAppletInstanceManager::readAppletData()
{
    gMAppletInstanceManagerStub->readAppletData();
}

MAppletInstanceData *MAppletInstanceManager::createAppletInstanceDataFromInstantiationData(const MAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData)
{
    return gMAppletInstanceManagerStub->createAppletInstanceDataFromInstantiationData(appletId, appletInstantiationData);
}

MAppletInstanceData *MAppletInstanceManager::createAppletInstanceDataFromPackageMetaData(const MAppletId &appletId, const QMap<QString, QVariant> &packageMetaData)
{
    return gMAppletInstanceManagerStub->createAppletInstanceDataFromPackageMetaData(appletId, packageMetaData);
}

void MAppletInstanceManager::setAppletHandleSizeHints(MAppletHandle &handle, MAppletInstanceData &data)
{
    gMAppletInstanceManagerStub->setAppletHandleSizeHints(handle, data);
}

QSizeF MAppletInstanceManager::qStringToQSizeF(const QString &string)
{
    return gMAppletInstanceManagerStub->qStringToQSizeF(string);
}

QString MAppletInstanceManager::dataPath() const
{
    return gMAppletInstanceManagerStub->dataPath();
}

QString MAppletInstanceManager::createAppletInstanceDataFileName(MAppletId id) const
{
    return gMAppletInstanceManagerStub->createAppletInstanceDataFileName(id);
}

void MAppletInstanceManager::freeAppletInstanceID(MAppletId id)
{
    gMAppletInstanceManagerStub->freeAppletInstanceID(id);
}

MAppletId MAppletInstanceManager::appletIDForWidget(QGraphicsWidget *widget) const
{
    return gMAppletInstanceManagerStub->appletIDForWidget(widget);
}

MAppletId MAppletInstanceManager::appletIDForPackageName(const QString &packageName) const
{
    return gMAppletInstanceManagerStub->appletIDForPackageName(packageName);
}

MAppletId::AppletInstanceID MAppletInstanceManager::appletInstanceIDFromKey(const QString &key)
{
    return gMAppletInstanceManagerStub->appletInstanceIDFromKey(key);
}

void MAppletInstanceManager::removeAppletInstanceData(MAppletId appletId)
{
    gMAppletInstanceManagerStub->removeAppletInstanceData(appletId);
}

MAppletId MAppletInstanceManager::getUnusedAppletID()
{
    return gMAppletInstanceManagerStub->getUnusedAppletID();
}

bool MAppletInstanceManager::isValidKey(const QString &key)
{
    return gMAppletInstanceManagerStub->isValidKey(key);
}

QString MAppletInstanceManager::getParameterName(const QString &key)
{
    return gMAppletInstanceManagerStub->getParameterName(key);
}

QString MAppletInstanceManager::createKey(const MAppletId &appletId, KeyType type)
{
    return gMAppletInstanceManagerStub->createKey(appletId, type);
}

void MAppletInstanceManager::storeData(MAppletInstanceData *data)
{
    gMAppletInstanceManagerStub->storeData(data);
}

MAppletId MAppletInstanceManager::createAppletId(MAppletId::AppletInstanceID instanceId) const
{
    return gMAppletInstanceManagerStub->createAppletId(instanceId);
}


#endif
