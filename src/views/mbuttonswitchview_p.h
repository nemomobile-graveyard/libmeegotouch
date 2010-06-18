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

#ifndef MBUTTONSWITCHVIEW_P_H
#define MBUTTONSWITCHVIEW_P_H

#include "mbuttonview_p.h"

class QVariantAnimation;

class MButtonSwitchViewPrivate : public MButtonViewPrivate
{
    Q_DECLARE_PUBLIC(MButtonSwitchView)

public:
    MButtonSwitchViewPrivate();
    ~MButtonSwitchViewPrivate();

    QSize thumbSize() const;
    QPoint thumbPos() const;
    QPoint thumbEndPos(bool checked) const;

    const QPixmap& maskedSliderImage() const;
    
    void updateHandle();

    int mouseOffset;

    bool m_thumbDown;
    bool m_thumbDragged;
    QPoint m_thumbPos;
    bool m_thumbPosValid;
    
    //mutable so that image can be recreated in the drawContents() const 
    //method if needed, other solution would need a timer because of the 
    //asynchronous pixmap loading
    mutable QPixmap m_maskedSliderPm;

    QVariantAnimation* m_handleAnimation;
    int m_animationSpeed; //pixels per sec
};

#endif
