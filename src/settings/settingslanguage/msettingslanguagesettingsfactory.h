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

#ifndef MSETTINGSLANGUAGESETTINGSFACTORY_H_
#define MSETTINGSLANGUAGESETTINGSFACTORY_H_

#include <QObject>

class QGraphicsLinearLayout;
class MWidgetController;
class MDataStore;
class MSettingsLanguageNode;
class MSettingsLanguageSettings;
class MSettingsLanguageWidget;
class QGraphicsLinearLayout;

//! \internal
/*!
 * A factory for translating settings binaries to widgets.
 */
class MSettingsLanguageSettingsFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    MSettingsLanguageSettingsFactory() {}
    Q_DISABLE_COPY(MSettingsLanguageSettingsFactory)

public:
    /*!
     * \brief Creates a widget from a MSettingsLanguageSettings representation.
     *
     * \param settingsItem the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static MWidgetController *createWidget(const MSettingsLanguageSettings &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore = NULL);

private:

    /*!
    *  Creates child widgets to a layout from a MSettingsLanguageNode representation.
    *
    * \param layout the layout where widgets are added.
    * \param node the nodes binary representation.
    * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
    * \param dataStore the data store for the settings.
    */
    static void createChildren(QGraphicsLinearLayout *layout, const MSettingsLanguageNode &node, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore);
};
//! \internal_end

#endif /* MSETTINGSLANGUAGESETTINGSFACTORY_H_ */
