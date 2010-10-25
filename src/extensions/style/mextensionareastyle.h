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

#ifndef MEXTENSIONAREASTYLE_H_
#define MEXTENSIONAREASTYLE_H_

#include <MWidgetStyle>

/*!
 * MExtensionAreaStyle is the style class for MExtensionArea.
 */
class M_EXTENSIONS_EXPORT MExtensionAreaStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MExtensionAreaStyle)

    //! Whether the applets on this mashup canvas should be inside containers or not
    M_STYLE_ATTRIBUTE(bool, containerMode, ContainerMode)
};

/*!
 * MExtensionAreaStyleContainer is the style container class for MExtensionArea.
 */
class M_EXTENSIONS_EXPORT MExtensionAreaStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MExtensionAreaStyle)
};

#endif /* MEXTENSIONAREASTYLE_H_ */
