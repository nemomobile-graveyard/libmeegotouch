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


/*!
    \class MAdvancedListItem
    \brief MAdvancedListItem implementation of an advanced widget which can be used with MList.

    \ingroup widgets

    \section MAdvancedListItem Overview

    MAdvancedListItem can show one line of text, a main icon, one or two icons on the side, and
    a progress indicator.
    Exact layout depends on the style and can be set in:

    \li MAdvancedListItem(MAdvancedListItem::ItemStyle, QGraphicsItem)
    \li setItemStyle(ItemStyle)

    Title and main icon and can be set using following functions:

    \li setImageWidget()
    \li setTitle()

    \section MAdvancedListItem Customizing appearance

    MAdvancedListItem appearance can be customized in 2 ways.

    Title, main icon, two side icons and progress indicator are using styles provided by common layout.

    \li title - CommonTitle depending on the item style.
    \li icon - CommonMainIcon
    \li sideTopIcon - CommonSubIconTop
    \li sideBottomIcon - CommonSubIconBottom
    \li progressIndicator - CommonProgressBar

    Another way is to inherit MAdvancedListItem and override:
    \li createLayout()
    \li clearLayout()
 */

class M_CORE_EXPORT MAdvancedListItem : public MListItem
{
    Q_OBJECT

    /*!
        \property MAdvancedListItem::title
        \brief Contains title text.
     */
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

    /*!
        \property MAdvancedListItem::imageWidget
        \brief Contains main icon image widget.
     */
    Q_PROPERTY(MImageWidget* imageWidget READ imageWidget WRITE setImageWidget)
    /*!
        \property MAdvancedListItem::sideTopImageWidget
        \brief Contains side top icon image widget.
     */
    Q_PROPERTY(MImageWidget* sideTopImageWidget READ sideTopImageWidget)
    /*!
        \property MAdvancedListItem::sideBottomImageWidget
        \brief Contains side bottom icon icon widget.
     */
    Q_PROPERTY(MImageWidget* sideBottomImageWidget READ sideBottomImageWidget)
    /*!
        \property MAdvancedListItem::progressIndicator
        \brief Contains progress indicator widget.
     */
    Q_PROPERTY(MProgressIndicator* progressIndicator READ progressIndicator)

public:
    /*!
      \brief Specifies layout of MAdvancedListItem
     */
    enum ItemStyle {
        /*!
          \brief Defines style which contains title, main icon, two icons on the side of the item.
                and a progress indicator.
         */
        IconWithTitleProgressIndicatorAndTwoSideIcons,
        /*!
          \brief Defines style which contains title, main icon, one top icon on the side of the item.
                and a progress indicator that is expanded to take the place of bottom icon.
         */
        IconWithTitleProgressIndicatorAndTopSideIcon
    };

public:
    /*!
      \brief MAdvancedListItem constructor.

      \param style The style of item to be initialized with, defaults to MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons.
      \param parent The item parent.

      \sa MAdvancedListItem::ItemStyle
      \sa MAdvancedListItem::setItemStyle()
    */
    MAdvancedListItem(ItemStyle itemStyle = IconWithTitleProgressIndicatorAndTwoSideIcons, QGraphicsItem *parent = 0);
    virtual ~MAdvancedListItem();

    /*!
      \deprecated The method is no longer required to be called after constructing the item.
                  There shouldn't be any need for this function.
    */
    void initLayout();

    /*!
      \brief Sets item style.

      \sa MAdvancedListItem::ItemStyle
      \sa MAdvancedListItem::itemStyle()
     */
    void setItemStyle(ItemStyle itemStyle);
    /*!
      \brief Returns current item style.

      \sa MAdvancedListItem::setItemStyle()
     */
    ItemStyle itemStyle() const;

    /*!
      \brief Sets the item title text.

      \sa MAdvancedListItem::title()
    */
    void setTitle(const QString &title);
    /*!
      \brief Returns the item title text.

      \sa MAdvancedListItem::setTitle()
    */
    QString title();

    /*!
      \brief Sets item main icon widget. Ownership is transfered to the item.

      \sa MAdvancedListItem::imageWidget()
      */
    void setImageWidget(MImageWidget *imageWidget);
    /*!
      \brief Creates or returns already created pointer to an items main icon widget.

      \sa MAdvancedListItem::setImageWidget()
     */
    MImageWidget *imageWidget();

    /*!
      \brief Creates or returns already created pointer to an items progress indicator.
     */
    MProgressIndicator *progressIndicator();

    /*!
      \brief Creates or returns already created pointer to an items top side icon widget.
     */
    MImageWidget *sideTopImageWidget();
    /*!
      \brief Creates or returns already created pointer to an items bottom side icon widget.
     */
    MImageWidget *sideBottomImageWidget();

protected:
    /*!
     \brief Creates or returns already created title label widget.

     \sa MAdvancedListItem::setTitle()
     \sa MAdvancedListItem::title()
     */
    MLabel * titleLabelWidget();

    /*!
      \brief Returns layout which will be assigned to MAdvancedListItem.
      Can be called several times, so it should clear layout which was already created.

      \sa MAdvancedListItem::titleLabelWidget()
      \sa MAdvancedListItem::imageWidget()
      \sa MAdvancedListItem::sideTopImageWidget()
      \sa MAdvancedListItem::sideBottomImageWidget()
      \sa MAdvancedListItem::progressIndicator()
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
    Q_DECLARE_PRIVATE(MAdvancedListItem)
    MAdvancedListItemPrivate *d_ptr;

};

#endif //MADVACEDLISTITEM_H
