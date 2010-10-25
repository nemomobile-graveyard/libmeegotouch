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

#ifndef MAPPLETUPDATEGEOMETRYMESSAGE_H
#define MAPPLETUPDATEGEOMETRYMESSAGE_H

#include "mappletmessage.h"
#include <QVector>
#include <QSizeF>

/*!
 * Update geometry message.
 */
class M_EXTENSIONS_EXPORT MAppletUpdateGeometryMessage : public MAppletMessage
{
private:
    // Size hints
    QVector<QSizeF> _sizeHints;

public:
    /*!
     * Constructs a MAppletUpdateGeometryMessage.
     *
     * \param sizeHints a vector containing size hints
     */
    explicit MAppletUpdateGeometryMessage(const QVector<QSizeF> &sizeHints = QVector<QSizeF>(Qt::NSizeHints));

    /*!
     * Destroys the MAppletUpdateGeometryMessage.
     */
    virtual ~MAppletUpdateGeometryMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the size hints of the message.
     * \return a vector containing the size hints.
     */
    QVector<QSizeF> sizeHints() const;

    /*!
     * Sets the size hints of the message.
     * \param sizeHints the new size hints.
     */
    void setSizeHints(const QVector<QSizeF> &sizeHints);
};

#endif // MAPPLETUPDATEGEOMETRYMESSAGE_H
