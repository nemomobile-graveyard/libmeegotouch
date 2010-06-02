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

#ifndef MCONTENTITEM_P_H__
#define MCONTENTITEM_P_H__

#include "private/mwidgetcontroller_p.h"
#include <QPixmap>
#include <QImage>
#include <MLabel>

class MContentItemPrivate : public MWidgetControllerPrivate
{
public:
    MContentItemPrivate();
    virtual ~MContentItemPrivate();

    QPixmap pixmap;
    QImage image;
    QPixmap optionalPixmap;
    QImage optionalImage;
    MLabel* smallText;
};

#endif
