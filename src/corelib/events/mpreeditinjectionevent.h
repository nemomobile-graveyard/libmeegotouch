/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MPREEDITINJECTIONEVENT_H
#define MPREEDITINJECTIONEVENT_H

#include "mexport.h"

#include <QEvent>

class MPreeditInjectionEventPrivate;

/*!
 * \brief The MPreeditInjectionEvent class provides the information about preedit text.
 *
 * MPreeditInjectionEvent should be sent from text edit to input context and offer the word as preedit.
 */
class M_CORE_EXPORT MPreeditInjectionEvent : public QEvent
{
public:
    /*!
     * \brief Default constructor.
     * \param preedit The word offered as preedit.
     */
    MPreeditInjectionEvent(const QString &preedit);

    /*!
     * \brief Constructor.
     * \param preedit The word offered as preedit.
     * \param eventCursorPosition The expected cursor position inside preedit. The valid value is from
     * 0 (at the beginning of the preedit) to the length of preedit (at the end of preedit).
     */
    MPreeditInjectionEvent(const QString &preedit, int eventCursorPosition);

    /*!
     *\brief Destructor
     */
    virtual ~MPreeditInjectionEvent();

    /*!
     * \brief Returns the offered preedit text.
     */
    QString preedit() const;

    /*!
     * \brief Returns the expected cursor position inside preedit.
     *
     * Note the eventCursorPosition is not the real cursor position, while it is the cursor position inside
     * preedit which this event expects. The input context will decide whether and where to display cursor.
     * The valid value is from 0 (at the beginning of the preedit) to the length of preedit (at the end
     * of preedit). Other values indicate the cursor position unknown.
     */
    int eventCursorPosition() const;

    /*!
     * \brief sets replacement information that should be used for QInputMethodEvent.
     */
    void setReplacement(int replacementStart, int replacementLength);

    /*!
     * \brief replacement start info as in QInputMethodEvent
     */
    int replacementStart() const;

    /*!
      \brief replacement length info as in QInputMethodEvent
    */
    int replacementLength() const;

    static QEvent::Type eventNumber();

protected:
    MPreeditInjectionEventPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(MPreeditInjectionEvent)
    Q_DISABLE_COPY(MPreeditInjectionEvent)
};

#endif
