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

#include "msettingslanguagetextfactory.h"
#include "msettingslanguagetextcontroller.h"
#include "msettingslanguagebinary.h"
#include "msettingslanguagetext.h"

#include <MWidgetView>
#include <MLabel>
#include <MTextEdit>
#include <MDataStore>
#include <MLayout>
#include <MLinearLayoutPolicy>

MWidgetController *MSettingsLanguageTextFactory::createWidget(const MSettingsLanguageText &settingsText, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget);

    MWidgetController *parentWidget = new MWidgetController;
    parentWidget->setView(new MWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageTextController *textController = new MSettingsLanguageTextController(parentWidget);

    // Create a horizontal layout
    MLayout *layout = new MLayout();
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    MLabel *label = new MLabel(settingsText.title());
    label->setObjectName("SettingsLanguageLabel");
    policy->addItem(label, Qt::AlignCenter);

    // Create a text edit widget and put it into the layout
    MTextEdit *textEdit = new MTextEdit;
    textEdit->setObjectName("SettingsLanguageTextTextEdit");
    textEdit->connect(textEdit, SIGNAL(lostFocus(Qt::FocusReason)), textController, SLOT(textEditLostFocus(Qt::FocusReason)));
    textEdit->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    textEdit->setProperty("key", settingsText.key());
    policy->addItem(textEdit, Qt::AlignCenter);

    // Get the previously entered value if it exists
    if (dataStore != NULL) {
        textEdit->setText(dataStore->value(settingsText.key()).toString());
    }

    return parentWidget;
}
