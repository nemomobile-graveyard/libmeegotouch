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

#ifndef DUIMESSAGEBOX_H
#define DUIMESSAGEBOX_H

#include <duidialog.h>
#include <duimessageboxmodel.h>

class DuiMessageBoxPrivate;

/*!
  \class DuiMessageBox
  \brief DuiMessageBox provides a dialog for informing the user or for asking the user a question.

  A DuiMessageBox is a DuiDialog that displays text in its central area.

  By default a DuiMessageBox has no central widget, but just the message text in its place.
  You can still add a central widget to it though, by calling setCentralWidget(). It will
  then be placed between the title bar and the message text.
 */
class DUI_EXPORT DuiMessageBox : public DuiDialog
{
    Q_OBJECT
    DUI_CONTROLLER(DuiMessageBox)
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    /*!
     * \brief Constructs a  message box with the given \a text and set of standard \a buttons.
     *
     * By default, the title bar will be hidden.
     *
     * \param text Text to be displayed in the dialog's central area
     * \param standardButtons Standard buttons to be put in the button box.
     * \sa DuiDialog::StandardButtons
     */
    DuiMessageBox(const QString &text = QString(), Dui::StandardButtons standardButtons = Dui::OkButton);

    /*!
     * \brief Constructs a message box with the given \a title, \a text and
     *        set of standard \a buttons.
     *
     * \param title Dialog's title, shown in the title bar.
     * \param text Text to be displayed in the dialog's central area
     * \param buttons Standard buttons to be put in the button box.
     * \sa DuiDialog::StandardButtons
     */
    DuiMessageBox(const QString &title, const QString &text,
                  Dui::StandardButtons buttons = Dui::OkButton);


    /*!
     * \brief Returns the dialog's text.
     * \return Text being displayed in the dialog's central area.
     * \sa setText()
     */
    QString text() const;

    /*!
     * \brief Sets the dialog's text.
     *
     * \param text Text to be displayed in the dialog's central area.
     * \sa text()
     */
    void setText(const QString &text);

    /*!
     * \brief Destructor
     */
    virtual ~DuiMessageBox();

private:
    Q_DECLARE_PRIVATE(DuiMessageBox)
    Q_DISABLE_COPY(DuiMessageBox)

    friend class DuiMessageBoxView;
    friend class DuiMessageBoxViewPrivate;
};

#endif

