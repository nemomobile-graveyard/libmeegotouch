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

#ifndef MSETTINGSLANGUAGETEXTCONTROLLER_H_
#define MSETTINGSLANGUAGETEXTCONTROLLER_H_

#include <QObject>

class MTextEdit;

//! \internal
/*!
 * An interaction controller for text items in settings language.
 */
class MSettingsLanguageTextController : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     * \param parent the parent object for this object.
     */
    MSettingsLanguageTextController(QObject *parent = NULL);

    /*!
     * Destructor.
     */
    virtual ~MSettingsLanguageTextController();

public slots:
    /*!
     * Reacts to text changes. Stores the text in the sender MTextEdit
     * widget to the storage backend.
     *
     * The widget should have the following properties set:
     * - key: the name of the value
     *
     * \param reason the reason why the focus was lost
     */
    void textEditLostFocus(Qt::FocusReason reason);
};
//! \internal_end

#endif /* MSETTINGSLANGUAGETEXTCONTROLLER_H_ */
