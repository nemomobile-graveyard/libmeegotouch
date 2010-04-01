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

#ifndef DUIEXTENSIONHANDLEVIEW_H
#define DUIEXTENSIONHANDLEVIEW_H

#include <duiwidgetview.h>
#include "duiextensionhandle.h"
#include "duiextensionhandlemodel.h"
#include "duiextensionhandlestyle.h"

class DuiExtensionHandleViewPrivate;

//! \internal
/*!
 * DuiExtensionHandleView is a view for the DuiExtensionHandle that
 * renders the extension's pixmap, shows whether the extension is in
 * a broken state or not and displays a dialog if the extension
 * is broken and the user taps it.
 */
class DuiExtensionHandleView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiExtensionHandleModel, DuiExtensionHandleStyle)

public:
    /*!
     * Destroyes the DuiExtensionHandleView.
     */
    virtual ~DuiExtensionHandleView();

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    //! \reimp_end

    /*!
     * Constructs a DuiExtensionHandleView. Should be used from derived classes.
     *
     * \param dd the private class
     * \param handle the DuiExtensionHandle controller to be used
     */
    DuiExtensionHandleView(DuiExtensionHandleViewPrivate &dd, DuiExtensionHandle *handle);
    DuiExtensionHandleViewPrivate *const d_ptr;

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    /*!
     * \brief A slot for receiving information about pixmaps that have been taken into use
     */
    void pixmapTakenIntoUse(Qt::HANDLE pixmapHandle);

    /*!
     * \brief A slot for processing the pixmap modifications
     */
    void pixmapModified(const QRectF &rect);

private :
    Q_DISABLE_COPY(DuiExtensionHandleView)
    Q_DECLARE_PRIVATE(DuiExtensionHandleView)
};
//! \internal_end

#endif // DUIEXTENSIONHANDLEVIEW_H
