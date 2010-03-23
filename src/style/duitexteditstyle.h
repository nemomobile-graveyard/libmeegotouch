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

#ifndef DUITEXTEDITSTYLE_H
#define DUITEXTEDITSTYLE_H

#include <QFont>
#include <QColor>
#include <QString>
#include <duiwidgetstyle.h>

class DUI_EXPORT DuiTextEditStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiTextEditStyle)

    DUI_STYLE_ATTRIBUTE(QFont,        font,                     Font)
    DUI_STYLE_ATTRIBUTE(QColor,       textColor,                TextColor)
    DUI_STYLE_ATTRIBUTE(QColor,       selectionTextColor,       SelectionTextColor)
    DUI_STYLE_ATTRIBUTE(QColor,       selectionBackgroundColor, SelectionBackgroundColor)
    DUI_STYLE_ATTRIBUTE(qreal,        selectionThreshold,       SelectionThreshold)
    DUI_STYLE_ATTRIBUTE(bool,         allowViewReposition,      AllowViewReposition)
    DUI_STYLE_ATTRIBUTE(QString,      maskString,               MaskString)
    DUI_STYLE_ATTRIBUTE(QColor,       promptColor,              PromptColor)
    DUI_STYLE_ATTRIBUTE(QString,      pasteFailedIcon,          PasteFailedIcon)
    DUI_STYLE_ATTRIBUTE(int,          pasteFailedDuration,      PasteFailedDuration)
    DUI_STYLE_ATTRIBUTE(int,          maskingDelay,             MaskingDelay)
};

class DUI_EXPORT DuiTextEditStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiTextEditStyle)
};

#endif

