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

/*!
    \class MDetailedListItem
    \brief MDetailedListItem implementation of a detailed widget which can be used with MList.

    \ingroup widgets

    \section MDetailedListItem Overview

    MDetailedListItem can show several lines of text, main icon and two icons or one top icon and sub label
    on left side of the item. The main icon can have two styles, as Icon and as Thumbnail.
    Exact layout depends on the style and can be set in:

    \li MDetailedListItem(MDetailedListItem::ItemStyle, QGraphicsItem)
    \li setItemStyle(ItemStyle)

    Text and icon can be set using following functions:

    \li setImageWidget()
    \li setTitle()
    \li setSubTitle()

    \section MDetailedListItem Customizing appearance

    MDetailedListItem appearance can be customized in 2 ways.

    Title, subtitle, main icon, side icons and side label are using styles provided by common layout.

    \li title -  CommonTitle and CommonSingleTitle depending on the item style.
    \li subTitle - CommonSubTitle.
    \li bottomLabel - CommonItemInfo
    \li icon - CommonMainIcon and CommonMainThumbnail depending on the item style.
    \li sideTopIcon - CommonSubIconTop.
    \li sideBottomIcon - CommonSubIconBottom.

    Another way is to inherit MDetailedListItem and override:
    \li createLayout()
    \li clearLayout()
 */

class M_CORE_EXPORT MDetailedListItem : public MListItem
{
public:
    Q_OBJECT

    /*!
        \property MDetailedListItem::title
        \brief Contains title text.
     */
    Q_PROPERTY(QString title READ title WRITE setTitle)
    /*!
        \property MDetailedListItem::subtitle
        \brief Contains subtitle text.
     */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    /*!
        \property MDetailedListItem::sideBottomTitle
        \brief Contains side bottom label text.
     */
    Q_PROPERTY(QString sideBottomTitle READ sideBottomTitle WRITE setSideBottomTitle)
    /*!
        \property MDetailedListItem::imageWidget
        \brief Contains pointer to main icon MImageWidget.
     */
    Q_PROPERTY(MImageWidget* imageWidget READ imageWidget WRITE setImageWidget)
    /*!
        \property MDetailedListItem::sideTopImageWidget
        \brief Contains pointer to side top icon MImageWidget.
     */
    Q_PROPERTY(MImageWidget* sideTopImageWidget READ sideTopImageWidget)
    /*!
        \property MDetailedListItem::sideBottomImageWidget
        \brief Contains pointer to side bottom icon MImageWidget.
     */
    Q_PROPERTY(MImageWidget* sideBottomImageWidget READ sideBottomImageWidget)

public:
    /*!
        \brief Specifies layout of MDetailedListItem
     */
    enum ItemStyle {
        //! \brief Defines style which contains a main icon, title with subtitle and two icons on the side.
        IconTitleSubtitleAndTwoSideIcons,
        //! \brief Defines style which contains a main icon, title with subtitle and small icon with a sublabel on the side.
        IconTitleSubtitleAndSideIconWithLabel,
        //! \brief Defines style which contains a main thumbnail, title with subtitle and two icons on the side.
        ThumbnailTitleSubtitleAndTwoSideIcons,
        //! \brief Defines style which contains a main thumbnail, a single title and two icons on the side.
        ThumbnailTitleAndTwoSideIcons
    };
    
    //! \brief Specifies main icon styles for MDetailedListItem
    enum IconStyle {
        //! \brief Defines main icon style as an icon with margins.
        Icon,
        //! \brief Defines main icon styles as a thumbnail without margins.
        Thumbnail
    };

public:
    /*!
      \brief MDetailedListItem constructor.

      \param style The style of item to be initialized with, defaults to MDetailedListItem::IconTitleSubtitleAndTwoSideIcons.
      \param parent The item parent.

      \sa MDetailedListItem::ItemStyle
      \sa MDetailedListItem::setItemStyle()
    */
    MDetailedListItem(MDetailedListItem::ItemStyle style = IconTitleSubtitleAndTwoSideIcons, QGraphicsItem *parent = NULL);
    /*!
      \brief MDetailedListItem destructor.
    */
    virtual ~MDetailedListItem();
    
