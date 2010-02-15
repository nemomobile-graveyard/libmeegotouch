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

#ifndef DUIAPPLETINSTANCEMANAGER_STUB
#define DUIAPPLETINSTANCEMANAGER_STUB

#include "duiappletinstancemanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletInstanceManagerStub : public StubBase
{
public:
    virtual void DuiAppletInstanceManagerConstructor(const QString &identifier, DuiDataStore *dataStore);
    virtual void DuiAppletInstanceManagerDestructor();
    virtual bool restoreApplets();
    virtual bool instantiateApplet(const QString &metadataFile);
    virtual void instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData);
    virtual void removeActionTriggered(bool checked);
    virtual void appletUninstalled(const QString &desktopFile);
    virtual bool removeApplet(DuiAppletId appletId);
    virtual void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    virtual void setAppletTitle(const QString &title);
    virtual void receiveOperation(QDBusPendingCallWatcher *watcher);
    virtual void init(const QString &mashupCanvasName, DuiDataStore *dataStore);
    virtual bool instantiateApplet(DuiAppletId appletId);
    virtual bool instantiateOutOfProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata);
    virtual bool instantiateInProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata);
    virtual void instantiateAppletPlaceHolder(DuiAppletInstanceData *data);
    virtual void queryInstallationStatus(DuiAppletInstanceData *data);
    virtual void createDataStore();
    virtual void readAppletData();
    virtual DuiAppletInstanceData *createAppletInstanceDataFromInstantiationData(const DuiAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData);
    virtual DuiAppletInstanceData *createAppletInstanceDataFromPackageMetaData(const DuiAppletId &appletId, const QMap<QString, QVariant> &packageMetaData);
    virtual void setAppletHandleSizeHints(DuiAppletHandle &handle, DuiAppletInstanceData &data);
    virtual QSizeF qStringToQSizeF(const QString &string);
    virtual QString dataPath() const;
    virtual QString createAppletInstanceDataFileName(DuiAppletId id) const;
    virtual void freeAppletInstanceID(DuiAppletId id);
    virtual DuiAppletId appletIDForWidget(DuiWidget *widget) const;
    virtual DuiAppletId appletIDForPackageName(const QString &packageName) const;
    virtual DuiAppletId::AppletInstanceID appletInstanceIDFromKey(const QString &key);
    virtual void removeAppletInstanceData(DuiAppletId appletId);
    virtual DuiAppletId getUnusedAppletID();
    virtual bool isValidKey(const QString &key);
    virtual QString getParameterName(const QString &key);
    virtual QString createKey(const DuiAppletId &appletId, int type);
    virtual void storeData(DuiAppletInstanceData *data);
    virtual DuiAppletId createAppletId(DuiAppletId::AppletInstanceID instanceId) const;
};

// 2. IMPLEMENT STUB
void DuiAppletInstanceManagerStub::DuiAppletInstanceManagerConstructor(const QString &identifier, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(identifier));
    params.append(new Parameter<DuiDataStore *>(dataStore));
    stubMethodEntered("constructor", params);
}
void DuiAppletInstanceManagerStub::DuiAppletInstanceManagerDestructor()
{

}
bool DuiAppletInstanceManagerStub::restoreApplets()
{
    stubMethodEntered("restoreApplets");
    return stubReturnValue<bool>("restoreApplets");
}

bool DuiAppletInstanceManagerStub::instantiateApplet(const QString &metadataFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(metadataFile));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<bool>("instantiateApplet");
}

void DuiAppletInstanceManagerStub::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    params.append(new Parameter<const QMap<QString, QVariant> & >(metaData));
    stubMethodEntered("instantiateAppletFromPackage", params);
}

void DuiAppletInstanceManagerStub::removeActionTriggered(bool checked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(checked));
    stubMethodEntered("removeActionTriggered", params);
}

void DuiAppletInstanceManagerStub::appletUninstalled(const QString &desktopFile)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(desktopFile));
    stubMethodEntered("appletUninstalled", params);
}

bool DuiAppletInstanceManagerStub::removeApplet(DuiAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(appletId));
    stubMethodEntered("removeApplet", params);
    return stubReturnValue<bool>("removeApplet");
}

void DuiAppletInstanceManagerStub::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(operation));
    params.append(new Parameter<const QString & >(pkg));
    params.append(new Parameter<const QString & >(error));
    stubMethodEntered("operationComplete", params);
}

