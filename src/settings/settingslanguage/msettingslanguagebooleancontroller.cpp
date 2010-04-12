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

#include "msettingslanguagebooleancontroller.h"

#include <MDataStore>
#include <MButton>

MSettingsLanguageBooleanController::MSettingsLanguageBooleanController(QObject *parent) :
    QObject(parent)
{
}

MSettingsLanguageBooleanController::~MSettingsLanguageBooleanController()
{
}

void MSettingsLanguageBooleanController::buttonToggled(bool checked)
{
    // get button from signal sender
    MButton *button = static_cast<MButton *>(sender());

    if (button != NULL) {
        MDataStore *dataStore = static_cast<MDataStore *>(button->property("dataStore").value<void *>());
        if (dataStore != NULL) {
            dataStore->createValue(button->property("key").toString(), checked);
        }
    }
}
