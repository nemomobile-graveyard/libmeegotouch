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

#ifndef MAPPLETSETGEOMETRYMESSAGE_H
#define MAPPLETSETGEOMETRYMESSAGE_H

#include "mappletmessage.h"

#include <QRectF>

/*!
 * An applet message for setting the geometry of a \c QGraphicsLayoutItem.
 */
class M_EXTENSIONS_EXPORT MAppletSetGeometryMessage : public MAppletMessage
{
private:
    //! The geometry property of the message.
    QRectF _geometry;

    //! The X pixmap handle.
    Qt::HANDLE _handle;

public:
    /*!
     * Constructor.
     *
     * The geometry can be set as a parameter. The default is a null rectangle.
     * \param geometry the geometry property of the message.
     * \param handle the pixmap handle
     */
    explicit MAppletSetGeometryMessage(const QRectF &geometry = QRectF(), Qt::HANDLE handle = 0);

    /*!
     * Destructor.
     */
    virtual ~MAppletSetGeometryMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the geometry property of the message.
     * \return the geometry.
     */
    QRectF geometry() const;

    /*!
     * Sets the geometry property of the message.
     * \param newGeometry the new geometry.
     */
    void setGeometry(const QRectF &newGeometry);

    /*!
     * Returns the X pixmap handle property of the message.
     * \return the X pixmap handle.
     */
    Qt::HANDLE handle() const;

    /*!
     * Sets the X pixmap handle property of the message.
     * \param newHandle the new X pixmap handle.
     */
    void setHandle(Qt::HANDLE newHandle);
};

#endif // MAPPLETSETGEOMETRYMESSAGE_H
