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

#ifndef MSTYLESHEETPARSER_STUB
#define MSTYLESHEETPARSER_STUB

#include "mstylesheetparser.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MStyleSheetParserStub : public StubBase
{
public:
    virtual void MStyleSheetParserConstructor();
    virtual void MStyleSheetParserDestructor();
    virtual bool load(const QString &filename);
    virtual QList<MStyleSheetSelector *> & selectors() const;
    MStyleSheetParserPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MStyleSheetParserStub::MStyleSheetParserConstructor()
{

}
void MStyleSheetParserStub::MStyleSheetParserDestructor()
{

}
bool MStyleSheetParserStub::load(const QString &filename)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    stubMethodEntered("load", params);
    return stubReturnValue<bool>("load");
}

QList<MStyleSheetSelector *> & MStyleSheetParserStub::selectors() const
{
    stubMethodEntered("selectors");
    return stubReturnValue< QList<MStyleSheetSelector *> >("selectors");
}



// 3. CREATE A STUB INSTANCE
MStyleSheetParserStub gDefaultMStyleSheetParserStub;
MStyleSheetParserStub *gMStyleSheetParserStub = &gDefaultMStyleSheetParserStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MStyleSheetParser::MStyleSheetParser()
{
    gMStyleSheetParserStub->MStyleSheetParserConstructor();
}

MStyleSheetParser::~MStyleSheetParser()
{
    gMStyleSheetParserStub->MStyleSheetParserDestructor();
}

bool MStyleSheetParser::load(const QString &filename)
{
    return gMStyleSheetParserStub->load(filename);
}

QList<MStyleSheetSelector *> & MStyleSheetParser::selectors() const
{
    return gMStyleSheetParserStub->selectors();
}


#endif
