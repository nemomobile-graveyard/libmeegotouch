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

#ifndef DUISETTINGSLANGUAGETEXTFACTORY_H_
#define DUISETTINGSLANGUAGETEXTFACTORY_H_

#include <QString>

class DuiWidgetController;
class DuiSettingsLanguageText;
class DuiSettingsLanguageWidget;
class DuiDataStore;

/*!
 * A factory for translating settings binaries to widgets.
 */
class DuiSettingsLanguageTextFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    DuiSettingsLanguageTextFactory() {}
    Q_DISABLE_COPY(DuiSettingsLanguageTextFactory)

public:
    /*!
     * \brief Creates a widget from a DuiSettingsLanguageText representation.
     *
     * \param settingsText the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static DuiWidgetController *createWidget(const DuiSettingsLanguageText &settingsText, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore = NULL);
};

#endif /* DUISETTINGSLANGUAGETEXTFACTORY_H_ */
