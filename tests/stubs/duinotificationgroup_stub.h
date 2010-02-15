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

#ifndef DUINOTIFICATIONGROUP_STUB
#define DUINOTIFICATIONGROUP_STUB

#include "duinotificationgroup.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiNotificationGroupStub : public StubBase
{
public:
    virtual void DuiNotificationGroupConstructor(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, const DuiRemoteAction &action, uint count, bool persistent);
    virtual void DuiNotificationGroupConstructor(unsigned int id);
    virtual void DuiNotificationGroupDestructor();
    virtual unsigned int id() const;
    virtual bool update(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, uint count, const DuiRemoteAction &action);
    virtual bool remove();
    virtual bool isValid() const;
    virtual void DuiNotificationGroupConstructor(DuiNotificationGroupPrivate &dd);
    DuiNotificationGroupPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiNotificationGroupStub::DuiNotificationGroupConstructor(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, const DuiRemoteAction &action, uint count, bool persistent)
{
    Q_UNUSED(eventType);
    Q_UNUSED(summary);
    Q_UNUSED(body);
    Q_UNUSED(imageURI);
    Q_UNUSED(action);
    Q_UNUSED(count);
    Q_UNUSED(persistent);

}
void DuiNotificationGroupStub::DuiNotificationGroupConstructor(unsigned int id)
{
    Q_UNUSED(id);

}
void DuiNotificationGroupStub::DuiNotificationGroupDestructor()
{

}
unsigned int DuiNotificationGroupStub::id() const
{
    stubMethodEntered("id");
    return stubReturnValue<unsigned int>("id");
}

bool DuiNotificationGroupStub::update(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, uint count, const DuiRemoteAction &action)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    params.append(new Parameter<const DuiRemoteAction & >(action));
    stubMethodEntered("update", params);
    return stubReturnValue<bool>("update");
}

bool DuiNotificationGroupStub::remove()
{
    stubMethodEntered("remove");
    return stubReturnValue<bool>("remove");
}

bool DuiNotificationGroupStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

void DuiNotificationGroupStub::DuiNotificationGroupConstructor(DuiNotificationGroupPrivate &dd)
{
    Q_UNUSED(dd);

}


// 3. CREATE A STUB INSTANCE
DuiNotificationGroupStub gDefaultDuiNotificationGroupStub;
DuiNotificationGroupStub *gDuiNotificationGroupStub = &gDefaultDuiNotificationGroupStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiNotificationGroup::DuiNotificationGroup(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, const DuiRemoteAction &action, uint count, bool persistent)
{
    gDuiNotificationGroupStub->DuiNotificationGroupConstructor(eventType, summary, body, imageURI, action, count, persistent);
}

DuiNotificationGroup::DuiNotificationGroup(unsigned int id)
{
    gDuiNotificationGroupStub->DuiNotificationGroupConstructor(id);
}

DuiNotificationGroup::~DuiNotificationGroup()
{
    gDuiNotificationGroupStub->DuiNotificationGroupDestructor();
}

unsigned int DuiNotificationGroup::id() const
{
    return gDuiNotificationGroupStub->id();
}

bool DuiNotificationGroup::update(const QString &eventType, const QString &summary, const QString &body, const QString &imageURI, uint count, const DuiRemoteAction &action)
{
    return gDuiNotificationGroupStub->update(eventType, summary, body, imageURI, count, action);
}

bool DuiNotificationGroup::remove()
{
    return gDuiNotificationGroupStub->remove();
}

bool DuiNotificationGroup::isValid() const
{
    return gDuiNotificationGroupStub->isValid();
}

DuiNotificationGroup::DuiNotificationGroup(DuiNotificationGroupPrivate &dd)
{
    gDuiNotificationGroupStub->DuiNotificationGroupConstructor(dd);
}


#endif
