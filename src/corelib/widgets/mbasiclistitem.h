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
 
#ifndef MBASICLISTITEM_H
#define MBASICLISTITEM_H

#include "mlistitem.h"

class MImageWidget;
class MLabel;
class QGraphicsGridLayout;

class MBasicListItemPrivate;

class M_EXPORT MBasicListItem : public MListItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    Q_PROPERTY(MImageWidget* imageWidget READ imageWidget WRITE setImageWidget)

public:
    enum ItemStyle {
        SingleTitle,
        TitleWithSubtitle,
        IconWithTitle,
        IconWithTitleAndSubtitle
    };

public:
    MBasicListItem(MBasicListItem::ItemStyle style = SingleTitle, QGraphicsItem *parent = NULL);
    virtual ~MBasicListItem();
    
    /*!
      \deprecated The method is no longer required to be called after constructing the item.
                  There shouldn't be any need for this function.
    */
    void initLayout();

    void setItemStyle(ItemStyle itemStyle);
    ItemStyle itemStyle() const;
    
    void setImageWidget(MImageWidget *imageWidget);
    MImageWidget *imageWidget();
    
    void setTitle(const QString &title);
    QString title();
    
    void setSubtitle(const QString &subtitle);
    QString subtitle();
    
protected:
    MLabel *titleLabelWidget();
    MLabel *subtitleLabelWidget();

    virtual QGraphicsLayout *createLayout();
    virtual void clearLayout();

    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    Q_DECLARE_PRIVATE(MBasicListItem)
    MBasicListItemPrivate *d_ptr;
};

#endif // MBASICLISTITEM_H
