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

class M_VIEWS_EXPORT MBubbleItemBackgroundStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MBubbleItemBackgroundStyle)

    M_STYLE_ATTRIBUTE(QString,                mirroredObjectName,   MirroredObjectName)
};

class M_VIEWS_EXPORT MBubbleItemBackgroundStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MBubbleItemBackgroundStyle)

    M_STYLE_MODE(Outgoing)
    M_STYLE_MODE(OutgoingPressed)
    M_STYLE_MODE(Incoming)
    M_STYLE_MODE(IncomingPressed)
};

/*! \internal_end */

#endif

