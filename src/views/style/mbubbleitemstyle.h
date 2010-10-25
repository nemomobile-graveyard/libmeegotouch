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

#ifndef MBUBBLEITEMSTYLE_H
#define MBUBBLEITEMSTYLE_H

#include <mwidgetstyle.h>

/*!
 * \internal
 */

class M_VIEWS_EXPORT MBubbleItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MBubbleItemStyle)

    M_STYLE_ATTRIBUTE(QFont,                  font,                     Font)
    M_STYLE_ATTRIBUTE(QColor,                 textColor,                TextColor)

    M_STYLE_ATTRIBUTE(QFont,                  timestampFont,            TimestampFont)
    M_STYLE_ATTRIBUTE(QColor,                 timestampTextColor,       TimestampTextColor)

    M_STYLE_ATTRIBUTE(QString,                avatarObjectName,   AvatarObjectName)
    M_STYLE_ATTRIBUTE(QString,                bubbleObjectName,   BubbleObjectName)
};

class M_VIEWS_EXPORT MBubbleItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MBubbleItemStyle)

    M_STYLE_MODE(Outgoing)
};

/*! \internal_end */

#endif // MBUBBLEITEMSTYLE_H
