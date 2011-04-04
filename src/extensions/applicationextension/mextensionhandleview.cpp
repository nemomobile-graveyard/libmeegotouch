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

#include "mextensionhandleview.h"
#include "mextensionhandleview_p.h"

#include "maction.h"
#include "mappletsharedmutex.h"
#include "mprogressindicator.h"
#include "mmessagebox.h"
#include "mlocale.h"
#include "mbutton.h"
#include "mappletsettingsdialog.h"
#include "mappletsettings.h"
#include "mscenemanager.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#endif

static void destroySharedXPixmap(QPixmap *qPixmap)
{
    if (qPixmap != NULL) {
#ifdef Q_WS_X11
        Pixmap xPixmap = qPixmap->handle();
        delete qPixmap;
        if (xPixmap != 0) {
            XFreePixmap(QX11Info::display(), xPixmap);
        }
#endif
    }
}

MExtensionHandleViewPrivate::MExtensionHandleViewPrivate(MExtensionHandle *handle) :
    controller(handle),
    handle(handle),
    pixmapTakenIntoUse(NULL),
    pixmapToBeTakenIntoUse(NULL),
    pixmapSizeToBeTakenIntoUse(NULL),
    progressIndicator(new MProgressIndicator(handle)),
    q_ptr(NULL)
{
    // The progress indicator is not visible by default
    progressIndicator->setVisible(false);
}

MExtensionHandleViewPrivate::~MExtensionHandleViewPrivate()
{
    destroyPixmaps();
}

void MExtensionHandleViewPrivate::connectSignals()
{
    Q_Q(MExtensionHandleView);

    // Listen for pixmap taken into use signals
    QObject::connect(handle, SIGNAL(pixmapTakenIntoUse(Qt::HANDLE)), q, SLOT(pixmapTakenIntoUse(Qt::HANDLE)));

    // Listen for scene changed signals
    QObject::connect(handle, SIGNAL(pixmapModified(QRectF)), q, SLOT(pixmapModified(QRectF)));
}

void MExtensionHandleViewPrivate::destroyPixmaps()
{
    if (pixmapTakenIntoUse != NULL) {
        destroySharedXPixmap(pixmapTakenIntoUse);
        pixmapTakenIntoUse = NULL;
    }
    if (pixmapToBeTakenIntoUse != NULL) {
        destroySharedXPixmap(pixmapToBeTakenIntoUse);
        pixmapToBeTakenIntoUse = NULL;
    }
}

void MExtensionHandleViewPrivate::resetView()
{
    destroyPixmaps();

    // Reset the old geometry to force the pixmap ID to be sent to the remote process
    oldGeometry = QRectF();
}

void MExtensionHandleViewPrivate::drawBrokenState()
{
    Q_Q(MExtensionHandleView);

    q->update();
}

QSizeF MExtensionHandleViewPrivate::hostToRemoteCoordinates(const QSizeF &size) const
{
    Q_Q(const MExtensionHandleView);

    return size / q->model()->scale();
}

QSizeF MExtensionHandleViewPrivate::remoteToHostCoordinates(const QSizeF &size) const
{
    Q_Q(const MExtensionHandleView);

    return size * q->model()->scale();
}

void MExtensionHandleViewPrivate::calculateScale()
{
    Q_Q(MExtensionHandleView);

    // Set a scaling factor if the minimum width is larger than the available width
    const MExtensionHandleStyleContainer *handleStyle = dynamic_cast<const MExtensionHandleStyleContainer *>(&q->style());
    if (handleStyle != NULL) {
        qreal minimumWidth = q->model()->sizeHints().at(Qt::MinimumSize).width();
        qreal availableWidth = (*handleStyle)->forcedMaximumSize().width();
        q->model()->setScale(minimumWidth > availableWidth ? availableWidth / minimumWidth : 1);
    }
}

void MExtensionHandleViewPrivate::sizeChanged(QSizeF size)
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

void MExtensionHandleViewPrivate::allocatePixmapToBeTakenIntoUse(QSizeF size)
{
#ifdef Q_WS_X11
    // Allocate a new pixmap to be taken into use
    QSizeF pixmapSize = size.expandedTo(QSizeF(1, 1));
    Pixmap pixmap = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(), pixmapSize.width(), pixmapSize.height(), QX11Info::appDepth());
    QApplication::syncX();
    pixmapToBeTakenIntoUse = new QPixmap();
    *pixmapToBeTakenIntoUse = QPixmap::fromX11Pixmap(pixmap, QPixmap::ExplicitlyShared);

    // Clear the pixmap
    QPainter painter(pixmapToBeTakenIntoUse);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(pixmapToBeTakenIntoUse->rect(), QColor(0, 0, 0, 0));

    // Communicate the new geometry and pixmap handle to be taken into use to the runner
    handle->sendGeometryMessage(QRectF(QPointF(), pixmapSize), pixmapToBeTakenIntoUse->handle());
#endif
}

