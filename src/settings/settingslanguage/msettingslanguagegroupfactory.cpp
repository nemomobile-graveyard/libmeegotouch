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

#include "msettingslanguagegroupfactory.h"
#include "msettingslanguagewidget.h"
#include "msettingslanguagenode.h"
#include "msettingslanguageselectionfactory.h"
#include "msettingslanguageintegerfactory.h"
#include "msettingslanguagetextfactory.h"
#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguageselection.h"
#include "msettingslanguageinteger.h"
#include "msettingslanguagetext.h"
#include "msettingslanguageboolean.h"
#include "msettingslanguagegroup.h"
#include "mtheme.h"

#include <QGraphicsLinearLayout>
#include <MContainer>
#include <MStylableWidget>

MWidgetController *MSettingsLanguageGroupFactory::createWidget(const MSettingsLanguageGroup &settingsGroup, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    // Create a container widget as a root widget
    MContainer *container = new MContainer;
    container->setStyleName("CommonContainerInverted");
    container->setTitle(settingsGroup.title());

    // Create a child with a vertical layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    MStylableWidget *childWidget = new MStylableWidget;
    childWidget->setLayout(layout);
    container->setCentralWidget(childWidget, true);

    // Go through all children of the item
    foreach(MSettingsLanguageNode *child, settingsGroup.children()) {
        MWidgetController *childWidget = NULL;

        const MSettingsLanguageSelection *selectionNode;
        const MSettingsLanguageInteger *intNode;
        const MSettingsLanguageText *textNode;
        const MSettingsLanguageBoolean *booleanNode;

        if ((selectionNode = dynamic_cast<const MSettingsLanguageSelection *>(child))) {
            childWidget = MSettingsLanguageSelectionFactory::createWidget(*selectionNode, rootWidget, dataStore);
        } else if ((intNode = dynamic_cast<const MSettingsLanguageInteger *>(child))) {
            childWidget = MSettingsLanguageIntegerFactory::createWidget(*intNode, rootWidget, dataStore);
        } else if ((textNode = dynamic_cast<const MSettingsLanguageText *>(child))) {
            childWidget = MSettingsLanguageTextFactory::createWidget(*textNode, rootWidget, dataStore);
        } else if ((booleanNode = dynamic_cast<const MSettingsLanguageBoolean *>(child))) {
            childWidget = MSettingsLanguageBooleanFactory::createWidget(*booleanNode, rootWidget, dataStore);
        }

        if (childWidget != NULL) {
            layout->addItem(childWidget);
        }
    }

    return container;
}
