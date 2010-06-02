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

class QGraphicsGridLayout;
class MProgressIndicator;
class MLabel;
class MImageWidget;

class M_EXPORT MAdvancedListItem : public MListItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(MImageWidget* imagewidget READ imageWidget WRITE setImageWidget)
    Q_PROPERTY(MImageWidget* sideTopImage READ sideTopImageWidget)
    Q_PROPERTY(MImageWidget* sideBottomImage READ sideBottomImageWidget)
    Q_PROPERTY(MProgressIndicator* progressIndicator READ progressIndicator)

public:
    enum ItemStyle {
        IconWithTitleProgressIndicatorAndTwoSideIcons = 0x0,
        IconWithTitleProgressIndicatorAndTopSideIcon
    };

public:
    MAdvancedListItem(ItemStyle itemStyle = IconWithTitleProgressIndicatorAndTwoSideIcons, QGraphicsItem *parent = 0);
    virtual ~MAdvancedListItem();

    void initLayout();

    void setTitle(const QString &title);
    QString title() const;

    void setImageWidget(MImageWidget *imageWidget);
    MImageWidget *imageWidget();

    MProgressIndicator *progressIndicator();

    MImageWidget *sideTopImageWidget();
    MImageWidget *sideBottomImageWidget();

protected:
    MLabel * titleLabelWidget();

private:
    QGraphicsLayout *createLayout();

private:
    QGraphicsGridLayout *layoutGrid;

    MProgressIndicator *progress;
    MLabel *titleLabel;
    MImageWidget *image;
    MImageWidget *sideTopImage;
    MImageWidget *sideBottomImage;

    ItemStyle listItemStyle;
};

#endif //MADVACEDLISTITEM_H
