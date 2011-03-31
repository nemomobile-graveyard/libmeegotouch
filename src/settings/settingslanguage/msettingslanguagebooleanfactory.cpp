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
#include "msettingslanguagewidget.h"

#include <QGraphicsLinearLayout>
#include <MStylableWidget>
#include <MLabel>
#include <MButton>
#include <MDataStore>

MWidgetController *MSettingsLanguageBooleanFactory::createWidget(const MSettingsLanguageBoolean &settingsBool, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget)

    MStylableWidget *parentWidget = new MStylableWidget;
    parentWidget->setStyleName("MSettingsLanguageItem");

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageBooleanController *boolController = new MSettingsLanguageBooleanController(parentWidget);

    // Create a horizontal layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    MLabel *label = new MLabel(settingsBool.title());
    label->setStyleName("CommonSingleTitleInverted");
    layout->addItem(label);

    // Create a button
    MButton *button = new MButton(parentWidget);
    button->setViewType(MButton::switchType);
    button->setStyleName("CommonSingleButtonInverted");
    button->setCheckable(true);
    button->connect(button, SIGNAL(toggled(bool)), boolController, SLOT(buttonToggled(bool)));

    // Get the previously selected value if it exists
    bool selectedValue = false;
    if (dataStore != NULL) {
        selectedValue = dataStore->value(settingsBool.key()).toBool();
    }
    button->setChecked(selectedValue);
    button->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    button->setProperty("key", settingsBool.key());
    layout->addItem(button);

    return parentWidget;
}
