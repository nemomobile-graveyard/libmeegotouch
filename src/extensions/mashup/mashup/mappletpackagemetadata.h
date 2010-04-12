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

#ifndef MAPPLETPACKAGEMETADATA_H_
#define MAPPLETPACKAGEMETADATA_H_

#include <QString>

//! \internal
namespace MAppletPackageMetaData
{
    //! Name of the package name key
    const QString PACKAGE_KEY = "Package";
    //! Name of the .desktop file name key
    const QString DESKTOP_FILE_KEY = "Maemo-Desktop-File";
    //! Name of the applet title key
    const QString APPLET_TITLE_KEY = "Maemo-Applet-Title";
    //! Name of the applet size in portrait key
    const QString APPLET_SIZE_PORTRAIT_KEY = "Maemo-Applet-Size-Portrait";
    //! Name of the applet size in landscape key
    const QString APPLET_SIZE_LANDSCAPE_KEY = "Maemo-Applet-Size-Landscape";
    //! Name of the icon key
    const QString ICON_KEY = "Maemo-Icon";
}
//! \internal_end

#endif /* MAPPLETPACKAGEMETADATA_H_ */
