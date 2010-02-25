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

#ifndef DUIAPPLICATIONEXTENSIONAREASTYLE_H_
#define DUIAPPLICATIONEXTENSIONAREASTYLE_H_

#include "duiextensionareastyle.h"

/*!
 * DuiApplicationExtensionAreaStyle is the style class for DuiApplicationExtensionArea.
 */
class DUI_EXPORT DuiApplicationExtensionAreaStyle : public DuiExtensionAreaStyle
{
    Q_OBJECT
    DUI_STYLE(DuiApplicationExtensionAreaStyle)
};

/*!
 * DuiApplicationExtensionAreaStyleContainer is the style container class for DuiApplicationExtensionArea.
 */
class DUI_EXPORT DuiApplicationExtensionAreaStyleContainer : public DuiExtensionAreaStyleContainer
{
    DUI_STYLE_CONTAINER(DuiApplicationExtensionAreaStyle)
};

#endif /* DUIAPPLICATIONEXTENSIONAREASTYLE_H_ */