void DuiAppletInstanceManagerStub::setAppletTitle(const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(title));
    stubMethodEntered("setAppletTitle", params);
}

void DuiAppletInstanceManagerStub::receiveOperation(QDBusPendingCallWatcher *watcher)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QDBusPendingCallWatcher * >(watcher));
    stubMethodEntered("receiveOperation", params);
}

void DuiAppletInstanceManagerStub::init(const QString &mashupCanvasName, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(mashupCanvasName));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("init", params);
}

bool DuiAppletInstanceManagerStub::instantiateApplet(DuiAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(appletId));
    stubMethodEntered("instantiateApplet", params);
    return stubReturnValue<bool>("instantiateApplet");
}

bool DuiAppletInstanceManagerStub::instantiateOutOfProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletInstanceData * >(data));
    params.append(new Parameter<const DuiAppletMetaData & >(metadata));
    stubMethodEntered("instantiateOutOfProcessApplet", params);
    return stubReturnValue<bool>("instantiateOutOfProcessApplet");
}

bool DuiAppletInstanceManagerStub::instantiateInProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletInstanceData * >(data));
    params.append(new Parameter<const DuiAppletMetaData & >(metadata));
    stubMethodEntered("instantiateInProcessApplet", params);
    return stubReturnValue<bool>("instantiateInProcessApplet");
}

void DuiAppletInstanceManagerStub::instantiateAppletPlaceHolder(DuiAppletInstanceData *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletInstanceData * >(data));
    stubMethodEntered("instantiateAppletPlaceHolder", params);
}

void DuiAppletInstanceManagerStub::queryInstallationStatus(DuiAppletInstanceData *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletInstanceData * >(data));
    stubMethodEntered("queryInstallationStatus", params);
}

void DuiAppletInstanceManagerStub::createDataStore()
{
    stubMethodEntered("createDataStore");
}

void DuiAppletInstanceManagerStub::readAppletData()
{
    stubMethodEntered("readAppletData");
}

DuiAppletInstanceData *DuiAppletInstanceManagerStub::createAppletInstanceDataFromInstantiationData(const DuiAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiAppletId & >(appletId));
    params.append(new Parameter<const QMap<QString, QVariant> & >(appletInstantiationData));
    stubMethodEntered("createAppletInstanceDataFromInstantiationData", params);
    return stubReturnValue<DuiAppletInstanceData *>("createAppletInstanceDataFromInstantiationData");
}

DuiAppletInstanceData *DuiAppletInstanceManagerStub::createAppletInstanceDataFromPackageMetaData(const DuiAppletId &appletId, const QMap<QString, QVariant> &packageMetaData)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiAppletId & >(appletId));
    params.append(new Parameter<const QMap<QString, QVariant> & >(packageMetaData));
    stubMethodEntered("createAppletInstanceDataFromPackageMetaData", params);
    return stubReturnValue<DuiAppletInstanceData *>("createAppletInstanceDataFromPackageMetaData");
}

void DuiAppletInstanceManagerStub::setAppletHandleSizeHints(DuiAppletHandle &handle, DuiAppletInstanceData &data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletHandle & >(handle));
    params.append(new Parameter<DuiAppletInstanceData & >(data));
    stubMethodEntered("setAppletHandleSizeHints", params);
}

QSizeF DuiAppletInstanceManagerStub::qStringToQSizeF(const QString &string)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(string));
    stubMethodEntered("qStringToQSizeF", params);
    return stubReturnValue<QSizeF>("qStringToQSizeF");
}

QString DuiAppletInstanceManagerStub::dataPath() const
{
    stubMethodEntered("dataPath");
    return stubReturnValue<QString>("dataPath");
}

QString DuiAppletInstanceManagerStub::createAppletInstanceDataFileName(DuiAppletId id) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(id));
    stubMethodEntered("createAppletInstanceDataFileName", params);
    return stubReturnValue<QString>("createAppletInstanceDataFileName");
}

void DuiAppletInstanceManagerStub::freeAppletInstanceID(DuiAppletId id)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(id));
    stubMethodEntered("freeAppletInstanceID", params);
}

DuiAppletId DuiAppletInstanceManagerStub::appletIDForWidget(DuiWidget *widget) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    stubMethodEntered("appletIDForWidget", params);
    return stubReturnValue<DuiAppletId>("appletIDForWidget");
}

