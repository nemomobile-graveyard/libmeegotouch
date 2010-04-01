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

#include "duisettingslanguageselectioncontroller.h"

#include <DuiButton>
#include <DuiDataStore>

DuiSettingsLanguageSelectionController::DuiSettingsLanguageSelectionController(QObject *parent) :
    QObject(parent)
{
}

DuiSettingsLanguageSelectionController::~DuiSettingsLanguageSelectionController()
{
}

void DuiSettingsLanguageSelectionController::buttonClicked(DuiButton *button)
{
    DuiDataStore *dataStore = static_cast<DuiDataStore *>(button->property("dataStore").value<void *>());
    if (dataStore != NULL) {
        dataStore->createValue(button->property("key").toString(), button->property("value").toInt());
    }
}
