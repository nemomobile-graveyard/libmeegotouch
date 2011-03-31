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

#ifndef MSETTINGSLANGUAGESELECTION_STUB
#define MSETTINGSLANGUAGESELECTION_STUB

#include "msettingslanguageselection.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageSelectionStub : public StubBase
{
public:
    virtual void MSettingsLanguageSelectionConstructor(const QString &key, const QString &title);
    virtual void MSettingsLanguageSelectionDestructor();
    virtual QString key() const;
    virtual QString title() const;
    virtual MSettingsLanguageOption *addOption(const QString &title, int value);
    virtual MSettingsLanguageOption *addOption(MSettingsLanguageOption *option);
    virtual uint numOptions() const;
    virtual QList<const MSettingsLanguageOption *> options() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageSelectionStub::MSettingsLanguageSelectionConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void MSettingsLanguageSelectionStub::MSettingsLanguageSelectionDestructor()
{

}
QString MSettingsLanguageSelectionStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString MSettingsLanguageSelectionStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}

MSettingsLanguageOption *MSettingsLanguageSelectionStub::addOption(const QString &title, int value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString >(title));
    params.append(new Parameter<int >(value));
    stubMethodEntered("addOption", params);
    return stubReturnValue<MSettingsLanguageOption *>("addOption");
}

MSettingsLanguageOption *MSettingsLanguageSelectionStub::addOption(MSettingsLanguageOption *option)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MSettingsLanguageOption * >(option));
    stubMethodEntered("addOption", params);
    return stubReturnValue<MSettingsLanguageOption *>("addOption");
}

uint MSettingsLanguageSelectionStub::numOptions() const
{
    stubMethodEntered("numOptions");
    return stubReturnValue<uint>("numOptions");
}

QList<const MSettingsLanguageOption *> MSettingsLanguageSelectionStub::options() const
{
    stubMethodEntered("options");
    return stubReturnValue<QList<const MSettingsLanguageOption *> >("options");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageSelectionStub gDefaultMSettingsLanguageSelectionStub;
MSettingsLanguageSelectionStub *gMSettingsLanguageSelectionStub = &gDefaultMSettingsLanguageSelectionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageSelection::MSettingsLanguageSelection(const QString &key, const QString &title)
{
    gMSettingsLanguageSelectionStub->MSettingsLanguageSelectionConstructor(key, title);
}

MSettingsLanguageSelection::~MSettingsLanguageSelection()
{
    gMSettingsLanguageSelectionStub->MSettingsLanguageSelectionDestructor();
}

QString MSettingsLanguageSelection::key() const
{
    return gMSettingsLanguageSelectionStub->key();
}

QString MSettingsLanguageSelection::title() const
{
    return gMSettingsLanguageSelectionStub->title();
}

MSettingsLanguageOption *MSettingsLanguageSelection::addOption(const QString &title, int value)
{
    return gMSettingsLanguageSelectionStub->addOption(title, value);
}

MSettingsLanguageOption *MSettingsLanguageSelection::addOption(MSettingsLanguageOption *option)
{
    return gMSettingsLanguageSelectionStub->addOption(option);
}

uint MSettingsLanguageSelection::numOptions() const
{
    return gMSettingsLanguageSelectionStub->numOptions();
}

QList<const MSettingsLanguageOption *> MSettingsLanguageSelection::options() const
{
    return gMSettingsLanguageSelectionStub->options();
}


#endif
