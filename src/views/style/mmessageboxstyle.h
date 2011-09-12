/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MMESSAGEBOXSTYLE_H
#define MMESSAGEBOXSTYLE_H

#include <mdialogstyle.h>

class M_VIEWS_EXPORT MMessageBoxStyle : public MDialogStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MMessageBoxStyle)

    /*!
        \property MMessageBoxStyle::iconStyleName
        \brief The style name which gets assigned to the internal icon.
    */
    M_STYLE_ATTRIBUTE(QString, iconStyleName, IconStyleName)

    /*!
        \property MMessageBoxStyle::titleStyleName
        \brief The style name which gets assigned to the internal title label.
    */
    M_STYLE_ATTRIBUTE(QString, titleStyleName, TitleStyleName)

    /*!
        \property MMessageBoxStyle::textStyleName
        \brief The style name which gets assigned to the internal body text label.
    */
    M_STYLE_ATTRIBUTE(QString, textStyleName, TextStyleName)
};

class M_VIEWS_EXPORT MMessageBoxStyleContainer : public MDialogStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MMessageBoxStyle)
};

#endif

