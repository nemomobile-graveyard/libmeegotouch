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

#ifndef DUIEXTENSIONHANDLEVIEW_P_H_
#define DUIEXTENSIONHANDLEVIEW_P_H_

#include "private/duiwidgetview_p.h"

class DuiExtensionHandleView;
class DuiExtensionHandle;
class QPixmap;
class DuiProgressIndicator;

class DuiExtensionHandleViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiExtensionHandleView)

public:
    DuiExtensionHandleViewPrivate(DuiExtensionHandle *handle);
    virtual ~DuiExtensionHandleViewPrivate();

    //! The controller
    DuiExtensionHandle *handle;

    //! A local copy of the latest remote pixmap
    QPixmap localPixmap;

    //! The pixmap that has been taken into use
    QPixmap *pixmapTakenIntoUse;

    //! The pixmap to be taken into use
    QPixmap *pixmapToBeTakenIntoUse;

    //! The size for the pixmap not taken into use yet
    QSizeF *pixmapSizeToBeTakenIntoUse;

    //! Progress indicator for showing feedback about various states
    DuiProgressIndicator *progressIndicator;

    //! Unprocessed changed rectangle
    QRectF pixmapModifiedRect;

    //! Previous geometry
    QRectF oldGeometry;

    //! Connects the signals
    void connectSignals();

    //! Clears the view state after initialization
    void resetView();

    //! Draws graphics on top of the pixmap to show the broken state.
    virtual void drawBrokenState();

    //! Handles changes in size
    void sizeChanged(QSizeF size);

    //! Allocates a new pixmap to be taken into use
    void allocatePixmapToBeTakenIntoUse(QSizeF size);

    //! Updates the local pixmap if necessary
    void updateLocalPixmap();

    /*! Shows a dialog for deciding what to do with a broken state. */
    virtual void showBrokenDialog() = 0;

    /*! Shows a dialog for deciding what to do when installation failed. */
    virtual void showInstallationFailedDialog(const QString &error) = 0;

    /*! Deletes the pixmaps */
    virtual void destroyPixmaps();

    //! Sends the geometry of the handle to the remote process
    void sendGeometryMessage();

    /*!
     * \brief Draw the contents of the pixmap
     *
     * \param painter the painter to draw with
     * \param sourceGeometry the part of the pixmap to use
     * \param targetGeometry the part of the target pixmap to draw to
     * \param brokenState \c true if the pixmap should be drawn in a broken state, \c false otherwise
     */
    virtual void drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const = 0;

    //! Converts a size from host coordinates to remote coordinates
    QSizeF hostToRemoteCoordinates(const QSizeF &size) const;

    //! Converts a size from remote coordinates to host coordinates
    QSizeF remoteToHostCoordinates(const QSizeF &size) const;

    //! Calculates the scale based on the size hints and available size
    void calculateScale();
};

#endif /* DUIEXTENSIONHANDLEVIEW_P_H_ */
