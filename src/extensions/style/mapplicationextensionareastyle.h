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

#ifndef MAPPLICATIONEXTENSIONAREASTYLE_H_
#define MAPPLICATIONEXTENSIONAREASTYLE_H_

#include "mextensionareastyle.h"

/*!
 * MApplicationExtensionAreaStyle is the style class for MApplicationExtensionArea.
 */
class M_EXTENSIONS_EXPORT MApplicationExtensionAreaStyle : public MExtensionAreaStyle
{
    Q_OBJECT
    M_STYLE(MApplicationExtensionAreaStyle)

    //! The orientation of the layout
    M_STYLE_ATTRIBUTE(Qt::Orientation, layoutOrientation, LayoutOrientation)
};

/*!
 * MApplicationExtensionAreaStyleContainer is the style container class for MApplicationExtensionArea.
 */
class M_EXTENSIONS_EXPORT MApplicationExtensionAreaStyleContainer : public MExtensionAreaStyleContainer
{
    M_STYLE_CONTAINER(MApplicationExtensionAreaStyle)
};

#endif /* MAPPLICATIONEXTENSIONAREASTYLE_H_ */
