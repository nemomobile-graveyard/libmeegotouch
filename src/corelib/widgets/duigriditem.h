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

#ifndef DUIGRIDITEM_H
#define DUIGRIDITEM_H

#include "duiwidgetcontroller.h"
#include "duigriditemmodel.h"
#include <QPixmap>

class DuiGridItemPrivate;

/**
    \class DuiGridItem
    \brief DuiGridItem is a combiner class for displaying thumbnail, title and subTitle,
           used by DuiList, DuiGrid and DuiPopupList.

    \ingroup widgets

    \section DuiGridItemOverview Overview
        Using this class it is possible to display one image and two text label.
        You can set image by id in theme system or QPixmap

        There are 4 different look of DuiGridItem
        DuiImage,               set imageVisible = true,  titleVisible = false, subtitleVisible = false
        DuiLabel,               set imageVisible = false, titleVisible = true,  subtitleVisible = false
        DuiImage+DuiLabel,      set imageVisible = true,  titleVisible = true,  subtitleVisible = false
        DuiImage+2 DuiLabel,    set imageVisible = true,  titleVisible = true,  subtitleVisible = true

        You can change the image alignment to LeftAlign/RightAlign by modify CSS
 */

class DUI_EXPORT DuiGridItem: public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiGridItem)

public:

    /**
        \property DuiGridItem::image
        \brief See DuiGridItemModel::image
    */
    Q_PROPERTY(QString image READ image WRITE setImage)

    /**
        \property DuiGridItem::pixmap
        \brief pixmap which will be displayed
    */
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

    /**
        \property DuiGridItem::title
        \brief See DuiGridItemModel::title
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /**
        \property DuiGridItem::subtitle
        \brief See DuiGridItemModel::subtitle
    */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)

    /**
        \property DuiGridItem::imageVisible
        \brief See DuiGridItemModel::imageVisible
    */
    Q_PROPERTY(bool imageVisible READ isImageVisible WRITE setImageVisible)

    /**
        \property DuiGridItem::titleVisible
        \brief See DuiGridItemModel::titleVisible
    */
    Q_PROPERTY(bool titleVisible READ isTitleVisible WRITE setTitleVisible)

    /**
        \property DuiGridItem::subtitleVisible
        \brief See DuiGridItemModel::subtitleVisible
    */
    Q_PROPERTY(bool subtitleVisible READ isSubtitleVisible WRITE setSubtitleVisible)

public:

    /**
        \brief Constructs a GridItem with a \a parent.
        \param parent Parent object.
     */
    DuiGridItem(QGraphicsItem *parent = 0);

    /**
        \brief Destructor.
     */
    virtual ~DuiGridItem();

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
    DuiGridItem(DuiGridItemPrivate *dd, DuiGridItemModel *model, QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DISABLE_COPY(DuiGridItem)
    Q_DECLARE_PRIVATE(DuiGridItem)

    friend class Ut_DuiGridItem;
    friend class DuiGridItemView;
};

#endif
