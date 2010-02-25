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

#include "duiextensionhandleview.h"
#include "duiextensionhandleview_p.h"

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

DuiExtensionHandleViewPrivate::DuiExtensionHandleViewPrivate(DuiExtensionHandle *handle) :
    handle(handle),
    pixmapTakenIntoUse(NULL),
    pixmapToBeTakenIntoUse(NULL),
    pixmapSizeToBeTakenIntoUse(NULL),
    progressIndicator(new DuiProgressIndicator(handle))
{
    // The progress indicator is not visible by default
    progressIndicator->setVisible(false);
}

DuiExtensionHandleViewPrivate::~DuiExtensionHandleViewPrivate()
{
    destroyPixmaps();
}

void DuiExtensionHandleViewPrivate::connectSignals()
{
    Q_Q(DuiExtensionHandleView);

    // Listen for pixmap taken into use signals
    QObject::connect(handle, SIGNAL(pixmapTakenIntoUse(Qt::HANDLE)), q, SLOT(pixmapTakenIntoUse(Qt::HANDLE)));

    // Listen for scene changed signals
    QObject::connect(handle, SIGNAL(pixmapModified(QRectF)), q, SLOT(pixmapModified(QRectF)));
}

void DuiExtensionHandleViewPrivate::destroyPixmaps()
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

void DuiExtensionHandleViewPrivate::resetView()
{
    destroyPixmaps();

    // Reset the old geometry to force the pixmap ID to be sent to the remote process
    oldGeometry = QRectF();
}

void DuiExtensionHandleViewPrivate::drawBrokenState()
{
    Q_Q(DuiExtensionHandleView);

    q->update();
}

QSizeF DuiExtensionHandleViewPrivate::hostToRemoteCoordinates(const QSizeF &size) const
{
    Q_Q(const DuiExtensionHandleView);

    return size / q->model()->scale();
}

QSizeF DuiExtensionHandleViewPrivate::remoteToHostCoordinates(const QSizeF &size) const
{
    Q_Q(const DuiExtensionHandleView);

    return size * q->model()->scale();
}

void DuiExtensionHandleViewPrivate::calculateScale()
{
    Q_Q(DuiExtensionHandleView);

    // Set a scaling factor if the minimum width is larger than the available width
    const DuiExtensionHandleStyleContainer *handleStyle = dynamic_cast<const DuiExtensionHandleStyleContainer *>(&q->style());
    if (handleStyle != NULL) {
        qreal minimumWidth = q->model()->sizeHints().at(Qt::MinimumSize).width();
        qreal availableWidth = (*handleStyle)->forcedMaximumSize().width();
        q->model()->setScale(minimumWidth > availableWidth ? availableWidth / minimumWidth : 1);
    }
}

void DuiExtensionHandleViewPrivate::sizeChanged(QSizeF size)
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

void DuiExtensionHandleViewPrivate::allocatePixmapToBeTakenIntoUse(QSizeF size)
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
    // Communicate the new geometry and pixmap handle to be taken into use to the runner
    handle->sendGeometryMessage(QRectF(QPointF(), pixmapSize), pixmapToBeTakenIntoUse->handle());
#endif
}

void DuiExtensionHandleViewPrivate::updateLocalPixmap()
{
    if (!pixmapModifiedRect.isEmpty() && pixmapTakenIntoUse != NULL && !pixmapTakenIntoUse->isNull()) {
        Q_Q(DuiExtensionHandleView);

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

DuiExtensionHandleView::DuiExtensionHandleView(DuiExtensionHandleViewPrivate &dd, DuiExtensionHandle *handle) :
    DuiWidgetView(dd, handle)
{
    Q_D(DuiExtensionHandleView);

    d->connectSignals();
}

DuiExtensionHandleView::~DuiExtensionHandleView()
{
}

void DuiExtensionHandleView::setupModel()
{
    DuiWidgetView::setupModel();

    // Let the view know what's the state of the model
    QList<const char *> m;
    m << DuiExtensionHandleModel::Scale;
    m << DuiExtensionHandleModel::CurrentState;
    m << DuiExtensionHandleModel::InstallationProgress;
    updateData(m);
}

void DuiExtensionHandleView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiExtensionHandleView);

    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiExtensionHandleModel::CurrentState) {
            switch (model()->currentState()) {
            case DuiExtensionHandleModel::INSTALLING:
                d->progressIndicator->setViewType(DuiProgressIndicator::barType);
                d->progressIndicator->setRange(0, 100);
                d->progressIndicator->setVisible(true);
                d->progressIndicator->setUnknownDuration(false);
                d->progressIndicator->setValue(model()->installationProgress());
                break;
            case DuiExtensionHandleModel::BROKEN:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                d->drawBrokenState();
                break;
            case DuiExtensionHandleModel::STARTING:
                d->progressIndicator->setVisible(true);
                d->progressIndicator->setUnknownDuration(true);
                break;
            case DuiExtensionHandleModel::STOPPED:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                break;
            case DuiExtensionHandleModel::RUNNING:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                d->resetView();
                break;
            }
        } else if (member == DuiExtensionHandleModel::InstallationProgress) {
            d->progressIndicator->setValue(model()->installationProgress());
        }
    }
}