DuiAppletId DuiAppletInstanceManagerStub::appletIDForPackageName(const QString &packageName) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(packageName));
    stubMethodEntered("appletIDForPackageName", params);
    return stubReturnValue<DuiAppletId>("appletIDForPackageName");
}

DuiAppletId::AppletInstanceID DuiAppletInstanceManagerStub::appletInstanceIDFromKey(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("appletInstanceIDFromKey", params);
    return stubReturnValue<DuiAppletId::AppletInstanceID>("appletInstanceIDFromKey");
}

void DuiAppletInstanceManagerStub::removeAppletInstanceData(DuiAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(appletId));
    stubMethodEntered("removeAppletInstanceData", params);
}

DuiAppletId DuiAppletInstanceManagerStub::getUnusedAppletID()
{
    stubMethodEntered("getUnusedAppletID");
    return stubReturnValue<DuiAppletId>("getUnusedAppletID");
}

bool DuiAppletInstanceManagerStub::isValidKey(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("isValidKey", params);
    return stubReturnValue<bool>("isValidKey");
}

QString DuiAppletInstanceManagerStub::getParameterName(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("getParameterName", params);
    return stubReturnValue<QString>("getParameterName");
}

QString DuiAppletInstanceManagerStub::createKey(const DuiAppletId &appletId, int type)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiAppletId & >(appletId));
    params.append(new Parameter<int >(type));
    stubMethodEntered("createKey", params);
    return stubReturnValue<QString>("createKey");
}

void DuiAppletInstanceManagerStub::storeData(DuiAppletInstanceData *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletInstanceData * >(data));
    stubMethodEntered("storeData", params);
}

DuiAppletId DuiAppletInstanceManagerStub::createAppletId(DuiAppletId::AppletInstanceID instanceId) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId::AppletInstanceID >(instanceId));
    stubMethodEntered("createAppletId", params);
    return stubReturnValue<DuiAppletId>("createAppletId");
}



// 3. CREATE A STUB INSTANCE
DuiAppletInstanceManagerStub gDefaultDuiAppletInstanceManagerStub;
DuiAppletInstanceManagerStub *gDuiAppletInstanceManagerStub = &gDefaultDuiAppletInstanceManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletInstanceManager::DuiAppletInstanceManager(const QString &identifier, DuiDataStore *dataStore)
{
    gDuiAppletInstanceManagerStub->DuiAppletInstanceManagerConstructor(identifier, dataStore);
}

DuiAppletInstanceManager::~DuiAppletInstanceManager()
{
    gDuiAppletInstanceManagerStub->DuiAppletInstanceManagerDestructor();
}

bool DuiAppletInstanceManager::restoreApplets()
{
    return gDuiAppletInstanceManagerStub->restoreApplets();
}

bool DuiAppletInstanceManager::instantiateApplet(const QString &metadataFile)
{
    return gDuiAppletInstanceManagerStub->instantiateApplet(metadataFile);
}

void DuiAppletInstanceManager::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    gDuiAppletInstanceManagerStub->instantiateAppletFromPackage(packageName, metaData);
}

void DuiAppletInstanceManager::removeActionTriggered(bool checked)
{
    gDuiAppletInstanceManagerStub->removeActionTriggered(checked);
}

void DuiAppletInstanceManager::appletUninstalled(const QString &desktopFile)
{
    gDuiAppletInstanceManagerStub->appletUninstalled(desktopFile);
}

bool DuiAppletInstanceManager::removeApplet(DuiAppletId appletId)
{
    return gDuiAppletInstanceManagerStub->removeApplet(appletId);
}

void DuiAppletInstanceManager::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    gDuiAppletInstanceManagerStub->operationComplete(operation, pkg, error);
}

void DuiAppletInstanceManager::setAppletTitle(const QString &title)
{
    gDuiAppletInstanceManagerStub->setAppletTitle(title);
}

void DuiAppletInstanceManager::receiveOperation(QDBusPendingCallWatcher *watcher)
{
    gDuiAppletInstanceManagerStub->receiveOperation(watcher);
}

void DuiAppletInstanceManager::init(const QString &mashupCanvasName, DuiDataStore *dataStore)
{
    gDuiAppletInstanceManagerStub->init(mashupCanvasName, dataStore);
}

bool DuiAppletInstanceManager::instantiateApplet(DuiAppletId appletId)
{
    return gDuiAppletInstanceManagerStub->instantiateApplet(appletId);
}

