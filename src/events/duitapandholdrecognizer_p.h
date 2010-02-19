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

#ifndef DUIGESTURETAPANDHOLDRECOGNIZER_P_H
#define DUIGESTURETAPANDHOLDRECOGNIZER_P_H

#include "duitapandholdrecognizer.h"

/*!
  Private class used by DuiTapAndHoldRecognizer objects to
  store variables during gesture recognition.
 */
class DuiTapAndHoldRecognizerPrivate
{
public:

    Q_DECLARE_PUBLIC(DuiTapAndHoldRecognizer)
    DuiTapAndHoldRecognizerPrivate();
    virtual ~DuiTapAndHoldRecognizerPrivate();

private:

    int     timeoutValue;
    int     movementThreshold;

    DuiTapAndHoldRecognizer* q_ptr;

#ifdef UNIT_TEST
    friend class Ut_DuiTapAndHoldRecognizer;
#endif
};

#endif
