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

#ifndef DUISETTINGSLANGUAGEWIDGET_STUB
#define DUISETTINGSLANGUAGEWIDGET_STUB

#include "duisettingslanguagewidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageWidgetStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageConstructor(QGraphicsItem *parent);
    virtual void DuiSettingsLanguageDestructor();
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageWidgetStub::DuiSettingsLanguageConstructor(QGraphicsItem *parent)
{
    Q_UNUSED(parent);

}
void DuiSettingsLanguageWidgetStub::DuiSettingsLanguageDestructor()
{

}


// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageWidgetStub gDefaultDuiSettingsLanguageWidgetStub;
DuiSettingsLanguageWidgetStub *gDuiSettingsLanguageWidgetStub = &gDefaultDuiSettingsLanguageWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageWidget::DuiSettingsLanguageWidget(QGraphicsItem *parent) : DuiWidgetController(parent)
{
    gDuiSettingsLanguageWidgetStub->DuiSettingsLanguageConstructor(parent);
}

DuiSettingsLanguageWidget::~DuiSettingsLanguageWidget()
{
    gDuiSettingsLanguageWidgetStub->DuiSettingsLanguageDestructor();
}


#endif
