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

#ifndef QTMAEMO6TESTSTYLEPRIVATE_H
#define QTMAEMO6TESTSTYLEPRIVATE_H

#include "qtmaemo6teststyle.h"

/*!
 * private part of the teststyle. This does nothing specific, it's just
 * used as base class for the final style's private class
 */
class QtMaemo6TestStylePrivate
{
    Q_DECLARE_PUBLIC(QtMaemo6TestStyle)
public:
            QtMaemo6TestStylePrivate():q_ptr(0) {};
    virtual ~QtMaemo6TestStylePrivate() {};

    void initStyle();

    bool m_styleInitialized;
protected:
    QtMaemo6TestStyle *q_ptr;
};

#endif
