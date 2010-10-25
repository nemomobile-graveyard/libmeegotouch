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

#ifndef MAPPLETINSTALLATIONSOURCEINTERFACE_H
#define MAPPLETINSTALLATIONSOURCEINTERFACE_H

#include <QtPlugin>
#include "mexport.h"
#include <mapplicationextensioninterface.h>

class MWidget;
class MAppletInstantiator;


/**
  * MAppletInventoryInterface is an interface the applet installation sources can use to instantiate applets in a package
  */
class MAppletInventoryInterface
{
public:
/*!
 * Called to instantiate an applet from a package
 *
 * \param packageName the name of the applet package to be instantiated
 */
virtual void instantiateAppletsInPackage(const QString &packageName) = 0;
};

/**
 * MAppletInstallationSourceInterface is the base class for applet installation source classes.
 *
 * Applet installation source developers need to implement this interface in a plugin library
 * and export their implementation from the library. The applet inventory will instantiate
 * the available installation sources when needed.
 *
 * \see \ref appletinstallationsource
 */
class M_EXTENSIONS_EXPORT MAppletInstallationSourceInterface : public MApplicationExtensionInterface
{
    Q_INTERFACES(MApplicationExtensionInterface)

public:
 /*!
  * Destructor.
  */
 virtual ~MAppletInstallationSourceInterface() {}

  /*!
   * Tells the extension about an implementation of the applet inventory interface.
   *
   * \param installationSource Applet inventory interface for instantiating applets
   */
  virtual void setMAppletInventoryInterface(MAppletInventoryInterface &installationSource) = 0;
};

Q_DECLARE_INTERFACE(MAppletInstallationSourceInterface, "com.meego.core.MAppletInstallationSourceInterface/1.0")

#endif // MAPPLETINSTALLATIONSOURCEINTERFACE_H
