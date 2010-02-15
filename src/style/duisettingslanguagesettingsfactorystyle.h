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

#ifndef DUISETTINGSLANGUAGESETTINGSFACTORYSTYLE_H_
#define DUISETTINGSLANGUAGESETTINGSFACTORYSTYLE_H_

#include <DuiWidgetStyle>

/*!
 * DuiSettingsLanguageSettingsFactoryStyle is the style class for DuiSettingsLanguageSettingFactory.
 */
class DUI_EXPORT DuiSettingsLanguageSettingsFactoryStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiSettingsLanguageSettingsFactoryStyle)

    //! The image ID of the content indicator to be shown next to clickable items
    DUI_STYLE_ATTRIBUTE(QString, contentIndicator, ContentIndicator)

    //! The size of the content indicator
    DUI_STYLE_ATTRIBUTE(QSize, contentIndicatorSize, ContentIndicatorSize)

    // TODO: remove this when there's support from DuiContainer for the styling
    //! The left margin for the content indicator when it is inside a DuiContainer
    DUI_STYLE_ATTRIBUTE(qreal, contentIndicatorContainerMargin, ContentIndicatorContainerMargin)

    //! The top margin of the content indicator when it is not inside a DuiContainer
    DUI_STYLE_ATTRIBUTE(qreal, contentIndicatorTopMargin, ContentIndicatorTopMargin)

    //! The left margin of the content indicator when it is not inside a DuiContainer
    DUI_STYLE_ATTRIBUTE(qreal, contentIndicatorLeftMargin, ContentIndicatorLeftMargin)

    //! The right margin of the content indicator when it is not inside a DuiContainer
    DUI_STYLE_ATTRIBUTE(qreal, contentIndicatorRightMargin, ContentIndicatorRightMargin)

    //! The bottom margin of the content indicator when it is not inside a DuiContainer
    DUI_STYLE_ATTRIBUTE(qreal, contentIndicatorBottomMargin, ContentIndicatorBottomMargin)
};

/*!
 * DuiSettingsLanguageSettingsFactoryStyleContainer is the style container class for DuiSettingsLanguageSettingFactory.
 */
class DUI_EXPORT DuiSettingsLanguageSettingsFactoryStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiSettingsLanguageSettingsFactoryStyle)
};

#endif /* DUISETTINGSLANGUAGESETTINGSFACTORYSTYLE_H_ */
