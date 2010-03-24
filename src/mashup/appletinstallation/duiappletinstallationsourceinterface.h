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

#ifndef DUIAPPLETINSTALLATIONSOURCEINTERFACE_H
#define DUIAPPLETINSTALLATIONSOURCEINTERFACE_H

#include <QtPlugin>
#include "duiexport.h"
#include <duiapplicationextensioninterface.h>

class DuiWidget;
class DuiAppletInstantiator;

/**
 * DuiAppletInstallationSourceInterface is the base class for applet installation source classes.
 *
 * Applet installation source developers need to implement this interface in a plugin library
 * and export their implementation from the library. The applet inventory will instantiate
 * the available installation sources when needed.
 *
 * \see \ref appletdevelopment
 */
class DUI_EXPORT DuiAppletInstallationSourceInterface : public DuiApplicationExtensionInterface
{
    Q_INTERFACES(DuiApplicationExtensionInterface)

public:
    /*!
     * Destructor.
     */
    virtual ~DuiAppletInstallationSourceInterface() {}
};

Q_DECLARE_INTERFACE(DuiAppletInstallationSourceInterface, "com.nokia.dui.core.AppletInstallationSourceInterface/1.0")

#endif // DUIAPPLETINSTALLATIONSOURCEINTERFACE_H
