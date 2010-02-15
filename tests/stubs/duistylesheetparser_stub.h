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

#ifndef DUISTYLESHEETPARSER_STUB
#define DUISTYLESHEETPARSER_STUB

#include "duistylesheetparser.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiStyleSheetParserStub : public StubBase
{
public:
    virtual void DuiStyleSheetParserConstructor();
    virtual void DuiStyleSheetParserDestructor();
    virtual bool load(const QString &filename);
    virtual QList<DuiStyleSheetSelector *> & selectors() const;
    DuiStyleSheetParserPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiStyleSheetParserStub::DuiStyleSheetParserConstructor()
{

}
void DuiStyleSheetParserStub::DuiStyleSheetParserDestructor()
{

}
bool DuiStyleSheetParserStub::load(const QString &filename)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    stubMethodEntered("load", params);
    return stubReturnValue<bool>("load");
}

QList<DuiStyleSheetSelector *> & DuiStyleSheetParserStub::selectors() const
{
    stubMethodEntered("selectors");
    return stubReturnValue< QList<DuiStyleSheetSelector *> >("selectors");
}



// 3. CREATE A STUB INSTANCE
DuiStyleSheetParserStub gDefaultDuiStyleSheetParserStub;
DuiStyleSheetParserStub *gDuiStyleSheetParserStub = &gDefaultDuiStyleSheetParserStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiStyleSheetParser::DuiStyleSheetParser()
{
    gDuiStyleSheetParserStub->DuiStyleSheetParserConstructor();
}

DuiStyleSheetParser::~DuiStyleSheetParser()
{
    gDuiStyleSheetParserStub->DuiStyleSheetParserDestructor();
}

bool DuiStyleSheetParser::load(const QString &filename)
{
    return gDuiStyleSheetParserStub->load(filename);
}

QList<DuiStyleSheetSelector *> & DuiStyleSheetParser::selectors() const
{
    return gDuiStyleSheetParserStub->selectors();
}


#endif
