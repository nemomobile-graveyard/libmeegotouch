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

#include "duiapplettester.h"

#include <DuiAppletMetaData>
#include <duifiledatastore.h>
#include <DuiWidget>
#include <duiappletloader.h>
#include <DuiApplication>
#include "duiapplettesterwindow.h"
#include <DuiAction>
#include <DuiLocale>
#include <DuiDialog>
#include <DuiButton>
#include <QGraphicsLinearLayout>
#include <DuiSettingsLanguageWidgetFactory>
#include <DuiSettingsLanguageWidget>
#include <DuiSettingsLanguageParser>
#include <duiappletsettingsdialog.h>
#include <duiappletsettings.h>
#include <duigconfdatastore.h>
#include <DuiScene>
#include <QDir>
#include <QDebug>

// Horizontal margin around the applet
static const int hMargin = 10;
// Vertical margin around the applet
static const int vMargin = 10;

DuiAppletTester::DuiAppletTester() :
    m_app(NULL),
    m_metadata(NULL),
    m_appletInstanceDataStore(NULL),
    m_widget(NULL),
    m_window(NULL),
    appletSettings(NULL)
{
}

DuiAppletTester::~DuiAppletTester()
{
    // Set m_widget to NULL (since it's already destroyed at this point) to prevent double deletion
    m_widget = NULL;
    teardown();
}

bool DuiAppletTester::init(const DuiAppletMetaData &metaData, DuiAppletId appletId)
{
    // Try to construct a valid instance data store and bail out if not successful
    m_appletInstanceDataStore = generateInstanceDataStorePath(metaData.appletBinary());
    if (m_appletInstanceDataStore == NULL) {
        teardown();
        return false;
    }

    // Construct applet settings and a dialog for them
    appletSettings = new DuiAppletSettings(metaData.fileName(), appletId);

    // Load the applet
    m_widget = DuiAppletLoader::loadApplet(metaData, *m_appletInstanceDataStore, *appletSettings->dataAccess());

    if (m_widget == NULL) {
        qWarning() << "Failed to construct actual applet widget.";
        teardown();
        return false;
    }
    m_metadata = &metaData;

    // Attach a duiaction if the applet contains settings
    if (appletSettings->hasSettings()) {
        //% "Applet Settings"
        DuiAction *action = new DuiAction(qtTrId("duiapplethandle_applet_settings"), m_widget);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(openAppletSettings()), Qt::QueuedConnection);
        m_widget->addAction(action);
    }
    return true;
}

void DuiAppletTester::openAppletSettings()
{
    if ( appletSettings )
	DuiAppletSettingsDialog::exec(*appletSettings);
}

int DuiAppletTester::exec()
{
    if (m_widget == NULL) {
        return -1;
    }

    if (m_window == NULL) {
        m_window = new DuiAppletTesterWindow;
        connect(m_window, SIGNAL(resized(QSize)), this, SLOT(windowResized(QSize)));

#ifndef __arm__
        // Bring back the window frame in case somebody has messed with it and removed it...
        if (m_window->windowFlags() & Qt::FramelessWindowHint) {
            m_window->setWindowFlags(m_window->windowFlags() ^ Qt::FramelessWindowHint);
        }
#endif
    }

    m_window->setWindowTitle(QString("Dui applet tester: ") + m_metadata->appletBinary());
    m_widget->setPos(0.0, 0.0);

    QGraphicsLinearLayout widgetLayout;
    widgetLayout.setContentsMargins(0, 0, 0, 0);

    QGraphicsWidget *parentWidget = new QGraphicsWidget();
    parentWidget->setLayout(&widgetLayout);
    widgetLayout.addItem(m_widget);
    m_window->scene()->addItem(parentWidget);
    parentWidget->setZValue(-1);

    QRect rect = QRect(QPoint(), m_widget->preferredSize().toSize());
    rect.adjust(0, 0, hMargin * 2, vMargin * 2);
    m_window->setFrameStyle(0);
    m_window->setGeometry(rect);
    m_window->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_window->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_window->centerOn(0.0, 0.0);

    m_window->show();

    return qApp->exec();
}

void DuiAppletTester::windowResized(const QSize &newSize)
{
    if (m_widget != NULL) {
        QSizeF newWidgetSize(newSize.width() - (hMargin * 2), newSize.height() - (vMargin * 2));
        newWidgetSize.boundedTo(m_widget->maximumSize());
        newWidgetSize.expandedTo(m_widget->minimumSize());
        m_widget->setGeometry(0.0, 0.0, newWidgetSize.width(), newWidgetSize.height());

        // Twingle with the widget position if the new window size is bigger than the scene rect.
        // This makes the applet top-left corner stay in the window top-left corner.
        QPointF widgetPos = m_widget->pos();
        if (m_window->sceneRect().width() < newSize.width()) {
            widgetPos.setX((m_window->sceneRect().width() - newSize.width()) / 2);
        }
        if (m_window->sceneRect().height() < newSize.height()) {
            widgetPos.setY((m_window->sceneRect().height() - newSize.height()) / 2);
        }
        m_widget->setPos(widgetPos);
    }
}

DuiDataStore *DuiAppletTester::generateInstanceDataStorePath(const QString &appletBinary)
{
    QString appletInstanceFileDataPath(appletBinary);
    appletInstanceFileDataPath = QDir::tempPath() + '/' + appletInstanceFileDataPath.replace('/', '_');
    appletInstanceFileDataPath.append(".datastore");
    appletInstanceFileDataPath = QDir::cleanPath(appletInstanceFileDataPath);
    qDebug() << "Instance file:" << appletInstanceFileDataPath;

    DuiFileDataStore *dataStore = new DuiFileDataStore(appletInstanceFileDataPath);
    if (!dataStore->isReadable() || !dataStore->isWritable()) {
        qWarning() << "Couldn't create applet instance data store";
        delete dataStore;
        dataStore = NULL;
    }

    return dataStore;
}

void DuiAppletTester::teardown()
{
    delete m_widget;
    m_widget = NULL;
    delete m_appletInstanceDataStore;
    m_appletInstanceDataStore = NULL;

    m_metadata = NULL;

    delete appletSettings;
    appletSettings = NULL;
}
