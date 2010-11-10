#ifndef MYPAGE_H
#define MYPAGE_H

#include <MApplicationPage>

class MImageWidget;
class QParallelAnimationGroup;
class QPropertyAnimation;
class QGraphicsLinearLayout;

static const int ImageCount = 4;

class MyPage : public MApplicationPage
{
    Q_OBJECT

public:
    MyPage(QGraphicsItem *parent = 0);

    virtual void createContent();

public slots:
    void showPreviousImage(bool stopCurrentAnimation = false);
    void showNextImage(bool stopCurrentAnimation = false);
    void hideImagesExceptCurrent();
    void showImageInfo();

protected:
    virtual void tapGestureEvent(QGestureEvent *event,
                                 QTapGesture *gesture);

    virtual void tapAndHoldGestureEvent(QGestureEvent *event,
                                        QTapAndHoldGesture *gesture);

    virtual void pinchGestureEvent(QGestureEvent *event,
                                   QPinchGesture *gesture);

    virtual void swipeGestureEvent(QGestureEvent *event,
                                   QSwipeGesture *gesture);

    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    int currentImageNumber;
    MImageWidget *images[ImageCount];

    QGraphicsLinearLayout *layout;

    QParallelAnimationGroup *swipeAnimation;
    QPropertyAnimation *oldImagePosAnimation;
    QPropertyAnimation *newImagePosAnimation;
    QPropertyAnimation *rotationAnimation;
    QPropertyAnimation *scaleAnimation;
};

#endif // MYPAGE_H
