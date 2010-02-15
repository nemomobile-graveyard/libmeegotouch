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

#include "duiaction.h"
#include "duiappletsharedmutex.h"
#include "duiprogressindicator.h"
#include "duimessagebox.h"
#include "duilocale.h"
#include "duibutton.h"
#include "duiappletsettingsdialog.h"
#include "duiappletsettings.h"
#include "duiscenemanager.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>

#ifdef Q_WS_X11
#include <QX11Info>
#endif

DuiAppletHandleViewPrivate::DuiAppletHandleViewPrivate(DuiAppletHandle *appletHandle) :
    appletHandle(appletHandle),
    pixmapTakenIntoUse(NULL),
    pixmapToBeTakenIntoUse(NULL),
    pixmapSizeToBeTakenIntoUse(NULL),
    progressIndicator(new DuiProgressIndicator(appletHandle))
{
    // The progress indicator is not visible by default
    progressIndicator->setVisible(false);
}

DuiAppletHandleViewPrivate::~DuiAppletHandleViewPrivate()
{
    destroyPixmaps();
}

void DuiAppletHandleViewPrivate::connectSignals()
{
    Q_Q(DuiAppletHandleView);

    // Listen for pixmap taken into use signals
    QObject::connect(appletHandle, SIGNAL(pixmapTakenIntoUse(Qt::HANDLE)), q, SLOT(pixmapTakenIntoUse(Qt::HANDLE)));

    // Listen for scene changed signals
    QObject::connect(appletHandle, SIGNAL(appletPixmapModified(QRectF)), q, SLOT(appletPixmapModified(QRectF)));
}

void DuiAppletHandleViewPrivate::destroyPixmaps()
{
    if (pixmapTakenIntoUse != NULL) {
        delete pixmapTakenIntoUse;
        pixmapTakenIntoUse = NULL;
    }
    if (pixmapToBeTakenIntoUse != NULL) {
        delete pixmapToBeTakenIntoUse;
        pixmapToBeTakenIntoUse = NULL;
    }
}

void DuiAppletHandleViewPrivate::showAppletBrokenDialog()
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
        appletHandle->reinit();
    } else if (mb.clickedButton() == removeButton) {
        appletHandle->removeApplet();
    }
}

void DuiAppletHandleViewPrivate::showAppletInstallationFailedDialog(const QString &error)
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
        appletHandle->removeApplet();
    }
}

void DuiAppletHandleViewPrivate::resetView()
{
    destroyPixmaps();

    // Reset the old geometry to force the applet pixmap ID to be sent to the applet
    oldGeometry = QRectF();
}

void DuiAppletHandleViewPrivate::drawAppletBrokenState()
{
    Q_Q(DuiAppletHandleView);

    q->update();
}

void DuiAppletHandleViewPrivate::drawAppletPixmap(QPainter *, const QRectF &, const QRectF &, bool) const
{
}

QSizeF DuiAppletHandleViewPrivate::hostToAppletCoordinates(const QSizeF &size) const
{
    Q_Q(const DuiAppletHandleView);

    return size / q->model()->appletScale();
}

QSizeF DuiAppletHandleViewPrivate::appletToHostCoordinates(const QSizeF &size) const
{
    Q_Q(const DuiAppletHandleView);

    return size * q->model()->appletScale();
}

void DuiAppletHandleViewPrivate::calculateAppletScale()
{
    Q_Q(DuiAppletHandleView);

    // Set a scaling factor if the applet's minimum width is larger than the available width
    const DuiAppletHandleStyleContainer *handleStyle = dynamic_cast<const DuiAppletHandleStyleContainer *>(&q->style());
    if (handleStyle != NULL) {
        qreal minimumWidth = q->model()->sizeHints().at(Qt::MinimumSize).width();
        qreal availableWidth = (*handleStyle)->maximumAppletSize().width();
        q->model()->setAppletScale(minimumWidth > availableWidth ? availableWidth / minimumWidth : 1);
    }
}

void DuiAppletHandleViewPrivate::appletSizeChanged(QSizeF size)
{
    if (pixmapToBeTakenIntoUse == NULL) {
        // The pixmap to be taken into use has been taken into use. A new one can be allocated
        allocatePixmapToBeTakenIntoUse(size);
    } else {
        // The pixmap to be taken into use has not been taken into use.
        // The new size should be taken into use when the pixmap has been taken into use.
        delete pixmapSizeToBeTakenIntoUse;
        pixmapSizeToBeTakenIntoUse = new QSizeF(size);
    }
}

