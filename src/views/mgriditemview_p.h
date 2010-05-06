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

#ifndef MGRIDITEMVIEW_P_H
#define MGRIDITEMVIEW_P_H

class QPixmap;
class QGraphicsGridLayout;
class MGridItem;
class MImageWidget;
class MLabel;

class MGridItemViewPrivate
{
    Q_DECLARE_PUBLIC(MGridItemView)

protected:
    MGridItemView *q_ptr;

public:
    MGridItemViewPrivate();
    virtual ~MGridItemViewPrivate();

    virtual void init();
    virtual void initLayout() const;

    inline void initTitle() const;
    inline void initSubtitle() const;
    inline void initImage() const;

    void setTitle(const QString &text);
    void setSubtitle(const QString &text);
    void setImage(const QString &name);
    void _q_updateImage();

    MGridItem *controller;
    QGraphicsGridLayout    *layout;
    mutable MLabel       *title;
    mutable MLabel       *subtitle;
    mutable MImageWidget *image;
    mutable bool    layoutDirty;
    Qt::Alignment   iconAlign;
};

#endif
