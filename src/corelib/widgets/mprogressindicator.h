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

#ifndef MPROGRESSINDICATOR_H
#define MPROGRESSINDICATOR_H

#include "mwidgetcontroller.h"
#include <mprogressindicatormodel.h>

class MProgressIndicatorPrivate;

/*!
    \class MProgressIndicator
    \brief Progress indicators are used to visualize graphically the current status of an operation.

    \ingroup widgets

    \tableofcontents
    \section MProgressIndicatorOverview Overview
        Progress indicator can have known or unknown duration.

    \section ProgressIndicatorUseGuidelines Usage guidelines
        \li For operations with known duration use progress indicator with known duration.
        \li For operations with unknown duration use progress indicator with unknown duration.
        \li Application design can decide whether to show progress indicator for the whole view
            (e.g. a web page in web browser) or for single items in the view separately
            (e.g. downloading online photos from Flickr web feed in Gallery).
        \li Application can decide to display label indicating the state of the operation next to indicator such as
            "124 / 345 kt received." or "75%".
        \li If the wait operation applies to the whole view, a spinner should be used to temporarily replace the view's
            application menu icon.

    \section ProgressIndicatorVariants Variants
        \li \link MProgressIndicatorBarView Progress bar \endlink
        \li \link MProgressIndicatorCircularView Spinner \endlink

\section MProgressIndicatorExamples Examples
        Here's how to create progress bar with fixed range. It also shows how to listen the value changed signal:
        \code
            MProgressIndicator* progressbar = new MProgressIndicator(parent, MProgressIndicator::barType);
            progressbar->setRange(0, 9);
            connect(progressbar, SIGNAL(valueChanged(int)), SLOT(valueChangedSlot(int)));
        \endcode

        This is how to create spinner with unknown duration:
        \code
            MProgressIndicator* spinner = new MProgressIndicator(parent, MProgressIndicator::spinnerType);
            spinner->setUnknownDuration(true);
        \endcode

    \section ProgressIndicatorOpenIssues Open issues
        \li How we can utilize device sensors together with progress indicators to create small WOW effects?
        \li Progress indicator visualization can be generated procedurally using the device sensors,
            for example by tilting the device liquid/sand inside the progress indicator moves while the
            indicator still shows the current status of the operation. How to? Depends on the selected type
            of the indicator.
        \li Operation duration + haptic feedback: after the indicator has been shown for long enough
            (current assumption: 8 seconds), a tacticon (=small vibra indicator) should be presented
            when the indicator goes away. This enables the user to turn his/her attention back on the
            device when it becomes usable again.

    \sa MProgressIndicatorModel MProgressIndicatorStyle
*/

class M_CORE_EXPORT MProgressIndicator : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MProgressIndicator)

    /*!
        \property MProgressIndicator::minimum
        \brief This property holds the progress indicator's minimum value.

        When setting this property, the \l maximum is adjusted if necessary to ensure that the range remains
        valid. If the current value falls outside the new range, the progress bar is reset with reset().
    */
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)

    /*!
        \property MProgressIndicator::maximum
        \brief This property holds the progress indicator's maximum value.

        When setting this property, the \l minimum is adjusted if necessary to ensure that the range remains
        valid. If the current value falls outside the new range, the progress bar is reset with reset().
    */
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)

    /*!
        \property MProgressIndicator::value
        \brief This property holds the progress indicator's current value.

        Attempting to change the current value to one outside the minimum-maximum range has no effect on the
        current value.
    */
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

    /*!
        \property MProgressIndicator::unknownDuration
        \brief This property holds whether the progress indicator is type of known duration / unknown duration.
    */
    Q_PROPERTY(bool unknownDuration READ unknownDuration WRITE setUnknownDuration)

public:

    /*!
        \var MProgressIndicator::spinnerType
        \brief View type for spinner.
    */
    static const MTheme::ViewType spinnerType;
    /*!
        \var MProgressIndicator::barType
        \brief View type for progress bar.
    */
    static const MTheme::ViewType barType;

    /*!
        \brief Constructs a progress indicator with a \a parent.
        \param parent Parent widget
        \param viewType Optional Associated view
    */
    explicit MProgressIndicator(QGraphicsItem *parent = 0, const QString &viewType = QString());

    /*!
        \brief Destructor
    */
    virtual ~MProgressIndicator();

    /*!
        \brief Returns the maximum value of the progress indicator.
        \return Maximum value of the progress indicator
    */
    int maximum() const;

    /*!
        \brief Returns the minimum value of the progress indicator.
        \return Minimum value of the progress indicator
    */
    int minimum() const;

    /*!
        \brief Returns the value of the progress indicator.
        \return Value of the progress indicator
    */
    int value() const;

    /*!
        \brief Status of indicating progress with unknown duration.
        \return True, if indicator is running in unknown duration mode
    */
    bool unknownDuration() const;

Q_SIGNALS:

    /*!
        \brief Signal is emitted when the value of the indicator changes.
    */
    void valueChanged(int value);

public Q_SLOTS:

    /*!
        \brief Rewinds the indicator and stops unknown duration indicator.
    */
    void reset();

    /*!
        \brief Set the \a maximum value of the indicator.
    */
    void setMaximum(int maximum);

    /*!
        \brief Set the \a minimum value of the indicator.
    */
    void setMinimum(int minimum);

    /*!
        \brief Sets the progress bar's minimum and maximum values to minimum and maximum respectively.
        If maximum is smaller than minimum, minimum becomes the only legal value.
        If the current value falls outside the new range, the progress bar is reset with reset().
    */
    void setRange(int minimum, int maximum);

    /*!
        \brief Set the \a value of the indicator.
        Attempting to change the current value to one outside the minimum-maximum range has no effect on the current value.
        \param value indicator value
     */
    void setValue(int value);

    /*!
        \brief Sets whether the indicator is working in known duration / unknown duration mode.
        \param run True, to set the progress indicator to unknown duration mode
    */
    void setUnknownDuration(bool unknownDuration);

protected:
    //! \internal
    MProgressIndicator(MProgressIndicatorPrivate *dd, MProgressIndicatorModel *model, QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MProgressIndicator)
    Q_DISABLE_COPY(MProgressIndicator)
    friend class Ut_MProgressIndicator;
};

#endif
