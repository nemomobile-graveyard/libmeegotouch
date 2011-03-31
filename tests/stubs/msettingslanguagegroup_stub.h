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

#ifndef MSETTINGSLANGUAGEGROUP_STUB
#define MSETTINGSLANGUAGEGROUP_STUB

#include "msettingslanguagegroup.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageGroupStub : public StubBase
{
public:
    virtual void MSettingsLanguageGroupConstructor(const QString &title);
    virtual void MSettingsLanguageGroupDestructor();
    virtual QString title() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageGroupStub::MSettingsLanguageGroupConstructor(const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void MSettingsLanguageGroupStub::MSettingsLanguageGroupDestructor()
{

}
QString MSettingsLanguageGroupStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageGroupStub gDefaultMSettingsLanguageGroupStub;
MSettingsLanguageGroupStub *gMSettingsLanguageGroupStub = &gDefaultMSettingsLanguageGroupStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageGroup::MSettingsLanguageGroup(const QString &title)
{
    gMSettingsLanguageGroupStub->MSettingsLanguageGroupConstructor(title);
}

MSettingsLanguageGroup::~MSettingsLanguageGroup()
{
    gMSettingsLanguageGroupStub->MSettingsLanguageGroupDestructor();
}

QString MSettingsLanguageGroup::title() const
{
    return gMSettingsLanguageGroupStub->title();
}


#endif
