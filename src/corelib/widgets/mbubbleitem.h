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

#ifndef MBUBBLEITEM_H
#define MBUBBLEITEM_H

#include <MWidgetController>
#include "mbubbleitemmodel.h"

class MBubbleItemPrivate;
class MImageWidget;

/*!
  \class MBubbleItem
  \brief MBubbleItem is a speech bubble like widget for conversation views.

  MBubbleItem, MListItem and MContentItem can usually be used for the same
  type of purposes, however the MBubbleItem API is message-centric and therefore
  better suited for messaging applications. When several bubble items are put into
  a layout the visual impression is that of a back and forth conversation.

  The speech bubble supports avatar images, separate styling for incoming and
  outgoing messages, as well as an area for application specific widgets.
*/
class M_CORE_EXPORT MBubbleItem : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MBubbleItem)

public:

    /*!
        \property MBubbleItem::senderName
        Name of the message sender.
    */
    Q_PROPERTY(QString senderName READ senderName WRITE setSenderName)

    /*!
        \property MBubbleItem::timeStamp
        Time the message was sent, displayed as part of the bubble.
    */
    Q_PROPERTY(QString timeStamp READ timeStamp WRITE setTimeStamp)

    /*!
        \property MBubbleItem::message
        The main body of the message.
    */
    Q_PROPERTY(QString message READ message WRITE setMessage)

    /*!
        \property MBubbleItem::commentsString
        The number of comments received, displayed as part of the bubble.
    */
    Q_PROPERTY(QString commentsString READ commentsString WRITE setCommentsString)

    /*!
        \property MBubbleItem::thumbsUpString
        The number of thumbs up received, displayed as part of the bubble.
    */
    Q_PROPERTY(QString thumbsUpString READ thumbsUpString WRITE setThumbsUpString)

    /*!
        The type determines if the message is incoming or outgoing, and the widget
        may be styled differently based on this.
     */
    enum MessageType {
        Incoming,
        Outgoing
    };

public:

    /*!
        Constructs a MBubbleItem instance. The optional \a parent argument is passed to MWidgetController's constructor.
     */
    explicit MBubbleItem(QGraphicsItem *parent = 0);

    /*!
        Destroys the item.
     */
    virtual ~MBubbleItem();

    /*!
        Returns the image widget used for the avatar.

        \sa setAvatar()
     */
    MImageWidget* avatar() const;

    /*!
        Returns the name of the message sender.

        \sa setSenderName()
     */
    QString senderName();

    /*!
        Returns the time stamp string displayed as part of the bubble.

        \sa setSenderName()
     */
    QString timeStamp();

    /*!
      Returns the body of the message.

      \sa setMessage()
     */
    QString message();

    /*!
      Returns the type of the message.

      \sa setMessageType()
     */
    MessageType messageType() const;

    /*!
      Returns the central widget.

      For performance reasons, the central widget is not created by default.
      If the central widget has not been previously set the function returns 0.

      \sa setCentralWidget()
     */
    QGraphicsWidget* centralWidget();

    /*!
      Returns the stack of informational widgets attached to the bubble.

      \sa addInformationWidget()
      \sa setCommentsString()
      \sa setThumbsUpString()
     */
    QStack<QGraphicsWidget*> informationWidgets();

    /*!
      Returns the string for the number of comments received for the bubble.

      \sa setCommentsString();
     */
    QString commentsString();

    /*!
      Returns the string for the number of thumbs-up received for the bubble.

      \sa setThumbsUpString()
     */
    QString thumbsUpString();

Q_SIGNALS:
    /*!
      This signal is emitted if a link pointing to \a url in the message is clicked.

      Links can be created using HTML notation, see the MLabel documentation for
      further information.
     */
    void linkActivated(const QString &url);

    /*!
      This signal is emitted if the bubble that contains the message is clicked.
    */
    void bubbleClicked();

