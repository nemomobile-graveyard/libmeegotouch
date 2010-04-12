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

#ifndef MAPPLETINVENTORYSTYLE_H
#define MAPPLETINVENTORYSTYLE_H

#include <QPoint>
#include <mextendingbackgroundstyle.h>

//! \internal
class MAppletInventoryStyle : public MExtendingBackgroundStyle
{
    Q_OBJECT
    M_STYLE(MAppletInventoryStyle)
};

class MAppletInventoryStyleContainer : public MExtendingBackgroundStyleContainer
{
    M_STYLE_CONTAINER(MAppletInventoryStyle)
};
//! \internal_end

#endif
