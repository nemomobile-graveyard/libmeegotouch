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

#ifndef DUISETTINGSLANGUAGEITEMFACTORY_H_
#define DUISETTINGSLANGUAGEITEMFACTORY_H_

#include <QString>

class DuiWidgetController;
class DuiDataStore;
class DuiSettingsLanguageSetting;
class DuiSettingsLanguageWidget;

/*!
 * A factory for translating settings binaries to widgets.
 */
class DuiSettingsLanguageSettingFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    DuiSettingsLanguageSettingFactory() {}
    Q_DISABLE_COPY(DuiSettingsLanguageSettingFactory)

public:
    /*!
     * \brief Creates a widget from a DuiSettingsLanguageSetting representation.
     *
     * \param settingsItem the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static DuiWidgetController *createWidget(const DuiSettingsLanguageSetting &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore = NULL);

};

#endif /* DUISETTINGSLANGUAGEITEMFACTORY_H_ */
