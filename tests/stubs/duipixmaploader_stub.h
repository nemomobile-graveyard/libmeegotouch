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

#ifndef DUIPIXMAPLOADER_STUB
#define DUIPIXMAPLOADER_STUB

#include "duipixmaploader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiPixmapLoaderStub : public StubBase
{
public:
    virtual void DuiPixmapLoaderConstructor();
    virtual void DuiPixmapLoaderDestructor();
    virtual bool addDirectoryToPixmapSearchListRecursive(const QString &applicationName, int version, const QString &directoryName);
    virtual bool addDirectoryToPixmapSearchList(const QString &applicationName, int version, const QString &directoryName);
    virtual int pixmapHandle(const QString &applicationName, int version, const QString &pixmapName, int width, int height);
    virtual bool releasePixmap(int handle);
    virtual bool deletePixmapSearchList(const QString &applicationName, int version);
    virtual bool addFileToPixmapSearchList(const QString &applicationName, int version, const QString &fileName);
    virtual QString getFileForElement(const QString &applicationName, int version, const QString &pixmapName);
    virtual PixmapSearchList *getPixmapSearchListForApplication(const QString &applicationName, int version);
    virtual SharedPixmap *getSharedPixmap(const QString &filename, const QString &elementName, int width, int height);
    virtual int getSharedPixmapHandle(const QString &filename, const QString &elementName, int width, int height);
    virtual bool existsSharedPixmap(const QString &filename, const QString &elementName, int width, int height);
    virtual bool addSharedPixmap(const QString &filename, int pixmapHandle, const QString &elementName, int width, int height);
    DuiPixmapLoaderPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiPixmapLoaderStub::DuiPixmapLoaderConstructor()
{

}

void DuiPixmapLoaderStub::DuiPixmapLoaderDestructor()
{

}

bool DuiPixmapLoaderStub::addDirectoryToPixmapSearchListRecursive(const QString &applicationName, int version, const QString &directoryName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(directoryName));
    stubMethodEntered("addDirectoryToPixmapSearchListRecursive", params);
    return stubReturnValue<bool>("addDirectoryToPixmapSearchListRecursive");
}

bool DuiPixmapLoaderStub::addDirectoryToPixmapSearchList(const QString &applicationName, int version, const QString &directoryName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(directoryName));
    stubMethodEntered("addDirectoryToPixmapSearchList", params);
    return stubReturnValue<bool>("addDirectoryToPixmapSearchList");
}

int DuiPixmapLoaderStub::pixmapHandle(const QString &applicationName, int version, const QString &pixmapName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(pixmapName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("pixmapHandle", params);
    return stubReturnValue<int>("pixmapHandle");
}

bool DuiPixmapLoaderStub::releasePixmap(int handle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(handle));
    stubMethodEntered("releasePixmap", params);
    return stubReturnValue<bool>("releasePixmap");
}

bool DuiPixmapLoaderStub::deletePixmapSearchList(const QString &applicationName, int version)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    stubMethodEntered("deletePixmapSearchList", params);
    return stubReturnValue<bool>("deletePixmapSearchList");
}

bool DuiPixmapLoaderStub::addFileToPixmapSearchList(const QString &applicationName, int version, const QString &fileName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(fileName));
    stubMethodEntered("addFileToPixmapSearchList", params);
    return stubReturnValue<bool>("addFileToPixmapSearchList");
}

QString DuiPixmapLoaderStub::getFileForElement(const QString &applicationName, int version, const QString &pixmapName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(pixmapName));
    stubMethodEntered("getFileForElement", params);
    return stubReturnValue<QString>("getFileForElement");
}

PixmapSearchList *DuiPixmapLoaderStub::getPixmapSearchListForApplication(const QString &applicationName, int version)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    stubMethodEntered("getPixmapSearchListForApplication", params);
    return stubReturnValue<PixmapSearchList *>("getPixmapSearchListForApplication");
}

SharedPixmap *DuiPixmapLoaderStub::getSharedPixmap(const QString &filename, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("getSharedPixmap", params);
    return stubReturnValue<SharedPixmap *>("getSharedPixmap");
}

