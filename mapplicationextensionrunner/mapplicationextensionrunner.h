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

#ifndef MAPPLICATIONEXTENSIONRUNNER_H
#define MAPPLICATIONEXTENSIONRUNNER_H

#include "mextensionrunner.h"

class MApplicationExtensionInterface;
class MApplicationExtensionMetaData;

/**
 * \ingroup applicationextensionrunner
 *
 * MApplicationExtensionRunner is a class that is used to start and run oop (out of process)
 * application extensions.
 */
class MApplicationExtensionRunner : public MExtensionRunner
{
public:
    /*!
     * MApplicationExtensionRunner constructor
     */
    MApplicationExtensionRunner();

    //! MApplicationExtensionRunner destructor
    virtual ~MApplicationExtensionRunner();

    //! \reimp
    virtual bool init(const QString &serverName, MApplicationExtensionMetaData &metaData);
    //! \reimp_end

protected:
    //! \reimp
    virtual void teardown();
    //! \reimp_end

    //! The loaded extension
    MApplicationExtensionInterface *extension;

#ifdef UNIT_TEST
    friend class Ut_MApplicationExtensionRunner;
#endif
};

#endif // MAPPLICATIONEXTENSIONRUNNER_H
