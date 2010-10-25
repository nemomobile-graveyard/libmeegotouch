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

#ifndef MSETTINGSLANGUAGEFACTORY_H_
#define MSETTINGSLANGUAGEFACTORY_H_

#include <MExport>
#include <QString>

class MWidget;
class MSettingsLanguageWidget;
class MSettingsLanguageBinary;
class MDataStore;

/*!
 * A factory for translating settings binaries to widgets.
 */
class M_SETTINGS_EXPORT MSettingsLanguageWidgetFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    MSettingsLanguageWidgetFactory() {}
    Q_DISABLE_COPY(MSettingsLanguageWidgetFactory)

public:
    /*!
     * \brief Creates a widget from a MSettingsLanguageBinary representation.
     *
     * \param settingsBinary the settings binary representation.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static MSettingsLanguageWidget *createWidget(const MSettingsLanguageBinary &settingsBinary, MDataStore *dataStore = NULL);
};

#endif /* MSETTINGSLANGUAGEFACTORY_H_ */
