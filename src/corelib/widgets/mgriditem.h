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

#ifndef MGRIDITEM_H
#define MGRIDITEM_H

#include "mwidgetcontroller.h"
#include "mgriditemmodel.h"
#include <QPixmap>

class MGridItemPrivate;

/**
    \class MGridItem
    \brief MGridItem is a combiner class for displaying thumbnail, title and subTitle,
           used by MList, MGrid and MPopupList.

    \ingroup widgets

    \section MGridItemOverview Overview
        Using this class it is possible to display one image and two text label.
        You can set image by id in theme system or QPixmap

        There are 4 different look of MGridItem
        MImage,               set imageVisible = true,  titleVisible = false, subtitleVisible = false
        MLabel,               set imageVisible = false, titleVisible = true,  subtitleVisible = false
        MImage+MLabel,      set imageVisible = true,  titleVisible = true,  subtitleVisible = false
        MImage+2 MLabel,    set imageVisible = true,  titleVisible = true,  subtitleVisible = true

        You can change the image alignment to LeftAlign/RightAlign by modify CSS

     \deprecated Please use MContentItem, MBasicListItem, MAdvancedListItem, MDetailedListItem
 */

class M_CORE_EXPORT MGridItem: public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MGridItem)

public:

    /**
        \property MGridItem::image
        \brief See MGridItemModel::image
    */
    Q_PROPERTY(QString image READ image WRITE setImage)

    /**
        \property MGridItem::pixmap
        \brief pixmap which will be displayed
    */
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

    /**
        \property MGridItem::title
        \brief See MGridItemModel::title
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /**
        \property MGridItem::subtitle
        \brief See MGridItemModel::subtitle
    */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)

    /**
        \property MGridItem::imageVisible
        \brief See MGridItemModel::imageVisible
    */
    Q_PROPERTY(bool imageVisible READ isImageVisible WRITE setImageVisible)

    /**
        \property MGridItem::titleVisible
        \brief See MGridItemModel::titleVisible
    */
    Q_PROPERTY(bool titleVisible READ isTitleVisible WRITE setTitleVisible)

    /**
        \property MGridItem::subtitleVisible
        \brief See MGridItemModel::subtitleVisible
    */
    Q_PROPERTY(bool subtitleVisible READ isSubtitleVisible WRITE setSubtitleVisible)

public:

    /**
        \brief Constructs a GridItem with a \a parent.
        \param parent Parent object.
     */
    MGridItem(QGraphicsItem *parent = 0);

    /**
        \brief Destructor.
     */
    virtual ~MGridItem();

    /**
        \brief Get the image name.
        \return image id in theme system
     */
    QString image() const;

    /**
        \brief Get the thumbnail pixmap
        \return thumbnail pixmap
     */
    QPixmap pixmap() const;

    /**
        \brief Get the title.
        \return title text string.
     */
    QString title() const;

    /**
        \brief Get the subtitle.
        \return subtitle text string.
     */
    QString subtitle() const;

    /**
        \brief Returns true if the image is visible.
    */
    bool isImageVisible() const;

    /**
        \brief Returns true if the title is visible.
    */
    bool isTitleVisible() const;

    /**
        \brief Returns true if the subtitle is visible.
    */
    bool isSubtitleVisible() const;

    /**
        \brief set GridItem be selected
        Override the base function in QGraphicsItem
    */
    void setSelected(bool selected);

public Q_SLOTS:

    /**
        \brief Sets image id
        \param id the image id in theme system
     */
    void setImage(const QString &id);

    /**
        \brief Sets thumbnail pixmap
        \param pixmap QPixmap
     */
    void setPixmap(const QPixmap &pixmap);

    /**
        \brief Set title text.
        \param text text.
     */
    void setTitle(const QString &text);

    /**
        \brief Set subtitle text.
        \param text text.
     */
    void setSubtitle(const QString &text);

    /**
        \brief Set the visibility of the image.
    */
    void setImageVisible(bool);

    /**
        \brief Set the visibility of the title.
    */
    void setTitleVisible(bool);

    /**
        \brief Set the visibility of the subtitle.
    */
    void setSubtitleVisible(bool);

Q_SIGNALS:

    /**
        \brief This signal is emitted when the pixmap is changed
     */
    void pixmapChanged();

protected:
    //! \internal
    MGridItem(MGridItemPrivate *dd, MGridItemModel *model, QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DISABLE_COPY(MGridItem)
    Q_DECLARE_PRIVATE(MGridItem)

    friend class Ut_MGridItem;
    friend class MGridItemView;
};

#endif
