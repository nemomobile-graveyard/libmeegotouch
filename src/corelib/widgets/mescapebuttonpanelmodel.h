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

#ifndef MESCAPEBUTTONPANELMODEL_H
#define MESCAPEBUTTONPANELMODEL_H

#include <mscenewindowmodel.h>

class M_CORE_EXPORT MEscapeButtonPanelModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MEscapeButtonPanelModel)

public:
    /*!
     * \brief Provides display modes for the escape button.
     */
    enum EscapeMode {
        BackMode,      /*!< The button is a back button, i.e. connects to
                            the MApplicationPage::backButtonClicked() signal.*/
        CloseMode      //!< The button closes the application window
    };

private:
    M_MODEL_PROPERTY(MEscapeButtonPanelModel::EscapeMode, mode, Mode, true, MEscapeButtonPanelModel::CloseMode)
};

#endif // MESCAPEBUTTONPANELMODEL_H

