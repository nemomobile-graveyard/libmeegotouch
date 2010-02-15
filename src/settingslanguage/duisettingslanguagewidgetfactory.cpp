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

#include "duisettingslanguagewidgetfactory.h"
#include "duisettingslanguagebinary.h"
#include "duisettingslanguagewidget.h"
#include "duisettingslanguagesettings.h"

#include "duisettingslanguagesettingsfactory.h"

#include <QGraphicsLinearLayout>

DuiSettingsLanguageWidget *DuiSettingsLanguageWidgetFactory::createWidget(const DuiSettingsLanguageBinary &settingsBinary, DuiDataStore *dataStore)
{
    DuiSettingsLanguageWidget *widget = new DuiSettingsLanguageWidget;
    widget->setObjectName("DuiSettingsLanguage");

    // Create a central layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    // Set layout parameters
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setLayout(layout);

    foreach(DuiSettingsLanguageNode * settingsNode, settingsBinary.children()) {
        DuiSettingsLanguageSettings *settingsItem = dynamic_cast<DuiSettingsLanguageSettings *>(settingsNode);
        if (settingsItem != NULL) {
            DuiWidget *itemWidget = DuiSettingsLanguageSettingsFactory::createWidget(*settingsItem, *widget, dataStore);
            itemWidget->setParentItem(widget);
            layout->addItem(itemWidget);
        }
    }

    return widget;
}
