// Make doxygen skip this internal class
//! \internal
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

#ifndef MSCENEEVENTEATER_H
#define MSCENEEVENTEATER_H

#include <QGraphicsWidget>

class MSceneEventEater : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit MSceneEventEater(QGraphicsItem *parent = 0);
    bool event(QEvent *event);
};

#endif // MSCENEEVENTEATER_H
//! \internal_end
