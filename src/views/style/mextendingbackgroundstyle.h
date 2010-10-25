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

#ifndef MEXTENDINGBACKGROUNDSTYLE_H
#define MEXTENDINGBACKGROUNDSTYLE_H

#include <mwidgetstyle.h>

/*!
 * Style class for MExtendingBackgroundView
 */
class M_VIEWS_EXPORT MExtendingBackgroundStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MExtendingBackgroundStyle)

    //! The direction where to extend the background beyond the screen edges (left/right/top/bottom)
    M_STYLE_ATTRIBUTE(QString, extendDirection, ExtendDirection)
};

class M_VIEWS_EXPORT MExtendingBackgroundStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MExtendingBackgroundStyle)
};

#endif
