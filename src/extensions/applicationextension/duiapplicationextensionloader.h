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


#ifndef DUIAPPLICATIONEXTENSIONLOADER_H_
#define DUIAPPLICATIONEXTENSIONLOADER_H_

#include "duiexport.h"

class DuiApplicationExtensionInterface;
class DuiApplicationExtensionMetaData;

//! \internal
/*!
 * \brief A class that loads DUI application extensions.
 */
class DuiApplicationExtensionLoader
{
public:
    /*!
     * Loads an application extension which implements the given interface.
     * The ownership of the returned object is transfered to the caller.
     * \param metadata the metadata that is used to load the application extension.
     * \return the extension object or \c NULL in an error.
     */
    static DuiApplicationExtensionInterface *loadExtension(const DuiApplicationExtensionMetaData &metadata);

private:
    /*!
     * A private default constructor so no objects of this class can be created.
     */
    DuiApplicationExtensionLoader();
};
//! \internal_end

#endif /* DUIAPPLICATIONEXTENSIONLOADER_H_ */
