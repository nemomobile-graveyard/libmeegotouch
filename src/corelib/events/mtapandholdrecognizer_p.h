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

#ifndef MGESTURETAPANDHOLDRECOGNIZER_P_H
#define MGESTURETAPANDHOLDRECOGNIZER_P_H

#include "mtapandholdrecognizer.h"
#include "mtapandholdrecognizerstyle.h"

/*!
  Private class used by MTapAndHoldRecognizer objects to
  store variables during gesture recognition.
 */
class MTapAndHoldRecognizerPrivate
{
public:

    Q_DECLARE_PUBLIC(MTapAndHoldRecognizer)
    MTapAndHoldRecognizerPrivate();
    virtual ~MTapAndHoldRecognizerPrivate();

private:

    const MTapAndHoldRecognizerStyle *style;

    MTapAndHoldRecognizer* q_ptr;

#ifdef UNIT_TEST
    friend class Ut_MTapAndHoldRecognizer;
#endif
};

#endif
