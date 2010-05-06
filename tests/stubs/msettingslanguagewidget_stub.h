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

#ifndef MSETTINGSLANGUAGEWIDGET_STUB
#define MSETTINGSLANGUAGEWIDGET_STUB

#include "msettingslanguagewidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageWidgetStub : public StubBase
{
public:
    virtual void MSettingsLanguageConstructor(QGraphicsItem *parent);
    virtual void MSettingsLanguageDestructor();
};

// 2. IMPLEMENT STUB
void MSettingsLanguageWidgetStub::MSettingsLanguageConstructor(QGraphicsItem *parent)
{
    Q_UNUSED(parent);

}
void MSettingsLanguageWidgetStub::MSettingsLanguageDestructor()
{

}


// 3. CREATE A STUB INSTANCE
MSettingsLanguageWidgetStub gDefaultMSettingsLanguageWidgetStub;
MSettingsLanguageWidgetStub *gMSettingsLanguageWidgetStub = &gDefaultMSettingsLanguageWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageWidget::MSettingsLanguageWidget(QGraphicsItem *parent) : MWidgetController(parent)
{
    gMSettingsLanguageWidgetStub->MSettingsLanguageConstructor(parent);
}

MSettingsLanguageWidget::~MSettingsLanguageWidget()
{
    gMSettingsLanguageWidgetStub->MSettingsLanguageDestructor();
}


#endif