    /*!
      \deprecated The method is no longer required to be called after constructing the item.
                  There shouldn't be any need for this function.
    */
    void initLayout();
    
    /*!
      \brief Sets item style.

      \sa MDetailedListItem::ItemStyle
      \sa MDetailedListItem::itemStyle()
     */
    void setItemStyle(ItemStyle itemStyle);
    /*!
      \brief Returns current item style.

      \sa MDetailedListItem::setItemStyle()
     */
    ItemStyle itemStyle() const;
    
    /*!
      \brief Sets main icon style.

      \sa MDetailedListItem::IconStyle
      \sa MDetailedListItem::iconStyle()
     */
    void setIconStyle(IconStyle style);
    /*!
      \brief Returns current main icon style.

      \sa MDetailedListItem::setIconStyle()
     */
    IconStyle iconStyle() const;
    
    /*!
      \brief Sets item icon widget. Ownership is transfered to the item.

      \sa MDetailedListItem::imageWidget()
     */
    void setImageWidget(MImageWidget *icon);
    /*!
      \brief Creates or returns already created pointer to an item icon widget.

      \sa MDetailedListItem::setImageWidget()
     */
    MImageWidget *imageWidget();
    
    /*!
      \brief Creates or returns already created pointer to an item side top icon widget.
     */
    MImageWidget *sideTopImageWidget();
    /*!
      \brief Creates or returns already created pointer to an item side bottom icon widget.
     */
    MImageWidget *sideBottomImageWidget();
    
    /*!
      \brief Sets the item title text.

      \sa MDetailedListItem::title()
    */
    void setTitle(const QString &title);
    /*!
      \brief Returns the item title text.

      \sa MDetailedListItem::setTitle()
    */
    QString title();
    
    /*!
      \brief Sets the item subtitle text.

      \sa MDetailedListItem::subtitle()
    */
    void setSubtitle(const QString &subtitle);
    /*!
      \brief Returns the item subtitle text.

      \sa MDetailedListItem::setSubtitle()
    */
    QString subtitle();
    

    /*!
      \brief Sets the item bottom side title text.

      \sa MDetailedListItem::sideBottomTitle()
    */
    void setSideBottomTitle(const QString &text);
    /*!
      \brief Returns the item bottom side title text.

      \sa MDetailedListItem::setSideBottomTitle()
    */
    QString sideBottomTitle();
    
protected:
    /*!
     \brief Creates or returns already created title widget.

     \sa MDetailedListItem::setTitle()
     \sa MDetailedListItem::title()
     */
    MLabel *titleLabelWidget();
    /*!
      \brief Creates or returns already created subtitle widget.

      \sa MDetailedListItem::setSubtitle()
      \sa MDetailedListItem::subtitle()
     */
    MLabel *subtitleLabelWidget();
    /*!
      \brief Creates or returns already created bottom side title widget.

      \sa MDetailedListItem::setSideBottomTitle()
      \sa MDetailedListItem::sideBottomTitle()
     */
    MLabel *sideBottomLabelWidget();

    /*!
      \brief Returns layout which will be assigned to MDetailedListItem.
      Can be called several times, so it should clear layout which was already created.

      \sa MDetailedListItem::titleLabelWidget()
      \sa MDetailedListItem::subtitleLabelWidget()
      \sa MDetailedListItem::sideBottomLabelWidget()
      \sa MDetailedListItem::imageWidget()
      \sa MDetailedListItem::sideTopImageWidget()
      \sa MDetailedListItem::sideBottomImageWidget()
     */
    virtual QGraphicsLayout *createLayout();
    /*!
      \brief Clears layout created by createLayout() function.
     */
    virtual void clearLayout();

    //! \internal
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MDetailedListItem)
    MDetailedListItemPrivate *d_ptr;
};


#endif // MDETAILEDLISTITEM_H
