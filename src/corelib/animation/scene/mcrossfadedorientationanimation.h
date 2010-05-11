// Make doxygen skip this internal class
//! \internal
#ifndef MCROSSFADEDORIENTATIONANIMATION_H
#define MCROSSFADEDORIENTATIONANIMATION_H

#include "morientationanimation.h"
#include <mcrossfadedorientationanimationstyle.h>

class MCrossFadedOrientationAnimationPrivate;

class MCrossFadedOrientationAnimation : public MOrientationAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MCrossFadedOrientationAnimation)
    M_ANIMATION_GROUP(MCrossFadedOrientationAnimationStyle)

public:
    /*!
     * \brief Constructor
     *
     * \param visibleSceneRect The rectangle of the scene that is rendered by MWindow.
     *                         Equals to the bounding rectangle of the root element in
     *                         scene coordinates.
     */
    MCrossFadedOrientationAnimation(const QRectF &visibleSceneRect, QObject *parent = 0);

    /*!
     * \brief Destructor
     */
    virtual ~MCrossFadedOrientationAnimation();

    virtual void addSceneWindow(MSceneWindow *window);
    virtual void removeSceneWindow(MSceneWindow *window);
    virtual void setTargetRotationAngle(M::OrientationAngle start, M::OrientationAngle end);

protected:
    //! \reimp
    // from MOrientationAnimation
    virtual void rootElementChanged();
    //! \reimp_end

    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
};

#endif
//! \internal_end