void MExtensionHandleViewPrivate::updateLocalPixmap()
{
    if (!pixmapModifiedRect.isEmpty() && pixmapTakenIntoUse != NULL && !pixmapTakenIntoUse->isNull()) {
        Q_Q(MExtensionHandleView);

        // Get the address of the pixmap mutex in shared memory and lock the mutex
        MAppletSharedMutex *mutex = q->model()->pixmapMutex();
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

MExtensionHandleView::MExtensionHandleView(MExtensionHandleViewPrivate &dd, MExtensionHandle *handle) :
    MWidgetView(handle),
    d_ptr(&dd)
{
    Q_D(MExtensionHandleView);
    d->q_ptr = this;
    d->connectSignals();
}

MExtensionHandleView::~MExtensionHandleView()
{
    delete d_ptr;
}

void MExtensionHandleView::setupModel()
{
    MWidgetView::setupModel();

    // Let the view know what's the state of the model
    QList<const char *> m;
    m << MExtensionHandleModel::Scale;
    m << MExtensionHandleModel::CurrentState;
    m << MExtensionHandleModel::InstallationProgress;
    updateData(m);
}

void MExtensionHandleView::updateData(const QList<const char *>& modifications)
{
    Q_D(MExtensionHandleView);

    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MExtensionHandleModel::CurrentState) {
            switch (model()->currentState()) {
            case MExtensionHandleModel::INSTALLING:
                d->progressIndicator->setViewType(MProgressIndicator::barType);
                d->progressIndicator->setRange(0, 100);
                d->progressIndicator->setVisible(true);
                d->progressIndicator->setUnknownDuration(false);
                d->progressIndicator->setValue(model()->installationProgress());
                break;
            case MExtensionHandleModel::BROKEN:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                d->drawBrokenState();
                break;
            case MExtensionHandleModel::STARTING:
                d->progressIndicator->setVisible(true);
                d->progressIndicator->setUnknownDuration(true);
                break;
            case MExtensionHandleModel::STOPPED:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                break;
            case MExtensionHandleModel::RUNNING:
                d->progressIndicator->setVisible(false);
                d->progressIndicator->setUnknownDuration(false);
                d->resetView();
                break;
            }
        } else if (member == MExtensionHandleModel::InstallationProgress) {
            d->progressIndicator->setValue(model()->installationProgress());
        }
    }
}

void MExtensionHandleView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    Q_D(const MExtensionHandleView);

    if (!d->localPixmap.isNull()) {
        // If there are modifications to be copied to the local pixmap try to copy them
        if (!d->pixmapModifiedRect.isEmpty()) {
            const_cast<MExtensionHandleViewPrivate *>(d)->updateLocalPixmap();
        }

        bool brokenState = model()->currentState() == MExtensionHandleModel::BROKEN || model()->currentState() == MExtensionHandleModel::STARTING;
        if (brokenState) {
            // In broken state use a specific opacity
            painter->setOpacity(style()->brokenOpacity());
        }

        // Draw the pixmap
        QRectF source(d->localPixmap.rect());
        QRectF destination(QPointF(), d->remoteToHostCoordinates(d->localPixmap.size()));
        d->drawPixmap(painter, source, destination, brokenState);
    }

    if (model()->currentState() == MExtensionHandleModel::BROKEN) {
        const QPixmap *brokenImage = style()->brokenImage();
        if (brokenImage != NULL) {
            QPoint p(size().width() - brokenImage->width(), 0);
            p += style()->brokenImageOffset();
            painter->setOpacity(1.0f);
            painter->drawPixmap(p, *brokenImage);
        }
    }
}

void MExtensionHandleView::setGeometry(const QRectF &rect)
{
    Q_D(MExtensionHandleView);

    // rect is always between minimum and maximum size because the size hints have been
    // bounded to these limits; calculate scaling factor if even the minimum size won't fit
    d->calculateScale();

    // Get the old size and the new size
    QSizeF oldSize = d->hostToRemoteCoordinates(d->oldGeometry.size());
    QSizeF newSize = d->hostToRemoteCoordinates(rect.size());

    // Apply the geometry locally immediately
    MWidgetView::setGeometry(rect);
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

QRectF MExtensionHandleView::boundingRect() const
{
    Q_D(const MExtensionHandleView);

    // Use the (downscaled) size as the bounding rectangle
    return QRectF(QPointF(), d->remoteToHostCoordinates(d->localPixmap.size()).expandedTo(style()->forcedMinimumSize()).boundedTo(style()->forcedMaximumSize()));
}

void MExtensionHandleView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MExtensionHandleView);

    // TODO: this should actually be handled by a mouse click, but at the
    // moment the base class doesn't contain such method...
    if (event->button() == Qt::LeftButton) {
        if (model()->currentState() == MExtensionHandleModel::BROKEN && model()->installationError().isEmpty()) {
            d->showBrokenDialog();
        } else if (model()->currentState() == MExtensionHandleModel::BROKEN && !model()->installationError().isEmpty()) {
            d->showInstallationFailedDialog(model()->installationError());
        }
    }
}

void MExtensionHandleView::pixmapTakenIntoUse(Qt::HANDLE)
{
    Q_D(MExtensionHandleView);

    // Delete the pixmap that was in use
    destroySharedXPixmap(d->pixmapTakenIntoUse);

    // Mark the new one as taken into use
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

QSizeF MExtensionHandleView::sizeHint(Qt::SizeHint which, const QSizeF &) const
{
    Q_D(const MExtensionHandleView);

    // Return the requested size hint bounded to forced minimum and maximum sizes defined in the style
    return d->remoteToHostCoordinates(model()->sizeHints().at(which)).expandedTo(style()->forcedMinimumSize()).boundedTo(style()->forcedMaximumSize());
}

void MExtensionHandleView::pixmapModified(const QRectF &rect)
{
    Q_D(MExtensionHandleView);

    // Unite the changed rectangle with the previous changed rectangle
    d->pixmapModifiedRect = rect.united(d->pixmapModifiedRect);

    // Update the local pixmap
    d->updateLocalPixmap();
}
