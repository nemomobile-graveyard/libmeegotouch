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

#ifndef MMESSAGEBOX_H
#define MMESSAGEBOX_H

#include <mdialog.h>
#include <mmessageboxmodel.h>

class MMessageBoxPrivate;

/*!
  \class MMessageBox
  \brief MMessageBox provides a dialog for informing the user or for asking the user a question.

  A MMessageBox is a MDialog that displays text in its central area.

  By default a MMessageBox has no central widget, but just the message text in its place.
  You can still add a central widget to it though, by calling setCentralWidget(). It will
  then be placed between the title bar and the message text.
 */
class M_CORE_EXPORT MMessageBox : public MDialog
{
    Q_OBJECT
    M_CONTROLLER(MMessageBox)

    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString iconId READ iconId WRITE setIconId)

public:
    /*!
     * \brief Constructs a  message box with the given \a text and set of standard \a buttons.
     *
     * By default, the title bar will be hidden.
     *
     * \param text Text to be displayed in the message box.
     * \param standardButtons Standard buttons to be put in the button box.
     * \sa M::StandardButtons
     */
    MMessageBox(const QString &text = QString(), M::StandardButtons standardButtons = M::OkButton);

    /*!
     * \brief Constructs a message box with the given \a title, \a text and
     *        set of standard \a buttons.
     *
     * \param title Dialog's title to be displayed by the message.
     * \param text Text to be displayed in the message box.
     * \param buttons Standard buttons to be put in the button box.
     * \sa M::StandardButtons
     */
    MMessageBox(const QString &title, const QString &text, M::StandardButtons buttons = M::OkButton);

    /*!
     * \brief Returns the dialog's text.
     * \return Text being displayed in the message box.
     * \sa setText()
     */
    QString text() const;

    /*!
     * \brief Sets the dialog's text.
     *
     * \param text Text to be displayed in the message box.
     * \sa text()
     */
    void setText(const QString &text);

    /*!
     * \brief Returns the dialog's icon id.
     *
     * \return Icon being displayed in the message box.
     * \sa setIconId(), iconPixmap(), setIconPixmap()
     */
    QString iconId() const;

    /*!
     * \brief Sets the dialog's icon.
     *
     * iconId() has a lower priority than iconPixmap(). If iconPixmap() contains
     * a valid (non-null) Pixmap, the message box icon will be taken from it
     * instead of from iconId().
     *
     * \param iconId Logical id of the icon to be displayed in the message box.
     * \sa iconId(), iconPixmap(), setIconPixmap()
     */
    void setIconId(const QString &iconId);

    /*!
     * \brief Returns the dialog's icon pixmap.
     *
     * By default it returns a null pixmap.
     *
     * \return Pixmap of the icon to be displayed in the message box.
     * \sa setIconPixmap(), iconId(), setIconId()
     */
    QPixmap iconPixmap() const;

    /*!
     * \brief Sets the dialog's icon pixmap.
     *
     * \param iconPixmap Pixmap of the icon to be used in the message box.
     * \sa iconPixmap(), iconId(), setIconId()
     */
    void setIconPixmap(QPixmap &iconPixmap);

    /*!
     * \brief Destructor
     */
    virtual ~MMessageBox();

protected:
    //! \reimp
    virtual void dismissEvent(MDismissEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MMessageBox)
    Q_DISABLE_COPY(MMessageBox)

    friend class MMessageBoxView;
    friend class MMessageBoxViewPrivate;
    friend class Ut_MMessageBox;
};

#endif