public Q_SLOTS:

    /**
      Replaces the current avatar image widget attached to the bubble with \a avatar.
      
      Any previous avatar is deleted if its QGraphicsItem::parent() is set to this bubble
      widget, otherwise it is left untouched.
      
      The avatar will always be deleted when the bubble is deleted.

      \sa avatar()
     */
    void setAvatar(MImageWidget* avatar);
    
    /*!
      Sets the avatar image to display the \a pixmap.
      
      If there is an existing avatar() widget, this will set the pixmap for that avatar.
      Otherwise a new MImageWidget will be created and its QGraphicsItem::parent() set
      to this bubble widget.

      \sa avatar()
    */
    void setAvatar(const QPixmap &pixmap);

    /**
      Sets the name of the message sender to \a name.

      This must be html escaped.  E.g.

      \code
          bubble->setSenderName(Qt::escape("I <3 everyone!"));
      \endcode

      \sa senderName()
     */
    void setSenderName(const QString &name);

    /**
      Sets the time stamp string of the message to \a timeStamp.

      This must be html escaped.

      \sa timeStamp()
     */
    void setTimeStamp(const QString &timeStamp);

    /**
       Sets the body of the message to \a message.

       This must be html escaped.

       \sa message()
     */
    void setMessage(const QString &message);

    /**
      Sets type of the message (incoming or outgoing) to \a messageType.

      \sa messageType()
     */

    void setMessageType(MessageType messageType);

    /**
      Sets the \a centralWidget.

      The central widget is the attachment point for application specific content
      inside the bubble item.

      Example:
      \code
      MBubbleItem *bubble = new MBubbleItem();
      MWidget *container = new MWidget();
      MImageWidget *image1 = new MImageWidget("foo");
      MImageWidget *image2 = new MImageWidget("bar");
      QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, container);
      layout->addItem(image1);
      layout->addItem(image2);

      bubble->setCentralWidget(container);
      \endcode

      \sa centralWidget()
     */
    void setCentralWidget(QGraphicsWidget* centralWidget);

    /**
      Adds the \a widget to the informational widgets stack.

      Informational widgets are displayed as part of the message bubble main body.
      There are two pre-defined information widgets for the bubble: number of comments and thumbs up received.

      The ownership of the widget is transfered to the item. If the widget must not be deleted by the item the 
      caller must invoke removeInformationWidget() to remove the widget from the graphics hierarchy.

      \sa informationWidgets();
      \sa setCommentsString();
      \sa setThumbsUpString();
      \sa removeInformationWidget();
     */
    void addInformationWidget(QGraphicsWidget *widget);

    /**
      Removes the \a widget from the informational widgets stack.

      Informational widgets are displayed as part of the message bubble main body.
      There are two pre-defined information widgets for the bubble: number of comments and thumbs up received.

      The removed widget is removed from the scene hierarchy and the ownership is transfered to caller.

      \sa informationWidgets()
      \sa setCommentsString();
      \sa setThumbsUpString();
      \sa addInformationWidget();
     */
    void removeInformationWidget(QGraphicsWidget *widget);

    /**
      Sets the string for number of comments received for the bubble item to \a comments.

      This is a convenience method that creates widgets on the informationWidgets() stack
      for showing the number of comments attached to the bubble item. An example of
      a string could "+3", but it is up to the application to determine how the string
      is formatted.

      \sa commentsString()
     */
    void setCommentsString(const QString &comments);

    /**
      Sets the string for number of thumbs up received for the bubble item to \a thumbsUp

      This is a convenience method that creates widgets on the informationWidgets() stack
      for showing the number of "thumbs up" attached to the bubble item. An example of
      a string could "+3", but it is up to the application to determine how the string
      is formatted.

      \sa thumbsUpString()
     */
    void setThumbsUpString(const QString &thumbsUp);

private:
    //! \internal
    MBubbleItem(MBubbleItemPrivate *dd, MBubbleItemModel *model, QGraphicsItem *parent);
    //! \internal_end

    friend class MBubbleItemView;
    friend class Ut_MBubbleItem;
    Q_DISABLE_COPY(MBubbleItem)
    Q_DECLARE_PRIVATE(MBubbleItem)
};

#endif
