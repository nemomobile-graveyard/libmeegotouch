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

#ifndef MBUBBLEITEMBACKGROUNDSTYLE_H
#define MBUBBLEITEMBACKGROUNDSTYLE_H

#include <mwidgetstyle.h>

/*!
 * \internal
 */

class M_EXPORT MBubbleItemBackgroundStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MBubbleItemBackgroundStyle)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  incomingBackground,    IncomingBackground)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  incomingMirroredBackground,    IncomingMirroredBackground)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  outgoingBackground,    OutgoingBackground)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *,  outgoingMirroredBackground,    OutgoingMirroredBackground)
};

/*!
 * \internal
 */

class M_EXPORT MBubbleItemBackgroundStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MBubbleItemBackgroundStyle)
};

#endif

