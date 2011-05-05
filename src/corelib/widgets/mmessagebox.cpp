/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mmessagebox.h"
#include "mmessagebox_p.h"

#ifdef HAVE_LIBNGF
#   include <dbus/dbus-glib-lowlevel.h>
#   include <QDBusMessage>
#   include <QDBusConnection>
#   ifdef HAVE_CONTEXTSUBSCRIBER
#      include <contextproperty.h>
#   endif
#endif

#include "mwidgetcreator.h"
#include "mdismissevent.h"
M_REGISTER_WIDGET(MMessageBox)

#ifdef HAVE_LIBNGF
#   define APPEAR_EFFECT_NORMAL "query"
#   define APPEAR_EFFECT_STRONG "query_strong"
#endif

MMessageBoxPrivate::MMessageBoxPrivate()
{
#ifdef HAVE_LIBNGF
    connection = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    dbus_connection_setup_with_g_main(connection, NULL);

    client = ngf_client_create(NGF_TRANSPORT_DBUS, connection);
#ifdef HAVE_CONTEXTSUBSCRIBER
    screenBlankedProperty = 0;
#endif //HAVE_CONTEXTSUBSCRIBER
#endif //HAVE_LIBNGF
}

MMessageBoxPrivate::~MMessageBoxPrivate()
{
#ifdef HAVE_LIBNGF
    ngf_client_destroy(client);
    dbus_connection_unref(connection);
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete screenBlankedProperty;
    screenBlankedProperty = 0;
#endif
#endif
}

void MMessageBoxPrivate::init()
{
    #ifdef HAVE_LIBNGF
    Q_Q(MMessageBox);
    bool ok;
    ok = q->connect(q,
        SIGNAL(sceneWindowStateChanged(MSceneWindow::SceneWindowState, MSceneWindow::SceneWindowState)),
        SLOT(_q_onSceneWindowStateChanged(MSceneWindow::SceneWindowState, MSceneWindow::SceneWindowState)));
    if (!ok) qFatal("signal connection failed");
    #endif /* HAVE_LIBNGF */
}

#ifdef HAVE_LIBNGF
void MMessageBoxPrivate::_q_onSceneWindowStateChanged(MSceneWindow::SceneWindowState newState,
                                                      MSceneWindow::SceneWindowState oldState)
{
    if ((oldState == MSceneWindow::Disappeared && newState == MSceneWindow::Appearing) ||
        (oldState == MSceneWindow::Disappeared && newState == MSceneWindow:: Appeared))
    {
        NgfProplist *p = NULL;
        const gchar *event = isScreenBlanked() ? APPEAR_EFFECT_STRONG : APPEAR_EFFECT_NORMAL;

        ngf_client_play_event(client, event, p);
    }
}

bool MMessageBoxPrivate::isScreenBlanked()
{
    bool result = false;

#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!screenBlankedProperty)
        screenBlankedProperty = new ContextProperty("Screen.Blanked");

    screenBlankedProperty->subscribe();
    screenBlankedProperty->waitForSubscription(true);

    result = screenBlankedProperty->value(false).toBool();

    // We don't want to track changes to this property since we are interested in
    // it only momentarily.
    screenBlankedProperty->unsubscribe();
#endif

    return result;
}
#endif /* HAVE_LIBNGF */

MMessageBox::MMessageBox(const QString &text, M::StandardButtons buttons)
    : MDialog(new MMessageBoxPrivate, buttons, new MMessageBoxModel, MSceneWindow::MessageBox)
{
    Q_D(MMessageBox);
    d->init();

    setText(text);

    setCentralWidget(0);
}

MMessageBox::MMessageBox(const QString &title, const QString &text, M::StandardButtons buttons)
    : MDialog(new MMessageBoxPrivate, buttons, new MMessageBoxModel, MSceneWindow::MessageBox)
{
    Q_D(MMessageBox);
    d->init();

    setTitle(title);
    setText(text);

    setCentralWidget(0);
}

MMessageBox::~MMessageBox()
{
}

QString MMessageBox::text() const
{
    return model()->text();
}

void MMessageBox::setText(const QString &text)
{
    model()->setText(text);
}

QString MMessageBox::iconId() const
{
    return model()->iconId();
}

void MMessageBox::setIconId(const QString &iconId)
{
    model()->setIconId(iconId);
}

void MMessageBox::dismissEvent(MDismissEvent *event)
{
    event->ignore();
}

QPixmap MMessageBox::iconPixmap() const
{
    return model()->iconPixmap();
}

void MMessageBox::setIconPixmap(QPixmap &iconPixmap)
{
    model()->setIconPixmap(iconPixmap);
}

#include "moc_mmessagebox.cpp"

