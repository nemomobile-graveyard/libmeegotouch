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


#ifndef MLISTHEADERSTYLE_H
#define MLISTHEADERSTYLE_H

#include <MWidgetStyle>

/*! \internal */

class MListHeaderStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MListHeaderStyle)

    M_STYLE_ATTRIBUTE(QString, titleStyleName, TitleStyleName)
    M_STYLE_ATTRIBUTE(QString, separatorStyleName, SeparatorStyleName)
};

class MListHeaderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MListHeaderStyle)
};

/*! \internal_end */

#endif // MLISTHEADERSTYLE_H
