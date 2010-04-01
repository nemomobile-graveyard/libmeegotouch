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

#ifndef DUIAPPLETMESSAGE_H_
#define DUIAPPLETMESSAGE_H_

#include "duiexport.h"

#include <QDataStream>

/*!
 * An abstract base class for messages that are used for communicating between
 * applets and the host process. Messages can be serialized to a byte stream
 * so they can be communicated to another processes.
 */
class DUI_EXPORT DuiAppletMessage
{
public:
    /*!
     * A type for the message.
     */

    enum DuiAppletMessageType {
        InvalidMessage = 0,
        VisibilityMessage,
        OrientationMessage,

        MousePressMessage,
        MouseReleaseMessage,
        MouseMoveMessage,
        CancelMessage,

        SetGeometryMessage,

        PixmapTakenIntoUseMessage,

        AppletAliveMessageRequest,
        AppletAliveMessageResponse,

        AppletIconMessage,
        AppletTitleMessage,
        AppletTextMessage,

        UpdateGeometryMessage,

        PixmapModifiedMessage,

        ObjectMenuRequestMessage,
        ObjectMenuMessage,
        ObjectMenuActionSelectedMessage,

        NumMessages
    };

private:
    /// The type of this message
    DuiAppletMessageType messageType;

protected:
    /*!
     * Constructor.
     *
     * Sets the type of the message.
     * \param type the message type.
     */
    DuiAppletMessage(DuiAppletMessageType type);

public:
    /*!
     * Destructor.
     */
    virtual ~DuiAppletMessage();

    /*!
     * Returns the type of the applet message.
     * \return applet message type.
     */
    DuiAppletMessageType type() const;

    /*!
     * Serializes the contents of the message to a data stream.
     *
     * It doesn't matter what format the message uses for the serialization.
     * It is only required that the \c unserialize() method knows how to read the data.
     * \param stream the stream to serialize the message to.
     */
    virtual void serialize(QDataStream &stream) const;

    /*!
     * Unserializes the contents of the message from a data stream.
     *
     * \param stream the stream to serialize the message from.
     */
    virtual void unserialize(QDataStream &stream);
};


#endif /* DUIAPPLETMESSAGE_H_ */
