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

#include "duiapplethandle.h"
#include "duiapplethandle_p.h"
#include "duiappletsettings.h"
#include <duiappleticonchangedmessage.h>
#include <duiapplettitlechangedmessage.h>
#include <duiapplettextchangedmessage.h>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiAppletHandle)

DuiAppletHandlePrivate::DuiAppletHandlePrivate() : DuiExtensionHandlePrivate()
{
}

DuiAppletHandlePrivate::~DuiAppletHandlePrivate()
{
}

DuiAppletHandle::DuiAppletHandle(QGraphicsItem *parent) :
    DuiExtensionHandle(new DuiAppletHandlePrivate, new DuiAppletHandleModel, parent)
{
}

DuiAppletHandle::~DuiAppletHandle()
{
}

void DuiAppletHandle::initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError)
{
    model()->setAppletId(appletId);
    DuiExtensionHandle::initPlaceHolder(packageName, installationError);
}

void DuiAppletHandle::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId)
{
    // Set the applet ID
    model()->setAppletId(appletId);

    // Create a new applet settings object and put it into the model
    model()->setAppletSettings(new DuiAppletSettings(metaDataFileName, appletId));

    // Call base class init with applet specific arguments
    QStringList extraArguments;
    extraArguments << appletId.toString() << appletInstanceFileDataPath;
    DuiExtensionHandle::init(runnerBinary, metaDataFileName, extraArguments);
}

void DuiAppletHandle::kill()
{
    DuiExtensionHandle::kill();

    // Remove the applet settings
    delete model()->appletSettings();
    model()->setAppletSettings(NULL);
}

void DuiAppletHandle::removeApplet()
{
    emit appletRemovalRequested(model()->appletId());
}

void DuiAppletHandle::setAppletIcon(const QString &newIcon)
{
    model()->setAppletIcon(newIcon);
    emit appletIconChanged(newIcon);
}

void DuiAppletHandle::setAppletTitle(const QString &newTitle)
{
    model()->setAppletTitle(newTitle);
    emit appletTitleChanged(newTitle);
}

void DuiAppletHandle::setAppletText(const QString &newText)
{
    model()->setAppletText(newText);
    emit appletTextChanged(newText);
}

QString DuiAppletHandle::appletIcon() const
{
    return model()->appletIcon();
}

QString DuiAppletHandle::appletTitle() const
{
    return model()->appletTitle();
}

QString DuiAppletHandle::appletText() const
{
    return model()->appletText();
}

void DuiAppletHandle::messageReceived(const DuiAppletMessage &message)
{
    switch (message.type()) {
    case DuiAppletMessage::APPLET_ICON_MESSAGE: {
        const DuiAppletIconChangedMessage *m = dynamic_cast<const DuiAppletIconChangedMessage *>(&message);
        if (m != NULL) {
            setAppletIcon(m->icon());
        }
        break;
    }

    case DuiAppletMessage::APPLET_TITLE_MESSAGE: {
        const DuiAppletTitleChangedMessage *m = dynamic_cast<const DuiAppletTitleChangedMessage *>(&message);
        if (m != NULL) {
            setAppletTitle(m->title());
        }
        break;
    }

    case DuiAppletMessage::APPLET_TEXT_MESSAGE: {
        const DuiAppletTextChangedMessage *m = dynamic_cast<const DuiAppletTextChangedMessage *>(&message);
        if (m != NULL) {
            setAppletText(m->text());
        }
        break;
    }

    default:
        DuiExtensionHandle::messageReceived(message);
        break;
    }
}
