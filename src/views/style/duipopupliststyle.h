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

#ifndef DUIPOPUPLISTSTYLE_H
#define DUIPOPUPLISTSTYLE_H

#include <duidialogstyle.h>

/*!
    \class DuiPopupListStyle
    \brief Style class for standard DuiPopupList.

    DuiPopupListStyle is derived from DuiDialogStyle.

    \ingroup styles
    \sa DuiPopupListStyleContainer DuiDialogStyle \ref styling DuiPopupList DuiPopupListView
*/

class DUI_EXPORT DuiPopupListStyle : public DuiDialogStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiPopupListStyle)

};

/*!
    \class DuiPopupListStyleContainer
    \brief Style mode container class for DuiPopupListStyle.

    \ingroup styles
    \sa DuiPopupListStyle
*/
class DUI_EXPORT DuiPopupListStyleContainer : public DuiDialogStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiPopupListStyle)
};

#endif


