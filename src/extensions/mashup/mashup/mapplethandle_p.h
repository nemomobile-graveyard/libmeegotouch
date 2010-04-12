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

#ifndef MAPPLETHANDLE_P_H
#define MAPPLETHANDLE_P_H

#include "mapplethandle.h"
#include "mextensionhandle_p.h"

class MSettingsLanguageBinary;

/*!
 * A private data class for MAppletHandle.
 */
class MAppletHandlePrivate : public MExtensionHandlePrivate
{
    Q_DECLARE_PUBLIC(MAppletHandle)
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    MAppletHandlePrivate();

    /*!
     * Destructor.
     */
    virtual ~MAppletHandlePrivate();

    //! A list of applet instance settings binaries read during initialization
    QList<MSettingsLanguageBinary *> instanceSettingsBinaries;

    //! A list of applet global settings binaries read during initialization
    QList<MSettingsLanguageBinary *> globalSettingsBinaries;
};

#endif
