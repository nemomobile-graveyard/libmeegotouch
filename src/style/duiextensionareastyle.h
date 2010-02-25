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

#ifndef DUIEXTENSIONAREASTYLE_H_
#define DUIEXTENSIONAREASTYLE_H_

#include <DuiWidgetStyle>

/*!
 * DuiExtensionAreaStyle is the style class for DuiExtensionArea.
 */
class DUI_EXPORT DuiExtensionAreaStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiExtensionAreaStyle)

    //! Whether the applets on this mashup canvas should be inside containers or not
    DUI_STYLE_ATTRIBUTE(bool, containerMode, ContainerMode)
};

/*!
 * DuiExtensionAreaStyleContainer is the style container class for DuiExtensionArea.
 */
class DUI_EXPORT DuiExtensionAreaStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiExtensionAreaStyle)
};

#endif /* DUIEXTENSIONAREASTYLE_H_ */
