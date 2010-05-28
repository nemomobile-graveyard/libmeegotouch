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

#define M_GRAPHICS_EFFECT(STYLE) \
    protected: \
    inline virtual const char* styleType() const { return #STYLE; } \
    virtual MGraphicsEffectStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MGraphicsEffect::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MGraphicsEffect::style()); }

#include <QGraphicsEffect>
#include "mgraphicseffectstyle.h"

class MGraphicsEffectPrivate;
class QPropertyAnimation;

//! \internal
/*! \class MGraphicsEffect
    \brief GraphicsEffect with styling and animation support.
 */
class MGraphicsEffect : public QGraphicsEffect
{
    Q_OBJECT
    Q_PROPERTY(qreal intensity READ intensity WRITE setIntensity)
public:
    MGraphicsEffect(QObject *parent = 0);
    virtual ~MGraphicsEffect();

    enum EnablingMode {
        NormalEnabling, //!< works exacly like QGraphicsEffect::setEnabled
        CumulativeEnabling //!< when enabled many times increases intensity of the effect
    };

    /*!
      overriden QGraphicsEffect::setEnabled
      */
    void setEnabled(bool enable, EnablingMode mode);

    /*!
      Returns current intensity of effect.
      */
    qreal intensity() const;

    /*!
      Sets intensity of effect.
     */
    void setIntensity(qreal newIntensity);

    /*!
      Sets animation that is used to animate this effect.
      If other animation was already stored in this effect, it will be deleted.
     */
    void setAnimation(QPropertyAnimation* animation);

    /*!
      Returns animation that is used to animate this effect
     */
    QPropertyAnimation* animation() const;

protected:
    virtual qreal baseRatio() const = 0;
    qreal ratio() const;


    MGraphicsEffectStyleContainer &style();
    const MGraphicsEffectStyleContainer &style() const;
    virtual const char *styleType() const;
    virtual MGraphicsEffectStyleContainer *createStyleContainer() const;
private:
    Q_DECLARE_PRIVATE(MGraphicsEffect)
    MGraphicsEffectPrivate *const d_ptr;
};
//! \internal_end

#endif // MGRAPHICSEFFECT_H
