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

#ifndef DUIAPPLETBUTTON_STUB
#define DUIAPPLETBUTTON_STUB

#include "duiappletbutton.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletButtonStub : public StubBase
{
public:
    virtual void DuiAppletButtonConstructor(QGraphicsItem *parent, const QString &type);
    virtual void DuiAppletButtonDestructor();
    virtual bool initialize(const DuiAppletMetaData &data);
    virtual QString metadataFilename() const;
};

// 2. IMPLEMENT STUB
void DuiAppletButtonStub::DuiAppletButtonConstructor(QGraphicsItem *parent, const QString &type)
{
    Q_UNUSED(parent);
    Q_UNUSED(type);

}
void DuiAppletButtonStub::DuiAppletButtonDestructor()
{

}
bool DuiAppletButtonStub::initialize(const DuiAppletMetaData &data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiAppletMetaData & >(data));
    stubMethodEntered("initialize", params);
    return stubReturnValue<bool>("initialize");
}

QString DuiAppletButtonStub::metadataFilename() const
{
    stubMethodEntered("metadataFilename");
    return stubReturnValue<QString>("metadataFilename");
}



// 3. CREATE A STUB INSTANCE
DuiAppletButtonStub gDefaultDuiAppletButtonStub;
DuiAppletButtonStub *gDuiAppletButtonStub = &gDefaultDuiAppletButtonStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletButton::DuiAppletButton(QGraphicsItem *parent, const QString &type)
{
    gDuiAppletButtonStub->DuiAppletButtonConstructor(parent, type);
}

DuiAppletButton::~DuiAppletButton()
{
    gDuiAppletButtonStub->DuiAppletButtonDestructor();
}

bool DuiAppletButton::initialize(const DuiAppletMetaData &data)
{
    metadataFile = data.fileName();
    return gDuiAppletButtonStub->initialize(data);
}

QString DuiAppletButton::metadataFilename() const
{
    return gDuiAppletButtonStub->metadataFilename().isEmpty() ? metadataFile : gDuiAppletButtonStub->metadataFilename();
}


#endif
