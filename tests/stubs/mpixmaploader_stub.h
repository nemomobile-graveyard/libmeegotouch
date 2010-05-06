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

#ifndef MPIXMAPLOADER_STUB
#define MPIXMAPLOADER_STUB

#include "mpixmaploader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MPixmapLoaderStub : public StubBase
{
public:
    virtual void MPixmapLoaderConstructor();
    virtual void MPixmapLoaderDestructor();
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
    MPixmapLoaderPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MPixmapLoaderStub::MPixmapLoaderConstructor()
{

}

void MPixmapLoaderStub::MPixmapLoaderDestructor()
{

}

bool MPixmapLoaderStub::addDirectoryToPixmapSearchListRecursive(const QString &applicationName, int version, const QString &directoryName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(directoryName));
    stubMethodEntered("addDirectoryToPixmapSearchListRecursive", params);
    return stubReturnValue<bool>("addDirectoryToPixmapSearchListRecursive");
}

bool MPixmapLoaderStub::addDirectoryToPixmapSearchList(const QString &applicationName, int version, const QString &directoryName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(directoryName));
    stubMethodEntered("addDirectoryToPixmapSearchList", params);
    return stubReturnValue<bool>("addDirectoryToPixmapSearchList");
}

int MPixmapLoaderStub::pixmapHandle(const QString &applicationName, int version, const QString &pixmapName, int width, int height)
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

bool MPixmapLoaderStub::releasePixmap(int handle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(handle));
    stubMethodEntered("releasePixmap", params);
    return stubReturnValue<bool>("releasePixmap");
}

bool MPixmapLoaderStub::deletePixmapSearchList(const QString &applicationName, int version)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    stubMethodEntered("deletePixmapSearchList", params);
    return stubReturnValue<bool>("deletePixmapSearchList");
}

bool MPixmapLoaderStub::addFileToPixmapSearchList(const QString &applicationName, int version, const QString &fileName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(fileName));
    stubMethodEntered("addFileToPixmapSearchList", params);
    return stubReturnValue<bool>("addFileToPixmapSearchList");
}

QString MPixmapLoaderStub::getFileForElement(const QString &applicationName, int version, const QString &pixmapName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    params.append(new Parameter<const QString & >(pixmapName));
    stubMethodEntered("getFileForElement", params);
    return stubReturnValue<QString>("getFileForElement");
}

PixmapSearchList *MPixmapLoaderStub::getPixmapSearchListForApplication(const QString &applicationName, int version)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(applicationName));
    params.append(new Parameter<int >(version));
    stubMethodEntered("getPixmapSearchListForApplication", params);
    return stubReturnValue<PixmapSearchList *>("getPixmapSearchListForApplication");
}

SharedPixmap *MPixmapLoaderStub::getSharedPixmap(const QString &filename, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("getSharedPixmap", params);
    return stubReturnValue<SharedPixmap *>("getSharedPixmap");
}

int MPixmapLoaderStub::getSharedPixmapHandle(const QString &filename, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("getSharedPixmapHandle", params);
    return stubReturnValue<int>("getSharedPixmapHandle");
}

bool MPixmapLoaderStub::existsSharedPixmap(const QString &filename, const QString &elementName, int width, int height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    params.append(new Parameter<const QString & >(elementName));
    params.append(new Parameter<int >(width));
    params.append(new Parameter<int >(height));
    stubMethodEntered("existsSharedPixmap", params);
    return stubReturnValue<bool>("existsSharedPixmap");
}

bool MPixmapLoaderStub::addSharedPixmap(const QString &filename, int pixmapHandle, const QString &elementName, int width, int height)
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
MPixmapLoaderStub gDefaultMPixmapLoaderStub;
MPixmapLoaderStub *gMPixmapLoaderStub = &gDefaultMPixmapLoaderStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
MPixmapLoader::MPixmapLoader()
{
    gMPixmapLoaderStub->MPixmapLoaderConstructor();
}

MPixmapLoader::~MPixmapLoader()
{
    gMPixmapLoaderStub->MPixmapLoaderDestructor();
}

bool MPixmapLoader::addDirectoryToPixmapSearchListRecursive(const QString &applicationName, int version, const QString &directoryName)
{
    return gMPixmapLoaderStub->addDirectoryToPixmapSearchListRecursive(applicationName, version, directoryName);
}

bool MPixmapLoader::addDirectoryToPixmapSearchList(const QString &applicationName, int version, const QString &directoryName)
{
    return gMPixmapLoaderStub->addDirectoryToPixmapSearchList(applicationName, version, directoryName);
}

int MPixmapLoader::pixmapHandle(const QString &applicationName, int version, const QString &pixmapName, int width, int height, bool usePngSizes)
{
    return gMPixmapLoaderStub->pixmapHandle(applicationName, version, pixmapName, width, height);
}

bool MPixmapLoader::releasePixmap(int handle)
{
    return gMPixmapLoaderStub->releasePixmap(handle);
}

bool MPixmapLoader::deletePixmapSearchList(const QString &applicationName, int version)
{
    return gMPixmapLoaderStub->deletePixmapSearchList(applicationName, version);
}

bool MPixmapLoader::addFileToPixmapSearchList(const QString &applicationName, int version, const QString &fileName)
{
    return gMPixmapLoaderStub->addFileToPixmapSearchList(applicationName, version, fileName);
}

QString MPixmapLoader::getFileForElement(const QString &applicationName, int version, const QString &pixmapName)
{
    return gMPixmapLoaderStub->getFileForElement(applicationName, version, pixmapName);
}

PixmapSearchList *MPixmapLoader::getPixmapSearchListForApplication(const QString &applicationName, int version)
{
    return gMPixmapLoaderStub->getPixmapSearchListForApplication(applicationName, version);
}

SharedPixmap *MPixmapLoader::getSharedPixmap(const QString &filename, const QString &elementName, int width, int height,
        bool usePngSizes)
{
    return gMPixmapLoaderStub->getSharedPixmap(filename, elementName, width, height);
}

int MPixmapLoader::getSharedPixmapHandle(const QString &filename, const QString &elementName, int width, int height)
{
    return gMPixmapLoaderStub->getSharedPixmapHandle(filename, elementName, width, height);
}

bool MPixmapLoader::existsSharedPixmap(const QString &filename, const QString &elementName, int width, int height)
{
    return gMPixmapLoaderStub->existsSharedPixmap(filename, elementName, width, height);
}

bool MPixmapLoader::addSharedPixmap(const QString &filename, int pixmapHandle, const QString &elementName, int width, int height)
{
    return gMPixmapLoaderStub->addSharedPixmap(filename, pixmapHandle, elementName, width, height);
}



#endif
