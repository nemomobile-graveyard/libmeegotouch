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

#ifndef DUICOLLATOR_STUB
#define DUICOLLATOR_STUB

#include "duicollator.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiCollatorStub : public StubBase
{
public:
    virtual void DuiCollatorConstructor();
    virtual void DuiCollatorConstructor(const DuiLocale &locale);
    virtual void DuiCollatorConstructor(const DuiCollator &other);
    virtual void DuiCollatorDestructor();
    virtual bool operator()(const QString &s1, const QString &s2) const;
    virtual DuiLocale::Comparison compare(const QString &first, const QString &second);
    virtual DuiLocale::Comparison compare(DuiLocale &locale, const QString &first, const QString &second);
};

// 2. IMPLEMENT STUB
void DuiCollatorStub::DuiCollatorConstructor()
{

}
void DuiCollatorStub::DuiCollatorConstructor(const DuiLocale &locale)
{
    Q_UNUSED(locale);

}
void DuiCollatorStub::DuiCollatorConstructor(const DuiCollator &other)
{
    Q_UNUSED(other);

}
void DuiCollatorStub::DuiCollatorDestructor()
{

}
bool DuiCollatorStub::operator()(const QString &s1, const QString &s2) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(s1));
    params.append(new Parameter<const QString & >(s2));
    stubMethodEntered("operator()", params);
    return stubReturnValue<bool>("operator()");
}

DuiLocale::Comparison DuiCollatorStub::compare(const QString &first, const QString &second)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(first));
    params.append(new Parameter<const QString & >(second));
    stubMethodEntered("compare", params);
    return stubReturnValue<DuiLocale::Comparison>("compare");
}

DuiLocale::Comparison DuiCollatorStub::compare(DuiLocale &locale, const QString &first, const QString &second)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale & >(locale));
    params.append(new Parameter<const QString & >(first));
    params.append(new Parameter<const QString & >(second));
    stubMethodEntered("compare", params);
    return stubReturnValue<DuiLocale::Comparison>("compare");
}



// 3. CREATE A STUB INSTANCE
DuiCollatorStub gDefaultDuiCollatorStub;
DuiCollatorStub *gDuiCollatorStub = &gDefaultDuiCollatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiCollator::DuiCollator() :
    d_ptr(0)
{
    gDuiCollatorStub->DuiCollatorConstructor();
}

DuiCollator::DuiCollator(const DuiLocale &locale) :
    d_ptr(0)
{
    gDuiCollatorStub->DuiCollatorConstructor(locale);
}

DuiCollator::DuiCollator(const DuiCollator &other) :
    d_ptr(0)
{
    gDuiCollatorStub->DuiCollatorConstructor(other);
}

DuiCollator::~DuiCollator()
{
    gDuiCollatorStub->DuiCollatorDestructor();
}

bool DuiCollator::operator()(const QString &s1, const QString &s2) const
{
    return gDuiCollatorStub->operator()(s1, s2);
}

DuiLocale::Comparison DuiCollator::compare(const QString &first, const QString &second)
{
    return gDuiCollatorStub->compare(first, second);
}

DuiLocale::Comparison DuiCollator::compare(DuiLocale &locale, const QString &first, const QString &second)
{
    return gDuiCollatorStub->compare(locale, first, second);
}


#endif
