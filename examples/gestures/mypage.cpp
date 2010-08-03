#include <MDebug>
#include <MImageWidget>
#include <QImage>
#include <QPinchGesture>
#include <QSwipeGesture>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <MSceneManager>
#include <QTimer>
#include <QGraphicsLinearLayout>

#include "mypage.h"

MyPage::MyPage(QGraphicsItem *parent)
    : MApplicationPage(parent),
    currentImageNumber(0)
{
    setTitle("Gestures example");

    // This enables delivery of low-level touch events to the widget.
    // While we aren't directly interested in these, on a multitouch
    // capable system the pinch gesture is recognized by the touch events.
    setAcceptTouchEvents(true);

    // The grabGesture methods enables delivery of the high-level gesture
    // events (QPinchGesture and QSwipeGesture in our case) to the widget.
    //
    // The reason the gestures are handled by the page instead of the
    // actual image widget, is that we want the entire page area to
    // function as a single pinching/rotating/swiping surface.
    //
    // In general, you should handle gestures on the largest possible
    // area, even if it holds multiple widgets. In case of multiple widgets,
    // you can determine the target of the gesture using the event's hotSpot
    // property.
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);

    // Setting up our animations

    swipeAnimation = new QParallelAnimationGroup(this);

    // Transition the old image out
    oldImagePosAnimation = new QPropertyAnimation();
    oldImagePosAnimation->setPropertyName("pos");
    oldImagePosAnimation->setEasingCurve(QEasingCurve::OutQuad);
    oldImagePosAnimation->setDuration(400);
    swipeAnimation->addAnimation(oldImagePosAnimation);

    // Transition the new image out
    newImagePosAnimation = new QPropertyAnimation();
    newImagePosAnimation->setPropertyName("pos");
    newImagePosAnimation->setEasingCurve(QEasingCurve::OutQuad);
    newImagePosAnimation->setDuration(400);
    swipeAnimation->addAnimation(newImagePosAnimation);

    // Reset the rotation of the current image
    rotationAnimation = new QPropertyAnimation;
    rotationAnimation->setPropertyName("rotation");
    rotationAnimation->setEasingCurve(QEasingCurve::OutQuad);
    rotationAnimation->setDuration(400);
    rotationAnimation->setEndValue(0.0f);
    swipeAnimation->addAnimation(rotationAnimation);

    // Reset the scale of the current image
    scaleAnimation = new QPropertyAnimation;
    scaleAnimation->setPropertyName("scale");
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuad);
    scaleAnimation->setDuration(400);
    scaleAnimation->setEndValue(1.0f);
    swipeAnimation->addAnimation(scaleAnimation);

    connect(swipeAnimation, SIGNAL(finished()), this, SLOT(hideImagesExceptCurrent()));
}

// The create content is called just before the page becomes visible
void MyPage::createContent()
{
    // Loading the images and make widgets
    for (int i = 0; i < ImageCount; ++i) {
        // The images are in the resource collection (images.qrc)
        QImage picture(QString(":image%1.jpg").arg(i+1));
        images[i] = new MImageWidget(&picture);
        // Initially position the widgets outside the page, as we want them
        // to transition in one by one.
        //images[i]->setPos(this->pos() + QPointF(this->size().width(), 0));
    }

    // Center the image on the page
    layout = new QGraphicsLinearLayout(Qt::Vertical, this->centralWidget());
    layout->addItem(images[currentImageNumber]);
}

void MyPage::hideImagesExceptCurrent()
{
    for (int i = 0; i < ImageCount; ++i) {
        if (i != currentImageNumber)
            images[i]->hide();
    }
}

// The gesture handlers:

