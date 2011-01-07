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

#ifndef MHELPBUTTON_H
#define MHELPBUTTON_H

#include <MButton>
class MHelpButtonPrivate;

/*!
    \class MHelpButton
    \brief MHelpButton is a button offering help for the user with a common look

    \ingroup widgets

    \section MHelpButtonOverview Overview
        MHelpButton offers a help button. If the user presses it, it will open
        the current userguide application at the page specified by pageID().

    \sa setPageID about how to specify the page.
*/
class M_EXPORT MHelpButton: public MButton
{
    Q_OBJECT

public:

    /*!
       \property MMHelpButton::pageID
       \brief See MMHelpButton::pageID
    */
    Q_PROPERTY(QString pageID READ pageID WRITE setPageID)

    /*!
     * \brief Constructs a help button
     *
     * Constructs a help button with a pageID.
     *
     * \param pageID The id of the page to be opened when the button is clicked.
     * \param parent The parent of the widget.
     *
     * \sa setPageID about how to specify the page.
     */
    MHelpButton(const QString& pageID, QGraphicsItem* parent = 0);

    /*!
      \brief Destroys the help button.
     */
    ~MHelpButton();

    /*!
     * @brief Gets the pageID
     *
     * \sa setPageID.
     */
    QString pageID() const;

    /*!
     * @brief Sets the pageID
     *
     * The pageID is the id of the page where userguide will be opened at
     * when the user clicks on the button.
     *
     * The id of a page is its relative path from the locale dependant
     * content directory, for example if the page resides under:
     *
     * \code
     * /usr/share/userguide/content/en/dir/page.cfg
     * \endcode
     *
     * then its id is:
     *
     * \code
     * dir/page.cfg
     * \endcode
     *
     * \sa pageID
     */
    void setPageID(const QString& id);

protected:
    //! \internal
    MHelpButton(MHelpButtonPrivate *dd, MButtonModel *model,
                QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MHelpButton)
    Q_DISABLE_COPY(MHelpButton)
    Q_PRIVATE_SLOT(d_func(), void _q_openHelp())
};


#endif // MHELPBUTTON_H

