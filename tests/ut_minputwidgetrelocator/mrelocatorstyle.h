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

#ifndef MRELOCATORSTYLE_H
#define MRELOCATORSTYLE_H

class QString;
class MSceneManager;
class MWidgetController;

class MRelocatorStyle
{
public:
    //! Vertical anchor position measured from the top of visible screen area.
    qreal verticalAnchorPosition() const;

    //! Margin on the top of visible screen area above which cursor will be relocated to anchor position.
    qreal topNoGoMargin() const;

    //! Margin on the bottom of visible screen area under which cursor will be relocated to anchor position.
    qreal bottomNoGoMargin() const;
}static gRelocatorStyle;

class MRelocatorStyleContainer
{
public:
    void initialize(const QString &, const QString &, const MWidgetController *){}
    void setModePhysicalKeyboard() const {}
    void setModeDefault() const {}
    void updateCurrentStyle() const {}
    const MRelocatorStyle* operator->() const
    {
        return &gRelocatorStyle;
    }
    void setSceneManager(MSceneManager *) {}
};

#endif
