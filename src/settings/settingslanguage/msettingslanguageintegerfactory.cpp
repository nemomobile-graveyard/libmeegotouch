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

#include "msettingslanguageinteger.h"
#include "msettingslanguageintegerfactory.h"
#include "msettingslanguageintegercontroller.h"

#include <MWidgetView>
#include <MLabel>
#include <MSlider>
#include <MDataStore>
#include <MLayout>
#include <MLinearLayoutPolicy>

MWidgetController *MSettingsLanguageIntegerFactory::createWidget(const MSettingsLanguageInteger &settingsInteger, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    Q_UNUSED(rootWidget);

    MWidgetController *parentWidget = new MWidgetController;
    parentWidget->setView(new MWidgetView(parentWidget));

    // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
    MSettingsLanguageIntegerController *intController = new MSettingsLanguageIntegerController(parentWidget);
    intController->setProperty("dataStore", qVariantFromValue(static_cast<void *>(dataStore)));
    intController->setProperty("key", settingsInteger.key());

    // Create a horizontal layout
    MLayout *layout = new MLayout();
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    parentWidget->setLayout(layout);

    // Create a label widget and put it into the layout
    MLabel *label = new MLabel(settingsInteger.title());
    label->setObjectName("SettingsLanguageLabel");
    policy->addItem(label, Qt::AlignCenter);

    MSlider *slider = new MSlider;
    slider->setObjectName("SettingsLanguageSlider");
    int minVal;
    if (settingsInteger.minValue(minVal)) {
        slider->setMinimum(minVal);
    }
    int maxVal;
    if (settingsInteger.maxValue(maxVal)) {
        slider->setMaximum(maxVal);
    }

    slider->connect(slider, SIGNAL(valueChanged(int)), intController, SLOT(changeValue(int)));
    policy->addItem(slider, Qt::AlignCenter);

    // Get the previously entered value if it exists
    if (dataStore != NULL) {
        slider->setValue(dataStore->value(settingsInteger.key()).toInt());
    }

    return parentWidget;
}
