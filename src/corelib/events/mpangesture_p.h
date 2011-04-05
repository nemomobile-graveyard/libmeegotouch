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


#ifndef MPANGESTURE_P_H
#define MPANGESTURE_P_H

#include <QPanGesture>

class MPanRecognizer;

/*!
  This class provides Pan gesture state.
  It's objects are delivered to registered handlers when a gesture is initiated.
 */
class MPanGesture : public QPanGesture
{
    Q_OBJECT

public:
    /*!
     Default constructor.
     */
    MPanGesture(QObject *parent = 0);

    /*!
     Default destructor.
     */
    virtual ~MPanGesture();

private:

    /*!
      The start position of the pan gesture in scene coordinates.
     */
    QPointF             startPos;
    Qt::Orientations    panDirection;
    bool                pressed;

    friend class MPanRecognizer;
    friend class MPanRecognizerPrivate;

#ifdef UNIT_TEST
    friend class  Ut_MPanRecognizer;
#endif
};

#endif
