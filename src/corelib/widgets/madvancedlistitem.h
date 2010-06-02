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
    Q_PROPERTY(MImageWidget* rightTopImage READ imageWidget)
    Q_PROPERTY(MImageWidget* rightTopImage READ imageWidget)
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

    QString title() const;
    void setTitle(const QString &title);

    MImageWidget *imageWidget();
    void setImageWidget(MImageWidget *imageWidget);

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
