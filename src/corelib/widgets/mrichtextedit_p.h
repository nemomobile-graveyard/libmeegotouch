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

#ifndef MRICHTEXTEDIT_P_H
#define MRICHTEXTEDIT_P_H

#include <QMimeData>
#include "mtextedit_p.h"
#include "mrichtextedit.h"

class MRichTextEditPrivate : public MTextEditPrivate
{
    Q_DECLARE_PUBLIC(MRichTextEdit)

public:
    /*!
     * \brief constructor
     */
    MRichTextEditPrivate();
    /*!
     * \brief destructor
     */
    virtual ~MRichTextEditPrivate();
    /*!
     * \brief Return the mime information about the selection
     * The mime will contain both text and html formats
     * \return MimeData - Caller has to release memory when done
     */
    QMimeData *createMimeDataFromSelection();

    /*!
     * \brief Paste the mimedata to the selection.
     * Paste richtext if available, otherwise paste plain text.
     * \param source - Input mime data
     */
    bool insertFromMimeData(const QMimeData *source);

    /*!
     * \brief copy the text cursor's selection to the clipbaord.
     * \return True if operation compeled successfully.
     */
    virtual bool copy();
};

#endif
