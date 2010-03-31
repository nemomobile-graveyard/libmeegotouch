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

#ifndef DUIABSTRACTWIDGETANIMATION_H
#define DUIABSTRACTWIDGETANIMATION_H

#include <duianimation.h>
#include <duiabstractwidgetanimationstyle.h>

#define DUI_WIDGET_ANIMATION(STYLE,VIEW) \
    DUI_ANIMATION(STYLE) \
    public: \
    inline virtual const char* viewType() const { return #VIEW; } \
    private: \
    inline VIEW* view() { return static_cast<VIEW*>(DuiAbstractWidgetAnimation::view()); } \
    inline const VIEW* view() const { return static_cast<const VIEW*>(DuiAbstractWidgetAnimation::view()); }


class DuiWidgetView;
class DuiAbstractWidgetAnimationPrivate;

/*!
  \class DuiAbstractWidgetAnimation
  \brief DuiAbstractWidgetAnimation class is a base class for all widget animations.

 */
class DUI_EXPORT DuiAbstractWidgetAnimation : public DuiAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiAbstractWidgetAnimation)
    DUI_ANIMATION(DuiAbstractWidgetAnimationStyle)

    friend class DuiWidgetView;
    friend class DuiSceneWindowView;

protected:
    /*!
        \brief Constructs the widget animation.

        This constructor is meant to be used inside the libdui to share the
        private data class pointer.
     */
    DuiAbstractWidgetAnimation(DuiAbstractWidgetAnimationPrivate *dd, QObject *parent);

public:

    /*!
        \brief Constructs the widget animation.
     */
    DuiAbstractWidgetAnimation(QObject *parent = NULL);

    /*!
        \brief Destructs the widget animation.
     */
    virtual ~DuiAbstractWidgetAnimation();

    /*!
        \brief Resets the initial state to the widget.

        Reimplement this method in your animation in so when the animation starts it
        will animate from initial state to target values.
        If this method is not called, then the animation should animate from
        the current values to target values.
     */
    virtual void resetToInitialState();

    //! \reimp
    virtual int duration() const;
    //! \reimp_end

    /*!
        \brief Returns the type of the view which this animation supports.

        This method gets automatically overriden by DUI_WIDGET_ANIMATION macro.
     */
    virtual const char *viewType() const {
        return "DuiWidgetView";
    }

protected:
    DuiWidgetView *view();
    const DuiWidgetView *view() const;

private:
    bool setView(DuiWidgetView *view);
};

#endif
