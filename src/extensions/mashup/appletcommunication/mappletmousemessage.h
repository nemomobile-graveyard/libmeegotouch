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

#ifndef MAPPLETMOUSEMESSAGE_H
#define MAPPLETMOUSEMESSAGE_H

#include "mappletmessage.h"

#include <QPointF>

/*!
 * A base class for mouse event related applet messages.
 *
 * A mouse event contains the position of the mouse pointer when the event occurred.
 */
class M_EXTENSIONS_EXPORT MAppletMouseMessage : public MAppletMessage
{
private:
    /// Location of the mouse pointer in the event.
    QPointF _position;

    /// Mouse button that triggered the event.
    Qt::MouseButton _button;

    /// State of mouse buttons when this event was triggered.
    Qt::MouseButtons _buttons;

public:
    /*!
     * Constructor for the inheriting classes.
     * \param type the type of the applet message.
     * \param pos the position of the mouse pointer in the event.
     * \param button mouse button that triggered this event.
     * \param buttons state of mouse buttons when triggering this event.
     */
    MAppletMouseMessage(MAppletMessageType type, const QPointF &pos = QPointF(), Qt::MouseButton button = Qt::NoButton, Qt::MouseButtons buttons = Qt::NoButton);

    /*!
     * Destructor.
     */
    virtual ~MAppletMouseMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the mouse pointer position of the event.
     * \return the mouse pointer position.
     */
    QPointF position() const;

    /*!
     * Sets the mouse pointer position of the event.
     * \param pos the new position.
     */
    void setPosition(const QPointF &pos);

    /*!
     * Returns the enumerator of the mouse button of the event.
     * \return the mouse button enumerator.
     */
    Qt::MouseButton button() const;

    /*!
     * Sets the mouse button enumerator that triggered this
     * event.
     * \param button triggering mouse button.
     */
    void setButton(const Qt::MouseButton &button);

    /*!
     * Returns the state of mouse buttons when this event
     * was triggered.
     * \return state of mouse buttons.
     */
    Qt::MouseButtons buttons() const;

    /*!
     * Sets the state of mouse buttons when this event was triggered.
     * \param buttons state of mouse buttons.
     */
    void setButtons(const Qt::MouseButtons &buttons);
};

#endif // MAPPLETMOUSEMESSAGE_H
