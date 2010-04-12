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

#ifndef MAPPLETID_H
#define MAPPLETID_H

#include <QString>

//! \internal

/*!
 * \class MAppletId
 * \brief MAppletId implements the applet instance identifier.
 * It consists of application name, mashup canvas name and applet instance id.
 * This class is used to uniquely identify an applet instance in the system.
 *
 */
class MAppletId
{
public:
    //! Type for the applet instance ids.
    typedef uint AppletInstanceID;

    /*!
     * \brief Constructs MAppletId with empty application name, empty mashup canvas name and applet instance id 0
     */
    MAppletId();

    /*!
     * \brief Creates the MAppletId object with the given parameters
     * \param applicationName  Name of application that applet instance belongs to
     * \param mashupCanvasName Applet mashup canvas name
     * \param instanceId Applet instance id
     */
    MAppletId(const QString &applicationName, const QString &mashupCanvasName, const AppletInstanceID &instanceId);

    /*!
     * \brief Creates the MAppletId object from a string in
     * &lt;application name&gt;/&lt;mashup name&gt;/&lt;applet instance id&gt; format.
     *
     * \param appletId the string to create a MAppletId from
     */
    explicit MAppletId(const QString &appletId);

    /*!
     * \brief Destructor
     */
    virtual ~MAppletId();

    /*!
     * \brief Return the applet global identifier.
     * The returned string will be of the format
     * &lt;application name&gt;/&lt;mashup name&gt;/&lt;applet instance id&gt;
     */
    QString toString() const;

    /*!
     * \brief Returns the application name.
     */
    QString applicationName() const;

    /*!
     * \brief Returns the mashup canvas name.
     */
    QString mashupCanvasName() const;

    /*!
     * \brief Returns the applet instance id.
     */
    AppletInstanceID instanceId() const;

    /*!
     * Compares a MAppletId with another MAppletId
     *
     * \param other the MAppletId to compare to
     * \return true if the MAppletIds differ, false otherwise
     */
    inline bool operator!=(const MAppletId &other) const {
        return other.applicationName_ != applicationName_ || other.mashupCanvasName_ != mashupCanvasName_ || other.appletInstanceId != appletInstanceId;
    }

private:
    //! Name of the application
    QString applicationName_;

    //! Mashup canvas for the applet instances
    QString mashupCanvasName_;

    //! Applet instance id
    AppletInstanceID appletInstanceId;
};

//! \internal_end

#endif // MAPPLETID_H
