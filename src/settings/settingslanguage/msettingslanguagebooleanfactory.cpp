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

#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguagebooleancontroller.h"
#include "msettingslanguageboolean.h"

#include <MWidgetView>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MDataStore>

MWidgetController *MSettingsLanguageBooleanFactory::createWidget(const MSettingsLanguageBoolean &settingsBool, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget)

    MWidgetController *parentWidget = new MWidgetController;
    parentWidget->setView(new MWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageBooleanController *boolController = new MSettingsLanguageBooleanController(parentWidget);

    // Create a horizontal layout
    MLayout *layout = new MLayout();
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a button
    MButton *button = new MButton(parentWidget);
    button->setCheckable(true);
    button->setViewType(MButton::toggleType);
    button->connect(button, SIGNAL(toggled(bool)), boolController, SLOT(buttonToggled(bool)));

    // Get the previously selected value if it exists
    bool selectedValue = false;
    if (dataStore != NULL) {
        selectedValue = dataStore->value(settingsBool.key()).toBool();
    }
    button->setChecked(selectedValue);

    button->setText(settingsBool.title());
    button->setObjectName("SettingsLanguageBooleanValueButton");
    button->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    button->setProperty("key", settingsBool.key());
    policy->addItem(button, Qt::AlignCenter);

    return parentWidget;
}
