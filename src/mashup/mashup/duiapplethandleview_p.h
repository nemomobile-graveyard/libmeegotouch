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

#ifndef DUIAPPLETHANDLEVIEW_P_H_
#define DUIAPPLETHANDLEVIEW_P_H_

#include "private/duiwidgetview_p.h"

class DuiAppletHandleView;
class DuiAppletHandle;
class QPixmap;
class DuiProgressIndicator;

class DuiAppletHandleViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiAppletHandleView)

public:
    DuiAppletHandleViewPrivate(DuiAppletHandle *appletHandle);
    virtual ~DuiAppletHandleViewPrivate();

    //! The controller
    DuiAppletHandle *appletHandle;

    //! A local copy of the latest applet pixmap
    QPixmap localPixmap;

    //! The applet pixmap that has been taken into use
    QPixmap *pixmapTakenIntoUse;

    //! The applet pixmap to be taken into use
    QPixmap *pixmapToBeTakenIntoUse;

    //! The size for the pixmap not taken into use yet
    QSizeF *pixmapSizeToBeTakenIntoUse;

    //! Progress indicator for showing feedback about various applet handle states
    DuiProgressIndicator *progressIndicator;

    //! Unprocessed changed rectangle
    QRectF pixmapModifiedRect;

    //! Previous geometry
    QRectF oldGeometry;

    //! Connects the signals
    void connectSignals();

    //! Clears the view state for a newly initialized applet
    void resetView();

    //! Draws graphics on top of the applet to show the broken state.
    virtual void drawAppletBrokenState();

    //! Handles changes in the applet size
    void appletSizeChanged(QSizeF size);

    //! Allocates a new applet pixmap to be taken into use
    void allocatePixmapToBeTakenIntoUse(QSizeF size);

    //! Updates the local applet pixmap if necessary
    void updateLocalPixmap();

    /*! Shows a dialog for deciding what to do with a broken applet. */
    void showAppletBrokenDialog();

    /*! Shows a dialog for deciding what to do with an installation failed applet. */
    void showAppletInstallationFailedDialog(const QString &error);

    /*! Deletes the pixmaps */
    virtual void destroyPixmaps();

    //! Sends the geometry of the applet handle to the applet
    void sendGeometryMessage();

    /*!
     * \brief Draw the contents of the applet pixmap
     *
     * \param painter the painter to draw with
     * \param sourceGeometry the part of the applet pixmap to use
     * \param targetGeometry the part of the target pixmap to draw to
     * \param brokenState \c true if the applet pixmap should be drawn in a broken state, \c false otherwise
     */
    virtual void drawAppletPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const;

    //! Converts a size from host coordinates to applet coordinates
    QSizeF hostToAppletCoordinates(const QSizeF &size) const;

    //! Converts a size from applet coordinates to host coordinates
    QSizeF appletToHostCoordinates(const QSizeF &size) const;

    //! Calculates the applet scale based on the size hints and available size
    void calculateAppletScale();
};

#endif /* DUIAPPLETHANDLEVIEW_P_H_ */
