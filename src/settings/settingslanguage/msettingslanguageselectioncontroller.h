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

#ifndef MSETTINGSLANGUAGESELECTIONCONTROLLER_H_
#define MSETTINGSLANGUAGESELECTIONCONTROLLER_H_

#include <QObject>

class MButton;

//! \internal
/*!
 * An interaction controller for selection items in settings language.
 */
class MSettingsLanguageSelectionController : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     * \param parent the parent object for this object.
     */
    MSettingsLanguageSelectionController(QObject *parent = NULL);

    /*!
     * Destructor.
     */
    virtual ~MSettingsLanguageSelectionController();

public slots:
    /*!
     * Reacts to button clicks. Stores the selected buttons values to storage backend.
     * The button should have the following properties set:
     * - key: the name of the value
     * - value: the value that should be set
     * \param button the button that was clicked
     */
    void buttonClicked(MButton *button);

};
//! \internal_end

#endif /* MSETTINGSLANGUAGESELECTIONCONTROLLER_H_ */
