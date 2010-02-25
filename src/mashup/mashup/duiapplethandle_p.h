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

#ifndef DUIAPPLETHANDLE_P_H
#define DUIAPPLETHANDLE_P_H

#include "duiapplethandle.h"
#include "duiextensionhandle_p.h"

class DuiSettingsLanguageBinary;

/*!
 * A private data class for DuiAppletHandle.
 */
class DuiAppletHandlePrivate : public DuiExtensionHandlePrivate
{
    Q_DECLARE_PUBLIC(DuiAppletHandle)
    Q_OBJECT

public:
    /*!
     * Constructor.
     */
    DuiAppletHandlePrivate();

    /*!
     * Destructor.
     */
    virtual ~DuiAppletHandlePrivate();

    //! A list of applet instance settings binaries read during initialization
    QList<DuiSettingsLanguageBinary *> instanceSettingsBinaries;

    //! A list of applet global settings binaries read during initialization
    QList<DuiSettingsLanguageBinary *> globalSettingsBinaries;
};

#endif
