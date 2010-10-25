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
 
#ifndef MDETAILEDLISTITEM_H
#define MDETAILEDLISTITEM_H

#include "mlistitem.h"

class MDetailedListItemPrivate;
class MImageWidget;
class MLabel;
class MStylableWidget;

class QGraphicsGridLayout;

class M_CORE_EXPORT MDetailedListItem : public MListItem
{
public:
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    Q_PROPERTY(QString sideBottomTitle READ sideBottomTitle WRITE setSideBottomTitle)
    Q_PROPERTY(MImageWidget* imageWidget READ imageWidget WRITE setImageWidget)
    Q_PROPERTY(MImageWidget* sideTopImageWidget READ sideTopImageWidget)
    Q_PROPERTY(MImageWidget* sideBottomImageWidget READ sideBottomImageWidget)

public:
    enum ItemStyle {
        IconTitleSubtitleAndTwoSideIcons,
        IconTitleSubtitleAndSideIconWithLabel,
        ThumbnailTitleSubtitleAndTwoSideIcons,
        ThumbnailTitleAndTwoSideIcons
    };
    
    enum IconStyle {
        Icon,
        Thumbnail
    };

public:
    MDetailedListItem(MDetailedListItem::ItemStyle style = IconTitleSubtitleAndTwoSideIcons, QGraphicsItem *parent = NULL);
    virtual ~MDetailedListItem();
    
    /*!
      \deprecated The method is no longer required to be called after constructing the item.
                  There shouldn't be any need for this function.
    */
    void initLayout();
    
    void setItemStyle(ItemStyle itemStyle);
    ItemStyle itemStyle() const;
    
    void setIconStyle(IconStyle style);
    IconStyle iconStyle() const;
    
    void setImageWidget(MImageWidget *icon);
    MImageWidget *imageWidget();
    
    MImageWidget *sideTopImageWidget();
    MImageWidget *sideBottomImageWidget();
    
    void setTitle(const QString &title);
    QString title();
    
    void setSubtitle(const QString &subtitle);
    QString subtitle();
    
    void setSideBottomTitle(const QString &text);
    QString sideBottomTitle();
    
protected:
    MLabel *titleLabelWidget();
    MLabel *subtitleLabelWidget();
    MLabel *sideBottomLabelWidget();

    virtual QGraphicsLayout *createLayout();
    virtual void clearLayout();

    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    Q_DECLARE_PRIVATE(MDetailedListItem)
    MDetailedListItemPrivate *d_ptr;
};


#endif // MDETAILEDLISTITEM_H
