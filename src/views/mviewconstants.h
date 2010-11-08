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

// This is a private header, internal to the MeeGo Touch views library

#ifndef MVIEWCONSTANTS_H
#define MVIEWCONSTANTS_H

// Distance in pixels from the widget bounding box inside which a release
// event would still be accepted and trigger the widget
#define M_RELEASE_MISS_DELTA 30

// Time in milliseconds for how long the pressed style mode (appeareance) of
// a widget is maintained
#define M_PRESS_STYLE_TIMEOUT 250

// Time in milliseconds for how long a button will go to unpressed state when
// it's clicked again before the pressed animation gets finished
#define M_PRESS_BLINK_TIMEOUT 50

#endif
