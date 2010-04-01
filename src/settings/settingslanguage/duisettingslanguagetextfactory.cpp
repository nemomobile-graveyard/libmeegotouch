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

#include "duisettingslanguagetextfactory.h"
#include "duisettingslanguagetextcontroller.h"
#include "duisettingslanguagebinary.h"
#include "duisettingslanguagetext.h"

#include <DuiWidgetView>
#include <DuiLabel>
#include <DuiTextEdit>
#include <DuiDataStore>
#include <QGraphicsLinearLayout>

DuiWidgetController *DuiSettingsLanguageTextFactory::createWidget(const DuiSettingsLanguageText &settingsText, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    Q_UNUSED(rootWidget);

    DuiWidgetController *parentWidget = new DuiWidgetController;
    parentWidget->setView(new DuiWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    DuiSettingsLanguageTextController *textController = new DuiSettingsLanguageTextController(parentWidget);

    // Create a horizontal layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    DuiLabel *label = new DuiLabel(settingsText.title());
    label->setObjectName("SettingsLanguageLabel");
    layout->addItem(label);

    // Create a text edit widget and put it into the layout
    DuiTextEdit *textEdit = new DuiTextEdit;
    textEdit->setObjectName("SettingsLanguageTextTextEdit");
    textEdit->connect(textEdit, SIGNAL(lostFocus(Qt::FocusReason)), textController, SLOT(textEditLostFocus(Qt::FocusReason)));
    textEdit->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    textEdit->setProperty("key", settingsText.key());
    layout->addItem(textEdit);

    // Get the previously entered value if it exists
    if (dataStore != NULL) {
        textEdit->setText(dataStore->value(settingsText.key()).toString());
    }

    return parentWidget;
}