void DuiAppletHandleViewPrivate::allocatePixmapToBeTakenIntoUse(QSizeF size)
{
    // Allocate a new pixmap to be taken into use
    QSizeF pixmapSize = size.expandedTo(QSizeF(1, 1));
    pixmapToBeTakenIntoUse = new QPixmap(pixmapSize.width(), pixmapSize.height());
    pixmapToBeTakenIntoUse->setAlphaChannel(*pixmapToBeTakenIntoUse);

    // Clear the pixmap
    QPainter painter(pixmapToBeTakenIntoUse);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(pixmapToBeTakenIntoUse->rect(), QColor(0, 0, 0, 0));

    // Synchronize X to make sure the pixmap handle is now valid
    QApplication::syncX();

// handle() is X11 specific
#ifdef Q_WS_X11
    // Communicate the new geometry and pixmap handle to be taken into use to the applet
    appletHandle->sendGeometryMessage(QRectF(QPointF(), pixmapSize), pixmapToBeTakenIntoUse->handle());
#endif
}

void DuiAppletHandleViewPrivate::updateLocalPixmap()
{
    if (!pixmapModifiedRect.isEmpty() && pixmapTakenIntoUse != NULL && !pixmapTakenIntoUse->isNull()) {
        Q_Q(DuiAppletHandleView);

        // Get the address of the pixmap mutex in shared memory and lock the mutex
        DuiAppletSharedMutex *mutex = q->model()->pixmapMutex();
        if (mutex != NULL && mutex->tryLock()) {
            // Mutex locked: copy the changed pixmap area to the local pixmap
            if (!localPixmap.isNull()) {
                QPainter painter(&localPixmap);
                painter.setCompositionMode(QPainter::CompositionMode_Source);
                painter.drawPixmap(pixmapModifiedRect, *pixmapTakenIntoUse, pixmapModifiedRect);
            }

            // Unlock the pixmap mutex
            mutex->unlock();

            // Update the changed region
            q->update(pixmapModifiedRect);

            // No modifications pending anymore
            pixmapModifiedRect = QRectF();
        }
    }
}



DuiAppletHandleView::DuiAppletHandleView(DuiAppletHandle *appletHandle) :
    DuiWidgetView(* new DuiAppletHandleViewPrivate(appletHandle), appletHandle)
{
    Q_D(DuiAppletHandleView);

    d->connectSignals();
}

DuiAppletHandleView::DuiAppletHandleView(DuiAppletHandleViewPrivate &dd, DuiAppletHandle *appletHandle) :
    DuiWidgetView(dd, appletHandle)
{
    Q_D(DuiAppletHandleView);

    d->connectSignals();
}

DuiAppletHandleView::~DuiAppletHandleView()
{
}

void DuiAppletHandleView::setupModel()
{
    DuiWidgetView::setupModel();

    // Let the view know what's the state of the model
    QList<const char *> m;
    m << DuiAppletHandleModel::AppletScale;
    m << DuiAppletHandleModel::State;
    m << DuiAppletHandleModel::AppletSettings;
    m << DuiAppletHandleModel::InstallationProgress;
    updateData(m);
}

void DuiAppletHandleView::openAppletSettings()
{
    if (model()->appletSettings() != NULL) {
        DuiAppletSettingsDialog settingsDialog(*model()->appletSettings());
        settingsDialog.exec();
    }
}

void DuiAppletHandleView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiAppletHandleView);

    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiAppletHandleModel::State) {
            switch (model()->state()) {
            case DuiAppletHandleModel::INSTALLING:
                d->progressIndicator->setViewType(DuiProgressIndicator::barType);
                d->progressIndicator->setRange(0, 100);
                d->progressIndicator->setVisible(true);
                d->progressIndicator->setUnknownDuration(false);
                d->progressIndicator->setValue(model()->installationProgress());
                break;
            case DuiAppletHandleModel::BROKEN:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                d->drawAppletBrokenState();
                break;
            case DuiAppletHandleModel::STARTING:
                d->progressIndicator->setVisible(true);
                d->progressIndicator->setUnknownDuration(true);
                break;
            case DuiAppletHandleModel::STOPPED:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                break;
            case DuiAppletHandleModel::RUNNING:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                d->resetView();
                break;
            }
        } else if (member == DuiAppletHandleModel::AppletSettings) {
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
        } else if (member == DuiAppletHandleModel::InstallationProgress) {
            d->progressIndicator->setValue(model()->installationProgress());
        }
    }
}

