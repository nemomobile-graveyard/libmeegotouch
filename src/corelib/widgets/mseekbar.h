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

#ifndef MSEEKBAR_H
#define MSEEKBAR_H

#include "mslider.h"
#include <mseekbarmodel.h>

/*!
    \class MSlider
    \brief MSeekBar is a special type of MSlider used for displaying playback status for multimedia content.

    \ingroup widgets

    \section MSeekBar Overview
        Seekbar includes received and non-received parts to visualize e.g. streamed/buffered content.

\section MSeekBarExampleCodes Example codes
        Seekbar load new content when seekbar value goes out of loadedcontent range:
        \code
            //connect outOfLoadedContentRange signal to playerOutOfLoadedContentRange local slot
            QObject::connect(playerSeekBar, SIGNAL(outOfLoadedContentRange()), this, SLOT(playerOutOfLoadedContentRange()));
            void SeekBarTesterPage::playerOutOfLoadedContentRange()
            {
                loadedContentMinimum = playerSeekBar->value();
                loadedContentMaximum = playerSeekBar->value();

                playerSeekBar->setLoadedContentMinimum(loadedContentMinimum);
                playerSeekBar->setLoadedContentMaximum(loadedContentMaximum);
            }
        \endcode

    \sa MSeekBarModel MSliderStyle
*/

class M_CORE_EXPORT MSeekBar : public MSlider
{
    Q_OBJECT
    M_CONTROLLER(MSeekBar)

    /*!
        \property MSlider::loadedContentMinimum
        \brief Minimum value in seekbar loaded content range.

        See MSeekBarModel::loadedContentMin for details.
    */
    Q_PROPERTY(int loadedContentMinimum READ loadedContentMinimum WRITE setLoadedContentMinimum)

    /*!
        \property MSlider::loadedContentMaximum
        \brief Maximum value in seekbar loaded content range.

        See MSeekBarModel::loadedContentMax for details.
    */
    Q_PROPERTY(int loadedContentMaximum READ loadedContentMaximum WRITE setLoadedContentMaximum)

public:
    /*!
        \brief Constructs a sekkbar.
        \param parent Parent m widget
        \param viewType Optional view type
     */
    explicit MSeekBar(QGraphicsItem *parent = 0, MSeekBarModel *model = 0);

    /*!
        \brief Gets minimum value in seekbar loaded content range.
        \return minimum value in seekbar loaded content range
     */
    int loadedContentMinimum() const;
    /*!
        \brief Gets maximum value in seekbar loaded content range.
        \return maximum value in seekbar loaded content range
     */
    int loadedContentMaximum() const;

public Q_SLOTS:
    /*!
        \brief Sets minimum value in seekbar loaded content range.
        \param minimum value in loaded content range
    */
    void setLoadedContentMinimum(int minimum);

    /*!
        \brief Sets maximum value in seekbar loaded content range.
        \param maximum value in loaded content range
    */
    void setLoadedContentMaximum(int maximum);

    /*!
        \brief Sets seekbar loaded content range.
        \param minimum value in loaded content range
        \param maximum value in loaded content range
    */
    void setLoadedContentRange(int minimum, int maximum);

    /*!
        \brief Sets new value for seekbar.
        \param value a value to be set (value is bounded into sekkbar value range)
    */
    virtual void setValue(int value);

Q_SIGNALS:
    /*!
        \brief Emitted when value goes out of loaded content range
    */
    void outOfLoadedContentRange();
};

#endif
