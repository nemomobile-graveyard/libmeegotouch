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

#ifndef DUISETTINGSLANGUAGEINTEGER_STUB
#define DUISETTINGSLANGUAGEINTEGER_STUB

#include "duisettingslanguageinteger.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageIntegerStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageIntegerConstructor(const QString &key, const QString &title);
    virtual void DuiSettingsLanguageIntegerDestructor();
    virtual QString key() const;
    virtual QString title() const;
    virtual bool minValue(int &value) const;
    virtual bool maxValue(int &value) const;
    virtual void setMinValue(int newMin);
    virtual void setMaxValue(int newMax);
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageIntegerStub::DuiSettingsLanguageIntegerConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void DuiSettingsLanguageIntegerStub::DuiSettingsLanguageIntegerDestructor()
{

}
QString DuiSettingsLanguageIntegerStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString DuiSettingsLanguageIntegerStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}

bool DuiSettingsLanguageIntegerStub::minValue(int &value) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int & >(value));
    stubMethodEntered("minValue", params);
    value = stubReturnValue<int>("minValueValue");
    return stubReturnValue<bool>("minValue");
}

bool DuiSettingsLanguageIntegerStub::maxValue(int &value) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int & >(value));
    stubMethodEntered("maxValue", params);
    value = stubReturnValue<int>("maxValueValue");
    return stubReturnValue<bool>("maxValue");
}

void DuiSettingsLanguageIntegerStub::setMinValue(int newMin)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newMin));
    stubMethodEntered("setMinValue", params);
}

void DuiSettingsLanguageIntegerStub::setMaxValue(int newMax)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newMax));
    stubMethodEntered("setMaxValue", params);
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageIntegerStub gDefaultDuiSettingsLanguageIntegerStub;
DuiSettingsLanguageIntegerStub *gDuiSettingsLanguageIntegerStub = &gDefaultDuiSettingsLanguageIntegerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageInteger::DuiSettingsLanguageInteger(const QString &key, const QString &title)
{
    gDuiSettingsLanguageIntegerStub->DuiSettingsLanguageIntegerConstructor(key, title);
}

DuiSettingsLanguageInteger::~DuiSettingsLanguageInteger()
{
    gDuiSettingsLanguageIntegerStub->DuiSettingsLanguageIntegerDestructor();
}

QString DuiSettingsLanguageInteger::key() const
{
    return gDuiSettingsLanguageIntegerStub->key();
}

QString DuiSettingsLanguageInteger::title() const
{
    return gDuiSettingsLanguageIntegerStub->title();
}

bool DuiSettingsLanguageInteger::minValue(int &value) const
{
    return gDuiSettingsLanguageIntegerStub->minValue(value);
}

bool DuiSettingsLanguageInteger::maxValue(int &value) const
{
    return gDuiSettingsLanguageIntegerStub->maxValue(value);
}

void DuiSettingsLanguageInteger::setMinValue(int newMin)
{
    gDuiSettingsLanguageIntegerStub->setMinValue(newMin);
}

void DuiSettingsLanguageInteger::setMaxValue(int newMax)
{
    gDuiSettingsLanguageIntegerStub->setMaxValue(newMax);
}


#endif
