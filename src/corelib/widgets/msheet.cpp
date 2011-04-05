/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msheet.h"
#include "msheetmodel.h"
#include "msheet_p.h"
#include <mscene.h>
#include <mscenemanager.h>
#include <mscenemanager_p.h>
#include <mwindow.h>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MSheet)

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
#endif

//////////////////
// MSheetPrivate

MSheetPrivate::MSheetPrivate()
    : standAloneWindow(0),
    appearSystemwideDeletionPolicy(0)
{
}

MSheetPrivate::~MSheetPrivate()
{
    if (standAloneWindow) {
        Q_Q(MSheet);
        standAloneWindow->close();
        standAloneWindow->scene()->removeItem(q);
        delete standAloneWindow;
        standAloneWindow = 0;
    }

    delete appearSystemwideDeletionPolicy;
    appearSystemwideDeletionPolicy = 0;
}

bool MSheetPrivate::canDisappear(bool dismissing)
{
    Q_Q(MSheet);

    if (standAloneWindow && standAloneWindow->scene() == q->scene()
        && standAloneWindow->isOnDisplay()) {

        // we are being shown systemwide. In this situations we let
        // the window manager handle our closing animation.

        if (dismissing) {
            q->connect(standAloneWindow, SIGNAL(displayExited()), SLOT(_q_makeSystemSheetDisappear()));
        } else {
            q->connect(standAloneWindow, SIGNAL(displayExited()), SLOT(_q_dismissSystemSheet()));
        }

        standAloneWindow->close();

        // FIXME: FINISH THIS IMPLEMENTATION HERE
        // let it disappear once standAloneWindow emits displayExited()
        return false;

    } else {
        return true;
    }
}

bool MSheetPrivate::canDisappear()
{
    return canDisappear(false);
}

bool MSheetPrivate::canDismiss()
{
    return canDisappear(true);
}

void MSheetPrivate::_q_makeSystemSheetDisappear()
{
    Q_Q(MSheet);

    QObject::disconnect(standAloneWindow, SIGNAL(displayExited()),
                        q, SLOT(_q_makeSystemSheetDisappear()));

    Q_ASSERT(standAloneWindow->scene() == q->scene());
    q->sceneManager()->disappearSceneWindowNow(q);
}

void MSheetPrivate::_q_dismissSystemSheet()
{
    Q_Q(MSheet);

    QObject::disconnect(standAloneWindow, SIGNAL(displayExited()),
                        q, SLOT(_q_dismissSystemSheet()));

    Q_ASSERT(standAloneWindow->scene() == q->scene());
    q->sceneManager()->dismissSceneWindowNow(q);
}

void MSheetPrivate::_q_onCentralWidgetDestroyed()
{
    Q_Q(MSheet);
    q->model()->setCentralWidget(0);
}

void MSheetPrivate::_q_onHeaderWidgetDestroyed()
{
    Q_Q(MSheet);
    q->model()->setHeaderWidget(0);
}

#ifdef Q_WS_X11
void MSheetPrivate::appendMSheetTypePropertyToStandAloneWindow()
{
    Q_ASSERT(standAloneWindow);

    Atom atomWindowType = XInternAtom(QX11Info::display(),
                                      "_MEEGOTOUCH_NET_WM_WINDOW_TYPE_SHEET", False);

    XChangeProperty(QX11Info::display(), standAloneWindow->effectiveWinId(),
                    XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False),
                    XA_ATOM, 32, PropModeAppend, (unsigned char*) &atomWindowType, 1);
}
#endif

void MSheetPrivate::_q_onStandAloneSheetDisappeared()
{
    Q_Q(MSheet);
    Q_ASSERT(standAloneWindow != 0);

    q->disconnect(SIGNAL(disappeared()), q, SLOT(_q_onStandAloneSheetDisappeared()));

    // Remove sheet from scene otherwise scene will delete sheet
    // on scene's destructor

    standAloneWindow->sceneManager()->d_func()->removeSceneWindow(q);

    if (q->scene()) {
        q->scene()->removeItem(q);
    }

    if (*appearSystemwideDeletionPolicy == MSceneWindow::DestroyWhenDone
        || (*appearSystemwideDeletionPolicy == MSceneWindow::DestroyWhenDismissed && dismissed)) {

        q->deleteLater();
    }

    delete appearSystemwideDeletionPolicy;
    appearSystemwideDeletionPolicy = 0;
}

void MSheetPrivate::appearSystemwide(MSceneWindow::DeletionPolicy policy)
{
    Q_Q(MSheet);

    if (standAloneWindow == 0) {
        standAloneWindow = new MWindow(new MSceneManager);
        standAloneWindow->setRoundedCornersEnabled(true);

#ifdef Q_WS_X11
        appendMSheetTypePropertyToStandAloneWindow();
#endif
    }

    q->connect(q, SIGNAL(disappeared()), SLOT(_q_onStandAloneSheetDisappeared()));

    if (!appearSystemwideDeletionPolicy) {
        appearSystemwideDeletionPolicy = new MSceneWindow::DeletionPolicy;
    }
    *appearSystemwideDeletionPolicy = policy;

    standAloneWindow->sceneManager()->appearSceneWindowNow(q);
    standAloneWindow->show();
}

//////////////////
// MSheet

MSheet::MSheet() :
    MSceneWindow(new MSheetPrivate,
                 new MSheetModel,
                 MSceneWindow::Sheet, QString(""), 0)
{
}

MSheet::~MSheet()
{
    setHeaderWidget(0);
    setCentralWidget(0);
}

QGraphicsWidget *MSheet::centralWidget()
{
    return model()->centralWidget();
}

void MSheet::setCentralWidget(QGraphicsWidget *newWidget)
{
    QGraphicsWidget *oldWidget  = model()->centralWidget();

    model()->setCentralWidget(newWidget);

    delete oldWidget;

    if (newWidget)
        connect(newWidget, SIGNAL(destroyed()), SLOT(_q_onCentralWidgetDestroyed()));
}

QGraphicsWidget *MSheet::headerWidget()
{
    return model()->headerWidget();
}

void MSheet::setHeaderWidget(QGraphicsWidget *newWidget)
{
    QGraphicsWidget *oldWidget  = model()->headerWidget();

    model()->setHeaderWidget(newWidget);

    delete oldWidget;

    if (newWidget)
        connect(newWidget, SIGNAL(destroyed()), SLOT(_q_onHeaderWidgetDestroyed()));
}

void MSheet::appearSystemwide(MSceneWindow::DeletionPolicy policy)
{
    Q_D(MSheet);
    d->appearSystemwide(policy);
}

void MSheet::setHeaderVisible(bool visible)
{
    model()->setHeaderVisible(visible);
}

bool MSheet::isHeaderVisible() const
{
    return model()->headerVisible();
}

#include "moc_msheet.cpp"
