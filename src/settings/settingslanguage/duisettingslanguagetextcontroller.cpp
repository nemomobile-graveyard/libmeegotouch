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

#include "duisettingslanguagetextcontroller.h"

#include <DuiTextEdit>
#include <DuiDataStore>

DuiSettingsLanguageTextController::DuiSettingsLanguageTextController(QObject *parent) :
    QObject(parent)
{
}

DuiSettingsLanguageTextController::~DuiSettingsLanguageTextController()
{
}

void DuiSettingsLanguageTextController::textEditLostFocus(Qt::FocusReason)
{
    // get text edit from signal sender
    DuiTextEdit *textEdit = static_cast<DuiTextEdit *>(sender());

    if (textEdit != NULL) {
        DuiDataStore *dataStore = static_cast<DuiDataStore *>(textEdit->property("dataStore").value<void *>());
        if (dataStore != NULL) {
            dataStore->createValue(textEdit->property("key").toString(), textEdit->text());
        }
    }
}