void DuiAppletHandleView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    Q_D(const DuiAppletHandleView);

    if (!d->localPixmap.isNull()) {
        // If there are modifications to be copied to the local pixmap try to copy them
        if (!d->pixmapModifiedRect.isEmpty()) {
            const_cast<DuiAppletHandleViewPrivate *>(d)->updateLocalPixmap();
        }

        bool brokenState = model()->state() == DuiAppletHandleModel::BROKEN || model()->state() == DuiAppletHandleModel::STARTING;
        if (brokenState) {
            // In broken state use a specific opacity
            painter->setOpacity(style()->brokenAppletOpacity());
        }

        // Draw the applet pixmap
        QRectF source(d->localPixmap.rect());
        QRectF destination(QPointF(), d->appletToHostCoordinates(d->localPixmap.size()));
        d->drawAppletPixmap(painter, source, destination, brokenState);
    }

    if (model()->state() == DuiAppletHandleModel::BROKEN) {
        const QPixmap *brokenAppletImage = style()->brokenAppletImage();
        if (brokenAppletImage != NULL) {
            QPoint p(size().width() - brokenAppletImage->width(), 0);
            p += style()->brokenAppletImageOffset();
            painter->setOpacity(1.0f);
            painter->drawPixmap(p, *brokenAppletImage);
        }
    }
}

void DuiAppletHandleView::setGeometry(const QRectF &rect)
{
    Q_D(DuiAppletHandleView);

    // rect is always between minimum and maximum applet size because the size hints have been
    // bounded to these limits; calculate scaling factor if even the minimum size won't fit
    d->calculateAppletScale();

    // Get the old size and the new size
    QSizeF oldSize = d->hostToAppletCoordinates(d->oldGeometry.size());
    QSizeF newSize = d->hostToAppletCoordinates(rect.size());

    // Apply the geometry locally immediately
    DuiWidgetView::setGeometry(rect);
    d->oldGeometry = rect;

    // Check whether the new size differs from the old size
    if (newSize != oldSize) {
        d->appletSizeChanged(newSize);
    }

    // Set the progress indicator position
    QRectF progressIndicatorRect;
    progressIndicatorRect.setSize(rect.size() * 0.5f);
    progressIndicatorRect.moveCenter(rect.center() - rect.topLeft());
    d->progressIndicator->setGeometry(progressIndicatorRect);
}

QRectF DuiAppletHandleView::boundingRect() const
{
    Q_D(const DuiAppletHandleView);

    // Use the (downscaled) applet size as the bounding rectangle
    return QRectF(QPointF(), d->appletToHostCoordinates(d->localPixmap.size()).expandedTo(style()->minimumAppletSize()).boundedTo(style()->maximumAppletSize()));
}

void DuiAppletHandleView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiAppletHandleView);

    // TODO: this should actually be handled by a mouse click, but at the
    // moment the base class doesn't contain such method...
    if (event->button() == Qt::LeftButton) {
        if (model()->state() == DuiAppletHandleModel::BROKEN && model()->installationError().isEmpty()) {
            d->showAppletBrokenDialog();
        } else if (model()->state() == DuiAppletHandleModel::BROKEN && !model()->installationError().isEmpty()) {
            d->showAppletInstallationFailedDialog(model()->installationError());
        }
    }
}

void DuiAppletHandleView::pixmapTakenIntoUse(Qt::HANDLE)
{
    Q_D(DuiAppletHandleView);

    // Delete the pixmap that was in use, mark the new one as taken into use
    delete d->pixmapTakenIntoUse;
    d->pixmapTakenIntoUse = d->pixmapToBeTakenIntoUse;
    d->pixmapToBeTakenIntoUse = NULL;
    d->localPixmap = d->pixmapTakenIntoUse->copy();

    if (d->pixmapSizeToBeTakenIntoUse != NULL) {
        // The applet pixmap has not been resized to the latest requested size so do it now
        d->appletSizeChanged(*d->pixmapSizeToBeTakenIntoUse);

        // Size taken into use
        delete d->pixmapSizeToBeTakenIntoUse;
        d->pixmapSizeToBeTakenIntoUse = NULL;
    }
}

QSizeF DuiAppletHandleView::sizeHint(Qt::SizeHint which, const QSizeF &) const
{
    Q_D(const DuiAppletHandleView);

    // Return the requested size hint bounded to minimum and maximum applet sizes defined in the style
    return d->appletToHostCoordinates(model()->sizeHints().at(which)).expandedTo(style()->minimumAppletSize()).boundedTo(style()->maximumAppletSize());
}

void DuiAppletHandleView::appletPixmapModified(const QRectF &rect)
{
    Q_D(DuiAppletHandleView);

    // Unite the changed rectangle with the previous changed rectangle
    d->pixmapModifiedRect = rect.united(d->pixmapModifiedRect);

    // Update the local pixmap
    d->updateLocalPixmap();
}

DUI_REGISTER_VIEW_NEW(DuiAppletHandleView, DuiAppletHandle)
