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

#include "mapplethandleview.h"
#include "mapplethandleview_p.h"
#include "mappletsettingsdialog.h"
#include <MMessageBox>
#include <MSceneManager>
#include <MAction>

#include <QGraphicsEffect>

MAppletHandleViewPrivate::MAppletHandleViewPrivate(MAppletHandle *appletHandle) :
    MExtensionHandleViewPrivate(appletHandle),
    brokenAppletPixmap(QPixmap())
{
}

MAppletHandleViewPrivate::~MAppletHandleViewPrivate()
{
    destroyPixmaps();
}

void MAppletHandleViewPrivate::destroyPixmaps()
{
    brokenAppletPixmap = QPixmap();

    MExtensionHandleViewPrivate::destroyPixmaps();
}

void MAppletHandleViewPrivate::showBrokenDialog()
{
    Q_Q(MAppletHandleView);

    // the following use of translatable strings is not good.
    // Splitting sentences like this makes it hard to translate them
    // into language where the structure of the sentence is very
    // different from English.
    QString s;
    QTextStream(&s) <<
                    "<font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "The applet"
                    qtTrId("mapplethandle_brokendialog_messagestart") <<
                    "</font> <font color=\"orange\">" <<
                    q->model()->appletTitle() <<
                    "</font> <font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "is not functioning properly.<br />"
                    //% "Please select whether to restart or remove the applet."
                    qtTrId("mapplethandle_brokendialog_messageend") << "</font>";

    MMessageBox mb(s, M::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Restart"
    MButtonModel *restartButton = mb.addButton(qtTrId("mapplethandle_brokendialog_restart"));
    //~ uispec-document ??? FIXME
    //% "Remove"
    MButtonModel *removeButton = mb.addButton(qtTrId("mapplethandle_brokendialog_remove"));

    controller->sceneManager()->execDialog(&mb);

    if (mb.clickedButton() == restartButton) {
        handle->reinit();
    } else if (mb.clickedButton() == removeButton) {
        MAppletHandle *appletHandle = static_cast<MAppletHandle *>(handle);
        appletHandle->removeApplet();
    }
}

void MAppletHandleViewPrivate::showInstallationFailedDialog(const QString &error)
{
    Q_Q(MAppletHandleView);

    // the following use of translatable strings is not good.
    // Splitting sentences like this makes it hard to translate them
    // into language where the structure of the sentence is very
    // different from English.
    QString s;
    QTextStream(&s) <<
                    "<font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "The applet"
                    qtTrId("mapplethandle_installationfaileddialog_messagestart") <<
                    "</font> <font color=\"orange\">" <<
                    q->model()->appletTitle() <<
                    "</font> <font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "failed to install properly with the following error message: <br />"
                    qtTrId("mapplethandle_installationfaileddialog_messagemiddle") <<
                    error <<
                    "<br />" <<
                    //% "Please remove the applet."
                    qtTrId("mapplethandle_installationfaileddialog_messageend") << "</font>";

    MMessageBox mb(s, M::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Remove"
    MButtonModel *removeButton = mb.addButton(qtTrId("mapplethandle_brokendialog_remove"));

    controller->sceneManager()->execDialog(&mb);
    if (mb.clickedButton() == removeButton) {
        MAppletHandle *appletHandle = static_cast<MAppletHandle *>(handle);
        appletHandle->removeApplet();
    }
}


void MAppletHandleViewPrivate::drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const
{
    // Draw the applet pixmap scaled so that it fits the screen
    if (brokenState && !brokenAppletPixmap.isNull()) {
        painter->drawPixmap(targetGeometry, brokenAppletPixmap, sourceGeometry);
    } else {
        painter->drawPixmap(targetGeometry, localPixmap, sourceGeometry);
    }
}


void MAppletHandleViewPrivate::blackAndWhite(QImage &img)
{
    int v;
    QRgb* buffer;

    for (int i = 0; i < img.height(); ++i) {
        buffer = (QRgb*)img.scanLine(i);
        for (int j = 0; j < img.width(); ++j) {
            v = qRed(*buffer)*30;
            v += qGreen(*buffer)*59;
            v += qBlue(*buffer)*11;

            v /= 100;

            *buffer = qRgba(v, v, v, qAlpha(*buffer));
            buffer++;
        }
    }
}

void MAppletHandleViewPrivate::drawBrokenState()
{
    Q_Q(MAppletHandleView);

    if(brokenAppletPixmap.isNull()) {
        QImage tmp = localPixmap.toImage();
        tmp.convertToFormat(QImage::Format_ARGB32);

        blackAndWhite(tmp);

        QGraphicsScene scene;

        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(tmp));
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
        blur->setBlurRadius(q->style()->brokenBlurRadius());
        item->setGraphicsEffect(blur);
        scene.addItem(item);

        brokenAppletPixmap = QPixmap(localPixmap.width(), localPixmap.height());
        brokenAppletPixmap.fill(QColor(0, 0, 0, 0));

        QPainter painter(&brokenAppletPixmap);
        scene.render(&painter);
    }

    MExtensionHandleViewPrivate::drawBrokenState();
}

MAppletHandleView::MAppletHandleView(MAppletHandle *appletHandle) :
    MExtensionHandleView(* new MAppletHandleViewPrivate(appletHandle), appletHandle)
{
}

MAppletHandleView::~MAppletHandleView()
{
}

void MAppletHandleView::setupModel()
{
    MExtensionHandleView::setupModel();

    // Let the view know what's the state of the model
    QList<const char *> m;
    m << MAppletHandleModel::AppletSettings;
    updateData(m);
}

void MAppletHandleView::openAppletSettings()
{
    if (model()->appletSettings() != NULL) {
        MAppletSettingsDialog::exec(*model()->appletSettings());
    }
}

void MAppletHandleView::updateData(const QList<const char *>& modifications)
{
    Q_D(MAppletHandleView);

    MExtensionHandleView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MAppletHandleModel::AppletSettings) {
            // If there's already an action for showing the applet settings, remove it
            foreach(QAction * action, d->controller->actions()) {
                //~ uispec-document ??? FIXME
                //% "Settings"
                if (action->text() == qtTrId("mapplethandle_applet_settings")) {
                    d->controller->removeAction(action);
                    delete action;
                }
            }
            if (model()->appletSettings() != NULL && model()->appletSettings()->hasSettings()) {
                //~ uispec-document ??? FIXME
                // Add an action for showing the applet settings
                //% "Settings"
                MAction *action = new MAction(qtTrId("mapplethandle_applet_settings"), d->controller);
                connect(action, SIGNAL(triggered(bool)), this, SLOT(openAppletSettings()), Qt::QueuedConnection);
                d->controller->addAction(action);
            }
        }
    }
}

M_REGISTER_VIEW_NEW(MAppletHandleView, MAppletHandle)
