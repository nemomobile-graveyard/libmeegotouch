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

#ifndef MAPPLETBUTTON_STUB
#define MAPPLETBUTTON_STUB

#include "mappletbutton.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletButtonStub : public StubBase
{
public:
    virtual void MAppletButtonConstructor(QGraphicsItem *parent, const QString &type);
    virtual void MAppletButtonDestructor();
    virtual bool initialize(const MAppletMetaData &data);
    virtual QString metadataFilename() const;
};

// 2. IMPLEMENT STUB
void MAppletButtonStub::MAppletButtonConstructor(QGraphicsItem *parent, const QString &type)
{
    Q_UNUSED(parent);
    Q_UNUSED(type);

}
void MAppletButtonStub::MAppletButtonDestructor()
{

}
bool MAppletButtonStub::initialize(const MAppletMetaData &data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MAppletMetaData & >(data));
    stubMethodEntered("initialize", params);
    return stubReturnValue<bool>("initialize");
}

QString MAppletButtonStub::metadataFilename() const
{
    stubMethodEntered("metadataFilename");
    return stubReturnValue<QString>("metadataFilename");
}



// 3. CREATE A STUB INSTANCE
MAppletButtonStub gDefaultMAppletButtonStub;
MAppletButtonStub *gMAppletButtonStub = &gDefaultMAppletButtonStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletButton::MAppletButton(QGraphicsItem *parent, const QString &type)
{
    gMAppletButtonStub->MAppletButtonConstructor(parent, type);
}

MAppletButton::~MAppletButton()
{
    gMAppletButtonStub->MAppletButtonDestructor();
}

bool MAppletButton::initialize(const MAppletMetaData &data)
{
    metadataFile = data.fileName();
    return gMAppletButtonStub->initialize(data);
}

QString MAppletButton::metadataFilename() const
{
    return gMAppletButtonStub->metadataFilename().isEmpty() ? metadataFile : gMAppletButtonStub->metadataFilename();
}


#endif
