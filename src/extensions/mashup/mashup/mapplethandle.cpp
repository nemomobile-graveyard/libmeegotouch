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

#include "mapplethandle.h"
#include "mapplethandle_p.h"
#include "mappletsettings.h"
#include <mappleticonchangedmessage.h>
#include <mapplettitlechangedmessage.h>
#include <mapplettextchangedmessage.h>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MAppletHandle)

MAppletHandlePrivate::MAppletHandlePrivate() : MExtensionHandlePrivate()
{
}

MAppletHandlePrivate::~MAppletHandlePrivate()
{
}

MAppletHandle::MAppletHandle(QGraphicsItem *parent) :
    MExtensionHandle(new MAppletHandlePrivate, new MAppletHandleModel, parent)
{
}

MAppletHandle::~MAppletHandle()
{
}

void MAppletHandle::initPlaceHolder(const MAppletId &appletId, const QString &packageName, const QString &installationError)
{
    model()->setAppletId(appletId);
    MExtensionHandle::initPlaceHolder(packageName, installationError);
}

void MAppletHandle::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const MAppletId &appletId)
{
    // Set the applet ID
    model()->setAppletId(appletId);

    // Create a new applet settings object and put it into the model
    model()->setAppletSettings(new MAppletSettings(metaDataFileName, appletId));

    // Call base class init with applet specific arguments
    QStringList extraArguments;
    extraArguments << appletId.toString() << appletInstanceFileDataPath;
    MExtensionHandle::init(runnerBinary, metaDataFileName, extraArguments);
}

void MAppletHandle::kill()
{
    MExtensionHandle::kill();

    // Remove the applet settings
    delete model()->appletSettings();
    model()->setAppletSettings(NULL);
}

void MAppletHandle::removeApplet()
{
    emit appletRemovalRequested(model()->appletId());
}

void MAppletHandle::setAppletIcon(const QString &newIcon)
{
    model()->setAppletIcon(newIcon);
    emit appletIconChanged(newIcon);
}

void MAppletHandle::setAppletTitle(const QString &newTitle)
{
    model()->setAppletTitle(newTitle);
    emit appletTitleChanged(newTitle);
}

void MAppletHandle::setAppletText(const QString &newText)
{
    model()->setAppletText(newText);
    emit appletTextChanged(newText);
}

QString MAppletHandle::appletIcon() const
{
    return model()->appletIcon();
}

QString MAppletHandle::appletTitle() const
{
    return model()->appletTitle();
}

QString MAppletHandle::appletText() const
{
    return model()->appletText();
}

void MAppletHandle::messageReceived(const MAppletMessage &message)
{
    switch (message.type()) {
    case MAppletMessage::AppletIconMessage: {
        const MAppletIconChangedMessage *m = dynamic_cast<const MAppletIconChangedMessage *>(&message);
        if (m != NULL) {
            setAppletIcon(m->icon());
        }
        break;
    }

    case MAppletMessage::AppletTitleMessage: {
        const MAppletTitleChangedMessage *m = dynamic_cast<const MAppletTitleChangedMessage *>(&message);
        if (m != NULL) {
            setAppletTitle(m->title());
        }
        break;
    }

    case MAppletMessage::AppletTextMessage: {
        const MAppletTextChangedMessage *m = dynamic_cast<const MAppletTextChangedMessage *>(&message);
        if (m != NULL) {
            setAppletText(m->text());
        }
        break;
    }

    default:
        MExtensionHandle::messageReceived(message);
        break;
    }
}
