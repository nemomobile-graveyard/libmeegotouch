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

#ifndef DUIASSEMBLY_H
#define DUIASSEMBLY_H

#include <duiexport.h>
#include <QString>

// forward declarations
class DuiAssemblyPrivate;
class DuiWidgetController;
class DuiStyleSheet;
class DuiLogicalValues;

/*!
   \class DuiAssembly
   \brief This class provides the assembly information to DuiTheme which uses it to load the correct .css & .conf files.
 */
class DUI_EXPORT DuiAssembly
{
    DuiAssemblyPrivate *const d_ptr;
public:

    /*!
       \brief Constructs the DuiAssembly object.
     */
    DuiAssembly(const QString &assemblyName);

    /*!
       \brief Destroys the DuiAssembly object.
     */
    virtual ~DuiAssembly();

    /*!
       \brief Returns the name of the assembly.
     */
    QString name() const;

    /*!
       \brief Returns view type for widget.
     */
    QString viewType(const DuiWidgetController *widget, bool &exactMatch) const;

    /*!
       \brief Returns stylesheet for this assembly.
     */
    DuiStyleSheet *stylesheet() const;

    /*!
       \brief Reloads all theme-related data
     */
    void themeChanged(const QStringList &themeInheritance, const DuiLogicalValues &logicalValues);
};

#endif
