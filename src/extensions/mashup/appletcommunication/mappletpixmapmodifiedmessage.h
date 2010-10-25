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

#ifndef MAPPLETPIXMAPMODIFIEDMESSAGE_H
#define MAPPLETPIXMAPMODIFIEDMESSAGE_H

#include "mappletmessage.h"

#include <QRectF>

/*!
 * An applet message for notifying the host that a part of the applet pixmap has been modified.
 */
class M_EXTENSIONS_EXPORT MAppletPixmapModifiedMessage : public MAppletMessage
{
private:
    //! The geometry of the modified region.
    QRectF _geometry;

public:
    /*!
     * Constructs a MAppletPixmapModifiedMessage.
     *
     * The geometry of the modified region can be set as a parameter.
     * The default is a null rectangle.
     *
     * \param geometry the geometry of the modified region.
     */
    explicit MAppletPixmapModifiedMessage(const QRectF &geometry = QRectF());

    /*!
     * Destroys the MAppletPixmapModifiedMessage.
     */
    virtual ~MAppletPixmapModifiedMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the geometry of the modified region.
     *
     * \return the geometry of the modified region
     */
    QRectF geometry() const;

    /*!
     * Sets the geometry of the modified region.
     *
     * \param newGeometry the new geometry of the modified region
     */
    void setGeometry(const QRectF &newGeometry);
};

#endif // MAPPLETPIXMAPMODIFIEDMESSAGE_H
