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

#ifndef DUIESCAPEBUTTONPANELMODEL_H
#define DUIESCAPEBUTTONPANELMODEL_H

#include <duiscenewindowmodel.h>

class DUI_EXPORT DuiEscapeButtonPanelModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiEscapeButtonPanelModel)

public:
    /*!
     * \brief Provides display modes for the escape button.
     */
    enum EscapeMode {
        BackMode,      /*!< The button is a back button, i.e. connects to
                            the DuiApplicationPage::backButtonClicked() signal.*/
        CloseMode      //!< The button closes the application window
    };

private:
    DUI_MODEL_PROPERTY(DuiEscapeButtonPanelModel::EscapeMode, mode, Mode, true, DuiEscapeButtonPanelModel::CloseMode)
};

#endif // DUIESCAPEBUTTONPANELMODEL_H

