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

#ifndef DUIGRIDITEMVIEW_P_H
#define DUIGRIDITEMVIEW_P_H

class QPixmap;
class QGraphicsGridLayout;
class DuiGridItem;
class DuiImageWidget;
class DuiLabel;

class DuiGridItemViewPrivate
{
    Q_DECLARE_PUBLIC(DuiGridItemView)

protected:
    DuiGridItemView *q_ptr;

public:
    DuiGridItemViewPrivate();
    virtual ~DuiGridItemViewPrivate();

    virtual void init();
    virtual void initLayout() const;

    inline void initTitle() const;
    inline void initSubtitle() const;
    inline void initImage() const;

    void setTitle(const QString &text);
    void setSubtitle(const QString &text);
    void setImage(const QString &name);
    void _q_updateImage();

    DuiGridItem *controller;
    QGraphicsGridLayout    *layout;
    mutable DuiLabel       *title;
    mutable DuiLabel       *subtitle;
    mutable DuiImageWidget *image;
    mutable bool    layoutDirty;
    Qt::Alignment   iconAlign;
};

#endif
