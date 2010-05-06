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

#ifndef MSETTINGSLANGUAGETEXTFACTORY_H_
#define MSETTINGSLANGUAGETEXTFACTORY_H_

#include <QString>

class MWidgetController;
class MSettingsLanguageText;
class MSettingsLanguageWidget;
class MDataStore;

//! \internal
/*!
 * A factory for translating settings binaries to widgets.
 */
class MSettingsLanguageTextFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    MSettingsLanguageTextFactory() {}
    Q_DISABLE_COPY(MSettingsLanguageTextFactory)

public:
    /*!
     * \brief Creates a widget from a MSettingsLanguageText representation.
     *
     * \param settingsText the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static MWidgetController *createWidget(const MSettingsLanguageText &settingsText, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore = NULL);
};
//! \internal_end

#endif /* MSETTINGSLANGUAGETEXTFACTORY_H_ */
