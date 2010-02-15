/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUIAPPLICATIONPRIVATE_MOCK_H
#define DUIAPPLICATIONPRIVATE_MOCK_H

#include <QString>
class DuiFeedbackPlayer;

class DuiApplicationPrivate
{
public:
    DuiApplicationPrivate(const QString &applicationName);
    virtual ~DuiApplicationPrivate();

    DuiFeedbackPlayer *feedbackPlayer;
};

#endif
