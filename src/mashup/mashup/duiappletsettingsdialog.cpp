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

#include "duiappletsettingsdialog.h"

#include "duidialog.h"
#include "duiwidgetcontroller.h"
#include "duiwidgetview.h"
#include "duisettingslanguagewidgetfactory.h"
#include "duisettingslanguagewidget.h"
#include "duilocale.h"
#include "duicontainer.h"
#include <QGraphicsLinearLayout>
#include "duidebug.h"

class QGraphicsLayoutItem;
class QGraphicsItem;
class DuiSettingsLanguageBinary;
class DuiDataStore;

/*!
 * Creates a container for showing applet instance/global settings
 * Also places the widgets into the container
 *
 * \param parent The parent of the container
 * \param title The title of the container
 * \param settingsBinaryList a list of applet settings binaries
 * \param settingsDataStore the settings data store for the settings
 */
static QGraphicsLayoutItem *createAppletSettingsWidgets(QGraphicsItem *parent,
							const QString &title,
							const DuiSettingsLanguageBinary &settingsBinary,
							DuiDataStore *settingsDataStore)
{
    DuiContainer *container = new DuiContainer(parent);
    container->setTitle(title);
    DuiWidgetController *centralWidget = new DuiWidgetController(container);
    container->setCentralWidget(centralWidget);
    QGraphicsLinearLayout *containerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget->setLayout(containerLayout);

    // Create and add settings widgets to container
    containerLayout->addItem(DuiSettingsLanguageWidgetFactory::createWidget(settingsBinary, settingsDataStore));

    return container;
}


void DuiAppletSettingsDialog::exec(const DuiAppletSettings& settings)
{
    if (!settings.hasSettings()) {
        return;
    }

    // Create a central widget for the settings dialog
    DuiWidgetController *widget = new DuiWidgetController;
    widget->setView(new DuiWidgetView(widget));
    widget->setObjectName("DuiAppletHandleSettingsWidget");
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setLayout(layout);

    const DuiSettingsLanguageBinary* instanceSettingsBinary = settings.instanceSettingsBinary();
    if (instanceSettingsBinary != NULL) {
        //~ uispec-document ??? FIXME
        //% "Applet Instance Settings"
        layout->addItem(createAppletSettingsWidgets(widget, qtTrId("duiappletsettingsdialog_applet_instance_settings_title"), *instanceSettingsBinary, settings.instanceDataStore()));
    }

    const DuiSettingsLanguageBinary* globalSettingsBinary = settings.globalSettingsBinary();
    if (globalSettingsBinary != NULL) {
        //~ uispec-document ??? FIXME
        //% "Applet Global Settings"
        layout->addItem(createAppletSettingsWidgets(widget, qtTrId("duiappletsettingsdialog_applet_global_settings_title"), *globalSettingsBinary, settings.globalDataStore()));
    }

    // Create a dialog to show the applet instance and global settings
    //~ uispec-document ??? FIXME
    //% "Applet Settings"
    DuiDialog dialog(qtTrId("duiapplethandle_applet_settings_title"), Dui::NoStandardButton);
    dialog.setCentralWidget(widget);
    dialog.setObjectName("AppletSettingsDialog");
    dialog.exec();
}


DuiAppletSettingsDialog::DuiAppletSettingsDialog()
{
}

DuiAppletSettingsDialog::~DuiAppletSettingsDialog()
{
}