void DuiExtensionHandleView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    Q_D(const DuiExtensionHandleView);

    if (!d->localPixmap.isNull()) {
        // If there are modifications to be copied to the local pixmap try to copy them
        if (!d->pixmapModifiedRect.isEmpty()) {
            const_cast<DuiExtensionHandleViewPrivate *>(d)->updateLocalPixmap();
        }

        bool brokenState = model()->currentState() == DuiExtensionHandleModel::BROKEN || model()->currentState() == DuiExtensionHandleModel::STARTING;
        if (brokenState) {
            // In broken state use a specific opacity
            painter->setOpacity(style()->brokenOpacity());
        }

        // Draw the pixmap
        QRectF source(d->localPixmap.rect());
        QRectF destination(QPointF(), d->remoteToHostCoordinates(d->localPixmap.size()));
        d->drawPixmap(painter, source, destination, brokenState);
    }

    if (model()->currentState() == DuiExtensionHandleModel::BROKEN) {
        const QPixmap *brokenImage = style()->brokenImage();
        if (brokenImage != NULL) {
            QPoint p(size().width() - brokenImage->width(), 0);
            p += style()->brokenImageOffset();
            painter->setOpacity(1.0f);
            painter->drawPixmap(p, *brokenImage);
        }
    }
}

void DuiExtensionHandleView::setGeometry(const QRectF &rect)
{
    Q_D(DuiExtensionHandleView);

    // rect is always between minimum and maximum size because the size hints have been
    // bounded to these limits; calculate scaling factor if even the minimum size won't fit
    d->calculateScale();

    // Get the old size and the new size
    QSizeF oldSize = d->hostToRemoteCoordinates(d->oldGeometry.size());
    QSizeF newSize = d->hostToRemoteCoordinates(rect.size());

    // Apply the geometry locally immediately
    DuiWidgetView::setGeometry(rect);
    d->oldGeometry = rect;

    // Check whether the new size differs from the old size
    if (newSize != oldSize) {
        d->sizeChanged(newSize);
    }

    // Set the progress indicator position
    QRectF progressIndicatorRect;
    progressIndicatorRect.setSize(rect.size() * 0.5f);
    progressIndicatorRect.moveCenter(rect.center() - rect.topLeft());
    d->progressIndicator->setGeometry(progressIndicatorRect);
}

QRectF DuiExtensionHandleView::boundingRect() const
{
    Q_D(const DuiExtensionHandleView);

    // Use the (downscaled) size as the bounding rectangle
    return QRectF(QPointF(), d->remoteToHostCoordinates(d->localPixmap.size()).expandedTo(style()->forcedMinimumSize()).boundedTo(style()->forcedMaximumSize()));
}

void DuiExtensionHandleView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiExtensionHandleView);

    // TODO: this should actually be handled by a mouse click, but at the
    // moment the base class doesn't contain such method...
    if (event->button() == Qt::LeftButton) {
        if (model()->currentState() == DuiExtensionHandleModel::BROKEN && model()->installationError().isEmpty()) {
            d->showBrokenDialog();
        } else if (model()->currentState() == DuiExtensionHandleModel::BROKEN && !model()->installationError().isEmpty()) {
            d->showInstallationFailedDialog(model()->installationError());
        }
    }
}

void DuiExtensionHandleView::pixmapTakenIntoUse(Qt::HANDLE)
{
    Q_D(DuiExtensionHandleView);

    // Delete the pixmap that was in use, mark the new one as taken into use
    delete d->pixmapTakenIntoUse;
    d->pixmapTakenIntoUse = d->pixmapToBeTakenIntoUse;
    d->pixmapToBeTakenIntoUse = NULL;
    d->localPixmap = d->pixmapTakenIntoUse->copy();

    if (d->pixmapSizeToBeTakenIntoUse != NULL) {
        // The pixmap has not been resized to the latest requested size so do it now
        d->sizeChanged(*d->pixmapSizeToBeTakenIntoUse);

        // Size taken into use
        delete d->pixmapSizeToBeTakenIntoUse;
        d->pixmapSizeToBeTakenIntoUse = NULL;
    }
}

QSizeF DuiExtensionHandleView::sizeHint(Qt::SizeHint which, const QSizeF &) const
{
    Q_D(const DuiExtensionHandleView);

    // Return the requested size hint bounded to forced minimum and maximum sizes defined in the style
    return d->remoteToHostCoordinates(model()->sizeHints().at(which)).expandedTo(style()->forcedMinimumSize()).boundedTo(style()->forcedMaximumSize());
}

void DuiExtensionHandleView::pixmapModified(const QRectF &rect)
{
    Q_D(DuiExtensionHandleView);

    // Unite the changed rectangle with the previous changed rectangle
    d->pixmapModifiedRect = rect.united(d->pixmapModifiedRect);

    // Update the local pixmap
    d->updateLocalPixmap();
}