// Pinch handler
void MyPage::pinchGestureEvent(QGestureEvent *event,
                               QPinchGesture *gesture)
{
    mDebug("pinchGestureEvent")
            << "State:" << gesture->state()
            << "Angle:" << gesture->rotationAngle()
            << "Scale" << gesture->scaleFactor();

    // By accepting the event we stop it from propagating to the
    // next widget.
    event->accept(Qt::PinchGesture);

    // Setting the transformation point to the center of the image.
    // This makes the image rotate around the center when we pinch and rotate,
    // instead of the top-left corner which is the default,
    MImageWidget *currentImage = images[currentImageNumber];
    currentImage->setTransformOriginPoint(currentImage->size().width() / 2,
                                          currentImage->size().height() / 2);

    // Now let's manipulate our image widget according to the data
    // contained in the the pinch gesture.

    // It is important to exclude gesture states other than "updated"
    // from the manipulation of the widget, as for example on
    // Qt::GestureStarted both the angle and scale will be 0.
    if (gesture->state() == Qt::GestureUpdated) {

        // The rotation delta is how much additional rotation we should apply
        // to our image. The absolute rotation value of the gesture is not
        // of interest, applying it to our image would make the widget "jump".
        qreal rotationDelta = gesture->rotationAngle() - gesture->lastRotationAngle();
        qreal newRotation = currentImage->rotation() + rotationDelta;
        currentImage->setRotation(newRotation);

        // Same deal for the scale as above, the current scale of the widget is
        // adjusted while the absolute scale value of the gesture is not important.
        qreal scaleDelta = gesture->scaleFactor() - gesture->lastScaleFactor();
        qreal newScale = currentImage->scale() + scaleDelta;
        // Let's limit the range the image can be scaled: Shrink by 80%, grow by 400%
        if (newScale > 0.2f && newScale < 4.0f)
            currentImage->setScale(newScale);
    }
}

// Swipe handler
void MyPage::swipeGestureEvent(QGestureEvent *event,
                               QSwipeGesture *gesture)
{
    // By accepting the event we stop it from propagating to the
    // next widget.
    event->accept();

    // Swiping towards left, brings in a new picure from the right
    if (gesture->horizontalDirection() == QSwipeGesture::Left) {
        mDebug("swipeGestureEvent") << "Left";
        showNextImage();
    } else if (gesture->horizontalDirection() == QSwipeGesture::Right) {
        mDebug("swipeGestureEvent") << "Right";
        showPreviousImage();
    }
}

// End of the gesture handlers

// Key handler
void MyPage::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        showPreviousImage();
    if (event->key() == Qt::Key_Right)
        showNextImage();
}

// Transition in a new image from the left
void MyPage::showPreviousImage()
{
    if (swipeAnimation->state() == QAbstractAnimation::Running)
        return;

    MImageWidget *oldImage = images[currentImageNumber];
    currentImageNumber = --currentImageNumber % ImageCount;
    if (currentImageNumber < 0)
        currentImageNumber = ImageCount - 1;
    MImageWidget *newImage = images[currentImageNumber];

    oldImagePosAnimation->setTargetObject(oldImage);
    oldImagePosAnimation->setStartValue(oldImage->pos());
    oldImagePosAnimation->setEndValue(QPointF(this->size().width(), 0)); // Animate out to the right

    newImagePosAnimation->setTargetObject(newImage);
    newImagePosAnimation->setStartValue(-QPointF(this->size().width(), 0)); // Animate in from the left...
    newImagePosAnimation->setEndValue(oldImage->pos()); // ... to the old image's position.

    scaleAnimation->setTargetObject(oldImage);
    rotationAnimation->setTargetObject(oldImage);

    swipeAnimation->start();

    newImage->show();
    layout->removeItem(oldImage);
    layout->addItem(newImage);
}

// Transition in a new image from the right
void MyPage::showNextImage()
{
    if (swipeAnimation->state() == QAbstractAnimation::Running)
        return;

    MImageWidget *oldImage = images[currentImageNumber];
    currentImageNumber = ++currentImageNumber % ImageCount;
    MImageWidget *newImage = images[currentImageNumber];

    oldImagePosAnimation->setTargetObject(oldImage);
    oldImagePosAnimation->setStartValue(oldImage->pos());
    oldImagePosAnimation->setEndValue(-QPointF(this->size().width(), 0)); // Animate out to the left

    newImagePosAnimation->setTargetObject(newImage);
    newImagePosAnimation->setStartValue(QPointF(this->size().width(), 0)); // Animate in from the right...
    newImagePosAnimation->setEndValue(oldImage->pos()); // ... to the old image's position.

    scaleAnimation->setTargetObject(oldImage);
    rotationAnimation->setTargetObject(oldImage);

    swipeAnimation->start();

    newImage->show();
    layout->removeItem(oldImage);
    layout->addItem(newImage);
}
