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

#ifndef DUIAPPLICATIONEXTENSIONRUNNER_H
#define DUIAPPLICATIONEXTENSIONRUNNER_H

#include "duiextensionrunner.h"

class DuiApplicationExtensionInterface;
class DuiApplicationExtensionMetaData;

/**
 * \ingroup applicationextensionrunner
 *
 * DuiApplicationExtensionRunner is a class that is used to start and run oop (out of process)
 * application extensions.
 */
class DuiApplicationExtensionRunner : public DuiExtensionRunner
{
public:
    /*!
     * DuiApplicationExtensionRunner constructor
     */
    DuiApplicationExtensionRunner();

    //! DuiApplicationExtensionRunner destructor
    virtual ~DuiApplicationExtensionRunner();

    //! \reimp
    virtual bool init(const QString &serverName, DuiApplicationExtensionMetaData &metaData);
    //! \reimp_end

protected:
    //! \reimp
    virtual void teardown();
    //! \reimp_end

    //! The loaded extension
    DuiApplicationExtensionInterface *extension;

#ifdef UNIT_TEST
    friend class Ut_DuiApplicationExtensionRunner;
#endif
};

#endif // DUIAPPLICATIONEXTENSIONRUNNER_H
