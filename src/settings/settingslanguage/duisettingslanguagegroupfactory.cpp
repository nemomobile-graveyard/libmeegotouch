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

#include "duisettingslanguagesettingfactory.h"
#include "duisettingslanguagewidget.h"
#include "duisettingslanguagesettingcontroller.h"
#include "duisettingslanguagenode.h"
#include "duisettingslanguageselectionfactory.h"
#include "duisettingslanguageinfofactory.h"
#include "duisettingslanguagetextfactory.h"
#include "duisettingslanguageintegerfactory.h"
#include "duisettingslanguageinfo.h"
#include "duisettingslanguagetext.h"
#include "duisettingslanguagesetting.h"
#include "duisettingslanguageselection.h"
#include "duisettingslanguageinteger.h"
#include "duitheme.h"
#include "duisettingslanguagesettingfactorystyle.h"
#include "duisettingslanguagebooleanfactory.h"
#include "duisettingslanguageboolean.h"

#include <QGraphicsLinearLayout>
#include <DuiButton>
#include <DuiImage>
#include <DuiContainer>
#include <DuiAction>
#include <DuiActionProvider>
#include <DuiWidgetView>

DuiWidgetController *DuiSettingsLanguageSettingFactory::createWidget(const DuiSettingsLanguageSetting &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    DuiWidgetController *widget = NULL;
    DuiSettingsLanguageSettingController *settingsController = NULL;

    DuiAction *action = NULL;
    // Try to get an action corresponding to the content attribute
    if (!settingsItem.content().isEmpty()) {
        action = DuiActionProvider::getDefaultAction(QUrl(settingsItem.content()));
        if (action != NULL) {
            action->setVisible(false);
            action->connect(action, SIGNAL(triggered()), &rootWidget, SIGNAL(actionTriggered()));
        }
    }

    const DuiSettingsLanguageSettingFactoryStyle *style =
        static_cast<const DuiSettingsLanguageSettingFactoryStyle *>
        (DuiTheme::style("DuiSettingsLanguageSettingFactoryStyle", "", "", "", Dui::Landscape, NULL));

    // Create content layout to layout content items in
    QGraphicsLinearLayout *contentLayout = new QGraphicsLinearLayout(Qt::Vertical);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    if (settingsItem.group()) {
        // Create a container widget as a root widget
        DuiContainer *container = new DuiContainer;
        container->setObjectName("SettingsLanguageContainer");
        container->setTitle(settingsItem.title());

        if (action != NULL) {
            container->addAction(action);
            // Trigger the action by clicking
            container->connect(container, SIGNAL(headerClicked()), action, SLOT(trigger()));

            // Create content indicator
            container->setIconID(style->contentIndicator());
            QGraphicsLayout *mainLayout = container->layout();
            // TODO: remove this when there's support from DuiContainer for the styling
            if (mainLayout) {
                QGraphicsWidget *header = dynamic_cast<QGraphicsWidget *>(mainLayout->itemAt(0));
                if (header) {
                    QGraphicsLayout *headerLayout = header->layout();
                    if (headerLayout) {
                        headerLayout->setContentsMargins(style->contentIndicatorContainerMargin(), 0, 0, 0);
                    }
                }
            }
        }

        // Create central widget as parent widget for content items
        DuiWidget *centralWidget = new DuiWidget(container);
        container->setCentralWidget(centralWidget, true);
        centralWidget->setLayout(contentLayout);
        widget = container;
    } else {
        // Create content indicator widget and put it into compound layout with the content layout
        QGraphicsLinearLayout *itemLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        itemLayout->setContentsMargins(0, 0, 0, 0);

        if (action == NULL) {
            widget = new DuiWidgetController;
            widget->setView(new DuiWidgetView(widget));
        } else {
            widget = new DuiButton;
            // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
            settingsController = new DuiSettingsLanguageSettingController(widget);
            // Handle activity by press/release events
            widget->connect(widget, SIGNAL(pressed()), settingsController, SLOT(activate()));
            widget->connect(widget, SIGNAL(released()), settingsController, SLOT(deactivate()));

            // Create a new layout for placing the content indicator icon
            QGraphicsLinearLayout *indicatorLayout = new QGraphicsLinearLayout(Qt::Horizontal);
            indicatorLayout->setContentsMargins(style->contentIndicatorLeftMargin(), style->contentIndicatorTopMargin(), style->contentIndicatorRightMargin(), style->contentIndicatorBottomMargin());
            DuiImage *indicator = new DuiImage(widget);
            indicator->setImage(style->contentIndicator(), style->contentIndicatorSize());
            indicator->setObjectName("SettingsLanguageContentIndicator");
            indicatorLayout->addItem(indicator);
            itemLayout->addItem(indicatorLayout);
            // TODO: remove this when bug #133359 is fixed in duiimage
            indicator->setZoomFactor(1.0, 1.0);

            widget->addAction(action);
            // Trigger the action by clicking
            widget->connect(widget, SIGNAL(clicked()), action, SLOT(trigger()));
        }

        widget->setObjectName("DuiSettingsLanguage");
        itemLayout->addItem(contentLayout);
        widget->setLayout(itemLayout);
    }

    // Go through all children of the item
    foreach(DuiSettingsLanguageNode * child, settingsItem.children()) {
        DuiWidgetController *childWidget = NULL;

        if (child->nodeType() == Dui::DeclSettings::Selection) {
            childWidget = DuiSettingsLanguageSelectionFactory::createWidget(*static_cast<DuiSettingsLanguageSelection *>(child), rootWidget, dataStore);
        } else if (child->nodeType() == Dui::DeclSettings::Info) {
            childWidget = DuiSettingsLanguageInfoFactory::createWidget(*static_cast<DuiSettingsLanguageInfo *>(child), rootWidget, dataStore);
            // Monitor activity state changes of the parent widget
            if (settingsController != NULL) {
                childWidget->setActive(widget->isActive());
                settingsController->connect(settingsController, SIGNAL(activityChanged(bool)), childWidget, SLOT(setActive(bool)));
            }
        } else if (child->nodeType() == Dui::DeclSettings::Integer) {
            childWidget = DuiSettingsLanguageIntegerFactory::createWidget(*static_cast<DuiSettingsLanguageInteger *>(child), rootWidget, dataStore);
        } else if (child->nodeType() == Dui::DeclSettings::Text) {
            childWidget = DuiSettingsLanguageTextFactory::createWidget(*static_cast<DuiSettingsLanguageText *>(child), rootWidget, dataStore);
        } else if (child->nodeType() == Dui::DeclSettings::Boolean) {
            childWidget = DuiSettingsLanguageBooleanFactory::createWidget(*static_cast<DuiSettingsLanguageBoolean *>(child), rootWidget, dataStore);
        }

        if (childWidget != NULL) {
            contentLayout->addItem(childWidget);
        }
    }

    DuiTheme::releaseStyle(style);

    return widget;
}
