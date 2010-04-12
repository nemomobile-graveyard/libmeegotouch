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

#ifndef MEXTENSIONHANDLEVIEW_H
#define MEXTENSIONHANDLEVIEW_H

#include <mwidgetview.h>
#include "mextensionhandle.h"
#include "mextensionhandlemodel.h"
#include "mextensionhandlestyle.h"

class MExtensionHandleViewPrivate;

//! \internal
/*!
 * MExtensionHandleView is a view for the MExtensionHandle that
 * renders the extension's pixmap, shows whether the extension is in
 * a broken state or not and displays a dialog if the extension
 * is broken and the user taps it.
 */
class MExtensionHandleView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MExtensionHandleModel, MExtensionHandleStyle)

public:
    /*!
     * Destroyes the MExtensionHandleView.
     */
    virtual ~MExtensionHandleView();

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
     * Constructs a MExtensionHandleView. Should be used from derived classes.
     *
     * \param dd the private class
     * \param handle the MExtensionHandle controller to be used
     */
    MExtensionHandleView(MExtensionHandleViewPrivate &dd, MExtensionHandle *handle);
    MExtensionHandleViewPrivate *const d_ptr;

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
    Q_DISABLE_COPY(MExtensionHandleView)
    Q_DECLARE_PRIVATE(MExtensionHandleView)
};
//! \internal_end

#endif // MEXTENSIONHANDLEVIEW_H
