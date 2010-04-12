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

#ifndef UT_MAPPLETCANCELMESSAGE_H
#define UT_MAPPLETCANCELMESSAGE_H

#include <QObject>

class MAppletCancelMessage;

class Ut_MAppletCancelMessage : public QObject
{
    Q_OBJECT

private:
    MAppletCancelMessage *message;

private slots:
    void init();
    void cleanup();

    void testDefaultAppletCancelMessageType();
};

#endif // UT_MAPPLETCANCELMESSAGE_H
