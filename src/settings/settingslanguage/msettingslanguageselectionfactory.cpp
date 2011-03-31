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

#include "msettingslanguageselectionfactory.h"
#include "msettingslanguageselectioncontroller.h"
#include "msettingslanguageselection.h"
#include "msettingslanguageoption.h"
#include "msettingslanguagewidget.h"

#include <MStylableWidget>
#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButtonGroup>
#include <MButton>
#include <MLabel>
#include <MDataStore>

MWidgetController *MSettingsLanguageSelectionFactory::createWidget(const MSettingsLanguageSelection &settingsSelection,
        MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget)

    MStylableWidget *parentWidget = new MStylableWidget;
    parentWidget->setStyleName("MSettingsLanguageItem");

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageSelectionController *selectionController = new MSettingsLanguageSelectionController(parentWidget);

    // Create a vertical layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    MLabel *label = new MLabel(settingsSelection.title());
    label->setStyleName("CommonSingleTitleInverted");
    layout->addItem(label);

    // Create a horizontal layout
    MLayout *buttonLayout = new MLayout;
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(buttonLayout, Qt::Horizontal);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    policy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    layout->addItem(buttonLayout);

    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    // Create a button group
    MButtonGroup *buttonGroup = new MButtonGroup(parentWidget);
    buttonGroup->connect(buttonGroup, SIGNAL(buttonClicked(MButton *)), selectionController, SLOT(buttonClicked(MButton *)));

    // Get the previously selected value or use 0 if it doesn't exist
    int selectedValue = dataStore != NULL ? dataStore->value(settingsSelection.key()).toInt() : 0;
    foreach(const MSettingsLanguageOption * value, settingsSelection.options()) {
        // Create a radio button for the enum value
        MButton *button = new MButton(parentWidget);
        button->setViewType(MButton::groupType);
        button->setSizePolicy(sizePolicy);
        button->setText(value->title());
        button->setCheckable(true);
        button->setChecked(value->value() == selectedValue);
        button->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
        button->setProperty("key", settingsSelection.key());
        button->setProperty("value", value->value());
        policy->addItem(button);

        // Add the button to a button group
        buttonGroup->addButton(button);
    }

    return parentWidget;
}
