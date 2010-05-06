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

#ifndef MSTYLESHEET_STUB
#define MSTYLESHEET_STUB

#include "mstylesheet.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MStyleSheetStub : public StubBase
{
public:
    virtual void MStyleSheetConstructor();
    virtual void MStyleSheetDestructor();
    virtual MStyleData *createStyleDataObject(const QObject *object, const MStyle::Orientation &orientation, const QString &mode, const MStyleDescription &description);
    virtual void findAttributeValues(const QObject *object, const QVector<QString> attributes, QVector<MStyleAttribute *> &out);
};

// 2. IMPLEMENT STUB
void MStyleSheetStub::MStyleSheetConstructor()
{

}
void MStyleSheetStub::MStyleSheetDestructor()
{

}
MStyleData *MStyleSheetStub::createStyleDataObject(const QObject *object, const MStyle::Orientation &orientation, const QString &mode, const MStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QObject * >(object));
    params.append(new Parameter<const MStyle::Orientation & >(orientation));
    params.append(new Parameter<const QString & >(mode));
    params.append(new Parameter<const MStyleDescription & >(description));
    stubMethodEntered("createStyleDataObject", params);
    return stubReturnValue<MStyleData *>("createStyleDataObject");
}

void MStyleSheetStub::findAttributeValues(const QObject *object, const QVector<QString> attributes, QVector<MStyleAttribute *> &out)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QObject * >(object));
    params.append(new Parameter<const QVector<QString> >(attributes));
    params.append(new Parameter<QVector<MStyleAttribute *> & >(out));
    stubMethodEntered("findAttributeValues", params);
}



// 3. CREATE A STUB INSTANCE
MStyleSheetStub gDefaultMStyleSheetStub;
MStyleSheetStub *gMStyleSheetStub = &gDefaultMStyleSheetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MStyleSheet::MStyleSheet()
{
    gMStyleSheetStub->MStyleSheetConstructor();
}

MStyleSheet::~MStyleSheet()
{
    gMStyleSheetStub->MStyleSheetDestructor();
}

MStyleData *MStyleSheet::createStyleDataObject(const QObject *object, const MStyle::Orientation &orientation, const QString &mode, const MStyleDescription &description)
{
    return gMStyleSheetStub->createStyleDataObject(object, orientation, mode, description);
}

void MStyleSheet::findAttributeValues(const QObject *object, const QVector<QString> attributes, QVector<MStyleAttribute *> &out)
{
    gMStyleSheetStub->findAttributeValues(object, attributes, out);
}


#endif
