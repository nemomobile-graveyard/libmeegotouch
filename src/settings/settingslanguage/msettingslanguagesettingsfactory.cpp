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
#include "msettingslanguagesettingsfactorystyle.h"
#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguageboolean.h"
#include "msettingslanguagegroup.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MImageWidget>
#include <MContainer>
#include <MAction>
#include <MWidgetView>

MWidgetController *MSettingsLanguageSettingsFactory::createWidget(const MSettingsLanguageSettings &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    const MSettingsLanguageSettingsFactoryStyle *style =
        static_cast<const MSettingsLanguageSettingsFactoryStyle *>
        (MTheme::style("MSettingsLanguageSettingsFactoryStyle", "", "", "", M::Landscape, NULL));

    // Create content layout to layout content items in
    MLayout *contentLayout = new MLayout();
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(contentLayout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);

    MWidgetController *widget = new MWidgetController;
    widget->setView(new MWidgetView(widget));
    widget->setObjectName("MSettingsLanguage");
    widget->setLayout(contentLayout);

    createChildren(*policy, settingsItem, rootWidget, dataStore);

    MTheme::releaseStyle(style);

    return widget;
}

void MSettingsLanguageSettingsFactory::createChildren(MLinearLayoutPolicy &layoutPolicy, const MSettingsLanguageNode &node,
        MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    // Go through all children of the item
    foreach(MSettingsLanguageNode * child, node.children()) {
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
        } else if ((dynamic_cast<const MSettingsLanguageGroup *>(child))) {
            MContainer *container = new MContainer;
            container->setObjectName("SettingsLanguageGroupContainer");
            MLayout *groupContentLayout = new MLayout();
            MLinearLayoutPolicy *contentGroupPolicy = new MLinearLayoutPolicy(groupContentLayout, Qt::Vertical);
            contentGroupPolicy->setContentsMargins(0, 0, 0, 0);
            container->setLayout(groupContentLayout);

            createChildren(*contentGroupPolicy, *child, rootWidget, dataStore);
            childWidget = container;
        }

        if (childWidget != NULL) {
            layoutPolicy.addItem(childWidget);
        }
    }
}
