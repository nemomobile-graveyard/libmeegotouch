#include <MDebug>
#include <MImageWidget>
#include <MMessageBox>
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

void MyPage::showImageInfo()
{
    MMessageBox *msgBox = new MMessageBox("Image info");

    QString text("Name: %1\nSize: %2");
    QString size("%1x%2");

    MImageWidget *currentImage = images[currentImageNumber];
    int width = currentImage->pixmap()->width() * currentImage->scale();
    int height = currentImage->pixmap()->height() * currentImage->scale();

    msgBox->setText(text.arg(currentImage->imageId()).arg(size.arg(width).arg(height)));
    msgBox->appear(MMessageBox::DestroyWhenDone);
}

// The gesture handlers:

// Tap handler
void MyPage::tapGestureEvent(QGestureEvent *event,
                             QTapGesture *gesture)
{
    mDebug("tapGestureEvent")
            << "State:" << gesture->state()
            << "Position:" << gesture->position();

    event->accept(Qt::TapGesture);
}

// Tap And Hold handler
void MyPage::tapAndHoldGestureEvent(QGestureEvent *event,
                                    QTapAndHoldGesture *gesture)
{
    mDebug("tapAndHoldGestureEvent")
            << "State:" << gesture->state()
            << "Position:" << gesture->position();

    event->accept(Qt::TapAndHoldGesture);

    if (gesture->state() == Qt::GestureFinished)
        showImageInfo();
}

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
        // The scaleFactor() methods provides us the delta value since last gesture event.
        qreal newScale = currentImage->scale() * gesture->scaleFactor();
        // Let's limit the range the image can be scaled: Shrink by 80%, grow by 400%
        if (newScale > 0.2f && newScale < 4.0f)
            currentImage->setScale(newScale);
    }
}

// Swipe handler
void MyPage::swipeGestureEvent(QGestureEvent *event,
                               QSwipeGesture *gesture)
{

    // If the vertical direction of swipe is other than NoDirection
    // then the user did not swipe horizontally enough. We will ignore
    // that event. It will be propagated to the widgets beneath us
    // in case any of them is interested.
    if (gesture->verticalDirection() != QSwipeGesture::NoDirection) {
        event->ignore(gesture);
        return;
    }

    // By accepting the event we stop it from propagating to the
    // next widget.
    event->accept(gesture);

    if (gesture->state() == Qt::GestureStarted) {
        // Swiping towards left, brings in a new picure from the right
        if (gesture->horizontalDirection() == QSwipeGesture::Left) {
            mDebug("swipeGestureEvent") << "Left";
            showNextImage();
        } else if (gesture->horizontalDirection() == QSwipeGesture::Right) {
            mDebug("swipeGestureEvent") << "Right";
            showPreviousImage();
        }
    } else if (gesture->state() == Qt::GestureCanceled) {
        // Revert the transition of images. The user didn't
        // do swipe gesture after all, he could for example change
        // the direction of finger movement.
        if (gesture->horizontalDirection() == QSwipeGesture::Left) {
            mDebug("swipeGestureEvent") << "Cancel left movement";
            showPreviousImage(true);
        } else if (gesture->horizontalDirection() == QSwipeGesture::Right) {
            mDebug("swipeGestureEvent") << "Cancel right movement";
            showNextImage(true);
        }
    }
    // We will also be getting GestureUpdated and GestureFinished states
    // here, but we don't need to react to them here.
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
void MyPage::showPreviousImage(bool stopCurrentAnimation)
{

    if (swipeAnimation->state() == QAbstractAnimation::Running) {
        if (stopCurrentAnimation) {
            // We are in the middle of animation and we need to revert it.
            swipeAnimation->stop();
        } else {
            return;
        }
    }

    MImageWidget *oldImage = images[currentImageNumber];
    currentImageNumber = (currentImageNumber - 1) % ImageCount;
    if (currentImageNumber < 0)
        currentImageNumber = ImageCount - 1;
    MImageWidget *newImage = images[currentImageNumber];

    oldImagePosAnimation->setTargetObject(oldImage);
    oldImagePosAnimation->setStartValue(oldImage->pos());
    oldImagePosAnimation->setEndValue(QPointF(this->size().width(), 0)); // Animate out to the right

    newImagePosAnimation->setTargetObject(newImage);
    newImagePosAnimation->setStartValue(-QPointF(this->size().width(), 0)); // Animate in from the left...
    newImagePosAnimation->setEndValue(QPointF(0,0)); // ... to the old image's position.

    scaleAnimation->setTargetObject(oldImage);
    rotationAnimation->setTargetObject(oldImage);

    swipeAnimation->start();

    newImage->show();
    layout->removeItem(oldImage);
    layout->addItem(newImage);
}

// Transition in a new image from the right
void MyPage::showNextImage(bool stopCurrentAnimation)
{
    if (swipeAnimation->state() == QAbstractAnimation::Running) {
        if (stopCurrentAnimation) {
            // We are in the middle of animation and we need to revert it.
            swipeAnimation->stop();
        } else {
            return;
        }
    }

    MImageWidget *oldImage = images[currentImageNumber];
    currentImageNumber = (currentImageNumber + 1) % ImageCount;
    MImageWidget *newImage = images[currentImageNumber];

    oldImagePosAnimation->setTargetObject(oldImage);
    oldImagePosAnimation->setStartValue(oldImage->pos());
    oldImagePosAnimation->setEndValue(-QPointF(this->size().width(), 0)); // Animate out to the left

    newImagePosAnimation->setTargetObject(newImage);
    newImagePosAnimation->setStartValue(QPointF(this->size().width(), 0)); // Animate in from the right...
    newImagePosAnimation->setEndValue(QPointF(0,0)); // ... to the old image's position.

    scaleAnimation->setTargetObject(oldImage);
    rotationAnimation->setTargetObject(oldImage);

    swipeAnimation->start();

    newImage->show();
    layout->removeItem(oldImage);
    layout->addItem(newImage);
}