bool DuiAppletInstanceManager::instantiateOutOfProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata)
{
    return gDuiAppletInstanceManagerStub->instantiateOutOfProcessApplet(data, metadata);
}

bool DuiAppletInstanceManager::instantiateInProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata)
{
    return gDuiAppletInstanceManagerStub->instantiateInProcessApplet(data, metadata);
}

void DuiAppletInstanceManager::instantiateAppletPlaceHolder(DuiAppletInstanceData *data)
{
    gDuiAppletInstanceManagerStub->instantiateAppletPlaceHolder(data);
}

void DuiAppletInstanceManager::queryInstallationStatus(DuiAppletInstanceData *data)
{
    gDuiAppletInstanceManagerStub->queryInstallationStatus(data);
}

void DuiAppletInstanceManager::createDataStore()
{
    gDuiAppletInstanceManagerStub->createDataStore();
}

void DuiAppletInstanceManager::readAppletData()
{
    gDuiAppletInstanceManagerStub->readAppletData();
}

DuiAppletInstanceData *DuiAppletInstanceManager::createAppletInstanceDataFromInstantiationData(const DuiAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData)
{
    return gDuiAppletInstanceManagerStub->createAppletInstanceDataFromInstantiationData(appletId, appletInstantiationData);
}

DuiAppletInstanceData *DuiAppletInstanceManager::createAppletInstanceDataFromPackageMetaData(const DuiAppletId &appletId, const QMap<QString, QVariant> &packageMetaData)
{
    return gDuiAppletInstanceManagerStub->createAppletInstanceDataFromPackageMetaData(appletId, packageMetaData);
}

void DuiAppletInstanceManager::setAppletHandleSizeHints(DuiAppletHandle &handle, DuiAppletInstanceData &data)
{
    gDuiAppletInstanceManagerStub->setAppletHandleSizeHints(handle, data);
}

QSizeF DuiAppletInstanceManager::qStringToQSizeF(const QString &string)
{
    return gDuiAppletInstanceManagerStub->qStringToQSizeF(string);
}

QString DuiAppletInstanceManager::dataPath() const
{
    return gDuiAppletInstanceManagerStub->dataPath();
}

QString DuiAppletInstanceManager::createAppletInstanceDataFileName(DuiAppletId id) const
{
    return gDuiAppletInstanceManagerStub->createAppletInstanceDataFileName(id);
}

void DuiAppletInstanceManager::freeAppletInstanceID(DuiAppletId id)
{
    gDuiAppletInstanceManagerStub->freeAppletInstanceID(id);
}

DuiAppletId DuiAppletInstanceManager::appletIDForWidget(DuiWidget *widget) const
{
    return gDuiAppletInstanceManagerStub->appletIDForWidget(widget);
}

DuiAppletId DuiAppletInstanceManager::appletIDForPackageName(const QString &packageName) const
{
    return gDuiAppletInstanceManagerStub->appletIDForPackageName(packageName);
}

DuiAppletId::AppletInstanceID DuiAppletInstanceManager::appletInstanceIDFromKey(const QString &key)
{
    return gDuiAppletInstanceManagerStub->appletInstanceIDFromKey(key);
}

void DuiAppletInstanceManager::removeAppletInstanceData(DuiAppletId appletId)
{
    gDuiAppletInstanceManagerStub->removeAppletInstanceData(appletId);
}

DuiAppletId DuiAppletInstanceManager::getUnusedAppletID()
{
    return gDuiAppletInstanceManagerStub->getUnusedAppletID();
}

bool DuiAppletInstanceManager::isValidKey(const QString &key)
{
    return gDuiAppletInstanceManagerStub->isValidKey(key);
}

QString DuiAppletInstanceManager::getParameterName(const QString &key)
{
    return gDuiAppletInstanceManagerStub->getParameterName(key);
}

QString DuiAppletInstanceManager::createKey(const DuiAppletId &appletId, KeyType type)
{
    return gDuiAppletInstanceManagerStub->createKey(appletId, type);
}

void DuiAppletInstanceManager::storeData(DuiAppletInstanceData *data)
{
    gDuiAppletInstanceManagerStub->storeData(data);
}

DuiAppletId DuiAppletInstanceManager::createAppletId(DuiAppletId::AppletInstanceID instanceId) const
{
    return gDuiAppletInstanceManagerStub->createAppletId(instanceId);
}


#endif
