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

#ifndef DUIPARALLELANIMATIONGROUP_H
#define DUIPARALLELANIMATIONGROUP_H

#define DUI_ANIMATION_GROUP(STYLE) \
    protected: \
    inline virtual const char* styleType() const { return #STYLE; } \
    virtual DuiAnimationStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(DuiParallelAnimationGroup::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(DuiParallelAnimationGroup::style()); }

#include <QParallelAnimationGroup>

#include <duiexport.h>
#include <duianimationstyle.h>


class DuiParallelAnimationGroupPrivate;

/*!
 \class DuiParallelAnimationGroup
 \brief DuiParallelAnimationGroup provides styling support for QParallelAnimationGroup.

 This class extends QParallelAnimationGroup with a style() function, through which
 a DuiAnimationStyleContainer derived class can be obtained containing a set of
 animation properties read from CSS.

 This class is not meant to be used directly, itstead it serves as a base class for
 other animations. Classes deriving from DuiParallelAnimationGroup should implement
 a style derived from DuiAnimationStyle that describes the style properties of the
 animations in the group.

 Classes inheriting DuiParallelAnimationGroup need to include the DUI_ANIMATION_GROUP
 macro in the class definiton, specifying the classname of the style to be used.
 For example:

 \code
 // Fade out the given item with the duration and easing defined in the style
 class FadeOutAnimation : public DuiParallelAnimationGroup {
      Q_OBJECT
      DUI_ANIMATION_GROUP(FadeOutAnimationStyle)

      public:
      FadeOutAnimation(QGraphicsItem *item, QObject *parent = NULL)
          : DuiParallelAnimationGroup(parent)
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
class DuiParallelAnimationGroup : public QParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiParallelAnimationGroup)

public:
    /*! \brief Constructs the DuiParallelAnimationGroup base class, and passes
       \p parent to the QParallelAnimationGroup's constructor.

       \sa QParallelAnimationGroup, QAnimationGroup, QVariantAnimation
     */
    DuiParallelAnimationGroup(QObject *parent = NULL);

    /*! \brief Stops the animation if it's running, then destroys the DuiParallelAnimationGroup.

       If the animation is part of a QAnimationGroup, it is automatically removed
       before it's destroyed.
     */
    virtual ~DuiParallelAnimationGroup();

protected:
    /*! \brief Returns a style container object for this animation.

       The DUI_ANIMATION_GROUP macro, added to inheriting classes, overrides this
       method to return the correct type.
     */
    DuiAnimationStyleContainer &style();
    /*! \brief Returns a style container object for this animation - const version.

       The DUI_ANIMATION_GROUP macro, added to inheriting classes, overrides this
       method to return the correct type.
     */
    const DuiAnimationStyleContainer &style() const;

    /*! \brief Returns the type of the style this animation uses.

      This is the name used in the CSS file.

      The DUI_ANIMATION_GROUP macro, added to inheriting classes, overrides this
      method to return the correct type.
     */
    virtual const char *styleType() const;

    /*! \brief Instantiates a style container for this animation.

      The DUI_ANIMATION macro, added to inheriting classes, overrides this
      method to return the correct type.
     */
    virtual DuiAnimationStyleContainer *createStyleContainer() const;

protected:
    /*! \internal */
    DuiParallelAnimationGroupPrivate *const d_ptr;
    DuiParallelAnimationGroup(DuiParallelAnimationGroupPrivate *dd, QObject *parent);
    /*! \internal_end */
};

#endif
