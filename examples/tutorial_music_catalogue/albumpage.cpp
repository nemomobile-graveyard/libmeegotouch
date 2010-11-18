#include "albumpage.h"

#include <MImageWidget>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QGestureEvent>
#include <QGraphicsLinearLayout>
#include <QPinchGesture>
#include <QPropertyAnimation>

AlbumPage::AlbumPage(const Album *album, QGraphicsItem *parent)
    : MApplicationPage(parent), album(album), albumCover(0)
{
    setTitle(album->title);
    connect(this, SIGNAL(appeared()), SLOT(fadeInAlbumCover()));
}

void AlbumPage::createContent()
{
    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);

    // Build a vertical layout that holds the cover art and the "By: Artist" label.
    QGraphicsLinearLayout *coverAndArtistLayout = new QGraphicsLinearLayout(Qt::Vertical);

    albumCover = new MImageWidget(new QImage(album->coverArtFilename));
    albumCover->setOpacity(0.0); // starts completely transparent
    coverAndArtistLayout->addItem(albumCover);

    QString byArtist = QString("By: %1").arg(album->artist);
    coverAndArtistLayout->addItem(new MLabel(byArtist));

    // Build a vertical layout that will hold the list of songs.
    QGraphicsLinearLayout *songsLayout = new QGraphicsLinearLayout(Qt::Vertical);
    songsLayout->addItem(new MLabel("Songs:"));
    MLabel *songLabel;
    for (int i = 0; i < album->songs.count(); i++) {
        songLabel = new MLabel(album->songs.at(i));
        songsLayout->addItem(songLabel);
    }

    // When in landscape orientation, have the cover and the songs list
    // side-by-side.
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->addItem(coverAndArtistLayout);
    landscapePolicy->addItem(songsLayout);
    layout->setLandscapePolicy(landscapePolicy);

    // When in portrait orientation, have the cover and the songs list
    // on top of each other.
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(coverAndArtistLayout);
    portraitPolicy->addItem(songsLayout);
    layout->setPortraitPolicy(portraitPolicy);
}

void AlbumPage::fadeInAlbumCover()
{
    QPropertyAnimation *fadeInAnimation = new QPropertyAnimation;
    fadeInAnimation->setTargetObject(albumCover);
    fadeInAnimation->setPropertyName("opacity");
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->setDuration(1000.0);
    fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void AlbumPage::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
    static QPointF originalZoomFactor;

    if (gesture->state() == Qt::GestureStarted) {
        albumCover->zoomFactor(&originalZoomFactor.rx(), &originalZoomFactor.ry());

        // Disable panning while we're pinching the image
        setPannable(false);
    } else if (gesture->state() == Qt::GestureFinished ||
            gesture->state() == Qt::GestureCanceled) {
        // Re-enable panning after the pinching gesture has ended.
        setPannable(true);
    }

    albumCover->setZoomFactor(
            originalZoomFactor.x() * gesture->scaleFactor(),
            originalZoomFactor.y() * gesture->scaleFactor());

    // Force a repaint of the album cover.
    albumCover->update();

    event->accept(gesture);
}
