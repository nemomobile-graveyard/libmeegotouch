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

#include "msettingslanguagesettingfactory.h"
#include "msettingslanguagewidget.h"
#include "msettingslanguagesettingcontroller.h"
#include "msettingslanguagenode.h"
#include "msettingslanguageselectionfactory.h"
#include "msettingslanguageinfofactory.h"
#include "msettingslanguagetextfactory.h"
#include "msettingslanguageintegerfactory.h"
#include "msettingslanguageinfo.h"
#include "msettingslanguagetext.h"
#include "msettingslanguagesetting.h"
#include "msettingslanguageselection.h"
#include "msettingslanguageinteger.h"
#include "mtheme.h"
#include "msettingslanguagesettingfactorystyle.h"
#include "msettingslanguagebooleanfactory.h"
#include "msettingslanguageboolean.h"

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MImage>
#include <MContainer>
#include <MAction>
#include <MActionProvider>
#include <MWidgetView>

MWidgetController *MSettingsLanguageSettingFactory::createWidget(const MSettingsLanguageSetting &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    MWidgetController *widget = NULL;
    MSettingsLanguageSettingController *settingsController = NULL;

    MAction *action = NULL;
    // Try to get an action corresponding to the content attribute
    if (!settingsItem.content().isEmpty()) {
        action = MActionProvider::getDefaultAction(QUrl(settingsItem.content()));
        if (action != NULL) {
            action->setVisible(false);
            action->connect(action, SIGNAL(triggered()), &rootWidget, SIGNAL(actionTriggered()));
        }
    }

    const MSettingsLanguageSettingFactoryStyle *style =
        static_cast<const MSettingsLanguageSettingFactoryStyle *>
        (MTheme::style("MSettingsLanguageSettingFactoryStyle", "", "", "", M::Landscape, NULL));

    // Create content layout to layout content items in
    QGraphicsLinearLayout *contentLayout = new QGraphicsLinearLayout(Qt::Vertical);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    if (settingsItem.group()) {
        // Create a container widget as a root widget
        MContainer *container = new MContainer;
        container->setObjectName("SettingsLanguageContainer");
        container->setTitle(settingsItem.title());

        if (action != NULL) {
            container->addAction(action);
            // Trigger the action by clicking
            container->connect(container, SIGNAL(headerClicked()), action, SLOT(trigger()));

            // Create content indicator
            container->setIconID(style->contentIndicator());
            QGraphicsLayout *mainLayout = container->layout();
            // TODO: remove this when there's support from MContainer for the styling
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
        MWidget *centralWidget = new MWidget(container);
        container->setCentralWidget(centralWidget, true);
        centralWidget->setLayout(contentLayout);
        widget = container;
    } else {
        // Create content indicator widget and put it into compound layout with the content layout
        QGraphicsLinearLayout *itemLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        itemLayout->setContentsMargins(0, 0, 0, 0);

        if (action == NULL) {
            widget = new MWidgetController;
            widget->setView(new MWidgetView(widget));
        } else {
            widget = new MButton;
            // Make an interaction controller and make it a child of the widget object (so that it gets destroyed when appropriate)
            settingsController = new MSettingsLanguageSettingController(widget);
            // Handle activity by press/release events
            widget->connect(widget, SIGNAL(pressed()), settingsController, SLOT(activate()));
            widget->connect(widget, SIGNAL(released()), settingsController, SLOT(deactivate()));

            // Create a new layout for placing the content indicator icon
            QGraphicsLinearLayout *indicatorLayout = new QGraphicsLinearLayout(Qt::Horizontal);
            indicatorLayout->setContentsMargins(style->contentIndicatorLeftMargin(), style->contentIndicatorTopMargin(), style->contentIndicatorRightMargin(), style->contentIndicatorBottomMargin());
            MImage *indicator = new MImage(widget);
            indicator->setImage(style->contentIndicator(), style->contentIndicatorSize());
            indicator->setObjectName("SettingsLanguageContentIndicator");
            indicatorLayout->addItem(indicator);
            itemLayout->addItem(indicatorLayout);
            // TODO: remove this when bug #133359 is fixed in mimage
            indicator->setZoomFactor(1.0, 1.0);

            widget->addAction(action);
            // Trigger the action by clicking
            widget->connect(widget, SIGNAL(clicked()), action, SLOT(trigger()));
        }

        widget->setObjectName("MSettingsLanguage");
        itemLayout->addItem(contentLayout);
        widget->setLayout(itemLayout);
    }

    // Go through all children of the item
    foreach(MSettingsLanguageNode * child, settingsItem.children()) {
        MWidgetController *childWidget = NULL;

        if (child->nodeType() == M::DeclSettings::Selection) {
            childWidget = MSettingsLanguageSelectionFactory::createWidget(*static_cast<MSettingsLanguageSelection *>(child), rootWidget, dataStore);
        } else if (child->nodeType() == M::DeclSettings::Info) {
            childWidget = MSettingsLanguageInfoFactory::createWidget(*static_cast<MSettingsLanguageInfo *>(child), rootWidget, dataStore);
            // Monitor activity state changes of the parent widget
            if (settingsController != NULL) {
                childWidget->setActive(widget->isActive());
                settingsController->connect(settingsController, SIGNAL(activityChanged(bool)), childWidget, SLOT(setActive(bool)));
            }
        } else if (child->nodeType() == M::DeclSettings::Integer) {
            childWidget = MSettingsLanguageIntegerFactory::createWidget(*static_cast<MSettingsLanguageInteger *>(child), rootWidget, dataStore);
        } else if (child->nodeType() == M::DeclSettings::Text) {
            childWidget = MSettingsLanguageTextFactory::createWidget(*static_cast<MSettingsLanguageText *>(child), rootWidget, dataStore);
        } else if (child->nodeType() == M::DeclSettings::Boolean) {
            childWidget = MSettingsLanguageBooleanFactory::createWidget(*static_cast<MSettingsLanguageBoolean *>(child), rootWidget, dataStore);
        }

        if (childWidget != NULL) {
            contentLayout->addItem(childWidget);
        }
    }

    MTheme::releaseStyle(style);

    return widget;
}
