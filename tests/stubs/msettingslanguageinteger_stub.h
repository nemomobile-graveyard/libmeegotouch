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

#ifndef MSETTINGSLANGUAGEINTEGER_STUB
#define MSETTINGSLANGUAGEINTEGER_STUB

#include "msettingslanguageinteger.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageIntegerStub : public StubBase
{
public:
    virtual void MSettingsLanguageIntegerConstructor(const QString &key, const QString &title);
    virtual void MSettingsLanguageIntegerDestructor();
    virtual QString key() const;
    virtual QString title() const;
    virtual bool minValue(int &value) const;
    virtual bool maxValue(int &value) const;
    virtual void setMinValue(int newMin);
    virtual void setMaxValue(int newMax);
};

// 2. IMPLEMENT STUB
void MSettingsLanguageIntegerStub::MSettingsLanguageIntegerConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void MSettingsLanguageIntegerStub::MSettingsLanguageIntegerDestructor()
{

}
QString MSettingsLanguageIntegerStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString MSettingsLanguageIntegerStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}

bool MSettingsLanguageIntegerStub::minValue(int &value) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int & >(value));
    stubMethodEntered("minValue", params);
    value = stubReturnValue<int>("minValueValue");
    return stubReturnValue<bool>("minValue");
}

bool MSettingsLanguageIntegerStub::maxValue(int &value) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int & >(value));
    stubMethodEntered("maxValue", params);
    value = stubReturnValue<int>("maxValueValue");
    return stubReturnValue<bool>("maxValue");
}

void MSettingsLanguageIntegerStub::setMinValue(int newMin)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newMin));
    stubMethodEntered("setMinValue", params);
}

void MSettingsLanguageIntegerStub::setMaxValue(int newMax)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newMax));
    stubMethodEntered("setMaxValue", params);
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageIntegerStub gDefaultMSettingsLanguageIntegerStub;
MSettingsLanguageIntegerStub *gMSettingsLanguageIntegerStub = &gDefaultMSettingsLanguageIntegerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageInteger::MSettingsLanguageInteger(const QString &key, const QString &title)
{
    gMSettingsLanguageIntegerStub->MSettingsLanguageIntegerConstructor(key, title);
}

MSettingsLanguageInteger::~MSettingsLanguageInteger()
{
    gMSettingsLanguageIntegerStub->MSettingsLanguageIntegerDestructor();
}

QString MSettingsLanguageInteger::key() const
{
    return gMSettingsLanguageIntegerStub->key();
}

QString MSettingsLanguageInteger::title() const
{
    return gMSettingsLanguageIntegerStub->title();
}

bool MSettingsLanguageInteger::minValue(int &value) const
{
    return gMSettingsLanguageIntegerStub->minValue(value);
}

bool MSettingsLanguageInteger::maxValue(int &value) const
{
    return gMSettingsLanguageIntegerStub->maxValue(value);
}

void MSettingsLanguageInteger::setMinValue(int newMin)
{
    gMSettingsLanguageIntegerStub->setMinValue(newMin);
}

void MSettingsLanguageInteger::setMaxValue(int newMax)
{
    gMSettingsLanguageIntegerStub->setMaxValue(newMax);
}


#endif
