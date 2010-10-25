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

#ifndef MANIMATION_H
#define MANIMATION_H

#define M_ANIMATION(STYLE) \
    protected: \
    inline virtual const char* styleType() const { return #STYLE; } \
    virtual MAnimationStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MAnimation::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MAnimation::style()); }

#include <QAbstractAnimation>

#include <mexport.h>
#include <manimationstyle.h>


class MAnimationPrivate;

/*!
 * \class MAnimation
 * \brief MAnimation provides an base class for animations.
 *
 * The class defines the functions for the functionality shared by all M
 * animations.
 *
 * By inheriting this class, you can create custom, CSS-styled, animations that
 * plug into the rest of the animation framework.
 *
 * The progress of an animation is given by its current time (currentTime()),
 * which is measured in milliseconds from the start of the animation (0) to its
 * end (duration()). The value is updated automatically while the animation is
 * running. It can also be set directly with setCurrentTime().
 *
 * See the base class documentation, QAbstractAnimation, for more details on
 * running, pausing and stopping animations.
 *
 * Classes inheriting from this will need to implement the duration() and
 * updateCurrentTime() virtual functions.  The duration() function lets you
 * report a duration for the animation (see QAbstractAnimation for more details)
 * and the animation framework calls  updateCurrentTime() when current time has
 * changed. By reimplementing this function, you can track the animation
 * progress. Note that neither the interval between calls nor the number of
 * calls to this function are defined; though, it will normally be 60 updates
 * per second.
 *
 * Classes inheriting MAnimation can be integrated into the rest of the Qt animation
 * framework, and can be used in a QAnimationGroup, combined with a
 * QVariantAnimation etc.
 *
 * Class inheriting MAnimation need to include the M_ANIMATION macro in the
 * class definiton.  For example:
 *
 * \code
 *   /// Fade in the given item over a period of 1 second
 *   class FadeInAnimation : public MAnimation {
 *       Q_OBJECT
 *       M_ANIMATION(FadeInAnimationStyle)
 *
 *     public:
 *       FadeInAnimation(QGraphicsItem *item, QObject *parent = NULL) : MAnimation(parent),mItem(item);
 *       virtual void updateCurrentTime (int currentTime ) {
 *          item->setOpacity(currentTime/1000.0);
 *       }
 *       virtual int duration () const { return 1000; }
 *     protected:
 *       QGraphicsItem *item;
 *   };
 * \endcode
 *
 */
class M_CORE_EXPORT MAnimation : public QAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MAnimation)

public:
    /*! \brief Constructs the MAnimation base class, and passes
     *  \p parent to the QAbstractAnimation's constructor.
     *
     *  \sa QAbstractAnimation, QAnimationGroup, QVariantAnimation, MGroupAnimation
     */
    MAnimation(QObject *parent = NULL);

    /*! \brief Stops the animation if it's running, then destroys the MAnimation.
     *
     *  If the animation is part of a QAnimationGroup, it is automatically removed
     *  before it's destroyed.
     */
    virtual ~MAnimation();

protected:
    /*! \brief Returns a style container object for this animation.
     *
     *  The M_ANIMATION macro, added to inheriting classes, overrides this
     *  method with return the correct type.
     */
    MAnimationStyleContainer &style();
    /*! \brief Returns a style container object for this animation - const version.
     *
     *  The M_ANIMATION macro, added to inheriting classes, overrides this
     *  method to return the correct type.
     */
    const MAnimationStyleContainer &style() const;

    /*! \brief Returns the type of the style this animation uses.
     *
     *  This is the name used in the CSS file.
     *
     *  The M_ANIMATION macro, added to inheriting classes, overrides this
     *  method with correct return type.
     */
    virtual const char *styleType() const;

    /*! \brief Instantiates a style container for this animation.
     *
     *  The M_ANIMATION macro, added to inheriting classes, overrides this
     *  method with correct return type.
     */
    virtual MAnimationStyleContainer *createStyleContainer() const;

protected:
    /*! \internal */
    MAnimationPrivate *const d_ptr;
    MAnimation(MAnimationPrivate *dd, QObject *parent);
    /*! \internal_end */


};

#endif
