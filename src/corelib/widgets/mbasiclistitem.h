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

/*!
    \class MBasicListItem
    \brief MBasicListItem implementation of a simple widget which can be used with MList.

    \ingroup widgets

    \section MBasicListItem Overview

    MBasicListItem can show several lines of text and an icon. Exact layout depends on the style
    and can be set in:

    \li MBasicListItem(MBasicListItem::ItemStyle, QGraphicsItem)
    \li setItemStyle(ItemStyle)

    Text and icon can be set using following functions:

    \li setImageWidget()
    \li setTitle()
    \li setSubTitle()

    \section MBasicListItem Customizing appearance

    MBasicListItem appearance can be customized in 2 ways.

    Title, subtitle and icon are using styles provided by common layout.

    \li title - CommonSingleTitle and CommonTitle depending on the item style.
    \li subTitle - CommonSubTitle
    \li icon - CommonMainIcon

    Another way is to inherit MBasicListItem and override:
    \li createLayout()
    \li clearLayout()
 */
class M_CORE_EXPORT MBasicListItem : public MListItem
{
    Q_OBJECT

    /*!
        \property MBasicListIte::title
        \brief Contains title text.
     */
    Q_PROPERTY(QString title READ title WRITE setTitle)
    /*!
        \property MBasicListItem::subtitle
        \brief Contains subtitle text.
     */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)
    /*!
        \property MBasicListItem::imageWidget
        \brief Contains pointer to MImageWidget.
     */
    Q_PROPERTY(MImageWidget* imageWidget READ imageWidget WRITE setImageWidget)

public:
    /*!
      \brief Specifies layout of MBasicListItem
     */
    enum ItemStyle {
        /*!
          \brief Defines style which contains only one title.
         */
        SingleTitle,
        /*!
          \brief Defines style which contains title and subtitle.
         */
        TitleWithSubtitle,
        /*!
          \brief Defines style which contains only one title and an icon.
         */
        IconWithTitle,
        /*!
          \brief Defines style which contains title, subtitle and an icon.
         */
        IconWithTitleAndSubtitle
    };

public:
    /*!
      \brief MBasicListItem constructor.

      \param style The style of item to be initialized with, defaults to MBasicListItem::SingleTitle.
      \param parent The item parent.

      \sa MBasicListItem::ItemStyle
      \sa MBasicListItem::setItemStyle()
    */
    MBasicListItem(MBasicListItem::ItemStyle style = SingleTitle, QGraphicsItem *parent = NULL);
    /*!
      \brief MBasicListItem destructor.
    */
    virtual ~MBasicListItem();

    /*!
      \deprecated The method is no longer required to be called after constructing the item.
                  There shouldn't be any need for this function.
    */
    void initLayout();

    /*!
      \brief Sets item style.

      \sa MBasicListItem::ItemStyle
      \sa MBasicListItem::itemStyle()
     */
    void setItemStyle(ItemStyle itemStyle);

    /*!
      \brief Returns current item style.

      \sa MBasicListItem::setItemStyle()
     */
    ItemStyle itemStyle() const;

    /*!
      \brief Sets item icon widget. Ownership is transfered to the item.

      \sa MBasicListItem::imageWidget()
      */
    void setImageWidget(MImageWidget *imageWidget);

    /*!
      \brief Creates or returns already created pointer to an item icon widget.

      \sa MBasicListItem::setImageWidget()
     */
    MImageWidget *imageWidget();

    /*!
      \brief Sets the item title text.

      \sa MBasicListItem::title()
    */
    void setTitle(const QString &title);

    /*!
      \brief Returns the item title text.

      \sa MBasicListItem::setTitle()
    */
    QString title();

    /*!
      \brief Sets the item subtitle text.

      \sa MBasicListItem::subtitle()
    */
    void setSubtitle(const QString &subtitle);

    /*!
      \brief Returns the item subtitle text.

      \sa MBasicListItem::setSubtitle()
    */
    QString subtitle();

protected:
    /*!
     \brief Creates or returns already created title widget.

     \sa MBasicListItem::setTitle()
     \sa MBasicListItem::title()
     */
    MLabel *titleLabelWidget();

    /*!
      \brief Creates or returns already created subtitle widget.

      \sa MBasicListItem::setSubtitle()
      \sa MBasicListItem::subtitle()
     */
    MLabel *subtitleLabelWidget();

    /*!
      \brief Returns layout which will be assigned to MBasicListItem.
      Can be called several times, so it should clear layout which was already created.

      \sa MBasicListItem::titleLabelWidget()
      \sa MBasicListItem::subtitleLabelWidget()
      \sa MBasicListItem::imageWidget()
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
    Q_DECLARE_PRIVATE(MBasicListItem)
    MBasicListItemPrivate *d_ptr;
};

#endif // MBASICLISTITEM_H
