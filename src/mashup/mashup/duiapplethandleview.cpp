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

#include "duiapplethandleview.h"
#include "duiapplethandleview_p.h"
#include "duiappletsettingsdialog.h"
#include <DuiMessageBox>
#include <DuiSceneManager>
#include <DuiAction>

#include <QGraphicsEffect>

DuiAppletHandleViewPrivate::DuiAppletHandleViewPrivate(DuiAppletHandle *appletHandle) :
    DuiExtensionHandleViewPrivate(appletHandle),
    brokenAppletPixmap(QPixmap())
{
}

DuiAppletHandleViewPrivate::~DuiAppletHandleViewPrivate()
{
    destroyPixmaps();
}

void DuiAppletHandleViewPrivate::destroyPixmaps()
{
    brokenAppletPixmap = QPixmap();

    DuiExtensionHandleViewPrivate::destroyPixmaps();
}

void DuiAppletHandleViewPrivate::showBrokenDialog()
{
    Q_Q(DuiAppletHandleView);

    // the following use of translatable strings is not good.
    // Splitting sentences like this makes it hard to translate them
    // into language where the structure of the sentence is very
    // different from English.
    QString s;
    QTextStream(&s) <<
                    "<font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "The applet"
                    qtTrId("duiapplethandle_brokendialog_messagestart") <<
                    "</font> <font color=\"orange\">" <<
                    q->model()->appletTitle() <<
                    "</font> <font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "is not functioning properly.<br />"
                    //% "Please select whether to restart or remove the applet."
                    qtTrId("duiapplethandle_brokendialog_messageend") << "</font>";

    DuiMessageBox mb(s, Dui::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Restart"
    DuiButtonModel *restartButton = mb.addButton(qtTrId("duiapplethandle_brokendialog_restart"));
    //~ uispec-document ??? FIXME
    //% "Remove"
    DuiButtonModel *removeButton = mb.addButton(qtTrId("duiapplethandle_brokendialog_remove"));

    controller->sceneManager()->execDialog(&mb);

    if (mb.clickedButton() == restartButton) {
        handle->reinit();
    } else if (mb.clickedButton() == removeButton) {
        DuiAppletHandle *appletHandle = static_cast<DuiAppletHandle *>(handle);
        appletHandle->removeApplet();
    }
}

void DuiAppletHandleViewPrivate::showInstallationFailedDialog(const QString &error)
{
    Q_Q(DuiAppletHandleView);

    // the following use of translatable strings is not good.
    // Splitting sentences like this makes it hard to translate them
    // into language where the structure of the sentence is very
    // different from English.
    QString s;
    QTextStream(&s) <<
                    "<font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "The applet"
                    qtTrId("duiapplethandle_installationfaileddialog_messagestart") <<
                    "</font> <font color=\"orange\">" <<
                    q->model()->appletTitle() <<
                    "</font> <font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "failed to install properly with the following error message: <br />"
                    qtTrId("duiapplethandle_installationfaileddialog_messagemiddle") <<
                    error <<
                    "<br />" <<
                    //% "Please remove the applet."
                    qtTrId("duiapplethandle_installationfaileddialog_messageend") << "</font>";

    DuiMessageBox mb(s, Dui::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Remove"
    DuiButtonModel *removeButton = mb.addButton(qtTrId("duiapplethandle_brokendialog_remove"));

    controller->sceneManager()->execDialog(&mb);
    if (mb.clickedButton() == removeButton) {
        DuiAppletHandle *appletHandle = static_cast<DuiAppletHandle *>(handle);
        appletHandle->removeApplet();
    }
}


void DuiAppletHandleViewPrivate::drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const
{
    // Draw the applet pixmap scaled so that it fits the screen
    if (brokenState && !brokenAppletPixmap.isNull()) {
        painter->drawPixmap(targetGeometry, brokenAppletPixmap, sourceGeometry);
    } else {
        painter->drawPixmap(targetGeometry, localPixmap, sourceGeometry);
    }
}


void DuiAppletHandleViewPrivate::blackAndWhite(QImage &img)
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

void DuiAppletHandleViewPrivate::drawBrokenState()
{
    Q_Q(DuiAppletHandleView);

    if(brokenAppletPixmap.isNull()) {
        QImage tmp = localPixmap.toImage();
        tmp.convertToFormat(QImage::Format_ARGB32);

        blackAndWhite(tmp);

        QGraphicsScene scene;

        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(tmp));
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
        blur->setBlurRadius(q->style()->brokenBlurRadius());
        item->setGraphicsEffect(new QGraphicsBlurEffect);
        scene.addItem(item);

        brokenAppletPixmap = QPixmap(localPixmap.width(), localPixmap.height());
        brokenAppletPixmap.fill(QColor(0, 0, 0, 0));

        QPainter painter(&brokenAppletPixmap);
        scene.render(&painter);
    }

    DuiExtensionHandleViewPrivate::drawBrokenState();
}

DuiAppletHandleView::DuiAppletHandleView(DuiAppletHandle *appletHandle) :
    DuiExtensionHandleView(* new DuiAppletHandleViewPrivate(appletHandle), appletHandle)
{
}

DuiAppletHandleView::~DuiAppletHandleView()
{
}

void DuiAppletHandleView::setupModel()
{
    DuiExtensionHandleView::setupModel();

    // Let the view know what's the state of the model
    QList<const char *> m;
    m << DuiAppletHandleModel::AppletSettings;
    updateData(m);
}

void DuiAppletHandleView::openAppletSettings()
{
    if (model()->appletSettings() != NULL) {
        DuiAppletSettingsDialog::exec(*model()->appletSettings());
    }
}

void DuiAppletHandleView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiAppletHandleView);

    DuiExtensionHandleView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiAppletHandleModel::AppletSettings) {
            // If there's already an action for showing the applet settings, remove it
            foreach(QAction * action, d->controller->actions()) {
                //~ uispec-document ??? FIXME
                //% "Settings"
                if (action->text() == qtTrId("duiapplethandle_applet_settings")) {
                    d->controller->removeAction(action);
                    delete action;
                }
            }
            if (model()->appletSettings() != NULL && model()->appletSettings()->hasSettings()) {
                //~ uispec-document ??? FIXME
                // Add an action for showing the applet settings
                //% "Settings"
                DuiAction *action = new DuiAction(qtTrId("duiapplethandle_applet_settings"), d->controller);
                connect(action, SIGNAL(triggered(bool)), this, SLOT(openAppletSettings()), Qt::QueuedConnection);
                d->controller->addAction(action);
            }
        }
    }
}

DUI_REGISTER_VIEW_NEW(DuiAppletHandleView, DuiAppletHandle)
