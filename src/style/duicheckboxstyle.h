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

#ifndef DUICHECKBOXSTYLE_H
#define DUICHECKBOXSTYLE_H

#include <QColor>
#include <duibuttonstyle.h>

/*!
    \class DuiCheckboxStyle
    \brief Style class for dui checkboxes.

    \code
        DuiCheckboxStyle {
            checkmark-image: duibutton-checkbox-checkmark 0 0 0 0;
        }
    \endcode

    \ingroup styles
    \sa DuiCheckboxStyleContainer DuiWidgetStyle DuiButtonStyle \ref styling DuiButton DuiCheckboxView
*/
class DUI_EXPORT DuiCheckboxStyle : public DuiButtonStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiCheckboxStyle)

    /*!
        \property DuiButtonSwitchStyle::checkmarkImage
        \brief Pixmap for the checkmark of the checkbox.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *,  checkmarkImage, CheckmarkImage)

};

/*!
    \class DuiCheckboxStyleContainer
    \brief Style mode container class for DuiCheckboxStyle.

    \ingroup styles
    \sa DuiCheckboxStyle
*/
class DUI_EXPORT DuiCheckboxStyleContainer : public DuiButtonStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiCheckboxStyle)
};

#endif

