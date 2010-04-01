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

#ifndef DUIBUTTONSWITCHVIEW_P_H
#define DUIBUTTONSWITCHVIEW_P_H

#include "duibuttonview_p.h"

class DuiButtonSwitchViewPrivate : public DuiButtonViewPrivate
{
    Q_DECLARE_PUBLIC(DuiButtonSwitchView)

public:
    DuiButtonSwitchViewPrivate();
    ~DuiButtonSwitchViewPrivate();

    QSize thumbSize() const;
    QPoint thumbPos() const;


    int mouseOffset;

    bool m_thumbDown;
    bool m_thumbDragged;
    QPoint m_thumbPos;
};

#endif
