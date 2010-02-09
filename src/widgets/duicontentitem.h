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

#ifndef DUICONTENTITEM_H__
#define DUICONTENTITEM_H__

#include <duiwidgetcontroller.h>
#include "duicontentitemmodel.h"
#include <DuiProgressIndicator>

class DuiContentItemPrivate;

/*!
    \class DuiContentItem
    \brief DuiContentItem is a widget for displaying thumbnail and 2 lines of text. Can be used as a standalone widget
        in a layout or in DuiList as list item.

    \ingroup widgets

    \section DuiContentItem Overview
        DuiContentItem provides functionality which is similar to button. It can show 2 lines of text and an icon (thumbnail).
        Tapping on widget will trigger clicked() signal. Widget can be selected via setSelected() and selection status can be
        queried via isSelected() function.

        DuiContentItem provides the following styles:
        \li ContentItemStyle::IconAndTwoTextLabels
        \li ContentItemStyle::SingleTextLabel
        \li ContentItemStyle::IconAndSingleTextLabel
        \li ContentItemStyle::TwoTextLabels

        DuiContentItem can be in one of the modes provided by ContentItemMode enumeration.
        For each mode there is a dedicated graphics which is up to the view to show. The purpose of mode is to indicate
        logical position of item so that correct background can be used. For more details take a look at DuiContentItemView.

        \sa DuiContentItemView
  */
class DUI_EXPORT DuiContentItem : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiContentItem)

public:
    /*!
      ContentItemStyle indicates how text and thumbnail should be shown. There are 4 predefined layouts.
      Exact look and feel depends on the view.

      \sa DuiContentItemView
      */
    enum ContentItemStyle
    {
         IconAndTwoTextLabels = 0,
         SingleTextLabel,
         IconAndSingleTextLabel,
         TwoTextLabels,
         SingleIcon,
         IconAndSingleTextLabelVertical,
         IconAndTwoTextLabelsVertical,
         TwoIconsTwoWidgets
     };

    /*!
      ContentItemMode indicates logical position of content item relative other items. Exact look and feel
      depends on the view.

      \sa DuiContentItemView
      */
    enum ContentItemMode {
        Default = 0,
        TopLeft,
        Top,
        TopRight,
        Left,
        Center,
        Right,
        Single,
        BottomLeft,
        Bottom,
        BottomRight,
        SingleRowLeft,
        SingleRowCenter,
        SingleRowRight,
        SingleColumnTop,
        SingleColumnCenter,
        SingleColumnBottom,
        TwoIconsTextLabelVertical
    };

    /*!
        \property DuiContentItem::itemStyle
        \brief specifies item style
    */
    Q_PROPERTY(ContentItemStyle itemStyle READ itemStyle)

    /*!
        \property DuiContentItem::itemMode
        \brief specifies item mode
    */
    Q_PROPERTY(ContentItemMode itemMode READ itemMode WRITE setItemMode)

    /*!
        \property DuiContentItem::pixmap
        \brief pixmap which will be displayed
    */
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

    /*!
        \property DuiContentItem::title
        \brief See DuiContentItemModel::title
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /*!
        \property DuiContentItem::subtitle
        \brief See DuiContentItemModel::subtitle
    */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)

    /*!
        \property DuiContentItom::optionalPixmap
        \brief Icon in the upper right corner which might be displayed.
    */
    Q_PROPERTY(QPixmap optionalPixmap READ optionalPixmap WRITE setOptionalPixmap)

public:

    /*!
        \brief Constructs a DuiContentItem with a specific style and a \a parent.
        \param itemStyle specifies style of content item
        \param parent Parent object.
     */
    DuiContentItem(DuiContentItem::ContentItemStyle itemStyle = DuiContentItem::IconAndTwoTextLabels, QGraphicsItem *parent = 0);

    /*!
        \brief Destructor.
     */
    virtual ~DuiContentItem();

    /*!
        \brief Get the thumbnail pixmap
        \return thumbnail pixmap
     */
    QPixmap pixmap() const;

    /*!
        \brief Get the title.
        \return title text string.
     */
    QString title() const;

    /*!
        \brief Get the subtitle.
        \return subtitle text string.
     */
    QString subtitle() const;

    /*!
      Returns item's mode.
      */
    DuiContentItem::ContentItemMode itemMode() const;

    /*!
      Returns item's style.
      */
    DuiContentItem::ContentItemStyle itemStyle() const;

    /*!
     \brief Returns a pointer to the additional item.
     */
    DuiWidget* additionalItem() const;

    /*!
     \brief Returns a pointer to the small widget.
     */
    DuiWidget* smallItem() const;

    /*!
      Returns optional pixmap (Icon)
     */
    QPixmap optionalPixmap() const;

public Q_SLOTS:
    /**
        \brief Sets thumbnail pixmap.
        \param pixmap QPixmap
     */
    void setPixmap(const QPixmap &pixmap);

    /**
        \brief Set title text. This is first line.
        \param text text.
     */
    void setTitle(const QString &text);

    /**
        \brief Set subtitle text. This is second line.
        \param text text.
     */
    void setSubtitle(const QString &text);

    /*!
      \brief Sets item's mode. Mode defines how item's background should look. Exact look is defined in style.
      */
    void setItemMode(ContentItemMode mode);

    /*!
     \brief Sets a DuiWidget as e.g. progress bar, below the title.
     \param widget DuiWidget
      In case there is already a subtitle it will be replaced.
     */
    void setAdditionalItem(DuiWidget* widget);

    /*!
     \brief Convenience method which sets a progress bar as the additional item.
     */
    void enableProgressBar();

    /*!
     \brief Sets a DuiWidget as a widget below the right icon.
     \param widget DuiWidget
     */
    void setSmallItem(DuiWidget* widget);

    /*!
     \brief Convenience method for setting a text below the right icon.
     */
    void setSmallText(QString text);

    /*!
        \brief Sets an optional pixmap (Icon).
        \param pixmap QPixmap
     */
    void setOptionalPixmap(const QPixmap& pixmap);

    /*!
      \brief Makes content item to send clicked() signal.
      */
    void click();

Q_SIGNALS:
    /*!
      \brief This signal is emitted when content item is pressed.
      */
    void clicked();

protected:
    //! \internal
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //! \cond
    DuiContentItem(DuiContentItemPrivate *dd, DuiContentItemModel *model, QGraphicsItem *parent);
    //! \endcond
    //! \internal_end

private:
    Q_DISABLE_COPY(DuiContentItem)
    Q_DECLARE_PRIVATE(DuiContentItem)
};

#endif
