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

#ifndef MADVACEDLISTITEM_H
#define MADVACEDLISTITEM_H

#include <MListItem>

class MAdvancedListItemPrivate;
class MImageWidget;
class MLabel;
class MProgressIndicator;

class QGraphicsGridLayout;

class M_CORE_EXPORT MAdvancedListItem : public MListItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)

    /*!
        \deprecated please use imageWidget property
    */
    Q_PROPERTY(MImageWidget* imagewidget READ imageWidget WRITE setImageWidget)

    /*!
        \deprecated please use sideTopImageWidget property
    */
    Q_PROPERTY(MImageWidget* sideTopImage READ sideTopImageWidget)

    /*!
        \deprecated please use sideBottomImageWidget property
    */
    Q_PROPERTY(MImageWidget* sideBottomImage READ sideBottomImageWidget)

    Q_PROPERTY(MImageWidget* imageWidget READ imageWidget WRITE setImageWidget)
    Q_PROPERTY(MImageWidget* sideTopImageWidget READ sideTopImageWidget)
    Q_PROPERTY(MImageWidget* sideBottomImageWidget READ sideBottomImageWidget)
    Q_PROPERTY(MProgressIndicator* progressIndicator READ progressIndicator)

public:
    enum ItemStyle {
        IconWithTitleProgressIndicatorAndTwoSideIcons,
        IconWithTitleProgressIndicatorAndTopSideIcon
    };

public:
    MAdvancedListItem(ItemStyle itemStyle = IconWithTitleProgressIndicatorAndTwoSideIcons, QGraphicsItem *parent = 0);
    virtual ~MAdvancedListItem();

    /*!
      \deprecated The method is no longer required to be called after constructing the item.
                  There shouldn't be any need for this function.
    */
    void initLayout();

    void setItemStyle(ItemStyle itemStyle);
    ItemStyle itemStyle() const;

    void setTitle(const QString &title);
    QString title();

    void setImageWidget(MImageWidget *imageWidget);
    MImageWidget *imageWidget();

    MProgressIndicator *progressIndicator();

    MImageWidget *sideTopImageWidget();
    MImageWidget *sideBottomImageWidget();

protected:
    MLabel * titleLabelWidget();

    virtual QGraphicsLayout *createLayout();
    virtual void clearLayout();

    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    Q_DECLARE_PRIVATE(MAdvancedListItem)
    MAdvancedListItemPrivate *d_ptr;

};

#endif //MADVACEDLISTITEM_H