int DuiPixmapLoaderStub::getSharedPixmapHandle(const QString &filename, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("getSharedPixmapHandle", params);
    return stubReturnValue<int>("getSharedPixmapHandle");
}

bool DuiPixmapLoaderStub::existsSharedPixmap(const QString &filename, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("existsSharedPixmap", params);
    return stubReturnValue<bool>("existsSharedPixmap");
}

bool DuiPixmapLoaderStub::addSharedPixmap(const QString &filename, int pixmapHandle, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<int >(pixmapHandle));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("addSharedPixmap", params);
    return stubReturnValue<bool>("addSharedPixmap");
}

// 3. CREATE A STUB INSTANCE
DuiPixmapLoaderStub gDefaultDuiPixmapLoaderStub;
DuiPixmapLoaderStub *gDuiPixmapLoaderStub = &gDefaultDuiPixmapLoaderStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiPixmapLoader::DuiPixmapLoader()
{
    gDuiPixmapLoaderStub->DuiPixmapLoaderConstructor();
}

DuiPixmapLoader::~DuiPixmapLoader()
{
    gDuiPixmapLoaderStub->DuiPixmapLoaderDestructor();
}

bool DuiPixmapLoader::addDirectoryToPixmapSearchListRecursive(const QString &applicationName, int version, const QString &directoryName)
{
    return gDuiPixmapLoaderStub->addDirectoryToPixmapSearchListRecursive(applicationName, version, directoryName);
}

bool DuiPixmapLoader::addDirectoryToPixmapSearchList(const QString &applicationName, int version, const QString &directoryName)
{
    return gDuiPixmapLoaderStub->addDirectoryToPixmapSearchList(applicationName, version, directoryName);
}

int DuiPixmapLoader::pixmapHandle(const QString &applicationName, int version, const QString &pixmapName, int width, int height, bool usePngSizes)
{
    return gDuiPixmapLoaderStub->pixmapHandle(applicationName, version, pixmapName, width, height);
}

bool DuiPixmapLoader::releasePixmap(int handle)
{
    return gDuiPixmapLoaderStub->releasePixmap(handle);
}

bool DuiPixmapLoader::deletePixmapSearchList(const QString &applicationName, int version)
{
    return gDuiPixmapLoaderStub->deletePixmapSearchList(applicationName, version);
}

bool DuiPixmapLoader::addFileToPixmapSearchList(const QString &applicationName, int version, const QString &fileName)
{
    return gDuiPixmapLoaderStub->addFileToPixmapSearchList(applicationName, version, fileName);
}

QString DuiPixmapLoader::getFileForElement(const QString &applicationName, int version, const QString &pixmapName)
{
    return gDuiPixmapLoaderStub->getFileForElement(applicationName, version, pixmapName);
}

PixmapSearchList *DuiPixmapLoader::getPixmapSearchListForApplication(const QString &applicationName, int version)
{
    return gDuiPixmapLoaderStub->getPixmapSearchListForApplication(applicationName, version);
}

SharedPixmap *DuiPixmapLoader::getSharedPixmap(const QString &filename, const QString &elementName, int width, int height,
        bool usePngSizes)
{
    return gDuiPixmapLoaderStub->getSharedPixmap(filename, elementName, width, height);
}

int DuiPixmapLoader::getSharedPixmapHandle(const QString &filename, const QString &elementName, int width, int height)
{
    return gDuiPixmapLoaderStub->getSharedPixmapHandle(filename, elementName, width, height);
}

bool DuiPixmapLoader::existsSharedPixmap(const QString &filename, const QString &elementName, int width, int height)
{
    return gDuiPixmapLoaderStub->existsSharedPixmap(filename, elementName, width, height);
}

bool DuiPixmapLoader::addSharedPixmap(const QString &filename, int pixmapHandle, const QString &elementName, int width, int height)
{
    return gDuiPixmapLoaderStub->addSharedPixmap(filename, pixmapHandle, elementName, width, height);
}



#endif
