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
#include "duiappletsettings.h"
#include "duigconfdatastore.h"
#include <QGraphicsLinearLayout>

DuiAppletSettingsDialog::DuiAppletSettingsDialog(const DuiAppletSettings &appletSettings) :
    appletSettings(appletSettings)
{
}

DuiAppletSettingsDialog::~DuiAppletSettingsDialog()
{
}

void DuiAppletSettingsDialog::exec() const
{
    if (!appletSettings.hasSettings()) {
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

    const DuiSettingsLanguageBinary *instanceSettingsBinary = appletSettings.instanceSettingsBinary();
    if (instanceSettingsBinary != NULL) {
        //~ uispec-document ??? FIXME
        //% "Applet Instance Settings"
        layout->addItem(createAppletSettingsWidgets(widget, qtTrId("duiappletsettingsdialog_applet_instance_settings_title"), *instanceSettingsBinary, appletSettings.instanceDataStore()));
    }

    const DuiSettingsLanguageBinary *globalSettingsBinary = appletSettings.globalSettingsBinary();
    if (globalSettingsBinary != NULL) {
        //~ uispec-document ??? FIXME
        //% "Applet Global Settings"
        layout->addItem(createAppletSettingsWidgets(widget, qtTrId("duiappletsettingsdialog_applet_global_settings_title"), *globalSettingsBinary, appletSettings.globalDataStore()));
    }

    // Create a dialog to show the applet instance and global settings
    //~ uispec-document ??? FIXME
    //% "Applet Settings"
    DuiDialog dialog(qtTrId("duiapplethandle_applet_settings_title"), widget,
                     Dui::NoStandardButton);
    dialog.setObjectName("AppletSettingsDialog");
    dialog.exec();
}

QGraphicsLayoutItem *DuiAppletSettingsDialog::createAppletSettingsWidgets(QGraphicsItem *parent, const QString &title, const DuiSettingsLanguageBinary &settingsBinary, DuiDataStore *settingsDataStore)
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
