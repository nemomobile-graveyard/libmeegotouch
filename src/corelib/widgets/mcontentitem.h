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

#ifndef MCONTENTITEM_H__
#define MCONTENTITEM_H__

#include <mwidgetcontroller.h>
#include "mcontentitemmodel.h"
#include <MProgressIndicator>

class MContentItemPrivate;

/*!
    \class MContentItem
    \brief MContentItem is a widget for displaying thumbnail and 2 lines of text. Can be used as a standalone widget
        in a layout or in MList as list item.

    \ingroup widgets

    \section MContentItem Overview
        MContentItem provides functionality which is similar to button. It can show 2 lines of text and an icon (thumbnail).
        Tapping on widget will trigger clicked() signal. Widget can be selected via setSelected() and selection status can be
        queried via isSelected() function.

        MContentItem provides the following styles:
        \li MContentItem::IconAndTwoTextLabels
        \li MContentItem::SingleTextLabel
        \li MContentItem::IconAndSingleTextLabel
        \li MContentItem::TwoTextLabels
        \li MContentItem::SingleIcon
        \li MContentItem::IconAndSingleTextLabelVertical
        \li MContentItem::IconAndTwoTextLabelsVertical
        \li MContentItem::TwoIconsTwoWidgets

        MContentItem can be in one of the modes provided by ContentItemMode enumeration.
        For each mode there is a dedicated graphics which is up to the view to show. The purpose of mode is to indicate
        logical position of item so that correct background can be used. For more details take a look at MContentItemView.

        \sa MContentItemView
  */
class M_CORE_EXPORT MContentItem : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MContentItem)

public:
    /*!
      ContentItemStyle indicates how text and thumbnail should be shown. There are 8 predefined layouts.
      Exact look and feel depends on the view.

      \sa MContentItemView
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

      \sa MContentItemView
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
        \property MContentItem::itemStyle
        \brief specifies item style
    */
    Q_PROPERTY(ContentItemStyle itemStyle READ itemStyle)

    /*!
        \property MContentItem::itemMode
        \brief specifies item mode
    */
    Q_PROPERTY(ContentItemMode itemMode READ itemMode WRITE setItemMode)

    /*!
        \property MContentItem::pixmap
        \brief pixmap which will be displayed
    */
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

    Q_PROPERTY(QImage image READ image WRITE setImage)

    /*!
        \property MContentItem::imageID
        \brief image from theme which will be displayed
    */
    Q_PROPERTY(QString imageID READ imageID WRITE setImageID)

    /*!
        \property MContentItem::title
        \brief See MContentItemModel::title
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /*!
        \property MContentItem::subtitle
        \brief See MContentItemModel::subtitle
    */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)

    /*!
        \property MContentItom::optionalPixmap
        \brief Icon in the upper right corner which might be displayed.
    */
    Q_PROPERTY(QPixmap optionalPixmap READ optionalPixmap WRITE setOptionalPixmap)

    Q_PROPERTY(QImage optionalImage READ optionalImage WRITE setOptionalImage)

    /*!
        \property MContentItem::optionalImage
        \brief image from theme which will be displayed as "optional image"
    */
    Q_PROPERTY(QString optionalImageID READ optionalImageID WRITE setOptionalImageID)

public:

    /*!
        \brief Constructs a MContentItem with a specific style and a \a parent.
        \param itemStyle specifies style of content item
        \param parent Parent object.
     */
    MContentItem(MContentItem::ContentItemStyle itemStyle = MContentItem::IconAndTwoTextLabels, QGraphicsItem *parent = 0);

    /*!
        \brief Destructor.
     */
    virtual ~MContentItem();

    /*!
        \brief Get the thumbnail pixmap
        \return thumbnail pixmap
     */
    QPixmap pixmap() const;

    /*!
        \brief Get the thumbnail image
        \return thumbnail image
     */
    QImage image() const;

    /*!
      Returns the \ref logicalid "logical ID" of the contentItem's image.
     */
    QString imageID() const;

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
    MContentItem::ContentItemMode itemMode() const;

    /*!
      Returns item's style.
      */
    MContentItem::ContentItemStyle itemStyle() const;

    /*!
      Returns the \ref logicalid "logical ID" of the contentItem's additional image.
     */
    MWidget* additionalItem() const;

    /*!
     \brief Returns a pointer to the small widget.
     */
    MWidget* smallItem() const;

    /*!
      Returns optional pixmap (Icon)
     */
    QPixmap optionalPixmap() const;

    /*!
      Returns optional image (Icon)
     */
    QImage optionalImage() const;

    /*!
      Returns the \ref logicalid "logical ID" of the contentItem's optional image.
     */
    QString optionalImageID() const;

public Q_SLOTS:
    /**
        \brief Sets thumbnail pixmap.
        \param pixmap QPixmap
     */
    void setPixmap(const QPixmap &pixmap);

    /**
        \brief Sets thumbnail image.
        \param pixmap Image
     */
    void setImage(const QImage &pixmap);

    /*!
      Sets the \ref logicalid "logical ID" of the contentItem image to \a id.

      If \a size is not specified, whatever size the graphics happens to be in
      the theme will be used.
     */
    void setImageID(const QString &id, const QSize &size = QSize());

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
     \brief Sets a MWidget as e.g. progress bar, below the title.
     \param widget MWidget
      In case there is already a subtitle it will be replaced.
     */
    void setAdditionalItem(MWidget* widget);

    /*!
     \brief Convenience method which sets a progress bar as the additional item.
     */
    void enableProgressBar();

    /*!
     \brief Sets a MWidget as a widget below the right icon.
     \param widget MWidget
     */
    void setSmallItem(MWidget* widget);

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
        \brief Sets an optional image (Icon).
        \param image QImage
     */
    void setOptionalImage(const QImage& image);

    /*!
      Sets the \ref logicalid "logical ID" of the contentItem's optional image to \a id.

      If \a size is not specified, whatever size the graphics happens to be in
      the theme will be used.
     */
    void setOptionalImageID(const QString &id, const QSize &s = QSize());

    /*!
      \brief Makes content item to send clicked() signal.
      */
    void click();

    /*!
      \brief Makes the content item to send longTapped signal.
      \param pos The position of the tap.

      If the MContentItem is used outside MList, the user needs to
      invoke item->grabGesture(Qt::TapAndHoldGesture) to start
      receiving longTapped() signal. You might also be interested in
      using cancel event with that gesture, please see also
      MWidget::grabGestureWithCancelPolicy() method.

      \sa MWidget::grabGestureWithCancelPolicy.
    */
    void longTap(const QPointF &pos);

Q_SIGNALS:
    /*!
      \brief This signal is emitted when content item is pressed.
      */
    void clicked();

    /*!
      \brief The signal is emitted when the content item has been tapped and holded.
    */
    void longTapped(const QPointF &pos);

protected:
    //! \internal
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    MContentItem(MContentItemPrivate *dd, MContentItemModel *model, QGraphicsItem *parent);   
    //! \internal_end

    /*!
       Handler of notifications of new receivers connected to MContentItem signals.
    */
    virtual void connectNotify(const char *signal);

    /*!
       Handler of notifications of receivers disconnecting from MContentItem signals.
    */
    virtual void disconnectNotify(const char *signal);


private:
    Q_DISABLE_COPY(MContentItem)
    Q_DECLARE_PRIVATE(MContentItem)
};

#endif
