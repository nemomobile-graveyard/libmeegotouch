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

#ifndef DUISTYLESHEET_STUB
#define DUISTYLESHEET_STUB

#include "duistylesheet.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiStyleSheetStub : public StubBase
{
public:
    virtual void DuiStyleSheetConstructor();
    virtual void DuiStyleSheetDestructor();
    virtual DuiStyleData *createStyleDataObject(const QObject *object, const DuiStyle::Orientation &orientation, const QString &mode, const DuiStyleDescription &description);
    virtual void findAttributeValues(const QObject *object, const QVector<QString> attributes, QVector<DuiStyleAttribute *> &out);
};

// 2. IMPLEMENT STUB
void DuiStyleSheetStub::DuiStyleSheetConstructor()
{

}
void DuiStyleSheetStub::DuiStyleSheetDestructor()
{

}
DuiStyleData *DuiStyleSheetStub::createStyleDataObject(const QObject *object, const DuiStyle::Orientation &orientation, const QString &mode, const DuiStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QObject * >(object));
    params.append(new Parameter<const DuiStyle::Orientation & >(orientation));
    params.append(new Parameter<const QString & >(mode));
    params.append(new Parameter<const DuiStyleDescription & >(description));
    stubMethodEntered("createStyleDataObject", params);
    return stubReturnValue<DuiStyleData *>("createStyleDataObject");
}

void DuiStyleSheetStub::findAttributeValues(const QObject *object, const QVector<QString> attributes, QVector<DuiStyleAttribute *> &out)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QObject * >(object));
    params.append(new Parameter<const QVector<QString> >(attributes));
    params.append(new Parameter<QVector<DuiStyleAttribute *> & >(out));
    stubMethodEntered("findAttributeValues", params);
}



// 3. CREATE A STUB INSTANCE
DuiStyleSheetStub gDefaultDuiStyleSheetStub;
DuiStyleSheetStub *gDuiStyleSheetStub = &gDefaultDuiStyleSheetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiStyleSheet::DuiStyleSheet()
{
    gDuiStyleSheetStub->DuiStyleSheetConstructor();
}

DuiStyleSheet::~DuiStyleSheet()
{
    gDuiStyleSheetStub->DuiStyleSheetDestructor();
}

DuiStyleData *DuiStyleSheet::createStyleDataObject(const QObject *object, const DuiStyle::Orientation &orientation, const QString &mode, const DuiStyleDescription &description)
{
    return gDuiStyleSheetStub->createStyleDataObject(object, orientation, mode, description);
}

void DuiStyleSheet::findAttributeValues(const QObject *object, const QVector<QString> attributes, QVector<DuiStyleAttribute *> &out)
{
    gDuiStyleSheetStub->findAttributeValues(object, attributes, out);
}


#endif
