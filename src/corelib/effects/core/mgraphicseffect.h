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
 

#ifndef MGRAPHICSEFFECT_H
#define MGRAPHICSEFFECT_H

#include <QGraphicsEffect>
#include <MGraphicsEffectStyle>

#define M_GRAPHICS_EFFECT(STYLE) \
    protected: \
    inline virtual const char* styleType() const { return #STYLE; } \
    virtual MGraphicsEffectStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MGraphicsEffect::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MGraphicsEffect::style()); }

class MGraphicsEffectPrivate;

/*!
 \class MGraphicsEffect
 \brief MGraphicsEffect provides styling support for QGraphicsEffect.

 This class extends QGraphicsEffect with a style() function, through which
 a MGraphicsEffectStyleContainer derived class can be obtained containing a set of
 effects properties read from CSS.

 This class is not meant to be used directly, instead it serves as a base class for
 other widget effects. Classes deriving from MGraphicsEffect should implement
 a style derived from MGraphicsEffectStyle that describes the style properties of the
 effects in the group.

 Classes inheriting MGraphicsEffect need to include the M_GRAPHICS_EFFECT
 macro in the class definiton, specifying the classname of the style to be used.

 */

class M_CORE_EXPORT MGraphicsEffect : public QGraphicsEffect
{
public:
    MGraphicsEffect(QObject *parent=0);
    virtual ~MGraphicsEffect();

protected:
    /*! \brief Returns a style container object for this graphics effect.

       The M_GRAPHICS_EFFECT macro, added to inheriting classes, overrides this
       method to return the correct type.
     */
    MGraphicsEffectStyleContainer &style();
    /*! \brief Returns a style container object for this graphics effect - const version.

       The M_GRAPHICS_EFFECT macro, added to inheriting classes, overrides this
       method to return the correct type.
     */
    const MGraphicsEffectStyleContainer &style() const;

    /*! \brief Returns the type of the style this graphics effect uses.

      This is the name used in the CSS file.

      The M_GRAPHICS_EFFECT macro, added to inheriting classes, overrides this
      method to return the correct type.
     */
    virtual const char *styleType() const;

    /*! \brief Instantiates a style container for this graphics effect.

      The M_GRAPHICS_EFFECT macro, added to inheriting classes, overrides this
      method to return the correct type.
     */
    virtual MGraphicsEffectStyleContainer *createStyleContainer() const;

private:
    Q_DECLARE_PRIVATE(MGraphicsEffect)
    MGraphicsEffectPrivate *d_ptr;
};

#endif // MGRAPHICSEFFECT_H
