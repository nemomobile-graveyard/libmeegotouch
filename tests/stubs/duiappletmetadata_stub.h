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

#ifndef DUIAPPLETMETADATA_STUB
#define DUIAPPLETMETADATA_STUB

#include "duiappletmetadata.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletMetaDataStub : public StubBase
{
public:
    virtual void DuiAppletMetaDataConstructor(const QString &filename);
    virtual void DuiAppletMetaDataDestructor();
    virtual bool isValid() const;
    virtual QString runnerBinary() const;
    virtual QString appletBinary() const;
    virtual QString resourceIdentifier() const;
    virtual QString extractLibraryName(const QString &libFileName);
};

// 2. IMPLEMENT STUB
void DuiAppletMetaDataStub::DuiAppletMetaDataConstructor(const QString &filename)
{
    Q_UNUSED(filename);

}
void DuiAppletMetaDataStub::DuiAppletMetaDataDestructor()
{

}
bool DuiAppletMetaDataStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

QString DuiAppletMetaDataStub::runnerBinary() const
{
    stubMethodEntered("runnerBinary");
    return stubReturnValue<QString>("runnerBinary");
}

QString DuiAppletMetaDataStub::appletBinary() const
{
    stubMethodEntered("appletBinary");
    return stubReturnValue<QString>("appletBinary");
}

QString DuiAppletMetaDataStub::resourceIdentifier() const
{
    stubMethodEntered("resourceIdentifier");
    return stubReturnValue<QString>("resourceIdentifier");
}

QString DuiAppletMetaDataStub::extractLibraryName(const QString &libFileName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(libFileName));
    stubMethodEntered("extractLibraryName", params);
    return stubReturnValue<QString>("extractLibraryName");
}



// 3. CREATE A STUB INSTANCE
DuiAppletMetaDataStub gDefaultDuiAppletMetaDataStub;
DuiAppletMetaDataStub *gDuiAppletMetaDataStub = &gDefaultDuiAppletMetaDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletMetaData::DuiAppletMetaData(const QString &filename) : DuiDesktopEntry(filename)
{
    gDuiAppletMetaDataStub->DuiAppletMetaDataConstructor(filename);
}

DuiAppletMetaData::~DuiAppletMetaData()
{
    gDuiAppletMetaDataStub->DuiAppletMetaDataDestructor();
}

bool DuiAppletMetaData::isValid() const
{
    return gDuiAppletMetaDataStub->isValid();
}

QString DuiAppletMetaData::runnerBinary() const
{
    return gDuiAppletMetaDataStub->runnerBinary();
}

QString DuiAppletMetaData::appletBinary() const
{
    return gDuiAppletMetaDataStub->appletBinary();
}

QString DuiAppletMetaData::resourceIdentifier() const
{
    return gDuiAppletMetaDataStub->resourceIdentifier();
}

QString DuiAppletMetaData::extractLibraryName(const QString &libFileName)
{
    return gDuiAppletMetaDataStub->extractLibraryName(libFileName);
}


#endif
