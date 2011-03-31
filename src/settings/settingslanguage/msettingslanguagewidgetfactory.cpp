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

#include "msettingslanguagewidgetfactory.h"
#include "msettingslanguagebinary.h"
#include "msettingslanguagewidget.h"
#include "msettingslanguagesettings.h"
#include "msettingslanguagesettingsfactory.h"

#include <QGraphicsLinearLayout>

MSettingsLanguageWidget *MSettingsLanguageWidgetFactory::createWidget(const MSettingsLanguageBinary &settingsBinary, MDataStore *dataStore)
{
    MSettingsLanguageWidget *widget = new MSettingsLanguageWidget;
    widget->setStyleName("MSettingsLanguage");

    // Create a central layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setLayout(layout);

    foreach(MSettingsLanguageNode *settingsNode, settingsBinary.children()) {
        MSettingsLanguageSettings *settingsItem = dynamic_cast<MSettingsLanguageSettings *>(settingsNode);
        if (settingsItem != NULL) {
            MWidget *itemWidget = MSettingsLanguageSettingsFactory::createWidget(*settingsItem, *widget, dataStore);
            itemWidget->setParentItem(widget);
            layout->addItem(itemWidget);
        }
    }

    return widget;
}
