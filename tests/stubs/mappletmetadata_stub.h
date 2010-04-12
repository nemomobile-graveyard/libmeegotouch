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

#ifndef MAPPLETMETADATA_STUB
#define MAPPLETMETADATA_STUB

#include "mappletmetadata.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletMetaDataStub : public StubBase
{
public:
    virtual void MAppletMetaDataConstructor(const QString &filename);
    virtual void MAppletMetaDataDestructor();
    virtual bool isValid() const;
    virtual QString runnerBinary() const;
    virtual QString appletBinary() const;
    virtual QString resourceIdentifier() const;
    virtual QString extractLibraryName(const QString &libFileName);
};

// 2. IMPLEMENT STUB
void MAppletMetaDataStub::MAppletMetaDataConstructor(const QString &filename)
{
    Q_UNUSED(filename);

}
void MAppletMetaDataStub::MAppletMetaDataDestructor()
{

}
bool MAppletMetaDataStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

QString MAppletMetaDataStub::runnerBinary() const
{
    stubMethodEntered("runnerBinary");
    return stubReturnValue<QString>("runnerBinary");
}

QString MAppletMetaDataStub::appletBinary() const
{
    stubMethodEntered("appletBinary");
    return stubReturnValue<QString>("appletBinary");
}

QString MAppletMetaDataStub::resourceIdentifier() const
{
    stubMethodEntered("resourceIdentifier");
    return stubReturnValue<QString>("resourceIdentifier");
}

QString MAppletMetaDataStub::extractLibraryName(const QString &libFileName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(libFileName));
    stubMethodEntered("extractLibraryName", params);
    return stubReturnValue<QString>("extractLibraryName");
}



// 3. CREATE A STUB INSTANCE
MAppletMetaDataStub gDefaultMAppletMetaDataStub;
MAppletMetaDataStub *gMAppletMetaDataStub = &gDefaultMAppletMetaDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletMetaData::MAppletMetaData(const QString &filename) : MDesktopEntry(filename)
{
    gMAppletMetaDataStub->MAppletMetaDataConstructor(filename);
}

MAppletMetaData::~MAppletMetaData()
{
    gMAppletMetaDataStub->MAppletMetaDataDestructor();
}

bool MAppletMetaData::isValid() const
{
    return gMAppletMetaDataStub->isValid();
}

QString MAppletMetaData::runnerBinary() const
{
    return gMAppletMetaDataStub->runnerBinary();
}

QString MAppletMetaData::appletBinary() const
{
    return gMAppletMetaDataStub->appletBinary();
}

QString MAppletMetaData::resourceIdentifier() const
{
    return gMAppletMetaDataStub->resourceIdentifier();
}

QString MAppletMetaData::extractLibraryName(const QString &libFileName)
{
    return gMAppletMetaDataStub->extractLibraryName(libFileName);
}


#endif
