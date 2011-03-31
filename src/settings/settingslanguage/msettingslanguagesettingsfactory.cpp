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

#include "msettingslanguagesettingsfactory.h"
#include "msettingslanguagewidget.h"
#include "msettingslanguagenode.h"
#include "msettingslanguageselectionfactory.h"
#include "msettingslanguagetextfactory.h"
#include "msettingslanguageintegerfactory.h"
#include "msettingslanguagetext.h"
#include "msettingslanguagesettings.h"
#include "msettingslanguageselection.h"
#include "msettingslanguageinteger.h"
#include "mtheme.h"
#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguageboolean.h"
#include "msettingslanguagegroupfactory.h"
#include "msettingslanguagegroup.h"

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MImageWidget>
#include <MContainer>
#include <MAction>
#include <MStylableWidget>

MWidgetController *MSettingsLanguageSettingsFactory::createWidget(const MSettingsLanguageSettings &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    // Create content layout to layout content items in
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    MStylableWidget *widget = new MStylableWidget;
    widget->setStyleName("MSettingsLanguage");
    widget->setLayout(layout);

    createChildren(layout, settingsItem, rootWidget, dataStore);
    layout->addStretch();

    return widget;
}

void MSettingsLanguageSettingsFactory::createChildren(QGraphicsLinearLayout *layout, const MSettingsLanguageNode &node,
        MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    // Go through all children of the item
    foreach(MSettingsLanguageNode * child, node.children()) {
        MWidgetController *childWidget = NULL;

        const MSettingsLanguageSelection *selectionNode;
        const MSettingsLanguageInteger *intNode;
        const MSettingsLanguageText *textNode;
        const MSettingsLanguageBoolean *booleanNode;
        const MSettingsLanguageGroup *groupNode;

        if ((selectionNode = dynamic_cast<const MSettingsLanguageSelection *>(child))) {
            childWidget = MSettingsLanguageSelectionFactory::createWidget(*selectionNode, rootWidget, dataStore);
        } else if ((intNode = dynamic_cast<const MSettingsLanguageInteger *>(child))) {
            childWidget = MSettingsLanguageIntegerFactory::createWidget(*intNode, rootWidget, dataStore);
        } else if ((textNode = dynamic_cast<const MSettingsLanguageText *>(child))) {
            childWidget = MSettingsLanguageTextFactory::createWidget(*textNode, rootWidget, dataStore);
        } else if ((booleanNode = dynamic_cast<const MSettingsLanguageBoolean *>(child))) {
            childWidget = MSettingsLanguageBooleanFactory::createWidget(*booleanNode, rootWidget, dataStore);
        } else if ((groupNode = dynamic_cast<const MSettingsLanguageGroup *>(child))) {
            childWidget = MSettingsLanguageGroupFactory::createWidget(*groupNode, rootWidget, dataStore);
        }

        if (childWidget != NULL) {
            layout->addItem(childWidget);
        }
    }
}
