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

#include "duisettingslanguagesettingsfactory.h"
#include "duisettingslanguagewidget.h"
#include "duisettingslanguagenode.h"
#include "duisettingslanguageselectionfactory.h"
#include "duisettingslanguagetextfactory.h"
#include "duisettingslanguageintegerfactory.h"
#include "duisettingslanguagetext.h"
#include "duisettingslanguagesettings.h"
#include "duisettingslanguageselection.h"
#include "duisettingslanguageinteger.h"
#include "duitheme.h"
#include "duisettingslanguagesettingsfactorystyle.h"
#include "duisettingslanguagebooleanfactory.h"
#include "duisettingslanguageboolean.h"
#include "duisettingslanguagegroup.h"

#include <QGraphicsLinearLayout>
#include <DuiButton>
#include <DuiImageWidget>
#include <DuiContainer>
#include <DuiAction>
#include <DuiWidgetView>

DuiWidgetController *DuiSettingsLanguageSettingsFactory::createWidget(const DuiSettingsLanguageSettings &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    const DuiSettingsLanguageSettingsFactoryStyle *style =
        static_cast<const DuiSettingsLanguageSettingsFactoryStyle *>
        (DuiTheme::style("DuiSettingsLanguageSettingsFactoryStyle", "", "", "", Dui::Landscape, NULL));

    // Create content layout to layout content items in
    QGraphicsLinearLayout *contentLayout = new QGraphicsLinearLayout(Qt::Vertical);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    DuiWidgetController *widget = new DuiWidgetController;
    widget->setView(new DuiWidgetView(widget));
    widget->setObjectName("DuiSettingsLanguage");
    widget->setLayout(contentLayout);

    createChildren(*contentLayout, settingsItem, rootWidget, dataStore);

    DuiTheme::releaseStyle(style);

    return widget;
}

void DuiSettingsLanguageSettingsFactory::createChildren(QGraphicsLinearLayout &layout, const DuiSettingsLanguageNode &node,
        DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    // Go through all children of the item
    foreach(DuiSettingsLanguageNode * child, node.children()) {
        DuiWidgetController *childWidget = NULL;

        const DuiSettingsLanguageSelection *selectionNode;
        const DuiSettingsLanguageInteger *intNode;
        const DuiSettingsLanguageText *textNode;
        const DuiSettingsLanguageBoolean *booleanNode;

        if ((selectionNode = dynamic_cast<const DuiSettingsLanguageSelection *>(child))) {
            childWidget = DuiSettingsLanguageSelectionFactory::createWidget(*selectionNode, rootWidget, dataStore);
        } else if ((intNode = dynamic_cast<const DuiSettingsLanguageInteger *>(child))) {
            childWidget = DuiSettingsLanguageIntegerFactory::createWidget(*intNode, rootWidget, dataStore);
        } else if ((textNode = dynamic_cast<const DuiSettingsLanguageText *>(child))) {
            childWidget = DuiSettingsLanguageTextFactory::createWidget(*textNode, rootWidget, dataStore);
        } else if ((booleanNode = dynamic_cast<const DuiSettingsLanguageBoolean *>(child))) {
            childWidget = DuiSettingsLanguageBooleanFactory::createWidget(*booleanNode, rootWidget, dataStore);
        } else if ((dynamic_cast<const DuiSettingsLanguageGroup *>(child))) {
            DuiContainer *container = new DuiContainer;
            container->setObjectName("SettingsLanguageGroupContainer");
            QGraphicsLinearLayout *groupContentLayout = new QGraphicsLinearLayout(Qt::Vertical);
            groupContentLayout->setContentsMargins(0, 0, 0, 0);
            container->setLayout(groupContentLayout);

            createChildren(*groupContentLayout, *child, rootWidget, dataStore);
            childWidget = container;
        }

        if (childWidget != NULL) {
            layout.addItem(childWidget);
        }
    }
}
