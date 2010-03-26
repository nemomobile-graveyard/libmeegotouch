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

#ifndef DUIMASHUPCANVAS_P_H
#define DUIMASHUPCANVAS_P_H

#include "private/duiwidgetcontroller_p.h"
#include "applicationextension/duiextensionarea_p.h"

class DuiDataStore;
class DuiAppletInstanceManager;

/*!
 * Private class for DuiMashupCanvas.
 */
class DuiMashupCanvasPrivate : public DuiExtensionAreaPrivate
{
    Q_DECLARE_PUBLIC(DuiMashupCanvas)

public:
    /*!
     * Constructor.
     */
    DuiMashupCanvasPrivate();

    /*!
     * Destructor.
     */
    virtual ~DuiMashupCanvasPrivate();

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     *
     * \param identifier the identifier for the mashup canvas.
     */
    void init(const QString &identifier);

    //! \reimp
    virtual void addWidget(DuiWidget *widget, DuiDataStore &store);
    //! \reimp_end

    //! Applet instance manager
    DuiAppletInstanceManager *instanceManager;

    //! The identifier of this canvas
    QString identifier;

    //! The D-Bus service address of this canvas
    QString serviceAddress;

    /*!
     * Provides a unique identifier in this process.
     * If the provided identifier is not previously used, the same identifier
     * is returned as is. If the identifier is previously used, some additional
     * characters are appended to the identifier to make it unique.
     * Also empty string is not accepted as an identifier but some characters are
     * provided as an identifier if an empty identifier is requested.
     * \param identifier the identifier.
     * \return a unique identifier.
     */
    static QString provideUniqueIdentifier(const QString &identifier);

    //! A list of used mashup canvas identifiers in this process
    static QSet<QString> allMashupCanvasIdentifiers;
};

#endif // DUIMASHUPCANVAS_P_H
