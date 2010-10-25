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

#ifndef MPARALLELANIMATIONGROUP_H
#define MPARALLELANIMATIONGROUP_H

#define M_ANIMATION_GROUP_INTERNAL(STYLE) \
    protected: \
    inline virtual const char* styleType() const { return #STYLE; } \
    virtual MAnimationStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MParallelAnimationGroup::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MParallelAnimationGroup::style()); }

#define M_ANIMATION_GROUP(STYLE) \
    protected: \
    inline virtual const char* styleType() const { return #STYLE; } \
    virtual MAnimationStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MParallelAnimationGroup::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MParallelAnimationGroup::style()); }

#include <QParallelAnimationGroup>

#include <mexport.h>
#include <manimationstyle.h>


class MParallelAnimationGroupPrivate;

/*!
 \class MParallelAnimationGroup
 \brief MParallelAnimationGroup provides styling support for QParallelAnimationGroup.

 This class extends QParallelAnimationGroup with a style() function, through which
 a MAnimationStyleContainer derived class can be obtained containing a set of
 animation properties read from CSS.

 This class is not meant to be used directly, itstead it serves as a base class for
 other animations. Classes deriving from MParallelAnimationGroup should implement
 a style derived from MAnimationStyle that describes the style properties of the
 animations in the group.

 Classes inheriting MParallelAnimationGroup need to include the M_ANIMATION_GROUP
 macro in the class definiton, specifying the classname of the style to be used.
 For example:

 \code
 // Fade out the given item with the duration and easing defined in the style
 class FadeOutAnimation : public MParallelAnimationGroup {
      Q_OBJECT
      M_ANIMATION_GROUP(FadeOutAnimationStyle)

      public:
      FadeOutAnimation(QGraphicsItem *item, QObject *parent = NULL)
          : MParallelAnimationGroup(parent)
      {
          QPropertyAnimation *opacityAnimation = new QPropertyAnimation;
          opacityAnimation->setPropertyName("opacity");
          opacityAnimation->setEasingCurve(style()->easingCurve());
          opacityAnimation->setDuration(style()->duration());
          opacityAnimation->setEndValue(0.0);
          opacityAnimation->setTargetObject(item);

          addAnimation(opacityAnimation);
      }
 };
 \endcode

 */
class M_CORE_EXPORT MParallelAnimationGroup : public QParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MParallelAnimationGroup)

public:
    /*! \brief Constructs the MParallelAnimationGroup base class, and passes
       \p parent to the QParallelAnimationGroup's constructor.

       \sa QParallelAnimationGroup, QAnimationGroup, QVariantAnimation
     */
    MParallelAnimationGroup(QObject *parent = NULL);

    /*! \brief Stops the animation if it's running, then destroys the MParallelAnimationGroup.

       If the animation is part of a QAnimationGroup, it is automatically removed
       before it's destroyed.
     */
    virtual ~MParallelAnimationGroup();

protected:
    /*! \brief Returns a style container object for this animation.

       The M_ANIMATION_GROUP macro, added to inheriting classes, overrides this
       method to return the correct type.
     */
    MAnimationStyleContainer &style();
    /*! \brief Returns a style container object for this animation - const version.

       The M_ANIMATION_GROUP macro, added to inheriting classes, overrides this
       method to return the correct type.
     */
    const MAnimationStyleContainer &style() const;

    /*! \brief Returns the type of the style this animation uses.

      This is the name used in the CSS file.

      The M_ANIMATION_GROUP macro, added to inheriting classes, overrides this
      method to return the correct type.
     */
    virtual const char *styleType() const;

    /*! \brief Instantiates a style container for this animation.

      The M_ANIMATION macro, added to inheriting classes, overrides this
      method to return the correct type.
     */
    virtual MAnimationStyleContainer *createStyleContainer() const;

protected:
    /*! \internal */
    MParallelAnimationGroupPrivate *const d_ptr;
    MParallelAnimationGroup(MParallelAnimationGroupPrivate *dd, QObject *parent);
    /*! \internal_end */
};

#endif
