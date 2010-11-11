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

#ifndef MASSEMBLY_H
#define MASSEMBLY_H

#include <mexport.h>
#include <QString>

// forward declarations
class MAssemblyPrivate;
class MWidgetController;
class MStyleSheet;
class MLogicalValues;

/*!
   \class MAssembly
   \brief This class provides the assembly information to MTheme which uses it to load the correct .css & .conf files.
 */
class M_CORE_EXPORT MAssembly
{
    MAssemblyPrivate *const d_ptr;
public:

    /*!
       \brief Constructs the MAssembly object.
     */
    MAssembly(const QString &assemblyName);

    /*!
       \brief Destroys the MAssembly object.
     */
    virtual ~MAssembly();

    /*!
       \brief Returns the name of the assembly.
     */
    QString name() const;

    /*!
       \brief Returns view type for widget.
     */
    QString viewType(const MWidgetController *widget, bool &exactMatch) const;

    /*!
       \brief Returns stylesheet for this assembly.
     */
    MStyleSheet *stylesheet() const;

    //! \deprecated Use themeChanged(const QStringList&) instead. Sice 0.20.54.
    Q_DECL_DEPRECATED void themeChanged(const QStringList &themeInheritance, const MLogicalValues &);

    /*!
       \brief Reloads all theme-related data
     */
    void themeChanged(const QStringList &themeInheritance);
};

#endif
